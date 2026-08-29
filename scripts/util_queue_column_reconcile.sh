#!/usr/bin/env bash
# util_queue_column_reconcile.sh — reconcile QUEUE.tsv's STATE column (col4) against claims/<topic>.claim,
# which is the authority. Audit by default; --fix repairs the one direction that is safe to repair.
#
# ⭐ WHY THIS EXISTS (hq_B 2026-08-29, on hq_P's queue-wide audit; row queue-column-unwritten-by-acquiring-verbs).
# s4e_msg.sh's acquiring verbs (claim / next / assign) wrote the claim file and NOT the state column, while the
# releasing verbs (park / done / unclaim) wrote the column — so the column learned about endings and never about
# beginnings. Measured before the cure: of 16 rows holding a live claim, 15 read FREE. The script fix stops NEW
# drift; this reconciles the rows that drifted before it landed.
#
# ⛔⭐ THE TWO DIRECTIONS ARE NOT THE SAME SEVERITY, AND CONFLATING THEM IS THE MISTAKE THIS SCRIPT EXISTS TO STOP.
#   A: live claim, column reads FREE      -> ILLEGIBLE, correctly dispatched. The picker skips any row with a claim
#      file regardless of the column, so nothing is double-served. Cosmetic-but-load-bearing: a human reading the
#      queue sees free work that is not free. SAFE TO REPAIR — mirroring the claim changes no dispatch outcome.
#   B: column names an owner, NO claim file -> UNSERVABLE. PASS 3 serves only FREE|'' so the column hides it, and
#      PASSES 1/2 iterate claim FILES so nothing resumes it. Real work with a baton that no `next` will ever hand
#      out, in any seat. ⛔ NOT auto-repaired: returning it to FREE is a DISPATCH act on someone else's row, and
#      the column is the only surviving record of who was meant to have it. It is REPORTED, loudly, to its owner.
set -u
PO="${S4E_POST:-/home/resources/postoffice}"; Q="$PO/QUEUE.tsv"; FIX=0
[ "${1:-}" = "--fix" ] && FIX=1
[ -f "$Q" ] || { echo "⛔ no QUEUE.tsv at $Q" >&2; exit 2; }
a_rows=""; a_n=0; b_n=0
while IFS=$'\t' read -r rank topic owner state rest; do
  case "${rank:-}" in ''|\#*) continue;; esac
  [ -n "${topic:-}" ] || continue
  c="$PO/claims/$topic.claim"
  if [ -f "$c" ] && ! grep -q '^DONE$' "$c"; then
    case "${state:-}" in
      FREE|'') held="$(head -1 "$c")"
        if grep -q '^ASSIGNED-BY ' "$c"; then want="ASSIGNED:$held"; else want="CLAIMED:$held"; fi
        a_n=$((a_n+1)); a_rows="$a_rows$topic	$want
"
        printf '  A rank %-3s %-62s FREE -> %s\n' "$rank" "$topic" "$want";;
    esac
  elif [ ! -f "$c" ]; then
    case "${state:-}" in
      ASSIGNED*|LOCKED*|RUNNING*|CLAIMED*)
        b_n=$((b_n+1))
        printf '  ⛔ B rank %-3s %-62s col=%-16s owner-col=%s  NO CLAIM FILE — UNSERVABLE\n' "$rank" "$topic" "$state" "${owner:-<empty>}";;
    esac
  fi
done < "$Q"
printf '\nQUEUE COLUMN RECONCILE: A(illegible, repairable)=%s   B(unservable, owner must decide)=%s\n' "$a_n" "$b_n"
if [ "$b_n" -gt 0 ]; then
  printf '⛔ THE B ROWS ARE NOT COSMETIC: no seat can be served them. Their owner must `park <topic> FREE` to\n'
  printf '   return them to the picker, or `assign <seat> <topic>` to lock them properly. Not done here — a\n'
  printf '   dispatch act on another HQ'"'"'s row is theirs to make, and this column is the last record of intent.\n'
fi
[ "$FIX" = 1 ] || { printf '\n(audit only — pass --fix to repair the A rows)\n'; exit 0; }
[ "$a_n" -gt 0 ] || { echo "nothing to repair"; exit 0; }
# ⛔ SAME LOCK AS mint AND s4e_set_row_state. QUEUE.tsv is one file and up to 16 seats claim into it at every
# prompt; a read-all/write-back without this is a torn queue. One lock, ONE awk pass over all A rows — not 15
# separate rewrites, each of which would be its own race window.
lock="$PO/.mint.lock"; got=0
for _i in $(seq 1 50); do mkdir "$lock" 2>/dev/null && { got=1; break; }; sleep 0.1; done
[ "$got" = 1 ] || { echo "⛔ could not acquire $lock after 5s — another mint/claim is stuck. Investigate; do not rm -rf blindly." >&2; exit 3; }
trap 'rmdir "$lock" 2>/dev/null' EXIT
map="$(mktemp)"; printf '%s' "$a_rows" > "$map"
tmp="$(mktemp)"
awk -F'\t' -v OFS='\t' -v m="$map" 'BEGIN{while((getline l < m)>0){split(l,p,"\t"); if(p[1]!="") w[p[1]]=p[2]}}
  /^[0-9]+\t/ && NF>3 && ($2 in w) {$4=w[$2]} {print}' "$Q" > "$tmp" && cat "$tmp" > "$Q"
rc=$?; rm -f "$tmp" "$map"
[ "$rc" = 0 ] && printf '✅ repaired %s A row(s) — the column now mirrors the claim.\n' "$a_n" || { echo "⛔ rewrite failed"; exit 1; }
