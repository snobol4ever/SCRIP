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
static int g_pu_seq = 0;
static std::string pul(const char * sfx) { return std::string(".Lpu") + std::to_string(g_pu_seq) + sfx; }
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_pattern_unary_i() {
    if (!PLATFORM_X86) return std::string();
    g_pu_seq++;
    const char * kind = _.op_kind ? _.op_kind : "LEN";
    long n = (long)_.op_ival;
    uint64_t proto_addr; long proto_len; uint64_t desc_addr;
    if (!strcmp(kind,"POS"))       { proto_addr=(uint64_t)(uintptr_t)bb_pos_proto;  proto_len=61; desc_addr=(uint64_t)(uintptr_t)&bb_pos_proto_desc; }
    else if (!strcmp(kind,"RPOS")) { proto_addr=(uint64_t)(uintptr_t)bb_rpos_proto; proto_len=66; desc_addr=(uint64_t)(uintptr_t)&bb_rpos_proto_desc; }
    else if (!strcmp(kind,"TAB"))  { proto_addr=(uint64_t)(uintptr_t)bb_tab_proto;  proto_len=83; desc_addr=(uint64_t)(uintptr_t)&bb_tab_proto_desc; }
    else if (!strcmp(kind,"RTAB")) { proto_addr=(uint64_t)(uintptr_t)bb_rtab_proto; proto_len=83; desc_addr=(uint64_t)(uintptr_t)&bb_rtab_proto_desc; }
    else                           { proto_addr=(uint64_t)(uintptr_t)bb_len_proto;  proto_len=78; desc_addr=(uint64_t)(uintptr_t)&bb_len_proto_desc; }
    uint64_t fn; { void (*fp)(DTP_FRAG_t*,const void*,uint32_t,const DTP_PROTO_DESC*,long,const char*) = rt_pattern_build; fn=(uint64_t)(uintptr_t)(void*)fp; }
    std::string proto_data;
    if (!strcmp(kind,"POS"))
        proto_data = x86("label", pul("_s"))
             + x86("raw", ".byte 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0")
             + x86("raw", ".byte 0xFF,0x25,0xF2,0xFF,0xFF,0xFF, 0x8B,0x0D,0xD4,0xFF,0xFF,0xFF, 0x41,0x39,0xCE,0x75")
             + x86("raw", ".byte 0x06,0xFF,0x25,0xD9,0xFF,0xFF,0xFF, 0xFF,0x25,0xDB,0xFF,0xFF,0xFF")
             + x86("label", pul("_desc"))
             + x86("raw", ".long 38, 32, 16, 24, -1, 0, -1");
    else if (!strcmp(kind,"RPOS"))
        proto_data = x86("label", pul("_s"))
             + x86("raw", ".byte 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0")
             + x86("raw", ".byte 0xFF,0x25,0xF2,0xFF,0xFF,0xFF, 0x8B,0x0D,0xD4,0xFF,0xFF,0xFF, 0x44,0x89,0xF8,0x29")
             + x86("raw", ".byte 0xC8,0x41,0x39,0xC6,0x75,0x06, 0xFF,0x25,0xD4,0xFF,0xFF,0xFF, 0xFF,0x25,0xD6,0xFF")
             + x86("raw", ".byte 0xFF,0xFF")
             + x86("label", pul("_desc"))
             + x86("raw", ".long 38, 32, 16, 24, -1, 0, -1");
    else if (!strcmp(kind,"TAB"))
        proto_data = x86("label", pul("_s"))
             + x86("raw", ".byte 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0")
             + x86("raw", ".byte 0x44,0x8B,0x35,0xE1,0xFF,0xFF,0xFF, 0xFF,0x25,0xEB,0xFF,0xFF,0xFF, 0x8B,0x0D,0xCD")
             + x86("raw", ".byte 0xFF,0xFF,0xFF, 0x44,0x39,0xF1,0x7C,0x15, 0x44,0x39,0xF9,0x7F,0x10, 0x44,0x89,0x35")
             + x86("raw", ".byte 0xC4,0xFF,0xFF,0xFF, 0x41,0x89,0xCE, 0xFF,0x25,0xC3,0xFF,0xFF,0xFF, 0xFF,0x25,0xC5")
             + x86("raw", ".byte 0xFF,0xFF,0xFF")
             + x86("label", pul("_desc"))
             + x86("raw", ".long 45, 32, 16, 24, -1, 0, -1");
    else if (!strcmp(kind,"RTAB"))
        proto_data = x86("label", pul("_s"))
             + x86("raw", ".byte 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0")
             + x86("raw", ".byte 0x44,0x8B,0x35,0xE1,0xFF,0xFF,0xFF, 0xFF,0x25,0xEB,0xFF,0xFF,0xFF, 0x8B,0x0D,0xCD")
             + x86("raw", ".byte 0xFF,0xFF,0xFF, 0x44,0x89,0xF8,0x29,0xC8, 0x44,0x39,0xF0,0x7C,0x10, 0x44,0x89,0x35")
             + x86("raw", ".byte 0xC4,0xFF,0xFF,0xFF, 0x41,0x89,0xC6, 0xFF,0x25,0xC3,0xFF,0xFF,0xFF, 0xFF,0x25,0xC5")
             + x86("raw", ".byte 0xFF,0xFF,0xFF")
             + x86("label", pul("_desc"))
             + x86("raw", ".long 45, 32, 16, 24, -1, 0, -1");
    else
        proto_data = x86("label", pul("_s"))
             + x86("raw", ".byte 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0")
             + x86("raw", ".byte 0x8B,0x0D,0xDA,0xFF,0xFF,0xFF, 0x41,0x29,0xCE, 0xFF,0x25,0xE9,0xFF,0xFF,0xFF, 0x8B")
             + x86("raw", ".byte 0x0D,0xCB,0xFF,0xFF,0xFF, 0x44,0x89,0xF0, 0x01,0xC8, 0x44,0x39,0xF8, 0x7F,0x09, 0x41")
             + x86("raw", ".byte 0x89,0xC6, 0xFF,0x25,0xC8,0xFF,0xFF,0xFF, 0xFF,0x25,0xCA,0xFF,0xFF,0xFF")
             + x86("label", pul("_desc"))
             + x86("raw", ".long 47, 32, 16, 24, -1, 0, -1");
    return x86("comment", "IR_PATTERN_UNARY_I")
         + x86("label",   _.lbl_α)
         + x86("lea",     "rdi", FRQ(_.op_off))
         + x86("lea",     "rsi", "[rip + __]", proto_addr, pul("_s"))
         + x86("mov32",   "edx", proto_len)
         + x86("lea",     "rcx", "[rip + __]", desc_addr,  pul("_desc"))
         + x86("mov32",   "r8d", n)
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
