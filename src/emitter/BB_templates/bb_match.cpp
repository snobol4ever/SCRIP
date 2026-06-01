/* bb_match.cpp — STACKLESS BB template for the SNOBOL4 pattern-match MATCH phase (IR_PAT_MATCH).
   PB-RB-3 (GOAL-SNOBOL4-BB CORRECTED PATTERN ARCHITECTURE, 2026-06-01, Opus 4.8). Phase 3 of the five-phase
   `SUBJ ? PAT [= REPL]` native model: BB_MATCH DRIVES the pattern element graph over Σ/δ/Δ with the SPITBOL
   Manual ch.18 unanchored OUTER start-loop. INLINE-JUMP model (Lon directive 2026-06-01, verbatim: "BB_MATCH
   would jump in and be jumped back into from the PATTERNS BB. Jump to box's alpha, return from box's omega") —
   NO C call, the proven combinator mechanism (walk_bb_flat port-threading, as XCAT/XALT already use). This box
   is entered by a JUMP and threads control by JUMP, never a (ζ,int entry) C call (NO C BYRD-BOX FUNCTIONS FACT
   RULE).

   CONTROL-FLOW CONTRACT (the four ports, SPITBOL Manual ch.18 algorithm steps 1-6):
     α (entry): the OUTER start-loop prologue. Establish the legacy scan globals the element matchers read —
       Σ (base ptr) and Σlen (length) from the SUBJECT box's ζ-frame slot (PB-0 stored base at [r12+subj],
       len at [r12+subj+8]); re-establish r10 = &Δ (the cursor cell every pattern element reads via [r10];
       SUBJECT's rt_sno_subject_load C-call clobbered r10, SysV caller-saved); seed the OUTER start cursor
       (a ζ-slot) to 0 (ch.18 step 1). NOTE: this is the LEGACY subject model (Σ/Σlen globals, cursor in Δ
       via [r10]); the ratified Σ=R13/δ=R14/Δ=R15 migration is the SEPARATE REG ladder — until REG-1 migrates
       bb_lit, MATCH populates the legacy cells the un-migrated element still reads.
     match_retry (internal): Δ = start (mov [r10], start), then JUMP into the element's α (elem_entry, the
       driver-defined label immediately preceding the inline element). The element matches at cursor Δ.
     element γ (match success) = the chain success continuation (lbl_γ): the element advanced Δ past its
       match; control leaves the whole MATCH for the statement's success path. (Span recording is deferred —
       this first cut proves the match SUCCEEDS; the matched span lives implicitly in [start, Δ].)
     element ω (no match at this start) = match_advance: SPITBOL ch.18 step 6 — advance the starting cursor
       by one; if start <= Σlen AND &ANCHOR is zero, re-enter the element (jmp match_retry); else (anchored,
       or start exhausted) the entire match has failed → jmp lbl_ω.
     β (resume): statement-level single-shot — a `SUBJ ? PAT` match does not re-offer once the statement
       consumed it (bounded; β = jmp ω). Within-pattern backtracking is the element boxes' OWN β/ω, not this
       box's; the OUTER unanchored advance is match_advance above.

   PER-BOX LOCAL STORAGE / NO-VALUE-STACK FACT RULES: the OUTER start cursor is a RW ζ-slot [r12+off]
   (bb_slot_alloc); Σ/Σlen/Δ are the legacy scan globals (RO-addressed by movabs to their fixed addresses, the
   SAME cells bb_lit reads). NO PATND_t, NO tree_t, NO value stack, NO ring. The element is reached by JUMP
   (jmp elem_entry / element ω jmp match_advance), never a (ζ,int entry) C call.

   The element entry (elem_entry) and the match_advance handler labels are passed by flat_drive_match
   (emit_bb.c) via the g_match_* emit-globals; the SUBJECT box's ζ-slot offset via g_sno_subject_slot.
   The element itself is emitted INLINE by the driver right after this box, via walk_bb_flat(element, lbl_γ,
   match_advance, element_β) — exactly as flat_drive_cat inline-emits its kids.

   Bytes (BINARY arm, mode-3 `--run`; sizes are illustrative — offsets computed at emit time):
       α prologue:
         49 8B 84 24 <subj>      mov rax, [r12+subj]         ; Σ base ptr from SUBJECT's ζ-slot
         48 B9 <&Σ>              movabs rcx, &Σ
         48 89 01                mov [rcx], rax              ; Σ = base
         41 8B 84 24 <subj+8>    mov eax, [r12+subj+8]       ; Σlen from SUBJECT's ζ-slot (low 32)
         48 B9 <&Σlen>          movabs rcx, &Σlen
         89 01                   mov [rcx], eax              ; Σlen = len
         49 BA <&Δ>             movabs r10, &Δ             ; re-establish r10 = &Δ (cursor cell)
         41 C7 84 24 <start> 0   mov dword [r12+start], 0    ; seed outer start = 0 (ch.18 step 1)
       match_retry (mr):
         41 8B 84 24 <start>     mov eax, [r12+start]        ; eax = start
         41 89 02                mov [r10], eax              ; Δ = start
         E9 <elem_entry rel32>   jmp elem_entry              ; into the inline element
       match_advance (element ω target, DEFINE):
         41 FF 84 24 <start>     inc dword [r12+start]       ; ch.18 step 6: advance start
         41 8B 84 24 <start>     mov eax, [r12+start]
         48 B9 <&Σlen>          movabs rcx, &Σlen
         3B 01                   cmp eax, [rcx]              ; start <= Σlen ?
         0F 8F <lbl_ω rel32>    jg  lbl_ω                   ; start exhausted -> whole match fails
         48 B9 <&kw_anchor>      movabs rcx, &kw_anchor
         48 83 39 00             cmp qword [rcx], 0
         0F 85 <lbl_ω rel32>    jne lbl_ω                   ; &ANCHOR nonzero -> no unanchored advance
         E9 <match_retry rel32>  jmp match_retry             ; (internal backward rel32)
       β (DEFINE):
         E9 <lbl_ω rel32>       jmp lbl_ω
*/
#include <string>
#include <string.h>
#include <stdint.h>
#include "emit_str.h"
extern "C" {
#include "bb_template_common.h"
int          bb_slot_alloc(IR_t * nd);
extern int   g_sno_subject_slot;
extern const char * g_match_elem_lbl;
extern const char * g_match_advance_lbl;
extern struct bb_label_t * g_match_elem_p;
extern struct bb_label_t * g_match_advance_p;
extern int64_t kw_anchor;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static std::string bb_match_str(IR_t * pBB, bb_bin_t & bin) {
    bin = {};
    if (!PLATFORM_X86) return std::string();
    if (MEDIUM_MACRO_DEF) return s_comment("# no macro form — IR_PAT_MATCH");
    int subj_off = g_sno_subject_slot;
    bb_label_t * elem_p = g_match_elem_p;
    bb_label_t * adv_p  = g_match_advance_p;
    if (!pBB || subj_off < 0 || !elem_p || !adv_p) {
        if (MEDIUM_BINARY) {
            fprintf(stderr, "[SBB] FATAL bb_match: missing context (subj_slot=%d elem=%p adv=%p) — flat_drive_match must set g_sno_subject_slot + g_match_*\n",
                    subj_off, (void*)elem_p, (void*)adv_p);
            abort();
        }
        return bomb_text("IR_PAT_MATCH: ch.18 outer-loop drive context not set (flat_drive_match)");
    }
    int start_off = bb_slot_alloc(pBB);
    if (MEDIUM_TEXT) {
        return bomb_text("IR_PAT_MATCH: mode-4 TEXT arm not yet written (PB-RB-8 sweep)");
    }
    if (MEDIUM_BINARY) {
        std::string b;
        /* α prologue ------------------------------------------------------------------------------------*/
        b += bytes(4, "\x49\x8B\x84\x24") + u32le((uint32_t)subj_off);          /* mov rax,[r12+subj]     */
        b += bytes(2, "\x48\xB9") + u64le(TEMPLATE_ADDR_SIGMA);                  /* movabs rcx,&Σ          */
        b += bytes(3, "\x48\x89\x01");                                          /* mov [rcx],rax          */
        b += bytes(4, "\x41\x8B\x84\x24") + u32le((uint32_t)(subj_off + 8));    /* mov eax,[r12+subj+8]   */
        b += bytes(2, "\x48\xB9") + u64le(TEMPLATE_ADDR_SIGLEN);                 /* movabs rcx,&Σlen       */
        b += bytes(2, "\x89\x01");                                              /* mov [rcx],eax          */
        b += bytes(2, "\x49\xBA") + u64le(TEMPLATE_ADDR_DELTA);                  /* movabs r10,&Δ          */
        b += bytes(4, "\x41\xC7\x84\x24") + u32le((uint32_t)start_off) + u32le(0); /* mov d[r12+start],0  */
        /* match_retry (internal) ------------------------------------------------------------------------*/
        int mr_off = (int)b.size();
        b += bytes(4, "\x41\x8B\x84\x24") + u32le((uint32_t)start_off);         /* mov eax,[r12+start]    */
        b += bytes(3, "\x41\x89\x02");                                          /* mov [r10],eax (Δ=start)*/
        b += bytes(1, "\xE9");                                                  /* jmp elem_entry         */
        bin.sites.push_back((int)b.size()); bin.labels.push_back(elem_p); bin.is_def.push_back(false);
        b += u32le(0);
        /* match_advance (DEFINE — element ω lands here) -------------------------------------------------*/
        bin.sites.push_back((int)b.size()); bin.labels.push_back(adv_p); bin.is_def.push_back(true);
        b += bytes(4, "\x41\xFF\x84\x24") + u32le((uint32_t)start_off);         /* inc d[r12+start]       */
        b += bytes(4, "\x41\x8B\x84\x24") + u32le((uint32_t)start_off);         /* mov eax,[r12+start]    */
        b += bytes(2, "\x48\xB9") + u64le(TEMPLATE_ADDR_SIGLEN);                 /* movabs rcx,&Σlen       */
        b += bytes(2, "\x3B\x01");                                              /* cmp eax,[rcx]          */
        b += bytes(2, "\x0F\x8F");                                              /* jg lbl_ω               */
        bin.sites.push_back((int)b.size()); bin.labels.push_back(_.lbl_ω_p); bin.is_def.push_back(false);
        b += u32le(0);
        b += bytes(2, "\x48\xB9") + u64le((uint64_t)(uintptr_t)&kw_anchor);      /* movabs rcx,&kw_anchor  */
        b += bytes(4, "\x48\x83\x39\x00");                                      /* cmp qword [rcx],0      */
        b += bytes(2, "\x0F\x85");                                              /* jne lbl_ω              */
        bin.sites.push_back((int)b.size()); bin.labels.push_back(_.lbl_ω_p); bin.is_def.push_back(false);
        b += u32le(0);
        b += bytes(1, "\xE9");                                                  /* jmp match_retry        */
        { int after = (int)b.size() + 4; b += u32le((uint32_t)(int32_t)(mr_off - after)); }
        /* β (DEFINE) ------------------------------------------------------------------------------------*/
        bin.sites.push_back((int)b.size()); bin.labels.push_back(_.lbl_β_p); bin.is_def.push_back(true);
        b += bytes(1, "\xE9");                                                  /* jmp lbl_ω              */
        bin.sites.push_back((int)b.size()); bin.labels.push_back(_.lbl_ω_p); bin.is_def.push_back(false);
        b += u32le(0);
        return b;
    }
    return std::string();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" void bb_match(IR_t * pBB) { bb_bin_t bin; bb_emit_asm_result(bb_match_str(pBB, bin), bin); }
