/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_SRC_C_PARSER_TAB_HPP_INCLUDED
# define YY_YY_SRC_C_PARSER_TAB_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 1 "src/c_parser.y" /* yacc.c:1909  */

  #include "ast.hpp"


  #include <cassert>

  extern const AST_Node *g_root; // A way of getting the AST out

  //! This is to fix problems when generating C++
  // We are declaring the functions provided by Flex, so
  // that Bison generated code can call them.
  int yylex(void);
  void yyerror(const char *);

#line 59 "src/c_parser.tab.hpp" /* yacc.c:1909  */

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    T_INT = 258,
    T_VOID = 259,
    T_FLOAT = 260,
    T_DOUBLE = 261,
    T_RETURN = 262,
    T_IF = 263,
    T_ELSE = 264,
    T_WHILE = 265,
    T_TIMES = 266,
    T_PLUS = 267,
    T_MINUS = 268,
    T_DIVIDE = 269,
    T_MOD = 270,
    T_INCREMENT = 271,
    T_DECREMENT = 272,
    T_EQUAL_TO = 273,
    T_NOT_EQUAL_TO = 274,
    T_LT = 275,
    T_GT = 276,
    T_LTEQ = 277,
    T_GTEQ = 278,
    T_LOGICAL_OR = 279,
    T_LOGICAL_AND = 280,
    T_LOGICAL_NOT = 281,
    T_TERNARY = 282,
    T_BITWISE_OR = 283,
    T_BITWISE_AND = 284,
    T_BITWISE_XOR = 285,
    T_BITWISE_COMP = 286,
    T_LEFT_SHIFT = 287,
    T_RIGHT_SHIFT = 288,
    T_EQUAL = 289,
    T_PLUS_EQUALS = 290,
    T_MINUS_EQUALS = 291,
    T_TIMES_EQUALS = 292,
    T_DIVIDE_EQUALS = 293,
    T_MOD_EQUALS = 294,
    T_OPEN_PARENTHESES = 295,
    T_CLOSE_PARENTHESES = 296,
    T_OPEN_BRACKETS = 297,
    T_CLOSE_BRACKETS = 298,
    T_OPEN_BRACES = 299,
    T_CLOSE_BRACES = 300,
    T_SEMICOLON = 301,
    T_COLON = 302,
    T_COMMA = 303,
    T_VARNAME = 304,
    T_EXPONENT = 305,
    T_LBRACKET = 306,
    T_RBRACKET = 307,
    T_LOG = 308,
    T_EXP = 309,
    T_SQRT = 310,
    T_NUMBER = 311,
    T_VARIABLE = 312
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 18 "src/c_parser.y" /* yacc.c:1909  */

  const AST_Node *expr;
  double number;
  std::string *string;

#line 135 "src/c_parser.tab.hpp" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_SRC_C_PARSER_TAB_HPP_INCLUDED  */
