/* bb_limit.cpp — BB template glue for BB_LIMIT (Icon `gen \ N` generator limitation).
   IBB-LIMIT (2026-05-29). x86 only — IS_JVM/JS/NET/WASM stub (RULES.md).

   JCON ir_a_Limit + mode-2 reference (bb_exec.c BB_LIMIT): the limit count is evaluated ONCE and
   cached; the wrapped generator (pBB->α) is pumped up to N times, each yield bumping a counter; when
   the counter reaches N OR the generator fails, the limitation fails. No value is produced by this
   node directly — the yielded value flows from the wrapped generator.

   Mode-3 transcription does NOT route the whole node through one template (the wrapped generator is a
   separate sub-graph the flat driver walks). The driver (flat_drive_limit in emit_bb.c) walks the
   count expr and the generator, interleaving three glue shapes from THIS file:

     bb_limit_begin(pBB) — after the count-expr walk leaves its value on the vstack: rt_limit_begin
                           pops it, caches the int-coerced max in &pBB->value, zeroes &pBB->counter, and
                           returns 1 iff max>0. `jz ω` (max<=0 → the limit yields nothing); else falls
                           through to the wrapped generator's fresh entry. EMIT_PAIR_JMP[0] = ω.
     bb_limit_inc(pBB)   — after the generator yields (value on the vstack): rt_limit_inc bumps the
                           counter, then `jmp γ` (yield the value). EMIT_PAIR_JMP[0] = γ.
     bb_limit_more(pBB)  — at the resume (β) entry: rt_limit_more tests counter<max; `jz ω`
                           (limit reached → fail); else `jmp gen_resume` (re-pump the generator).
                           EMIT_PAIR_JMP[0] = ω, EMIT_PAIR_JMP[1] = gen_resume.

   All three are FACT-compliant byte producers living in this template file; the driver calls them
   directly (mid-node glue, not graph-node entries) — same precedent as bb_case_store / bb_case_gate.   */
