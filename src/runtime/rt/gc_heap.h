#ifndef GC_HEAP_H
#define GC_HEAP_H
#include <stdint.h>
#include <stddef.h>
#include "runtime/core/g_runtime.h"
#include "runtime/rt/gc_heap_rt_hblk.h"
#include "runtime/rt/gc_heap_etc.h"
#include "runtime/rt/gc_heap_rt_asm_helpers_api.h"
#include "runtime/rt/gc_heap_rt_gc_frame_fn.h"
struct DESCR_t;
struct gc_frame_map_t;
#ifdef __cplusplus
extern "C" {
#endif
void rt_gcheap_warmup(void);
void *rt_ws_alloc_descr(size_t n);
void *rt_wsb_alloc(size_t n);
void *rt_wsb_realloc(void *p, size_t n);
void *rt_ws_alloc_raw(size_t nbytes);
void *rt_rvec_realloc(void *p, size_t nbytes);
void *rt_gcheap_grow_block(void *p, uint16_t type, uint64_t n);
void gv_reserve(gv_t *v, uint16_t kind, uint32_t esz, uint64_t need, const char *name);
void *gv_push(gv_t *v, uint16_t kind, uint32_t esz, const char *name);
void gv_gc_root(gv_t *v);
void *rt_pl_struct_alloc(uint16_t type, size_t n);
void *rt_pvec_alloc(size_t n);
void *rt_pvec_realloc(void *p, size_t n);
void *rt_pm_struct_alloc(uint16_t type, size_t n);
char *rt_str_dup(const char *s);
extern rt_sxt_fr_t * const rt_sxt_fr_p;
void rt_sxt_gva_count(int n);
void rt_sxt_gva_lo(int k0, int first);
void rt_sxt_break(const char *s);
void rt_sxt_note(char *s, long len);
long rt_sxt_match(const char *s);
char *rt_sxt_extend(char *s, long al, long bl);
long rt_gcheap_verify(void);
int rt_gc_blk_marked(const rt_hblk_t *h);
int rt_gc_stale_addr_report(void *fault, void *ip);
int rt_gc_frames_visit(rt_gc_frame_fn fn, void *a);
void rt_gc_segments_visit(void (*fn)(const char *, const char *, void *), void *a);
long rt_gcheap_free(void);
long rt_gc_runs_count(void);
void rt_gc_assert_dead(const void *payload);
void *rt_gc_assert_instances(uint16_t type, long n);
long rt_gc_cb_open(void);
long rt_gc_cb_close(long mark, const char *file, int line, void *lo, void *hi);
void rt_gc_poll(void);
long rt_gc_polls_count(void);
void rt_gc_point(struct DESCR_t *d0, const char **r0);
void rt_gc_point_arr_probe_c(struct DESCR_t *arr, int n, const char **saved_subject_reg, char *floor);
void rt_gc_visit_raw(const char **loc);
void rt_gc_visit_raw_in(const char **loc, const void *holder);
void rt_gc_root_range_add(const char *lo, const char *hi);
void rt_gc_frame_maps_add(const struct gc_frame_map_t *m);
void rt_gc_frame_maps_drop_range(const void *lo, const void *hi);
void rt_gc_frame_maps_install(const struct gc_frame_map_t *const *maps, int n);
void rt_gc_frame_maps_install_counted(const void *tab);
const struct gc_frame_map_t *const *rt_gc_frame_maps(int *n);
void rt_gc_frame_maps_dump(void);
void rt_gc_root_range_add_seamsafe(const char *lo, const char *hi);
void rt_gc_root_range_del(const char *lo);
#ifdef __cplusplus
}
#endif
#endif
