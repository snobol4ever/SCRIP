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
    KW_FAIL = 316,                 /* KW_FAIL  */
    KW_CLASS = 317,                /* KW_CLASS  */
    KW_METHOD = 318,               /* KW_METHOD  */
    KW_HAS = 319,                  /* KW_HAS  */
    KW_NEW = 320,                  /* KW_NEW  */
    KW_ROLE = 321,                 /* KW_ROLE  */
    KW_MULTI = 322,                /* KW_MULTI  */
    KW_PROTO = 323,                /* KW_PROTO  */
    OP_NAME = 324,                 /* OP_NAME  */
    OP_REDUCE = 325,               /* OP_REDUCE  */
    ARR_ALL_SLICE = 326,           /* ARR_ALL_SLICE  */
    SLURPY_POS = 327,              /* SLURPY_POS  */
    SLURPY_LOL = 328,              /* SLURPY_LOL  */
    SLURPY_NAMED = 329,            /* SLURPY_NAMED  */
    KW_HANDLES = 330,              /* KW_HANDLES  */
    WORDLIST = 331,                /* WORDLIST  */
    OP_COLON_D = 332,              /* OP_COLON_D  */
    OP_COLON_U = 333,              /* OP_COLON_U  */
    YADA = 334,                    /* YADA  */
    KW_GRAMMAR = 335,              /* KW_GRAMMAR  */
    KW_TOKEN = 336,                /* KW_TOKEN  */
    KW_RULE = 337,                 /* KW_RULE  */
    KW_REGEX = 338,                /* KW_REGEX  */
    KW_MODULE = 339,               /* KW_MODULE  */
    OP_FATARROW = 340,             /* OP_FATARROW  */
    OP_RANGE = 341,                /* OP_RANGE  */
    OP_RANGE_EX = 342,             /* OP_RANGE_EX  */
    OP_ARROW = 343,                /* OP_ARROW  */
    OP_EQ = 344,                   /* OP_EQ  */
    OP_NE = 345,                   /* OP_NE  */
    OP_LE = 346,                   /* OP_LE  */
    OP_GE = 347,                   /* OP_GE  */
    OP_SEQ = 348,                  /* OP_SEQ  */
    OP_SNE = 349,                  /* OP_SNE  */
    OP_SLT = 350,                  /* OP_SLT  */
    OP_SLE = 351,                  /* OP_SLE  */
    OP_SGT = 352,                  /* OP_SGT  */
    OP_SGE = 353,                  /* OP_SGE  */
    OP_AND = 354,                  /* OP_AND  */
    OP_OR = 355,                   /* OP_OR  */
    OP_TERNARY1 = 356,             /* OP_TERNARY1  */
    OP_TERNARY2 = 357,             /* OP_TERNARY2  */
    OP_BIND = 358,                 /* OP_BIND  */
    OP_DOTEQ = 359,                /* OP_DOTEQ  */
    OP_SMATCH = 360,               /* OP_SMATCH  */
    OP_INC = 361,                  /* OP_INC  */
    OP_DEC = 362,                  /* OP_DEC  */
    OP_ADD_EQ = 363,               /* OP_ADD_EQ  */
    OP_SUB_EQ = 364,               /* OP_SUB_EQ  */
    OP_MUL_EQ = 365,               /* OP_MUL_EQ  */
    OP_DIV_EQ = 366,               /* OP_DIV_EQ  */
    OP_CAT_EQ = 367,               /* OP_CAT_EQ  */
    OP_DOR = 368,                  /* OP_DOR  */
    OP_DIV = 369,                  /* OP_DIV  */
    ADV_EXISTS = 370,              /* ADV_EXISTS  */
    ADV_DELETE = 371,              /* ADV_DELETE  */
    OP_BAND = 372,                 /* OP_BAND  */
    OP_SHL = 373,                  /* OP_SHL  */
    OP_DIVIS = 374,                /* OP_DIVIS  */
    OP_REP_X = 375,                /* OP_REP_X  */
    OP_REP_XX = 376,               /* OP_REP_XX  */
    OP_POW = 377,                  /* OP_POW  */
    UMINUS = 378                   /* UMINUS  */
  };
  typedef enum raku_yytokentype raku_yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined RAKU_YYSTYPE && ! defined RAKU_YYSTYPE_IS_DECLARED
union RAKU_YYSTYPE
{
#line 472 "raku.y"

    long      ival;
    double    dval;
    char     *sval;
    tree_t  *node;
    ExprList *list;

#line 215 "raku.tab.h"

};
typedef union RAKU_YYSTYPE RAKU_YYSTYPE;
# define RAKU_YYSTYPE_IS_TRIVIAL 1
# define RAKU_YYSTYPE_IS_DECLARED 1
#endif


extern RAKU_YYSTYPE raku_yylval;


int raku_yyparse (void);


#endif /* !YY_RAKU_YY_RAKU_TAB_H_INCLUDED  */
