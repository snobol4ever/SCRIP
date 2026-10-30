#include <string>
#include "g_runtime.h"
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "dtp.h"
typedef struct { long fn; long how; } rt_call_next_t;
extern rt_call_next_t rt_apply_open(DESCR_t * args, int nargs);
extern rt_call_next_t rt_call_callee_try_sn4(DESCR_t * args, int nargs, sno_callee_rec_t * r, DESCR_t * out);
extern long rt_tweak_open(DESCR_t * out, const char * cname, trace_pend_t * pend);
extern void rt_call_pend_close(trace_pend_t * pend);
extern DESCR_t rt_apply_land_γ(DESCR_t frame0, long word, const long * sv);
extern DESCR_t rt_apply_land_ω(long word, const long * sv);
typedef struct { long fn; long how; } rt_eval_next_t;
extern rt_eval_next_t rt_eval_open(DESCR_t * args, int nargs, void * rec);
extern rt_eval_next_t rt_eval_open_raise(DESCR_t * args, int nargs, void * rec);
extern DESCR_t rt_eval_land(long word);
}
#include "x86_asm.h"
#define BGTE_EVAL_REC 112
#define BGTE_VERDICT_EVAL 3
std::string bb_glue_try_enter(const char * try_sym, uint64_t try_fp, const char * lg_sym, uint64_t lg_fp, const char * lw_sym, uint64_t lw_fp, int base, int val_id, int join_id, int stno, int in_shim,
    const std::string & tweak_out_lea = std::string(), int eval_arm = 0, int rec_off = -1, int reg = 0);
