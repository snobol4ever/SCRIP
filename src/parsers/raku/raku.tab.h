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
    VAR_ARRAY_TWIGIL = 270,        /* VAR_ARRAY_TWIGIL  */
    VAR_HASH_TWIGIL = 271,         /* VAR_HASH_TWIGIL  */
    CARET = 272,                   /* CARET  */
    VAR_CAPTURE = 273,             /* VAR_CAPTURE  */
    VAR_FH = 274,                  /* VAR_FH  */
    VAR_NAMED_CAPTURE = 275,       /* VAR_NAMED_CAPTURE  */
    KW_USE = 276,                  /* KW_USE  */
    TESTOP = 277,                  /* TESTOP  */
    KW_MY = 278,                   /* KW_MY  */
    KW_SAY = 279,                  /* KW_SAY  */
    KW_PRINT = 280,                /* KW_PRINT  */
    KW_IF = 281,                   /* KW_IF  */
    KW_ELSE = 282,                 /* KW_ELSE  */
    KW_ELSIF = 283,                /* KW_ELSIF  */
    KW_WHILE = 284,                /* KW_WHILE  */
    KW_FOR = 285,                  /* KW_FOR  */
    KW_SUB = 286,                  /* KW_SUB  */
    KW_GATHER = 287,               /* KW_GATHER  */
    KW_TAKE = 288,                 /* KW_TAKE  */
    KW_RETURN = 289,               /* KW_RETURN  */
    KW_EXIT = 290,                 /* KW_EXIT  */
    KW_CONSTANT = 291,             /* KW_CONSTANT  */
    KW_ENUM = 292,                 /* KW_ENUM  */
    KW_GIVEN = 293,                /* KW_GIVEN  */
    KW_WHEN = 294,                 /* KW_WHEN  */
    KW_DEFAULT = 295,              /* KW_DEFAULT  */
    KW_WITH = 296,                 /* KW_WITH  */
    KW_WITHOUT = 297,              /* KW_WITHOUT  */
    KW_EXISTS = 298,               /* KW_EXISTS  */
    KW_DELETE = 299,               /* KW_DELETE  */
    KW_UNLESS = 300,               /* KW_UNLESS  */
    KW_UNTIL = 301,                /* KW_UNTIL  */
    KW_REPEAT = 302,               /* KW_REPEAT  */
    KW_LOOP = 303,                 /* KW_LOOP  */
    KW_LAST = 304,                 /* KW_LAST  */
    KW_NEXT = 305,                 /* KW_NEXT  */
    KW_MAP = 306,                  /* KW_MAP  */
    KW_GREP = 307,                 /* KW_GREP  */
    KW_SORT = 308,                 /* KW_SORT  */
    KW_TRY = 309,                  /* KW_TRY  */
    KW_CATCH = 310,                /* KW_CATCH  */
    KW_DIE = 311,                  /* KW_DIE  */
    KW_CLASS = 312,                /* KW_CLASS  */
    KW_METHOD = 313,               /* KW_METHOD  */
    KW_HAS = 314,                  /* KW_HAS  */
    KW_NEW = 315,                  /* KW_NEW  */
    KW_ROLE = 316,                 /* KW_ROLE  */
    KW_MULTI = 317,                /* KW_MULTI  */
    KW_PROTO = 318,                /* KW_PROTO  */
    OP_NAME = 319,                 /* OP_NAME  */
    OP_REDUCE = 320,               /* OP_REDUCE  */
    ARR_ALL_SLICE = 321,           /* ARR_ALL_SLICE  */
    SLURPY_POS = 322,              /* SLURPY_POS  */
    SLURPY_LOL = 323,              /* SLURPY_LOL  */
    SLURPY_NAMED = 324,            /* SLURPY_NAMED  */
    KW_HANDLES = 325,              /* KW_HANDLES  */
    WORDLIST = 326,                /* WORDLIST  */
    OP_COLON_D = 327,              /* OP_COLON_D  */
    OP_COLON_U = 328,              /* OP_COLON_U  */
    YADA = 329,                    /* YADA  */
    KW_GRAMMAR = 330,              /* KW_GRAMMAR  */
    KW_TOKEN = 331,                /* KW_TOKEN  */
    KW_RULE = 332,                 /* KW_RULE  */
    KW_REGEX = 333,                /* KW_REGEX  */
    OP_FATARROW = 334,             /* OP_FATARROW  */
    OP_RANGE = 335,                /* OP_RANGE  */
    OP_RANGE_EX = 336,             /* OP_RANGE_EX  */
    OP_ARROW = 337,                /* OP_ARROW  */
    OP_EQ = 338,                   /* OP_EQ  */
    OP_NE = 339,                   /* OP_NE  */
    OP_LE = 340,                   /* OP_LE  */
    OP_GE = 341,                   /* OP_GE  */
    OP_SEQ = 342,                  /* OP_SEQ  */
    OP_SNE = 343,                  /* OP_SNE  */
    OP_SLT = 344,                  /* OP_SLT  */
    OP_SLE = 345,                  /* OP_SLE  */
    OP_SGT = 346,                  /* OP_SGT  */
    OP_SGE = 347,                  /* OP_SGE  */
    OP_AND = 348,                  /* OP_AND  */
    OP_OR = 349,                   /* OP_OR  */
    OP_TERNARY1 = 350,             /* OP_TERNARY1  */
    OP_TERNARY2 = 351,             /* OP_TERNARY2  */
    OP_BIND = 352,                 /* OP_BIND  */
    OP_DOTEQ = 353,                /* OP_DOTEQ  */
    OP_SMATCH = 354,               /* OP_SMATCH  */
    OP_INC = 355,                  /* OP_INC  */
    OP_DEC = 356,                  /* OP_DEC  */
    OP_ADD_EQ = 357,               /* OP_ADD_EQ  */
    OP_SUB_EQ = 358,               /* OP_SUB_EQ  */
    OP_MUL_EQ = 359,               /* OP_MUL_EQ  */
    OP_DIV_EQ = 360,               /* OP_DIV_EQ  */
    OP_CAT_EQ = 361,               /* OP_CAT_EQ  */
    OP_DOR = 362,                  /* OP_DOR  */
    OP_DIV = 363,                  /* OP_DIV  */
    OP_BAND = 364,                 /* OP_BAND  */
    OP_SHL = 365,                  /* OP_SHL  */
    OP_DIVIS = 366,                /* OP_DIVIS  */
    OP_REP_X = 367,                /* OP_REP_X  */
    OP_REP_XX = 368,               /* OP_REP_XX  */
    OP_POW = 369,                  /* OP_POW  */
    UMINUS = 370                   /* UMINUS  */
  };
  typedef enum raku_yytokentype raku_yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined RAKU_YYSTYPE && ! defined RAKU_YYSTYPE_IS_DECLARED
union RAKU_YYSTYPE
{
#line 370 "raku.y"

    long      ival;
    double    dval;
    char     *sval;
    tree_t  *node;
    ExprList *list;

#line 207 "raku.tab.h"

};
typedef union RAKU_YYSTYPE RAKU_YYSTYPE;
# define RAKU_YYSTYPE_IS_TRIVIAL 1
# define RAKU_YYSTYPE_IS_DECLARED 1
#endif


extern RAKU_YYSTYPE raku_yylval;


int raku_yyparse (void);


#endif /* !YY_RAKU_YY_RAKU_TAB_H_INCLUDED  */
