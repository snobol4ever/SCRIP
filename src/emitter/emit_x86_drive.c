#include "emit_bb.h"
#include "emit_core.h"
#include "emit_globals.h"
#include "emit_drive.h"
#include "emit_x86_drive.h"
#include "IR.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
extern int           bb_slot_get(IR_t *nd);
extern void          bb_slot_register(IR_t *nd, int off);
extern int           bb_slot_alloc16(IR_t *nd);
extern int           bb_slot_alloc16_or_get(IR_t *nd);
extern int           bb_slot_claim(int bytes);
extern int           bb_varslot(const char *name);
extern int           bb_varslot_peek(const char *name);
extern int           is_global(const char *name);
extern int           gva_index_of(const char *name);
extern int           g_gva_active;
extern IR_graph_t *  g_emit_cfg;
extern int           g_descr_flat_chain;
/*====================================================================================================================*/
/*--------------------------------------------------------------------------------------------------------------------*/
#define DRIVE_FILL(nd,s,f,b) do { \
    bb_fill_alpha(nd); \
    g_emit.lbl_γ=(s)->name; g_emit.lbl_ω=(f)->name; g_emit.lbl_β=(b)->name; \
    g_emit.lbl_γ_p=(s); g_emit.lbl_ω_p=(f); g_emit.lbl_β_p=(b); \
    walk_bb_node((nd), emit_outf()); } while(0)
