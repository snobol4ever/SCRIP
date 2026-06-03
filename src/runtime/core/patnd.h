#ifndef PATND_H
#define PATND_H
#include <stdint.h>
typedef enum {
    XCHR,
    XSPNC,
    XBRKC,
    XANYC,
    XNNYC,
    XLNTH,
    XPOSI,
    XRPSI,
    XTB,
    XRTB,
    XFARB,
    XARBN,
    XSTAR,
    XFNCE,
    XFAIL,
    XABRT,
    XSUCF,
    XBAL,
    XEPS,
    XCAT,
    XOR,
    XDSAR,
    XFNME,
    XNME,
    XCALLCAP,
    XVAR,
    XATP,
    XBRKX,
} XKIND_t;
/*--------------------------------------------------------------------------------------------------------------------*/
struct _PATND_t;
typedef struct _PATND_t PATND_t;
struct _PATND_t {
    XKIND_t      kind;
    int          materialising;
    const char  *STRVAL_fn;
    int64_t      num;
    PATND_t    **children;
    int          nchildren;
    DESCR_t      var;
    DESCR_t     *args;
    int          nargs;
    char       **arg_names;
    int          n_arg_names;
    int          imm;
};
/*--------------------------------------------------------------------------------------------------------------------*/
#define PATND_CHILD0(p)  ((p)->c ? (p)->c[0] : NULL)
#include <stdio.h>
void patnd_print(const PATND_t *p, FILE *out);
PATND_t *patnd_make_xchr(const char *lit);
PATND_t *patnd_make_eps(void);
/*--------------------------------------------------------------------------------------------------------------------*/
int patnd_contains_arbno(const PATND_t *pp);
int patnd_contains_defer(const PATND_t *pp);
int patnd_is_combinator_root(const PATND_t *pp);
int patnd_needs_xlate(const PATND_t *pp);
int patnd_is_pure_altcat(const PATND_t *pp);
#endif
