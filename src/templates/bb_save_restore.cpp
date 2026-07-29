#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
void  rt_flat_wire_adopt(void *gw, void *ww, void *rsp, void *rbp);
void  rt_flat_wire_adopt_isle(void *gw, void *ww, void *rsp, void *rbp, void *r12v);
void *rt_flat_ret_snap(void);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* IR_SAVE_RESTORE — SN4-FLAT-PROC (s176) functional linkage family.  SPITBOL manual Ch.8: a DEFINE'd function saves the fname variable, formals, and locals on a pushdown stack at entry and restores
 * them when the body transfers to the reserved labels RETURN (yield fname's value), FRETURN (signal failure), or NRETURN (Ch.9: yield the NAME held in fname).  In the flat regime the body statements
 * live ONLY in the one main chain, so the return labels are program-wide FLOATER boxes and the save/restore protocol rides the pcall record; roles discriminate on op_ival:
 *   3 WIRE-ADOPT — first box of a DEFINE stub blob, runs right after the jmp-entry prologue: rbp = this activation's base, the prologue parked outside-γ at [rbp+kt-24], outside-ω at [rbp+kt-16],
 *     caller rbp at [rbp+kt-8] (xa_flat header contract), and blob-entry rsp = rbp+kt (pre-carve).  Marshal all four into rt_flat_wire_adopt, which writes the OPEN pcall record's wire quad — the way
 *     home from any depth.  γ continues to the IR_GOTO_DEFERRED entry transfer.
 *   1 RETURN floater / 2 FRETURN floater — reached as ordinary labels through the registry (any goto, incl. $-computed).  rt_flat_ret_snap PEEKS (never pops) the open record's wire quad into a static
 *     buffer {γ-wire@0, ω-wire@8, rsp@16, rbp@24} and returns its address; the tail restores rbp/rsp and jmps the port's wire with rax:rdx riding untouched, landing exactly on the wire the opener
 *     installed (rt_proc_enter's landings on the C path) — the landing's epilogue leaf then performs the pop + name-restore + result protocol VERBATIM, so semantics are byte-identical to the old
 *     extracted-body exit.  Discarded intermediate C frames (rt_chain_enter / rt_goto_transfer) hold only callee-saved pins that are global invariants in emitted code, so the wholesale rsp restore is
 *     sound.  Level-0 transfer and wire-less activations die loudly inside the leaf.
 *   0 site OPEN — reserved for the emitted flat call site (next slice); bombs if reached. */
std::string bb_save_restore() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    long role = (long)_.op_ival;
    if (role == 3) {
        int kt = g_emit.flat_frame_bytes;
        if (x86_zc_frame() != ZC_FRAME_RSP) return x86("comment", "IR_SAVE_RESTORE wire-adopt (island, Z4-7 slice 2): the JMP_NONRSP prologue parks the LOW header — outside-γ at [rsp+8], outside-ω at [rsp+16], caller zr at [rsp+24], zr = rsp+32, pad at [rsp+0] — so the reads mirror THAT geometry, not the RSP high header; caller rbp is LIVE (the island arm never clobbers it) and the saved caller r12 rides as the 5th marshal into the widened wire quad the floaters restore from")
             + x86_alpha()
             + x86("mov", "rdi", RDQ("rsp", 8))
             + x86("mov", "rsi", RDQ("rsp", 16))
             + x86("lea", "rdx", RDQ("rsp", kt))
             + x86("mov", "rcx", "rbp")
             + x86("mov", "r8", RDQ("rsp", 24))
             + x86_align_enter()
             + x86("call", "rt_flat_wire_adopt_isle", (uint64_t)(uintptr_t)(void *)rt_flat_wire_adopt_isle)
             + x86_align_leave()
             + x86_gamma();
        if (!emit_jmp_pin_rbp()) return x86("comment", "IR_SAVE_RESTORE wire-adopt (depth-static): header wires + entry rsp via rsp, caller rbp LIVE IN THE REGISTER -> open pcall record")   /* FLATDISP-7 (s194): the ungated prologue never saved or clobbered rbp, so the caller's value is still IN rbp (marshal it directly, no [kt-8] slot exists) and rsp == base here (wire-adopt is the first box after the prologue, pre-carve) — the rsp-relative reads address the same header bytes the pinned arm reads through rbp.  Same falsifiable tripwire as the epilogue arms. */
             + x86_alpha()
             + x86("mov", "rdi", RDQ("rsp", kt - 24))
             + x86("mov", "rsi", RDQ("rsp", kt - 16))
             + x86("lea", "rdx", RDQ("rsp", kt))
             + x86("mov", "rcx", "rbp")
             + x86_align_enter()
             + x86("call", "rt_flat_wire_adopt", (uint64_t)(uintptr_t)(void *)rt_flat_wire_adopt)
             + x86_align_leave()
             + x86_gamma();
        return x86("comment", "IR_SAVE_RESTORE wire-adopt: header wires + entry rsp + caller rbp -> open pcall record")
             + x86_alpha()
             + x86("mov", "rdi", RDQ("rbp", kt - 24))
             + x86("mov", "rsi", RDQ("rbp", kt - 16))
             + x86("lea", "rdx", RDQ("rbp", kt))
             + x86("mov", "rcx", RDQ("rbp", kt - 8))
             + x86_align_enter()
             + x86("call", "rt_flat_wire_adopt", (uint64_t)(uintptr_t)(void *)rt_flat_wire_adopt)
             + x86_align_leave()
             + x86_gamma();
    }
    if (role == 1 || role == 2) {
        return x86("comment", role == 1 ? "IR_SAVE_RESTORE RETURN floater: restore caller machine state, jmp gamma wire" : "IR_SAVE_RESTORE FRETURN floater: restore caller machine state, jmp omega wire")
             + x86_alpha()
             + x86_align_enter()
             + x86("call", "rt_flat_ret_snap", (uint64_t)(uintptr_t)(void *)rt_flat_ret_snap)
             + x86_align_leave()
             + x86("mov", "rcx", RDQ("rax", role == 1 ? 0 : 8))
             + x86("mov", "rbp", RDQ("rax", 24))
             + (x86_zc_frame() != ZC_FRAME_RSP ? x86("mov", "r12", RDQ("rax", 32)) : std::string())
             + x86("mov", "rsp", RDQ("rax", 16))
             + x86("jmp", "rcx");
    }
    return x86_bomb("IR_SAVE_RESTORE role 0 (emitted site OPEN) is a next-slice citizen — no producer should build it yet");
}
