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