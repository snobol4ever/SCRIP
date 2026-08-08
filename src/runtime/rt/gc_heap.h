#ifndef GC_HEAP_H
#define GC_HEAP_H
#include <stdint.h>
/* GC-0 (ARCH-ZETA-LOCAL-STORAGE §6e): the scrip-owned bump heap with SIL title-word block headers.
 * One 16B header per block — the v311.sil title DESCR reborn (silly/arena.c is the reference port):
 *   fwd   ↔ title.a.i — the forward address home GC-2 writes during regeneration; zero between collections.
 *   size  ↔ title.v   — TOTAL block bytes including this header, 16-aligned (linear walk: p += size).
 *   type  ↔ the datatype half of title.f — value-world blocks carry the DESCR DTYPE_t VERBATIM (DT_S=1 is
 *           the first migrated family); activation-family codes live above the DTYPE range per §6c.
 *   flags ↔ the flag half of title.f — HBF_TTL always set (the SIL TTL marker; verify walks assert it),
 *           HBF_MARK is GC-1's live bit (never set in GC-0).
 * Allocation is PURE BUMP between collections — SNOBOL4's original allocator (§6a) — and GC-0 ships with
 * ZERO collector: exhaustion is a loud bomb naming the knob, exactly the ζ-arena Stage-α discipline.
 * u32 size is an implementation width (4GB/block), NOT an &MAXLNGTH-style discrimination cap — DESCR tags
 * make magnitude discrimination unnecessary (§6b, manual pin 1). */
#define HB_ZCOL  200
#define HB_ZPROM 201
#define HB_FILL  202
#define HB_WS    203
#define HB_ZBLK  204
#define HB_WSC   205
#define HB_AGGV  206
#define HB_AGGP  207
#define HB_AGGT  208
#define HB_PLJ   209
#define HB_WSS   210
#define HBF_TTL  0x0001
#define HBF_MARK 0x0002
#define HBF_PIN  0x0004
typedef struct rt_hblk_t { uint64_t fwd; uint32_t size; uint16_t type; uint16_t flags; } rt_hblk_t;
struct DESCR_t;
void *rt_gcheap_alloc(uint16_t type, uint64_t payload_bytes);
void  rt_gcheap_warmup(void);   /* W1-GC-WARMUP: drive gc_static_segs_init from a C frame before JIT entry (dl_iterate_phdr movaps requires aligned RSP) */
char *rt_str_alloc(long n);
char *rt_str_dup(const char *s);
void  rt_sxt_gva_count(int n);
void  rt_sxt_break(const char *s);
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
void  rt_gc_pin_ptr(const char *p);
void  rt_gc_root_pin_add(const char *p);
void  rt_gc_root_pin_del(const char *p);
void  rt_gc_root_range_add(const char *lo, const char *hi);
void  rt_gc_root_range_del(const char *lo);
#endif
