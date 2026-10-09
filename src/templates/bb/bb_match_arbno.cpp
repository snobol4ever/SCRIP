#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
extern "C" void * rt_zcol_push(void ** ptr_cell, int * cap_cell, int i, long elem_sz);
extern "C" int sn4_arbno_seal_omega(void);
extern "C" int sn4_arbno_tailbeta(void);
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define kkN() (_.op_arbno_body_kk)
#define ARB_BODYBETA() (sn4_arbno_tailbeta() ? PAIR(4) : PAIR(1))
#define ARB_CS() (32L + ((_.op_arbno_win_bytes + 15) & ~15))
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define bb_match_arbno_frameless_k() ( \
      x86("comment", "IR_MATCH_ARBNO_FRAMELESS_K (ARB-LON-K16: re-homed cell, one-level static offsets)") \
    + x86_alpha() \
    + x86("sub", "rsp", 16L) \
    + x86("mov", RDD("rsp", 0), "r14d") \
    + x86("mov", RDD("rsp", 4), "r14d") \
    + x86_gamma() \
    + x86_beta() \
    + x86("jmp", PAIR(0)) \
    + x86("def", PAIR(2)) \
    + x86("mov", "eax", RDD("rsp", kkN() + 4)) \
    + x86("cmp", "r14d", "eax") \
    + x86("je", PAIR(1)) \
    + x86("mov", "eax", RDD("rsp", kkN())) \
    + x86("sub", "rsp", 16L) \
    + x86("mov", RDD("rsp", 0), "eax") \
    + x86("mov", RDD("rsp", 4), "r14d") \
    + x86_gamma() \
    + x86("def", PAIR(3)) \
    + x86("def", PAIR(5)) \
    + x86("mov", "eax", RDD("rsp", 0)) \
    + x86("cmp", "r14d", "eax") \
    + x86("jne", L(3)) \
    + x86("add", "rsp", 16L) \
    + x86_omega() \
    + x86("def", L(3)) \
    + x86("add", "rsp", 16L) \
    + x86("jmp", PAIR(1)) \
)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_match_arbno_frameless() {
    return x86("comment", "IR_MATCH_ARBNO_FRAMELESS (ARBNO-LON: one cell, two compares, no chain)")
         + x86_alpha()
         + x86("sub", "rsp", 16L)
         + x86("mov", RDD("rsp", 0), "r14d")
         + x86("mov", RDD("rsp", 4), "r14d")
         + x86_gamma()
         + x86_beta()
         + x86("jmp", PAIR(0))
         + x86("def", PAIR(2))
         + x86("mov", "eax", RDD("rsp", 4))
         + x86("cmp", "r14d", "eax")
         + x86("je", ARB_BODYBETA())
         + x86("mov", RDD("rsp", 4), "r14d")
         + x86_gamma()
         + x86("def", PAIR(3))
         + x86("def", PAIR(5))
         + x86("mov", "eax", RDD("rsp", 0))
         + x86("cmp", "r14d", "eax")
         + IF(!(_.op_tail_seal && sn4_arbno_seal_omega()), x86("jne", ARB_BODYBETA()))
         + x86("add", "rsp", 16L)
         + x86_omega();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define arbno_win_save(dst) emit_for(0, (_.op_arbno_win_bytes + 7) / 8, [&](int k) { return x86("mov", "rax", RDQ("rbp", _.op_arbno_win_lo + k * 8)) \
                                                                                   + x86("mov", RDQ("rsp", (dst) + k * 8), "rax"); })
#define arbno_win_restore(cell, src) emit_for(0, (_.op_arbno_win_bytes + 7) / 8, [&](int k) { return x86("mov", "rax", RDQ((cell), (src) + k * 8)) \
                                                                                           + x86("mov", RDQ("rbp", _.op_arbno_win_lo + k * 8), "rax"); })
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_match_arbno_frame() {
    return x86_alpha()
         + x86("sub", "rsp", ARB_CS())
         + x86("mov", RDD("rsp", 0), "r14d")
         + x86("mov", RDD("rsp", 4), "r14d")
         + x86("mov", RDQ("rsp", 8), "r12")
         + x86("mov", "rax", AFCQ(0))
         + x86("mov", RDQ("rsp", 16), "rax")
         + x86("mov", AFCQ(0), "rsp")
         + x86_gamma()
         + x86_beta()
         + x86("comment", "PEND-MARK b: roll the pend cursor back to the last COMMITTED instance before retrying the body")
         + x86("mov", "rax", AFCQ(0))
         + x86("mov", "r12", RDQ("rax", 8))
         + x86("jmp", PAIR(0))
         + x86("def", PAIR(2))
         + x86("mov", "rcx", AFCQ(0))
         + x86("mov", "eax", RDD("rcx", 4))
         + x86("cmp", "r14d", "eax")
         + x86("comment", "NULL-BODY GUARD: the body matched without moving the cursor -- recede INTO the body (a CHAIN body's beta is its LAST node's, PAIR(4)) for a longer match")
         + x86("je", ARB_BODYBETA())
         + x86("comment", "COMMIT: push a fresh cell BELOW the body's live frames and snapshot the body's slot window into it")
         + x86("sub", "rsp", ARB_CS())
         + x86("mov", "eax", RDD("rcx", 0))
         + x86("mov", RDD("rsp", 0), "eax")
         + x86("mov", RDD("rsp", 4), "r14d")
         + x86("mov", RDQ("rsp", 8), "r12")
         + x86("mov", RDQ("rsp", 16), "rcx")
         + arbno_win_save(32)
         + x86("mov", AFCQ(0), "rsp")
         + x86_gamma()
         + x86("def", PAIR(3))
         + x86("def", PAIR(5))
         + x86("mov", "rcx", AFCQ(0))
         + x86("mov", "eax", RDD("rcx", 0))
         + x86("mov", "r14d", RDD("rcx", 4))
         + x86("mov", "rdx", RDQ("rcx", 16))
         + x86("mov", AFCQ(0), "rdx")
         + x86("cmp", "r14d", "eax")
         + IF(!(_.op_tail_seal && sn4_arbno_seal_omega()), x86("je", L(3))
              + x86("comment",
                  "RECEDE into the instance below's body beta.  r12 is left where the body's own beta expects it: the body rolls "
                      "back its own pend entries, and a second rollback here emptied a deferred capture (COO-62)")
              + arbno_win_restore("rcx", 32)
              + x86("lea", "rsp", RDQ("rcx", (int)ARB_CS()))
              + x86("jmp", ARB_BODYBETA())
              + x86("def", L(3)))
         + x86("lea", "rsp", RDQ("rcx", (int)ARB_CS()))
         + x86_omega();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_arbno() {
    x86_begin();
    emit_diag_arbno_arm(_.op_arbno_body_kk > 0 ? "FRAMELESS_K" : _.op_off < 0 ? "bomb-slot" : (_.op_sa < 0 || _.op_sb <= 0) ? "bomb-geom" : (_.op_arbno_body_defer_unsafe
        || !_.op_arbno_body_k0) ? (_.op_arbno_frame_off == -1 ? "bomb-defer-unframed" : "ARBNO-FRAME") : "FRAMELESS");
    return _.op_arbno_body_kk > 0
             ? bb_match_arbno_frameless_k()
         : _.op_off < 0
             ? x86_alpha() + x86_bomb("IR_MATCH_ARBNO: slot not granted (zls)")
         : (_.op_sa < 0 || _.op_sb <= 0)
             ? x86_alpha() + x86_bomb("IR_MATCH_ARBNO: COLLECTION geometry not staged (zls_arbno_geom)")
         : (_.op_arbno_body_defer_unsafe || !_.op_arbno_body_k0) && _.op_arbno_frame_off != -1
             ? bb_match_arbno_frame()
         : (_.op_arbno_body_defer_unsafe || !_.op_arbno_body_k0)
             ? x86_alpha() + x86_bomb("IR_MATCH_ARBNO: body contains a DEFER unsafe for the plain-frameless arm, and no ARBNO-FRAME slot was granted")
                            + x86_beta() + x86_bomb("IR_MATCH_ARBNO: unreachable beta (defer-unsafe refuse)")
                            + x86("def", PAIR(2))
                            + x86("def", PAIR(3))
                            + x86("def", PAIR(5))
             : bb_match_arbno_frameless();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
