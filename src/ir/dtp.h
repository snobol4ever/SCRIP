/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef DTP_H
#define DTP_H
#include <stdint.h>
#include "ct_vec.h"
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
typedef struct sno_callee_rec { const char *name; uint32_t gen1; int32_t fi; cv_t types; int64_t cgen; void *ctor; } sno_callee_rec_t;
#ifdef __cplusplus
static_assert(sizeof(sno_callee_rec_t) == 56 && __builtin_offsetof(sno_callee_rec_t, gen1) == 8 && __builtin_offsetof(sno_callee_rec_t, fi) == 12 && __builtin_offsetof(sno_callee_rec_t, types) == 16 && __builtin_offsetof(sno_callee_rec_t, cgen) == 40 && __builtin_offsetof(sno_callee_rec_t, ctor) == 48, "a callee's record, one per name the program calls, is its name; for a field read the definition generation it was filled at plus one, the field's index when every type declaring it agrees (else -1) and the list of those types' dat_types indices; for a constructor the constructor generation the full road last constructed at and the DATA type it constructed -- emitted as .Lcallee_<sym> in mode 4 (.quad, .long, .long, .quad, three .long and a pad, .quad, .quad) and held in the emitter's directory in mode 3 (Lon 2026-09-29: no by-name lookup for function calls)");
#else
_Static_assert(sizeof(sno_callee_rec_t) == 56 && __builtin_offsetof(sno_callee_rec_t, gen1) == 8 && __builtin_offsetof(sno_callee_rec_t, fi) == 12 && __builtin_offsetof(sno_callee_rec_t, types) == 16 && __builtin_offsetof(sno_callee_rec_t, cgen) == 40 && __builtin_offsetof(sno_callee_rec_t, ctor) == 48, "a callee's record, one per name the program calls, is its name; for a field read the definition generation it was filled at plus one, the field's index when every type declaring it agrees (else -1) and the list of those types' dat_types indices; for a constructor the constructor generation the full road last constructed at and the DATA type it constructed -- emitted as .Lcallee_<sym> in mode 4 (.quad, .long, .long, .quad, three .long and a pad, .quad, .quad) and held in the emitter's directory in mode 3 (Lon 2026-09-29: no by-name lookup for function calls)");
#endif
typedef struct sno_dstar_rec { char mark[8]; const char *star; int32_t pidx; uint32_t flags; } sno_dstar_rec_t;
#define SNO_DSTAR_VARREF 1u
#define SNO_DTX_REC(d) ((sno_dstar_rec_t *)(d).p)
#ifdef __cplusplus
static_assert(sizeof(sno_dstar_rec_t) == 24 && __builtin_offsetof(sno_dstar_rec_t, star) == 8 && __builtin_offsetof(sno_dstar_rec_t, pidx) == 16, "a deferred-capture star target's record (. *f() and . *X at match end) begins with the two bytes '*' and 1 -- the capture pump's star test reads the first, and no *name string carries a byte 1 second -- then the *name string, the procedure's registry index once the pump has found it (-1 before), and flags (bit 1 SNO_DSTAR_VARREF: the name is a variable, minted by CONVERT, never a procedure); emitted as .Ldstar_<sym> in mode 4 and held in the emitter's directory in mode 3; a DT_X value carries this record in its pointer slot and no name string (Lon 2026-09-29: no by-name lookup for function calls)");
#else
_Static_assert(sizeof(sno_dstar_rec_t) == 24 && __builtin_offsetof(sno_dstar_rec_t, star) == 8 && __builtin_offsetof(sno_dstar_rec_t, pidx) == 16, "a deferred-capture star target's record (. *f() and . *X at match end) begins with the two bytes '*' and 1 -- the capture pump's star test reads the first, and no *name string carries a byte 1 second -- then the *name string, the procedure's registry index once the pump has found it (-1 before), and flags (bit 1 SNO_DSTAR_VARREF: the name is a variable, minted by CONVERT, never a procedure); emitted as .Ldstar_<sym> in mode 4 and held in the emitter's directory in mode 3; a DT_X value carries this record in its pointer slot and no name string (Lon 2026-09-29: no by-name lookup for function calls)");
#endif
void *bb_dstar_rec_addr(const char *star);
void *bb_callee_rec_addr(const char *name);
void emit_callee_records_data(void);
void *bb_thunk_rec_addr(const char *name);
void bb_thunk_rec_fill(const char *name, void *fn, int32_t frame_bytes, int32_t zstatic);
#ifdef __cplusplus
}
#endif
#endif
