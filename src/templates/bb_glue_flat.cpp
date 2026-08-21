#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_glue_flat_enter() {
    if (!PLATFORM_X86) return std::string();
    return IF((x86_zstorage() == ZC_STORAGE_CELL_STACK || x86_zstorage() == ZC_STORAGE_CELL_HEAP) && _.op_fc_bytes > 0, x86("sub", "rsp", _.op_fc_bytes));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_glue_flat_leave() {
    if (!PLATFORM_X86) return std::string();
    return IF((x86_zstorage() == ZC_STORAGE_CELL_STACK || x86_zstorage() == ZC_STORAGE_CELL_HEAP) && _.op_fc_bytes > 0, x86("add", "rsp", _.op_fc_bytes));
}
int g_glue_entered = 0;
int g_glue_o_sup = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline bool bb_glue_outer_whack() { extern int g_glue_o_sup; if (g_glue_o_sup) return false; static int s = -1; if (s < 0) { const char * e = getenv("SCRIP_GLUE_SYM"); s = (e && *e == '1') ? 1 : 0; } return s ? (g_glue_entered != 0) : true; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_glue_outer_γ() {
    if (!PLATFORM_X86) return std::string();
    bool _chain = g_emit.flat_jmp_entry != 0;
    return IF(bb_glue_outer_whack(), bb_glue_framed_leave())
         + IF(!_chain, x86("xor", "edi", "edi") + x86("call_bare", "exit", (uint64_t)(uintptr_t)(void(*)(int))exit))
         + IF( _chain, x86("mov32", "eax", (long)DT_S) + x86("ret"));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_glue_outer_ω() {
    if (!PLATFORM_X86) return std::string();
    bool _chain = g_emit.flat_jmp_entry != 0;
    return IF(bb_glue_outer_whack(), bb_glue_framed_leave())
         + IF(!_chain, x86("mov32", "edi", 1) + x86("call_bare", "exit", (uint64_t)(uintptr_t)(void(*)(int))exit))
         + IF( _chain, x86("mov32", "eax", (long)DT_FAIL) + x86("ret"));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int bb_wire_stack_on(void) { const char * e = getenv("SCRIP_WIRE_STACK"); return (e && *e == '1') ? 1 : 0; }
std::string bb_glue_wire_land(void) { return std::string(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_glue_wire_exit(int is_gamma) {
    if (!PLATFORM_X86) return std::string();
    if (bb_wire_stack_on()) return x86_jmp_mem("rsp", is_gamma ? 0 : 8);
    return x86("jmp", is_gamma ? "r10" : "r11");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_glue_wire_γ() { return bb_glue_wire_exit(1); }
std::string bb_glue_wire_ω() { return bb_glue_wire_exit(0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_glue_pass_wires(int gid, int wid) {
    if (!PLATFORM_X86) return std::string();
    return x86_lea_id("rcx", gid)
         + x86_lea_id("rdx", wid)
         + x86_jmp_reg("rax");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_glue_pass_wires_blob(int gid, int wid) {
    if (!PLATFORM_X86) return std::string();
    if (bb_wire_stack_on())
        return x86_lea_id("rcx", wid) + x86("push", "rcx")
             + x86_lea_id("rcx", gid) + x86("push", "rcx")
             + x86_jmp_reg("rax");
    return x86_lea_id("r10", gid)
         + x86_lea_id("r11", wid)
         + x86_jmp_reg("rax");
}
