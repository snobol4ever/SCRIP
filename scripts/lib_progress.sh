#!/usr/bin/env bash
# lib_progress.sh -- the shell face of THE PROGRESS DATABASE APPEND. ONE writer: util_progress_append.py (its
# docstring is the contract; the table is /home/resources/progress/results.tsv, the rule is progress/README.md,
# CEO-319 / CEO-331: every suite run by any session appends its per-program rows in the sitting it rewrites its
# SCORE.md cell). Source this beside lib_gate.sh; every function returns the writer's rc (0 written or
# S4E_PROGRESS_OFF, 2 refused loudly) and NEVER swallows it -- the caller decides whether a run that could not
# record is a landing verdict (it is not).
#
#   progress_append <class> <suite> <lang> <program> <mode> <outcome> [secs] [note]
#   progress_append_rows_tsv <file>                       # class suite lang program mode outcome [secs [note]] per line
#   progress_append_results_tsv <suite> <lang> <file>     # scorecard_snobol4.sh results.tsv (suite program m3 m4 t3 t4 note)
#   progress_append_triangulation <lang> <file>           # bench_triangulate_* TSV (kernel engine a1 a2 ratio verdict ...)
#   progress_context                                      # prints scrip= corpus= measurer= db= off= -- what a row would carry
#
# Control arms: S4E_PROGRESS_DB=<path> (gates point it at a scratch file), S4E_PROGRESS_OFF=1 (says so on stderr).
_PROGRESS_PY="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/util_progress_append.py"
_progress_run() {
    [ -f "$_PROGRESS_PY" ] || { echo "⛔ PROGRESS APPEND REFUSES(2): writer missing at $_PROGRESS_PY" >&2; return 2; }
    python3 "$_PROGRESS_PY" "$@"
}
progress_append() {
    [ $# -ge 6 ] || { echo "⛔ PROGRESS APPEND REFUSES(2): progress_append needs class suite lang program mode outcome [secs] [note], got $# arg(s)" >&2; return 2; }
    _progress_run append --class "$1" --suite "$2" --lang "$3" --program "$4" --mode "$5" --outcome "$6" --secs "${7:-0}" --note "${8:-}"
}
progress_append_rows_tsv() { _progress_run rows-tsv "$1"; }
progress_append_results_tsv() { _progress_run results-tsv --suite "$1" --lang "$2" "$3"; }
progress_append_triangulation() { _progress_run triangulation --lang "$1" "$2"; }
progress_context() { _progress_run context; }
