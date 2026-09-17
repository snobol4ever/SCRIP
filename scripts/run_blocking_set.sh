#!/usr/bin/env bash
# run_blocking_set.sh -- RUN EVERY ARM OF THE BLOCKING SET, THEN REPORT.  The loop-and-report driver behind
# `make test`, ruled CEO-582 (2026-09-11) on the cfo's first full census of that target.
#
# THE DEFECT IT EXISTS TO KILL, MEASURED BEFORE IT WAS BUILT: `make test` was ONE target of ~150 recipe
# lines and make ABORTS ON THE FIRST FAILING LINE, so when arm 15 went red the set died 24 seconds in and
# 131 ARMS NEVER EXECUTED -- for all 13 seats, for as long as that arm stayed red.  Not red.  Not green.
# UNMEASURED, which our own denominator law names the worst of the three states.  `make -k` does not rescue
# it (measured, 149s, same single red): -k continues across TARGETS, never across the lines of ONE recipe.
# The set could therefore only ever reveal ONE red per cure cycle, each cycle paying a build.  The cfo's
# loop-and-continue census found EIGHT non-green where the target reported ONE -- five reds and three
# refusals, one of which would have passed VACUOUSLY.  WE REPORTED 1 OF 8.
#
# ⛔ THE GUARDS, all from the ruling and all mandatory -- nothing here is weakened relative to abort-first:
#   · the exit code stays NON-ZERO whenever a blocking arm is red (rc=1) or refused (rc=2);
#   · REDS AND REFUSALS ARE PRINTED AS SEPARATE CLASSES, never summed, because rc=1 FAILED and rc=2
#     COULD-NOT-MEASURE are different facts and the second is the better hiding place;
#   · the summary SATISFIES ITS OWN DENOMINATOR -- green + red + refused == arms, printed, and the driver
#     REFUSES rc=2 if that identity does not hold, so a run that dies partway can never read as a clean one.
#
# ⭐ THE POPULATION IS DECLARED, NOT DISCOVERED, and it is declared where it always was: the recipe lines of
# the `test-sequential` target in the Makefile, with every per-arm comment they carry.  That target is the
# old abort-on-first-red form, kept RUNNABLE on purpose -- it is the declaration this driver reads AND an
# executable twin of it, so the two cannot diverge silently, and `make test-sequential` remains the way to
# bisect a set that has gone strange.  test_gate_make_test_loops_and_reports.sh pins the agreement.
#
# ⭐ WHY NOT A SEPARATE arms.txt (the preflight shape): preflight's population genuinely cannot be derived
# (its own comment says a static grep classifies it wrongly in both directions).  This one can be, and the
# arms carry ~50 lines of measured provenance comments that thirteen seats grep by gate name in the
# Makefile.  Moving them would have broken every `grep <gate> Makefile` in the org to save a parser.
#
# USAGE
#   bash scripts/run_blocking_set.sh                  # run the declared set, loop and report
#   bash scripts/run_blocking_set.sh --list           # print the arms and their class, run nothing
#   bash scripts/run_blocking_set.sh --arms-from F    # run an arm list from a file (one per line) instead
#   bash scripts/run_blocking_set.sh --target T       # read a different declaration target
#   bash scripts/run_blocking_set.sh --shard K/N      # run ONE interleaved slice of the declared set
#   bash scripts/run_blocking_set.sh --shards N       # fan out N slices as children and MERGE their report
#
# ⭐ SHARDS, AND WHAT THEY ARE ACCEPTED ON (ceo CEO-829, from the cfo's economy measurement: 338 arms, ~35
# minutes at load 1.5-2.6, against an origin/main that took 22 landings today -- and the gaps between the last
# twelve were 0,2,2,9,4,0,3,3,322,1,3,49 minutes, so THE AVERAGE HIDES THE SHAPE: landings arrive in BURSTS of
# three or four inside five minutes and a 35-minute set cannot fit inside a burst at all).
# ⛔ THE ACCEPTANCE IS NOT A SPEEDUP. It is that a sharded run reds THE SAME RED NAME SET as a sequential one on
# the same tree (CEO-825 clause 1): an arm whose verdict changes with HOW it was run is the finding, and a faster
# set that quietly greens an arm is worth less than the slow one. Speed is reported BESIDE name-set identity,
# never instead of it; test_gate_blocking_set_shards_agree_with_sequential.sh is what asserts it.
# ⛔ THE HAZARD IS REAL AND THE CENSUS THAT FOUND IT UNDER-DETECTS, so hazardous arms are DECLARED, not sniffed:
# 280 of 336 arms build a mktemp workdir, but the port-trace gates cd into the checkout ROOT (CLAUDE.md records
# their tmp3/tmp4 litter there), timing-sensitive arms read a loaded box differently, and the one-runner board
# arms must not run twice at once. scripts/blocking_set_serial_arms.txt names the patterns that RUN ALONE in the
# parent before any fan-out. A null hazard census bounds the probe, not the thing probed.
# An arm line prefixed `-` is REPORTED: it is run and classified like any other, it is printed in its class,
# and it CANNOT change the exit code -- exactly what a leading `-` already meant to make.
#
# EXIT: 0 every blocking arm green / 1 at least one blocking arm RED / 2 no blocking red but at least one
# blocking REFUSAL, or the driver itself could not measure (no arms, an unexpandable make variable, a
# denominator that does not sum).
set -u
NAME=run_blocking_set
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$HERE/.." && pwd)"
MAKEFILE="$ROOT/Makefile"
DECL_TARGET="test-sequential"
ARMS_FILE=""
LIST_ONLY=0
SHARD=""
SHARDS=""
SERIAL_FILE="$HERE/blocking_set_serial_arms.txt"
refuse() { echo "⛔ REFUSED(2) [$NAME]: $*" >&2; exit 2; }
while [ $# -gt 0 ]; do
    case "$1" in
        --list) LIST_ONLY=1 ;;
        --arms-from) shift; [ $# -gt 0 ] || refuse "--arms-from needs a file"; ARMS_FILE="$1" ;;
        --arms-from=*) ARMS_FILE="${1#*=}" ;;
        --target) shift; [ $# -gt 0 ] || refuse "--target needs a name"; DECL_TARGET="$1" ;;
        --target=*) DECL_TARGET="${1#*=}" ;;
        --makefile) shift; [ $# -gt 0 ] || refuse "--makefile needs a file"; MAKEFILE="$1" ;;
        --makefile=*) MAKEFILE="${1#*=}" ;;
        --shard) shift; [ $# -gt 0 ] || refuse "--shard needs K/N"; SHARD="$1" ;;
        --shard=*) SHARD="${1#*=}" ;;
        --shards) shift; [ $# -gt 0 ] || refuse "--shards needs a count"; SHARDS="$1" ;;
        --shards=*) SHARDS="${1#*=}" ;;
        --serial-arms) shift; [ $# -gt 0 ] || refuse "--serial-arms needs a file"; SERIAL_FILE="$1" ;;
        --serial-arms=*) SERIAL_FILE="${1#*=}" ;;
        -h|--help) sed -n '1,40p' "${BASH_SOURCE[0]}"; exit 0 ;;
        *) refuse "unrecognised argument '$1' -- a driver that treats an unknown flag as a filename is how a typo becomes a plausible answer" ;;
    esac
    shift
