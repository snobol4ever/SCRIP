#!/usr/bin/env bash
# test_gate_ct_grow_takes_reallocs_contract_and_frees_the_old_block.sh -- ct_grow in src/ir/ct_arena.c has realloc's contract
# (ARCH-DYNAMIC-STORAGE.md § 4.0, Lon 2026-09-23: "grown in place and copy and grown just like realloc does. Of course grow in
# place only occurs at the frontier."; CEO-1211). Row compiler-every-population-table-grows-in-the-compile-time-arena-through-
# ct-grow-no-fixed-limit-a-program-can-reach (cfo).
#
# ARM 1, the primitive, through a C harness linked against the allocator's own source (no build of scrip needed):
#   1a FITS      a grow within the block's power-of-two class returns the SAME pointer.
#   1b FRONTIER  the last block carved grows IN PLACE: same pointer, payload intact, the frontier advanced.
#   1c MOVES     a block that is not at the frontier moves: a new pointer, the payload copied, and -- THE ONE BEHAVIOUR CHANGE --
#                the old block is FREED. Under SCRIP_CT_POISON=1 SCRIP_CT_NORECYCLE=1 its payload reads 0xDD and a second
#                ct_grow of it refuses loudly (rc 3). Before the change ct_grow abandoned the old block (alloc-copy-abandon), so
#                the payload still read the old bytes: a cure that stopped freeing reds this arm.
#   1d RECYCLES  without NORECYCLE, the freed block is the next same-class allocation (it went back to its bin).
#   1e BIG       a block above the 8 MB bin is its own mapping: it moves and the payload is copied.
# ARM 2, the one caller the drop broke, found by reading all 64 call sites: bb_ab_emit_nodes (src/templates/bb/bb_define.cpp)
#   snapshots g_emit and restores it per DEFINE. The snapshot carries op_arg_slot, so once drive_arg_slots_reserve grew that
#   array the restore handed back the freed block: with the carry missing, SCRIP_AB=1 on a program whose first DEFINE has 60
#   names and whose second has 20 dies rc 3 "ct_grow on a block this allocator did not hand out" (measured on an ablation tree,
#   2026-09-23); with the carry it compiles rc 0. SCRIP_AB is opt-in, which is why no corpus program reaches it.
# rc 0 GREEN · 1 RED (named) · 2 REFUSE (cannot measure).
set -u
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"; ROOT="$(cd "$HERE/.." && pwd)"
GATE_NAME=ct_grow_takes_reallocs_contract_and_frees_the_old_block
. "$HERE/lib_gate.sh" || { echo "GATE UNPROVEN(2) [$GATE_NAME]: lib_gate.sh unavailable"; exit 2; }
gate_parse_args "$@"
command -v gcc > /dev/null || { echo "GATE UNPROVEN(2) [$GATE_NAME]: no gcc"; exit 2; }
T="$(mktemp -d)" || exit 2
trap 'rm -rf "$T"' EXIT
red=0
cat > "$T/h.c" <<'EOF'
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ct_arena.h"
static int bad = 0;
#define CHECK(c, m) do { if (!(c)) { printf("  RED  %s\n", m); bad = 1; } else printf("  ok   %s\n", m); } while (0)
int main(int argc, char **argv) {
    const char *arm = argc > 1 ? argv[1] : "";
    if (!strcmp(arm, "shape")) {
        unsigned char *p = ct_alloc(100), *q, *r, *s, *t, *u; int k, ok = 1;
        for (k = 0; k < 100; k++) p[k] = (unsigned char) k;
        q = ct_grow(p, 120); CHECK(q == p, "1a FITS: a grow within the 128-byte class returns the same pointer");
        r = ct_grow(p, 200); CHECK(r == p, "1b FRONTIER: the last block carved grows in place");
        for (k = 0; k < 100; k++) if (r[k] != (unsigned char) k) ok = 0;
        CHECK(ok, "1b FRONTIER: the payload is intact after the in-place grow");
        s = ct_alloc(10); (void) s;
        t = ct_grow(r, 1000); CHECK(t != r, "1c MOVES: a block no longer at the frontier moves");
        ok = 1; for (k = 0; k < 100; k++) if (t[k] != (unsigned char) k) ok = 0;
        CHECK(ok, "1c MOVES: the payload is copied");
        u = ct_alloc(200); CHECK(u == r, "1d RECYCLES: the freed block is the next allocation of its class");
        { unsigned char *b = ct_alloc((size_t) 9 << 20), *c; b[0] = 7; b[((size_t) 9 << 20) - 1] = 9; c = ct_grow(b, (size_t) 18 << 20);
          CHECK(c != b && c[0] == 7 && c[((size_t) 9 << 20) - 1] == 9, "1e BIG: a block above the 8 MB bin moves to its own mapping, payload copied"); }
        return bad;
    }
    if (!strcmp(arm, "poison")) {
        unsigned char *p = ct_alloc(100), *s, *t; int k, dd = 1;
        memset(p, 0x41, 100); s = ct_alloc(10); (void) s;
        t = ct_grow(p, 1000);
        for (k = 0; k < 100; k++) if (p[k] != 0xDD) dd = 0;
        CHECK(t != p && t[0] == 0x41 && t[99] == 0x41, "1c MOVES: under poison the payload is copied before the old block is painted");
        CHECK(dd, "1c FREES: the old block reads 0xDD -- ct_grow dropped it (alloc-copy-abandon would still read 0x41)");
        fflush(stdout);
        ct_grow(p, 2000);
        printf("  RED  1c FREES: a second ct_grow of the dropped block returned instead of refusing\n");
        return 1;
    }
    return 2;
}
EOF
gcc -O0 -I"$ROOT/src/ir" "$T/h.c" "$ROOT/src/ir/ct_arena.c" -o "$T/h" 2> "$T/cc.err" || { echo "GATE UNPROVEN(2) [$GATE_NAME]: the harness did not compile against src/ir/ct_arena.c"; head -5 "$T/cc.err"; exit 2; }
echo "ARM 1 -- the primitive (src/ir/ct_arena.c)"
"$T/h" shape || red=1
SCRIP_CT_POISON=1 SCRIP_CT_NORECYCLE=1 "$T/h" poison > "$T/p.out" 2> "$T/p.err"; prc=$?
cat "$T/p.out"
if [ "$prc" = 3 ] && grep -q 'did not hand out' "$T/p.err"; then echo "  ok   1c FREES: a second ct_grow of the dropped block refuses loudly (rc 3)"; else echo "  RED  1c FREES: a second ct_grow of the dropped block gave rc $prc, want 3 with the refusal"; red=1; fi
grep -q '  RED ' "$T/p.out" && red=1
echo "ARM 2 -- the snapshot restore in bb_ab_emit_nodes carries the grown op_arg_slot"
gate_require_exec "$ROOT/scrip" "the scrip binary"
gate_require_fresh "$ROOT" src "$ROOT/scrip" "$ROOT/out/libscrip_rt.so"
python3 - "$T/w.sno" <<'EOF'
import sys
L = ["        DEFINE('BIG(A,B)%s')" % ",".join("L%d" % i for i in range(1, 61)),
     "        DEFINE('MID(P)%s')" % ",".join("K%d" % i for i in range(1, 21)),
     "        DEFINE('TINY(Q)')",
     "                                                   :(GO)",
     "BIG     BIG = A + B                                  :(RETURN)",
     "MID     MID = P * 3                                  :(RETURN)",
     "TINY    TINY = Q + 1                                 :(RETURN)",
     "GO      OUTPUT = BIG(3,4) MID(5) TINY(41)",
     "END"]
open(sys.argv[1], "w").write("\n".join(L) + "\n")
EOF
( cd "$T" && SCRIP_AB=1 timeout 60 "$ROOT/scrip" --compile -o w.s w.sno < /dev/null > /dev/null 2> w.err ); arc=$?
if [ "$arc" = 0 ] && [ -s "$T/w.s" ]; then echo "  ok   2 SCRIP_AB=1 compiles a 60-name DEFINE followed by a 20-name DEFINE (rc 0)"; else echo "  RED  2 SCRIP_AB=1 compile rc $arc -- $(head -c 160 "$T/w.err" | tr '\n' ' ')"; red=1; fi
gate_stamp
[ "$red" = 0 ] && { echo "GATE GREEN [$GATE_NAME]"; exit 0; }
echo "GATE RED(1) [$GATE_NAME]"; exit 1
