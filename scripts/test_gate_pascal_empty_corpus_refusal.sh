#!/bin/bash
# test_gate_pascal_empty_corpus_refusal.sh -- DONE-WHEN for pascal-m3-empty-corpus-false-pass /
# pascal-m4-empty-corpus-false-pass. Those rows' own literal DONE-WHEN ("bash
# test_gate_pascal_m3.sh"/"_m4.sh") cannot close on this tree: Pascal has ~42-52 real,
# pre-existing, separately-tracked failures (pascal-restore-prezeta's territory), so the plain
# gate exits non-zero regardless of whether THIS specific bug is fixed. This script isolates the
# actual regression the two rows are about: an empty/mistyped CORPUS path must REFUSE (non-zero
# exit), never silently print PASS=0 FAIL=0 and exit 0 -- independent of whatever the real corpus
# currently contains.
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
EMPTY="$(mktemp -d)"
trap 'rm -rf "$EMPTY"' EXIT

fail=0
for mode in m3 m4; do
    out="$(CORPUS="$EMPTY" bash "$HERE/test_gate_pascal_$mode.sh" 2>&1)"
    rc=$?
    if [ $rc -eq 0 ]; then
        echo "⛔ REGRESSION: test_gate_pascal_$mode.sh exited 0 on an EMPTY corpus (should refuse non-zero)" >&2
        echo "$out" >&2
        fail=1
    fi
done

if [ $fail -eq 0 ]; then
    echo "✅ GATE OK: both test_gate_pascal_m3.sh and test_gate_pascal_m4.sh correctly refuse (non-zero exit) on an empty CORPUS path"
fi
exit $fail
