#include <string>
#include <string.h>
#include <stdint.h>
#include <cstdio>
#include <cstdlib>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
#include "ast.h"
#include "../runtime/builtins/gen.h"
extern DESCR_t rt_call_arr_gen(const char *, DESCR_t *, int, int64_t *);
extern DESCR_t rt_call_arr_gen_strict(const char *, DESCR_t *, int, int64_t *);
extern DESCR_t rt_call_arr_bl_strict(const char *, DESCR_t *, int, int);
extern DESCR_t rt_call_arr_bl_sn4(const char *, DESCR_t *, int, int);
int bb_slot_get(IR_t * nd);
int bb_varslot_peek(const char * name);
int is_global(const char * name);
DESCR_t rt_call_arr(const char * fn, DESCR_t * args, int nargs);
DESCR_t rt_call_arr_bl(const char * fn, DESCR_t * args, int nargs, int bidlen);
extern "C" {
#include "builtin_ids.h"
#include "snobol4_system_fns.h"
#include "dtp.h"
const char * bb_ab_sym_name(const char * nm);
extern "C" void * bb_dstar_rec_addr(const char * star);
}
extern "C" int prolog_atom_intern(const char *);
extern "C" DESCR_t rt_call_bid_sn4(const char *, DESCR_t *, int, int);
extern "C" DESCR_t rt_call_name_sn4(const char *, DESCR_t *, int, int);
extern "C" void * dat_find_type(const char *);
extern "C++" std::string bb_callee_rdx(const char * fn);
extern "C++" int bb_callee_baked_kind(const char * fn, int strict);
extern "C++" const char * bb_callee_baked_sym(int k);
extern "C++" uint64_t bb_callee_baked_fp(int k);
int64_t rt_gvar_get_int(const char * name);
extern int g_gva_active;
int gva_index_of(const char * name);
DESCR_t NV_GET_fn(const char * name);
int rt_is_truthy(DESCR_t v);
int rk_is_truthy(DESCR_t v);
}
#include "x86_asm.h"
extern "C++" long bid_bake_of(const char * fn);
extern "C++" const char * sn4_byname_sym(const char * fn, int strict);
extern "C++" uint64_t sn4_byname_fp(const char * fn, int strict);
#define RO_SEAL_ADDR(n, sym, addr) \
    (x86("def", L(n)) \
   + x86(".quad", (sym), (uint64_t)(uintptr_t)(addr)))
