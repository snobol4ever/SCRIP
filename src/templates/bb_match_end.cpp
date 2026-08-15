#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
}
extern "C" void rt_zls_release_to(void *mark);
extern "C" void rt_match_ctx_restore(uint64_t sig, uint64_t len, uint64_t capgen);
extern "C" long rt_dcap_end_ok_open(const char *mark, const char *top, const char *subj);
extern "C" long rt_dcap_step(DESCR_t fret);
/* CAS-SENTINEL-CLEAN: g_patstk_sp extern removed */
extern "C" void rt_dcap_end_ok_close(void);
extern "C" void *rt_proc_open_fn(void);
extern "C" DESCR_t rt_proc_call_epilogue_γ(DESCR_t frame0);
extern "C" DESCR_t rt_proc_call_epilogue_ω(void);
extern "C" long zvo_owner_dout(int cur_head);
#include "x86_asm.h"
#define rfc() (x86_port_mode() == ZC_PORT_FORTH && _.op_fc_disp >= 0)
#define hfc() (x86_port_mode() == ZC_PORT_FORTH && _.op_fc_wbytes > 0)   /* M-2: mirrors bb_match_begin's hfc() -- true when MATCH_BEGIN allocated the 80B hfc sentinel+PATCTX region */
static int oscap_l(void) { static int v = -1; if (v < 0) { const char * e = getenv("SCRIP_OS_CAP"); v = (e && *e == '0') ? 0 : 1; } return v; }
static int has_replace_l(void) { if (!g_emit_cfg) return 0; for (int _i = 0; _i < g_emit_cfg->n; _i++) { IR_t * _nd = g_emit_cfg->all[_i]; if (_nd && (_nd->op == IR_MATCH_REPLACE || _nd->op == IR_MATCH_FENCE1 || _nd->op == IR_MATCH_ABORT || _nd->op == IR_MATCH_ARBNO)) return 1; } return 0; }   /* ⭐ FENCE1/ABORT joined (measured, H01/H04/H06/H18/H28): both whack rsp to the ___ floor mid-match, discarding the zclaim(48) that backs HKQ -- subsequent quartet traffic then aliases re-carved spine cells (SEGV).  Same law as the slot authority declines: whack-hazard graphs keep legacy homes.  ARBNO joined (measured, X10): the ARB-LON-K16 mechanism-2 statement release reads old____ through the POSITIVE [___+pin] slot (x86_asm.h HEAD-PIN restore), which the widened head abandons for HKQ(0) -- correct output then exit SEGV on the stale base.  ARBNO graphs keep the legacy quartet home until that restore reads through the one macro too (later rung). */
#define stfh() (_.flat_stmt_frame || (oscap_l() && _.flat_deep_arrival && !_.flat_jmp_entry && !_.flat_lcl_proc && !_.zframe_graph && !_.flat_pat && !_.flat_gen && !has_replace_l()))   /* ⭐ OS-2·SLICE-1 QUARTET RE-HOME: the PATCTX/old____ quartet's FRQ(op_off+40..72) positive-off spelling on carve-less legacy deep MAINS rides finite CRT slack and was CAUGHT (hardware watchpoint) smashing environ[1] via the [___+312] old____ store when the head's ZLS off grew past ~336 (dc_recur m4) -- the s22r/s23a class, quartet family.  The stfh arm is the head's OWN correct negative-home protocol (HKQ [___-48..-8] backed by its zclaim(48)); widening its predicate to the GLUE-O legacy-deep-main class re-homes stores AND restores through the ONE macro in both files -- no split ends possible.  REPLACE graphs excluded: bb_match_replace.cpp:33 restores ___ from FRQ(op_off+40) directly and would read the abandoned slot; their quartet keeps legacy slack behavior (pre-existing risk, unchanged).  oscap_l: SCRIP_OS_CAP=0 restores the bare flat_stmt_frame predicate -- byte-identical legacy per the killswitch law.  Capture δ slots live at [___-64-16k], strictly BELOW this zone -- no overlap by construction. */
static const char * HKQ(int k) { static char b[8][40]; static int i; i = (i + 1) & 7; snprintf(b[i], 40, "qword ptr [rsp# + %d]", 16 + 8 * k); return b[i]; }
static const char * HKM() { return "qword ptr [rsp# + 8]"; }   /* ⭐ SLACK-RIDER RE-HOME (this session): the bb_match_begin twin, verbatim -- zls2_mark's negative home.  DELIBERATELY DUPLICATED rather than shared, exactly as HKQ already is across these two files: the pair is the head's ABI with itself and the existing file-local spelling is the established house shape here.  A drift between these two lines is the split-ends failure HKN(k) exists to catch, so both files name the slot through HKN(5) at every use. */
static const char * HKD() { return "dword ptr [rsp# + 0]"; }   /* ⭐ SLACK-RIDER RE-HOME (this session): the bb_match_begin twin, verbatim -- start_δ's negative home.  RELEASE runs inside the SAME bracket as the head's α, so [___-64] is depth-free at the post-unwind write exactly as it was at the write that seeded it -- the same argument HKQ's own comment makes for the quartet, and the reason this cell can move homes without any change to WHEN it is read. */   /* SUBJ-ARM-2: the head's statement-bracket ___ housekeeping slots (bb_match_begin.cpp twin, same k map: 0=deep-___ 1=r13 2=r14 3=r15 4=capgen) -- RELEASE runs inside the SAME bracket, so [___-48+8k] is depth-free at the post-unwind read exactly as at the head's alpha write */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string release_pump() {
    return std::string()
         + x86_xfer_enter()
         + x86_anchor_enter()
         + x86("mov",  "rsi", "r12")   /* CAS-R12-UNIFY: top IS r12 -- cell read deleted.  CAS-MARKER: mark is recovered by scanning down to HEAD's tag-0 marker (r8 walks a COPY; r12 must stay at top -- the pump consumes the entries above the marker); one config-blind mechanism serves every basis */
         + (emit_match_rbp()
             ? x86("note", "cas_mark") + x86("mov", "rdi", RDQ("rbp", -8))   /* MATCH-RBP (s65e): the walk floor IS the saved mark — one mov replaces the L(5) tag-0 scan; no sentinel exists to skip, so no +24 */
             : x86("mov",  "r8", "rsi")
             + x86("def",  L(5))
             + x86("sub",  "r8", (long)24)
             + x86("mov",  "rax", RDQ("r8", 0))
             + x86("test", "rax", "rax")
             + x86("jne",  L(5))
             + x86("lea",  "rdi", RDQ("r8", 24)))
         + x86("mov",  "rdx", "r13")
         + x86("call", "rt_dcap_end_ok_open", (uint64_t)(uintptr_t)(void *)(long (*)(const char *, const char *, const char *))rt_dcap_end_ok_open)
         + x86("def",  L(1))
         + x86("test", "rax", "rax")
         + x86("je",   L(2))
         + x86("call", "rt_proc_open_fn", (uint64_t)(uintptr_t)(void *)(void *(*)(void))rt_proc_open_fn)
         /* ZW-0 stage 2: island push/restore arms deleted -- unreachable under ZC_FRAME_RSP default */
         + bb_glue_pass_wires(3, 4)   /* GLUE-SYM (s22x): dormant legacy anchor hoisted above the glue; byte-identical at the ZC_FRAME_RSP default */
         + x86("def",  L(3))
         /* ZW-0 stage 2: island rsp/zr dance deleted */
         + x86("call", "rt_proc_call_epilogue_γ", (uint64_t)(uintptr_t)(void *)(DESCR_t (*)(DESCR_t))rt_proc_call_epilogue_γ)
         + x86("mov",  "rdi", "rax")
         + x86("mov",  "rsi", "rdx")
         + x86("call", "rt_dcap_step", (uint64_t)(uintptr_t)(void *)(long (*)(DESCR_t))rt_dcap_step)
         + x86("jmp",  L(1))
         + x86("def",  L(4))
         /* ZW-0 stage 2: island rsp/zr dance deleted */
         + x86("call", "rt_proc_call_epilogue_ω", (uint64_t)(uintptr_t)(void *)(DESCR_t (*)(void))rt_proc_call_epilogue_ω)
         + x86("mov",  "rdi", "rax")
         + x86("mov",  "rsi", "rdx")
         + x86("call", "rt_dcap_step", (uint64_t)(uintptr_t)(void *)(long (*)(DESCR_t))rt_dcap_step)
         + x86("jmp",  L(1))
         + x86("def",  L(2))
         + x86("call", "rt_dcap_end_ok_close", (uint64_t)(uintptr_t)(void *)(void (*)(void))rt_dcap_end_ok_close)
         + x86_anchor_leave()
         + x86_xfer_leave()
         + (emit_match_rbp()
             ? x86("note", "cas_mark") + x86("mov", "r12", RDQ("rbp", -8))   /* MATCH-RBP: SPIN the CAS to the MARK — one mov replaces the L(10) tag-0 pop scan */
             + x86("note", HKN(1)) + x86("mov", "r13", RDQ("rbp", -16))
             + x86("note", HKN(2)) + x86("mov", "r14", RDQ("rbp", -24))
             + x86("note", HKN(3)) + x86("mov", "r15", RDQ("rbp", -32))
             : x86("note", "cas_mark") + x86("def", L(10)) + x86("sub", "r12", (long)24) + x86("mov", "rax", RDQ("r12", 0)) + x86("test", "rax", "rax") + x86("jne", L(10))   /* ⭐ CAS-POP-SCAN (09h, supersedes W-1c.3 NO-SCAN, MEASURED): the single sub was arithmetic against a CALLEE-SAVED register the C pump cannot move -- gdb over 1000 ASSIGN_COND hits: r12 drifts exactly 24 B/capture-match, arena wall ~700k (pattern_bt_deep); pattern_bt "OK" at 500k was 12 MB of margin.  Fence law licenses the tag-0 marker scan (L(5)/L(9) discipline): MATCH_END is the commit, everything above the HEAD marker is dead post-pump. */
             + x86("note", HKN(1)) + x86("mov", "r13", stfh() ? HKQ(1) : FRQ(_.op_off + 48))   /* M-2 BUG-6 FIX: PATCTX-DESCENT sub rsp,80 deleted. After CAS restore rsp=cas_rsp_mark=pre-32B-carve rsp (MATCH_BEGIN stores cas_rsp_mark BEFORE x86_zclaim(32), and saves r13/r14/r15/___ also pre-carve at [rsp+48/56/64/40]). No descent needed: FRQ(op_off+48) resolves to [rsp+48] = the exact save address. */
             + x86("note", HKN(2)) + x86("mov", "r14", stfh() ? HKQ(2) : FRQ(_.op_off + 56))
             + x86("note", HKN(3)) + x86("mov", "r15", stfh() ? HKQ(3) : FRQ(_.op_off + 64)))
         + x86("mov", "rdi", "r13")
         + x86("mov", "rsi", "r15")
         + x86("call", "rt_match_ctx_restore", (uint64_t)(uintptr_t)(void *)rt_match_ctx_restore)   /* re-sync the C-side Σ/Σlen mirror; CAPGEN-ERAD: arg3 dropped, restore no longer writes g_cap_gen */
         + IF(emit_match_rbp(), x86("note", "frame_whack") + x86("mov", "rsp", "rbp") + x86("pop", "rbp"))   /* MATCH-RBP: the whole-frame WHACK — releases the frame AND every in-bracket ζ-cell in two ops; supersedes the [r8+8] rsp recovery, the zls2 slot, and the planner's carve accounting (stfh_k reads 0 through the wrapper) */
         + IF(_.op_dval == 0.0 && _.flat_deep_arrival, x86("note", HKN(0)) + std::string(""))   /* BRACKET-GATE (s193): paired with head's gated +40 save.  HEAD-PIN (s22z): under the pin the restore rides the terminal cut instead.  M-2 BUG-6 NOTE: !op_tail exclusion removed — PAIR(2) ___RAWD reads (bb_match_arbno.cpp PAIR(2)) fire BEFORE MATCH_END α; by the time release_pump runs, PAIR(2) is complete and HEAD-PIN ___ is safe to restore.  Tail path does NOT restore at ARBNO omega (L(2) only runs on full exhaust, not on first-match-success). */
         /* M-2 BUG-6 FIX: PATCTX-ASCENT add rsp,80 deleted (paired with DESCENT deletion above). After PATCTX restores rsp remains at pre-32B-carve level; STATEMENT_END's add rsp,16 releases the subject var cell correctly. */
         + x86_gamma();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_end() {
    x86_begin();
    if (getenv("SCRIP_MEND_ADDR_DIAG")) fprintf(stderr, "[MEND] op_off=%d op_fc_disp=%d op_dval=%g rfc=%d zc_frame=%d op_tail=%d\n", _.op_off, _.op_fc_disp, _.op_dval, rfc() ? 1 : 0, (int)x86_zc_frame(), _.op_tail);   /* L-3b STEP-3 DIAGNOSTIC (this session): env-gated (SCRIP_MEND_ADDR_DIAG), additive-only, prints EVERY input the write-side branch (start/end cursor stash) depends on -- placed to compare against bb_match_replace.cpp's read-side op_off/op_zpat (STEP-2 diagnostic) and check whether the writer's op_fc_disp/+32 terms are symmetric with what the reader compensates for.  Zero behavior change: fprintf only, no instruction emitted, only fires when the env var is set. */
    return !PLATFORM_X86 ? std::string()
         : _.op_off < 0
         ? x86_alpha() + x86_bomb("IR_MATCH_END: head slot not resolved (operand[0] missing or unowned)")
         : _.op_tail && rfc()
         ? x86("comment", "IR_MATCH_END (CAS-MARKER-CARRY tail: scan to the head's tag-0 sentinel, recover patstk (+16) and the rsp mark (+8) off it, one-mov unwind -- depth-free on every success-path depth, where the old RSP(op_fc_disp) reloads under-counted the live leaf cells the non-popping γ spine leaves (the 041 class: [rsp+16] read the assign_save cell, rsp := 0x7fff00000000).  Marker NOT popped here -- the pump walks the pend entries above it and its own L(6) scan pops the lot)")
         + x86_alpha()
         + (emit_match_rbp()
             ? std::string()   /* MATCH-RBP (s65e): no sentinel to scan, no rsp to recover here — the pump reads the mark off rbp; the frame whack after ctx_restore releases rsp */
             : x86("mov", "r8", "r12")   /* CAS-R12-UNIFY: seed the recovery scan from r12 (the one authority); r8 walks a COPY -- marker NOT popped here, the pump's L(6) scan pops the lot */
             + x86("def", L(8))
             + x86("sub", "r8", (long)24)
             + x86("mov", "rax", RDQ("r8", 0))
             + x86("test", "rax", "rax")
             + x86("jne", L(8))
             + IF(emit_arbno_rbp_unwind(), x86_arbno_rbp_unwind("r8", 8, 11, 12))   /* ⭐ ARBNO-RBP EPILOGUE (E-1): the SUCCESS exit. r8 holds the head sentinel and [r8+8] is the banked match-entry rsp -- the value the very next instruction loads -- so it is exactly the MARK the walk needs: above every ARBNO frame (all carved after the match began) and at/below the pre-match rbp. IN FRONT of the whack by design: once rsp moves the dead frames are below it. rax is dead here (the L(8) scan above exits holding tag 0). */
             + x86("mov", "rsp", RDQ("r8", 8)))   /* CAS-SENTINEL-CLEAN: patstk restore from [r8+16] removed; rsp restore from [r8+8] kept */
         + release_pump()
         : x86("comment", "IR_MATCH_END")
         + x86_alpha()
         + IF(x86_zc_frame() == ZC_FRAME_RSP && !emit_match_rbp(), x86("mov", "r8", "r12")   /* ⭐ W-1c.3 Part B: rfc/non-rfc fork DELETED -- both arms now scan via r8 (L(9)); the non-rfc slot read at FRQ(op_off+8) is removed (slot no longer written at alpha since the patstk slot-save was deleted from bb_match_begin).  r8 survives to the unwind below: rfc reads [r8+8] for rsp; non-rfc uses x86_zls2_release_to_call(op_off+16). */
                                               + x86("def", L(9))
                                               + x86("sub", "r8", (long)24)
                                               + x86("mov", "rax", RDQ("r8", 0))
                                               + x86("test", "rax", "rax")
                                               + x86("jne", L(9)))   /* CAS-SENTINEL-CLEAN: patstk restore from [r8+16] removed */
         + (_.op_dval != 0.0
                ? IF(rfc(), x86("mov", "eax", RDD("rsp", (int)_.op_fc_disp))
                          + (x86_zc_frame() == ZC_FRAME_RSP ? x86("mov", RDD("rsp", (int)(_.op_off + _.op_fc_disp + (emit_match_rbp() ? 64 : 32))), "eax")
                                                      : x86("mov", stfh() ? HKD() : FR(_.op_off), "eax")))
                + (x86_zc_frame() == ZC_FRAME_RSP && rfc() ? x86("mov", RSP((int)(_.op_off + 24 + _.op_fc_disp + (emit_match_rbp() ? 64 : 32))), "r14")
                                                     : x86("mov", FRQ(_.op_off + 24), "r14"))   /* ⭐ R-3(c)/L-3b MRBP WRITE-SIDE (third instance of the 64B mrbp hole; gdb-measured on l3_spl_break_nonterm: writer start@..e960/end@..e978, reader FR(op_off) reads @..e980/..e998 -- delta exactly 32 on both slots): the formula's +32 constant IS the legacy stfh/hfc head carve the s38 STEP-0' triangulation anchored on; under emit_match_rbp() the head moves RSP 64 (push rbp+4 pushes+sub 24) so the writer landed 32 BELOW the s38 fixed absolute the reader still targets -- rt_match_replace then received slen=0 + garbage cursors (SCRIP_REPL_TRACE witness) and replaced the WHOLE subject.  Same law as the zd_read cross-head fix at emit.cpp: the two head geometries are mutually exclusive branches of bb_match_begin:34, 64 replaces 32, never adds to it. */
                : std::string())
         /* ZW-0 stage 2: island rt_zls_release_to arm deleted -- unreachable under ZC_FRAME_RSP default */
         + (emit_match_rbp() ? std::string()   /* MATCH-RBP: rsp untouched here — the frame whack after ctx_restore in the pump tail is the sole release */
            : x86_zc_frame() == ZC_FRAME_RSP ? IF(emit_arbno_rbp_unwind(), x86_arbno_rbp_unwind("r8", 8, 11, 12)) + x86("mov", "rsp", RDQ("r8", 8))   /* ⭐ ARBNO-RBP EPILOGUE (E-1): same walk on the non-tail success arm -- the L(9) scan above leaves the same r8 sentinel and the same dead rax, so the MARK and the scratch are identical to the tail arm. The two arms are mutually exclusive ternary branches, so reusing L(11)/L(12) cannot collide. *//* s53 RSP-ONLY: BOTH arms unwind via the arena -- the L(9) walk above already found this activation's begin marker in r8, and [r8+8] is the rsp bb_match_begin BANKED there (depth-IMMUNE).  The old non-rfc HKM read ([rsp#+8]) was depth-static and any surviving interior record (DEFER's {pad,resume} success pair -- the r8.sno conviction: rsp loaded a CODE address) displaced it.  One value, one home, two spellings collapsed to the banked one. */
            : (rfc() ? x86("mov", "rsp", RDQ("r8", 8))   /* CAS-MARKER-CARRY unwind: depth-free; marker NOT popped -- the pump walks the pend entries above it and its L(6) scan pops the lot */
               : x86_zls2_release_to_call(stfh() ? HKM() : FRQ(_.op_off + 16))))
         + x86_align_leave()
         + release_pump();
}
