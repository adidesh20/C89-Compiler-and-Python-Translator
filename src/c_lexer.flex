%option noyywrap

%{
    extern "C" int fileno(FILE *stream);
    #include <iostream>
    #include "c_parser.tab.hpp"
%}

%%
/* ========================================VARIABLE TYPES======================================== */

"int"           {yylval.string = new std::string(yytext); return T_INT;}

/* =====================================CONDITIONAL SHIZZLE====================================== */

"if"            {yylval.string = new std::string(yytext); return T_IF;}
"else"          {yylval.string = new std::string(yytext); return T_ELSE;}
"while"         {yylval.string = new std::string(yytext); return T_WHILE;}

/* ==========================================IDENTIFIER=========================================== */

[a-zA-Z_][a-zA-Z0-9_]*          {yylval.string=new std::string(yytext); return T_VARIABLE;}
[0-9]+([.][0-9]*)? { yylval.number=strtod(yytext, 0); return T_NUMBER; }


/* ==========================================OPERATORS============================================ */

/* Arithmetic */
[*]                     {return T_TIMES;}
[+]                     {return T_PLUS;}
[-]                     {return T_MINUS;}
[/]                     {return T_DIVIDE;}
[%]                     {return T_MOD;}
[++]                    {return T_INCREMENT;}
[--]                    {return T_DECREMENT;}

/* Comparison */
[==]                    {return T_EQUAL_TO;}
[!=]                    {return T_NOT_EQUAL_TO;}
[<]                     {return T_LT;}
[>]                     {return T_GT;}
[<=]                    {return T_LTEQ;}
[>=]                    {return T_GTEQ;}    

/* Logical */
[||]                    {return T_LOGICAL_OR;}
[&&]                    {return T_LOGCIAL_AND;}
[!]                     {return T_LOGICAL_NOT}

/* Bitwise */
[|]                     {return T_BITWISE_OR;}
[&]                     {return T_BITWISE_AND;}
[^]                     {return T_BITWISE_XOR;}
[~]                     {return T_BITWISE_COMP;}
[<<]                    {return T_LEFT_SHIFT;}
[>>]                    {return T_RIGHT_SHIFT;}

/* Assignment */
[=]                     {return T_EQUAL;}
[+=]                    {return T_PLUS_EQUALS;}
[-=]                    {return T_MINUS_EQUALS;}
[*=]                    {return T_TIMES_EQUALS;}
[/=]                    {return T_DIVIDE_EQUALS;}
[%=]                    {return T_MOD_EQUALS;}

/* Brackets */
[(]                     {return T_OPEN_PARENTHESES;}
[)]                     {return T_OPEN_PARENTHESES;}
[\[]                    {return T_OPEN_BRACKETS;}
[\]]                    {return T_CLOSE_BRACKETS;}
[{]                     {return T_OPEN_BRACES;}
[}]                     {return T_CLOSE_BRACES;}

/* Other */
[;]				      { return T_SEMICOLON; }
[:]				      { return T_COLON; }
[,]				      { return T_COMMA; }

%%

void yyerror (char const *s)
{
    fprintf(stderr, "Flex Error: %s\n", s); /* s is the text that wasn't matched */
    exit(1);
}

