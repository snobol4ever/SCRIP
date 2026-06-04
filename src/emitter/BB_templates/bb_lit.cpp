#include <string>
#include <cstring>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit_bb.h"
#include "emit.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static inline const char * lit()        { return _.op_sval ? _.op_sval : ""; }
static inline long         litlen()     { return (long)strlen(lit()); }
static inline const char * litlabel()   { const char * l = emit_intern_str(lit()); if (l) return l;
                                          static char b[24]; strtab_label(b, sizeof b, lit()); return b; }
static inline uint64_t     litaddr()    { return (uint64_t)(uintptr_t)lit(); }
static inline uint64_t     memcmpaddr() { return (uint64_t)(uintptr_t)memcmp; }
/*--------------------------------------------------------------------------------------------------------------------*/
static std::string bb_lit_str() {
    int nid = _.nid; int sid = 0;
    if (PLATFORM_X86)
        return IF(MEDIUM_TEXT,
                   x86("label", _.lbl_α)
                 + x86("comment", emit_fmt("BOX LIT(%s)  [REG-1 Σ=r13 δ=r14 Δ=r15]",
                                      litlen() > 24 ? emit_fmt("'%.24s...'", lit()).c_str()
                                                    : emit_fmt("'%s'", lit()).c_str())))
             + x86("mov", "eax", "r14d")
             + x86("add", "eax", litlen())
             + x86("cmp", "eax", "r15d")
             + x86("jg", PORT_OMEGA)
             + x86("movsxd", "rcx", "r14d")
             + x86("lea", "rdi", "[r13 + rcx]")
             + x86("lea", "rsi", "[rip + __]", litaddr(), litlabel())
             + x86("mov", "rdx", litlen())
             + x86("push", "r10")
             + x86("call", "memcmp", memcmpaddr())
             + x86("pop", "r10")
             + x86("test", "eax", "eax")
             + x86("jne", PORT_OMEGA)
             + x86("add", "r14d", litlen())
             + x86("mov", "[r10]", "r14d")
             + x86("jmp", PORT_GAMMA)
             + x86("def", PORT_BETA)
             + x86("sub", "r14d", litlen())
             + x86("mov", "[r10]", "r14d")
             + x86("jmp", PORT_OMEGA);
    return std::string();
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_lit(void) {
    bb_emit_x86(bb_lit_str());
    if (MEDIUM_TEXT) g_emit_pos += 7;
}
