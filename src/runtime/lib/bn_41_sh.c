#include "runtime/lib/bn_head.h"
#include "runtime/core/g_runtime_misc.h"
#include "ir/dtp.h"
#include "runtime/by_name_dispatch.h"
#include "runtime/core/core_error.h"
#include "runtime/core/icn_builtins.h"
#include "runtime/core/nv.h"
#include <sys/stat.h>
#include <math.h>
#include "runtime/snobol4_system_fns.h"
#include "runtime/rt/rt_arena.h"
#include "runtime/builtins/gen_value.h"
#include "runtime/core/runtime_shim.h"
#include "driver/driver.h"
#include "driver/driver_globals.h"
#include "driver/driver_data.h"
#include "runtime/builtin_ids.h"
#include "runtime/builtin_leaf.h"
#include "runtime/rt/rt_list_view.h"
#include <errno.h>
#include <ctype.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <sys/socket.h>
#include "runtime/lib/bn_01_sh.h"
#include "runtime/lib/bn_02_sh.h"
#include "runtime/lib/bn_03_sh.h"
#include "runtime/lib/bn_04_pl.h"
#include "runtime/lib/bn_08_sh.h"
#include "runtime/lib/bn_09_rk.h"
#include "runtime/lib/bn_10_pas.h"
#include "runtime/lib/bn_11_pl.h"
#include "runtime/lib/bn_12_rk.h"
#include "runtime/lib/bn_13_rk.h"
#include "runtime/lib/bn_16_rk.h"
#include "runtime/lib/bn_17_pas.h"
#include "runtime/lib/bn_19_rk.h"
#include "runtime/lib/bn_24_sh.h"
#include "runtime/lib/bn_25_sh.h"
#include "runtime/lib/bn_26_sh.h"
#include "runtime/lib/bn_36_sh.h"
#include "runtime/lib/bn_37_pas.h"
#include "runtime/lib/bn_39_sh.h"
#include "runtime/lib/bn_40_sh.h"
#include "runtime/lib/bn_41_sh.h"
extern int rt_icn_cset_member_n(const char *, long, int);
int core_icn_error(int code, DESCR_t val);
extern ssize_t rt_line_read(char **buf, size_t *cap, FILE *f);
extern gv_t g_call_args;
void rt_call_args_need(int n);
extern DESCR_t dat_construct_rq(DatType *, DESCR_t *, int, long *);
extern void *rt_ws_alloc_descr(size_t);
extern const char *icon_real_str(double r, char *buf, int bufsz);
extern int rt_proc_is_registered(const char *name);
extern int rt_proc_nparams(const char *name);
extern DESCR_t rt_deref(DESCR_t d);
extern FILE *fh_get(int);
extern unsigned rt_dtax_gen;
int try_call_builtin_by_name_bl_s_rq(const char *fn, DESCR_t *args, int nargs, DESCR_t *out, int bidlen, int strict, long *rq) {
    if (!strict && nargs == 1 && IS_DATA_INST_fn(args[0]) && args[0].u && args[0].u->type) {
        DATBLK_t *idb = args[0].u->type;
        const char _f0 = fn ? fn[0] : 0;
        for (int fi = 0; fi < idb->nfields; fi++) if (idb->fields[fi] && idb->fields[fi][0] == _f0 && !strcmp(idb->fields[fi], fn)) {
            extern DESCR_t c_dat_field_get(const char *field, DESCR_t obj);
            *out = rtx_try_dat_field_get(fn, args[0]);
            return 1;
        }
    }
    if (!fn || !out) return 0;
    for (int _ni = 0; _ni < nargs; _ni++) {
        if (args[_ni].v == DT_S && args[_ni].s && args[_ni].slen && args[_ni].slen != 0xFFFFFFFFu && args[_ni].s[args[_ni].slen]) args[_ni].s = (char *)rt_cstr_materialize(args[_ni]);
    }
    if (bidlen >= 0) { const int _fb0 = bidlen & BID_BAKE_MASK; if (_fb0 == BID_SNOx24NAME && nargs == 1) return bn_sno_name(args, nargs, out); }
    if (bidlen >= 0 && rt_dtax_gen == 0 && !dtax_off()) {
        const int _fb = bidlen & BID_BAKE_MASK;
#if RT_DIAG
        extern void bidprof_init(void);
        if (g_runtime.bn.bidprof_on < 0) bidprof_init();
        if (g_runtime.bn.bidprof_on && _fb >= 0 && _fb < 1024) g_runtime.bn.bidprof[_fb]++;
#endif
        int _fr = -1;
        if (_fb == BID_SIZE) _fr = bn_size(args, nargs, out);
        else if (_fb == BID_REPLACE) _fr = bn_replace(args, nargs, out);
        else if (_fb == BID_TRIM) _fr = bn_trim(args, nargs, out);
        else if (_fb == BID_SUBSTR) _fr = bn_substr(args, nargs, out);
        else if (_fb == BID_REVERSE) _fr = bn_reverse(args, nargs, out);
        else if (_fb == BID_INTEGER) _fr = bn_integer(args, nargs, out);
        else if (_fb == BID_DUPL) _fr = bn_dupl(args, nargs, out);
        else if (_fb == BID_LPAD) _fr = bn_lpad(args, nargs, out);
        else if (_fb == BID_RPAD) _fr = bn_rpad(args, nargs, out);
        else if (_fb == BID_REMDR) _fr = bn_remdr(args, nargs, out);
        else if (_fb == BID_DATE) _fr = bn_date(args, nargs, out);
        else if (_fb == BID_TIME) _fr = bn_time(args, nargs, out);
        else if (_fb == BID_IDENT) _fr = bn_identdiffer(args, nargs, out, 1);
        else if (_fb == BID_DIFFER) _fr = bn_identdiffer(args, nargs, out, 0);
        else if (_fb == BID_SNOx24NAME && nargs == 1) _fr = bn_sno_name(args, nargs, out);
        if (_fr >= 0) return _fr;
    }
    const size_t _fnlen = (bidlen >= 0) ? (size_t)((unsigned)bidlen >> 16) : strlen(fn);
    const int _bid = (bidlen >= 0) ? (int)(bidlen & BID_BAKE_MASK) : bid_of(fn, (unsigned)_fnlen);
    if (_bid == 0) {
        int _br = bt_row_of(BT_VOC_SNO, fn, (unsigned)_fnlen);
        if (_br < 0) _br = bt_row_of(BT_VOC_ICN, fn, (unsigned)_fnlen);
        if (_br >= 0 && g_bt_leaf[_br]) {
            const DESCR_t _bv = g_bt_leaf[_br](args, nargs);
            if (_bv.v != RTX_NOT_HANDLED) { *out = _bv; return 1; }
        } else if (_br >= 0 && g_bt_open[_br] && g_bt_open[_br](args, nargs, out, rq)) return 1;
    }
#if RT_DIAG
    { extern void bidprof_init(void); if (g_runtime.bn.bidprof_on < 0) bidprof_init(); if (g_runtime.bn.bidprof_on && _bid >= 0 && _bid < 1024) g_runtime.bn.bidprof[_bid]++; }
#endif
    {
        extern int rt_g_want_name;
        if (rt_g_want_name && nargs == 1 && IS_DATA_TAG_fn(args[0].v)) {
            extern const char *FUNC_ENTRY_fn(const char *);
            extern int rt_dat_field_of_any(const char *);
            extern DESCR_t rt_field_var(const char *field, DESCR_t obj);
            const char *_fe = FUNC_ENTRY_fn(fn);
            if (!_fe) _fe = fn;
            if (rt_dat_field_of_any(_fe)) { rt_g_want_name = 0; *out = rt_field_var(_fe, args[0]); return 1; }
        }
    }
    dtax_ent_t *_dx = 0;
    int _dx_hit = 0;
    int _dx_skip_ctor = 0;
    int _dx_skip_syn = 0;
    unsigned _dxh = 5381u;
    unsigned char _dxl = 0;
    int _dx_bid_path = 0;
    if (!dtax_off()) {
        if (_bid > 0 && _bid <= 1024 && _fnlen && _fnlen < 14) {
            _dxl = (unsigned char)_fnlen;
            _dx = &g_runtime.bn.dtax_bid[_bid];
            _dx_bid_path = 1;
        } else {
            const char *_q = fn;
            while (*_q && _dxl < 14) { _dxh = _dxh * 131u + (unsigned char)*_q; _q++; _dxl++; }
            if (!(!*_q && _dxl)) _dxl = 0;
            else _dx = &g_runtime.bn.dtax[_dxh & 255u];
        }
        if (_dx) {
            if (_dx->gen == rt_dtax_gen && (_dx_bid_path || (_dx->len == _dxl && !memcmp(_dx->dxn, fn, _dxl)))) {
                if (_dx->kind == 4 && _dx->ctor) { int _cr = ((int (*)(DESCR_t *, int, DESCR_t *))_dx->ctor)(args, nargs, out); if (_cr >= 0) return _cr; }
                if (_dx->kind == 5 && _dx->ctor) { int _cr = ((int (*)(DESCR_t *, int, DESCR_t *, int))_dx->ctor)(args, nargs, out, (int)_dx->nf); if (_cr >= 0) return _cr; }
                if (_dx->kind == 2 && _dx->syn) return try_call_builtin_by_name_eval_rq(_dx->syn, args, nargs, out, rq);
                if (_dx->kind == 1 && _dx->ctor) {
                    extern DESCR_t dat_construct_byref(void *, DESCR_t *, int);
                    extern int dat_nfields_byref(void *);
                    if (nargs <= dat_nfields_byref(_dx->ctor)) { rt_ctor_ic_note(fn, _dx->ctor); *out = dat_construct_byref(_dx->ctor, args, nargs); return 1; }
                    _dx_hit = 1;
                    _dx_skip_ctor = 1;
                } else if (_dx->kind == 0) {
                    _dx_hit = 1;
                    _dx_skip_ctor = 1;
                    _dx_skip_syn = 1;
                } else if (_dx->kind == 3) {
                    _dx_hit = 1;
                    _dx_skip_ctor = 1;
                }
            }
        }
    }
    if (!_dx_skip_ctor) {
        extern DatType *dat_find_type(const char *);
        extern DESCR_t dat_construct(DatType *, DESCR_t *, int);
        extern int dat_nfields_byref(void *);
        DatType *_udt = dat_find_type(fn);
        if (_udt && _dx) {
            _dx->gen = rt_dtax_gen;
            _dx->len = _dxl;
            _dx->kind = 1;
            _dx->nf = (short)dat_nfields_byref((void *)_udt);
            memcpy(_dx->dxn, fn, _dxl);
            _dx->ctor = (void *)_udt;
            _dx->syn = 0;
        }
        if (_udt && nargs <= _udt->nfields) { rt_ctor_ic_note(fn, (void *)_udt); *out = dat_construct_rq(_udt, args, nargs, rq); return 1; }
        if (!_udt && _dx && !_dx_hit) { _dx->gen = rt_dtax_gen; _dx->len = _dxl; _dx->kind = 3; _dx->nf = 0; memcpy(_dx->dxn, fn, _dxl); _dx->ctor = 0; _dx->syn = 0; }
    }
    {
        size_t _fl = _fnlen;
        if (_fl >= 2 && _fl <= 8) {
            int _r = -1;
            switch (((unsigned)_fl << 8) | (unsigned char)fn[0]) {
                case (2u<<8)|'E':
                if (fn[1]=='Q') { dtx5(_dx, _dxl, fn, (void *)bn_numrel, 0); _r = bn_numrel(args, nargs, out, 0); }
                break;
                case (2u<<8)|'N':
                if (fn[1]=='E') { dtx5(_dx, _dxl, fn, (void *)bn_numrel, 1); _r = bn_numrel(args, nargs, out, 1); }
                break;
                case (2u<<8)|'L':
                if (fn[1]=='T') {
                    dtx5(_dx, _dxl, fn, (void *)bn_numrel, 2);
                    _r = bn_numrel(args, nargs, out, 2);
                } else if (fn[1]=='E') {
                    dtx5(_dx, _dxl, fn, (void *)bn_numrel, 3);
                    _r = bn_numrel(args, nargs, out, 3);
                }
                break;
                case (2u<<8)|'G':
                if (fn[1]=='T') {
                    dtx5(_dx, _dxl, fn, (void *)bn_numrel, 4);
                    _r = bn_numrel(args, nargs, out, 4);
                } else if (fn[1]=='E') {
                    dtx5(_dx, _dxl, fn, (void *)bn_numrel, 5);
                    _r = bn_numrel(args, nargs, out, 5);
                }
                break;
                case (3u<<8)|'L':
                {
                    int _o = fn[1]=='G'&&fn[2]=='T' ? 0 : fn[1]=='L'&&fn[2]=='T' ? 1 : fn[1]=='G'&&fn[2]=='E' ? 2 : fn[1]=='L'&&fn[2]=='E' ? 3 : fn[1]=='E'&&fn[2]=='Q' ? 4 : fn[1]=='N'&&
                        fn[2]=='E' ? 5 : -1;
                    if (_o >= 0) { dtx5(_dx, _dxl, fn, (void *)bn_lexrel, (short)_o); _r = bn_lexrel(args, nargs, out, _o); }
                }
                break;
                case (4u<<8)|'S':
                if ((_bid == BID_SIZE)) { dtx4(_dx, _dxl, fn, (void *)bn_size); _r = bn_size(args, nargs, out); }
                break;
                case (4u<<8)|'T':
                if ((_bid == BID_TIME)) {
                    dtx4(_dx, _dxl, fn, (void *)bn_time);
                    _r = bn_time(args, nargs, out);
                } else if ((_bid == BID_TRIM)) {
                    dtx4(_dx, _dxl, fn, (void *)bn_trim);
                    _r = bn_trim(args, nargs, out);
                }
                break;
                case (4u<<8)|'D':
                if ((_bid == BID_DATE)) {
                    dtx4(_dx, _dxl, fn, (void *)bn_date);
                    _r = bn_date(args, nargs, out);
                } else if ((_bid == BID_DUPL)) {
                    dtx4(_dx, _dxl, fn, (void *)bn_dupl);
                    _r = bn_dupl(args, nargs, out);
                }
                break;
                case (4u<<8)|'F':
                if ((_bid == BID_FAIL)) { *out = FAILDESCR; return 1; }
                break;
                case (4u<<8)|'L':
                if ((_bid == BID_LPAD)) { dtx4(_dx, _dxl, fn, (void *)bn_lpad); _r = bn_lpad(args, nargs, out); }
                break;
                case (4u<<8)|'R':
                if ((_bid == BID_RPAD)) { dtx4(_dx, _dxl, fn, (void *)bn_rpad); _r = bn_rpad(args, nargs, out); }
                break;
                case (5u<<8)|'I':
                if ((_bid == BID_IDENT)) { dtx5(_dx, _dxl, fn, (void *)bn_identdiffer, 1); _r = bn_identdiffer(args, nargs, out, 1); }
                break;
                case (5u<<8)|'R':
                if ((_bid == BID_REMDR)) { dtx4(_dx, _dxl, fn, (void *)bn_remdr); _r = bn_remdr(args, nargs, out); }
                break;
                case (6u<<8)|'S':
                if ((_bid == BID_SUBSTR)) { dtx4(_dx, _dxl, fn, (void *)bn_substr); _r = bn_substr(args, nargs, out); }
                break;
                case (6u<<8)|'D':
                if ((_bid == BID_DIFFER)) { dtx5(_dx, _dxl, fn, (void *)bn_identdiffer, 0); _r = bn_identdiffer(args, nargs, out, 0); }
                break;
                case (7u<<8)|'R':
                if ((_bid == BID_REPLACE)) {
                    dtx4(_dx, _dxl, fn, (void *)bn_replace);
                    _r = bn_replace(args, nargs, out);
                } else if ((_bid == BID_REVERSE)) {
                    dtx4(_dx, _dxl, fn, (void *)bn_reverse);
                    _r = bn_reverse(args, nargs, out);
                }
                break;
                case (7u<<8)|'I':
                if ((_bid == BID_INTEGER)) { dtx4(_dx, _dxl, fn, (void *)bn_integer); _r = bn_integer(args, nargs, out); }
                break;
                case (7u<<8)|'S':
                if ((_bid == BID_SUCCEED)) { *out = NULVCL; return 1; }
                break;
                case (8u<<8)|'D':
                if ((_bid == BID_DATATYPE) && nargs == 1) _r = bn_type_datatype(fn, args, nargs, out);
                break;
                case (8u<<8)|'S':
                if ((_bid == BID_SNOx24NAME) && nargs == 1) _r = bn_sno_name(args, nargs, out);
                else if ((_bid == BID_SNOx24NRET)) { extern int rt_g_ret_by_name; rt_g_ret_by_name = 1; *out = NULVCL; _r = 1; }
                break;
                default:
                break;
            }
            if (_r >= 0) return _r;
        }
    }
    if (icn_argtype_gate(_bid, args, nargs, out, strict)) return 1;
    if (g_runtime.bn.bidjmp_on) switch (_bid) {
        case BID___rk_bool:
        goto L_bidjmp_5076;
        case BID___rk_defined:
        goto L_bidjmp_5087;
        case BID___rk_dor:
        goto L_bidjmp_5090;
        case BID___rk_bool_val:
        goto L_bidjmp_5093;
        case BID___pas_chr:
        goto L_bidjmp_5103;
        case BID___pas_chrlit:
        goto L_bidjmp_5109;
        case BID___pas_enum_name:
        goto L_bidjmp_5112;
        case BID___pas_read_i:
        goto L_bidjmp_5121;
        case BID___pas_read_c:
        goto L_bidjmp_5124;
        case BID___pas_readln:
        goto L_bidjmp_5127;
        case BID___pas_eof:
        goto L_bidjmp_5130;
        case BID___pas_eoln:
        goto L_bidjmp_5135;
        case BID___pas_getbufch:
        goto L_bidjmp_5149;
        case BID___pas_trunc:
        goto L_bidjmp_5152;
        case BID___pas_abs:
        goto L_bidjmp_5156;
        case BID___pas_writeln:
        goto L_bidjmp_5171;
        case BID___pas_write:
        goto L_bidjmp_5171;
        case BID_write:
        goto L_bidjmp_5209;
        case BID_writes:
        goto L_bidjmp_5209;
        case BID_integer:
        goto L_bidjmp_5232;
        case BID_real:
        goto L_bidjmp_5264;
        case BID_string:
        goto L_bidjmp_5273;
        case BID_numeric:
        goto L_bidjmp_5283;
        case BID_cset:
        goto L_bidjmp_5320;
        case BID_ord:
        goto L_bidjmp_5330;
        case BID_image:
        goto L_bidjmp_5336;
        case BID_args:
        goto L_bidjmp_5339;
        case BID_proc:
        goto L_bidjmp_5385;
        case BID_repl:
        goto L_bidjmp_5524;
        case BID_reverse:
        goto L_bidjmp_5531;
        case BID_map:
        goto L_bidjmp_5537;
        case BID_trim:
        goto L_bidjmp_5567;
        case BID_getenv:
        goto L_bidjmp_5576;
        case BID_collect:
        goto L_bidjmp_5584;
        case BID_left:
        goto L_bidjmp_5585;
        case BID_right:
        goto L_bidjmp_5613;
        case BID_center:
        goto L_bidjmp_5639;
        case BID_detab:
        goto L_bidjmp_5671;
        case BID_entab:
        goto L_bidjmp_5704;
        case BID_abs:
        goto L_bidjmp_5747;
        case BID_max:
        goto L_bidjmp_5754;
        case BID_min:
        goto L_bidjmp_5765;
        case BID_sqrt:
        goto L_bidjmp_5777;
        case BID_atan:
        goto L_bidjmp_5791;
        case BID_log:
        goto L_bidjmp_5797;
        case BID_dtor:
        goto L_bidjmp_5803;
        case BID_rtod:
        goto L_bidjmp_5804;
        case BID_iand:
        goto L_bidjmp_5805;
        case BID_ior:
        goto L_bidjmp_5806;
        case BID_ixor:
        goto L_bidjmp_5807;
        case BID_ishift:
        goto L_bidjmp_5808;
        case BID_icom:
        goto L_bidjmp_5809;
        case BID_copy:
        goto L_bidjmp_5811;
        case BID_list:
        goto L_bidjmp_5840;
        case BID_table:
        goto L_bidjmp_5864;
        case BID_read:
        goto L_bidjmp_5874;
        case BID_reads:
        goto L_bidjmp_5883;
        case BID_runerr:
        goto L_bidjmp_5894;
        case BID_stop:
        goto L_bidjmp_5895;
        case BID_any:
        goto L_bidjmp_5920;
        case BID_many:
        goto L_bidjmp_5938;
        case BID_upto:
        goto L_bidjmp_5958;
        case BID_tab:
        goto L_bidjmp_5978;
        case BID_move:
        goto L_bidjmp_5991;
        case BID_pos:
        goto L_bidjmp_6004;
        case BID_match:
        goto L_bidjmp_6012;
        case BID_bal:
        goto L_bidjmp_6033;
        case BID_find:
        goto L_bidjmp_6067;
        case BID_NONNULL:
        goto L_bidjmp_6081;
        case BID_ICN_CASE_EQ:
        goto L_bidjmp_6088;
        case BID_ICN_SWAP_TOP2:
        goto L_bidjmp_6104;
        case BID_ICN_NULL:
        goto L_bidjmp_6108;
        case BID_insert:
        goto L_bidjmp_6115;
        case BID_delete:
        goto L_bidjmp_6124;
        case BID_member:
        goto L_bidjmp_6132;
        case BID_key:
        goto L_bidjmp_6140;
        case BID___apply__:
        goto L_bidjmp_6149;
        case BID_push:
        goto L_bidjmp_6162;
        case BID_put:
        goto L_bidjmp_6182;
        case BID_get:
        goto L_bidjmp_6206;
        case BID_pop:
        goto L_bidjmp_6221;
        case BID_pull:
        goto L_bidjmp_6236;
        case BID_sort:
        goto L_bidjmp_6249;
        case BID_FIELD_GET:
        goto L_bidjmp_6322;
        case BID_FIELD_SET:
        goto L_bidjmp_6330;
        case BID_MAKELIST:
        goto L_bidjmp_6341;
        case BID_RECORD_REGISTER:
        goto L_bidjmp_6345;
        case BID_RECORD_MAKE:
        goto L_bidjmp_6351;
        case BID_open:
        goto L_bidjmp_6361;
        case BID_remove:
        goto L_bidjmp_6376;
        case BID_close:
        goto L_bidjmp_6381;
        case BID_flush:
        goto L_bidjmp_9999;
        case BID_serial:
        goto L_bidjmp_9998;
        case BID_errorclear:
        goto L_bidjmp_9997;
        case BID_IDENTICAL:
        goto L_bidjmp_6411;
        case BID_set:
        goto L_bidjmp_6423;
        case BID_ASGN:
        goto L_bidjmp_6439;
        case BID_name:
        goto L_bidjmp_6446;
        case BID_variable:
        goto L_bidjmp_6462;
        case BID_SNOx24NAME:
        goto L_bidjmp_6468;
        case BID_ARRAY:
        goto L_bidjmp_6469;
        case BID_TABLE:
        goto L_bidjmp_6479;
        case BID_ITEM:
        goto L_bidjmp_6484;
        case BID_PROTOTYPE:
        goto L_bidjmp_6490;
        case BID_CONVERT:
        goto L_bidjmp_6499;
        case BID_DATA:
        goto L_bidjmp_6521;
        case BID_SNOx24KWSET:
        goto L_bidjmp_6528;
        case BID_SNOx24STMT:
        goto L_bidjmp_6534;
        case BID_SNOx24MKEXPR:
        goto L_bidjmp_6540;
        case BID_SNOx24PBK:
        goto L_bidjmp_6545;
        case BID_SNOx24PBN:
        goto L_bidjmp_6546;
        case BID_SNOx24PB0:
        goto L_bidjmp_6547;
        case BID_SNOx24PBC:
        goto L_bidjmp_6548;
        case BID_SNOx24PCUR:
        goto L_bidjmp_6549;
        case BID_SNOx24PBALT:
        goto L_bidjmp_6550;
        case BID_SNOx24PARB:
        goto L_bidjmp_6551;
        case BID_SNOx24PFEN:
        goto L_bidjmp_6552;
        case BID_SNOx24PDEF:
        goto L_bidjmp_6553;
        case BID_SNOx24MKPAT:
        goto L_bidjmp_6554;
        case BID_OPSYN:
        goto L_bidjmp_6563;
        case BID_CODE:
        goto L_bidjmp_6567;
        case BID_EVAL:
        goto L_bidjmp_6572;
        case BID_VALUE:
        goto L_bidjmp_6581;
        case BID_SNOx24NRET:
        goto L_bidjmp_6587;
        case BID_SNOx24WANTNM:
        goto L_bidjmp_6588;
        case BID_APPLY:
        goto L_bidjmp_6589;
        default:
        break;
    }
    L_bidjmp_5076:
    ;
    if ((_bid == BID___rk_bool) && nargs == 1) {
        DESCR_t v = args[0];
        int t = 0;
        if (IS_FAIL_fn(v)) t = 0;
        else if (IS_INT_fn(v)) t = (v.i != 0);
        else if (IS_REAL_fn(v)) t = (v.r != 0.0);
        else if (v.v == DT_SNUL) t = 0;
        else if (v.v == DT_DATA) t = rk_match_is_nil(v) ? 0 : rk_qh_is(v) ? rk_qh_n(v) > 0 : 1;
        else if ((v.v == DT_A && v.arr) || (v.v == DT_T && v.tbl)) t = rk_agg_truth(v);
        else if (v.v == DT_FH) t = 1;
        else { const char *s = v.s ? v.s : ""; t = rk_typeobj_name(v) ? 0 : (s[0] != '\0'); }
        if (!t) { *out = FAILDESCR; return 1; }
        *out = v;
        return 1;
    }
    L_bidjmp_5087:
    ;
    if ((_bid == BID___rk_defined) && nargs == 1) {
        DESCR_t v = args[0];
        int t = (!IS_FAIL_fn(v)) && (v.v != DT_SNUL) && !rk_typeobj_name(v) && !rk_failure_is(v) && !rk_slip_undef(v);
        if (rk_failure_is(v)) rk_failure_handle(v);
        *out = t ? INTVAL(1) : FAILDESCR;
        return 1;
    }
    L_bidjmp_5090:
    ;
    if ((_bid == BID___rk_dor) && nargs == 2) {
        DESCR_t a = args[0];
        int def = (!IS_FAIL_fn(a)) && (a.v != DT_SNUL) && !rk_typeobj_name(a) && !rk_failure_is(a) && !rk_slip_undef(a);
        if (rk_failure_is(a)) rk_failure_handle(a);
        *out = def ? a : args[1];
        return 1;
    }
    L_bidjmp_5093:
    ;
    if ((_bid == BID___rk_bool_val) && nargs == 1) {
        DESCR_t v = args[0];
        int t = 0;
        if (IS_FAIL_fn(v)) t = 0;
        else if (IS_INT_fn(v)) t = (v.i != 0);
        else if (IS_REAL_fn(v)) t = (v.r != 0.0);
        else if (v.v == DT_SNUL) t = 0;
        else if (v.v == DT_DATA) t = rk_match_is_nil(v) ? 0 : rk_qh_is(v) ? rk_qh_n(v) > 0 : 1;
        else if ((v.v == DT_A && v.arr) || (v.v == DT_T && v.tbl)) t = rk_agg_truth(v);
        else if (v.v == DT_FH) t = 1;
        else { const char *s = v.s ? v.s : ""; t = rk_typeobj_name(v) ? 0 : (s[0] != '\0'); }
        *out = INTVAL(t);
        return 1;
    }
    L_bidjmp_5103:
    ;
    if ((_bid == BID___pas_chr) && pas_bi_chr(args, nargs, out)) return 1;
    L_bidjmp_5109:
    ;
    if ((_bid == BID___pas_chrlit) && pas_bi_chrlit(args, nargs, out)) return 1;
    L_bidjmp_5112:
    ;
    if ((_bid == BID___pas_enum_name) && nargs == 2) {
        long long ord = IS_INT_fn(args[0]) ? args[0].i : (IS_REAL_fn(args[0]) ? (long long)args[0].r : 0);
        const char *csv = VARVAL_fn(args[1]);
        if (!csv) csv = "";
        const char *p = csv;
        long long k = 0;
        while (*p && k < ord) { if (*p == ',') k++; p++; }
        const char *st = p;
        while (*p && *p != ',') p++;
        size_t L = (size_t)(p - st);
        char *s = (char *)rt_wsb_alloc(L + 1);
        memcpy(s, st, L);
        s[L] = '\0';
        *out = (DESCR_t){ .v = DT_S, .s = s, .slen = (uint32_t)L };
        return 1;
    }
    L_bidjmp_5121:
    ;
    if ((_bid == BID___pas_read_i) && (nargs == 0 || nargs == 1 || nargs == 2)) {
        *out = pas_read_number(stdin, nargs == 1 || (nargs == 2 && args[0].i != 0), nargs == 2 && args[1].i != 0, fn);
        return 1;
    }
    L_bidjmp_5124:
    ;
    if ((_bid == BID___pas_read_c) && nargs == 0) { int c = getchar(); if (c == EOF) c = 26; if (c == '\n') c = ' '; *out = INTVAL((long long)(unsigned char)c); return 1; }
    L_bidjmp_5127:
    ;
    if ((_bid == BID___pas_readln) && nargs == 0) { int c; while ((c = getchar()) != '\n' && c != EOF) (void)c; *out = NULVCL; return 1; }
    L_bidjmp_5130:
    ;
    if ((_bid == BID___pas_eof) && nargs == 0) { int c = getchar(); if (c == EOF) { *out = INTVAL(1); return 1; } ungetc(c, stdin); *out = INTVAL(0); return 1; }
    L_bidjmp_5135:
    ;
    if ((_bid == BID___pas_eoln) && nargs == 0) {
        int c = getchar();
        if (c == EOF) pas_file_err("6.6.6.5", "eoln(input) is activated while eof(input) is true", fn);
        if (c == '\n') { ungetc(c, stdin); *out = INTVAL(1); return 1; }
        ungetc(c, stdin);
        *out = INTVAL(0);
        return 1;
    }
    if ((_bid == BID___pas_read_i_f) && nargs == 2) {
        extern FILE *fh_get(int);
        FILE *f = IS_FH_fn(args[0]) ? fh_get((int)args[0].i) : NULL;
        if (!f) f = stdin;
        *out = pas_read_number(f, 1, 0, fn);
        return 1;
    }
    if ((_bid == BID___pas_read_i_f) && nargs == 3) {
        extern FILE *fh_get(int);
        FILE *f = IS_FH_fn(args[0]) ? fh_get((int)args[0].i) : NULL;
        if (!f) f = stdin;
        *out = pas_read_number(f, args[1].i != 0, args[2].i != 0, fn);
        return 1;
    }
    if (((_bid == BID___pas_read_i_f) || (_bid == BID___pas_read_c_f) || (_bid == BID___pas_readln_f) || (_bid == BID___pas_eof_f) || (_bid == BID___pas_eoln_f) || (_bid == BID___pas_getbufch_f)) &&
        nargs == 1) {
        extern FILE *fh_get(int);
        FILE *f = IS_FH_fn(args[0]) ? fh_get((int)args[0].i) : NULL;
        if (!f && IS_STR_fn(args[0]) && args[0].slen && VARVAL_fn(args[0]) && *VARVAL_fn(args[0])) {
            extern void core_runtime_error(int code, const char *msg);
            core_runtime_error(103, "file not open for reading (reset it first)");
        }
        if (!f) f = stdin;
        if ((_bid == BID___pas_read_i_f)) { *out = pas_read_number(f, 0, 0, fn); return 1; }
        if ((_bid == BID___pas_read_c_f)) { int c = fgetc(f); if (c == EOF) c = 26; if (c == '\n') c = ' '; *out = INTVAL((long long)(unsigned char)c); return 1; }
        if ((_bid == BID___pas_readln_f)) { int c; while ((c = fgetc(f)) != '\n' && c != EOF) (void)c; *out = NULVCL; return 1; }
        if ((_bid == BID___pas_eof_f)) {
            if (!IS_FH_fn(args[0])) pas_file_err("6.6.6.5", "eof(f) is activated while f is undefined", fn);
            int c = fgetc(f);
            if (c == EOF) { *out = INTVAL(1); return 1; }
            ungetc(c, f);
            *out = INTVAL(0);
            return 1;
        }
        if ((_bid == BID___pas_eoln_f)) {
            if (!IS_FH_fn(args[0])) pas_file_err("6.6.6.5", "eoln(f) is activated while f is undefined", fn);
            int c = fgetc(f);
            if (c == EOF) {
                int open_line = 0;
                clearerr(f);
                if (f != stdin && fseek(f, 0, SEEK_END) == 0) { long n = ftell(f); if (n > 0 && fseek(f, n - 1, SEEK_SET) == 0) { open_line = fgetc(f) != '\n'; fseek(f, 0, SEEK_END); } }
                if (!open_line) pas_file_err("6.6.6.5", "eoln(f) is activated while eof(f) is true", fn);
                *out = INTVAL(1);
                return 1;
            }
            if (c == '\n') { ungetc(c, f); *out = INTVAL(1); return 1; }
            ungetc(c, f);
            *out = INTVAL(0);
            return 1;
        }
        { int c = fgetc(f); if (c == EOF) { *out = INTVAL((long long)' '); return 1; } ungetc(c, f); *out = INTVAL((long long)(unsigned char)c); return 1; }
    }
    L_bidjmp_5149:
    ;
    if ((_bid == BID___pas_getbufch) && nargs == 0) {
        int c = getchar();
        if (c == EOF) { *out = INTVAL((long long)' '); return 1; }
        ungetc(c, stdin);
        *out = INTVAL((long long)(unsigned char)c);
        return 1;
    }
    L_bidjmp_5152:
    ;
    if ((_bid == BID___pas_trunc) && pas_bi_trunc(args, nargs, out)) return 1;
    L_bidjmp_5156:
    ;
    if ((_bid == BID___pas_abs) && pas_bi_abs(args, nargs, out)) return 1;
    if (nargs == 1 && ((_bid == BID___pas_sin)||(_bid == BID___pas_cos)||(_bid == BID___pas_exp)||(_bid == BID___pas_sqrt)||(_bid == BID___pas_ln)||(_bid == BID___pas_arctan))) {
        double d = IS_REAL_fn(args[0]) ? args[0].r : (double)(IS_INT_fn(args[0]) ? args[0].i : 0);
        double r;
        if ((_bid == BID___pas_sin)) r = sin(d);
        else if ((_bid == BID___pas_cos)) r = cos(d);
        else if ((_bid == BID___pas_exp)) r = exp(d);
        else if ((_bid == BID___pas_sqrt)) {
            if (d < 0) pas_file_err("6.6.6.2", "sqrt(x) of a negative x", NULL);
            r = sqrt(d);
        } else if ((_bid == BID___pas_ln)) {
            if (!(d > 0)) pas_file_err("6.6.6.2", "ln(x) of an x that is not greater than zero", NULL);
            r = log(d);
        } else r = atan(d);
        DESCR_t rv = {0};
        rv.v = DT_R;
        rv.r = r;
        *out = rv;
        return 1;
    }
    if ((_bid == BID___pas_round) && pas_bi_round(args, nargs, out)) return 1;
    if ((_bid == BID___pas_halt) && (nargs == 0 || nargs == 1)) { fflush(NULL); exit(nargs == 1 ? (int)(IS_INT_fn(args[0]) ? args[0].i : 0) : 0); }
    if ((_bid == BID___pas_frac) && nargs == 1) {
        double d = IS_REAL_fn(args[0]) ? args[0].r : (double)(IS_INT_fn(args[0]) ? args[0].i : 0);
        DESCR_t r = {0};
        r.v = DT_R;
        r.r = d - trunc(d);
        *out = r;
        return 1;
    }
    if ((_bid == BID___pas_assert) && nargs == 1) {
        long long cond = IS_INT_fn(args[0]) ? args[0].i : (IS_REAL_fn(args[0]) ? (args[0].r != 0.0) : 0);
        if (!cond) { fflush(NULL); fprintf(stderr, "Runtime error 227 at $0\n"); exit(227); }
        *out = NULVCL;
        return 1;
    }
    if ((_bid == BID_MATCHAT) && nargs == 0) { extern DESCR_t _MATCHAT_(DESCR_t *, int); *out = _MATCHAT_(args, nargs); return 1; }
    if ((_bid == BID___pas_mkdir) && nargs == 1) { char sb[256]; const char *d = to_cstring(args[0], sb, sizeof sb); mkdir(d, 0777); *out = NULVCL; return 1; }
    if ((_bid == BID___pas_rmdir) && nargs == 1) { char sb[256]; const char *d = to_cstring(args[0], sb, sizeof sb); rmdir(d); *out = NULVCL; return 1; }
    if ((_bid == BID___pas_swapendian) && nargs == 2) {
        unsigned long long v = (unsigned long long)(IS_INT_fn(args[0]) ? args[0].i : 0);
        long long w = IS_INT_fn(args[1]) ? args[1].i : 4;
        if (w < 1) w = 1;
        if (w > 8) w = 8;
        unsigned long long r = 0;
        for (long long b = 0; b < w; b++) r = (r << 8) | ((v >> (8 * b)) & 0xFFu);
        *out = INTVAL((long long)r);
        return 1;
    }
    if ((_bid == BID___pas_pos) && nargs == 2) {
        char nb[64];
        const char *needle = to_cstring(args[0], nb, sizeof nb);
        char hb[64];
        const char *hay = to_cstring(args[1], hb, sizeof hb);
        size_t nl_ = needle ? pas_slen(args[0], needle) : 0, hl_ = hay ? pas_slen(args[1], hay) : 0;
        long long at_ = 0;
        for (size_t k_ = 0; nl_ && hl_ >= nl_ && k_ + nl_ <= hl_ && !at_; k_++) if (!memcmp(hay + k_, needle, nl_)) at_ = (long long)k_ + 1;
        *out = INTVAL(at_);
        return 1;
    }
    L_bidjmp_5171:
    ;
    if ((_bid == BID___pas_writeln) || (_bid == BID___pas_write)) {
        int nl = (fn[6] == 'w' && fn[7] == 'r' && fn[8] == 'i' && fn[9] == 't' && fn[10] == 'e' && fn[11] == 'l');
        int _start = 0;
        FILE *_dest = stdout;
        if (nargs >= 1 && IS_FH_fn(args[0])) {
            extern FILE *fh_get(int);
            FILE *_fp = fh_get((int)args[0].i);
            if (_fp) _dest = _fp;
            _start = 2;
        } else if (nargs >= 2 && IS_INT_fn(args[1]) && args[1].i == -9) {
            extern void core_runtime_error(int code, const char *msg);
            core_runtime_error(103, "file not open for writing (rewrite it first)");
            _start = 2;
        }
        pas_wsink_t _k = { _dest, NULL, 0, 0 };
        pas_write_items(&_k, args, nargs, _start);
        if (nl) fputc('\n', _dest);
        *out = NULVCL;
        return 1;
    }
    L_bidjmp_5209:
    ;
    if ((_bid == BID_write) || (_bid == BID_writes)) {
        L_write_body_5209:
        ;
        int nl = (fn[5] == '\0');
        int start = 0;
        FILE *dest = stdout;
        int _wdi = 1;
        for (int _wi = start; _wi < nargs; _wi++) {
            DESCR_t av = args[_wi];
            if (IS_FAIL_fn(av)) { *out = FAILDESCR; return 1; }
            if (IS_FH_fn(av)) {
                if ((int)av.i >= 0 && (int)av.i < FH_N && g_fh[(int)av.i].closed) { core_icn_error(213, av); *out = FAILDESCR; return 1; }
                _wdi = (int)av.i;
                FILE *fp = fh_get((int)av.i);
                if (fp) { if (nl && _wi > 0) fputc('\n', dest); if ((nl && fp != stdout) || fp == stderr) fflush(stdout); dest = fp; }
                continue;
            }
            if (av.v == DT_SNUL) continue;
            if (_wdi == 1 && g_fh[1].closed) { core_icn_error(213, FAILDESCR); *out = FAILDESCR; return 1; }
            if (strict && av.v != DT_S && av.v != DT_I && av.v != DT_R && av.v != DT_BIG) { core_icn_error(109, av); *out = FAILDESCR; return 1; }
            if (IS_STR_fn(av) && !IS_CSET_fn(av)) {
                const char *_bs = VARVAL_fn(av);
                uint32_t _bn = av.slen ? av.slen : (_bs ? (uint32_t)strlen(_bs) : 0u);
                if (strict || (dest != stdout && dest != stderr) || (_bs && _bn && memchr(_bs, 0, _bn))) { if (_bs && _bn) fwrite(_bs, 1, _bn, dest); continue; }
            }
            out_write_descr(dest, av, nl);
        }
        if (nl && _wdi == 1 && g_fh[1].closed) { core_icn_error(213, FAILDESCR); *out = FAILDESCR; return 1; }
        if (nl) fputc('\n', dest);
        *out = nargs > start ? args[nargs-1] : (nargs > 0 ? args[0] : NULVCL);
        return 1;
    }
    L_bidjmp_5232:
    ;
    if ((_bid == BID_integer) && nargs == 1) {
        DESCR_t av = args[0];
        if (IS_INT_fn(av)) { *out = av; return 1; }
        if (IS_REAL_fn(av)) {
            if (!isfinite(av.r)) { *out = FAILDESCR; return 1; }
            if (fabs(av.r) >= 9223372036854775807.0) {
                extern DESCR_t rt_big_from_str(const char *);
                char _bb[400];
                snprintf(_bb, sizeof _bb, "%.0f", trunc(av.r));
                *out = rt_big_from_str(_bb);
                return 1;
            }
            *out = INTVAL((long long)av.r);
            return 1;
        }
        { extern int rt_big_is(DESCR_t); if (rt_big_is(av)) { *out = av; return 1; } }
        const char *s = VARVAL_fn(av);
        if (!s) { *out = FAILDESCR; return 1; }
        if (icn_numeric_is_c_hex(s)) { *out = FAILDESCR; return 1; }
        { long long rv; int rr = icon_radix_int(s, &rv); if (rr > 0) { *out = INTVAL(rv); return 1; } if (rr < 0) { *out = icon_radix_big(s); return 1; } }
        {
            extern DESCR_t rt_big_from_str(const char *);
            errno = 0;
            char *e2;
            strtoll(s, &e2, 10);
            if (e2 != s && (*e2 == 0 || *e2 == 32) && errno == ERANGE) { DESCR_t bg = rt_big_from_str(s); if (!IS_FAIL_fn(bg)) { *out = bg; return 1; } }
        }
        char *end;
        long long iv = strtoll(s, &end, 10);
        if (end != s && (*end=='\0'||*end==' ')) { *out = INTVAL(iv); return 1; }
        double rv = strtod(s, &end);
        if (end != s && (*end=='\0'||*end==' ')) {
            if (!isfinite(rv)) { *out = FAILDESCR; return 1; }
            if (rv >= 9223372036854775808.0 || rv < -9223372036854775808.0) {
                extern DESCR_t rt_big_from_str(const char *);
                char _rb[400];
                snprintf(_rb, sizeof _rb, "%.0f", rv);
                *out = rt_big_from_str(_rb);
                return 1;
            }
            *out = INTVAL((long long)rv);
            return 1;
        }
        *out = FAILDESCR;
        return 1;
    }
    L_bidjmp_5264:
    ;
    if ((_bid == BID_real) && nargs == 1) {
        DESCR_t av = args[0];
        if (IS_REAL_fn(av)) { *out = av; return 1; }
        if (IS_INT_fn(av)) { *out = REALVAL((double)av.i); return 1; }
        const char *s = VARVAL_fn(av);
        if (!s) { *out = FAILDESCR; return 1; }
        if (icn_numeric_is_c_hex(s)) { *out = FAILDESCR; return 1; }
        { long long rv; int rr = icon_radix_int(s, &rv); if (rr > 0) { *out = REALVAL((double)rv); return 1; } }
        char *end;
        errno = 0;
        double rv = strtod(s, &end);
        if (end != s && (*end=='\0'||*end==' ')) { if (!isfinite(rv) || (errno == ERANGE && !isfinite(rv))) { *out = FAILDESCR; return 1; } *out = REALVAL(rv); return 1; }
        *out = FAILDESCR;
        return 1;
    }
    L_bidjmp_5273:
    ;
    if ((_bid == BID_string) && nargs == 1) {
        DESCR_t av = args[0];
        if (av.v == DT_SNUL) { *out = FAILDESCR; return 1; }
        if (IS_CSET_fn(av)) { *out = rtx_try_rt_str_coerce(av); return 1; }
        if (IS_STR_fn(av)) { *out = av; return 1; }
        if (av.v == DT_BIG) { extern char *rt_big_str(DESCR_t); *out = STRVAL(rt_big_str(av)); return 1; }
        if (IS_INT_fn(av)) return icn_bi_string_of_int(av, out);
        char *buf = rt_wsb_alloc(64);
        if (IS_REAL_fn(av)) { icon_real_str(av.r,buf,64); } else { *out = FAILDESCR; return 1; }
        *out = STRVAL(buf);
        return 1;
    }
    L_bidjmp_5283:
    ;
    if ((_bid == BID_numeric) && nargs == 1) {
        DESCR_t av = args[0];
        if (IS_INT_fn(av)||IS_REAL_fn(av)) { *out = av; return 1; }
        { extern int rt_big_is(DESCR_t); if (rt_big_is(av)) { *out = av; return 1; } }
        const char *s = VARVAL_fn(av);
        if (!s||!*s) { *out = FAILDESCR; return 1; }
        {
            const char *p = s;
            while (*p == ' ' || *p == '\t') p++;
            int neg = 0;
            if (*p == '+') p++;
            else if (*p == '-') { neg = 1; p++; }
            int base = 0;
            const char *bstart = p;
            while (*p >= '0' && *p <= '9') { base = base * 10 + (*p - '0'); p++; }
            if (p > bstart && (*p == 'r' || *p == 'R') && base >= 2 && base <= 36) {
                p++;
                long long v = 0;
                while (*p) {
                    int d = -1;
                    if (*p >= '0' && *p <= '9') d = *p - '0';
                    else if (*p >= 'a' && *p <= 'z') d = *p - 'a' + 10;
                    else if (*p >= 'A' && *p <= 'Z') d = *p - 'A' + 10;
                    if (d < 0 || d >= base) break;
                    v = v * base + d;
                    p++;
                }
                while (*p == ' ' || *p == '\t') p++;
                if (*p == '\0') { *out = INTVAL(neg ? -v : v); return 1; }
            }
        }
        errno = 0;
        char *end;
        long long iv = strtoll(s, &end, 10);
        if (end != s && icn_numeric_tail_is_blank(end) && errno == ERANGE) {
            extern DESCR_t rt_big_from_str(const char *);
            const char *b0 = s;
            while (*b0 == ' ' || *b0 == '\t') b0++;
            size_t bl = (size_t)(end - b0);
            char *bs = rt_wsb_alloc(bl + 1);
            memcpy(bs, b0, bl);
            bs[bl] = '\0';
            DESCR_t bg = rt_big_from_str(bs);
            if (!IS_FAIL_fn(bg)) { *out = bg; return 1; }
        }
        if (end != s && icn_numeric_tail_is_blank(end)) { *out = INTVAL(iv); return 1; }
        if (icn_numeric_is_c_hex(s)) { *out = FAILDESCR; return 1; }
        double rv = strtod(s, &end);
        if (end != s && icn_numeric_tail_is_blank(end)) { if (!isfinite(rv)) { *out = FAILDESCR; return 1; } *out = REALVAL(rv); return 1; }
        *out = FAILDESCR;
        return 1;
    }
    if (((_bid == BID_char) || (_bid == BID_chr)) && nargs == 1) {
        DESCR_t av = args[0];
        const char *raw = VARVAL_fn(av);
        long long rv;
        int n = (int)(IS_INT_fn(av) ? av.i : (raw && icon_radix_int(raw, &rv)) ? rv : (long long)strtol(raw?raw:"0",NULL,10));
        char *buf = rt_wsb_alloc(2);
        buf[0]=(char)(n&0xFF);
        buf[1]='\0';
        *out = BSTRVAL(buf, 1);
        return 1;
    }
    L_bidjmp_5320:
    ;
    if ((_bid == BID_cset) && nargs == 1) {
        DESCR_t av = args[0];
        if (IS_CSET_fn(av)) { *out = av; return 1; }
        char _cbuf[64];
        const char *raw;
        int rawlen;
        if (IS_INT_fn(av)) {
            snprintf(_cbuf,sizeof _cbuf,"%lld",(long long)av.i);
            raw=_cbuf;
            rawlen=(int)strlen(_cbuf);
        } else if (IS_REAL_fn(av)) {
            icon_real_str(av.r,_cbuf,sizeof _cbuf);
            raw=_cbuf;
            rawlen=(int)strlen(_cbuf);
        } else {
            raw = VARVAL_fn(av);
            if (!raw) raw = "";
            rawlen=(int)descr_slen(av);
        }
        *out = CSETVAL(cset_canonical(raw, rawlen));
        return 1;
    }
    L_bidjmp_5330:
    ;
    if ((_bid == BID_ord) && nargs == 1) {
        DESCR_t av = args[0];
        char _ob[64];
        const char *s = IS_INT_fn(av) ? (snprintf(_ob, sizeof _ob, "%lld", (long long)av.i), _ob) : IS_REAL_fn(av) ? icon_real_str(av.r, _ob, sizeof _ob) : VARVAL_fn(av);
        if (!s || !((IS_INT_fn(av) || IS_REAL_fn(av)) ? strlen(s) : descr_slen(av))) { *out = FAILDESCR; return 1; }
        *out = INTVAL((unsigned char)s[0]);
        return 1;
    }
    if (((_bid == BID_type) || (_bid == BID_DATATYPE)) && nargs == 1) return bn_type_datatype(fn, args, nargs, out);
    L_bidjmp_5336:
    ;
    if ((_bid == BID_image) && nargs == 0) { *out = STRVAL("&null"); return 1; }
    L_bidjmp_5339:
    ;
    if ((_bid == BID_args) && nargs == 1) {
        DESCR_t a = args[0];
        if (IS_PROCVAL_fn(a) && icn_opproc_arity(a.s) > 0) { *out = INTVAL(icn_opproc_arity(a.s)); return 1; }
        if (IS_PROCVAL_fn(a)) {
            static const struct {
                const char *nm;
                int np;
            } _bt[] = { {"push",-2},{"put",-2},{"insert",-2},{"delete",-2}, {"pop",1},{"get",1},{"pull",1},{"bal",3},{"find",3},{"upto",3}, {"any",3},{"many",3},{"match",3},{"tab",1},{"move", 1},
                {"pos",1}, {"write",-1},{"writes",-1},{"writef",-1}, {"read",0},{"reads",1},{"close",1},{"open",2},{"flush",1}, {"image",1},{"type",1},{"copy",1},{"sort",2},{"sortf",2}, {"reverse",1},
                {"size",1},{"left",3},{"right",3},{"center",3}, {"string",1},{"integer",1},{"real",1},{"char",1},{"ord",1}, {"trim",2},{"map",3},{"repl",2},{"cset",1},{"list",2}, {"table",1},{"set",
                1},{"key",1},{"member",2}, {"proc",2},{"args",1},{"name",1},{"variable",1}, {"abs",1},{"sqrt",1},{"sin",1},{"cos",1},{"atan",2}, {"exp",1},{"log",2},{"max", -1},{"min",-1}, {"iand",2},
                {"ior",2},{"ixor",2},{"icom",1},{"ishift",2}, {"numeric",1},{"entab",-1},{"detab",-1},{"seq",2}, {"collect",2},{"display",2},{"runerr",2}, {"errorclear",0}, {"function",0},{"serial",
                1},{"system",1},{"exit",1}, {"chdir",1},{"delay",1},{"getch",0},{"getche",0},{"kbhit",0},{"loadfunc",2}, {NULL,0}};
            int np = rt_proc_nparams(a.s);
            int _ia = (np < 0 && a.s) ? icn_builtin_arity(a.s) : ICN_ARITY_UNKNOWN;
            if (_ia != ICN_ARITY_UNKNOWN) np = _ia;
            if (np < 0 && _ia == ICN_ARITY_UNKNOWN && a.s) for (int _bi=0;_bt[_bi].nm;_bi++) if (!strcmp(_bt[_bi].nm,a.s)){ np=_bt[_bi].np; break; }
            if (np < 0 && a.s && !strcmp(a.s, "main")) {
                np = 0;
                for (int i = 0; i < g_stage2.proc_count; i++) if (g_stage2.proc_table[i].name && !strcmp(g_stage2.proc_table[i].name, "main")) { np = g_stage2.proc_table[i].nparams; break; }
            }
            if (np < 0 && a.s) { DatType *_rct = dat_find_type(a.s); if (_rct) { *out = INTVAL((int64_t)_rct->nfields); return 1; } }
            if (np >= 0 && a.s) { extern int rt_proc_is_variadic(const char *); if (rt_proc_is_variadic(a.s)) np = -np; }
            if (np >= 0) { *out = INTVAL(np); return 1; }
            *out = INTVAL(np < -1 ? np : -1);
            return 1;
        }
        if (a.v == DT_E) {
            for (int i=0;i<g_stage2.proc_count;i++) {
                if (g_stage2.proc_table[i].entry_pc == (int)a.i) { *out = INTVAL(g_stage2.proc_table[i].nparams <= 0 ? -2 : g_stage2.proc_table[i].nparams); return 1; }
            }
            *out = INTVAL(-2);
            return 1;
        }
        if (strict) { core_icn_error(106, a); *out = FAILDESCR; return 1; }
        if (IS_STR_fn(a)) { *out = INTVAL(-2); return 1; }
        *out = FAILDESCR;
        return 1;
    }
    L_bidjmp_5385:
    ;
    if ((_bid == BID_proc) && (nargs == 2 || nargs == 1)) {
        if (nargs == 2 && args[0].v == DT_CO) {
            extern void *g_scrip_co_current;
            extern void *scrip_co_gc_root(void);
            extern int core_icn_act_top(void);
            extern const char *core_icn_act_name(int);
            extern DESCR_t rt_proc_value(const char *);
            long lvl = (long)to_int(args[1]);
            int top = core_icn_act_top();
            const char *pn = (args[0].p == (g_scrip_co_current ? g_scrip_co_current : scrip_co_gc_root()) && lvl >= 0 && lvl < top) ? core_icn_act_name(top - (int)lvl) : (const char *)0;
            *out = pn ? rt_proc_value(pn) : FAILDESCR;
            return 1;
        }
        if (args[0].v == DT_E) { *out = args[0]; return 1; }
        if (args[0].v != DT_S && args[0].v != DT_SNUL) { *out = FAILDESCR; return 1; }
        const char *pname = VARVAL_fn(args[0]);
        int arity = (nargs >= 2) ? (int)to_int(args[1]) : -1;
        if (!pname) { *out = FAILDESCR; return 1; }
        int _icn_field_only = 0;
        {
            extern int rt_dat_field_of_any(const char *);
            _icn_field_only = strict && rt_dat_field_of_any(pname) && !dat_find_type(pname) && !icn_builtin_is_known(pname) && icn_builtin_arity(pname) == ICN_ARITY_UNKNOWN;
        }
        if (!_icn_field_only && arity == 0 && (icn_builtin_is_known(pname) || (strict ? dat_find_type(pname) != 0 : rt_builtin_is_known(pname)) || icn_builtin_arity(pname) != ICN_ARITY_UNKNOWN)) {
            *out = PROCVAL_BUILTIN(rt_heap_strdup_c(pname));
            return 1;
        }
        if (strict && arity == 0) { *out = FAILDESCR; return 1; }
        if (arity < 0) { DESCR_t gv = NV_GET_fn(pname); if (IS_PROCVAL_fn(gv) && gv.s) { *out = gv; return 1; } }
        for (int i = 0; i < g_stage2.proc_count; i++) {
            if (g_stage2.proc_table[i].name && strcmp(g_stage2.proc_table[i].name, pname) == 0) {
                if (arity < 0 || g_stage2.proc_table[i].nparams == arity || g_stage2.proc_table[i].nparams <= 0) {
                    extern DESCR_t rt_proc_value(const char *);
                    *out = rt_proc_value(g_stage2.proc_table[i].name);
                    return 1;
                }
            }
        }
        if (strict && (rt_proc_is_registered(pname) || !strcmp(pname, "main")) && (arity < 0 || rt_proc_nparams(pname) == arity || rt_proc_nparams(pname) <= 0)) {
            extern DESCR_t rt_proc_value(const char *);
            *out = rt_proc_value(rt_heap_strdup_c(pname));
            return 1;
        }
        {
            extern int rt_proc_is_registered(const char *name);
            extern int rt_proc_nparams(const char *name);
            if (rt_proc_is_registered(pname)) {
                int np = rt_proc_nparams(pname);
                if (arity < 0 || np == arity || np <= 0) { extern DESCR_t rt_proc_value(const char *); *out = rt_proc_value(rt_heap_strdup_c(pname)); return 1; }
            }
        }
        if (!_icn_field_only && (icn_builtin_is_known(pname) || (strict ? dat_find_type(pname) != 0 : rt_builtin_is_known(pname)) || icn_builtin_arity(pname) != ICN_ARITY_UNKNOWN)) {
            DESCR_t bv = {0};
            bv.v = DT_E;
            bv.slen = 0xFFFFFFFEu;
            bv.s = rt_heap_strdup_c(pname);
            *out = bv;
            return 1;
        }
        { const char *op = icn_opproc_find(pname, arity < 0 ? 1 : arity); if (op) { DESCR_t bv = {0}; bv.v = DT_E; bv.slen = 0xFFFFFFFEu; bv.s = (char *)op; *out = bv; return 1; } }
        *out = FAILDESCR;
        return 1;
    }
    if ((_bid == BID_image) && nargs == 1) {
        DESCR_t av = args[0];
        if (IS_FAIL_fn(av)) { *out = FAILDESCR; return 1; }
        char *buf = rt_heap_alloc_c(256);
        if (av.v == DT_SNUL) { *out = STRVAL("&null"); return 1; }
        if (av.v == DT_EXTL) { extern DESCR_t rt_extl_image(DESCR_t); *out = rt_extl_image(av); return 1; }
        if (av.v == DT_CO) {
            extern long scrip_coexpr_serial_of(void *);
            extern long scrip_coexpr_activations_of(void *);
            snprintf(buf,128,"co-expression_%ld(%ld)", scrip_coexpr_serial_of(av.p), scrip_coexpr_activations_of(av.p));
            *out = STRVAL(buf);
            return 1;
        }
        if (av.v == DT_P && av.p) { extern int pat_image_str(void *, DESCR_t *); if (pat_image_str(av.p, out)) return 1; }
        if (av.v == DT_E) {
            const char *nm = procval_name(av);
            if (!nm) nm = "?";
            char unm[64];
            if (!strncmp(nm, "ICN$U", 5)) { size_t k = 0; for (const char * q = nm + 5; *q && k + 1 < sizeof unm; q++) unm[k++] = (char) tolower((unsigned char) *q); unm[k] = '\0'; nm = unm; }
            snprintf(buf,256,
                dat_find_type(nm) ? "record constructor %s" : (!IS_PROCVAL_EXTERNAL_fn(av) && !IS_PROCVAL_BUILTIN_fn(av) && (rt_proc_is_registered(nm) || !strcmp(nm, "main"))) ? "procedure %s" :
                "function %s", nm);
            *out = STRVAL(buf);
            return 1;
        }
        if (IS_FH_fn(av)) {
            int idx = (int)av.i;
            if (idx == 0) { snprintf(buf,256,"&input"); *out = STRVAL(buf); return 1; }
            if (idx == 1) { snprintf(buf,256,"&output"); *out = STRVAL(buf); return 1; }
            if (idx == 2) { snprintf(buf,256,"&errout"); *out = STRVAL(buf); return 1; }
            if (idx >= 0 && idx < FH_N && g_fh[idx].name) { snprintf(buf,256,"file(%s)",g_fh[idx].name); *out = STRVAL(buf); return 1; }
            snprintf(buf,256,"file(?)");
            *out = STRVAL(buf);
            return 1;
        }
        if (IS_INT_fn(av)) { snprintf(buf,256,"%lld",(long long)av.i); *out = STRVAL(buf); return 1; }
        if (av.v == DT_BIG) { extern char *rt_big_image_str(DESCR_t); *out = STRVAL(rt_big_image_str(av)); return 1; }
        if (IS_REAL_fn(av)) { icon_real_str(av.r,buf,128); *out = STRVAL(buf); return 1; }
        if (av.v==DT_T) { snprintf(buf,128,"%s_%ld(%d)", (av.tbl && av.tbl->is_set) ? "set" : "table", av.tbl?av.tbl->id:0, av.tbl?av.tbl->size:0); *out = STRVAL(buf); return 1; }
        if (IS_DATA_INST_fn(av) && av.u) {
            const char *tname = av.u->type ? av.u->type->name : "record";
            if (strcmp(tname,"list")==0) {
                int cnt = (av.u->type && av.u->type->nfields>=2 && av.u->fields) ? (int)av.u->fields[1].i : 0;
                long id = rt_record_image_id(av.u);
                snprintf(buf,128,"list_%ld(%d)",id,cnt);
                *out = STRVAL(buf);
                return 1;
            }
            { int nf = (av.u->type ? av.u->type->nfields : 0); long id = rt_record_image_id(av.u); snprintf(buf,256,"record %s_%ld(%d)",tname,id,nf); *out = STRVAL(buf); return 1; }
        }
        if (av.v==DT_DATA) { *out = STRVAL("record"); return 1; }
        if (av.v==DT_E) {
            for (int i=0;i<g_stage2.proc_count;i++) if (g_stage2.proc_table[i].entry_pc==(int)av.i) { snprintf(buf,128,"procedure %s",g_stage2.proc_table[i].name); *out=STRVAL(buf); return 1; }
            snprintf(buf,128,"procedure");
            *out=STRVAL(buf);
            return 1;
        }
        if (IS_CSET_fn(av)) {
            const char *cs = av.s ? av.s : "";
            int cslen = (int)descr_slen(av);
            const char *kname = kw_cset_name(cs, cslen);
            if (kname) { *out = STRVAL(kname); return 1; }
            char *outs = rt_heap_alloc_c(cslen * 4 + 3);
            int o = 0;
            outs[o++] = '\'';
            for (int i = 0; i < cslen; i++) {
                unsigned char c = (unsigned char)cs[i];
                if (c == '\'') {
                    outs[o++] = '\\';
                    outs[o++] = '\'';
                } else if (c == '\\') {
                    outs[o++] = '\\';
                    outs[o++] = '\\';
                } else if (c == '\n') {
                    outs[o++] = '\\';
                    outs[o++] = 'n';
                } else if (c == '\t') {
                    outs[o++] = '\\';
                    outs[o++] = 't';
                } else if (c == '\r') {
                    outs[o++] = '\\';
                    outs[o++] = 'r';
                } else if (c == '\b') {
                    outs[o++] = '\\';
                    outs[o++] = 'b';
                } else if (c == '\v') {
                    outs[o++] = '\\';
                    outs[o++] = 'v';
                } else if (c == '\f') {
                    outs[o++] = '\\';
                    outs[o++] = 'f';
                } else if (c == 0x1b) {
                    outs[o++] = '\\';
                    outs[o++] = 'e';
                } else if (c == 0x7f) {
                    outs[o++] = '\\';
                    outs[o++] = 'd';
                } else if (c < 0x20 || c >= 0x7f) {
                    o += snprintf(outs+o, 5, "\\x%02x", c);
                } else outs[o++] = (char)c;
            }
            outs[o++] = '\'';
            outs[o] = '\0';
            *out = STRVAL(outs);
            return 1;
        }
        if (IS_STR_fn(av) && av.s) { }
        const char *s=VARVAL_fn(av);
        if (!s) s = "";
        int sl = (av.slen && av.slen != 0xFFFFFFFFu) ? (int)av.slen : (int)strlen(s);
        char *outs = rt_heap_alloc_c(sl*4 + 3);
        int o = 0;
        outs[o++] = '"';
        for (int i = 0; i < sl; i++) {
            unsigned char c = (unsigned char)s[i];
            switch (c) {
                case '"':
                outs[o++]='\\';
                outs[o++]='"';
                break;
                case '\\':
                outs[o++]='\\';
                outs[o++]='\\';
                break;
                case '\n':
                outs[o++]='\\';
                outs[o++]='n';
                break;
                case '\t':
                outs[o++]='\\';
                outs[o++]='t';
                break;
                case '\r':
                outs[o++]='\\';
                outs[o++]='r';
                break;
                case '\b':
                outs[o++]='\\';
                outs[o++]='b';
                break;
                case '\v':
                outs[o++]='\\';
                outs[o++]='v';
                break;
                case '\f':
                outs[o++]='\\';
                outs[o++]='f';
                break;
                case 0x1b:
                outs[o++]='\\';
                outs[o++]='e';
                break;
                case 0x7f:
                outs[o++]='\\';
                outs[o++]='d';
                break;
                default:
                if (c < 0x20 || c >= 0x7f) { o += snprintf(outs+o, 5, "\\x%02x", c); } else { outs[o++] = (char)c; }
            }
        }
        outs[o++] = '"';
        outs[o] = '\0';
        *out = STRVAL(outs);
        return 1;
    }
    if ((_bid == BID_image) && nargs >= 2) {
        DESCR_t av = args[0];
        if (0 && IS_STR_fn(av) && av.s) { }
        DESCR_t one_out = FAILDESCR;
        if (try_call_builtin_by_name("image", args, 1, &one_out)) { *out = one_out; return 1; }
        *out = FAILDESCR;
        return 1;
    }
    L_bidjmp_5524:
    ;
    if ((_bid == BID_repl) && nargs == 2) {
        const char *s=VARVAL_fn(args[0]);
        if(!s)s="";
        extern int64_t core_icn_to_int_d(DESCR_t);
        int n=(int)core_icn_to_int_d(args[1]);
        if(n<0){ core_icn_error(205, args[1]); n=0; }
        int sl=icn_true_len(args[0], s);
        char *buf=rt_wsb_alloc(sl*n+1);
        buf[0]='\0';
        for(int i=0;i<n;i++) memcpy(buf+i*sl,s,sl);
        buf[sl*n]='\0';
        *out = BSTRVAL(buf, sl*n);
        return 1;
    }
    L_bidjmp_5531:
    ;
    if ((_bid == BID_reverse) && nargs >= 1 && icn_arg_is_list(args[0])) return bn_uni_list_reverse(args, nargs, out);
    if ((_bid == BID_reverse) && nargs >= 1) {
        const char *s=VARVAL_fn(args[0]);
        if(!s)s="";
        int sl=icn_true_len(args[0], s);
        char *buf=rt_wsb_alloc(sl+1);
        for(int i=0;i<sl;i++) buf[i]=s[sl-1-i];
        buf[sl]='\0';
        *out = BSTRVAL(buf, sl);
        return 1;
    }
    L_bidjmp_5537:
    ;
    if ((_bid == BID_map) && nargs >= 1 && nargs <= 3) {
        static const char *const UCASE = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        static const char *const LCASE = "abcdefghijklmnopqrstuvwxyz";
        const char *s=VARVAL_fn(args[0]);
        if(!s)s="";
        const char *from = UCASE, *to = LCASE;
        if (nargs >= 2) { DESCR_t fv=args[1]; if (fv.v != DT_SNUL) { const char *fs = VARVAL_fn(fv); if (fs) from = fs; } }
        if (nargs >= 3) { DESCR_t tv=args[2]; if (tv.v != DT_SNUL) { const char *ts = VARVAL_fn(tv); if (ts) to = ts; } }
        int sl=icn_true_len(args[0], s);
        char *buf=rt_wsb_alloc(sl+1);
        int fl=icn_true_len(nargs >= 2 ? args[1] : NULVCL, from), tl=icn_true_len(nargs >= 3 ? args[2] : NULVCL, to);
        typedef struct { int fl, tl; char mc_from[256], mc_to[256]; unsigned char mc_tbl[256]; } map_cache_t;
        static map_cache_t map_cache[8];
        static int map_cache_next = 0;
        static int map_cache_init = 0;
        if (!map_cache_init) { for (int e = 0; e < 8; e++) map_cache[e].fl = map_cache[e].tl = -1; map_cache_init = 1; }
        const int tn = (tl < fl) ? tl : fl;
        map_cache_t *mc = 0;
        if (fl <= 256 && tl <= 256) {
            for (int e = 0; e < 8; e++) {
                map_cache_t *ce = &map_cache[e];
                if (ce->fl == fl && ce->tl == tl && !memcmp(ce->mc_from, from, (size_t)fl) && !memcmp(ce->mc_to, to, (size_t)tn)) { mc = ce; break; }
            }
            if (!mc) {
                mc = &map_cache[map_cache_next];
                map_cache_next = (map_cache_next + 1) & 7;
                for (int c = 0; c < 256; c++) mc->mc_tbl[c] = (unsigned char)c;
                for (int j = 0; j < fl; j++) mc->mc_tbl[(unsigned char)from[j]] = (unsigned char)((j < tl) ? to[j] : from[j]);
                memcpy(mc->mc_from, from, (size_t)fl);
                memcpy(mc->mc_to, to, (size_t)tn);
                mc->fl = fl;
                mc->tl = tl;
            }
            for (int i=0;i<sl;i++) buf[i] = (char)mc->mc_tbl[(unsigned char)s[i]];
        } else {
            for (int i=0;i<sl;i++) { char c=s[i]; int hit=0; for (int j=fl-1;j>=0;j--) { if (from[j]==c) { buf[i] = (j<tl) ? to[j] : c; hit=1; break; } } if (!hit) buf[i]=c; }
        }
        buf[sl]='\0';
        *out = BSTRVAL(buf, sl);
        return 1;
    }
    L_bidjmp_5567:
    ;
    if ((_bid == BID_trim) && nargs >= 1) return bn_icn_trim(args, nargs, out);
    L_bidjmp_5576:
    ;
    if ((_bid == BID_getenv) && nargs == 1) {
        const char *name = VARVAL_fn(args[0]);
        if (!name) { *out = FAILDESCR; return 1; }
        const char *val = getenv(name);
        if (!val) { *out = FAILDESCR; return 1; }
        size_t vl = strlen(val);
        char *buf = rt_wsb_alloc(vl+1);
        memcpy(buf, val, vl);
        buf[vl] = '\0';
        *out = STRVAL(buf);
        return 1;
    }
    L_bidjmp_5584:
    ;
    if ((_bid == BID_collect) && nargs <= 2) { extern long rt_gc_collect(void); rt_gc_collect(); *out = NULVCL; return 1; }
    L_bidjmp_5585:
    ;
    if ((_bid == BID_left) && nargs >= 1) return bn_icn_left(args, nargs, out);
    L_bidjmp_5613:
    ;
    if ((_bid == BID_right) && nargs >= 1) return bn_icn_right(args, nargs, out);
    L_bidjmp_5639:
    ;
    if ((_bid == BID_center) && nargs >= 1) return bn_icn_center(args, nargs, out);
    if (((_bid == BID_detab) || (_bid == BID_entab)) && nargs == 0) { core_icn_error(103, NULVCL); *out = FAILDESCR; return 1; }
    L_bidjmp_5671:
    ;
    if ((_bid == BID_detab) && nargs == 0) { core_icn_error(103, NULVCL); *out = FAILDESCR; return 1; }
    if ((_bid == BID_detab) && nargs >= 1) {
        if (args[0].v == DT_I || args[0].v == DT_R) { *out = FAILDESCR; return 1; }
        if (args[0].v == DT_A || args[0].v == DT_T || args[0].v == DT_DATA) { core_icn_error(103, args[0]); *out = FAILDESCR; return 1; }
        const char *s = VARVAL_fn(args[0]);
        if (!s) s = "";
        int stops[nargs], nstops = 0;
        for (int j = 1; j < nargs; j++) {
            if (IS_FAIL_fn(args[j]) || args[j].v == DT_SNUL) continue;
            if (!IS_INT_fn(args[j]) && !IS_REAL_fn(args[j])) {
                int _cok = 0;
                if (args[j].v == DT_S && args[j].s) {
                    const char *_p = args[j].s;
                    while (*_p == ' ') _p++;
                    if (*_p == '+' || *_p == '-') _p++;
                    int _d = 0;
                    while (*_p >= '0' && *_p <= '9') { _p++; _d = 1; }
                    while (*_p == ' ') _p++;
                    _cok = _d && !*_p;
                }
                if (!_cok) { core_icn_error(101, args[j]); *out = FAILDESCR; return 1; }
            }
            { int _ns = (int)to_int(args[j]); if (nstops > 0 && _ns <= stops[nstops-1]) { core_icn_error(210, args[j]); *out = FAILDESCR; return 1; } stops[nstops++] = _ns; }
        }
        if (nstops == 0) { stops[0] = 9; nstops = 1; }
        int gap = (nstops >= 2) ? stops[nstops-1] - stops[nstops-2] : stops[0] - 1;
        if (gap < 1) gap = 1;
        int cap = 4096;
        char *buf = rt_wsb_alloc(cap);
        int bi = 0, col = 0;
        int slen0 = icn_true_len(args[0], s);
        for (int i = 0; i < slen0; i++) {
            if (s[i] == '\t') {
                int next = -1;
                for (int k = 0; k < nstops; k++) if (stops[k] > col+1) { next=stops[k]; break; }
                if (next < 0) { int base = stops[nstops-1]; int beyond = col + 1 - base; next = base + ((beyond / gap) + 1) * gap; }
                int sp = next - (col+1);
                while (sp-- > 0) { if (bi>=cap-1){ cap*=2; buf=rt_wsb_realloc(buf,cap); } buf[bi++]=' '; col++; }
            } else if (s[i] == '\b') {
                if (col > 0) col--;
                if (bi>=cap-1){ cap*=2; buf=rt_wsb_realloc(buf,cap); }
                buf[bi++]=s[i];
            } else if (s[i] == '\n' || s[i] == '\r') {
                col = 0;
                if (bi>=cap-1){ cap*=2; buf=rt_wsb_realloc(buf,cap); }
                buf[bi++]=s[i];
            } else {
                if (bi>=cap-1){ cap*=2; buf=rt_wsb_realloc(buf,cap); }
                buf[bi++]=s[i];
                if (isprint((unsigned char)s[i])) col++;
            }
        }
        buf[bi]='\0';
        *out=BSTRVAL(buf, bi);
        return 1;
    }
    L_bidjmp_5704:
    ;
    if ((_bid == BID_entab) && nargs == 0) { core_icn_error(103, NULVCL); *out = FAILDESCR; return 1; }
    if ((_bid == BID_entab) && nargs >= 1) {
        if (args[0].v == DT_I || args[0].v == DT_R) { *out = FAILDESCR; return 1; }
        if (args[0].v == DT_A || args[0].v == DT_T || args[0].v == DT_DATA) { core_icn_error(103, args[0]); *out = FAILDESCR; return 1; }
        const char *s = VARVAL_fn(args[0]);
        if (!s) s = "";
        int stops[nargs], nstops = 0;
        for (int j = 1; j < nargs; j++) {
            if (IS_FAIL_fn(args[j]) || args[j].v == DT_SNUL) continue;
            if (!IS_INT_fn(args[j]) && !IS_REAL_fn(args[j])) {
                int _cok = 0;
                if (args[j].v == DT_S && args[j].s) {
                    const char *_p = args[j].s;
                    while (*_p == ' ') _p++;
                    if (*_p == '+' || *_p == '-') _p++;
                    int _d = 0;
                    while (*_p >= '0' && *_p <= '9') { _p++; _d = 1; }
                    while (*_p == ' ') _p++;
                    _cok = _d && !*_p;
                }
                if (!_cok) { core_icn_error(101, args[j]); *out = FAILDESCR; return 1; }
            }
            { int _ns = (int)to_int(args[j]); if (nstops > 0 && _ns <= stops[nstops-1]) { core_icn_error(210, args[j]); *out = FAILDESCR; return 1; } stops[nstops++] = _ns; }
        }
        if (nstops == 0) { stops[0] = 9; nstops = 1; }
        int gap = (nstops >= 2) ? stops[nstops-1] - stops[nstops-2] : stops[0] - 1;
        if (gap < 1) gap = 1;
        int cap = 4096;
        char *buf = rt_wsb_alloc(cap);
        int bi = 0, col = 1;
        int slen = icn_true_len(args[0], s);
        for (int i = 0; i < slen; ) {
            char c = s[i];
            if (c == ' ') {
                int target = col + 1;
                int j = i + 1;
                while (j < slen && s[j] == ' ') { target++; j++; }
                if (target - col > 1) {
                    int nt = icn_nxttab(col, stops, nstops, gap);
                    int do_tab = 1;
                    if (nt == col + 1) { int nt1 = icn_nxttab(nt, stops, nstops, gap); if (nt1 > target) do_tab = 0; }
                    if (do_tab) {
                        while (nt <= target) { if (bi>=cap-1){ cap*=2; buf=rt_wsb_realloc(buf,cap); } buf[bi++]='\t'; col = nt; nt = icn_nxttab(col, stops, nstops, gap); }
                        while (col++ < target) { if (bi>=cap-1){ cap*=2; buf=rt_wsb_realloc(buf,cap); } buf[bi++]=' '; }
                    } else {
                        while (col < target) { if (bi>=cap-1){ cap*=2; buf=rt_wsb_realloc(buf,cap); } buf[bi++]=' '; col++; }
                    }
                } else {
                    if (bi>=cap-1){ cap*=2; buf=rt_wsb_realloc(buf,cap); }
                    buf[bi++]=' ';
                }
                col = target;
                i = j;
            } else if (c == '\t') {
                col = icn_nxttab(col, stops, nstops, gap);
                if (bi>=cap-1){ cap*=2; buf=rt_wsb_realloc(buf,cap); }
                buf[bi++]='\t';
                i++;
            } else if (c == '\b') {
                if (col > 1) col--;
                if (bi>=cap-1){ cap*=2; buf=rt_wsb_realloc(buf,cap); }
                buf[bi++]=c;
                i++;
            } else if (c == '\n' || c == '\r') {
                col = 1;
                if (bi>=cap-1){ cap*=2; buf=rt_wsb_realloc(buf,cap); }
                buf[bi++]=c;
                i++;
            } else {
                if (bi>=cap-1){ cap*=2; buf=rt_wsb_realloc(buf,cap); }
                buf[bi++]=c;
                if (isprint((unsigned char)c)) col++;
                i++;
            }
        }
        buf[bi]='\0';
        *out=BSTRVAL(buf, bi);
        return 1;
    }
    L_bidjmp_5747:
    ;
    if ((_bid == BID_abs) && nargs == 1) {
        { extern int rk_rat_is(DESCR_t); extern int rk_rat_method(const char *, DESCR_t, DESCR_t *, DESCR_t *); if (rk_rat_is(args[0]) && rk_rat_method("abs", args[0], NULL, out)) return 1; }
        if (args[0].v == DT_CPLX) { extern double rt_cplx_abs(DESCR_t); *out = REALVAL(rt_cplx_abs(args[0])); return 1; }
        extern void c_rt_coerce_num2_d(const DESCR_t *self, const DESCR_t *other, DESCR_t *out, long codes);
        extern int rt_big_is(DESCR_t);
        extern int rt_big_sign(DESCR_t);
        extern DESCR_t rt_big_neg(DESCR_t);
        DESCR_t av = args[0];
        DESCR_t nv;
        { DESCR_t _rv; if (strict && relop_radix_coerce(av, &_rv)) av = _rv; }
        c_rt_coerce_num2_d(&av, &av, &nv, 0);
        if (rt_big_is(nv)) { *out = rt_big_sign(nv) < 0 ? rt_big_neg(nv) : nv; return 1; }
        if (IS_REAL_fn(nv)) { *out = REALVAL(fabs(nv.r)); return 1; }
        if (nv.i == (-9223372036854775807LL - 1)) { *out = rt_big_neg(nv); return 1; }
        *out = INTVAL(nv.i < 0 ? -nv.i : nv.i);
        return 1;
    }
    L_bidjmp_5754:
    ;
    if ((_bid == BID_max) && nargs >= 2) {
        DESCR_t best = args[0];
        for (int _j = 1; _j < nargs; _j++) {
            DESCR_t cv = args[_j];
            int gt = ((best.v == DT_DATA || cv.v == DT_DATA) && rk_order_both_numeric(best, cv)) ? rk_order_cmp_num(best, cv) < 0 : (IS_REAL_fn(best)||IS_REAL_fn(cv)) ?
                ((IS_REAL_fn(best)?best.r:(double)best.i) < (IS_REAL_fn(cv)?cv.r:(double)cv.i)) : (best.i < cv.i);
            if (gt) best = cv;
        }
        *out = best;
        return 1;
    }
    L_bidjmp_5765:
    ;
    if ((_bid == BID_min) && nargs >= 2) {
        DESCR_t best = args[0];
        for (int _j = 1; _j < nargs; _j++) {
            DESCR_t cv = args[_j];
            int lt = ((best.v == DT_DATA || cv.v == DT_DATA) && rk_order_both_numeric(best, cv)) ? rk_order_cmp_num(best, cv) > 0 : (IS_REAL_fn(best)||IS_REAL_fn(cv)) ?
                ((IS_REAL_fn(best)?best.r:(double)best.i) > (IS_REAL_fn(cv)?cv.r:(double)cv.i)) : (best.i > cv.i);
            if (lt) best = cv;
        }
        *out = best;
        return 1;
    }
#define TONUM(av) (IS_REAL_fn(av) ? (av).r : IS_INT_fn(av) ? (double)(av).i : ((av).v==DT_S && (av).s ? strtod((av).s,NULL) : rk_rat_num(av)))
    L_bidjmp_5777:
    ;
    if ((_bid == BID_sqrt) && nargs >= 1) { DESCR_t av = args[0]; double v = TONUM(av); if (strict) return icn_math1(sqrt, 3, av, out); *out = REALVAL(sqrt(v)); return 1; }
#define MATH1(fname, cfn, dom) if (!strcmp(fn, fname) && nargs >= 1) { double _v = TONUM(args[0]); if (strict) return icn_math1(cfn, dom, args[0], out); *out = REALVAL(cfn(_v)); return 1; }
    MATH1("sin", sin, 0) MATH1("cos", cos, 0) MATH1("tan", tan, 2) MATH1("asin", asin, 1) MATH1("acos", acos, 1) MATH1("exp", exp, 2)
#undef MATH1
    L_bidjmp_5791:
    ;
    if ((_bid == BID_atan) && nargs >= 1) {
        DESCR_t _a0 = args[0], _a1 = nargs >= 2 ? args[1] : args[0], _rv;
        if (strict && relop_radix_coerce(_a0, &_rv)) _a0 = _rv;
        if (strict && nargs >= 2 && relop_radix_coerce(_a1, &_rv)) _a1 = _rv;
        double v = TONUM(_a0);
        if (nargs >= 2 && args[1].v != DT_SNUL) { double v2 = TONUM(_a1); *out = REALVAL(atan2(v,v2)); } else *out = REALVAL(atan(v));
        return 1;
    }
    L_bidjmp_5797:
    ;
    if ((_bid == BID_log) && nargs >= 1) {
        if (strict) return icn_math_log(args, nargs, out);
        double v = TONUM(args[0]);
        if (nargs >= 2 && args[1].v != DT_SNUL) { double base = TONUM(args[1]); *out = REALVAL(log(v)/log(base)); } else *out = REALVAL(log(v));
        return 1;
    }
    L_bidjmp_5803:
    ;
    if ((_bid == BID_dtor) && nargs >= 1) { double v=TONUM(args[0]); if (strict) return icn_math1(icn_dtor, 0, args[0], out); *out=REALVAL(v*3.14159265358979323846/180.0); return 1; }
    L_bidjmp_5804:
    ;
    if ((_bid == BID_rtod) && nargs >= 1) { double v=TONUM(args[0]); if (strict) return icn_math1(icn_rtod, 0, args[0], out); *out=REALVAL(v*180.0/3.14159265358979323846); return 1; }
    L_bidjmp_5805:
    ;
    if ((_bid == BID_iand) && nargs==2) { *out = rt_iand_d(args, nargs); return 1; }
    L_bidjmp_5806:
    ;
    if ((_bid == BID_ior) && nargs==2) { *out = rt_ior_d(args, nargs); return 1; }
    L_bidjmp_5807:
    ;
    if ((_bid == BID_ixor) && nargs==2) { *out = rt_ixor_d(args, nargs); return 1; }
    L_bidjmp_5808:
    ;
    if ((_bid == BID_ishift)&& nargs==2) { *out = rt_ishift_d(args, nargs); return 1; }
    L_bidjmp_5809:
    ;
    if ((_bid == BID_icom) && nargs==1) {
        extern int rt_big_is(DESCR_t);
        extern DESCR_t rt_big_neg(DESCR_t);
        extern DESCR_t rt_big_sub(DESCR_t, DESCR_t);
        if (rt_big_is(args[0])) { *out = rt_big_sub(rt_big_neg(args[0]), INTVAL(1)); return 1; }
        int64_t a=IS_INT_fn(args[0])?args[0].i:(int64_t)args[0].r;
        *out=INTVAL(~a);
        return 1;
    }
#undef TONUM
    L_bidjmp_5811:
    ;
    if ((_bid == BID_copy) && nargs == 1) {
        DESCR_t src = args[0];
        if (src.v == DT_EXTL) { extern DESCR_t rt_extl_copy(DESCR_t); *out = rt_extl_copy(src); return 1; }
        if (src.v == DT_T && src.tbl) {
            extern TBBLK_t *set_new(void);
            TBBLK_t *nt = src.tbl->is_set ? set_new() : table_new();
            nt->dflt = src.tbl->dflt;
            nt->init = src.tbl->init;
            nt->inc = src.tbl->inc;
            table_copy_in_order(nt, src.tbl);
            DESCR_t d = {0};
            d.v = DT_T;
            d.slen = 0;
            d.tbl = nt;
            *out = d;
            return 1;
        }
        if (src.v == DT_DATA) {
            DESCR_t *src_elems = NULL;
            int n = 0;
            if (rt_lv_is_list(src, &src_elems, &n)) {
                DESCR_t *new_elems = (DESCR_t *)rt_ws_alloc_descr((size_t)(n > 0 ? n : 1));
                if (src_elems && n > 0) memcpy(new_elems, src_elems, (size_t)n * sizeof(DESCR_t));
                DESCR_t eptr = {0};
                eptr.v = DT_DATA;
                eptr.slen = DATA_ELEMS_SLEN;
                eptr.ptr = (void *)new_elems;
                *out = DATCON_fn("list", eptr, INTVAL(n), STRVAL("list"), INTVAL(n), NULVCL);
                return 1;
            }
        }
        if (IS_DATA_INST_fn(src) && src.u && src.u->type) {
            DATBLK_t *rt = src.u->type;
            int nf = rt->nfields > 0 ? rt->nfields : 0;
            DATINST_t *nu = (DATINST_t *) rt_gcheap_alloc(HB_DINST, sizeof(DATINST_t));
            nu->type = rt;
            if (rt->serial_next < 1) rt->serial_next = 1;
            nu->id = rt->serial_next++;
            nu->dumpno = rt_sno_dumpno_next();
            nu->fields = (DESCR_t *)rt_ws_alloc_descr((size_t)(nf > 0 ? nf : 1));
            for (int i = 0; i < nf; i++) nu->fields[i] = src.u->fields[i];
            DESCR_t d = {0};
            d.v = DT_DATA;
            d.slen = DATA_INST_SLEN;
            d.u = nu;
            *out = d;
            return 1;
        }
        *out = src;
        return 1;
    }
    if (!strcmp(fn, "ICN$VARNAME") && nargs == 1) {
        char stat[320];
        DESCR_t *fc = icn_variable_frame_cell(args[0], stat, sizeof stat);
        if (fc) { *out = (DESCR_t){ .v = DT_N, .slen = 1, .ptr = (void *)fc }; return 1; }
        if (stat[0]) { DESCR_t sn = STRVAL(stat); return bn_sno_name(&sn, 1, out); }
        if (args[0].v == DT_S && args[0].s && args[0].slen > 0 && args[0].s[0] != 38) {
            extern const char *NV_intern_name_n(const char *, size_t);
            const char *in = NV_intern_name_n(args[0].s, (size_t)args[0].slen);
            if (!in || !NV_EXISTS_fn(in)) { *out = FAILDESCR; return 1; }
        }
        return bn_sno_name(args, nargs, out);
    }
    if (!strcmp(fn, "ICN$UNAME") && nargs == 1) return icn_unicon_name(args[0], out);
    L_bidjmp_5840:
    ;
    if ((_bid == BID_list) && nargs >= 0) {
        int n = 0;
        DESCR_t init = NULVCL;
        if (nargs >= 1) {
            DESCR_t nv = args[0];
            if (!IS_FAIL_fn(nv) && nv.v != DT_SNUL) {
                if (IS_INT_fn(nv)) n = (int)nv.i;
                else if (IS_REAL_fn(nv)) n = (int)nv.r;
                else if (icn_cvt_int_ok(nv)) n = (int)to_int(nv);
                else { *out = FAILDESCR; return 1; }
                if (n < 0) { *out = FAILDESCR; return 1; }
            }
        }
        if (nargs >= 2) { DESCR_t iv = args[1]; if (!IS_FAIL_fn(iv)) init = iv; }
        DESCR_t *elems = rt_ws_alloc_descr((size_t)(n>0?n:1));
        for (int i = 0; i < n; i++) elems[i] = init;
        DESCR_t eptr = {0};
        eptr.v=DT_DATA;
        eptr.slen=DATA_ELEMS_SLEN;
        eptr.ptr=(void*)elems;
        *out = DATCON_fn("list", eptr, INTVAL(n), STRVAL("list"), INTVAL(n), NULVCL);
        return 1;
    }
    L_bidjmp_5864:
    ;
    if ((_bid == BID_table) && nargs <= 2) {
        TBBLK_t *tbl = table_new();
        if (nargs == 1) { tbl->dflt = args[0]; } else { tbl->dflt = NULVCL; }
        DESCR_t d = {0};
        d.v = DT_T;
        d.slen = 0;
        d.tbl = tbl;
        *out = d;
        return 1;
    }
    L_bidjmp_5874:
    ;
    if ((_bid == BID_read) && nargs == 0) {
        if (g_fh[0].closed) { core_icn_error(212, FHVAL(0)); *out = FAILDESCR; return 1; }
        char *ln = NULL;
        size_t cap = 0;
        ssize_t got = rt_line_read(&ln, &cap, stdin);
        if (got < 0) { ct_drop(ln); *out = FAILDESCR; return 1; }
        size_t len = (size_t)got;
        if (len > 0 && ln[len-1] == '\n') ln[--len] = '\0';
        if (len > 0 && ln[len-1] == '\r') ln[--len] = '\0';
        char *r = rt_wsb_alloc(len + 1);
        memcpy(r, ln, len + 1);
        ct_drop(ln);
        DESCR_t rs = {0};
        rs.v = DT_S;
        rs.slen = (uint32_t)len;
        rs.s = r;
        *out = rs;
        return 1;
    }
    L_bidjmp_5883:
    ;
    L_bidjmp_5894:
    ;
    if ((_bid == BID_runerr) && nargs >= 1) {
        long long _ec = IS_INT_fn(args[0]) ? (long long)args[0].i : 500;
        { if (g_runtime.abi.error != 0) return icn_argtype_raise((int)_ec, (nargs >= 2) ? args[1] : FAILDESCR, out); }
        core_icn_error((int)_ec, (nargs >= 2) ? args[1] : FAILDESCR);
        exit(1);
    }
    L_bidjmp_5895:
    ;
    if ((_bid == BID_stop)) {
        FILE *dest = stderr;
        fflush(stdout);
        for (int _si = 0; _si < nargs; _si++) {
            DESCR_t _a = args[_si];
            if (IS_FH_fn(_a)) { FILE *fp = fh_get((int)_a.i); if (fp) { if (_si > 0) fputc('\n', dest); dest = fp; } continue; }
            if (_a.v == DT_SNUL) continue;
            out_write_descr(dest, _a, 1);
        }
        fputc('\n', dest);
        fflush(stdout);
        fflush(dest);
        exit(1);
    }
    if ((_bid == BID_exit)) {
        extern int64_t core_icn_to_int_d(DESCR_t);
        long long _st = (nargs >= 1 && args[0].v != DT_SNUL && args[0].v != 0) ? (long long)core_icn_to_int_d(args[0]) : 0;
        exit((int)_st);
    }
    if (!strcmp(fn, "getch") || !strcmp(fn, "getche")) {
        fflush(stdout);
        int _c = icn_rchar(fn[5] == 'e');
        if (_c < 0) { *out = FAILDESCR; return 1; }
        char *_b = rt_wsb_alloc(2);
        _b[0] = (char)_c;
        _b[1] = 0;
        *out = STRVAL(_b);
        return 1;
    }
    if (!strcmp(fn, "loadfunc") && nargs >= 2) {
        extern DESCR_t rt_extfn_mint(const char *, void *);
        const char *_l = VARVAL_fn(args[0]);
        const char *_fnm = VARVAL_fn(args[1]);
        void *_h = _l ? dlopen(_l, RTLD_NOW) : 0;
        void *_f = (_h && _fnm) ? dlsym(_h, _fnm) : 0;
        if (!_f) {
            const char *_de = dlerror();
            fprintf(stderr, "\nloadfunc(\"%s\",\"%s\"): %s\n", _l ? _l : "", _fnm ? _fnm : "", _de ? _de : "");
            fflush(stderr);
            icn_loadfunc_cstr_args(args, nargs);
            if (g_runtime.abi.error != 0) return icn_argtype_raise(216, args[1], out);
            core_runtime_error(216, "external function not found");
            *out = FAILDESCR;
            return 1;
        }
        *out = rt_extfn_mint(_fnm, _f);
        return 1;
    }
    L_bidjmp_5920:
    ;
    if ((_bid == BID_any) && nargs >= 1) return bn_icn_any(args, nargs, out);
    L_bidjmp_5938:
    ;
    if ((_bid == BID_many) && nargs >= 1) return bn_icn_many(args, nargs, out);
    L_bidjmp_5958:
    ;
    if ((_bid == BID_upto) && nargs >= 1) {
        const char *cv = VARVAL_fn(args[0]);
        if (!cv) { *out = FAILDESCR; return 1; }
        long cvn = IS_CSET_fn(args[0]) ? (long)descr_slen(args[0]) : (args[0].v == DT_S) ? (long)args[0].slen : -1;
        const char *s;
        int i1, i2;
        if (!bn_str_anal(args, nargs, 1, &s, &i1, &i2)) { *out = FAILDESCR; return 1; }
        const unsigned char *bits = IS_CSET_fn(args[0]) ? cset_blk_bits(cv) : (const unsigned char *)0;
        int p = i1;
        if (bits) while (p < i2 && !((bits[(unsigned char)s[p - 1] >> 3] >> ((unsigned char)s[p - 1] & 7)) & 1)) p++;
        else while (p < i2 && !rt_icn_cset_member_n(cv, cvn, s[p - 1])) p++;
        if (p >= i2) { *out = FAILDESCR; return 1; }
        *out = INTVAL(p);
        return 1;
    }
    L_bidjmp_5978:
    ;
    if ((_bid == BID_tab) && nargs == 1) return bn_icn_tab(args, out);
    L_bidjmp_5991:
    ;
    if ((_bid == BID_move) && nargs == 1) return bn_icn_move(args, out);
    L_bidjmp_6004:
    ;
    if ((_bid == BID_pos) && nargs == 1) {
        bn_win_t w = bn_win();
        int slen = (int)w.n;
        int target = (int)to_int(args[0]);
        if (target <= 0) target = slen + 1 + target;
        if (target < 1 || target > slen + 1 || target != w.pos) { *out = FAILDESCR; return 1; }
        *out = INTVAL(target);
        return 1;
    }
    L_bidjmp_6012:
    ;
    if ((_bid == BID_match) && nargs >= 1) {
        const char *pat = VARVAL_fn(args[0]);
        if (!pat) { *out = FAILDESCR; return 1; }
        const char *s;
        int i1, i2;
        if (!bn_str_anal(args, nargs, 1, &s, &i1, &i2)) { *out = FAILDESCR; return 1; }
        int slen = (int)strlen(s), plen2 = (int)strlen(pat), p0 = i1 - 1;
        if (p0 + plen2 > slen || i1 + plen2 > i2) { *out = FAILDESCR; return 1; }
        if (strncmp(s + p0, pat, (size_t)plen2) != 0) { *out = FAILDESCR; return 1; }
        *out = INTVAL(i1 + plen2);
        return 1;
    }
    L_bidjmp_6033:
    ;
    if ((_bid == BID_bal)) { return bn_bal_gen(args, nargs, out, (int64_t *)0); }
    L_bidjmp_6067:
    ;
    if ((_bid == BID_find) && nargs >= 1) {
        const char *needle = VARVAL_fn(args[0]);
        if (!needle) { *out = FAILDESCR; return 1; }
        int nlen = (args[0].v == DT_S && args[0].s && args[0].slen != 0xFFFFFFFFu) ? (int)args[0].slen : (int)strlen(needle);
        const char *hay;
        int i1, i2;
        if (!bn_str_anal(args, nargs, 1, &hay, &i1, &i2)) { *out = FAILDESCR; return 1; }
        int term = (i2 - 1) - nlen;
        for (int i = i1 - 1; i <= term; i++) if (memcmp(hay + i, needle, (size_t)nlen) == 0) { *out = INTVAL(i + 1); return 1; }
        *out = FAILDESCR;
        return 1;
    }
    L_bidjmp_6081:
    ;
    if ((_bid == BID_NONNULL) && nargs == 1) {
        DESCR_t v = args[0];
        if (IS_FAIL_fn(v)) { *out = FAILDESCR; return 1; }
        if (v.v == DT_SNUL) { *out = FAILDESCR; return 1; }
        if (v.v == DT_S && (!v.s || descr_slen(v) == 0)) { *out = FAILDESCR; return 1; }
        *out = v;
        return 1;
    }
    L_bidjmp_6088:
    ;
    if ((_bid == BID_ICN_CASE_EQ) && nargs == 2) {
        DESCR_t topic = args[0], val = args[1];
        if (IS_FAIL_fn(topic) || IS_FAIL_fn(val)) { *out = FAILDESCR; return 1; }
        int eq = 0;
        if ((IS_INT_fn(topic) || IS_REAL_fn(topic)) && (IS_INT_fn(val) || IS_REAL_fn(val))) {
            double tv = IS_REAL_fn(topic) ? topic.r : (double)topic.i;
            double vv = IS_REAL_fn(val) ? val.r : (double)val.i;
            eq = (tv == vv);
        } else {
            const char *ts = VARVAL_fn(topic);
            if (!ts) ts = "";
            const char *vs = VARVAL_fn(val);
            if (!vs) vs = "";
            eq = (strcmp(ts, vs) == 0);
        }
        *out = eq ? val : FAILDESCR;
        return 1;
    }
    L_bidjmp_6104:
    ;
    if ((_bid == BID_ICN_SWAP_TOP2) && nargs == 2) { *out = args[0]; return 1; }
    L_bidjmp_6108:
    ;
    if ((_bid == BID_ICN_NULL) && nargs == 1) {
        DESCR_t v = args[0];
        if (IS_FAIL_fn(v)) { *out = FAILDESCR; return 1; }
        if (v.v == DT_SNUL) { *out = NULVCL; return 1; }
        if (v.v == DT_S && (!v.s || descr_slen(v) == 0)) { *out = NULVCL; return 1; }
        *out = FAILDESCR;
        return 1;
    }
    L_bidjmp_6115:
    ;
    if ((_bid == BID_insert) && nargs >= 1 && icn_arg_is_list(args[0])) return bn_uni_list_insert(args, nargs, out);
    if ((_bid == BID_insert) && nargs >= 1) {
        DESCR_t td = args[0];
        if (td.v != DT_T) { *out = FAILDESCR; return 1; }
        DESCR_t kd = (nargs >= 2) ? args[1] : NULVCL;
        DESCR_t vd = (td.tbl && td.tbl->is_set) ? kd : ((nargs >= 3) ? args[2] : NULVCL);
        table_set_descr_d(td.tbl, kd, vd);
        *out = td;
        return 1;
    }
    L_bidjmp_6124:
    ;
    if ((_bid == BID_delete) && nargs >= 1 && icn_arg_is_list(args[0])) return bn_uni_list_delete(args, nargs, out);
    if ((_bid == BID_delete) && nargs >= 1) {
        DESCR_t td = args[0];
        if (nargs == 1 && IS_STR_fn(td)) { const char *path = VARVAL_fn(td); if (!path || !path[0] || unlink(path) != 0) { *out = FAILDESCR; return 1; } *out = NULVCL; return 1; }
        if (td.v != DT_T) { *out = FAILDESCR; return 1; }
        DESCR_t kd = (nargs >= 2) ? args[1] : NULVCL;
        table_delete_d(td.tbl, kd);
        *out = td;
        return 1;
    }
    L_bidjmp_6132:
    ;
    if ((_bid == BID_member) && nargs >= 1) {
        DESCR_t td = args[0];
        if (td.v != DT_T) { *out = FAILDESCR; return 1; }
        DESCR_t kd = (nargs >= 2) ? args[1] : NULVCL;
        if (!table_has_d(td.tbl,kd)) { *out=FAILDESCR; return 1; }
        *out = kd;
        return 1;
    }
    L_bidjmp_6140:
    ;
    if ((_bid == BID_key) && nargs == 1) {
        DESCR_t td = args[0];
        if (td.v != DT_T || !td.tbl) { *out=FAILDESCR; return 1; }
        for (unsigned _bi=0;_bi<td.tbl->nbuck;_bi++) if (td.tbl->buckets[_bi] && td.tbl->buckets[_bi]->len) { *out = td.tbl->buckets[_bi]->ent[0].key_descr; return 1; }
        *out = FAILDESCR;
        return 1;
    }
    L_bidjmp_6149:
    ;
    if ((_bid == BID___apply__) && nargs == 2) {
        DESCR_t callee = args[0];
        DESCR_t lv = args[1];
        int an = rt_apply_unpack(lv, (DESCR_t *)0, 0);
        DESCR_t buf[an > 0 ? an : 1];
        int n = an < 0 ? an : rt_apply_unpack(lv, buf, an);
        if (n < 0) return icn_argtype_raise(126, lv, out);
        *out = RT_GC_CALLBACK(rt_call_value(callee, buf, n));
        return 1;
    }
    L_bidjmp_6162:
    ;
    if ((_bid == BID_push) && nargs >= 1) {
        DESCR_t ld = args[0];
        if (ld.v != DT_DATA) return 0;
        {
            DESCR_t *_a=0;
            int _n=0;
            if (rt_lv_is_list(ld,&_a,&_n)) {
                int _nv2 = (nargs > 1) ? nargs - 1 : 1;
                for (int _pi = 0; _pi < _nv2; _pi++) {
                    DESCR_t _vd = (nargs > 1) ? args[1 + _pi] : NULVCL;
                    rt_lvb_push(ld,_n);
                    DESCR_t *_nb=rt_ws_alloc_descr((size_t)(_n+1));
                    _nb[0]=_vd;
                    if(_a&&_n>0) memcpy(_nb+1,_a,_n*sizeof(DESCR_t));
                    _a=_nb;
                    _n++;
                    rt_lv_set_elems(ld,DATA_ELEMS(_nb));
                    rt_lv_set_size(ld,_n);
                    rt_lv_set_cap(ld,_n);
                }
                *out = ld;
                return 1;
            }
        }
        return 0;
    }
    L_bidjmp_6182:
    ;
    if ((_bid == BID_put) && nargs >= 1) return bn_icn_put(args, nargs, out);
    L_bidjmp_6206:
    ;
    if ((_bid == BID_get) && nargs == 1) return bn_icn_get(args[0], out);
    L_bidjmp_6221:
    ;
    if ((_bid == BID_pop) && nargs == 1) {
        DESCR_t ld = args[0];
        if (ld.v != DT_DATA) return 0;
        {
            DESCR_t *_a=0;
            int _n=0;
            if (rt_lv_is_list(ld,&_a,&_n)) {
                if(!_a||_n<=0) { *out=FAILDESCR; return 1; }
                rt_lvb_get(ld,_n);
                DESCR_t _r=_a[0];
                rt_lv_set_elems(ld,DATA_ELEMS(_a+1));
                rt_lv_set_size(ld,_n-1);
                { long _c=rt_lv_cap(ld); if(_c>0) rt_lv_set_cap(ld,_c-1); }
                *out=_r;
                return 1;
            }
        }
        return 0;
    }
    L_bidjmp_6236:
    ;
    if ((_bid == BID_pull) && nargs == 1) {
        DESCR_t ld = args[0];
        if (ld.v != DT_DATA) return 0;
        {
            DESCR_t *_a=0;
            int _n=0;
            if (rt_lv_is_list(ld,&_a,&_n)) { if(!_a||_n<=0) { *out=FAILDESCR; return 1; } rt_lvb_pull(ld,_n); DESCR_t _r=_a[_n-1]; rt_lv_set_size(ld,_n-1); *out=_r; return 1; }
        }
        return 0;
    }
    L_bidjmp_6249:
    ;
    { int _sf = sort_family(_bid, args, nargs, out, 0); if (_sf >= 0) return _sf; }
    L_bidjmp_6322:
    ;
    if ((_bid == BID_FIELD_GET) && nargs == 2) {
        DESCR_t obj = args[0];
        DESCR_t fname_d = args[1];
        const char *fname = VARVAL_fn(fname_d);
        if (!fname || obj.v != DT_DATA) { *out=FAILDESCR; return 1; }
        extern DESCR_t c_dat_field_get(const char *field, DESCR_t obj);
        *out = rtx_try_dat_field_get(fname, obj);
        return 1;
    }
    L_bidjmp_6330:
    ;
    if ((_bid == BID_FIELD_SET) && nargs == 3) {
        DESCR_t val = args[0];
        DESCR_t obj = args[1];
        DESCR_t fname_d = args[2];
        const char *fname = VARVAL_fn(fname_d);
        if (!fname || obj.v != DT_DATA) { *out=FAILDESCR; return 1; }
        extern DESCR_t *data_field_ptr(const char *field, DESCR_t obj);
        DESCR_t *cell = data_field_ptr(fname, obj);
        if (cell) { *cell = val; *out = val; return 1; }
        *out = FAILDESCR;
        return 1;
    }
    L_bidjmp_6341:
    ;
    if ((_bid == BID_MAKELIST)) { extern DESCR_t rt_make_list(DESCR_t *args, int nargs); *out = rt_make_list(args, nargs); return 1; }
    L_bidjmp_6345:
    ;
    if ((_bid == BID_RECORD_REGISTER) && nargs >= 1) {
        extern void record_register(const char *spec);
        const char *spec = VARVAL_fn(args[0]);
        if (spec && *spec) record_register(spec);
        *out = NULVCL;
        return 1;
    }
    L_bidjmp_6351:
    ;
    if ((_bid == BID_RECORD_MAKE) && nargs >= 1) {
        const char *rname = VARVAL_fn(args[0]);
        if (!rname || !*rname) { *out=FAILDESCR; return 1; }
        DatType *_dt = dat_find_type(rname);
        if (!_dt) { *out=FAILDESCR; return 1; }
        *out = dat_construct_rq(_dt, args + 1, nargs - 1, rq);
        return 1;
    }
    L_bidjmp_6361:
    ;
    if ((_bid == BID_open) && (nargs == 1 || nargs == 2 || (nargs > 2 && (args[1].v == DT_S || args[1].v == DT_SNUL) && args[1].s && (strchr(args[1].s, 'n') || strchr(args[1].s, 'h'))))) {
        const char *path = (args[0].v == DT_S || args[0].v == DT_SNUL) ? args[0].s : NULL;
        if (!path) { *out = FAILDESCR; return 1; }
        const char *mode = (nargs >= 2 && (args[1].v == DT_S||args[1].v == DT_SNUL) && args[1].s) ? args[1].s : "r";
        const char *cmode = "r";
        char mbuf[8];
        int is_pipe = 0;
        if (strict && strpbrk(mode, "gG")) { *out = FAILDESCR; return 1; }
        if (strchr(mode, 'n')) {
            if (nargs == 2 && strchr(mode, 'u') && strchr(mode, 'a') && !strchr(mode, 'r')) {
                const char *want = rt_cstr_d(args[0]);
                for (int i = 3; i < FH_N; i++) {
                    int t = 0;
                    socklen_t tl = sizeof t;
                    if (!g_fh[i].fp || !g_fh[i].name || strcmp(g_fh[i].name, want) || getsockopt(fileno(g_fh[i].fp), SOL_SOCKET, SO_TYPE, &t, &tl) != 0 || t != SOCK_DGRAM) continue;
                    int nfd = dup(fileno(g_fh[i].fp));
                    if (nfd < 0) { *out = FAILDESCR; return 1; }
                    FILE *afp = fdopen(nfd, "r+");
                    if (!afp) { close(nfd); *out = FAILDESCR; return 1; }
                    setvbuf(afp, NULL, _IONBF, 0);
                    int aidx = fh_alloc(afp);
                    if (aidx < 0) { fclose(afp); *out = FAILDESCR; return 1; }
                    g_fh[aidx].name = rt_heap_strdup_c(want);
                    *out = FHVAL(aidx);
                    return 1;
                }
            }
            extern FILE *px_sock_open(const char *, const char *, const DESCR_t *, int);
            FILE *sfp = px_sock_open(path, mode, nargs > 2 ? args + 2 : NULL, nargs > 2 ? nargs - 2 : 0);
            int sidx = sfp ? fh_alloc(sfp) : -1;
            if (sidx < 0) { if (sfp) fclose(sfp); *out = FAILDESCR; return 1; }
            g_fh[sidx].name = rt_heap_strdup_c(path);
            *out = FHVAL(sidx);
            return 1;
        }
        if (icn_open_spec_is_icon(mode)) { icn_open_cmode(mode, mbuf, &is_pipe); cmode = mbuf; } else if (strstr(mode,"w")) cmode = "w";
        else if (strstr(mode,"a")) cmode = "a";
        if (is_pipe && (strcmp(cmode, "r") && strcmp(cmode, "w"))) { *out = FAILDESCR; return 1; }
        if (is_pipe) { fflush(NULL); setvbuf(stdout, NULL, _IOLBF, 0); }
        extern FILE *rt_dir_snapshot(const char *);
        FILE *fp = NULL;
        int _isdir = 0;
        struct stat _dst;
        if (!is_pipe && !strcmp(cmode, "r") && !stat(path, &_dst) && S_ISDIR(_dst.st_mode)) { fp = rt_dir_snapshot(path); if (!fp) { *out = FAILDESCR; return 1; } _isdir = 1; }
        if (!fp) fp = is_pipe ? popen(path, cmode) : fopen(path, cmode);
        if (!fp) { g_rt_misc.icn_errtext = strerror(errno); g_rt_misc.icn_err_valid = 1; *out = FAILDESCR; return 1; }
        int idx = fh_alloc(fp);
        if (idx < 0) { if (is_pipe) pclose(fp); else fclose(fp); *out = FAILDESCR; return 1; }
        if (is_pipe) g_fh[idx].type = 'p';
        if (_isdir && idx >= 0 && idx < FH_N) g_fh[idx].type = 'd';
        {
            extern void fh_set_untranslated(int, int);
            const char *_us = (nargs == 2 && (args[1].v == DT_S || args[1].v == DT_SNUL)) ? VARVAL_fn(args[1]) : NULL;
            fh_set_untranslated(idx, (_us && icn_open_spec_is_icon(_us)) ? icn_open_untranslated(_us) : 0);
        }
        if (idx >= 0 && idx < FH_N) g_fh[idx].name = rt_heap_strdup_c(path);
        *out = FHVAL(idx);
        return 1;
    }
    L_bidjmp_6376:
    ;
    if ((_bid == BID_remove) && nargs == 1) { const char *_rp = VARVAL_fn(args[0]); if (!_rp) _rp = ""; if (unlink(_rp) == 0) { *out = NULVCL; return 1; } *out = FAILDESCR; return 1; }
    L_bidjmp_6381:
    ;
    if ((_bid == BID_close) && nargs == 1) {
        if (IS_FH_fn(args[0]) || IS_INT_fn(args[0])) {
            int idx = (int)args[0].i;
            FILE *fp = fh_get(idx);
            int pst = -1;
            if (fp && idx > 2) { if (g_fh[idx].type == 'p') { fflush(stdout); pst = pclose(fp); } else fclose(fp); g_fh[idx].type = 0; fh_free(idx); } else if (fp) fflush(fp);
            if (idx >= 0 && idx < FH_N) g_fh[idx].closed = 1;
            if (pst >= 0) { *out = INTVAL((pst >> 8) & 0xFF); return 1; }
        }
        *out = args[0];
        return 1;
    }
    L_bidjmp_9997:
    ;
    if ((_bid == BID_errorclear) && nargs == 0) { g_rt_misc.icn_err_valid = 0; *out = NULVCL; return 1; }
    L_bidjmp_9998:
    ;
    if ((_bid == BID_serial) && nargs == 1) {
        if (IS_DATA_INST_fn(args[0]) && args[0].u) { *out = INTVAL(args[0].u->id); return 1; }
        if (args[0].v == DT_T && args[0].tbl) { *out = INTVAL(args[0].tbl->id); return 1; }
        if (args[0].v == DT_CO && args[0].p) { extern long scrip_coexpr_serial_of(void *); *out = INTVAL(scrip_coexpr_serial_of(args[0].p)); return 1; }
        *out = FAILDESCR;
        return 1;
    }
    L_bidjmp_9999:
    ;
    if ((_bid == BID_flush) && nargs == 1) { if (IS_FH_fn(args[0]) || IS_INT_fn(args[0])) { int idx = (int)args[0].i; FILE *fp = fh_get(idx); if (fp) fflush(fp); } *out = args[0]; return 1; }
    if ((_bid == BID_read) && nargs == 1) return bn_icn_read_fh(args[0], out);
    if ((_bid == BID_reads) && nargs >= 1) {
        {
            int _ri = (args[0].v == DT_SNUL) ? 0 : (IS_FH_fn(args[0]) || IS_INT_fn(args[0])) ? (int)args[0].i : -1;
            if (_ri >= 0 && _ri < FH_N && g_fh[_ri].closed) { core_icn_error(212, args[0].v == DT_SNUL ? FHVAL(0) : args[0]); *out = FAILDESCR; return 1; }
        }
        FILE *fp = (args[0].v == DT_SNUL) ? fh_get(0) : (IS_FH_fn(args[0]) || IS_INT_fn(args[0])) ? fh_get((int)args[0].i) : NULL;
        if (!fp) { *out = FAILDESCR; return 1; }
        int n = (nargs >= 2 && args[1].v != DT_SNUL && !IS_FAIL_fn(args[1])) ? (int)to_int(args[1]) : 1;
        if (n <= 0) { *out = FAILDESCR; return 1; }
        {
            int _fhi = (args[0].v == DT_SNUL) ? 0 : (int)args[0].i;
            if (_fhi >= 0 && _fhi < FH_N && g_fh[_fhi].type == 'd') {
                char *db = rt_wsb_alloc(n + 1);
                int dl = 0, dc, saw = 0;
                while ((dc = fgetc(fp)) != EOF) { saw = 1; if (dc == '\n') break; if (dl < n) db[dl++] = (char)dc; }
                if (!saw) { *out = FAILDESCR; return 1; }
                db[dl] = '\0';
                DESCR_t dr = {0};
                dr.v = DT_S;
                dr.slen = (uint32_t)dl;
                dr.s = db;
                *out = dr;
                return 1;
            }
        }
        char *buf = rt_wsb_alloc(n + 1);
        int got = (int)fread(buf, 1, (size_t)n, fp);
        if (got <= 0) { *out = FAILDESCR; return 1; }
        buf[got] = '\0';
        DESCR_t r = {0};
        r.v = DT_S;
        r.slen = (uint32_t)got;
        r.s = buf;
        *out = r;
        return 1;
    }
    L_bidjmp_6411:
    ;
    if ((_bid == BID_IDENTICAL) && nargs == 2) { *out = bn_identical(args[0], args[1]) ? args[1] : FAILDESCR; return 1; }
    L_bidjmp_6423:
    ;
    if ((_bid == BID_set) && nargs <= 1) {
        TBBLK_t *tbl = set_new();
        if (nargs == 1 && args[0].v == DT_DATA) {
            DESCR_t *elems = NULL;
            int n = 0;
            if (rt_lv_is_list(args[0], &elems, &n)) { char kb[64]; if (elems) for (int _i = 0; _i < n; _i++) table_set_descr_d(tbl, elems[_i], elems[_i]); }
        }
        *out = TABLE_VAL(tbl);
        return 1;
    }
    L_bidjmp_6439:
    ;
    if ((_bid == BID_ASGN) && nargs == 2) {
        DESCR_t rhs = args[0];
        if (IS_FAIL_fn(rhs)) { *out = FAILDESCR; return 1; }
        DESCR_t lref = args[1];
        if (lref.v == DT_S && lref.s) NV_SET_fn(lref.s, rhs);
        *out = rhs;
        return 1;
    }
    L_bidjmp_6446:
    ;
    if ((_bid == BID_name) && nargs == 1) {
        DESCR_t a = args[0];
        if (a.v == DT_N && a.slen == 0 && a.s && *a.s) { *out = STRVAL(rt_heap_strdup_c(a.s)); return 1; }
        if (IS_NAMETRAP_fn(a) && a.p) {
            VCELL_t *vc = (VCELL_t *)a.p;
            if (vc->key && *vc->key) { *out = STRVAL(rt_heap_strdup_c(vc->key)); return 1; }
            if (vc->tbl) {
                DESCR_t kn;
                int r2 = try_call_builtin_by_name("image", &vc->key_d, 1, &kn);
                const char *key = (r2 && kn.v == DT_S && kn.s) ? kn.s : "";
                *out = STRVAL(rt_heap_fmt("T[%s]", key));
                return 1;
            }
            if (vc->cellp && vc->pos > 0) { *out = STRVAL(rt_heap_fmt("L[%ld]", (long)vc->pos)); return 1; }
            if (vc->cellp && vc->pos < 0 && vc->sv.u && vc->sv.u->type && vc->sv.u->type->fields && -vc->pos <= vc->sv.u->type->nfields) {
                const char *tn = vc->sv.u->type->name ? vc->sv.u->type->name : "", *fnm = vc->sv.u->type->fields[-vc->pos - 1] ? vc->sv.u->type->fields[-vc->pos - 1] : "";
                *out = STRVAL(rt_heap_fmt("%s.%s", tn, fnm));
                return 1;
            }
            if (vc->sv.v == DT_N && (vc->sv.slen == 0 || vc->sv.slen == 2) && vc->len > 0) {
                DESCR_t bn;
                int r1 = try_call_builtin_by_name("name", &vc->sv, 1, &bn);
                const char *base = (r1 && bn.v == DT_S && bn.s) ? bn.s : "";
                *out = STRVAL(rt_heap_fmt("%s[%ld:%ld]", base, (long)vc->pos, (long)(vc->pos + vc->len)));
                return 1;
            }
            *out = FAILDESCR;
            return 1;
        }
        *out = FAILDESCR;
        return 1;
    }
    L_bidjmp_6462:
    ;
    if ((_bid == BID_variable) && nargs == 1) {
        const char *vname = (args[0].v == DT_S || args[0].v == DT_SNUL) ? icn_callee_name(args[0]) : NULL;
        if (!vname || !*vname) { *out = FAILDESCR; return 1; }
        char stat[320];
        { DESCR_t *fc = icn_variable_frame_cell(args[0], stat, sizeof stat); if (fc) { *out = *fc; return 1; } if (stat[0]) vname = stat; }
        DESCR_t v = NV_GET_fn(vname);
        if (IS_FAIL_fn(v) || (v.v == DT_SNUL && !NV_EXISTS_fn(vname))) {
            if (!strcmp(vname, "&random") || !strcmp(vname, "&trace") || !strcmp(vname, "&error") || !strcmp(vname, "&dump")) {
                extern DESCR_t kw_read(const char *);
                *out = kw_read(vname + 1);
                return 1;
            }
            *out = FAILDESCR;
            return 1;
        }
        *out = v;
        return 1;
    }
    L_bidjmp_6468:
    ;
    if ((_bid == BID_SNOx24NAME) && nargs == 1) return bn_sno_name(args, nargs, out);
    L_bidjmp_6469:
    ;
    if ((_bid == BID_ARRAY) && nargs >= 1) { extern DESCR_t sno_array_d(DESCR_t a, DESCR_t init); *out = sno_array_d(args[0], nargs >= 2 ? args[1] : NULVCL); return 1; }
    L_bidjmp_6479:
    ;
    if ((_bid == BID_TABLE) && nargs <= 3) {
        TBBLK_t *tb = table_new_args(nargs >= 1 ? (int)to_int(args[0]) : 0, nargs >= 2 ? (int)to_int(args[1]) : 0);
        tb->null_one = 1;
        if (nargs >= 3) tb->dflt = args[2];
        DESCR_t d;
        memset(&d, 0, sizeof d);
        d.v = DT_T;
        d.slen = 0;
        d.tbl = tb;
        *out = d;
        return 1;
    }
    L_bidjmp_6484:
    ;
    if ((_bid == BID_ITEM) && nargs >= 2) {
        extern DESCR_t c_rt_subscript_var(DESCR_t base, DESCR_t idx);
        extern DESCR_t rt_deref(DESCR_t v);
        DESCR_t cur = args[0];
        for (int k = 1; k < nargs; k++) { cur = rtx_try_rt_subscript_var(cur, args[k]); if (IS_FAIL_fn(cur)) { *out = FAILDESCR; return 1; } }
        *out = rt_deref(cur);
        return 1;
    }
    L_bidjmp_6490:
    ;
    if (_bid == BID_PROTOTYPE) { *out = agg_prototype(nargs >= 1 ? args[0] : NULVCL); return 1; }
    L_bidjmp_6499:
    ;
    if ((_bid == BID_CONVERT) && nargs == 2) {
        char tb[32];
        const char *ts = to_cstring(args[1], tb, sizeof tb);
        if (!ts) ts = "";
        const char * tu = ts;
        DESCR_t a = args[0];
        extern DESCR_t rt_sno_cnv_num(DESCR_t, int);
        if (a.v == DT_S && a.s) a.s = (char *)rt_cstr_d(a);
        if (!strcmp(tu,"INTEGER")) { *out = rt_sno_cnv_num(a, 'I'); return 1; }
        if (!strcmp(tu,"REAL")) { *out = rt_sno_cnv_num(a, 'R'); return 1; }
        if (!strcmp(tu,"STRING")) { if (rt_str_arg_bad(a)) { *out = FAILDESCR; return 1; } const char *sv = VARVAL_fn(a); *out = STRVAL(rt_heap_strdup_c(sv ? sv : "")); return 1; }
        return 0;
    }
    L_bidjmp_6521:
    ;
    if ((_bid == BID_DATA) && nargs == 1) {
        extern DatType *dat_register(const char *spec);
        const char *sp = VARVAL_fn(args[0]);
        if (!sp || !*sp) { *out = FAILDESCR; return 1; }
        char nb[strlen(sp) + 1];
        int k = 0;
        for (; sp[k] && sp[k] != '('; k++) nb[k] = sp[k];
        nb[k] = 0;
        if (nb[0] && sn4_sysfn_protected(nb)) { extern int kwb_error(int code, const char *msg); kwb_error(248, "attempted redefinition of system function"); *out = FAILDESCR; return 1; }
        if (sp[k] == '(') {
            const char *fs = sp + k + 1;
            const char *fe = strchr(fs, ')');
            if (!fe) fe = fs + strlen(fs);
            while (fs < fe) {
                const char *cm = fs;
                while (cm < fe && *cm != ',') cm++;
                const char *b = fs;
                while (b < cm && (*b == ' ' || *b == '\t')) b++;
                const char *e2 = cm;
                while (e2 > b && (e2[-1] == ' ' || e2[-1] == '\t')) e2--;
                size_t fl = (size_t)(e2 - b);
                if (fl > 0) {
                    char fb[fl + 1];
                    memcpy(fb, b, fl);
                    fb[fl] = 0;
                    if (sn4_sysfn_protected(fb)) { extern int kwb_error(int code, const char *msg); kwb_error(248, "attempted redefinition of system function"); *out = FAILDESCR; return 1; }
                }
                fs = (cm < fe) ? cm + 1 : fe;
            }
        }
        { extern int dat_spec_is_current(const char *spec); if (!dat_spec_is_current(sp)) dat_register(sp); }
        { extern void dat_set_live(const char *name, int live); if (nb[0]) dat_set_live(nb, 1); }
        *out = NULVCL;
        return 1;
    }
    L_bidjmp_6528:
    ;
    if ((_bid == BID_SNOx24KWSET) && nargs == 2) {
        extern int rt_keyword_write_snobol4(const char *sval, DESCR_t v);
        char kb[64];
        const char *kn = to_cstring(args[0], kb, sizeof kb);
        *out = rt_keyword_write_snobol4(kn ? kn : "", args[1]) ? args[1] : FAILDESCR;
        return 1;
    }
    L_bidjmp_6534:
    ;
    if ((_bid == BID_SNOx24STMT) && (nargs == 1 || nargs == 2 || nargs == 3)) {
        extern void rt_stmt_enter(long stno, long line);
#if RT_DIAG
        if (!IS_INT(args[0]) && nargs == 1) { extern void rt_trace_label_hook(const char *); const char *lb = VARVAL_fn(args[0]); rt_trace_label_hook(lb ? lb : ""); *out = NULVCL; return 1; }
#else
        if (!IS_INT(args[0]) && nargs == 1) { *out = NULVCL; return 1; }
#endif
        long n = IS_INT(args[0]) ? (long)args[0].i : 0;
        long ln = (nargs >= 2 && IS_INT(args[1])) ? (long)args[1].i : 0;
        if (n >= 0) rt_stmt_enter(n, ln);
        if (nargs == 3) { extern void rt_stmt_file_init(const char *file); const char *fp = VARVAL_fn(args[2]); rt_stmt_file_init(fp ? fp : ""); }
        *out = NULVCL;
        return 1;
    }
    if ((_bid == BID_ICNx24LINE) && (nargs == 1 || nargs == 2)) {
        g_runtime.abi.line = IS_INT(args[0]) ? (long)args[0].i : 0;
        if (nargs == 2) { extern void rt_stmt_file_init(const char *file); const char *fp = VARVAL_fn(args[1]); rt_stmt_file_init(fp ? fp : ""); }
        *out = NULVCL;
        return 1;
    }
    L_bidjmp_6540:
    ;
    if ((_bid == BID_SNOx24MKEXPR) && nargs == 1) { extern DESCR_t rt_sno_mkexpr_of(DESCR_t); *out = rt_sno_mkexpr_of(args[0]); return 1; }
    L_bidjmp_6545:
    ;
    if ((_bid == BID_SNOx24PBK) && nargs == 2) {
        extern DESCR_t pat_mk_cset(int, const char *);
        extern int pat_cset_code(int);
        extern int pat_cset_arg_ok(int, DESCR_t);
        extern const char *pat_cset_text(DESCR_t);
        *out = pat_cset_arg_ok(pat_cset_code((int)to_int(args[0])), args[1]) ? pat_mk_cset((int)to_int(args[0]), pat_cset_text(args[1])) : FAILDESCR;
        return 1;
    }
    L_bidjmp_6546:
    ;
    if ((_bid == BID_SNOx24PBN) && nargs == 2) {
        extern DESCR_t pat_mk_num(int, int64_t);
        extern int pat_num_arg_ok(int, DESCR_t);
        *out = pat_num_arg_ok((int)to_int(args[0]), args[1]) ? pat_mk_num((int)to_int(args[0]), to_int(args[1])) : FAILDESCR;
        return 1;
    }
    L_bidjmp_6547:
    ;
    if ((_bid == BID_SNOx24PB0) && nargs == 1) { extern DESCR_t pat_mk_nil(int); *out = pat_mk_nil((int)to_int(args[0])); return 1; }
    L_bidjmp_6548:
    ;
    if ((_bid == BID_SNOx24PBC) && nargs == 3) { extern DESCR_t pat_mk_capt(int, const char *, DESCR_t); *out = pat_mk_capt((int)to_int(args[0]), rt_sno_target_name(args[1]), args[2]); return 1; }
    L_bidjmp_6549:
    ;
    if ((_bid == BID_SNOx24PCUR) && nargs == 1) { extern DESCR_t pat_mk_cursor(const char *); *out = pat_mk_cursor(rt_sno_target_name(args[0])); return 1; }
    L_bidjmp_6550:
    ;
    if ((_bid == BID_SNOx24PBALT) && nargs == 2) { extern DESCR_t pat_alt(DESCR_t, DESCR_t); *out = pat_alt(args[0], args[1]); return 1; }
    L_bidjmp_6551:
    ;
    if ((_bid == BID_SNOx24PARB) && nargs == 1) { extern DESCR_t pat_arbno(DESCR_t); *out = pat_arbno(args[0]); return 1; }
    L_bidjmp_6552:
    ;
    if ((_bid == BID_SNOx24PFEN) && nargs == 1) { extern DESCR_t pat_fence_p(DESCR_t); *out = pat_fence_p(args[0]); return 1; }
    L_bidjmp_6553:
    ;
    if ((_bid == BID_SNOx24PDEF) && nargs >= 1 && nargs <= 3) {
        extern DESCR_t pat_defer_text(const char *, DESCR_t, int);
        *out = pat_defer_text(rt_sno_target_name(args[0]), nargs >= 2 ? args[1] : NULVCL, nargs >= 3 ? (int) rk_ival(args[2]) : 1);
        return 1;
    }
    L_bidjmp_6554:
    ;
    if ((_bid == BID_SNOx24MKPAT) && nargs >= 1) {
        extern void *rt_proc_get_fn(const char *name);
        const char *nm = VARVAL_fn(args[0]);
        if (!nm) nm = "";
        void *pf = rt_proc_get_fn(nm);
        if (!pf) { fprintf(stderr, "[SNO] SNO$MKPAT: compiled pattern blob '%s' not registered\n", nm); *out = FAILDESCR; return 1; }
        extern void *dtp_wrap_fn_sz(void *, int64_t, int32_t);
        extern long rt_fn_frame_bytes_known(void *);
        extern long rt_fn_zstatic_known(void *);
        DESCR_t pd = {0};
        pd.v = DT_P;
        pd.slen = 0;
        pd.p = dtp_wrap_fn_sz(pf, (int64_t)rt_fn_frame_bytes_known(pf), (int32_t)rt_fn_zstatic_known(pf));
        if (nargs >= 2) {
            extern void rt_patv_freeze(void *, const char *, long);
            long _n = 0;
            if (IS_INT_fn(args[1])) _n = (long)args[1].i;
            else { const char *cs = VARVAL_fn(args[1]); _n = cs ? atol(cs) : 0; }
            if (_n > 0) rt_patv_freeze(pd.p, nm, _n);
        }
        *out = pd;
        return 1;
    }
    L_bidjmp_6563:
    ;
    if ((_bid == BID_OPSYN) && nargs >= 2) { extern DESCR_t opsyn(DESCR_t, DESCR_t, DESCR_t); *out = opsyn(args[0], args[1], nargs > 2 ? args[2] : NULVCL); return 1; }
    L_bidjmp_6567:
    ;
    if ((_bid == BID_CODE) && (nargs >= 1 && nargs <= 3)) {
        extern DESCR_t code_at(const char *, long, long);
        if (rt_name_textless(args[0])) { *out = FAILDESCR; return 1; }
        const char *cs = VARVAL_fn(args[0]);
        long _cb = (nargs >= 2 && IS_INT(args[1])) ? (long)args[1].i : 0;
        long _el = (nargs == 3 && IS_INT(args[2])) ? (long)args[2].i : 0;
        *out = cs ? code_at(cs, _cb, _el) : FAILDESCR;
        return 1;
    }
    L_bidjmp_6572:
    ;
    if ((_bid == BID_EVAL) && nargs == 1) {
        extern DESCR_t rt_call_named_proc(const char *name, DESCR_t *args, int nargs);
        DESCR_t av = args[0];
        if (rq && rq[1] == RT_RQ_EVAL_ARM && rt_eval_open_takes(av)) { rq[0] = RT_TRY_EVAL_OPEN; rq[1] = (long)(uintptr_t)args; *out = FAILDESCR; return 1; }
        if (av.v == DT_X) { extern DESCR_t rt_sno_dtx_value_rec(sno_dstar_rec_t *); *out = rt_sno_dtx_value_rec(SNO_DTX_REC(av)); return 1; }
        if (av.v != DT_P && av.v != DT_E) { extern void rt_eval_stage_leave(const char *); rt_eval_stage_leave((const char *)0); }
        if (IS_INT_fn(av) || IS_REAL_fn(av)) { *out = av; return 1; }
        if (av.v == DT_SNUL) { *out = NULVCL; return 1; }
        if (av.v == DT_S) { extern DESCR_t EVAL_fn(DESCR_t); *out = EVAL_fn(av); return 1; }
        if (av.v == DT_P || rt_name_textless(av)) { core_runtime_error(103, "eval argument is not expression"); *out = FAILDESCR; return 1; }
        if (av.v == DT_E) { extern DESCR_t EXPVAL_fn(DESCR_t); *out = EXPVAL_fn(av); return 1; }
        *out = FAILDESCR;
        return 1;
    }
    L_bidjmp_6581:
    ;
    if ((_bid == BID_VALUE) && nargs == 1) {
        extern DESCR_t NV_GET_fn(const char *);
        extern DESCR_t rt_deref(DESCR_t);
        extern int rt_dat_field_of_any_live(const char *);
        if (rt_dat_field_of_any_live("VALUE")) { extern DESCR_t c_dat_field_get(const char *field, DESCR_t obj); *out = rtx_try_dat_field_get("VALUE", args[0]); return 1; }
        if (args[0].v == DT_N) { *out = rt_deref(args[0]); return 1; }
        const char *nm = VARVAL_fn(args[0]);
        if (!nm || !*nm) { *out = FAILDESCR; return 1; }
        *out = NV_GET_fn(nm);
        return 1;
    }
    L_bidjmp_6587:
    ;
    if ((_bid == BID_SNOx24NRET)) { extern int rt_g_ret_by_name; rt_g_ret_by_name = 1; *out = NULVCL; return 1; }
    L_bidjmp_6588:
    ;
    if ((_bid == BID_SNOx24WANTNM)) { extern int rt_g_want_name; rt_g_want_name = 1; *out = NULVCL; return 1; }
    L_bidjmp_6589:
    ;
    if ((_bid == BID_APPLY) && nargs >= 1) {
        const char *pn = (args[0].v == DT_N && args[0].slen == 0) ? args[0].s : VARVAL_fn(args[0]);
        if (!pn || !*pn) { *out = FAILDESCR; return 1; }
        extern int rt_proc_is_registered(const char *);
        extern DESCR_t rt_call_proc_descr(const char *, int);
        if (rt_proc_is_registered(pn)) {
            int na = nargs - 1;
            rt_call_args_need(na);
            for (int k = 0; k < na; k++) CALL_ARGS[k] = args[k + 1];
            *out = RT_GC_CALLBACK(rt_call_proc_descr(pn, na));
            return 1;
        }
        {
            extern int rt_g_want_name;
            extern int rt_dat_field_of_any(const char *);
            extern DESCR_t rt_field_var(const char *field, DESCR_t obj);
            if (rt_g_want_name && nargs >= 2 && args[1].v == DT_DATA && rt_dat_field_of_any(pn)) { rt_g_want_name = 0; *out = rt_field_var(pn, args[1]); return 1; }
        }
        return try_call_builtin_by_name_eval_rq(pn, args + 1, nargs - 1, out, rq);
    }
    if (nargs == 1) {
        DESCR_t a = args[0];
        if (IS_FAIL_fn(a)) { *out = FAILDESCR; return 1; }
        if (fn[0]=='+' && fn[1]=='\0') {
            if (IS_INT_fn(a)||IS_REAL_fn(a)) { *out=a; return 1; }
            const char *s=VARVAL_fn(a);
            if(!s||!*s){ *out=FAILDESCR; return 1; }
            char *e=NULL;
            long long iv=strtoll(s,&e,10);
            if(e&&*e=='\0'){ *out=INTVAL(iv); return 1; }
            double dv=strtod(s,&e);
            if(e&&*e=='\0'){ *out=REALVAL(dv); return 1; }
            *out=FAILDESCR;
            return 1;
        }
        if (fn[0]=='-' && fn[1]=='\0') { _OPCOERCE(a); *out = IS_REAL_fn(a) ? REALVAL(-a.r) : INTVAL(-a.i); return 1; }
        if (fn[0]=='*' && fn[1]=='\0') {
            if (IS_INT_fn(a)||IS_REAL_fn(a)) { *out=INTVAL(1); return 1; }
            if (IS_DATA_INST_fn(a) && a.u && a.u->type) { *out=INTVAL(a.u->type->nfields); return 1; }
            const char *s=VARVAL_fn(a);
            *out=INTVAL(s?(long long)strlen(s):0LL);
            return 1;
        }
        if (fn[0]=='!' && fn[1]=='\0') {
            extern int core_call_registered_fn(const char *, DESCR_t *, int, DESCR_t *);
            extern const char *sn4_unary_op_key(const char *);
            if (core_call_registered_fn(sn4_unary_op_key(fn), args, nargs, out)) return 1;
            core_runtime_error(29, "undefined operator referenced");
            *out=FAILDESCR;
            return 1;
        }
        if (fn[0]=='/' && fn[1]=='\0') { if (IS_INT_fn(a)) { *out=(a.i==0)?a:FAILDESCR; return 1; } if (IS_REAL_fn(a)) { *out=(a.r==0.0)?a:FAILDESCR; return 1; } *out=FAILDESCR; return 1; }
        if (fn[0]=='\\' && fn[1]=='\0') { *out=(a.v==DT_SNUL)?FAILDESCR:a; return 1; }
        if (fn[0]=='~' && fn[1]=='\0') {
            const char *s=NULL;
            int slen=-1;
            if (IS_INT_fn(a)) {
                char *nb=rt_wsb_alloc(32);
                snprintf(nb,32,"%lld",(long long)a.i);
                s=nb;
            } else if (IS_REAL_fn(a)) {
                char *nb=rt_wsb_alloc(64);
                icon_real_str(a.r,nb,64);
                s=nb;
            } else {
                s=VARVAL_fn(a);
                if (IS_CSET_fn(a)) slen=(int)descr_slen(a);
            }
            if(!s) s="";
            if (slen<0) slen=(int)strlen(s);
            unsigned char in_set[256]={0};
            for (int i=0;i<slen;i++) in_set[(unsigned char)s[i]]=1;
            char *buf=rt_wsb_alloc(256);
            int n=0;
            for(int c=1;c<256;c++) if(!in_set[c]) buf[n++]=(char)c;
            buf[n]='\0';
            *out=STRVAL(buf);
            return 1;
        }
        if (fn[0]=='?' && fn[1]=='\0') {
            if (IS_INT_fn(a)) { *out=(a.i>0)?INTVAL((long long)(rand()%(int)a.i)+1):FAILDESCR; return 1; }
            if (IS_REAL_fn(a)) { *out=REALVAL((double)rand()/RAND_MAX*a.r); return 1; }
            const char *s=VARVAL_fn(a);
            if (s&&*s) { int n=(int)strlen(s); char *ch=rt_wsb_alloc(2); ch[0]=s[rand()%n]; ch[1]='\0'; *out=STRVAL(ch); return 1; }
            *out=FAILDESCR;
            return 1;
        }
    }
    if (nargs == 2) {
        DESCR_t l=args[0], r=args[1];
        if (IS_FAIL_fn(l)||IS_FAIL_fn(r)) { *out=FAILDESCR; return 1; }
        if (fn[0]=='+' && fn[1]=='\0') { _OPCOERCE(l); _OPCOERCE(r); *out=add(l,r); return 1; }
        if (fn[0]=='-' && fn[1]=='\0') { _OPCOERCE(l); _OPCOERCE(r); *out=sub(l,r); return 1; }
        if (fn[0]=='*' && fn[1]=='\0') { _OPCOERCE(l); _OPCOERCE(r); *out=mul(l,r); return 1; }
        if (fn[0]=='/' && fn[1]=='\0') { _OPCOERCE(l); _OPCOERCE(r); *out=DIVIDE_fn(l,r); return 1; }
        if (fn[0]=='%' && fn[1]=='\0') { _OPCOERCE(l); _OPCOERCE(r); long li=IS_INT_fn(l)?l.i:(long)l.r, ri=IS_INT_fn(r)?r.i:(long)r.r; *out=ri?INTVAL(li%ri):FAILDESCR; return 1; }
        if (fn[0]=='^' && fn[1]=='\0') {
            _OPCOERCE(l);
            _OPCOERCE(r);
            if (IS_INT_fn(l)&&IS_INT_fn(r)&&r.i>=0) { long long base=l.i, res=1; for(int k=0;k<(int)r.i;k++) res*=base; *out=INTVAL(res); return 1; }
            if (IS_INT_fn(l)&&IS_INT_fn(r)&&r.i<0) { double rv=pow((double)l.i,(double)r.i); *out=INTVAL((long long)rv); return 1; }
            double base=IS_REAL_fn(l)?l.r:(double)l.i, exp=IS_REAL_fn(r)?r.r:(double)r.i;
            *out=(DESCR_t){.v=DT_R,.r=pow(base,exp)};
            return 1;
        }
        if ((_bid == BID_x3C)) _NUMREL(<);
        if ((_bid == BID_x3Cx3D)) _NUMREL(<=);
        if ((_bid == BID_x3E)) _NUMREL(>);
        if ((_bid == BID_x3Ex3D)) _NUMREL(>=);
        if ((_bid == BID_x3D)) _NUMREL(==);
        if ((_bid == BID_x7Ex3D)) _NUMREL(!=);
        if ((_bid == BID_x3Cx3C)) _STRREL(<);
        if ((_bid == BID_x3Cx3Cx3D)) _STRREL(<=);
        if ((_bid == BID_x3Ex3E)) _STRREL(>);
        if ((_bid == BID_x3Ex3Ex3D)) _STRREL(>=);
        if ((_bid == BID_x3Dx3D)) _STRREL(==);
        if ((_bid == BID_x7Ex3Dx3D)) _STRREL(!=);
        if ((_bid == BID_x3Dx3Dx3D)) { extern int c_descr_identical(DESCR_t, DESCR_t); *out=rtx_try_descr_identical(l,r)?r:FAILDESCR; return 1; }
        if ((_bid == BID_x7Ex3Dx3Dx3D)) { extern int c_descr_identical(DESCR_t, DESCR_t); *out=rtx_try_descr_identical(l,r)?FAILDESCR:r; return 1; }
        if ((_bid == BID_x5Bx5D)) { *out=subscript_get(l,r); return 1; }
        if ((_bid == BID_x2Bx2B) || (_bid == BID_x2Dx2D) || (_bid == BID_x2Ax2A)) {
            if (l.v == DT_T && l.tbl && l.tbl->is_set && r.v == DT_T && r.tbl && r.tbl->is_set) {
                if (fn[0]=='+') *out=TABLE_VAL(set_union(l.tbl, r.tbl));
                else if (fn[1]=='-') *out=TABLE_VAL(set_diff(l.tbl, r.tbl));
                else *out=TABLE_VAL(set_inter(l.tbl, r.tbl));
                return 1;
            }
            char _lbuf[64], _rbuf[64];
            const char *la, *ra;
            if (IS_INT_fn(l)) {
                snprintf(_lbuf,sizeof _lbuf,"%lld",(long long)l.i);
                la=_lbuf;
            } else if (IS_REAL_fn(l)) {
                icon_real_str(l.r,_lbuf,sizeof _lbuf);
                la=_lbuf;
            } else {
                la=VARVAL_fn(l);
                if(!la) la="";
            }
            if (IS_INT_fn(r)) {
                snprintf(_rbuf,sizeof _rbuf,"%lld",(long long)r.i);
                ra=_rbuf;
            } else if (IS_REAL_fn(r)) {
                icon_real_str(r.r,_rbuf,sizeof _rbuf);
                ra=_rbuf;
            } else {
                ra=VARVAL_fn(r);
                if(!ra) ra="";
            }
            int lalen = IS_CSET_fn(l) ? (int)descr_slen(l) : (l.v == DT_S && l.slen) ? (int)l.slen : -1;
            if (lalen < 0) lalen = (int)strlen(la);
            int ralen = IS_CSET_fn(r) ? (int)descr_slen(r) : (r.v == DT_S && r.slen) ? (int)r.slen : -1;
            if (ralen < 0) ralen = (int)strlen(ra);
            int lblk = IS_CSET_fn(l) && l.s != NULL, rblk = IS_CSET_fn(r) && r.s != NULL;
            int outlen;
            const char *ur;
            if (fn[0]=='+') {
                ur=cset_union(la,lalen,lblk,ra,ralen,rblk,&outlen);
                *out=CSETVAL(outlen > 0 ? ur : kw_cset_empty());
            } else if (fn[1]=='-') {
                ur=cset_diff(la,lalen,lblk,ra,ralen,rblk,&outlen);
                *out=CSETVAL(outlen > 0 ? ur : kw_cset_empty());
            } else {
                ur=cset_inter(la,lalen,lblk,ra,ralen,rblk,&outlen);
                *out=CSETVAL(outlen > 0 ? ur : kw_cset_empty());
            }
            return 1;
        }
    }
#undef _OPCOERCE
#undef _NUMREL
#undef _STRREL
    { DatType *_rdt = dat_find_type(fn); if (_rdt) { *out = dat_construct_rq(_rdt, args, nargs, rq); return 1; } }
    if (script_try_call_builtin_by_name_rq(fn, args, nargs, out, rq)) return 1;
    {
        DatType *dt = dat_find_type(fn);
        if (dt && nargs <= dt->nfields) {
            DESCR_t fv[dt->nfields > 0 ? dt->nfields : 1];
            int nf = dt->nfields;
            for (int i = 0; i < nf; i++) fv[i] = (i < nargs) ? args[i] : NULVCL;
            *out = dat_construct_rq(dt, fv, nf, rq);
            return 1;
        }
    }
    {
        extern int rt_dat_field_of_any_live(const char *);
        if (nargs == 1 && rt_dat_field_of_any_live(fn)) { extern DESCR_t c_dat_field_get(const char *field, DESCR_t obj); *out = rtx_try_dat_field_get(fn, args[0]); return 1; }
    }
    if (!_dx_skip_syn) {
        extern const char *rt_builtin_synonym(const char *);
        const char *syn = rt_builtin_synonym(fn);
        if (syn) {
            if (_dx && !_dx_hit) { _dx->gen = rt_dtax_gen; _dx->len = _dxl; _dx->kind = 2; _dx->nf = 0; memcpy(_dx->dxn, fn, _dxl); _dx->ctor = 0; _dx->syn = syn; }
            return try_call_builtin_by_name_eval_rq(syn, args, nargs, out, rq);
        }
        if (_dx && !_dx_hit) { _dx->gen = rt_dtax_gen; _dx->len = _dxl; _dx->kind = 0; _dx->nf = 0; memcpy(_dx->dxn, fn, _dxl); _dx->ctor = 0; _dx->syn = 0; }
    }
    {
        extern int core_call_registered_fn(const char *, DESCR_t *, int, DESCR_t *);
        char c0 = fn[0];
        if (c0 && !((c0 >= 'A' && c0 <= 'Z') || (c0 >= 'a' && c0 <= 'z') || c0 == '_') && core_call_registered_fn(fn, args, nargs, out)) return 1;
    }
    {
        const int _ia = icn_builtin_arity(fn);
        if (_ia >= 0 && _ia != nargs) {
            DESCR_t _pa[_ia > 0 ? _ia : 1];
            for (int i = 0; i < _ia; i++) _pa[i] = (i < nargs) ? args[i] : NULVCL;
            return try_call_builtin_by_name_bl(fn, _pa, _ia, out, -1);
        }
    }
    return 0;
}
