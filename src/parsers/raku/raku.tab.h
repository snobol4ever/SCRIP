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
#line 3 "raku.y"

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
    LIT_BOOL = 259,                /* LIT_BOOL  */
    LIT_FLOAT = 260,               /* LIT_FLOAT  */
    LIT_STR = 261,                 /* LIT_STR  */
    LIT_INTERP_STR = 262,          /* LIT_INTERP_STR  */
    LIT_REGEX = 263,               /* LIT_REGEX  */
    LIT_MATCH_GLOBAL = 264,        /* LIT_MATCH_GLOBAL  */
    LIT_SUBST = 265,               /* LIT_SUBST  */
    VAR_SCALAR = 266,              /* VAR_SCALAR  */
    VAR_ARRAY = 267,               /* VAR_ARRAY  */
    VAR_HASH = 268,                /* VAR_HASH  */
    VAR_TWIGIL = 269,              /* VAR_TWIGIL  */
    IDENT = 270,                   /* IDENT  */
    VAR_ARRAY_TWIGIL = 271,        /* VAR_ARRAY_TWIGIL  */
    VAR_HASH_TWIGIL = 272,         /* VAR_HASH_TWIGIL  */
    VAR_SCALAR_IDX = 273,          /* VAR_SCALAR_IDX  */
    VAR_SCALAR_HASH = 274,         /* VAR_SCALAR_HASH  */
    VAR_SCALAR_ANGLE = 275,        /* VAR_SCALAR_ANGLE  */
    CARET = 276,                   /* CARET  */
    DOLLAR_LBRACKET = 277,         /* DOLLAR_LBRACKET  */
    VAR_CAPTURE = 278,             /* VAR_CAPTURE  */
    VAR_FH = 279,                  /* VAR_FH  */
    VAR_NAMED_CAPTURE = 280,       /* VAR_NAMED_CAPTURE  */
    KW_USE = 281,                  /* KW_USE  */
    TESTOP = 282,                  /* TESTOP  */
    KW_MY = 283,                   /* KW_MY  */
    KW_SAY = 284,                  /* KW_SAY  */
    KW_PRINT = 285,                /* KW_PRINT  */
    KW_IF = 286,                   /* KW_IF  */
    KW_ELSE = 287,                 /* KW_ELSE  */
    KW_ELSIF = 288,                /* KW_ELSIF  */
    KW_WHILE = 289,                /* KW_WHILE  */
    KW_FOR = 290,                  /* KW_FOR  */
    KW_SUB = 291,                  /* KW_SUB  */
    KW_GATHER = 292,               /* KW_GATHER  */
    KW_TAKE = 293,                 /* KW_TAKE  */
    KW_RETURN = 294,               /* KW_RETURN  */
    KW_EXIT = 295,                 /* KW_EXIT  */
    KW_CONSTANT = 296,             /* KW_CONSTANT  */
    KW_ENUM = 297,                 /* KW_ENUM  */
    KW_JOIN = 298,                 /* KW_JOIN  */
    KW_GIVEN = 299,                /* KW_GIVEN  */
    KW_WHEN = 300,                 /* KW_WHEN  */
    KW_DEFAULT = 301,              /* KW_DEFAULT  */
    KW_WITH = 302,                 /* KW_WITH  */
    KW_WITHOUT = 303,              /* KW_WITHOUT  */
    KW_EXISTS = 304,               /* KW_EXISTS  */
    KW_DELETE = 305,               /* KW_DELETE  */
    KW_UNLESS = 306,               /* KW_UNLESS  */
    KW_UNTIL = 307,                /* KW_UNTIL  */
    KW_REPEAT = 308,               /* KW_REPEAT  */
    KW_LOOP = 309,                 /* KW_LOOP  */
    KW_LAST = 310,                 /* KW_LAST  */
    KW_NEXT = 311,                 /* KW_NEXT  */
    KW_MAP = 312,                  /* KW_MAP  */
    KW_GREP = 313,                 /* KW_GREP  */
    KW_SORT = 314,                 /* KW_SORT  */
    KW_REVERSE = 315,              /* KW_REVERSE  */
    KW_TRY = 316,                  /* KW_TRY  */
    KW_CATCH = 317,                /* KW_CATCH  */
    KW_DIE = 318,                  /* KW_DIE  */
    KW_FAIL = 319,                 /* KW_FAIL  */
    KW_CLASS = 320,                /* KW_CLASS  */
    KW_METHOD = 321,               /* KW_METHOD  */
    KW_HAS = 322,                  /* KW_HAS  */
    KW_NEW = 323,                  /* KW_NEW  */
    KW_ROLE = 324,                 /* KW_ROLE  */
    KW_MULTI = 325,                /* KW_MULTI  */
    KW_PROTO = 326,                /* KW_PROTO  */
    OP_NAME = 327,                 /* OP_NAME  */
    OP_REDUCE = 328,               /* OP_REDUCE  */
    ARR_ALL_SLICE = 329,           /* ARR_ALL_SLICE  */
    SLURPY_POS = 330,              /* SLURPY_POS  */
    SLURPY_LOL = 331,              /* SLURPY_LOL  */
    SLURPY_NAMED = 332,            /* SLURPY_NAMED  */
    KW_HANDLES = 333,              /* KW_HANDLES  */
    WORDLIST = 334,                /* WORDLIST  */
    OP_COLON_D = 335,              /* OP_COLON_D  */
    OP_COLON_U = 336,              /* OP_COLON_U  */
    YADA = 337,                    /* YADA  */
    KW_GRAMMAR = 338,              /* KW_GRAMMAR  */
    KW_TOKEN = 339,                /* KW_TOKEN  */
    KW_RULE = 340,                 /* KW_RULE  */
    KW_REGEX = 341,                /* KW_REGEX  */
    KW_MODULE = 342,               /* KW_MODULE  */
    PHASER = 343,                  /* PHASER  */
    OP_FATARROW = 344,             /* OP_FATARROW  */
    OP_RANGE = 345,                /* OP_RANGE  */
    OP_RANGE_EX = 346,             /* OP_RANGE_EX  */
    OP_ARROW = 347,                /* OP_ARROW  */
    OP_EQ = 348,                   /* OP_EQ  */
    OP_NE = 349,                   /* OP_NE  */
    OP_LE = 350,                   /* OP_LE  */
    OP_GE = 351,                   /* OP_GE  */
    OP_SEQ = 352,                  /* OP_SEQ  */
    OP_SNE = 353,                  /* OP_SNE  */
    OP_SLT = 354,                  /* OP_SLT  */
    OP_SLE = 355,                  /* OP_SLE  */
    OP_SGT = 356,                  /* OP_SGT  */
    OP_SGE = 357,                  /* OP_SGE  */
    OP_CMP3 = 358,                 /* OP_CMP3  */
    OP_CMPG = 359,                 /* OP_CMPG  */
    OP_LEG = 360,                  /* OP_LEG  */
    OP_AND = 361,                  /* OP_AND  */
    OP_OR = 362,                   /* OP_OR  */
    OP_TERNARY1 = 363,             /* OP_TERNARY1  */
    OP_TERNARY2 = 364,             /* OP_TERNARY2  */
    OP_BIND = 365,                 /* OP_BIND  */
    OP_DOTEQ = 366,                /* OP_DOTEQ  */
    OP_SMATCH = 367,               /* OP_SMATCH  */
    OP_INC = 368,                  /* OP_INC  */
    OP_DEC = 369,                  /* OP_DEC  */
    OP_ADD_EQ = 370,               /* OP_ADD_EQ  */
    OP_SUB_EQ = 371,               /* OP_SUB_EQ  */
    OP_MUL_EQ = 372,               /* OP_MUL_EQ  */
    OP_DIV_EQ = 373,               /* OP_DIV_EQ  */
    OP_CAT_EQ = 374,               /* OP_CAT_EQ  */
    OP_DOR = 375,                  /* OP_DOR  */
    OP_DIV = 376,                  /* OP_DIV  */
    ADV_EXISTS = 377,              /* ADV_EXISTS  */
    ADV_DELETE = 378,              /* ADV_DELETE  */
    OP_BAND = 379,                 /* OP_BAND  */
    OP_SHL = 380,                  /* OP_SHL  */
    OP_GCD = 381,                  /* OP_GCD  */
    OP_LCM = 382,                  /* OP_LCM  */
    OP_MODW = 383,                 /* OP_MODW  */
    OP_NBAND = 384,                /* OP_NBAND  */
    OP_UMUL = 385,                 /* OP_UMUL  */
    OP_UDIV = 386,                 /* OP_UDIV  */
    OP_BORT = 387,                 /* OP_BORT  */
    OP_NBOR = 388,                 /* OP_NBOR  */
    OP_QBOR = 389,                 /* OP_QBOR  */
    OP_QBXOR = 390,                /* OP_QBXOR  */
    OP_UMINUS_I = 391,             /* OP_UMINUS_I  */
    OP_COMPOSE = 392,              /* OP_COMPOSE  */
    OP_COMPOSEU = 393,             /* OP_COMPOSEU  */
    OP_SETINT = 394,               /* OP_SETINT  */
    OP_SETMUL = 395,               /* OP_SETMUL  */
    OP_SETUNI = 396,               /* OP_SETUNI  */
    OP_SETSUM = 397,               /* OP_SETSUM  */
    OP_SETDIF = 398,               /* OP_SETDIF  */
    OP_SETSYM = 399,               /* OP_SETSYM  */
    OP_XORJ = 400,                 /* OP_XORJ  */
    OP_RANGE_XL = 401,             /* OP_RANGE_XL  */
    OP_RANGE_XB = 402,             /* OP_RANGE_XB  */
    OP_BUT = 403,                  /* OP_BUT  */
    OP_DOESW = 404,                /* OP_DOESW  */
    OP_COLL = 405,                 /* OP_COLL  */
    OP_UNICMP = 406,               /* OP_UNICMP  */
    OP_IDENT3 = 407,               /* OP_IDENT3  */
    OP_EQV = 408,                  /* OP_EQV  */
    OP_BEFORE = 409,               /* OP_BEFORE  */
    OP_AFTER = 410,                /* OP_AFTER  */
    OP_SETCONT = 411,              /* OP_SETCONT  */
    OP_SETELEM = 412,              /* OP_SETELEM  */
    OP_APPROX = 413,               /* OP_APPROX  */
    OP_SMARTM = 414,               /* OP_SMARTM  */
    OP_NSMARTM = 415,              /* OP_NSMARTM  */
    OP_MINOP = 416,                /* OP_MINOP  */
    OP_MAXOP = 417,                /* OP_MAXOP  */
    OP_XOROP = 418,                /* OP_XOROP  */
    OP_DIVIS = 419,                /* OP_DIVIS  */
    OP_REP_X = 420,                /* OP_REP_X  */
    OP_REP_XX = 421,               /* OP_REP_XX  */
    OP_POW = 422,                  /* OP_POW  */
    UMINUS = 423                   /* UMINUS  */
  };
  typedef enum raku_yytokentype raku_yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined RAKU_YYSTYPE && ! defined RAKU_YYSTYPE_IS_DECLARED
union RAKU_YYSTYPE
{
#line 659 "raku.y"

    long      ival;
    double    dval;
    char     *sval;
    tree_t  *node;
    ExprList *list;

#line 260 "raku.tab.h"

};
typedef union RAKU_YYSTYPE RAKU_YYSTYPE;
# define RAKU_YYSTYPE_IS_TRIVIAL 1
# define RAKU_YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined RAKU_YYLTYPE && ! defined RAKU_YYLTYPE_IS_DECLARED
typedef struct RAKU_YYLTYPE RAKU_YYLTYPE;
struct RAKU_YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define RAKU_YYLTYPE_IS_DECLARED 1
# define RAKU_YYLTYPE_IS_TRIVIAL 1
#endif


extern RAKU_YYSTYPE raku_yylval;
extern RAKU_YYLTYPE raku_yylloc;

int raku_yyparse (void);


#endif /* !YY_RAKU_YY_RAKU_TAB_H_INCLUDED  */
