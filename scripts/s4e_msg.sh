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
# ⛔ UNREAD MAIL IS SHOUTED ON EVERY COMMAND (HQ, 2026-08-22, after seat2 skipped THE LOOP step 1 and left an HQ
# ruling unread in its inbox while asking Lon the same question in chat). The inbox is HQ's ONLY channel to a
# running seat -- Lon does not relay -- so it cannot depend on the seat remembering to `check`. Every subcommand
# except check/clear now surfaces pending mail first. A seat cannot run `next`, `done`, `board` or `banner`
# without being told it has unread HQ mail.
case "$cmd" in check|clear) ;; *)
  _n=0; for _f in "$PO/$ME/inbox"/*.msg; do [ -f "$_f" ] && _n=$((_n+1)); done
  if [ "$_n" -gt 0 ]; then
    printf '\n⛔⛔⛔ %s HAS %s UNREAD HQ MESSAGE(S) — READ THEM BEFORE ANYTHING ELSE ⛔⛔⛔\n' "$ME" "$_n"
    for _f in "$PO/$ME/inbox"/*.msg; do [ -f "$_f" ] || continue; printf '    %s\n' "$(head -1 "$_f")"; done
    printf '    bash SCRIP/scripts/s4e_msg.sh check      <- do this now\n\n'; fi ;;
esac
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
         # ⛔ THE BANNER FIRES ITSELF HERE (HQ 2026-08-22, after seat4 finished its row and gave NO banner until Lon
         # asked for one). LAW 15 lived only as a step in the seat's CLAUDE.md -- and a step in a markdown file is a
         # hope, not a mechanism, exactly like the inbox before `check` was forced. A seat that closes a row runs
         # `done`, so `done` prints the banner. Same reason `board` does. Suppress with S4E_NO_BANNER=1.
         if [ -f "$c" ] && [ "$(head -1 "$c")" = "$ME" ]; then grep -q '^DONE$' "$c" || echo DONE >> "$c"; echo "done $topic"
              [ "${S4E_NO_BANNER:-0}" = "1" ] || "$0" banner "$topic" "${3:-}"
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
  banner) # ⛔ FACTS ONLY -- NO PREDICTIONS (Lon 2026-08-22: "Why are you trying to predict the future. Quit saying
         # in the banner what you will do. You do not know the future."). Every line below is a measured fact about
         # state as it stands. What a later session does is not knowable here: HQ can re-rank the queue, and THE LOOP
         # reads the inbox before the queue. Two laws still hold: the verdict is COMPUTED, never typed, and it turns
         # on ONE question -- did the work land and get pushed (handoff_status.sh rc, the only sanctioned source).
         # An open question to HQ is HQ's backlog, never this seat's failure.
         hs="$S4E/SCRIP/scripts/handoff_status.sh"
         if [ -f "$hs" ]; then hout="$(timeout 300 bash "$hs" 2>&1)"; hrc=$?; else hout="handoff_status.sh NOT FOUND at $hs"; hrc=2; fi
         held=""; for c in "$PO"/claims/*.claim; do [ -f "$c" ] || continue
           if [ "$(head -1 "$c")" = "$ME" ] && ! grep -q '^DONE$' "$c"; then held="$(basename "$c" .claim)"; break; fi; done
         qwait=0; for f in "$PO"/hq/inbox/*.msg; do [ -f "$f" ] || continue; case "$(basename "$f")" in *-"$ME"-q-*) qwait=$((qwait+1));; esac; done
         inbx=0; for f in "$PO/$ME/inbox"/*.msg; do [ -f "$f" ] && inbx=$((inbx+1)); done
         freerows=0; nrow=""; nrank=""
         while IFS=$'\t' read -r rank topic brief step; do case "$rank" in ''|\#*) continue;; esac
           [ -f "$PO/claims/$topic.claim" ] && continue; freerows=$((freerows+1))
           [ -z "$nrow" ] && { nrow="$topic"; nrank="$rank"; }; done < "$PO/QUEUE.tsv" 2>/dev/null
         # A clone predating the 2026-08-21 filter-repo rewrite shares no recent history with origin. Must FETCH
         # first: an unfetched pre-rewrite clone has a stale origin/* ref and reads as pristine.
         diverged=""
         for r in "$S4E"/*/; do [ -d "$r/.git" ] || continue
           br=$(git -C "$r" rev-parse --abbrev-ref HEAD 2>/dev/null) || continue
           git -C "$r" fetch -q origin 2>/dev/null || continue
           git -C "$r" rev-parse --verify -q "origin/$br" >/dev/null 2>&1 || continue
           if ! git -C "$r" merge-base --is-ancestor HEAD "origin/$br" 2>/dev/null; then
             bh=$(git -C "$r" rev-list --count "HEAD..origin/$br" 2>/dev/null || echo 0)
             [ "${bh:-0}" -gt 50 ] && diverged="$diverged $(basename "$r")"; fi; done
         pline="$(printf '%s\n' "$hout" | grep -aiE 'HANDOFF (COMPLETE|BLOCKED)|COMPLETE|BLOCKED:' | head -1)"; [ -n "$pline" ] || pline="$(printf '%s\n' "$hout" | tail -1)"
         # ⛔ IT IS A BANNER, NOT A REPORT (Lon 2026-08-22: "You turned a banner into a TL;DR"). ONE line, glanced at.
         # SUCCESS and safe-to-/clear are the SAME fact -- both are handoff_status.sh rc=0 -- so there is one thing
         # to say. Detail is not lost, it moves behind `banner -v`, which nobody has to read.
         b='════════════════════════════════════════════════════════════════════════════════'
         # ⛔ LEVEL OF SUCCESS (Lon 2026-08-22: "Why are you not giving any feedback whatsoever as to the level of
         # success?"). Pushed-and-clean is NOT the same as accomplished -- a seat that did nothing also pushes clean.
         # So the banner counts what actually LANDED in the last 12h: commits across this seat's repos, FINDINGs added,
         # and whether the row closed. All measured from git, none of it typed. NOTHING LANDED is called out loudly.
         # ⛔ ATTRIBUTABLE ONLY. A bare `log --since` counts commits this clone merely PULLED -- it credited a seat
         # that did nothing with 4 commits. This project's commit messages carry the seat id and the row topic, so
         # the level is measured by ATTRIBUTION: commits naming this seat or its row, and FINDING files naming either.
         rowst="none"; row1=""
         for c in "$PO"/claims/*.claim; do [ -f "$c" ] || continue
           [ "$(head -1 "$c")" = "$ME" ] || continue
           if grep -q '^DONE$' "$c"; then [ "$rowst" = "none" ] && { rowst="CLOSED"; row1="$(basename "$c" .claim)"; }
           else rowst="OPEN"; row1="$(basename "$c" .claim)"; break; fi; done
         cmts=0; for r in "$S4E"/*/; do [ -d "$r/.git" ] || continue
           n=$(git -C "$r" log --since='12 hours ago' -i --grep="$ME" ${row1:+--grep="$row1"} --oneline 2>/dev/null | wc -l); cmts=$((cmts+n)); done
         fnd=$(git -C "$S4E/.github" log --since='12 hours ago' --diff-filter=A --name-only --format= 2>/dev/null | grep '^FINDING-' | grep -ci -e "$ME" ${row1:+-e "$row1"} || true); fnd="${fnd:-0}"
         if [ "$cmts" -eq 0 ] && [ "$fnd" -eq 0 ]; then lvl="⚠ NOTHING ATTRIBUTABLE LANDED"
         else lvl="row ${rowst} · ${cmts} commit(s) · ${fnd} FINDING(s), attributed /12h"; fi
         # ⛔ BEHIND-ONLY IS NOT A FAILURE. handoff_status.sh answers "is this tree in sync"; the banner answers a
         # NARROWER question -- does anything of value live ONLY in this session. A clone merely BEHIND origin (clean
         # tree, nothing unpushed) loses nothing on /clear; it just pulls next time. Measured directly per repo, since
         # calling that FAILURE would have flagged both fireable seats red for something neither seat did.
         onlyhere=0
         for r in "$S4E"/*/; do [ -d "$r/.git" ] || continue
           d=$(git -C "$r" status --porcelain 2>/dev/null | wc -l)
           br=$(git -C "$r" rev-parse --abbrev-ref HEAD 2>/dev/null)
           u=$(git -C "$r" rev-list --count "origin/$br..$br" 2>/dev/null || echo 0)
           onlyhere=$((onlyhere + d + ${u:-0})); done
         if   [ -n "$diverged" ]; then line="⛔ STOP — $ME — PRE-REWRITE CLONE:$diverged — re-clone before use"
         elif [ "$onlyhere" -eq 0 ] && [ "$hrc" -ne 0 ]; then line="✅ SUCCESS — $ME — safe to /clear (behind origin, nothing unpushed) — $lvl"
         elif [ "$hrc" -eq 0 ]; then line="✅ SUCCESS — $ME — safe to /clear — $lvl"
         else                        line="⛔ FAILURE — $ME — do NOT /clear — $lvl — $(printf '%s' "$pline" | sed 's/^ *-* *//')"; fi
         printf '\n%s\n  %s\n%s\n\n' "$b" "$line" "$b"
         if [ "${2:-}" = "-v" ] || [ "${3:-}" = "-v" ]; then
           printf '  its row        : %s\n' "${held:-none open}"
           printf '  its inbox      : %s message(s)   [THE LOOP reads inbox before the queue]\n' "$inbx"
           printf '  its questions  : %s waiting on HQ\n' "$qwait"
           printf '  queue          : %s free row(s); topmost free is %s (rank %s)\n' "$freerows" "${nrow:-none}" "${nrank:--}"
           [ -n "$diverged" ] && printf '  repair         : for r in%s; do git -C %s/$r fetch -q origin && git -C %s/$r reset --hard -q origin/main; done\n' "$diverged" "$S4E" "$S4E"
           printf '  re-prompt      : Run THE LOOP from your CLAUDE.md: bash SCRIP/scripts/s4e_msg.sh check, then next — execute the brief it prints.\n\n'; fi
         { [ "$hrc" -eq 0 ] || [ "$onlyhere" -eq 0 ]; } && [ -z "$diverged" ] && exit 0 || exit 1;;
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
  board) if [ $# -gt 1 ]; then shift; grep -v "^$ME |" "$PO/BOARD.md" 2>/dev/null > "$PO/.b.$$" || true; printf '%s | %s | %s\n' "$ME" "$*" "$(date -u +%H:%M)" >> "$PO/.b.$$"; mv "$PO/.b.$$" "$PO/BOARD.md"; fi; cat "$PO/BOARD.md"
         # posting a board line IS the handoff gesture -- so the banner fires here too (see `done` above).
         [ "${S4E_NO_BANNER:-0}" = "1" ] || "$0" banner;;
  *) echo "usage: next|done|ask|send|check|clear|claim|board|banner|fleet"; exit 2;;
esac
