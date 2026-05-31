/* bb_iterate.cpp — BB template for IR_ITERATE.

   ⛔ TWO PATHS, discriminated by pBB->sval at emit time:

   1. RAKU \x01-string-array iteration (sval != NULL):
        sval = iterable-array variable name (intern'd source string baked into
        an .asciz slot in .data). State in .data: counter (.quad, byte-offset
        of NEXT segment start). α: reset counter to 0; β: continue with the
        already-advanced counter.
        Each entry: NV_GET_fn(name) → DESCR_t in rax:rdx (SysV ABI struct ≤16);
        unpack slen (high 32 bits of rax) and base ptr (rdx); scan from counter
        for next \x01 (or end-of-string); yield substring via rt_push_str;
        advance counter past the \x01 separator. Exhaustion (counter ≥ slen) → ω.
        100% template emission per Lon's directive: no port-logic helpers; only
        conversion/effect helpers (NV_GET_fn, rt_push_str) via @PLT.
        RK-BB-3a (2026-05-27, Opus 4.7).

        ⛔ KNOWN ISSUE (RK-BB-3a partial): in the full mode-4 program the load
        gets slen=0 from NV_GET_fn — the variable lookup returns NULVCL even
        though `say(@arr)` (which uses rt_nv_get) sees the populated value.
        Isolated asm test (gcc-compiled standalone) with the SAME instruction
        sequence yields 3 segments correctly, so the asm logic is verified.
        The mode-4 path therefore is structurally correct but blocked on some
        wrapper-level issue (label resolution, section-switch fall-through, or
        scope mismatch between var stores and the BB body's NV_GET). Mode-2
        polymorphism is GREEN — see bb_exec.c IR_ITERATE.

   2. ICON !string char-by-char (legacy, sval == NULL):
        Compile-time hay/slen baked in; uses a static slots table; writes DT_S
        to r12 directly. DEAD CODE: Icon's !E now lowers via IR_LIST_BANG (not
        IR_ITERATE). Preserved verbatim for regression-safety only. NOT reached
        by any current frontend.

   FACT RULE: every byte emitted via s_* helpers — no seg_byte, SL_B,
   sl_emit_one, or emit_standard_blob. PEERS RULE: discriminator is sval
   presence (no new IR_t fields). */
