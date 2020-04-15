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

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    KEYWORD = 258,
    STRING_LIT = 259,
    NUMBER = 260,
    ID = 261,
    ENDF = 262,
    PRINT = 263,
    NL = 264,
    COLON = 265,
    GTE = 266,
    LTE = 267,
    GT = 268,
    LT = 269,
    TAB = 270,
    FOR = 271,
    WHILE = 272,
    IF = 273,
    ELSE = 274,
    IN = 275,
    DEF = 276,
    CLASS = 277,
    RETURN = 278,
    RET = 279,
    SPACE = 280,
    COMMA = 281,
    SEMICOLON = 282,
    ERROR = 283,
    PLUS = 284,
    MINUS = 285,
    MUL = 286,
    DIVIDE = 287,
    LBRACKET = 288,
    RBRACKET = 289,
    EQ = 290
  };
#endif
/* Tokens.  */
#define KEYWORD 258
#define STRING_LIT 259
#define NUMBER 260
#define ID 261
#define ENDF 262
#define PRINT 263
#define NL 264
#define COLON 265
#define GTE 266
#define LTE 267
#define GT 268
#define LT 269
#define TAB 270
#define FOR 271
#define WHILE 272
#define IF 273
#define ELSE 274
#define IN 275
#define DEF 276
#define CLASS 277
#define RETURN 278
#define RET 279
#define SPACE 280
#define COMMA 281
#define SEMICOLON 282
#define ERROR 283
#define PLUS 284
#define MINUS 285
#define MUL 286
#define DIVIDE 287
#define LBRACKET 288
#define RBRACKET 289
#define EQ 290

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
