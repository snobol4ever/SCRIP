#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
typedef struct { uint64_t ptr; uint64_t len; } ScanSubjRegs;
ScanSubjRegs rt_match_enter(uint64_t lo, uint64_t hi);
void rt_match_ctx_restore(uint64_t sig, uint64_t len, uint64_t capgen);
extern "C" long *rt_anchor_ptr(void);
}
#include "x86_asm.h"
#define subjc() (_.op_subj_cell)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_begin() {
    x86_begin();
    return x86("comment", "IR_MATCH_BEGIN")
         + x86_alpha()
         + IF(_.op_zres, x86("note", ZOPN(0))
                       + x86("mov", "rdi", x86_zref(_.op_zread[0] + 0, 1))
                       + x86("note", ZOPN(0))
                       + x86("mov", "rsi", x86_zref(_.op_zread[0] + 8, 1)))
         + IF(!_.op_zres && subjc(), x86("mov", "rdi", "qword ptr [rsp + 0]")
                     + x86("mov", "rsi", "qword ptr [rsp + 8]")
                     + x86_zrelease(16))
         + IF(!_.op_zres && !subjc(), x86("mov", "rdi", FRQ(_.op_sa))
                      + x86("mov", "rsi", FRQ(_.op_sa + 8)))
         + x86("push", "rbp")
         + x86("mov",  "rbp", "rsp")
         + x86("note", "cas_mark")
         + x86("push", "r12")
         + x86("note", HKN(1))
         + x86("push", "r13")
         + x86("note", HKN(2))
         + x86("push", "r14")
         + x86("note", HKN(3))
         + x86("push", "r15")
         + x86("sub",  "rsp", (long)(24 + _.op_frame_extra))
         + x86("rtcc_wb")
         + x86("call_bare", "rt_match_enter", (uint64_t)(uintptr_t)(void *)rt_match_enter)
         + x86("mov", "r13", "rax")
         + x86("mov", "r15", "rdx")
         + x86("rtcc_rl")
         + x86("note", "start_δ")
         + x86("mov", RDD("rbp", -40), (long)0)
         + x86("def", L(0))
         + x86("note", "start_δ")
         + x86("mov", "r14d", RDD("rbp", -40))
         + IF(({ static int _bg = -1; if (_bg < 0) { const char * e = getenv("SCRIP_DEFER_BETA_GUARD"); _bg = (e && *e == '0') ? 0 : 1; } _bg; }),
               x86("note", "match_beta_cont")
             + x86("mov", "rcx", "[rip@got + __]", (uint64_t)(uintptr_t)(const void *)&rtccb[0], "rtccb")
             + x86("mov", "rax", RDQ("rcx", 248))
             + x86("mov", RDQ("rbp", -48), "rax")
             + x86_lea_id("rax", 13)
             + x86("mov", RDQ("rcx", 248), "rax"))
         + x86_gamma()
         + x86_beta()
         + IF(({ static int _bg2 = -1; if (_bg2 < 0) { const char * e = getenv("SCRIP_DEFER_BETA_GUARD"); _bg2 = (e && *e == '0') ? 0 : 1; } _bg2; }), x86("def", L(13)))
         + x86("note", "retry_whack")
         + x86("lea", "rsp", RDQ("rbp", -56 - _.op_frame_extra))
         + x86("note", "start_δ")
         + x86("add", RDD("rbp", -40), (long)1)
         + x86("note", "start_δ")
         + x86("mov", "eax", RDD("rbp", -40))
         + x86("cmp", "eax", "r15d")
         + x86("jg",  L(1))
         + x86("mov", "rcx", "[rip@got + __]", (uint64_t)(uintptr_t)(const void *)rt_anchor_ptr(), "rt_anchor_g")
         + x86("mov", "rax", "[rcx]")
         + x86("cmp64", "rax", (long)0)
         + x86("jne", L(1))
         + x86("jmp", L(0))
         + x86("def", L(1))
         + IF(g_emit.xa_bb_emit_pair_n >= 4 && g_emit.xa_bb_emit_pair_define[3] != NULL, x86("def", PAIR(3)))
         + IF(g_emit.xa_bb_emit_pair_n >= 5 && g_emit.xa_bb_emit_pair_define[4] != NULL, x86("def", PAIR(4)))
         + IF(({ static int _bg3 = -1; if (_bg3 < 0) { const char * e = getenv("SCRIP_DEFER_BETA_GUARD"); _bg3 = (e && *e == '0') ? 0 : 1; } _bg3; }),
               x86("note", "mbc_restore")
             + x86("mov", "rcx", "[rip@got + __]", (uint64_t)(uintptr_t)(const void *)&rtccb[0], "rtccb")
             + x86("mov", "rax", RDQ("rbp", -48))
             + x86("mov", RDQ("rcx", 248), "rax"))
         + x86("note", "cas_mark")
         + x86("mov", "r12", RDQ("rbp", -8))
         + x86("note", HKN(1))
         + x86("mov", "r13", RDQ("rbp", -16))
         + x86("note", HKN(2))
         + x86("mov", "r14", RDQ("rbp", -24))
         + x86("note", HKN(3))
         + x86("mov", "r15", RDQ("rbp", -32))
         + x86("mov", "rdi", "r13")
         + x86("mov", "rsi", "r15")
         + x86("call", "rt_match_ctx_restore", (uint64_t)(uintptr_t)(void *)rt_match_ctx_restore)
         + x86("mov", "rsp", "rbp")
         + x86("pop", "rbp")
         + x86_omega();
}
