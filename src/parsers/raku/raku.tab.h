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

#ifndef YY_RAKU_YY_RAKU_TAB_H_INCLUDED
# define YY_RAKU_YY_RAKU_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef RAKU_YYDEBUG
# if defined YYDEBUG
#if YYDEBUG
#   define RAKU_YYDEBUG 1
#  else
#   define RAKU_YYDEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define RAKU_YYDEBUG 0
# endif /* ! defined YYDEBUG */
#endif  /* ! defined RAKU_YYDEBUG */
#if RAKU_YYDEBUG
extern int raku_yydebug;
#endif
/* "%code requires" blocks.  */
#line 2 "raku.y"

#include "ast.h"
#include "../snobol4/scrip_cc.h"
typedef struct ExprList {
    tree_t **items;
    int      count;
    int      cap;
} ExprList;

#line 67 "raku.tab.h"

/* Token kinds.  */
#ifndef RAKU_YYTOKENTYPE
# define RAKU_YYTOKENTYPE
  enum raku_yytokentype
  {
    RAKU_YYEMPTY = -2,
    RAKU_YYEOF = 0,                /* "end of file"  */
    RAKU_YYerror = 256,            /* error  */
    RAKU_YYUNDEF = 257,            /* "invalid token"  */
    LIT_INT = 258,                 /* LIT_INT  */
    LIT_FLOAT = 259,               /* LIT_FLOAT  */
    LIT_STR = 260,                 /* LIT_STR  */
    LIT_INTERP_STR = 261,          /* LIT_INTERP_STR  */
    LIT_REGEX = 262,               /* LIT_REGEX  */
    LIT_MATCH_GLOBAL = 263,        /* LIT_MATCH_GLOBAL  */
    LIT_SUBST = 264,               /* LIT_SUBST  */
    VAR_SCALAR = 265,              /* VAR_SCALAR  */
    VAR_ARRAY = 266,               /* VAR_ARRAY  */
    VAR_HASH = 267,                /* VAR_HASH  */
    VAR_TWIGIL = 268,              /* VAR_TWIGIL  */
    IDENT = 269,                   /* IDENT  */
    QIDENT = 270,                  /* QIDENT  */
    VAR_ARRAY_TWIGIL = 271,        /* VAR_ARRAY_TWIGIL  */
    VAR_HASH_TWIGIL = 272,         /* VAR_HASH_TWIGIL  */
    CARET = 273,                   /* CARET  */
    DOLLAR_LBRACKET = 274,         /* DOLLAR_LBRACKET  */
    VAR_CAPTURE = 275,             /* VAR_CAPTURE  */
    VAR_FH = 276,                  /* VAR_FH  */
    VAR_NAMED_CAPTURE = 277,       /* VAR_NAMED_CAPTURE  */
    KW_USE = 278,                  /* KW_USE  */
    TESTOP = 279,                  /* TESTOP  */
    KW_MY = 280,                   /* KW_MY  */
    KW_SAY = 281,                  /* KW_SAY  */
    KW_PRINT = 282,                /* KW_PRINT  */
    KW_IF = 283,                   /* KW_IF  */
    KW_ELSE = 284,                 /* KW_ELSE  */
    KW_ELSIF = 285,                /* KW_ELSIF  */
    KW_WHILE = 286,                /* KW_WHILE  */
    KW_FOR = 287,                  /* KW_FOR  */
    KW_SUB = 288,                  /* KW_SUB  */
    KW_GATHER = 289,               /* KW_GATHER  */
    KW_TAKE = 290,                 /* KW_TAKE  */
    KW_RETURN = 291,               /* KW_RETURN  */
    KW_EXIT = 292,                 /* KW_EXIT  */
    KW_CONSTANT = 293,             /* KW_CONSTANT  */
    KW_ENUM = 294,                 /* KW_ENUM  */
    KW_JOIN = 295,                 /* KW_JOIN  */
    KW_GIVEN = 296,                /* KW_GIVEN  */
    KW_WHEN = 297,                 /* KW_WHEN  */
    KW_DEFAULT = 298,              /* KW_DEFAULT  */
    KW_WITH = 299,                 /* KW_WITH  */
    KW_WITHOUT = 300,              /* KW_WITHOUT  */
    KW_EXISTS = 301,               /* KW_EXISTS  */
    KW_DELETE = 302,               /* KW_DELETE  */
    KW_UNLESS = 303,               /* KW_UNLESS  */
    KW_UNTIL = 304,                /* KW_UNTIL  */
    KW_REPEAT = 305,               /* KW_REPEAT  */
    KW_LOOP = 306,                 /* KW_LOOP  */
    KW_LAST = 307,                 /* KW_LAST  */
    KW_NEXT = 308,                 /* KW_NEXT  */
    KW_MAP = 309,                  /* KW_MAP  */
    KW_GREP = 310,                 /* KW_GREP  */
    KW_SORT = 311,                 /* KW_SORT  */
    KW_REVERSE = 312,              /* KW_REVERSE  */
    KW_TRY = 313,                  /* KW_TRY  */
    KW_CATCH = 314,                /* KW_CATCH  */
    KW_DIE = 315,                  /* KW_DIE  */
    KW_CLASS = 316,                /* KW_CLASS  */
    KW_METHOD = 317,               /* KW_METHOD  */
    KW_HAS = 318,                  /* KW_HAS  */
    KW_NEW = 319,                  /* KW_NEW  */
    KW_ROLE = 320,                 /* KW_ROLE  */
    KW_MULTI = 321,                /* KW_MULTI  */
    KW_PROTO = 322,                /* KW_PROTO  */
    OP_NAME = 323,                 /* OP_NAME  */
    OP_REDUCE = 324,               /* OP_REDUCE  */
    ARR_ALL_SLICE = 325,           /* ARR_ALL_SLICE  */
    SLURPY_POS = 326,              /* SLURPY_POS  */
    SLURPY_LOL = 327,              /* SLURPY_LOL  */
    SLURPY_NAMED = 328,            /* SLURPY_NAMED  */
    KW_HANDLES = 329,              /* KW_HANDLES  */
    WORDLIST = 330,                /* WORDLIST  */
    OP_COLON_D = 331,              /* OP_COLON_D  */
    OP_COLON_U = 332,              /* OP_COLON_U  */
    YADA = 333,                    /* YADA  */
    KW_GRAMMAR = 334,              /* KW_GRAMMAR  */
    KW_TOKEN = 335,                /* KW_TOKEN  */
    KW_RULE = 336,                 /* KW_RULE  */
    KW_REGEX = 337,                /* KW_REGEX  */
    KW_MODULE = 338,               /* KW_MODULE  */
    OP_FATARROW = 339,             /* OP_FATARROW  */
    OP_RANGE = 340,                /* OP_RANGE  */
    OP_RANGE_EX = 341,             /* OP_RANGE_EX  */
    OP_ARROW = 342,                /* OP_ARROW  */
    OP_EQ = 343,                   /* OP_EQ  */
    OP_NE = 344,                   /* OP_NE  */
    OP_LE = 345,                   /* OP_LE  */
    OP_GE = 346,                   /* OP_GE  */
    OP_SEQ = 347,                  /* OP_SEQ  */
    OP_SNE = 348,                  /* OP_SNE  */
    OP_SLT = 349,                  /* OP_SLT  */
    OP_SLE = 350,                  /* OP_SLE  */
    OP_SGT = 351,                  /* OP_SGT  */
    OP_SGE = 352,                  /* OP_SGE  */
    OP_AND = 353,                  /* OP_AND  */
    OP_OR = 354,                   /* OP_OR  */
    OP_TERNARY1 = 355,             /* OP_TERNARY1  */
    OP_TERNARY2 = 356,             /* OP_TERNARY2  */
    OP_BIND = 357,                 /* OP_BIND  */
    OP_DOTEQ = 358,                /* OP_DOTEQ  */
    OP_SMATCH = 359,               /* OP_SMATCH  */
    OP_INC = 360,                  /* OP_INC  */
    OP_DEC = 361,                  /* OP_DEC  */
    OP_ADD_EQ = 362,               /* OP_ADD_EQ  */
    OP_SUB_EQ = 363,               /* OP_SUB_EQ  */
    OP_MUL_EQ = 364,               /* OP_MUL_EQ  */
    OP_DIV_EQ = 365,               /* OP_DIV_EQ  */
    OP_CAT_EQ = 366,               /* OP_CAT_EQ  */
    OP_DOR = 367,                  /* OP_DOR  */
    OP_DIV = 368,                  /* OP_DIV  */
    OP_BAND = 369,                 /* OP_BAND  */
    OP_SHL = 370,                  /* OP_SHL  */
    OP_DIVIS = 371,                /* OP_DIVIS  */
    OP_REP_X = 372,                /* OP_REP_X  */
    OP_REP_XX = 373,               /* OP_REP_XX  */
    OP_POW = 374,                  /* OP_POW  */
    UMINUS = 375                   /* UMINUS  */
  };
  typedef enum raku_yytokentype raku_yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined RAKU_YYSTYPE && ! defined RAKU_YYSTYPE_IS_DECLARED
union RAKU_YYSTYPE
{
#line 420 "raku.y"

    long      ival;
    double    dval;
    char     *sval;
    tree_t  *node;
    ExprList *list;

#line 212 "raku.tab.h"

};
typedef union RAKU_YYSTYPE RAKU_YYSTYPE;
# define RAKU_YYSTYPE_IS_TRIVIAL 1
# define RAKU_YYSTYPE_IS_DECLARED 1
#endif


extern RAKU_YYSTYPE raku_yylval;


int raku_yyparse (void);


#endif /* !YY_RAKU_YY_RAKU_TAB_H_INCLUDED  */
