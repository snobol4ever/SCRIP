#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "rt/rt_pl_trail.h"
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static_assert(PL_TR_CLEANUP_OFF == 32 && PL_TR_PROBE_B_OFF == 40, "bb_cleanup reads the trail header's cleanup cell at +32 and the probe's B word at +40");
#define CL_REC(k) FRQ(_.op_off + (k))
static std::string cl_hdr(const char * r) { return x86("mov", r, "r12") + x86("and", r, X86_PL_TR_ARENA_MASK); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string cl_open() {
    return x86("comment", "IR_CLEANUP open: link this frame's setup_call_cleanup record under the trail header's cleanup cell, bank B_s, mark it ACTIVE, keep C")
         + x86_alpha()
         + x86("mov", "rdx", CL_REC(CLEANUP_REC_STATE))
         + x86("test", "rdx", "rdx")
         + x86("jne", L(5))
         + x86("lea", "rcx", CL_REC(0))
         + cl_hdr("rax")
         + x86("mov", "rdx", RDQ("rax", PL_TR_CLEANUP_OFF))
         + x86("mov", CL_REC(CLEANUP_REC_LINK), "rdx")
         + x86("mov", CL_REC(CLEANUP_REC_BS), "r13")
         + x86("mov32", "edx", (long)CLEANUP_ACTIVE)
         + x86("mov", CL_REC(CLEANUP_REC_STATE), "rdx")
         + x86("mov", "rdx", FRQ(_.op_sa))
         + x86("mov", CL_REC(CLEANUP_REC_C), "rdx")
         + x86("mov", "rdx", FRQ(_.op_sa + 8))
         + x86("mov", CL_REC(CLEANUP_REC_C + 8), "rdx")
         + x86("mov", RDQ("rax", PL_TR_CLEANUP_OFF), "rcx")
         + x86_gamma()
         + x86("def", L(5))
         + x86_bomb("setup_call_cleanup: OPEN found its record still linked -- a scoped cut left it PENDING and the construct was entered again")
         + x86_beta_trampoline();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string cl_test() {
    return x86("comment",
        "IR_CLEANUP test: G's exit is deterministic when B reads B_s again (omega, to the commit and the cleanup); otherwise the record is PENDING and the construct exits with G's choices kept (gamm"
        "a)")
         + x86_alpha()
         + x86("mov", "rax", CL_REC(CLEANUP_REC_BS))
         + x86("cmp", "r13", "rax")
         + x86_omega("je")
         + x86("mov32", "edx", (long)CLEANUP_PENDING)
         + x86("mov", CL_REC(CLEANUP_REC_STATE), "rdx")
         + x86_gamma()
         + x86_beta_trampoline();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string cl_pop() {
    return x86("comment", "IR_CLEANUP pop: the record must be the youngest; unlink it and mark it DONE")
         + x86_alpha()
         + cl_hdr("rax")
         + x86("lea", "rcx", CL_REC(0))
         + x86("mov", "rdx", RDQ("rax", PL_TR_CLEANUP_OFF))
         + x86("cmp", "rdx", "rcx")
         + x86("jne", L(2))
         + x86("mov", "rdx", CL_REC(CLEANUP_REC_LINK))
         + x86("mov", RDQ("rax", PL_TR_CLEANUP_OFF), "rdx")
         + x86("mov32", "edx", (long)CLEANUP_DONE)
         + x86("mov", CL_REC(CLEANUP_REC_STATE), "rdx")
         + x86_gamma()
         + x86("def", L(2))
         + x86_bomb("setup_call_cleanup: a record popped while a younger one is still linked under the trail header's cleanup cell")
         + x86_beta_trampoline();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string cl_resume() {
    return x86("comment",
        "IR_CLEANUP resume: alpha concedes (the construct's failure exit); beta is its redo -- DONE concedes, otherwise ACTIVE again and G is retried, a ball in flight too, so G concedes into the ex"
        "haustion road")
         + x86_alpha()
         + x86_omega()
         + x86_beta()
         + x86("mov", "rax", CL_REC(CLEANUP_REC_STATE))
         + x86("cmp", "rax", (long)CLEANUP_PENDING)
         + x86_omega("jne")
         + x86("mov32", "edx", (long)CLEANUP_ACTIVE)
         + x86("mov", CL_REC(CLEANUP_REC_STATE), "rdx")
         + x86("jmp", PAIR(0));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string cl_probe() {
    return x86("comment",
        "IR_CLEANUP probe: before B rises, a PENDING youngest record whose B_s is not younger than the new B is dropped -- bank the new B in the trail header and run '$scc_cut' (gamma)")
         + x86_alpha()
         + cl_hdr("rax")
         + x86("mov", "rcx", RDQ("rax", PL_TR_CLEANUP_OFF))
         + x86("test", "rcx", "rcx")
         + x86_omega("je")
         + x86("cmp", RDQ("rcx", CLEANUP_REC_STATE), (long)CLEANUP_PENDING)
         + x86_omega("jne")
         + (_.op_sa < 0 ? x86("mov", "rdx", RDQ(x86_fb(), g_emit.flat_frame_bytes - 40)) : x86("mov", "rdx", FRQ(_.op_sa + 16)))
         + x86("test", "rdx", "rdx")
         + x86("je", L(3))
         + x86("mov", "rsi", RDQ("rcx", CLEANUP_REC_BS))
         + x86("cmp", "rsi", "rdx")
         + x86_omega("ja")
         + x86("def", L(3))
         + x86("mov", RDQ("rax", PL_TR_PROBE_B_OFF), "rdx")
         + x86_gamma()
         + x86_beta_trampoline();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string cl_drain() {
    return x86("comment", "IR_CLEANUP drain: the exhaustion road's head -- every younger record was resolved on the way here, so the record must be the youngest")
         + x86_alpha()
         + cl_hdr("rax")
         + x86("mov", "rcx", RDQ("rax", PL_TR_CLEANUP_OFF))
         + x86("lea", "rdx", CL_REC(0))
         + x86("cmp", "rcx", "rdx")
         + x86_gamma("je")
         + x86_bomb("setup_call_cleanup: the exhaustion road found a younger record still linked, or its own unlinked")
         + x86_beta_trampoline();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string cl_bank() {
    return x86("comment", "IR_CLEANUP bank: a cut scope with no mark of its own (an inline call/1) keeps B at its entry where a scoped cut's probe reads it")
         + x86_alpha()
         + x86("mov", FRQ(_.op_sa + 16), "r13")
         + x86_gamma()
         + x86_beta_trampoline();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_cleanup() {
    x86_begin();
    if (_.op_off < 0 && _.op_ival != CLEANUP_PROBE && _.op_ival != CLEANUP_BANK) return x86_alpha() + x86_bomb("bb_cleanup: no record slot (op_off)");
    if (!x86_fb_pinned()) return x86_alpha() + x86_bomb("bb_cleanup: a setup_call_cleanup record outside a pinned RBP frame");
    switch ((int)_.op_ival) {
        case CLEANUP_OPEN: return cl_open();
        case CLEANUP_TEST: return cl_test();
        case CLEANUP_POP: return cl_pop();
        case CLEANUP_RESUME: return cl_resume();
        case CLEANUP_PROBE: return cl_probe();
        case CLEANUP_DRAIN: return cl_drain();
        case CLEANUP_BANK: return cl_bank();
        default: return x86_alpha() + x86_bomb("bb_cleanup: unknown sub-kind");
    }
}
