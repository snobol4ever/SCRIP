# PBG-2 handoff — TABLE int-key read/write (table_access)

Status: investigation COMPLETE, design LOCKED, one template drafted. Wiring NOT yet applied.
Tree is at clean HEAD (`bb_idx_get.cpp` is inert — not in any build rule). Nothing regresses.

## What is already correct (do NOT rebuild)
- Runtime semantics DONE: `subscript_get`/`subscript_set` in `src/runtime/pattern_match.c`
  (DT_T branch) already do exact SPITBOL table semantics: int key -> "%lld" string,
  missing key -> table default / NULVCL (no entry created on read), `table_set_descr` on write.
  ABI: subscript_get(arr,idx): arr=rdi:rsi, idx=rdx:rcx, returns DESCR in rax:rdx.
  subscript_set(arr,idx,val): arr=rdi:rsi, idx=rdx:rcx, val=r8:r9, returns int (1=ok) in eax.
- `NV_GET_fn(name)` returns the var's DESCR in rax:rdx (bb_var relies on this).
- WIP `rt_table_idx_get/set` in aggregates.c are REDUNDANT duplicates of subscript_*; ignore.
- emit_core IR_IDX_SET dispatch (emit_core.c ~441) EXISTS.
- walk_bb_flat IR_IDX (~3243) and IR_IDX_SET (~3244) dispatch EXIST (call flat_drive_idx_get/set).

## DESCR layout (critical for immediates)
`struct { DTYPE_t v; uint32_t slen; union{...}; }` = 16 bytes, no padding.
eightbyte0 = v | (slen<<32); eightbyte1 = payload. DT_S=1, DT_T=5, DT_I=6, DT_SNUL=0.
Int-immediate key/val `K`: eightbyte0 = DT_I = 6 (slen 0); eightbyte1 = K.
  -> `movabs rdx, 6` ; `movabs rcx, K` (for the idx arg).

## Chain-flag finding (why we go BY-NAME, not slot model)
Global IR_VAR deposits its NV_GET value into a frame slot ONLY under `g_gvar_flat_chain`
(bb_var arm 1). LIT_I deposits a slot ONLY under `g_descr_flat_chain || g_gvar_callarg_live`
(emit_bb.c ~2841). Neither single chain handles BOTH a global-var key AND a lit-int key, so the
slot model that the stale `flat_drive_idx_set` assumed cannot serve `T<3>` and `T<I>` uniformly.
BY-NAME (emit NV_GET(name) directly in the template, read operand metadata at emit time) sidesteps
this entirely and matches the proven `bb_binop_gvar_arith_slot` pattern. Base of `T<k>` is ALWAYS a
plain var, so by-name is always valid for the base.

## op_a mapping (read path)
emit_core.c ~332: for IR_ASSIGN, `op_a = operands[0]`; `op_a_node_kind = ir_norm_call_kind(op_a->op)`;
`op_a_slot = bb_slot_get(op_a)`. So the IDX value box MUST be pushed as operands[0] of the assign,
and MUST be chained (walked) BEFORE the assign so it deposits its DESCR into its own slot (= op_off
set by flat_drive_idx_get = bb_slot_alloc16(idx)); bb_prepare then reads op_a_slot = that slot.
`lc_build` does NOT auto-push operands — push explicitly with `ir_operand_push`.

## The 13 sites (FOUNDATION: base=VAR, key=VAR|LIT_I, value=VAR|LIT_I)

READ  `OUTPUT = T<k>`  (greens half of probe)
 R1 lower_snobol4.c lower_assign: add `case TT_IDX` BEFORE the default IDX-drop (currently ~795
    drops it). Build: asn=build(IR_ASSIGN,γ,ω); IR_LIT(asn).sval=lhs;
    idx=build(IR_IDX,asn,ω); base_box=build(IR_VAR,NULL,NULL),IR_LIT.sval=subj->c[0] name;
    key_box=build(IR_LIT_I or IR_VAR,NULL,NULL) from subj->c[1];
    ir_operand_push(idx,base_box); ir_operand_push(idx,key_box); ir_operand_push(asn,idx);
    return idx;  (entry=idx; idx.γ->asn->γ)
 R2 emit_bb.c walk_bb_flat IR_ASSIGN dispatch (~3148): add `|| ac0->op == IR_IDX` so it routes to
    flat_drive_gvar_assign.
 R3 emit_core.c IR_ASSIGN dispatch (~396): add `|| op_a->op == IR_IDX` to the bb_gvar_assign whitelist.
 R4 emit_bb.c flat_drive_idx_get (~1764, currently dead stub): set op_name1=base sval;
    if key->op==IR_LIT_I: op_node_kind=IR_LIT_I, op_ival=IR_LIT(key).ival;
    else (IR_VAR): op_node_kind=IR_VAR, op_name2=key sval, op_sa=bb_slot_claim(16) (table scratch);
    op_off=bb_slot_alloc16(pBB); then FILL(pBB,lbl_γ,lbl_ω,lbl_β) (leaf; bb_idx_get emits jmp γ/def β).
 R5 emit_core.c: add `case IR_IDX: bb_emit_x86(bb_idx_get()); return 0;`
 R6 bb_idx_get.cpp: DRAFTED (this session). By-name; handles LIT_I-immediate and VAR keys.
 R7 bb_gvar_assign.cpp: add an arm `else if (_.op_a_node_kind == (int)IR_IDX)` that is a VERBATIM
    clone of the IR_CALL arm (load DESCR from FRQ(op_a_slot)/FRQ(op_a_slot+8) -> rt_gvar_assign_descr).

