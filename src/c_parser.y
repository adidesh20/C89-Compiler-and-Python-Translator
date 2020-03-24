%code requires{
  #include "ast.hpp"


  #include <cassert>

  extern const AST_Node *g_root; // A way of getting the AST out

  //! This is to fix problems when generating C++
  // We are declaring the functions provided by Flex, so
  // that Bison generated code can call them.
  int yylex(void);
  void yyerror(const char *);
}

// Represents the value associated with any kind of
// AST node.
%union{
  const AST_Node *expr;
  double number;
  std::string *string;
}

%token T_INT T_VOID T_FLOAT T_DOUBLE
%token T_IF T_ELSE T_WHILE
%token T_TIMES T_PLUS T_MINUS T_DIVIDE T_MOD
%token T_INCREMENT T_DECREMENT
%token T_EQUAL_TO T_NOT_EQUAL_TO T_LT T_GT T_LTEQ T_GTEQ
%token T_LOGICAL_OR T_LOGICAL_AND T_LOGICAL_NOT T_TERNARY
%token T_BITWISE_OR T_BITWISE_AND T_BITWISE_XOR T_BITWISE_COMP
%token T_LEFT_SHIFT T_RIGHT_SHIFT
%token T_EQUAL T_PLUS_EQUALS T_MINUS_EQUALS T_TIMES_EQUALS T_DIVIDE_EQUALS T_MOD_EQUALS
%token T_OPEN_PARENTHESES T_CLOSE_PARENTHESES
%token T_OPEN_BRACKETS T_CLOSE_BRACKETS
%token T_OPEN_BRACES T_CLOSE_BRACES
%token T_SEMICOLON T_COLON T_COMMA
%token T_VARNAME

%token T_TIMES T_DIVIDE T_PLUS T_MINUS T_EXPONENT
%token T_LBRACKET T_RBRACKET
%token T_LOG T_EXP T_SQRT
%token T_NUMBER T_VARIABLE

%type <expr> ROOT  PROGRAM EXTERNAL_DECLARATION GLOBAL_DECLARATION GLOBAL_VAR_DEF_LIST GLOBAL_VAR_DEF SCOPE SCOPE_STATEMENTS STATEMENT LOCAL_DECLARATION  EXPR TERM UNARY  FACTOR 
%type <expr> FUNCTION_DEC FUNCTION_DEF PARAMETER_LIST PARAMETER  LOCAL_VAR_DEF LOCAL_VAR_DEF_LIST ASSIGNMENT STAT_SCOPE TERNARY LOGICAL_AND LOGICAL_OR COMPARE_EQUAL COMPARE_DIFFERENT ADD_SUBTRACT
%type <number> T_NUMBER
%type <string> T_VARIABLE  TYPE  T_VOID T_INT T_DOUBLE T_FLOAT 

%start ROOT

%%



ROOT: PROGRAM {g_root = $1;};

PROGRAM:
    EXTERNAL_DECLARATION PROGRAM {$$ = new Program($1,$2);}
  | EXTERNAL_DECLARATION         {$$ = new Program($1, NULL);}
  

 
  ;

EXTERNAL_DECLARATION:
    FUNCTION_DEF   {$$ = $1;}
  | FUNCTION_DEC   {$$ = $1;}
  | GLOBAL_DECLARATION {$$ = $1;}
  ;


GLOBAL_DECLARATION:
    TYPE GLOBAL_VAR_DEF_LIST T_SEMICOLON           {$$=new GloTypeset (*$1,$2);}                                 

  ;

GLOBAL_VAR_DEF_LIST:      
  GLOBAL_VAR_DEF T_COMMA GLOBAL_VAR_DEF_LIST      {$$ = new GloVarList ($1,$3);}
  |GLOBAL_VAR_DEF                          {$$ = new GloVarList ($1,NULL);}
  ;

GLOBAL_VAR_DEF:
  T_VARIABLE                    {$$ = new GlobalVariable_Definition(*$1,NULL);}
  |T_VARIABLE T_EQUAL EXPR        {$$ = new GlobalVariable_Definition(*$1,$3);}
  ;