#include <string>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
#include "emit.h"
#include "descr.h"
extern DESCR_t NV_GET_fn(const char *name);
extern void    rt_push_str(const char *s, uint32_t slen);
extern void *  GC_malloc(size_t n);
extern size_t  strlen(const char *);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_iterate_str(IR_t * pBB, bb_bin_t & bin) {
    bin = {};
    if (!PLATFORM_X86) return std::string();
    int id = bb_node_id(pBB);

    if (MEDIUM_MACRO_DEF) return s_comment("# no macro form — ITERATE");

    /* ── RAKU PATH ── pBB->sval = iterable-array variable name. */
    if (pBB->sval && *pBB->sval) {
        std::string name_lbl = emit_fmt(".Liter%d_name", id);
        std::string cnt_lbl  = emit_fmt(".Liter%d_cnt",  id);
        std::string load_lbl = emit_fmt(".Liter%d_load", id);
        std::string scan_lbl = emit_fmt(".Liter%d_scan", id);
        std::string send_lbl = emit_fmt(".Liter%d_send", id);
        std::string have_slen_lbl = emit_fmt(".Liter%d_have_slen", id);

        if (MEDIUM_TEXT) {
            /* .data slots: NUL-terminated var name + .quad counter. */
            std::string slots =
                  s_directive(".section .data")
                + s_directive(emit_fmt("%s: .asciz \"%s\"", name_lbl.c_str(), pBB->sval))
                + s_directive(cnt_lbl + ": .quad 0")
                + s_directive(".section .text")
                + s_directive(".intel_syntax noprefix");

            /* α: reset counter to 0, fall through to load. */
            std::string alpha =
                  s_1asm(emit_fmt("%s:", _.lbl_α))
                + s_2asm("lea rax,", "[rip + " + cnt_lbl + "]")
                + s_2asm("mov qword ptr [rax],", "0")
                + s_1asm("jmp " + load_lbl);

            /* β: counter is already advanced; fall through to load. */
            std::string beta =
                  s_L1asm(std::string(_.lbl_β) + ":", "");

            /* load: NV_GET_fn(name) → DESCR_t in rax:rdx; bounds-check counter. */
            std::string load =
                  s_L1asm(load_lbl + ":", "")
                + s_2asm("lea rdi,", "[rip + " + name_lbl + "]")
                + s_2asm("call", "NV_GET_fn@PLT")
                /* rax low32 = v, rax high32 = slen, rdx = s pointer */
                + s_2asm("mov rsi,", "rax")
                + s_2asm("shr rsi,", "32")              /* rsi = slen (zero-ext) */
                + s_2asm("mov r10,", "rdx")             /* r10 = base ptr (preserve across call) */
                /* slen-fallback (RK-BB-3a-mode4-debug, Opus 4.7, 2026-05-27):       */
                /* The codebase-wide convention is that DESCR_t.slen for DT_S        */
                /* strings is frequently left at 0 — every downstream consumer       */
                /* (say/write/elems/arr_get/...) recovers via strlen(s). STRVAL()    */
                /* in snobol4.h hard-codes .slen=0, and changing that touches every  */
                /* call site. So we honor the convention here: when slen==0 and the */
                /* base ptr is non-NULL, call strlen@PLT inline to recover the      */
                /* real length. 100% template emission — no rt_* helper.            */
                /* Stack discipline: rsp is 16-aligned at this point (main's        */
                /* push rbp + the previous call's caller-side alignment). push r10  */
                /* misaligns to 8; sub rsp, 8 restores 16 before the strlen call.   */
                + s_2asm("test esi,", "esi")            /* slen == 0? */
                + s_2asm("jnz",  have_slen_lbl)
                + s_2asm("test r10,", "r10")            /* base ptr NULL? */
                + s_2asm("jz",   have_slen_lbl)         /* keep slen=0; bounds will hit ω */
                + s_2asm("push", "r10")                 /* save base ptr (caller-saved) */
                + s_2asm("sub rsp,", "8")               /* re-align to 16 */
                + s_2asm("mov rdi,", "r10")             /* strlen(base ptr) */
                + s_2asm("call", "strlen@PLT")
                + s_2asm("mov rsi,", "rax")             /* rsi = real slen */
                + s_2asm("add rsp,", "8")
                + s_2asm("pop",  "r10")                 /* restore base ptr */
                + s_L1asm(have_slen_lbl + ":", "")
                + s_2asm("lea rax,", "[rip + " + cnt_lbl + "]")
                + s_2asm("mov rcx,", "[rax]")           /* rcx = counter */
                + s_2asm("cmp rcx,", "rsi")
                + s_2asm("jge", _.lbl_ω);                /* counter ≥ slen → exhaustion */

            /* scan: advance rcx until s[rcx] == \x01 or rcx == slen.            */
            /*       r9 = start of segment counter; rcx walks. */
            std::string scan =
                  s_2asm("mov r9,", "rcx")              /* r9 = seg start counter */
                + s_L1asm(scan_lbl + ":", "")
                + s_2asm("cmp rcx,", "rsi")
                + s_2asm("jge", send_lbl)               /* hit slen → end of last seg */
                + s_2asm("cmp byte ptr [r10 + rcx],", "1")  /* \x01 separator */
                + s_2asm("je",  send_lbl)
                + s_2asm("inc", "rcx")
                + s_1asm("jmp " + scan_lbl);

            /* send: extract NUL-terminated copy of segment; advance counter past   */
            /*       separator; push fresh DESCR via rt_push_str; jmp γ.            */
            /*                                                                       */
            /* Why the copy: downstream consumers (write/say, fputs, strchr-based    */
            /* scans) treat DT_S strings as NUL-terminated and IGNORE the slen       */
            /* field — STRVAL() macro hardcodes .slen=0 codebase-wide. If we push a  */
            /* mid-string ptr+len pair, write() prints all bytes from ptr to the     */
            /* nearest NUL (entire rest of the array). So we MUST hand downstream a  */
            /* freshly-allocated, NUL-terminated segment.                            */
            /*                                                                       */
            /* GC_malloc is a memory-conversion helper (already used throughout the  */
            /* runtime). rep movsb is purely inline — no PLT call, no port logic.    */
            /* Stack discipline: rsp is 16-aligned at send entry; 2 pushes preserve  */
            /* alignment across GC_malloc; 1 push + rep movsb + pop straddles the   */
            /* memcpy (no calls in between, so 8-misalign is harmless there).        */
            std::string send =
                  s_L1asm(send_lbl + ":", "")
                + s_2asm("mov r8,",  "rcx")             /* r8 = end counter */
                + s_2asm("sub r8,",  "r9")              /* r8 = seg_len */
                + s_2asm("lea r11,", "[r10 + r9]")      /* r11 = seg start ptr */
                /* Advance counter past the separator: end + 1. If end == slen,  */
                /* counter > slen → next call hits ω cleanly.                    */
                + s_2asm("inc",      "rcx")
                + s_2asm("lea rax,", "[rip + " + cnt_lbl + "]")
                + s_2asm("mov [rax],", "rcx")
                /* GC_malloc(seg_len + 1) → rax = fresh ptr. Preserve r8, r11. */
                + s_2asm("push", "r8")                  /* save seg_len */
                + s_2asm("push", "r11")                 /* save src ptr (16-aligned now) */
                + s_2asm("mov rdi,", "r8")
                + s_2asm("add rdi,", "1")               /* rdi = seg_len + 1 (NUL slot) */
                + s_2asm("call", "GC_malloc@PLT")       /* rax = new ptr */
                + s_2asm("pop",  "r11")                 /* restore src ptr */
                + s_2asm("pop",  "r8")                  /* restore seg_len */
                /* memcpy(rax, r11, r8) via rep movsb. rcx is the count register. */
                + s_2asm("mov rdi,", "rax")             /* dst */
                + s_2asm("mov rsi,", "r11")             /* src */
                + s_2asm("mov rcx,", "r8")              /* count */
                + s_2asm("push", "rax")                 /* save dst (rep clobbers rdi) */
                + s_1asm("rep movsb")
                + s_2asm("pop",  "rax")                 /* restore dst */
                /* NUL-terminate at [rax + r8]. */
                + s_2asm("mov byte ptr [rax + r8],", "0")
                /* rt_push_str(new_ptr, seg_len). */
                + s_2asm("mov rdi,", "rax")
                + s_2asm("mov rsi,", "r8")
                + s_2asm("call",     "rt_push_str@PLT")
                + s_2asm("jmp",      _.lbl_γ);

            return slots + alpha + beta + load + scan + send;
        }

        if (MEDIUM_BINARY) {
            /* M3-RK-NOINTERP-1c (Opus 4.7, 2026-05-29): wire MEDIUM_BINARY arm for Raku iterate.    */
            /* Mirrors the MEDIUM_TEXT arm above byte-for-byte, but emits raw x86 through bb_emit_   */
            /* asm_result so SM_BB_INVOKE's scratch-buffer-flush (sm_bb_invoke.cpp:48-185) can route */
            /* it into the mode-3 sm_run_native byte stream.  Closes Cluster 1 of MODE3-DISPATCH-    */
            /* GAP: for_array, for_array_simple, for_array_underscore, map_grep_sort24. */
            /*                                                                                       */
            /* Helper functions called via absolute movabs+call (no PLT in mode-3 native):           */
            /*   NV_GET_fn(name)  → DESCR_t in rax:rdx (low32=v, hi32=slen; rdx=base ptr)            */
            /*   strlen(s)        → rax = real length (slen-fallback when DESCR.slen==0)             */
            /*   GC_malloc(n)     → rax = fresh ptr                                                   */
            /*   rt_push_str(s,n) → push DESCR_t{DT_S,n,s} on value stack                            */
            /*                                                                                       */
            /* State slots:                                                                          */
            /*   &pBB->counter — uint64_t byte-offset into the SOH-joined array string (matches      */
            /*                   bb_to/bb_to_by counter idiom; valid in mode-3 since emitter+rt are  */
            /*                   the same process).                                                  */
            /*   pBB->sval     — interned C string of the iterable's variable name; also a same-     */
            /*                   process pointer in mode-3.                                          */
            /*                                                                                       */
            /* Internal control flow uses rel32 self-patches inside the buffer (mirroring bb_to.cpp  */
            /* alpha_jmp pattern but with the long form for safety across any future code growth).   */
            /* External labels routed via bin.sites:                                                  */
            /*   lβ — DEFINED at the entry of the load block (after α, β falls through).             */
            /*   lω — REFERENCED by the bounds-check jge after the load.                              */
            /*   lγ — REFERENCED by the final jmp after rt_push_str.                                  */
            /* bin.sites must be in ascending offset order (bb_emit_asm_result patch loop walks      */
            /* with strictly-advancing pos — see M3-RK-NOINTERP-1b post-mortem).                      */
            uint64_t cnt_addr        = (uint64_t)(uintptr_t)&pBB->counter;
            uint64_t name_addr       = (uint64_t)(uintptr_t)pBB->sval;
            uint64_t nv_get_addr;     { DESCR_t (*fp)(const char*) = NV_GET_fn;     nv_get_addr     = (uint64_t)(uintptr_t)(void*)fp; }
            uint64_t strlen_addr;     { size_t  (*fp)(const char*) = ::strlen;      strlen_addr     = (uint64_t)(uintptr_t)(void*)fp; }
            uint64_t gc_malloc_addr;  { void *  (*fp)(size_t)      = GC_malloc;     gc_malloc_addr  = (uint64_t)(uintptr_t)(void*)fp; }
            uint64_t rt_push_str_addr;{ void    (*fp)(const char*,uint32_t)=rt_push_str; rt_push_str_addr=(uint64_t)(uintptr_t)(void*)fp; }
            std::string b;
            /* ── α: reset &pBB->counter to 0; fall into load via rel32 jump. ── */
            /* movabs rax, &cnt                                  48 B8 [u64]                        */
            b += bytes(2,"\x48\xB8") + u64le(cnt_addr);
            /* mov qword ptr [rax], 0   (sign-ext imm32)         48 C7 00 00 00 00 00               */
            b += bytes(3,"\x48\xC7\x00") + u32le(0);
            /* jmp .Lload (rel32, self-patched after lβ is fixed)  E9 [rel32]                       */
            int alpha_jmp = (int)b.size();
            b += bytes(1,"\xE9") + u32le(0);
            /* ── β-define / .Lload entry ── (defined for the wrapper, fall-through to load).        */
            int beta_off = (int)b.size();
            /* movabs rdi, name                                  48 BF [u64]                         */
            b += bytes(2,"\x48\xBF") + u64le(name_addr);
            /* movabs rax, &NV_GET_fn ; call rax                 48 B8 [u64] ; FF D0                 */
            b += bytes(2,"\x48\xB8") + u64le(nv_get_addr);
            b += bytes(2,"\xFF\xD0");
            /* mov rsi, rax ; shr rsi, 32 ; mov r10, rdx        48 89 C6 ; 48 C1 EE 20 ; 49 89 D2  */
            b += bytes(3,"\x48\x89\xC6");
            b += bytes(4,"\x48\xC1\xEE\x20");
            b += bytes(3,"\x49\x89\xD2");
            /* test esi, esi                                     85 F6                                */
            b += bytes(2,"\x85\xF6");
            /* jnz .Lhaveslen (rel32, self-patched)              0F 85 [rel32]                       */
            int jnz_to_have = (int)b.size();
            b += bytes(2,"\x0F\x85") + u32le(0);
            /* test r10, r10                                     4D 85 D2                             */
            b += bytes(3,"\x4D\x85\xD2");
            /* jz .Lhaveslen (rel32, self-patched)               0F 84 [rel32]                       */
            int jz_to_have = (int)b.size();
            b += bytes(2,"\x0F\x84") + u32le(0);
            /* push r10 ; sub rsp, 8                             41 52 ; 48 83 EC 08                 */
            b += bytes(2,"\x41\x52");
            b += bytes(4,"\x48\x83\xEC\x08");
            /* mov rdi, r10                                      4C 89 D7                             */
            b += bytes(3,"\x4C\x89\xD7");
            /* movabs rax, strlen ; call rax                     48 B8 [u64] ; FF D0                 */
            b += bytes(2,"\x48\xB8") + u64le(strlen_addr);
            b += bytes(2,"\xFF\xD0");
            /* mov rsi, rax                                      48 89 C6                             */
            b += bytes(3,"\x48\x89\xC6");
            /* add rsp, 8 ; pop r10                              48 83 C4 08 ; 41 5A                 */
            b += bytes(4,"\x48\x83\xC4\x08");
            b += bytes(2,"\x41\x5A");
            /* .Lhaveslen: ← jnz/jz targets land here                                                 */
            int have_off = (int)b.size();
            ((char*)b.data())[jnz_to_have+2] = (char)((have_off - (jnz_to_have+6)) & 0xFF);
            ((char*)b.data())[jnz_to_have+3] = (char)(((have_off - (jnz_to_have+6)) >> 8 ) & 0xFF);
            ((char*)b.data())[jnz_to_have+4] = (char)(((have_off - (jnz_to_have+6)) >> 16) & 0xFF);
            ((char*)b.data())[jnz_to_have+5] = (char)(((have_off - (jnz_to_have+6)) >> 24) & 0xFF);
            ((char*)b.data())[jz_to_have+2 ] = (char)((have_off - (jz_to_have+6 )) & 0xFF);
            ((char*)b.data())[jz_to_have+3 ] = (char)(((have_off - (jz_to_have+6 )) >> 8 ) & 0xFF);
            ((char*)b.data())[jz_to_have+4 ] = (char)(((have_off - (jz_to_have+6 )) >> 16) & 0xFF);
            ((char*)b.data())[jz_to_have+5 ] = (char)(((have_off - (jz_to_have+6 )) >> 24) & 0xFF);
            /* movabs rax, &cnt ; mov rcx, [rax]                 48 B8 [u64] ; 48 8B 08              */
            b += bytes(2,"\x48\xB8") + u64le(cnt_addr);
            b += bytes(3,"\x48\x8B\x08");
            /* cmp rcx, rsi                                      48 39 F1                             */
            b += bytes(3,"\x48\x39\xF1");
            /* jge lω (rel32, bin.sites)                         0F 8D [rel32]                       */
            int fail_off = (int)b.size();
            b += bytes(2,"\x0F\x8D") + u32le(0);
            /* mov r9, rcx                                       49 89 C9                             */
            b += bytes(3,"\x49\x89\xC9");
            /* .Lscan: ← back-target for inc+jmp loop                                                 */
            int scan_off = (int)b.size();
            /* cmp rcx, rsi                                      48 39 F1                             */
            b += bytes(3,"\x48\x39\xF1");
            /* jge .Lsend (rel32, self-patched)                  0F 8D [rel32]                       */
            int jge_to_send = (int)b.size();
            b += bytes(2,"\x0F\x8D") + u32le(0);
            /* cmp byte ptr [r10 + rcx], 1                       41 80 3C 0A 01                       */
            b += bytes(5,"\x41\x80\x3C\x0A\x01");
            /* je .Lsend (rel32, self-patched)                   0F 84 [rel32]                       */
            int je_to_send = (int)b.size();
            b += bytes(2,"\x0F\x84") + u32le(0);
            /* inc rcx                                           48 FF C1                             */
            b += bytes(3,"\x48\xFF\xC1");
            /* jmp .Lscan (rel32)                                E9 [rel32]                          */
            {
                int here = (int)b.size();
                b += bytes(1,"\xE9") + u32le((uint32_t)(int32_t)(scan_off - (here + 5)));
            }
            /* .Lsend: ← jge/je targets land here                                                     */
            int send_off = (int)b.size();
            ((char*)b.data())[jge_to_send+2] = (char)((send_off - (jge_to_send+6)) & 0xFF);
            ((char*)b.data())[jge_to_send+3] = (char)(((send_off - (jge_to_send+6)) >> 8 ) & 0xFF);
            ((char*)b.data())[jge_to_send+4] = (char)(((send_off - (jge_to_send+6)) >> 16) & 0xFF);
            ((char*)b.data())[jge_to_send+5] = (char)(((send_off - (jge_to_send+6)) >> 24) & 0xFF);
            ((char*)b.data())[je_to_send+2 ] = (char)((send_off - (je_to_send+6 )) & 0xFF);
            ((char*)b.data())[je_to_send+3 ] = (char)(((send_off - (je_to_send+6 )) >> 8 ) & 0xFF);
            ((char*)b.data())[je_to_send+4 ] = (char)(((send_off - (je_to_send+6 )) >> 16) & 0xFF);
            ((char*)b.data())[je_to_send+5 ] = (char)(((send_off - (je_to_send+6 )) >> 24) & 0xFF);
            /* mov r8, rcx ; sub r8, r9                          49 89 C8 ; 4D 29 C8                  */
            b += bytes(3,"\x49\x89\xC8");
            b += bytes(3,"\x4D\x29\xC8");
            /* lea r11, [r10 + r9]                               4F 8D 1C 0A                          */
            b += bytes(4,"\x4F\x8D\x1C\x0A");
            /* inc rcx ; movabs rax,&cnt ; mov [rax], rcx       48 FF C1 ; 48 B8 [u64] ; 48 89 08    */
            b += bytes(3,"\x48\xFF\xC1");
            b += bytes(2,"\x48\xB8") + u64le(cnt_addr);
            b += bytes(3,"\x48\x89\x08");
            /* push r8 ; push r11                                41 50 ; 41 53                        */
            b += bytes(2,"\x41\x50");
            b += bytes(2,"\x41\x53");
            /* mov rdi, r8 ; add rdi, 1                          4C 89 C7 ; 48 83 C7 01               */
            b += bytes(3,"\x4C\x89\xC7");
            b += bytes(4,"\x48\x83\xC7\x01");
            /* movabs rax, GC_malloc ; call rax                  48 B8 [u64] ; FF D0                  */
            b += bytes(2,"\x48\xB8") + u64le(gc_malloc_addr);
            b += bytes(2,"\xFF\xD0");
            /* pop r11 ; pop r8                                  41 5B ; 41 58                        */
            b += bytes(2,"\x41\x5B");
            b += bytes(2,"\x41\x58");
            /* mov rdi, rax ; mov rsi, r11 ; mov rcx, r8        48 89 C7 ; 4C 89 DE ; 4C 89 C1       */
            b += bytes(3,"\x48\x89\xC7");
            b += bytes(3,"\x4C\x89\xDE");
            b += bytes(3,"\x4C\x89\xC1");
            /* push rax ; rep movsb ; pop rax                    50 ; F3 A4 ; 58                      */
            b += bytes(1,"\x50");
            b += bytes(2,"\xF3\xA4");
            b += bytes(1,"\x58");
            /* mov byte ptr [rax + r8], 0                        42 C6 04 00 00                        */
            b += bytes(5,"\x42\xC6\x04\x00\x00");
            /* mov rdi, rax ; mov rsi, r8                        48 89 C7 ; 4C 89 C6                  */
            b += bytes(3,"\x48\x89\xC7");
            b += bytes(3,"\x4C\x89\xC6");
            /* movabs rax, rt_push_str ; call rax                48 B8 [u64] ; FF D0                  */
            b += bytes(2,"\x48\xB8") + u64le(rt_push_str_addr);
            b += bytes(2,"\xFF\xD0");
            /* jmp lγ (rel32, bin.sites)                         E9 [rel32]                            */
            int succ_off = (int)b.size();
            b += bytes(1,"\xE9") + u32le(0);
            /* ── α self-patch: fix `jmp .Lload` rel32 (alpha_jmp+1..+4) to (beta_off - (alpha_jmp+5)). ── */
            {
                int32_t rel = (int32_t)(beta_off - (alpha_jmp + 5));
                ((char*)b.data())[alpha_jmp+1] = (char)( rel        & 0xFF);
                ((char*)b.data())[alpha_jmp+2] = (char)((rel >> 8 ) & 0xFF);
                ((char*)b.data())[alpha_jmp+3] = (char)((rel >> 16) & 0xFF);
                ((char*)b.data())[alpha_jmp+4] = (char)((rel >> 24) & 0xFF);
            }
            /* bin.sites ascending: beta_off (define lβ), fail_off+2 (rel32 → lω), succ_off+1 (rel32 → lγ). */
            bin = { {beta_off, fail_off+2, succ_off+1},
                    {_.lbl_β_p, _.lbl_ω_p, _.lbl_γ_p},
                    {true, false, false} };
            return b;
        }
        return std::string();
    }

    /* ── LEGACY ICON PATH (sval == NULL) ── compile-time hay/slen.   */
    /* DEAD CODE: kept verbatim for shape compatibility; Icon !E lowers via      */
    /* IR_LIST_BANG. The MEDIUM_TEXT "inc qword ptr [rax]," and MEDIUM_BINARY     */
    /* r12-writes are known broken — preserved for now. See file header. */
    {
        const char *hay = "";  /* TODO: read from runtime scan subject */
        int64_t    slen = pBB->ival;
        std::string cnt  = emit_fmt(".Liter%d_c", id);
        std::string chk  = emit_fmt(".Liter%d_chk", id);
        std::string back = std::string(_.lbl_β);
        if (MEDIUM_BINARY) {
            uint64_t cnt_addr = (uint64_t)(uintptr_t)&pBB->counter;
            uint64_t hay_     = (uint64_t)(uintptr_t)hay;
            std::string b;
            b += bytes(2,"\x48\xB9") + u64le(cnt_addr);
            b += bytes(3,"\x48\xC7\x01") + u32le(0);
            int jmp_chk = (int)b.size();
            b += bytes(2,"\xEB\x00");
            int back_off = (int)b.size();
            b += bytes(2,"\x48\xB9") + u64le(cnt_addr);
            b += bytes(3,"\x48\xFF\x01");
            int chk_off = (int)b.size();
            ((char*)b.data())[jmp_chk+1] = (char)(chk_off-(jmp_chk+2));
            b += bytes(2,"\x48\xB9") + u64le(cnt_addr);
            b += bytes(3,"\x48\x8B\x09");
            b += bytes(2,"\x48\xBF") + u64le((uint64_t)slen);
            b += bytes(3,"\x48\x39\xF9");
            int fail_off = (int)b.size();
            b += bytes(6,"\x0F\x8D\x00\x00\x00\x00");
            b += bytes(2,"\x48\xB8") + u64le(hay_);
            b += bytes(4,"\x48\x8D\x04\x08");
            b += bytes(4,"\x41\xC7\x04\x24") + u32le(1);
            b += bytes(5,"\x41\xC7\x44\x24\x04") + u32le(1);
            b += bytes(5,"\x49\x89\x44\x24\x08");
            b += bytes(4,"\x49\x83\xC4\x10");
            int succ_off = (int)b.size();
            b += bytes(5,"\xE9\x00\x00\x00\x00");
            bin = { {fail_off+2, succ_off+1, back_off},
                    {_.lbl_ω_p, _.lbl_γ_p, _.lbl_β_p},
                    {false, false, true} };
            return b;
        }
        if (MEDIUM_TEXT) {
            std::string slots;
            slots += s_directive(".section .data");
            slots += s_directive(emit_fmt(".Liter%d_slots:", id));
            for (int64_t k = 0; k < slen; k++) {
                unsigned char c = (unsigned char)hay[k];
                slots += s_directive(emit_fmt(".byte 0x%02x, 0", (int)c));
            }
            slots += s_directive(cnt + ": .quad 0");
            slots += s_directive(".section .text");
            slots += s_directive(".intel_syntax noprefix");
            std::string alpha =
                  s_1asm(emit_fmt("%s:", _.lbl_α))
                + s_2asm("lea rax,", "[rip + " + cnt + "]")
                + s_2asm("mov qword ptr [rax],", "0")
                + s_1asm("jmp " + chk);
            /* PRESERVED: the malformed "inc qword ptr [rax]," still here — DEAD */
            std::string beta =
                  s_L1asm(back + ":", "")
                + s_2asm("lea rax,", "[rip + " + cnt + "]")
                + s_2asm("inc qword ptr [rax],", "");
            std::string body =
                  s_L1asm(chk + ":", "")
                + s_2asm("lea rax,", "[rip + " + cnt + "]")
                + s_2asm("mov rcx,", "[rax]")
                + s_2asm("cmp rcx,", emit_fmt("%ld", (long)slen))
                + s_2asm("jge", _.lbl_ω)
                + s_2asm("lea rax,", emit_fmt("[rip + .Liter%d_slots]", id))
                + s_2asm("lea rdi,", "[rax + rcx*2]")
                + s_2asm("mov eax,", "0x00010001")
                + s_2asm("mov dword ptr [r12],", "1")
                + s_2asm("mov dword ptr [r12+4],", "1")
                + s_2asm("mov [r12+8],", "rdi")
                + s_2asm("add", "r12, 16")
                + s_2asm("jmp", _.lbl_γ);
            return slots + alpha + beta + body;
        }
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_iterate(IR_t * pBB) {
    bb_bin_t bin;
    bb_emit_asm_result(bb_iterate_str(pBB, bin), bin);
}
