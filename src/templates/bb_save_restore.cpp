#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
#include "bb_templates.h"
#include "ab_abi.h"
#include "pin_va.h"
long  rt_proc_call_open_slim(const char *name, int np, int nargs);
int   bb_scc_probe(const char *fname, int nargs, int *np_out, int *nsave_out, int *gk_out, int *res_gk_out);
int   bb_scc_handoff_arm(void);
int   bb_scc_handoff_room(void);
void  bb_scc_handoff_pending_clear(void);
}
#include "x86_asm.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* IR_SAVE_RESTORE — SN4-FLAT-PROC (s176) functional linkage family.  SPITBOL manual Ch.8: a DEFINE'd function saves the fname variable, formals, and locals on a pushdown stack at entry and restores
 * them when the body transfers to the reserved labels RETURN (yield fname's value), FRETURN (signal failure), or NRETURN (Ch.9: yield the NAME held in fname).  In the flat regime the body statements
 * live ONLY in the one main chain, so the return labels are program-wide FLOATER boxes and the save/restore protocol rides the pcall record; roles discriminate on op_ival:
 *   3 WIRE-ADOPT — first box of a DEFINE stub blob, runs right after the jmp-entry prologue: ___ = this activation's base, the prologue parked outside-γ at [___+kt-24], outside-ω at [___+kt-16],
 *     caller ___ at [___+kt-8] (xa_flat header contract), and blob-entry rsp = ___+kt (pre-carve).  Marshal all four into rt_flat_wire_adopt, which writes the OPEN pcall record's wire quad — the way
 *     home from any depth.  γ continues to the IR_GOTO_DEFERRED entry transfer.
 *   1 RETURN floater / 2 FRETURN floater — reached as ordinary labels through the registry (any goto, incl. $-computed).  rt_flat_ret_snap PEEKS (never pops) the open record's wire quad into a static
 *     buffer {γ-wire@0, ω-wire@8, rsp@16, ___@24} and returns its address; the tail restores ___/rsp and jmps the port's wire with rax:rdx riding untouched, landing exactly on the wire the opener
 *     installed (rt_proc_enter's landings on the C path) — the landing's epilogue leaf then performs the pop + name-restore + result protocol VERBATIM, so semantics are byte-identical to the old
 *     extracted-body exit.  Discarded intermediate C frames (rt_chain_enter / rt_goto_transfer) hold only callee-saved pins that are global invariants in emitted code, so the wholesale rsp restore is
 *     sound.  Level-0 transfer and wire-less activations die loudly inside the leaf.
 *   0 site OPEN — reserved for the emitted flat call site (next slice); bombs if reached. */
std::string bb_save_restore() {
    x86_begin();
    if (!PLATFORM_X86) return std::string();
    long role = (long)_.op_ival;
    if (role == 3) {
        /* ⛔⭐⭐⭐ LON RULING s54 (in-chat, verbatim substance): "delete all of that global variables. GONE. DONE.
         * We do not do that here. We have registers R10 and R11 for linkage, and plan on using the stack to
         * store things, not global variables."  The rt_flat_wire_adopt call — which banked the wire quad into
         * the g_pcall global array — is DELETED.  Wires ride rΓ=r10 · rΩ=r11 (LADDER WREG assignment, locked
         * s12).  Entry-rsp banking moves to the SPINE (stack-resident record), not a parallel global — that
         * half is the next seat's WREG-3 shape; until it lands, RETURN-from-interior-growth is KNOWN BROKEN
         * (Lon: "worry not about breakage").  git revert is the undo. */
        /* ⭐ FUNCTION LINKAGE s55 (Lon in-chat): "Can you not use R10 and R11 for success and fail return address
         * for FUNCTION linkage?  Just like any BB BLOB interface."  YES — the call sites now speak r10/r11 DIRECTLY
         * (bb_glue_pass_wires_blob), so the rcx/rdx→r10/r11 adopt hop is DELETED.  This box emits nothing: the wire
         * pair arrives already seated.  rcx/rdx remain the chain contract (rt_chain_enter, EVAL/CODE) only. */
        return x86("comment", "IR_SAVE_RESTORE wire-adopt (s55: EMPTY — sites set r10/r11 directly, adopt hop deleted)")
             + x86_alpha()
             + x86_gamma();
    }
    if (role == 1 || role == 2 || role == -1 /* NRETURN */) {
        /* ⛔⭐⭐⭐ LON RULING s54: globals GONE.  The RT_AB_ANCHOR global read and the rt_flat_ret_snap
         * (g_pcall peek) legacy path are DELETED from the floaters.  RETURN/NRETURN = jmp r10 (γ wire);
         * FRETURN = jmp r11 (ω wire) — LADDER WREG blob-exit spelling, now the floater spelling too.
         * ⛔ KNOWN BROKEN until WREG-3 lands: (a) rsp is NOT restored to entry depth here — the FORTH
         * paired-release discipline must hold interior-to-exit, and defers that survive on the spine break
         * it; (b) r10/r11 are SysV caller-saved — every C crossing in the body clobbers them until the
         * per-activation spine capture (WREG-3's law: pendings capture {r10,r11} at push, β restores).
         * Lon: "worry not about breakage."  The save-set restore stays caller-side (role 0's own rsp slots
         * + the landing epilogue) — already stack-resident. */
        return x86("comment", role == 1 ? "IR_SAVE_RESTORE RETURN floater (s54: jmp r10, globals GONE)" :
                               role == 2 ? "IR_SAVE_RESTORE FRETURN floater (s54: jmp r11, globals GONE)" :
                                           "IR_SAVE_RESTORE NRETURN floater (s54: jmp r10, globals GONE)")
             + x86_alpha()
             + (role == 2 ? x86("jmp", "r11") : x86("jmp", "r10"));
    }
    /* role 0 — CALL2BB slice 2 (Lon s21x-c: "Have each BB allocate its RESULT value… its LOCAL STORAGE needs… by one instruction, decrement RSP"; "DEFINE, when CONSTANT FOLDED, emits exactly TWO BBs:
     * an IR_SAVE_RESTORE and an IR_CALL").  THE CALL-SITE SAVE/INSTALL BOX: this box's LOCALS are the save-set slots — carved by its OWN single `sub rsp` (never a whole-graph carve, never ___-indexed),
     * offsets slide from RSP.  Body = the BP-7 SCC prefix migrated out of bb_call_proc_staged: (1) spill the DEFINE save-set's old GVA cell values (fname/formals/locals per manual Ch.8 p.104 + Ch.19
     * DEFINE: "saved prior to function entry, and restored upon function return") into the own slots; (2) open_slim — ALL runtime guards re-checked before ANY side effect, so a 0 return needs nothing
     * undone (the spilled copies are pure reads, released by the decline's add); (3) committed: install the staged args into the formals' NV GLOBAL cells (SNOBOL4's one namespace — no ζ in the call
     * process, Lon SCOPE LAW).  The outcome rides rax into the adjacent staged call box (1 = record open + block LIVE across the edge for its landings to restore+release; 0 = declined + block released
     * → the call box's classic fallback at base depth).  Handoff via bb_scc_handoff_set = the emit-order contract (sr0.γ → call, chain-adjacent); non-eligible sites and any unresolved arg slot fall to
     * a pass-through so the call box keeps its full merged body — behavior identical to the gate-off world at those sites.  FRQB(slot, sb) self-compensates the live carve (the 083 FLATDISP-LIVE-BUMP
     * law); the fc_hit window arm lives inside FRQB, self-disabling here since DRIVE_FILL reset this node's grant fields. */
    int c2np = 0, c2nsave = 0, c2res_gk = -1; int c2gk[64]; long c2nargs = (long)_.op_arg_slot_n;
    int elig = bb_scc_probe(_.op_sval, (int)c2nargs, &c2np, &c2nsave, c2gk, &c2res_gk);
    for (int i = 0; elig && i < (int)c2nargs; i++) if (_.op_arg_slot[i] < 0) elig = 0;
    if (elig && !bb_scc_handoff_room()) elig = 0;   /* pending missing (drive shape surprise) or table full: decline safely — the call box keeps its merged body */
    if (!elig) { bb_scc_handoff_pending_clear(); return x86("comment", "IR_SAVE_RESTORE role 0: site not SCC-eligible (or an arg slot unresolved, or no handoff room) — pass-through; the staged call box runs its full merged body")
             + x86_alpha()
             + x86_gamma(); }
    long sb = 16L * (long)c2nsave;
    uint64_t slim_fp; { long (*fp)(const char *, int, int) = rt_proc_call_open_slim; slim_fp = (uint64_t)(uintptr_t)(void *)fp; }
    std::string s = x86("comment", "IR_SAVE_RESTORE role 0 (CALL2BB slice 2): own sub-rsp carve, save-set spill (GVA -> own slots), open_slim, staged args -> NV globals; outcome rides rax into the staged call box")
         + x86_alpha()
         + x86_zclaim(sb)
         + FOR(0, c2nsave, [&](int k) {
               return x86("note", gva_name(c2gk[k])) + x86("mov", "rax", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(c2gk[k], 0) : ABSQ(RT_GVA_VA + (unsigned long)c2gk[k] * 16)) + x86_rsp_store64(16 * k, "rax")
                    + x86("note", gva_name(c2gk[k])) + x86("mov", "rax", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(c2gk[k], 8) : ABSQ(RT_GVA_VA + (unsigned long)c2gk[k] * 16 + 8)) + x86_rsp_store64(16 * k + 8, "rax"); })
         + x86_scan_sync_out()
         + x86_ro_load_q("rdi", 0)
         + x86("mov32", "esi", (long)c2np)
         + x86("mov32", "edx", c2nargs)
         + x86("call", "rt_proc_call_open_slim", slim_fp)
         + x86_scan_sync_in_rr()
         + x86("test", "rax", "rax")
         + x86("jne", L(1))
         + x86_zrelease(sb)
         + x86("jmp", L(2))
         + x86("def", L(1))
         + FOR(0, (int)c2nargs, [&](int i) { int slot = _.op_arg_slot[i];
               return x86("mov", "rax", FRQB(slot, (int)sb))
                    + x86("note", gva_name(c2gk[i])) + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(c2gk[i], 0) : ABSQ(RT_GVA_VA + (unsigned long)c2gk[i] * 16), "rax")
                    + x86("mov", "rax", FRQB(slot + 8, (int)sb))
                    + x86("note", gva_name(c2gk[i])) + x86("mov", (g_rtcc_on && RTCC_GLOBAL_R9_GVA) ? GVARQ(c2gk[i], 8) : ABSQ(RT_GVA_VA + (unsigned long)c2gk[i] * 16 + 8), "rax"); })
         + x86("mov32", "eax", 1L)
         + x86("def", L(2))
         + x86_gamma()
         + x86_ro_seal_str(0, _.op_sval ? _.op_sval : "");
    if (!bb_scc_handoff_arm()) return x86_alpha() + x86_bomb("IR_SAVE_RESTORE role 0: handoff arm failed after the room check passed (impossible unless the drive deposit raced)");
    return s;
}
