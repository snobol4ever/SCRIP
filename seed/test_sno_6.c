#ifdef __GNUC__
#define __kernel
#define __global
#include <malloc.h>
#include <string.h>
#include <stdbool.h>
extern int printf(const char *, ...);
#endif
/*------------------------------------------------------------------------------------------------*/
/* test_sno_6.c — BB_SWITCH GOLDEN (Lon mechanism, 2026-06-23 redirect — SUPERSEDES test_sno_5).   */
/*                                                                                                  */
/* test_sno_5's thesis ("each glob HEAD's α AND β set R12 as their first instruction") is RETIRED:  */
/* a two-line head template cannot be extended out to BB GLOBS. The live mechanism is:              */
/*                                                                                                  */
/*   (1) NO box ever sets R12. Boxes inside a glob only READ [R12+off]; the register is established  */
/*       ONCE at the glob BOUNDARY, externally, never as a box's first instruction.                  */
/*   (2) THREE external establishment sites, none of them an α/β first-instruction:                  */
/*         • STATEMENT glob (G2, not re-entrant) — R12 = static frame addr (a `lea r12,[rip+frame]`  */
/*           at the XA statement-entry; compile-time constant). Modeled here as `R12 = &secN`.       */
/*         • PATTERN glob (G1, stored *P re-entered per loop iter / recursion) — R12 set by a         */
/*           BB_SWITCH broker at the DEFER call-site: alloc-or-cache the pattern frame, PUSH the      */
/*           caller's R12 (C stack), set R12 = pattern frame, jmp callee α/β; the paired resume box   */
/*           POPs R12 back to the caller. The whole `'C' 'D'` tree shares that one pattern frame.     */
/*         • (FUNCTION glob G3, recursive — same broker, fresh frame per activation; DEFERRED, not    */
/*            exercised here. DDS-1 is the pattern case.)                                             */
/*   (3) BB_SWITCH for PATTERNS saves ONLY R12. Σ/δ/Δ (r13/r14/r15) are NOT touched — the subject     */
/*       flows continuously through a pattern; BB_SCAN (the scan head) owns r13/r14/r15. (A FUNCTION  */
/*       switch would additionally save the subject trio; that flavor is out of scope here.)         */
/*   (4) On BACKTRACK, the callee is RE-ENTERED at β as a FRESH call→return pair through the SAME      */
/*       broker. The broker RESTORES the cached pattern-frame pointer into R12 before jmp callee β —  */
/*       so β resumes the right frame WITHOUT the callee's head reloading R12 itself. The C-stack      */
/*       push/pop nests per transition; the frame instance persists in the caller's slot across β.    */
/*   (5) every box is a LIT (the save-nothing corner). The CAT seq accumulator at [R12_pat+seq] is    */
/*       the ONE thing that reads the pattern frame — it validates the broker put a live frame in R12. */
/*                                                                                                  */
/* The model: each glob is a C function; `[r12+off]` is access through R12 cast to the active plane's */
/* frame type; the C call stack IS the push/pop of R12 (the call-style extra-BLOB jump of ARCH-x86).  */
/*------------------------------------------------------------------------------------------------*/
typedef struct { const char * σ; int δ; } str_t;
typedef struct { unsigned int pos; __global char * buffer; } output_t;
/*------------------------------------------------------------------------------------------------*/
void    write_nl(output_t * out)                 { printf("%s", "\n"); }
void    write_sz(output_t * out, const char * s) { printf("%s", s); }
str_t   write_str(output_t * out, str_t str)     { printf("%.*s", str.δ, str.σ); return str; }
/*------------------------------------------------------------------------------------------------*/
/* The REGISTERS (globals model the x86-64 register file).                                          */
static const char * Σ = (const char *) 0;   /* r13 — subject base (BB_SWITCH-pat never touches)   */
static int          Δ = 0;                  /* r14 — cursor (BB_SWITCH-pat never touches)         */
static int          Ω = 0;                  /* r15 — subject length (BB_SWITCH-pat never touches) */
static void *       R12 = (void *) 0;       /* r12 — THE ONE frame ptr; set only at glob boundary */
static const int α = 0, β = 1;
static const str_t empty = (str_t) {(const char *) 0, 0};
static inline bool   not_null(str_t x)            { return x.σ != (const char *) 0; }
static inline int    len(const char * s)          { int δ = 0; for (; *s; δ++) s++; return δ; }
static inline str_t  str(const char * σ, int δ)   { return (str_t) {σ, δ}; }
static inline str_t  cat(str_t x, str_t y)        { return (str_t) {x.σ, x.δ + y.δ}; }
static output_t * out = (output_t *) 0;
/*------------------------------------------------------------------------------------------------*/
/* R12-trace: make the single-register save/restore at glob boundaries empirically visible.         */
static void * SEC1 = 0; static void * SEC2 = 0; static void * PF = 0;
static void show_r12(const char * at) {
    const char * who = (R12 == SEC1) ? "section#1"
                     : (R12 == SEC2) ? "section#2"
                     : (R12 == PF)   ? "pattern  "
                     : (R12 == 0)    ? "null     "
                                     : "?        ";
    printf("        [R12 -> %-9s] %s\n", who, at);
}
/*================================================================================================*/
/* THE PATTERN GLOB  PAT = 'C' 'D'  — ONE code body. NO box sets R12. Boxes read [R12+seq] only.     */
/* This function NEVER assigns R12; its caller (the BB_SWITCH broker) has already set R12 = frame.    */
/*------------------------------------------------------------------------------------------------*/
typedef struct pat_frame { str_t seq; } pat_frame_t;
str_t PAT_body(int entry);
str_t PAT_body(int entry) {
    /* HEAD does NOT touch R12 — it only routes entry→inside port. R12 was set by the broker.        */
    if (entry == α) goto PAT_α;
    if (entry == β) goto PAT_β;
    /*--------------------------------------------------------------------------------------------*/
    str_t         LITC;                                                  /* LIT 'C' — save-nothing */
    LITC_α:       if (Σ[Δ+0] != 'C')                       goto LITC_ω;
                  LITC = str(Σ+Δ,1); Δ+=1;                 goto LITC_γ;
    LITC_β:       Δ-=1;                                     goto LITC_ω;
    /*--------------------------------------------------------------------------------------------*/
    str_t         LITD;                                                  /* LIT 'D' — save-nothing */
    LITD_α:       if (Σ[Δ+0] != 'D')                       goto LITD_ω;
                  LITD = str(Σ+Δ,1); Δ+=1;                 goto LITD_γ;
    LITD_β:       Δ-=1;                                     goto LITD_ω;
    /*--------------------------------------------------------------------------------------------*/
    /* seq accumulator lives in the PATTERN frame at [R12+seq] — the ONLY read of the pattern frame. */
    #define SEQ (((pat_frame_t *) R12)->seq)
    seq_α:        SEQ = str(Σ+Δ,0);                        goto LITC_α;
    seq_β:                                                  goto LITD_β;
    LITC_γ:       SEQ = cat(SEQ, LITC);                     goto LITD_α;
    LITC_ω:                                                 goto seq_ω;
    LITD_γ:       SEQ = cat(SEQ, LITD);                     goto seq_γ;
    LITD_ω:                                                 goto LITC_β;
    /*--------------------------------------------------------------------------------------------*/
    PAT_α:                                                  goto seq_α;   /* inside α */
    PAT_β:                                                  goto seq_β;   /* inside β */
    seq_γ:        return SEQ;                                             /* outside γ */
    seq_ω:        return empty;                                          /* outside ω */
    #undef SEQ
}
/*================================================================================================*/
/* BB_SWITCH (pattern flavor) — the broker. Save/restore of R12 lives HERE, not in any box.          */
/*   α : alloc a FRESH pattern frame, CACHE its ptr in the caller's frame slot.                       */
/*   β : RESTORE the cached pattern-frame ptr (the SAME instance the α call created).                 */
/* Either way: PUSH caller R12 (the C call holds it on the stack), set R12 = pattern frame, jmp        */
/* callee; on return through γ OR ω, POP caller R12. Σ/δ/Δ untouched — pattern flavor.                 */
/*------------------------------------------------------------------------------------------------*/
static str_t bb_switch_pat(void ** caller_frame_slot, int entry) {
    pat_frame_t * pf;
    if (entry == α) { pf = (pat_frame_t *) calloc(1, sizeof(pat_frame_t)); *caller_frame_slot = pf; }
    else            { pf = (pat_frame_t *) *caller_frame_slot; }
    PF = pf;
    void * saved_r12 = R12;                                /* PUSH r12 (held on C stack as a local)  */
    R12 = pf; show_r12(entry == α ? "  SWITCH→PAT α (alloc+set)" : "  SWITCH→PAT β (cache+set)");
    str_t result = PAT_body(entry);                        /* jmp callee α/β — body never sets R12    */
    R12 = saved_r12; show_r12("  SWITCH←PAT   (pop/restore)");/* POP r12 back to caller                */
    return result;
}
/*================================================================================================*/
/* SITE 1:  S ? PAT          ( S = "CDab" )  -> succeeds, captures "CD".                              */
/* SITE 2:  S ? PAT 'Z'      ( S = "CDab" )  -> 'Z' subsequent fails, BACKTRACKS into PAT β through    */
/*                                              a SECOND BB_SWITCH (cache+set); PAT exhausts; fails.   */
/* Both sites: SAME PAT code body, DISTINCT pattern frame instance, ONE register R12 — set at the      */
/* section-glob entry (static) and swapped by the BB_SWITCH broker. NO box's first instruction.        */
/*================================================================================================*/
__kernel void snobol(
    __global const char * in,
    __global       char * buffer,
             const int    num_chars) {
    /*--------------------------------------------------------------------------------------------*/
    output_t output = {0, buffer};
    out = &output;
    /*============================================================================================*/
    /* SITE 1 — section glob #1. Section frame: start-cursor save + a slot to cache PAT's frame.    */
    /*--------------------------------------------------------------------------------------------*/
    struct sec1_frame { int start_save; void * pat_frame; } sec1 = {0, 0}; SEC1 = &sec1;
    str_t s1;
    Σ = "CDab"; Δ = 0; Ω = len(Σ);
    R12 = &sec1; show_r12("site#1 STATEMENT entry (static lea r12)");  /* SECTION glob entry sets R12 */
    ((struct sec1_frame *) R12)->start_save = Δ;                        /* match-driver state [R12+off]*/
    s1 = bb_switch_pat(&sec1.pat_frame, α);                            /* broker establishes pattern  */
    /* (R12 already restored to &sec1 by the broker's pop)                                          */
    printf("Site 1 (S ? PAT):     ");
    if (not_null(s1)) { write_sz(out, "match="); write_str(out, s1); write_nl(out); }
    else              { write_sz(out, "(fail)"); write_nl(out); }
    /*============================================================================================*/
    /* SITE 2 — section glob #2, DISTINCT frames. PAT 'Z' forces a backtrack RE-ENTRY into PAT β.   */
    /*--------------------------------------------------------------------------------------------*/
    struct sec2_frame { int start_save; void * pat_frame; str_t cap; } sec2 = {0, 0, {0,0}}; SEC2 = &sec2;
    str_t s2;
    Σ = "CDab"; Δ = 0; Ω = len(Σ);
    R12 = &sec2; show_r12("site#2 STATEMENT entry (static lea r12)");  /* SECTION glob entry sets R12 */
    s2 = bb_switch_pat(&sec2.pat_frame, α);                           /* PAT matches "CD" (Δ=2)       */
    if (not_null(s2)) {
        ((struct sec2_frame *) R12)->cap = s2;                         /* runs in SECTION plane        */
        if (Σ[Δ+0] == 'Z') { Δ+=1; }                                  /* subsequent LIT 'Z'           */
        else {                                                         /* Σ[2]='a' != 'Z' -> FAIL      */
            s2 = bb_switch_pat(&sec2.pat_frame, β);                   /* BACKTRACK: broker cache+sets  */
        }                                                             /* PAT exhausts -> empty via ω   */
    }
    printf("Site 2 (S ? PAT 'Z'): ");
    if (not_null(s2)) { write_sz(out, "match="); write_str(out, s2); write_nl(out); }
    else              { write_sz(out, "(fail)"); write_nl(out); }
    /*--------------------------------------------------------------------------------------------*/
    printf("\nONE register R12. Set at section-glob entry (static) + swapped by BB_SWITCH broker.\n");
    printf("No box's first instruction touches R12. β re-entry restored via broker, not callee head.\n");
}
/*================================================================================================*/
#ifdef __GNUC__
static char szOutput[1024] = {0};
int main() {
    snobol((const char *) 0, szOutput, sizeof(szOutput));
    return 0;
}
#endif
/*------------------------------------------------------------------------------------------------*/
