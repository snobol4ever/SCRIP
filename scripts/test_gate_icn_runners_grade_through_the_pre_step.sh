#!/usr/bin/env bash
"$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/util_require_fresh.sh" --gate "$(basename "${BASH_SOURCE[0]}" .sh)" || exit $?
# test_gate_icn_runners_grade_through_the_pre_step.sh -- EVERY ICON RUNNER HANDS SCRIP THE PRE-STEP'S GENERATED TEXT, NEVER THE SHIPPED
# SOURCE (RULES.md FACT RULE SCRIP DOES NOT PREPROCESS, Lon 2026-09-30, CEO-1366; the coo's row runners-run-the-pre-step-...; the tool is
# the ceo's out/scrip-ipp, 2db37dc2e, proven against icont -E by test_gate_icn_pre_step_tool_matches_icont_e.sh).
#
# FOUR ARMS, hermetic (a scratch package under mktemp, never corpus/):
#  (a) THE CENSUS: the arizona, jcon, IPL and bench runners and util_parser_grid.sh source lib_icon_pre_step.sh and build their twin;
#      corpus_suite_harness.py pre-steps every Icon entry (icon_pre_step in run_suite_entry); no runner or harness line hands scrip -E
#      anything. A runner that stops sourcing the library is read here, not on a board a day later.
#  (b) THE TWIN: icon_twin_tree over a scratch package -- a program that $includes a file and $defines a constant, a library in the
#      same directory that $defines one too -- generates text with no directive left in it, carries the program's own line numbering
#      in a #line marker (SCRIP's lexer honours it), leaves the scratch package byte-identical, and SCRIP compiles the twin program
#      (linking the twin library) and runs it in the SHIPPED directory to the right answer -- including a line it reads from lib.icn AS
#      DATA, which must be the shipped text (arizona io_lib_driver reads filetext("io_lib.icn")).
#  (c) THE REFUSAL IS NAMED: a program carrying a malformed directive is not in the twin, and icon_twin_refused names it with the
#      tool's own message -- never dropped, never handed to scrip raw.
#  (d) THE HARNESS: icon_pre_step() replaces a scratch entry with its generated text and returns None; on the malformed one it
#      returns the PRE-STEP REFUSED text that run_suite_entry records as the entry's verdict in every mode.
# FAIL_ONCE=1 plants a runner that no longer sources the library and requires arm (a) to red.
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"; G="$(basename "${BASH_SOURCE[0]}" .sh)"
export S4E_HOME="${S4E_HOME:-$(cd "$ROOT/.." && pwd)}"
SCRIP="$ROOT/scrip"; [ -x "$SCRIP" ] || { echo "⛔ GATE REFUSE(2) [$G]: no scrip at $SCRIP -- make"; exit 2; }
. "$HERE/lib_icon_pre_step.sh" || { echo "⛔ GATE REFUSE(2) [$G]: lib_icon_pre_step.sh unloadable"; exit 2; }
icon_pre_step_tool > /dev/null || { echo "⛔ GATE REFUSE(2) [$G]: no pre-step tool -- make builds out/scrip-ipp"; exit 2; }
T=$(mktemp -d) || exit 2; trap 'rm -rf "$T"' EXIT
fails=0; checks=0
ck() { checks=$((checks + 1)); if [ "$1" = ok ]; then echo "  ok    $2"; else fails=$((fails + 1)); echo "  FAIL  $2"; fi; }

RUNNERS="test_icon_arizona_suite.sh test_icon_jcon_suite.sh test_icon_ipl_suite.sh test_icon_bench_suite.sh util_parser_grid.sh"
SRCDIR="$HERE"
if [ "${FAIL_ONCE:-0}" = 1 ]; then
    SRCDIR="$T/plant"; mkdir -p "$SRCDIR"; for r in $RUNNERS corpus_suite_harness.py; do cp "$HERE/$r" "$SRCDIR/$r"; done
    sed -i 's/lib_icon_pre_step\.sh/lib_icon_pre_step_GONE.sh/g' "$SRCDIR/test_icon_jcon_suite.sh"
fi
miss=""
for r in $RUNNERS; do
    grep -q '^\. "\$HERE/lib_icon_pre_step\.sh"' "$SRCDIR/$r" || { miss="$miss $r(no-source)"; continue; }
    grep -qE 'icon_twin_(tree|list) ' "$SRCDIR/$r" || miss="$miss $r(no-twin)"
