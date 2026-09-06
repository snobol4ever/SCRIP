#!/usr/bin/env bash
# scripts/test_corpus_snobol4.sh — SNOBOL4 broad corpus, modes 2+3+4
# Mode-4 gate (hard). Modes 2+3 informational. Reinstated 2026-06-08.
# Compares output against .ref files. Reports PASS/FAIL/SKIP per mode.
#
# Self-contained per RULES.md: paths from $0, timeout on every run.
# AUTHORS: Lon Jones Cherryholmes · Claude Sonnet 4.6   DATE: 2026-05-14
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME: the sibling root (all repos + oracles are siblings under ONE root; /home/claude2-style seat roots work with zero env; S4E_HOME overrides)

HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SCRIP="${SCRIP:-$HERE/../scrip}"
RT_DIR="${RT_DIR:-$HERE/../out}"
CORPUS="$S4E/corpus"
# ⛔⭐⭐ THE BINARY MAY NOT MOVE UNDER A RUNNING BOARD. Measured here 2026-09-06 (hq_S): a board was started and
# an incremental `make` was run while it was still grading. The harness happened to try to exec ./scrip during
# the relink, died with PermissionError, and REFUSED -- which is the honest outcome and the ONLY reason it was
# noticed. Had the relink finished a second earlier, half the population would have been graded on one binary
# and half on another, and the board would have printed A COMPLETE, PLAUSIBLE TABLE with no way for any reader
# to tell. That is this house's own recurring shape: an instrument that cannot fail prints exactly like one
# that passed.
# ⛔ FINGERPRINT BOTH, AND THE SECOND IS THE ONE THAT MATTERS: ./scrip links the emitter and runtime
# DYNAMICALLY, so it is BYTE-IDENTICAL across the two arms of any emitter or runtime change -- a scrip-only
# check is vacuous exactly when it matters most (measured on an emit.cpp A/B, where two agents verified
# scrip's md5 as proof the binary had not moved and the check was empty for the property in question).
# ⭐ AND THE STRONGER MOVE, WHERE IT IS AFFORDABLE (hq_P, same day): put the change behind a killswitch and run
# BOTH ARMS OFF ONE BUILD, env-only. That removes the hazard rather than detecting it. This guard is for the
# case a killswitch cannot cover -- a board that takes minutes while somebody else, or your own next command,
# rebuilds the tree underneath it.
_sn4_bin_fp() { md5sum "$SCRIP" "$RT_DIR/libscrip_rt.so" 2>/dev/null | cut -c1-12 | tr '\n' ' '; }
_SN4_BIN_FP0="$(_sn4_bin_fp)"
_sn4_bin_unmoved() {   # REFUSES rc=2 if either artifact changed since this board started
    local now; now="$(_sn4_bin_fp)"
    [ -n "$_SN4_BIN_FP0" ] || { echo "⛔ REFUSE(rc=2): could not fingerprint ./scrip and out/libscrip_rt.so at start -- a board that cannot tell whether its binary moved must not print a verdict"; exit 2; }
    [ "$now" = "$_SN4_BIN_FP0" ] || {
        echo "⛔ REFUSE(rc=2): THE BINARY MOVED UNDER THIS BOARD -- start [$_SN4_BIN_FP0] end [$now]"
        echo "   (scrip and libscrip_rt.so, md5 prefixes, in that order). Part of this population was graded on one build and part on another,"
        echo "   so every number above describes no single tree. Re-run on a quiet tree; do NOT quote this board."
        exit 2; }
}
# ⛔⭐ 10s WAS A FAIL FACTORY AT FLEET LOAD, AND THE KILL WAS INDISTINGUISHABLE FROM A WRONG ANSWER
# (hq_C 2026-08-29, verified by hq_B). This bound is PER PROGRAM, not for the board. A program taking 2s on a
# quiet box can exceed 10s at load 30 with ~20 concurrent boards -- and because the captures below said
# `|| true`, rc=124 was DISCARDED at the point of capture, the SIGTERM-truncated stdout was compared to the
# .ref like any other answer, and the board printed FAIL. So two runs of THE CORRECTNESS BLOCKING FLOOR on an
# identical tree at different fleet loads could legitimately disagree, with nothing in the output saying which
# was contended. Raised to an order of magnitude over the measured per-program cost (a 1381-program board
# measures M3=20s M4=53s in total, i.e. tens of ms each) rather than beside it.
# ⛔ The raise alone does NOT remove the ambiguity, it only moves the threshold. The real cure is the third
# state: see the rc capture in run_test and the TIMEOUT-KILLED refusal near the verdict.
# ⛔⭐⭐ TIMEOUT HAS TWO CONSUMERS AND THEY DISAGREE ABOUT WHO SET IT (hq_C 2026-08-29, own-goal report;
# asymmetry measured by hq_B). This script reads it per program, AND corpus_suite_harness.py:120 reads the
# SAME env var per suite entry (float(os.environ.get("TIMEOUT","10"))). Nobody had documented that.
# ⛔ THE TRAP IS THE ASYMMETRY, not the sharing:
#   `TIMEOUT=600 bash scripts/test_corpus_snobol4.sh`  -> puts it in the ENVIRONMENT, so the harness
#                                                          INHERITS it and every suite entry gets 600s too.
#   the assignment below                                -> NOT exported, so the harness does NOT see it and
#                                                          keeps its own default of 10.
# So the identical name means different things depending on WHERE it was set, and only the command-line form
# reaches the families. hq_C set 600 believing they were buying immunity from load-manufactured FAILs; it
# propagated into the harness and made every expected-hang XFAIL entry burn up to 600s PER MODE -- a ~50
# minute board against 466s here -- and the verdict was IDENTICAL either way, because those entries are XFAIL
# at 10s and XFAIL at 600s. A large cost for a hazard that did not exist on this population.
# ⚠️ If you want to change what the FAMILIES get, export it or set it per-call (see the TIMEOUT=30 below).
# Changing the value here moves only the loose/hardcoded programs.
TIMEOUT="${TIMEOUT:-120}"
# ⭐ SHARDED / RESUMABLE ARM (row corpus-runner-master-suite-exceeds-single-call-cap, hq_B 2026-09-02). The master block
# is one 1700+-entry harness run (~220s here alone, ~7.7 min under fleet load), so a caller under a single-call cap got a
# REFUSAL (rc=2, "no SUITE_BOARD line") instead of a verdict -- honest, but a blocking gate nobody can run in one call is a
# gate nobody runs. Now: `--shard k/N` grades ONLY the master's k-th interleaved N-th of the entries (the harness's own
# --shard, so every entry lands in exactly one shard) and writes its SUITE_BOARD to a CHECKPOINT stamped with the scrip
# binary's and the master pair's md5 -- then exits WITHOUT a verdict ("verdict at --combine"). `--combine N` reads the N
# checkpoints, REFUSES rc=2 if any is missing or was cut on a different binary/master (stale by construction, never summed),
# SUMS every count into one synthesized board line, and then runs everything else (demo arms, floors, GATE line) exactly
# as the monolithic run does. PROVEN byte-equal once: monolithic summary == --shard 1/3, 2/3, 3/3 + --combine 3 (ledger).
# Checkpoints live under $SCRIP_BOARD_CKPT (default /tmp/si_board_shards<seat-root-with-dashes>, per checkout like the objdir).
SHARD=""; COMBINE=""
while [ $# -gt 0 ]; do
    case "$1" in
        --shard=*)   SHARD="${1#--shard=}";;
        --shard)     shift; SHARD="${1:-}";;
        --combine=*) COMBINE="${1#--combine=}";;
        --combine)   shift; COMBINE="${1:-}";;
        -h|--help)   sed -n '/^# ⭐ SHARDED/,/^SHARD=""/p' "$0" | grep '^#' | sed 's/^# \{0,1\}//'; exit 0;;
        *) echo "⛔ REFUSED: unknown argument '$1' (accepted: --shard k/N | --combine N | --help)"; exit 2;;
    esac; shift
