#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "ab_abi.h"
extern int * const rt_k_level_p;
extern long rt_stno_stack[];
extern int g_core_errjmp_n;
}
#include "x86_asm.h"
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
static inline bool bb_glue_outer_whack() {
    extern int g_glue_o_sup;
    if (g_glue_o_sup) return false;
    static int s = -1;
    if (s < 0)
        { const char * e = getenv("SCRIP_GLUE_SYM"); s = (e && *e == '1') ? 1 : 0; }
    return s ? (g_glue_entered != 0) : true;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline bool bb_glue_outer_needs_ret() {
    return (g_emit.flat_jmp_entry != 0) && !(g_emit_cfg && g_emit_cfg->runtime_fragment_graph);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void sno_setexit_fire_on_end(void);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_glue_icn_root_tap(int kind) {
    extern std::string xa_icn_trace_tap(const char * pname, int kind, int np); extern const char * xa_icn_trace_pname(void);
    if (!(g_emit_cfg && g_emit_cfg->root_graph && g_emit_cfg->icn_cells_graph)) return std::string();
    return xa_icn_trace_tap(xa_icn_trace_pname(), kind, 0);
}
std::string bb_glue_outer_γ() {
    return bb_glue_icn_root_tap(2)
         + IF(bb_glue_outer_whack(), bb_glue_framed_leave())
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
         + IF(bb_glue_outer_whack(), bb_glue_framed_leave())
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
    return bb_glue_icn_root_tap(is_gamma ? 2 : 3) + x86_jmp_mem("rsp", is_gamma ? 0 : 8);
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
std::string bb_glue_lvl_slot_rcx(void) {
    return x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&rt_k_level_p, "rt_k_level_p")
         + x86("mov", "rax", RDQ("rax", 0))
         + x86("mov", "ecx", RDD("rax", 0))
         + x86("movsxd", "rcx", "ecx")
         + x86("and", "rcx", (long)SNO_LVL_MASK)
         + FOR(0, SNO_LVL_SHIFT, [&](int) { return x86("add", "rcx", "rcx"); })
         + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)rt_stno_stack, "rt_stno_stack")
         + x86("add", "rcx", "rax");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_glue_act_record(int keep_rax) {
    int k = keep_rax ? 8 : 0;
    return x86("comment", "ACTIVATION RECORD (row snobol4-a-setexit-handler-runs-at-top-level-so-freturn-from-it-is-error-242): the wire pair just pushed IS the activation base; a SETEXIT handler that RETURNs resumes here")
         + IF(keep_rax, x86("push", "rax"))
         + bb_glue_lvl_slot_rcx()
         + x86("lea", "rax", RDQ("rsp", k))
         + x86("mov", RDQ("rcx", SNO_LVL_ACT_RSP), "rax")
         + x86("mov", RDQ("rcx", SNO_LVL_ACT_R12), "r12")
         + x86("mov", "rax", RDQ("rsp", k))
         + x86("mov", RDQ("rcx", SNO_LVL_GAMMA), "rax")
         + x86("mov", "rax", std::string("[rip@got + __]"), (uint64_t)(uintptr_t)(void *)&g_core_errjmp_n, "g_core_errjmp_n")
         + x86("mov", "eax", RDD("rax", 0))
         + x86("movsxd", "rax", "eax")
         + x86("mov", RDQ("rcx", SNO_LVL_ERRJMP), "rax")
         + IF(keep_rax, x86("pop", "rax"))
         + x86("mov", "rcx", RDQ("rsp", 0));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_glue_pass_wires_blob(int gid, int wid) {
    return x86_lea_id("rcx", wid) + x86("push", "rcx")
         + x86_lea_id("rcx", gid) + x86("push", "rcx")
         + x86_jmp_reg("rax");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_glue_pass_wires_blob_act(int gid, int wid) {
    return x86_lea_id("rcx", wid) + x86("push", "rcx")
         + x86_lea_id("rcx", gid) + x86("push", "rcx")
         + bb_glue_act_record(1)
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
extern "C" void rt_pat_prim_land_γ(DESCR_t frame0, long word);
extern "C" void rt_pat_prim_land_ω(long word);
extern "C" long rt_pat_prim_int_take(void);
extern "C" long rt_pat_prim_str_take(const char **out_ptr, long *out_len);
std::string bb_glue_enter_c2bb(int base, int lg, int lw) {
    std::string save =  x86("sub", "rsp", 64L)
         + x86_rsp_store64(56, "r12")
         + x86_rsp_store64(48, "rbx")
         + x86_rsp_store32_imm(32, (long)DT_I) + x86_rsp_store32_imm(36, 0L) + x86_rsp_store64(40, "rdx")
         + x86_rsp_store32_imm(16, (long)DT_I) + x86_rsp_store32_imm(20, 0L) + x86_rsp_store64(24, "r14")
         + x86_rsp_store32_imm(0,  (long)DT_S) + x86_rsp_store32(4, "r15d") + x86_rsp_store64(8, "r13");
    auto restore = [&]() {
        return x86_rsp_load64("r13", 8) + x86_rsp_load32("r15d", 4) + x86_rsp_load64("r14", 24)
             + x86_rsp_load64("rbx", 48) + x86_rsp_load64("r12", 56)
             + x86("add", "rsp", 64L); };
    auto land_γ = [&](long drop) {
        return x86("add", "rsp", drop)
             + x86("mov", "rdi", "rax")
             + x86("mov", "rsi", "rdx")
             + x86_rsp_load64("rdx", 40)
             + restore()
             + x86_jmp_id(lg); };
    auto land_ω = [&](long drop) {
        return x86("add", "rsp", drop)
             + x86_rsp_load64("rdi", 40)
             + restore()
             + x86_jmp_id(lw); };
    return save
         + x86("mov",  "rcx", "rdx")
         + x86("and",  "rcx", 255L)
         + x86("cmp",  "rcx", 2L)
         + x86_jcc_id("je", base)
         + x86("cmp",  "rcx", 1L)
         + x86_jcc_id("je", base + 100)
         + bb_glue_pass_wires_blob_regs(base + 1, base + 2)
         + x86_deflabel_id(base)
         + x86("sub",  "rsp", 48L)
         + x86("mov",  RDQ("rsp", 0), 0L)
         + x86_lea_id("rcx", base + 3)
         + x86("mov",  RDQ("rsp", 8), "rcx")
         + x86_lea_id("rcx", base + 4)
         + x86("mov",  RDQ("rsp", 16), "rcx")
         + x86("mov",  RDQ("rsp", 24), 0L)
         + x86("mov",  RDQ("rsp", 32), 16L)
         + x86("lea",  "rcx", RDQ("rsp", 0))
         + x86_jmp_reg("rax")
         + x86_deflabel_id(base + 3)
         + land_γ(48L)
         + x86_deflabel_id(base + 4)
         + land_ω(48L)
         + x86_deflabel_id(base + 1)
         + land_γ(16L)
         + x86_deflabel_id(base + 2)
         + land_ω(16L)
         + x86_deflabel_id(base + 100)
         + bb_glue_pass_wires_blob_regs(base + 101, base + 102)
         + x86_deflabel_id(base + 101)
         + land_γ(0L)
         + x86_deflabel_id(base + 102)
         + land_ω(0L);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_glue_prim_open_enter(int base) {
    return x86("call", "rt_pat_prim_open", (uint64_t)(uintptr_t)(void *)rt_pat_prim_open)
         + x86_rt_gc_poll_rec_sigma_word(1)
         + x86("test", "rax", "rax")
         + x86_jcc_id("jz", base + 7)
         + bb_glue_enter_c2bb(base, base + 5, base + 6)
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
std::string bb_glue_prim_int(int base) {
    return bb_glue_prim_open_enter(base)
         + x86("call", "rt_pat_prim_int_take", (uint64_t)(uintptr_t)(void *)rt_pat_prim_int_take)
         + x86_rt_gc_poll_rec_sigma(1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_glue_prim_str(int base, int ptr_d, int ptr_sd, int len_d, int len_sd) {
    std::string s = bb_glue_prim_open_enter(base);
    s += x86("lea",  "rdi", LFDQ(ptr_d, ptr_sd));
    s += x86("lea",  "rsi", LFD(len_d, len_sd));
    s += x86("call", "rt_pat_prim_str_take", (uint64_t)(uintptr_t)(void *)rt_pat_prim_str_take);
    s += x86_rt_gc_poll_rec_sigma(1);
    return s;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_glue_prim_str_rsp(int base, int ptr_off, int len_off) {
    std::string s = bb_glue_prim_open_enter(base);
    s += x86("lea",  "rdi", RDQ("rsp", ptr_off));
    s += x86("lea",  "rsi", RDQ("rsp", len_off));
    s += x86("call", "rt_pat_prim_str_take", (uint64_t)(uintptr_t)(void *)rt_pat_prim_str_take);
    s += x86_rt_gc_poll_rec_sigma(1);
    return s;
}
