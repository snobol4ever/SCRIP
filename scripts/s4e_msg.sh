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
  banner) # ⛔ LON READS ONLY THIS. He drives the fleet with /clear + one fixed re-fire prompt, so the banner must answer,
         # in plain imperative English, the single question he actually has: WHAT DO I DO NOW. Two laws shape it.
         # (1) THE VERDICT IS EXACTLY ONE QUESTION -- did the work land and get pushed (handoff_status.sh rc, the ONLY
         # sanctioned doneness source). Nothing else may flip it: an open question to HQ is HQ's backlog, NOT this seat's
         # failure (Lon 2026-08-22). (2) IT IS COMPUTED, NEVER TYPED -- a seat cannot assert its own SUCCESS.
         hs="$S4E/SCRIP/scripts/handoff_status.sh"
         if [ -f "$hs" ]; then hout="$(timeout 300 bash "$hs" 2>&1)"; hrc=$?; else hout="handoff_status.sh NOT FOUND at $hs"; hrc=2; fi
         held=""; for c in "$PO"/claims/*.claim; do [ -f "$c" ] || continue
           if [ "$(head -1 "$c")" = "$ME" ] && ! grep -q '^DONE$' "$c"; then held="$(basename "$c" .claim)"; break; fi; done
         qwait=0; for f in "$PO"/hq/inbox/*.msg; do [ -f "$f" ] || continue; case "$(basename "$f")" in *-"$ME"-q-*) qwait=$((qwait+1));; esac; done
         pline="$(printf '%s\n' "$hout" | grep -aiE 'HANDOFF (COMPLETE|BLOCKED)|COMPLETE|BLOCKED:' | head -1)"; [ -n "$pline" ] || pline="$(printf '%s\n' "$hout" | tail -1)"
         if [ "$hrc" -eq 0 ]; then verdict="✅  S U C C E S S"; landed="YES — everything is committed and pushed"
         else                       verdict="⛔  F A I L U R E"; landed="NO  — $pline"; fi
         if [ -n "$held" ]; then rowline="$held — STILL OPEN"; else rowline="none open — last row is finished and closed"; fi
         # ⛔ THE ONE QUESTION LON HAS (his words, 2026-08-22): "I need to know whether to continue or not."
         # So this resolves to a genuine YES/NO, not advice. NO is reserved for the two states where re-firing
         # provably cannot advance: nothing left to pick up, or the seat is parked on an unanswered question and
         # would stop in the same place. Everything else is YES, with the consequence spelled out.
         freerows=0
         while IFS=$'\t' read -r rank topic brief step; do case "$rank" in ''|\#*) continue;; esac
           [ -f "$PO/claims/$topic.claim" ] || freerows=$((freerows+1)); done < "$PO/QUEUE.tsv" 2>/dev/null
         # ⛔ "AFTER /clear" IS A CLOSED ENUM OF FIVE VALUES. Lon NEVER continues a session -- finished or not, he
         # /clears and re-prompts (his words, 2026-08-22). So the useful question is never "should I continue"; it is
         # WHAT THE FRESH SESSION WILL DO, and whether he must fix something before re-prompting. Two independent
         # attributes: the VERDICT above says whether this session's work landed; this says what the next one does.
         #   ✅ RESUMES <row>      row still open; the fresh session carries it on
         #   ✅ PICKS A NEW ROW    this row is COMPLETE and closed; fresh session takes fresh work
         #   ✅ RETRIES THE PUSH   work never reached origin; fresh session pushes it
         #   ⛔ WILL STALL         parked on an unanswered HQ question -- answer it, else the next session stops identically
         #   ⛔ NOTHING TO DO      no open row and no free rows; HQ must unblock rows before re-prompting is worth it
         if   [ -z "$held" ] && [ "$freerows" -eq 0 ]; then cont="⛔ NOTHING TO DO"
              todo="Re-prompting is pointless right now — no open row, and the queue has NO free rows."
              todo2="HQ must add or unblock rows first. Every row with a claim file, DONE or not, is hidden from the picker."
         elif [ -n "$held" ] && [ "$qwait" -gt 0 ]; then cont="⛔ WILL STALL"
              todo="Parked on row $held awaiting an HQ answer ($qwait question(s))."
              todo2="A fresh session stops in the same place. Answer it first, then re-prompt."
         elif [ "$hrc" -ne 0 ]; then cont="✅ RETRIES THE PUSH"
              todo="This session's work never reached origin${held:+; the fresh session resumes row $held}."
              todo2="If it asks you for a push credential, that is what it is stuck on."
         elif [ -n "$held" ]; then cont="✅ RESUMES $held"
              todo="The fresh session carries on with that same row."; todo2=""
         else cont="✅ PICKS A NEW ROW"
              todo="This row is COMPLETE and closed — the fresh session takes fresh work ($freerows rows free)."; todo2=""; fi
         b='════════════════════════════════════════════════════════════════════════════════'
         printf '\n%s\n' "$b"; printf '  %s        seat %s\n' "$verdict" "$ME"; printf '%s\n' "$b"
         printf '  did the work land? : %s\n' "$landed"
         printf '  its row            : %s\n' "$rowline"
         # ⛔ Lon /clears blind -- he has NO view of a session's context fullness (his words, 2026-08-22).
         # This line is the ONE self-reported field on the banner and is labelled as such: no script can
         # compute a model's context use. Pass it as arg 3: `s4e_msg.sh banner <topic> 62`.
         ctx="${3:-}"
         if [ -n "$ctx" ]; then
           if   [ "$ctx" -ge 75 ] 2>/dev/null; then cwarn="⛔ NEARLY FULL — everything is routed and pushed; a /clear here loses nothing"
           elif [ "$ctx" -ge 50 ] 2>/dev/null; then cwarn="over half gone — keep routing to files as you go"
           else cwarn="plenty of room"; fi
           printf '  context (self-rep) : %s%%  — %s\n' "$ctx" "$cwarn"; fi
         printf '%s\n' "$b"
         printf '  ➜  AFTER /clear    : %s\n' "$cont"
         printf '                       %s\n' "$todo"; [ -n "$todo2" ] && printf '                       %s\n' "$todo2"
         printf '                       /clear, then paste this:\n'
         printf '                       Run THE LOOP from your CLAUDE.md: bash SCRIP/scripts/s4e_msg.sh check, then next — execute the brief it prints.\n'
         printf '%s\n\n' "$b"
         [ "$hrc" -eq 0 ] && exit 0 || exit 1;;
  fleet) # ⛔ LON'S HEALTH VIEW (Lon 2026-08-22: "I'll not read much but I will check on the health").
         # ONE screen for the whole fleet, all COMPUTED. Deliberately does NOT run handoff_status.sh per seat
         # (that walks every repo, 9x over) -- it inspects each seat root's clones directly, which is the same
         # truth for the two things that matter: uncommitted work, and commits that never reached origin.
         printf '\n  SEAT   ROW (open claim)                        TREE                  Q  LAST BOARD LINE\n'
         printf '  ────── ────────────────────────────────────────  ────────────────────  ─  ───────────────────────────────\n'
         for seat in hq seat1 seat2 seat3 seat4 seat5 seat6 seat7 seat8; do
           case "$seat" in hq) root=/home/claude;; *) root="/home/claude${seat#seat}";; esac
           [ -d "$root" ] || continue
           row="—"; for c in "$PO"/claims/*.claim; do [ -f "$c" ] || continue
             if [ "$(head -1 "$c")" = "$seat" ] && ! grep -q '^DONE$' "$c"; then row="$(basename "$c" .claim)"; break; fi; done
           dirty=0; unpushed=0; repos=0
           for r in "$root"/*/; do [ -d "$r/.git" ] || continue; repos=$((repos+1))
             d=$(git -C "$r" status --porcelain 2>/dev/null | wc -l); dirty=$((dirty+d))
             br=$(git -C "$r" rev-parse --abbrev-ref HEAD 2>/dev/null)
             u=$(git -C "$r" rev-list --count "origin/$br..$br" 2>/dev/null || echo 0); unpushed=$((unpushed+${u:-0})); done
           if [ "$repos" -eq 0 ]; then tree="no clones"
           elif [ "$dirty" -eq 0 ] && [ "$unpushed" -eq 0 ]; then tree="clean"
           else tree="⛔ ${dirty} dirty ${unpushed} unpushed"; fi
           q=0; for f in "$PO"/hq/inbox/*.msg; do [ -f "$f" ] || continue; case "$(basename "$f")" in *-"$seat"-q-*) q=$((q+1));; esac; done
           bl="$(grep -m1 "^$seat |" "$PO/BOARD.md" 2>/dev/null | cut -d'|' -f2- | cut -c1-46)"; [ -n "$bl" ] || bl="(never posted)"
           printf '  %-6s %-40.40s  %-20.20s  %s  %s\n' "$seat" "$row" "$tree" "$q" "$bl"; done
         free=0; tot=0
         while IFS=$'\t' read -r rank topic brief step; do case "$rank" in ''|\#*) continue;; esac
           tot=$((tot+1)); [ -f "$PO/claims/$topic.claim" ] || free=$((free+1)); done < "$PO/QUEUE.tsv" 2>/dev/null
         printf '\n  queue: %s rows, %s free for the picker (a row with ANY claim file, DONE or not, is hidden)\n' "$tot" "$free"
         printf '  Q = questions from that seat waiting on HQ. A seat with an open ROW resumes it when re-fired.\n\n';;
  board) if [ $# -gt 1 ]; then shift; grep -v "^$ME |" "$PO/BOARD.md" 2>/dev/null > "$PO/.b.$$" || true; printf '%s | %s | %s\n' "$ME" "$*" "$(date -u +%H:%M)" >> "$PO/.b.$$"; mv "$PO/.b.$$" "$PO/BOARD.md"; fi; cat "$PO/BOARD.md";;
  *) echo "usage: next|done|ask|send|check|clear|claim|board|banner|fleet"; exit 2;;
esac
