/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef DTP_H
#define DTP_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
void *dtp_wrap_fn_sz(void *fn, int64_t zsz, int32_t zstatic);
int64_t dtp_zsz_of(void *headv);
int dtp_zstatic_of(void *headv);
typedef struct sno_thunk_rec { void *fn; int32_t frame_bytes; int32_t zstatic; } sno_thunk_rec_t;
#ifdef __cplusplus
static_assert(sizeof(sno_thunk_rec_t) == 16 && __builtin_offsetof(sno_thunk_rec_t, frame_bytes) == 8 && __builtin_offsetof(sno_thunk_rec_t, zstatic) == 12, "a compiled pattern thunk's record is one .quad entry and two .long fields, emitted as .Lthk_<sym> in mode 4 and filled in place in mode 3 (Lon 2026-09-29: every address baked, no name at run time)");
#else
_Static_assert(sizeof(sno_thunk_rec_t) == 16 && __builtin_offsetof(sno_thunk_rec_t, frame_bytes) == 8 && __builtin_offsetof(sno_thunk_rec_t, zstatic) == 12, "a compiled pattern thunk's record is one .quad entry and two .long fields, emitted as .Lthk_<sym> in mode 4 and filled in place in mode 3 (Lon 2026-09-29: every address baked, no name at run time)");
#endif
void *bb_thunk_rec_addr(const char *name);
void bb_thunk_rec_fill(const char *name, void *fn, int32_t frame_bytes, int32_t zstatic);
#ifdef __cplusplus
}
#endif
#endif