done
# ⛔ THE DECLARATION IS READ, NEVER GUESSED.  A recipe line is an arm; a `#`-only recipe line is prose and is
# skipped; the trailing `   # ...` comment is the arm's provenance and is stripped from the command but kept
# for nothing else -- it is a SHELL comment today (make passes recipe lines to sh verbatim), so leaving it in
# would also be harmless.  We strip it so the make-variable check below can look at the COMMAND only.
FLAGS=(); CMDS=()
read_decl() {
    local line flag cmd
    while IFS= read -r line; do
        case "$line" in
            "	#"*) continue ;;
            "	"*) line="${line#	}" ;;
            *) continue ;;
        esac
        flag=""
        case "$line" in -*) flag="-"; line="${line#-}" ;; esac
        cmd="$(printf '%s\n' "$line" | sed 's/[ 	]#.*$//; s/[ 	]*$//')"
        [ -n "$cmd" ] || continue
        FLAGS+=("$flag"); CMDS+=("$cmd")
    done < <(awk -v t="^${DECL_TARGET}:" '$0 ~ t {f=1; next} f && /^$/ {exit} f' "$MAKEFILE")
}
read_file() {
    local line flag cmd
    while IFS= read -r line; do
        case "$line" in ""|"#"*) continue ;; esac
        flag=""
        case "$line" in -*) flag="-"; line="${line#-}" ;; esac
        cmd="$(printf '%s\n' "$line" | sed 's/[ 	]#.*$//; s/[ 	]*$//')"
        [ -n "$cmd" ] || continue
        FLAGS+=("$flag"); CMDS+=("$cmd")
    done < "$1"
}
if [ -n "$ARMS_FILE" ]; then
    [ -f "$ARMS_FILE" ] || refuse "arms file '$ARMS_FILE' not found"
    read_file "$ARMS_FILE"
    SOURCE="$ARMS_FILE"