#define RO_SEAL_STR(n, s) \
    (x86("def", L(n)) \
   + x86(".quad", LS(n), (s)) \
   + x86("label", LS(n)) \
   + x86(".string", (s)))
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define zoff(nd) ((nd) ? zls_off((nd)) : -1)
#define bcbn_baked_kind(fn, strict) (((fn) && (fn)[0] && !x86_is_scan_builtin_name((fn)) && strcmp((fn), "tab") && strcmp((fn), "move")) ? bb_callee_baked_kind((fn), (strict)) : 0)
extern std::string bb_call_proc_staged_str(IR_t *);
extern std::string bb_call_fn_str(IR_t *);
extern std::string bb_call_bool_str(IR_t *);
std::string marshal_call_arg(IR_t * lf, IR_graph_t * sg, int aoff, IR_t * owner, int idx);
std::string marshal_call_arg(IR_t * lf, IR_graph_t * sg, int aoff, IR_t * owner, int idx) {
    if (owner && owner == _.node && idx >= 0 && idx < _.op_arg_slot_n && _.op_arg_slot[idx] >= 0) {
        int ps = _.op_arg_slot[idx];
        if (ps == aoff) return x86("comment", std::string("marshal arg") + std::to_string(idx) + " = direct: the producer was granted the argv slot [zr+" + std::to_string(aoff)
            + "], nothing to copy");
        std::string s = x86("comment", std::string("marshal arg") + std::to_string(idx)
                          + " = producer-box slot [zr+" + std::to_string(ps) + "] -> [zr+" + std::to_string(aoff) + "]");
        s += x86("mov", "rax", FRQ(ps));
        s += x86("mov", FRQ(aoff), "rax");
        s += x86("mov", "rax", FRQ(ps + 8));
        s += x86("mov", FRQ(aoff + 8), "rax");
        return s;
    }
    if (!lf) return std::string();
    if (lf->op == IR_LIT_INTEGER) {
        std::string s;
        s += x86("comment", std::string("marshal arg") + std::to_string(idx) + " = LIT_I -> [zr+" + std::to_string(aoff) + "]");
        s += x86("mov", FRQ(aoff), (long)DT_I);
        s += x86_movabs_r64("rax", (uint64_t)IR_LIT(lf).ival);
        s += x86("mov", FRQ(aoff + 8), "rax");
        return s;
    }
    if (lf->op == IR_LIT_REAL) {
        uint64_t bits; double d = IR_LIT(lf).dval; memcpy(&bits, &d, 8);
        std::string s;
        s += x86("comment", std::string("marshal arg") + std::to_string(idx) + " = LIT_F -> [zr+" + std::to_string(aoff) + "]");
        s += x86("mov", FRQ(aoff), (long)DT_R);
        s += x86_movabs_r64("rax", bits);
        s += x86("mov", FRQ(aoff + 8), "rax");
        return s;
    }
    if (lf->op == IR_LIT_ATOM) {
        std::string s;
        s += x86("comment", std::string("marshal arg") + std::to_string(idx) + " = LIT_ATOM (compile-time atom id) -> [zr+" + std::to_string(aoff) + "]");
        s += x86("mov", FRQ(aoff), (long)DT_PLATOM);
        s += x86_movabs_r64("rax", (uint64_t)(unsigned)prolog_atom_intern(IR_LIT(lf).sval ? IR_LIT(lf).sval : ""));
        s += x86("mov", FRQ(aoff + 8), "rax");
        return s;
    }
    if (lf->op == IR_LIT_STRING && lf->seal == IR_SEAL_DSTAR_REF) {
        int nseal = idx * 2, nskip = idx * 2 + 1;
        std::string star = std::string("*") + (IR_LIT(lf).sval ? IR_LIT(lf).sval : "");
        std::string lbl = std::string(".Ldstar_") + bb_ab_sym_name(star.c_str());
        std::string s;
        s += x86("comment", std::string("marshal arg") + std::to_string(idx) + " = DSTAR-REF (the unevaluated expression's star record, baked) -> [zr+" + std::to_string(aoff) + "]");
        s += x86("mov", FRQ(aoff), (long)DT_I);
        s += x86("mov", "rax", ROQ(nseal));
        s += x86("mov", FRQ(aoff + 8), "rax");
        s += x86_jmp_id(nskip);
        s += RO_SEAL_ADDR(nseal, lbl.c_str(), bb_dstar_rec_addr(star.c_str()));
        s += x86_deflabel_id(nskip);
        return s;
    }
    if (lf->op == IR_LIT_STRING && lf->seal == IR_SEAL_THUNK_REF) {
        int nseal = idx * 2, nskip = idx * 2 + 1;
        std::string lbl = std::string(".Lthk_") + bb_ab_sym_name(IR_LIT(lf).sval ? IR_LIT(lf).sval : "");
        std::string s;
        s += x86("comment", std::string("marshal arg") + std::to_string(idx) + " = THUNK-REF (the compiled pattern's record, baked) -> [zr+" + std::to_string(aoff) + "]");
        s += x86("mov", FRQ(aoff), (long)DT_I);
        s += x86("mov", "rax", ROQ(nseal));
        s += x86("mov", FRQ(aoff + 8), "rax");
        s += x86_jmp_id(nskip);
        s += RO_SEAL_ADDR(nseal, lbl.c_str(), bb_thunk_rec_addr(IR_LIT(lf).sval));
        s += x86_deflabel_id(nskip);
        return s;
    }
    if (lf->op == IR_LIT_STRING) {
        int nseal = idx * 2, nskip = idx * 2 + 1;
        std::string s;
        s += x86("comment", std::string("marshal arg") + std::to_string(idx)
           + " = LIT_S (string REG-RO sealed in-band) -> [zr+" + std::to_string(aoff) + "]");
        s += x86("mov", FRQ(aoff), (long)DT_S);
        s += x86("mov", "rax", ROQ(nseal));
        s += x86("mov", FRQ(aoff + 8), "rax");
        s += x86_jmp_id(nskip);
        s += RO_SEAL_STR(nseal, IR_LIT(lf).sval ? IR_LIT(lf).sval : "");
        s += x86_deflabel_id(nskip);
        return s;
    }
    if ((lf->op == IR_CALL && (IR_LIT(lf).dval == 2.0 || IR_LIT(lf).dval == 3.0)) || ir_is_call_kind(lf->op)) {
        int staged = (lf->op == IR_CALL_PROC_STAGED || lf->op == IR_PROC_GEN);
        if (owner && owner == _.node && staged && bb_slot_get(lf) >= 0) {
            int ps = bb_slot_get(lf); std::string s = x86("comment", std::string("marshal arg") + std::to_string(idx)
                                      + " = spine call-result slot [zr+" + std::to_string(ps) + "] -> [zr+" + std::to_string(aoff) + "]");
            s += x86("mov", "rax", FRQ(ps));
            s += x86("mov", FRQ(aoff), "rax");
            s += x86("mov", "rax", FRQ(ps + 8));
            s += x86("mov", FRQ(aoff + 8), "rax");
            return s;
        } return x86_bomb("bb_call: call-in-arith/marshal-arg via the PARKED marshal_single_call trampoline (dead at NCB-1b, 0/592 sweep). "
                        "If you are seeing this, LOWER routed a shape here that it did not before: reuse the NCB-1b bcps_det_arm window, "
                        "do NOT resurrect the trampoline. See bb_call.marshal-single-call-parked-e49b25db.cpp");
    }
    if (lf->op == IR_VAR && IR_LIT(lf).sval && IR_LIT(lf).sval[0] != '&' && is_global(IR_LIT(lf).sval))
        return x86_bomb("bb_call marshal: a global VAR argument with no producer slot reached the NV_GET road, retired 2026-09-26 on a "
            "plant proof (0 of 4693 corpus sources; every global argument arrives slotted)");
    {
        int is_local_var = (lf->op == IR_VAR && IR_LIT(lf).sval && IR_LIT(lf).sval[0] != '&' && !is_global(IR_LIT(lf).sval));
        int ps = is_local_var ? -1 : bb_slot_get(lf);
        if (ps < 0 && !is_local_var) ps = zoff(lf);
        if (ps >= 0) {
            if (ps == aoff) return x86("comment", std::string("marshal arg") + std::to_string(idx) + " = direct: the nested producer was granted the argv slot [zr+" + std::to_string(aoff)
                + "], nothing to copy");
            std::string s = x86("comment", std::string("marshal arg") + std::to_string(idx)
                              + " = nested producer-box slot [zr+" + std::to_string(ps) + "] -> [zr+" + std::to_string(aoff) + "]");
            s += x86("mov", "rax", FRQ(ps));
            s += x86("mov", FRQ(aoff), "rax");
            s += x86("mov", "rax", FRQ(ps + 8));
            s += x86("mov", FRQ(aoff + 8), "rax");
            return s;
        }
    }
    {
        int voff = bb_varslot_peek(IR_LIT(lf).sval ? IR_LIT(lf).sval : "");
        if (voff < 0) return x86_bomb("bb_call marshal: IR_VAR arg names a local with no LOWER-granted varslot (TE-4: grant in ir_drive_slot_assign)");
        std::string s;
        s += x86("comment", std::string("marshal arg") + std::to_string(idx) + " = varslot [zr+" + std::to_string(voff) + "] -> [zr+" + std::to_string(aoff) + "]");
        s += x86("mov", "rax", FRQ(voff));
        s += x86("mov", FRQ(aoff), "rax");
        s += x86("mov", "rax", FRQ(voff + 8));
        s += x86("mov", FRQ(aoff + 8), "rax");
        return s;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_leaf_inline_known(const char * fn, int narg);
std::string pl_leaf_inline_arm(const char * fn, int narg, int argbase, int resoff, IR_t * first_operand);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_call_byname_str(IR_t * pBB) {
    const char * fn = _.op_sval ? _.op_sval : "";
    int64_t narg = _.op_ival;
    IR_graph_t ** subs = (IR_graph_t **)(intptr_t) _.op_counter;
    if (_.op_zres) {
        std::string s = x86_alpha()
                      + x86("comment", std::string("BOX CALL ZD-7 byname ") + fn + "(...) -> rt_call_arr [ZD: args from ZOPQ, result to ZRES]");
        if (narg > 0) {
            s += x86("sub", "rsp", (long)(narg * 16));
            for (int i = 0; i < (int)narg; i++) {
                s += x86("mov", "rax", ZOPQ(i, (int)narg * 16 + 0));
                s += x86("mov", x86_zref(i * 16 + 0, 1), "rax");
                s += x86("mov", "rax", ZOPQ(i, (int)narg * 16 + 8));
                s += x86("mov", x86_zref(i * 16 + 8, 1), "rax");
            }
        }
        if (int _bk = bcbn_baked_kind(fn, _.op_strict)) {
            s += x86("comment", (std::string(_bk == 2 ? "CALLEE CALL " : "FIELD CALL ") + fn + " -> " + bb_callee_baked_sym(_bk) + " with its record baked (no name, no lookup)").c_str());
            if (narg > 0) s += x86("lea", "rdi", RDQ("rsp", 0));
            else s += x86("xor", "edi", "edi");
            s += x86("mov32", "esi", (long)narg);
            s += bb_callee_rdx(fn);
            if (_bk == 2) {
                s += x86("lea", "rcx", RDQ("rsp", (int)narg * 16));
                s += bb_glue_callee_try_enter(100, 108, 29);
                s += x86_rsp_load64("rax", (int)narg * 16) + x86_rsp_load64("rdx", (int)narg * 16 + 8);
                s += x86_deflabel_id(29) + x86_rt_gc_poll_res();
            } else {
            s += x86("call", bb_callee_baked_sym(_bk), bb_callee_baked_fp(_bk));
            s += x86_rt_gc_poll_res();
            }
        } else {
        {
            std::string fl = std::string(".L") + x86_boxkind() + "_bynamefnzd" + std::to_string((long long)_.nid);
            bb_label_t * _dm = emit_label_intern((fl + "$def").c_str());
            if (!_dm || !bb_label_defined(_dm)) { if (_dm) _dm->offset = 0;
            s += x86("directive", ".section .rodata");
            s += x86("directive", (fl + ": .string \"" + fn + "\"").c_str());
            s += x86("directive", ".section .text");
            s += x86("directive", ".intel_syntax noprefix"); }
            s += x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t)fn, fl.c_str());
        }
        if (narg > 0) s += x86("lea", "rsi", RDQ("rsp", 0));
        else s += x86("xor", "esi", "esi");
        s += x86("mov32", "edx", (long)narg);
        s += x86("mov32", "ecx", bid_bake_of(fn));
        s += x86("call", sn4_byname_sym(fn, _.op_strict), sn4_byname_fp(fn, _.op_strict));
        s += x86_rt_gc_poll_res();
        }
        if (narg > 0) s += x86("add", "rsp", (long)(narg * 16));
        s += x86("cmp", "al", (long)DT_FAIL);
        s += x86_omega("je");
        s += x86("note", ZRESN())
           + x86("mov", ZRES(0), "rax");
        s += x86("note", ZRESN())
           + x86("mov", ZRES(8), "rdx");
        s += x86_gamma();
        s += x86_beta_trampoline();
        return s;
    }
    int resoff = zoff(_.node);
    if (resoff < 0) return x86_alpha() + x86_bomb("bb_call_byname: no LOWER slot grant (TMP-ERADICATE)");
    if (_.node && (int)narg > _.node->n_operands) return x86_alpha() + x86_bomb("bb_call_byname: arg count exceeds LOWER grant (TMP-ERADICATE)");
    int argbase = zls_argv_off(pBB); if (argbase < 0) argbase = resoff + 16;
    std::string fl = std::string(".L") + x86_boxkind() + "_bynamefn" + std::to_string((long long)_.nid);
    std::string s = x86_alpha()
        + x86("comment", std::string("BOX CALL ") + fn + "(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]");
    for (int i = (int)narg - 1; i >= 0; i--)
        s += marshal_call_arg(subs && subs[i] ? subs[i]->entry : NULL, subs && subs[i] ? subs[i] : NULL, argbase + i * 16, _.node, i);
    { std::string arm = pl_leaf_inline_arm(fn, (int)narg, argbase, resoff, subs && subs[0] ? subs[0]->entry : (IR_t *)0); if (!arm.empty()) return s + arm; }
    bool scansync = x86_is_scan_builtin_name(fn);
    bool curmov = fn && (!strcmp(fn, "tab") || !strcmp(fn, "move"));
    int dsave = argbase + 16 * (int)narg;
    if (curmov) s += x86("mov", FRQ(dsave), "r14");
    if (scansync) s += x86_scan_sync_out_force();
    if (int _bk = bcbn_baked_kind(fn, _.op_strict)) {
    s += x86("comment", (std::string(_bk == 2 ? "CALLEE CALL " : "FIELD CALL ") + fn + " -> " + bb_callee_baked_sym(_bk) + " with its record baked (no name, no lookup)").c_str());
    s += x86("lea", "rdi", FRQ(argbase));
    s += x86("mov32", "esi", (long)narg);
    s += bb_callee_rdx(fn);
    if (_bk == 2) {
    s += x86("lea", "rcx", FRQ(resoff));
    s += bb_glue_callee_try_enter(100, 108, 29);
    s += x86("mov", "rax", FRQ(resoff))
       + x86("mov", "rdx", FRQ(resoff + 8));
    s += x86_deflabel_id(29);
    } else {
    s += x86("rtcc_wb");
    s += x86("call_bare", bb_callee_baked_sym(_bk), bb_callee_baked_fp(_bk));
    s += x86("rtcc_rl");
    }
    s += x86("mov", FRQ(resoff), "rax");
    s += x86("mov", FRQ(resoff + 8), "rdx");
    if (scansync) s += x86_scan_sync_in_rr_force();
    s += x86_rt_gc_poll();
    } else {
    { bb_label_t * _dm = emit_label_intern((fl + "$def").c_str());
      if (!_dm || !bb_label_defined(_dm)) { if (_dm) _dm->offset = 0;
    s += x86("directive", ".section .rodata")
       + x86("directive", (fl + ": .string \"" + fn + "\"").c_str())
       + x86("directive", ".section .text")
       + x86("directive", ".intel_syntax noprefix"); } }
    s += x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t)fn, fl.c_str());
    s += x86("lea", "rsi", FRQ(argbase));
    s += x86("mov32", "edx", (long)narg);
    s += x86("rtcc_wb");
    s += x86("mov32", "ecx", bid_bake_of(fn));
    s += x86("call_bare", sn4_byname_sym(fn, _.op_strict), sn4_byname_fp(fn, _.op_strict));
    s += x86("rtcc_rl");
    s += x86("mov", FRQ(resoff), "rax");
    s += x86("mov", FRQ(resoff + 8), "rdx");
    if (scansync) s += x86_scan_sync_in_rr_force();
    s += x86_rt_gc_poll();
    }
    s += x86("cmp", "al", (long)DT_FAIL);
    s += x86_omega("je");
    s += x86_gamma();
    s += x86_beta();
    if (curmov) s += x86("mov", "r14", FRQ(dsave)) + x86_scan_sync_out_force();
    s += x86_omega();
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_call_byname_gen_str(IR_t * pBB) {
    const char * fn = _.op_sval ? _.op_sval : "";
    int64_t narg = _.op_ival;
    IR_graph_t ** subs = (IR_graph_t **)(intptr_t) _.op_counter;
    int resoff = zoff(_.node);
    if (resoff < 0) return x86_alpha() + x86_bomb("bb_call_byname_gen: no LOWER slot grant (TMP-ERADICATE)");
    if (_.node && (int)narg > _.node->n_operands) return x86_alpha() + x86_bomb("bb_call_byname_gen: arg count exceeds LOWER grant (TMP-ERADICATE)");
    int argbase = zls_argv_off(pBB); if (argbase < 0) argbase = resoff + 16;
    int genoff = resoff + 16 * (1 + (int)narg);
    std::string fl = std::string(".L") + x86_boxkind() + "_bynamegenfn" + std::to_string((long long)_.nid);
    std::string s = x86_alpha()
        + x86("comment", std::string("BOX CALL_GEN ") + fn + "(...) -> rt_call_arr_gen by-name [four-port generator; alpha zeroes resume cell, beta re-pumps invoke with persisted cell]");
    for (int i = (int)narg - 1; i >= 0; i--)
        s += marshal_call_arg(subs && subs[i] ? subs[i]->entry : NULL, subs && subs[i] ? subs[i] : NULL, argbase + i * 16, _.node, i);
    s += x86("mov", FRQ(genoff), (long)0);
    bool scansync = x86_is_scan_builtin_name(fn);
    if (scansync) s += x86_scan_sync_out_force();
    s += x86("def", L(60));
    { bb_label_t * _dm = emit_label_intern((fl + "$def").c_str());
      if (!_dm || !bb_label_defined(_dm)) { if (_dm) _dm->offset = 0;
    s += x86("directive", ".section .rodata")
       + x86("directive", (fl + ": .string \"" + fn + "\"").c_str())
       + x86("directive", ".section .text")
       + x86("directive", ".intel_syntax noprefix"); } }
    s += x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t)fn, fl.c_str());
    s += x86("lea", "rsi", FRQ(argbase));
    s += x86("mov32", "edx", (long)narg);
    s += x86("lea", "rcx", FRQ(genoff));
    s += x86("rtcc_wb");
    s += x86("call_bare", (_.op_strict ? "rt_call_arr_gen_strict" : "rt_call_arr_gen"), TEMPLATE_FN_ADDR((_.op_strict ? rt_call_arr_gen_strict : rt_call_arr_gen)));
    s += x86_rt_gc_poll_res();
    s += x86("rtcc_rl");
    s += x86("mov", FRQ(resoff), "rax");
    s += x86("mov", FRQ(resoff + 8), "rdx");
    s += x86("cmp", "al", (long)DT_FAIL);
    s += x86_omega("je");
    s += x86_gamma();
    s += x86_beta();
    s += x86("jmp", L(60));
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_call(IR_t * pBB) {
    switch (_.op_call_route) {
        case CALL_ROUTE_BYNAME: return bb_call_byname_str(pBB);
        case CALL_ROUTE_BYNAME_GEN: return bb_call_byname_gen_str(pBB);
        case CALL_ROUTE_DVAL2_BOMB: return x86_alpha() + x86_bomb("CALL dval=2 descr-chain arm aborted per LANGUAGE-BLIND rule");
        case CALL_ROUTE_PROC_STAGED: return bb_call_proc_staged_str(pBB);
        case CALL_ROUTE_RK_BOOL_SLOT: return bb_call_bool_str(pBB);
        case CALL_ROUTE_FN: return bb_call_fn_str(pBB);
        default: break;
    }
    fprintf(stderr, "[IBB] FATAL bb_call: unsupported call shape fn='%s'\n", _.op_sval ? _.op_sval : "");
    abort();
    return std::string();
}
