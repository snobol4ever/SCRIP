/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef GC_HEAP_H
#define GC_HEAP_H
#include <stdint.h>
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
/*⭐ HB_AGGB (s262) -- A TABLE BUCKET'S CONTIGUOUS INDEX ARRAY: TBSLOT_t {hkey, entry*} records.
  ⛔ IT IS ITS OWN TYPE BECAUSE THE SWEEP READS TYPES STRUCTURALLY.  An HB_AGGP block is cast to a single TBPAIR_t
  and its key_descr/val visited as descriptors; a slot array landing in that class had its 64-bit hkey read as a
  DESCR_t and segfaulted the collector (measured, not feared).  HB_AGGB carries NO descriptors -- the entries it
  points at are marked by gc_visit_tbblk walking the table -- so its sweep arm is a deliberate no-op. */
#define HB_AGGB  211
#define HB_IS_AGG(t_) (((t_) >= HB_AGGV && (t_) <= HB_AGGT) || (t_) == HB_AGGB)
#define HBF_TTL  0x0001
#define HBF_MARK 0x0002
#define HBF_PIN  0x0004
typedef struct rt_hblk_t { uint64_t fwd; uint32_t size; uint16_t type; uint16_t flags; } rt_hblk_t;
struct DESCR_t;
void *rt_gcheap_alloc(uint16_t type, uint64_t payload_bytes);
void  rt_gcheap_warmup(void);
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
void  rt_gc_root_pin_add_seamsafe(const char *p);
void  rt_gc_root_pin_del(const char *p);
void  rt_gc_root_range_add(const char *lo, const char *hi);
void  rt_gc_root_range_add_seamsafe(const char *lo, const char *hi);
void  rt_gc_root_range_del(const char *lo);
#endif
