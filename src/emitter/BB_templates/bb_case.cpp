/* bb_case.cpp — BB template glue for IR_CASE (Icon `case E of { K: V; ...; default: VD }`).
   IBB-9-CASE (Opus 4.8, 2026-05-29). x86 only — IS_JVM/JS/NET/WASM stub (RULES.md).

   JCON ir_a_Case + mode-2 reference (bb_exec.c IR_CASE): the selector is evaluated ONCE; each clause
   key is compared to it (numeric equality iff both are integers, else string equality); on the first
   match the clause value is evaluated and becomes the case result. A trailing lone clause body is the
   default. No backtracking — case is a bounded selector.

   Mode-3 transcription does NOT route the whole node through one template (the key/value clause bodies
   are separate sub-graphs the flat driver walks). Instead the driver (flat_drive_case in emit_bb.c)
   emits the clause bodies and interleaves two glue shapes from THIS file:

     bb_case_store(pBB)  — after the selector walk leaves its value on the vstack: pop it into the
                           per-node slot &pBB->value (rt_pop_store_descr). No branch; falls through.

     bb_case_gate(pBB)   — after a clause-key walk leaves the key on the vstack: rt_case_eq(&slot)
                           pops the key, compares to the stored selector, sets LAST_OK; then read
                           LAST_OK and `jne val_entry` (match → run that clause body). No match falls
                           through to the next clause's key walk. EMIT_PAIR_JMP[0] = val_entry.

   Both glue emits are FACT-compliant byte producers living in this template file; the driver calls them
   directly (not via walk_bb_node) because they are mid-node glue, not graph-node entries.
*/
#include <string>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "emit_bb.h"
void rt_pop_store_descr(void *slot);
void rt_case_eq(const void *slot);
int  rt_last_ok(void);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_case_store(IR_t * pBB) {
    if (!PLATFORM_X86 || !pBB) return;
    uint64_t slot = (uint64_t)(uintptr_t)&pBB->value;
    uint64_t fn;  { void (*fp)(void *) = rt_pop_store_descr; fn = (uint64_t)(uintptr_t)(void*)fp; }
    if (MEDIUM_TEXT) {
        std::string s = s_comment("# BOX IR_CASE store selector (rt_pop_store_descr)")
             + s_2asm("movabs rdi,", emit_fmt("%llu", (unsigned long long)slot))
             + s_2asm("movabs rax,", emit_fmt("%llu", (unsigned long long)fn))
             + s_2asm("call", "rax");
        emit_text_n(s.data(), s.size());
        return;
    }
    if (MEDIUM_BINARY) {
        bb_bin_t bin = {};
        std::string out = bytes(2, "\x48\xBF") + u64le(slot)
                        + bytes(2, "\x48\xB8") + u64le(fn)
                        + bytes(2, "\xFF\xD0");
        bb_emit_asm_result(out, bin);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_case_gate(IR_t * pBB) {
    if (!PLATFORM_X86 || !pBB) return;
    bb_label_t *val_entry = g_emit.xa_bb_emit_pair_n > 0 ? g_emit.xa_bb_emit_pair_jmp[0] : NULL;
    uint64_t slot = (uint64_t)(uintptr_t)&pBB->value;
    uint64_t fn_eq;  { void (*fp)(const void *) = rt_case_eq; fn_eq  = (uint64_t)(uintptr_t)(void*)fp; }
    uint64_t fn_lok; { int  (*fp)(void)         = rt_last_ok; fn_lok = (uint64_t)(uintptr_t)(void*)fp; }
    if (MEDIUM_TEXT) {
        std::string s = s_comment("# BOX IR_CASE clause gate (rt_case_eq + LAST_OK branch)")
             + s_2asm("movabs rdi,", emit_fmt("%llu", (unsigned long long)slot))
             + s_2asm("movabs rax,", emit_fmt("%llu", (unsigned long long)fn_eq))
             + s_2asm("call", "rax")
             + s_2asm("movabs rax,", emit_fmt("%llu", (unsigned long long)fn_lok))
             + s_2asm("call", "rax")
             + s_2asm("test", "eax, eax")
             + s_2asm("jne", val_entry ? val_entry->name : ".Lcase_noval");
        emit_text_n(s.data(), s.size());
        return;
    }
    if (MEDIUM_BINARY) {
        /*  off  bytes                          asm                                                    */
        /*   0   48 BF <u64 slot>               movabs rdi, &pBB->value                                */
        /*  10   48 B8 <u64 rt_case_eq>         movabs rax, &rt_case_eq                                */
        /*  20   FF D0                          call rax                                               */
        /*  22   48 B8 <u64 rt_last_ok>         movabs rax, &rt_last_ok                                */
        /*  32   FF D0                          call rax                                               */
        /*  34   85 C0                          test eax, eax                                          */
        /*  36   0F 85 <rel32 → val_entry>      jne val_entry      ← patch @ 38; fall through on miss  */
        /*  42   end                                                                                    */
        bb_bin_t bin = { {38}, {val_entry}, {false} };
        std::string out = bytes(2, "\x48\xBF") + u64le(slot)
                        + bytes(2, "\x48\xB8") + u64le(fn_eq)
                        + bytes(2, "\xFF\xD0")
                        + bytes(2, "\x48\xB8") + u64le(fn_lok)
                        + bytes(2, "\xFF\xD0")
                        + bytes(2, "\x85\xC0")
                        + bytes(2, "\x0F\x85") + u32le(0);
        bb_emit_asm_result(out, bin);
    }
}