std::string bb_glue_enter_chain_ret(int lid, int reg = 0);
std::string bb_glue_enter_c2bb(int base, int lg, int lw, int hi = -1, int blk = 0, int reg = 0);
std::string bb_glue_apply_try_enter(int base, int val_id, int join_id, int in_shim, int reg = 0);
std::string bb_glue_call_pend_run(int base, long nargs, long off, int reg = 0);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_glue_flat_enter() {
    return IF(_.op_fc_bytes > 0, x86("sub", "rsp", _.op_fc_bytes));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_glue_flat_leave() {
    return IF(_.op_fc_bytes > 0, x86("add", "rsp", _.op_fc_bytes));
}
int g_glue_entered = 0;
int g_glue_o_sup = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define bb_glue_outer_needs_ret() ((bool)((g_emit.flat_jmp_entry != 0) && !(g_emit_cfg && g_emit_cfg->runtime_fragment_graph)))
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void sno_setexit_fire_on_end(void);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_glue_icn_root_tap(int kind) {
    extern std::string xa_icn_trace_tap(const char * pname, int kind, int np, int r11d); extern const char * xa_icn_trace_pname(void);
    return IF(!(g_emit_cfg && g_emit_cfg->root_graph && g_emit_cfg->cell_frame),
               std::string())
         + IF(!(!(g_emit_cfg && g_emit_cfg->root_graph && g_emit_cfg->cell_frame)),
               xa_icn_trace_tap(xa_icn_trace_pname(), kind, 0, 16));
}
std::string bb_glue_outer_γ() {
    return bb_glue_icn_root_tap(2)
         + IF(!bb_glue_outer_needs_ret(),
              x86("call_bare", "sno_setexit_fire_on_end", (uint64_t)(uintptr_t)(void(*)(void))sno_setexit_fire_on_end)
            + x86_rt_gc_poll()
            + x86("xor", "edi", "edi")
            + x86("call_bare", "exit", (uint64_t)(uintptr_t)(void(*)(int))exit))
         + IF( bb_glue_outer_needs_ret(),
              x86("mov32", "eax", (long)DT_S)
            + x86("ret"));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_glue_outer_ω() {
    return bb_glue_icn_root_tap(3)
         + IF(!bb_glue_outer_needs_ret(),
              x86("mov32", "edi", 1)
            + x86("call_bare", "exit", (uint64_t)(uintptr_t)(void(*)(int))exit))
         + IF( bb_glue_outer_needs_ret(),
              x86("mov32", "eax", (long)DT_FAIL)
            + x86("ret"));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_glue_wire_land(void) { return std::string(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_glue_wire_exit(int is_gamma) {
    return x86_jmp_mem("rsp", is_gamma ? 0 : 8);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_glue_wire_γ() { return bb_glue_wire_exit(1); }
std::string bb_glue_wire_ω() { return bb_glue_wire_exit(0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_glue_pass_wires(int gid, int wid) {
    return x86_lea_id("rcx", gid)
         + x86_lea_id("rdx", wid)
         + x86_jmp_reg("rax");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_glue_pass_wires_blob(int gid, int wid) {
    return x86_lea_id("rcx", wid) + x86("push", "rcx")
         + x86_lea_id("rcx", gid) + x86("push", "rcx")
         + x86_jmp_reg("rax");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_glue_pass_wires_blob_regs(int gid, int wid) {
    return x86_lea_id("rcx", wid) + x86("push", "rcx")
         + x86_lea_id("rcx", gid) + x86("push", "rcx")
         + x86_lea_id("rcx", gid) + x86_lea_id("rdx", wid)
         + x86_jmp_reg("rax");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { long fn; long how; } rt_prim_next_t;
extern "C" rt_prim_next_t rt_pat_prim_open(const char *varname);
extern "C" int emit_star_is_thunk(const char * star);
extern "C" const char * bb_ab_sym_name(const char * nm);
extern "C" void * bb_dstar_rec_addr(const char * star);
extern "C" void rt_tiny_glue_enter(void);
extern "C" struct gv_s g_call_args;
extern "C" void rt_pat_prim_land_γ(DESCR_t frame0, long word);
extern "C" void rt_pat_prim_land_ω(long word);
extern "C" long rt_pat_prim_int_take(long code);
extern "C" long rt_pat_prim_str_take(const char **out_ptr, long *out_len, long codes);
extern "C" long rt_pat_prim_member(int ch, long codes);
int bb_glue_in_region(int reg) { return reg && _.op_zrel >= 128; }
static int glue_reg(int reg) { return bb_glue_in_region(reg); }
#define GR(rg) ((rg) ? 48 : 0)
#define GLUE_SAVE(rg) ( \
      IF(!(rg), x86("sub", "rsp", 64L)) \
    + x86_rsp_store64(56 + GR(rg), "r12") \
    + x86_rsp_store64(48 + GR(rg), "rbx") \
    + x86_rsp_store32_imm(32 + GR(rg), (long)DT_I) + x86_rsp_store32_imm(36 + GR(rg), 0L) + x86_rsp_store64(40 + GR(rg), "rdx") \
    + x86_rsp_store32_imm(16 + GR(rg), (long)DT_I) + x86_rsp_store32_imm(20 + GR(rg), 0L) + x86_rsp_store64(24 + GR(rg), "r14") \
    + x86_rsp_store32_imm(0 + GR(rg), (long)DT_S) + x86_rsp_store32(4 + GR(rg), "r15d") + x86_rsp_store64(8 + GR(rg), "r13") \
)
#define GLUE_RESTORE(rg) ( \
      x86_rsp_load64("r13", 8 + GR(rg)) + x86_rsp_load32("r15d", 4 + GR(rg)) + x86_rsp_load64("r14", 24 + GR(rg)) \
    + x86_rsp_load64("rbx", 48 + GR(rg)) + x86_rsp_load64("r12", 56 + GR(rg)) \
    + IF(!(rg), x86("add", "rsp", 64L)) \
)
#define GLUE_LAND_G(rg, lg, drop, word) ( \
      x86_rsp_land((rg) ? 0L : (long)(drop)) \
    + x86_gc_site_adj(X86_SITE_LANDING, (word)) \
    + IF(!(rg), x86("add", "rsp", (long)(drop))) \
    + x86("mov", "rdi", "rax") \
    + x86("mov", "rsi", "rdx") \
    + x86_rsp_load64("rdx", 40 + GR(rg)) \
    + GLUE_RESTORE(rg) \
    + x86_jmp_id(lg) \
)
#define GLUE_LAND_W(rg, lw, drop, word) ( \
      x86_rsp_land((rg) ? 0L : (long)(drop)) \
    + x86_gc_site_adj(X86_SITE_LANDING, (word)) \
    + IF(!(rg), x86("add", "rsp", (long)(drop))) \
    + x86_rsp_load64("rdi", 40 + GR(rg)) \
    + GLUE_RESTORE(rg) \
    + x86_jmp_id(lw) \
)
static std::string bb_glue_wires_region(int gid, int wid) {
    return x86_lea_id("rcx", wid) + x86("mov", RDQ("rsp", 8), "rcx")
         + x86_lea_id("rcx", gid) + x86("mov", RDQ("rsp", 0), "rcx")
         + x86_lea_id("rcx", gid) + x86_lea_id("rdx", wid)
         + x86_jmp_reg("rax");
}
static std::string bb_glue_c2bb_body(int base, int lg, int lw, int hi, int blk, int rg) {
    return emit_seq({ GLUE_SAVE(rg),
         x86_rsp_mark_save()
         + x86("mov", "rcx", "rdx")
         + x86("and", "rcx", 63L)
         + IF(blk, x86("cmp", "rcx", 3L) + x86_jcc_id("je", hi + 3))
         + x86("cmp", "rcx", 2L)
         + x86_jcc_id("je", base)
         + x86("cmp", "rcx", 1L)
         + x86_jcc_id("je", hi)
         + (rg ? bb_glue_wires_region(base + 1, base + 2) : bb_glue_pass_wires_blob_regs(base + 1, base + 2))
         + x86_deflabel_id(base)
         + IF(!rg, x86("sub", "rsp", 48L))
         + x86("mov", RDQ("rsp", 0), 0L)
         + x86_lea_id("rcx", base + 3)
         + x86("mov", RDQ("rsp", 8), "rcx")
         + x86_lea_id("rcx", base + 4)
         + x86("mov", RDQ("rsp", 16), "rcx")
         + x86("mov", RDQ("rsp", 24), 0L)
         + x86("mov", RDQ("rsp", 32), 16L)
         + x86_load_got("rcx", "rt_tiny_glue_enter", (uint64_t)(uintptr_t)(void *)rt_tiny_glue_enter)
         + x86_jmp_reg("rcx")
         + x86_deflabel_id(base + 3)
         + GLUE_LAND_G(rg, lg, 48L, 0)
         + x86_deflabel_id(base + 4)
         + GLUE_LAND_W(rg, lw, 48L, 0)
         + x86_deflabel_id(base + 1)
         + GLUE_LAND_G(rg, lg, 16L, 0)
         + x86_deflabel_id(base + 2)
         + GLUE_LAND_W(rg, lw, 16L, 0)
         + x86_deflabel_id(hi)
         + bb_glue_pass_wires_blob_regs(hi + 1, hi + 2)
         + x86_deflabel_id(hi + 1)
         + GLUE_LAND_G(rg, lg, 0L, 16)
         + x86_deflabel_id(hi + 2)
         + GLUE_LAND_W(rg, lw, 0L, 16)
         + IF(blk,
               x86("note",
                   "the block protocol, Raku regime, on the by-name road: the open answered a pinned callee (how 3) and its parameter count in rdx>>8; the block is carved on this spine and filled fr"
                   "om the staged medium, the wires go in rcx and rdx, and the callee's release pops the block")
             + x86_deflabel_id(hi + 3)
             + x86("mov", "rcx", "rdx")
             + x86("shl", "rcx", 24L)
             + x86("shr", "rcx", 32L)
             + x86("mov", "r8", "rcx")
             + x86("shl", "r8", 4L)
             + x86("sub", "rsp", "r8")
             + x86("add", "rcx", "rcx")
             + x86("mov", "rsi", "[rip@got + __]", (uint64_t)(uintptr_t)&g_call_args, "g_call_args")
             + x86("mov", "rsi", RDQ("rsi", 0))
             + x86("mov", "rdi", "rsp")
             + x86("rep_movsq")
             + x86_lea_id("rcx", hi + 4)
             + x86_lea_id("rdx", hi + 5)
             + x86_jmp_reg("rax")
             + x86_deflabel_id(hi + 4)
             + x86_rsp_land(0L)
             + x86_gc_site_adj(X86_SITE_LANDING, 0)
             + x86_rsp_load64("rdx", 40 + GR(rg))
             + GLUE_RESTORE(rg)
             + x86_jmp_id(lg)
             + x86_deflabel_id(hi + 5)
             + GLUE_LAND_W(rg, lw, 0L, 0)) });
}
std::string bb_glue_enter_c2bb(int base, int lg, int lw, int hi, int blk, int reg) {
    return bb_glue_c2bb_body(base, lg, lw, hi < 0 ? base + 100 : hi, blk, glue_reg(reg) && hi < 0);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_glue_stno_unit_push(int reg) {
    return IF(!glue_reg(reg), x86("sub", "rsp", 16L))
         + x86("mov", "rcx", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&g_runtime, "g_runtime")
         + x86("mov", "rcx", RDQ("rcx", GRT_AT(stno)))
         + x86_rsp_store64(glue_reg(reg) ? 112 : 0, "rcx")
         + x86("mov", "rcx", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&g_runtime, "g_runtime")
         + x86("mov", "rcx", RDQ("rcx", GRT_AT(line)))
         + x86_rsp_store64(glue_reg(reg) ? 120 : 8, "rcx");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define BGTE_CELL(rec_off) (_.op_zres && (rec_off) >= 0)
#define BGTE_RO(rec_off) (BGTE_CELL(rec_off) ? (long)(rec_off) + _.op_zrel : 0L)
#define BGTE_EVAL_REG(reg, rec_off) (glue_reg(reg) && BGTE_CELL(rec_off))
static std::string bb_glue_try_eval_arm(int base, int val_id, int join_id, int rec_off, int reg) {
    return x86("cmp", "rax", (long)BGTE_VERDICT_EVAL)
         + x86_jcc_id("jne", base + 70)
         + x86("mov", "rdi", "rdx")
         + x86("mov32", "esi", 1L)
         + IF(!BGTE_CELL(rec_off), x86("sub", "rsp", (long)BGTE_EVAL_REC))
         + IF(!BGTE_CELL(rec_off), x86("mov", "rdx", "rsp"))
         + IF(BGTE_CELL(rec_off), x86("lea", "rdx", RDQ("rsp", BGTE_RO(rec_off))))
         + x86("call", "rt_eval_open_raise", (uint64_t)(uintptr_t)(void *)rt_eval_open_raise)
         + x86_rt_gc_poll_rec_sigma_word(1, BGTE_EVAL_REG(reg, rec_off))
         + x86("test", "rax", "rax")
         + x86_jcc_id("jz", base + 72)
         + bb_glue_enter_chain_ret(base + 71, BGTE_EVAL_REG(reg, rec_off))
         + x86("call", "rt_eval_land", (uint64_t)(uintptr_t)(void *)rt_eval_land)
         + x86_rt_gc_poll_rec_res(BGTE_EVAL_REG(reg, rec_off))
         + IF(!BGTE_CELL(rec_off), x86("add", "rsp", (long)BGTE_EVAL_REC))
         + x86_jmp_id(join_id)
         + x86_deflabel_id(base + 72)
         + IF(!BGTE_CELL(rec_off), x86("add", "rsp", (long)BGTE_EVAL_REC))
         + x86_jmp_id(val_id)
         + x86_deflabel_id(base + 70);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_glue_try_enter(const char * try_sym, uint64_t try_fp, const char * lg_sym, uint64_t lg_fp, const char * lw_sym, uint64_t lw_fp, int base, int val_id, int join_id, int stno, int in_shim,
    const std::string & tweak_out_lea, int eval_arm, int rec_off, int reg)
    {
    return x86("call", try_sym, try_fp)
         + x86_rt_gc_poll_rec_sigma_word(1, glue_reg(reg))
         + x86("test", "rax", "rax")
         + x86_jcc_id("jz", val_id)
         + IF(!tweak_out_lea.empty(),
               x86("cmp", "rax", 2L)
             + x86_jcc_id("jne", base + 9)
             + tweak_out_lea
             + x86("mov", "rsi", "rdx")
             + IF(!BGTE_CELL(rec_off), x86("sub", "rsp", 112L))
             + x86("mov", RDQ("rsp", BGTE_RO(rec_off)), 0L)
             + x86("mov", RDQ("rsp", BGTE_RO(rec_off) + 8), 0L)
             + IF(!BGTE_CELL(rec_off), x86("mov", "rdx", "rsp"))
             + IF(BGTE_CELL(rec_off), x86("lea", "rdx", RDQ("rsp", BGTE_RO(rec_off))))
             + x86("call", "rt_tweak_open", (uint64_t)(uintptr_t)(void *)rt_tweak_open)
             + x86_rt_gc_poll()
             + bb_glue_call_pend_run(base + 30, 2L, BGTE_RO(rec_off), glue_reg(reg) && BGTE_CELL(rec_off))
             + IF(!BGTE_CELL(rec_off), x86("add", "rsp", 112L))
             + x86_jmp_id(val_id)
             + x86_deflabel_id(base + 9))
         + IF(eval_arm, bb_glue_try_eval_arm(base, val_id, join_id, rec_off, reg))
         + IF(stno, bb_glue_stno_unit_push(glue_reg(reg)))
         + bb_glue_enter_c2bb(base, base + 5, base + 6, in_shim ? base + 7 : -1, 0, glue_reg(reg))
         + x86_deflabel_id(base + 5)
         + IF(in_shim, x86_rsp_mark_pop())
         + IF(stno && !glue_reg(reg), x86("mov", "rcx", "rsp")) + IF(stno && glue_reg(reg), x86("lea", "rcx", RDQ("rsp", 112)))
         + x86("call", lg_sym, lg_fp)
         + x86_rt_gc_poll_rec_res(glue_reg(reg))
         + IF(stno && !glue_reg(reg), x86("add", "rsp", 16L))
         + x86_jmp_id(join_id)
         + x86_deflabel_id(base + 6)
         + IF(stno && !glue_reg(reg), x86("mov", "rsi", "rsp")) + IF(stno && glue_reg(reg), x86("lea", "rsi", RDQ("rsp", 112)))
         + x86("call", lw_sym, lw_fp)
         + x86_rt_gc_poll_rec_res(glue_reg(reg))
         + IF(stno && !glue_reg(reg), x86("add", "rsp", 16L))
         + x86_jmp_id(join_id)
         + x86_deflabel_id(val_id);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_glue_callee_try_enter(int base, int val_id, int join_id, const std::string & tweak_out_lea, int rec_off, int reg) {
    return bb_glue_try_enter("rt_call_callee_try_sn4", (uint64_t)(uintptr_t)(void *)rt_call_callee_try_sn4, "rt_apply_land_γ", (uint64_t)(uintptr_t)(void *)rt_apply_land_γ,
                             "rt_apply_land_ω", (uint64_t)(uintptr_t)(void *)rt_apply_land_ω, base, val_id, join_id, 1, 0, tweak_out_lea, 1, rec_off, reg);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define PEND_CALL_ID(base, n) ((base) + (n))
std::string bb_glue_call_pend_run(int base, long nargs, long off, int reg) {
    return x86("mov", "rcx", RDQ("rsp", off))
         + x86("test", "rcx", "rcx")
         + x86_jcc_id("jz", PEND_CALL_ID(base, 20))
         + x86("lea", "rdi", RDQ("rsp", off))
         + x86("mov32", "esi", nargs)
         + bb_glue_apply_try_enter(base, PEND_CALL_ID(base, 21), PEND_CALL_ID(base, 22), 0, reg)
         + x86_deflabel_id(PEND_CALL_ID(base, 22))
         + x86("lea", "rdi", RDQ("rsp", off))
         + x86("call", "rt_call_pend_close", (uint64_t)(uintptr_t)(void *)rt_call_pend_close)
         + x86_deflabel_id(PEND_CALL_ID(base, 20));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_glue_apply_try_enter(int base, int val_id, int join_id, int in_shim, int reg) {
    return bb_glue_try_enter("rt_apply_open", (uint64_t)(uintptr_t)(void *)rt_apply_open, "rt_apply_land_γ", (uint64_t)(uintptr_t)(void *)rt_apply_land_γ, "rt_apply_land_ω",
        (uint64_t)(uintptr_t)(void *)rt_apply_land_ω, base, val_id, join_id, 1, in_shim, std::string(), 0, -1, reg);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define PEND_TRACE_ID(base, in_shim, n) ((base) + ((in_shim) ? (n) : (n) + 10))
std::string bb_glue_trace_pend_run(int base, const std::string & after, int in_shim, long off) {
    return IF(in_shim, x86_rsp_mark_push())
         + x86("mov", "rcx", RDQ("rsp", off))
         + x86("test", "rcx", "rcx")
         + x86_jcc_id("jz", PEND_TRACE_ID(base, in_shim, 10))
         + x86("lea", "rdi", RDQ("rsp", off))
         + x86("mov32", "esi", 3L)
         + bb_glue_apply_try_enter(base, PEND_TRACE_ID(base, in_shim, 11), PEND_TRACE_ID(base, in_shim, 12), in_shim)
         + x86("lea", "rdi", RDQ("rsp", off))
         + x86("call", "rt_trace_pend_run", (uint64_t)(uintptr_t)(void *)rt_trace_pend_run)
         + x86_rt_gc_poll()
         + x86_deflabel_id(PEND_TRACE_ID(base, in_shim, 12))
         + x86("lea", "rdi", RDQ("rsp", off))
         + x86("call", "rt_trace_pend_close", (uint64_t)(uintptr_t)(void *)rt_trace_pend_close)
         + x86_rt_gc_poll()
         + after
         + x86_deflabel_id(PEND_TRACE_ID(base, in_shim, 10));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_glue_enter_chain_ret(int lid, int reg) {
    return GLUE_SAVE(glue_reg(reg))
         + x86_rsp_mark_save()
         + x86("mov", "rcx", std::string("[rip@got + __]"), TEMPLATE_ADDR_SIGMA, "Σ")
         + x86("mov", "r13", RDQ("rcx", 0))
         + x86("mov", "rcx", std::string("[rip@got + __]"), TEMPLATE_ADDR_SIGLEN, "Σlen")
         + x86("mov", "r15d", RDD("rcx", 0))
         + x86("xor", "ecx", "ecx")
         + x86("push", "rcx")
         + x86_lea_id("rcx", lid)
         + x86("mov", "rdx", "rcx")
         + x86("push", "rcx")
         + x86_jmp_reg("rax")
         + x86_deflabel_id(lid)
         + x86_rsp_land(8L)
         + x86_gc_site_adj(X86_SITE_LANDING, 8)
         + x86("add", "rsp", 8L)
         + x86_rsp_load64("rdi", 40 + GR(glue_reg(reg)))
         + GLUE_RESTORE(glue_reg(reg));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_glue_name_or_rec_lea(const char * nm) {
    return emit_star_is_thunk(nm) ? x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t)bb_dstar_rec_addr(nm), (std::string(".Ldstar_") + bb_ab_sym_name(nm)).c_str())
                                  : x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t)(const void *)nm, x86_strtab_lbl(nm).c_str());
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int bb_patv_slot_of(const char * sv) {
    const char * d = (sv && !strncmp(sv, "PAT$", 4)) ? strstr(sv, "$V") : NULL;
    return (d && strspn(d + 2, "0123456789") > 0 && !d[2 + strspn(d + 2, "0123456789")]) ? atoi(d + 2) : -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_glue_patv_args(void) {
    return (_.op_head_spine && _.op_head_rsp < 0)
         ? x86_bomb("bb_glue_patv_args: a frameless thunk reads its head from a box with no spine depth")
         : (x86("mov", "rdi", _.op_head_spine ? RDQ("rsp", _.op_head_rsp) : RDQ("rbp", -24))
          + x86("mov", "esi", (long) bb_patv_slot_of(_.op_sval))
          + x86("lea", "rdx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)_.op_sval, x86_strtab_lbl(_.op_sval).c_str()));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_glue_patv_cell(const char * reg) {
    return (_.op_head_spine && _.op_head_rsp < 0)
         ? x86_bomb("bb_glue_patv_cell: a frameless thunk reads its head from a box with no spine depth")
         : x86_reg_disp32_lea64(reg, _.op_head_spine ? "rsp" : "rbp", _.op_head_spine ? _.op_head_rsp : -24);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_glue_prim_open_enter(int base) {
    return x86("call", "rt_pat_prim_open", (uint64_t)(uintptr_t)(void *)rt_pat_prim_open)
         + x86_rt_gc_poll_rec_sigma_word(1)
         + x86("test", "rax", "rax")
         + x86_jcc_id("jz", base + 7)
         + bb_glue_enter_c2bb(base, base + 5, base + 6, -1)
         + x86_deflabel_id(base + 5)
         + x86("call", "rt_pat_prim_land_γ", (uint64_t)(uintptr_t)(void *)rt_pat_prim_land_γ)
         + x86_rt_gc_poll_rec_sigma(0)
         + x86_jmp_id(base + 7)
         + x86_deflabel_id(base + 6)
         + x86("call", "rt_pat_prim_land_ω", (uint64_t)(uintptr_t)(void *)rt_pat_prim_land_ω)
         + x86_rt_gc_poll_rec_sigma(0)
         + x86_deflabel_id(base + 7);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_glue_prim_int(int base, int code) {
    return bb_glue_prim_open_enter(base)
         + x86("mov", "rdi", (long)code)
         + x86("call", "rt_pat_prim_int_take", (uint64_t)(uintptr_t)(void *)rt_pat_prim_int_take)
         + x86_rt_gc_poll_rec_sigma(1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_glue_prim_member(int base, int code) {
    return bb_glue_prim_open_enter(base)
         + x86("mov32", "edi", -1L)
         + x86("mov", "eax", "r14d")
         + x86("cmp", "eax", "r15d")
         + x86_jcc_id("jge", base + 9)
         + x86("movsxd", "rcx", "r14d")
         + x86("movzx", "edi", "[r13+rcx]")
         + x86_deflabel_id(base + 9)
         + x86("mov", "rsi", (long)code | ((long)code << 16))
         + x86("call", "rt_pat_prim_member", (uint64_t)(uintptr_t)(void *)rt_pat_prim_member)
         + x86_rt_gc_poll_rec_sigma(1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_glue_prim_str(int base, int ptr_d, int ptr_sd, int len_d, int len_sd, int code) {
    return emit_seq({ bb_glue_prim_open_enter(base),
                      x86("lea", "rdi", LFDQ(ptr_d, ptr_sd)),
                      x86("lea", "rsi", LFD(len_d, len_sd)),
                      x86("mov", "rdx", (long)code | ((long)code << 16)),
                      x86("call", "rt_pat_prim_str_take", (uint64_t)(uintptr_t)(void *)rt_pat_prim_str_take),
                      x86_rt_gc_poll_rec_sigma(1) });
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_glue_prim_str_rsp(int base, int ptr_off, int len_off, int code) {
    return emit_seq({ bb_glue_prim_open_enter(base),
                      x86("lea", "rdi", RDQ("rsp", ptr_off)),
                      x86("lea", "rsi", RDQ("rsp", len_off)),
                      x86("mov", "rdx", (long)code | ((long)code << 16)),
                      x86("call", "rt_pat_prim_str_take", (uint64_t)(uintptr_t)(void *)rt_pat_prim_str_take),
                      x86_rt_gc_poll_rec_sigma(1) });
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" {
extern rt_call_next_t rt_binop_overload_open(DESCR_t a, DESCR_t b, int op);
extern DESCR_t rt_call_land_γ(DESCR_t frame0, long word);
extern DESCR_t rt_call_land_ω(long word);
extern int rt_is_truthy(DESCR_t v);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_glue_binop_open(int base, const std::string & load, const std::string & store, long op, int ldecline, int truthy) {
    return x86("comment", "an infix multi on objects: rt_binop_overload_open returns the candidate's address and the box enters it (CEO-1576; armed programs only)")
         + load
         + x86("mov", "r8d", op)
         + x86("call", "rt_binop_overload_open", (uint64_t)(uintptr_t)(void *)rt_binop_overload_open)
         + x86_rt_gc_poll_rec_sigma_word(1)
         + x86("test", "rax", "rax")
         + x86("je", L(ldecline))
         + bb_glue_enter_c2bb(base, base + 5, base + 6, -1)
         + x86_deflabel_id(base + 5)
         + x86("call", "rt_call_land_γ", (uint64_t)(uintptr_t)(void *)rt_call_land_γ)
         + x86_rt_gc_poll_rec_res()
         + store
         + IF(truthy,
               x86("mov", "rdi", "rax")
             + x86("mov", "rsi", "rdx")
             + x86("call", "rt_is_truthy", (uint64_t)(uintptr_t)(void *)rt_is_truthy)
             + x86("test", "eax", "eax")
             + x86_omega("je"))
         + x86_gamma()
         + x86_deflabel_id(base + 6)
         + x86("call", "rt_call_land_ω", (uint64_t)(uintptr_t)(void *)rt_call_land_ω)
         + x86_rt_gc_poll_rec_res()
         + x86_omega();
}
