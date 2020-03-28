/* A Bison parser, made by GNU Bison 3.5.3.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

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
#line 1 "src/c_parser.y"

  #include "ast.hpp"


  #include <cassert>

  extern const AST_Node *g_root; // A way of getting the AST out

  //! This is to fix problems when generating C++
  // We are declaring the functions provided by Flex, so
  // that Bison generated code can call them.
  int yylex(void);
  void yyerror(const char *);

#line 63 "src/c_parser.tab.hpp"

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
    T_FOR = 266,
    T_TIMES = 267,
    T_PLUS = 268,
    T_MINUS = 269,
    T_DIVIDE = 270,
    T_MOD = 271,
    T_INCREMENT = 272,
    T_DECREMENT = 273,
    T_EQUAL_TO = 274,
    T_NOT_EQUAL_TO = 275,
    T_LT = 276,
    T_GT = 277,
    T_LTEQ = 278,
    T_GTEQ = 279,
    T_LOGICAL_OR = 280,
    T_LOGICAL_AND = 281,
    T_LOGICAL_NOT = 282,
    T_TERNARY = 283,
    T_BITWISE_OR = 284,
    T_BITWISE_AND = 285,
    T_BITWISE_XOR = 286,
    T_BITWISE_COMP = 287,
    T_LEFT_SHIFT = 288,
    T_RIGHT_SHIFT = 289,
    T_EQUAL = 290,
    T_PLUS_EQUALS = 291,
    T_MINUS_EQUALS = 292,
    T_TIMES_EQUALS = 293,
    T_DIVIDE_EQUALS = 294,
    T_MOD_EQUALS = 295,
    T_OPEN_PARENTHESES = 296,
    T_CLOSE_PARENTHESES = 297,
    T_OPEN_BRACKETS = 298,
    T_CLOSE_BRACKETS = 299,
    T_OPEN_BRACES = 300,
    T_CLOSE_BRACES = 301,
    T_SEMICOLON = 302,
    T_COLON = 303,
    T_COMMA = 304,
    T_VARNAME = 305,
    T_EXPONENT = 306,
    T_LBRACKET = 307,
    T_RBRACKET = 308,
    T_LOG = 309,
    T_EXP = 310,
    T_SQRT = 311,
    T_NUMBER = 312,
    T_VARIABLE = 313
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 18 "src/c_parser.y"

  const AST_Node *expr;
  double number;
  std::string *string;

#line 139 "src/c_parser.tab.hpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_SRC_C_PARSER_TAB_HPP_INCLUDED  */
