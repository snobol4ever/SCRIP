#include <climits>
#include <string>
#include <string.h>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
#include "../runtime/builtins/gen.h"
extern DESCR_t rt_call_arr_gen(const char *, DESCR_t *, int, int64_t *);
extern DESCR_t rt_call_arr_gen_strict(const char *, DESCR_t *, int, int64_t *);
extern DESCR_t rt_call_pas_bi(DESCR_t * args, int nargs, long idx);
#include "dtp.h"
DESCR_t rt_call_fld_sn4(DESCR_t *args, int nargs, sno_callee_rec_t *r);
}
#include "x86_asm.h"
extern "C" {
typedef struct { long fn; long how; } rk_next_t;
extern rk_next_t rk_method_open(DESCR_t * args, int nargs, void * rec);
extern DESCR_t rk_method_land_γ(DESCR_t frame0, long word);
extern DESCR_t rk_method_land_ω(long word);
typedef struct { long fn; long how; } rt_call_next_t;
extern rt_call_next_t rt_apply_open(DESCR_t * args, int nargs);
extern DESCR_t rt_apply_land_γ(DESCR_t frame0, long word, const long * sv);
extern DESCR_t rt_apply_land_ω(long word, const long * sv);
typedef struct { long fn; long how; } rt_eval_next_t;
extern rt_eval_next_t rt_eval_open(DESCR_t * args, int nargs, void * rec);
#define BCFN_EVAL_REC 112
extern DESCR_t rt_eval_land(long word);
extern DESCR_t rt_eval_dtx_land_γ(DESCR_t frame0, long word, void * rec);
extern DESCR_t rt_eval_dtx_land_ω(long word, void * rec);
extern DESCR_t rt_call_land_γ(DESCR_t frame0, long word);
extern DESCR_t rt_call_land_ω(long word);
}
std::string elem_access_zd_fast(void);
std::string elem_access_flat_fast(int argbase, int resoff);
std::string elem_access_flat_join(void);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define CF_FN() (_.op_sval ? _.op_sval : "")
#define CF_N() ((int)_.op_ival)
#define CF_BLK() (_.op_call_blk)
#define CF_REC_CELL() (_.op_zres && _.op_call_rec >= 0)
#define CF_ARG() (_.op_call_argbase)
#define CF_RES() (_.op_call_resoff)
#define CF_REFUSED() (_.op_call_refuse != CALL_REFUSE_NONE)
#define CF_WHO() (_.op_call_route == CALL_ROUTE_FN ? 2 : _.op_call_route == CALL_ROUTE_BYNAME_GEN ? 4 : 0)
#define MARG(i) (((const marg_t *)_.op_marg.p)[(i)])
#define MAOFF(i) (CF_ARG() + 16 * (i))
#define RO_SEAL_ADDR(n, sym, addr) \
    (x86(".quad_ro", L(n), (sym), (uint64_t)(uintptr_t)(addr)))
