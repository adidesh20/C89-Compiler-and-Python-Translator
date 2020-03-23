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
%token T_LOGICAL_OR T_LOGICAL_AND T_LOGICAL_NOT
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

%type <expr> ROOT  PROGRAM EXTERNAL_DECLARATION GLOBAL_DECLARATION SCOPE SCOPE_STATEMENTS STATEMENT LOCAL_DECLARATION  EXPR TERM UNARY  FACTOR 
%type <expr> FUNCTION_DEC FUNCTION_DEF PARAMETER_LIST PARAMETER
%type <number> T_NUMBER
%type <string> T_VARIABLE  RETURN_TYPE VAR_TYPE T_VOID T_INT T_DOUBLE T_FLOAT 

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
    VAR_TYPE T_VARIABLE T_SEMICOLON                                           {$$ = new GlobalVariable_Definition(*$1,*$2,NULL);}
  | VAR_TYPE T_VARIABLE T_EQUAL EXPR T_SEMICOLON                              {$$ = new GlobalVariable_Definition(*$1,*$2,$4);}
  ;

FUNCTION_DEF:
    RETURN_TYPE T_VARIABLE T_OPEN_PARENTHESES T_CLOSE_PARENTHESES SCOPE         {$$ = new Function_Definition(*$1,*$2,NULL,$5);}
  | RETURN_TYPE T_VARIABLE T_OPEN_PARENTHESES  PARAMETER_LIST T_CLOSE_PARENTHESES SCOPE  {$$ = new Function_Definition(*$1,*$2,$4,$6);}
  ;

FUNCTION_DEC:
  | RETURN_TYPE T_VARIABLE T_OPEN_PARENTHESES T_CLOSE_PARENTHESES T_SEMICOLON        {$$ = new Function_Declaration(*$1,*$2, NULL);}
  | RETURN_TYPE T_VARIABLE T_OPEN_PARENTHESES  PARAMETER_LIST T_CLOSE_PARENTHESES T_SEMICOLON {$$ = new Function_Declaration(*$1,*$2,$4);}
  ;

PARAMETER_LIST:
    PARAMETER T_COMMA PARAMETER_LIST {$$ = new Parameter_list($1,$3);}
  | PARAMETER              {$$ = new Parameter_list($1,NULL); }
  ;

PARAMETER: 
    VAR_TYPE T_VARIABLE {$$= new Parameter(*$1,*$2);}
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
    ;
 


LOCAL_DECLARATION:
    VAR_TYPE T_VARIABLE                        {$$ = new LocalVariable_Definition (*$1,*$2,NULL);}
  | VAR_TYPE T_VARIABLE T_EQUAL EXPR   {$$ = new LocalVariable_Definition (*$1,*$2,$4);}
  ;

RETURN_TYPE:
    T_VOID    {$$ = $1;}
  | T_INT     {$$ = $1;}
  | T_DOUBLE  {$$ = $1;}
  | T_FLOAT   {$$ = $1;}
  ;

VAR_TYPE:
    T_INT     {$$ = $1;}
  | T_DOUBLE  {$$ = $1;}
  | T_FLOAT   {$$ = $1;}
  ;

EXPR: 
   EXPR T_PLUS TERM { $$ = new AddOperator($1, $3); }
  |EXPR T_MINUS TERM { $$ = new SubOperator($1, $3); }
  |TERM           { $$ = $1; };


TERM:   
   TERM T_TIMES UNARY  { $$ = new MulOperator($1, $3); }
  |TERM T_DIVIDE UNARY  { $$ = new DivOperator($1, $3); }
	|UNARY          { $$ = $1; };

UNARY: 
  T_MINUS UNARY  { $$ = new NegOperator($2); }
	|FACTOR        { $$ = $1; };


FACTOR: 
  T_NUMBER     {  $$ = new Number( $1 ); }
  | T_OPEN_PARENTHESES EXPR T_CLOSE_PARENTHESES { $$ = $2; }
  | T_VARIABLE         {$$ = new Variable (*$1); }




%%

const AST_Node *g_root; // Definition of variable (to match declaration earlier)

const AST_Node *parseAST()
{
  g_root=0;
  yyparse();
  return g_root;
}