FUNCTION_DEF:
    TYPE T_VARIABLE T_OPEN_PARENTHESES T_CLOSE_PARENTHESES SCOPE         {$$ = new Function_Definition(*$1,*$2,NULL,$5);}
  | TYPE T_VARIABLE T_OPEN_PARENTHESES  PARAMETER_LIST T_CLOSE_PARENTHESES SCOPE  {$$ = new Function_Definition(*$1,*$2,$4,$6);}
  ;

FUNCTION_DEC:
  | TYPE T_VARIABLE T_OPEN_PARENTHESES T_CLOSE_PARENTHESES T_SEMICOLON        {$$ = new Function_Declaration(*$1,*$2, NULL);}
  | TYPE T_VARIABLE T_OPEN_PARENTHESES  PARAMETER_LIST T_CLOSE_PARENTHESES T_SEMICOLON {$$ = new Function_Declaration(*$1,*$2,$4);}
  ;

PARAMETER_LIST:
    PARAMETER T_COMMA PARAMETER_LIST {$$ = new Parameter_list($1,$3);}
  | PARAMETER              {$$ = new Parameter_list($1,NULL); }
  ;

PARAMETER: 
    TYPE T_VARIABLE {$$= new Parameter(*$1,*$2);}
    ;

SCOPE: 
  T_OPEN_BRACES SCOPE_STATEMENTS T_CLOSE_BRACES  {$$ = new ScopeBody($2);}
  | T_OPEN_BRACES T_CLOSE_BRACES {$$ = new ScopeBody(NULL);} ;

SCOPE_STATEMENTS:
    STATEMENT SCOPE_STATEMENTS {$$ = new ScopeStatements($1,$2);}
  | STATEMENT {$$ = new ScopeStatements($1,NULL);} 
  ;

STATEMENT: 
    LOCAL_DECLARATION  T_SEMICOLON             {$$ = $1;}

  | EXPR T_SEMICOLON                       {$$ = $1;}
  ;

  STAT_SCOPE:
    STATEMENT {$$ = new NoBraces($1);}
  | SCOPE {$$=$1;}
  ;

LOCAL_DECLARATION:
    TYPE LOCAL_VAR_DEF_LIST            {$$=new LocalTypeset (*$1,$2);}                                 
  ;



LOCAL_VAR_DEF_LIST:      
  LOCAL_VAR_DEF T_COMMA LOCAL_VAR_DEF_LIST      {$$ = new LocalVarList ($1,$3);}
  |LOCAL_VAR_DEF                          {$$ = new LocalVarList ($1,NULL);}
  ;

LOCAL_VAR_DEF:
  T_VARIABLE                    {$$ = new LocalVariable_Definition(*$1,NULL);}
  |T_VARIABLE T_EQUAL EXPR        {$$ = new LocalVariable_Definition(*$1,$3);}
  ;


TYPE:
    T_INT     {$$ = $1;}
  | T_DOUBLE  {$$ = $1;}
  | T_FLOAT   {$$ = $1;}
  | T_VOID     {$$ = $1;}
  ;


EXPR:
   LOGICAL_OR {$$ = $1;}
  | ASSIGNMENT {$$ = $1;}
  ;
ASSIGNMENT:
  T_VARIABLE T_EQUAL EXPR                                                           {$$ = new AssOpEqual(*$1,$3);}
  | T_VARIABLE T_PLUS_EQUALS EXPR                                                   {$$ = new AssOpAdd(*$1, $3);}
  | T_VARIABLE T_MINUS_EQUALS EXPR                                                  {$$ = new AssOpSub(*$1, $3);}
  | T_VARIABLE T_TIMES_EQUALS EXPR                                                  {$$ = new AssOpMul(*$1, $3);}
  | T_VARIABLE T_DIVIDE_EQUALS EXPR                                               {$$ = new AssOpDiv(*$1, $3);}
  ;



LOGICAL_OR:
    LOGICAL_AND T_LOGICAL_OR LOGICAL_OR           {$$ = new LogicalOrOperator($1, $3);}
  | LOGICAL_AND                               {$$ = $1;}
  ;