#define RO_SEAL_STR(n, s) \
    (x86(".quad_ro", L(n), LS(n), (s)) \
   + x86("label", LS(n)) \
   + x86(".string", (s)))
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * const cf_refuse_msg[] = {
    "bb_call_byname: no LOWER slot grant (TMP-ERADICATE)", "bb_call_byname: arg count exceeds LOWER grant (TMP-ERADICATE)",
    "bb_call_fn: no LOWER slot grant (TMP-ERADICATE)", "bb_call_fn: arg count exceeds LOWER grant (TMP-ERADICATE)",
    "bb_call_byname_gen: no LOWER slot grant (TMP-ERADICATE)", "bb_call_byname_gen: arg count exceeds LOWER grant (TMP-ERADICATE)",
};
static std::string cf_refuse() {
    return IF(_.op_call_refuse == CALL_REFUSE_NO_SLOT, x86_bomb(cf_refuse_msg[CF_WHO()]))
         + IF(_.op_call_refuse == CALL_REFUSE_ARGC, x86_bomb(cf_refuse_msg[CF_WHO() + 1]))
         + IF(_.op_call_refuse == CALL_REFUSE_DET_LEAF,
               x86_bomb("bb_call_fn: the lowerer sealed this call as a det leaf and dop_direct_fp does not know the callee -- the narrowing is REFUSED for a callee the registry does not know"));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string cf_marg_slot(int i) {
    return x86("mov", "rax", FRQ(MARG(i).src))
         + x86("mov", FRQ(MAOFF(i)), "rax")
         + x86("mov", "rax", FRQ(MARG(i).src + 8))
         + x86("mov", FRQ(MAOFF(i) + 8), "rax");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string cf_marg_imm(int i, long tag) {
    return x86("mov", FRQ(MAOFF(i)), tag)
         + x86_movabs_r64("rax", MARG(i).imm)
         + x86("mov", FRQ(MAOFF(i) + 8), "rax");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string cf_marg_seal(int i, long tag, const std::string & seal) {
    return x86("mov", FRQ(MAOFF(i)), tag)
         + x86("mov", "rax", ROQ(2 * i))
         + x86("mov", FRQ(MAOFF(i) + 8), "rax")
         + x86_jmp_id(2 * i + 1)
         + seal
         + x86_deflabel_id(2 * i + 1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string cf_marg(int i) {
    return IF(MARG(i).kind == MARG_SLOT, cf_marg_slot(i))
         + IF(MARG(i).kind == MARG_LIT_I, cf_marg_imm(i, (long)DT_I))
         + IF(MARG(i).kind == MARG_LIT_F, cf_marg_imm(i, (long)DT_R))
         + IF(MARG(i).kind == MARG_LIT_ATOM, cf_marg_imm(i, (long)DT_PLATOM))
         + IF(MARG(i).kind == MARG_LIT_S, cf_marg_seal(i, (long)DT_S, RO_SEAL_STR(2 * i, MARG(i).str)))
         + IF(MARG(i).kind == MARG_DSTAR || MARG(i).kind == MARG_THUNK, cf_marg_seal(i, (long)DT_I, RO_SEAL_ADDR(2 * i, MARG(i).str, MARG(i).imm)))
         + IF(MARG(i).kind == MARG_BOMB_TRAMPOLINE,
               x86_bomb("bb_call: call-in-arith/marshal-arg via the PARKED marshal_single_call trampoline (dead at NCB-1b, 0/592 sweep). "
                        "If you are seeing this, LOWER routed a shape here that it did not before: reuse the NCB-1b bcps_det_arm window, "
                        "do NOT resurrect the trampoline. See bb_call.marshal-single-call-parked-e49b25db.cpp"))
         + IF(MARG(i).kind == MARG_BOMB_GLOBAL,
               x86_bomb("bb_call marshal: a global VAR argument with no producer slot reached the NV_GET road, retired 2026-09-26 on a "
                        "plant proof (0 of 4693 corpus sources; every global argument arrives slotted)"))
         + IF(MARG(i).kind == MARG_BOMB_VARSLOT, x86_bomb("bb_call marshal: IR_VAR arg names a local with no LOWER-granted varslot (TE-4: grant in planner_stage_graph)"));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string cf_marshal() { return FOR(0, _.op_marg_n, [](int k) { return cf_marg(_.op_marg_n - 1 - k); }); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string cf_zd_copy() {
    return IF(CF_N() > 0,
               FOR(0, CF_N(), [](int i) {
                   return emit_seq({ x86("mov", "rax", ZOPQS(i, 0, 0)),
                                     x86("mov", x86_zref(CF_BLK() + i * 16 + 0, 1), "rax"),
                                     x86("mov", "rax", ZOPQS(i, 0, 8)),
                                     x86("mov", x86_zref(CF_BLK() + i * 16 + 8, 1), "rax") }); }));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * cf_zd_cursor() { return _.op_call_dsave != INT_MIN ? RDQ("rbp", _.op_call_dsave) : x86_zref(16, 1); }
static std::string cf_zd_save() { return IF(_.op_call_curmov, x86("mov", cf_zd_cursor(), "r14")); }
static std::string cf_zd_sync() { return IF(_.op_call_scansync, x86_scan_env_store()); }
static std::string cf_zd_beta() { return _.op_beta_dead ? std::string() :
    emit_seq({ x86_deflabel(X86P_BETA), IF(_.op_call_curmov, x86("mov", "r14", cf_zd_cursor())), x86_jmp(X86P_OMEGA) }); }
static std::string cf_zd_sync_in() { return IF(_.op_call_scansync, x86_scan_env_load_rr()); }
std::string cf_zd_exit() {
    return emit_seq({ x86("cmp", "al", (long)DT_FAIL),
                      x86_omega("je"),
                      x86("note", ZRESN())
                    + x86("mov", ZRES(0), "rax"),
                      x86("note", ZRESN())
                    + x86("mov", ZRES(8), "rdx"),
                      IF(_.op_call_sb_exit, x86_omega()),
                      IF(!_.op_call_sb_exit, x86_gamma()),
                      IF(!_.op_call_sb_exit, cf_zd_beta()) });
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string cf_flat_exit() {
    return emit_seq({ x86("cmp", "al", (long)DT_FAIL),
                      x86_omega("je"),
                      x86_gamma(),
                      x86_beta(),
                      IF(_.op_call_curmov, x86("mov", "r14", FRQ(_.op_call_dsave))),
                      x86_omega() });
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string cf_pre_road() { return emit_seq({ IF(_.op_call_curmov, x86("mov", FRQ(_.op_call_dsave), "r14")), IF(_.op_call_scansync, x86_scan_env_store()) }); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string cf_fnlbl() {
    return IF(_.op_call_fnlbl_fresh,
               x86("directive", ".section .rodata")
             + x86("directive", (std::string(_.op_call_fnlbl) + ": .string \"" + CF_FN() + "\"").c_str())
             + x86("directive", ".section .text")
             + x86("directive", ".intel_syntax noprefix"))
         + x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t)CF_FN(), _.op_call_fnlbl);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string cf_join29(int open, int eval) { return IF(open, x86_deflabel_id(29) + IF(eval && !CF_REC_CELL(), x86("add", "rsp", (long)BCFN_EVAL_REC))); }
static std::string cf_sync_in() { return IF(_.op_call_scansync, x86_scan_env_load_rr()); }
static std::string cf_argv_rsi(int eval) {
    return IF(CF_N() > 0, x86("lea", "rsi", x86_zref((eval && !CF_REC_CELL() ? BCFN_EVAL_REC : 0) + CF_BLK(), 1)))
         + IF(CF_N() <= 0, x86("xor", "esi", "esi"));
}
static std::string cf_argv_rdi() {
    return IF(CF_N() > 0, x86("lea", "rdi", x86_zref(CF_BLK(), 1)))
         + IF(CF_N() <= 0, x86("xor", "edi", "edi"));
}
static std::string cf_try_enter(const std::string & out_lea) {
    return x86("movabs", "rdx", ((uint64_t)(uint32_t)_.op_call_bid << 32) | (uint64_t)(uint32_t)CF_N())
         +
             bb_glue_try_enter(_.op_call_try_sym, _.op_call_try_fp, "rt_call_land_γ", TEMPLATE_FN_ADDR(rt_call_land_γ), "rt_call_land_ω", TEMPLATE_FN_ADDR(rt_call_land_ω), 100, 108, 29, 0, 0,
             out_lea, !strcmp(_.op_call_try_sym, "rt_call_arr_bl_sn4_try"), _.op_call_rec, _.op_zres);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string cf_byname_flat(int open, int eval) {
    return emit_seq({ cf_fnlbl(),
                      x86("lea", "rsi", FRQ(CF_ARG() + (eval ? BCFN_EVAL_REC : 0))),
                      _.op_call_try
                      ? emit_seq({ x86("lea", "rcx", FRQ(CF_RES())),
                                   cf_try_enter(x86("lea", "rdi", FRQ(CF_RES()))),
                                   x86("mov", "rax", FRQ(CF_RES())),
                                   x86("mov", "rdx", FRQ(CF_RES() + 8)),
                                   IF(!open, x86_deflabel_id(29)) })
                      : emit_seq({ x86("mov32", "edx", (long)CF_N()),
                                   x86("rtcc_wb"),
                                   x86("mov32", "ecx", _.op_call_bid),
                                   x86("call_bare", _.op_call_sym, _.op_call_fp),
                                   x86("rtcc_rl") }),
                      cf_join29(open, eval),
                      x86("mov", FRQ(CF_RES()), "rax"),
                      x86("mov", FRQ(CF_RES() + 8), "rdx"),
                      cf_sync_in(),
                      x86_rt_gc_poll() });
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string cf_byname_zd(int open, int eval) {
    return emit_seq({ cf_fnlbl(),
                      cf_argv_rsi(eval),
                      _.op_call_try
                      ? emit_seq({ x86("lea", "rcx", ZRES(0)),
                                   cf_try_enter(x86("lea", "rdi", ZRES(0))),
                                   x86("mov", "rax", ZRES(0)),
                                   x86("mov", "rdx", ZRES(8)),
                                   IF(!open, x86_deflabel_id(29)) })
                      : emit_seq({ x86("mov32", "edx", (long)CF_N()),
                                   x86("mov32", "ecx", _.op_call_bid),
                                   x86("call", _.op_call_sym, _.op_call_fp) }),
                      cf_join29(open, eval),
                      cf_zd_sync_in(),
                      x86_rt_gc_poll_res(std::string(), 1) });
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string cf_baked_flat(int try_) {
    return emit_seq({ x86("lea", "rdi", FRQ(CF_ARG())),
                      x86("mov32", "esi", (long)CF_N()),
                      x86("lea", "rdx", "[rip + __]", _.op_call_rec_addr, _.op_call_rec_lbl),
                      try_
                      ? emit_seq({ x86("lea", "rcx", FRQ(CF_RES())),
                                   bb_glue_callee_try_enter(100, 108, 29, x86("lea", "rdi", FRQ(CF_RES()))),
                                   x86("mov", "rax", FRQ(CF_RES()))
                                 + x86("mov", "rdx", FRQ(CF_RES() + 8)),
                                   x86_deflabel_id(29) })
                      : emit_seq({ x86("rtcc_wb"),
                                   x86("call_bare", "rt_call_fld_sn4", TEMPLATE_FN_ADDR(rt_call_fld_sn4)),
                                   x86("rtcc_rl") }),
                      x86("mov", FRQ(CF_RES()), "rax"),
                      x86("mov", FRQ(CF_RES() + 8), "rdx"),
                      cf_sync_in(),
                      x86_rt_gc_poll() });
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string cf_baked_zd(int try_) {
    return emit_seq({ cf_argv_rdi(),
                      x86("mov32", "esi", (long)CF_N()),
                      x86("lea", "rdx", "[rip + __]", _.op_call_rec_addr, _.op_call_rec_lbl),
                      try_
                      ? emit_seq({ x86("lea", "rcx", x86_zref(0, 1)),
                                   bb_glue_callee_try_enter(100, 108, 29, x86("lea", "rdi", x86_zref(0, 1)), _.op_call_rec, _.op_zres),
                                   x86("mov", "rax", ZRES(0)) + x86("mov", "rdx", ZRES(8)),
                                   x86_deflabel_id(29) + x86_rt_gc_poll_res(std::string(), 1) })
                      : emit_seq({ x86("call", "rt_call_fld_sn4", TEMPLATE_FN_ADDR(rt_call_fld_sn4)), x86_rt_gc_poll_res(std::string(), 1) }) });
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string cf_pas_bi_flat() {
    return emit_seq({ x86("lea", "rdi", FRQ(CF_ARG())),
                      x86("mov32", "esi", (long)CF_N()),
                      x86("mov32", "edx", _.op_call_idx),
                      x86("rtcc_wb"),
                      x86("call_bare", "rt_call_pas_bi", TEMPLATE_FN_ADDR(rt_call_pas_bi)),
                      x86("rtcc_rl"),
                      x86("mov", FRQ(CF_RES()), "rax"),
                      x86("mov", FRQ(CF_RES() + 8), "rdx"),
                      x86_rt_gc_poll() });
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string cf_pas_bi_zd() {
    return emit_seq({ cf_argv_rdi(),
                      x86("mov32", "esi", (long)CF_N()),
                      x86("mov32", "edx", _.op_call_idx),
                      x86("call", "rt_call_pas_bi", TEMPLATE_FN_ADDR(rt_call_pas_bi)),
                      x86_rt_gc_poll_res(std::string(), 1) });
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string cf_direct_flat() {
    return emit_seq({ IF(_.op_call_elem_access, elem_access_flat_fast(CF_ARG(), CF_RES())),
                      x86("lea", "rdi", FRQ(CF_ARG())),
                      x86("mov32", "esi", (long)CF_N()),
                      x86("call", _.op_call_sym, _.op_call_fp),
                      x86("mov", FRQ(CF_RES()), "rax"),
                      x86("mov", FRQ(CF_RES() + 8), "rdx"),
                      x86_rt_gc_poll_fails(),
                      IF(_.op_call_elem_access, elem_access_flat_join()) });
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string cf_direct_zd() {
    return emit_seq({ x86("lea", "rdi", x86_zref(CF_BLK(), 1)),
                      x86("mov32", "esi", (long)CF_N()),
                      x86("call", _.op_call_sym, _.op_call_fp),
                      x86_rt_gc_poll_res(x86_cap_raise(std::string()), 1) });
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bcfn_method_open_enter(int base, int decl_id, int join_id, int rec_off, const char * osym, uint64_t ofp) {
    return IF(rec_off < 0, x86_bomb("bb_call_fn: a meth_call or redispatch node without the argv grant that holds its redispatch record (CEO-1552)"))
         + IF(rec_off >= 0,
               x86("lea", "rdx", _.op_zres ? FRQB(rec_off, 0) : FRQ(rec_off))
             + x86("call", osym, ofp)
             + x86_rt_gc_poll_rec_sigma_word(1, _.op_zres)
             + x86("test", "rax", "rax")
             + x86_jcc_id("jz", decl_id)
             + bb_glue_enter_c2bb(base, base + 5, base + 6, -1, 1, _.op_zres)
             + x86_deflabel_id(base + 5)
             + x86("call", "rk_method_land_γ", (uint64_t)(uintptr_t)(void *)rk_method_land_γ)
             + x86_rt_gc_poll_rec_res(_.op_zres)
             + x86_jmp_id(join_id)
             + x86_deflabel_id(base + 6)
             + x86("call", "rk_method_land_ω", (uint64_t)(uintptr_t)(void *)rk_method_land_ω)
             + x86_rt_gc_poll_rec_res(_.op_zres)
             + x86_jmp_id(join_id));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bcfn_apply_open_enter(int base, int decl_id, int join_id) {
    return x86("call", "rt_apply_open", (uint64_t)(uintptr_t)(void *)rt_apply_open)
         + x86_rt_gc_poll_rec_sigma_word(1, _.op_zres)
         + x86("test", "rax", "rax")
         + x86_jcc_id("jz", decl_id)
         + bb_glue_stno_unit_push(_.op_zres)
         + bb_glue_enter_c2bb(base, base + 5, base + 6, -1, 0, _.op_zres)
         + x86_deflabel_id(base + 5)
         + (bb_glue_in_region(_.op_zres) ? x86("lea", "rcx", RDQ("rsp", 64)) : x86("mov", "rcx", "rsp"))
         + x86("call", "rt_apply_land_γ", (uint64_t)(uintptr_t)(void *)rt_apply_land_γ)
         + x86_rt_gc_poll_rec_res(_.op_zres)
         + IF(!bb_glue_in_region(_.op_zres), x86("add", "rsp", 16L))
         + x86_jmp_id(join_id)
         + x86_deflabel_id(base + 6)
         + (bb_glue_in_region(_.op_zres) ? x86("lea", "rsi", RDQ("rsp", 64)) : x86("mov", "rsi", "rsp"))
         + x86("call", "rt_apply_land_ω", (uint64_t)(uintptr_t)(void *)rt_apply_land_ω)
         + x86_rt_gc_poll_rec_res(_.op_zres)
         + IF(!bb_glue_in_region(_.op_zres), x86("add", "rsp", 16L))
         + x86_jmp_id(join_id);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define BCFN_REC_PTR(reg) ( IF(!CF_REC_CELL(), x86("mov", (reg), "rsp")) \
                          + IF(CF_REC_CELL(), x86("lea", (reg), x86_zref(_.op_call_rec, 1))) )
#define BCFN_EVAL_OPEN_ENTER(base, decl_id, join_id) ( IF(!CF_REC_CELL(), x86("sub", "rsp", (long)BCFN_EVAL_REC)) \
                                                     + BCFN_REC_PTR("rdx") \
                                                     + x86("call", "rt_eval_open", (uint64_t)(uintptr_t)(void *)rt_eval_open) \
         + x86_rt_gc_poll_rec_sigma_word(1, CF_REC_CELL()) \
         + x86("test", "rax", "rax") \
         + x86_jcc_id("jz", (decl_id)) \
         + x86("mov", "rcx", "rdx") \
         + x86("shr", "rcx", 62L) \
         + x86("test", "rcx", "rcx") + x86_jcc_id("jnz", (base) + 37) \
         + bb_glue_enter_chain_ret((base) + 1, CF_REC_CELL()) \
         + x86("call", "rt_eval_land", (uint64_t)(uintptr_t)(void *)rt_eval_land) \
         + x86_rt_gc_poll_rec_res(CF_REC_CELL()) \
         + x86_jmp_id((join_id)) \
         + x86_deflabel_id((base) + 37) + x86("shl", "rdx", 2L) \
         + x86("shr", "rdx", 2L) \
         + bb_glue_enter_c2bb((base) + 30, (base) + 35, (base) + 36, -1, 0, CF_REC_CELL()) \
         + x86_deflabel_id((base) + 35) \
         + BCFN_REC_PTR("rcx") \
         + x86("call", "rt_eval_dtx_land_γ", (uint64_t)(uintptr_t)(void *)rt_eval_dtx_land_γ) \
         + x86_rt_gc_poll_rec_res(CF_REC_CELL()) \
         + x86_jmp_id((join_id)) \
         + x86_deflabel_id((base) + 36) \
         + BCFN_REC_PTR("rsi") \
         + x86("call", "rt_eval_dtx_land_ω", (uint64_t)(uintptr_t)(void *)rt_eval_dtx_land_ω) \
         + x86_rt_gc_poll_rec_res(CF_REC_CELL()) \
         + x86_jmp_id((join_id)) )
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define CF_ARGV_LEA() ( \
      IF(_.op_zres, x86("lea", "rdi", x86_zref(CF_BLK(), 1))) \
    + IF(!_.op_zres, x86("lea", "rdi", FRQ(CF_ARG()))) \
)
static std::string cf_open_method(const char * osym, uint64_t ofp) {
    return emit_seq({ CF_ARGV_LEA(), x86("mov32", "esi", (long)CF_N()), bcfn_method_open_enter(20, 28, 29, _.op_call_rec_off, osym, ofp), x86_deflabel_id(28) });
}
static std::string cf_open_apply() { return emit_seq({ CF_ARGV_LEA() + x86("mov32", "esi", (long)CF_N()) + bcfn_apply_open_enter(60, 68, 29), x86_deflabel_id(68) }); }
static std::string cf_open_eval() { return emit_seq({ CF_ARGV_LEA(), x86("mov32", "esi", (long)CF_N()), BCFN_EVAL_OPEN_ENTER(80, 88, 29), x86_deflabel_id(88) }); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define CF_ZD_HEAD() IF(_.op_call_elem_access && !_.op_sb, elem_access_zd_fast())
std::string bb_call_direct() {
    return x86_alpha()
         + IF(CF_REFUSED(), cf_refuse())
         + IF(!CF_REFUSED() && _.op_zres, emit_seq({ CF_ZD_HEAD(), cf_zd_copy(), cf_direct_zd(), cf_zd_exit() }))
         + IF(!CF_REFUSED() && !_.op_zres, emit_seq({ cf_marshal(), cf_direct_flat(), cf_flat_exit() }));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_call_pl_leaf() {
    return x86_alpha()
         + IF(CF_REFUSED(), cf_refuse())
         + IF(!CF_REFUSED() && _.op_zres, emit_seq({ CF_ZD_HEAD(), cf_zd_copy(), pl_leaf_inline_arm(), pl_leaf_zd_tail(), cf_zd_exit() }))
         + IF(!CF_REFUSED() && !_.op_zres, emit_seq({ cf_marshal(), pl_leaf_inline_arm() }));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_call_pas_bi() {
    return x86_alpha()
         + IF(CF_REFUSED(), cf_refuse())
         + IF(!CF_REFUSED() && _.op_zres, emit_seq({ cf_zd_copy(), cf_pas_bi_zd(), cf_zd_exit() }))
         + IF(!CF_REFUSED() && !_.op_zres, emit_seq({ cf_marshal(), cf_pre_road(), cf_pas_bi_flat(), cf_flat_exit() }));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_call_callee_rec() {
    return x86_alpha()
         + IF(CF_REFUSED(), cf_refuse())
         + IF(!CF_REFUSED() && _.op_zres, emit_seq({ CF_ZD_HEAD(), cf_zd_copy(), cf_baked_zd(1), cf_zd_exit() }))
         + IF(!CF_REFUSED() && !_.op_zres, emit_seq({ cf_marshal(), cf_pre_road(), cf_baked_flat(1), cf_flat_exit() }));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_call_field_rec() {
    return x86_alpha()
         + IF(CF_REFUSED(), cf_refuse())
         + IF(!CF_REFUSED() && _.op_zres, emit_seq({ CF_ZD_HEAD(), cf_zd_copy(), cf_baked_zd(0), cf_zd_exit() }))
         + IF(!CF_REFUSED() && !_.op_zres, emit_seq({ cf_marshal(), cf_pre_road(), cf_baked_flat(0), cf_flat_exit() }));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_call_byname() {
    return x86_alpha()
         + IF(CF_REFUSED(), cf_refuse())
         + IF(!CF_REFUSED() && _.op_zres, emit_seq({ CF_ZD_HEAD(), cf_zd_save(), cf_zd_copy(), cf_zd_sync(), cf_byname_zd(0, 0), cf_zd_exit() }))
         + IF(!CF_REFUSED() && !_.op_zres, emit_seq({ cf_marshal(), cf_pre_road(), cf_byname_flat(0, 0), cf_flat_exit() }));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_call_try() {
    return x86_alpha()
         + IF(CF_REFUSED(), cf_refuse())
         + IF(!CF_REFUSED() && _.op_zres, emit_seq({ CF_ZD_HEAD(), cf_zd_save(), cf_zd_copy(), cf_zd_sync(), cf_byname_zd(0, 0), cf_zd_exit() }))
         + IF(!CF_REFUSED() && !_.op_zres, emit_seq({ cf_marshal(), cf_pre_road(), cf_byname_flat(0, 0), cf_flat_exit() }));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_call_method() {
    return x86_alpha()
         + IF(CF_REFUSED(), cf_refuse())
         + IF(!CF_REFUSED() && _.op_zres, emit_seq({ CF_ZD_HEAD(), cf_zd_copy(), cf_open_method("rk_method_open", TEMPLATE_FN_ADDR(rk_method_open)), cf_byname_zd(1, 0), cf_zd_exit() }))
         + IF(!CF_REFUSED() && !_.op_zres, emit_seq({ cf_marshal(), cf_open_method("rk_method_open", TEMPLATE_FN_ADDR(rk_method_open)), cf_byname_flat(1, 0), cf_flat_exit() }));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_call_redisp() {
    return x86_alpha()
         + IF(CF_REFUSED(), cf_refuse())
         + IF(!CF_REFUSED() && _.op_zres, emit_seq({ CF_ZD_HEAD(), cf_zd_copy(), cf_open_method(_.op_call_open_sym, _.op_call_open_fp), cf_byname_zd(1, 0), cf_zd_exit() }))
         + IF(!CF_REFUSED() && !_.op_zres, emit_seq({ cf_marshal(), cf_open_method(_.op_call_open_sym, _.op_call_open_fp), cf_byname_flat(1, 0), cf_flat_exit() }));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_call_apply() {
    return x86_alpha()
         + IF(CF_REFUSED(), cf_refuse())
         + IF(!CF_REFUSED() && _.op_zres, emit_seq({ CF_ZD_HEAD(), cf_zd_copy(), cf_open_apply(), cf_byname_zd(1, 0), cf_zd_exit() }))
         + IF(!CF_REFUSED() && !_.op_zres, emit_seq({ cf_marshal(), cf_open_apply(), cf_byname_flat(1, 0), cf_flat_exit() }));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_call_eval() {
    return x86_alpha()
         + IF(CF_REFUSED(), cf_refuse())
         + IF(!CF_REFUSED() && _.op_zres, emit_seq({ CF_ZD_HEAD(), cf_zd_copy(), cf_open_eval(), cf_byname_zd(1, 1), cf_zd_exit() }))
         + IF(!CF_REFUSED() && !_.op_zres, emit_seq({ cf_marshal(), cf_open_eval(), cf_byname_flat(1, 1), cf_flat_exit() }));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_call_gen() {
    return x86_alpha()
         + IF(CF_REFUSED(), cf_refuse())
         + IF(!CF_REFUSED(), emit_seq({ cf_marshal(),
                                        x86("mov", FRQ(_.op_call_genoff), (long)0),
                                        x86("def", L(60)),
                                        IF(_.op_call_scansync, x86_scan_env_store()),
                                        cf_fnlbl(),
                                        x86("lea", "rsi", FRQ(CF_ARG())),
                                        x86("mov32", "edx", (long)CF_N()),
                                        x86("lea", "rcx", FRQ(_.op_call_genoff)),
                                        x86("rtcc_wb"),
                                        x86("call_bare", (_.op_strict ? "rt_call_arr_gen_strict" : "rt_call_arr_gen"), TEMPLATE_FN_ADDR((_.op_strict ? rt_call_arr_gen_strict : rt_call_arr_gen))),
                                        IF(_.op_call_scansync, x86_scan_env_load_rr()),
                                        x86_rt_gc_poll_res(),
                                        x86("rtcc_rl"),
                                        x86("mov", FRQ(CF_RES()), "rax"),
                                        x86("mov", FRQ(CF_RES() + 8), "rdx"),
                                        x86("cmp", "al", (long)DT_FAIL),
                                        x86_omega("je"),
                                        x86_gamma(),
                                        x86_beta(),
                                        x86("jmp", L(60)) }));
}
