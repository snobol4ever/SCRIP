# BB-REVAMP-TRACKER — SPEC v2 (Lon 2026-06-04). Tick ONLY when the file is REGENERATED to v2 and gated.
# WORK QUEUE FOR: .github/GOAL-BB-FIXUP.md (the continuous fixup session / routine). Protocol, laws, ladder live THERE; per-file state lives HERE.
# CURSOR: bb_call.cpp
# LAP RESTART (Lon 2026-06-06, end of 6th run): RING RE-SORTED ALPHABETICALLY (lap-end re-sort on Lon word) + cursor at ring TOP bb_aggregate_nb.cpp for the corrected-rule lap — every file (incl. ticked-clean) re-audited against ZERO-BINARY-IN-bb_*.cpp; rb is ABSOLUTE; conversion to x86() (encoders added to x86_asm.h, byte-verified vs as) is sweep scope; pin the encoder-gap list with Lon before the first conversion stop.
# INHERITED RED 2026-06-06 (5th run): prove_lower2 on origin/main 9193511 = 66 PASS + 2 FAIL (cases #49 nodes=10 vs expected 8, #50 nodes=9 vs expected 7) and script still exits rc=0 (no hard gate on FAIL). Present IDENTICALLY with/without fixup edits (stash A/B, verdict-list diff EMPTY) — not fixup-caused. Suspect: PL-GZ-7 3d9ccfd IR_ITE_COMMIT/IR_ITE_GATE +2-node insertion vs hardcoded prove-harness expected counts; SNO-HY-2b 9193511's "prove_lower2 67->68" claim measured pre-rebase. Semantic call (fix lowering vs update counts) — owner PL-GZ / Lon, NOT swept per law 5. Evidence: HANDOFF-2026-06-06-OPUS48-BB-FIXUP-LAP1-STOPS-15-16.md
# INHERITED RED 2026-06-06 (5th run cont., stop 18): prove_lower2 harness NO LONGER LINKS on origin/main 0a614f2 — PB-12 added bb_label_landing calls to src/lower/lower.c (defined in src/lower/lower_program.c) but scripts/prove_lower2.sh links only lower.c+lower_prolog.c+scrip_ir.c+prove_lower2.c, NOT lower_program.c → ld undefined-reference, zero verdicts. Pre-PB-12 (d85d8f3) lower.c has 0 refs; not fixup-caused (bb_term_inspect.cpp is not in that link; pre-rebase run with the fixup edit was 66 PASS + 2 FAIL green). Fix is one link-line addition — owner PASCAL-BB / harness owner / Lon, NOT swept per law 5. This supersedes/buries the rc=0 2-FAIL issue above until the harness links again.
# CURSOR semantics (GOAL-BB-FIXUP.md THE CURSOR): the round-robin position. Fix file at cursor -> advance cursor IN THE SAME COMMIT as the tick/note -> push -> next. End of list wraps (laps). Ticked files get a cheap re-audit on later laps. Attended sessions only; cursor makes every session resume from cold.
# RULE CORRECTED (Lon 2026-06-06, 6th run): RULES.md TEMPLATE-ONLY EMISSION now matches the ⛔ ONE MEDIUM, INVISIBLE canon — ZERO BINARY emission anywhere in a bb_*.cpp (top level OR helpers; static helpers are inside the fence); x86() internals (x86_asm.h) are the ONLY emitter of BINARY and TEXT, side-by-side. rb (raw_bytes) is an ABSOLUTE violation class; every prior '[S] rb — owner GOAL-PROLOG-BB' parking in this file is SUPERSEDED — conversion to x86() is sweep scope (add missing encoders to x86_asm.h byte-verified vs as; laws 1-7 govern; non-neutral cases flagged). Stop-19 bb_term_io work (built+gated, asm-diff EMPTY) was RESET TO A uncommitted on Lon's word — redo under corrected rule.
# v2: no locals · one return/PLATFORM · IF()/FOR() · 1 src line = 1 asm line · real Greek α β γ ω · no MEDIUM_* top-level · no emit_fmt · no comments · no blank lines.
# v2 ONE-IR-ONE-LOGIC (Lon 2026-06-04): N IR kinds → 1 template allowed (near-identical shapes parameterized); 1 IR → 1 BB the norm; 1 IR kind → MULTIPLE distinct four-port logics in one template FORBIDDEN — break the file out by splitting the IR kind in LOWER into separate IR codes, each → its own template + emit_core dispatch case.
# v2 EMIT-BLIND / NO NEIGHBOR INQUIRY (Lon 2026-06-04): a template never dereferences a neighboring IR node (pBB->α/β/γ/ω->t kind tests, neighbor ->ival/sval/dval reads, neighbor kids/aux walks) to ADMIT a shape or CHOOSE an emission — that is IR LOWERING done in the emitter, a lowering-stage design flaw. Fix upstream: LOWER splits the IR shape (ONE-IR-ONE-LOGIC) and delivers operands via _.op_*/ζ-slots; graph inspection lives in the driver, never a template. LOWER decides, templates emit.
# EMIT-BLIND violator snapshot 2026-06-04 (grep 'pBB->[αβγω]' refs per file): bb_is_cmp(28) bb_atom_string(28) bb_term_inspect(15) bb_term_io(12) bb_aggregate_nb(9) bb_succ_plus(8) bb_list(8) bb_io(8) bb_type_test(4) bb_retract_throw(4) bb_assign_frame(4) bb_assign_frame_ref(4) — plus local-var neighbor walks (lf->t / fin->α->t / a0->t) in bb_call, bb_call_proc_staged, bb_call_write_slot, bb_return, bb_every.
- [ ] bb_aggregate_nb.cpp — (TIER H 2026-06-06: ef 9→0, pe 2→0 (PORT_*→Greek), lv 12→0 (per-arm helpers agg_bin_all/nbset/nbget + agg_txt_all/nbset/nbget with signature-line decls), nw 2→0; binary-arm BYTES verbatim, single-strtab_label-per-site registration preserved; asm-diff EMPTY (git-stash baseline, 18-file corpus); 60→31) — (RB CONVERSION 2026-06-06 7th run: rb 25→5 — all three agg_bin_* arms converted to x86() under corrected rule, byte-identical via new movabs/xor encoders + REX.W add/sub (each byte-verified vs as); asm-diff EMPTY ×3 probes (bbN-normalized) + LIVE nb_setval/nb_getval TEXT arms fire (rt_nb_setval_term/rt_nb_getval_term@PLT in .s), m4-run A/B identical rc=0; remaining rb=5 = x86_lit_bytes( bridges around [S] emit_term_from_node_bin (audit substring artifact — honest [S] markers, removable only with LOWER term plumbing); 31→11) — [S] eb=6: pBB->α/α->γ admission+operand reads need LOWER _.op_*/ζ-slot plumbing, design not pinned
- [ ] bb_alt.cpp — (TIER H 2026-06-06: emit_fmt→inline to_string, PORT_*→literal Greek, locals 6→3 via FOR()/inline; asm-equiv diff EMPTY on smoke+pat corpus (corpus does not exercise bb_alt mode-4)) — (lap-2 re-audit 2026-06-06 7th run: rb=0 ✓ corrected-rule clean; nw=4 lv=3 confirmed all [S]-entangled) — [S] EMIT-BLIND residue: operand-aux arm->t/ival/sval walk (emit_arm/seal_arm + admission loop, nw=4 lv=3) needs LOWER operand plumbing via _.op_*/ζ-slots; design not pinned
- [x] bb_arith.cpp — (✅ v2 2026-06-06; asm-diff EMPTY; details in git)
- [x] bb_assign_frame.cpp — (✅ v2 2026-06-06 7th run: lv 10→0 — k-ladder de-crammed to baf_* per-arm helpers (signature-line decls, lazy if-chain selector, single-eval strtab preserved); guards verbatim; asm-diff EMPTY ×3 probes; LIVE m3 define→42/frame2→hi3 through binary flat chain; audit CLEAN all counters 0)
- [x] bb_assign_frame_ref.cpp — (✅ v2 2026-06-06 7th run: lv 10→0 — bafr_* twin of bb_assign_frame recipe (extra hop deref [rcx+voff+8] + 0/8 stores verbatim); asm-diff EMPTY ×4 probes; LIVE m2+m3 A/B identical incl. name-ref probe; audit CLEAN)
- [x] bb_assign_local.cpp — (✅ v2 2026-06-06; asm-diff EMPTY; details in git)
- [x] bb_atom.cpp — (✅ v2 2026-06-06; asm-diff EMPTY; details in git)
- [ ] bb_atom_string.cpp — (TIER H 2026-06-06: emit_fmt(59)→0 (std::to_string + std::string("[rip + ")+lbl+"]" + std::string(_.lbl_β)+":"), PORT_*→Greek (6→0), dead succ_back local excised (lv 50→49); 265→199; asm-diff EMPTY 27-file corpus; all gates at floors) — (RB CONVERSION 2026-06-06 8th run: rb 101→4 — all SIX MEDIUM_BINARY arms converted to x86() under corrected rule (movabs/mov32/xor/mov-rr/RSP() rsp-store-load/call-ro/REX.W sub-add + NEW stk32 encoder C7-04-24 byte-verified vs as, dispatch added x86_asm.h); per-instruction byte-map vs original hex; behavior A/B identical on 10 probes × m2/m3/m4 = 30 files; asm-diff EMPTY ×10 bbN-normalized; m4 LIVE through all 6 arm families incl. atom_chars path-B struct + copy_term compound; m3 = PL-GZ-1b LOUD interp-fallback IDENTICAL both sides (these builtins m3-silent, byte-map is the proof); remaining rb=4 = x86_lit_bytes( bridges around [S] emit_term_from_node_bin — audit substring artifact, honest [S] markers; 199→102) — [S] eb=28 nw=21: pBB->α/a0->t neighbor reads in every arm → LOWER _.op_*/ζ-slot plumbing, design not pinned — [S] lv=49: all locals entangled with TIER S neighbor reads
- [x] bb_binop_arith.cpp — (✅ v2 2026-06-06; asm-diff EMPTY; details in git)
- [x] bb_binop_concat_slot.cpp — (✅ v2 2026-06-06; asm-diff EMPTY; details in git)
- [x] bb_binop_gvar_arith.cpp — (✅ v2 2026-06-06; asm-diff EMPTY; details in git)
- [x] bb_binop_gvar_arith_slot.cpp — (✅ v2 2026-06-06; asm-diff EMPTY; details in git)
- [x] bb_binop_gvar_relop.cpp — (✅ v2 2026-06-06; asm-diff EMPTY; details in git)
- [x] bb_binop_relop.cpp — (✅ v2 2026-06-06; asm-diff EMPTY; details in git)
- [ ] bb_call.cpp — LONG wrapped; OPEN: pBB refs + a0->t AST-walk in emitter (RULES violation), 3 bombs
- [ ] bb_call_fn.cpp
- [ ] bb_call_proc_staged.cpp
- [ ] bb_call_userproc.cpp
- [ ] bb_call_write_slot.cpp
- [ ] bb_callee_frame.cpp — (v1 done: 8-rule grep-clean; LONG wrapped (string-split, content-identical))
- [ ] bb_catch.cpp
- [ ] bb_cell_call.cpp — (v1 done: 8-rule grep-clean; LONG wrapped (string-split, content-identical))
- [ ] bb_cell_choice.cpp — (v1 done: 8-rule grep-clean; LONG wrapped (string-split, content-identical))
- [ ] bb_cell_cut.cpp — (ADDED TO RING at lap-end re-sort 2026-06-06: generator-created, never audited)
- [ ] bb_cell_ite.cpp — (ADDED TO RING at lap-end re-sort 2026-06-06: generator-created, never audited)
- [ ] bb_cell_unify.cpp
- [ ] bb_choice.cpp — (v1 done: prose comments stripped; gates green)
- [ ] bb_conj.cpp — (v1 done: prose comments stripped; gates green)
- [ ] bb_cut.cpp
- [ ] bb_det_cmp.cpp — (ADDED TO RING 2026-06-06 7th run: generator-created by PL-GZ-8 ad6372f, never audited)
- [ ] bb_det_is.cpp — (ADDED TO RING 2026-06-06 7th run: generator-created by PL-GZ-8 ad6372f, never audited)
- [ ] bb_det_nl.cpp
- [ ] bb_det_write.cpp
- [ ] bb_disj.cpp
- [ ] bb_every.cpp
- [ ] bb_fail.cpp
- [ ] bb_findall.cpp — (TIER H 2026-06-06: ef 4→0 (emit_fmt→std::string concat), pe 1→0 (PORT_*→literal Greek), lv 2→0 (std::string b inlined, std::string goal_build→static bff_goal() helper); asm-diff EMPTY (label-normalized, corpus does not exercise bb_findall mode-4); 13→6) — [S] rb=6: MEDIUM_BINARY raw-byte arms = medium-invisible conversion, owner GOAL-PROLOG-BB — [S] IR walk (gn->t/cand->t via fs->gcfg): EMIT-BLIND in spirit, design not pinned
- [ ] bb_gather.cpp
- [ ] bb_gen_scan.cpp
- [ ] bb_goal.cpp — (v1 done: prose comments stripped; gates green)
- [ ] bb_gvar_assign.cpp — (v1 done: 8-rule grep-clean; LONG wrapped) — ⛔ ONE-IR-ONE-LOGIC VIOLATOR, SHRINKING (3a `75b5bd6`: lit_s+lit_i split; 3b `d3d5688`: var+concat+call+descr split → IR_ASSIGN_VAR/CONCAT/CALL/DESCR, 4 new SPEC-v2 templates). Old arms RETAINED for plain IR_ASSIGN creators (lower.c 224/441/532/680/688) + non-SNO. Remaining: binop arm; 3c capture design not pinned (Lon).
- [ ] bb_gvar_assign_lit_i.cpp — (ADDED TO RING at lap-end re-sort 2026-06-06: generator-created, never audited)
- [ ] bb_gvar_assign_lit_s.cpp — (ADDED TO RING at lap-end re-sort 2026-06-06: generator-created, never audited)
- [ ] bb_io.cpp — (TIER H 2026-06-06: ef 6→0, pe 4→0 (BINARY ports→literal Greek glyphs, XK_PORT-preserving), lv 8→0 (bio_succ/bio_write_body/bio_fbits_str/bio_bin_write_arg helpers); eb 8→6; asm-diff EMPTY (label-normalized); 51→30) — [S] eb=6: pBB->α admission/operand reads → LOWER _.op_*/ζ-slots, design not pinned — [S] rb=24: MEDIUM_BINARY raw-byte arms, owner GOAL-PROLOG-BB
- [ ] bb_is_cmp.cpp — (TIER H 2026-06-06: ef 39→0 (emit_fmt→std::to_string/concat), pe 2→0 (BINARY tails→literal Greek glyphs, XK_PORT-preserving), lv 28→0 (icm_arith/icm_ord/icm_cmp/icm_op/icm_tail/icm_k/icm_i/icm_fb/icm_floaty helpers; dead succ_back excised; std::string b ×2 inlined), V-2 prose + byte-annotation comments purged (history: HANDOFF-2026-05-27-OPUS-SONNET docs); asm-diff EMPTY (label-normalized, LIVE check — is/2 + >= fire in corpus mode-4); 121→64) — [S] eb=34 nw=8 (UP from 28/2: de-aliasing exposed grandkid pBB->β->α/β reads to the counter — honest visibility, same semantics): is/2 shape admission + operand fusion → LOWER _.op_*/ζ-slot plumbing, design not pinned — [S] rb=22: MEDIUM_BINARY raw-byte arms, owner GOAL-PROLOG-BB
- [ ] bb_ite.cpp — (v1 done: prose comments stripped; gates green)
- [ ] bb_iterate.cpp
- [ ] bb_keyword.cpp
- [ ] bb_list.cpp — (TIER H 2026-06-06: ef 22→0, pe 2→0 (BINARY-arm ports→literal Greek glyphs, XK_PORT-preserving per ⛔ BINARY-ARM PORTS), lv 23→0 (bls_lbl/bls_bin_alc/bls_bin_sort_term/bls_bin_sort_scalar/bls_txt_alc/bls_txt_sort + shared bin/txt tail+ports helpers, signature-line decls; dead succ_back excised; byte-annotation comments purged); asm-diff EMPTY (bbN-normalized, 17-file corpus) + LIVE probe msort-scalar + atomic_list_concat/2 TEXT arms fire byte-identical (BINARY arm verbatim-relocated, mode-3 falls back to interp for probe — unexercised); 100→52) — [S] eb=8 nw=12 (UP from 6: de-aliased a0->t/a1->t reads in helpers now counter-visible, same semantics): admission/operand fusion → LOWER _.op_*/ζ-slot plumbing, design not pinned — [S] rb=32: MEDIUM_BINARY raw-byte arms, owner GOAL-PROLOG-BB
- [ ] bb_lit.cpp
- [ ] bb_lit_scalar.cpp
- [ ] bb_logicvar.cpp
- [ ] bb_match.cpp — ⛔ ONE-IR-ONE-LOGIC VIOLATOR: ONE IR_PAT_MATCH kind → 3 distinct logics (HEAD/RETRY/ADVANCE) selected by op_ival. Break out: three IR codes (MATCH_HEAD/MATCH_RETRY/MATCH_ADVANCE), three templates. AUDIT also: bb_keyword.cpp (4 keyword arms — near-identical or distinct? confirm), bb_scan_stmt.cpp (literal vs non-literal arms).
- [ ] bb_pat_abort.cpp
- [ ] bb_pat_alt.cpp — (v1 done: prose comments stripped; gates green)
- [ ] bb_pat_any.cpp
- [ ] bb_pat_arb.cpp
- [ ] bb_pat_arbno.cpp
- [ ] bb_pat_atp.cpp
- [ ] bb_pat_break.cpp — (v1 done: PASS 8/8 hygiene (REG-RO + SNO-HY-1 de-cram deferred to their ladders))
- [ ] bb_pat_breakx.cpp — (ADDED TO RING at lap-end re-sort 2026-06-06: generator-created, never audited)
- [ ] bb_pat_capture.cpp
- [ ] bb_pat_cat.cpp — (v1 done: prose comments stripped; gates green)
- [ ] bb_pat_defer.cpp
- [ ] bb_pat_fence.cpp
- [ ] bb_pat_len.cpp
- [ ] bb_pat_notany.cpp
- [ ] bb_pat_pos.cpp
- [ ] bb_pat_rem.cpp
- [ ] bb_pat_rtab.cpp — (ADDED TO RING at lap-end re-sort 2026-06-06: generator-created, never audited)
- [ ] bb_pat_span.cpp
- [ ] bb_pat_span_var.cpp — (ADDED TO RING at lap-end re-sort 2026-06-06: generator-created, never audited)
- [ ] bb_pat_tab.cpp
- [ ] bb_query_frame.cpp — (v1 done: 8-rule grep-clean; LONG wrapped (string-split, content-identical))
- [ ] bb_resolve.cpp — (TIER H 2026-06-06: ef 40→0 (emit_fmt→inline to_string/concat), lv 20→0 (signature-line decls + bfrm/bnth/briplbl/bslot/bmset one-line helpers + FOR() child loops + bterm_goal/arith/mset branch helpers), PLR-K-9 prose block purged (history in HANDOFF-2026-05-29-OPUS48-PROLOG-BB-PLRK789.md), dispatch first-match short-circuit preserved verbatim; asm-diff EMPTY on 18-file prolog-heavy corpus incl. ancestor.pl which fires emit_build_compound_term; 63→2) — [S] rb=2 residue: emit_term_from_node_bin raw movabs node-ptr bytes + bunknown() MEDIUM_BINARY E9-rel32-0 fall-through placeholder (≠ TEXT jmp-γ stub — collapse NOT behavior-neutral, medium-invisible conversion belongs to GOAL-PROLOG-BB owning session per gate note) — [S] emit_build_compound_term/_conj_chain nd-> IR-walkers are emitter-side lowering (EMIT-BLIND in spirit, missed by nw alias list); LOWER term-spec plumbing design not pinned
- [ ] bb_retract_throw.cpp — (TIER H 2026-06-06: ef 4→0 (emit_fmt→std::to_string/concat), pe 1→0 (BINARY ports→literal Greek glyphs, XK_PORT-preserving), lv 2→0 (rtt_lbl/rtt_ball_scalar helpers, signature-line decls; ball_build + alpha_ptr inlined; byte-annotation comments purged); E9-rel32-0 retract placeholder + g_sm_native_unsupported flag verbatim; asm-diff EMPTY (bbN-normalized, 17-file corpus) + LIVE catch(throw(oops)) probe TEXT arm byte-identical; 18→10) — [S] eb=4: pBB->α admission/operand reads → LOWER _.op_*/ζ-slot plumbing, design not pinned — [S] rb=6: MEDIUM_BINARY raw-byte arms (incl. E9-rel32-0 placeholder), owner GOAL-PROLOG-BB
- [ ] bb_return.cpp
- [ ] bb_scan_any.cpp
- [ ] bb_scan_bal.cpp — (v1 done: 8-rule grep-clean; LONG wrapped (string-split, content-identical))
- [ ] bb_scan_find.cpp — (v1 done: 8-rule grep-clean; LONG wrapped (string-split, content-identical))
- [ ] bb_scan_many.cpp
- [ ] bb_scan_match.cpp — (v1 done: 8-rule grep-clean; LONG wrapped (string-split, content-identical))
- [ ] bb_scan_move.cpp — (v1 done: 8-rule grep-clean; LONG wrapped (string-split, content-identical))
- [ ] bb_scan_pos.cpp
- [ ] bb_scan_stmt.cpp
- [ ] bb_scan_tab.cpp — (v1 done: 8-rule grep-clean; LONG wrapped (string-split, content-identical))
- [ ] bb_scan_upto.cpp — (v1 done: 8-rule grep-clean; LONG wrapped (string-split, content-identical))
- [ ] bb_subject.cpp
- [ ] bb_succ_plus.cpp — (TIER H 2026-06-06: ef 18→0 (emit_fmt→std::to_string/concat), pe 2→0 (BINARY ports→literal Greek glyphs, XK_PORT-preserving), lv 11→0 (bsp_lbl/bsp_bin_succ/bsp_bin_plus/bsp_txt_succ/bsp_txt_plus + shared bin-ports/txt-tail helpers, signature-line decls; dead succ_back excised); succ β-port operand + plus γ-chain admission verbatim; asm-diff EMPTY (bbN-normalized, 17-file corpus + 3 probes) incl LIVE probe — succ(3,X)+plus(2,3,Z) TEXT arms fire byte-identical (rt_succ@PLT/rt_plus@PLT in .s, m2 4/5); 73→45) — [S] eb=8 nw=12 (UP from 4: de-aliased a0/a1->t reads in helpers now counter-visible, same semantics): admission/operand fusion → LOWER _.op_*/ζ-slot plumbing, design not pinned — [S] rb=25: MEDIUM_BINARY raw-byte arms, owner GOAL-PROLOG-BB
- [ ] bb_succeed.cpp
- [ ] bb_term_inspect.cpp — (TIER H 2026-06-06: ef 51→0 (emit_fmt→std::to_string/concat), pe 3→0 (BINARY ports→literal Greek glyphs, XK_PORT-preserving per ⛔ BINARY-ARM PORTS), lv 24→0 (bti_lbl/bti_bin_functor/bti_bin_arg/bti_bin_univ_tt|t1|1t/bti_txt_functor_t|s/bti_txt_arg_t|s/bti_txt_univ_tt|t1|1t|ss + shared bin-ports/txt-tail helpers, signature-line decls; strtab_label registration ORDER preserved per arm (functor s1,s2[,s0]; arg s0,s2[,s1]; univ guards reproduced by bti_lbl's IR_ATOM gate); dead succ_back excised; prose comments purged); asm-diff EMPTY (bbN-normalized, 12-file: 9 corpus + 3 probes) + LIVE probes fire ALL 8 TEXT arms byte-identical (rt_functor_term/rt_functor/rt_arg_term/rt_arg/rt_univ_term/rt_univ_term_list/rt_univ_term_term/rt_univ all @PLT in .s; m2+m3+m4-run A/B identical; mode-3 LOUD interp-fallback, BINARY arms verbatim-relocated); 161→84) — [S] eb=12 nw=22 (UP from 14: de-aliased a0/a1/a2->t reads in helpers now counter-visible, same semantics): functor/arg/=.. shape admission + operand fusion → LOWER _.op_*/ζ-slot plumbing, design not pinned — [S] rb=50: MEDIUM_BINARY raw-byte arms (incl. emit_term_from_node_bin node-ptr movabs), owner GOAL-PROLOG-BB
- [ ] bb_term_io.cpp
- [ ] bb_to.cpp
- [ ] bb_type_test.cpp
- [ ] bb_unify.cpp
- [ ] bb_unop.cpp
- [ ] bb_var.cpp
- [ ] bb_var_frame.cpp
- [ ] bb_var_frame_ref.cpp
- [ ] bb_var_global.cpp
