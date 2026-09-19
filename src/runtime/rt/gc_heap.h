/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef GC_HEAP_H
#define GC_HEAP_H
#include <stdint.h>
#include <stddef.h>
#define HB_ZCOL  200
#define HB_FILL  202
#define HB_WS    203
#define HB_ZBLK  204
#define HB_WSC   205
#define HB_AGGV  206
#define HB_AGGP  207
#define HB_AGGT  208
#define HB_AGGB  211
#define HB_ARR   212
#define HB_DINST 213
#define HB_DVEC  214
#define HB_WSB   215
#define HB_PLDB  216
#define HB_PLDBS 217
#define HB_PLDBR 218
#define HB_PLDBK 219
#define HB_PVEC  220
#define HB_DTP   221
#define HB_DTPRCP 222
#define HB_IS_AGG(t_) (((t_) >= HB_AGGV && (t_) <= HB_AGGT) || (t_) == HB_AGGB)
#define HBF_TTL  0x0001
#define HBF_MARK 0x0002
typedef struct rt_hblk_t { uint64_t fwd; uint32_t size; uint16_t type; uint16_t flags; } rt_hblk_t;
struct DESCR_t;
void *rt_gcheap_alloc(uint16_t type, uint64_t payload_bytes);
void  rt_gcheap_warmup(void);
char *rt_str_alloc(long n);
void *rt_ws_alloc_descr(size_t n);
void *rt_wsb_alloc(size_t n);
void *rt_wsb_realloc(void *p, size_t n);
void *rt_pl_struct_alloc(uint16_t type, size_t n);
void *rt_pvec_alloc(size_t n);
void *rt_pvec_realloc(void *p, size_t n);
void *rt_pm_struct_alloc(uint16_t type, size_t n);
char *rt_str_dup(const char *s);
typedef struct { char *owner; long len; int gva_n; int off; } rt_sxt_fr_t;
extern rt_sxt_fr_t g_sxt_fr;
#define g_sxt_owner (g_sxt_fr.owner)
void  rt_sxt_gva_count(int n);
void  rt_sxt_break(const char *s);
static inline __attribute__((always_inline)) void rt_sxt_break_fast(const char *s) { if (s && s == g_sxt_owner) g_sxt_owner = (char *)0; }
void  rt_sxt_note(char *s, long len);
long  rt_sxt_match(const char *s);
char *rt_sxt_extend(char *s, long al, long bl);
long  rt_gcheap_verify(void);
long  rt_gc_collect(void);
long  rt_gcheap_free(void);
long  rt_gc_runs_count(void);
void  rt_gc_poll(void);
long  rt_gc_polls_count(void);
void  rt_gc_point(struct DESCR_t *d0, const char **r0);
void  rt_gc_point_arr(struct DESCR_t *arr, int n, const char **r0);
void  rt_gc_visit_descr(struct DESCR_t *d);
void  rt_gc_visit_raw(const char **loc);
void  rt_gc_root_range_add(const char *lo, const char *hi);
struct gc_frame_map_t;
void  rt_gc_frame_maps_add(const struct gc_frame_map_t *m);
void  rt_gc_frame_maps_install(const struct gc_frame_map_t *const *maps, int n);
void  rt_gc_frame_maps_install_counted(const void *tab);
const struct gc_frame_map_t *const *rt_gc_frame_maps(int *n);
void  rt_gc_frame_maps_dump(void);
void  rt_gc_frame_map_check(const struct DESCR_t *cell);
void  rt_gc_root_range_add_seamsafe(const char *lo, const char *hi);
void  rt_gc_root_range_del(const char *lo);
#endif
