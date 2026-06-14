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
static int g_pa_seq = 0;
std::string bb_pattern_arb() {
    if (!PLATFORM_X86) return std::string();
    g_pa_seq++;
    return x86("comment", "IR_PATTERN_ARB")
         + x86("label",   _.lbl_α)
         + x86("lea",     "rdi", FRQ(_.op_off))
         + x86("lea",     "rsi", "[rip + __]", (uint64_t)(uintptr_t)(const void *)bb_arb_proto, (std::string(".Lpa") + std::to_string(g_pa_seq) + "_s"))
         + x86("mov32",   "edx", 104L)
         + x86("lea",     "rcx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)&bb_arb_proto_desc, (std::string(".Lpa") + std::to_string(g_pa_seq) + "_desc"))
         + x86("mov32",   "r8d", 0L)
         + x86("mov32",   "r9d", 0L)
         + x86("push",    "rbx")
         + x86("mov",     "rbx", "rsp")
         + x86("and",     "rsp", -16L)
         + x86("call",    "rt_pattern_build", (uint64_t)(uintptr_t)(void*)(void(*)(DTP_FRAG_t*,const void*,uint32_t,const DTP_PROTO_DESC*,long,const char*))rt_pattern_build)
         + x86("mov",     "rsp", "rbx")
         + x86("pop",     "rbx")
         + x86("jmp",     "γ")
         + x86("label", (std::string(".Lpa") + std::to_string(g_pa_seq) + "_s"))
         + x86("raw", ".byte 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0")
         + x86("raw", ".byte 0x8B,0x05,0xE2,0xFF,0xFF,0xFF,0x8B,0x0D,0xE0,0xFF,0xFF,0xFF,0xFF,0xC1,0x89,0x0D")
         + x86("raw", ".byte 0xD8,0xFF,0xFF,0xFF,0x01,0xC8,0x44,0x39,0xF8,0x7F,0x09,0x41,0x89,0xC6,0xFF,0x25")
         + x86("raw", ".byte 0xCC,0xFF,0xFF,0xFF,0x44,0x8B,0x35,0xBD,0xFF,0xFF,0xFF,0xFF,0x25,0xC7,0xFF,0xFF")
         + x86("raw", ".byte 0xFF,0x44,0x89,0x35,0xB0,0xFF,0xFF,0xFF,0xC7,0x05,0xAA,0xFF,0xFF,0xFF,0x00,0x00")
         + x86("raw", ".byte 0x00,0x00,0xFF,0x25,0xA8,0xFF,0xFF,0xFF")
         + x86("label", (std::string(".Lpa") + std::to_string(g_pa_seq) + "_desc"))
         + x86("raw", ".long 81, 32, 16, 24, -1, -1, -1")
         + x86("def",     "β")
         + x86("jmp",     "ω");
}
