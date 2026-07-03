#include "core.h"
#include "builtins/gen_runtime.h"
#include <string.h>
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t sm_call_proc(int entry_pc, int nparams, DESCR_t *args, int nargs)
{
    extern DESCR_t sm_eval_subexpr(int epc);
    if (entry_pc < 0) return FAILDESCR;
    if (frame_depth >= FRAME_STACK_MAX) return FAILDESCR;
    GenFrame *f = &frame_stack[frame_depth++];
    memset(f, 0, sizeof *f);
    int nslots = (nparams > 0) ? nparams : 1;
    if (nslots > FRAME_SLOT_MAX) nslots = FRAME_SLOT_MAX;
    f->env_n = nslots;
    for (int i = 0; i < nparams && i < nargs && i < FRAME_SLOT_MAX; i++)
        f->env[i] = args[i];
    int found_pi = -1;
    tree_t *found_proc = NULL;
    {
        for (int i = 0; i < g_stage2.proc_count; i++) {
            if (g_stage2.proc_table[i].entry_pc == entry_pc) { found_pi = i; break; }
        }
        if (found_pi >= 0 && g_stage2.proc_table[found_pi].proc) {
            found_proc = g_stage2.proc_table[found_pi].proc;
            tree_t *body_nd = (found_proc->t == TT_PROC_DECL && found_proc->n >= 3) ? found_proc->c[2] : NULL;
            if (body_nd) for (int bi = 0; bi < body_nd->n; bi++)
                scope_patch(&g_stage2.proc_table[found_pi].lower_sc, body_nd->c[bi]);
            int total_slots = g_stage2.proc_table[found_pi].lower_sc.n;
            if (total_slots > f->env_n) f->env_n = total_slots;
            f->sc = g_stage2.proc_table[found_pi].lower_sc;
            Scope *sc = &g_stage2.proc_table[found_pi].lower_sc;
            GenFrame *parent_f = (frame_depth >= 2) ? &frame_stack[frame_depth - 2] : NULL;
            for (int bi = 0; body_nd && bi < body_nd->n; bi++) {
                tree_t *st = body_nd->c[bi];
                if (!st || st->t != TT_STATIC_DECL) continue;
                for (int j = 0; j < st->n; j++) {
                    tree_t *vn = st->c[j];
                    if (!vn || !vn->v.sval) continue;
                    int slot = scope_get(sc, vn->v.sval);
                    if (slot < 0 || slot >= f->env_n) continue;
                    DESCR_t saved;
                    if (static_get(found_proc, vn->v.sval, &saved)) {
                        f->env[slot] = saved;
                    } else if (parent_f && slot < parent_f->env_n
                               && parent_f->env[slot].v != 0) {
                        f->env[slot] = parent_f->env[slot];
                    }
                }
            }
        }
    }
    DESCR_t result = sm_eval_subexpr(entry_pc);
    if (found_pi >= 0 && found_proc) {
        Scope *sc = &g_stage2.proc_table[found_pi].lower_sc;
        tree_t *body_nd2 = (found_proc->t == TT_PROC_DECL && found_proc->n >= 3) ? found_proc->c[2] : NULL;
        for (int bi = 0; body_nd2 && bi < body_nd2->n; bi++) {
            tree_t *st = body_nd2->c[bi];
            if (!st || st->t != TT_STATIC_DECL) continue;
            for (int j = 0; j < st->n; j++) {
                tree_t *vn = st->c[j];
                if (!vn || !vn->v.sval) continue;
                int slot = scope_get(sc, vn->v.sval);
                if (slot < 0 || slot >= f->env_n) continue;
                static_set(found_proc, vn->v.sval, f->env[slot]);
            }
        }
    }
    init_save_frame();
    frame_depth--;
    return result;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t proc_table_call(int pi, DESCR_t *args, int nargs)
{
    if (pi < 0 || pi >= g_stage2.proc_count) return FAILDESCR;
    extern stage2_t g_stage2;
    if (g_stage2.proc_table[pi].entry_pc >= 0 && 1)
        return sm_call_proc(g_stage2.proc_table[pi].entry_pc, g_stage2.proc_table[pi].nparams, args, nargs);
    return FAILDESCR;
}