else
    [ -f "$MAKEFILE" ] || refuse "$MAKEFILE not found"
    grep -q "^${DECL_TARGET}:" "$MAKEFILE" || refuse "no '${DECL_TARGET}:' target in $MAKEFILE -- the declaration this driver reads is gone or renamed"
    read_decl
    SOURCE="$MAKEFILE:${DECL_TARGET}"
fi
N=${#CMDS[@]}
# ⛔ ZERO ARMS IS A REFUSAL, NOT A GREEN RUN.  An empty population printing the success shape is the exact
# false-green `make test` itself carried for months (named in .PHONY, no recipe, exit 0 having run nothing).
[ "$N" -gt 0 ] || refuse "extracted ZERO arms from $SOURCE -- a set that graded nothing must never print the success shape"
# ⛔ MAKE VARIABLES.  make expands these before the shell sees them and this driver is not make.  `$(MAKE)` is
# the one form the declaration uses and the one form we expand; ANY other is a refusal rather than a guess,
# because `$(FOO)` handed to bash is command substitution and would run FOO as a command.
for i in $(seq 0 $((N - 1))); do
    CMDS[$i]="${CMDS[$i]//\$(MAKE)/${MAKE:-make}}"
    case "${CMDS[$i]}" in
        *'$('*) refuse "arm $((i + 1)) carries a make variable this driver cannot expand: ${CMDS[$i]}" ;;
    esac
done
# ⛔ THE SERIAL SET IS DECLARED, NOT SNIFFED (see the header).  A pattern here matches an arm's command as a
# substring; a matching arm NEVER runs inside a shard.  A missing file is not a refusal -- it means no arm has
# been declared hazardous yet -- but an UNREADABLE one is, because "I could not read the hazard list" and "the
# hazard list is empty" are the two facts this driver must never conflate.
SERIAL_PATS=()
if [ -n "$SERIAL_FILE" ] && [ -e "$SERIAL_FILE" ]; then
    [ -r "$SERIAL_FILE" ] || refuse "serial-arms file '$SERIAL_FILE' exists but cannot be read -- an unreadable hazard list is not an empty one"
    while IFS= read -r _l; do
        case "$_l" in ""|"#"*) continue ;; esac
        SERIAL_PATS+=("$_l")
    done < "$SERIAL_FILE"
fi
is_serial_arm() {  # is_serial_arm <cmd>
    local c="$1" pat
    for pat in ${SERIAL_PATS[@]+"${SERIAL_PATS[@]}"}; do
        case "$c" in *"$pat"*) return 0 ;; esac
    done
    return 1
}
# ⛔ SHARD SELECTION IS INTERLEAVED AND PARTITIONS THE SET EXACTLY ONCE -- the same k/N convention the suite
# harness uses, so N shards' arm lists concatenate back to the declaration with nothing doubled and nothing
# dropped.  A serial arm is in NO shard: the parent runs it.
SHARD_K=0; SHARD_N=0
if [ -n "$SHARD" ]; then
    case "$SHARD" in
        [1-9]*/[1-9]*) SHARD_K="${SHARD%%/*}"; SHARD_N="${SHARD##*/}" ;;
        *) refuse "--shard takes K/N with 1 <= K <= N (got '$SHARD')" ;;
    esac
    [ "$SHARD_K" -ge 1 ] && [ "$SHARD_K" -le "$SHARD_N" ] || refuse "--shard $SHARD is outside 1..N"
    [ -n "$SHARDS" ] && refuse "--shard and --shards are different jobs: one slice, or a fan-out over all of them"
