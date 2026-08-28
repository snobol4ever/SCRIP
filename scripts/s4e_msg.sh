#!/usr/bin/env bash
# s4e_msg.sh — the postoffice helper (/home/resources/postoffice/PROTOCOL.md is law). Usage:
#   s4e_msg.sh next                       ONE-SHOT PICK-AND-LOCK: resume your own unfinished
#                                         claim, else atomically lock the topmost free QUEUE.tsv
#                                         row and print its brief pointer + first step
#   s4e_msg.sh done <topic>               ⭐ VERIFIES the task baton's DONE-WHEN and REFUSES if it fails
#                                         (LAW 1: done is COMPUTED, never declared). Override, loudly and
#                                         recorded: S4E_DONE_OVERRIDE="why". Claims persist as done-markers.
#   s4e_msg.sh ask <topic> "text"         question box: sends to hq as q-<topic>
#   s4e_msg.sh send <to> <topic> "text"   s4e_msg.sh check   s4e_msg.sh clear
#   s4e_msg.sh claim <topic>              s4e_msg.sh board [my new status text]
#   s4e_msg.sh sweep                      ⭐ LAW 4: move landed (DONE) rows out of QUEUE.tsv into
#                                         QUEUE.done.tsv — the buffer must never become a graveyard again
#   s4e_msg.sh assign <seat> <topic>      ⭐ V2-1/LAW 2 ASSIGNMENT IS THE LOCK: HQ writes <seat>'s claim
#                                         atomically + rings a contentless doorbell; that seat's next serves it FIRST
#   s4e_msg.sh banner [topic]             ⛔ MANDATORY LAST ACT OF EVERY SESSION: prints the COMPUTED
#                                         SUCCESS/FAILURE banner + whether re-firing advances anything
set -u
# ⛔ DOTGLOB (seat8, 2026-08-22): every "$S4E"/*/ or "$root"/*/ directory scan below silently skipped
# .github -- bash's bare `*` glob does not match dot-directories by default. That blinds FOUR checks to
# .github specifically: pre-rewrite-clone divergence (banner), this-session commit attribution (banner's
# NOTHING-LANDED headline), unpushed/dirty "safe to /clear" detection (banner), and the fleet health
# dashboard (`fleet`) -- for every seat, always, silently. Measured: a session with a real, pushed .github
# commit + FINDING and a clean handoff_status.sh still printed "NOTHING LANDED" because cmts only walked
# non-dot dirs. `.claude` also becomes visible under dotglob but is not a git repo, so every site's existing
# `[ -d "$r/.git" ] || continue` guard filters it out for free. No other glob in this file is dot-sensitive
# (claims/msg globs are suffix-anchored, e.g. *.claim/*.msg -- nothing here is ever named with a leading dot).
shopt -s dotglob
S4E="${S4E_HOME:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)}"   # D-17 sibling root
PO="${S4E_POST:-/home/resources/postoffice}"
# ⭐ s266 (ceo request, Lon reporting restarted seats assuming DUO): MODE IS COMPUTED, NEVER ASSUMED FROM PROSE.
# /home/resources/postoffice/MODE (ceo custody, first line = value) is the single authority; absent = LOUD, never
# a silent default -- the identity-assert law applied to mode.
s4e_mode_line() { local m; m="$(head -1 "$PO/MODE" 2>/dev/null)"; if [ -n "$m" ]; then printf 'MODE: %s\n' "$m"; else printf '⛔ MODE FILE ABSENT (%s/MODE) -- DO NOT ASSUME A MODE. Ask ceo; DUO-by-default applies ONLY when ceo has not published the file, and its absence here is an ERROR, not a default.\n' "$PO"; fi; }
ME="${S4E_SEAT:-}"
# ⛔⭐ LAW 6 -- IDENTITY IS ASSERTED, NEVER GLOBBED (ARCH-FLEET-CEO.md, preflight V2-4, hq_P s258). The old block
# ended in `*) ME="$(basename "$S4E")"` and every write path did `mkdir -p` on whatever came out. That pair is
# EXACTLY how the phantom `claude01/` mailbox was born: seat01's clone predated the zero-padding commit, its ME
# resolved to the literal "claude01", the script CREATED /home/resources/postoffice/claude01/inbox on the spot,
# and the seat then read an empty box for a day while HQ's real mail piled up in seat01/. An invented identity
# that silently succeeds is the "non-empty is not alive" false-signal class on the fleet bus itself. Now: the
# guess is CANONICALISED first (the known legacy spellings fold onto their real mailbox), then ASSERTED -- and
# an identity with no mailbox is a loud non-zero exit, never a fresh directory.
# ⭐ /home/claude IS `ceo` (Lon s257 topology ruling). The legacy `hq` mailbox stays readable until its backlog
# drains; it is simply no longer any root's identity.
s4e_canon() { case "$1" in
    claude)                    echo ceo;;
    claude0[1-9]|claude1[0-6]) echo "seat${1#claude}";;
    claude[1-9])               echo "seat0${1#claude}";;
    seat[1-9])                 echo "seat0${1#seat}";;
    *)                         echo "$1";; esac; }
