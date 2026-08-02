#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
extern "C" void *rt_flat_ret_snap(void);
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* GLUE-1 FLAT (Lon directive s21x-n: "Devise two glue codes, one flat jumps and the second the same plus setup of stack frame" + "You might have to put some IF logic in the templates for implementing
 * the FOUR ZETA modes, which is fine").  THE FLAT GLUE: a BB's own storage bracket with NO frame pointer -- the box enters, carves its cell with ONE instruction, and every port transition is a plain jmp.
 * This is the ~99.999% case of the s21x-c design of record (law 3: RSP-only for every box and construct until a genuine brick wall); the FRAMED twin next door is the other ~0.001%.
 * PROMOTED FROM x86_asm.h (s21x-n): these four lines lived as inline helpers beside the encoders from s21x-m and had ZERO callers -- scaffolding that was never wired.  They belong here, not there, on the
 * project's own taxonomy: x86_asm.h owns ENCODINGS (one instruction, both media, medium switched invisibly -- R2), a template owns a SEQUENCE of x86(...) calls for a box.  The deciding rule is R6: the
 * four-mode variance below wants the sanctioned IF(...) combinator folded into ONE concatenation, and R6 is a template rule; expressed among the encoders the same variance would have to be C++ ternaries.
 * K COMES FROM `_`, NOT A PARAMETER (R5): the granted cell size is a promoted node scalar (_.op_fc_bytes, set at the ONE choke in walk_bb_node_inner from zw_node_k -- the ONE K AUTHORITY).  The s21x-m
 * scaffolding took `long K` as an argument, which quietly invited a second opinion about a box's size to exist at the call site; reading `_` means the allocator and the accessor (x86_zop) are looking at
 * the same field and cannot disagree.
 * THE FOUR MODES, and why three of them emit nothing here: a per-BB carve is what CELL_STACK *is*.  Under FRAME_R12 and FRAME_RSP the graph's storage was already carved whole at entry, so a per-BB carve
 * would be a SECOND allocation of the same bytes -- the named anti-pattern (roman) from the other direction.  Under CELL_HEAP the cell comes off the rbx-topped bump frontier, not rsp, and that arm is
 * HZ-1's (ARCH-ICON.md: mov rax,rbx / add rbx,K / cmp rbx,[RT_WS_LIMIT] / ja refill) -- NOT written here, and deliberately loud rather than silent, because the one failure mode this subsystem keeps
 * re-learning (x86_fc_hit's documented silent fallback; the s21x-m carve-only class) is that a storage decision which quietly does nothing produces plausible code and a wrong answer. */
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_glue_flat_enter() {
    /* GLUE-3 (Lon s21x-o): NO x86_begin() here BY DESIGN.  x86_begin mints a fresh x86_uid from g_flat_node_id in TEXT medium, and that uid names the box's RO constant labels (.Lx<uid>_0).  The glue is not a
     * top-level box template -- it is a FRAGMENT emitted inside another box's alpha/beta/gamma/omega port, so it must inherit that box's uid rather than mint one.  Calling x86_begin advanced the counter once
     * per port and renumbered every downstream label, which is exactly what the byte-identity A/B caught: instructions identical, .Lx5_0 became .Lx8_0.  A pure label rename is harmless to execution and would
     * have been invisible in a run-only test -- and would then have churned every committed .s artifact for no reason, which is precisely the kind of unexplained diff the artifact discipline exists to prevent. */
    if (!PLATFORM_X86) return std::string();
    return IF(x86_zstorage() == ZC_STORAGE_CELL_STACK && _.op_fc_bytes > 0, x86("sub", "rsp", _.op_fc_bytes))
         + IF(x86_zstorage() == ZC_STORAGE_CELL_HEAP  && _.op_fc_bytes > 0, x86_bomb("bb_glue_flat_enter: CELL_HEAP per-BB carve is HZ-1, not implemented"));
}
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* THE LEAVE SIDE IS NOT THE MIRROR OF THE ENTER SIDE, AND THAT ASYMMETRY IS THE WHOLE PROTOCOL (s21x-m law 1, measured -- do not "simplify" it back).  Which exits free depends on whether the cell is
 * READ by anyone.  A CARVE-ONLY cell (op_fc_base < 0, nobody addresses it) is rsp-neutral at every box boundary: allocate at alpha AND beta, free at gamma AND omega.  A WINDOWED cell (op_fc_base >= 0,
 * the box or its consumer reads it) must SURVIVE gamma -- that is S10c suspension and it is the entire point of having the cell -- so it frees at omega only, and beta re-enters on the still-live cell
 * without re-allocating.  The v0 that mirrored enter/leave blindly measured m3 109/207: gamma-suspending an unreferenced cell buys nothing and displaces every flat read below it. */
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_glue_flat_leave() {
    if (!PLATFORM_X86) return std::string();
    return IF(x86_zstorage() == ZC_STORAGE_CELL_STACK && _.op_fc_bytes > 0, x86("add", "rsp", _.op_fc_bytes));
}
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* OUTER-EXIT-1 (Lon directive s22p: "the gamma/omega landings ... belong to the OUTERMOST BOX's own ports and to the dynamic glue, never to a graph-level epilogue").  These two are what the deleted
 * xa_flat_epilogue was ACTUALLY for in the ordinary case, stripped of the five regimes that were only ever co-tenants in its if/else chain: the outermost box is CALLed by main (xa_file_header emits
 * `call main_alpha`), so its two ports are RETURNS, and the whole protocol is the eax code the caller reads -- 1 = the chain succeeded, 99 = it failed.  edx is zeroed as the omega/value wire per the
 * s22o wire contract (rcx = gamma-return, rdx = omega-return): a determinate outer chain carries no result out, so the wire ships NULL rather than whatever the last box left in the register.
 * NO STACK MOTION HERE BY DESIGN.  There is no prologue any more, so the outermost box carved nothing at alpha and there is nothing to release at gamma/omega -- the balanced per-BB spine returns rsp to
 * entry depth on its own.  The moment the spine goes NON-POPPING (the ladder rung above this one), that ceases to be true and the WHACK belongs here: bb_glue_framed_leave() in front of the ret, which
 * discards the activation wholesale at whatever depth it actually reached.  That is the sanctioned law-4 RBP -- one frame pointer, at the one sync point, for the one job a depth-immune base is for. */
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* THE WHACK PREDICATE -- ONE AUTHORITY.  TRUE NOW (s22p, one-shot bridge + non-popping flip atomic).
 * The flip is safe because the outermost α NOW emits bb_glue_framed_enter() (emit.cpp codegen_flat_chain_body
 * α preamble, same commit), pinning rbp to the stack base BEFORE the graph runs.  The whack (mov rsp,rbp;
 * pop rbp) at γ/ω discards the activation WHOLESALE at whatever depth it actually reached -- no per-box pop
 * accounting needed, no second opinion about depth.  This is the law-4 RBP: one frame pointer, at the one
 * known sync point (graph completion / FENCE checkpoint), for the one job a depth-immune base is for.
 * THE ORDER CONSTRAINT THAT MAKES THIS SAFE (measured, not assumed -- the 058 disease):
 *   (1) framed_enter at outermost α  fires FIRST -- pushes rbp, seeds rbp=rsp
 *   (2) graph body runs, rsp wanders freely (non-popping FORTH spine)
 *   (3) framed_leave at γ/ω          fires LAST -- restores rsp=rbp, pops rbp
 * Reversing (1)/(3) or omitting (1) while keeping (3) loads the CRT caller's rbp into rsp.
 * emit.cpp:2121 records the same trap for the first statement head stub. */
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int g_glue_entered = 0;   /* GLUE-SYM (s206): set at the framed-enter site (emit.cpp), read here.  ONE decision, recorded once, consulted by its own counterpart -- the shape zc_nofc and x86_jcc_invert were both collapsed into after drifting as two spellings. */
static inline bool bb_glue_outer_whack() { static int s = -1; if (s < 0) { const char * e = getenv("SCRIP_GLUE_SYM"); s = (e && *e == '1') ? 1 : 0; } return s ? (g_glue_entered != 0) : true; }   /* ⭐⭐⭐ GLUE-SYM (s206): THE WHACK MUST FOLLOW THE ENTER.  This returned true UNCONDITIONALLY while the enter at emit.cpp:2153 is guarded by (nparams==0 && !flat_jmp_entry && !flat_pat && !flat_gen && !g_gen_proc_active) -- so every pat/gen/jmp-entry/gen-proc graph emitted `mov rsp,rbp; pop rbp` against a base it never established, which is verbatim the failure this file's own header names four lines up ("omitting (1) while keeping (3) loads the CRT caller's rbp into rsp").  MEASURED on a 7-line repro (procedure g() with two suspends, every write(g())): proc_g carries push rbp=0 / mov rsp,rbp=2 / pop rbp=3 and SEGVs.  ⛔ THE OPPOSITE FIX WAS TRIED FIRST AND FALSIFIED: forcing the ENTER to fire for those classes (so it matched the unconditional whack) scored Icon 184/79/30 -> 181/82/30, THREE PROGRAMS WORSE -- the pinned classes decline the push deliberately, because a jmp-entry graph's base is established by its CALLER and a dc-prep graph's by rt_pl_dc_prep, so an extra push shifts rsp under offsets computed against the real base.  The enter is therefore RIGHT and the whack was the drifted spelling.  NOT A REGRESSION: CARVE-KILL (ef9a7d2c/1ba33ea6) deleted xa_flat_prologue, which used to seed rbp for exactly these classes, and the surviving whack inherited a `true` written when the prologue still covered them. */
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_glue_outer_γ() {
    /* ONE-SHOT BRIDGE gamma landing.  TEXT (mode-4): whack+exit(0) -- no ret, no eax.
     * BINARY (mode-3): whack+eax=1+ret back to C caller (no PLT available in JIT slab).
     * ⭐ EXIT-ALIGN (s22q): the `add rsp, 24` s22p put here is DELETED, and it was an ABI violation on EVERY
     * mode-4 program, not a fragile constant that merely might drift.  It was restoring rsp to main's CRT
     * entry value before `call exit@PLT` -- but exit() NEVER RETURNS, so restoration buys nothing, while
     * entry parity is exactly the WRONG parity to call from: after framed_leave rsp is already ≡0 (mod 16),
     * which is call-correct, and +24 moved it to ≡8.  MEASURED at rt_rspd_report (the .so destructor exit()
     * runs): rsp%16 = 8 in BOTH a crashing program (W06_pos) and a passing one (002_output_integer_literal)
     * -- i.e. every m4 program was misaligned and the survivors were LUCKY, which is why the fail set looked
     * arbitrary.  The crash lands wherever glibc first touches an aligned SSE op on a spilled local (getenv
     * via _dl_call_fini here), never in the graph, which is what made it look like a pattern defect. */
    if (!PLATFORM_X86) return std::string();
    return IF(bb_glue_outer_whack(), bb_glue_framed_leave())
         + IF(MEDIUM_TEXT,   x86("xor", "edi", "edi") + x86("call", "exit@PLT"))
         + IF(MEDIUM_BINARY, x86("mov32", "eax", (long)DT_S) + x86("ret"));
}
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_glue_outer_ω() {
    /* ONE-SHOT BRIDGE omega landing.  TEXT: exit(1).  BINARY: eax=99, ret.  EXIT-ALIGN (s22q): see γ above --
     * the `add rsp, 24` is deleted here for the same reason and by the same measurement. */
    if (!PLATFORM_X86) return std::string();
    return IF(bb_glue_outer_whack(), bb_glue_framed_leave())
         + IF(MEDIUM_TEXT,   x86("mov32", "edi", 1) + x86("call", "exit@PLT"))
         + IF(MEDIUM_BINARY, x86("mov32", "eax", (long)DT_FAIL) + x86("ret"));
}
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* WIRE-EXIT GLUE (Lon directive s22v: "dynamic glue templates for one-shot and pass-through access to complete BB graphs which have one entry and one exit" + "WHACK-FREE at completion").  A DEFINE
 * stub blob is wire-entered: rt_proc_call_open/_slim pushed a pcall record, passed the caller's γ/ω landings in rcx/rdx, and the blob's role-3 WIRE-ADOPT box (its first box) recorded them plus entry
 * rsp / caller rbp into the record's wire quad.  The blob's OWN shared γ/ω ports therefore exit the same way the RETURN/FRETURN floaters do -- snap the open record, restore the caller's machine state
 * from it (rsp from the RECORDED entry value: the known sync point is the adopt, no rbp assumption, no whack), and jmp home through the port's wire.  γ rides the gw wire (RETURN semantics: fname's
 * cell holds whatever was assigned, null if nothing); ω rides the ww wire (FRETURN semantics: the caller's :F() sees the failure).  This retires the whack+exit@PLT landing on stub blobs, which (a)
 * whacked an rbp NO authority had pinned for this class (the α pin guard excludes jmp-entry; the RBPPAIR falsification s22u proved suppressing the whack alone is NOT the cure) and (b) reported
 * exit(0) -- a SILENT SUCCESS -- on the γ arm of a path that means the transfer machinery fell through.  Level-0 arrival dies loudly inside rt_flat_ret_snap (error 18, "return from level zero"),
 * strictly better than the silent exit.  ONE AUTHORITY: bb_save_restore.cpp's role-1/2 floaters consume this same function for their tails, so the wire-exit sequence exists exactly once. */
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_glue_wire_exit(int is_gamma) {
    if (!PLATFORM_X86) return std::string();
    return x86_align_enter()
         + x86("call", "rt_flat_ret_snap", (uint64_t)(uintptr_t)(void *)rt_flat_ret_snap)
         + x86_align_leave()
         + x86("mov", "rcx", RDQ("rax", is_gamma ? 0 : 8))
         + x86("mov", "rbp", RDQ("rax", 24))
         + (x86_zc_frame() != ZC_FRAME_RSP ? x86("mov", "r12", RDQ("rax", 32)) : std::string())
         + x86("mov", "rsp", RDQ("rax", 16))
         + x86("jmp", "rcx");
}
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_glue_wire_γ() { return bb_glue_wire_exit(1); }
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_glue_wire_ω() { return bb_glue_wire_exit(0); }
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* PASS-THROUGH GLUE (Lon s22v, the four-glue grid): the BARE WIRE CONTRACT — lea the caller's own γ/ω continuation labels into rcx/rdx and jmp the resolved target in rax.  This is the ONE dynamic
 * linkage the whole system speaks (rt_chain_enter, rt_proc_call_open/_slim, and every emitted call/defer/capture site all spell exactly this); ONE-SHOT linkage = this contract PLUS a pcall record
 * (open pushes, role-3 adopts, bb_glue_wire_exit/floaters come home through it); PASS-THROUGH = the bare contract, nothing to record, nothing to release — the target's exits ride the wires straight
 * back into this box's own continuation labels.  BB_DEFER's blob entry is the canonical consumer (need TWO of the grid); the remaining hand-rolled trios (bb_call_proc_staged ×5, bb_call_value,
 * bb_match_capture, bb_match_end, defer's second site) are the CONVERSION BACKLOG ledgered in GOAL-SNOBOL4-BB.md — convert on touch, byte-identical by construction. */
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_glue_pass_wires(int gid, int wid) {
    if (!PLATFORM_X86) return std::string();
    return x86_lea_id("rcx", gid)
         + x86_lea_id("rdx", wid)
         + x86_jmp_reg("rax");
}
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
