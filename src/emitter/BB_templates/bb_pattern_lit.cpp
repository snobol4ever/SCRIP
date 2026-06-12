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
static int g_pb_seq = 0;
static inline void         pb_bump()     { g_pb_seq++; }
static inline std::string  pbl(const char * sfx) { return std::string(".Lpb") + std::to_string(g_pb_seq) + sfx; }
static inline const char * plit()        { return _.op_sval ? _.op_sval : ""; }
static inline long         plitlen()     { return (long)strlen(plit()); }
static inline const char * plitlabel()   { const char * l = emit_intern_str(plit()); if (l) return l;
                                           static char b[24]; strtab_label(b, sizeof b, plit()); return b; }
static inline uint64_t     plitaddr()    { return (uint64_t)(uintptr_t)plit(); }
static inline std::string  pb_off()      { return std::to_string((long)_.op_off); }
static inline uint64_t     proto_addr()  { return (uint64_t)(uintptr_t)(const void *)bb_lit_proto; }
static inline uint64_t     desc_addr()   { return (uint64_t)(uintptr_t)(const void *)&bb_lit_proto_desc; }
static inline uint64_t     pb_fn()       { void (*fp)(DTP_FRAG_t*,const void*,uint32_t,const DTP_PROTO_DESC*,long,const char*) = rt_pattern_build;
                                           return (uint64_t)(uintptr_t)(void *)fp; }
static inline std::string  pb_proto_data() {
    return x86("label", pbl("_s"))
         + x86("raw", ".byte 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0")
         + x86("raw", ".byte 0x8B,0x0D,0xE2,0xFF,0xFF,0xFF, 0x41,0x29,0xCE, 0xFF,0x25,0xE9,0xFF,0xFF,0xFF")
         + x86("raw", ".byte 0x8B,0x0D,0xD3,0xFF,0xFF,0xFF, 0x44,0x89,0xF0, 0x01,0xC8, 0x44,0x39,0xF8")
         + x86("raw", ".byte 0x7F,0x38, 0x48,0x8B,0x35,0xBA,0xFF,0xFF,0xFF")
         + x86("raw", ".byte 0x49,0x63,0xCE, 0x49,0x8D,0x7C,0x0D,0x00, 0x8B,0x0D,0xB4,0xFF,0xFF,0xFF")
         + x86("raw", ".byte 0x85,0xC9, 0x74,0x10, 0x8A,0x06, 0x3A,0x07, 0x75,0x19")
         + x86("raw", ".byte 0x48,0xFF,0xC6, 0x48,0xFF,0xC7, 0xFF,0xC9, 0xEB,0xEC")
         + x86("raw", ".byte 0x8B,0x0D,0x9A,0xFF,0xFF,0xFF, 0x41,0x01,0xCE")
         + x86("raw", ".byte 0xFF,0x25,0x99,0xFF,0xFF,0xFF, 0xFF,0x25,0x9B,0xFF,0xFF,0xFF")
         + x86("label", pbl("_desc"))
         + x86("raw", ".long 47, 32, 16, 24, -1, 0, 8");
}
/*--------------------------------------------------------------------------------------------------------------------*/
std::string bb_pattern_lit() {
    if (PLATFORM_X86) {
        pb_bump();
        return x86("label", _.lbl_α)
             + x86("comment", std::string("BOX PATTERN_LIT('") + plit() + "')  [BUILD ζ=r12 frag@" + pb_off() + "]")
             + x86("lea",   "rdi", FRQ(_.op_off))
             + x86("lea",   "rsi", "[rip + __]", proto_addr(), pbl("_s"))
             + x86("mov32", "edx", 125L)
             + x86("lea",   "rcx", "[rip + __]", desc_addr(), pbl("_desc"))
             + x86("mov32", "r8d", plitlen())
             + x86("lea",   "r9",  "[rip + __]", plitaddr(), plitlabel())
             + x86("push",  "rbx")
             + x86("mov",   "rbx", "rsp")
             + x86("and",   "rsp", -16L)
             + x86("call",  "rt_pattern_build", pb_fn())
             + x86("mov",   "rsp", "rbx")
             + x86("pop",   "rbx")
             + x86("jmp",   "γ")
             + pb_proto_data()
             + x86("def",   "β")
             + x86("jmp",   "ω");
    }
    return std::string();
}