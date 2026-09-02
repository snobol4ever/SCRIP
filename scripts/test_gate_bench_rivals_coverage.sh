#!/bin/bash
# test_gate_bench_rivals_coverage.sh <pascal|raku|prolog> -- rivals-campaign coverage gate.
# SHARED between bench-rivals-raku-pascal and bench-rivals-prolog (one authority, never two copies --
# whichever row landed first builds it, per both task files' LINKS).
#
# Spec (bench-rivals-raku-pascal.task.md ## NEXT): counts the live kernels under corpus/benchmarks/<lang>/
# and REFUSES (rc=2) unless every one of them is covered -- either MEASURED (an AGREE verdict in a
# committed triangulation-*.tsv, bench_triangulate_snobol4.sh's schema) or DECLARED (a named exclusion in
# EXCLUDED.tsv, e.g. a .input-driven kernel the wall-clock angle cannot bracket). Three-bucket shape:
# measured / declared / nothing-falls-through -- a kernel that is neither is a REFUSAL, never a silent skip.
# This gate only READS committed results data; it never runs a benchmark itself (that is the triangulator's
# job, one per language, e.g. bench_triangulate_snobol4.sh -- not yet built for pascal/raku/prolog).
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME self-derive
CORPUS="${CORPUS:-$S4E/corpus}"

lang="${1:-}"
case "$lang" in
    pascal) ext=pas;  rival="Free Pascal (fpc)" ;;
    raku)   ext=raku; rival="Rakudo" ;;
    prolog) ext=pl;   rival="SWI-Prolog / GNU Prolog" ;;
    *) echo "usage: $0 <pascal|raku|prolog>" >&2; exit 2 ;;
esac

DIR="$CORPUS/benchmarks/$lang"
EXCLUDED="$DIR/EXCLUDED.tsv"

if [ ! -d "$DIR" ]; then
    echo "⛔ UNPROVEN: $DIR does not exist -- corpus not imported yet, not a clean pass" >&2
    exit 2
fi

# Identify kernels by basename (matches the flat pascal/raku layout and bench_triangulate_snobol4.sh's own
# TSV `kernel` column convention). Recursive find, not a top-level glob -- but two subdir NAMES are always
# PRUNED regardless of language, per corpus/benchmarks/prolog/README.md's own documented convention:
#   src/    -- "pristine ... reference only -- never run by the SCRIP gate" (verbatim, that README).
#   vanroy/ -- NOT a second kernel set: SCRIP/README.md:958 calls it "loop drivers", and
#              bench_prolog_vanroy.sh's own header confirms it is a REGENERATED, checked-in DERIVED
#              ARTIFACT of bench/'s kernels (a loop-wrapped timing form of the same program), exactly
#              analogous to a bench/<name>.s sitting beside bench/<name>.pl -- never counted as its own
#              correctness unit. Counting src/+vanroy/ alongside bench/ is what produced the "102 files,
#              28 collisions" false denominator (bench-rivals-prolog task LEDGER, seat15 2026-08-27) --
#              it was counting the SAME kernel 2-4 times under different names, not finding new kernels.
# ⛔ THE HARNESS'S OWN INFRASTRUCTURE IS NOT A KERNEL (seat12 measured 2026-09-01, hq_B cured: the two-number basis put
# prelude_swipl.pl / prelude_gplc.pl / epilogue_gplc.pl at the language root on 08-30 -- the per-engine self-timing hooks and the
# gplc halt epilogue, the very files that MAKE the basis possible -- and this enumeration counted them as three uncovered kernels:
# total=26 missing=4 where the truth was total=23 missing=1. Same false-denominator disease the src/+vanroy/ prunes above were
# written about, one directory level up: a prelude has no .expected, no work to time, and cannot ever be MEASURED or honestly
# DECLARED. Pruned by NAME PREFIX, the one shape that survives a new engine's prelude being added.
raw=()
while IFS= read -r -d '' f; do
    case "$(basename "$f")" in prelude_*|epilogue_*) continue;; esac
    raw+=("$(basename "$f" ".$ext")")
done < <(find "$DIR" \( -type d \( -name src -o -name vanroy \) -prune \) -o -name "*.$ext" -print0)

