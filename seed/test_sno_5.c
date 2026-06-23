#ifdef __GNUC__
#define __kernel
#define __global
#include <malloc.h>
#include <string.h>
#include <stdbool.h>
extern int printf(const char *, ...);
#endif
/*------------------------------------------------------------------------------------------------*/
/* test_sno_5.c — GLOB-HEAD-SETS-R12 GOLDEN (Lon mechanism, 2026-06-23).                            */
/*                                                                                                  */
/* Demonstrates the DDS-1 design before the emitter generates it:                                   */
/*   (1) a STORED pattern  *P  ( PAT = 'C' 'D' , a seq of two LIT boxes )                            */
/*       reused at TWO match sites — ONE code body, TWO distinct ζ instances.                        */
/*   (2) each BB GLOB has a HEAD presenting outside γ/ω + inside α/β; the head's                     */
/*       α AND β each set R12 (the ONE frame pointer) as their first act.                            */
/*   (3) TWO ζ PLANES (section + pattern) TIME-MULTIPLEX a SINGLE register R12 —                     */
/*       NOT two registers. β reloads because the inter-glob excursion clobbered R12.                */
/*   (4) every box is a LIT (the save-nothing corner) — exactly DDS-1's scope.                       */
/*                                                                                                  */
/* The model is the seed-series idiom: each glob is a C function taking (ζ,entry); `[r12+off]`        */
/* is modeled as access through R12 cast to the active plane's frame type.                           */
/*------------------------------------------------------------------------------------------------*/
typedef struct { const char * σ; int δ; } str_t;
typedef struct { unsigned int pos; __global char * buffer; } output_t;
/*------------------------------------------------------------------------------------------------*/
void    write_nl(output_t * out)                 { printf("%s", "\n"); }
void    write_sz(output_t * out, const char * s) { printf("%s", s); }
str_t   write_str(output_t * out, str_t str)     { printf("%.*s", str.δ, str.σ); return str; }
/*------------------------------------------------------------------------------------------------*/
/* The REGISTERS (globals model the x86-64 register file).                                          */
static const char * Σ = (const char *) 0;   /* r13 — subject base   */
static int          Δ = 0;                  /* r14 — cursor (0-based) */
static int          Ω = 0;                  /* r15 — subject length */
static void *       R12 = (void *) 0;       /* r12 — THE ONE frame pointer, time-multiplexed       */
static const int α = 0, β = 1;
static const str_t empty = (str_t) {(const char *) 0, 0};
static inline bool   not_null(str_t x)            { return x.σ != (const char *) 0; }
static inline int    len(const char * s)          { int δ = 0; for (; *s; δ++) s++; return δ; }
static inline str_t  str(const char * σ, int δ)   { return (str_t) {σ, δ}; }
static inline str_t  cat(str_t x, str_t y)        { return (str_t) {x.σ, x.δ + y.δ}; }
static output_t * out = (output_t *) 0;
/*------------------------------------------------------------------------------------------------*/
#define ENTER(ref, size) enter((void **) (ref), (size))
static inline void * enter(void ** ζζ, size_t size) {
    void * ζ = *ζζ;
    if (size) { if (ζ) memset(ζ, 0, size); else ζ = *ζζ = calloc(1, size); }
    return ζ;
}
/*------------------------------------------------------------------------------------------------*/
/* R12-trace: make the single-register time-multiplex empirically visible.                          */
static void * SEC1 = 0; static void * SEC2 = 0;
static void show_r12(const char * at) {
    const char * who = (R12 == SEC1) ? "section#1"
                     : (R12 == SEC2) ? "section#2"
                     : (R12 == 0)    ? "null"
                                     : "pattern";
    printf("        [R12 -> %-9s ] %s\n", who, at);
}
/*================================================================================================*/
/* STORED PATTERN  PAT = 'C' 'D'  — ONE code body, its OWN ζ plane (pattern frame).                 */
/* Pattern frame holds the seq accumulator at [R12+seq]. LIT boxes are save-nothing (no slot).      */
/*------------------------------------------------------------------------------------------------*/
typedef struct pat_frame { str_t seq; } pat_frame_t;
str_t PAT(pat_frame_t ** ζζ, int entry);
str_t PAT(pat_frame_t ** ζζ, int entry) {
    pat_frame_t * ζ = *ζζ;
    /* GLOB HEAD — α and β EACH set R12 first. α: fresh frame. β: RELOAD (excursion clobbered it). */
    if (entry == α) { ζ = ENTER(ζζ, sizeof(pat_frame_t)); R12 = ζ; show_r12("PAT head α  (set)"); goto PAT_α; }
    if (entry == β) {                                     R12 = ζ; show_r12("PAT head β  (reload)"); goto PAT_β; }
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
    /* seq accumulator lives in the PATTERN frame: [R12+seq].                                       */
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
/* SITE 1:  S ? PAT          ( S = "CDab" )  -> succeeds, captures "CD".  Shows α + success.        */
/* SITE 2:  S ? PAT 'Z'      ( S = "CDab" )  -> 'Z' subsequent fails, BACKTRACKS into PAT β,         */
/*                                              exercising the head β-RELOAD; site fails.            */
/* Both sites: SAME PAT code, DISTINCT ζ instance, ONE register R12 swapped by the glob heads.       */
/*================================================================================================*/
__kernel void snobol(
    __global const char * in,
    __global       char * buffer,
             const int    num_chars) {
    /*--------------------------------------------------------------------------------------------*/
    output_t output = {0, buffer};
    out = &output;
    /*============================================================================================*/
    /* SITE 1 — section glob #1.  Section frame holds the match-driver start-cursor save.           */
    /*--------------------------------------------------------------------------------------------*/
    struct sec1_frame { int start_save; } sec1; SEC1 = &sec1;
    str_t s1;
    pat_frame_t * pf1 = (pat_frame_t *) 0;
    Σ = "CDab"; Δ = 0; Ω = len(Σ);
    R12 = &sec1; show_r12("site#1 head α (set)");           /* SECTION head α sets R12 = section#1 */
    ((struct sec1_frame *) R12)->start_save = Δ;            /* match-driver state in [R12+off]     */
    s1 = PAT(&pf1, α);                                      /* enter pattern: PAT head sets R12    */
    R12 = &sec1; show_r12("site#1 cont  (reload)");         /* SECTION β-first-instr: RELOAD R12    */
    printf("Site 1 (S ? PAT):     ");
    if (not_null(s1)) { write_sz(out, "match="); write_str(out, s1); write_nl(out); }
    else              { write_sz(out, "(fail)"); write_nl(out); }
    /*============================================================================================*/
    /* SITE 2 — section glob #2, DISTINCT ζ (pf2, sec2).  PAT 'Z' forces a backtrack into PAT β.    */
    /*--------------------------------------------------------------------------------------------*/
    struct sec2_frame { int start_save; str_t cap; } sec2; SEC2 = &sec2;
    str_t s2, litZ;
    pat_frame_t * pf2 = (pat_frame_t *) 0;
    Σ = "CDab"; Δ = 0; Ω = len(Σ);
    R12 = &sec2; show_r12("site#2 head α (set)");           /* SECTION head α sets R12 = section#2 */
    s2 = PAT(&pf2, α);                                      /* PAT matches "CD" (Δ=2)              */
    R12 = &sec2; show_r12("site#2 cont  (reload)");         /* reload before the subsequent box    */
    if (not_null(s2)) {
        ((struct sec2_frame *) R12)->cap = s2;
        /* subsequent LIT 'Z' — runs in the SECTION plane (R12 = section#2) */
        if (Σ[Δ+0] == 'Z') { litZ = str(Σ+Δ,1); Δ+=1; }    /* would succeed... */
        else {                                              /* ...but Σ[2]='a' != 'Z' -> FAIL */
            s2 = PAT(&pf2, β);                              /* BACKTRACK: PAT head β RELOADS R12   */
            R12 = &sec2; show_r12("site#2 cont  (reload)"); /* reload after pattern exhausts       */
        }
    }
    printf("Site 2 (S ? PAT 'Z'): ");
    if (not_null(s2)) { write_sz(out, "match="); write_str(out, s2); write_nl(out); }
    else              { write_sz(out, "(fail)"); write_nl(out); }
    /*--------------------------------------------------------------------------------------------*/
    printf("\nONE register R12, two ζ planes, swapped only by glob-head α/β. No second register.\n");
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
