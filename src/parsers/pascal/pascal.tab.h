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

#include "ir/ast.h"
#include "parsers/snobol4/scrip_cc.h"
#include "ir/ast_tt_capt_immed_asgn.h"
#include "ir/ast_node_new.h"
enum { PAS_DIALECT_ISO_DEFAULT, PAS_DIALECT_ISO, PAS_DIALECT_FPC, PAS_DIALECT_OBJFPC, PAS_DIALECT_DELPHI };
int pascal_dialect(void);
void pascal_dialect_set(int d);
int pascal_def_state(const char *n);
void pascal_def_set(const char *n, int on);
int pascal_def_value(const char *n, double *v);
void pascal_def_setval(const char *n, double v);
int *pascal_cond_unknown_p(void);
int *pascal_macro_on_p(void);
void pascal_flip_dialect(int d);
void pascal_dir_arm(void);
void pascal_stamp_dialect(tree_t *leaf);
void pascal_note_type_ident(tree_t *id);
void pascal_note_type_decl(tree_t *id);
unsigned *pascal_sw_set_p(void);
unsigned *pascal_sw_on_p(void);
void pascal_sw_push(void);
void pascal_sw_pop(void);
int *pascal_cond_skip_p(void);
int *pascal_cond_depth_p(void);
int *pascal_cond_taken_p(void);
const char *pascal_mode_macro(void);
void pascal_mode_macro_set(const char *m);
int pascal_last_tok(void);
void pascal_last_ident_set(const char *p, int n);
tree_t *pascal_last_ident_qlit(void);

#line 90 "pascal.tab.h"

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
    OTHERWISESY = 271,             /* OTHERWISESY  */
    DOSY = 272,                    /* DOSY  */
    DOWNTOSY = 273,                /* DOWNTOSY  */
    FORSY = 274,                   /* FORSY  */
    REPEATSY = 275,                /* REPEATSY  */
    WHILESY = 276,                 /* WHILESY  */
    TOSY = 277,                    /* TOSY  */
    UNTILSY = 278,                 /* UNTILSY  */
    WITHSY = 279,                  /* WITHSY  */
    CASESY = 280,                  /* CASESY  */
    PROCEDURESY = 281,             /* PROCEDURESY  */
    PACKEDSY = 282,                /* PACKEDSY  */
    BITPACKEDSY = 283,             /* BITPACKEDSY  */
    OFSY = 284,                    /* OFSY  */
    FILESY = 285,                  /* FILESY  */
    ENDSY = 286,                   /* ENDSY  */
    SETSY = 287,                   /* SETSY  */
    VARSY = 288,                   /* VARSY  */
    THENSY = 289,                  /* THENSY  */
    RECORDSY = 290,                /* RECORDSY  */
    EXPORTSSY = 291,               /* EXPORTSSY  */
    FUNCTIONSY = 292,              /* FUNCTIONSY  */
    BEGINSY = 293,                 /* BEGINSY  */
    BECOMES = 294,                 /* BECOMES  */
    TYPESY = 295,                  /* TYPESY  */
    IFSY = 296,                    /* IFSY  */
    ELSESY = 297,                  /* ELSESY  */
    INOP = 298,                    /* INOP  */
    NOTSY = 299,                   /* NOTSY  */
    IDIV = 300,                    /* IDIV  */
    IMOD = 301,                    /* IMOD  */
    ANDOP = 302,                   /* ANDOP  */
    OROP = 303,                    /* OROP  */
    LTOP = 304,                    /* LTOP  */
    LEOP = 305,                    /* LEOP  */
    GTOP = 306,                    /* GTOP  */
    GEOP = 307,                    /* GEOP  */
    NEOP = 308,                    /* NEOP  */
    EQOP = 309,                    /* EQOP  */
    PLUS = 310,                    /* PLUS  */
    MINUS = 311,                   /* MINUS  */
    MUL = 312,                     /* MUL  */
    RDIV = 313,                    /* RDIV  */
    COMMA = 314,                   /* COMMA  */
    PERIOD = 315,                  /* PERIOD  */
    COLON = 316,                   /* COLON  */
    ARROW = 317,                   /* ARROW  */
    LBRACK = 318,                  /* LBRACK  */
    RBRACK = 319,                  /* RBRACK  */
    LPARENT = 320,                 /* LPARENT  */
    RPARENT = 321,                 /* RPARENT  */
    DOTDOT = 322,                  /* DOTDOT  */
    ATSIGN = 323,                  /* ATSIGN  */
    ADDBECOMES = 324,              /* ADDBECOMES  */
    SUBBECOMES = 325,              /* SUBBECOMES  */
    MULBECOMES = 326,              /* MULBECOMES  */
    DIVBECOMES = 327,              /* DIVBECOMES  */
    INTCONST = 328,                /* INTCONST  */
    CHARCODE = 329,                /* CHARCODE  */
    REALCONST = 330,               /* REALCONST  */
    STRINGCONST = 331,             /* STRINGCONST  */
    IDENT = 332,                   /* IDENT  */
    SHIFTOP = 333,                 /* SHIFTOP  */
    XOROP = 334,                   /* XOROP  */
    POWOP = 335,                   /* POWOP  */
    CLASSSY = 336,                 /* CLASSSY  */
    ISOP = 337,                    /* ISOP  */
    ASOP = 338,                    /* ASOP  */
    OBJECTSY = 339,                /* OBJECTSY  */
    TRYSY = 340,                   /* TRYSY  */
    EXCEPTSY = 341,                /* EXCEPTSY  */
    FINALLYSY = 342,               /* FINALLYSY  */
    RAISESY = 343,                 /* RAISESY  */
    INHERITEDSY = 344,             /* INHERITEDSY  */
    CONSTRUCTORSY = 345,           /* CONSTRUCTORSY  */
    DESTRUCTORSY = 346,            /* DESTRUCTORSY  */
    PROPERTYSY = 347,              /* PROPERTYSY  */
    ONSY = 348,                    /* ONSY  */
    GENERICSY = 349,               /* GENERICSY  */
    GENERICTSY = 350,              /* GENERICTSY  */
    SPECIALIZESY = 351,            /* SPECIALIZESY  */
    GENLT = 352,                   /* GENLT  */
    GENGT = 353                    /* GENGT  */
  };
  typedef enum pascal_yytokentype pascal_yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined PASCAL_YYSTYPE && ! defined PASCAL_YYSTYPE_IS_DECLARED
union PASCAL_YYSTYPE
{
#line 212 "pascal.y"
 tree_t *node; 

#line 208 "pascal.tab.h"

};
typedef union PASCAL_YYSTYPE PASCAL_YYSTYPE;
# define PASCAL_YYSTYPE_IS_TRIVIAL 1
# define PASCAL_YYSTYPE_IS_DECLARED 1
#endif


extern PASCAL_YYSTYPE pascal_yylval;


int pascal_yyparse (void);


#endif /* !YY_PASCAL_YY_PASCAL_TAB_H_INCLUDED  */
