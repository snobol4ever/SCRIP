#include <string>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
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
static inline bool bb_glue_outer_whack() { return true; }
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_glue_outer_γ() {
    /* ONE-SHOT BRIDGE gamma landing.  TEXT (mode-4): whack+add rsp,24+exit(0) -- no ret, no eax.
     * BINARY (mode-3): whack+eax=1+ret back to C caller (no PLT available in JIT slab). */
    if (!PLATFORM_X86) return std::string();
    return IF(bb_glue_outer_whack(), bb_glue_framed_leave())
         + IF(MEDIUM_TEXT,   x86("add", "rsp", 24) + x86("xor", "edi", "edi") + x86("call", "exit@PLT"))
         + IF(MEDIUM_BINARY, x86("mov32", "eax", 1) + x86("ret"));
}
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_glue_outer_ω() {
    /* ONE-SHOT BRIDGE omega landing.  TEXT: exit(1).  BINARY: eax=99, ret. */
    if (!PLATFORM_X86) return std::string();
    return IF(bb_glue_outer_whack(), bb_glue_framed_leave())
         + IF(MEDIUM_TEXT,   x86("add", "rsp", 24) + x86("mov32", "edi", 1) + x86("call", "exit@PLT"))
         + IF(MEDIUM_BINARY, x86("mov32", "eax", 99) + x86("ret"));
}
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
