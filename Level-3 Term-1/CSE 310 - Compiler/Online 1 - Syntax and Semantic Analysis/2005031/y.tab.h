/* A Bison parser, made by GNU Bison 3.5.1.  */

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
    ID = 258,
    LPAREN = 259,
    RPAREN = 260,
    SEMICOLON = 261,
    COMMA = 262,
    LCURL = 263,
    RCURL = 264,
    INT = 265,
    FLOAT = 266,
    VOID = 267,
    LSQUARE = 268,
    CONST_INT = 269,
    RSQUARE = 270,
    FOR = 271,
    IF = 272,
    ELSE = 273,
    WHILE = 274,
    PRINTLN = 275,
    RETURN = 276,
    ASSIGNOP = 277,
    LOGICOP = 278,
    RELOP = 279,
    ADDOP = 280,
    MULOP = 281,
    NOT = 282,
    CONST_FLOAT = 283,
    INCOP = 284,
    DECOP = 285,
    BREAK = 286,
    CASE = 287,
    COLON = 288,
    SWITCH = 289,
    DEFAULT = 290,
    LOWER_THAN_ELSE = 291
  };
#endif
/* Tokens.  */
#define ID 258
#define LPAREN 259
#define RPAREN 260
#define SEMICOLON 261
#define COMMA 262
#define LCURL 263
#define RCURL 264
#define INT 265
#define FLOAT 266
#define VOID 267
#define LSQUARE 268
#define CONST_INT 269
#define RSQUARE 270
#define FOR 271
#define IF 272
#define ELSE 273
#define WHILE 274
#define PRINTLN 275
#define RETURN 276
#define ASSIGNOP 277
#define LOGICOP 278
#define RELOP 279
#define ADDOP 280
#define MULOP 281
#define NOT 282
#define CONST_FLOAT 283
#define INCOP 284
#define DECOP 285
#define BREAK 286
#define CASE 287
#define COLON 288
#define SWITCH 289
#define DEFAULT 290
#define LOWER_THAN_ELSE 291

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 88 "2005031.y"

	SymbolInfo* symbolInfo;

#line 133 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
