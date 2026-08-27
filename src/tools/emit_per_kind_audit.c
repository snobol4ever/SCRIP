#include "emit_per_kind_audit.h"
#include "IR.h"
#include "emit.h"
#include "SM.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
static IR_t g_audit_nodes[IR_OP_COUNT];
static IR_t  g_audit_child_lit_i;
static IR_t  g_audit_child_lit_s;
static IR_t  g_audit_child_var;
static IR_t *g_audit_child2[2];
static IR_t *g_audit_child3[3];
static bb_match_kids_state_t g_audit_kids1;
static bb_match_kids_state_t g_audit_kids2;
static bb_match_kids_state_t g_audit_kids3;
typedef struct { IR_e kind; const char *name; } kind_entry_t;
static const kind_entry_t g_kinds[] = {
    { IR_LIT_INTEGER,           "IR_LIT_INTEGER" },
    { IR_LIT_STRING,           "IR_LIT_STRING" },
    { IR_LIT_REAL,           "IR_LIT_REAL" },
    { IR_LIT_NUL,         "IR_LIT_NUL" },
    { IR_VAR,             "IR_VAR" },
    { IR_VAR_FRAME,       "IR_VAR_FRAME" },
    { IR_ASSIGN_FRAME,    "IR_ASSIGN_FRAME" },
    { IR_ASSIGN,          "IR_ASSIGN" },
    { IR_AUGOP,           "IR_AUGOP" },
    { IR_BINOP,           "IR_BINOP" },
    { IR_UNOP,            "IR_UNOP" },
    { IR_CALL,            "IR_CALL" },
    { IR_SEQ,             "IR_SEQ" },
    { IR_FAIL,            "IR_FAIL" },
    { IR_SUCCEED,         "IR_SUCCEED" },
    { IR_RETURN,          "IR_RETURN" },
    { IR_IF,              "IR_IF" },
    { IR_TO_BY,           "IR_TO_BY" },
    { IR_EVERY,           "IR_EVERY" },
    { IR_WHILE,           "IR_WHILE" },
    { IR_UNTIL,           "IR_UNTIL" },
    { IR_REPEAT,          "IR_REPEAT" },
    { IR_ALT,             "IR_ALT" },
    { IR_SIZE,            "IR_SIZE" },
    { IR_CASE,            "IR_CASE" },
    { IR_LIMIT,           "IR_LIMIT" },
    { IR_SUSPEND,         "IR_SUSPEND" },
    { IR_PROC,            "IR_PROC" },
    { IR_SCAN,            "IR_SCAN" },
    { IR_NONNULL,         "IR_NONNULL" },
    { IR_INTERROGATE,     "IR_INTERROGATE" },
    { IR_MATCH_LIT,         "IR_MATCH_LIT" },
    { IR_MATCH_ANY,         "IR_MATCH_ANY" },
    { IR_MATCH_SPAN,        "IR_MATCH_SPAN" },
    { IR_MATCH_SPAN_VAR,    "IR_MATCH_SPAN_VAR" },
    { IR_MATCH_BREAK,       "IR_MATCH_BREAK" },
    { IR_MATCH_BREAKX,      "IR_MATCH_BREAKX" },
    { IR_MATCH_ARB,         "IR_MATCH_ARB" },
    { IR_MATCH_ARBNO,       "IR_MATCH_ARBNO" },
    { IR_MATCH_ALTERNATE,         "IR_MATCH_ALTERNATE" },
    { IR_MATCH_ASSIGN_IMM,  "IR_MATCH_ASSIGN_IMM" },
    { IR_MATCH_ASSIGN_COND, "IR_MATCH_ASSIGN_COND" },
    { IR_MATCH_LEN,         "IR_MATCH_LEN" },
    { IR_MATCH_NOTANY,      "IR_MATCH_NOTANY" },
    { IR_MATCH_POS,         "IR_MATCH_POS" },
    { IR_MATCH_RPOS,        "IR_MATCH_RPOS" },
    { IR_MATCH_TAB,         "IR_MATCH_TAB" },
    { IR_MATCH_RTAB,        "IR_MATCH_RTAB" },
    { IR_MATCH_REM,         "IR_MATCH_REM" },
    { IR_MATCH_FENCE0,       "IR_MATCH_FENCE0" },
    { IR_MATCH_FENCE1,       "IR_MATCH_FENCE1" },
    { IR_MATCH_ABORT,       "IR_MATCH_ABORT" },
    { IR_MATCH_CALLOUT,     "IR_MATCH_CALLOUT" },
    { IR_CHOICE,       "IR_CHOICE" },
    { IR_UNIFY,        "IR_UNIFY" },
    { IR_CUT,          "IR_CUT" },
    { IR_GOAL,         "IR_GOAL" },
    { IR_BUILTIN,      "IR_BUILTIN" },
    { IR_LOGICVAR,          "IR_LOGICVAR" },
    { IR_ATOM,         "IR_ATOM" },
    { IR_ARITH,        "IR_ARITH" },
    { IR_DISJ,          "IR_DISJ" },
    { IR_GCONJ,          "IR_GCONJ" },
    { IR_TO,          "IR_TO" },
    { IR_UPTO,        "IR_UPTO" },
    { IR_TO_BY,       "IR_TO_BY" },
    { IR_ITERATE,     "IR_ITERATE" },
    { IR_GEN_ALT,   "IR_GEN_ALT" },
    { IR_LIMIT,       "IR_LIMIT" },
    { IR_TO_NESTED,   "IR_TO_NESTED" },
    { IR_PROC_GEN,    "IR_PROC_GEN" },
    { IR_BREAK,           "IR_BREAK" },
    { IR_NEXT,            "IR_NEXT" },
    { IR_IDENTICAL,       "IR_IDENTICAL" },
    { IR_NULL_TEST,       "IR_NULL_TEST" },
    { IR_RANDOM,          "IR_RANDOM" },
    { IR_NEG,             "IR_NEG" },
    { IR_POS,             "IR_POS" },
    { IR_CSET_COMPL,      "IR_CSET_COMPL" },
    { IR_CSET_UNION,      "IR_CSET_UNION" },
    { IR_CSET_DIFF,       "IR_CSET_DIFF" },
    { IR_CSET_INTER,      "IR_CSET_INTER" },
    { IR_GEN_SCAN,        "IR_GEN_SCAN" },
    { IR_KEYWORD_ICON,     "IR_KEYWORD_ICON" },
    { IR_KEYWORD_SNOBOL4,  "IR_KEYWORD_SNOBOL4" },
    { IR_IDX,         "IR_IDX" },
    { IR_SECTION,     "IR_SECTION" },
    { IR_LIST_BANG,   "IR_LIST_BANG" },
    { IR_RECORD_DEF,  "IR_RECORD_DEF" },
    { IR_FIELD_GET,   "IR_FIELD_GET" },
    { IR_FIELD_SET,   "IR_FIELD_SET" },
    { IR_IDX_SET,     "IR_IDX_SET" },
    { IR_KEY_GEN,     "IR_KEY_GEN" },
    { IR_SWAP,            "IR_SWAP" },
    { IR_CONJUNCTION,        "IR_CONJUNCTION" },
    { IR_INITIAL,         "IR_INITIAL" },
    { IR_LCONCAT,     "IR_LCONCAT" },
    { IR_FIND_GEN,    "IR_FIND_GEN" },
    { IR_SEQ_GEN,     "IR_SEQ_GEN" },
};
static const int g_kind_count = (int)(sizeof(g_kinds) / sizeof(g_kinds[0]));
typedef struct { SM_op_t op; const char *name; } sm_entry_t;
typedef struct {
    bb_emit_mode_t mode;
    const char    *name;
    const char    *ext;
} backend_entry_t;
static const sm_entry_t g_sm_ops[] = {
    { SM_LABEL,                 "SM_LABEL" },
    { SM_PUSH_LIT_I,            "SM_PUSH_LIT_I" },
    { SM_PUSH_LIT_S,            "SM_PUSH_LIT_S" },
    { SM_PUSH_LIT_CS,           "SM_PUSH_LIT_CS" },
    { SM_PUSH_LIT_F,            "SM_PUSH_LIT_F" },
    { SM_PUSH_NULL,             "SM_PUSH_NULL" },
    { SM_PUSH_NULL_NOFLIP,      "SM_PUSH_NULL_NOFLIP" },
    { SM_PUSH_VAR,              "SM_PUSH_VAR" },
    { SM_STORE_VAR,             "SM_STORE_VAR" },
    { SM_VOID_POP,              "SM_VOID_POP" },
    { SM_PUSH_EXPRESSION,       "SM_PUSH_EXPRESSION" },
    { SM_CALL_EXPRESSION,       "SM_CALL_EXPRESSION" },
    { SM_CONCAT,                "SM_CONCAT" },
    { SM_NEG,                   "SM_NEG" },
    { SM_COERCE_NUM,            "SM_COERCE_NUM" },
    { SM_EXP,                   "SM_EXP" },
    { SM_ADD,                   "SM_ADD" },
    { SM_SUB,                   "SM_SUB" },
    { SM_MUL,                   "SM_MUL" },
    { SM_DIV,                   "SM_DIV" },
    { SM_MOD,                   "SM_MOD" },
    { SM_INCR,                  "SM_INCR" },
    { SM_DECR,                  "SM_DECR" },
    { SM_LCOMP,                 "SM_LCOMP" },
    { SM_ACOMP,                 "SM_ACOMP" },
    { SM_STNO,                  "SM_STNO" },
    { SM_HALT,                  "SM_HALT" },
    { SM_JUMP,                  "SM_JUMP" },
    { SM_JUMP_S,                "SM_JUMP_S" },
    { SM_JUMP_F,                "SM_JUMP_F" },
    { SM_CALL_FN,               "SM_CALL_FN" },
    { SM_SUSPEND_VALUE,         "SM_SUSPEND_VALUE" },
    { SM_RETURN,                "SM_RETURN" },
    { SM_RETURN_S,              "SM_RETURN_S" },
    { SM_RETURN_F,              "SM_RETURN_F" },
    { SM_FRETURN,               "SM_FRETURN" },
    { SM_FRETURN_S,             "SM_FRETURN_S" },
    { SM_FRETURN_F,             "SM_FRETURN_F" },
    { SM_NRETURN,               "SM_NRETURN" },
    { SM_NRETURN_S,             "SM_NRETURN_S" },
    { SM_NRETURN_F,             "SM_NRETURN_F" },
    { SM_DEFINE_ENTRY,          "SM_DEFINE_ENTRY" },
    { SM_DEFINE,                "SM_DEFINE" },
    { SM_PAT_LIT,               "SM_PAT_LIT" },
    { SM_PAT_ANY,               "SM_PAT_ANY" },
    { SM_PAT_NOTANY,            "SM_PAT_NOTANY" },
    { SM_PAT_SPAN,              "SM_PAT_SPAN" },
    { SM_PAT_BREAK,             "SM_PAT_BREAK" },
    { SM_PAT_LEN,               "SM_PAT_LEN" },
    { SM_PAT_POS,               "SM_PAT_POS" },
    { SM_PAT_RPOS,              "SM_PAT_RPOS" },
    { SM_PAT_TAB,               "SM_PAT_TAB" },
    { SM_PAT_RTAB,              "SM_PAT_RTAB" },
    { SM_PAT_ARB,               "SM_PAT_ARB" },
    { SM_PAT_ARBNO,             "SM_PAT_ARBNO" },
    { SM_PAT_REM,               "SM_PAT_REM" },
    { SM_PAT_BAL,               "SM_PAT_BAL" },
    { SM_PAT_FENCE0,            "SM_PAT_FENCE0" },
    { SM_PAT_FENCE1,            "SM_PAT_FENCE1" },
    { SM_PAT_FAIL,              "SM_PAT_FAIL" },
    { SM_PAT_SUCCEED,           "SM_PAT_SUCCEED" },
    { SM_PAT_ABORT,             "SM_PAT_ABORT" },
    { SM_PAT_EPS,               "SM_PAT_EPS" },
    { SM_PAT_CAT,               "SM_PAT_CAT" },
    { SM_PAT_ALT,               "SM_PAT_ALT" },
    { SM_PAT_DEREF,             "SM_PAT_DEREF" },
    { SM_PAT_REFNAME,           "SM_PAT_REFNAME" },
    { SM_PAT_CAPTURE,           "SM_PAT_CAPTURE" },
    { SM_PAT_CAPTURE_FN,        "SM_PAT_CAPTURE_FN" },
    { SM_PAT_CAPTURE_FN_ARGS,   "SM_PAT_CAPTURE_FN_ARGS" },
    { SM_PAT_USERCALL,          "SM_PAT_USERCALL" },
    { SM_PAT_USERCALL_ARGS,     "SM_PAT_USERCALL_ARGS" },
    { SM_EXEC_STMT,             "SM_EXEC_STMT" },
};
static const int g_sm_count = (int)(sizeof(g_sm_ops) / sizeof(g_sm_ops[0]));
static const backend_entry_t g_sm_backends[] = {
    { EMIT_TEXT,             "x86/text",       "s"   },
    { EMIT_BINARY_WIRED,     "x86/binary",     "bin" },
    { EMIT_JVM,              "jvm/text",       "j"   },
    { EMIT_JS,               "js/text",        "js"  },
    { EMIT_NET,              "net/text",       "il"  },
    { EMIT_WASM,             "wasm/text",      "wat" },
};
static const int g_sm_backend_count =
    (int)(sizeof(g_sm_backends) / sizeof(g_sm_backends[0]));
