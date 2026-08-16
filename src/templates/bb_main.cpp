#include <string>
#include <cstdint>
#include "emit.h"
extern "C" {
#include "bb_template_common.h"
}
#include "x86_asm.h"
#include "bb_templates.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* bb_main — THE MAIN TOP SECTION (Lon s122: "create a BB template, bb_main.cpp, for that top section, i.e. main four ports and the return stubs").  The four CLASS-O ports of the outer main graph are STATIC: β = jmp ω, γ = exit 0, ω = exit 1.  Nothing in them depends on the body, so they belong beside main, not stranded 900 lines below it.  ONE AUTHORITY: the port BODIES live here; the label DEFINES stay on the emit side because x86("label",..) is TEXT-ONLY BY CONSTRUCTION (x86_asm.h:1705 returns the empty string under MEDIUM_BINARY) and a binary port label must be BOUND to a real offset through emit_label_define_bb — a string-returning template cannot bind one.  This file therefore carries ZERO MEDIUM_* of its own and delegates the medium-split γ/ω bodies to the existing bb_glue_outer_γ/ω twins, which already resolve exit@PLT vs absolute-call internally. */
/* ⛔⭐⭐⭐ THE ENTRY BRIDGE IS THE WHOLE RUNG, AND IT IS WHY s118 FAILED TWICE.  emit_chain's BINARY return is `return (bb_box_fn)buf` (emit.cpp:3413) — in mode 3 THE CHAIN'S ENTRY POINT IS THE FIRST BYTE EMITTED.  There is no `main:` symbol in binary at all (that is emit_textf on the m4 path only) and lbl_α is defined ONLY under `text_externalise && g_is_text` (emit.cpp:2730).  So hoisting the ports above α makes entry land on main_β → jmp main_ω → exit(1), and the program reads rc=1 having executed no statement.  That is EXACTLY the s118 ports-only signature ("fibonacci m3 itself rc=1", FINDING-2026-08-16-s118-top-ports.md), and it is why the s118 FULL variant — which carried a one-jmp α bridge — passed fibonacci while ports-only did not.  The FINDING's three suspects are ALL TEXT-medium artifacts (x86_begin mints uids only under !MEDIUM_BINARY, x86_asm.h:926; g_emit_pos moves only under g_is_text, emit.cpp:2733) and NONE can reach a binary run: the two variants were never two interleaved mechanisms, they were ONE mechanism plus ONE accident.  bb_main_entry_bridge is that bridge, made explicit and unconditional. */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_main_entry_bridge() {
    if (!PLATFORM_X86) return std::string();
    return x86("comment", "bb_main ENTRY BRIDGE: binary chain entry IS the buffer base (emit_chain returns (bb_box_fn)buf) — the hoisted ports must never become the entry point")
         + x86_jmp_ext(_.flat_main_body_p);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_main_β() {
    if (!PLATFORM_X86) return std::string();
    return x86_jmp_ext(_.flat_main_ω_p);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ⭐⭐ FLOATERS INTO THE TOP SECTION (s123, the s122 cursor's NEXT RUNG (a)).  The three program-wide singleton landings — RETURN / FRETURN / NRETURN — are STATIC STUBS: every byte they emit is a function of fnrbp() and their own staged ports, and NOT ONE BYTE reads the node they were minted from (bb_define_sr roles 1/2 = x86_alpha + the record pop + jmp rcx; bb_nreturn_mark = x86_alpha + the by-name mark + x86_gamma).  They are therefore emissible from anywhere, and they belong at the top beside main with the other landings rather than stranded 1100 lines below the statement that jumps to them.  ⛔ THE POINT OF THE RUNG IS THE DRIVE LOOP, NOT THE LAYOUT: driving a floater made it a participant in the loop's emission-ORDER staging accumulators, and zd_sr_role claims the SAME ival values 1/2 the loop reads — that coupling, named in the s122 cursor, is what broke cap_imm_nret under the s118 full variant.  A node that never enters the loop cannot perturb its accumulators, so hoisting DISSOLVES the coupling by construction instead of ordering around it.  ONE AUTHORITY: the BODIES stay in bb_define.cpp — this file composes the top section, it does not own a second copy of a floater (the bb_glue_outer_γ/ω delegation precedent, same file, same reason).  Kind 3 is NRETURN, which is NOT bb_define role 3 (wire-adopt) — the two numberings are unrelated and the emit-side staging keeps them apart. */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_main_floater(int kind) {
    if (!PLATFORM_X86) return std::string();
    return (kind == 3) ? bb_nreturn_mark() : bb_define();
}
