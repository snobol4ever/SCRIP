/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
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
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_PASCAL_YY_PASCAL_TAB_H_INCLUDED
# define YY_PASCAL_YY_PASCAL_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef PASCAL_YYDEBUG
# if defined YYDEBUG
#if YYDEBUG
#   define PASCAL_YYDEBUG 1
#  else
#   define PASCAL_YYDEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define PASCAL_YYDEBUG 0
# endif /* ! defined YYDEBUG */
#endif  /* ! defined PASCAL_YYDEBUG */
#if PASCAL_YYDEBUG
extern int pascal_yydebug;
#endif
/* "%code requires" blocks.  */
#line 2 "pascal.y"

#include "ast.h"
#include "../snobol4/scrip_cc.h"
typedef struct PNodeList { tree_t **items; int count; int cap; } PNodeList;

#line 63 "pascal.tab.h"

/* Token kinds.  */
#ifndef PASCAL_YYTOKENTYPE
# define PASCAL_YYTOKENTYPE
  enum pascal_yytokentype
  {
    PASCAL_YYEMPTY = -2,
    PASCAL_YYEOF = 0,              /* "end of file"  */
    PASCAL_YYerror = 256,          /* error  */
    PASCAL_YYUNDEF = 257,          /* "invalid token"  */
    GOTOSY = 258,                  /* GOTOSY  */
    PROGRAMSY = 259,               /* PROGRAMSY  */
    SEMICOLON = 260,               /* SEMICOLON  */
    ARRAYSY = 261,                 /* ARRAYSY  */
    LABELSY = 262,                 /* LABELSY  */
    CONSTSY = 263,                 /* CONSTSY  */
    FORWARDSY = 264,               /* FORWARDSY  */
    DOSY = 265,                    /* DOSY  */
    DOWNTOSY = 266,                /* DOWNTOSY  */
    FORSY = 267,                   /* FORSY  */
    REPEATSY = 268,                /* REPEATSY  */
    WHILESY = 269,                 /* WHILESY  */
    TOSY = 270,                    /* TOSY  */
    UNTILSY = 271,                 /* UNTILSY  */
    WITHSY = 272,                  /* WITHSY  */
    CASESY = 273,                  /* CASESY  */
    PROCEDURESY = 274,             /* PROCEDURESY  */
    PACKEDSY = 275,                /* PACKEDSY  */
    OFSY = 276,                    /* OFSY  */
    FILESY = 277,                  /* FILESY  */
    ENDSY = 278,                   /* ENDSY  */
    SETSY = 279,                   /* SETSY  */
    VARSY = 280,                   /* VARSY  */
    THENSY = 281,                  /* THENSY  */
    RECORDSY = 282,                /* RECORDSY  */
    FUNCTIONSY = 283,              /* FUNCTIONSY  */
    BEGINSY = 284,                 /* BEGINSY  */
    BECOMES = 285,                 /* BECOMES  */
    TYPESY = 286,                  /* TYPESY  */
    IFSY = 287,                    /* IFSY  */
    ELSESY = 288,                  /* ELSESY  */
    INOP = 289,                    /* INOP  */
    NOTSY = 290,                   /* NOTSY  */
    IDIV = 291,                    /* IDIV  */
    IMOD = 292,                    /* IMOD  */
    ANDOP = 293,                   /* ANDOP  */
    OROP = 294,                    /* OROP  */
    LTOP = 295,                    /* LTOP  */
    LEOP = 296,                    /* LEOP  */
    GTOP = 297,                    /* GTOP  */
    GEOP = 298,                    /* GEOP  */
    NEOP = 299,                    /* NEOP  */
    EQOP = 300,                    /* EQOP  */
    PLUS = 301,                    /* PLUS  */
    MINUS = 302,                   /* MINUS  */
    MUL = 303,                     /* MUL  */
    RDIV = 304,                    /* RDIV  */
    COMMA = 305,                   /* COMMA  */
    PERIOD = 306,                  /* PERIOD  */
    COLON = 307,                   /* COLON  */
    ARROW = 308,                   /* ARROW  */
    LBRACK = 309,                  /* LBRACK  */
    RBRACK = 310,                  /* RBRACK  */
    LPARENT = 311,                 /* LPARENT  */
    RPARENT = 312,                 /* RPARENT  */
    DOTDOT = 313,                  /* DOTDOT  */
    INTCONST = 314,                /* INTCONST  */
    REALCONST = 315,               /* REALCONST  */
    STRINGCONST = 316,             /* STRINGCONST  */
    IDENT = 317                    /* IDENT  */
  };
  typedef enum pascal_yytokentype pascal_yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined PASCAL_YYSTYPE && ! defined PASCAL_YYSTYPE_IS_DECLARED
union PASCAL_YYSTYPE
{
#line 572 "pascal.y"

    tree_t    *node;
    PNodeList *list;
    char      *str;
    long long  ival;
    double     dval;

#line 150 "pascal.tab.h"

};
typedef union PASCAL_YYSTYPE PASCAL_YYSTYPE;
# define PASCAL_YYSTYPE_IS_TRIVIAL 1
# define PASCAL_YYSTYPE_IS_DECLARED 1
#endif


extern PASCAL_YYSTYPE pascal_yylval;


int pascal_yyparse (void);


#endif /* !YY_PASCAL_YY_PASCAL_TAB_H_INCLUDED  */
