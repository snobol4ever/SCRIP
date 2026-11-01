#ifndef EMITTER_EMIT_GC_SITE_H
#define EMITTER_EMIT_GC_SITE_H
#ifndef __cplusplus
#error emit_gc_site.h is C++ only
#endif
#include <string>
#include <cstdint>
extern "C++" int emit_gc_site_new(int kind, int adj);
extern "C++" int emit_gc_site_rsp_based(void);
extern "C++" int emit_gc_site_raw(int kind, int rule, int depth);
extern "C++" void emit_gc_site_play(int idx, const void * pc, int delta, int aligned, int unknown);
extern "C++" std::string emit_gc_site_label(int idx);
extern "C++" std::string emit_gc_map_register(int value_bytes, int frame_bytes, int header_bytes, unsigned flags, int frame_rel, uint64_t link);
#endif
