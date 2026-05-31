/* bb_list_bang.cpp — BB template for Icon `!E` (IR_LIST_BANG, IBB-12, 2026-05-29).
   The list/table/record/string element generator. pBB->α is the iterable expr.

   The flat driver (flat_drive_list_bang in emit_bb.c) walks pBB->α FIRST, so on the α (fresh)
   entry the runtime vstack holds the evaluated iterable. This template is the generator node:
   it has BOTH a live α entry (reset the generator, index 0) and a live β entry (resume, advance
   the index). Both call rt_list_bang with the three persistent per-node state-slot addresses
   (&pBB->value cached iterable, &pBB->counter index, &pBB->state phase) — stable emit-time
   addresses, exactly as bb_to/bb_initial use &pBB->counter — differing only in the reset flag
   (α: ecx=1, β: ecx=0). The helper pushes the yielded element + sets LAST_OK on a hit (returns 1),
   or pushes FAIL + clears LAST_OK on exhaustion (returns 0). The template tests rax: 1 → jmp γ
   (yield), 0 → jmp ω (exhausted). This is the standard four-port generator shape (cf. bb_to).

   SysV ABI arg registers: rdi=obj_slot, rsi=idx_slot, rdx=state_slot, ecx=reset.

   Bytes (122 total): α half [0..60], β half [61..121].
     α: movabs rdi,obj ; movabs rsi,idx ; movabs rdx,state ; mov ecx,1 ;
        movabs rax,&fn ; call rax ; test rax,rax ; jz ω ; jmp γ
     β: (lbl_β here) movabs rdi,obj ; movabs rsi,idx ; movabs rdx,state ; mov ecx,0 ;
        movabs rax,&fn ; call rax ; test rax,rax ; jz ω ; jmp γ
   Site offsets (ascending): 52 ω-patch, 57 γ-patch, 61 β-define, 113 ω-patch, 118 γ-patch. */
#include <string>
#include <string.h>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "descr.h"
int rt_list_bang(DESCR_t *obj_slot, int64_t *idx_slot, int *state_slot, int reset);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_list_bang_str(IR_t * pBB, bb_bin_t & bin) {
    bin = {};
    if (!PLATFORM_X86) return std::string();
    if (MEDIUM_MACRO_DEF) return s_comment("# no macro form — IR_LIST_BANG");
    if (!pBB) { fprintf(stderr, "[IBB] FATAL bb_list_bang: null node\n"); abort(); }
    uint64_t obj_slot   = (uint64_t)(uintptr_t)&pBB->value;
    uint64_t idx_slot   = (uint64_t)(uintptr_t)&pBB->counter;
    uint64_t state_slot = (uint64_t)(uintptr_t)&pBB->state;
    uint64_t fptr; { int (*fp)(DESCR_t*, int64_t*, int*, int) = rt_list_bang; fptr = (uint64_t)(uintptr_t)(void*)fp; }
    if (MEDIUM_TEXT) {
        return s_1asm(emit_fmt("%s:", _.lbl_α))
             + s_comment("# BOX IR_LIST_BANG (!E) [IBB-12 rt_list_bang]")
             + s_2asm("mov rdi,", emit_fmt("0x%lx", (unsigned long)obj_slot))
             + s_2asm("mov rsi,", emit_fmt("0x%lx", (unsigned long)idx_slot))
             + s_2asm("mov rdx,", emit_fmt("0x%lx", (unsigned long)state_slot))
             + s_2asm("mov ecx,", "1")
             + s_2asm("call", "rt_list_bang@PLT")
             + s_2asm("test", "rax, rax")
             + s_2asm("jz", _.lbl_ω)
             + s_2asm("jmp", _.lbl_γ)
             + s_L1asm(emit_fmt("%s:", _.lbl_β), "")
             + s_2asm("mov rdi,", emit_fmt("0x%lx", (unsigned long)obj_slot))
             + s_2asm("mov rsi,", emit_fmt("0x%lx", (unsigned long)idx_slot))
             + s_2asm("mov rdx,", emit_fmt("0x%lx", (unsigned long)state_slot))
             + s_2asm("mov ecx,", "0")
             + s_2asm("call", "rt_list_bang@PLT")
             + s_2asm("test", "rax, rax")
             + s_2asm("jz", _.lbl_ω)
             + s_2asm("jmp", _.lbl_γ);
    }
    if (MEDIUM_BINARY) {
        std::string b;
        /* α half */
        b += bytes(2, "\x48\xBF") + u64le(obj_slot);     /* movabs rdi, obj_slot   */
        b += bytes(2, "\x48\xBE") + u64le(idx_slot);     /* movabs rsi, idx_slot   */
        b += bytes(2, "\x48\xBA") + u64le(state_slot);   /* movabs rdx, state_slot */
        b += bytes(1, "\xB9")     + u32le(1u);           /* mov ecx, 1             */
        b += bytes(2, "\x48\xB8") + u64le(fptr);         /* movabs rax, &fn        */
        b += bytes(2, "\xFF\xD0");                       /* call rax               */
        b += bytes(3, "\x48\x85\xC0");                   /* test rax, rax          */
        b += bytes(2, "\x0F\x84") + u32le(0);            /* jz ω  (patch @52)      */
        b += bytes(1, "\xE9")     + u32le(0);            /* jmp γ (patch @57)      */
        /* β half — lbl_β defined at offset 61 */
        b += bytes(2, "\x48\xBF") + u64le(obj_slot);     /* movabs rdi, obj_slot   */
        b += bytes(2, "\x48\xBE") + u64le(idx_slot);     /* movabs rsi, idx_slot   */
        b += bytes(2, "\x48\xBA") + u64le(state_slot);   /* movabs rdx, state_slot */
        b += bytes(1, "\xB9")     + u32le(0u);           /* mov ecx, 0             */
        b += bytes(2, "\x48\xB8") + u64le(fptr);         /* movabs rax, &fn        */
        b += bytes(2, "\xFF\xD0");                       /* call rax               */
        b += bytes(3, "\x48\x85\xC0");                   /* test rax, rax          */
        b += bytes(2, "\x0F\x84") + u32le(0);            /* jz ω  (patch @113)     */
        b += bytes(1, "\xE9")     + u32le(0);            /* jmp γ (patch @118)     */
        bin = { {52, 57, 61, 113, 118},
                {_.lbl_ω_p, _.lbl_γ_p, _.lbl_β_p, _.lbl_ω_p, _.lbl_γ_p},
                {false, false, true, false, false} };
        return b;
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_list_bang(IR_t * pBB) { bb_bin_t bin; bb_emit_asm_result(bb_list_bang_str(pBB, bin), bin); }