done
grep -q 'refused = icon_pre_step(paths, cand)' "$SRCDIR/corpus_suite_harness.py" || miss="$miss corpus_suite_harness.py(no-pre-step)"
e_lines="$(cd "$SRCDIR" && grep -nE '"\$SCRIP"[^|;]* -E( |$)' $RUNNERS corpus_suite_harness.py 2>/dev/null | head -3)"
[ -n "$e_lines" ] && miss="$miss scrip-E:[$e_lines]"
[ -z "$miss" ] && ck ok "(a) the four Icon runners and the grid source lib_icon_pre_step.sh and build their twin; the harness pre-steps every Icon entry; nothing hands scrip -E a file" \
               || ck no "(a) a runner grades around the pre-step:$miss"

P="$T/pkg"; mkdir -p "$P/sub"
cat > "$P/sub/consts.icn" <<'EOF'
$define GREETING "hello"
EOF
cat > "$P/sub/lib.icn" <<'EOF'
$define TWICE 2
procedure twice(n);
    return n * TWICE;
end
EOF
cat > "$P/sub/prog.icn" <<'EOF'
$include "consts.icn"
$define THREE 3
link lib;
procedure main();
    write(GREETING, " ", twice(THREE));
    write(read(open("lib.icn")));
end
EOF
cat > "$P/sub/bad.icn" <<'EOF'
$endif
procedure main();
    write("never");
end
EOF
sum0="$(cd "$P" && find . -type f -exec md5sum {} + | sort)"
icon_twin_tree "$P" "$T/twin" 2> "$T/twin.log"; trc=$?
tw="$T/twin/sub/prog.icn"
if [ "$trc" -eq 0 ] && [ -s "$tw" ] && ! grep -qE '^[[:space:]]*\$(define|include|ifdef|ifndef|undef|endif|else)' "$tw" "$T/twin/sub/lib.icn" \
   && grep -q '^#line [0-9]* "prog.icn"' "$tw" && [ "$(cd "$P" && find . -type f -exec md5sum {} + | sort)" = "$sum0" ]; then
    want="$(printf 'hello 6\n$define TWICE 2')"
    out="$(cd "$P/sub" && IPATH="$T/twin/sub" timeout 20 "$SCRIP" "$T/twin/sub/prog.icn" < /dev/null 2>&1)"; orc=$?
    [ "$orc" -eq 0 ] && [ "$out" = "$want" ] && ck ok "(b) the twin carries no directive and its #line numbering, leaves the package untouched; scrip compiles it (twin library linked) and runs it in the SHIPPED directory, where lib.icn read as data is the shipped text" \
        || ck no "(b) scrip on the twin, run in the shipped directory, printed '$(printf '%s' "$out" | head -3 | tr '\n' ' ')' rc=$orc, not 'hello 6' then the shipped first line of lib.icn"
else
    ck no "(b) the twin is wrong (rc=$trc): $(head -2 "$T/twin.log" | tr '\n' ' ')"
fi
if [ ! -e "$T/twin/sub/bad.icn" ] && m="$(icon_twin_refused "$T/twin" sub/bad.icn)" && printf '%s' "$m" | grep -q 'PRE-STEP REFUSED:.*endif'; then
    ck ok "(c) the malformed program is left out of the twin and named: $(printf '%s' "$m" | cut -c1-90)"
else
    ck no "(c) the malformed program was not named as refused (in twin: $([ -e "$T/twin/sub/bad.icn" ] && echo yes || echo no); message: '${m:-}')"
fi

H="$T/h"; mkdir -p "$H"; cp "$P/sub/prog.icn" "$P/sub/consts.icn" "$P/sub/bad.icn" "$H/"
hr="$(cd "$HERE" && python3 - "$H" <<'PY'
import sys
from pathlib import Path
import corpus_suite_harness as h
paths = h.resolve_paths()
d = Path(sys.argv[1])
a = h.icon_pre_step(paths, d / "prog.icn")
b = h.icon_pre_step(paths, d / "bad.icn")
text = (d / "prog.icn").read_text()
print("A=%s B=%s DIRECTIVE_LEFT=%s" % (a is None, (b or "")[:40].replace(" ", "_"), "$define" in text or "$include" in text))
PY
)"
case "$hr" in "A=True B=PRE-STEP_REFUSED"*"DIRECTIVE_LEFT=False") ck ok "(d) the harness replaces an entry with its generated text and names a refusal: $hr" ;;
    *) ck no "(d) the harness pre-step read '$hr'" ;; esac

echo "population: $checks arm(s); $(echo $RUNNERS | wc -w) runners + the harness censused; a scratch package of 4 files (a program, an include, a library, a malformed program)"
[ "$fails" -eq 0 ] && { echo "GATE PASS(0) [$G]: every Icon runner grades through SCRIP's own pre-step ($checks arms)"; exit 0; }
echo "⛔ GATE FAIL(1) [$G]: $fails of $checks arm(s) red"; exit 1