s4e_boxes() { for _b in "$PO"/*/; do [ -d "$_b/inbox" ] || continue; basename "$_b"; done; }
s4e_assert_box() { [ -d "$PO/$1/inbox" ] && return 0
    printf '\n⛔⛔⛔ NO POSTOFFICE MAILBOX FOR %s "%s" ⛔⛔⛔\n' "${2:-identity}" "$1" >&2
    printf '    %s does NOT exist, and this script no longer creates one on the fly (LAW 6: that is how the\n' "$PO/$1/inbox" >&2
    printf '    phantom claude01/ mailbox was born and how seat01 lost a day of HQ mail).\n' >&2
    printf '    known mailboxes: %s\n' "$(s4e_boxes | tr '\n' ' ')" >&2
    printf '    if this identity is genuinely new, an HQ creates it DELIBERATELY: %s mailbox %s\n\n' "$0" "$1" >&2
    exit 3; }
# ⭐ THE OWNING HQ IS A FACT ON DISK, NEVER A GUESS. `ask` used to hardcode `hq`; with hq retiring and two HQs
# owning different questions, guessing would route a correctness question into the perf HQ's backlog. Order:
# $S4E_HQ -> the mailbox's own HQ file -> legacy hq while it still exists -> REFUSE and say so.
s4e_hq() { if [ -n "${S4E_HQ:-}" ]; then echo "$S4E_HQ"
    elif [ -s "$PO/$ME/HQ" ]; then head -1 "$PO/$ME/HQ"
    elif [ -d "$PO/hq/inbox" ]; then echo hq
    else echo ""; fi; }
if [ -z "$ME" ]; then case "$S4E" in
    /home/claude)           ME=ceo;;
    /home/claude_C)         ME=hq_C;;
    /home/claude_P)         ME=hq_P;;
    /home/claude_B)         ME=hq_B;;
    /home/claude[0-9][0-9]) ME="seat${S4E#/home/claude}";;
    /home/claude[1-9])      ME="seat0${S4E#/home/claude}";;
    *)                      ME="$(basename "$S4E")";; esac; fi
ME="$(s4e_canon "$ME")"
# ⭐ SHARED BY next() AND banner() (fix-dispatch-bus-two-failure-modes, s266). Resolve a topic's QUEUE.tsv
# rank, or a large sentinel when the row is absent/orphaned. Hoisted to top level (was a next()-local
# function) so both commands sort candidates the SAME deterministic way instead of each falling back to
# glob (alphabetical) file order whenever a seat holds more than one claim.
qrow()  { grep -P "^[0-9]+\t\Q$1\E\t" "$PO/QUEUE.tsv" 2>/dev/null | head -1; }
qrank() { local row; row="$(qrow "$1")"; [ -n "$row" ] && printf '%s' "$row" | cut -f1 || echo 999999; }
# ⭐ picker-skips-blocked-rows (ceo, 2026-08-28) -- SHARED BY next()'s PASS 3. Is the topic named by a
# BLOCKED-ON:/PARKED-AWAITING: state DONE? `done` only ever APPENDS the DONE marker (never deletes a
# claim) and `park` only ever clears a LIVE non-done claim, so claims/*.claim is the durable record;
# QUEUE.done.tsv is the DONE-WHEN's own belt-and-suspenders for a blocker whose claim some future law
# ever does prune.
s4e_blocker_done() {
    local b="$1"; [ -n "$b" ] || return 1
    [ -f "$PO/claims/$b.claim" ] && grep -q '^DONE$' "$PO/claims/$b.claim" && return 0
    # ⛔⭐ DO NOT "FIX" THIS BY ALSO CHECKING THE ROW'S STATE COLUMN — PRESENCE IS THE WHOLE SIGNAL, AND IT IS SOUND.
    # `sweep` moves a row into QUEUE.done.tsv ONLY IF its claim carries DONE — the IDENTICAL test to the line above
    # (see the sweep) case). So membership here == "claim carried DONE at sweep time", which is exactly what this
    # branch is for. ⛔ QUEUE.done.tsv's col3/col4 are the BUFFER'S values copied verbatim at sweep time and are
    # STALE BY CONSTRUCTION: `done`/OVERRIDE appends DONE to claims/<topic>.claim and nothing rewrites the queue
    # row's state column first, so 171 of 300 landed rows read FREE/ASSIGNED and are nonetheless genuinely DONE.
    # A state check here would reject ~57% of legitimately-done blockers and re-block every row behind them.
    # ⭐ MEASURED, not hypothetical (hq_C 2026-08-28, on seat06's report): this file's HEADER used to name legacy v1
    # columns (`brief`/`first-step-and-done-when`) it has never carried, which makes col4 read as an authoritative
    # state field and makes this branch look like a presence-vs-state bug. It is not. seat06 read this function
    # carefully, reproduced the symptom twice, and released a rank-1 row (tests-consolidate-icon) TWICE on a
    # correctly-observed symptom with a non-existent root cause — the blocker (icon-corpus-semicolonize) had been
    # genuinely DONE by documented OVERRIDE since 2026-08-24, and branch 1 above fires on it anyway.
    # FINDING-2026-08-28-hq_C-the-done-tsv-header-named-columns-the-file-does-not-have.md
    grep -qP "^[0-9]+\t\Q$b\E\t" "$PO/QUEUE.done.tsv" 2>/dev/null
}
cmd="${1:-check}"
case "$cmd" in mailbox|"") ;; *) s4e_assert_box "$ME" identity;; esac
# ⛔ ORPHANED .msg.* ARE SWEPT ON EVERY RUN (LAW 6, second half). `send` writes the message to a mktemp
# $PO/.msg.XXXXXX and then mv's it into the destination inbox; when that mv failed the temp file just SAT there
# -- one rotted 46 hours at the postoffice root, a seat-to-seat brief neither end ever knew was lost. A message
# whose header still names a reachable mailbox is RE-DELIVERED here; one that does not is REPORTED loudly and
# left on disk. Never deleted: an undeliverable message is evidence, not garbage.
s4e_sweep_orphans() { for _o in "$PO"/.msg.*; do [ -f "$_o" ] || continue
    _h="$(head -1 "$_o")"; _to="$(printf '%s' "$_h" | sed -n 's/^FROM [^ ]* TO \([^ ]*\) RE .*$/\1/p')"
    _tp="$(printf '%s' "$_h" | sed -n 's/^FROM [^ ]* TO [^ ]* RE \([^ :]*\).*$/\1/p')"
    _fr="$(printf '%s' "$_h" | sed -n 's/^FROM \([^ ]*\) TO .*$/\1/p')"
    _to="$(s4e_canon "${_to:-}")"; _fr="$(s4e_canon "${_fr:-}")"
    if [ -n "$_to" ] && [ -d "$PO/$_to/inbox" ] && [ -n "$_tp" ]; then
      if mv "$_o" "$PO/$_to/inbox/$(date +%s%N)-${_fr:-unknown}-${_tp}.msg" 2>/dev/null; then
        printf '⭐ ORPHAN SWEPT: a message stranded at the postoffice root was delivered to %s (topic %s, from %s)\n' "$_to" "$_tp" "${_fr:-unknown}"; continue; fi; fi
    printf '⛔ ORPHAN MESSAGE, UNDELIVERABLE AND LEFT IN PLACE: %s -- header reads: %s\n' "$_o" "$(printf '%s' "$_h" | cut -c1-100)" >&2; done; }
# ⭐ ONE mailbox->root map and ONE HQ-set, used by BOTH `fleet` and `banner` so the two views can never
# disagree about who exists. A mailbox with no root is not an error -- `hq` is exactly that today: retiring,
# still holding a 29-message backlog that must stay VISIBLE until it is drained.
s4e_root() { case "$1" in ceo|hq) echo /home/claude;; hq_C) echo /home/claude_C;; hq_P) echo /home/claude_P;; hq_B) echo /home/claude_B;;
    seat0[1-9]|seat1[0-6]) echo "/home/claude${1#seat}";; *) echo "";; esac; }
s4e_hqboxes() { for _h in hq hq_C hq_P hq_B ceo; do [ -d "$PO/$_h/inbox" ] && echo "$_h"; done; }
s4e_is_hq() { case "$1" in hq|hq_C|hq_P|hq_B|ceo) return 0;; *) return 1;; esac; }
# ⛔ DECORATED NO-OP EVASION, COMPANION FIX to `done`'s own no-op blocklist below (row
# `donewhen-decorated-noop-evasion`; the gate `test_gate_baton_donewhen_runnable.sh` carries the identical
# fix and the full rationale). Strips a trailing shell comment the way bash itself would -- quote-aware: a
# `#` starts a comment only when it is outside any quoting and at the start of a word -- WITHOUT ever
# executing its argument. Used so `done`'s blocklist judges `exit 0 # nothing to verify` by what it reduces
# to (`exit 0`), not by its undigested text.
s4e_strip_donewhen_comment() {
    awk '
    {
        line = $0; out = ""; state = 0; n = length(line); i = 1
        while (i <= n) {
            c = substr(line, i, 1)
            if (state == 1) {
                out = out c
                if (c == "\047") state = 0
                i++; continue
            }
            if (state == 2) {
                if (c == "\\" && i < n) { out = out c substr(line, i + 1, 1); i += 2; continue }
                out = out c
                if (c == "\"") state = 0
                i++; continue
            }
            if (c == "\\" && i < n) { out = out c substr(line, i + 1, 1); i += 2; continue }
            if (c == "\047") { state = 1; out = out c; i++; continue }
            if (c == "\"") { state = 2; out = out c; i++; continue }
            if (c == "#") {
                prev = (i == 1) ? " " : substr(line, i - 1, 1)
                if (prev == " " || prev == "\t") break
                out = out c; i++; continue
            }
            out = out c; i++
        }
        sub(/[ \t]+$/, "", out)
        print out
    }'
}
# age in whole minutes of the oldest .msg in a mailbox; empty when the box is clear.
s4e_oldest_min() { _old=""; for _f in "$PO/$1/inbox"/*.msg; do [ -f "$_f" ] || continue
    _m=$(( ( $(date +%s) - $(stat -c %Y "$_f" 2>/dev/null || echo 0) ) / 60 ))
    [ -z "$_old" ] && _old=$_m; [ "$_m" -gt "$_old" ] && _old=$_m; done; echo "$_old"; }
# ⭐ compact age for a MTIME, in the units a reader actually compares ("7m", "3h12m", "2d01h"). Empty in -> "-".
s4e_age_compact() { [ -n "${1:-}" ] && [ "$1" -gt 0 ] 2>/dev/null || { echo "-"; return; }
    _s=$(( $(date +%s) - $1 )); [ "$_s" -lt 0 ] && _s=0
    if   [ "$_s" -lt 3600 ]; then printf '%dm\n' $(( _s / 60 ))
    elif [ "$_s" -lt 86400 ]; then printf '%dh%02dm\n' $(( _s / 3600 )) $(( (_s % 3600) / 60 ))
    else printf '%dd%02dh\n' $(( _s / 86400 )) $(( (_s % 86400) / 3600 )); fi; }
# ⭐ banner-attributes-wrong-row-on-unclaim (s273): a SESSION-SCOPED receipt of the last row THIS seat
# actually closed (released or done'd), written at the moment of the transition. banner's row1 fallback
# (bare invocation, no $pref -- Stop hook, `board`) reads this instead of rescanning ALL historical claims
# under this identity, which is what let an ancient unrelated DONE claim surface as if it were this
# session's work. Two lines: topic, then "STATE ISO-TIMESTAMP". Overwritten each transition -- only the
# LATEST closing action is ever attributed, matching "what did this seat just finish."
s4e_mark_row() { mkdir -p "$PO/$ME"; printf '%s\n%s %s\n' "$1" "$2" "$(date -u +%Y-%m-%dT%H:%MZ)" > "$PO/$ME/.last-row"; }
s4e_sweep_orphans
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
  send)  to="$(s4e_canon "${2:?to}")"; topic="${3:?topic}"; shift 3; s4e_assert_box "$to" destination
         # ⛔ THE TOPIC BECOMES A FILENAME, SO IT IS VALIDATED BEFORE IT BECOMES A PATH (s191, seat1).  MEASURED, not hypothetical:
         # calling `send seat8 "<a whole message containing SCRIP/scripts/...>"` made the topic carry slashes, the mv failed with
         # "No such file or directory" -- AND THE SCRIPT PRINTED `sent` ANYWAY.  A seat-to-seat message that is silently dropped
         # while the sender is told it was delivered is unrecoverable and invisible to BOTH ends: the 'non-empty is not alive'
         # false-signal class, on the fleet bus itself.  Two guards, because either alone still lies: refuse a topic that cannot
         # be a filename, and make the RESULT the thing that is reported -- `sent` is now printed only if the mv succeeded.
         case "$topic" in ""|*/*|*$'\n'*) echo "⛔ REFUSED: topic must be a short filename-safe slug (no / and no newline), not the message body. Usage: $0 send <to> <topic> \"text\"" >&2; exit 2;; esac
         # ⭐⭐ --stdin READS THE BODY FROM STDIN, WHICH TAKES THE CALLER'S SHELL OUT OF THE BODY PATH ENTIRELY
         # (hq_C 2026-08-27, on hq_P's find). ⛔ THE DEFECT IT CURES IS NOT A BACKTICK PROBLEM, IT IS A SHELL-EXPANSION
         # PROBLEM: a body written inside DOUBLE QUOTES is expanded by the caller's shell BEFORE this script ever runs,
         # so backticks, $(...) and bare $NAME are all silently REPLACED BY THEIR EXPANSION -- usually nothing. Both HQs
         # hit it the same day: hq_C lost the word `done` from a sentence about the done subcommand; hq_P lost a whole
         # $(subst ...) from a Makefile line, ONE MESSAGE AFTER READING hq_C's warning about it. Discipline demonstrably
         # does not fix this -- hq_P had the warning in hand and still walked in, because the rule was stated as "no
         # backticks" and the metacharacter that bit them was different. ⛔ AND THIS SCRIPT CANNOT DETECT IT: the damage
         # happens in the CALLER's shell, so what arrives here is a well-formed body that simply is not what was typed,
         # with no residue to test for. That is why the cure has to be a SAFE INPUT PATH rather than a validator.
         # ⭐ USE:  s4e_msg.sh send <to> <topic> --stdin <<'MSG'   ... body ...   MSG
         # The QUOTED heredoc delimiter is the load-bearing part: <<'MSG' disables expansion, <<MSG does not.
         if [ "${1:-}" = "--stdin" ] || [ "${1:-}" = "-" ]; then _body="$(cat)"; else _body="$*"; fi
         [ -n "$_body" ] || { echo "⛔ REFUSED: empty message body. With --stdin, check the heredoc actually delivered." >&2; exit 1; }
         t="$(mktemp "$PO/.msg.XXXXXX")"; { echo "FROM $ME TO $to RE $topic"; echo "$_body"; } > "$t"
         d="$PO/$to/inbox/$(date +%s%N)-$ME-$topic.msg"
         if mv "$t" "$d" && [ -s "$d" ]; then echo "sent -> $to/$topic"; else rm -f "$t"; echo "⛔ NOT SENT -- could not write $d. The message was DROPPED; nothing was delivered." >&2; exit 1; fi;;
  ask)   topic="${2:?topic}"; shift 2; _hq="$(s4e_hq)"
         [ -n "$_hq" ] || { echo "⛔ REFUSED: no owning HQ resolved for $ME. Set S4E_HQ=hq_C|hq_P|hq_B, or have your HQ write it: echo hq_C > $PO/$ME/HQ" >&2; exit 2; }
         exec "$0" send "$_hq" "q-$topic" "$*";;
  # ⛔ CLEAR DELETES ONLY WHAT CHECK DISPLAYED (hq_C s269). MEASURED, not hypothetical: a message arrived
  # between this seat's `check` and its `clear`, and the old `rm -f *.msg` destroyed it UNREAD and
  # UNRECOVERABLE -- clear does not archive, and no sent-copy is kept anywhere. The banner had even printed
  # "2 UNREAD" one line above the rm. THE LOOP's own step 1 (read -> act -> clear) makes that window a
  # certainty, not an accident: acting on a brief is exactly when a seat is slowest. So `check` now records
  # what it showed, and `clear` refuses to remove anything it did not.
  check) d="$PO/$ME/inbox"; n=$(ls "$d" 2>/dev/null | wc -l); lc="$PO/$ME/.last-check"
         echo "[$ME] inbox: $n message(s)"; : > "$lc"
         for f in "$d"/*.msg; do [ -f "$f" ] || continue; basename "$f" >> "$lc"; echo "--- $(basename "$f")"; cat "$f"; done;;
  clear) d="$PO/$ME/inbox"; lc="$PO/$ME/.last-check"
         [ -f "$lc" ] || { echo "⛔ REFUSED: nothing has been read in this seat. Run 'check' first -- clear only removes what check displayed." >&2; exit 2; }
         _cl=0; _kept=0
         for f in "$d"/*.msg; do [ -f "$f" ] || continue; _b="$(basename "$f")"
           if grep -qxF "$_b" "$lc" 2>/dev/null; then rm -f "$f"; _cl=$((_cl+1))
           else _kept=$((_kept+1)); echo "⛔ KEPT UNREAD (arrived after your last check): $_b"; fi; done
         rm -f "$lc"
         if [ "$_kept" -gt 0 ]; then echo "[$ME] cleared $_cl, KEPT $_kept UNREAD -- run 'check' again before you stop"; exit 1
         else echo "[$ME] inbox cleared ($_cl)"; fi;;
  # ⭐ CREATION IS A DELIBERATE ACT WITH A NAME (V2-4). LAW 6 forbids mailboxes appearing as a side effect of a
  # typo, not mailboxes existing -- Lon adds seats, and a fleet that cannot enrol one is not operable. So the
  # capability survives as ONE explicit subcommand that says what it did, and every implicit mkdir is gone.
  mailbox) nm="$(s4e_canon "${2:?mailbox name}")"
         case "$nm" in ""|*/*|*$'\n'*|.*) echo "⛔ REFUSED: mailbox name must be a plain slug" >&2; exit 2;; esac
         if [ -d "$PO/$nm/inbox" ]; then echo "mailbox $nm already exists"; else mkdir -p "$PO/$nm/inbox" && echo "created mailbox $nm (deliberate, by $ME)"; fi;;
  claim) topic="${2:?topic}"; c="$PO/claims/$topic.claim"; mkdir -p "$PO/claims"
         if [ -f "$c" ]; then own="$(head -1 "$c")"; if [ "$own" = "$ME" ]; then echo "already yours"; else echo "CLAIMED by $own — pick other work"; exit 1; fi
         else t="$(mktemp "$PO/claims/.c.XXXXXX")"; echo "$ME" > "$t"
              if ln "$t" "$c" 2>/dev/null; then rm -f "$t"; echo "claimed $topic"; else rm -f "$t"; echo "RACE LOST: $(head -1 "$c" 2>/dev/null) owns it"; exit 1; fi; fi;;
  unclaim) # ⭐ s265 — RELEASE AN UNWORKED CLAIM. Minted because THREE seats hit its absence in one day (seat08,
         # seat09, seat13): a stale-clone picker mis-locked a row, the seat correctly refused to work it, and then had
         # NO WAY TO PUT IT BACK. `done` was the only exit and `done` is COMPUTED — closing a row you never worked
         # would have to defeat the DONE-WHEN gate, which is the false-green trap the gate exists to stop. So seats
         # held locks on rows they were not working, and a claimed row hides itself from the picker: one stale clone
         # silently removed a row from the whole fleet's reach. ⛔ REFUSES a claim that is DONE (that is a receipt, not
         # a lock) and one you do not own. The release is APPENDED to the baton's LEDGER, so a lock that was taken and
         # returned leaves a trace instead of vanishing.
         topic="${2:?topic}"; c="$PO/claims/$topic.claim"
         [ -f "$c" ] || { echo "no claim on $topic — nothing to release"; exit 1; }
         own="$(head -1 "$c")"
         # ⛔ s272 hq_C — THE CODE CONTRADICTED ITS OWN MESSAGE. The header above and this very string both said "or by
         # an HQ", and the test was a bare $own = $ME, so NO HQ could ever release anything. That is not cosmetic: in
         # FLEET-12 the seats that hold the stalest locks are seat13..seat16, which DO NOT EXIST in this mode and can
         # never come back to release their own claims. 9 rows were hidden from the picker by seats that are not
         # running, with no in-tool way to free them short of rm. An HQ release is ANNOTATED with who forced it.
         if [ "$own" != "$ME" ]; then
             s4e_is_hq "$ME" || { echo "⛔ $topic is claimed by $own, not you — a lock is released by its holder or by an HQ"; exit 1; }
             why="${3:-released by HQ}"; why="$why (forced by $ME over $own's claim)"
         fi
         # ⛔⭐ A COMPUTED DONE AND AN OVERRIDE DONE ARE NOT THE SAME THING, AND UNTIL 2026-08-27 THIS LINE COULD NOT
         # TELL THEM APART (hq_C, as queue custodian, on a ceo audit ask). `done` at :288 appends the SAME bare `DONE`
         # token on BOTH paths -- the DONE-WHEN-passed path and the S4E_DONE_OVERRIDE path -- so a seat who overrode
         # with the reason "Session-boundary pause, not task completion" left a claim indistinguishable from a genuine
         # computed receipt. This refusal then made it PERMANENT: corpus-suites-consolidation, a genuinely open row,
         # was invisible to the picker from 2026-08-24 to 2026-08-27 because the only tool that could free it refused
         # on a token that did not mean what it read as. ⭐ SAME SHAPE AS THE OTHER INSTRUMENTS THIS WEEK: two distinct
         # states collapsing into one indistinguishable output (PASS(0) = "checked, clean" or "never asked"; rc=2 =
         # "work unfinished" or "binary missing"). ⛔ THE RATIONALE IN THE HEADER IS "that is a RECEIPT, not a lock" --
         # and an OVERRIDE done is precisely NOT a receipt: nothing was computed, a human asserted it. So it is
         # releasable, loudly and with a trace. A bare computed DONE still refuses, unchanged.
         if grep -q '^DONE$' "$c"; then
           if grep -q '^OVERRIDE-BY ' "$c"; then
             echo "⚠ $topic is DONE by OVERRIDE, not by a computed DONE-WHEN — releasing (an override is an assertion, not a receipt)."
             sed -n 's/^OVERRIDE-BY /    original override: /p' "$c" | head -1 | cut -c1-200
           else
             echo "⛔ $topic is DONE — that claim is a receipt, not a lock. Leave it."; exit 1
           fi
         fi
         why="${why:-${3:-released unworked}}"
         b="$PO/tasks/$topic.task.md"
         [ -f "$b" ] && printf '\n- %s **RELEASED** by %s — %s (claim removed; row returns to the picker)\n' "$(date -u +%Y-%m-%dT%H:%MZ)" "$ME" "$why" >> "$b"
         s4e_mark_row "$topic" RELEASED
         rm -f "$c"; echo "released $topic — $why";;
  park)  # ⭐ s265 — TAKE A ROW OUT OF THE PICKER WITHOUT CLOSING IT. Minted with `unclaim`, same day, same cause:
         # `161-o2-red` was PARKED BY LON at s258 and its baton said so, yet QUEUE.tsv still carried it as rank-0 FREE,
         # so the rank-sorted picker served it as the TOPMOST work in the fleet. seat08 read the ruling, refused it,
         # and then had to SIT ON THE CLAIM to stop the next idle seat being handed the same parked row. ⛔ The state
         # column was decorative — 94 of 94 rows read FREE and nothing consulted it. `park` writes it and PASS 3 now
         # obeys it, so a ruling recorded in a baton is finally enforced by the dispatcher instead of by a seat's
         # goodwill. Un-park with: park <topic> FREE.
         # ⭐ picker-skips-blocked-rows: `park <topic> BLOCKED-ON:<other-topic>` (or PARKED-AWAITING:<other-topic>,
         # the pre-existing spelling several rows already use) is not just cosmetic prose — next()'s PASS 3 parses
         # it and un-parks the row back to FREE BY ITSELF the moment <other-topic>'s claim goes DONE (or it is
         # swept into QUEUE.done.tsv). Recording a block this way, not as bare PARKED/BLOCKED text, is what makes
         # it self-clearing instead of needing a human to remember to come back and re-park it.
         topic="${2:?topic}"; st="${3:-PARKED}"; q="$PO/QUEUE.tsv"
         grep -qP "^[0-9]+\t\Q$topic\E\t" "$q" || { echo "⛔ no QUEUE.tsv row named $topic"; exit 1; }
         # ⛔ s266 — PARK MUST NOT DESTROY ANOTHER SEAT'S RUNNING CLAIM. hq_C parked rung-E5-suspend-cache as
         # SUPERSEDED while seat13 was mid-flight on it; the rm below deleted their claim, so their computed `done`
         # said "not your claim" AFTER the fix was already landed and pushed. A park is a routing verdict on a ROW;
         # it has no authority over a claim it does not hold. Now: a live claim held by someone else REFUSES the
         # park (like assign); S4E_PARK_FORCE=1 overrides but ANNOTATES the claim instead of deleting it, so the
         # holder's `done` still works.
         c="$PO/claims/$topic.claim"
         if [ -f "$c" ] && ! grep -q '^DONE$' "$c" && [ "$(head -1 "$c")" != "$ME" ]; then
             hold="$(head -1 "$c")"
             if [ "${S4E_PARK_FORCE:-0}" != "1" ]; then echo "⛔ REFUSED: $topic is claimed RUNNING by $hold — a park is not an eviction. Ask them, or S4E_PARK_FORCE=1 to park AROUND the claim (it is preserved)."; exit 1; fi
             printf 'PARKED-AROUND by %s %s (claim preserved; holder done still works)\n' "$ME" "$(date -u +%Y-%m-%dT%H:%MZ)" >> "$c"; echo "  (claim held by $hold PRESERVED — parked around it)"
         elif [ -f "$c" ] && ! grep -q '^DONE$' "$c" ; then
             rm -f "$c"; echo "  (cleared my own holding claim — the state column carries this now, not a lock)"
         fi
         tmp="$(mktemp)"; awk -F'\t' -v OFS='\t' -v t="$topic" -v s="$st" '$2==t&&NF>3{$4=s} {print}' "$q" > "$tmp" && cat "$tmp" > "$q" && rm -f "$tmp"
         b="$PO/tasks/$topic.task.md"
         [ -f "$b" ] && printf '\n- %s **STATE -> %s** by %s\n' "$(date -u +%Y-%m-%dT%H:%MZ)" "$st" "$ME" >> "$b"
         echo "$topic state -> $st";;
  done)  topic="${2:?topic}"; c="$PO/claims/$topic.claim"
         # ⛔ THE BANNER FIRES ITSELF HERE (HQ 2026-08-22, after seat4 finished its row and gave NO banner until Lon
         # asked for one). LAW 15 lived only as a step in the seat's CLAUDE.md -- and a step in a markdown file is a
         # hope, not a mechanism, exactly like the inbox before `check` was forced. A seat that closes a row runs
         # `done`, so `done` prints the banner. Same reason `board` does. Suppress with S4E_NO_BANNER=1.
         if [ -f "$c" ] && [ "$(head -1 "$c")" = "$ME" ]; then
              # ⛔⭐⭐ DONE IS COMPUTED, NEVER DECLARED (ARCH-FLEET-CEO.md LAW 1 "NO HAND-TYPED VERDICTS", γ port).
              # Until now `done` appended the DONE marker UNCONDITIONALLY and never ran the task's DONE-WHEN --
              # so the one command whose entire job is to certify completion accepted the seat's word for it.
              # That is the exact shape of the v1 free-r10 inversion (an HQ ruling DONE on prose), sitting inside
              # the control plane written to forbid it. A law the tooling does not enforce is a hope.
              tf="$PO/tasks/$topic.task.md"
              if [ -f "$tf" ]; then
                dw="$(sed -n 's/^DONE-WHEN:[[:space:]]*//p' "$tf" | head -1)"
                if [ -z "$dw" ]; then
                  echo "⛔ REFUSED: $tf has no DONE-WHEN: line. A task with no computable completion test cannot be closed." >&2; exit 1; fi
                # ⛔⭐ A DONE-WHEN MUST EXAMINE SOMETHING (hq_P found this by source-reading the previous version).
                # The old code accepted ANY non-empty string and ANY exit 0 as proof, so `DONE-WHEN: true` — or `:`,
                # or `exit 0` — closed a row having verified nothing. That is EXACTLY the vacuous-gate defect hq_P
                # had just finished removing from 31 gates, reappearing in the one command whose job is to certify
                # completion. Two checks, because either alone still lets something through.
                case "$(printf '%s' "$dw" | tr -d '[:space:]')" in
                  true|:|exit0|/bin/true|/usr/bin/true|""|"#"*)
                    printf '⛔ REFUSED: the DONE-WHEN in %s is a shell no-op (%s). It certifies nothing.\n' "$tf" "$dw" >&2
                    printf '   A DONE-WHEN must be a command that EXAMINES the tree and can exit non-zero when the work is not done.\n' >&2
                    exit 1;; esac
                # ⛔⭐ DECORATED NO-OP EVASION -- THE BLOCKLIST ABOVE ALONE IS NOT ENOUGH (found live, this session,
                # row `donewhen-decorated-noop-evasion`, while proving out the companion fix in
                # test_gate_baton_donewhen_runnable.sh). `tr -d '[:space:]'` then exact-match slides past a
                # trailing comment or a decorative argument -- `exit 0 # nothing to verify` and `: ok, done` are
                # NOT `exit0`/`:`. The VACUITY PROBE below independently catches those two specific witnesses
                # (they contain neither `/` nor `$`, so the probe still runs on them) -- but the probe SKIPS
                # itself whenever `dw` contains `/` or `$` (a path or a variable reference), which a decorated
                # no-op can easily also contain. REPRODUCED LIVE in a scratch postoffice: `: ok, done # see
                # /path/to/notes` and `exit 0 # nothing to verify, cf $HOME` both CLOSED THE ROW (exit 0, DONE
                # appended) under the pre-fix code -- the vacuity probe's own skip condition was the opening.
                # This check runs BEFORE the vacuity probe and does not depend on it: same comment-stripping,
                # same args-tolerant true/:/only-slash-true family, same bare-`echo` refusal as the gate script.
                dw_nc="$(printf '%s\n' "$dw" | s4e_strip_donewhen_comment)"
                dw_norm="$(printf '%s' "$dw_nc" | sed -E 's/^[[:space:]]+//; s/[[:space:]]+$//; s/[[:space:]]+/ /g')"
                if ! printf '%s' "$dw_norm" | grep -qE '[;|&`]|\$\('; then
                  _dw_first="$(printf '%s' "$dw_norm" | awk '{for(i=1;i<=NF;i++){if($i !~ /^[A-Za-z_][A-Za-z0-9_]*=/){print $i; exit}}}')"
                  _dw_noop=0
                  case "$dw_norm" in ""|"exit 0"|exit0) _dw_noop=1 ;; esac
                  case "$_dw_first" in true|:|/bin/true|/usr/bin/true|echo) _dw_noop=1 ;; esac
                  if [ "$_dw_noop" = "1" ]; then
                    printf '⛔ REFUSED: the DONE-WHEN in %s is a decorated shell no-op (%s -> reduces to: %s). It certifies nothing.\n' "$tf" "$dw" "$dw_norm" >&2
                    printf '   A DONE-WHEN must be a command that EXAMINES the tree and can exit non-zero when the work is not done.\n' >&2
                    exit 1
                  fi
                fi
                # VACUITY PROBE, borrowed from hq_P V2-5: run it in an EMPTY scratch directory. A criterion that
                # passes with nothing to examine is not examining anything. Skipped when the command names an
                # absolute path, because those legitimately still resolve from anywhere.
                case "$dw" in
                  */*|*'$'*) : ;;                       # names a path or expands a variable — probe would be meaningless
                  *) _vac="$(mktemp -d)"
                     ( cd "$_vac" && timeout 20 bash -c "$dw" ) >/dev/null 2>&1; _vrc=$?
                     rm -rf "$_vac"
                     # ⛔⭐ A TIMEOUT IS NOT AN ANSWER (hq_P 2026-08-27, source-read; hole was real and unreached).
                     # The probe used to branch on the COMMAND SUCCEEDING, so EVERY non-zero exit counted as proof the
                     # criterion examines the tree -- and `timeout` exits 124, non-zero, on a criterion that NEVER
                     # FINISHED. The two answers this probe exists to separate -- "correctly refused with nothing to
                     # examine" and "never ran to completion" -- shared one output, with no way to say which. That is
                     # fail-OPEN, inside the one command whose whole job is certifying completion. Same shape as the
                     # week's other mute instruments (mtime = lock-taken-not-work; PASS(0) = checked-or-never-asked).
                     # An instrument that cannot measure REFUSES rc=2; it does not pass what it failed to read.
                     if [ "$_vrc" -eq 124 ]; then
                       printf '⛔ REFUSED (rc=2): the DONE-WHEN in %s did not FINISH within 20s in an empty directory, so the\n' "$tf" >&2
                       printf '   vacuity probe could not measure it. This is NOT a pass -- a criterion that hangs when there is\n' >&2
                       printf '   nothing to examine is not evidence that it examines anything.\n   command: %s\n' "$dw" >&2
                       exit 2; fi
                     if [ "$_vrc" -eq 0 ]; then
                       printf '⛔ REFUSED: the DONE-WHEN in %s passes in an EMPTY directory, so it is not examining this tree.\n' "$tf" >&2
                       printf '   command: %s\n   Make it name what it inspects. (hq_P V2-5 used the same probe to find 31 vacuous gates.)\n' "$dw" >&2
                       exit 1; fi;; esac
                if [ -n "${S4E_DONE_OVERRIDE:-}" ]; then
                  # ω-class escape hatch: loud, recorded, and never silent. For the case where the DONE-WHEN itself
                  # is proven wrong -- which is a real event, and is why it must be auditable rather than forbidden.
                  printf 'OVERRIDE-BY %s %s reason: %s\n' "$ME" "$(date -u +%FT%TZ)" "$S4E_DONE_OVERRIDE" >> "$c"
                  printf '⚠ DONE-WHEN OVERRIDDEN by %s -- reason recorded in the claim: %s\n' "$ME" "$S4E_DONE_OVERRIDE"
                else
                  printf 'verifying DONE-WHEN (γ): %s\n' "$dw"
                  # ⛔ bash -c, NOT bare eval: `timeout` execs its argument, so it CANNOT run a shell BUILTIN. A criterion
                  # beginning with `cd` — a natural and common way to write one — died with "timeout: failed to run
                  # command 'cd'" and exit 127, making that row permanently uncloseable. My own certifier rejecting
                  # valid work. Found by running all 86 live criteria from the CWD `done` actually uses.
                  # ⭐⭐ THE OUTPUT IS CAPTURED, NOT DISCARDED (hq_C 2026-08-27, proposed to ceo and approved; CEO-30).
                  # A MUTE CORRECT GATE IS ITS OWN DEFECT CLASS. This refusal used to print ONLY the exit code, so
                  # "DONE-WHEN exited 2" meant "work unfinished" OR "scrip binary missing" OR "path wrong" OR "corpus
                  # subtree moved" -- indistinguishable. MEASURED COST: hq_C's own row refused three times in one
                  # session; two cycles went to a WRONG hypothesis (CWD anchoring, which seat10 had just legitimately
                  # fixed elsewhere and was sitting in the BOARD as a ready-made plausible cause) before the real cause
                  # turned out to be a vanished ./scrip binary -- itself the Stop-hook pristine race, diagnosed the
                  # same day. hq_P hit the identical shape from the other side with a different wrong hypothesis.
                  # ⛔ A MUTE GATE PLUS A PLAUSIBLE NEARBY CULPRIT DOES NOT MERELY FAIL TO NAME THE CAUSE -- IT POINTS
                  # SOMEWHERE ELSE. The criterion's own stderr said "REFUSED TO GRADE: scrip not built at ..." the whole
                  # time; nothing was broken except that nobody could see it. Changes no verdict, weakens no gate, and
                  # has no false-green path: it only makes a correct refusal legible.
                  _dwlog="$(mktemp)"
                  # ⛔⭐ S4E_HOME IS EXPORTED INTO THE CRITERION, AND WITHOUT THIS 11 LIVE ROWS COULD NEVER CLOSE
                  # (hq_C 2026-08-27, found closing srcreorg-ladder). `S4E` is derived here as "${S4E_HOME:-<from $0>}"
                  # -- an INPUT variable, never exported -- so a DONE-WHEN written as `cd "$S4E_HOME/SCRIP" && ...`,
                  # which 11 task files use and which reads as the obviously-correct portable idiom, expanded to the
                  # EMPTY STRING and ran `cd /SCRIP`. ⛔ THE REAL DEFECT IS NOT THAT IT FAILED, IT IS THAT IT FAILED
                  # CONDITIONALLY: a seat whose shell happened to export S4E_HOME graded its own tree and closed the
                  # row, and a seat without it got "No such file or directory" on the identical criterion and identical
                  # tree. rtx29-standdown closed exactly that way. A verdict that depends on the grader's environment
                  # is not a verdict, and it is invisible from either side -- the seat that closes sees nothing wrong,
                  # and the seat that is refused blames its own work. Exported, every criterion resolves against the
                  # LOCKING SEAT'S OWN ROOT, deterministically, for everyone. ⛔ Deliberately NOT exported into the
                  # vacuity probe above: that one must run starved in an empty dir, and handing it a real root is
                  # precisely what would let a vacuous criterion find something to pass on. (Moot today -- the probe
                  # skips anything containing '$' -- but the reason it stays that way belongs written down.)
                  if ( cd "$S4E" && S4E_HOME="$S4E" S4E_SEAT="$ME" timeout "${S4E_DONE_TIMEOUT:-900}" bash -c "$dw" ) >"$_dwlog" 2>&1; then
                    rm -f "$_dwlog"
                    printf '  ✅ DONE-WHEN exited 0 — completion is COMPUTED, not claimed.\n'
                  else
                    rc=$?
                    printf '\n⛔⛔⛔ NOT DONE — the task DONE-WHEN exited %s. The claim is UNCHANGED and the row stays open.\n' "$rc" >&2
                    printf '    command : %s\n' "$dw" >&2
                    printf '    task    : %s\n' "$tf" >&2
                    if [ -s "$_dwlog" ]; then
                      printf '    ⭐ WHAT THE CRITERION ITSELF SAID (last 20 lines) -- read this BEFORE hypothesising:\n' >&2
                      sed -e 's/^/    | /' "$_dwlog" | tail -20 >&2
                    else
                      printf '    ⚠ the criterion produced NO output at all -- that is itself a clue: a silent non-zero is\n' >&2
                      printf '      usually a test that never ran (missing file, bad path, rc=127) rather than one that failed.\n' >&2
                    fi
                    rm -f "$_dwlog"
                    printf '    If the DONE-WHEN itself is WRONG, that is a real finding: fix it in the task file and say so\n' >&2
                    printf '    in the LEDGER, or re-run with S4E_DONE_OVERRIDE="why" which records the reason in the claim.\n' >&2
                    printf '    ⛔ Do NOT weaken a DONE-WHEN to make it pass -- that is the false-green trap this gate exists to stop.\n\n' >&2
                    exit 1; fi; fi
              else
                printf '⚠ NO TASK BATON at %s — closing on the seat word alone, which LAW 1 forbids for a baton-backed row.\n' "$tf"
                printf '  This is the v1 path and survives only while rows are still being converted (V2-2). Mint a baton for this topic.\n'
              fi
              grep -q '^DONE$' "$c" || echo DONE >> "$c"; echo "done $topic"
              s4e_mark_row "$topic" DONE
              [ "${S4E_NO_BANNER:-0}" = "1" ] || "$0" banner "$topic" "${3:-}"
         else echo "not your claim"; exit 1; fi;;
  assign) # ⭐ V2-1 / LAW 2 — ASSIGNMENT IS THE LOCK (ARCH-FLEET-CEO.md). HQ writes the seat's claim ATOMICALLY on HQ's
         # side, which makes the v1 dispatch race UNREPRESENTABLE: v1 mailed a brief AND let the seat run `next`, so two
         # channels answered "what am I working on" with nothing arbitrating -- that race is what killed seat13's session
         # (it held five rows, worked a sixth, and starved). There is now exactly one answer and it is a file on disk.
         seat="${2:?seat}"; topic="${3:?topic}"; q="$PO/QUEUE.tsv"; mkdir -p "$PO/claims"
         case "$seat"  in ""|*/*|*$'\n'*) echo "⛔ REFUSED: seat must be a filename-safe mailbox name (seat07, hq_C, ...)" >&2; exit 2;; esac
         case "$topic" in ""|*/*|*$'\n'*) echo "⛔ REFUSED: topic must be a filename-safe slug, not the message body" >&2; exit 2;; esac
         # ⛔ LAW 6 IDENTITY IS ASSERTED, NEVER GLOBBED: refuse a seat with no mailbox, and NEVER create one on the fly --
         # auto-creation is exactly how phantom claude01/ was born, and it marooned two HQ messages where nobody looked.
         [ -d "$PO/$seat/inbox" ] || { printf '⛔ REFUSED: seat "%s" has NO postoffice mailbox (%s). assign never creates one (LAW 6).\n   mailboxes that exist: %s\n' "$seat" "$PO/$seat/inbox" "$(cd "$PO" && ls -d */inbox 2>/dev/null | sed 's|/inbox||' | tr '\n' ' ')" >&2; exit 2; }
         # ⛔ AND REFUSE WORK THAT DOES NOT EXIST OR IS ALREADY LANDED -- v1 re-dispatched finished rows because nothing checked.
         row="$(grep -P "^[0-9]+\t\Q$topic\E\t" "$q" 2>/dev/null | head -1)"
         [ -n "$row" ] || { echo "⛔ REFUSED: no QUEUE.tsv row named '$topic'. You cannot assign work that has no row." >&2; exit 2; }
         c="$PO/claims/$topic.claim"
         if [ -f "$c" ]; then own="$(head -1 "$c")"
           if grep -q '^DONE$' "$c"; then echo "⛔ REFUSED: '$topic' is already DONE (held by $own). Sweep it to QUEUE.done.tsv — do NOT re-dispatch landed work." >&2; exit 1; fi
           if [ "$own" = "$seat" ]; then echo "already assigned: $topic -> $seat"; exit 0; fi
           echo "⛔ REFUSED: '$topic' is held by $own, not $seat. Release that claim first (that is a deliberate act, not a retry)." >&2; exit 1; fi
         t="$(mktemp "$PO/claims/.c.XXXXXX")"; { echo "$seat"; echo "ASSIGNED-BY $ME $(date -u +%FT%TZ)"; } > "$t"
         if ln "$t" "$c" 2>/dev/null; then rm -f "$t"; else rm -f "$t"; echo "⛔ RACE LOST: $(head -1 "$c" 2>/dev/null) owns '$topic'" >&2; exit 1; fi
         # THE DOORBELL CARRIES NO CONTENT (ARCH-FLEET-CEO: "the mail never carries content that isn't also in a file").
         # A seat that never reads this message still resumes correctly, because the claim + task file are authoritative.
         if S4E_NO_BANNER=1 "$0" send "$seat" "task-$topic" "ASSIGNED: $topic. Run: bash SCRIP/scripts/s4e_msg.sh next — it serves this row FIRST, ahead of anything you picked yourself. The task file is authoritative; this message is only the doorbell." >/dev/null 2>&1
         then echo "assigned $topic -> $seat (claim written, doorbell sent)"
         else echo "assigned $topic -> $seat (claim written; ⛔ DOORBELL NOT SENT — the claim still governs, $seat gets it from next)"; fi;;
  next)  q="$PO/QUEUE.tsv"; mkdir -p "$PO/claims"
         s4e_mode_line
         # ⛔⭐ s265 — A STALE CLONE SILENTLY REVERTS TO PRE-V2 DISPATCH, AND THAT IS NOW A REFUSAL, NOT A WARNING.
         # Measured the same day by TWO seats: seat09's clone was 79 commits behind and seat13's was 2, so both ran
         # v1's flat file-order picker — no rank sort, no assign-awareness. seat09 locked a rank-1 row while its own
         # HQ assignment sat unserved; seat13 locked rank 4 while ~30 rank-0 rows were FREE. Neither got any warning,
         # because the V2 fix only ever protected a seat whose clone ALREADY had it. ⛔ The check cannot be "git fetch
         # and compare" — a seat that never pulled has a stale origin/main too, so that test passes precisely when it
         # must fail. So the authority is the SHARED postoffice, which no clone can be behind: PROTOCOL-VERSION there
         # is the required protocol, S4E_PROTO below is this script's own, and next() REFUSES rather than dispatching
         # from a picker it cannot vouch for. A seat that mis-locks a row removes it from the whole fleet's reach.
         S4E_PROTO=3
         if [ -f "$PO/PROTOCOL-VERSION" ]; then
           need="$(head -1 "$PO/PROTOCOL-VERSION" | tr -cd '0-9')"; need="${need:-0}"
           if [ "$S4E_PROTO" -lt "$need" ]; then
             echo "⛔ REFUSING TO DISPATCH — your s4e_msg.sh is PROTOCOL $S4E_PROTO, the fleet is on $need."
             echo "   Your clone is behind origin and would run the OLD picker: no rank sort, no assign-awareness."
             echo "   That mis-locks a row and HIDES IT FROM THE WHOLE FLEET (a claimed row leaves the picker)."
             echo "   Fix, then re-run:  cd \"$S4E/SCRIP\" && git pull --rebase origin main"
             exit 3; fi; fi
         # ⭐ V2-1 SERVE ORDER (LAW 2): my-ASSIGNED  ->  my-unfinished  ->  RANK-SORTED free.
         # v1 served the topmost row in FILE ORDER, which made rank decorative: 53 rank-0 rows sat buried behind lesser
         # ones, and the deliberately fenced rank-99 M1 gate row was one claim away from being served as ordinary work.
         # ONE matcher for BOTH the presence test and the brief print, so they cannot disagree (that disagreement IS a bug
         # this row fixes). Row absent => the claim is an ORPHAN: renamed or retired out from under its holder (HQ LAW 14).
         # Skip it, SAY WHY, keep the claim (it is the record), fall through to live work. ORPHAN-SKIP IS DISARMED WHEN THE
         # QUEUE ITSELF IS DEAD: a missing/empty QUEUE.tsv would make EVERY claim look orphaned and unpin the whole fleet
         # at once -- an infrastructure failure is not a rename, so we pin and say so.
         qrows="$(grep -cP '^[0-9]+\t' "$q" 2>/dev/null)"; qrows="${qrows:-0}"   # qrow/qrank are top-level now, shared with banner()
         # ONE printer for every serve path, so a resumed row and a freshly locked row can never describe themselves differently.
         serve() { local st="$1" verb="$2" sfx="${3:-}" srow; srow="$(qrow "$st")"; printf '%s %s %s\n' "$verb" "$st" "$sfx"
           if [ -f "$PO/tasks/$st.task.md" ]; then printf 'task: %s\n' "$PO/tasks/$st.task.md"
             printf '      ⭐ THE BATON IS THE TASK FILE, NOT THIS PRINTOUT — read GOAL + DONE-WHEN + the ONE ## NEXT block,\n'
             printf '      work THAT, then rewrite ## NEXT before you stop. Questions go in ## QA, receipts in ## LEDGER.\n'; fi
           # ⭐ V2-2: QUEUE.tsv is an INDEX (rank·topic·owner·state), not a brief store. Fields 3 and 4 are
           # owner and state now, so printing them as "brief:"/"first:" would announce "brief: unassigned".
           # The baton is the content; the index only says who owns it and what state it is in.
           printf '%s\n' "$srow" | awk -F'\t' 'NF>3{print "owner: " $3 "   state: " $4}
                                                  NF>1&&NF<4{print "brief: " $3; print "first: " $4}'
           [ -f "$PO/tasks/$st.task.md" ] || printf '⛔ NO BATON at %s/tasks/%s.task.md — under V2-2 every live row must have one. Tell your HQ; do not invent the work.\n' "$PO" "$st"; }
         # PASS 1 -- rows an HQ ASSIGNED to me that I have not started. These outrank anything I picked for myself.
         # ⭐ RANK-SORTED even among MY OWN claims (fix-dispatch-bus-two-failure-modes, s266, seat07's
         # q-s4e-msg-banner-attribution-undercount): this used to be a bare glob loop, so a seat holding TWO
         # assigned-not-yet-running claims was served whichever topic sorted first ALPHABETICALLY, not the
         # fleet's own rank priority. Same defect shape V2-1 already fixed for Pass 3 (free rows); it had
         # just never been applied to "mine" too. Candidates are gathered first, THEN sorted by qrank, THEN
         # served -- one lowest-rank winner, deterministic regardless of claims/ directory order.
         while IFS=$'\t' read -r _rk t; do
           [ -n "${t:-}" ] || continue
           c="$PO/claims/$t.claim"; echo "RUNNING" >> "$c"
           serve "$t" "ASSIGNED->RUNNING" "(dispatched by $(grep -m1 '^ASSIGNED-BY ' "$c" | cut -d' ' -f2))"; exit 0
         done < <(for c in "$PO"/claims/*.claim; do [ -f "$c" ] || continue
             [ "$(head -1 "$c")" = "$ME" ] || continue
             grep -q '^DONE$' "$c" && continue
             grep -q '^ASSIGNED-BY ' "$c" || continue
             grep -q '^RUNNING$' "$c" && continue
             t="$(basename "$c" .claim)"; printf '%s\t%s\n' "$(qrank "$t")" "$t"
           done | sort -t$'\t' -s -k1,1n)
         # PASS 2 -- my own unfinished work, same rank-sort fix, same reason.
         while IFS=$'\t' read -r _rk t; do
           [ -n "${t:-}" ] || continue
           c="$PO/claims/$t.claim"; row="$(qrow "$t")"
           if [ -z "$row" ] && [ "$qrows" -gt 0 ]; then
             echo "SKIP $t — your unfinished claim names a topic with NO QUEUE.tsv row (renamed or retired out from under you). Claim KEPT as the record; falling through to live work. If you were mid-rung on it: s4e_msg.sh ask $t 'row vanished under me'"
             continue; fi
           if [ -z "$row" ]; then
             echo "⛔ $q IS MISSING OR HAS NO ROWS — cannot tell a renamed row from an unreadable queue, so RESUMING $t rather than unpinning you. Ask hq before trusting any next(1) verdict."; fi
           grep -q '^RUNNING$' "$c" || echo "RUNNING" >> "$c"
           serve "$t" "RESUME" "(yours, unfinished — s4e_msg.sh done $t when the handoff clause is met)"; exit 0
         done < <(for c in "$PO"/claims/*.claim; do [ -f "$c" ] || continue
             [ "$(head -1 "$c")" = "$ME" ] || continue
             grep -q '^DONE$' "$c" && continue
             t="$(basename "$c" .claim)"; printf '%s\t%s\n' "$(qrank "$t")" "$t"
           done | sort -t$'\t' -s -k1,1n)
         [ -f "$q" ] || { echo "no QUEUE.tsv — ask hq"; exit 1; }
         # PASS 3 -- free rows, RANK-SORTED numerically, -s so file order still breaks ties inside one rank.
         while IFS=$'\t' read -r rank topic brief step; do
           case "$rank" in ''|\#*) continue;; esac
           # ⛔ s265 — THE STATE COLUMN IS LOAD-BEARING NOW. It was decorative (94 of 94 rows FREE, nothing read it),
           # which is how `161-o2-red` — PARKED BY LON at s258, and saying so in its own baton — kept being served as
           # the fleet's TOPMOST rank-0 row. A ruling that only a human enforces is not a ruling. Anything not FREE is
           # not work: PARKED, BLOCKED, DONE all skip here.
           # ⭐ picker-skips-blocked-rows (ceo, 2026-08-28): a BLOCKED-ON:<topic>/PARKED-AWAITING:<topic> row used
           # to skip HERE FOREVER, even the session AFTER its named blocker landed DONE, because nothing ever asked
           # again — bench-rivals-raku-pascal sat encoded as plain "FREE" (nobody had even written the block down)
           # through 9+ seats independently re-discovering the identical block, which is the OTHER half of this same
           # defect: a block a human found but never RECORDED is invisible to a check that only reads the column.
           # Self-heals: a resolved dependency lets next() skip past a stale BLOCKED-ON state instead of honoring
           # it forever — re-ask whether the named blocker is done EVERY time this row is considered, and if so
           # `park` the row back to FREE right here (ledger line included) before falling through to the check below.
           case "$step" in
             BLOCKED-ON:*|PARKED-AWAITING:*)
               blk="${step#*:}"
               if s4e_blocker_done "$blk" && "$0" park "$topic" FREE >/dev/null 2>&1; then step=FREE; fi ;;
           esac
           case "$step" in FREE|'') : ;; *) continue;; esac
           [ -f "$PO/claims/$topic.claim" ] && continue
           if "$0" claim "$topic" >/dev/null 2>&1; then
             echo "RUNNING" >> "$PO/claims/$topic.claim"
             serve "$topic" "LOCKED" "(rank $rank)"; exit 0; fi
         done < <(grep -P '^[0-9]+\t' "$q" | sort -t$'\t' -s -k1,1n)
         echo "QUEUE EMPTY — every row claimed. Ask hq: s4e_msg.sh ask work 'queue empty'"; exit 1;;
  banner) # ⛔ FACTS ONLY -- NO PREDICTIONS (Lon 2026-08-22: "Why are you trying to predict the future. Quit saying
         # in the banner what you will do. You do not know the future."). Every line below is a measured fact about
         # state as it stands. What a later session does is not knowable here: HQ can re-rank the queue, and THE LOOP
         # reads the inbox before the queue. Two laws still hold: the verdict is COMPUTED, never typed, and it turns
         # on ONE question -- did the work land and get pushed (handoff_status.sh rc, the only sanctioned source).
         # An open question to HQ is HQ's backlog, never this seat's failure.
         s4e_mode_line
         # ⭐ ceo-only, WARN-only (Lon trust-audit 2026-08-28: health checks must be HARNESS-FIRED, not remembered —
         # the same defect class as the old hand-fired banner). Fast (<1s), reads the postoffice, never flips the
         # verdict; rc=2 from the census prints as its own refusal line rather than a silent green.
         if [ "$ME" = "ceo" ] && [ -x "$S4E/SCRIP/scripts/util_queue_visibility_census.py" ]; then
             _cns="$(python3 "$S4E/SCRIP/scripts/util_queue_visibility_census.py" 2>&1 | tail -1)"
             printf 'QUEUE CENSUS (computed): %s\n' "$_cns"
         fi
         hs="$S4E/SCRIP/scripts/handoff_status.sh"
         # ⛔ SKIP_S_ARTIFACT_CHECK=1 (ceo 2026-08-27, Lon's "go fix it" — the missing-banners defect, seat09/hq_P diagnosis):
         # the .s-artifact verifier runs `make pristine` (minutes; races and WIPES live builds), so every banner that reached
         # it blew the Stop hook's 100s timeout and Lon saw NO banner — precisely on SUCCESS. The check is WARN-ONLY by design
         # (cannot flip the verdict), so skipping it here changes nothing the banner asserts. The FULL check still runs on a
         # direct handoff_status.sh invocation — the real handoff verdict, where HQ-27 wants the pristine.
         if [ -f "$hs" ]; then hout="$(SKIP_S_ARTIFACT_CHECK=1 timeout 300 bash "$hs" 2>&1)"; hrc=$?; else hout="handoff_status.sh NOT FOUND at $hs"; hrc=2; fi
         # ⭐ fix-dispatch-bus-two-failure-modes (s266, seat07's q-s4e-msg-banner-attribution-undercount):
         # `held` used to be "whichever of my OPEN claims sorts first ALPHABETICALLY" -- a seat holding two
         # open claims could run `done <topic>` to close ONE and have its OWN banner report the OTHER.
         # `done` already passes the topic it just verified as $2 (see the `done` arm below); when banner
         # gets a real topic (not "-v", the direct-invocation verbose flag) THAT topic wins outright, no
         # scan needed. A bare invocation (Stop hook, `board`) falls back to the same rank-sort next()
         # uses, never glob order.
         pref="${2:-}"; [ "$pref" = "-v" ] && pref=""
         held=""
         if [ -n "$pref" ] && [ -f "$PO/claims/$pref.claim" ] && [ "$(head -1 "$PO/claims/$pref.claim")" = "$ME" ] && ! grep -q '^DONE$' "$PO/claims/$pref.claim"; then
           held="$pref"
         else
           held="$(for c in "$PO"/claims/*.claim; do [ -f "$c" ] || continue
               [ "$(head -1 "$c")" = "$ME" ] || continue
               grep -q '^DONE$' "$c" && continue
               t="$(basename "$c" .claim)"; printf '%s\t%s\n' "$(qrank "$t")" "$t"
             done | sort -t$'\t' -s -k1,1n | head -1 | cut -f2)"
         fi
         qwait=0; for hb in $(s4e_hqboxes); do for f in "$PO/$hb"/inbox/*.msg; do [ -f "$f" ] || continue; case "$(basename "$f")" in *-"$ME"-q-*) qwait=$((qwait+1));; esac; done; done
         inbx=0; for f in "$PO/$ME/inbox"/*.msg; do [ -f "$f" ] && inbx=$((inbx+1)); done
         # ⛔⭐ V2-3 -- DRAIN BEFORE MINT, MADE MECHANICAL (LAW 3, ARCH-FLEET-CEO.md; hq_P s258). Measured basis, from
         # the fleet-v1 retrospective: 29 messages and 15 seat questions sat unread in HQ's inbox for 1h47m while HQ
         # minted new rows; seat13 starved holding five of them; HQ's own board line was the oldest on the board. The
         # rule "answer every pending question before minting" existed and was ignored, for the same reason the banner
         # law was ignored before the Stop hook: a step in a markdown file is a hope, not a mechanism. So the HQ's own
         # verdict now refuses to say ✅ while a seat's question has been rotting. ⛔ Binds HQ ONLY -- a seat's inbox
         # is HQ talking TO it, and holding unread mail is not a seat's protocol failure.
         staleage="$(s4e_oldest_min "$ME")"; stalemin="${S4E_DRAIN_MIN:-30}"; drain=0
         if s4e_is_hq "$ME" && [ -n "$staleage" ] && [ "$staleage" -gt "$stalemin" ]; then drain=1; fi
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
         # Same fix as `held` above, reusing $pref: after `done` marks a row DONE it is no longer "open",
         # so `held`'s open-only pref check would miss it -- row1/rowst must accept a CLOSED pref too, since
         # that is exactly the row `done` just closed and wants attributed here.
         # ⛔⭐ FIXED (banner-attributes-wrong-row-on-unclaim, s273): the OLD else-branch rescanned EVERY
         # claim ever held by $ME, open or DONE, and sorted by (done-flag, rank) -- so a session with ZERO
         # open claims (the exact shape `unclaim` leaves behind) fell through to "whichever ancient DONE
         # claim under this identity sorts first", attributing a stale prior session's row (and, via the
         # cmts/fnd --grep="$row1" below, sometimes its commit/FINDING counts too) to a session that never
         # touched it. Measured 4 independent times (seat08/seat06/seat02 LEDGER, this bundle's own repro).
         # THE FIX: never guess across sessions. $held (computed above) is ALREADY the correct "my lowest-
         # rank OPEN claim, DONE claims excluded" answer -- reuse it outright instead of re-deriving a
         # DONE-admitting variant. When there is no open claim (unclaim left none, or nothing was ever
         # claimed this session), fall back to the SESSION-SCOPED receipt `s4e_mark_row` writes at the
         # moment `unclaim`/`done` actually close a row -- never to claims/*.claim history at large.
         rowst="none"; row1=""; rowmark_ts=""
         if [ -n "$pref" ] && [ -f "$PO/claims/$pref.claim" ] && [ "$(head -1 "$PO/claims/$pref.claim")" = "$ME" ]; then
           row1="$pref"; grep -q '^DONE$' "$PO/claims/$pref.claim" && rowst="CLOSED" || rowst="OPEN"
         elif [ -n "$held" ]; then
           row1="$held"; rowst="OPEN"
         elif [ -s "$PO/$ME/.last-row" ]; then
           row1="$(sed -n 1p "$PO/$ME/.last-row")"; rowst="$(sed -n 2p "$PO/$ME/.last-row" | cut -d' ' -f1)"
           rowmark_ts="$(sed -n 2p "$PO/$ME/.last-row" | cut -d' ' -f2-)"
         fi
         # ⭐ SESSION-SCOPED WINDOW, WIDEN-ONLY (banner-attributes-wrong-row-on-unclaim, s273 -- SELF-
         # CORRECTED same session after this exact code produced a live false "NOTHING LANDED" on this
         # seat's own board post). The marker records WHEN THE ROW CLOSED, not when the session started --
         # using it as a `--since` floor NARROWS the window to "after I finished," which finds ZERO of the
         # commits that led UP TO the close (measured: a session with 7 real attributable commits over the
         # preceding hours read 0 the instant `since` was pinned to its own just-written close marker).
         # So the marker may only WIDEN the window (for a session that ran longer than 12h), never narrow
         # it below the flat default -- take whichever of the two is EARLIER.
         since="12 hours ago"
         if [ -n "$rowmark_ts" ]; then
           m_epoch=$(date -u -d "$rowmark_ts" +%s 2>/dev/null || echo 0)
           cut_epoch=$(date -u -d "12 hours ago" +%s 2>/dev/null || echo 0)
           if [ "$m_epoch" -gt 0 ] 2>/dev/null && [ "$m_epoch" -lt "$cut_epoch" ] 2>/dev/null; then since="$rowmark_ts"; fi
         fi
         cmts=0; for r in "$S4E"/*/; do [ -d "$r/.git" ] || continue
           n=$(git -C "$r" log --since="$since" -i --grep="$ME" ${row1:+--grep="$row1"} --oneline 2>/dev/null | wc -l); cmts=$((cmts+n)); done
         # seat8 2026-08-22: every FINDING-*.md ever written (202/202 checked) names the seat the OLD,
         # unpadded way ("seat8"), because s255's zero-padding change touched $ME fleet-wide but no seat's
         # file-naming habit. Matching $ME alone ("seat08") against the corpus finds ZERO files, always,
         # for every single-digit seat -- so a same-session FINDING silently fails attribution here too.
         mealt="${ME/#seat0/seat}"
         fnd=$(git -C "$S4E/.github" log --since="$since" --diff-filter=A --name-only --format= 2>/dev/null | grep '^FINDING-' | grep -ci -e "$ME" -e "$mealt" ${row1:+-e "$row1"} || true); fnd="${fnd:-0}"
         if [ "$cmts" -eq 0 ] && [ "$fnd" -eq 0 ]; then lvl="⚠ NOTHING ATTRIBUTABLE LANDED"
         else lvl="row ${rowst}${row1:+ ${row1}} · ${cmts} commit(s) · ${fnd} FINDING(s), attributed since ${since}"; fi
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
         # rc is deliberately NOT changed by the drain refusal, for the same reason NOTHING LANDED did not change it:
         # rc answers "is it safe to /clear", and unread mail is safe to /clear -- it is on disk and waits. What the
         # drain law governs is the VERDICT LON READS, and that is this line.
         elif [ "$drain" -eq 1 ]; then line="⛔ DRAIN FIRST — $ME — ${inbx} unread, oldest ${staleage}m (limit ${stalemin}m). LAW 3: answer every pending question into its task file BEFORE minting or assigning. No ✅ until the inbox is current — $lvl"
         # ⛔⛔ s255, LON: "I never stopped a FLEET worker whose banner did not say SUCCESS after I prompted 'show me
         # the required banner.' So they lied."  THE SEATS DID NOT LIE -- THIS HEADLINE ANSWERED THE WRONG QUESTION.
         # SUCCESS was emitted on handoff_status rc=0, i.e. "tree clean, nothing unpushed" -- which A SEAT THAT DID
         # ABSOLUTELY NOTHING SATISFIES TRIVIALLY. Doing nothing was indistinguishable from doing everything. Proof
         # on the day: two freshly-created empty seats printed "✅ SUCCESS", and seat6 -- which released its row
         # having touched no code -- would have too. The code ALREADY computed the honest fact one screen above
         # ($lvl = "NOTHING ATTRIBUTABLE LANDED" when cmts==0 && fnd==0) and then appended it as DECORATION to a
         # SUCCESS verdict. The wrong one was in the headline. Now: nothing landed => the headline says so.
         # rc is deliberately UNCHANGED -- it still answers "safe to /clear", which is a different question and the
         # one tooling consumes. The banner's TEXT is what Lon reads, and it is now the one that must be earned.
         elif [ "$cmts" -eq 0 ] && [ "$fnd" -eq 0 ]; then line="⚠ NOTHING LANDED — $ME — tree is clean and safe to /clear, but this session produced NO commit and NO FINDING. That is not success; it is an empty session."
         elif [ "$onlyhere" -eq 0 ] && [ "$hrc" -ne 0 ]; then line="✅ SUCCESS — $ME — safe to /clear (behind origin, nothing unpushed) — $lvl"
         elif [ "$hrc" -eq 0 ]; then line="✅ SUCCESS — $ME — safe to /clear — $lvl"
         else                        line="⛔ FAILURE — $ME — do NOT /clear — $lvl — $(printf '%s' "$pline" | sed 's/^ *-* *//')"; fi
         # ⭐ V2-3, second half: the BOARD LINE carries the oldest-unanswered age and the row topic. BOARD.md is what
         # `fleet` renders and what Lon reads when he is not reading a banner, and in v1 it could not show either --
         # so an HQ sitting on a 1h47m question looked exactly like an HQ with an empty inbox.
         # ⛔ BOTH CLAUSES ARE UNCONDITIONAL, and that is the fix for a defect this gate caught in its own first
         # green run: the row topic was riding inside $lvl, and $lvl COLLAPSES to "NOTHING ATTRIBUTABLE LANDED"
         # whenever a session has no attributable commit -- so precisely the sessions worth chasing (a seat holding
         # a row and producing nothing) were the ones whose board line refused to name the row. A field that
         # disappears exactly when it matters is a blind instrument (LAW 0, species 3).
         [ -z "${row1:-}" ]   || line="$line · row ${row1}"
         [ -z "$staleage" ]   || line="$line · mail ${inbx}/${staleage}m"
         printf '\n%s\n  %s\n%s\n\n' "$b" "$line" "$b"
         # ⛔⛔ THE BANNER WAS FIRING AND NOBODY COULD SEE IT (Lon 2026-08-22 s256: "The FLEET workers are not showing
         # a banner at the end. claude08 just sat silent like an idiot").  MEASURED, from seat08's OWN transcript --
         # two stop_hook_summary records, 3867ms and 4166ms, "hookErrors": [], "hasOutput": true.  The hook fires and
         # SUCCEEDS every time.  The field that decides whether a human ever reads it is "level": "suggestion": the
         # s255 Stop hook wraps this output in {"systemMessage": ...}, which the client files as an ADVISORY HINT, not
         # as transcript output.  The banner has been landing in a drawer since the day it was automated.
         # ⭐ CURE, and it is LAW 18 one level further out: a status that exists only in a display channel we cannot
         # verify is not a status.  PERSIST IT.  The verdict is already computed one line above; write it to BOARD.md
         # so `fleet`, `board`, and Lon reading the file all see the same COMPUTED line whatever the client does with
         # the hint.  Suppressed when `board` is the caller, so a seat's typed status is never clobbered by this.
         if [ "${S4E_BANNER_NO_BOARD:-0}" != "1" ] && [ -d "$PO" ]; then
           grep -v "^$ME |" "$PO/BOARD.md" 2>/dev/null > "$PO/.b.$$" || true
           printf '%s | %s | %s\n' "$ME" "$line" "$(date -u +%H:%M)" >> "$PO/.b.$$"
           mv "$PO/.b.$$" "$PO/BOARD.md" 2>/dev/null || rm -f "$PO/.b.$$"; fi
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
         # ⛔⭐ THE LOCK-AGE COLUMN IS ANNOTATED IN ITS OWN HEADER, NOT IN A NOTE BESIDE IT (hq_P, 2026-08-27).
         # A caveat that lives next to a field is read by whoever already knows -- and the seat in a hurry, who is
         # exactly who misreads it, never gets there. So the trap is spelled INSIDE the column name: this number is
         # when the LOCK was taken. It is NOT a work signal. A seat that claimed a row and walked away and a seat
         # mid-cure print the IDENTICAL number, and nothing in the field can tell you which you are looking at.
         # ⭐ THE FIX FOR A FIELD THAT ANSWERS A NARROWER QUESTION IS A SECOND FIELD BESIDE IT, NOT A BETTER NAME
         # FOR THE FIRST (hq_P's shape, adopted verbatim): COMMITS-SINCE sits immediately to its right and measures
         # what LOCK AGE cannot -- work actually attributed to that seat or row since the moment it took the lock.
         # Read together they separate the two states; read alone, LOCK AGE reports a stalled row as a busy one,
         # which is precisely how two rows sat 115m and 83m with zero output while the fleet screen looked normal.
         # ⭐ The sub-head is part of the COLUMN, and every cell in it is exactly 10 display columns wide so the
         # annotation sits under the field it annotates rather than drifting one column left of it. ⛔⭐ NO EMOJI IN
         # AN ALIGNED CELL, AND NO FIELD WIDTH CAN RESCUE ONE -- MEASURED, hq_C 2026-08-27, correcting this
         # comment's own earlier mechanism. The cause is not padding, it is that bash truncates `%.Ns` by BYTES
         # while the terminal aligns by COLUMNS, and "⛔" is 1 char / 3 bytes / 2 columns -- three different
         # numbers, which is the whole trap. "⛔ 1 dirty 0 unpushed" is chars=20 bytes=22, so at %-20.20s it
         # rendered "⛔ 1 dirty 0 unpush": a column SHORT (following fields slide LEFT) and a WORD CHOPPED.
         # ⛔ Widening does NOT fix it, tested: %-21.21s still chops ("...unpushe"), and %-22.22s finally fits
         # the text but still paints 21 columns, not 22, because the 3 bytes only ever buy 2 columns. The gap is
         # unclosable by width -- DELETE THE EMOJI, which is what TREE now does (see its cell, fixed).
         printf '\n  SEAT     ROW (open claim)                LOCK AGE    COMMITS     TREE                  Q  MAIL      LAST BOARD LINE\n'
         printf '  %-8s %-30s  %-10s  %-10s  %s\n' "" "" "lock only" "real work" ""
         printf '  ──────── ──────────────────────────────  ──────────  ──────────  ────────────────────  ─  ────────  ─────────────────────\n'
         # ⛔ SEAT LIST IS DISCOVERED, NEVER TYPED (s255): a hand-typed list silently omits any seat Lon adds,
         # and a seat missing from the health screen is a seat nobody looks at. Numeric sort so 10 follows 9.
         # ⛔⭐ THE CENSUS ENUMERATES POSTOFFICE MAILBOXES, NOT /home/claude* GLOBS (LAW 6, V2-4). The glob it
         # replaced could not see hq_C, hq_P or ceo AT ALL -- the two HQs running the fleet were invisible on the
         # fleet's own health screen, and so was every message waiting in their inboxes. It also silently omitted
         # any mailbox whose seat root is not yet cloned, which is the state every new seat starts in. The
         # postoffice IS the fleet roster; the home directories are just where the clones happen to live.
         for seat in $(s4e_boxes | sort); do
           root="$(s4e_root "$seat")"
           row="-"; lockep=0; for c in "$PO"/claims/*.claim; do [ -f "$c" ] || continue
             if [ "$(head -1 "$c")" = "$seat" ] && ! grep -q '^DONE$' "$c"; then row="$(basename "$c" .claim)"
               # ⛔ THE CLAIM FILE'S MTIME IS THE LOCK-ACQUISITION INSTANT AND NOTHING ELSE. It is not touched by
               # work, by commits, or by the seat running anything -- only by taking (or re-taking) the lock.
               lockep=$(stat -c %Y "$c" 2>/dev/null || echo 0); break; fi; done
           lockage="$(s4e_age_compact "$lockep")"
           dirty=0; unpushed=0; repos=0; csince=0
           for r in "$root"/*/; do [ -n "$root" ] || continue; [ -d "$r/.git" ] || continue; repos=$((repos+1))
             d=$(git -C "$r" status --porcelain 2>/dev/null | wc -l); dirty=$((dirty+d))
             br=$(git -C "$r" rev-parse --abbrev-ref HEAD 2>/dev/null)
             u=$(git -C "$r" rev-list --count "origin/$br..$br" 2>/dev/null || echo 0); unpushed=$((unpushed+${u:-0}))
             # ⭐ ATTRIBUTED, exactly as `banner` measures level-of-success: a bare `log --since` counts commits this
             # clone merely PULLED and would credit an idle seat with the whole fleet's output. Commit messages here
             # carry the seat id and the row topic, so attribution is the measurement. `seat8` and `seat08` are both
             # matched -- every FINDING and many commits still name seats the pre-s255 unpadded way, and matching the
             # padded form alone finds ZERO for every single-digit seat, silently.
             if [ "$lockep" -gt 0 ]; then
               salt="${seat/#seat0/seat}"
               n=$(git -C "$r" log --since="@$lockep" -i --grep="$seat" --grep="$salt" --grep="$row" --oneline 2>/dev/null | wc -l)
               csince=$((csince+${n:-0})); fi; done
           if [ -z "$root" ]; then tree="(no root, retiring)"
           elif [ "$repos" -eq 0 ]; then tree="no clones"
           elif [ "$dirty" -eq 0 ] && [ "$unpushed" -eq 0 ]; then tree="clean"
           # ⛔ NO EMOJI IN A WIDTH-CONSTRAINED CELL (hq_P 2026-08-27, fixed here by hq_C). This cell prints through
           # %-20.20s, and bash truncates by BYTES while a terminal aligns by COLUMNS. "⛔" is 1 char / 3 bytes /
           # 2 columns, so the old text measured chars=20 bytes=22: the precision chopped it to "⛔ 1 dirty 0 unpush"
           # -- ONE COLUMN SHORT (shifting Q/MAIL/BOARD left on exactly the rows that signal trouble) AND a
           # truncated word. Both the alignment and the DATA were lost, on the rows a reader most needs to read.
           # Plain ASCII here, matching the adjacent COMMITS cell's own "0 STALLED": the words are the signal.
           # Emoji stay legal in the LAST column only (bl), which is %s and constrains no width.
           else tree="${dirty} dirty ${unpushed} unpushed"; fi
           # a question can now be waiting in ANY HQ's inbox (hq is retiring, hq_C and hq_P own the two lanes),
           # so the count sums over the whole HQ set -- counting only legacy hq/ would read 0 the day it retires.
           q=0; for hb in $(s4e_hqboxes); do for f in "$PO/$hb"/inbox/*.msg; do [ -f "$f" ] || continue; case "$(basename "$f")" in *-"$seat"-q-*) q=$((q+1));; esac; done; done
           unread=0; for f in "$PO/$seat"/inbox/*.msg; do [ -f "$f" ] && unread=$((unread+1)); done
           if [ "$unread" -eq 0 ]; then mail="-"; else om="$(s4e_oldest_min "$seat")"; mail="$unread/${om:-0}m"; fi
           bl="$(grep -m1 "^$seat |" "$PO/BOARD.md" 2>/dev/null | cut -d'|' -f2- | cut -c1-40)"; [ -n "$bl" ] || bl="(never posted)"
           # ⛔ A ROW LOCKED OVER AN HOUR WITH NOTHING ATTRIBUTED IS THE STATE THIS COLUMN EXISTS TO SURFACE, so it is
           # marked rather than left for the reader to compute across two fields. Under an hour it stays quiet: a
           # freshly-claimed row legitimately has no commits yet, and flagging that would train the eye to ignore it.
           if [ "$lockep" -eq 0 ]; then csh="-"
           elif [ "$csince" -eq 0 ] && [ "$(( $(date +%s) - lockep ))" -gt 3600 ]; then csh="0 STALLED"
           else csh="$csince"; fi
           printf '  %-8s %-30.30s  %-10.10s  %-10.10s  %-20.20s  %s  %-8.8s  %s\n' "$seat" "$row" "$lockage" "$csh" "$tree" "$q" "$mail" "$bl"; done
         free=0; tot=0
         while IFS=$'\t' read -r rank topic brief step; do case "$rank" in ''|\#*) continue;; esac
           tot=$((tot+1)); [ -f "$PO/claims/$topic.claim" ] || free=$((free+1)); done < "$PO/QUEUE.tsv" 2>/dev/null
         printf '\n  queue: %s rows, %s free for the picker (a row with ANY claim file, DONE or not, is hidden)\n' "$tot" "$free"
         printf '  Q = questions from that seat waiting on ANY HQ.  MAIL = unread in its inbox / age of the oldest.\n'
         printf '  ⛔ LOCK AGE = how long the CLAIM FILE has existed = when the lock was TAKEN. It is NOT a work signal:\n'
         printf '     a seat that claimed a row and stalled prints the same number as a seat mid-cure. COMMITS SINCE LOCK\n'
         printf '     is the field that measures work (attributed to the seat or its row, across that root every repo).\n'
         printf '     Read the pair. "0 STALLED" = locked over an hour, nothing attributed -- a claimed row hides itself\n'
         printf '     from the picker, so a stalled lock blocks that row for the WHOLE fleet until an HQ releases it.\n'
         printf '  Roster is the postoffice mailbox list, never a home-dir glob -- the glob could not see the HQs.\n\n';;
  sweep) # ⭐ LAW 4 — THE QUEUE IS A DISPATCH BUFFER, NOT A MEMORY. v1 reached 62% dead rows (112 of 181 DONE) because
         # nothing ever moved a landed row out, so the picker walked a graveyard and HQ re-dispatched finished work.
         # This moves every row whose claim carries DONE into QUEUE.done.tsv (the memory) and rewrites the buffer.
         # ⛔ IT NEVER DELETES: every swept row is appended to QUEUE.done.tsv and the pre-sweep buffer is backed up.
         q="$PO/QUEUE.tsv"; d="$PO/QUEUE.done.tsv"; [ -f "$q" ] || { echo "no QUEUE.tsv"; exit 1; }
         cp "$q" "$q.bak.sweep-$(date -u +%Y%m%dT%H%M%SZ)"
         keep="$(mktemp)"; gone="$(mktemp)"; nk=0; ng=0
         while IFS= read -r line; do
           case "$line" in \#*) printf '%s\n' "$line" >> "$keep"; continue;; '') continue;; esac
           topic="$(printf '%s' "$line" | cut -f2)"
           if [ -f "$PO/claims/$topic.claim" ] && grep -q '^DONE$' "$PO/claims/$topic.claim"; then printf '%s\n' "$line" >> "$gone"; ng=$((ng+1))
           else printf '%s\n' "$line" >> "$keep"; nk=$((nk+1)); fi
         done < "$q"
         if [ "$ng" -gt 0 ]; then
           [ -s "$d" ] || printf '# S4E QUEUE — LANDED ROWS (the MEMORY; QUEUE.tsv is the BUFFER). Append-only.\n# rank\ttopic\tbrief\tfirst-step-and-done-when\n' > "$d"
           printf '# --- swept %s by %s: %s rows ---\n' "$(date -u +%FT%TZ)" "$ME" "$ng" >> "$d"; cat "$gone" >> "$d"
           mv "$keep" "$q"; chmod 664 "$q"; else rm -f "$keep"; fi
         rm -f "$gone"; printf 'sweep: %s live rows kept, %s DONE rows moved to QUEUE.done.tsv (nothing deleted; buffer backed up)\n' "$nk" "$ng";;
  board) if [ $# -gt 1 ]; then shift; grep -v "^$ME |" "$PO/BOARD.md" 2>/dev/null > "$PO/.b.$$" || true; printf '%s | %s | %s\n' "$ME" "$*" "$(date -u +%H:%M)" >> "$PO/.b.$$"; mv "$PO/.b.$$" "$PO/BOARD.md"; fi; cat "$PO/BOARD.md"
         # posting a board line IS the handoff gesture -- so the banner fires here too (see `done` above).
         [ "${S4E_NO_BANNER:-0}" = "1" ] || S4E_BANNER_NO_BOARD=1 "$0" banner;;
  *) echo "usage: next|done|ask|send|check|clear|claim|assign|sweep|board|banner|fleet|mailbox"; exit 2;;
esac
