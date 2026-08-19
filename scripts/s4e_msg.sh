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
set -u
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 sibling root
PO="${S4E_POST:-/home/resources/postoffice}"
ME="${S4E_SEAT:-}"
if [ -z "$ME" ]; then case "$S4E" in /home/claude) ME=hq;; /home/claude[1-9]) ME="seat${S4E#/home/claude}";; *) ME="$(basename "$S4E")";; esac; fi
cmd="${1:-check}"
case "$cmd" in
  send)  to="${2:?to}"; topic="${3:?topic}"; shift 3; mkdir -p "$PO/$to/inbox"
         t="$(mktemp "$PO/.msg.XXXXXX")"; { echo "FROM $ME TO $to RE $topic"; echo "$*"; } > "$t"
         mv "$t" "$PO/$to/inbox/$(date +%s%N)-$ME-$topic.msg"; echo "sent -> $to/$topic";;
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
         for c in "$PO"/claims/*.claim; do [ -f "$c" ] || continue
           if [ "$(head -1 "$c")" = "$ME" ] && ! grep -q '^DONE$' "$c"; then
             t="$(basename "$c" .claim)"
             echo "RESUME $t (yours, unfinished — s4e_msg.sh done $t when the handoff clause is met)"
             grep -P "^[0-9]+\t\Q$t\E\t" "$q" 2>/dev/null | head -1 | awk -F'\t' '{print "brief: " $3; print "first: " $4}'
             exit 0; fi; done
         [ -f "$q" ] || { echo "no QUEUE.tsv — ask hq"; exit 1; }
         while IFS=$'\t' read -r rank topic brief step; do
           case "$rank" in ''|\#*) continue;; esac
           [ -f "$PO/claims/$topic.claim" ] && continue
           if "$0" claim "$topic" >/dev/null 2>&1; then
             echo "LOCKED $topic (rank $rank)"; echo "brief: $brief"; echo "first: $step"; exit 0; fi
         done < "$q"
         echo "QUEUE EMPTY — every row claimed. Ask hq: s4e_msg.sh ask work 'queue empty'"; exit 1;;
  board) if [ $# -gt 1 ]; then shift; grep -v "^$ME |" "$PO/BOARD.md" 2>/dev/null > "$PO/.b.$$" || true; printf '%s | %s | %s\n' "$ME" "$*" "$(date -u +%H:%M)" >> "$PO/.b.$$"; mv "$PO/.b.$$" "$PO/BOARD.md"; fi; cat "$PO/BOARD.md";;
  *) echo "usage: next|done|ask|send|check|clear|claim|board"; exit 2;;
esac
