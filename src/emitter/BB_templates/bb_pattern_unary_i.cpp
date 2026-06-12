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
static inline void         pu_bump()  { g_pu_seq++; }
static inline std::string  pul(const char * sfx) { return std::string(".Lpu") + std::to_string(g_pu_seq) + sfx; }
static inline const char * pu_kind()  { return _.op_kind ? _.op_kind : "LEN"; }
static inline long         pu_n()     { return (long)_.op_ival; }
static inline std::string  pu_off()   { return std::to_string((long)_.op_off); }
/*--------------------------------------------------------------------------------------------------------------------*/
static inline uint64_t pu_proto_addr() {
    if (!strcmp(pu_kind(),"POS"))  return (uint64_t)(uintptr_t)(const void *)bb_pos_proto;
    if (!strcmp(pu_kind(),"RPOS")) return (uint64_t)(uintptr_t)(const void *)bb_rpos_proto;
    if (!strcmp(pu_kind(),"TAB"))  return (uint64_t)(uintptr_t)(const void *)bb_tab_proto;
    if (!strcmp(pu_kind(),"RTAB")) return (uint64_t)(uintptr_t)(const void *)bb_rtab_proto;
    return (uint64_t)(uintptr_t)(const void *)bb_len_proto;
}
static inline uint64_t pu_desc_addr() {
    if (!strcmp(pu_kind(),"POS"))  return (uint64_t)(uintptr_t)(const void *)&bb_pos_proto_desc;
    if (!strcmp(pu_kind(),"RPOS")) return (uint64_t)(uintptr_t)(const void *)&bb_rpos_proto_desc;
    if (!strcmp(pu_kind(),"TAB"))  return (uint64_t)(uintptr_t)(const void *)&bb_tab_proto_desc;
    if (!strcmp(pu_kind(),"RTAB")) return (uint64_t)(uintptr_t)(const void *)&bb_rtab_proto_desc;
    return (uint64_t)(uintptr_t)(const void *)&bb_len_proto_desc;
}
static inline long pu_proto_len() {
    if (!strcmp(pu_kind(),"POS"))  return 61;
    if (!strcmp(pu_kind(),"RPOS")) return 66;
    if (!strcmp(pu_kind(),"TAB"))  return 83;
    if (!strcmp(pu_kind(),"RTAB")) return 83;
    return 78;
}
static inline uint64_t pu_fn() {
    void (*fp)(DTP_FRAG_t*,const void*,uint32_t,const DTP_PROTO_DESC*,long,const char*) = rt_pattern_build;
    return (uint64_t)(uintptr_t)(void *)fp;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static inline std::string pu_proto_data() {
    if (!strcmp(pu_kind(),"POS"))
        return x86("label", pul("_s"))
             + x86("raw", ".byte 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0")
             + x86("raw", ".byte 0xFF,0x25,0xF2,0xFF,0xFF,0xFF, 0x8B,0x0D,0xD4,0xFF,0xFF,0xFF, 0x41,0x39,0xCE,0x75")
             + x86("raw", ".byte 0x06,0xFF,0x25,0xD9,0xFF,0xFF,0xFF, 0xFF,0x25,0xDB,0xFF,0xFF,0xFF")
             + x86("label", pul("_desc"))
             + x86("raw", ".long 38, 32, 16, 24, -1, 0, -1");
    if (!strcmp(pu_kind(),"RPOS"))
        return x86("label", pul("_s"))
             + x86("raw", ".byte 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0")
             + x86("raw", ".byte 0xFF,0x25,0xF2,0xFF,0xFF,0xFF, 0x8B,0x0D,0xD4,0xFF,0xFF,0xFF, 0x44,0x89,0xF8,0x29")
             + x86("raw", ".byte 0xC8,0x41,0x39,0xC6,0x75,0x06, 0xFF,0x25,0xD4,0xFF,0xFF,0xFF, 0xFF,0x25,0xD6,0xFF")
             + x86("raw", ".byte 0xFF,0xFF")
             + x86("label", pul("_desc"))
             + x86("raw", ".long 38, 32, 16, 24, -1, 0, -1");
    if (!strcmp(pu_kind(),"TAB"))
        return x86("label", pul("_s"))
             + x86("raw", ".byte 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0")
             + x86("raw", ".byte 0x44,0x8B,0x35,0xE1,0xFF,0xFF,0xFF, 0xFF,0x25,0xEB,0xFF,0xFF,0xFF, 0x8B,0x0D,0xCD")
             + x86("raw", ".byte 0xFF,0xFF,0xFF, 0x44,0x39,0xF1,0x7C,0x15, 0x44,0x39,0xF9,0x7F,0x10, 0x44,0x89,0x35")
             + x86("raw", ".byte 0xC4,0xFF,0xFF,0xFF, 0x41,0x89,0xCE, 0xFF,0x25,0xC3,0xFF,0xFF,0xFF, 0xFF,0x25,0xC5")
             + x86("raw", ".byte 0xFF,0xFF,0xFF")
             + x86("label", pul("_desc"))
             + x86("raw", ".long 45, 32, 16, 24, -1, 0, -1");
    if (!strcmp(pu_kind(),"RTAB"))
        return x86("label", pul("_s"))
             + x86("raw", ".byte 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0")
             + x86("raw", ".byte 0x44,0x8B,0x35,0xE1,0xFF,0xFF,0xFF, 0xFF,0x25,0xEB,0xFF,0xFF,0xFF, 0x8B,0x0D,0xCD")
             + x86("raw", ".byte 0xFF,0xFF,0xFF, 0x44,0x89,0xF8,0x29,0xC8, 0x44,0x39,0xF0,0x7C,0x10, 0x44,0x89,0x35")
             + x86("raw", ".byte 0xC4,0xFF,0xFF,0xFF, 0x41,0x89,0xC6, 0xFF,0x25,0xC3,0xFF,0xFF,0xFF, 0xFF,0x25,0xC5")
             + x86("raw", ".byte 0xFF,0xFF,0xFF")
             + x86("label", pul("_desc"))
             + x86("raw", ".long 45, 32, 16, 24, -1, 0, -1");
    return x86("label", pul("_s"))
         + x86("raw", ".byte 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0")
         + x86("raw", ".byte 0x8B,0x0D,0xDA,0xFF,0xFF,0xFF, 0x41,0x29,0xCE, 0xFF,0x25,0xE9,0xFF,0xFF,0xFF, 0x8B")
         + x86("raw", ".byte 0x0D,0xCB,0xFF,0xFF,0xFF, 0x44,0x89,0xF0, 0x01,0xC8, 0x44,0x39,0xF8, 0x7F,0x09, 0x41")
         + x86("raw", ".byte 0x89,0xC6, 0xFF,0x25,0xC8,0xFF,0xFF,0xFF, 0xFF,0x25,0xCA,0xFF,0xFF,0xFF")
         + x86("label", pul("_desc"))
         + x86("raw", ".long 47, 32, 16, 24, -1, 0, -1");
}
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_pattern_unary_i() {
    if (PLATFORM_X86)
        return x86("label", _.lbl_α)
             + x86("comment", std::string("BOX PATTERN_") + pu_kind() + "(" + std::to_string(pu_n()) + ")  [BUILD ζ=r12 frag@" + pu_off() + "]")
             + x86("lea",   "rdi", FRQ(_.op_off))
             + x86("lea",   "rsi", "[rip + __]", pu_proto_addr(), pul("_s"))
             + x86("mov32", "edx", pu_proto_len())
             + x86("lea",   "rcx", "[rip + __]", pu_desc_addr(), pul("_desc"))
             + x86("mov32", "r8d", pu_n())
             + x86("mov32", "r9d", 0L)
             + x86("push",  "rbx")
             + x86("mov",   "rbx", "rsp")
             + x86("and",   "rsp", -16L)
             + x86("call",  "rt_pattern_build", pu_fn())
             + x86("mov",   "rsp", "rbx")
             + x86("pop",   "rbx")
             + x86("jmp",   "γ")
             + pu_proto_data()
             + x86("def",   "β")
             + x86("jmp",   "ω");
    return std::string();
}