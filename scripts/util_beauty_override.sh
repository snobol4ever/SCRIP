#!/usr/bin/env bash
# util_beauty_override.sh — ⭐ THE CLASS-A REPRODUCER (s183, HQ Fable): ablate beauty by overriding ONE grammar
# line, rather than building a standalone witness up from ingredients.
#
#   bash scripts/util_beauty_override.sh <Parse|Command|Stmt|Label|...> "<replacement text>" [stdin-file]
#
# ⛔ WHY THIS EXISTS AND WHY YOU SHOULD REACH FOR IT FIRST.  HQ tried NINE times to build a standalone witness
# for beauty's class-A SEGV out of its identified ingredients (an EVAL-built runtime-composed pattern reached
# through one and then two defer levels, followed by a variable operand, with and without FENCE).  EVERY ONE
# AGREED WITH THE ORACLE.  beauty's context — 16 includes, the ALPHABET block, the polluted name space — is
# LOAD-BEARING for this crash, exactly as the s178 fwctx conjunction was.  Reducing beauty DOWNWARD works where
# building UP does not, so this script is the cheap path: one line changed, one run, one verdict.
#
# It copies the beauty directory to a scratch dir (so the .inc files resolve), rewrites the named grammar
# assignment INCLUDING its `+` continuation lines, runs SCRIP and the oracle on the same input, and prints both.
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 PORTABLE-HOME
S4A="${S4E_ASSETS:-$([ -d "$S4E/x64" ] && echo "$S4E" || echo /home/claude)}"   # D-17b: ASSET root -- oracles/vendor trees live at the HQ root on this machine (Lon: seats carry ONLY .github/SCRIP/corpus); a root owning its own x64 (HQ, or a full standalone clone-set) is self-contained.
set -uo pipefail
NAME="${1:?usage: util_beauty_override.sh <GrammarName> \"<replacement>\" [stdin-file]}"
REPL="${2:?replacement text required}"
BDIR="$S4E/corpus/programs/snobol4/demo/beauty"
IN="${3:-}"
SBL="$S4A/x64/bin/sbl"
[ -x "$SBL" ] || { echo "⛔ ORACLE ABSENT ($SBL). D-17b: seats do not clone x64 -- point S4E_ASSETS at a root that has it." >&2; exit 2; }
W="$(mktemp -d)"; trap 'rm -rf "$W"' EXIT
cp "$BDIR"/* "$W"/ 2>/dev/null
if [ -z "$IN" ]; then printf '\n' > "$W/ovr.in"; IN="$W/ovr.in"; fi
NAME="$NAME" REPL="$REPL" python3 - "$W" <<'PY'
import os,sys,re
w=sys.argv[1]; name=os.environ['NAME']; repl=os.environ['REPL']
L=open(os.path.join(w,'beauty.sno'),errors='replace').read().split('\n')
out=[]; i=0; hit=0
pat=re.compile(r'^\s*'+re.escape(name)+r'\s*=')
while i < len(L):
    if not hit and pat.match(L[i]):
        out.append('                  %-14s =  %s' % (name, repl)); hit=1; i+=1
        while i < len(L) and L[i].startswith('+'): i+=1          # swallow continuation lines
        continue
    out.append(L[i]); i+=1
if not hit: sys.stderr.write("⛔ grammar name %r not found in beauty.sno — nothing overridden\n" % name); sys.exit(3)
open(os.path.join(w,'ovr.sno'),'w').write('\n'.join(out))
PY
[ $? -eq 0 ] || exit 3
( cd "$W" && timeout 60 "$SBL" -bf ovr.sno < "$IN" > oracle.out 2>/dev/null )   # ⛔ -bf: beauty needs it
( cd "$W" && timeout 60 "$S4E/SCRIP/scrip" ovr.sno < "$IN" > scrip.out 2>/dev/null ); rc=$?
case $rc in 139) v="SEGV";; 124) v="HANG";; 0) if cmp -s "$W/scrip.out" "$W/oracle.out"; then v="AGREE"; else v="DIFF"; fi;; *) v="rc$rc";; esac
printf "%-10s = %-46s -> %-6s rc=%-4s scrip=%-18s oracle=%s\n" "$NAME" "$REPL" "$v" "$rc" \
    "$(tr '\n' '/' < "$W/scrip.out" | head -c 18)" "$(tr '\n' '/' < "$W/oracle.out" | head -c 22)"