fi
if [ -n "$SHARDS" ]; then
    case "$SHARDS" in [1-9]|[1-9][0-9]) : ;; *) refuse "--shards takes a count 1..99 (got '$SHARDS')" ;; esac
fi
if [ "$SHARD_N" -gt 0 ]; then
    KEEP_F=(); KEEP_C=(); _slot=0
    for i in $(seq 0 $((N - 1))); do
        if is_serial_arm "${CMDS[$i]}"; then continue; fi
        if [ "$((_slot % SHARD_N))" -eq "$((SHARD_K - 1))" ]; then KEEP_F+=("${FLAGS[$i]}"); KEEP_C+=("${CMDS[$i]}"); fi
        _slot=$((_slot + 1))
    done
    FLAGS=(${KEEP_F[@]+"${KEEP_F[@]}"}); CMDS=(${KEEP_C[@]+"${KEEP_C[@]}"}); N=${#CMDS[@]}
    SOURCE="$SOURCE shard=$SHARD_K/$SHARD_N"
    # ⛔ A SHARD WITH ZERO ARMS IS NOT A GREEN RUN.  It means N exceeds the arm count, and printing the success
    # shape for it is the same false green an empty population always is.
    [ "$N" -gt 0 ] || refuse "shard $SHARD_K/$SHARD_N holds ZERO arms (the set has fewer shardable arms than shards) -- a slice that graded nothing must never print the success shape"
fi

if [ "$LIST_ONLY" -eq 1 ]; then
    for i in $(seq 0 $((N - 1))); do
        if [ -n "${FLAGS[$i]}" ]; then printf '%3d  REPORTED  %s\n' "$((i + 1))" "${CMDS[$i]}"
        else printf '%3d  BLOCKING  %s\n' "$((i + 1))" "${CMDS[$i]}"; fi
    done
    printf '%s: %d arm(s) declared in %s\n' "$NAME" "$N" "$SOURCE"
    exit 0
fi
# ⭐ THE FAN-OUT.  The parent runs the DECLARED SERIAL ARMS ITSELF, one at a time, then starts N children, each
# `--shard k/N` of this same script over the same declaration, and MERGES their per-arm machine lines.  The merge
# is by ARM NAME, which is what the acceptance compares: same red name set, same refused name set, same total.
if [ -n "$SHARDS" ]; then
    WORK="$(mktemp -d)" || refuse "no temp dir for the fan-out"
    trap 'rm -rf "$WORK"' EXIT
    echo "=== blocking set: $N arm(s) from $SOURCE -- FAN-OUT over $SHARDS shard(s); declared serial arms run in the parent first ==="
    printf '%s\n' "${SERIAL_PATS[@]+${SERIAL_PATS[@]}}" | sed '/^$/d; s/^/  serial (never sharded): /'
    t_all=$SECONDS
    : > "$WORK/results.tsv"
    for i in $(seq 0 $((N - 1))); do
        is_serial_arm "${CMDS[$i]}" || continue
        t0=$SECONDS
        out="$(cd "$ROOT" && bash -c "${CMDS[$i]}" </dev/null 2>&1)"; rc=$?
        printf 'ARMRESULT\t%s\t%s\t%s\t%s\n' "$rc" "$((SECONDS - t0))" "$([ -n "${FLAGS[$i]}" ] && echo R || echo B)" "${CMDS[$i]}" >> "$WORK/results.tsv"
        printf '[serial] rc=%-3s %4ds  %s\n' "$rc" "$((SECONDS - t0))" "${CMDS[$i]}"
    done
    for k in $(seq 1 "$SHARDS"); do
        ( bash "${BASH_SOURCE[0]}" --shard "$k/$SHARDS" --target "$DECL_TARGET" --makefile "$MAKEFILE" \
              ${ARMS_FILE:+--arms-from "$ARMS_FILE"} --serial-arms "$SERIAL_FILE" > "$WORK/shard.$k.out" 2>&1; echo $? > "$WORK/shard.$k.rc" ) &
    done
    wait
    for k in $(seq 1 "$SHARDS"); do
        krc="$(cat "$WORK/shard.$k.rc" 2>/dev/null || echo 2)"
        printf 'shard %d/%s: rc=%s, %s arm(s)\n' "$k" "$SHARDS" "$krc" "$(grep -c '^ARMRESULT	' "$WORK/shard.$k.out" 2>/dev/null || echo 0)"
        grep '^ARMRESULT	' "$WORK/shard.$k.out" >> "$WORK/results.tsv" 2>/dev/null
        if [ "$krc" = 2 ] && ! grep -q '^ARMRESULT	' "$WORK/shard.$k.out" 2>/dev/null; then
            echo "⛔ shard $k/$SHARDS produced NO arm results:"; tail -3 "$WORK/shard.$k.out" | sed 's/^/     /'
        fi
    done
    echo
    g=0; r=0; f=0; bred=0; bref=0
    # ⛔ THE FAN-OUT RE-EMITS EVERY MACHINE LINE.  The per-arm ARMRESULT line is this driver's CONTRACT with any
    # consumer -- it is how a verdict is read BY ARM NAME rather than by count -- and a shape of this driver that
    # printed only its merged prose would be unreadable to the very gate that grades shard-vs-sequential identity.
    while IFS="	" read -r _tag rc sec kind cmd; do
        [ "$_tag" = ARMRESULT ] || continue
        printf 'ARMRESULT\t%s\t%s\t%s\t%s\n' "$rc" "$sec" "$kind" "$cmd"
        _rep=""; [ "$kind" = R ] && _rep="   (REPORTED)"
        if [ "$rc" = 0 ]; then g=$((g + 1))
        elif [ "$rc" = 2 ]; then f=$((f + 1)); printf '  REFUSED   %s%s\n' "$cmd" "$_rep"; [ "$kind" = B ] && bref=$((bref + 1))
        else r=$((r + 1)); printf '  RED  rc=%-3s %s%s\n' "$rc" "$cmd" "$_rep"; [ "$kind" = B ] && bred=$((bred + 1)); fi
    done < "$WORK/results.tsv"
    tot=$((g + r + f))
    printf 'blocking set: arms=%d  green=%d  red=%d  refused=%d   (%d+%d+%d=%d)  shards=%s  wall=%ds\n' \
        "$N" "$g" "$r" "$f" "$g" "$r" "$f" "$tot" "$SHARDS" "$((SECONDS - t_all))"
    [ "$tot" -eq "$N" ] || refuse "the fan-out reported $tot arm(s) for a declared $N -- a shard lost or doubled work, and a set that cannot account for its own arms is not a measurement (speed is reported BESIDE name-set identity, never instead of it)"
    if [ "$bred" -gt 0 ]; then echo "⛔ BLOCKING SET RED: $bred blocking arm(s) failed, $bref refused."; exit 1; fi
    if [ "$bref" -gt 0 ]; then echo "⛔ BLOCKING SET UNPROVEN: no blocking arm failed, but $bref could not measure (rc=2).  A refusal is not a pass."; exit 2; fi
    echo "✅ BLOCKING SET GREEN: all $N arm(s) ran across $SHARDS shard(s); every blocking arm passed."
    exit 0
fi

echo "=== blocking set: $N arm(s) from $SOURCE -- EVERY arm runs; reds and refusals are reported separately at the end ==="
green=0; red=0; refused=0
RED_IDX=(); REF_IDX=(); RC=(); OUT=(); SEC=()
for i in $(seq 0 $((N - 1))); do
    cmd="${CMDS[$i]}"
    t0=$SECONDS
    out="$(cd "$ROOT" && bash -c "$cmd" </dev/null 2>&1)"; rc=$?
    dt=$((SECONDS - t0))
    RC+=("$rc"); OUT+=("$out"); SEC+=("$dt")
    # ⛔ THE THREE CLASSES ARE THE THREE EXIT CODES (lib_gate.sh): 0 examined and clean, 2 COULD NOT EXAMINE,
    # anything else examined and bad.  A refusal is never folded into red and never into green.
    if [ "$rc" -eq 0 ]; then green=$((green + 1)); cls=GREEN
    elif [ "$rc" -eq 2 ]; then refused=$((refused + 1)); cls=REFUSED; REF_IDX+=("$i")
    else red=$((red + 1)); cls=RED; RED_IDX+=("$i"); fi
    printf '[%3d/%3d] %-8s rc=%-3s %4ds  %s%s\n' "$((i + 1))" "$N" "$cls" "$rc" "$dt" "$cmd" "${FLAGS[$i]:+   (REPORTED)}"
    # ⛔ the machine line a fan-out parent merges by ARM NAME -- the identity the acceptance compares.  The
    # blocking/reported column is a WORD (B or R) and never empty: IFS=tab COLLAPSES a run of tabs, so an empty
    # field shifts every field after it one to the left, and the first version of this line read a blocking RED
    # as a REPORTED one and exited 0 over it.  A separator that eats an empty field is not a separator.
    printf 'ARMRESULT\t%s\t%s\t%s\t%s\n' "$rc" "$dt" "$([ -n "${FLAGS[$i]}" ] && echo R || echo B)" "$cmd"
done
echo
if [ "${#RED_IDX[@]}" -gt 0 ]; then
    echo "⛔ RED -- examined and FAILED (${#RED_IDX[@]}):"
    for i in "${RED_IDX[@]}"; do
        printf '  RED  rc=%-3s %s%s\n' "${RC[$i]}" "${CMDS[$i]}" "${FLAGS[$i]:+   (REPORTED -- does not fail this run)}"
        printf '%s\n' "${OUT[$i]}" | tail -5 | sed 's/^/       /'
    done
    echo
fi
if [ "${#REF_IDX[@]}" -gt 0 ]; then
    echo "⛔ REFUSED -- COULD NOT MEASURE, rc=2 (${#REF_IDX[@]}).  NOT red and NOT green: an arm in this class graded nothing:"
    for i in "${REF_IDX[@]}"; do
        printf '  REFUSED   %s%s\n' "${CMDS[$i]}" "${FLAGS[$i]:+   (REPORTED -- does not fail this run)}"
        printf '%s\n' "${OUT[$i]}" | tail -5 | sed 's/^/       /'
    done
    echo
fi
# ⛔ THE DENOMINATOR IS PRINTED AND CHECKED.  This line is the whole point of the target: a run that died
# partway used to be indistinguishable from a clean one, and now the arithmetic has to close.
printf 'blocking set: arms=%d  green=%d  red=%d  refused=%d   (%d+%d+%d=%d)\n' \
    "$N" "$green" "$red" "$refused" "$green" "$red" "$refused" "$((green + red + refused))"
[ "$((green + red + refused))" -eq "$N" ] || refuse "the classes do not sum to the arm count -- this driver cannot report its own run"
if [ -f "$HERE/lib_gate.sh" ]; then . "$HERE/lib_gate.sh" 2>/dev/null && gate_stamp 2>/dev/null; fi
# ⛔ REPORTED ARMS CANNOT CHANGE THE VERDICT, which is exactly what a leading `-` meant to make.  Everything
# else does: a blocking red exits 1, a blocking refusal with no red exits 2.
bred=0; bref=0
for i in "${RED_IDX[@]:-}"; do [ -n "${i:-}" ] && [ -z "${FLAGS[$i]}" ] && bred=$((bred + 1)); done
for i in "${REF_IDX[@]:-}"; do [ -n "${i:-}" ] && [ -z "${FLAGS[$i]}" ] && bref=$((bref + 1)); done
if [ "$bred" -gt 0 ]; then
    echo "⛔ BLOCKING SET RED: $bred blocking arm(s) failed, $bref refused, $((red - bred + refused - bref)) non-green arm(s) were REPORTED-only."
    exit 1
fi
if [ "$bref" -gt 0 ]; then
    echo "⛔ BLOCKING SET UNPROVEN: no blocking arm failed, but $bref could not measure (rc=2).  A refusal is not a pass."
    exit 2
fi
echo "✅ BLOCKING SET GREEN: all $N arm(s) ran; every blocking arm passed."
exit 0
