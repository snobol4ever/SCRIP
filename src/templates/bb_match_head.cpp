#include <string>
#include <stdint.h>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
typedef struct { uint64_t ptr; uint64_t len; } ScanSubjRegs;
ScanSubjRegs rt_match_enter(uint64_t lo, uint64_t hi);
void rt_match_ctx_restore(uint64_t sig, uint64_t len, uint64_t capgen);
void * rt_zls_mark(void);
void   rt_zls_release_to(void *mark);
extern long g_anchor;
}
#include "x86_asm.h"
extern "C" uint64_t g_patstk_sp;
extern "C" uint32_t g_cap_gen;
#define hfc() (x86_port_mode() == ZC_PORT_FORTH && _.op_fc_wbytes > 0)
#define subjc() (_.op_subj_cell)
#define stfh() (_.flat_stmt_frame)
static const char * HKQ(int k) { static char b[8][40]; static int i; i = (i + 1) & 7; snprintf(b[i], 40, "qword ptr [rbp + %d]", -48 + 8 * k); return b[i]; }   /* SUBJ-ARM-2 (Lon s192, the predicted brick wall: "oh I need a RBP stable base pointer" -- STATEMENT housekeeping is the sanctioned RBP class): the five POST-UNWIND-lifetime head fields live in statement-bracket rbp slots, carved sub rsp,48 at alpha (40 used + 8 pad keeps the C-call 16-alignment: bracket base B==8 mod 16, B-56-32 == 0), reclaimed by the bracket leave's mov rsp,rbp for free.  k: 0=deep-rbp(+40 twin) 1=r13 2=r14 3=r15 4=capgen.  Spelled "+ -N" because x86_parse's generic reg-disp arm strtols after " + "; under an UNPINNED armed main the rbp base takes that generic arm (no frame compensation -- rbp is the depth-free statement base, exactly the point). */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_head() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    return ((_.op_sa < 0 && !subjc()) || _.op_off < 0)
         ? x86_bomb("IR_MATCH_HEAD: subject/start slot not promoted (emit_drive)")
         : x86("comment", "IR_MATCH_HEAD")
         + x86_alpha()
         + IF(subjc(), x86("mov", "rdi", "qword ptr [rsp + 0]")
                     + x86("mov", "rsi", "qword ptr [rsp + 8]")
                     + x86_zrelease(16))   /* SUBJECT-CELL rung (a) (Lon: "index operands from RSP... pop into Sigma/delta/Delta"): the registered subject producer chain left its 16B DESCR at TOS; POP IT FIRST -- before ANY flat-spelled access below -- so alpha's remaining depth equals the flat arm's exactly and fc_leaf_walk's D=32+prefix for every downstream pattern box holds untouched.  rdi/rsi survive to the rt_match_enter call: the intervening saves touch only rcx/rax/r13/r14/r15/rbp (the non-RSP-frame rt_zls_mark arm cannot fire here -- promotion is gated ZC_FRAME_RSP). */
         + IF(stfh(), x86_zclaim(48))   /* SUBJ-ARM-2: the head's LOCAL STORAGE, one instruction (Lon: "each BB allocates its LOCAL STORAGE... by one instruction, decrement RSP"); the mark below captures rsp AFTER this carve, so the S10e unwind returns here and the rbp-slot fields survive to RELEASE and the failure exit */
         + x86("mov", stfh() ? HKQ(1) : FRQ(_.op_off + 48), "r13")   /* PATCTX (Lon directive 2026-07-29): save the OUTER Σ/δ/Δ before rt_match_enter sets them anew; both exits restore.  α depth == the FRQ-baked depth (the hfc 32B cell is carved below), so the plain slot spelling is valid in every port/frame arm. */
         + x86("mov", stfh() ? HKQ(2) : FRQ(_.op_off + 56), "r14")
         + x86("mov", stfh() ? HKQ(3) : FRQ(_.op_off + 64), "r15")
         + x86("lea", "rcx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)&g_cap_gen, "g_cap_gen")
         + x86("mov", "eax", RDD("rcx", 0))
         + x86("mov", stfh() ? HKQ(4) : FRQ(_.op_off + 72), "rax")   /* PATCTX-2: the capture generation id is pattern context too -- read BEFORE rt_match_enter draws a fresh id from the well; both exits hand it back via rt_match_ctx_restore arg 3 */
         + IF(_.flat_deep_arrival, x86("mov", stfh() ? HKQ(0) : FRQ(_.op_off + 40), "rbp"))   /* BRACKET-GATE (s193): the +40 save exists to bracket the ARBNO zv() borrow (and any deep repoint); a depth-static graph has no repointer, so save AND both restores gate together on the same predicate the outer quartet reads — drift-proof by shared condition. */
         + IF(x86_zc_frame() != ZC_FRAME_RSP, IF(hfc(), x86_zclaim(32))
             + IF(hfc(), x86("call", "rt_zls_mark", (uint64_t)(uintptr_t)(void *)rt_zls_mark)
                       + x86("mov", FRQ(_.op_off + 8), "rax"))
             + (hfc() ? x86("mov", "rax", "rsp")
                      + x86("add", "rax", (long)32)
                      + x86("mov", FRQ(_.op_off + 16), "rax")
                      : x86_zls2_mark_save(_.op_off + 16)))
         + IF(!subjc(), x86("mov", "rdi", FRQ(_.op_sa))
                      + x86("mov", "rsi", FRQ(_.op_sa + 8)))   /* legacy flat-slot subject read -- the arm the SUBJECT-CELL rung retires; under subjc the DESCR already rides rdi/rsi from the TOS pop above */
         + x86("call", "rt_match_enter", (uint64_t)(uintptr_t)(void *)rt_match_enter)
         + x86("mov", "r13", "rax")
         + x86("mov", "r15", "rdx")
         + x86("mov", "r10", ABSQ(RT_CAS_TOP)) + x86("mov", RDQ("r10", 0), (long)0) + x86("mov", RDQ("r10", 8), "rsp") + x86("lea", "rcx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)&g_patstk_sp, "g_patstk_sp") + x86("mov", "rax", RDQ("rcx", 0)) + x86("mov", RDQ("r10", 16), "rax") + x86("add", "r10", (long)24) + x86("mov", ABSQ(RT_CAS_TOP), "r10")   /* CAS-MARKER-CARRY (s22x): the sentinel's 16 unused bytes now carry the rsp mark (+8, == the α base the hfc claim's slot mark records) and the patstk snapshot (+16) -- the marker readers (tail RELEASE, the fail exit below) recover BOTH depth-free off the marker they already scan to, deleting the second variable-depth reach the original CAS-MARKER note promised.  rax/rcx dead here (Σ/Δ already captured into r13/r15); rsp == α base in the RSP+hfc arm (its claim fires below this push). */   /* CAS-MARKER (Lon s8 directive: "instantiate a bottom marker at each start of a new pattern match"): the pend stack carries its OWN bracket -- a tag-0 sentinel entry (varname pointers are never 0) pushed at match start.  Replaces the flat +32 slot save: no frame-addressed read, no compile-time depth, config-blind.  RELEASE and the fail exit below scan down to this marker instead of reloading a slot -- the first of the two variable-depth reaches deleted on the road to pure FORTH cells. */
         + IF(x86_zc_frame() == ZC_FRAME_RSP, (hfc() ? x86("mov", "rax", "rsp")
                                                + x86_zclaim(32)
                                                + x86("mov", FRQ(_.op_off + 16), "rax")
                                                : x86_zls2_mark_save(_.op_off + 16))
             + x86("lea", "rcx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)&g_patstk_sp, "g_patstk_sp")
             + x86("mov", "rax", RDQ("rcx", 0))
             + x86("mov", FRQ(_.op_off + 8), "rax"))
         + x86("mov", FR(_.op_off), (long)0)
         + x86("def", L(0))
         + x86("mov", "r14d", FR(_.op_off))
         + x86_gamma()
         + x86_beta()
         + x86("add", FR(_.op_off), (long)1)
         + x86("mov", "eax", FR(_.op_off))
         + x86("cmp", "eax", "r15d")
         + x86("jg",  L(1))
         + x86("mov", "rcx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)&g_anchor, "g_anchor")
         + x86("mov", "rax", "[rcx]")
         + x86("cmp64", "rax", (long)0)
         + x86("jne", L(1))
         + x86("jmp", L(0))
         + x86("def", L(1))
         + (hfc() && x86_zc_frame() == ZC_FRAME_RSP
             ? x86("mov", "r10", ABSQ(RT_CAS_TOP))   /* CAS-MARKER-CARRY (s22x) fail exit: scan FIRST, then recover patstk (+16) and the rsp mark (+8) off the marker -- depth-free on EVERY arrival depth, where the old [rsp+8]/[rsp+16] reloads assumed the β-balanced α+claim depth and read leaf cells on any unbalanced path.  The pop (CAS_TOP := marker base) rides the same r10. */
             + x86("def", L(2))
             + x86("sub", "r10", (long)24)
             + x86("mov", "rax", RDQ("r10", 0))
             + x86("test", "rax", "rax")
             + x86("jne", L(2))
             + x86("mov", "rax", RDQ("r10", 16))
             + x86("lea", "rcx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)&g_patstk_sp, "g_patstk_sp")
             + x86("mov", RDQ("rcx", 0), "rax")
             + x86("mov", "rsp", RDQ("r10", 8))
             + x86("mov", ABSQ(RT_CAS_TOP), "r10")
             : IF(x86_zc_frame() == ZC_FRAME_RSP, x86("mov", "rax", FRQ(_.op_off + 8))
                 + x86("lea", "rcx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)&g_patstk_sp, "g_patstk_sp")
                 + x86("mov", RDQ("rcx", 0), "rax"))
             + (x86_zc_frame() == ZC_FRAME_RSP
                 ? x86_zls2_release_to_call(_.op_off + 16)
                 : ( IF(hfc(), x86("mov", "rdi", FRQ(_.op_off + 8)))
                   + x86_align_enter()
                   + IF(!hfc(), x86("mov",  "rdi", FRQ(_.op_off + 8)))
                   + x86("call", "rt_zls_release_to", (uint64_t)(uintptr_t)(void *)rt_zls_release_to)
                   + x86_zls2_release_to_call(_.op_off + 16)
                   + x86_align_leave()))
             + x86("mov", "r10", ABSQ(RT_CAS_TOP)) + x86("def", L(2)) + x86("sub", "r10", (long)24) + x86("mov", "rax", RDQ("r10", 0)) + x86("test", "rax", "rax") + x86("jne", L(2)) + x86("mov", ABSQ(RT_CAS_TOP), "r10"))   /* CAS-MARKER: fail-exit pops pend entries AND the marker by scanning to tag 0 -- depth-free, replaces the flat +32 reload */
         + x86("mov", "r13", stfh() ? HKQ(1) : FRQ(_.op_off + 48))   /* PATCTX restore on the failure exit -- post-unwind rsp is back at α depth in every arm, so the slot spelling holds; under the regime the rbp slots are depth-free by construction */
         + x86("mov", "r14", stfh() ? HKQ(2) : FRQ(_.op_off + 56))
         + x86("mov", "r15", stfh() ? HKQ(3) : FRQ(_.op_off + 64))
         + x86("mov", "rdi", "r13")
         + x86("mov", "rsi", "r15")
         + x86("mov", "rdx", stfh() ? HKQ(4) : FRQ(_.op_off + 72))
         + x86("call", "rt_match_ctx_restore", (uint64_t)(uintptr_t)(void *)rt_match_ctx_restore)   /* re-sync the C-side Σ/Σlen mirror (pattern_match.c / runtime_eval.c readers) */
         + IF(_.flat_deep_arrival, x86("mov", "rbp", stfh() ? HKQ(0) : FRQ(_.op_off + 40)))   /* BRACKET-GATE (s193): restore only if the save above ran */
         + x86_omega();
}
