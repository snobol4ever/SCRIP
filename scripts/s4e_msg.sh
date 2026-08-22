#!/usr/bin/env bash
# s4e_msg.sh — the postoffice helper (/home/resources/postoffice/PROTOCOL.md is law). Usage:
#   s4e_msg.sh next                       ONE-SHOT PICK-AND-LOCK: resume your own unfinished
#                                         claim, else atomically lock the topmost free QUEUE.tsv
#                                         row and print its brief pointer + first step
#   s4e_msg.sh done <topic>               mark your claim finished (frees nothing — claims persist
#                                         as done-markers; next stops resuming it)
#   s4e_msg.sh ask <topic> "text"         question box: sends to hq as q-<topic>
#   s4e_msg.sh send <to> <topic> "text"   s4e_msg.sh check   s4e_msg.sh clear
#   s4e_msg.sh claim <topic>              s4e_msg.sh board [my new status text]
#   s4e_msg.sh banner [topic]             ⛔ MANDATORY LAST ACT OF EVERY SESSION: prints the COMPUTED
#                                         SUCCESS/FAILURE banner + whether re-firing advances anything
set -u
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 sibling root
PO="${S4E_POST:-/home/resources/postoffice}"
ME="${S4E_SEAT:-}"
if [ -z "$ME" ]; then case "$S4E" in /home/claude) ME=hq;; /home/claude[1-9]) ME="seat${S4E#/home/claude}";; *) ME="$(basename "$S4E")";; esac; fi
cmd="${1:-check}"
case "$cmd" in
  send)  to="${2:?to}"; topic="${3:?topic}"; shift 3; mkdir -p "$PO/$to/inbox"
         # ⛔ THE TOPIC BECOMES A FILENAME, SO IT IS VALIDATED BEFORE IT BECOMES A PATH (s191, seat1).  MEASURED, not hypothetical:
         # calling `send seat8 "<a whole message containing SCRIP/scripts/...>"` made the topic carry slashes, the mv failed with
         # "No such file or directory" -- AND THE SCRIPT PRINTED `sent` ANYWAY.  A seat-to-seat message that is silently dropped
         # while the sender is told it was delivered is unrecoverable and invisible to BOTH ends: the 'non-empty is not alive'
         # false-signal class, on the fleet bus itself.  Two guards, because either alone still lies: refuse a topic that cannot
         # be a filename, and make the RESULT the thing that is reported -- `sent` is now printed only if the mv succeeded.
         case "$topic" in ""|*/*|*$'\n'*) echo "⛔ REFUSED: topic must be a short filename-safe slug (no / and no newline), not the message body. Usage: $0 send <to> <topic> \"text\"" >&2; exit 2;; esac
         t="$(mktemp "$PO/.msg.XXXXXX")"; { echo "FROM $ME TO $to RE $topic"; echo "$*"; } > "$t"
         d="$PO/$to/inbox/$(date +%s%N)-$ME-$topic.msg"
         if mv "$t" "$d" && [ -s "$d" ]; then echo "sent -> $to/$topic"; else rm -f "$t"; echo "⛔ NOT SENT -- could not write $d. The message was DROPPED; nothing was delivered." >&2; exit 1; fi;;
  ask)   topic="${2:?topic}"; shift 2; exec "$0" send hq "q-$topic" "$*";;
  check) d="$PO/$ME/inbox"; mkdir -p "$d"; n=$(ls "$d" 2>/dev/null | wc -l)
         echo "[$ME] inbox: $n message(s)"; for f in "$d"/*.msg; do [ -f "$f" ] || continue; echo "--- $(basename "$f")"; cat "$f"; done;;
  clear) rm -f "$PO/$ME/inbox/"*.msg 2>/dev/null; echo "[$ME] inbox cleared";;
  claim) topic="${2:?topic}"; c="$PO/claims/$topic.claim"; mkdir -p "$PO/claims"
         if [ -f "$c" ]; then own="$(head -1 "$c")"; if [ "$own" = "$ME" ]; then echo "already yours"; else echo "CLAIMED by $own — pick other work"; exit 1; fi
         else t="$(mktemp "$PO/claims/.c.XXXXXX")"; echo "$ME" > "$t"
              if ln "$t" "$c" 2>/dev/null; then rm -f "$t"; echo "claimed $topic"; else rm -f "$t"; echo "RACE LOST: $(head -1 "$c" 2>/dev/null) owns it"; exit 1; fi; fi;;
  done)  topic="${2:?topic}"; c="$PO/claims/$topic.claim"
         if [ -f "$c" ] && [ "$(head -1 "$c")" = "$ME" ]; then grep -q '^DONE$' "$c" || echo DONE >> "$c"; echo "done $topic"
         else echo "not your claim"; exit 1; fi;;
  next)  q="$PO/QUEUE.tsv"; mkdir -p "$PO/claims"
         # ONE matcher for BOTH the presence test and the brief print, so they cannot disagree (that
         # disagreement IS the bug this row fixes). Row absent => the claim is an ORPHAN: renamed or
         # retired out from under its holder (HQ LAW 14). Skip it, SAY WHY, keep the claim (it is the
         # record), fall through to live work. ORPHAN-SKIP IS DISARMED WHEN THE QUEUE ITSELF IS DEAD:
         # a missing/empty QUEUE.tsv would make EVERY claim look orphaned and unpin the whole fleet at
         # once -- an infrastructure failure is not a rename, so we pin and say so.
         qrow() { grep -P "^[0-9]+\t\Q$1\E\t" "$q" 2>/dev/null | head -1; }
         qrows="$(grep -cP '^[0-9]+\t' "$q" 2>/dev/null)"; qrows="${qrows:-0}"
         for c in "$PO"/claims/*.claim; do [ -f "$c" ] || continue
           if [ "$(head -1 "$c")" = "$ME" ] && ! grep -q '^DONE$' "$c"; then
             t="$(basename "$c" .claim)"; row="$(qrow "$t")"
             if [ -z "$row" ] && [ "$qrows" -gt 0 ]; then
               echo "SKIP $t — your unfinished claim names a topic with NO QUEUE.tsv row (renamed or retired out from under you). Claim KEPT as the record; falling through to live work. If you were mid-rung on it: s4e_msg.sh ask $t 'row vanished under me'"
               continue; fi
             if [ -z "$row" ]; then
               echo "⛔ $q IS MISSING OR HAS NO ROWS — cannot tell a renamed row from an unreadable queue, so RESUMING $t rather than unpinning you. Ask hq before trusting any next(1) verdict."; fi
             echo "RESUME $t (yours, unfinished — s4e_msg.sh done $t when the handoff clause is met)"
             printf '%s\n' "$row" | awk -F'\t' 'NF>1{print "brief: " $3; print "first: " $4}'
             exit 0; fi; done
         [ -f "$q" ] || { echo "no QUEUE.tsv — ask hq"; exit 1; }
         while IFS=$'\t' read -r rank topic brief step; do
           case "$rank" in ''|\#*) continue;; esac
           [ -f "$PO/claims/$topic.claim" ] && continue
           if "$0" claim "$topic" >/dev/null 2>&1; then
             echo "LOCKED $topic (rank $rank)"; echo "brief: $brief"; echo "first: $step"; exit 0; fi
         done < "$q"
         echo "QUEUE EMPTY — every row claimed. Ask hq: s4e_msg.sh ask work 'queue empty'"; exit 1;;
  banner) # ⛔ THE VERDICT IS COMPUTED, NEVER TYPED (Lon 2026-08-22): the seat ENDS every session with this, and Lon reads
         # only this. It fuses the three facts that decide whether re-firing this seat advances anything: the PUSH truth
         # ⛔ THE VERDICT IS EXACTLY ONE QUESTION: DID THE WORK LAND AND GET PUSHED (handoff_status.sh rc, the ONLY
         # sanctioned doneness source). Nothing else may flip it -- an open question to HQ is HQ's backlog, NOT this
         # seat's failure, and a seat that finished and pushed is a SUCCESS even with questions outstanding (Lon
         # 2026-08-22, correcting the first cut). Claim state and pending questions inform CONTINUE only.
         hs="$S4E/SCRIP/scripts/handoff_status.sh"
         if [ -f "$hs" ]; then hout="$(timeout 300 bash "$hs" 2>&1)"; hrc=$?; else hout="handoff_status.sh NOT FOUND at $hs"; hrc=2; fi
         held=""; for c in "$PO"/claims/*.claim; do [ -f "$c" ] || continue
           if [ "$(head -1 "$c")" = "$ME" ] && ! grep -q '^DONE$' "$c"; then held="$(basename "$c" .claim)"; break; fi; done
         qwait=0; for f in "$PO"/hq/inbox/*.msg; do [ -f "$f" ] || continue; case "$(basename "$f")" in *-"$ME"-q-*) qwait=$((qwait+1));; esac; done
         topic="${2:-${held:-(none)}}"
         if [ "$hrc" -eq 0 ]; then verdict="✅  S U C C E S S"; else verdict="⛔  F A I L U R E"; fi
         if   [ "$hrc" -ne 0 ]; then nxt="WILL RE-BLOCK — work is NOT pushed. Read the push line above; a credential may be needed."
         elif [ -n "$held" ] && [ "$qwait" -gt 0 ]; then nxt="RESUMES $held — but this seat is waiting on HQ ($qwait question(s)), so it may re-block on the same spot."
         elif [ -n "$held" ];    then nxt="RESUMES $held — row still open, re-fire this seat to continue it."
         else                         nxt="PICKS A NEW ROW — this seat's row is closed and pushed."; fi
         pline="$(printf '%s\n' "$hout" | grep -aiE 'HANDOFF (COMPLETE|BLOCKED)|BLOCKED:' | head -1)"; [ -n "$pline" ] || pline="$(printf '%s\n' "$hout" | tail -1)"
         b='════════════════════════════════════════════════════════════════════════════════'
         printf '\n%s\n' "$b"; printf '  %s        seat=%s  row=%s\n' "$verdict" "$ME" "$topic"; printf '%s\n' "$b"
         printf '  push   : %s  (handoff_status.sh rc=%s)\n' "$pline" "$hrc"
         printf '  claim  : %s\n' "${held:-none held — row closed or never claimed}"
         printf '  CONTINUE: %s\n' "$nxt"
         printf '%s\n' "$b"
         printf '  Next prompt to paste after /clear:\n'
         printf '    Run THE LOOP from your CLAUDE.md: bash SCRIP/scripts/s4e_msg.sh check, then next — execute the brief it prints.\n'
         printf '%s\n\n' "$b"
         [ "$hrc" -eq 0 ] && exit 0 || exit 1;;
  board) if [ $# -gt 1 ]; then shift; grep -v "^$ME |" "$PO/BOARD.md" 2>/dev/null > "$PO/.b.$$" || true; printf '%s | %s | %s\n' "$ME" "$*" "$(date -u +%H:%M)" >> "$PO/.b.$$"; mv "$PO/.b.$$" "$PO/BOARD.md"; fi; cat "$PO/BOARD.md";;
  *) echo "usage: next|done|ask|send|check|clear|claim|board|banner"; exit 2;;
esac
