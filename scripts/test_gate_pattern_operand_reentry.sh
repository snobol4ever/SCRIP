#!/usr/bin/env bash
# test_gate_pattern_operand_reentry.sh -- pattern operand temporaries must be per-ACTIVATION, not per-SITE.
# Row snobol4-pattern-operand-temporaries-are-per-site-globals-clobbered-by-reentry (hq_C mint, seat01 census,
# hq_P cure 2026-09-06).
#
# ⛔⭐ WHY THIS GATE CARRIES A MIRROR ARM AND A NEGATIVE CONTROL, AND WHY THAT IS THE WHOLE POINT.
# The tree already contained a change that CURES the row's published witness: SCRIP_PB_ARGORDER=0. It cures it
# by storing operand 0 LAST, after the re-entrant call returns -- which MOVES the window rather than closing
# it, and reverses SNOBOL4's left-to-right evaluation order on the way. A gate holding only the published
# witness (recursion in the RIGHT operand) is GREEN on that wrong fix. The MIRROR arm -- the same shape with
# the recursion in the LEFT operand -- is what rejects it. A fix that relocates a window passes every test
# aimed at the side it moved off, so the mirror is not redundancy, it is the arm with the discriminating power.
#
# ⭐ ARM 3 IS THE DISCRIMINATOR AND MUST STAY GREEN: recursion WITHOUT re-entry of the staging site was never
# broken. It is here so a future reader cannot conclude "recursion is the trigger" -- the trigger is SITE
# RE-ENTRY. If arm 3 ever goes red the cure has broken ordinary recursion, which no board would attribute here.
#
# ⭐ SELF-TEST (the arm hq_U's row taught): the gate re-runs arms 1 and 2 under SCRIP_PB_ARGORDER=1, which
# reproduces the PRE-CURE lowering in the SAME binary, and REFUSES unless that goes RED. A gate that cannot
# fail on the defect it names is a decoration; this one proves it rejects the exact lowering it was minted
# against, with no second build and no pinned artifact.
set -u
HERE="$(cd "$(dirname "$0")" && pwd)"; SD="$HERE/.."; SCRIP="$SD/scrip"
[ -x "$SCRIP" ] || { echo "⛔ REFUSE(rc=2): no scrip binary at $SCRIP -- build first (make)"; exit 2; }
"$HERE/util_require_fresh.sh" --gate test_gate_pattern_operand_reentry "$SCRIP" "${RT_DIR:-$SD/out}/libscrip_rt.so" || exit 2
W="$(mktemp -d)"; trap 'rm -rf "$W"' EXIT
# arm 1 -- re-entrant call in the RIGHT operand (the row's own shape).  arm 2 -- the MIRROR, in the LEFT.
# Both must yield the full alternation a1 | a2 | a3; a clobbered outer operand loses exactly the OUTERMOST one.
cat > "$W/right.sno" <<'EOF'
	DEFINE('F(N)')				:(F_END)
F	F  =  'a' N
	GE(N,3)					:S(RETURN)
	F  =  F  |  F(N + 1)			:(RETURN)
F_END
	DEFINE('T(NAME,SUBJ,PAT)')		:(T_END)
T	SUBJ  POS(0) *PAT RPOS(0)		:F(TF)
	OUTPUT  =  NAME ' matched'		:(RETURN)
TF	OUTPUT  =  NAME ' NO-MATCH'		:(RETURN)
T_END
	P  =  F(1)
	T('a1', 'a1', P)
	T('a2', 'a2', P)
	T('a3', 'a3', P)
END
EOF
sed 's/F  =  F  |  F(N + 1)/F  =  F(N + 1)  |  F/' "$W/right.sno" > "$W/left.sno"
# arm 3 -- the DISCRIMINATOR: recursive, but the staging site runs only at the outermost level.
cat > "$W/norentry.sno" <<'EOF'
	DEFINE('A(N)')				:(A_END)
