/* ATTIC: dead code excised from src/contracts/scrip_ir.c (commit base 2a35216) — unreachable from main per --gc-sections oracle. Provenance only; not compiled. */
/*--------------------------------------------------------------------------------------------------------------------*/
void bb_reset(IR_graph_t * bbg) {
    if (!bbg) return;
    for (int i = 0; i < bbg->n; i++) {
        IR_t * bb = bbg->all[i];
        if (!bb) continue;
        IR_EXEC(bb).value   = FAILDESCR;
        if (bb->op != IR_PAT_ARBNO && bb->op != IR_SCAN && bb->op != IR_GEN_SCAN && bb->op != IR_GOTO && bb->op != IR_GATHER && bb->op != IR_MAP && bb->op != IR_GREP && bb->op != IR_PROG && bb->op != IR_SUSPEND && !(bb->op == IR_SEQ && IR_LIT(bb).dval == 1.0) && !(bb->op == IR_CALL && (IR_LIT(bb).dval == 2.0 || IR_LIT(bb).dval == 3.0)) && !ir_is_scan_kind(bb->op)) IR_EXEC(bb).counter = 0;
        IR_EXEC(bb).state   = 0;
    }
    ag_ring_clear(bbg);
}
/*--------------------------------------------------------------------------------------------------------------------*/
bb_node_state_t * bb_snapshot_state(IR_graph_t * bbg) {
    if (!bbg || bbg->n <= 0) return NULL;
    bb_node_state_t * snap = (bb_node_state_t *)malloc((size_t)bbg->n * sizeof(bb_node_state_t));
    if (!snap) return NULL;
    for (int i = 0; i < bbg->n; i++) {
        IR_t * bb = bbg->all[i];
        if (!bb) { memset(&snap[i], 0, sizeof snap[i]); snap[i].value = FAILDESCR; continue; }
        snap[i].value   = IR_EXEC(bb).value;
        snap[i].counter = IR_EXEC(bb).counter;
        snap[i].state   = IR_EXEC(bb).state;
        snap[i].resolve_cs = NULL; snap[i].ch_cur = 0; snap[i].ch_mark = 0; snap[i].ch_saved_env = NULL;
        snap[i].ch_last_body = NULL; snap[i].ch_last_act = NULL;
        snap[i].ch_cp = NULL; snap[i].ch_cut_barrier = NULL;
        snap[i].ch_body_snaps = NULL; snap[i].ch_nbodies = 0;
        if (bb->op == IR_GOAL) {
            bb_goal_state_t * zc = (bb_goal_state_t *)(intptr_t)IR_LIT(bb).ival;
            if (zc) snap[i].resolve_cs = zc->cs;
        } else if (bb->op == IR_CHOICE) {
            bb_choice_state_t * zc = (bb_choice_state_t *)(intptr_t)IR_LIT(bb).ival;
            if (zc) { snap[i].ch_cur = zc->cur; snap[i].ch_mark = zc->mark; snap[i].ch_saved_env = zc->saved_env;
                      snap[i].ch_last_body = zc->last_body; snap[i].ch_last_act = zc->last_act;
                      snap[i].ch_cp = zc->cp; snap[i].ch_cut_barrier = zc->cut_barrier;
                      if (zc->nbodies > 0 && zc->bodies) {
                          snap[i].ch_nbodies = zc->nbodies;
                          snap[i].ch_body_snaps = (void **)calloc((size_t)zc->nbodies, sizeof(void *));
                          if (snap[i].ch_body_snaps)
                              for (int b = 0; b < zc->nbodies; b++)
                                  snap[i].ch_body_snaps[b] = (void *)bb_snapshot_state(zc->bodies[b]);
                      } }
        }
    }
    return snap;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void bb_restore_state(IR_graph_t * bbg, bb_node_state_t * snap) {
    if (!bbg || !snap) { free(snap); return; }
    for (int i = 0; i < bbg->n; i++) {
        IR_t * bb = bbg->all[i];
        if (!bb) continue;
        IR_EXEC(bb).value   = snap[i].value;
        IR_EXEC(bb).counter = snap[i].counter;
        IR_EXEC(bb).state   = snap[i].state;
        if (bb->op == IR_GOAL) {
            bb_goal_state_t * zc = (bb_goal_state_t *)(intptr_t)IR_LIT(bb).ival;
            if (zc) zc->cs = snap[i].resolve_cs;
        } else if (bb->op == IR_CHOICE) {
            bb_choice_state_t * zc = (bb_choice_state_t *)(intptr_t)IR_LIT(bb).ival;
            if (zc) { zc->cur = snap[i].ch_cur; zc->mark = snap[i].ch_mark; zc->saved_env = snap[i].ch_saved_env;
                      zc->last_body = (IR_graph_t *)snap[i].ch_last_body; zc->last_act = snap[i].ch_last_act;
                      zc->cp = snap[i].ch_cp; zc->cut_barrier = snap[i].ch_cut_barrier;
                      if (snap[i].ch_body_snaps) {
                          for (int b = 0; b < snap[i].ch_nbodies; b++)
                              if (b < zc->nbodies && zc->bodies)
                                  bb_restore_state(zc->bodies[b], (bb_node_state_t *)snap[i].ch_body_snaps[b]);
                              else
                                  free(snap[i].ch_body_snaps[b]);
                          free(snap[i].ch_body_snaps); snap[i].ch_body_snaps = NULL;
                      } }
        }
    }
    free(snap);
}
