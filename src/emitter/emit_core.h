#ifndef EMIT_CORE_H
#define EMIT_CORE_H
#ifdef __cplusplus
extern "C" {
#endif
#define TEXT_MODE_INVOCATION  0
#define TEXT_MODE_DEFINITION  1
#include "bb_pool.h"
#include "x86_opcodes.h"
#include "IR.h"
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>
typedef enum {
    EMIT_TEXT             = 0,
    EMIT_BINARY_WIRED     = 1,
    EMIT_BINARY_BROKERED  = 2,
    EMIT_MACRO_DEF        = 3,
    EMIT_TEXT_INLINE      = 4,
    EMIT_JVM              = 5,
    EMIT_JS               = 6,
    EMIT_NET              = 7,
    EMIT_WASM             = 8
} bb_emit_mode_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define EMIT_BINARY     EMIT_BINARY_WIRED
typedef enum {
    BB_PLATFORM_X86  = 0,
    BB_PLATFORM_JVM  = 1,
    BB_PLATFORM_NET  = 2,
    BB_PLATFORM_JS   = 3,
    BB_PLATFORM_WASM = 4
} bb_platform_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef enum {
    BB_MEDIUM_TEXT      = 0,
    BB_MEDIUM_BINARY    = 1,
    BB_MEDIUM_MACRO_DEF = 2
} bb_medium_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern bb_platform_t   g_platform;
extern bb_medium_t     g_medium;
extern int             g_bb_brokered;
extern int             g_use_sm_macros;
extern int             g_use_bb_macros;
#define PLATFORM_X86   (g_platform == BB_PLATFORM_X86)
#define PLATFORM_JVM   (g_platform == BB_PLATFORM_JVM)
#define PLATFORM_NET   (g_platform == BB_PLATFORM_NET)
#define PLATFORM_JS    (g_platform == BB_PLATFORM_JS)
#define PLATFORM_WASM  (g_platform == BB_PLATFORM_WASM)
#define MEDIUM_TEXT      (g_medium == BB_MEDIUM_TEXT)
#define MEDIUM_BINARY    (g_medium == BB_MEDIUM_BINARY)
#define MEDIUM_MACRO_DEF (g_medium == BB_MEDIUM_MACRO_DEF)
#define BB_BROKERED    (g_bb_brokered)
#define BB_WIRED       (!g_bb_brokered)
#define USE_SM_MACROS  (g_use_sm_macros)
#define USE_BB_MACROS  (g_use_bb_macros)
#define BB_LABEL_NAME_MAX   80
#define BB_LABEL_UNRESOLVED (-1)
#define EMIT_UNRESOLVED     BB_LABEL_UNRESOLVED
#define EMIT_LABEL_MAX      BB_LABEL_NAME_MAX
typedef struct bb_label_t { char name[BB_LABEL_NAME_MAX]; int offset; } bb_label_t;
#define bb_label_defined(lbl)  ((lbl)->offset != BB_LABEL_UNRESOLVED)
#define emit_label_ok(l)       bb_label_defined(l)
typedef enum { JMP_JMP = 0, JMP_JE, JMP_JNE, JMP_JL, JMP_JGE, JMP_JG } jmp_kind_t;
#define BB_PATCH_MAX   512
#define EMIT_PATCH_MAX BB_PATCH_MAX
typedef enum { PATCH_REL8, PATCH_REL32 } bb_patch_kind_t;
typedef struct { int site; bb_label_t * label; bb_patch_kind_t kind; } bb_patch_t;
typedef int emitter_t;
extern bb_emit_mode_t  bb_emit_mode;
extern int             g_sm_native_unsupported;
extern FILE          * bb_emit_out;
extern bb_buf_t        bb_emit_buf;
extern int             bb_emit_pos;
extern int             bb_emit_size;
extern bb_patch_t      bb_patch_list[BB_PATCH_MAX];
extern int             bb_patch_count;
extern int             g_is_text;
extern int             g_emit_text_mode;
void     bb_emit_begin      (bb_buf_t buf, int size);
int      bb_emit_end        (void);
void     bb_emit_patch_rel32(bb_label_t * lbl);
void     bb_emit_byte       (uint8_t b);
void     bb_emit_u32        (uint32_t v);
void     bb_emit_u64        (uint64_t v);
void     bb_emit_i32        (int32_t v);
void     emit_mode_set         (bb_emit_mode_t m, FILE * out);
FILE *   emit_outf             (void);
void     fmt_body_append       (const char * instr, const char * operands);
void     emit_label_initf      (bb_label_t * lbl, const char * fmt, ...);
void     bb_label_define       (bb_label_t * lbl);
bb_label_t * emit_label_alloc  (const char * fmt, ...);
bb_label_t * emit_label_intern (const char * name);
void         emit_label_pool_reset(void);
void     emit_text_rawf        (const char * fmt, ...);
void     emit_text_stno_banner (int stno, int lineno, const char * src_text);
void     emit_label_define_bb  (bb_label_t * lbl);
void     emit_jmp_label        (bb_label_t * target, jmp_kind_t kind);
void     emit_call_label       (bb_label_t * target);
struct IR_t;
int walk_bb_node(struct IR_t * nd, FILE * out);
char * walk_bb_node_str_c(struct IR_t * nd);
void jvm_push_int2(FILE * out, long v);
void jvm_emit_ldc_string(FILE * out, const char * s);
void js_escape_string(FILE * out, const char * s);
#include "XA.h"
struct tree_t;
void strtab_label(char *buf, size_t bufsz, const char *s);
void strtab_reset(void);
int  strtab_intern(const char *s);
void xa_dispatch(XA_op_t op);
#ifdef __cplusplus
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif
#endif
