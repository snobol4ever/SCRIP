#!/usr/bin/env bash
# util_parser_sc_census.sh -- HOW MANY PROGRAMS DOES EACH SELF-HOSTED parser_<lang>.sc ACTUALLY TRANSLATE?
# Minted 2026-09-16 by hq_snocone on Lon's question "List the number of properly translated programs
# per parser?" -- which had no instrument that could answer it, only three drivers that could not run.
# ⛔ IT PRINTS ITS DENOMINATOR. A parser that translates nothing reads 0 over a stated population with
# the first refusal NAMED, never a blank or a silent skip.
# CLASSIFICATION, deliberately strict: a program counts PARSED only when the parser emits a TREE, i.e.
# a first non-blank line starting with '('. "Parse Error" is REFUSED. Anything else -- a crash, a heap
# exhaustion, a timeout, a runtime error -- is CRASH, and its first line is shown, because a parser that
# dies is not the same fact as a parser that declines and must never be counted as one.
# ⛔ THE CHAIN IS LOADED BY CONCATENATION, NOT BY -L, AND THAT IS NOT A STYLE CHOICE: scrip collects -L
# for every language but expands it into -INCLUDE only on the SNOBOL4 path (src/driver/scrip.c:1218);
# the Snocone branch calls snocone_compile() and never reads n_preload, so a .sc program CANNOT load a
# library chain. Concatenation is the only loader that works today, and it is verified to work -- under
# it digits, nl, nPush, nTop and SPAN all resolve and behave.
# Build the chains first (rt = the 14-file bootstrap runtime, in the order run_scrip_parser.sh declares):
#   cat bootstrap/{global,case,assign,match,counter,stack,tree,ShiftReduce,tdump,gen,qize,semantic,omega,trace}.sc > rt.sc
#   for l in icon pascal prolog raku rebus snobol4 snocone; do cat rt.sc bootstrap/parser_$l.sc > chain_$l.sc; done
# Usage: N=<programs per language> bash scripts/util_parser_sc_census.sh     (chains expected beside it)
SP=/tmp/claude-1000/-home-claude-snocone/efd61ac8-4fe8-433d-91bc-35d1188e5e8c/scratchpad
S=/home/claude_snocone/SCRIP/scrip; C=/home/claude_snocone/corpus; N=${N:-10}
printf '%-9s %-4s %-7s %-8s %-7s  %s\n' lang pop PARSED REFUSED CRASH "first refused"
for spec in "icon icn" "pascal pas" "prolog pl" "raku raku" "rebus reb" "snobol4 sno" "snocone sc"; do
  set -- $spec; lang=$1; ext=$2
  mapfile -t files < <(find $C -name "*.$ext" -type f 2>/dev/null | grep -vE '/library/|/bootstrap/|ALL\.' | sort | head -$N)
  p=0; r=0; c=0; first=""
  for f in "${files[@]}"; do
    out=$(timeout 15 $S $SP/chain_$lang.sc < "$f" 2>&1 | grep -vE '^\s*$|^SEQ[0-9]' | head -1)
    case "$out" in
      \(*)            p=$((p+1)) ;;
      *"Parse Error"*) r=$((r+1)); [ -z "$first" ] && first="$(basename $f)" ;;
      *)              c=$((c+1)); [ -z "$first" ] && first="$(basename $f) [${out:0:28}]" ;;
    esac
  done
  printf '%-9s %-4s %-7s %-8s %-7s  %s\n' "$lang" "${#files[@]}" "$p" "$r" "$c" "$first"
done
