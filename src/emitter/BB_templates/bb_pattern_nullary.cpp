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
static std::string pnl(const char * sfx) { return std::string(".Lpn") + std::to_string(g_pn_seq) + sfx; }
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_pattern_nullary() {
    if (!PLATFORM_X86) return std::string();
    g_pn_seq++;
    const char * kind = _.op_kind ? _.op_kind : "FAIL";
    uint64_t proto_addr; long proto_len; uint64_t desc_addr;
    if (!strcmp(kind,"REM"))         { proto_addr=(uint64_t)(uintptr_t)bb_rem_proto;     proto_len=61; desc_addr=(uint64_t)(uintptr_t)&bb_rem_proto_desc; }
    else if (!strcmp(kind,"SUCCEED")){ proto_addr=(uint64_t)(uintptr_t)bb_succeed_proto; proto_len=44; desc_addr=(uint64_t)(uintptr_t)&bb_succeed_proto_desc; }
    else if (!strcmp(kind,"FENCE"))  { proto_addr=(uint64_t)(uintptr_t)bb_fence_proto;   proto_len=44; desc_addr=(uint64_t)(uintptr_t)&bb_fence_proto_desc; }
    else if (!strcmp(kind,"ABORT"))  { proto_addr=(uint64_t)(uintptr_t)bb_abort_proto;   proto_len=44; desc_addr=(uint64_t)(uintptr_t)&bb_abort_proto_desc; }
    else                             { proto_addr=(uint64_t)(uintptr_t)bb_fail_proto;    proto_len=44; desc_addr=(uint64_t)(uintptr_t)&bb_fail_proto_desc; }
    uint64_t fn; { void (*fp)(DTP_FRAG_t*,const void*,uint32_t,const DTP_PROTO_DESC*,long,const char*) = rt_pattern_build; fn=(uint64_t)(uintptr_t)(void*)fp; }
    std::string proto_data;
    if (!strcmp(kind,"REM"))
        proto_data = x86("label", pnl("_s"))
             + x86("raw", ".byte 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0")
             + x86("raw", ".byte 0x44,0x8B,0x35,0xE1,0xFF,0xFF,0xFF, 0xFF,0x25,0xEB,0xFF,0xFF,0xFF, 0x44,0x89,0x35")
             + x86("raw", ".byte 0xD4,0xFF,0xFF,0xFF, 0x45,0x89,0xFE, 0xFF,0x25,0xD3,0xFF,0xFF,0xFF")
             + x86("label", pnl("_desc"))
             + x86("raw", ".long 45, 32, 16, 24, -1, -1, -1");
    else if (!strcmp(kind,"SUCCEED"))
        proto_data = x86("label", pnl("_s"))
             + x86("raw", ".byte 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0")
             + x86("raw", ".byte 0xFF,0x25,0xEA,0xFF,0xFF,0xFF, 0xFF,0x25,0xE4,0xFF,0xFF,0xFF")
             + x86("label", pnl("_desc"))
             + x86("raw", ".long 38, 32, 16, 24, -1, -1, -1");
    else if (!strcmp(kind,"FENCE"))
        proto_data = x86("label", pnl("_s"))
             + x86("raw", ".byte 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0")
             + x86("raw", ".byte 0xFF,0x25,0xF2,0xFF,0xFF,0xFF, 0xFF,0x25,0xE4,0xFF,0xFF,0xFF")
             + x86("label", pnl("_desc"))
             + x86("raw", ".long 38, 32, 16, 24, -1, -1, -1");
    else if (!strcmp(kind,"ABORT"))
        proto_data = x86("label", pnl("_s"))
             + x86("raw", ".byte 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0")
             + x86("raw", ".byte 0xFF,0x25,0xF2,0xFF,0xFF,0xFF, 0xFF,0x25,0xEC,0xFF,0xFF,0xFF")
             + x86("label", pnl("_desc"))
             + x86("raw", ".long 38, 32, 16, 24, -1, -1, -1");
    else
        proto_data = x86("label", pnl("_s"))
             + x86("raw", ".byte 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0")
             + x86("raw", ".byte 0xFF,0x25,0xF2,0xFF,0xFF,0xFF, 0xFF,0x25,0xEC,0xFF,0xFF,0xFF")
             + x86("label", pnl("_desc"))
             + x86("raw", ".long 38, 32, 16, 24, -1, -1, -1");
    return x86("comment", "IR_PATTERN_NULLARY")
         + x86("label",   _.lbl_α)
         + x86("lea",     "rdi", FRQ(_.op_off))
         + x86("lea",     "rsi", "[rip + __]", proto_addr, pnl("_s"))
         + x86("mov32",   "edx", proto_len)
         + x86("lea",     "rcx", "[rip + __]", desc_addr,  pnl("_desc"))
         + x86("mov32",   "r8d", 0L)
         + x86("mov32",   "r9d", 0L)
         + x86("push",    "rbx")
         + x86("mov",     "rbx", "rsp")
         + x86("and",     "rsp", -16L)
         + x86("call",    "rt_pattern_build", fn)
         + x86("mov",     "rsp", "rbx")
         + x86("pop",     "rbx")
         + x86("jmp",     "γ")
         + proto_data
         + x86("def",     "β")
         + x86("jmp",     "ω");
}
