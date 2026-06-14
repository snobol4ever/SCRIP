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
std::string bb_pattern_unary_i() {
    if (!PLATFORM_X86) return std::string();
    g_pu_seq++;
    return x86("comment", "IR_PATTERN_UNARY_I")
         + x86("label",   _.lbl_α)
         + x86("lea",     "rdi", FRQ(_.op_off))
         + x86("lea",     "rsi", "[rip + __]",
                 !strcmp((_.op_kind ? _.op_kind : "LEN"), "POS")  ? (uint64_t)(uintptr_t)bb_pos_proto
               : !strcmp((_.op_kind ? _.op_kind : "LEN"), "RPOS") ? (uint64_t)(uintptr_t)bb_rpos_proto
               : !strcmp((_.op_kind ? _.op_kind : "LEN"), "TAB")  ? (uint64_t)(uintptr_t)bb_tab_proto
               : !strcmp((_.op_kind ? _.op_kind : "LEN"), "RTAB") ? (uint64_t)(uintptr_t)bb_rtab_proto
               :                       (uint64_t)(uintptr_t)bb_len_proto,
                 (std::string(".Lpu") + std::to_string(g_pu_seq) + "_s"))
         + x86("mov32",   "edx",
                 !strcmp((_.op_kind ? _.op_kind : "LEN"), "POS")  ? 61L
               : !strcmp((_.op_kind ? _.op_kind : "LEN"), "RPOS") ? 66L
               : !strcmp((_.op_kind ? _.op_kind : "LEN"), "TAB")  ? 83L
               : !strcmp((_.op_kind ? _.op_kind : "LEN"), "RTAB") ? 83L
               :                       78L)
         + x86("lea",     "rcx", "[rip + __]",
                 !strcmp((_.op_kind ? _.op_kind : "LEN"), "POS")  ? (uint64_t)(uintptr_t)&bb_pos_proto_desc
               : !strcmp((_.op_kind ? _.op_kind : "LEN"), "RPOS") ? (uint64_t)(uintptr_t)&bb_rpos_proto_desc
               : !strcmp((_.op_kind ? _.op_kind : "LEN"), "TAB")  ? (uint64_t)(uintptr_t)&bb_tab_proto_desc
               : !strcmp((_.op_kind ? _.op_kind : "LEN"), "RTAB") ? (uint64_t)(uintptr_t)&bb_rtab_proto_desc
               :                       (uint64_t)(uintptr_t)&bb_len_proto_desc,
                 (std::string(".Lpu") + std::to_string(g_pu_seq) + "_desc"))
         + x86("mov32",   "r8d", (long)_.op_ival)
         + x86("mov32",   "r9d", 0L)
         + x86("push",    "rbx")
         + x86("mov",     "rbx", "rsp")
         + x86("and",     "rsp", -16L)
         + x86("call",    "rt_pattern_build", (uint64_t)(uintptr_t)(void*)(void(*)(DTP_FRAG_t*,const void*,uint32_t,const DTP_PROTO_DESC*,long,const char*))rt_pattern_build)
         + x86("mov",     "rsp", "rbx")
         + x86("pop",     "rbx")
         + x86("jmp",     "γ")
         + (  !strcmp((_.op_kind ? _.op_kind : "LEN"), "POS")  ?
( x86("label", (std::string(".Lpu") + std::to_string(g_pu_seq) + "_s"))
           + x86("raw", ".byte 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0")
           + x86("raw", ".byte 0xFF,0x25,0xF2,0xFF,0xFF,0xFF, 0x8B,0x0D,0xD4,0xFF,0xFF,0xFF, 0x41,0x39,0xCE,0x75")
           + x86("raw", ".byte 0x06,0xFF,0x25,0xD9,0xFF,0xFF,0xFF, 0xFF,0x25,0xDB,0xFF,0xFF,0xFF")
           + x86("label", (std::string(".Lpu") + std::to_string(g_pu_seq) + "_desc"))
           + x86("raw", ".long 38, 32, 16, 24, -1, 0, -1") )
            : !strcmp((_.op_kind ? _.op_kind : "LEN"), "RPOS") ?
( x86("label", (std::string(".Lpu") + std::to_string(g_pu_seq) + "_s"))
           + x86("raw", ".byte 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0")
           + x86("raw", ".byte 0xFF,0x25,0xF2,0xFF,0xFF,0xFF, 0x8B,0x0D,0xD4,0xFF,0xFF,0xFF, 0x44,0x89,0xF8,0x29")
           + x86("raw", ".byte 0xC8,0x41,0x39,0xC6,0x75,0x06, 0xFF,0x25,0xD4,0xFF,0xFF,0xFF, 0xFF,0x25,0xD6,0xFF")
           + x86("raw", ".byte 0xFF,0xFF")
           + x86("label", (std::string(".Lpu") + std::to_string(g_pu_seq) + "_desc"))
           + x86("raw", ".long 38, 32, 16, 24, -1, 0, -1") )
            : !strcmp((_.op_kind ? _.op_kind : "LEN"), "TAB")  ?
( x86("label", (std::string(".Lpu") + std::to_string(g_pu_seq) + "_s"))
           + x86("raw", ".byte 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0")
           + x86("raw", ".byte 0x44,0x8B,0x35,0xE1,0xFF,0xFF,0xFF, 0xFF,0x25,0xEB,0xFF,0xFF,0xFF, 0x8B,0x0D,0xCD")
           + x86("raw", ".byte 0xFF,0xFF,0xFF, 0x44,0x39,0xF1,0x7C,0x15, 0x44,0x39,0xF9,0x7F,0x10, 0x44,0x89,0x35")
           + x86("raw", ".byte 0xC4,0xFF,0xFF,0xFF, 0x41,0x89,0xCE, 0xFF,0x25,0xC3,0xFF,0xFF,0xFF, 0xFF,0x25,0xC5")
           + x86("raw", ".byte 0xFF,0xFF,0xFF")
           + x86("label", (std::string(".Lpu") + std::to_string(g_pu_seq) + "_desc"))
           + x86("raw", ".long 45, 32, 16, 24, -1, 0, -1") )
            : !strcmp((_.op_kind ? _.op_kind : "LEN"), "RTAB") ?
( x86("label", (std::string(".Lpu") + std::to_string(g_pu_seq) + "_s"))
           + x86("raw", ".byte 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0")
           + x86("raw", ".byte 0x44,0x8B,0x35,0xE1,0xFF,0xFF,0xFF, 0xFF,0x25,0xEB,0xFF,0xFF,0xFF, 0x8B,0x0D,0xCD")
           + x86("raw", ".byte 0xFF,0xFF,0xFF, 0x44,0x89,0xF8,0x29,0xC8, 0x44,0x39,0xF0,0x7C,0x10, 0x44,0x89,0x35")
           + x86("raw", ".byte 0xC4,0xFF,0xFF,0xFF, 0x41,0x89,0xC6, 0xFF,0x25,0xC3,0xFF,0xFF,0xFF, 0xFF,0x25,0xC5")
           + x86("raw", ".byte 0xFF,0xFF,0xFF")
           + x86("label", (std::string(".Lpu") + std::to_string(g_pu_seq) + "_desc"))
           + x86("raw", ".long 45, 32, 16, 24, -1, 0, -1") )
            :
( x86("label", (std::string(".Lpu") + std::to_string(g_pu_seq) + "_s"))
           + x86("raw", ".byte 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0")
           + x86("raw", ".byte 0x8B,0x0D,0xDA,0xFF,0xFF,0xFF, 0x41,0x29,0xCE, 0xFF,0x25,0xE9,0xFF,0xFF,0xFF, 0x8B")
           + x86("raw", ".byte 0x0D,0xCB,0xFF,0xFF,0xFF, 0x44,0x89,0xF0, 0x01,0xC8, 0x44,0x39,0xF8, 0x7F,0x09, 0x41")
           + x86("raw", ".byte 0x89,0xC6, 0xFF,0x25,0xC8,0xFF,0xFF,0xFF, 0xFF,0x25,0xCA,0xFF,0xFF,0xFF")
           + x86("label", (std::string(".Lpu") + std::to_string(g_pu_seq) + "_desc"))
           + x86("raw", ".long 47, 32, 16, 24, -1, 0, -1") )
           )
         + x86("def",     "β")
         + x86("jmp",     "ω");
}
