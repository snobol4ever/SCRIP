#include "emit_core.h"
#include "emit_globals.h"
#include "emit_io.h"
#include <string>
#include "emit_str.h"
#include "stage2.h"
#include "BB_templates/bb_templates.h"
#include "XA_templates/xa_templates.h"
#include "emit_form.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>
bb_emit_mode_t  bb_emit_mode = EMIT_BINARY_WIRED;
int             g_sm_native_unsupported = 0;
FILE           *bb_emit_out  = NULL;
bb_platform_t   g_platform      = BB_PLATFORM_X86;
bb_medium_t     g_medium        = BB_MEDIUM_BINARY;
int             g_use_sm_macros = 0;
int             g_use_bb_macros = 0;
void emit_mode_set(bb_emit_mode_t m, FILE *out)
{
    bb_emit_mode = m;
    bb_emit_out  = out;
    switch (m) {
    case EMIT_TEXT:             g_platform=BB_PLATFORM_X86;  g_medium=BB_MEDIUM_TEXT;      g_use_sm_macros=0; break;
    case EMIT_TEXT_INLINE:      g_platform=BB_PLATFORM_X86;  g_medium=BB_MEDIUM_TEXT;      g_use_sm_macros=1; break;
    case EMIT_MACRO_DEF:        g_platform=BB_PLATFORM_X86;  g_medium=BB_MEDIUM_MACRO_DEF; g_use_sm_macros=0; break;
    case EMIT_BINARY_WIRED:     g_platform=BB_PLATFORM_X86;  g_medium=BB_MEDIUM_BINARY;    g_use_sm_macros=0; break;
    case EMIT_JVM:              g_platform=BB_PLATFORM_JVM;  g_medium=BB_MEDIUM_TEXT;      g_use_sm_macros=0; break;
    case EMIT_JS:               g_platform=BB_PLATFORM_JS;   g_medium=BB_MEDIUM_TEXT;      g_use_sm_macros=0; break;
    case EMIT_NET:              g_platform=BB_PLATFORM_NET;  g_medium=BB_MEDIUM_TEXT;      g_use_sm_macros=0; break;
    case EMIT_WASM:             g_platform=BB_PLATFORM_WASM; g_medium=BB_MEDIUM_TEXT;      g_use_sm_macros=0; break;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
FILE *emit_outf(void) { return bb_emit_out ? bb_emit_out : stdout; }
#include <string.h>
bb_buf_t   bb_emit_buf   = NULL;
int        bb_emit_pos   = 0;
int        bb_emit_size  = 0;
bb_patch_t bb_patch_list[BB_PATCH_MAX];
int        bb_patch_count = 0;
static bb_label_t ** g_label_pool      = NULL;
static int           g_label_pool_n    = 0;
static int           g_label_pool_max  = 0;
void emit_label_pool_reset(void)
{
    for (int i = 0; i < g_label_pool_n; i++) free(g_label_pool[i]);
    g_label_pool_n = 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
bb_label_t *emit_label_alloc(const char *fmt, ...)
{
    if (g_label_pool_n >= g_label_pool_max) {
        int new_max = g_label_pool_max ? g_label_pool_max * 2 : 64;
        bb_label_t **g = (bb_label_t **)realloc(g_label_pool, (size_t)new_max * sizeof(bb_label_t *));
        if (!g) return NULL;
        g_label_pool     = g;
        g_label_pool_max = new_max;
    }
    bb_label_t *lbl = (bb_label_t *)calloc(1, sizeof(bb_label_t));
    if (!lbl) return NULL;
    va_list ap; va_start(ap, fmt);
    vsnprintf(lbl->name, BB_LABEL_NAME_MAX, fmt, ap);
    va_end(ap);
    lbl->offset = BB_LABEL_UNRESOLVED;
    g_label_pool[g_label_pool_n++] = lbl;
    return lbl;
}
/*--------------------------------------------------------------------------------------------------------------------*/
bb_label_t *emit_label_intern(const char *name)
{
    if (!name) return NULL;
    for (int i = 0; i < g_label_pool_n; i++)
        if (g_label_pool[i] && strcmp(g_label_pool[i]->name, name) == 0) return g_label_pool[i];
    return emit_label_alloc("%s", name);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void bb_emit_begin(bb_buf_t buf, int size)
{
    bb_emit_buf    = buf;
    bb_emit_pos    = 0;
    bb_emit_size   = size;
    bb_patch_count = 0;
    emit_label_pool_reset();
}
/*--------------------------------------------------------------------------------------------------------------------*/
extern int bb_emit_overflow;
int bb_emit_end(void)
{
    if (bb_emit_overflow) {
        bb_patch_count = 0;
        return -1;
    }
    if (bb_patch_count > 0) {
        fprintf(stderr, "bb_emit_end: %d unresolved forward reference(s):\n",
                bb_patch_count);
        for (int i = 0; i < bb_patch_count; i++)
            fprintf(stderr, "  site=%d label='%s'\n",
                    bb_patch_list[i].site,
                    bb_patch_list[i].label->name);
        abort();
    }
    return bb_emit_pos;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void bb_emit_patch_rel32(bb_label_t *lbl)
{
    if (!MEDIUM_BINARY) {
        fprintf(stderr,
                "bb_emit_patch_rel32: TEXT-mode reach (target='%s') — "
                "use bb_insn_*_rel32 mnemonic helpers\n",
                lbl->name);
        abort();
    }
    if (bb_label_defined(lbl)) {
        int disp = lbl->offset - (bb_emit_pos + 4);
        bb_emit_i32(disp);
        return;
    }
    if (bb_patch_count >= BB_PATCH_MAX) {
        bb_emit_overflow = 1;
        return;
    }
    bb_patch_list[bb_patch_count].site  = bb_emit_pos;
    bb_patch_list[bb_patch_count].label = lbl;
    bb_patch_list[bb_patch_count].kind  = PATCH_REL32;
    bb_patch_count++;
    bb_emit_u32(0x00000000);
}
/*--------------------------------------------------------------------------------------------------------------------*/
int bb_emit_overflow = 0;
void bb_emit_byte(uint8_t b)
{
    if (bb_emit_mode != EMIT_BINARY_WIRED) {
        fprintf(stderr,
                "bb_emit_byte: non-BINARY-mode reach (mode=%d, b=0x%02x) — "
                "convert caller to a named bb_insn_* helper\n",
                (int)bb_emit_mode, (unsigned)b);
        abort();
    }
    if (bb_emit_pos >= bb_emit_size) {
        bb_emit_overflow = 1;
        return;
    }
    bb_emit_buf[bb_emit_pos++] = b;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void  bb_emit_u32(uint32_t v)  { bb_emit_byte((uint8_t)(v)); bb_emit_byte((uint8_t)(v>>8)); bb_emit_byte((uint8_t)(v>>16)); bb_emit_byte((uint8_t)(v>>24)); }
void  bb_emit_u64(uint64_t v)  { bb_emit_u32((uint32_t)(v)); bb_emit_u32((uint32_t)(v >> 32)); }
void  bb_emit_i32(int32_t v)   { uint32_t u; memcpy(&u, &v, 4); bb_emit_u32(u); }
int  g_is_text        = 0;
int  g_emit_text_mode = TEXT_MODE_INVOCATION;
int  g_emit_pos       = 0;
void emitter_init_binary(bb_buf_t buf, int size)
{
    g_is_text = 0; g_emit_text_mode = TEXT_MODE_INVOCATION; g_emit_pos = 0;
    bb_emit_overflow = 0;
    bb_emit_mode = EMIT_BINARY_WIRED;
    g_platform = BB_PLATFORM_X86; g_medium = BB_MEDIUM_BINARY;
    bb_emit_begin(buf, size);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void emitter_init_text(FILE *out, int mode)
{
    g_is_text = 1; g_emit_text_mode = mode; g_emit_pos = 0;
    bb_emit_mode = EMIT_TEXT;
    g_platform = BB_PLATFORM_X86; g_medium = BB_MEDIUM_TEXT;
    bb_emit_out  = out ? out : stdout;
    emit_io_set_sink(out ? out : stdout);
}
/*--------------------------------------------------------------------------------------------------------------------*/
int  emitter_end(void)        { return g_is_text ? g_emit_pos : bb_emit_end(); }
void  emitter_init_macro_def(FILE *out) { emitter_init_text(out, TEXT_MODE_DEFINITION); }
static void  ef_b1 (uint8_t a)                                   { bb_emit_byte(a); }
static void  ef_b2 (uint8_t a, uint8_t b)                        { bb_emit_byte(a); bb_emit_byte(b); }
static void  ef_b3 (uint8_t a, uint8_t b, uint8_t c)             { bb_emit_byte(a); bb_emit_byte(b); bb_emit_byte(c); }
static void  ef_b4 (uint8_t a, uint8_t b, uint8_t c, uint8_t d)  { bb_emit_byte(a); bb_emit_byte(b); bb_emit_byte(c); bb_emit_byte(d); }
static void  ef_u32(uint32_t v)                                  { bb_emit_u32(v); }
static void  ef_u64(uint64_t v)                                  { bb_emit_u64(v); }
static void ef_t3c(const char *mnem, const char *fmt, ...)
{
    char buf[256]; buf[0] = '\0';
    if (fmt) { va_list ap; va_start(ap,fmt); vsnprintf(buf,sizeof(buf),fmt,ap); va_end(ap); }
    fprintf(bb_emit_out, "%s %s\n", mnem ? mnem : "", buf);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void ef_t3c_jmp(const char *mnem, const char *target)
{ fprintf(bb_emit_out, "%s %s\n", mnem ? mnem : "", target ? target : ""); }
void emit_label_define_bb(bb_label_t *lbl)
{
    if (g_is_text) {
        char buf[256]; snprintf(buf, sizeof(buf), "%s:", lbl->name);
        fprintf(bb_emit_out, "%s\n", buf);
    } else {
        bb_emit_mode_t s = bb_emit_mode; bb_emit_mode = EMIT_BINARY_WIRED;
        bb_label_define(lbl);
        bb_emit_mode = s;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
void emit_jmp_label(bb_label_t *target, jmp_kind_t kind)
{
    static const char    *mn[]    = {"jmp","je","jne","jl","jge","jg"};
    static const uint8_t  ops[6][2] = {{0xE9,0x00},{0x0F,0x84},{0x0F,0x85},{0x0F,0x8C},{0x0F,0x8D},{0x0F,0x8F}};
    int k = (int)kind < 6 ? (int)kind : 0;
    if (g_is_text) { ef_t3c_jmp(mn[k], target->name); g_emit_pos += 6; }
    else { if (k==0) ef_b1(0xE9); else ef_b2(ops[k][0], ops[k][1]); bb_emit_patch_rel32(target); }
}
/*--------------------------------------------------------------------------------------------------------------------*/
void emit_call_label(bb_label_t *target)
{
    if (g_is_text) { ef_t3c_jmp("call", target->name); g_emit_pos += 5; }
    else { ef_b1(0xE8); bb_emit_patch_rel32(target); }
}
/*--------------------------------------------------------------------------------------------------------------------*/
void bb_label_define(bb_label_t *lbl)
{
    if (!MEDIUM_BINARY) {
        FILE *f = bb_emit_out ? bb_emit_out : stdout;
        char lbuf[256]; snprintf(lbuf, sizeof(lbuf), "%s:", lbl->name);
        fprintf(f, "%s\n", lbuf);
        return;
    }
    if (bb_emit_overflow) return;
    lbl->offset = bb_emit_pos;
    for (int i = 0; i < bb_patch_count; i++) {
        bb_patch_t *p = &bb_patch_list[i];
        if (p->label != lbl) continue;
        int target = lbl->offset;
        if (p->kind == PATCH_REL8) {
            int disp = target - (p->site + 1);
            if (disp < -128 || disp > 127) {
                fprintf(stderr, "bb_label_define: rel8 overflow for '%s': disp=%d\n",
                        lbl->name, disp);
                abort();
            }
            bb_emit_buf[p->site] = (uint8_t)(int8_t)disp;
        } else {
            int disp = target - (p->site + 4);
            uint32_t u;
            memcpy(&u, &disp, 4);
            bb_emit_buf[p->site + 0] = (uint8_t)(u      );
            bb_emit_buf[p->site + 1] = (uint8_t)(u >>  8);
            bb_emit_buf[p->site + 2] = (uint8_t)(u >> 16);
            bb_emit_buf[p->site + 3] = (uint8_t)(u >> 24);
        }
        bb_patch_list[i] = bb_patch_list[--bb_patch_count];
        i--;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
void emit_label_initf(bb_label_t *lbl, const char *fmt, ...)
{
    va_list ap; va_start(ap, fmt);
    vsnprintf(lbl->name, BB_LABEL_NAME_MAX, fmt, ap);
    va_end(ap);
    lbl->offset = BB_LABEL_UNRESOLVED;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void emit_text_stno_banner(int stno, int lineno, const char *src_text)
{
#define STNO_RULE \
    "#=======================================================================================================================\n"
    switch (bb_emit_mode) {
    case EMIT_BINARY_WIRED:
        return;
    case EMIT_TEXT_INLINE:
    case EMIT_TEXT:
    case EMIT_MACRO_DEF: {
        FILE *f = bb_emit_out ? bb_emit_out : stdout;
        fputs(STNO_RULE, f);
        if (src_text && *src_text)
            fprintf(f, "# stmt %d  (line %d):  %s\n", stno, lineno, src_text);
        else if (lineno > 0)
            fprintf(f, "# stmt %d  (line %d)\n", stno, lineno);
        else
            fprintf(f, "# stmt %d\n", stno);
        fputs(STNO_RULE, f);
        return;
    }
    }
#undef STNO_RULE
}
/*--------------------------------------------------------------------------------------------------------------------*/
void emit_text_rawf(const char *fmt, ...) {
    if (bb_emit_mode != EMIT_TEXT) return;
    va_list ap; va_start(ap, fmt); vfprintf(emit_outf(), fmt, ap); va_end(ap);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void jvm_push_int2(FILE * out, long v) {
    if (v == -1) { fprintf(out, "    iconst_m1\n"); return; }
    if (v >= 0 && v <= 5) { fprintf(out, "    iconst_%ld\n", v); return; }
    if (v >= -128 && v <= 127) { fprintf(out, "    bipush %ld\n", v); return; }
    if (v >= -32768 && v <= 32767) { fprintf(out, "    sipush %ld\n", v); return; }
    fprintf(out, "    ldc %ld\n", v);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void jvm_emit_ldc_string(FILE * out, const char * s) {
    fprintf(out, "    ldc \"");
    for (const char * p = s; *p; p++) {
        if      (*p == '"')  fprintf(out, "\\\"");
        else if (*p == '\\') fprintf(out, "\\\\");
        else if (*p == '\n') fprintf(out, "\\n");
        else if (*p == '\r') fprintf(out, "\\r");
        else if (*p == '\t') fprintf(out, "\\t");
        else                 fputc(*p, out);
    }
    fprintf(out, "\"\n");
}
/*--------------------------------------------------------------------------------------------------------------------*/
#include "IR.h"
#include "emit_ir.h"
void net_escape_ldstr(FILE * out, const char * s) {
    fprintf(out, "    ldstr      \"");
    if (!s) { fprintf(out, "\"\n"); return; }
    for (const unsigned char * p = (const unsigned char *)s; *p; p++) {
        if (*p == '"')       fprintf(out, "\\\"");
        else if (*p == '\\') fprintf(out, "\\\\");
        else if (*p < 0x20 || *p == 0x7f) fprintf(out, "\\u%04X", (unsigned)*p);
        else fputc(*p, out);
    }
    fprintf(out, "\"\n");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void net_class_hdr(FILE * out, int sid, int nid) {
    fprintf(out, ".class nested public auto ansi beforefieldinit pat_%d_%d\n", sid, nid);
    fprintf(out, "       extends [mscorlib]System.Object\n");
    fprintf(out, "       implements [boxes]Snobol4.Runtime.Boxes.IByrdBox\n{\n");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void net_α_hdr(FILE * out) {
    fprintf(out, "  .method public virtual instance valuetype [boxes]Snobol4.Runtime.Boxes.Spec\n");
    fprintf(out, "          Alpha(class [boxes]Snobol4.Runtime.Boxes.MatchState ms) cil managed\n  {\n");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void net_β_hdr(FILE * out) {
    fprintf(out, "  .method public virtual instance valuetype [boxes]Snobol4.Runtime.Boxes.Spec\n");
    fprintf(out, "          Beta(class [boxes]Snobol4.Runtime.Boxes.MatchState ms) cil managed\n  {\n");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void net_fail_ret(FILE * out) {
    fprintf(out, "    ldsfld     valuetype [boxes]Snobol4.Runtime.Boxes.Spec [boxes]Snobol4.Runtime.Boxes.Spec::Fail\n");
    fprintf(out, "    ret\n");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void net_cursor_load(FILE * out) {
    fprintf(out, "    ldarg.1\n");
    fprintf(out, "    ldfld      int32 [boxes]Snobol4.Runtime.Boxes.MatchState::Cursor\n");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void net_spec_of(FILE * out) {
    fprintf(out, "    call       valuetype [boxes]Snobol4.Runtime.Boxes.Spec [boxes]Snobol4.Runtime.Boxes.Spec::Of(int32, int32)\n");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void net_charset_class(FILE * out, int sid, int nid, const char * tag) {
    net_class_hdr(out, sid, nid);
    fprintf(out, "  .field private string _chars\n");
    fprintf(out, "  .method public specialname rtspecialname instance void .ctor(string chars) cil managed\n  {\n");
    fprintf(out, "    .maxstack 3\n    ldarg.0\n    call       instance void [mscorlib]System.Object::.ctor()\n");
    fprintf(out, "    ldarg.0\n    ldarg.1\n    dup\n    brtrue     %s_%d_%d_NN\n    pop\n    ldstr      \"\"\n", tag, sid, nid);
    fprintf(out, "  %s_%d_%d_NN:\n    stfld      string pat_%d_%d::_chars\n    ret\n  }\n", tag, sid, nid, sid, nid);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void net_push_i4(FILE * out, int v) {
    if (v >= 0 && v <= 8)          { fprintf(out, "    ldc.i4.%d\n", v); }
    else if (v == -1)               { fprintf(out, "    ldc.i4.m1\n"); }
    else if (v >= -128 && v <= 127) { fprintf(out, "    ldc.i4.s   %d\n", v); }
    else                            { fprintf(out, "    ldc.i4     %d\n", v); }
}
/*--------------------------------------------------------------------------------------------------------------------*/
int walk_bb_node(IR_t * nd, FILE * out) {
    extern void bb_prepare_capture_arbno(IR_t *nd, int imm);
    extern void bb_prepare(IR_t *nd);
    extern int  bb_slot_get(IR_t *nd);
    if (!nd) return 1;
    g_emit.node = nd;
    emit_io_set_sink(out);
    g_emit.sid  = 0;
    g_emit.nid  = bb_node_id(nd);
    g_emit.op_sval = IR_LIT(nd).sval;
    g_emit.op_ival = IR_LIT(nd).ival;
    g_emit.op_node_kind = (int)nd->op;
    g_emit.op_dval = IR_LIT(nd).dval;
    g_emit.op_counter = IR_EXEC(nd).counter;
    IR_t *op_a = (nd->op != IR_SCAN) ? ((nd->n_operands > 0) ? nd->operands[0] : (IR_t *)0) : ((nd->n_operands > 2) ? nd->operands[2] : (IR_t *)0);
    g_emit.op_a_sval = op_a ? IR_LIT(op_a).sval : (const char *)0;
    g_emit.op_a_node_kind = op_a ? (int)op_a->op : -1;
    g_emit.op_a_slot = (op_a && op_a->op != IR_LIT_F && op_a->op != IR_LIT_NUL) ? bb_slot_get(op_a) : -1;
    g_emit.op_a_counter = op_a ? IR_EXEC(op_a).counter : 0;
    g_emit.op_a_ival_sg = op_a ? IR_LIT(op_a).ival : 0;
    g_emit.op_a_dval = op_a ? IR_LIT(op_a).dval : 0;
    switch (nd->op) {
    case IR_PAT_LIT:         bb_lit();               return 0;
    case IR_PAT_ANY:         bb_match_any();           return 0;
    case IR_PAT_NOTANY:      bb_match_notany();        return 0;
    case IR_PAT_SPAN:        bb_match_span();            return 0;
    case IR_PAT_BREAK:       bb_match_break();           return 0;
    case IR_PAT_BREAKX:      bb_match_breakx();          return 0;
    case IR_PAT_ARB:         bb_match_arb();             return 0;
    case IR_PAT_CAT:         bb_match_cat();             return 0;
    case IR_PAT_ALT:         bb_match_alt();             return 0;
    case IR_PAT_LEN:         bb_match_len();           return 0;
    case IR_PAT_POS:         bb_match_pos();             return 0;
    case IR_PAT_TAB:         bb_match_tab();             return 0;
    case IR_PAT_RTAB:        bb_match_rtab();            return 0;
    case IR_PAT_SPAN_VAR:    bb_match_span_var();        return 0;
    case IR_PAT_ATP:         bb_match_atp();             return 0;
    case IR_PAT_REM:         bb_match_rem();           return 0;
    case IR_PAT_FENCE:       bb_match_fence();           return 0;
    case IR_PAT_ABORT:       bb_match_abort();           return 0;
    case IR_PAT_ARBNO:       bb_match_arbno(nd);             return 0;
    case IR_SUBJECT:         bb_subject();             return 0;
    case IR_PAT_MATCH_HEAD:    bb_match_head();        return 0;
    case IR_PAT_MATCH_RETRY:   bb_match_retry();       return 0;
    case IR_PAT_MATCH_ADVANCE: bb_match_advance();     return 0;
    case IR_PATTERN_LIT:     bb_pattern_lit();                                                            return 0;
    case IR_PATTERN_ANY:     bb_pattern_unary_s();                                                        return 0;
    case IR_PATTERN_NOTANY:  bb_pattern_unary_s();                                                        return 0;
    case IR_PATTERN_SPAN:    bb_pattern_unary_s();                                                        return 0;
    case IR_PATTERN_BREAK:   bb_pattern_unary_s();                                                        return 0;
    case IR_PATTERN_BREAKX:  bb_pattern_unary_s();                                                        return 0;
    case IR_PATTERN_LEN:     bb_pattern_unary_i();                                                        return 0;
    case IR_PATTERN_POS:     bb_pattern_unary_i();                                                        return 0;
    case IR_PATTERN_RPOS:    bb_pattern_unary_i();                                                        return 0;
    case IR_PATTERN_TAB:     bb_pattern_unary_i();                                                        return 0;
    case IR_PATTERN_RTAB:    bb_pattern_unary_i();                                                        return 0;
    case IR_PATTERN_ARB:     bb_pattern_arb();                                                             return 0;
    case IR_PATTERN_BAL:     bb_pattern_stub("bb_pattern_bal: builder pending (B7b)");                    return 0;
    case IR_PATTERN_ABORT:   bb_pattern_nullary();                                                        return 0;
    case IR_PATTERN_FENCE:   bb_pattern_nullary();                                                        return 0;
    case IR_PATTERN_FAIL: case IR_PATTERN_REM: case IR_PATTERN_SUCCEED: bb_pattern_nullary();             return 0;
    case IR_PATTERN_ARBNO:   bb_pattern_stub("bb_pattern_arbno: builder pending (B9)");                   return 0;
    case IR_PATTERN_FENCE_P: bb_pattern_stub("bb_pattern_fence_p: builder pending (B9)");                 return 0;
    case IR_PATTERN_CAT:     bb_pattern_cat();                                                            return 0;
    case IR_PATTERN_ALT:     bb_pattern_alt();                                                            return 0;
    case IR_PATTERN_CAPTURE: bb_pattern_stub("bb_pattern_capture: builder pending (B8)");                 return 0;
    case IR_PATTERN_DEFER:   bb_pattern_stub("bb_pattern_defer: builder pending (B10)");                  return 0;
    case IR_DTP_ASSIGN:      bb_dtp_assign();                                                             return 0;
    case IR_PAT_ASSIGN_COND:
    case IR_PAT_ASSIGN_IMM:  bb_match_capture();         return 0;
    case IR_ARITH:           bb_prepare(nd); bb_arith();           return 0;
    case IR_LIT_I:
    case IR_LIT_S:
    case IR_LIT_F:
    case IR_LIT_NUL:              bb_lit_scalar(nd);         return 0;
    case IR_VAR:                  { extern int g_icn_globals_nv; extern void bb_keyword(IR_t *); if (IR_LIT(nd).sval && IR_LIT(nd).sval[0] == '&') bb_keyword(nd); else if (IR_EXEC(nd).state == 1 && g_icn_globals_nv) bb_var_global(nd); else bb_var(nd); } return 0;
    case IR_ASSIGN: {
        extern int g_descr_flat_chain; extern void bb_gvar_assign(IR_t *);
        if (!g_descr_flat_chain && IR_LIT(nd).sval && op_a && (op_a->op == IR_LIT_S || op_a->op == IR_LIT_I || op_a->op == IR_BINOP || op_a->op == IR_VAR || op_a->op == IR_SEQ || op_a->op == IR_SEQ_EXPR || op_a->op == IR_CALL)) { bb_gvar_assign(nd); return 0; }
        if (g_descr_flat_chain && IR_LIT(nd).sval) { extern void bb_assign_local(void); bb_assign_local(); return 0; }
        fprintf(out, "; [walk_bb_node: kind=%d unhandled]\n", (int)nd->op); return 1;
    }
    case IR_ASSIGN_LIT_S: { extern void bb_gvar_assign_lit_s(void); bb_gvar_assign_lit_s(); return 0; }
    case IR_ASSIGN_LIT_I: { extern void bb_gvar_assign_lit_i(void); bb_gvar_assign_lit_i(); return 0; }
    case IR_ASSIGN_VAR:    { extern void bb_gvar_assign_var(void);    bb_gvar_assign_var();    return 0; }
    case IR_ASSIGN_CONCAT: { extern void bb_gvar_assign_concat(void); bb_gvar_assign_concat(); return 0; }
    case IR_ASSIGN_CALL:   { extern void bb_gvar_assign_call(void);   bb_gvar_assign_call();   return 0; }
    case IR_ASSIGN_DESCR:  { extern void bb_gvar_assign_descr(void);  bb_gvar_assign_descr();  return 0; }
    case IR_SCAN: { extern void bb_scan_stmt(IR_t *); bb_scan_stmt(nd); return 0; }
    case IR_GEN_SCAN: { extern void bb_gen_scan(IR_t *); bb_gen_scan(nd); return 0; }
    case IR_KEYWORD: { extern void bb_keyword(IR_t *); bb_keyword(nd); return 0; }
    case IR_RETURN: { extern int g_descr_flat_chain; extern void bb_return(IR_t *);
        if (g_descr_flat_chain) { IR_t *rv = (nd->n_operands > 0 && nd->operands[0]) ? nd->operands[0] : (IR_t *)0; g_emit.op_sa = rv ? bb_slot_get(rv) : -1; bb_return(nd); return 0; }
        fprintf(out, "; [walk_bb_node: kind=%d unhandled]\n", (int)nd->op); return 1; }
    case IR_AUGOP:
    case IR_CALL: {
        extern int g_icn_scan_regs_live;
        if (g_icn_scan_regs_live && IR_LIT(nd).sval) {
            extern void bb_scan_pos(IR_t *); extern void bb_scan_any(IR_t *); extern void bb_scan_match(IR_t *);
            extern void bb_scan_many(IR_t *); extern void bb_scan_tab(IR_t *); extern void bb_scan_move(IR_t *);
            extern void bb_scan_upto(IR_t *); extern void bb_scan_find(IR_t *); extern void bb_scan_bal(IR_t *);
            if (!strcmp(IR_LIT(nd).sval, "pos"))   { bb_scan_pos(nd);   return 0; }
            if (!strcmp(IR_LIT(nd).sval, "any"))   { bb_scan_any(nd);   return 0; }
            if (!strcmp(IR_LIT(nd).sval, "match")) { bb_scan_match(nd); return 0; }
            if (!strcmp(IR_LIT(nd).sval, "many"))  { bb_scan_many(nd);  return 0; }
            if (!strcmp(IR_LIT(nd).sval, "tab"))   { bb_scan_tab(nd);   return 0; }
            if (!strcmp(IR_LIT(nd).sval, "move"))  { bb_scan_move(nd);  return 0; }
            if (!strcmp(IR_LIT(nd).sval, "upto"))  { bb_scan_upto(nd);  return 0; }
            if (!strcmp(IR_LIT(nd).sval, "find"))  { bb_scan_find(nd);  return 0; }
            if (!strcmp(IR_LIT(nd).sval, "bal"))   { bb_scan_bal(nd);   return 0; }
        }
        bb_call(nd);
        return 0;
    }
    case IR_VAR_FRAME:            { extern void bb_var_frame(IR_t *); bb_var_frame(nd); return 0; }
    case IR_ASSIGN_FRAME:         bb_prepare(nd); { extern void bb_assign_frame(void); bb_assign_frame(); } return 0;
    case IR_VAR_FRAME_REF:        { extern void bb_var_frame_ref(IR_t *); bb_var_frame_ref(nd); return 0; }
    case IR_ASSIGN_FRAME_REF:     bb_prepare(nd); { extern void bb_assign_frame_ref(void); bb_assign_frame_ref(); } return 0;
    case IR_BINOP_RELOP:          bb_binop_relop(nd);       return 0;
    case IR_BINOP_ARITH:          bb_binop_arith(nd);       return 0;
    case IR_BINOP_GVAR_ARITH:     bb_binop_gvar_arith(nd);  return 0;
    case IR_BINOP_GVAR_RELOP:     bb_binop_gvar_relop(nd);  return 0;
    case IR_BINOP_GVAR_ARITH_SLOT: bb_binop_gvar_arith_slot(nd); return 0;
    case IR_BINOP_CONCAT:         bb_binop_concat_slot(nd); return 0;
    case IR_SUCCEED:              bb_succeed();        return 0;
    case IR_EVERY:                bb_every(nd);        return 0;
    case IR_TO:
    case IR_TO_BY:                { extern void bb_to(IR_t *); bb_to(nd); } return 0;
    case IR_LIST_BANG:            bb_iterate(nd);      return 0;
    case IR_ALT:                  bb_prepare(nd); { extern void bb_alt(void); bb_alt(); } return 0;
    case IR_SCAN_POS:             { extern void bb_scan_pos(IR_t *); bb_scan_pos(nd); } return 0;
    case IR_SCAN_ANY:             { extern void bb_scan_any(IR_t *); bb_scan_any(nd); } return 0;
    case IR_SCAN_MATCH:           { extern void bb_scan_match(IR_t *); bb_scan_match(nd); } return 0;
    case IR_SCAN_MANY:            { extern void bb_scan_many(IR_t *); bb_scan_many(nd); } return 0;
    case IR_SCAN_TAB:             { extern void bb_scan_tab(IR_t *); bb_scan_tab(nd); } return 0;
    case IR_SCAN_MOVE:            { extern void bb_scan_move(IR_t *); bb_scan_move(nd); } return 0;
    case IR_SCAN_UPTO:            { extern void bb_scan_upto(IR_t *); bb_scan_upto(nd); } return 0;
    case IR_SCAN_FIND:            { extern void bb_scan_find(IR_t *); bb_scan_find(nd); } return 0;
    case IR_SCAN_BAL:             { extern void bb_scan_bal(IR_t *); bb_scan_bal(nd); } return 0;
    case IR_GATHER:               { extern void bb_gather(IR_t *); bb_gather(nd); } return 0;
    case IR_SIZE:                 bb_unop();        return 0;
    case IR_PAT_DEFER:            bb_match_defer();    return 0;
    case IR_CUT:             bb_cut();                                 return 0;
    case IR_DISJ:          bb_disj();                                 return 0;
    case IR_GCONJ:          bb_conj();                                 return 0;
    case IR_ITE:          bb_ite();                                 return 0;
    case IR_CATCH:        bb_prepare(nd); bb_catch();               return 0;
    case IR_UNIFY:           bb_prepare(nd); bb_unify();           return 0;
    case IR_GOAL:            bb_prepare(nd); bb_goal(nd);          return 0;
    case IR_BUILTIN:         bb_prepare(nd); bb_resolve(nd);       return 0;
    case IR_LOGICVAR:        bb_logicvar(nd);                      return 0;
    case IR_ATOM:            bb_prepare(nd); bb_atom();            return 0;
    case IR_CHOICE:          bb_choice(nd);                        return 0;
    case IR_QUERY_FRAME:     { extern void bb_query_frame(void); bb_query_frame(); return 0; }
    case IR_DET_WRITE:       { extern void bb_det_write(void); bb_det_write(); return 0; }
    case IR_DET_NL:          { extern void bb_det_nl(void); bb_det_nl(); return 0; }
    case IR_DET_IS:          { extern void bb_det_is(void); bb_prepare(nd); bb_det_is(); return 0; }
    case IR_DET_CMP:         { extern void bb_det_cmp(void); bb_prepare(nd); bb_det_cmp(); return 0; }
    case IR_CELL_UNIFY:      { extern void bb_cell_unify(void); bb_prepare(nd); bb_cell_unify(); return 0; }
    case IR_CELL_CHOICE:     { extern void bb_cell_choice(void); bb_prepare(nd); bb_cell_choice(); return 0; }
    case IR_CELL_CALL:       { extern void bb_cell_call(void); bb_prepare(nd); bb_cell_call(); return 0; }
    case IR_CALLEE_FRAME:    { extern void bb_callee_frame(void); bb_prepare(nd); bb_callee_frame(); return 0; }
    case IR_CELL_CUT:        { extern void bb_cell_cut(void); bb_cell_cut(); return 0; }
    case IR_CELL_ITE:        { extern void bb_cell_ite(void); bb_prepare(nd); bb_cell_ite(); return 0; }
    case IR_FAIL:            bb_fail(nd);                          return 0;
    case IR_NEG:
    case IR_POS:
    case IR_NONNULL:
    case IR_NULL_TEST:
    case IR_UNOP:
    case IR_NOT:                  bb_unop();           return 0;
    default:
        fprintf(out, "; [walk_bb_node: kind=%d unhandled]\n", (int)nd->op);
        return 1;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
char * walk_bb_node_str_c(IR_t * nd) {
    if (!nd) { char * e = (char *)malloc(1); if (e) e[0] = '\0'; return e; }
    char *   buf   = NULL;
    size_t   len   = 0;
    FILE *   mem   = open_memstream(&buf, &len);
    if (!mem) { char * e = (char *)malloc(1); if (e) e[0] = '\0'; return e; }
    FILE *   sv_bb = bb_emit_out;
    bb_emit_out    = mem;
    walk_bb_node(nd, mem);
    fflush(mem);
    fclose(mem);
    bb_emit_out    = sv_bb;
    emit_io_set_sink(sv_bb);
    if (!buf) { char * e = (char *)malloc(1); if (e) e[0] = '\0'; return e; }
    return buf;
}
/*--------------------------------------------------------------------------------------------------------------------*/
#define WASM_STRTAB_MAX 4096
#define WASM_STR_DATA_BASE 0x100000
typedef struct { const char * s; int addr; int len; } WasmStrEntry;
static WasmStrEntry g_wasm_strtab[WASM_STRTAB_MAX];
static int g_wasm_strtab_n = 0;
static int g_wasm_str_next = WASM_STR_DATA_BASE;
static void wasm_strtab_reset(void) { g_wasm_strtab_n = 0; g_wasm_str_next = WASM_STR_DATA_BASE; }
int wasm_intern_str(const char * s) {
    int len = s ? (int)strlen(s) : 0;
    for (int i = 0; i < g_wasm_strtab_n; i++)
        if (g_wasm_strtab[i].len == len && (len == 0 || memcmp(g_wasm_strtab[i].s, s, len) == 0)) return g_wasm_strtab[i].addr;
    if (g_wasm_strtab_n >= WASM_STRTAB_MAX) return WASM_STR_DATA_BASE;
    int addr = g_wasm_str_next;
    g_wasm_strtab[g_wasm_strtab_n].s    = s;
    g_wasm_strtab[g_wasm_strtab_n].addr = addr;
    g_wasm_strtab[g_wasm_strtab_n].len  = len;
    g_wasm_strtab_n++;
    g_wasm_str_next += len + 1;
    return addr;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int wasm_intern_name(const char * s) {
    if (!s) return wasm_intern_str(s);
    int len = (int)strlen(s);
    static char buf[256];
    if (len >= (int)sizeof(buf)) len = (int)sizeof(buf) - 1;
    for (int i = 0; i < len; i++) {
        unsigned char c = (unsigned char)s[i];
        buf[i] = (c >= 'a' && c <= 'z') ? (char)(c - 32) : (char)c;
    }
    buf[len] = '\0';
    return wasm_intern_str(buf);
}
/*--------------------------------------------------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C++" {
#endif
std::string wasm_emit_data_segments_str(void) {
    std::string r;
    for (int i = 0; i < g_wasm_strtab_n; i++) {
        const char * s   = g_wasm_strtab[i].s;
        int          len = g_wasm_strtab[i].len;
        int          adr = g_wasm_strtab[i].addr;
        r += emit_fmt("  (data (i32.const 0x%x) \"", adr);
        for (int j = 0; j < len; j++) {
            unsigned char c = (unsigned char)s[j];
            if (c == '"' || c == '\\') r += emit_fmt("\\%02x", (unsigned)c);
            else if (c < 32 || c > 126) r += emit_fmt("\\%02x", (unsigned)c);
            else r += emit_fmt("%c", (int)c);
        }
        r += emit_fmt("\\00\")  ;; len=%d\n", len);
    }
    return r;
}
/*--------------------------------------------------------------------------------------------------------------------*/
#ifdef __cplusplus
}
/*--------------------------------------------------------------------------------------------------------------------*/
#endif
#define WASM_USERFNS_MAX 256
#define WASM_MAX_PARAMS  16
typedef struct { char name[128]; int entry_pc; int nparams; char params[WASM_MAX_PARAMS][128]; } WasmUserFn;
static WasmUserFn g_wasm_userfns[WASM_USERFNS_MAX];
static int        g_wasm_userfns_n = 0;
static void wasm_userfns_reset(void) { g_wasm_userfns_n = 0; }
WasmUserFn * wasm_userfn_find(const char * name) {
    if (!name) return NULL;
    for (int i = 0; i < g_wasm_userfns_n; i++) if (strcmp(g_wasm_userfns[i].name, name) == 0) return &g_wasm_userfns[i];
    return NULL;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int wasm_parse_define_signature(WasmUserFn * fn, const char * sig) {
    if (!sig) return 0;
    const char * lp = strchr(sig, '(');
    const char * rp = lp ? strchr(lp + 1, ')') : NULL;
    if (!lp) { snprintf(fn->name, sizeof(fn->name), "%s", sig); fn->nparams = 0; return 1; }
    int nlen = (int)(lp - sig); if (nlen >= (int)sizeof(fn->name)) nlen = (int)sizeof(fn->name) - 1;
    memcpy(fn->name, sig, (size_t)nlen); fn->name[nlen] = '\0';
    fn->nparams = 0;
    if (!rp || rp <= lp + 1) return 1;
    const char * s = lp + 1;
    while (s < rp && fn->nparams < WASM_MAX_PARAMS) {
        while (s < rp && (*s == ' ' || *s == '\t' || *s == ',')) s++;
        const char * ps = s;
        while (s < rp && *s != ',' && *s != ' ' && *s != '\t') s++;
        int plen = (int)(s - ps); if (plen <= 0) continue;
        if (plen >= (int)sizeof(fn->params[0])) plen = (int)sizeof(fn->params[0]) - 1;
        memcpy(fn->params[fn->nparams], ps, (size_t)plen); fn->params[fn->nparams][plen] = '\0';
        fn->nparams++;
    }
    return 1;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void jvm_sanitize_name(char * dst, size_t dsz, const char * src) {
    size_t j = 0;
    for (size_t i = 0; src[i] && j + 1 < dsz; i++) {
        char c = src[i];
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_') dst[j++] = c;
        else dst[j++] = '_';
    }
    if (j == 0 && j + 4 < dsz) { dst[j++] = 's'; dst[j++] = 'n'; dst[j++] = 'o'; }
    dst[j] = '\0';
}
/*--------------------------------------------------------------------------------------------------------------------*/
static char ** net_parse_define_proto(const char * proto, char ** out_fname, int * out_n) {
    *out_fname = NULL; *out_n = 0;
    if (!proto) return NULL;
    const char * lp = strchr(proto, '(');
    const char * rp = lp ? strchr(lp, ')') : NULL;
    if (!lp) {
        size_t flen = strlen(proto); char * fn = (char *)malloc(flen + 1);
        memcpy(fn, proto, flen); fn[flen] = '\0'; *out_fname = fn; return NULL;
    }
    size_t flen = (size_t)(lp - proto); char * fn = (char *)malloc(flen + 1);
    memcpy(fn, proto, flen); fn[flen] = '\0'; *out_fname = fn;
    if (!rp || rp <= lp + 1) return NULL;
    int cap = 4, count = 0;
    char ** params = (char **)malloc((size_t)(cap + 1) * sizeof(char *));
    const char * s = lp + 1;
    while (s < rp) {
        while (s < rp && (*s == ' ' || *s == '\t')) s++;
        const char * pstart = s;
        while (s < rp && *s != ',' && *s != ' ' && *s != '\t') s++;
        size_t plen = (size_t)(s - pstart);
        if (plen > 0) {
            if (count >= cap) { cap *= 2; params = (char **)realloc(params, (size_t)(cap + 1) * sizeof(char *)); }
            char * p = (char *)malloc(plen + 1); memcpy(p, pstart, plen); p[plen] = '\0'; params[count++] = p;
        }
        while (s < rp && (*s == ' ' || *s == '\t')) s++;
        if (s < rp && *s == ',') s++;
    }
    params[count] = NULL; *out_n = count; return params;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int g_sno_m4_dense_nid = 0;
static IR_t * g_nid_key[262144];
static int    g_nid_val[262144];
static int    g_nid_count = 0;
int bb_node_id(IR_t * nd) {
    if (!nd) return 0;
    if (!g_sno_m4_dense_nid) return (int)((uintptr_t)nd % 100000u);
    uintptr_t h = ((uintptr_t)nd >> 4) & 262143u;
    while (g_nid_key[h]) { if (g_nid_key[h] == nd) return g_nid_val[h]; h = (h + 1u) & 262143u; }
    g_nid_key[h] = nd; g_nid_val[h] = ++g_nid_count; return g_nid_val[h];
}
int bb_is_generator(IR_e k) {
    if (k >= IR_PAT_LIT   && k <= IR_PAT_DEFER)  return 1;
    if (k >= IR_CHOICE && k <= IR_GOAL)      return 1;
    if (k >= IR_TO    && k <= IR_PROC_GEN) return 1;
    if (k == IR_SCAN || k == IR_TO_BY ||
        k == IR_EVERY || k == IR_WHILE    || k == IR_LIMIT || k == IR_SUSPEND) return 1;
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
#define IR_WALK_MAX 4096
static int g_visited[IR_WALK_MAX];
static int g_vcount = 0;
static void bb_walk_rec(IR_t * nd, void (*visit)(IR_t *, void *), void * ctx) {
    if (!nd) return;
    int id = bb_node_id(nd);
    for (int i = 0; i < g_vcount; i++) if (g_visited[i] == id) return;
    if (g_vcount < IR_WALK_MAX) g_visited[g_vcount++] = id;
    visit(nd, ctx);
    if (nd->op != IR_SCAN) for (int i = 0; i < nd->n_operands; i++) bb_walk_rec(nd->operands[i], visit, ctx);
    bb_walk_rec(nd->γ.node, visit, ctx); bb_walk_rec(nd->ω.node, visit, ctx);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void bb_walk(IR_graph_t * cfg, void (*visit)(IR_t *, void *), void * ctx) {
    if (!cfg || !cfg->entry) return;
    g_vcount = 0;
    bb_walk_rec(cfg->entry, visit, ctx);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void js_escape_string(FILE * out, const char * s) {
    fprintf(out, "\"");
    for (; s && *s; s++) {
        unsigned char c = (unsigned char)*s;
        if      (c == '"')  fprintf(out, "\\\"");
        else if (c == '\\') fprintf(out, "\\\\");
        else if (c == '\n') fprintf(out, "\\n");
        else if (c == '\r') fprintf(out, "\\r");
        else if (c == '\t') fprintf(out, "\\t");
        else if (c < 0x20 || c > 0x7e) fprintf(out, "\\x%02x", c);
        else fprintf(out, "%c", c);
    }
    fprintf(out, "\"");
}
/*--------------------------------------------------------------------------------------------------------------------*/
void xa_dispatch(XA_op_t op)
{
    switch (op) {
    case XA_MACRO_LIBRARY_OPEN:    xa_macro_library_open();    return;
    case XA_MACRO_LIBRARY_CLOSE:   xa_macro_library_close();   return;
    case XA_BB_MACRO_LIBRARY:      xa_bb_macro_library();      return;
    case XA_EXEC_STMT_BLOB:        xa_exec_stmt_blob();        return;
    case XA_FILE_HEADER:           xa_file_header();           return;
    case XA_FILE_FOOTER:           xa_file_footer();           return;
    case XA_BB_PTR_SLOT:           xa_bb_ptr_slot();           return;
    case XA_ENTRY_DISPATCH:        xa_entry_dispatch();        return;
    case XA_FLAT_PROLOGUE:         xa_flat_prologue();         return;
    case XA_FLAT_EPILOGUE:         xa_flat_epilogue();         return;
    case XA_FLAT_DATA_SECTION:     xa_flat_data_section();     return;
    case XA_PROLOGUE:              xa_prologue();              return;
    case XA_EPILOGUE:              xa_epilogue();              return;
    case XA_WASM_MAIN_OPEN:        xa_wasm_main_open();        return;
    case XA_WASM_MAIN_CLOSE:       xa_wasm_main_close();       return;
    case XA_JS_LABEL_REGISTER:     xa_js_label_register();     return;
    case XA_EXPRESSION_REGISTRY:   xa_expression_registry();   return;
    case XA_KIDS_RODATA:                                    return;
    case XA_SUB_BUILDER:                                    return;
    case XA_BUILDER:                                        return;
    case XA_REGISTRY_TABLE:                                 return;
    case XA_STRTAB_RODATA:         xa_strtab_rodata();         return;
    case XA_CAP_FIXUP:             xa_cap_fixup();             return;
    case XA_PATTERN_BLOBS:         xa_pattern_blobs();         return;
    default: return;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
#define SMX_STRTAB_CAP 8192
static struct { const char *s; int idx; } g_strtab[SMX_STRTAB_CAP];
static int g_strtab_n = 0;
void strtab_reset(void) { g_strtab_n = 0; }
int strtab_intern(const char *s)
{
    if (!s) s = "";
    for (int i = 0; i < g_strtab_n; i++)
        if (g_strtab[i].s == s || strcmp(g_strtab[i].s, s) == 0) return g_strtab[i].idx;
    if (g_strtab_n >= SMX_STRTAB_CAP) { fprintf(stderr, "strtab overflow\n"); abort(); }
    int idx = g_strtab_n;
    g_strtab[g_strtab_n].s = s; g_strtab[g_strtab_n].idx = idx; g_strtab_n++;
    return idx;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void strtab_label(char *buf, size_t bufsz, const char *s)
{
    if (!s) s = "";
    int idx = strtab_intern(s);
    snprintf(buf, bufsz, ".S%d", idx);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void xa_emit_strtab_rodata(void)
{
    if (g_strtab_n <= 0) { strtab_reset(); return; }
    static const char *labels[SMX_STRTAB_CAP];
    static std::string escbuf[SMX_STRTAB_CAP];
    static const char *escaped[SMX_STRTAB_CAP];
    static char        lblbuf[SMX_STRTAB_CAP][24];
    for (int i = 0; i < g_strtab_n; i++) {
        snprintf(lblbuf[i], sizeof lblbuf[i], ".S%d:", g_strtab[i].idx);
        labels[i]  = lblbuf[i];
        escbuf[i]  = gas_escape_str(g_strtab[i].s ? g_strtab[i].s : "");
        escaped[i] = escbuf[i].c_str();
    }
    g_emit.xa_strtab_n       = g_strtab_n;
    g_emit.xa_strtab_labels  = labels;
    g_emit.xa_strtab_escaped = escaped;
    xa_dispatch(XA_STRTAB_RODATA);
    g_emit.xa_strtab_n       = 0;
    g_emit.xa_strtab_labels  = NULL;
    g_emit.xa_strtab_escaped = NULL;
    strtab_reset();
}
