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
enum { PAS_DIALECT_ISO_DEFAULT, PAS_DIALECT_ISO, PAS_DIALECT_FPC, PAS_DIALECT_OBJFPC, PAS_DIALECT_DELPHI };
int pascal_dialect(void);
void pascal_dialect_set(int d);
int pascal_def_state(const char *n);
void pascal_def_set(const char *n, int on);
int pascal_def_value(const char *n, double *v);
void pascal_def_setval(const char *n, double v);
int *pascal_cond_unknown_p(void);
int *pascal_macro_on_p(void);
unsigned *pascal_sw_set_p(void);
unsigned *pascal_sw_on_p(void);
void pascal_sw_push(void);
void pascal_sw_pop(void);
int *pascal_cond_skip_p(void);
int *pascal_cond_depth_p(void);
int *pascal_cond_taken_p(void);
const char *pascal_mode_macro(void);
void pascal_mode_macro_set(const char *m);

#line 80 "pascal.tab.h"

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
    USESSY = 265,                  /* USESSY  */
    UNITSY = 266,                  /* UNITSY  */
    INTERFACESY = 267,             /* INTERFACESY  */
    IMPLEMENTATIONSY = 268,        /* IMPLEMENTATIONSY  */
    INITIALIZATIONSY = 269,        /* INITIALIZATIONSY  */
    FINALIZATIONSY = 270,          /* FINALIZATIONSY  */
    DOSY = 271,                    /* DOSY  */
    DOWNTOSY = 272,                /* DOWNTOSY  */
    FORSY = 273,                   /* FORSY  */
    REPEATSY = 274,                /* REPEATSY  */
    WHILESY = 275,                 /* WHILESY  */
    TOSY = 276,                    /* TOSY  */
    UNTILSY = 277,                 /* UNTILSY  */
    WITHSY = 278,                  /* WITHSY  */
    CASESY = 279,                  /* CASESY  */
    PROCEDURESY = 280,             /* PROCEDURESY  */
    PACKEDSY = 281,                /* PACKEDSY  */
    OFSY = 282,                    /* OFSY  */
    FILESY = 283,                  /* FILESY  */
    ENDSY = 284,                   /* ENDSY  */
    SETSY = 285,                   /* SETSY  */
    VARSY = 286,                   /* VARSY  */
    THENSY = 287,                  /* THENSY  */
    RECORDSY = 288,                /* RECORDSY  */
    FUNCTIONSY = 289,              /* FUNCTIONSY  */
    BEGINSY = 290,                 /* BEGINSY  */
    BECOMES = 291,                 /* BECOMES  */
    TYPESY = 292,                  /* TYPESY  */
    IFSY = 293,                    /* IFSY  */
    ELSESY = 294,                  /* ELSESY  */
    INOP = 295,                    /* INOP  */
    NOTSY = 296,                   /* NOTSY  */
    IDIV = 297,                    /* IDIV  */
    IMOD = 298,                    /* IMOD  */
    ANDOP = 299,                   /* ANDOP  */
    OROP = 300,                    /* OROP  */
    LTOP = 301,                    /* LTOP  */
    LEOP = 302,                    /* LEOP  */
    GTOP = 303,                    /* GTOP  */
    GEOP = 304,                    /* GEOP  */
    NEOP = 305,                    /* NEOP  */
    EQOP = 306,                    /* EQOP  */
    PLUS = 307,                    /* PLUS  */
    MINUS = 308,                   /* MINUS  */
    MUL = 309,                     /* MUL  */
    RDIV = 310,                    /* RDIV  */
    COMMA = 311,                   /* COMMA  */
    PERIOD = 312,                  /* PERIOD  */
    COLON = 313,                   /* COLON  */
    ARROW = 314,                   /* ARROW  */
    LBRACK = 315,                  /* LBRACK  */
    RBRACK = 316,                  /* RBRACK  */
    LPARENT = 317,                 /* LPARENT  */
    RPARENT = 318,                 /* RPARENT  */
    DOTDOT = 319,                  /* DOTDOT  */
    ATSIGN = 320,                  /* ATSIGN  */
    INTCONST = 321,                /* INTCONST  */
    CHARCODE = 322,                /* CHARCODE  */
    REALCONST = 323,               /* REALCONST  */
    STRINGCONST = 324,             /* STRINGCONST  */
    IDENT = 325                    /* IDENT  */
  };
  typedef enum pascal_yytokentype pascal_yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined PASCAL_YYSTYPE && ! defined PASCAL_YYSTYPE_IS_DECLARED
union PASCAL_YYSTYPE
{
#line 133 "pascal.y"
 tree_t *node; 

#line 170 "pascal.tab.h"

};
typedef union PASCAL_YYSTYPE PASCAL_YYSTYPE;
# define PASCAL_YYSTYPE_IS_TRIVIAL 1
# define PASCAL_YYSTYPE_IS_DECLARED 1
#endif


extern PASCAL_YYSTYPE pascal_yylval;


int pascal_yyparse (void);


#endif /* !YY_PASCAL_YY_PASCAL_TAB_H_INCLUDED  */