# ⛔⭐ THE UNIVERSE IS THE LOOP-DRIVER SET WHEN A LANGUAGE KEEPS ONE (hq_P 2026-09-02, cured on a red this gate had carried since
# 2026-09-01). corpus/benchmarks/prolog/EXCLUDED.tsv, bench_prolog_vanroy.sh --two-number and hq_B's ruling of 2026-09-01 all say the
# Prolog universe is the 21 vanroy/ kernels (names from vanroy/, source text from bench/); this gate counted every bench/*.pl -- 23 --
# and REFUSED on queensn (a broken bench/-only file the legacy path auto-SKIPs) and witness_depth_nrev8 (a probe witness), the two
# files that are in bench/ and not in the universe. seat12 had correctly removed queensn from EXCLUDED.tsv as "a declaration against a
# kernel this file's universe does not contain" -- and the gate went red for exactly that reason, so the two authorities disagreed on the
# denominator. ONE universe: when $DIR/vanroy/*.$ext exists, its basenames ARE the kernel set; a vanroy name with no bench/ source is an
# orphan driver and REFUSES (a denominator naming a kernel nobody can run is a lie); bench/-only files print as INFO, never as missing
# and never as covered. Languages without a driver dir (pascal, raku) enumerate exactly as before.
shopt -s nullglob; drivers=("$DIR"/vanroy/*."$ext"); shopt -u nullglob
if [ ${#drivers[@]} -gt 0 ]; then
    universe=(); orphans=()
    for f in "${drivers[@]}"; do k="$(basename "$f" ".$ext")"; universe+=("$k"); [ -f "$DIR/bench/$k.$ext" ] || orphans+=("$k"); done
    if [ ${#orphans[@]} -gt 0 ]; then
        echo "⛔ REFUSED: $lang loop drivers with no bench/ source (orphan denominator entries): ${orphans[*]}" >&2; exit 2; fi
    extra=(); for k in "${raw[@]}"; do case " ${universe[*]} " in *" $k "*) ;; *) extra+=("$k");; esac; done
    [ ${#extra[@]} -gt 0 ] && echo "INFO: $lang bench/-only files outside the ${#universe[@]}-kernel universe (not graded, not covered): ${extra[*]}"
    mapfile -t kernels < <(printf '%s\n' "${universe[@]}" | sort -u)
else
    mapfile -t kernels < <(printf '%s\n' "${raw[@]}" | sort -u)
fi
total=${#kernels[@]}

if [ "$total" -eq 0 ]; then
    echo "⛔ UNPROVEN: 0 .$ext kernels found under $DIR -- corpus-path typo, unpopulated import, or wrong extension, not a clean pass" >&2
    exit 2
fi

# ⛔ A same-named kernel in two subdirs (e.g. prolog bench/ vs vanroy/) would silently collide under
# basename identity -- refuse loudly rather than trust an ambiguous denominator (denominator honesty, per
# ARCH-BENCH-CAMPAIGN-README-TABLES.md). None observed for prolog as of this writing; guarded anyway.
raw_unique=$(printf '%s\n' "${raw[@]}" | sort -u | grep -c .)
if [ "$raw_unique" -ne "${#raw[@]}" ]; then
    echo "⛔ REFUSED: duplicate kernel basenames under $DIR across subdirectories (${#raw[@]} files, $total unique names)" >&2
    echo "   this gate identifies kernels by basename; a same-named kernel in two subdirs is ambiguous -- needs a naming convention (e.g. subdir-prefixed) before this gate can trust the denominator" >&2
    exit 2
fi

declare -A declared
if [ -f "$EXCLUDED" ]; then
    while IFS=$'\t' read -r k reason; do
        [[ "$k" == \#* || -z "$k" ]] && continue
        declared["$k"]="$reason"
    done < "$EXCLUDED"
fi

# A kernel only counts MEASURED on an AGREE verdict -- cross-prove-or-VOID means a DISAGREE row is not a
# citable measurement (ARCH doc: "Disagreement beyond tolerance = VOID-THE-RUN, publish neither number").
declare -A measured
shopt -s nullglob
for tsv in "$DIR"/triangulation-*.tsv; do
    while IFS=$'\t' read -r k engine a1 a2 ratio verdict din dout; do
        [[ "$k" == \#* || "$k" == "kernel" || -z "$k" ]] && continue
        [ "$verdict" = "AGREE" ] && measured["$k"]=1
    done < "$tsv"
done
shopt -u nullglob

covered=0; m_count=0; d_count=0; missing=()
for k in "${kernels[@]}"; do
    if [ -n "${measured[$k]:-}" ]; then
        covered=$((covered+1)); m_count=$((m_count+1))
    elif [ -n "${declared[$k]:-}" ]; then
        covered=$((covered+1)); d_count=$((d_count+1))
    else
        missing+=("$k")
    fi
done

echo "RIVALS-COVERAGE $lang vs $rival: total=$total measured=$m_count declared=$d_count covered=$covered missing=${#missing[@]}"
if [ ${#missing[@]} -gt 0 ]; then
    echo "⛔ REFUSED: $lang missing three-angle coverage for: ${missing[*]}" >&2
    echo "   (needs an AGREE row in $DIR/triangulation-*.tsv, or a named line \"kernel<TAB>reason\" in $EXCLUDED)" >&2
    exit 2
fi
exit 0
