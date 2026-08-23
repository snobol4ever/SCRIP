#!/bin/bash
# build_silly.sh — build silly-snobol4 from SCRIP/SILly
set -e
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
cd "$ROOT/SILly" && make
echo "Built: /tmp/silly-snobol4"
