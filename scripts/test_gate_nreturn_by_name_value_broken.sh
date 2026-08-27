#!/usr/bin/env bash
# test_gate_nreturn_by_name_value_broken.sh -- computed DONE-WHEN for nreturn-by-name-value-broken.
# RE-POINTED 2026-08-27 (seat02 session 2) per hq_C's own recommendation on this row: the original two
# witnesses ($mk()='stored' and n07_nreturn.sno) are both malformed or misread relative to the SPITBOL
# oracle (see hq_C's FINDING/commit 22069a1c) -- this gate checks the REAL, oracle-grounded gap instead:
# Snocone `<call>() = value` (no `$`) must deliver the SAME result the classic `DEFINE`+`NRETURN` idiom
# already delivers (`STORE() = 43`, SPITBOL manual p.133), in both integer and string form, without
# regressing the classic witness, the SNOBOL4 gate, or the Icon/Raku/Prolog smokes.
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME
set -u
SCRIP_DIR="$S4E/SCRIP"
SCRIP="${SCRIP:-$SCRIP_DIR/scrip}"
TMP="$(mktemp -d)"; trap 'rm -rf "$TMP"' EXIT

fail() { echo "⛔ NOT DONE: $*" >&2; exit 1; }

[ -x "$SCRIP" ] || fail "scrip not built at $SCRIP -- run 'make pristine' in SCRIP/ first"
cd "$SCRIP_DIR" || fail "cannot cd to $SCRIP_DIR"

cat > "$TMP/mk_int.sc" <<'EOF'
function mkname() { mkname = .dummy; nreturn; }
mkname() = 43;
OUTPUT = dummy;
EOF
cat > "$TMP/mk_str.sc" <<'EOF'
function mkname() { mkname = .dummy; nreturn; }
mkname() = 'stored';
OUTPUT = dummy;
EOF
cat > "$TMP/classic43.sno" <<'EOF'
        DEFINE('store()dummy')                    :(main)
store   store = .dummy                            :(NRETURN)
main    store() = 43
        OUTPUT = dummy
END
EOF

check_one() {
    local src="$1" expect="$2" label="$3"
    "$SCRIP" --run "$src" < /dev/null > "$TMP/o3" 2>"$TMP/e3"; local rc=$?
    { [ "$rc" -eq 0 ] && [ "$(cat "$TMP/o3")" = "$expect" ]; } || fail "$label m3: rc=$rc out=$(cat "$TMP/o3") err=$(cat "$TMP/e3")"
    "$SCRIP" --compile "$src" > "$TMP/c.s" 2>"$TMP/cerr" || fail "$label m4 compile: $(cat "$TMP/cerr")"
    gcc -c "$TMP/c.s" -o "$TMP/c.o" 2>"$TMP/aerr" || fail "$label m4 assemble: $(cat "$TMP/aerr")"
    gcc "$TMP/c.o" -L"$SCRIP_DIR/out" -lscrip_rt -lm -Wl,-rpath,"$SCRIP_DIR/out" -o "$TMP/c.bin" 2>"$TMP/lerr" || fail "$label m4 link: $(cat "$TMP/lerr")"
    "$TMP/c.bin" < /dev/null > "$TMP/o4" 2>"$TMP/e4"; rc=$?
    { [ "$rc" -eq 0 ] && [ "$(cat "$TMP/o4")" = "$expect" ]; } || fail "$label m4: rc=$rc out=$(cat "$TMP/o4") err=$(cat "$TMP/e4")"
}

# --- the real gap: Snocone call-LHS NRETURN-by-name assignment, both an int and a string payload ---
check_one "$TMP/mk_int.sc" "43" "Snocone mkname()=43"
check_one "$TMP/mk_str.sc" "stored" "Snocone mkname()='stored'"

# --- non-regression: the classic oracle-verified idiom this Snocone form mirrors ---
check_one "$TMP/classic43.sno" "43" "classic STORE()=43"

# --- SNOBOL4 corpus gate: FAIL=0 SKIP=0 both modes, read fresh (CEO-s272 law: the shape, not a number) ---
bash scripts/test_corpus_snobol4.sh > "$TMP/gate.out" 2>&1
grep -qE '✅ GATE OK: m3 PASS=[0-9]+ FAIL=0 .* m4 PASS=[0-9]+ FAIL=0 SKIP=0 .* MISSING=0' "$TMP/gate.out" || fail "SNOBOL4 gate not clean: $(tail -8 "$TMP/gate.out")"

# --- Icon/Raku smokes: zero FAIL both modes (their own scripts already enforce HARD zero-FAIL) ---
bash scripts/test_smoke_icon.sh > "$TMP/icon.out" 2>&1 || fail "Icon smoke not clean: $(tail -8 "$TMP/icon.out")"
bash scripts/test_smoke_raku.sh > "$TMP/raku.out" 2>&1 || fail "Raku smoke not clean: $(tail -8 "$TMP/raku.out")"

# --- Prolog smoke: tolerate ONLY the pre-existing 'clause' failure (rc=1 with exactly that FAIL) ---
bash scripts/test_smoke_prolog.sh > "$TMP/prolog.out" 2>&1
prc=$?
if [ "$prc" -ne 0 ]; then
    fails="$(grep -oE '\[m[234] FAIL *\] [a-zA-Z0-9_]+' "$TMP/prolog.out" | awk '{print $NF}' | sort -u)"
    [ "$fails" = "clause" ] || fail "Prolog smoke has a NEW failure beyond the known pre-existing 'clause': [$fails]"
fi

echo "✅ nreturn-by-name-value-broken DONE-WHEN: Snocone NRETURN-by-name assignment matches the classic oracle-verified idiom, no regressions"
exit 0
