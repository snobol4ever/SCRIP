#include "emit_per_kind_audit.h"
#include "BB.h"
#include "SM.h"
#include "emit_bb.h"
#include "emit_core.h"
#include "emit_globals.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
static IR_t g_audit_nodes[BB_OP_COUNT];
static IR_t  g_audit_child_lit_i;
static IR_t  g_audit_child_lit_s;
static IR_t  g_audit_child_var;
static IR_t *g_audit_child2[2];
static IR_t *g_audit_child3[3];
static bb_pat_kids_state_t g_audit_kids1;
static bb_pat_kids_state_t g_audit_kids2;
static bb_pat_kids_state_t g_audit_kids3;
typedef struct { IR_e kind; const char *name; } kind_entry_t;
static const kind_entry_t g_kinds[] = {
    { BB_LIT_I,           "BB_LIT_I" },
    { BB_LIT_S,           "BB_LIT_S" },
    { BB_LIT_F,           "BB_LIT_F" },
    { BB_LIT_NUL,         "BB_LIT_NUL" },
    { BB_VAR,             "BB_VAR" },
    { BB_ASSIGN,          "BB_ASSIGN" },
    { BB_AUGOP,           "BB_AUGOP" },
    { BB_BINOP,           "BB_BINOP" },
    { BB_UNOP,            "BB_UNOP" },
    { BB_CALL,            "BB_CALL" },
    { BB_SEQ,             "BB_SEQ" },
    { BB_FAIL,            "BB_FAIL" },
    { BB_SUCCEED,         "BB_SUCCEED" },
    { BB_GOTO,            "BB_GOTO" },
    { BB_RETURN,          "BB_RETURN" },
    { BB_IF,              "BB_IF" },
    { BB_TO_BY,           "BB_TO_BY" },
    { BB_EVERY,           "BB_EVERY" },
    { BB_WHILE,           "BB_WHILE" },
    { BB_UNTIL,           "BB_UNTIL" },
    { BB_REPEAT,          "BB_REPEAT" },
    { BB_ALT,             "BB_ALT" },
    { BB_SIZE,            "BB_SIZE" },
    { BB_CASE,            "BB_CASE" },
    { BB_LIMIT,           "BB_LIMIT" },
    { BB_SUSPEND,         "BB_SUSPEND" },
    { BB_PROC,            "BB_PROC" },
    { BB_SCAN,            "BB_SCAN" },
    { BB_NONNULL,         "BB_NONNULL" },
    { BB_INTERROGATE,     "BB_INTERROGATE" },
    { BB_NOT,             "BB_NOT" },
    { BB_PAT_LIT,         "BB_PAT_LIT" },
    { BB_PAT_ANY,         "BB_PAT_ANY" },
    { BB_PAT_SPAN,        "BB_PAT_SPAN" },
    { BB_PAT_BREAK,       "BB_PAT_BREAK" },
    { BB_PAT_ARB,         "BB_PAT_ARB" },
    { BB_PAT_ARBNO,       "BB_PAT_ARBNO" },
    { BB_PAT_CAT,         "BB_PAT_CAT" },
    { BB_PAT_ALT,         "BB_PAT_ALT" },
    { BB_PAT_ASSIGN_IMM,  "BB_PAT_ASSIGN_IMM" },
    { BB_PAT_ASSIGN_COND, "BB_PAT_ASSIGN_COND" },
    { BB_PAT_LEN,         "BB_PAT_LEN" },
    { BB_PAT_NOTANY,      "BB_PAT_NOTANY" },
    { BB_PAT_POS,         "BB_PAT_POS" },
    { BB_PAT_TAB,         "BB_PAT_TAB" },
    { BB_PAT_REM,         "BB_PAT_REM" },
    { BB_PAT_FENCE,       "BB_PAT_FENCE" },
    { BB_PAT_ABORT,       "BB_PAT_ABORT" },
    { BB_PAT_CALLOUT,     "BB_PAT_CALLOUT" },
    { BB_CHOICE,       "BB_CHOICE" },
    { BB_UNIFY,        "BB_UNIFY" },
    { BB_CUT,          "BB_CUT" },
    { BB_GOAL,         "BB_GOAL" },
    { BB_BUILTIN,      "BB_BUILTIN" },
    { BB_LOGICVAR,          "BB_LOGICVAR" },
    { BB_ATOM,         "BB_ATOM" },
    { BB_ARITH,        "BB_ARITH" },
    { BB_DISJ,          "BB_DISJ" },
    { BB_GCONJ,          "BB_GCONJ" },
    { BB_TO,          "BB_TO" },
    { BB_UPTO,        "BB_UPTO" },
    { BB_TO_BY,       "BB_TO_BY" },
    { BB_ITERATE,     "BB_ITERATE" },
    { BB_GEN_ALT,   "BB_GEN_ALT" },
    { BB_LIMIT,       "BB_LIMIT" },
    { BB_GEN_BINOP,       "BB_GEN_BINOP" },
    { BB_TO_NESTED,   "BB_TO_NESTED" },
    { BB_PROC_GEN,    "BB_PROC_GEN" },
    { BB_BREAK,           "BB_BREAK" },
    { BB_NEXT,            "BB_NEXT" },
    { BB_IDENTICAL,       "BB_IDENTICAL" },
    { BB_NULL_TEST,       "BB_NULL_TEST" },
    { BB_RANDOM,          "BB_RANDOM" },
    { BB_NEG,             "BB_NEG" },
    { BB_POS,             "BB_POS" },
    { BB_CSET_COMPL,      "BB_CSET_COMPL" },
    { BB_CSET_UNION,      "BB_CSET_UNION" },
    { BB_CSET_DIFF,       "BB_CSET_DIFF" },
    { BB_CSET_INTER,      "BB_CSET_INTER" },
    { BB_GEN_SCAN,        "BB_GEN_SCAN" },
    { BB_KEYWORD,     "BB_KEYWORD" },
    { BB_BINOP_GEN,       "BB_BINOP_GEN" },
    { BB_IDX,         "BB_IDX" },
    { BB_SECTION,     "BB_SECTION" },
    { BB_LIST_BANG,   "BB_LIST_BANG" },
    { BB_RECORD_DEF,  "BB_RECORD_DEF" },
    { BB_FIELD_GET,   "BB_FIELD_GET" },
    { BB_FIELD_SET,   "BB_FIELD_SET" },
    { BB_IDX_SET,     "BB_IDX_SET" },
    { BB_KEY_GEN,     "BB_KEY_GEN" },
    { BB_SWAP,            "BB_SWAP" },
    { BB_SEQ_EXPR,        "BB_SEQ_EXPR" },
    { BB_INITIAL,         "BB_INITIAL" },
    { BB_LCONCAT,     "BB_LCONCAT" },
    { BB_FIND_GEN,    "BB_FIND_GEN" },
    { BB_SEQ_GEN,     "BB_SEQ_GEN" },
};
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const int g_kind_count = (int)(sizeof(g_kinds) / sizeof(g_kinds[0]));
typedef struct { SM_op_t op; const char *name; } sm_entry_t;
typedef struct {
    bb_emit_mode_t mode;
    const char    *name;
    const char    *ext;
} backend_entry_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const int g_sm_count = (int)(sizeof(g_sm_ops) / sizeof(g_sm_ops[0]));
static const backend_entry_t g_sm_backends[] = {
    { EMIT_TEXT,             "x86/text",       "s"   },
    { EMIT_BINARY_WIRED,     "x86/binary",     "bin" },
    { EMIT_JVM,              "jvm/text",       "j"   },
    { EMIT_JS,               "js/text",        "js"  },
    { EMIT_NET,              "net/text",       "il"  },
    { EMIT_WASM,             "wasm/text",      "wat" },
};
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const int g_backend_count = (int)(sizeof(g_backends) / sizeof(g_backends[0]));
static void prime_child_nodes(void) {
    memset(&g_audit_child_lit_i, 0, sizeof g_audit_child_lit_i);
    memset(&g_audit_child_lit_s, 0, sizeof g_audit_child_lit_s);
    memset(&g_audit_child_var,   0, sizeof g_audit_child_var);
    g_audit_child_lit_i.t = BB_LIT_I;
    g_audit_child_lit_i.ival = 42;
    g_audit_child_lit_i.α = &g_audit_child_lit_i;
    g_audit_child_lit_i.β = &g_audit_child_lit_i;
    g_audit_child_lit_s.t = BB_LIT_S;
    g_audit_child_lit_s.sval = "audit_str";
    g_audit_child_lit_s.α = &g_audit_child_lit_s;
    g_audit_child_lit_s.β = &g_audit_child_lit_s;
    g_audit_child_var.t = BB_VAR;
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
    nd->t = kind;
    nd->α = nd;
    nd->β = nd;
    nd->γ = NULL;
    nd->ω = NULL;
    switch (kind) {
    case BB_LIT_I:        nd->ival = 42; break;
    case BB_LIT_F:        nd->dval = 3.14; break;
    case BB_LIT_S:        nd->sval = "audit_lit"; break;
    case BB_VAR:          nd->sval = "audit_var"; break;
    case BB_ASSIGN:       nd->sval = "audit_dst"; nd->α = g_audit_child2[0]; nd->β = g_audit_child2[1]; break;
    case BB_PAT_LIT:      nd->sval = "audit"; break;
    case BB_PAT_ANY:      nd->sval = "abc"; break;
    case BB_PAT_SPAN:     nd->sval = "0123"; break;
    case BB_PAT_BREAK:    nd->sval = "."; break;
    case BB_PAT_NOTANY:   nd->sval = "xyz"; break;
    case BB_PAT_LEN:      nd->ival = 5; break;
    case BB_PAT_POS:      nd->ival = 0; break;
    case BB_PAT_TAB:      nd->ival = 10; break;
    case BB_PAT_ASSIGN_IMM:
    case BB_PAT_ASSIGN_COND:
        nd->sval = "captured";
        nd->counter = (int64_t)(intptr_t)&g_audit_kids1;
        break;
    case BB_PAT_CAT:
    case BB_PAT_ALT:
    case BB_PAT_ARBNO:
        nd->counter = (int64_t)(intptr_t)&g_audit_kids2;
        break;
    case BB_PAT_CALLOUT:  nd->sval = "audit_callout"; break;
    case BB_LOGICVAR:       nd->ival = 0; nd->sval = "X"; break;
    case BB_ATOM:      nd->sval = "foo"; break;
    case BB_GOAL:   nd->sval = "p"; nd->α = g_audit_child2[0]; nd->ival = 1; break;
    case BB_BUILTIN:   nd->sval = "atom_codes"; nd->α = g_audit_child2[0]; nd->β = g_audit_child2[1]; break;
    case BB_ARITH:     nd->sval = "+"; nd->α = g_audit_child2[0]; nd->β = g_audit_child2[1]; break;
    case BB_DISJ:
    case BB_GCONJ:
    case BB_CHOICE:
    case BB_UNIFY:
        nd->α = g_audit_child2[0]; nd->β = g_audit_child2[1];
        break;
    case BB_KEYWORD:  nd->sval = "&pos"; break;
    case BB_RECORD_DEF: nd->sval = "rec(a,b)"; break;
    case BB_FIELD_GET:
    case BB_FIELD_SET:
        nd->sval = "f"; nd->α = g_audit_child2[0]; nd->β = g_audit_child2[1];
        break;
    case BB_SECTION:
        nd->ival = 0;
        nd->α = g_audit_child3[0]; nd->β = g_audit_child3[1]; nd->γ = g_audit_child3[2];
        break;
    case BB_BINOP:
    case BB_AUGOP:
    case BB_BINOP_GEN:
        nd->sval = "+"; nd->α = g_audit_child2[0]; nd->β = g_audit_child2[1];
        break;
    case BB_UNOP:
        nd->sval = "-"; nd->α = g_audit_child2[0];
        break;
    case BB_CALL:
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
    g_emit.is_binary = (be->mode == EMIT_BINARY_WIRED || be->mode == EMIT_BINARY_BROKERED);
    emit_io_set_sink(out);
    g_emit.node = nd;
    g_emit.sid  = 0;
    g_emit.nid  = (int)nd->t;
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
    if (bb_kind_is_driver_owned((int)nd->t) && be->mode != EMIT_JVM && be->mode != EMIT_JS && be->mode != EMIT_NET && be->mode != EMIT_WASM) {
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
    g_emit.is_binary = (be->mode == EMIT_BINARY_WIRED || be->mode == EMIT_BINARY_BROKERED);
    emit_io_set_sink(out);
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
