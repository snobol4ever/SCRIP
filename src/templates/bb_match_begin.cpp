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
extern "C" long *rt_anchor_ptr(void);   /* ZW-4: g_anchor demoted to static in keywords.c; this accessor returns its address for RIP-relative embed */
}
#include "x86_asm.h"
extern "C" uint64_t g_patstk_sp;
extern "C" uint32_t g_cap_gen;
#define hfc() (x86_port_mode() == ZC_PORT_FORTH && _.op_fc_wbytes > 0)
#define subjc() (_.op_subj_cell)
static int hpin(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_HEAD_PIN"); v = (e && *e == '0') ? 0 : 1; } return v; }   /* HEAD-PIN killswitch (s22z): =0 restores the unpinned armed head for A/B */
static int rpin(void) { return _.flat_layout_unknown && hpin() && (subjc() || _.op_zres || !_.flat_jmp_entry); }   /* EXTENT-GATE (this session, Lon ruling 2026-08-06): MATCH_BEGIN sets an RBP frame ONLY for unknown stack-depth statements -- when extent is compile-time determinable (flat_layout_unknown=0) STATEMENT_END whacks by ADD and no frame is needed.  Replaced flat_deep_arrival (construct-presence) with flat_layout_unknown (extent-indeterminability): flat_layout_unknown=1 iff zls_g_region(g)<=0, i.e. the ZD planner could not produce a static region size.  Statically-extented ARBNO statements (K16 arm, K0-in-place arm) now emit zero frame overhead and release by static ADD.  jmp-entry guard retained: chains arriving at runtime-variable depth keep the pre-session regime until CARRIED-OPEN retires the compile-time spelling.  Degrade never die. */
#define stfh() (_.flat_stmt_frame)
static int hoff(int off) { long lv; return (_.op_uhead >= 0 && zvo_resolve(off, _.op_udout, _.op_uhead, &lv) && lv >= 0) ? (int)lv : off; }   /* rsp-relative WRITE spelling: reader-depth resolution, unchanged */
static int hoffb(int off) { long lv; return (_.op_uhead >= 0 && zvo_resolve_base(off, _.op_uhead, &lv) && lv >= 0) ? (int)lv : off; }   /* ZD-5: BASE (pin-world) spelling for op_stmt_pin publish + post-pin readers -- the pinned rbp is the claim base, so the slot is base-relative regardless of reader rsp depth */
static int hdel(void) { long od = (_.op_uhead >= 0) ? zvo_owner_dout(_.op_uhead) : 0; long d = _.op_udout - od; return (d > 0 && d < 4096) ? (int)d : 0; }   /* ZD-5: alpha rsp sits hdel() bytes BELOW the claim base on hybrid-armed runs (the non-popped producer cells); 0 on declined runs by the dout==Kc identity */   /* ⭐ PIN-REBASE (s23a): the pin slot's CLAIM-relative offset -- the raw flat op_off+40 was baked against the deleted whole-graph base and overshoots the statement claim (066: 312 against a 176B claim, [rsp+312] at pin time == caller/envp territory); at the pin rsp==claim base, so the slot is (off - umin) via the SAME zvo owner table every FRQ reader resolves through -- one authority, and the X86H_JMP cut restores read the staged value so writer+readers move together.  Miss (no claim staged) keeps the raw spelling, byte-identical for the unclaimed population. */
static const char * HKQ(int k) { static char b[8][40]; static int i; i = (i + 1) & 7; snprintf(b[i], 40, "qword ptr [rbp + %d]", -48 + 8 * k); return b[i]; }   /* SUBJ-ARM-2 (Lon s192, the predicted brick wall: "oh I need a RBP stable base pointer" -- STATEMENT housekeeping is the sanctioned RBP class): the five POST-UNWIND-lifetime head fields live in statement-bracket rbp slots, carved sub rsp,48 at alpha (40 used + 8 pad keeps the C-call 16-alignment: bracket base B==8 mod 16, B-56-32 == 0), reclaimed by the bracket leave's mov rsp,rbp for free.  k: 0=deep-rbp(+40 twin) 1=r13 2=r14 3=r15 4=capgen.  Spelled "+ -N" because x86_parse's generic reg-disp arm strtols after " + "; under an UNPINNED armed main the rbp base takes that generic arm (no frame compensation -- rbp is the depth-free statement base, exactly the point). */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_begin() {
    x86_begin();
    if (rpin() && _.op_off >= 0 && !_.op_zw) g_emit.op_stmt_pin = hoffb(_.op_off + 40);   /* ZW-12: under the canonical frame there is no pin to publish -- old rbp rides the STACK (push rbp) and every match exit whacks it back; the terminal-cut restore is the OLD regime's mechanism and must not fire against a frame that no longer stores rbp in the claim */   /* REPL-PIN (this session): the (subjc||zres) conjunct excluded the legacy-subject population -- replacement statements, whose second subject consumer denies the fence grant -- leaving their rbp-flavored FRQ slots UNPINNED against the ambient C frame (gdb: rt_match_enter(lo=argv-territory, hi=0), the silent-empty class on string_pattern/mixed_workload/roman).  The pin is the head's law-4 base for EVERY deep-arrival statement, not just the granted/armed ones. */   /* ZD-5: armed heads pin too (their claim rides the same coordinate law); publish in BASE coordinates -- the terminal-cut reader dereferences [rbp + op_stmt_pin] against the pinned base */   /* ⭐ HEAD-PIN carry (s22z): publish the pin slot's region offset for every terminal cut of THIS statement (x86_asm.h X86H_JMP arms) -- emit-time staging at template top, the bb_slot_claim precedent.  Same predicate as the pin emission below; cleared at the choke when the next statement's uclaim stages. */
    if (!PLATFORM_X86) return std::string();
    return ((_.op_sa < 0 && !subjc() && !_.op_zres) || _.op_off < 0)
         ? x86_bomb("IR_MATCH_BEGIN: subject/start slot not promoted (emit_drive)")
         : _.op_zw2
         ? x86("comment", "IR_MATCH_BEGIN (MECHANISM-2 W-1: push rbp; mov rbp,rsp; sub rsp,Kc_zwr. Header at [rbp-16]..[rbp-64]; blob cells via FR [rsp+8+off]; Kc_zwr=Kc_cells+72, Kc_zwr≡8(mod16) → C calls 0-mod-16. whack=mov rsp,rbp;pop rbp)")
         + x86_alpha()
         + x86("note", "mech2_boundary") + x86("push", "rbp")   /* push: old_rbp at [α-8]; rsp=α-8 */
         + x86("mov", "rbp", "rsp")   /* rbp=α-8; header slots [rbp-16]..[rbp-72] safe below; subj at [rbp+8]=[α],[rbp+16]=[α+8] */
         + x86("note", "mech2_framebase") + x86("mov", "r12", "rsp")   /* ⭐ W-1b ARBNO-RBP FIX: save α-8 into r12 (= rsp after push, before sub). R12 is free (R12-ERAD/ARCH-ICON), callee-saved by C ABI. Used for whack (mov rsp,r12; pop rbp) and rbp-restore at fail-exit/match-end after ARBNO clobbers rbp. Also serves as mech2_framebase in bb_match_end. */
         + IF(_.op_udout > 0, x86("note", "mech2_blob_carve") + x86("sub", "rsp", (long)_.op_udout))   /* ⭐ W-1: Kc_zwr=Kc_cells+72; sub rsp,Kc_zwr; rsp=α-8-Kc_zwr≡0(mod16) for C calls. Header [rbp-16]..[rbp-64] is in carved region. Blob FR: [rsp+8+off]=[α-Kc_zwr+off] also in carved region. Gate op_udout>0; zero-Kc heads byte-identical. */
         + IF(_.op_zres, x86("note", ZOPN(0)) + x86("mov", "rdi", RDQ("rsp", (long)_.op_udout + 8 + _.op_zread[0] + 0))   /* zres: [rsp+Kc_zwr+8+zread]=[α+zread]; bypass FRQ (HEAD zdepth=0) */
                       + x86("note", ZOPN(0)) + x86("mov", "rsi", RDQ("rsp", (long)_.op_udout + 8 + _.op_zread[0] + 8))
                       + IF(_.op_sa >= 0 && !subjc(), x86("mov", RDQ("rsp", 8 + _.op_sa), "rdi")   /* mirror into blob cell [rsp+8+sa] */
                                        + x86("mov", RDQ("rsp", 8 + _.op_sa + 8), "rsi")))
         + IF(!_.op_zres && subjc(), x86("mov", "rdi", "qword ptr [rbp + 8]")   /* subjc TOS: [rbp+8]=[α] = var ptr */
                     + x86("mov", "rsi", "qword ptr [rbp + 16]"))              /* [rbp+16]=[α+8] = var len */
         + IF(!_.op_zres && !subjc(), x86("mov", "rdi", RDQ("rsp", (long)_.op_udout + 8 + _.op_sa))   /* !subjc: [rsp+Kc_zwr+8+sa]=[α+sa]; bypass FRQ (HEAD zdepth=0) */
                     + x86("mov", "rsi", RDQ("rsp", (long)_.op_udout + 8 + _.op_sa + 8)))
         + x86("note", HKN(1)) + x86("mov", RDQ("rbp", -16), "r13")   /* ⭐ W-1 FIXED HEADER (negative rbp): [rbp-16]=[α-24]=outer_Σ — same address scheme as ZW-12, safe in carved region, matches match_end restores */
         + x86("note", HKN(2)) + x86("mov", RDQ("rbp", -24), "r14")   /* [rbp-24]=outer_δ */
         + x86("note", HKN(3)) + x86("mov", RDQ("rbp", -32), "r15")   /* [rbp-32]=outer_Δ */
         + x86("lea", "rcx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)&g_cap_gen, "g_cap_gen")
         + x86("mov", "eax", RDD("rcx", 0))
         + x86("note", HKN(4)) + x86("mov", RDQ("rbp", -40), "rax")   /* [rbp-40]=cap_gen */
         + x86("note", "cas_top") + x86("mov", "r10", ABSQ(RT_CAS_TOP)) + x86("mov", RDQ("r10", 0), (long)0) + x86("note", "cas_rsp_mark") + x86("mov", RDQ("r10", 8), "rsp") + x86("lea", "rcx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)&g_patstk_sp, "g_patstk_sp") + x86("mov", "rax", RDQ("rcx", 0)) + x86("note", "cas_patstk") + x86("mov", RDQ("r10", 16), "rax") + x86("add", "r10", (long)24) + x86("note", "cas_top") + x86("mov", ABSQ(RT_CAS_TOP), "r10") + x86("note", "cas_sentinel") + x86("mov", RDQ("rbp", -72), "r10")   /* CAS marker: after sub rsp; cas_rsp_mark=post-carve floor; captures above this mark. [rbp-72]=post-push CAS_TOP (=sentinel_base+24): saved AFTER the push so restore on retry keeps sentinel tag=0 intact and new COND entries go above it, not over it. The scan in match_end finds sentinel by reading backward from new top to the tag=0 entry at sentinel_base. */
         + x86("call", "rt_match_enter", (uint64_t)(uintptr_t)(void *)rt_match_enter)   /* C call: rsp≡0(mod16) ✓ */
         + x86("mov", "r13", "rax")
         + x86("mov", "r15", "rdx")
         + IF(x86_zc_frame() == ZC_FRAME_RSP, (hfc() ? x86("mov", "rax", "rsp")
                                                + x86_zclaim(32)
                                                + x86("note", "rsp_mark") + x86("mov", RDQ("rbp", -64), "rax")   /* [rbp-64]=zls2_mark */
                                                : x86("note", HKN(5)) + x86("mov", RDQ("rbp", -64), "rsp"))   /* zls2_mark: rsp directly, bypass FRQ */
             + x86("lea", "rcx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)&g_patstk_sp, "g_patstk_sp")
             + x86("mov", "rax", RDQ("rcx", 0))
             + x86("note", "patstk_mark") + x86("mov", RDQ("rbp", -56), "rax"))   /* [rbp-56]=patstk_mark */
         + x86("note", "start_δ") + x86("mov", RDD("rbp", -48), (long)0)   /* [rbp-48]=start_δ (dword) */
         + x86("def", L(0))
         + x86("note", "start_δ") + x86("mov", "r14d", RDD("rbp", -48))   /* retry: reload start_δ */
         + x86_gamma()
         + x86_beta()
         + x86("note", "start_δ") + x86("add", RDD("rbp", -48), (long)1)
         + x86("note", "start_δ") + x86("mov", "eax", RDD("rbp", -48))
         + x86("cmp", "eax", "r15d")
         + x86("jg",  L(1))
         + x86("mov", "rcx", "[rip@got + __]", (uint64_t)(uintptr_t)(const void *)rt_anchor_ptr(), "rt_anchor_g")
         + x86("mov", "rax", "[rcx]")
         + x86("cmp64", "rax", (long)0)
         + x86("jne", L(1))
         + x86("note", "cas_sentinel") + x86("mov", "r10", RDQ("rbp", -72)) + x86("mov", ABSQ(RT_CAS_TOP), "r10")   /* Bug4 fix: discard stale COND entries from failed attempt; restore CAS_TOP to sentinel base (ZW-12 analogue: r12←[rbp-40];RT_CAS_TOP←r12) */
         + IF(x86_zc_frame() == ZC_FRAME_RSP, x86("note", "patstk_mark") + x86("lea", "rcx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)&g_patstk_sp, "g_patstk_sp") + x86("mov", "rax", RDQ("rbp", -56)) + x86("mov", RDQ("rcx", 0), "rax")   /* Bug4 fix: restore g_patstk_sp to α snapshot; discards any captures pushed during failed attempt */
               + x86("note", "rsp_mark") + x86("mov", "rsp", RDQ("rbp", -64)))   /* Bug4 fix: restore rsp to α floor; collapses any zls2/SAVE growth from failed attempt (safe: blob cells below floor, Forth spine above rbp-8 untouched) */
         + x86("jmp", L(0))
         + x86("def", L(1))
         + (hfc() && x86_zc_frame() == ZC_FRAME_RSP
             ? x86("mov", "r10", ABSQ(RT_CAS_TOP))
             + x86("def", L(2))
             + x86("sub", "r10", (long)24)
             + x86("mov", "rax", RDQ("r10", 0))
             + x86("test", "rax", "rax")
             + x86("jne", L(2))
             + x86("note", "cas_patstk") + x86("mov", "rax", RDQ("r10", 16))
             + x86("lea", "rcx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)&g_patstk_sp, "g_patstk_sp")
             + x86("mov", RDQ("rcx", 0), "rax")
             + x86("note", "cas_rsp_mark") + x86("mov", "rsp", RDQ("r10", 8))
             + x86("mov", ABSQ(RT_CAS_TOP), "r10")
             : IF(x86_zc_frame() == ZC_FRAME_RSP, x86("note", "patstk_mark") + x86("mov", "rax", RDQ("rbp", -56))
                 + x86("lea", "rcx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)&g_patstk_sp, "g_patstk_sp")
                 + x86("mov", RDQ("rcx", 0), "rax"))
             + (x86_zc_frame() == ZC_FRAME_RSP
                 ? (x86_align_leave() + x86("mov", "rsp", RDQ("rbp", -64)) + x86_align_enter())   /* zls2_release: [rbp-64]=mark */
                 : ( IF(hfc(), x86("note", "patstk_mark") + x86("mov", "rdi", RDQ("rbp", -56)))
                   + x86_align_enter()
                   + IF(!hfc(), x86("note", "patstk_mark") + x86("mov",  "rdi", RDQ("rbp", -56)))
                   + x86("call", "rt_zls_release_to", (uint64_t)(uintptr_t)(void *)rt_zls_release_to)
                   + x86_align_enter() + x86("mov", "rsp", RDQ("rbp", -64)) + x86_align_leave()   /* zls2 restore */
                   + x86_align_leave()))
             + x86("mov", "r10", ABSQ(RT_CAS_TOP)) + x86("def", L(2)) + x86("sub", "r10", (long)24) + x86("mov", "rax", RDQ("r10", 0)) + x86("test", "rax", "rax") + x86("jne", L(2)) + x86("mov", ABSQ(RT_CAS_TOP), "r10"))
         + x86("note", "mech2_framebase") + x86("mov", "rbp", "r12")   /* ⭐ W-1b ARBNO-RBP FIX: restore rbp←r12 (=α-8) before header reads; ARBNO may have set rbp to element-view making [rbp-N] reads return garbage without this restore */
         + x86("note", HKN(1)) + x86("mov", "r13", RDQ("rbp", -16))   /* restore from fixed header */
         + x86("note", HKN(2)) + x86("mov", "r14", RDQ("rbp", -24))
         + x86("note", HKN(3)) + x86("mov", "r15", RDQ("rbp", -32))
         + x86("mov", "rdi", "r13")
         + x86("mov", "rsi", "r15")
         + x86("note", HKN(4)) + x86("mov", "rdx", RDQ("rbp", -40))
         + x86("call", "rt_match_ctx_restore", (uint64_t)(uintptr_t)(void *)rt_match_ctx_restore)
         + x86("note", "mech2_whack") + x86("mov", "rsp", "r12")   /* ⭐ W-1b: use r12 (=α-8) directly; rbp already restored above but using r12 is explicit and survives any further clobber between ctx_restore and here */
         + x86("pop", "rbp")   /* pops [α-8] = old_rbp; rsp → α */
         + x86_omega()
         : _.op_zw
         ? x86("comment", "IR_MATCH_BEGIN (ZW-12 canonical frame)")
         + x86_alpha()
         + IF(_.op_zres, x86("note", ZOPN(0)) + x86("mov", "rdi", x86_zref((int)_.op_uclaim + _.op_zread[0] + 0, 1))
                       + x86("note", ZOPN(0)) + x86("mov", "rsi", x86_zref((int)_.op_uclaim + _.op_zread[0] + 8, 1))
                       + IF(_.op_sa >= 0 && !subjc(), x86("mov", FRQ(_.op_sa), "rdi")
                                        + x86("mov", FRQ(_.op_sa + 8), "rsi")))
         + IF(!_.op_zres && subjc(), x86("mov", "rdi", "qword ptr [rsp + 8]")   /* ⭐ MECH2: +8 to compensate the push rbp -- subject TOS was at [rsp+0] before push, now at [rsp+8] */
                     + x86("mov", "rsi", "qword ptr [rsp + 16]"))
         + IF(!_.op_zres && !subjc(), x86("mov", "rdi", FRQ(_.op_sa))
                     + x86("mov", "rsi", FRQ(_.op_sa + 8)))
         + x86("note", "match_frame") + x86("push", "rbp")
         + x86("mov", "rbp", "rsp")
         + x86("lea", "rbp", "qword ptr [rbp# + 8]")   /* ⭐ ZW-15 (OMEGA s36): rbp=claim_base.  push rbp placed old_rbp at [original_rsp-8]; lea rbp,[rbp+8] sets rbp=original_rsp=claim_base so FRQ(blob_off)=[rbp+blob_off]=[claim_base+blob_off] -- the s23i blob shift-fragile hazard is retired.  old_rbp at [rbp-8]; frame cells [rbp-16]..[rbp-64]; ZW_FRAME_K=64; whack: lea rsp,[rbp-8]; pop rbp.  rbp# escape: this is a raw-register lea, not a frame offset, must parse XK_REGDISP not XK_FR64 (the ZB-FC-1 cmp hazard, line 1245/1246 note). */
         + x86("sub", "rsp", (long)ZW_FRAME_K)
         + x86("note", HKN(1)) + x86("mov", RDQ("rbp", -16), "r13")
         + x86("note", HKN(2)) + x86("mov", RDQ("rbp", -24), "r14")
         + x86("note", HKN(3)) + x86("mov", RDQ("rbp", -32), "r15")
         + x86("note", "cas_base") + x86("mov", RDQ("rbp", -40), "r12")   /* ⭐ ZW-3 O-5s2: r12 is the live CAS top from the graph-entry seed.  Save r12 to [rbp-40]=cas_base (ZW-15: was -32). */
         + x86("lea", "rcx", "[rip@got + __]", (uint64_t)(uintptr_t)(const void *)rt_anchor_ptr(), "rt_anchor_g")
         + x86("note", "anchor_snapshot") + x86("mov", "rax", RDQ("rcx", 0)) + x86("mov", RDQ("rbp", -48), "rax")
         + x86("lea", "rcx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)&g_cap_gen, "g_cap_gen")
         + x86("note", HKN(4)) + x86("mov", "eax", RDD("rcx", 0)) + x86("mov", RDQ("rbp", -64), "rax")
         + x86("call", "rt_match_enter", (uint64_t)(uintptr_t)(void *)rt_match_enter)
         + x86("mov", "r13", "rax")
         + x86("mov", "r15", "rdx")
         + x86("note", "start_δ") + x86("mov", RDQ("rbp", -56), (long)0)
         + x86("def", L(0))
         + x86("note", "start_δ") + x86("mov", "r14d", RDD("rbp", -56))
         + x86_gamma()
         + x86_beta()
         + x86("note", "cas_base") + x86("mov", "r12", RDQ("rbp", -40))   /* ZW-3 retry: r12←base discards any COND records from the failed attempt (ZW-15: was -32). */
         + x86("note", "start_δ") + x86("mov", "eax", RDD("rbp", -56)) + x86("add", "eax", (long)1)
         + x86("cmp", "eax", "r15d")
         + x86("jg",  L(1))
         + x86("note", "anchor_snapshot") + x86("mov", "rcx", RDQ("rbp", -48)) + x86("test", "rcx", "rcx")
         + x86("jne", L(1))
         + x86("note", "start_δ") + x86("mov", RDQ("rbp", -56), "rax")   /* the 32-bit add already zero-extended rax, so the qword store carries a clean cursor */
         + x86("jmp", L(0))
         + x86("def", L(1))
         + x86("note", "cas_base") + x86("mov", "r12", RDQ("rbp", -40)) + x86("mov", ABSQ(RT_CAS_TOP), "r12")   /* ZW-3 ω bulk discard (ZW-15: was -32) */
         + x86("note", HKN(1)) + x86("mov", "r13", RDQ("rbp", -16))
         + x86("note", HKN(2)) + x86("mov", "r14", RDQ("rbp", -24))
         + x86("note", HKN(3)) + x86("mov", "r15", RDQ("rbp", -32))
         + x86("mov", "rdi", "r13")
         + x86("mov", "rsi", "r15")
         + x86("note", HKN(4)) + x86("mov", "rdx", RDQ("rbp", -64))
         + x86("call", "rt_match_ctx_restore", (uint64_t)(uintptr_t)(void *)rt_match_ctx_restore)
         + x86("note", "whack") + x86("lea", "rsp", "qword ptr [rbp# + -8]")   /* ZW-15: old_rbp at [rbp-8]; lea rsp,[rbp-8] sets rsp=claim_base-8; pop rbp restores old_rbp, rsp=claim_base */
         + x86("pop", "rbp")
         + x86_omega()
         : x86("comment", "IR_MATCH_BEGIN")
         + x86_alpha()
         + IF(_.op_zres, x86("note", ZOPN(0)) + x86("mov", "rdi", x86_zref((int)_.op_uclaim + _.op_zread[0] + 0, 1))
                       + x86("note", ZOPN(0)) + x86("mov", "rsi", x86_zref((int)_.op_uclaim + _.op_zread[0] + 8, 1))
                       + IF(_.op_sa >= 0 && !subjc(), x86("mov", FRQ(_.op_sa), "rdi")
                                        + x86("mov", FRQ(_.op_sa + 8), "rsi")))   /* ⭐ CLAIM-AT-HEAD v2 (this session; supersedes the intermediate HEAD-FOLD): the claim is carved at THIS box's alpha by the port hook -- the match owns its scanner storage -- so pre-head producer cells ride ABOVE the claim and the head's own rsp IS the claim base with zero instruction cost: no pop, no fold, PURE NON-POPPING (the directive verbatim), and the blob's raw claim spellings (rspd/x86_zref escapes, shift-fragile by the ON-5/ARGREAD design -- gdb-bracketed in 156 as a wild rt_cap_push slot pointer) hold at OFF-world distances by construction.  The subject read is op_uclaim + op_zread[0]: the claim the hook carved below, plus the staged delta to the subject's cell among the above-claim producers -- zread is 0 when the subject producer is adjacent (strip, roman) but POS(N)/LEN(n) variable operands put coerce chains on the spine between subject and head (MEASURED: 061_capture_in_arbno read the coerce cell as the subject descr under a hardcoded +0 and failed every match).  The MIRROR restores the dataflow the !subjc OFF world fed via the producer's flat stores (which the armed producer no longer emits) -- REPLACE and any op_sa reader see the same bytes as ever.  GATED !subjc(): on subject-cell-granted graphs the OFF world popped and never wrote op_sa -- the slot belongs to another node's layout there, and writing it clobbers live state (MEASURED: 054_pat_arbno_alt, the mirror's two stores were the ONLY functional diff and flipped it red). */
         + IF(!_.op_zres && subjc(), x86("mov", "rdi", "qword ptr [rsp + 0]")
                     + x86("mov", "rsi", "qword ptr [rsp + 8]")
                     + x86_zrelease(16))   /* SUBJECT-CELL rung (a) (Lon: "index operands from RSP... pop into Sigma/delta/Delta"): the registered subject producer chain left its 16B DESCR at TOS; POP IT FIRST -- before ANY flat-spelled access below -- so alpha's remaining depth equals the flat arm's exactly and fc_leaf_walk's D=32+prefix for every downstream pattern box holds untouched.  rdi/rsi survive to the rt_match_enter call: the intervening saves touch only rcx/rax/r13/r14/r15/rbp (the non-RSP-frame rt_zls_mark arm cannot fire here -- promotion is gated ZC_FRAME_RSP). */
         + IF(rpin(),
               x86("note", HKN(0)) + x86("mov", RDQ("rsp", hoff(_.op_off + 40)), "rbp")
             + x86("note", "stmt_base") + (hdel() > 0 ? x86("lea", "rbp", RDQ("rsp", hdel())) : x86("mov", "rbp", "rsp")))   /* ZD-5 MATCH-SPINE: the pin lands ON the claim base -- alpha rsp is hdel() below it when armed producer cells ride above the claim (non-popping law), 0 when declined, so declined emission stays byte-identical (mov). */   /* ⭐ HEAD-PIN (s22z, killswitch SCRIP_HEAD_PIN=0): the match CONSTRUCT establishes its own law-4 base -- rbp := statement region base, the address every FRQ slot in the head/fence/release/exhaust extent was baked against.  CARVE-KILL deleted the whole-graph carve whose base the one-time main pin used to BE, so at HEAD every [rbp+off] statement slot resolved against main's (m4) or rt_chain_enter's (m3) frame: dead CRT territory in m4 (survived by luck), LIVE C frame in m3 -- measured 061_pat_fence_fn_seal, armed: output-correct then rc=124, gdb-sampled as a full statement re-execution loop (NV_SET + slab PCs) because the head's [rbp+88..120] housekeeping corrupted the chain's return path.  THE LICENSE is the STF-UNFLIP ruling's own words: no per-statement bracket EVER, but law 4's rbp serves "CONSTRUCT housekeeping that must survive an unwind" -- and s192 already named STATEMENT housekeeping as the sanctioned RBP class.  This is that citizen, construct-scoped: pin here, restores at the construct's own two exits (the +40 restores below and in bb_match_end, BRACKET-GATE paired since s193, correct with ZERO edits once the save writes the OLD base).  PLACEMENT: after the subject pop rsp == region base by the PATCTX invariant ("alpha depth == the FRQ-baked depth"), so the rsp-relative spelling writes the true region slot BEFORE rbp moves; before every FRQ save below so r13/r14/r15/capgen land in the region, not the ambient frame.  op_zgpop stays SOLE release authority -- this pin moves no rsp, frees nothing, and dies at the construct exits. */
         + IF(stfh(), x86_zclaim(48))   /* SUBJ-ARM-2: the head's LOCAL STORAGE, one instruction (Lon: "each BB allocates its LOCAL STORAGE... by one instruction, decrement RSP"); the mark below captures rsp AFTER this carve, so the S10e unwind returns here and the rbp-slot fields survive to RELEASE and the failure exit */
         + x86("note", HKN(1)) + x86("mov", stfh() ? HKQ(1) : FRQ(_.op_off + 48), "r13")   /* PATCTX (Lon directive 2026-07-29): save the OUTER Σ/δ/Δ before rt_match_enter sets them anew; both exits restore.  α depth == the FRQ-baked depth (the hfc 32B cell is carved below), so the plain slot spelling is valid in every port/frame arm. */
         + x86("note", HKN(2)) + x86("mov", stfh() ? HKQ(2) : FRQ(_.op_off + 56), "r14")
         + x86("note", HKN(3)) + x86("mov", stfh() ? HKQ(3) : FRQ(_.op_off + 64), "r15")
         + x86("lea", "rcx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)&g_cap_gen, "g_cap_gen")
         + x86("mov", "eax", RDD("rcx", 0))
         + x86("note", HKN(4)) + x86("mov", stfh() ? HKQ(4) : FRQ(_.op_off + 72), "rax")   /* PATCTX-2: the capture generation id is pattern context too -- read BEFORE rt_match_enter draws a fresh id from the well; both exits hand it back via rt_match_ctx_restore arg 3 */
         + IF(_.flat_deep_arrival && !rpin(), x86("note", HKN(0)) + x86("mov", stfh() ? HKQ(0) : FRQ(_.op_off + 40), "rbp"))   /* REPL-PIN: complementary to the widened pin on the SAME hpin() -- the old !(subjc&&hpin) guard let this raw flat save fire BESIDE the pin for the zres population, a stray write the pin's rsp-relative old_rbp slot made redundant; exclusive-by-predicate now, per the pin comment's own "exclusive by construction" law. */   /* BRACKET-GATE (s193): the +40 save exists to bracket the ARBNO zv() borrow (and any deep repoint); a depth-static graph has no repointer, so save AND both restores gate together on the same predicate the outer quartet reads — drift-proof by shared condition.  HEAD-PIN (s22z): under the pin the slot already holds the OLD rbp (written rsp-relatively before the pin above); this FRQ save would overwrite it with the NEW base and turn both exit restores into self-no-ops — the exact disease the pin cures — so the arms are exclusive by construction. */
         /* ZW-0 stage 2: island arm deleted -- unreachable under ZC_FRAME_RSP default */
         + IF(!_.op_zres && !subjc(), rpin()
                      ? x86("mov", "rdi", RDQ("rsp", hoff(_.op_sa)))
                      + x86("mov", "rsi", RDQ("rsp", hoff(_.op_sa + 8)))
                      : x86("mov", "rdi", FRQ(_.op_sa))
                      + x86("mov", "rsi", FRQ(_.op_sa + 8)))   /* legacy flat-slot subject read -- the arm the SUBJECT-CELL rung retires; under subjc the DESCR already rides rdi/rsi from the TOS pop above.  REPL-PIN (this session): under the widened pin the raw flat op_sa is the deleted-carve coordinate (256 against a 176B claim on tB, exactly the hoffb comment's 066 shape) while the subject producer WROTE through zvo to the claim slot -- so the pinned read resolves through the SAME owner table, base-relative off the pinned rbp; the unpinned arm keeps the legacy spelling byte-identical. */
         + x86("call", "rt_match_enter", (uint64_t)(uintptr_t)(void *)rt_match_enter)
         + x86("mov", "r13", "rax")
         + x86("mov", "r15", "rdx")
         + x86("note", "cas_top") + x86("mov", RDQ("r12", 0), (long)0) + x86("note", "cas_rsp_mark") + x86("mov", RDQ("r12", 8), "rsp") + x86("lea", "rcx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)&g_patstk_sp, "g_patstk_sp") + x86("mov", "rax", RDQ("rcx", 0)) + x86("note", "cas_patstk") + x86("mov", RDQ("r12", 16), "rax") + x86("note", "cas_top") + x86("add", "r12", (long)24)   /* CAS-R12-UNIFY (Lon mandate, this session): sentinel written AT r12 directly -- the r10 stage and the cell store are deleted; [RT_CAS_TOP] is boot-seed only. */   /* CAS-MARKER-CARRY (s22x): the sentinel's 16 unused bytes now carry the rsp mark (+8, == the α base the hfc claim's slot mark records) and the patstk snapshot (+16) -- the marker readers (tail RELEASE, the fail exit below) recover BOTH depth-free off the marker they already scan to, deleting the second variable-depth reach the original CAS-MARKER note promised.  rax/rcx dead here (Σ/Δ already captured into r13/r15); rsp == α base in the RSP+hfc arm (its claim fires below this push). */   /* CAS-MARKER (Lon s8 directive: "instantiate a bottom marker at each start of a new pattern match"): the pend stack carries its OWN bracket -- a tag-0 sentinel entry (varname pointers are never 0) pushed at match start.  Replaces the flat +32 slot save: no frame-addressed read, no compile-time depth, config-blind.  RELEASE and the fail exit below scan down to this marker instead of reloading a slot -- the first of the two variable-depth reaches deleted on the road to pure FORTH cells. */
         + IF(x86_zc_frame() == ZC_FRAME_RSP, (hfc() ? x86("mov", "rax", "rsp")
                                                + x86_zclaim(32)
                                                + x86("note", "rsp_mark") + x86("mov", FRQ(_.op_off + 16), "rax")
                                                : x86_zls2_mark_save(_.op_off + 16))
             + x86("lea", "rcx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)&g_patstk_sp, "g_patstk_sp")
             + x86("mov", "rax", RDQ("rcx", 0))
             + x86("note", "patstk_mark") + x86("mov", FRQ(_.op_off + 8), "rax"))
         + x86("note", "start_δ") + x86("mov", FR(_.op_off), (long)0)   /* ⭐ OBJ-NOTE ON-3 (s23e): this dword IS the unanchored-match retry cursor -- the position the NEXT attempt starts from, not the live δ.  SPITBOL manual pp.66-68 is the contract being embodied: an unanchored match that fails does not give up, it re-attempts one character further right ("waiting for the cursor to come to them"), which is why the loop below bumps this slot, compares it against Δ (r15d, subject length) to know when the subject is exhausted, and consults &ANCHOR -- a nonzero &ANCHOR forbids the retry entirely, so the pattern must match at position 0.  Named start_δ rather than a bare "counter" because it is a CURSOR VALUE in δ's units and lands in r14d itself; outer_δ (HKN(2)) is the different thing one slot family over -- the ENCLOSING match's cursor, saved to be given back. */
         + x86("def", L(0))
         + x86("note", "start_δ") + x86("mov", "r14d", FR(_.op_off))
         + x86_gamma()
         + x86_beta()
         + x86("note", "start_δ") + x86("add", FR(_.op_off), (long)1)
         + x86("note", "start_δ") + x86("mov", "eax", FR(_.op_off))
         + x86("cmp", "eax", "r15d")
         + x86("jg",  L(1))
         + x86("mov", "rcx", "[rip@got + __]", (uint64_t)(uintptr_t)(const void *)rt_anchor_ptr(), "rt_anchor_g")
         + x86("mov", "rax", "[rcx]")
         + x86("cmp64", "rax", (long)0)
         + x86("jne", L(1))
         + x86("jmp", L(0))
         + x86("def", L(1))
         + (hfc() && x86_zc_frame() == ZC_FRAME_RSP
             ? x86("def", L(2))   /* CAS-R12-UNIFY fail exit: scan ON r12 itself -- r12 is the one authority so walking it down IS the pop; lands at the marker base, then recover patstk (+16) and the rsp mark (+8) off it, depth-free on EVERY arrival depth exactly as the s22x carry designed.  The r10 stage and both cell touches are deleted. */
             + x86("sub", "r12", (long)24)
             + x86("mov", "rax", RDQ("r12", 0))
             + x86("test", "rax", "rax")
             + x86("jne", L(2))
             + x86("note", "cas_patstk") + x86("mov", "rax", RDQ("r12", 16))
             + x86("lea", "rcx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)&g_patstk_sp, "g_patstk_sp")
             + x86("mov", RDQ("rcx", 0), "rax")
             + x86("note", "cas_rsp_mark") + x86("mov", "rsp", RDQ("r12", 8))
             : IF(x86_zc_frame() == ZC_FRAME_RSP, x86("note", "patstk_mark") + x86("mov", "rax", FRQ(_.op_off + 8))
                 + x86("lea", "rcx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)&g_patstk_sp, "g_patstk_sp")
                 + x86("mov", RDQ("rcx", 0), "rax"))
             + (x86_zc_frame() == ZC_FRAME_RSP
                 ? x86_zls2_release_to_call(_.op_off + 16)
                 : ( IF(hfc(), x86("note", "patstk_mark") + x86("mov", "rdi", FRQ(_.op_off + 8)))
                   + x86_align_enter()
                   + IF(!hfc(), x86("note", "patstk_mark") + x86("mov",  "rdi", FRQ(_.op_off + 8)))
                   + x86("call", "rt_zls_release_to", (uint64_t)(uintptr_t)(void *)rt_zls_release_to)
                   + x86_zls2_release_to_call(_.op_off + 16)
                   + x86_align_leave()))
             + x86("def", L(2)) + x86("sub", "r12", (long)24) + x86("mov", "rax", RDQ("r12", 0)) + x86("test", "rax", "rax") + x86("jne", L(2)))   /* CAS-R12-UNIFY: fail-exit pops pend entries AND the marker by scanning r12 down to tag 0 -- the walk IS the pop, cell touches deleted */
         + x86("note", HKN(1)) + x86("mov", "r13", stfh() ? HKQ(1) : FRQ(_.op_off + 48))   /* PATCTX restore on the failure exit -- post-unwind rsp is back at α depth in every arm, so the slot spelling holds; under the regime the rbp slots are depth-free by construction.  ⭐ OBJ-NOTE ON-3 (s23e): the RESTORE side now names the same five slots the α saves named, via the shared HKN(k) table -- s23c annotated the writes and left every read bare, so a .s reader saw '[rbp+96]' with no way to know it was the enclosing match's subject coming back. */
         + x86("note", HKN(2)) + x86("mov", "r14", stfh() ? HKQ(2) : FRQ(_.op_off + 56))
         + x86("note", HKN(3)) + x86("mov", "r15", stfh() ? HKQ(3) : FRQ(_.op_off + 64))
         + x86("mov", "rdi", "r13")
         + x86("mov", "rsi", "r15")
         + x86("note", HKN(4)) + x86("mov", "rdx", stfh() ? HKQ(4) : FRQ(_.op_off + 72))
         + x86("call", "rt_match_ctx_restore", (uint64_t)(uintptr_t)(void *)rt_match_ctx_restore)   /* re-sync the C-side Σ/Σlen mirror (pattern_match.c / runtime_eval.c readers) */
         + IF(_.flat_deep_arrival && !_.op_stmt_pin, x86("note", HKN(0)) + x86("mov", "rbp", stfh() ? HKQ(0) : FRQ(_.op_off + 40)))   /* BRACKET-GATE (s193): restore only if the save above ran.  HEAD-PIN (s22z): under the pin this restore MOVES to the statement-terminal cut (x86_asm.h) -- the sole release authority -- because the direct af/β fail edges bypass this tail entirely (the 061 m3 witness: `add rsp,K; jmp NO` with rbp left on the region); restoring here TOO would leave the cut's [rbp+off] read pointing into the restored OLD frame. */
         + x86_omega();
}