static SM_t g_audit_sm[SM_OPCODE_COUNT];
static const backend_entry_t g_backends[] = {
    { EMIT_TEXT,             "x86/text",        "s"   },
    { EMIT_BINARY_WIRED,     "x86/binary",      "bin" },
    { EMIT_MACRO_DEF,        "x86/text_macro",  "s"   },
    { EMIT_JVM,              "jvm/text",        "j"   },
    { EMIT_JS,               "js/text",         "js"  },
    { EMIT_NET,              "net/text",        "il"  },
    { EMIT_WASM,             "wasm/text",       "wat" },
};
static const int g_backend_count = (int)(sizeof(g_backends) / sizeof(g_backends[0]));
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void prime_child_nodes(void) {
    memset(&g_audit_child_lit_i, 0, sizeof g_audit_child_lit_i);
    memset(&g_audit_child_lit_s, 0, sizeof g_audit_child_lit_s);
    memset(&g_audit_child_var,   0, sizeof g_audit_child_var);
    g_audit_child_lit_i.op = IR_LIT_INTEGER;
    g_audit_child_lit_i.ival = 42;
    g_audit_child_lit_i.α = &g_audit_child_lit_i;
    g_audit_child_lit_i.β = &g_audit_child_lit_i;
    g_audit_child_lit_s.op = IR_LIT_STRING;
    g_audit_child_lit_s.sval = "audit_str";
    g_audit_child_lit_s.α = &g_audit_child_lit_s;
    g_audit_child_lit_s.β = &g_audit_child_lit_s;
    g_audit_child_var.op = IR_VAR;
    g_audit_child_var.sval = "audit_var";
    g_audit_child_var.α = &g_audit_child_var;
    g_audit_child_var.β = &g_audit_child_var;
    g_audit_child2[0] = &g_audit_child_lit_i;
    g_audit_child2[1] = &g_audit_child_var;
    g_audit_child3[0] = &g_audit_child_lit_i;
    g_audit_child3[1] = &g_audit_child_var;
    g_audit_child3[2] = &g_audit_child_lit_s;
    g_audit_kids1.kids = g_audit_child2; g_audit_kids1.nkids = 1;
    g_audit_kids2.kids = g_audit_child2; g_audit_kids2.nkids = 2;
    g_audit_kids3.kids = g_audit_child3; g_audit_kids3.nkids = 3;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void prime_node_for_kind(IR_t *nd, IR_e kind) {
    memset(nd, 0, sizeof *nd);
    nd->op = kind;
    nd->α = nd;
    nd->β = nd;
    nd->γ.node = NULL;
    nd->ω.node = NULL;
    switch (kind) {
    case IR_LIT_INTEGER:        nd->ival = 42; break;
    case IR_LIT_REAL:        nd->dval = 3.14; break;
    case IR_LIT_STRING:        nd->sval = "audit_lit"; break;
    case IR_VAR:          nd->sval = "audit_var"; break;
    case IR_ASSIGN:       nd->sval = "audit_dst"; nd->α = g_audit_child2[0]; nd->β = g_audit_child2[1]; break;
    case IR_MATCH_LIT:      nd->sval = "audit"; break;
    case IR_MATCH_ANY:      nd->sval = "abc"; break;
    case IR_MATCH_SPAN:     nd->sval = "0123"; break;
    case IR_MATCH_SPAN_VAR: nd->sval = "V"; nd->ival = 1; break;
    case IR_MATCH_BREAK:    nd->sval = "."; break;
    case IR_MATCH_BREAKX:   nd->sval = "."; break;
    case IR_MATCH_NOTANY:   nd->sval = "xyz"; break;
    case IR_MATCH_LEN:      nd->ival = 5; break;
    case IR_MATCH_POS:      nd->ival = 0; break;
    case IR_MATCH_RPOS:     nd->ival = 0; break;
    case IR_MATCH_TAB:      nd->ival = 10; break;
    case IR_MATCH_RTAB:     nd->ival = 10; break;
    case IR_MATCH_ASSIGN_IMM:
    case IR_MATCH_ASSIGN_COND:
        nd->sval = "captured";
        (void)(&g_audit_kids1);
        break;
    case IR_MATCH_ALTERNATE:
    case IR_MATCH_ARBNO:
        (void)(&g_audit_kids2);
        break;
    case IR_MATCH_CALLOUT:  nd->sval = "audit_callout"; break;
    case IR_LOGICVAR:       nd->ival = 0; nd->sval = "X"; break;
    case IR_ATOM:      nd->sval = "foo"; break;
    case IR_GOAL:   nd->sval = "p"; nd->α = g_audit_child2[0]; nd->ival = 1; break;
    case IR_BUILTIN:   nd->sval = "atom_codes"; nd->α = g_audit_child2[0]; nd->β = g_audit_child2[1]; break;
    case IR_ARITH:     nd->sval = "+"; nd->α = g_audit_child2[0]; nd->β = g_audit_child2[1]; break;
    case IR_DISJ:
    case IR_GCONJ:
    case IR_CHOICE:
    case IR_UNIFY:
        nd->α = g_audit_child2[0]; nd->β = g_audit_child2[1];
        break;
    case IR_KEYWORD_ICON:  nd->sval = "&pos"; break;
    case IR_KEYWORD_SNOBOL4:  nd->sval = "&alphabet"; break;
    case IR_RECORD_DEF: nd->sval = "rec(a,b)"; break;
    case IR_FIELD_GET:
    case IR_FIELD_SET:
        nd->sval = "f"; nd->α = g_audit_child2[0]; nd->β = g_audit_child2[1];
        break;
    case IR_SECTION:
        nd->ival = 0;
        nd->α = g_audit_child3[0]; nd->β = g_audit_child3[1]; nd->γ.node = g_audit_child3[2]; memcpy(nd->γ.sz, "α", 3);
        break;
    case IR_BINOP:
    case IR_AUGOP:
        nd->sval = "+"; nd->α = g_audit_child2[0]; nd->β = g_audit_child2[1];
        break;
    case IR_UNOP:
        nd->sval = "-"; nd->α = g_audit_child2[0];
        break;
    case IR_CALL:
        nd->sval = "f"; nd->α = g_audit_child2[0];
        break;
    default:
        break;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int mkdir_p(const char *path) {
    char tmp[1024];
    snprintf(tmp, sizeof tmp, "%s", path);
    size_t n = strlen(tmp);
    if (n == 0) return 0;
    if (tmp[n - 1] == '/') tmp[n - 1] = '\0';
    for (char *p = tmp + 1; *p; p++) {
        if (*p == '/') {
            *p = '\0';
            if (mkdir(tmp, 0755) != 0 && errno != EEXIST) return -1;
            *p = '/';
        }
    }
    if (mkdir(tmp, 0755) != 0 && errno != EEXIST) return -1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int emit_one_cell(const char *out_dir,
                         const backend_entry_t *be,
                         const kind_entry_t   *ke,
                         IR_t                 *nd,
                         int                  *out_zero_bytes)
{
    char dir_path[1024], file_path[1024];
    snprintf(dir_path,  sizeof dir_path,  "%s/%s", out_dir, be->name);
    snprintf(file_path, sizeof file_path, "%s/%s.%s", dir_path, ke->name, be->ext);
    if (mkdir_p(dir_path) != 0) {
        fprintf(stderr, "audit: mkdir_p(%s) failed: %s\n", dir_path, strerror(errno));
        return 1;
    }
    FILE *out = fopen(file_path, "wb");
    if (!out) {
        fprintf(stderr, "AUDIT-ERR open %s: errno\n", file_path);
        return 1;
    }
    static uint8_t bin_buf[16 * 1024];
    int is_bin = (be->mode == EMIT_BINARY_WIRED);
    if (is_bin) {
        emitter_init_binary((bb_buf_t)bin_buf, (int)sizeof bin_buf);
        bb_emit_out = NULL;
        emit_mode_set(be->mode, NULL);
    } else {
        emitter_init_text(out, TEXT_MODE_INVOCATION);
        emit_mode_set(be->mode, out);
    }
    g_emit.backend  = be->mode;
    g_emit.is_binary = (be->mode == EMIT_BINARY_WIRED);
    emit_set_sink(out);
    g_emit.node = nd;
    g_emit.sid  = 0;
    g_emit.nid  = (int)nd->op;
    g_emit.lbl_γ = "L_succ_audit";
    g_emit.lbl_ω = "L_fail_audit";
    g_emit.lbl_β = "L_back_audit";
    static bb_label_t s_audit_succ = { "L_succ_audit", -1 };
    static bb_label_t s_audit_fail = { "L_fail_audit", -1 };
    static bb_label_t s_audit_back = { "L_back_audit", -1 };
    s_audit_succ.offset = -1; s_audit_fail.offset = -1; s_audit_back.offset = -1;
    g_emit.lbl_γ_p = is_bin ? &s_audit_succ : NULL;
    g_emit.lbl_ω_p = is_bin ? &s_audit_fail : NULL;
    g_emit.lbl_β_p = is_bin ? &s_audit_back : NULL;
    g_emit.child_fn = NULL;
    g_emit.op_name1 = nd->sval;
    g_emit.op_name2 = NULL;
    g_emit.op_kind  = NULL;
    int rc;
    if (bb_kind_is_driver_owned((int)nd->op) && be->mode != EMIT_JVM && be->mode != EMIT_JS && be->mode != EMIT_NET && be->mode != EMIT_WASM) {
        extern void walk_bb_flat(IR_t *, bb_label_t *, bb_label_t *, bb_label_t *);
        walk_bb_flat(nd, &s_audit_succ, &s_audit_fail, &s_audit_back);
        rc = 0;
    } else {
        rc = walk_bb_node(nd, out);
    }
    if (is_bin) {
        extern int bb_emit_pos;
        extern int bb_patch_count;
        int n = bb_emit_pos;
        if (n > 0) fwrite(bin_buf, 1, (size_t)n, out);
        bb_patch_count = 0;
    }
    fclose(out);
    struct stat st;
    if (stat(file_path, &st) == 0 && st.st_size == 0) (*out_zero_bytes)++;
    return rc;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void prime_sm_for_op(SM_t *ins, SM_op_t op) {
    memset(ins, 0, sizeof *ins);
    ins->op = op;
    ins->a[0].i = 42; ins->a[0].s = "audit_arg";
    ins->a[1].i = 7;  ins->a[1].s = "audit_arg2";
    ins->a[2].i = 1;  ins->a[2].s = "audit_arg3";
    switch (op) {
    case SM_PUSH_LIT_F:
        ins->a[0].f = 3.14; break;
    case SM_PUSH_LIT_I:
    case SM_PUSH_NULL:
    case SM_PUSH_NULL_NOFLIP:
        ins->a[0].s = NULL; ins->a[0].i = 42; break;
    case SM_LABEL:
        ins->a[0].i = 0; ins->a[0].s = "L0"; break;
    case SM_JUMP: case SM_JUMP_S: case SM_JUMP_F:
        ins->a[0].s = NULL; ins->a[0].i = 0;
        ins->a[1].s = "L_target";
        break;
    case SM_CALL_FN:
        ins->a[0].s = "audit_fn";
        ins->a[1].s = NULL; ins->a[1].i = 1;
        break;
    case SM_EXEC_STMT:
        ins->a[0].s = "audit_subj";
        ins->a[1].s = NULL; ins->a[1].i = 0;
        ins->a[2].s = NULL; ins->a[2].i = 0;
        break;
    case SM_STNO:
        ins->a[0].s = NULL; ins->a[0].i = 1;
        ins->a[1].s = NULL; ins->a[1].i = 1;
        ins->a[2].s = "audit_src"; break;
    case SM_PAT_USERCALL_ARGS:
        ins->a[0].s = "audit_fn";
        ins->a[1].s = NULL; ins->a[1].i = 2;
        break;
    case SM_PAT_CAPTURE_FN_ARGS:
        ins->a[0].s = "audit_fn";
        ins->a[1].i = 0;
        ins->a[2].s = NULL; ins->a[2].i = 2;
        break;
    case SM_PUSH_EXPRESSION:
        ins->a[0].s = NULL; ins->a[0].i = 0;
        ins->a[1].s = NULL; ins->a[1].i = 2;
        break;
    case SM_DEFINE_ENTRY:
    case SM_DEFINE:
        ins->a[0].s = "audit_fn_def";
        ins->a[1].s = NULL; ins->a[1].i = 2;
        break;
    default:
        break;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int emit_one_sm_cell(const char *out_dir,
                            const backend_entry_t *be,
                            const sm_entry_t      *oe,
                            SM_t                  *ins)
{
    char dir_path[1024], file_path[1024];
    snprintf(dir_path,  sizeof dir_path,  "%s/%s", out_dir, be->name);
    snprintf(file_path, sizeof file_path, "%s/%s.%s", dir_path, oe->name, be->ext);
    if (mkdir_p(dir_path) != 0) {
        fprintf(stderr, "audit: mkdir_p(%s) failed: %s\n", dir_path, strerror(errno));
        return 1;
    }
    FILE *out = fopen(file_path, "wb");
    if (!out) {
        fprintf(stderr, "AUDIT-ERR open %s: %s\n", file_path, strerror(errno));
        return 1;
    }
    emitter_init_text(out, TEXT_MODE_INVOCATION);
    emit_mode_set(be->mode, out);
    g_emit.backend  = be->mode;
    g_emit.is_binary = (be->mode == EMIT_BINARY_WIRED);
    emit_set_sink(out);
    g_emit.instr = ins;
    g_emit.i    = 0;
    g_emit.n    = 1;
    g_emit.sid  = 0;
    g_emit.nid  = (int)ins->op;
    g_emit.node = NULL;
    g_emit.lbl_γ = "L_succ_audit";
    g_emit.lbl_ω = "L_fail_audit";
    g_emit.lbl_β = "L_back_audit";
    g_emit.enclosing_fname = NULL;
    g_emit.prev_instr_name = "";
    (void)codegen_sm_dispatch(ins);
    fclose(out);
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int emit_per_kind_audit_run(const char *out_dir) {
    if (!out_dir || !*out_dir) {
        fprintf(stderr, "audit: --audit-per-kind requires a directory argument\n");
        return 1;
    }
    if (mkdir(out_dir, 0755) != 0 && errno != EEXIST) {
        fprintf(stderr, "audit: mkdir(%s) failed: %s\n", out_dir, strerror(errno));
        return 1;
    }
    prime_child_nodes();
    int ok_cells = 0, zero_cells = 0, fail_cells = 0;
    for (int b = 0; b < g_backend_count; b++) {
        const backend_entry_t *be = &g_backends[b];
        for (int k = 0; k < g_kind_count; k++) {
            const kind_entry_t *ke = &g_kinds[k];
            IR_t *nd = &g_audit_nodes[(int)ke->kind];
            prime_node_for_kind(nd, ke->kind);
            int zero_before = zero_cells;
            int rc = emit_one_cell(out_dir, be, ke, nd, &zero_cells);
            if (rc) {
                fail_cells++;
                fprintf(stderr, "audit FAIL %s/%s\n", be->name, ke->name);
            } else if (zero_cells > zero_before) {
                ok_cells++;
            } else {
                ok_cells++;
            }
        }
    }
    int sm_ok = 0, sm_fail = 0;
    for (int b = 0; b < g_sm_backend_count; b++) {
        const backend_entry_t *be = &g_sm_backends[b];
        for (int o = 0; o < g_sm_count; o++) {
            const sm_entry_t *oe = &g_sm_ops[o];
            SM_t *ins = &g_audit_sm[(int)oe->op];
            prime_sm_for_op(ins, oe->op);
            int rc = emit_one_sm_cell(out_dir, be, oe, ins);
            if (rc) {
                sm_fail++;
                fprintf(stderr, "audit FAIL %s/%s\n", be->name, oe->name);
            } else {
                sm_ok++;
            }
        }
    }
    fprintf(stdout,
        "audit summary: BB %d cells (%d non-empty stub-aware), SM %d cells, "
        "%d failed total\n",
        ok_cells + fail_cells,
        ok_cells - zero_cells,
        sm_ok + sm_fail,
        fail_cells + sm_fail);
    return (fail_cells + sm_fail) ? 1 : 0;
}
