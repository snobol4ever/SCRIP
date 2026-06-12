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
static int g_us_seq = 0;
static inline void         us_bump()  { g_us_seq++; }
static inline std::string  usl(const char * sfx) { return std::string(".Lus") + std::to_string(g_us_seq) + sfx; }
static inline const char * us_kind()  { return _.op_kind ? _.op_kind : "ANY"; }
static inline const char * us_cset()  { return _.op_sval ? _.op_sval : ""; }
static inline std::string  us_off()   { return std::to_string((long)_.op_off); }
static inline std::string  us_label() { const char * l = emit_intern_str(us_cset()); if (l) return l;
                                        static char b[24]; strtab_label(b, sizeof b, us_cset()); return b; }
static inline uint64_t     us_addr()  { return (uint64_t)(uintptr_t)us_cset(); }
/*--------------------------------------------------------------------------------------------------------------------*/
static inline uint64_t us_proto_addr() {
    if (!strcmp(us_kind(),"NOTANY")) return (uint64_t)(uintptr_t)(const void *)bb_notany_proto;
    if (!strcmp(us_kind(),"SPAN"))   return (uint64_t)(uintptr_t)(const void *)bb_span_proto;
    if (!strcmp(us_kind(),"BREAK"))  return (uint64_t)(uintptr_t)(const void *)bb_break_proto;
    if (!strcmp(us_kind(),"BREAKX")) return (uint64_t)(uintptr_t)(const void *)bb_breakx_proto;
    return (uint64_t)(uintptr_t)(const void *)bb_any_proto;
}
static inline uint64_t us_desc_addr() {
    if (!strcmp(us_kind(),"NOTANY")) return (uint64_t)(uintptr_t)(const void *)&bb_notany_proto_desc;
    if (!strcmp(us_kind(),"SPAN"))   return (uint64_t)(uintptr_t)(const void *)&bb_span_proto_desc;
    if (!strcmp(us_kind(),"BREAK"))  return (uint64_t)(uintptr_t)(const void *)&bb_break_proto_desc;
    if (!strcmp(us_kind(),"BREAKX")) return (uint64_t)(uintptr_t)(const void *)&bb_breakx_proto_desc;
    return (uint64_t)(uintptr_t)(const void *)&bb_any_proto_desc;
}
static inline long us_proto_len() {
    if (!strcmp(us_kind(),"NOTANY")) return 89;
    if (!strcmp(us_kind(),"SPAN"))   return 101;
    if (!strcmp(us_kind(),"BREAK"))  return 96;
    if (!strcmp(us_kind(),"BREAKX")) return 165;
    return 89;
}
static inline uint64_t us_fn() {
    void (*fp)(DTP_FRAG_t*,const void*,uint32_t,const DTP_PROTO_DESC*,long,const char*) = rt_pattern_build;
    return (uint64_t)(uintptr_t)(void *)fp;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static inline std::string us_proto_data() {
    if (!strcmp(us_kind(),"NOTANY"))
        return x86("label", usl("_s"))
             + x86("raw", ".byte 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0")
             + x86("raw", ".byte 0xFF,0x25,0xF2,0xFF,0xFF,0xFF,0x45,0x39,0xFE,0x7D,0x28,0x49,0x63,0xC6,0x41,0x0F")
             + x86("raw", ".byte 0xB6,0x54,0x05,0x00,0x48,0x8B,0x35,0xC5,0xFF,0xFF,0xFF,0x8A,0x0E,0x84,0xC9,0x74")
             + x86("raw", ".byte 0x09,0x38,0xD1,0x74,0x0E,0x48,0xFF,0xC6,0xEB,0xF1,0x41,0xFF,0xC6,0xFF,0x25,0xBD")
             + x86("raw", ".byte 0xFF,0xFF,0xFF,0xFF,0x25,0xBF,0xFF,0xFF,0xFF")
             + x86("label", usl("_desc"))
             + x86("raw", ".long 38, 32, 16, 24, -1, 0, -1");
    if (!strcmp(us_kind(),"SPAN"))
        return x86("label", usl("_s"))
             + x86("raw", ".byte 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0")
             + x86("raw", ".byte 0xFF,0x25,0xF2,0xFF,0xFF,0xFF,0x44,0x89,0xF0,0x44,0x39,0xF8,0x7D,0x23,0x4C,0x63")
             + x86("raw", ".byte 0xC8,0x43,0x0F,0xB6,0x54,0x0D,0x00,0x48,0x8B,0x35,0xC2,0xFF,0xFF,0xFF,0x8A,0x0E")
             + x86("raw", ".byte 0x84,0xC9,0x74,0x0D,0x38,0xD1,0x74,0x05,0x48,0xFF,0xC6,0xEB,0xF1,0xFF,0xC0,0xEB")
             + x86("raw", ".byte 0xD8,0x44,0x39,0xF0,0x74,0x09,0x41,0x89,0xC6,0xFF,0x25,0xB1,0xFF,0xFF,0xFF,0xFF")
             + x86("raw", ".byte 0x25,0xB3,0xFF,0xFF,0xFF")
             + x86("label", usl("_desc"))
             + x86("raw", ".long 38, 32, 16, 24, -1, 0, -1");
    if (!strcmp(us_kind(),"BREAK"))
        return x86("label", usl("_s"))
             + x86("raw", ".byte 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0")
             + x86("raw", ".byte 0xFF,0x25,0xF2,0xFF,0xFF,0xFF,0x44,0x89,0xF0,0x44,0x39,0xF8,0x7D,0x2C,0x4C,0x63")
             + x86("raw", ".byte 0xC8,0x43,0x0F,0xB6,0x54,0x0D,0x00,0x48,0x8B,0x35,0xC2,0xFF,0xFF,0xFF,0x8A,0x0E")
             + x86("raw", ".byte 0x84,0xC9,0x74,0x09,0x38,0xD1,0x74,0x09,0x48,0xFF,0xC6,0xEB,0xF1,0xFF,0xC0,0xEB")
             + x86("raw", ".byte 0xD8,0x41,0x89,0xC6,0xFF,0x25,0xB6,0xFF,0xFF,0xFF,0xFF,0x25,0xB8,0xFF,0xFF,0xFF")
             + x86("label", usl("_desc"))
             + x86("raw", ".long 38, 32, 16, 24, -1, 0, -1");
    if (!strcmp(us_kind(),"BREAKX"))
        return x86("label", usl("_s"))
             + x86("raw", ".byte 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0")
             + x86("raw", ".byte 0x8B,0x05,0xE2,0xFF,0xFF,0xFF,0xFF,0xC0,0x44,0x39,0xF8,0x7D,0x32,0x4C,0x63,0xC8")
             + x86("raw", ".byte 0x43,0x0F,0xB6,0x54,0x0D,0x00,0x48,0x8B,0x35,0xC3,0xFF,0xFF,0xFF,0x8A,0x0E,0x84")
             + x86("raw", ".byte 0xC9,0x74,0x09,0x38,0xD1,0x74,0x09,0x48,0xFF,0xC6,0xEB,0xF1,0xFF,0xC0,0xEB,0xD8")
             + x86("raw", ".byte 0x89,0x05,0xB2,0xFF,0xFF,0xFF,0x41,0x89,0xC6,0xFF,0x25,0xB1,0xFF,0xFF,0xFF,0xFF")
             + x86("raw", ".byte 0x25,0xB3,0xFF,0xFF,0xFF,0x44,0x89,0xF0,0x44,0x39,0xF8,0x7D,0x32,0x4C,0x63,0xC8")
             + x86("raw", ".byte 0x43,0x0F,0xB6,0x54,0x0D,0x00,0x48,0x8B,0x35,0x83,0xFF,0xFF,0xFF,0x8A,0x0E,0x84")
             + x86("raw", ".byte 0xC9,0x74,0x09,0x38,0xD1,0x74,0x09,0x48,0xFF,0xC6,0xEB,0xF1,0xFF,0xC0,0xEB,0xD8")
             + x86("raw", ".byte 0x89,0x05,0x72,0xFF,0xFF,0xFF,0x41,0x89,0xC6,0xFF,0x25,0x71,0xFF,0xFF,0xFF,0xFF")
             + x86("raw", ".byte 0x25,0x73,0xFF,0xFF,0xFF")
             + x86("label", usl("_desc"))
             + x86("raw", ".long 101, 32, 16, 24, -1, 0, -1");
    return x86("label", usl("_s"))
         + x86("raw", ".byte 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0")
         + x86("raw", ".byte 0xFF,0x25,0xF2,0xFF,0xFF,0xFF,0x45,0x39,0xFE,0x7D,0x28,0x49,0x63,0xC6,0x41,0x0F")
         + x86("raw", ".byte 0xB6,0x54,0x05,0x00,0x48,0x8B,0x35,0xC5,0xFF,0xFF,0xFF,0x8A,0x0E,0x84,0xC9,0x74")
         + x86("raw", ".byte 0x12,0x38,0xD1,0x74,0x05,0x48,0xFF,0xC6,0xEB,0xF1,0x41,0xFF,0xC6,0xFF,0x25,0xBD")
         + x86("raw", ".byte 0xFF,0xFF,0xFF,0xFF,0x25,0xBF,0xFF,0xFF,0xFF")
         + x86("label", usl("_desc"))
         + x86("raw", ".long 38, 32, 16, 24, -1, 0, -1");
}
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_pattern_unary_s() {
    if (PLATFORM_X86) {
        us_bump();
        return x86("label", _.lbl_α)
             + x86("comment", std::string("BOX PATTERN_") + us_kind() + "('" + us_cset() + "')  [BUILD ζ=r12 frag@" + us_off() + "]")
             + x86("lea",   "rdi", FRQ(_.op_off))
             + x86("lea",   "rsi", "[rip + __]", us_proto_addr(), usl("_s"))
             + x86("mov32", "edx", us_proto_len())
             + x86("lea",   "rcx", "[rip + __]", us_desc_addr(), usl("_desc"))
             + x86("mov32", "r8d", 0L)
             + x86("lea",   "r9",  "[rip + __]", us_addr(), us_label())
             + x86("push",  "rbx")
             + x86("mov",   "rbx", "rsp")
             + x86("and",   "rsp", -16L)
             + x86("call",  "rt_pattern_build", us_fn())
             + x86("mov",   "rsp", "rbx")
             + x86("pop",   "rbx")
             + x86("jmp",   "γ")
             + us_proto_data()
             + x86("def",   "β")
             + x86("jmp",   "ω");
    }
    return std::string();
}