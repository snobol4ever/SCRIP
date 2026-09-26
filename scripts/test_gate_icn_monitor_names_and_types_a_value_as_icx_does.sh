#!/usr/bin/env bash
# test_gate_icn_monitor_names_and_types_a_value_as_icx_does.sh -- A VALUE EVENT NAMES THE SOURCE VARIABLE AND TYPES THE VALUE AS
# ICX DOES (hq_icon 2026-09-25, crawl work under CEO-1270 / CEO-1272 (c)).
#
# Three false divergences the monitor census over the 111 graded Arizona programs measured, each from SCRIP's plug:
#   (1) a static local went on the wire under its lowering's mangled global name (main__STATIC__y) where icx sends y;
#   (2) SCRIP's own synthetic globals (__icn_progname, the initial-block flags PROC__INITFLAG__n) produced VALUE events that no
#       source assignment makes, so icx never sends them;
#   (3) a set went on the wire as MWT_TABLE (SCRIP keeps a set as a table block with is_set) where icx sends MWT_DATA.
# core.c's rt_trace_value now shows a static by its source name and emits nothing for a synthetic global, and one typing helper
# (mon_wire_type) sends a set as DATA, as monitor_icx.c types T_Set.
#
# THE WITNESS assigns a static local and a set and then runs a statement that needs &progname: it must bracket AGREE.
# On core.c before this change it DIVERGES at the static's VALUE (red).
set -uo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
G=test_gate_icn_monitor_names_and_types_a_value_as_icx_does
[ -x "$HERE/../scrip" ] || { echo "⛔ GATE REFUSE(2) [$G]: scrip not built"; exit 2; }
S4E="${S4E_HOME:-$(cd "$HERE/../.." && pwd)}"; S4A="${S4E_ASSETS:-$([ -d "$S4E/x64" ] && echo "$S4E" || echo /home/resources)}"
IM="${ICON_MON_ROOT:-$S4A/icon-mon}"
[ -x "$IM/bin/icont" ] && [ -x "$IM/bin/iconx" ] || { echo "⛔ GATE REFUSE(2) [$G]: the instrumented Icon fork is not built at $IM -- this gate cannot measure without icx"; exit 2; }
T="$(mktemp -d "${TMPDIR:-/tmp}/valname_gate.XXXXXX")" || { echo "⛔ GATE REFUSE(2) [$G]: mktemp failed"; exit 2; }
trap 'rm -rf "$T"' EXIT
cat > "$T/valname.icn" <<'EOF'
procedure f()
   static k;
   k := 5;
   write(k)
end
procedure main()
   local s, n;
   s := set([1, 2]);
   f();
   n := *s;
   write(n, " ", *&progname > 0)
end
EOF
out="$(cd "$T" && timeout 300 bash "$HERE/monitor_run.sh" valname.icn --oracle 2>&1)"; rc=$?
line="$(printf '%s\n' "$out" | grep -aE '^\[monitor_run\] (AGREE|DIVERGE|UNGRADED)|REFUSE|^\| \*\*>' | tail -2 | tr '\n' ' ' | cut -c1-240)"
if [ "$rc" -eq 0 ] && printf '%s\n' "$out" | grep -qa '^\[monitor_run\] AGREE'; then
    echo "  $line"
    echo "✅ GATE PASS [$G]: a static, a set and &progname bracket AGREE against icx"; exit 0
fi
echo "  rc=$rc $line"
echo "⛔ GATE FAIL [$G]: the witness did not bracket AGREE -- a VALUE event is named or typed unlike icx"; exit 1