#define DRIVE_PAIR_RESET()      do { g_emit.xa_bb_emit_pair_n = 0; } while(0)
#define DRIVE_PAIR_JMP(tgt)     do { int _i=g_emit.xa_bb_emit_pair_n++; g_emit.xa_bb_emit_pair_define[_i]=NULL; g_emit.xa_bb_emit_pair_jmp[_i]=(tgt); } while(0)
#define DRIVE_PAIR_DEF_JMP(l,t) do { int _i=g_emit.xa_bb_emit_pair_n++; g_emit.xa_bb_emit_pair_define[_i]=(l); g_emit.xa_bb_emit_pair_jmp[_i]=(t); } while(0)
/*--------------------------------------------------------------------------------------------------------------------*/
static int drive_value_slot(IR_t *nd) {
    int e = bb_slot_get(nd);
    if (e >= 0) return e;
    if (nd && nd->tmp >= 0) { bb_slot_register(nd, nd->tmp); bb_flat_cursor_reserve(nd->tmp + 16); return nd->tmp; }
    return bb_slot_alloc16(nd);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static void drive_unowned(IR_t *nd) {
    fprintf(stderr, "FATAL emit_drive: IR op=%d has no template in the universal driver. Every op must be handled; the driver never declines silently. Implement op=%d.\n", nd ? (int)nd->op : -1, nd ? (int)nd->op : -1);
    abort();
}
/*--------------------------------------------------------------------------------------------------------------------*/
void emit_x86_drive(IR_t *nd, bb_label_t *lbl_γ, bb_label_t *lbl_ω, bb_label_t *lbl_β) {
    if (!nd) { drive_unowned(nd); return; }
    switch (nd->op) {
    case IR_LIT_S: case IR_LIT_I: case IR_LIT_F:
        g_emit.op_off = drive_value_slot(nd); DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_KEYWORD:
        g_emit.op_sval = IR_LIT(nd).sval; g_emit.op_off = drive_value_slot(nd); DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_VAR: {
        const char *vn = IR_LIT(nd).sval;
        if (vn && vn[0] == '&') { g_emit.op_sval = vn; g_emit.op_sa = -1; g_emit.op_off = drive_value_slot(nd); }
        else if (vn && is_global(vn)) { g_emit.op_sa = -1; g_emit.op_off = drive_value_slot(nd); g_emit.op_sval = vn; g_emit.op_gva_k = g_gva_active ? gva_index_of(vn) : -1; }
        else if (vn) { int voff = bb_varslot_peek(vn); g_emit.op_sa = voff; if (voff >= 0) { g_emit.op_off = voff; if (bb_slot_get(nd) < 0) bb_slot_register(nd, voff); } else g_emit.op_off = -1; }
        else { g_emit.op_sa = -1; g_emit.op_off = -1; }
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_BINOP: case IR_BINOP_RELOP: {
        g_emit.op_relop_descr = 0; g_emit.op_num_real = 0; g_emit.op_arith_descr = 0; g_emit.op_gva_k1 = -1; g_emit.op_gva_k2 = -1;
        int sa = -1, sb = -1;
        if (binop_is_num_real(g_emit_cfg, nd)) { int ra = bb_slot_get(bb_child0(nd)), rb = bb_slot_get(bb_child1(nd)); if (ra >= 0 && rb >= 0) { sa = ra; sb = rb; g_emit.op_num_real = 1; } }
        if (!g_emit.op_num_real) { sa = descr_binop_opnd_slot(bb_child0(nd)); sb = descr_binop_opnd_slot(bb_child1(nd)); }
        if (sa < 0 || sb < 0) { drive_unowned(nd); break; }
        g_emit.op_sa = sa; g_emit.op_sb = sb; g_emit.op_off = drive_value_slot(nd); g_emit.op_binop_kind = (int)binop_slot_kind(nd);
        DRIVE_PAIR_RESET(); DRIVE_PAIR_DEF_JMP(lbl_β, lbl_ω); DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_UNOP: case IR_NOT: {
        int sa = descr_binop_opnd_slot(bb_child0(nd));
        if (sa < 0) { drive_unowned(nd); break; }
        g_emit.op_sa = sa; g_emit.op_off = drive_value_slot(nd); DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_ASSIGN: {
        const char *vn = IR_LIT(nd).sval;
        if (!vn || is_global(vn)) { drive_unowned(nd); break; }
        g_emit.op_sb = bb_varslot(vn); g_emit.op_off = drive_value_slot(nd); DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_CALL: case IR_CALL_BUILTIN: case IR_CALL_PROC_STAGED: case IR_CALL_USERPROC: case IR_CALL_BYNAME: case IR_CALL_GVAR_USERPROC: {
        int na = nd->n_operands; if (na > OP_ARG_SLOT_MAX) na = OP_ARG_SLOT_MAX;
        for (int i = 0; i < na; i++) { IR_t * a = ir_call_arg(nd, i); g_emit.op_arg_slot[i] = (a && a->tmp >= 0) ? a->tmp : -1; }
        g_emit.op_arg_slot_n = na; g_emit.op_write_route = bb_call_write_route(nd);
        DRIVE_PAIR_RESET(); DRIVE_PAIR_DEF_JMP(lbl_β, lbl_ω); DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_TO: {
        if (!bb_child0(nd) || !bb_child1(nd)) { drive_unowned(nd); break; }
        g_emit.op_sa = bb_slot_get(bb_child0(nd)); g_emit.op_sb = bb_slot_get(bb_child1(nd));
        g_emit.op_num_real = (IR_LIT(nd).sval && strcmp(IR_LIT(nd).sval, "ar") == 0) ? 1 : 0;
        int already = (bb_slot_get(nd) >= 0); g_emit.op_off = bb_slot_alloc16_or_get(nd);
        if (!already) (void) bb_slot_claim(g_emit.op_num_real ? 16 : 8);
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    }
    case IR_CONJ:
        if (nd->n_operands > 0 && nd->operands[0] && bb_slot_get(nd) < 0) { int voff = bb_slot_get(nd->operands[0]); if (voff >= 0) bb_slot_register(nd, voff); }
        DRIVE_PAIR_RESET(); DRIVE_PAIR_JMP(lbl_γ); DRIVE_PAIR_DEF_JMP(lbl_β, lbl_ω); DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_SUCCEED:
        DRIVE_PAIR_RESET(); DRIVE_PAIR_JMP(lbl_γ); DRIVE_PAIR_DEF_JMP(lbl_β, lbl_ω); DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_FAIL:
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    case IR_RETURN:
        DRIVE_FILL(nd, lbl_γ, lbl_ω, lbl_β); break;
    default:
        drive_unowned(nd); break;
    }
}
