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
std::string bb_lit() {
    if (PLATFORM_X86)
        return IF(MEDIUM_TEXT,
                   x86("label", _.lbl_α)
                 + x86("comment", std::string("BOX LIT(") + (litlen() > 24 ? std::string("'") + std::string(lit(), 24) + "...'"
                                                                           : std::string("'") + lit() + "'") + ")  [REG-1 Σ=r13 δ=r14 Δ=r15]"))
             + x86("mov", "eax", "r14d")
             + x86("add", "eax", litlen())
             + x86("cmp", "eax", "r15d")
             + x86("jg", "ω")
             + x86("movsxd", "rcx", "r14d")
             + x86("lea", "rdi", "[r13 + rcx]")
             + x86("lea", "rsi", "[rip + __]", litaddr(), litlabel())
             + x86("mov", "rdx", litlen())
             + x86("sub", "rsp", (long)8)
             + x86("call", "memcmp", memcmpaddr())
             + x86("add", "rsp", (long)8)
             + x86("test", "eax", "eax")
             + x86("jne", "ω")
             + x86("add", "r14d", litlen())
             + x86("jmp", "γ")
             + x86("def", "β")
             + x86("sub", "r14d", litlen())
             + x86("jmp", "ω");
    return std::string();
}