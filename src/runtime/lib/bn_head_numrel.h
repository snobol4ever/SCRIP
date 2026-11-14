#ifndef BN_HEAD_NUMREL_H
#define BN_HEAD_NUMREL_H
#include <glob.h>
#include "runtime/core/core_rtx_str_shim_api.h"
#include "runtime/core/core_arithmetic_api.h"
#include "runtime/core/g_runtime.h"
#include "runtime/core/g_runtime_pas.h"
#include "runtime/core/g_runtime_bn.h"
#include "runtime/rt/rtx_try.h"
#include "runtime/core/array.h"
#include "runtime/core/datatype.h"
#include "runtime/core/table.h"
#include "runtime/rt/rt_pl_trail.h"
#include "runtime/core/g_runtime_abi.h"
#include "runtime/rt/gc_heap_etc.h"
#include "runtime/rt/gc_heap.h"
#include "runtime/rt/gc_heap_rt_asm_helpers_api.h"
#include "emitter/sil_macros.h"
#include "runtime/rt/rt.h"
#include "parsers/prolog/pl_cell.h"
#ifdef __cplusplus
extern "C" {
#endif
#define _OPCOERCE(d) do { if (!IS_INT_fn(d) && !IS_REAL_fn(d)) { const char *_s = IS_STR(d) ? rt_cstr_d(d) : VARVAL_fn(d); if (_s && *_s) { char *_e=NULL; long long _iv=strtoll(_s,&_e,10); if (_e && \
    !*_e){(d)=INTVAL(_iv);} else {double _rv=strtod(_s,&_e); if(_e && !*_e){(d)=REALVAL(_rv);}else{*out=FAILDESCR;return 1;}} } else { *out=FAILDESCR; return 1; } } } while(0)
#define _NUMREL(op) do { DESCR_t _l=args[0],_r=args[1]; _OPCOERCE(_l); _OPCOERCE(_r); double _lv2=IS_REAL_fn(_l)?_l.r:(double)_l.i, _rv2=IS_REAL_fn(_r)?_r.r:(double)_r.i; *out=(_lv2 op _rv2)?(( \
    IS_REAL_fn(_l)&&!IS_REAL_fn(_r))?REALVAL(_rv2):_r):FAILDESCR; return 1; } while(0)
#define _STRREL(op) do { DESCR_t _l=args[0],_r=args[1]; const char *_ls=VARVAL_fn(_l); if(!_ls)_ls=""; const char *_rs=VARVAL_fn(_r); if(!_rs)_rs=""; int _cmp=strcmp(_ls,_rs); *out=(_cmp op 0)? \
    rtx_try_rt_str_coerce(_r):FAILDESCR; return 1; } while(0)
#define _SNOCOERCE(d) do { if (!IS_INT_fn(d) && !IS_REAL_fn(d)) { const char *_s9 = IS_STR(d) ? rt_cstr_d(d) : VARVAL_fn(d); if (!_s9 || !*_s9) { (d) = INTVAL(0); } else { _OPCOERCE(d); } } } while(0)
#ifdef __cplusplus
}
#endif
#endif
