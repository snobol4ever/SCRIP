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
    KW_CONSTANT = 290,             /* KW_CONSTANT  */
    KW_GIVEN = 291,                /* KW_GIVEN  */
    KW_WHEN = 292,                 /* KW_WHEN  */
    KW_DEFAULT = 293,              /* KW_DEFAULT  */
    KW_WITH = 294,                 /* KW_WITH  */
    KW_WITHOUT = 295,              /* KW_WITHOUT  */
    KW_EXISTS = 296,               /* KW_EXISTS  */
    KW_DELETE = 297,               /* KW_DELETE  */
    KW_UNLESS = 298,               /* KW_UNLESS  */
    KW_UNTIL = 299,                /* KW_UNTIL  */
    KW_REPEAT = 300,               /* KW_REPEAT  */
    KW_LOOP = 301,                 /* KW_LOOP  */
    KW_LAST = 302,                 /* KW_LAST  */
    KW_NEXT = 303,                 /* KW_NEXT  */
    KW_MAP = 304,                  /* KW_MAP  */
    KW_GREP = 305,                 /* KW_GREP  */
    KW_SORT = 306,                 /* KW_SORT  */
    KW_TRY = 307,                  /* KW_TRY  */
    KW_CATCH = 308,                /* KW_CATCH  */
    KW_DIE = 309,                  /* KW_DIE  */
    KW_CLASS = 310,                /* KW_CLASS  */
    KW_METHOD = 311,               /* KW_METHOD  */
    KW_HAS = 312,                  /* KW_HAS  */
    KW_NEW = 313,                  /* KW_NEW  */
    KW_ROLE = 314,                 /* KW_ROLE  */
    KW_MULTI = 315,                /* KW_MULTI  */
    KW_PROTO = 316,                /* KW_PROTO  */
    OP_NAME = 317,                 /* OP_NAME  */
    OP_REDUCE = 318,               /* OP_REDUCE  */
    ARR_ALL_SLICE = 319,           /* ARR_ALL_SLICE  */
    SLURPY_POS = 320,              /* SLURPY_POS  */
    SLURPY_LOL = 321,              /* SLURPY_LOL  */
    SLURPY_NAMED = 322,            /* SLURPY_NAMED  */
    KW_HANDLES = 323,              /* KW_HANDLES  */
    WORDLIST = 324,                /* WORDLIST  */
    OP_COLON_D = 325,              /* OP_COLON_D  */
    OP_COLON_U = 326,              /* OP_COLON_U  */
    YADA = 327,                    /* YADA  */
    KW_GRAMMAR = 328,              /* KW_GRAMMAR  */
    KW_TOKEN = 329,                /* KW_TOKEN  */
    KW_RULE = 330,                 /* KW_RULE  */
    KW_REGEX = 331,                /* KW_REGEX  */
    OP_FATARROW = 332,             /* OP_FATARROW  */
    OP_RANGE = 333,                /* OP_RANGE  */
    OP_RANGE_EX = 334,             /* OP_RANGE_EX  */
    OP_ARROW = 335,                /* OP_ARROW  */
    OP_EQ = 336,                   /* OP_EQ  */
    OP_NE = 337,                   /* OP_NE  */
    OP_LE = 338,                   /* OP_LE  */
    OP_GE = 339,                   /* OP_GE  */
    OP_SEQ = 340,                  /* OP_SEQ  */
    OP_SNE = 341,                  /* OP_SNE  */
    OP_SLT = 342,                  /* OP_SLT  */
    OP_SLE = 343,                  /* OP_SLE  */
    OP_SGT = 344,                  /* OP_SGT  */
    OP_SGE = 345,                  /* OP_SGE  */
    OP_AND = 346,                  /* OP_AND  */
    OP_OR = 347,                   /* OP_OR  */
    OP_TERNARY1 = 348,             /* OP_TERNARY1  */
    OP_TERNARY2 = 349,             /* OP_TERNARY2  */
    OP_BIND = 350,                 /* OP_BIND  */
    OP_DOTEQ = 351,                /* OP_DOTEQ  */
    OP_SMATCH = 352,               /* OP_SMATCH  */
    OP_INC = 353,                  /* OP_INC  */
    OP_DEC = 354,                  /* OP_DEC  */
    OP_ADD_EQ = 355,               /* OP_ADD_EQ  */
    OP_SUB_EQ = 356,               /* OP_SUB_EQ  */
    OP_MUL_EQ = 357,               /* OP_MUL_EQ  */
    OP_DIV_EQ = 358,               /* OP_DIV_EQ  */
    OP_CAT_EQ = 359,               /* OP_CAT_EQ  */
    OP_DOR = 360,                  /* OP_DOR  */
    OP_DIV = 361,                  /* OP_DIV  */
    OP_BAND = 362,                 /* OP_BAND  */
    OP_SHL = 363,                  /* OP_SHL  */
    OP_DIVIS = 364,                /* OP_DIVIS  */
    OP_REP_X = 365,                /* OP_REP_X  */
    OP_REP_XX = 366,               /* OP_REP_XX  */
    OP_POW = 367,                  /* OP_POW  */
    UMINUS = 368                   /* UMINUS  */
  };
  typedef enum raku_yytokentype raku_yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined RAKU_YYSTYPE && ! defined RAKU_YYSTYPE_IS_DECLARED
union RAKU_YYSTYPE
{
#line 311 "raku.y"

    long      ival;
    double    dval;
    char     *sval;
    tree_t  *node;
    ExprList *list;

#line 205 "raku.tab.h"

};
typedef union RAKU_YYSTYPE RAKU_YYSTYPE;
# define RAKU_YYSTYPE_IS_TRIVIAL 1
# define RAKU_YYSTYPE_IS_DECLARED 1
#endif


extern RAKU_YYSTYPE raku_yylval;


int raku_yyparse (void);


#endif /* !YY_RAKU_YY_RAKU_TAB_H_INCLUDED  */