LOGICAL_AND:
    COMPARE_EQUAL T_LOGICAL_AND LOGICAL_AND          {$$ = new LogicalAndOperator($1, $3);}
  | COMPARE_EQUAL                               {$$ = $1;}
  ;

 /* BIT_OR:
    BIT_XOR T_BITWISE_OR BIT_OR           {$$ = new BitwiseOrOperator($1,$3);}
  | BIT_XOR                               {$$ = $1;}
  ;

BIT_XOR:
    BIT_AND T_BITWISE_XOR BIT_XOR         {$$ = new BitwiseXorOperator($1,$3);}
  | BIT_AND                               {$$ = $1;}
  ;

BIT_AND:
    EQUALITY T_BITWISE_AND BIT_AND        {$$ = new BitwiseAndOperator($1,$3);}
  | EQUALITY                              {$$ = $1;}
  ;
 */
COMPARE_EQUAL:
    COMPARE_DIFFERENT T_EQUAL_TO COMPARE_EQUAL { $$ = new EqualToOperator($1, $3);}
  | COMPARE_DIFFERENT T_NOT_EQUAL_TO COMPARE_EQUAL { $$ = new NotEqualToOperator($1, $3);}
  | COMPARE_DIFFERENT { $$ = $1;}
  ;
COMPARE_DIFFERENT:
    ADD_SUBTRACT T_LT COMPARE_DIFFERENT          { $$ = new LessThanOperator($1, $3);}
  | ADD_SUBTRACT T_LTEQ COMPARE_DIFFERENT    { $$ = new LessThanEqualOperator($1, $3);}
  | ADD_SUBTRACT T_GT COMPARE_DIFFERENT       { $$ = new GreaterThanOperator($1, $3);}
  | ADD_SUBTRACT T_GTEQ COMPARE_DIFFERENT { $$ = new GreaterThanEqualOperator($1, $3);}
  | ADD_SUBTRACT                                 {$$ = $1;}
  ;

/*
SHIFT:
    ADD_SUBTRACT T_LSHIFT SHIFT       {$$ = new LeftShiftOperator($1,$3);}
  | ADD_SUBTRACT T_RSHIFT SHIFT       {$$ = new RightShiftOperator($1,$3);}
  | ADD_SUBTRACT                      {$$ = $1;}
  ;
*/


ADD_SUBTRACT: 
   TERM T_PLUS ADD_SUBTRACT { $$ = new AddOperator($1, $3); }
  |TERM T_MINUS ADD_SUBTRACT { $$ = new SubOperator($1, $3); }
  |TERM           { $$ = $1; }
  ;


TERM:   
   UNARY T_TIMES TERM  { $$ = new MulOperator($1, $3); }
  |UNARY T_DIVIDE TERM  { $$ = new DivOperator($1, $3); }
	|UNARY          { $$ = $1; }
  ;




UNARY: 
  T_MINUS UNARY  { $$ = new NegOperator($2); }
	|FACTOR         {$$ = $1; }
  |T_VARIABLE T_INCREMENT          {$$ = new IncrementOperator(*$1, "post");}
  | T_INCREMENT T_VARIABLE        {$$ = new IncrementOperator(*$2, "pre");}
  | T_VARIABLE T_DECREMENT         {$$ = new DecrementOperator(*$1,"post");}
  | T_DECREMENT  T_VARIABLE      {$$ = new DecrementOperator(*$2, "pre");}
  | T_LOGICAL_NOT FACTOR             {$$ = new NotOperator($2);}
  ;
  
  ;


FACTOR: 
  T_NUMBER     {  $$ = new Number( $1 ); }
  | T_OPEN_PARENTHESES EXPR T_CLOSE_PARENTHESES { $$ = $2; }
  | T_VARIABLE         {$$ = new Variable (*$1); }
  
  ;




%%

const AST_Node *g_root; // Definition of variable (to match declaration earlier)

const AST_Node *parseAST()
{
  g_root=0;
  yyparse();
  return g_root;
}
