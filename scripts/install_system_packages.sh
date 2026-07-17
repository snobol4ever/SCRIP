#!/usr/bin/env bash
# build_packages.sh — install system packages needed for snobol4ever development
# Idempotent. Safe to run multiple times.
# Usage: bash build/build_packages.sh
set -euo pipefail
PKGS="build-essential libgmp-dev m4 nasm wabt bison flex"
# NOTE: libgc-dev is GONE — GC-U-4 (s67) deleted Boehm from the tree/link; the -lgc script residue was
# swept with this line's stale core.h claim. The ONE unified GC (gc_heap.c) needs no external library.
MISSING=""
for p in $PKGS; do
    dpkg -s "$p" &>/dev/null || MISSING="$MISSING $p"
done
if [ -n "$MISSING" ]; then
    apt-get install -y $MISSING 2>&1 | tail -3
    echo "OK  installed:$MISSING"
else
    echo "SKIP all packages already installed"
fi
