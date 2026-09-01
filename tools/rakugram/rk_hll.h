/* rk_hll.h -- the hand-written NQP HLL::Grammar layer for the Grammar.nqp port. */
#ifndef RK_HLL_H
#define RK_HLL_H
#include "rk_cur.h"
typedef int (*RkRule)(RkCur *);
int rk_ws(RkCur *); int rk_ident(RkCur *); int rk_identifier(RkCur *);
int rk_decint(RkCur *); int rk_hexint(RkCur *); int rk_octint(RkCur *); int rk_binint(RkCur *);
int rk_before(RkCur *, RkRule); int rk_after(RkCur *, RkRule);
int rk_panic(RkCur *); int rk_typed_panic(RkCur *); int rk_sorry(RkCur *); int rk_worry(RkCur *);
int rk_obs(RkCur *); int rk_malformed(RkCur *); int rk_missing(RkCur *); int rk_NYI(RkCur *);
#endif
