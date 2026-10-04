#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
extern DESCR_t rt_call_arr_bl_strict(const char *, DESCR_t *, int, int);
extern DESCR_t rt_call_arr_bl_sn4(const char *, DESCR_t *, int, int);
#include "bb_template_common.h"
#include "bb_templates.h"
DESCR_t rt_call_arr(const char * fn, DESCR_t * args, int nargs);
DESCR_t rt_call_arr_bl(const char * fn, DESCR_t * args, int nargs, int bidlen);
extern "C" int zls_g_resume(const IR_graph_t *);
extern "C" {
#include "builtin_ids.h"
#include "snobol4_system_fns.h"
}
static int bid_bake_on(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_BID_BAKE"); v = (e && *e == '0') ? 0 : 1; } return v; }
static long bid_bake_of(const char * fn) { if (!bid_bake_on() || !fn) return -1L; size_t n = strlen(fn); if (n > 0xFFFFu) return -1L;
    return (long)(((unsigned long)n << 16) | (unsigned long)(unsigned)bid_of(fn, (unsigned)n) | (sn4_is_system_fn(fn) ? (unsigned long)BID_BAKE_SYSFN : 0UL) | ((sn4_direct_on() && sn4_is_leaf_fn(fn)) ? (unsigned long)BID_BAKE_LEAF : 0UL)); }
extern "C" DESCR_t rt_call_bid_sn4(const char *, DESCR_t *, int, int);
extern "C" DESCR_t rt_call_name_sn4(const char *, DESCR_t *, int, int);
static int sn4_byname_kind(const char * fn, int strict) { long bw = bid_bake_of(fn); if (strict != 1 && bw >= 0 && (bw & BID_BAKE_LEAF)) return 1; if (strict == 2 && bw >= 0 && !(bw & BID_BAKE_SYSFN) && (bw & BID_BAKE_MASK) == 0 && sn4_direct_on()) return 2; if (strict == 2) return 3; if (strict) return 4; return 5; }
static const char * sn4_byname_sym(const char * fn, int strict) { switch (sn4_byname_kind(fn, strict)) { case 1: return "rt_call_bid_sn4"; case 2: return "rt_call_name_sn4"; case 3: return "rt_call_arr_bl_sn4"; case 4: return "rt_call_arr_bl_strict"; default: return "rt_call_arr_bl"; } }
static uint64_t sn4_byname_fp(const char * fn, int strict) { switch (sn4_byname_kind(fn, strict)) { case 1: return (uint64_t)(uintptr_t)(void *)rt_call_bid_sn4; case 2: return (uint64_t)(uintptr_t)(void *)rt_call_name_sn4; case 3: return (uint64_t)(uintptr_t)(void *)rt_call_arr_bl_sn4; case 4: return (uint64_t)(uintptr_t)(void *)rt_call_arr_bl_strict; default: return (uint64_t)(uintptr_t)(void *)rt_call_arr_bl; } }
int bb_slot_get(IR_t * nd);
void bb_slot_register(IR_t * nd, int off);
}
#include "x86_asm.h"
std::string marshal_call_arg(IR_t * lf, IR_graph_t * sg, int aoff, IR_t * owner, int idx);
void * dop_direct_fp(const char * fn, int64_t narg, const char ** sym);
extern "C" {
#include "dtp.h"
DESCR_t rt_call_fld_sn4(DESCR_t *args, int nargs, sno_callee_rec_t *r);
DESCR_t rt_call_callee_sn4(DESCR_t *args, int nargs, sno_callee_rec_t *r);
int rt_dat_field_of_any(const char *name);
void * dat_find_type(const char *name);
const char * bb_ab_sym_name(const char * nm);
}
int bb_callee_baked_kind(const char * fn, int strict) { if (!fn || !fn[0] || sn4_byname_kind(fn, strict) != 2) return 0; return (rt_dat_field_of_any(fn) && !dat_find_type(fn)) ? 1 : 2; }
const char * bb_callee_baked_sym(int k) { return k == 2 ? "rt_call_callee_sn4" : "rt_call_fld_sn4"; }
uint64_t bb_callee_baked_fp(int k) { return k == 2 ? (uint64_t)(uintptr_t)(void *)rt_call_callee_sn4 : (uint64_t)(uintptr_t)(void *)rt_call_fld_sn4; }
std::string bb_callee_rdx(const char * fn) { std::string lbl = std::string(".Lcallee_") + bb_ab_sym_name(fn); return x86("lea", "rdx", "[rip + __]", (uint64_t)(uintptr_t)bb_callee_rec_addr(fn), lbl.c_str()); }
std::string pl_leaf_inline_arm(const char * fn, int narg, int argbase, int resoff, IR_t * first_operand);
std::string pl_leaf_zd_cold(const char * fn, int narg);
int pl_leaf_inline_known(const char * fn, int narg);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" {
typedef struct { long fn; long how; } rk_next_t;
extern rk_next_t rk_method_open(DESCR_t * args, int nargs);
extern DESCR_t rk_method_land_γ(DESCR_t frame0, long word);
extern DESCR_t rk_method_land_ω(long word);
typedef struct { long fn; long how; } rt_call_next_t;
extern rt_call_next_t rt_apply_open(DESCR_t * args, int nargs);
extern DESCR_t rt_apply_land_γ(DESCR_t frame0, long word);
extern DESCR_t rt_apply_land_ω(long word);
typedef struct { long fn; long how; } rt_eval_next_t;
extern rt_eval_next_t rt_eval_open(DESCR_t * args, int nargs);
extern DESCR_t rt_eval_land(long word);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bcfn_opens_as_method(const char * fn, int nargs) { return (fn && nargs >= 2 && !strcmp(fn, "meth_call")) ? 1 : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bcfn_method_open_enter(int base, int decl_id, int join_id) {
    return  x86("call", "rk_method_open", (uint64_t)(uintptr_t)(void *)rk_method_open)
         + x86_rt_gc_poll_rec_sigma_word(1)
         + x86("test", "rax", "rax")
         + x86_jcc_id("jz", decl_id)
         + bb_glue_enter_c2bb(base, base + 5, base + 6)
         + x86_deflabel_id(base + 5)
         + x86("call", "rk_method_land_γ", (uint64_t)(uintptr_t)(void *)rk_method_land_γ)
         + x86_rt_gc_poll_rec_res()
         + x86_jmp_id(join_id)
         + x86_deflabel_id(base + 6)
         + x86("call", "rk_method_land_ω", (uint64_t)(uintptr_t)(void *)rk_method_land_ω)
         + x86_rt_gc_poll_rec_res()
         + x86_jmp_id(join_id);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bcfn_opens_as_apply(const char * fn, int nargs) { return (fn && nargs >= 1 && !strcmp(fn, "APPLY")) ? 1 : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bcfn_apply_open_enter(int base, int decl_id, int join_id) {
    return  x86("call", "rt_apply_open", (uint64_t)(uintptr_t)(void *)rt_apply_open)
         + x86_rt_gc_poll_rec_sigma_word(1)
         + x86("test", "rax", "rax")
         + x86_jcc_id("jz", decl_id)
         + bb_glue_enter_c2bb(base, base + 5, base + 6)
         + x86_deflabel_id(base + 5)
         + x86("call", "rt_apply_land_γ", (uint64_t)(uintptr_t)(void *)rt_apply_land_γ)
         + x86_rt_gc_poll_rec_res()
         + x86_jmp_id(join_id)
         + x86_deflabel_id(base + 6)
         + x86("call", "rt_apply_land_ω", (uint64_t)(uintptr_t)(void *)rt_apply_land_ω)
         + x86_rt_gc_poll_rec_res()
         + x86_jmp_id(join_id);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define BCFN_OPENS_AS_EVAL(fn, nargs, strict) ((fn) && (nargs) == 1 && (strict) == 2 && !strcmp((fn), "EVAL"))
#define BCFN_EVAL_OPEN_ENTER(base, decl_id, join_id) ( x86("call", "rt_eval_open", (uint64_t)(uintptr_t)(void *)rt_eval_open) \
         + x86_rt_gc_poll_rec_sigma_word(1) \
         + x86("test", "rax", "rax") \
         + x86_jcc_id("jz", (decl_id)) \
         + bb_glue_enter_chain_ret((base) + 1) \
         + x86("call", "rt_eval_land", (uint64_t)(uintptr_t)(void *)rt_eval_land) \
         + x86_rt_gc_poll_rec_res() \
         + x86_jmp_id((join_id)) )
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bcfn_result_slot(IR_t * nd) {
    { int _s = nd ? zls_off(nd) : -1; if (_s >= 0) { if (bb_slot_get(nd) < 0) bb_slot_register(nd, _s); return _s; } }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_call_fn_str(IR_t * pBB) {
    int polled_in_arm = 0;
    const char * fn = _.op_sval ? _.op_sval : "";
    int nargs = (int) _.op_ival;
    if (_.op_zres) {
        std::string s = x86_alpha()
                      + x86("comment", std::string("BOX IR_CALL ZD-7 ") + fn + "(...) -> rt_call_arr [ZD: args from ZOPQ, result to ZRES]");
        if (nargs > 0) {
            s += x86("sub", "rsp", (long)(nargs * 16));
            for (int i = 0; i < nargs; i++) {
                s += x86("mov", "rax", ZOPQ(i, nargs * 16 + 0));
                s += x86("mov", x86_zref(i * 16 + 0, 1), "rax");
                s += x86("mov", "rax", ZOPQ(i, nargs * 16 + 8));
                s += x86("mov", x86_zref(i * 16 + 8, 1), "rax");
            }
        }
        const char * zdsym = 0; void * zdfp = dop_direct_fp(fn, (int64_t)nargs, &zdsym);
        if (!zdfp && !pl_leaf_inline_known(fn, nargs) && _.node && _.node->seal == IR_SEAL_CALL_DET_LEAF) return x86_alpha() + x86_bomb("bb_call_fn: the lowerer sealed this call as a det leaf and dop_direct_fp does not know the callee -- the narrowing is REFUSED for a callee the registry does not know");
        if (zdfp) {
            s += x86("comment", (std::string("PL-REGAIN-2 direct det leaf under ZD: ") + zdsym + " (no by-name dispatch)").c_str());
            s += x86_reg_disp32_lea64("rdi", "rsp", 0);
            s += x86("mov32", "esi", (long)nargs);
            s += x86("call", zdsym, (uint64_t)(uintptr_t)zdfp);
            s += x86_rt_gc_poll_res();
            polled_in_arm = 1;
        } else if (pl_leaf_inline_known(fn, nargs)) {
            s += pl_leaf_zd_cold(fn, nargs);
            polled_in_arm = 1;
        } else {
        int _mopen = bcfn_opens_as_method(fn, nargs);
        if (_mopen) { s += x86_reg_disp32_lea64("rdi", "rsp", 0) + x86("mov32", "esi", (long)nargs) + bcfn_method_open_enter(20, 28, 29); s += x86_deflabel_id(28); }
        int _aopen = bcfn_opens_as_apply(fn, nargs);
        if (_aopen) { s += x86_reg_disp32_lea64("rdi", "rsp", 0) + x86("mov32", "esi", (long)nargs) + bcfn_apply_open_enter(60, 68, 29); s += x86_deflabel_id(68); }
        if (BCFN_OPENS_AS_EVAL(fn, nargs, _.op_strict)) {
            s += x86("lea", "rdi", RDQ("rsp", 0));
            s += x86("mov32", "esi", (long)nargs);
            s += BCFN_EVAL_OPEN_ENTER(80, 88, 29);
            s += x86_deflabel_id(88);
        }
        int _bk = bb_callee_baked_kind(fn, _.op_strict);
        if (_bk && !_mopen && !_aopen && !BCFN_OPENS_AS_EVAL(fn, nargs, _.op_strict)) {
            s += x86("comment", (std::string(_bk == 2 ? "CALLEE CALL " : "FIELD CALL ") + fn + " -> " + bb_callee_baked_sym(_bk) + " with its record baked (no name, no lookup)").c_str());
            if (nargs > 0) s += x86_reg_disp32_lea64("rdi", "rsp", 0);
            else           s += x86("xor", "edi", "edi");
            s += x86("mov32", "esi", (long)nargs);
            s += bb_callee_rdx(fn);
            s += x86("call", bb_callee_baked_sym(_bk), bb_callee_baked_fp(_bk));
            s += x86_rt_gc_poll_res();
        } else {
        {
            std::string fl = std::string(".L") + x86_boxkind() + "_rkfnzd" + std::to_string(g_flat_node_id++);
            s += x86("directive", ".section .rodata");
            s += x86("directive", (fl + ": .string \"" + fn + "\"").c_str());
            s += x86("directive", ".section .text");
            s += x86("directive", ".intel_syntax noprefix");
            s += x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t)fn, fl.c_str());
        }
        if (nargs > 0) s += x86_reg_disp32_lea64("rsi", "rsp", 0);
        else           s += x86("xor", "esi", "esi");
        s += x86("mov32", "edx", (long)nargs);
        s += x86("mov32", "ecx", bid_bake_of(fn));
        s += x86("call", sn4_byname_sym(fn, _.op_strict), sn4_byname_fp(fn, _.op_strict));
        if (!(_mopen || _aopen || BCFN_OPENS_AS_EVAL(fn, nargs, _.op_strict))) s += x86_rt_gc_poll_res();
        }
        if (_mopen || _aopen || BCFN_OPENS_AS_EVAL(fn, nargs, _.op_strict)) s += x86_deflabel_id(29) + x86_rt_gc_poll_res();
        }
        if (nargs > 0) s += x86("add", "rsp", (long)(nargs * 16));
        { int _wpop_save = _.op_wpop; int _zgpop_save = _.op_zgpop; if (_.op_sb) { _.op_wpop = 0; _.op_zgpop = 0; }
        s += x86("cmp", "al", (long)DT_FAIL);
        s += x86_omega("je");
        s += x86("note", ZRESN()) + x86("mov", ZRES(0), "rax");
        s += x86("note", ZRESN()) + x86("mov", ZRES(8), "rdx");
        if (_.op_sb) { s += x86_omega(); _.op_wpop = _wpop_save; _.op_zgpop = _zgpop_save; return s; } _.op_wpop = _wpop_save; _.op_zgpop = _zgpop_save; }
        s += x86_gamma();
        s += x86_beta_trampoline();
        return s;
    }
    int resoff = bcfn_result_slot(pBB);
    if (resoff < 0) return x86_alpha() + x86_bomb("bb_call_fn: no LOWER slot grant (TMP-ERADICATE)");
    if (_.node && nargs > _.node->n_operands) return x86_alpha() + x86_bomb("bb_call_fn: arg count exceeds LOWER grant (TMP-ERADICATE)");
    int argbase = zls_argv_off(pBB); if (argbase < 0) argbase = resoff + 16;
    IR_graph_t ** subs = (IR_graph_t **)(intptr_t) _.op_counter;
    std::string s = x86_alpha()
                  + x86("comment", std::string("BOX IR_CALL ") + fn + "(...) -> rt_call_arr [operand-marshal, FAIL->ω]");
    const char * dsym = 0; void * dfp = dop_direct_fp(fn, (int64_t)nargs, &dsym);
    if (!dfp && !pl_leaf_inline_known(fn, nargs) && _.node && _.node->seal == IR_SEAL_CALL_DET_LEAF) return x86_alpha() + x86_bomb("bb_call_fn: the lowerer sealed this call as a det leaf and dop_direct_fp does not know the callee -- the narrowing is REFUSED for a callee the registry does not know");
    for (int i = nargs - 1; i >= 0; i--)
        s += marshal_call_arg((subs && subs[i]) ? subs[i]->entry : ir_call_arg(pBB, i), (subs && subs[i]) ? subs[i] : NULL, argbase + i * 16, _.node, i);
    { std::string arm = pl_leaf_inline_arm(fn, nargs, argbase, resoff, (subs && subs[0]) ? subs[0]->entry : ir_call_arg(pBB, 0)); if (!arm.empty()) return s + arm; }
    if (dfp) {
        s += x86("comment", (std::string("PL-REGAIN-2 direct det leaf: ") + dsym + " (no by-name dispatch)").c_str());
        s += x86("lea", "rdi", FRQ(argbase));
        s += x86("mov32", "esi", (long)nargs);
        s += x86("call", dsym, (uint64_t)(uintptr_t)dfp);
        s += x86("mov", FRQ(resoff), "rax");
        s += x86("mov", FRQ(resoff + 8), "rdx");
        s += x86_rt_gc_poll();
        polled_in_arm = 1;
    } else {
        int _mopen = bcfn_opens_as_method(fn, nargs);
        if (_mopen) { s += x86("lea", "rdi", FRQ(argbase)) + x86("mov32", "esi", (long)nargs) + bcfn_method_open_enter(20, 28, 29); s += x86_deflabel_id(28); }
        int _aopen = bcfn_opens_as_apply(fn, nargs);
        if (_aopen) { s += x86("lea", "rdi", FRQ(argbase)) + x86("mov32", "esi", (long)nargs) + bcfn_apply_open_enter(60, 68, 29); s += x86_deflabel_id(68); }
        if (BCFN_OPENS_AS_EVAL(fn, nargs, _.op_strict)) {
            s += x86("lea", "rdi", FRQ(argbase));
            s += x86("mov32", "esi", (long)nargs);
            s += BCFN_EVAL_OPEN_ENTER(80, 88, 29);
            s += x86_deflabel_id(88);
        }
        int _bk = bb_callee_baked_kind(fn, _.op_strict);
        if (_bk && !_mopen && !_aopen && !BCFN_OPENS_AS_EVAL(fn, nargs, _.op_strict)) {
            s += x86("comment", (std::string(_bk == 2 ? "CALLEE CALL " : "FIELD CALL ") + fn + " -> " + bb_callee_baked_sym(_bk) + " with its record baked (no name, no lookup)").c_str());
            s += x86("lea", "rdi", FRQ(argbase));
            s += x86("mov32", "esi", (long)nargs);
            s += bb_callee_rdx(fn);
            s += x86("rtcc_wb");
            s += x86("call_bare", bb_callee_baked_sym(_bk), bb_callee_baked_fp(_bk));
            s += x86("rtcc_rl");
            s += x86("mov", FRQ(resoff), "rax");
            s += x86("mov", FRQ(resoff + 8), "rdx");
            s += x86_rt_gc_poll();
        } else {
        std::string fl = std::string(".L") + x86_boxkind() + "_rkfn" + std::to_string(g_flat_node_id++);
        s += x86("directive", ".section .rodata");
        s += x86("directive", (fl + ": .string \"" + fn + "\"").c_str());
        s += x86("directive", ".section .text");
        s += x86("directive", ".intel_syntax noprefix");
        s += x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t)fn, fl.c_str());
        s += x86("lea", "rsi", FRQ(argbase));
        s += x86("mov32", "edx", (long)nargs);
        s += x86("rtcc_wb");
        s += x86("mov32", "ecx", bid_bake_of(fn));
        s += x86("call_bare", sn4_byname_sym(fn, _.op_strict), sn4_byname_fp(fn, _.op_strict));
        s += x86("rtcc_rl");
        if (!(_mopen || _aopen || BCFN_OPENS_AS_EVAL(fn, nargs, _.op_strict))) {
            s += x86("mov", FRQ(resoff), "rax");
            s += x86("mov", FRQ(resoff + 8), "rdx");
            s += x86_rt_gc_poll();
        }
        }
        if (_mopen || _aopen || BCFN_OPENS_AS_EVAL(fn, nargs, _.op_strict)) {
            s += x86_deflabel_id(29);
            s += x86("mov", FRQ(resoff), "rax");
            s += x86("mov", FRQ(resoff + 8), "rdx");
            s += x86_rt_gc_poll();
        }
    }
    s += x86("cmp", "al", (long)DT_FAIL);
    s += x86_omega("je");
    s += x86_gamma();
    s += x86_beta();
    s += x86_omega();
    return s;
}