#include <string>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "emit_bb.h"
int rt_limit_begin(DESCR_t *max_slot, int64_t *count_slot);
int rt_limit_more(DESCR_t *max_slot, int64_t *count_slot);
int rt_limit_inc(int64_t *count_slot);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_limit_begin(IR_t * pBB) {
    if (!PLATFORM_X86 || !pBB) return;
    bb_label_t *lbl_ω = g_emit.xa_bb_emit_pair_n > 0 ? g_emit.xa_bb_emit_pair_jmp[0] : NULL;
    uint64_t max_slot   = (uint64_t)(uintptr_t)&pBB->value;
    uint64_t count_slot = (uint64_t)(uintptr_t)&pBB->counter;
    uint64_t fn; { int (*fp)(DESCR_t*, int64_t*) = rt_limit_begin; fn = (uint64_t)(uintptr_t)(void*)fp; }
    if (MEDIUM_TEXT) {
        std::string s = s_comment("# BOX BB_LIMIT begin (rt_limit_begin: cache max, count=0)")
             + s_2asm("movabs rdi,", emit_fmt("%llu", (unsigned long long)max_slot))
             + s_2asm("movabs rsi,", emit_fmt("%llu", (unsigned long long)count_slot))
             + s_2asm("movabs rax,", emit_fmt("%llu", (unsigned long long)fn))
             + s_2asm("call", "rax")
             + s_2asm("test", "rax, rax")
             + s_2asm("jz", lbl_ω ? lbl_ω->name : ".Llimit_noomega");
        emit_text_n(s.data(), s.size());
        return;
    }
    if (MEDIUM_BINARY) {
        /*  off  bytes                       asm                                       */
        /*   0   48 BF <u64 max_slot>        movabs rdi, &pBB->value                    */
        /*  10   48 BE <u64 count_slot>      movabs rsi, &pBB->counter                  */
        /*  20   48 B8 <u64 fn>              movabs rax, &rt_limit_begin            */
        /*  30   FF D0                       call rax                                   */
        /*  32   48 85 C0                    test rax, rax                              */
        /*  35   0F 84 <rel32 → ω>           jz ω      (patch @37; fall through on hit) */
        bb_bin_t bin = { {37}, {lbl_ω}, {false} };
        std::string out = bytes(2, "\x48\xBF") + u64le(max_slot)
                        + bytes(2, "\x48\xBE") + u64le(count_slot)
                        + bytes(2, "\x48\xB8") + u64le(fn)
                        + bytes(2, "\xFF\xD0")
                        + bytes(3, "\x48\x85\xC0")
                        + bytes(2, "\x0F\x84") + u32le(0);
        bb_emit_asm_result(out, bin);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_limit_inc(IR_t * pBB) {
    if (!PLATFORM_X86 || !pBB) return;
    bb_label_t *lbl_γ = g_emit.xa_bb_emit_pair_n > 0 ? g_emit.xa_bb_emit_pair_jmp[0] : NULL;
    uint64_t count_slot = (uint64_t)(uintptr_t)&pBB->counter;
    uint64_t fn; { int (*fp)(int64_t*) = rt_limit_inc; fn = (uint64_t)(uintptr_t)(void*)fp; }
    if (MEDIUM_TEXT) {
        std::string s = s_comment("# BOX BB_LIMIT inc (rt_limit_inc: count++) + jmp γ")
             + s_2asm("movabs rdi,", emit_fmt("%llu", (unsigned long long)count_slot))
             + s_2asm("movabs rax,", emit_fmt("%llu", (unsigned long long)fn))
             + s_2asm("call", "rax")
             + s_2asm("jmp", lbl_γ ? lbl_γ->name : ".Llimit_nogamma");
        emit_text_n(s.data(), s.size());
        return;
    }
    if (MEDIUM_BINARY) {
        /*   0   48 BF <u64 count_slot>      movabs rdi, &pBB->counter        */
        /*  10   48 B8 <u64 fn>              movabs rax, &rt_limit_inc    */
        /*  20   FF D0                       call rax                         */
        /*  22   E9 <rel32 → γ>              jmp γ   (patch @23)              */
        bb_bin_t bin = { {23}, {lbl_γ}, {false} };
        std::string out = bytes(2, "\x48\xBF") + u64le(count_slot)
                        + bytes(2, "\x48\xB8") + u64le(fn)
                        + bytes(2, "\xFF\xD0")
                        + bytes(1, "\xE9") + u32le(0);
        bb_emit_asm_result(out, bin);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_limit_more(IR_t * pBB) {
    if (!PLATFORM_X86 || !pBB) return;
    bb_label_t *lbl_ω      = g_emit.xa_bb_emit_pair_n > 0 ? g_emit.xa_bb_emit_pair_jmp[0] : NULL;
    bb_label_t *gen_resume = g_emit.xa_bb_emit_pair_n > 1 ? g_emit.xa_bb_emit_pair_jmp[1] : NULL;
    uint64_t max_slot   = (uint64_t)(uintptr_t)&pBB->value;
    uint64_t count_slot = (uint64_t)(uintptr_t)&pBB->counter;
    uint64_t fn; { int (*fp)(DESCR_t*, int64_t*) = rt_limit_more; fn = (uint64_t)(uintptr_t)(void*)fp; }
    if (MEDIUM_TEXT) {
        std::string s = s_comment("# BOX BB_LIMIT more (rt_limit_more: counter<max?) jz ω; jmp gen_resume")
             + s_2asm("movabs rdi,", emit_fmt("%llu", (unsigned long long)max_slot))
             + s_2asm("movabs rsi,", emit_fmt("%llu", (unsigned long long)count_slot))
             + s_2asm("movabs rax,", emit_fmt("%llu", (unsigned long long)fn))
             + s_2asm("call", "rax")
             + s_2asm("test", "rax, rax")
             + s_2asm("jz", lbl_ω ? lbl_ω->name : ".Llimit_noomega")
             + s_2asm("jmp", gen_resume ? gen_resume->name : ".Llimit_nogenresume");
        emit_text_n(s.data(), s.size());
        return;
    }
    if (MEDIUM_BINARY) {
        /*   0   48 BF <u64 max_slot>        movabs rdi, &pBB->value           */
        /*  10   48 BE <u64 count_slot>      movabs rsi, &pBB->counter         */
        /*  20   48 B8 <u64 fn>              movabs rax, &rt_limit_more    */
        /*  30   FF D0                       call rax                          */
        /*  32   48 85 C0                    test rax, rax                     */
        /*  35   0F 84 <rel32 → ω>           jz ω           (patch @37)        */
        /*  41   E9 <rel32 → gen_resume>     jmp gen_resume (patch @42)        */
        bb_bin_t bin = { {37, 42}, {lbl_ω, gen_resume}, {false, false} };
        std::string out = bytes(2, "\x48\xBF") + u64le(max_slot)
                        + bytes(2, "\x48\xBE") + u64le(count_slot)
                        + bytes(2, "\x48\xB8") + u64le(fn)
                        + bytes(2, "\xFF\xD0")
                        + bytes(3, "\x48\x85\xC0")
                        + bytes(2, "\x0F\x84") + u32le(0)
                        + bytes(1, "\xE9") + u32le(0);
        bb_emit_asm_result(out, bin);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Legacy node-entry symbol (BB_LIMIT was previously a stub routed from emit_core.c). The flat driver  */
/* now owns all control flow; this remains only so the emit_core dispatch table links. It must never   */
/* be reached for an Icon limit (the driver handles it) — abort loudly if it is.                        */
extern "C" void bb_limit(IR_t * pBB) {
    (void)pBB;
    fprintf(stderr, "[IBB] FATAL bb_limit: node-entry template reached — BB_LIMIT must route through flat_drive_limit\n");
    abort();
}