A	NE(N,1)					:S(A_DEEP)
	A  =  'x1'
	A  =  A  |  A(2)			:(RETURN)
A_DEEP	A  =  'x2'				:(RETURN)
A_END
	DEFINE('T(NAME,SUBJ,PAT)')		:(T_END)
T	SUBJ  POS(0) *PAT RPOS(0)		:F(TF)
	OUTPUT  =  NAME ' matched'		:(RETURN)
TF	OUTPUT  =  NAME ' NO-MATCH'		:(RETURN)
T_END
	P  =  A(1)
	T('x1', 'x1', P)
	T('x2', 'x2', P)
END
EOF
WANT3="$(printf 'a1 matched\na2 matched\na3 matched')"
WANT2="$(printf 'x1 matched\nx2 matched')"
fail=0
grade() {  # $1 label  $2 file  $3 want  $4.. env
    local lab="$1" f="$2" want="$3"; shift 3
    local m3 m4 rc=0
    m3="$(cd "$W" && env "$@" timeout 20 "$SCRIP" --run "$f" </dev/null 2>&1)"
    [ "$m3" = "$want" ] || { echo "  ⛔ $lab m3 RED:"; printf '%s\n' "$m3" | sed 's/^/      /'; rc=1; }
    if (cd "$W" && env "$@" timeout 60 "$SCRIP" --compile -o "$W/p.s" "$f" </dev/null >/dev/null 2>&1) \
       && gcc -no-pie "$W/p.s" -L"$SD/out" -lscrip_rt -lm -Wl,-rpath,"$SD/out" -o "$W/p.bin" 2>/dev/null; then
        m4="$(cd "$W" && env "$@" timeout 20 "$W/p.bin" </dev/null 2>&1)"
        [ "$m4" = "$want" ] || { echo "  ⛔ $lab m4 RED:"; printf '%s\n' "$m4" | sed 's/^/      /'; rc=1; }
    else echo "  ⛔ $lab m4 REFUSED: compile/link failed"; rc=1; fi
    return $rc
}
echo "== pattern operand staging must be per-ACTIVATION (row: per-site-globals-clobbered-by-reentry)"
grade "arm1 re-entry in RIGHT operand" right.sno "$WANT3" || fail=1
grade "arm2 re-entry in LEFT operand (MIRROR)" left.sno "$WANT3" || fail=1
grade "arm3 recursion WITHOUT site re-entry (discriminator)" norentry.sno "$WANT2" || fail=1
# ---- SELF-TEST: the pre-cure lowering must FAIL arms 1+2, or this gate proves nothing.
neg=0
grade "self-test arm1 @PB_ARGORDER=1" right.sno "$WANT3" SCRIP_PB_ARGORDER=1 >/dev/null 2>&1 || neg=$((neg+1))
grade "self-test arm2 @PB_ARGORDER=0" left.sno  "$WANT3" SCRIP_PB_ARGORDER=0 >/dev/null 2>&1 || neg=$((neg+1))
if [ "$neg" -lt 2 ]; then
    echo "  ⛔ REFUSE(rc=2): SELF-TEST DID NOT FAIL ON THE PRE-CURE LOWERING ($neg/2 arms went red)."
    echo "     SCRIP_PB_ARGORDER=1 reproduces the interleaved eval/store chain and =0 the reversed one;"
    echo "     both are the defect this gate names, so both MUST be red here. A gate that cannot fail on"
    echo "     its own defect is a decoration -- fix the gate before trusting a green verdict from it."
    exit 2
fi
echo "  SELF-TEST OK: both pre-cure lowerings ($neg/2) are REJECTED by these arms"
[ "$fail" = 0 ] && { echo "OK: operand temporaries survive re-entry of their own staging site (3 arms, both modes)"; exit 0; }
echo "⛔ GATE RED: a pattern operand was clobbered by re-entry of its staging site"; exit 1