done
case "$SHARD" in ""|[0-9]*/[0-9]*) :;; *) echo "⛔ REFUSED: --shard wants k/N, got '$SHARD'"; exit 2;; esac
case "$COMBINE" in ""|[1-9]*) :;; *) echo "⛔ REFUSED: --combine wants N (a count), got '$COMBINE'"; exit 2;; esac
[ -n "$SHARD" ] && [ -n "$COMBINE" ] && { echo "⛔ REFUSED: --shard and --combine are two different calls, not one"; exit 2; }
CKPT="${SCRIP_BOARD_CKPT:-/tmp/si_board_shards$(printf '%s' "$S4E" | tr / -)}"
board_stamp() { printf 'stamp scrip=%s master=%s-%s\n' "$(md5sum < "$SCRIP" | cut -c1-12)" "$(md5sum < "$MASTER_SNO" | cut -c1-12)" "$(md5sum < "$MASTER_REF" | cut -c1-12)"; }
board_checkpoint_write() {   # <k/N> <run board line> <ast board line>
    local f="$CKPT/master.${1%/*}-of-${1#*/}.board"; mkdir -p "$CKPT" || { echo "⛔ REFUSED: cannot create checkpoint dir $CKPT"; exit 2; }
    { board_stamp; printf '%s\n' "$2"; printf '%s\n' "$3"; } > "$f"
    echo "SHARD $1 boarded -> $f"; echo "   $2"; echo "   $3"; echo "   no verdict here: run the other shards, then '$0 --combine ${1#*/}' for the GATE line"
}
# ⛔⭐ BOTH POPULATIONS OR NEITHER (hq_B 2026-09-05). The master is graded by TWO board lines since the run below
# carries --by-modes-column: SUITE_BOARD over the run-graded entries and SUITE_BOARD_AST over the ones whose .ref is
# a --dump-ast dump. Summing only the run line would hand --combine's caller a verdict that LOOKS whole while the ast
# population went ungraded -- the silently-narrowed denominator this runner refuses everywhere else, reintroduced by
# the very change that removed the loud refusal. A combined board must lose nothing the monolithic run had.
# ⭐ `shard` is EXCLUDED from the sum: it matches the generic key pattern, so the old loop summed 1/3+2/3+3/3 and
# printed `shard=6` on a line that grades no shard at all. Harmless to field(), and a plain lie to a human reader.
board_combine() {   # <N> -> prints the synthesized SUITE_BOARD and SUITE_BOARD_AST lines on stdout; refusals on stderr, rc 2
    local n="$1" k f want="$(board_stamp)" got line prefix key kv
    for k in $(seq 1 "$n"); do
        f="$CKPT/master.$k-of-$n.board"
        [ -f "$f" ] || { echo "⛔ GATE REFUSES: checkpoint for shard $k/$n missing at $f -- run '$0 --shard $k/$n' first; a partial sum is not a board" >&2; return 2; }
        got="$(sed -n 1p "$f")"; [ "$got" = "$want" ] || { echo "⛔ GATE REFUSES: checkpoint $k/$n was cut on a different tree ($got vs now $want) -- stale by construction, re-run that shard" >&2; return 2; }
    done
    for prefix in SUITE_BOARD SUITE_BOARD_AST; do
        unset sum; declare -A sum; local -a keys=()
        for k in $(seq 1 "$n"); do
            f="$CKPT/master.$k-of-$n.board"
            line="$(grep -m1 "^$prefix " "$f")"; [ -n "$line" ] || { echo "⛔ GATE REFUSES: checkpoint $k/$n carries no $prefix line -- it predates the two-population board; re-run '$0 --shard $k/$n'" >&2; return 2; }
            echo "$line" | grep -q " shard=$k/$n " || { echo "⛔ GATE REFUSES: checkpoint $k/$n's $prefix is tagged '$(echo "$line" | grep -oE 'shard=[0-9/]+')', not shard=$k/$n" >&2; return 2; }
            for kv in $(echo "$line" | grep -oE '[a-z0-9_]+=[0-9]+' | grep -v '^shard='); do key="${kv%%=*}"; [ -n "${sum[$key]+x}" ] || keys+=("$key"); sum[$key]=$(( ${sum[$key]:-0} + ${kv#*=} )); done
        done
        printf '%s family=ALL combined=%s' "$prefix" "$n"; for key in "${keys[@]}"; do printf ' %s=%s' "$key" "${sum[$key]}"; done; printf '\n'
    done
}
TMOUT3=0; TMOUT4=0; TMOUT_LIST=""
INC="${INC:-$CORPUS/include}"
BEAUTY="${BEAUTY:-$CORPUS/tests/snobol4/beauty_suite}"
DEMO="${DEMO:-$CORPUS/demos/snobol4}"

# ⛔⛔⛔ s272 hq_C (CEO audit correction 2) — A MISSING PREREQUISITE IS A REFUSAL (rc=2), NEVER A GREEN EXIT.
# These two arms printed SKIP and exited 0, so a box with no compiler built, or no corpus cloned, reported
# SUCCESS to every caller that reads $? — which is every gate, every board, and handoff_status.sh. That is the
# ABSENT-ORACLE FALSE-GREEN class, and it is the exact twin of the `make test` no-recipe trap: a check that
# ran nothing and said nothing was wrong. ⭐ rc=2 means REFUSED-TO-GRADE and is distinct from rc=1 FAILED:
# a caller can tell 'I could not measure' from 'I measured and it is broken'. Silence could say neither.
if [ ! -x "$SCRIP" ]; then echo "⛔ REFUSED TO GRADE: scrip not built at $SCRIP" >&2; exit 2; fi
if [ ! -d "$CORPUS" ]; then echo "⛔ REFUSED TO GRADE: corpus not found at $CORPUS" >&2; exit 2; fi
# ⛔⭐ A BOARD THAT CANNOT SAY WHAT IT GRADED MUST REFUSE, NOT LABEL (FINDING-2026-08-30-hq_C-the-snobol4-board-
# grades-whatever-scrip-exists-and-labels-that-verdict-with-git-head.md). This script never builds -- it grades
# whichever $SCRIP happens to be sitting in the tree and stamps the report with git HEAD, so a green board was
# not evidence about the SHA it named: a pristine build of the exact commit a board called clean SIGSEGV'd two
# counted entries. gate_require_fresh refuses rc=2 (UNPROVEN, never a silent pass) before grading anything.
if ! . "$HERE/lib_gate.sh" 2>/dev/null || ! command -v gate_require_fresh >/dev/null 2>&1; then
    echo "⛔ REFUSED TO GRADE: lib_gate.sh unavailable or missing gate_require_fresh -- cannot verify binary freshness" >&2
    exit 2
fi
gate_require_fresh "$HERE/.." src "$SCRIP" "$RT_DIR/libscrip_rt.so"
# ⛔⛔ REFUSE ON A MISSING SUBTREE -- DO NOT SILENTLY DISCOVER FEWER PROGRAMS (hq_C s271). This board read
# "PASS=342 FAIL=0" for a whole session because $DEMO pointed at a path that did not exist: every visible signal
# said green while 22 programs had left the denominator. A clean numerator over a shrunken denominator is the most
# dangerous shape a board has, and corpus paths have moved three times in two days, so this WILL happen again.
# FAIL=0 is not a verdict; FAIL=0 over the expected denominator is.
for _d in "$DEMO"; do   # beauty_suite left this list 2026-08-29: its 13 drivers were absorbed into the MASTER suite (one-flat-suite cutover) and the dir no longer exists; the master block refuses on ITS absence instead
    if [ ! -d "$_d" ]; then echo "⛔ GATE REFUSES: corpus subtree missing: $_d"; echo "   The corpus layout moved. Repoint this script; do NOT read a smaller total as a pass."; exit 2; fi
done

PASS3=0; FAIL3=0; FAILURES3=""
PASS4=0; FAIL4=0; SKIP4=0; FAILURES4=""
MISSING=0; MISSING_LIST=""

WORKDIR="$(mktemp -d)"
trap 'rm -rf "$WORKDIR"' EXIT
T_M3=0; T_M4=0; T0_ALL=$SECONDS

compile_mode4() {
    local sno="$1" out="$2"
    local tmp; tmp="$(mktemp -d)"
    SNO_LIB="$INC" "$SCRIP" --compile "$sno" > "$tmp/p.s" 2>/dev/null || { rm -rf "$tmp"; return 1; }
    (cd "$HERE/.." && gcc -c "$tmp/p.s" -o "$tmp/p.o" 2>/dev/null) || { rm -rf "$tmp"; return 1; }
    # ⛔ -no-pie is NOT applied here, deliberately -- RULED, not blocked: row
    # `m4-pie-vs-no-pie-changes-behaviour-not-just-signal` (seat10 2026-08-28), full analysis in
    # .github/ARCH-ENGINE.md § "Mode-4 Link Mode". gdb on the faulting RSP showed -no-pie is the broken arm, not
    # a measurement artifact: fz_red_m2a_fence_cap_gen / fz_segv_10 are .ref="match" (not crash-expected), PIE
    # runs them clean 20/20, -no-pie SIGSEGVs them 20/20 with RSP==0x0 at fault -- a normal stack address at the
    # identical point under PIE. See corpus_suite_harness.py's compile_m4() for the mirrored banner.
    gcc "$tmp/p.o" -L"$RT_DIR" -lscrip_rt -lm \
        -Wl,-rpath,"$RT_DIR" -o "$out" 2>/dev/null || { rm -rf "$tmp"; return 1; }
    rm -rf "$tmp"
}

# ⛔⛔ A HARDCODED PATH THAT NO LONGER RESOLVES IS A MISSING PROGRAM, NOT A MISSING TEST (hq_C s272, seat04's find).
# The s271 guard above checks the DIRECTORIES exist; it cannot see that $DEMO/wordcount/wordcount.sno stopped
# resolving when the tree re-nested underneath it. These two lines used to `return` silently, so each of the ~40
# hardcoded demo rows below could leave the board with NO signal at all -- neither PASS, nor FAIL, nor SKIP. The
# corpus demo paths have now been repointed FIVE times in one day (6ce46ebc dac73079 843cacfb 1177e66e 50923f55),
# and every single break was noticed only because a human recognised the printed total had shrunk. That is not an
# instrument. ⛔ The DISCOVERED corpora (crosscheck, beauty) filter a ref-less .sno BEFORE calling here, so a
# missing file can only ever mean a stale hardcoded path -- which is always a defect and never a legitimate skip.
run_test() {
    local label="$1" sno="$2" ref="$3" input="${4:-}" filter="${5:-}"
    if [ ! -f "$sno" ]; then MISSING=$((MISSING+1)); MISSING_LIST="${MISSING_LIST}  ${label}: no program at ${sno}\n"; return; fi
    if [ ! -f "$ref" ]; then MISSING=$((MISSING+1)); MISSING_LIST="${MISSING_LIST}  ${label}: no oracle ref at ${ref}\n"; return; fi
    local exp; exp=$(cat "$ref")
    local slug; slug=$(echo "$label" | tr '/: ' '_')
    local inp_arg; [ -n "$input" ] && [ -f "$input" ] && inp_arg="$input" || inp_arg=""


    # ── Mode 3: --run ──────────────────────────────────────────────────────
    local T0m3=$SECONDS
    local got3
    # ⛔⭐ CAPTURE THE rc INSTEAD OF THROWING IT AWAY. `|| true` discarded rc=124, so the runner never held
    # the one byte that says "we did not wait long enough" -- it then compared truncated stdout to the .ref and
    # reported the only fact it had left. A SIGTERMed program was NOT MEASURED: neither PASS nor FAIL but
    # UNPROVEN, the same three-state doctrine lib_gate.sh exists to enforce.
    local rc3=0
    if [ -n "$inp_arg" ]; then
        got3=$(SNO_LIB="$INC" timeout "$TIMEOUT" "$SCRIP" --run "$sno" < "$inp_arg" 2>/dev/null); rc3=$?
    else
        got3=$(SNO_LIB="$INC" timeout "$TIMEOUT" "$SCRIP" --run "$sno" < /dev/null 2>/dev/null); rc3=$?
    fi
    [ -n "$filter" ] && got3=$(printf '%s\n' "$got3" | grep -v "$filter" || true)
    T_M3=$((T_M3+SECONDS-T0m3))
    if [ "$rc3" -eq 124 ]; then
        TMOUT3=$((TMOUT3+1)); TMOUT_LIST="${TMOUT_LIST}  TIMEOUT-M3 ${label} (killed at ${TIMEOUT}s -- NOT graded)\n"
    elif [ "$got3" = "$exp" ]; then PASS3=$((PASS3+1))
    else FAIL3=$((FAIL3+1)); FAILURES3="${FAILURES3}  FAIL-M3 ${label}\n"; fi

    # ── Mode 4: --compile → assemble → link → run ─────────────────────────
    local T0m4=$SECONDS
    if [ ! -f "$RT_DIR/libscrip_rt.so" ]; then SKIP4=$((SKIP4+1)); return; fi
    local bin="$WORKDIR/${slug}.bin"
    if ! compile_mode4 "$sno" "$bin"; then SKIP4=$((SKIP4+1)); FAILURES4="${FAILURES4}  SKIP(compile/link) ${label}\n"; return; fi
    local got4
    local rc4=0
    if [ -n "$inp_arg" ]; then
        got4=$(SNO_LIB="$INC" timeout "$TIMEOUT" "$bin" < "$inp_arg" 2>/dev/null); rc4=$?
    else
        got4=$(SNO_LIB="$INC" timeout "$TIMEOUT" "$bin" < /dev/null 2>/dev/null); rc4=$?
    fi
    [ -n "$filter" ] && got4=$(printf '%s\n' "$got4" | grep -v "$filter" || true)
    T_M4=$((T_M4+SECONDS-T0m4))
    if [ "$rc4" -eq 124 ]; then
        TMOUT4=$((TMOUT4+1)); TMOUT_LIST="${TMOUT_LIST}  TIMEOUT-M4 ${label} (killed at ${TIMEOUT}s -- NOT graded)\n"
    elif [ "$got4" = "$exp" ]; then PASS4=$((PASS4+1))
    else FAIL4=$((FAIL4+1)); FAILURES4="${FAILURES4}  FAIL ${label}\n"; fi
}

# ── Crosscheck corpus ──────────────────────────────────────────────────────────
while IFS= read -r sno; do
    ref="${sno%.sno}.ref"
    input="${sno%.sno}.input"
    [ ! -f "$ref" ] && continue
    label=$(basename "$sno" .sno)
    run_test "$label" "$sno" "$ref" "$input" ""
done < <(find "$CORPUS/crosscheck" -name "*.sno" 2>/dev/null | sort)
# ⛔⭐ THE LOOSE CROSSCHECK TREE IS GONE, AND ITS ABSENCE HAS TWO CAUSES THAT LOOK IDENTICAL HERE
# (hq_B 2026-08-29, on seat16's report of `find: corpus/crosscheck: No such file or directory`).
#   (a) CONVERTED -- every loose file became a suite family under $CORPUS/tests/snobol4/crosscheck, which the
#       suite block below discovers and grades. Correct end state; the loop above SHOULD find nothing.
#   (b) MOVED/RENAMED -- the layout shifted again and this path is simply stale, in which case the board loses
#       the whole subtree and still prints FAIL=0.
# The $DEMO/$BEAUTY guard above already refuses (b) for its two subtrees, with the right words: "A clean
# numerator over a shrunken denominator is the most dangerous shape a board has, and corpus paths have moved
# three times in two days, so this WILL happen again." It happened again, to the ONE subtree not in that list.
# ⛔ So crosscheck gets the same refusal, conditioned on the distinguisher the other two do not need: absence of
# the loose tree is acceptable ONLY when the converted families are actually present to be graded instead.
if [ ! -d "$CORPUS/crosscheck" ]; then
    _cc_master="$CORPUS/tests/snobol4/ALL.csv"
    if [ -f "$_cc_master" ] && grep -q ',crosscheck_' "$_cc_master"; then
        echo "note: loose crosscheck tree is absent and its entries live in the MASTER suite (one-flat-suite ruling) — graded by the master block below, not skipped."
    else
        echo "⛔ GATE REFUSES: corpus subtree missing: $CORPUS/crosscheck"
        # ⛔⭐ THIS MESSAGE USED TO NAME $_cc_conv, A VARIABLE THAT NO LONGER EXISTS. The guard was repointed at
        # the MASTER when crosscheck was absorbed into it, but the refusal text kept citing the old
        # directory variable -- and with no `set -u` an undefined name expands to EMPTY, so the refusal read
        # "...exist at " and pointed the reader nowhere. ⭐ Error paths are the least-exercised code in any
        # instrument, so a stale reference there survives every green run and only surfaces on the day
        # something is already wrong -- degrading the diagnostic exactly when the diagnostic is all you have.
        echo "   AND the master suite at $_cc_master carries no crosscheck_ entries either, so this board would grade a"
        echo "   NARROWER DENOMINATOR and still print FAIL=0. The corpus layout moved. Repoint this script;"
        echo "   do NOT read a smaller total as a pass."
        exit 2
    fi
fi

# ── THE ONE FLAT SUITE (Lon 2026-08-29: "one big *.sno and one big *.ref ... Not 10's of folders and 100's of files") ──
# Replaces the three per-family discovery blocks that lived here (crosscheck / probe / top-level-misc --
# each with its own floor): every absorbed family's entries are graded through the ONE master pair, with
# per-entry provenance in ALL.csv's origin column and levels as rank prefixes. The un-absorbable
# residue (beauty_suite drivers, rtx_func_11, linker, probe_loose, stdin classes) is listed LOUDLY in
# ALL.excluded.txt and still graded by its own blocks below (beauty) or its own instruments.
# Counter folding is byte-identical to the retired blocks': fail+crash -> FAIL, hang/unproven -> TMOUT
# (measured-nothing, never a verdict), m4 compile/link skip -> SKIP4. XFAIL entries are graded separately
# by the harness (a pre-existing red never inflates FAIL); XPASS is echoed loudly -- a cured bug whose
# marker nobody promoted is actionable in the opposite direction.
# ⭐ HARNESS honours an environment override (ladder I rung I9, hq_B 2026-09-01): the row's DONE-WHEN injects a fake harness that
# prints a cause line and exits 1, to prove the refusal below CARRIES the harness's stderr. The default is unchanged; the override is
# a test hook, and a test that cannot inject its instrument cannot prove the fail direction.
HARNESS="${HARNESS:-$HERE/corpus_suite_harness.py}"
# the real harness carries a shebang but no executable bit and is run through python3, unchanged; an INJECTED harness that is
# executable is run directly, so a fail-direction proof can substitute any program for it (the I9 DONE-WHEN injects a bash script).
run_harness() { if [ -x "$HARNESS" ]; then "$HARNESS" "$@"; else python3 "$HARNESS" "$@"; fi; }
MASTER_SNO="$CORPUS/tests/snobol4/ALL.sno"
MASTER_REF="$CORPUS/tests/snobol4/ALL.ref"
MASTER_ENTRY_FLOOR="${MASTER_ENTRY_FLOOR:-1576}"   # FLOOR, not a pinned total (RULES.md): growth needs no re-pin; only an attributed retirement may lower it, in the commit that shrinks the master
if [ ! -f "$HARNESS" ]; then
    echo "⛔ GATE REFUSES: corpus_suite_harness.py missing at $HARNESS"; exit 2
fi
if [ ! -f "$MASTER_SNO" ] || [ ! -f "$MASTER_REF" ]; then
    echo "⛔ GATE REFUSES: the master suite is missing at $MASTER_SNO"
    echo "   The per-family suite files were RETIRED INTO the master (Lon 2026-08-29 one-flat-suite ruling);"
    echo "   a checkout without it cannot grade the suite population at all. Pull corpus; do NOT read the"
    echo "   remaining loose blocks' smaller total as the board."
    exit 2
fi
# ⛔ THE HARNESS'S STDERR IS KEPT, NOT DISCARDED (hq_B 2026-09-01). This line read `2>/dev/null`, so when the master run
# produced no SUITE_BOARD line the refusal below could only say THAT it happened -- the cause (a refuse() message, a
# traceback, a linker error, a kill) was gone. Measured live: a board that refused after 7.7 minutes under fleet load
# 18-24 with nothing to read but "no SUITE_BOARD line", while the same harness run by hand with stderr kept was clean.
# ⭐⭐ AND THE EXIT STATUS IS KEPT TOO, WHICH IS THE HALF STDERR CANNOT COVER (seat03 2026-09-01, row
# master-suite-board-refuses-under-fleet-load-..., converged with hq_B's fix above on the same file the same day).
# MEASURED: a SIGTERM'd harness exits **143** and writes **ZERO bytes of stderr** -- Python's default disposition
# terminates without a traceback -- so for the kill case, which is the one this row exists for, keeping stderr alone
# prints an empty tail and still cannot name the cause. `board=$(run_harness ... | grep ...)` also threw the status
# away, because $? is grep's. Three parties read the same bare refusal three different ways on 2026-09-01 (hq_P "my
# binary", hq_C "something kills by name", seat03 "a 600s wrapper timed it out") and all three were guessing.
# ⛔ ALSO READ FROM SOURCE, because it decides which cure this row may take: the harness CANNOT kill "its own
# workers" -- it has none (no multiprocessing/concurrent.futures/threads) and imports no signal module, no os.kill,
# no alarm, no atexit. Its only kill is subprocess.run(timeout=) on a CHILD, which yields a HANG verdict and still
# boards. A missing board therefore means the harness PROCESS died: an external event.
_hout="$(mktemp)"; _herr="$(mktemp)"
# ⛔⭐⭐ --by-modes-column IS MANDATORY HERE, AND ITS ABSENCE MADE THIS RUNNER UNABLE TO MEASURE AT ALL (hq_B
# 2026-09-05). 28 of the master's entries declare `modes=ast` in ALL.csv -- their .ref is a `--dump-ast` DUMP, not
# program output -- and asking for `--modes m3,m4` without this flag would EXECUTE them and diff them against an AST
# dump they were never meant to match. The harness refuses that outright (rc=2, no SUITE_BOARD line), which is the
# right call and is exactly what it did: the SNOBOL4 master, the control arm every seat grades a landing on, produced
# a REFUSAL rather than a verdict from the moment the parser fixtures were absorbed. ⭐ The refusal was honest and
# loud and still cost the whole fleet its instrument -- a check that cannot be satisfied is as blocking as one that
# lies, it just fails in the direction you can trust. board_icon_master.sh, test_gate_pascal_m{3,4}.sh and
# test_raku_ir_full_suite.sh all pass this flag already; SNOBOL4's master was the one that never did.
if [ -n "$COMBINE" ]; then
    _combined=$(board_combine "$COMBINE") || { rm -f "$_hout" "$_herr"; exit 2; }; harness_rc=0
    board=$(printf '%s\n' "$_combined" | grep '^SUITE_BOARD ')
    ast_board=$(printf '%s\n' "$_combined" | grep '^SUITE_BOARD_AST ')
    echo "master: COMBINED from $COMBINE shard checkpoints under $CKPT (each stamped with this binary and master)"
else
    run_harness run "$MASTER_SNO" "$MASTER_REF" --modes m3,m4 --by-modes-column ${SHARD:+--shard "$SHARD"} > "$_hout" 2> "$_herr"; harness_rc=$?
    board=$(grep '^SUITE_BOARD ' "$_hout")
    ast_board=$(grep '^SUITE_BOARD_AST ' "$_hout")
fi
# ⛔ AND THE SECOND BOARD IS CHECKED THE SAME WAY. With --by-modes-column the harness prints SUITE_BOARD_AST
# UNCONDITIONALLY (total=0 if the ast population is empty), so its absence is never "no ast entries" -- it is a
# harness that did not honour the flag, and grading on the run line alone would then be a verdict over an unknown
# fraction of the suite. Refuse, never assume zero: a census that cannot see its population must not print 0.
if [ -z "$ast_board" ] && [ -n "$board" ]; then
    echo "⛔ GATE REFUSES: harness printed SUITE_BOARD but no SUITE_BOARD_AST line -- --by-modes-column was not honoured"
    echo "   REFUSAL cause=harness-printed-no-ast-board signal=none rc=${harness_rc}"
    echo "   CAUSE: the ast-graded entries (ALL.csv modes=ast) would go ungraded while the board still printed a total."
    sed -n '1,40p' "$_herr" | sed 's/^/     | /'
    rm -f "$_hout" "$_herr"; exit 2
fi
if [ -z "$board" ]; then
    echo "⛔ GATE REFUSES: harness produced no SUITE_BOARD line for the master suite"
    # ⭐⭐ THE REFUSAL CARRIES ONE MACHINE-READABLE LINE, NOT ONLY PROSE (hq_C 2026-09-01, same row). The prose
    # below is for a human reading a log; a gate, a sweep or a DONE-WHEN that wants to know "was this refusal a
    # kill or a real red" must not have to parse English. ⛔ MEASURED, and the reason this line exists: the row's
    # own DONE-WHEN greps this file for `killed-by|cause=|signal=` and matched NOTHING, because the prose spells
    # it "CAUSE:" and "KILLED by SIG..." -- the cure was fully landed and correct and the gate still read as
    # not-done. A criterion written against an IMAGINED wording grades spelling, never behaviour (RULES.md
    # § TRANSCRIPTION IS WHERE PROVENANCE DIES). The contract is now: cause= is one of killed-by-signal |
    # harness-exited-nonzero | harness-printed-no-board; signal= is a SIGNAME or none; rc= is the raw status.
    if [ "$harness_rc" -gt 128 ]; then
        sig=$((harness_rc - 128)); signame="$(kill -l "$sig" 2>/dev/null || echo "$sig")"
        echo "   REFUSAL cause=killed-by-signal signal=SIG${signame} rc=${harness_rc}"
        echo "   CAUSE: the harness was KILLED by SIG${signame} (rc=$harness_rc). It did not fail -- it was killed."
        echo "   ⭐ THIS IS NOT YOUR TREE AND NOT A RED BOARD. RE-RUN IT. Do not diagnose the compiler from this."
        [ "$sig" -eq 15 ] && echo "   SIGTERM has a known external source on this box: a box-wide 'pkill -f corpus_suite_harness.py' kills"
        [ "$sig" -eq 15 ] && echo "   EVERY seat's board, not just the caller's (hq_P disclosure 2026-09-01 ~18:28 CDT, 19 processes killed)."
        [ "$sig" -eq 9 ]  && echo "   SIGKILL is usually the OOM killer or systemd-oomd: check 'journalctl --since -10min | grep -i oom'."
    elif [ "$harness_rc" -ne 0 ]; then
        echo "   REFUSAL cause=harness-exited-nonzero signal=none rc=${harness_rc}"
        echo "   CAUSE: the harness EXITED $harness_rc without boarding -- unlike a kill, this IS your tree or the harness."
    else
        echo "   REFUSAL cause=harness-printed-no-board signal=none rc=0"
        echo "   CAUSE: the harness exited 0 but printed no SUITE_BOARD line -- a defect in the harness itself."
    fi
    if [ -s "$_herr" ]; then
        echo "   harness stderr, last lines (the cause lives here, never in the line above):"
        grep -vE '^\s*$' "$_herr" | tail -8 | sed 's/^/     | /'
    else
        echo "   (harness stderr was EMPTY -- expected for a signal death, which writes nothing.)"
    fi
    rm -f "$_hout" "$_herr"; exit 2
fi
rm -f "$_hout" "$_herr"
if [ -n "$SHARD" ]; then board_checkpoint_write "$SHARD" "$board" "$ast_board"; exit 0; fi
field() { echo "$board" | grep -oE "$1=[0-9]+" | cut -d= -f2; }
mt=$(field total)
m3p=$(field m3_pass); m3f=$(field m3_fail); m3c=$(field m3_crash); m3h=$(field m3_hang); m3u=$(field m3_unproven); m3x=$(field m3_xfail); m3xp=$(field m3_xpass)
m4p=$(field m4_pass); m4f=$(field m4_fail); m4c=$(field m4_crash); m4h=$(field m4_hang); m4u=$(field m4_unproven); m4s=$(field m4_skip); m4x=$(field m4_xfail); m4xp=$(field m4_xpass)
# ⛔⭐ THE SECOND POPULATION, GRADED HERE OR NOWHERE (hq_B 2026-09-05). The two boards are printed separately and
# NEVER summed by the harness, deliberately -- their denominators mean different things and one number spanning both
# could not be read. But "not summed" must not decay into "not read": grading only SUITE_BOARD would print a full,
# plausible, entirely green verdict with the ast entries graded by nobody. That is the same silently-narrowed
# population as a hard-coded root list or a truncated listing, and it is reached here by fixing the loud refusal.
# ⭐ AST IS MODE-INDEPENDENT -- a `--dump-ast` diff is taken once, before either mode's codegen -- so it gets its own
# counter and its own line rather than being folded into m3/m4, where one red would have to be double-counted to
# stay symmetric and would then read as two defects.
afield() { echo "$ast_board" | grep -oE "$1=[0-9]+" | cut -d= -f2; }
astt=$(afield total); astp=$(afield ast_pass); astf=$(afield ast_fail); astc=$(afield ast_crash)
asth=$(afield ast_hang); astu=$(afield ast_unproven); astx=$(afield ast_xfail); astxp=$(afield ast_xpass)
ASTFAIL=$(( ${astf:-0} + ${astc:-0} ))
PASS3=$((PASS3+m3p)); FAIL3=$((FAIL3+m3f+m3c)); TMOUT3=$((TMOUT3+m3h+m3u))
PASS4=$((PASS4+m4p)); FAIL4=$((FAIL4+m4f+m4c)); TMOUT4=$((TMOUT4+m4h+m4u)); SKIP4=$((SKIP4+m4s))
[ "$((m3f+m3c))" -gt 0 ] && FAILURES3="${FAILURES3}  FAIL-M3 suite:master (rerun: python3 $HARNESS run $MASTER_SNO $MASTER_REF --modes m3; per-entry attributes: ALL.csv)\n"
[ "$((m4f+m4c))" -gt 0 ] && FAILURES4="${FAILURES4}  FAIL suite:master (rerun: python3 $HARNESS run $MASTER_SNO $MASTER_REF --modes m4; per-entry attributes: ALL.csv)\n"
echo "master: total=$mt · m3 xfail=$m3x xpass=$m3xp · m4 xfail=$m4x xpass=$m4xp"
echo "master-ast: total=$astt pass=$astp FAIL=$ASTFAIL (fail=${astf:-0} crash=${astc:-0} hang=${asth:-0} unproven=${astu:-0}) · xfail=$astx xpass=$astxp — graded by --dump-ast diff, once, not per mode"
[ "$ASTFAIL" -gt 0 ] && FAILURES4="${FAILURES4}  FAIL suite:master-ast $ASTFAIL entr(y/ies) (rerun: python3 $HARNESS run $MASTER_SNO $MASTER_REF --modes m3,m4 --by-modes-column 2>&1 | grep -E '^ *(FAIL|CRASH) ast')\n"
[ "$((m3xp+m4xp))" -gt 0 ] && echo "⭐ XPASS>0: a bug got FIXED and its XFAIL marker was never promoted -- as actionable as a failure, in the opposite direction (names: python3 $HARNESS run ... | grep XPASS)"
# ⛔ THE FLOOR IS OVER THE WHOLE MASTER, NOT THE RUN HALF (hq_B 2026-09-05). SUITE_BOARD's `total` counts only the
# run-graded entries once --by-modes-column is on, so flooring on `mt` alone would silently lower the bar by the size
# of the ast population -- and a floor that shrinks when entries move between populations is not a floor.
_mt_all=$(( mt + ${astt:-0} ))
if [ "$_mt_all" -lt "$MASTER_ENTRY_FLOOR" ]; then
    MISSING=$((MISSING+MASTER_ENTRY_FLOOR-_mt_all))
    MISSING_LIST="${MISSING_LIST}  master-entry-count: master total ${_mt_all} (run ${mt} + ast ${astt:-0}) is under the floor ${MASTER_ENTRY_FLOOR} -- entries vanished from the master, or this checkout is behind origin\n"
fi

# ── Beauty library drivers: RETIRED INTO THE MASTER (one-flat-suite cutover 2026-08-29) ──
# The 13 *_driver.sno/.ref pairs and their .inc companions were absorbed into ALL.* (origins
# beauty_suite__*, see ALL.csv) and the loose pairs deleted; the master block above grades them.

# ── Demo programs ─────────────────────────────────────────────────────────────
# Coverage audit (demo-corpus-coverage-audit, 2026-08-22): wordcount's ref/input were
# missing (silent no-op, neither PASS nor FAIL) -- regenerated via x64/bin/sbl -bf.
run_test "demo_wordcount" "$DEMO/wordcount/wordcount.sno" "$DEMO/wordcount/wordcount.ref" "$DEMO/wordcount/wordcount.input" ""
run_test "demo_treebank"  "$DEMO/treebank/treebank.sno"  "$DEMO/treebank/treebank.ref"  "$DEMO/treebank/treebank.input"  ""
run_test "demo_claws5"    "$DEMO/claws5/claws5.sno"    "$DEMO/claws5/claws5.ref"    "$DEMO/claws5/claws5.input"    ""
TIMEOUT=30 \
run_test "demo_roman"     "$DEMO/roman/roman.sno"     "$DEMO/roman/roman.ref"     ""                      "^ms:"
# 15 rows below newly gated by the same audit -- each independently oracle-verified
# (x64/bin/sbl -bf) and scrip-verified before wiring in; see FINDING-2026-08-22-*-demo-corpus-coverage-audit.md
run_test "demo_arithmetic"          "$DEMO/arithmetic/arithmetic.sno"          "$DEMO/arithmetic/arithmetic.ref"          "" ""
run_test "demo_counter"             "$DEMO/counter/counter.sno"             "$DEMO/counter/counter.ref"             "" ""
run_test "demo_hello"               "$DEMO/hello/hello.sno"               "$DEMO/hello/hello.ref"               "" ""
run_test "demo_pattern_test"        "$DEMO/pattern_test/pattern_test.sno"        "$DEMO/pattern_test/pattern_test.ref"        "" ""
run_test "demo_claws5_match"        "$DEMO/claws5/claws5-match.sno"        "$DEMO/claws5/claws5-match.ref"        "$DEMO/claws5/claws5.input"     ""
run_test "demo_claws5_match_fence"  "$DEMO/claws5/claws5-match-fence.sno"  "$DEMO/claws5/claws5-match-fence.ref"  "$DEMO/claws5/claws5.input"     ""
run_test "demo_treebank_match"      "$DEMO/treebank/treebank-match.sno"      "$DEMO/treebank/treebank-match.ref"      "$DEMO/treebank/treebank.input"   ""
run_test "demo_treebank_match_fence" "$DEMO/treebank/treebank-match-fence.sno" "$DEMO/treebank/treebank-match-fence.ref" "$DEMO/treebank/treebank.input" ""
run_test "demo_treebank_alloc"      "$DEMO/treebank/treebank-alloc.sno"      "$DEMO/treebank/treebank-alloc.ref"      "$DEMO/treebank/treebank.input"   ""
run_test "demo_porter"              "$DEMO/porter/porter.sno"              "$DEMO/porter/porter.ref"              "$DEMO/porter/porter.input"     ""
# calculator-1/-2 (full evaluators) print a trailing nondeterministic "match_ms=" timing
# line -- same class as demo_roman's "^ms:" filter above, just a different literal marker.
run_test "demo_calculator_1"        "$DEMO/calculator/calculator-1.sno"        "$DEMO/calculator/calculator-1.ref"        "$DEMO/calculator/calculator.input" "^match_ms="
run_test "demo_calculator_2"        "$DEMO/calculator/calculator-2.sno"        "$DEMO/calculator/calculator-2.ref"        "$DEMO/calculator/calculator.input" "^match_ms="
# OUTPUT() association to a '[-fn]' descriptor spec, routed by hq_P 2026-08-28: every write to the
# associated variable was silently discarded in BOTH modes (rc=0, ordinary output intact), because
# _OUTPUT_ never called the -fn parser that _INPUT_ had been calling all along. Graded on -f1 rather
# than -f2 because this harness compares stdout; the defect was never fd2-specific.
# feat_io_fd_assoc: retired hardcoded row (one-flat-suite cutover) -- absorbed into the master (origin feat_f21_io_fd_assoc... see ALL.csv), graded there; the loose pair is deleted.
# k41: the IMMEDIATE pattern lambda, the arm that is LANDED (row lang-lambda-pattern-primitives).
# ⛔ NOT k40. k40 is the row's INSTRUMENT, authored before the cure and RED ON PURPOSE until the
# whole feature lands (conditional lambda, backtrack-unqueue, stored-pattern round trip are all
# still unlanded) -- wiring THAT into the board would turn a deliberate measurement into a broken
# gate. A red instrument cannot also be a regression guard, so k41 guards what works and k40 keeps
# measuring the distance left. ⛔ SELF-PINNED, never sbl-graded: SPITBOL cannot even LEX the file --
# sbl -bf dies at the Greek line with ERROR 230 "illegal character" -- so no oracle arm exists.
# k41_lambda_immediate: retired hardcoded row (ceo s283h, probe total-conversion) -- the witness now lives in the conformance2 SUITE (tests/snobol4/probe/conformance2.sno, entry k41_lambda_immediate_landed) and is graded by the suite loop below; keeping the loose-path row too would double-grade it against a path that no longer exists.
# ⭐ hq_B, independently and after the fact: ceo's retirement above is CORRECT and the verification is
# recorded here so nobody re-derives it. corpus/probe was deleted wholesale by corpus c06960a1 (seat16, on
# Lon's order, closing corpus-crosscheck-probe-total-conversion), which left this hardcoded path dangling and
# the FLOOR REFUSING rc=2 for every seat until ceo landed the fix. ⛔ This script warns in its own words that
# retiring an entry PERMANENTLY SHRINKS THE BOARD and must not be the reflex for an unresolvable path -- the
# usual cause is a cross-repo window whose right action is `git pull` in corpus. VERIFIED that this was NOT
# that case: the witness survives as `k41_lambda_immediate_landed`, entry 2 of
# tests/snobol4/probe/conformance2.sno AND of its .ref, a boarded family the probe suite loop already grades.
# Coverage moved rather than vanished, so retiring is bookkeeping -- and a REPOINTED line would double-count.
# ⭐ Corpus paths have moved four times in three days. When a hardcoded path breaks the question is never
# "does the file exist" but "is the COVERAGE still graded somewhere": deleting on the first answer shrinks
# the board, deleting on the second is bookkeeping. Two seats reached the same retirement independently
# (ceo/hq_B above, seat15 concurrently below -- same diagnosis, same fix, converged without coordination).
run_test "demo_calculator_1_match"       "$DEMO/calculator/calculator-1-match.sno"       "$DEMO/calculator/calculator-1-match.ref"       "$DEMO/calculator/calculator.input" ""
run_test "demo_calculator_1_match_fence" "$DEMO/calculator/calculator-1-match-fence.sno" "$DEMO/calculator/calculator-1-match-fence.ref" "$DEMO/calculator/calculator.input" ""
run_test "demo_calculator_2_match"       "$DEMO/calculator/calculator-2-match.sno"       "$DEMO/calculator/calculator-2-match.ref"       "$DEMO/calculator/calculator.input" ""
run_test "demo_calculator_2_match_fence" "$DEMO/calculator/calculator-2-match-fence.sno" "$DEMO/calculator/calculator-2-match-fence.ref" "$DEMO/calculator/calculator.input" ""
run_test "demo_json"                     "$DEMO/json/json.sno"                     "$DEMO/json/json.ref"                     "$DEMO/json/json.input"       ""
run_test "demo_json_match"               "$DEMO/json/json-match.sno"               "$DEMO/json/json-match.ref"               "$DEMO/json/json.input"       ""
run_test "demo_json_match_fence"         "$DEMO/json/json-match-fence.sno"         "$DEMO/json/json-match-fence.ref"         "$DEMO/json/json.input"       ""
# ⭐ s266 -- THE THREE json PROGRAMS ARE UN-SKIPPED. They were excluded on a comment reading "HANGS (m3 AND m4)
# ... needs >30s (currently: forever)" and "wrong verdict on valid JSON". Both cures landed 2026-08-23: the hang
# was multi-choice pattern blobs having no drift-immune choice record and no blob re-entry (SCRIP d6eafac3), and
# the citm-scale stack leak was bare-FENCE0 static release (hq_P a42571b7). Measured at that HEAD, stderr kept
# SEPARATE from stdout: all three PASS in m3 AND m4 against their oracle refs. ⛔ THE SEPARATION IS LOad-BEARING
# and is why this looked broken twice: json/calculator write `match_ms=` to TERMINAL precisely so stdout stays
# byte-comparable, so any harness capturing with 2>&1 merges a timing line into the graded stream and reports a
# DIFF that is pure instrument. hq_C re-made that exact mistake while verifying this un-skip and caught it only
# by diffing against the LIVE oracle, which agreed with scrip byte-for-byte while the .ref appeared not to.
# ⭐ The denominator moves 361 -> 364. A skip is a silent subtraction from the denominator: these three were
# green for hours and no board could say so, because the runner had been told once that they hang forever.
# NOT gated -- each has a one-line reason, full repro in the FINDING above:
#   demo/snobol4/calculator/calculator-2.sno                -- diverges from the live oracle almost immediately (not the match_ms line); real bug, not nondeterminism (no RANDOM/RAND in program or generator)
#   demo/expression.sno                  -- -INCLUDEs 15 files (global.sno, ShiftReduce.sno, Gen.sno, ...) absent from this checkout; won't parse

T_ALL=$((SECONDS-T0_ALL))
TOTAL=$((PASS4+FAIL4+SKIP4))
echo "mode-3 (--run):     PASS=$PASS3 FAIL=$FAIL3"
echo "mode-4 (--compile): PASS=$PASS4 FAIL=$FAIL4 SKIP=$SKIP4  ($TOTAL total)"
[ -n "$FAILURES3" ] && printf "$FAILURES3" | head -20
[ -n "$FAILURES4" ] && printf "$FAILURES4" | head -40

printf "TIME M3=%ds M4=%ds TOTAL=%ds\n" "$T_M3" "$T_M4" "$T_ALL"
[ "$((TMOUT3+TMOUT4))" -gt 0 ] && printf "TIMEOUT-KILLED m3=%d m4=%d at %ss/program — NOT graded, NOT failures\n" "$TMOUT3" "$TMOUT4" "$TIMEOUT"
# ⛔⭐ STAMP THE TREE ON THE BOARD ITSELF (seat09's ask, 2026-08-29, from a live disagreement).
# seat09 and hq_B both ran THIS SCRIPT and got different denominators -- 1339 and 1377 -- each reproducible
# on demand in its own root. Neither number is stale and neither of us could settle it, because the output
# said what was measured but not WHAT TREE IT WAS MEASURED ON. A denominator is a fact about a corpus, and
# this board folds DEMO + BEAUTY + crosscheck families + misc from a SEPARATE REPO whose commit the SCRIP
# hash does not pin: two roots can sit at the same SCRIP commit and different corpus commits and disagree
# by 38 programs with nothing in either transcript to show it.
# ⭐ lib_gate.sh already stamps per-repo HEAD + dirty/clean + machine on every gate verdict, but THE BOARD
# THAT MATTERS MOST DID NOT SOURCE IT -- the instrument most often quoted in commit messages was the one
# with no provenance. Sourced tolerantly: if the lib is unreachable the board still runs, because a missing
# stamp must never cost a measurement.
if . "$HERE/lib_gate.sh" 2>/dev/null && command -v gate_stamp >/dev/null 2>&1; then gate_stamp
else echo "    (tree stamp unavailable — lib_gate.sh not sourced; record SCRIP and corpus HEAD by hand when quoting these numbers)"; fi

# ⛔⛔ THE VERDICT IS AN EXIT CODE, NOT A PRINTED NUMBER (hq_C s272). Until now this script's last statement was
# the printf above, so it exited 0 with any number of mode-4 failures -- the SAME false-green shape as `make test`,
# sitting inside the blocking set itself. CLAUDE.md has said "mode-4 is the hard gate" throughout; nothing enforced
# it. m3 stays informational per that documented contract, but it is printed in the verdict so it cannot hide.
# ⛔⭐ A TIMEOUT-KILLED PROGRAM WAS NOT MEASURED, SO IT SHRINKS THE DENOMINATOR EXACTLY LIKE A MISSING PATH.
# Reported and refused SEPARATELY from FAIL, because "answered wrong" and "we did not wait long enough" are
# different facts and this board printed one word for both. ⭐ The separation is load-bearing, not the raised
# bound: at ANY bound a contended box can cross it, so the instrument must be able to SAY it was contended
# rather than silently convert contention into a correctness regression.
if [ "$((TMOUT3+TMOUT4))" -gt 0 ]; then
    echo "⛔ GATE REFUSES: $((TMOUT3+TMOUT4)) program(s) KILLED at the ${TIMEOUT}s per-program bound (m3=$TMOUT3 m4=$TMOUT4) -- NOT graded:"
    printf "$TMOUT_LIST"
    echo "   These are NOT failures and NOT passes. The board is SMALLER than its PASS counts suggest."
    echo "   ⛔ Do NOT read this as a regression. Re-run on a quieter box, or raise the bound:"
    echo "      TIMEOUT=600 bash scripts/test_corpus_snobol4.sh"
    echo "   ⭐ A program killed even on an idle box at a generous bound is a real hang and wants a row."
    exit 2
fi
if [ "$MISSING" -gt 0 ]; then
    echo "⛔ GATE REFUSES: $MISSING hardcoded corpus path(s) no longer resolve -- the board is SMALLER than it looks:"
    printf "$MISSING_LIST"
    # ⛔⭐⭐ WHY THIS BLOCK NOW DIAGNOSES BEFORE IT ADVISES (hq_P 2026-08-27 s276, after THREE seats in ONE DAY).
    # This refusal used to end with the flat instruction "Repoint them". ⛔ THAT NAMED THE WRONG REMEDY FOR THE
    # DOMINANT CAUSE, AND THE WRONG REMEDY IS THE IRREVERSIBLE ONE. A suite family lands as a CROSS-REPO change --
    # the corpus commit carrying <family>.{sno,ref} and the SCRIP commit adding it to the family list -- and there
    # is no atomic commit across two repos, so for a window any checkout that pulled one and not the other sees
    # paths that "do not resolve". The correct action there is `git pull` in corpus. REPOINTING OR RETIRING THE
    # ENTRY INSTEAD PERMANENTLY SHRINKS THE BOARD, and the gate was the thing telling seats to do it: seat12
    # proposed retiring two live families, seat11 proposed repointing seven, and a third report reached seat08
    # secondhand as a phantom regression -- all in one day, all steered by this message.
    # ⭐ THE FIX IS TO ASK THE QUESTION THE SEAT CANNOT: does the path exist at origin? If yes the checkout is
    # STALE; if no it is genuinely gone. Same file, same symptom, opposite cures -- so the gate must not guess.
    # ⛔ AND IT MUST DEGRADE TO "UNDETERMINED", NEVER TO THE DESTRUCTIVE ADVICE: with no origin, no network, or a
    # failed fetch we print both possibilities and recommend NOTHING. An instrument that cannot measure refuses.
    _stale=0; _gone=0; _undet=0
    if [ -d "$CORPUS/.git" ] && git -C "$CORPUS" rev-parse --verify -q origin/main >/dev/null 2>&1; then
        timeout 20s git -C "$CORPUS" fetch -q origin 2>/dev/null || true
        for _p in $(printf "$MISSING_LIST" | grep -oE "$CORPUS/[^ ]+" | sort -u); do
            _rel="${_p#$CORPUS/}"
            if git -C "$CORPUS" cat-file -e "origin/main:$_rel" 2>/dev/null; then _stale=$((_stale+1)); else _gone=$((_gone+1)); fi
        done
    else _undet=1; fi
    _behind="$(git -C "$CORPUS" rev-list --count HEAD..origin/main 2>/dev/null || echo '?')"
    if [ "$_stale" -gt 0 ]; then
        echo "   ⛔⭐ DIAGNOSIS: YOUR CORPUS CHECKOUT IS STALE -- $_stale of the unresolved path(s) EXIST at origin/main"
        echo "      (corpus is $_behind commit(s) behind origin). ⛔ DO NOT REPOINT AND DO NOT RETIRE THESE ENTRIES:"
        echo "      they are live files and the entries are correct. Repointing would shrink the board permanently."
        echo "      ✅ FIX:  git -C $CORPUS pull --ff-only     then re-run this gate."
        [ "$_gone" -gt 0 ] && echo "      ⚠ $_gone other path(s) are absent at origin too -- treat those separately, see below."
    fi
    if [ "$_gone" -gt 0 ] && [ "$_stale" -eq 0 ]; then
        echo "   DIAGNOSIS: $_gone path(s) are absent at origin/main too, so this is NOT a stale checkout."
        echo "      Repoint or retire those family entries; do NOT read the shrunken total as a pass."
    fi
    if [ "$_undet" -eq 1 ]; then
        echo "   ⚠ UNDETERMINED: no corpus origin/main to compare against, so this gate CANNOT tell a stale checkout"
        echo "      from a genuinely retired suite. ⛔ Recommending nothing -- establish which it is before changing anything."
    fi
    echo "   FAIL=0 over a shrunken denominator is not green."
    exit 2
fi
# ⛔⭐⭐ THE ONE LEADERBOARD, AND THIS IS THE HIGHEST-TRAFFIC BOARD WE HAVE -- it sits inside `make test`,
# so every seat runs it constantly, which makes its SCORE.md row both the most worth keeping live and the
# only one whose write could disturb twenty other roots. RULED by ceo CEO-174 (2026-09-03) on hq_T's ask:
# wire it, with this line naming itself out loud so the resulting .github change is never a haunting, AND
# write the row ONLY when the measured tree is clean and committed. That condition lives in the helper, is
# COMPUTED from the tree and cannot be chosen -- a dirty-tree number is a scouting datum, printed here and
# deliberately not landed, because it names a tree nobody else can check out. The clean run IS the landing
# run, so the row rides the landing's own .github-last push and no seat meets an unexplained dirty .github.
# ⛔ Placed ABOVE the FAIL exit: a red board is still a measurement, and a leaderboard that records only
# green runs is a trophy cabinet showing each suite's best remembered day rather than its state.
# ⛔⭐ A TIMEOUT-KILLED PROGRAM IS MISSING FROM THE DENOMINATOR AND THE ROW MUST SAY SO (hq_T ruling
# 2026-09-04, on hq_C's measurement: 4 programs KILLED at the 120s bound under load 3.6 with sixteen seats
# up). This board ALREADY tells the terminal reader -- "TIMEOUT-KILLED m3=N m4=N ... NOT graded, NOT
# failures" -- and told the LEADERBOARD nothing, so the published row read as a full-population measurement
# while N programs had never run. That is the two-audiences shape exactly: the human sees the caveat, the
# board that everyone quotes does not.
# ⭐ THE RULING, written down here because this is where it binds: THE BOUND DOES NOT SCALE WITH LOAD. A
# bound that moves with the machine makes a verdict irreproducible -- the same program passes or fails
# depending on its neighbours, and two roots could never reconcile a disagreement. And boards do NOT need a
# quiet boundary for CORRECTNESS, because a kill is already bucketed as NOT GRADED and NOT A FAILURE. What
# was actually wrong was a CONSUMER rendering that refusal as "FAIL m3=UNPARSEABLE" (rowed separately). The
# only thing owed here is that the row carry the caveat the terminal already carries.
_sn4_killed=""
[ "$((TMOUT3+TMOUT4))" -gt 0 ] && _sn4_killed=" · ⛔ TIMEOUT-KILLED m3=$TMOUT3 m4=$TMOUT4 at ${TIMEOUT}s/program — NOT graded and NOT failures, so this row is measured over a SHORT denominator: re-run at a quieter moment before quoting it as the population"
# ⛔ THE ROW CARRIES THE AST POPULATION TOO, for the reason the TIMEOUT-KILLED caveat above exists: the terminal
# reader sees both boards and the leaderboard everybody quotes would have seen only one, which is the two-audiences
# defect. Fraction form kept on both (util_score_row.py refuses a grid write without one).
_sn4_board="m3 $PASS3/$TOTAL FAIL=$FAIL3 · m4 $PASS4/$TOTAL FAIL=$FAIL4 SKIP=$SKIP4 · ast $astp/$astt FAIL=$ASTFAIL MISSING=0$_sn4_killed (\`test_corpus_snobol4.sh\`)"
echo "ONE LEADERBOARD: recording this board into .github/SCORE.md (test_corpus_snobol4.sh; skipped with a notice if the tree is dirty)"
python3 "$HERE/util_score_row.py" write --lang snobol4 --column board --modes m3,m4 \
    --measurer "${S4E_SEAT:-}" --text "$_sn4_board" \
    || echo "⚠ SCORE.md NOT UPDATED -- record this row by hand (the REFUSED line above says why)"
# ⭐ THE PROGRESS LINE, after the rewrite (see board_icon_master.sh for the same call and why it is here
# rather than only in lib_gate.sh: this runner writes its row directly, bypassing gate_score_row).
python3 "$HERE/util_score_row.py" progress 2>/dev/null || true
# ⛔ THE BINARY CHECK COMES BEFORE THE VERDICT, not after it: a board graded across a relink must REFUSE
# rather than choose between PASS and FAIL, and it must refuse before anything downstream (the leaderboard row
# above included) can quote it.
_sn4_bin_unmoved
# ⛔ AST FAILURES BLOCK. A parser fixture whose --dump-ast diff moved is a real red in the shared front end -- it
# reaches BOTH modes, so calling it informational the way mode-3 is would be strictly weaker than either mode's bar.
if [ "$FAIL4" -gt 0 ] || [ "$ASTFAIL" -gt 0 ]; then
    echo "⛔ GATE FAIL: mode-4 FAIL=$FAIL4 · ast FAIL=$ASTFAIL (mode-3 FAIL=$FAIL3, informational)"; exit 1
fi
echo "✅ GATE OK: m3 PASS=$PASS3 FAIL=$FAIL3 · m4 PASS=$PASS4 FAIL=$FAIL4 SKIP=$SKIP4 · ast PASS=$astp FAIL=$ASTFAIL · MISSING=0"
exit 0
