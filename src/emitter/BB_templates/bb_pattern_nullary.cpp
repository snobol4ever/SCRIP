#include <string>
#include <cstring>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit_bb.h"
#include "emit.h"
#include "dtp.h"
}
#include "x86_asm.h"
/*--------------------------------------------------------------------------------------------------------------------*/
static int g_pn_seq = 0;
static inline void         pn_bump()  { g_pn_seq++; }
static inline std::string  pnl(const char * sfx) { return std::string(".Lpn") + std::to_string(g_pn_seq) + sfx; }
static inline const char * pn_kind()  { return _.op_kind ? _.op_kind : "FAIL"; }
static inline std::string  pn_off()   { return std::to_string((long)_.op_off); }
/*--------------------------------------------------------------------------------------------------------------------*/
static inline uint64_t pn_proto_addr() {
    if (!strcmp(pn_kind(),"REM"))     return (uint64_t)(uintptr_t)(const void *)bb_rem_proto;
    if (!strcmp(pn_kind(),"SUCCEED")) return (uint64_t)(uintptr_t)(const void *)bb_succeed_proto;
    if (!strcmp(pn_kind(),"FENCE"))   return (uint64_t)(uintptr_t)(const void *)bb_fence_proto;
    if (!strcmp(pn_kind(),"ABORT"))   return (uint64_t)(uintptr_t)(const void *)bb_abort_proto;
    return (uint64_t)(uintptr_t)(const void *)bb_fail_proto;
}
static inline uint64_t pn_desc_addr() {
    if (!strcmp(pn_kind(),"REM"))     return (uint64_t)(uintptr_t)(const void *)&bb_rem_proto_desc;
    if (!strcmp(pn_kind(),"SUCCEED")) return (uint64_t)(uintptr_t)(const void *)&bb_succeed_proto_desc;
    if (!strcmp(pn_kind(),"FENCE"))   return (uint64_t)(uintptr_t)(const void *)&bb_fence_proto_desc;
    if (!strcmp(pn_kind(),"ABORT"))   return (uint64_t)(uintptr_t)(const void *)&bb_abort_proto_desc;
    return (uint64_t)(uintptr_t)(const void *)&bb_fail_proto_desc;
}
static inline long pn_proto_len() {
    if (!strcmp(pn_kind(),"REM")) return 61;
    return 44;
}
static inline uint64_t pn_fn() {
    void (*fp)(DTP_FRAG_t*,const void*,uint32_t,const DTP_PROTO_DESC*,long,const char*) = rt_pattern_build;
    return (uint64_t)(uintptr_t)(void *)fp;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static inline std::string pn_proto_data() {
    if (!strcmp(pn_kind(),"REM"))
        return x86("label", pnl("_s"))
             + x86("raw", ".byte 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0")
             + x86("raw", ".byte 0x44,0x8B,0x35,0xE1,0xFF,0xFF,0xFF, 0xFF,0x25,0xEB,0xFF,0xFF,0xFF, 0x44,0x89,0x35")
             + x86("raw", ".byte 0xD4,0xFF,0xFF,0xFF, 0x45,0x89,0xFE, 0xFF,0x25,0xD3,0xFF,0xFF,0xFF")
             + x86("label", pnl("_desc"))
             + x86("raw", ".long 45, 32, 16, 24, -1, -1, -1");
    if (!strcmp(pn_kind(),"SUCCEED"))
        return x86("label", pnl("_s"))
             + x86("raw", ".byte 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0")
             + x86("raw", ".byte 0xFF,0x25,0xEA,0xFF,0xFF,0xFF, 0xFF,0x25,0xE4,0xFF,0xFF,0xFF")
             + x86("label", pnl("_desc"))
             + x86("raw", ".long 38, 32, 16, 24, -1, -1, -1");
    if (!strcmp(pn_kind(),"FENCE"))
        return x86("label", pnl("_s"))
             + x86("raw", ".byte 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0")
             + x86("raw", ".byte 0xFF,0x25,0xF2,0xFF,0xFF,0xFF, 0xFF,0x25,0xE4,0xFF,0xFF,0xFF")
             + x86("label", pnl("_desc"))
             + x86("raw", ".long 38, 32, 16, 24, -1, -1, -1");
    if (!strcmp(pn_kind(),"ABORT"))
        return x86("label", pnl("_s"))
             + x86("raw", ".byte 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0")
             + x86("raw", ".byte 0xFF,0x25,0xF2,0xFF,0xFF,0xFF, 0xFF,0x25,0xEC,0xFF,0xFF,0xFF")
             + x86("label", pnl("_desc"))
             + x86("raw", ".long 38, 32, 16, 24, -1, -1, -1");
    return x86("label", pnl("_s"))
         + x86("raw", ".byte 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0")
         + x86("raw", ".byte 0xFF,0x25,0xF2,0xFF,0xFF,0xFF, 0xFF,0x25,0xEC,0xFF,0xFF,0xFF")
         + x86("label", pnl("_desc"))
         + x86("raw", ".long 38, 32, 16, 24, -1, -1, -1");
}
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_pattern_nullary() {
    if (PLATFORM_X86) {
        pn_bump();
        return x86("label", _.lbl_α)
             + x86("comment", std::string("BOX PATTERN_") + pn_kind() + "  [BUILD ζ=r12 frag@" + pn_off() + "]")
             + x86("lea",   "rdi", FRQ(_.op_off))
             + x86("lea",   "rsi", "[rip + __]", pn_proto_addr(), pnl("_s"))
             + x86("mov32", "edx", pn_proto_len())
             + x86("lea",   "rcx", "[rip + __]", pn_desc_addr(), pnl("_desc"))
             + x86("mov32", "r8d", 0L)
             + x86("mov32", "r9d", 0L)
             + x86("push",  "rbx")
             + x86("mov",   "rbx", "rsp")
             + x86("and",   "rsp", -16L)
             + x86("call",  "rt_pattern_build", pn_fn())
             + x86("mov",   "rsp", "rbx")
             + x86("pop",   "rbx")
             + x86("jmp",   "γ")
             + pn_proto_data()
             + x86("def",   "β")
             + x86("jmp",   "ω");
    }
    return std::string();
}