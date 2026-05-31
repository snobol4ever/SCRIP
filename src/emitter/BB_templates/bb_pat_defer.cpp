/* bb_pat_defer.cpp — BB_PAT_DEFER: runtime-resolved pattern-valued variable.
 * sval = varname; ival = 0 for direct (PAT in pattern pos), 1 for *V deref form.
 * α: call rt_defer_match(varname, ival, Δ) → new_Δ (≥0) or -1 (fail).
 * β: → ω directly (no retry — single-attempt like LIT).
 * X86 TEXT + BINARY arms; JVM/JS/NET/WASM stub. */
#include <string>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit_bb.h"
#include "emit.h"
}
extern "C" int rt_defer_match(const char *varname, int ival_flag, int cur_delta);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_pat_defer_str(IR_t * pBB, bb_bin_t & bin) {
    bin = {};
    if (PLATFORM_X86) {
        const char *vname = pBB->sval ? pBB->sval : "";
        const char *vname_label = emit_intern_str(vname);
        int ival = (int)pBB->ival;
        if (MEDIUM_BINARY) {
            /* SBL-DEFER-NESTED (2026-05-29 Opus 4.8): byte mirror of the MEDIUM_TEXT arm, flat-shaped
             * (Δ in [r10], rel32 → γ/ω; β-define then jmp ω) like bb_pat_pos / bb_lit. JIT-in-process:
             * bake &vname (pBB->sval, stable) and &rt_defer_match as imm64 (cf. bb_lit baking &memcmp).
             * rt_defer_match resolves the var and matches a string literal (strncmp, alignment-tolerant)
             * OR a sub-pattern via exec_stmt→bb_broker. The sub-pattern path makes nested calls that hit
             * aligned SSE stores, so the call site MUST be 16-byte aligned. This box runs in two contexts
             * with different entry alignment (flat slab vs brokered child via pre_build_children →
             * bb_build_brokered), so align rsp dynamically: stash Δ ptr + rbx (callee-saved, survives the
             * C call), force `and rsp,-16`, call, then restore rsp/rbx/r10. (Single `push r10` left rsp
             * mis-aligned in the brokered-child case → SIGSEGV on 068/117/143/150.)
             *   [0-9]   48 BF +u64(&vname)   movabs rdi, &vname
             *   [10-14] BE    +u32(ival)     mov    esi, ival
             *   [15-17] 41 8B 12             mov    edx, [r10]         (Δ = cur_delta)
             *   [18-27] 48 B8 +u64(&fn)      movabs rax, &rt_defer_match
             *   [28-29] 41 52                push   r10                (save Δ ptr)
             *   [30]    53                   push   rbx                (save callee-saved)
             *   [31-33] 48 89 E3             mov    rbx, rsp           (remember rsp)
             *   [34-37] 48 83 E4 F0          and    rsp, -16           (force 16-align)
             *   [38-39] FF D0                call   rax
             *   [40-42] 48 89 DC             mov    rsp, rbx           (restore rsp)
             *   [43]    5B                   pop    rbx
             *   [44-45] 41 5A                pop    r10                (restore Δ ptr)
             *   [46-47] 85 C0                test   eax, eax
             *   [48-49] 0F 88; [50-53] rel32 js     → ω                site 50
             *   [54-56] 41 89 02             mov    [r10], eax         (Δ = new delta)
             *   [57]    E9;     [58-61] rel32 jmp   → γ                site 58
             *   [62]    E9 (β-define here);  [63-66] rel32 jmp → ω     site 62 (is_def) / site 63 */
            bin = bb_bin_t{ {50, 58, 62, 63},
                            {_.lbl_ω_p, _.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p},
                            {false, false, true, false} };
            return bytes(2, "\x48\xBF") + u64le((uint64_t)(uintptr_t)vname)
                 + bytes(1, "\xBE") + u32le((uint32_t)ival)
                 + bytes(3, "\x41\x8B\x12")
                 + bytes(2, "\x48\xB8") + u64le((uint64_t)(uintptr_t)rt_defer_match)
                 + bytes(2, "\x41\x52")
                 + bytes(1, "\x53")
                 + bytes(3, "\x48\x89\xE3")
                 + bytes(4, "\x48\x83\xE4\xF0")
                 + bytes(2, "\xFF\xD0")
                 + bytes(3, "\x48\x89\xDC")
                 + bytes(1, "\x5B")
                 + bytes(2, "\x41\x5A")
                 + bytes(2, "\x85\xC0")
                 + bytes(2, "\x0F\x88") + u32le(0)
                 + bytes(3, "\x41\x89\x02")
                 + bytes(1, "\xE9") + u32le(0)
                 + bytes(1, "\xE9") + u32le(0);
        }
        if (MEDIUM_TEXT) {
            return s_1asm(std::string(_.lbl_α) + ":")
                 + s_comment(emit_fmt("# BOX DEFER(%s%s)", ival ? "*" : "", vname))
                 + s_2asm("lea", emit_fmt("rdi, [rip + %s]", vname_label ? vname_label : "??"))
                 + s_2asm("mov", emit_fmt("esi, %d", ival))
                 + s_2asm("mov", "edx, [r10]")
                 + s_2asm("push", "r10")
                 + s_2asm("call", "rt_defer_match@PLT")
                 + s_2asm("pop", "r10")
                 + s_2asm("test", "eax, eax")
                 + s_2asm("js", _.lbl_ω)
                 + s_2asm("mov", "[r10], eax")
                 + s_2asm("jmp", _.lbl_γ)
                 + s_1asm(std::string(_.lbl_β) + ":")
                 + s_2asm("jmp", _.lbl_ω);
        }
        return std::string();
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_pat_defer(IR_t * pBB) {
    bb_bin_t bin;
    bb_emit_asm_result(bb_pat_defer_str(pBB, bin), bin);
}
