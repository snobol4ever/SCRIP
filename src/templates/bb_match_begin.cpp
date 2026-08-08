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
/* CAS-SENTINEL-CLEAN: g_patstk_sp extern removed — no pattern stack island */
extern "C" uint32_t g_cap_gen;
#define hfc() (x86_port_mode() == ZC_PORT_FORTH && _.op_fc_wbytes > 0)
#define subjc() (_.op_subj_cell)
static int hpin(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_HEAD_PIN"); v = (e && *e == '0') ? 0 : 1; } return v; }   /* HEAD-PIN killswitch (s22z): =0 restores the unpinned armed head for A/B */
static int jepin(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_JEPIN"); v = (e && *e == '0') ? 0 : 1; } return v && _.flat_jmp_entry && _.flat_deep_arrival; }   /* JE-PIN (X12, FINDING-2026-08-07f): a DEFINE-stub chain (CLASS P, wire-entered) has NO rbp seed -- GLUE-O's _legacy conjunct skips the framed enter, scan_live=0 skips BLOB-GRANT, and the EXTENT-GATE's !flat_jmp_entry conjunct skipped this pin -- while deep_arrival flips every FRQ rbp-relative, so the head's PATCTX saves scribbled the ambient rt C frame ([rbp+0x60] = live continuation slot; gdb-bracketed: mov r13,[rbp+96] with rbp=rt frame, r13=0 pre-enter, zeroing the m3 exit path).  m4 survives because the body rides main's chain under main_alpha's GLUE-O pin and C calls preserve rbp.  The REPL-PIN law already names the cure: the pin is the head's law-4 base for EVERY deep-arrival statement.  Independent of flat_layout_unknown: this term is BASE ESTABLISHMENT, not extent bracketing.  SCRIP_JEPIN=0 restores the EXTENT-GATE predicate byte-identical. */
static int apin(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_ARMED_PIN"); v = (e && *e == '0') ? 0 : 1; } return v; }   /* PB-2 ARMED-PIN killswitch: =0 restores old rpin() for A/B. */
static int stmt_has_proc_invoke(void) { if (!g_emit_cfg) return 0; for (int _i = 0; _i < g_emit_cfg->n; _i++) { IR_t * _nd = g_emit_cfg->all[_i]; if (!_nd) continue; if (_nd->op == IR_MATCH_DEFER && !(_nd->pat_static && IR_LIT(_nd).sval && !strncmp(IR_LIT(_nd).sval, "PATV$", 5))) return 1; } return 0; }   /* PB-2 PROC-INVOKE GUARD: IR_MATCH_DEFER compiles a separately-emitted PAT$ proc blob whose prologue does sub rsp,K + mov rbp,rsp and whose omega does lea rsp,[rbp+K] + restore rbp=[rbp+88].  Under the ARMED-PIN (rbp==claim_base), the proc's saved-rbp slot [rbp+88] holds claim_base; the proc omega then executes lea rsp,[claim_base+K], pointing rsp into the middle of the claim region -- SIGSEGV.  Without the pin (rbp==ambient CRT frame), the proc omega's lea rsp,[CRT_rbp+K] lands in normal stack territory.  IR_MATCH_VALUE is EXCLUDED from this guard: it resolves at runtime via rt_dcap_pump/VALUE machinery (no separately compiled proc blob, no problematic omega), so VALUE-bearing graphs keep the ARMED-PIN.  PATV$-EXCLUSION (Bug-1-FIX s17): PATV$-prefixed pat_static=1 DEFER nodes are BUILD-snapshot inlines (rt_defer_open slow-path: push r14/r15/r13 + call; no sub rsp,K+mov rbp,rsp proc prologue) -- excluding them from the proc-invoke guard lets the ARMED-PIN fire for pb_snapshot_imm-class graphs so PATCTX saves land inside the claim region, not in the caller frame. */
static int rpin(void) { return hpin() && ((_.flat_layout_unknown && (subjc() || _.op_zres || !_.flat_jmp_entry)) || jepin());   /* M-3 MERGE: ARMED-PIN disjunct excised -- its discriminators (op_udout/op_zw/op_zw2) were UCLAIM planner outputs physically deleted; no static claim exists to pin */  }   /* EXTENT-GATE (Lon ruling 2026-08-06): MATCH_BEGIN sets an RBP frame ONLY for unknown-depth statements; flat_layout_unknown=0 uses ADD-based whack.  ⭐ PB-2 ARMED-PIN (s15): static-extent ZD-H hybrid (has_blob=1, nblob_real=0, no ZWS/ZWR/STF) needs a stable rbp base for PATCTX saves -- without the pin, FRQ(op_off+N) under a GLUE-O rbp re-adds op_flat_disp at encode time, landing in the CRT frame (canary smash).  Excluded: !op_zw/!op_zw2 = ZWS/ZWR have own frame protocols; !flat_stmt_frame = STF has own bracket; !stmt_has_proc_invoke = DEFER/VALUE blobs whose omega restores rsp via claim_base rbp, corrupting the stack (PB-2 s16 root cause: D10/D11/H21).  Killswitch SCRIP_ARMED_PIN=0. */
#define stfh() (_.flat_stmt_frame)
static const char * HKQ(int k) { static char b[8][40]; static int i; i = (i + 1) & 7; snprintf(b[i], 40, "qword ptr [rbp + %d]", -48 + 8 * k); return b[i]; }   /* SUBJ-ARM-2 (Lon s192, the predicted brick wall: "oh I need a RBP stable base pointer" -- STATEMENT housekeeping is the sanctioned RBP class): the five POST-UNWIND-lifetime head fields live in statement-bracket rbp slots, carved sub rsp,48 at alpha (40 used + 8 pad keeps the C-call 16-alignment: bracket base B==8 mod 16, B-56-32 == 0), reclaimed by the bracket leave's mov rsp,rbp for free.  k: 0=deep-rbp(+40 twin) 1=r13 2=r14 3=r15 4=capgen.  Spelled "+ -N" because x86_parse's generic reg-disp arm strtols after " + "; under an UNPINNED armed main the rbp base takes that generic arm (no frame compensation -- rbp is the depth-free statement base, exactly the point). */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_begin() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    return ((_.op_sa < 0 && !subjc() && !_.op_zres) || _.op_off < 0)
         ? x86_bomb("IR_MATCH_BEGIN: subject/start slot not promoted (emit_drive)")
         : x86("comment", "IR_MATCH_BEGIN")
         + x86_alpha()
         + IF(_.op_zres, x86("note", ZOPN(0)) + x86("mov", "rdi", x86_zref(_.op_zread[0] + 0, 1))
                       + x86("note", ZOPN(0)) + x86("mov", "rsi", x86_zref(_.op_zread[0] + 8, 1))
                       + IF(_.op_sa >= 0 && !subjc(), x86("mov", FRQ(_.op_sa), "rdi")
                                        + x86("mov", FRQ(_.op_sa + 8), "rsi")))   /* ⭐ CLAIM-AT-HEAD v2 (this session; supersedes the intermediate HEAD-FOLD): the claim is carved at THIS box's alpha by the port hook -- the match owns its scanner storage -- so pre-head producer cells ride ABOVE the claim and the head's own rsp IS the claim base with zero instruction cost: no pop, no fold, PURE NON-POPPING (the directive verbatim), and the blob's raw claim spellings (rspd/x86_zref escapes, shift-fragile by the ON-5/ARGREAD design -- gdb-bracketed in 156 as a wild rt_cap_push slot pointer) hold at OFF-world distances by construction.  The subject read is op_uclaim + op_zread[0]: the claim the hook carved below, plus the staged delta to the subject's cell among the above-claim producers -- zread is 0 when the subject producer is adjacent (strip, roman) but POS(N)/LEN(n) variable operands put coerce chains on the spine between subject and head (MEASURED: 061_capture_in_arbno read the coerce cell as the subject descr under a hardcoded +0 and failed every match).  The MIRROR restores the dataflow the !subjc OFF world fed via the producer's flat stores (which the armed producer no longer emits) -- REPLACE and any op_sa reader see the same bytes as ever.  GATED !subjc(): on subject-cell-granted graphs the OFF world popped and never wrote op_sa -- the slot belongs to another node's layout there, and writing it clobbers live state (MEASURED: 054_pat_arbno_alt, the mirror's two stores were the ONLY functional diff and flipped it red). */
         + IF(!_.op_zres && subjc(), x86("mov", "rdi", "qword ptr [rsp + 0]")
                     + x86("mov", "rsi", "qword ptr [rsp + 8]")
                     + x86_zrelease(16))   /* SUBJECT-CELL rung (a) (Lon: "index operands from RSP... pop into Sigma/delta/Delta"): the registered subject producer chain left its 16B DESCR at TOS; POP IT FIRST -- before ANY flat-spelled access below -- so alpha's remaining depth equals the flat arm's exactly and fc_leaf_walk's D=32+prefix for every downstream pattern box holds untouched.  rdi/rsi survive to the rt_match_enter call: the intervening saves touch only rcx/rax/r13/r14/r15/rbp (the non-RSP-frame rt_zls_mark arm cannot fire here -- promotion is gated ZC_FRAME_RSP). */
         + IF(rpin(),
               x86("note", HKN(0)) + x86("mov", RDQ("rsp", _.op_off + 40), "rbp")
             + x86("note", "stmt_base") + x86("mov", "rbp", "rsp"))   /* ZD-5 MATCH-SPINE: the pin lands ON the claim base -- alpha rsp is hdel() below it when armed producer cells ride above the claim (non-popping law), 0 when declined, so declined emission stays byte-identical (mov). */   /* ⭐ HEAD-PIN (s22z, killswitch SCRIP_HEAD_PIN=0): the match CONSTRUCT establishes its own law-4 base -- rbp := statement region base, the address every FRQ slot in the head/fence/release/exhaust extent was baked against.  CARVE-KILL deleted the whole-graph carve whose base the one-time main pin used to BE, so at HEAD every [rbp+off] statement slot resolved against main's (m4) or rt_chain_enter's (m3) frame: dead CRT territory in m4 (survived by luck), LIVE C frame in m3 -- measured 061_pat_fence_fn_seal, armed: output-correct then rc=124, gdb-sampled as a full statement re-execution loop (NV_SET + slab PCs) because the head's [rbp+88..120] housekeeping corrupted the chain's return path.  THE LICENSE is the STF-UNFLIP ruling's own words: no per-statement bracket EVER, but law 4's rbp serves "CONSTRUCT housekeeping that must survive an unwind" -- and s192 already named STATEMENT housekeeping as the sanctioned RBP class.  This is that citizen, construct-scoped: pin here, restores at the construct's own two exits (the +40 restores below and in bb_match_end, BRACKET-GATE paired since s193, correct with ZERO edits once the save writes the OLD base).  PLACEMENT: after the subject pop rsp == region base by the PATCTX invariant ("alpha depth == the FRQ-baked depth"), so the rsp-relative spelling writes the true region slot BEFORE rbp moves; before every FRQ save below so r13/r14/r15/capgen land in the region, not the ambient frame.  op_zgpop stays SOLE release authority -- this pin moves no rsp, frees nothing, and dies at the construct exits. */
         + IF(stfh(), x86_zclaim(48))   /* SUBJ-ARM-2: the head's LOCAL STORAGE, one instruction (Lon: "each BB allocates its LOCAL STORAGE... by one instruction, decrement RSP"); the mark below captures rsp AFTER this carve, so the S10e unwind returns here and the rbp-slot fields survive to RELEASE and the failure exit */
         /* ZW-0 stage 2: island arm deleted -- unreachable under ZC_FRAME_RSP default */
         + IF(!_.op_zres && !subjc(), rpin()
                      ? x86("mov", "rdi", RDQ("rsp", _.op_sa))
                      + x86("mov", "rsi", RDQ("rsp", _.op_sa + 8))
                      : x86("mov", "rdi", FRQ(_.op_sa))
                      + x86("mov", "rsi", FRQ(_.op_sa + 8)))   /* legacy flat-slot subject read -- the arm the SUBJECT-CELL rung retires; under subjc the DESCR already rides rdi/rsi from the TOS pop above.  REPL-PIN (this session): under the widened pin the raw flat op_sa is the deleted-carve coordinate (256 against a 176B claim on tB, exactly the hoffb comment's 066 shape) while the subject producer WROTE through zvo to the claim slot -- so the pinned read resolves through the SAME owner table, base-relative off the pinned rbp; the unpinned arm keeps the legacy spelling byte-identical. */
         + IF(x86_zc_frame() == ZC_FRAME_RSP && hfc(), x86("mov", "rax", "rsp")
                                                + x86_zclaim(80)
                                                + x86("note", "rsp_mark") + x86("mov", FRQ(_.op_off + 16), "rax"))   /* ⭐ M-2 HFC-CLAIM REORDER: sub rsp,80 now fires BEFORE the PATCTX saves so [rsp+48..72] resolves inside the 80B claim region (pre-M-3 the UCLAIM head claim allocated those bytes; post-M-3 the hfc sub MUST precede the saves).  rax = pre-sub rsp, written to rsp_mark slot and reused for cas_rsp_mark below BEFORE cap_gen overwrites it. */
         + x86("note", HKN(1)) + x86("mov", stfh() ? HKQ(1) : FRQ(_.op_off + 48), "r13")   /* PATCTX (Lon directive 2026-07-29): save the OUTER Σ/δ/Δ before rt_match_enter sets them anew; both exits restore.  M-2: saves now fire AFTER sub rsp,80 so FRQ resolves inside the 80B claim. */
         + x86("note", HKN(2)) + x86("mov", stfh() ? HKQ(2) : FRQ(_.op_off + 56), "r14")
         + x86("note", HKN(3)) + x86("mov", stfh() ? HKQ(3) : FRQ(_.op_off + 64), "r15")
         + x86("note", "cas_top") + x86("mov", RDQ("r12", 0), (long)0) + x86("note", "cas_rsp_mark") + x86("mov", RDQ("r12", 8), hfc() ? "rax" : "rsp") + x86("mov", RDQ("r12", 16), (long)0) + x86("note", "cas_top") + x86("add", "r12", (long)24)   /* CAS-SENTINEL-CLEAN: [+0]=tag0,[+8]=rsp_mark,[+16]=0(was patstk,dead pad); CAS-MARKER (Lon s8): tag-0 sentinel at match start; RELEASE/fail scan to it depth-free.  M-2: hfc arm uses rax (= pre-sub rsp, still valid here; cap_gen overwrites rax below); non-hfc arm keeps "rsp" (unchanged). */
         + x86("lea", "rcx", "[rip + __]", (uint64_t)(uintptr_t)(const void *)&g_cap_gen, "g_cap_gen")
         + x86("mov", "eax", RDD("rcx", 0))
         + x86("note", HKN(4)) + x86("mov", stfh() ? HKQ(4) : FRQ(_.op_off + 72), "rax")   /* PATCTX-2: the capture generation id is pattern context too -- read BEFORE rt_match_enter draws a fresh id from the well; both exits hand it back via rt_match_ctx_restore arg 3 */
         + IF(_.flat_deep_arrival && !rpin(), x86("note", HKN(0)) + x86("mov", stfh() ? HKQ(0) : FRQ(_.op_off + 40), "rbp"))   /* REPL-PIN: complementary to the widened pin on the SAME hpin() -- the old !(subjc&&hpin) guard let this raw flat save fire BESIDE the pin for the zres population, a stray write the pin's rsp-relative old_rbp slot made redundant; exclusive-by-predicate now, per the pin comment's own "exclusive by construction" law. */   /* BRACKET-GATE (s193): the +40 save exists to bracket the ARBNO zv() borrow (and any deep repoint); a depth-static graph has no repointer, so save AND both restores gate together on the same predicate the outer quartet reads — drift-proof by shared condition.  HEAD-PIN (s22z): under the pin the slot already holds the OLD rbp (written rsp-relatively before the pin above); this FRQ save would overwrite it with the NEW base and turn both exit restores into self-no-ops — the exact disease the pin cures — so the arms are exclusive by construction. */
         + x86("call", "rt_match_enter", (uint64_t)(uintptr_t)(void *)rt_match_enter)
         + x86("mov", "r13", "rax")
         + x86("mov", "r15", "rdx")
         + IF(x86_zc_frame() == ZC_FRAME_RSP && !hfc(), x86_zls2_mark_save(_.op_off + 16))   /* ⭐ W-1c.3 Part B non-hfc arm only: hfc arm already saved rsp_mark above before the sub; non-hfc keeps its original placement. */
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
         + IF(g_emit.xa_bb_emit_pair_n >= 4 && g_emit.xa_bb_emit_pair_define[3] != NULL, x86("def", PAIR(3)))   /* SEAL-UNWIND (this rung): the pat_seal landing -- fence/ABORT kills enter HERE, the anchor-exhaust unwind (CAS sentinel pop, rsp restore, PATCTX rt_match_ctx_restore, claim release, then omega = statement-fail with NO anchor advance).  Closes the zeta_storage KNOWN BYPASS: seal kills used to jmp the raw fail statement, skipping this entire unwind (MEASURED: t1m bare-FENCE-failing = SEGV-at-exit; t1x2 two such statements = silent zero-output; fence_probe T6 = cross-statement contamination SEGV).  Guard: only defs when the drive staged pair[3] (statement-graph BEGINs); blob-path BEGINs stage nothing and emit nothing here. */
         + (hfc() && x86_zc_frame() == ZC_FRAME_RSP
             ? x86("note", "cas_mark") + x86("sub", "r12", (long)24)   /* W-1c.3 NO-SCAN: r12 arrives at marker+24 by LIFO invariant; single sub reaches sentinel */
             + x86("note", "cas_rsp_mark") + x86("mov", "rsp", RDQ("r12", 8))   /* CAS-SENTINEL-CLEAN: patstk restore from [r12+16] removed; rsp restore from [r12+8] = pre-sub VAR level (cas_rsp_mark=rax=pre-sub, see above) */
             + IF(hfc() && x86_zc_frame() == ZC_FRAME_RSP, x86_zclaim(80))   /* M-2 PATCTX-DESCENT (af path): re-enter the 80B hfc slot zone so FRQ(op_off+48..72) resolves to the bytes the saves wrote; add rsp,80 below undoes this after the restores */
             : x86("note", "cas_mark") + x86("sub", "r12", (long)24)   /* W-1c.3 Part B non-hfc: LIFO sub first */
             + (x86_zc_frame() == ZC_FRAME_RSP
                 ? x86_zls2_release_to_call(_.op_off + 16)
                 : ( x86_align_enter()
                   + x86("call", "rt_zls_release_to", (uint64_t)(uintptr_t)(void *)rt_zls_release_to)   /* CAS-SENTINEL-CLEAN: patstk rdi load from [r12+16] removed */
                   + x86_zls2_release_to_call(_.op_off + 16)
                   + x86_align_leave())))
         + x86("note", HKN(1)) + x86("mov", "r13", stfh() ? HKQ(1) : FRQ(_.op_off + 48))   /* PATCTX restore on the failure exit -- post-unwind rsp is back at α depth in every arm, so the slot spelling holds; under the regime the rbp slots are depth-free by construction.  ⭐ OBJ-NOTE ON-3 (s23e): the RESTORE side now names the same five slots the α saves named, via the shared HKN(k) table -- s23c annotated the writes and left every read bare, so a .s reader saw '[rbp+96]' with no way to know it was the enclosing match's subject coming back. */
         + x86("note", HKN(2)) + x86("mov", "r14", stfh() ? HKQ(2) : FRQ(_.op_off + 56))
         + x86("note", HKN(3)) + x86("mov", "r15", stfh() ? HKQ(3) : FRQ(_.op_off + 64))
         + x86("mov", "rdi", "r13")
         + x86("mov", "rsi", "r15")
         + x86("note", HKN(4)) + x86("mov", "rdx", stfh() ? HKQ(4) : FRQ(_.op_off + 72))
         + x86("call", "rt_match_ctx_restore", (uint64_t)(uintptr_t)(void *)rt_match_ctx_restore)   /* re-sync the C-side Σ/Σlen mirror (pattern_match.c / runtime_eval.c readers) */
         + IF(_.flat_deep_arrival, x86("note", HKN(0)) + x86("mov", "rbp", stfh() ? HKQ(0) : FRQ(_.op_off + 40)))   /* BRACKET-GATE (s193): restore only if the save above ran.  HEAD-PIN (s22z): under the pin this restore MOVES to the statement-terminal cut (x86_asm.h) -- the sole release authority -- because the direct af/β fail edges bypass this tail entirely (the 061 m3 witness: `add rsp,K; jmp NO` with rbp left on the region); restoring here TOO would leave the cut's [rbp+off] read pointing into the restored OLD frame. */
         + IF(hfc() && x86_zc_frame() == ZC_FRAME_RSP, x86_zrelease(80))   /* M-2 PATCTX-ASCENT (af path): exit the hfc slot zone entered above; rsp now at pre-sub VAR level; ZD wpop (emitted by x86_omega) releases VAR's 16B */
         + x86_omega();
}
