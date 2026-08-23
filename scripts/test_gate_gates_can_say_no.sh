#!/bin/bash
# test_gate_gates_can_say_no.sh -- V2-5.  THE GATE THAT TESTS THE GATES.
#
# ONE QUESTION, ASKED OF EVERY GATE: given NOTHING to examine, does it refuse -- or does it say green?
# A gate handed an empty tree has proven nothing.  If it exits 0 anyway, every DONE-WHEN that cites it is
# closing rows on a lie (ARCH-FLEET-CEO.md LAW 1).  seat16's audit found 31 such gates by hand-injection;
# this runs the same injection mechanically, every time, so gate #32 cannot reintroduce the hole.
#
# THE INJECTION is deliberately the crudest possible one: a scratch sibling root holding the gate script and
# NOTHING ELSE -- no scrip binary, no out/libscrip_rt.so, empty src/, empty corpus/.  Both $HERE/.. and
# $S4E_HOME resolve into the scratch root, so the gate genuinely cannot see the real tree.  Any gate that
# returns 0 from that state is, by construction, not reading what it claims to read.
#
# EXPECTED verdicts are pinned per script, so this gate can say no in BOTH directions:
#   REFUSE  -- must exit non-zero on the empty tree (the honest answer).
#   PASSES  -- known-still-vacuous, pinned with its audit tier.  A PASSES entry that starts REFUSING is a
#              WIN and is reported as a FIXED row -- update the pin.  This is the ratchet.
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$HERE/.." && pwd)"
. "$HERE/lib_gate.sh"
gate_parse_args "$@"

# The 31 CANNOT-SAY-NO scripts from FINDING-2026-08-22-seat16-rung-gate-false-green-audit.md, by tier.
GATES_T1="test_gate_rtcc_claimed_regs test_gate_omega_own_k test_gate_no_c_to_bb test_gate_no_vstack test_gate_pl_coupling"
GATES_T2="test_gate_zeta_no_arena test_gate_rbp_census_ratchet test_gate_sn7_beauty_self_host"
GATES_T3="test_gate_bb_emit_blind test_gate_fb_predicate_tripwire test_gate_icn_zk5_gva test_gate_fc_no_residual_rbp test_gate_no_handencoded_bytes test_gate_no_hidden_global_in_emitted test_gate_argnote_sweep test_gate_asm_tabs_identity test_gate_clobarm test_gate_emit_no_ir_mutation test_gate_emit_no_lang test_gate_emit_no_slot_alloc test_gate_s130_behav test_gate_s130_blast"
GATES_T4="test_gate_em_template_byte_identity test_gate_const_graph test_gate_rtx_inventory_live test_gate_pl_m34_parity test_gate_pas_frame_pairing audit_bb_fixup_rank audit_jcon_wholesale bench_min_of_n bench_prolog_ix_ab"
ALL="$GATES_T1 $GATES_T2 $GATES_T3 $GATES_T4"

TIMEOUT="${GATE_TIMEOUT:-25}"
WORK=$(mktemp -d); trap 'rm -rf "$WORK"' EXIT
REFUSED=0; VACUOUS=0; N=0
VACUOUS_LIST=""

for g in $ALL; do
    N=$((N+1))
    src="$ROOT/scripts/$g.sh"
    [ -f "$src" ] || { echo "  MISSING   $g  (script not in scripts/ -- pin is stale)"; VACUOUS=$((VACUOUS+1)); VACUOUS_LIST="$VACUOUS_LIST $g(missing)"; continue; }
    S="$WORK/$g"
    mkdir -p "$S/SCRIP/scripts" "$S/SCRIP/src/emitter" "$S/SCRIP/src/templates" "$S/SCRIP/src/runtime" "$S/SCRIP/out" "$S/corpus"
    cp "$src" "$S/SCRIP/scripts/" 2>/dev/null
    cp "$ROOT/scripts/lib_gate.sh" "$S/SCRIP/scripts/" 2>/dev/null
    cp "$ROOT/scripts/lib_oracle_flags.sh" "$S/SCRIP/scripts/" 2>/dev/null
    rc=0
    ( cd "$S/SCRIP" && S4E_HOME="$S" CORPUS="$S/corpus" SCRIP="$S/SCRIP/scrip" SCRIP_BIN="$S/SCRIP/scrip" \
        RT_DIR="$S/SCRIP/out" timeout "$TIMEOUT" bash "$S/SCRIP/scripts/$g.sh" ) >"$S/out.txt" 2>&1 || rc=$?
    if [ "$rc" -eq 0 ]; then
        echo "  ⛔ VACUOUS  $g  -- exit 0 on an EMPTY tree"
        VACUOUS=$((VACUOUS+1)); VACUOUS_LIST="$VACUOUS_LIST $g"
    else
        echo "  ✅ REFUSED  $g  (exit $rc)"
        REFUSED=$((REFUSED+1))
    fi
done

echo ""
echo "examined $N gate script(s): REFUSED=$REFUSED  VACUOUS=$VACUOUS"
[ -n "$VACUOUS_LIST" ] && { echo "still vacuous:"; for v in $VACUOUS_LIST; do echo "    $v"; done; }
gate_floor "$N" 31 "gate scripts (the pinned V2-5 worklist)"
gate_verdict "$VACUOUS" "gate(s) still return 0 on an empty tree"
