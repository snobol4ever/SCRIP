#ifndef EMITTER_BB_LABEL_H
#define EMITTER_BB_LABEL_H
#include <stdint.h>
#include "ir/ct_vec.h"
#ifdef __cplusplus
extern "C" {
#endif
#define BB_LABEL_UNRESOLVED (-1)
typedef struct bb_label_t { const char * name; int offset; uint32_t name_cap; } bb_label_t;
#define bb_label_defined(lbl) ((lbl)->offset != BB_LABEL_UNRESOLVED)
typedef enum { JMP_JMP = 0, JMP_JE, JMP_JNE, JMP_JL, JMP_JGE, JMP_JG } jmp_kind_t;
typedef enum { PATCH_REL8, PATCH_REL32, PATCH_ABS64 } bb_patch_kind_t;
typedef struct { int site; bb_label_t * label; bb_patch_kind_t kind; int next; } bb_patch_t;
extern cv_t bb_patch_list;
extern int bb_patch_count;
void bb_emit_patch_rel32(bb_label_t * lbl);
void bb_emit_patch_abs64(bb_label_t * lbl);
void emit_label_initf (bb_label_t * lbl, const char * fmt, ...);
void bb_label_define (bb_label_t * lbl);
bb_label_t * emit_label_alloc (const char * fmt, ...);
bb_label_t * emit_label_intern (const char * name);
void emit_label_pool_reset(void);
void bb_label_alias (bb_label_t * stub, bb_label_t * tgt);
bb_label_t * bb_label_fold (bb_label_t * l);
void bb_label_alias_reset(void);
void emit_label_define_bb (bb_label_t * lbl);
void emit_jmp_label (bb_label_t * target, jmp_kind_t kind);
#ifdef __cplusplus
}
#endif
#endif