WRITE `T<k> = v`  (greens other half)
 W1 lower_snobol4.c lower_stmt_body: add `if (subj->t == TT_IDX)` arm BEFORE the
    `subj->t != TT_VAR && != TT_KEYWORD -> return NULL` guard (~856). Build:
    st=build(IR_IDX_SET,γ_tgt,ω_tgt); push base_box, key_box, value_box (from subj->c[0],c[1],repl);
    return st; (entry=st)
 W2 emit_bb.c flat_drive_idx_set (~1781, currently stale slot model): REWRITE by-name. Set
    op_name1=base; key meta (op_node_kind/op_ival or op_name2); value meta into op_a_* fields
    (op_a_node_kind / op_a_ival_sg / op_a_sval); op_sa=op_sb=op_sc=bb_slot_claim(16) (scratch
    table/key/val); then EMIT_PAIR_RESET/DEF_JMP(lbl_β,lbl_ω)/FILL(pBB,lbl_γ,lbl_ω,lbl_β).
 W3 bb_idx_set.cpp: REWRITE by-name (current version reads slot model). Sequence:
    NV_GET(base)->save sa; build key->sb (LIT_I immediate or NV_GET var); build val->sc; then
    rdi:rsi=sa, rdx:rcx=sb, r8:r9=sc; call subscript_set; cmp eax,0; je ω; jmp γ; def β; jmp ω.

SHARED
 S1 emit_bb.c bb_fill_alpha (~242): add arms for IR_IDX and IR_IDX_SET that strtab_label the names
    into op_parts_lbl[]: [0]=op_name1 (base), [1]=op_name2 (key var), and for IR_IDX_SET [2]=op_a_sval
    (value var). Use a local static char pool like gvspool/defpool. Only label non-empty names.
 S2 emit_templates.h (or wherever bb_idx_set is declared): add `std::string bb_idx_get();`.
 S3 Makefile / template build list: add `bb_idx_get.cpp` next to `bb_idx_set.cpp` (RT_PIC template srcs).

## Probe + gate (must all hold before commit)
PROBE (foundation target):
  printf '    T = TABLE(16)\n    T<3> = 99\n    OUTPUT = T<3>\nEND\n' > /tmp/tb.sno
  scrip --compile /tmp/tb.sno > /tmp/tb.s && gcc -no-pie /tmp/tb.s -Lout -lscrip_rt -lgc -lm \
    -Wl,-rpath,$PWD/out -o /tmp/tb && /tmp/tb   # expect: 99   (oracle: x64/bin/sbl -b /tmp/tb.sno)
GATE (non-regression — count of bench whose M4==oracle must NOT drop from 5):
  bash scripts/test_bench_snobol4_modes.sh   (arith_loop, op_dispatch, pattern_bt, string_concat,
  fibonacci must stay green). Plus the smoke / pat-rung / fence gates per GOAL file.

## Remaining for table_access GREEN (PBG-2 clause D — NEXT, multi-step)
Foundation above does NOT green table_access. That benchmark also needs:
  (D1) value = BINOP  (`T<I> = I * 2`): value operand is IR_BINOP, not simple. Extend the WRITE
       path to chain the value box before IR_IDX_SET and walk it (gvar-arith deposits a DESCR slot),
       then bb_idx_set reads op_sc = that slot instead of the by-name immediate.
  (D2) mixed arith   (`SUM = SUM + T<I>`): right operand is an IDX read. Extend bb_arith_is_dynamic
       to treat IR_IDX as dynamic and materialize the gvar operand (the PBG-1 follow-up).
After D1+D2: probe `table_access` -> 250500 and it goes green (+1).
