%code requires{
    #include "../include/ast.hpp"
    #include <cassert>

    extern NodePtr g_root; // A way of getting the AST out

    int yylex(void);
    void yyerror(const char *);

}

%union{
  const Expression *expr;
  double number;
  std::string *string;
}

%token T_INT
%token T_IF T_ELSE T_WHILE
%token T_TIMES T_PLUS T_MINUS T_DIVIDE T_MOD
%token T_INCREMENT T_DECREMENT
%token T_EQUAL_TO T_NOT_EQUAL_TO T_LT T_GT T_LTEQ T_GTEQ
%token T_LOGICAL_OR T_LOGICAL_AND T_LOGICAL_NOT
%token T_BITWISE_OR T_BITWISE_AND T_BITWISE_XOR T_BITWISE_COMP
%token T_LEFT_SHIFT T_RIGHT_SHIFT
%token T_EQUAL T_PLUS_EQUALS T_MINUS_EQUALS T_TIMES_EQUALS T_DIVIDE_EQUALS T_MOD_EQUALS
%token T_OPEN_PARENTHESES T_OPEN_PARENTHESES
%token T_OPEN_BRACKETS T_CLOSE_BRACKETS
%token T_OPEN_BRACES T_CLOSE_BRACES
%token T_SEMICOLON T_COLON T_COMMA
%token T_VARNAME



%token T_TIMES T_DIVIDE T_PLUS T_MINUS T_EXPONENT
%token T_LBRACKET T_RBRACKET
%token T_LOG T_EXP T_SQRT
%token T_NUMBER T_VARIABLE

%type <expr> EXPR TERM UNARY EXPONENT FACTOR 
%type <number> T_NUMBER
%type <string> T_VARIABLE T_LOG T_EXP T_SQRT FUNCTION_NAME TYPE_SPEC
   
%start ROOT

%%


ROOT: PROGRAM { g_root = $1; };

PROGRAM: 
    EXTERNAL_DEC PROGRAM {$$ = new Program($1,$2);}
  | EXTERNAL_DEC         {$$ = new Program($1, NULL);}
  ;

EX_DECLARATION:
    FUNCTION_DEC_DEF   {$$ = $1;}
  | GLOBAL_DECLARATION {$$ = $1;}
  ;

GLOBAL_DECLARATION:
    TYPE_SPECIFY T_VARIABLE T_SEMICOLON                                                   {$$ = new GlobalVarDefintion(*$1,*$2,NULL);}
  | TYPE_SPECIFY T_VARIABLE T_EQUAL C_EXPRESSION T_SEMICOLON                              {$$ = new GlobalVarDec(*$1,*$2,$4);}
  | TYPE_SPECIFY T_VARIABLE T_COMMA G_ADDITIONAL_DECS T_SEMICOLON                         {$$ = new MultipleDecs (*$1,*$2,$4, true);}
  | TYPE_SPECIFY T_VARIABLE T_SQUARE_LBRACKET T_NUMBER T_SQUARE_RBRACKET T_SEMICOLON      {$$ = new GlobalArrayDec (*$1,*$2,$4);}
  | T_ENUM T_VARIABLE T_CURLY_LBRACKET G_ENUM_LIST T_CURLY_RBRACKET T_SEMICOLON             {$$ = new EnumDeclaration(*$2,$4);}
  
  ;
TYPE_SPEC:
    T_VOID    {$$ = $1;}
  | T_INT     {$$ = $1;}
  | T_DOUBLE  {$$ = $1;}
  | T_FLOAT   {$$ = $1;}
  ;


/*EXPR : EXPR T_PLUS TERM { $$ = new AddOperator($1, $3); }
       | EXPR T_MINUS TERM { $$ = new SubOperator($1, $3); }
       |TERM           { $$ = $1; }


TERM :   TERM T_TIMES UNARY  { $$ = new MulOperator($1, $3); }
       |TERM T_DIVIDE UNARY  { $$ = new DivOperator($1, $3); }
	     |UNARY          { $$ = $1; }
       

UNARY : T_MINUS UNARY  { $$ = new NegOperator($2); }
	     |EXPONENT        { $$ = $1; }


EXPONENT : FACTOR T_EXPONENT UNARY {$$ = new ExpOperator($1, $3);}
       | FACTOR {$$ = $1;}

FACTOR : T_NUMBER     {  $$ = new Number( $1 ); }
       | T_LBRACKET EXPR T_RBRACKET { $$ = $2; }
       | T_VARIABLE         {$$ = new Variable (*$1); }
       | T_LOG T_LBRACKET EXPR T_RBRACKET {$$ = new LogFunction($3); }
       | T_EXP T_LBRACKET EXPR T_RBRACKET {$$ = new ExpFunction($3); }
       | T_SQRT T_LBRACKET EXPR T_RBRACKET {$$ = new SqrtFunction($3); }
*/

%%

const Expression *g_root; // Definition of variable (to match declaration earlier)

const Expression *parseAST()
{
  g_root=0;
  yyparse();
  return g_root;
}
