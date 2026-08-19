#!/usr/bin/env bash
# s4e_msg.sh — the postoffice helper (/home/claude/postoffice/PROTOCOL.md is law). Usage:
#   s4e_msg.sh send <to> <topic> "text"   s4e_msg.sh check   s4e_msg.sh clear
#   s4e_msg.sh claim <topic>              s4e_msg.sh board [my new status text]
set -u
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 sibling root
PO="${S4E_POST:-/home/claude/postoffice}"
ME="${S4E_SEAT:-}"
if [ -z "$ME" ]; then case "$S4E" in /home/claude) ME=hq;; /home/claude1) ME=seat1;; /home/claude2) ME=seat2;; /home/claude3) ME=seat3;; *) ME="$(basename "$S4E")";; esac; fi
cmd="${1:-check}"
case "$cmd" in
  send)  to="${2:?to}"; topic="${3:?topic}"; shift 3; mkdir -p "$PO/$to/inbox"
         t="$(mktemp "$PO/.msg.XXXXXX")"; { echo "FROM $ME TO $to RE $topic"; echo "$*"; } > "$t"
         mv "$t" "$PO/$to/inbox/$(date +%s%N)-$ME-$topic.msg"; echo "sent -> $to/$topic";;
  check) d="$PO/$ME/inbox"; mkdir -p "$d"; n=$(ls "$d" 2>/dev/null | wc -l)
         echo "[$ME] inbox: $n message(s)"; for f in "$d"/*.msg; do [ -f "$f" ] || continue; echo "--- $(basename "$f")"; cat "$f"; done;;
  clear) rm -f "$PO/$ME/inbox/"*.msg 2>/dev/null; echo "[$ME] inbox cleared";;
  claim) topic="${2:?topic}"; c="$PO/claims/$topic.claim"; mkdir -p "$PO/claims"
         if [ -f "$c" ]; then own="$(head -1 "$c")"; if [ "$own" = "$ME" ]; then echo "already yours"; else echo "CLAIMED by $own — pick other work"; exit 1; fi
         else t="$(mktemp "$PO/claims/.c.XXXXXX")"; echo "$ME" > "$t"
              if ln "$t" "$c" 2>/dev/null; then rm -f "$t"; echo "claimed $topic"; else rm -f "$t"; echo "RACE LOST: $(head -1 "$c" 2>/dev/null) owns it"; exit 1; fi; fi;;
  board) if [ $# -gt 1 ]; then shift; grep -v "^$ME |" "$PO/BOARD.md" 2>/dev/null > "$PO/.b.$$" || true; printf '%s | %s | %s\n' "$ME" "$*" "$(date -u +%H:%M)" >> "$PO/.b.$$"; mv "$PO/.b.$$" "$PO/BOARD.md"; fi; cat "$PO/BOARD.md";;
  *) echo "usage: send|check|clear|claim|board"; exit 2;;
esac
