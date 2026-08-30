/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef GC_HEAP_H
#define GC_HEAP_H
#include <stdint.h>
#define HB_ZCOL  200
#define HB_FILL  202
#define HB_WS    203
#define HB_ZBLK  204
#define HB_WSC   205
#define HB_AGGV  206
#define HB_AGGP  207
#define HB_AGGT  208
#define HB_PLJ   209
#define HB_WSS   210
/*⭐ HB_AGGB (s262) -- A TABLE BUCKET'S CONTIGUOUS INDEX ARRAY: TBSLOT_t {hkey, entry*} records.
  ⛔ IT IS ITS OWN TYPE BECAUSE THE SWEEP READS TYPES STRUCTURALLY.  An HB_AGGP block is cast to a single TBPAIR_t
  and its key_descr/val visited as descriptors; a slot array landing in that class had its 64-bit hkey read as a
  DESCR_t and segfaulted the collector (measured, not feared).  HB_AGGB carries NO descriptors -- the entries it
  points at are marked by gc_visit_tbblk walking the table -- so its sweep arm is a deliberate no-op. */
#define HB_AGGB  211
/*⭐ HB_ARR/HB_DINST (gc-stress-arm-nondeterministic) -- ARBLK_t/DATINST_t headers used to share the generic HB_WS tag
  with every other rt_ws_alloc payload shape in the workspace island (DESCR_t data buffers included), so gc_zeta_frame
  could only check "is this address the start of SOME real WS block", not "...of an ARBLK_t/DATINST_t specifically".
  A conservatively-scanned garbage word whose payload happened to equal a data buffer's address passed that check and
  got its bytes read as ARBLK_t fields -- own type tags close the gap the same way HB_AGGV/HB_AGGT already do for the
  main heap. rt_ws_alloc_tag is the only thing that may write these; a bare rt_ws_alloc stays HB_WS. */
#define HB_ARR   212
#define HB_DINST 213
#define HB_IS_AGG(t_) (((t_) >= HB_AGGV && (t_) <= HB_AGGT) || (t_) == HB_AGGB)
#define HBF_TTL  0x0001
#define HBF_MARK 0x0002
typedef struct rt_hblk_t { uint64_t fwd; uint32_t size; uint16_t type; uint16_t flags; } rt_hblk_t;
struct DESCR_t;
void *rt_gcheap_alloc(uint16_t type, uint64_t payload_bytes);
void  rt_gcheap_warmup(void);
char *rt_str_alloc(long n);
char *rt_str_dup(const char *s);
typedef struct { char *owner; long len; int gva_n; int off; } rt_sxt_fr_t;
extern rt_sxt_fr_t g_sxt_fr;
#define g_sxt_owner (g_sxt_fr.owner)
void  rt_sxt_gva_count(int n);
void  rt_sxt_break(const char *s);
/* ⭐ inline fast path for hot callers (row perf-sxt-break-unconditional-call-tax), same precedent
   shape as rt_protected.h's is_protected_pat_lead: static inline is a promise -O0 does not keep,
   always_inline is honoured. NOT the s264 tag-predicate/DESCR_t-by-value hazard (GOAL-HQ-PERFORM.md
   :177) -- g_sxt_owner is a plain GLOBAL, not stack/register state a conservative GC scan depends
   on, and this body allocates nothing, so there is no GC-triggering point inside it for a value to
   go missing across. rt_sxt_break itself is untouched and stays the real symbol rtx_icnvar.s calls
   (`call rt_sxt_break@PLT`) -- it now just delegates here, one logic definition either way. */
static inline __attribute__((always_inline)) void rt_sxt_break_fast(const char *s) { if (s && s == g_sxt_owner) g_sxt_owner = (char *)0; }
void  rt_sxt_note(char *s, long len);
long  rt_sxt_match(const char *s);
char *rt_sxt_extend(char *s, long al, long bl);
long  rt_gcheap_verify(void);
long  rt_gc_collect(void);
long  rt_gcheap_free(void);
long  rt_gc_runs_count(void);
void  rt_gc_point(struct DESCR_t *d0, const char **r0);
void  rt_gc_point_arr(struct DESCR_t *arr, int n, const char **r0);
void  rt_gc_visit_descr(struct DESCR_t *d);
void  rt_gc_visit_raw(const char **loc);
void  rt_gc_root_range_add(const char *lo, const char *hi);
void  rt_gc_root_range_add_seamsafe(const char *lo, const char *hi);
void  rt_gc_root_range_del(const char *lo);
#endif
