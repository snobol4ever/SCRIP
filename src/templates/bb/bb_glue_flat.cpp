#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
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
extern "C" void rt_pat_prim_land_γ(DESCR_t frame0, long word);
extern "C" void rt_pat_prim_land_ω(long word);
extern "C" long rt_pat_prim_int_take(void);
extern "C" long rt_pat_prim_str_take(const char **out_ptr, long *out_len);
std::string bb_glue_enter_c2bb(int base, int lg, int lw) {
    std::string save = x86("comment", "ARCH-GC 6.2i THE ENTRY RECORD IS TAGGED CELLS (cfo 2026-09-19, on the cto's x86_xfer_enter example and the frame walker's own reading): this glue is the ONE spelling of a box entering an opened target, and it used to push rbx r12 r13 r14 r15 rdx as six RAW words around every C-to-BB entry. SCRIP_GC_MAPS=3 named r13, the match subject, raw at -184 on hb_defer_subject.sno at every collection below a return poll: only the word sweep rewrites such a word, and the E cut deletes the sweep, so the glue would pop a stale subject into the callee. The three that can hold a collected-heap pointer or be mistaken for one now ride DESCR cells the collector reads BY TAG -- {DT_S, r15d, r13} the subject, {DT_I, 0, r14} the cursor, {DT_I, 0, rdx} the protocol word (how in its low byte, nsb above it and a registry index at bit 40, which is a plausible-looking address to a raw sweep and is not one). rbx and r12 stay raw and are NAMED: rbx carries a stack word and r12 the CAS pointer, neither of which is ever a block of the collected heap. 48 bytes become 64, so rsp parity at the transfer is unchanged, and nothing indexes into the record.")
         + x86("sub", "rsp", 64L)
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
         + land_ω(16L);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_glue_prim_open_enter(int base) {
    return x86("call", "rt_pat_prim_open", (uint64_t)(uintptr_t)(void *)rt_pat_prim_open)
         + x86("comment", "ARCH-GC section 3: a poll at the EMITTED return of every allocating runtime call on this road -- the open may allocate through the prologue, and the packed spine word in rdx is saved across the poll because the poll speaks the argument registers")
         + x86("push", "rdx")
         + x86_rt_gc_poll_rec_sigma(1)
         + x86("pop",  "rdx")
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
