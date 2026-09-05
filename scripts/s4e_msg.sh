#!/usr/bin/env bash
# s4e_msg.sh — the postoffice helper (/home/resources/postoffice/PROTOCOL.md is law). Usage:
#   s4e_msg.sh next                       ONE-SHOT PICK-AND-LOCK: resume your own unfinished
#                                         claim, else atomically lock the topmost free QUEUE.tsv
#                                         row and print its brief pointer + first step
#   s4e_msg.sh done <topic>               ⭐ VERIFIES the task baton's DONE-WHEN and REFUSES if it fails
#                                         (LAW 1: done is COMPUTED, never declared). Override, loudly and
#                                         recorded: S4E_DONE_OVERRIDE="why". Claims persist as done-markers.
#                                         Budget: S4E_DONE_TIMEOUT=<seconds>, default 3600. A criterion that
#                                         does not finish REFUSES rc=2 (could not measure) -- distinct from
#                                         rc=1 (measured, red). Raising the budget never weakens the check.
#   s4e_msg.sh ask <topic> "text"         question box: sends to hq as q-<topic>
#   s4e_msg.sh send <to> <topic> "text"   s4e_msg.sh check   s4e_msg.sh clear
#   s4e_msg.sh claim <topic>              s4e_msg.sh board [my new status text]
#   s4e_msg.sh sweep                      ⭐ LAW 4: move landed (DONE) rows out of QUEUE.tsv into
#                                         QUEUE.done.tsv — the buffer must never become a graveyard again
#   s4e_msg.sh assign <seat> <topic>      ⭐ V2-1/LAW 2 ASSIGNMENT IS THE LOCK: HQ writes <seat>'s claim
#                                         atomically + rings a contentless doorbell; that seat's next serves it FIRST
#   s4e_msg.sh mint <topic> [rank] "GOAL"  ⭐ THE ONLY WAY A SEAT CAN ADD NEW WORK (2026-08-29 ceo ruling):
#                                         atomically writes a QUEUE.tsv row + a skeleton task baton; refuses a
#                                         duplicate topic and a bad-topic filename the same way send does
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
    claude0[1-9]|claude1[0-9]|claude20) echo "seat${1#claude}";;
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
    else echo ""; fi; }
# ⛔⭐ A DRAINED MAILBOX REFUSES DELIVERY -- IT DOES NOT ACCEPT MAIL NOBODY WILL READ.
# A box with a DRAINED file has no reader. Sending there SUCCEEDS at every level this script can see --
# the mv works, the file is non-empty, `sent` prints -- and the message is simply never read by anyone.
# MEASURED 2026-09-03: the ceo found FOUR messages rotting in hq/inbox, including a LON OVERRIDE from
# seat07 unread for eight hours and a cured row whose owner could not close it. ⭐ This is the
# 'non-empty is not alive' false-signal class landing on the bus itself, and it is why the legacy `hq`
# fallback is gone from s4e_hq() above: with every seat carrying an HQ file, that fallback could only
# ever route mail to the one identity guaranteed not to read it.
# ⛔ The rule is DATA, not a hardcoded name: any box may be drained by writing $PO/<box>/DRAINED, and
# the refusal prints that file so the reason travels with the refusal instead of living in this script.
s4e_assert_not_drained() { [ -f "$PO/$1/DRAINED" ] || return 0
    printf '\n⛔⛔⛔ REFUSED: %s IS A DRAINED MAILBOX -- NOTHING WAS SENT ⛔⛔⛔\n' "$1" >&2
    sed 's/^/    /' "$PO/$1/DRAINED" >&2
    _t="$(s4e_hq)"
    if [ -n "$_t" ]; then printf '\n    YOUR HQ is %s, from the HQ file %s.\n' "$_t" "$PO/$ME/HQ" >&2
        printf '    Send it there:  %s send %s <topic> --stdin <<'"'"'MSG'"'"'\n' "$0" "$_t" >&2
        printf '    or let the bus resolve it for you:  %s ask <topic> --stdin <<'"'"'MSG'"'"'\n' "$0" >&2
    else printf '\n    ⛔ AND %s HAS NO HQ FILE (%s) -- an HQ must write one: echo <hq> > %s\n' "$ME" "$PO/$ME/HQ" "$PO/$ME/HQ" >&2; fi
    printf '\n' >&2; exit 2; }
if [ -z "$ME" ]; then case "$S4E" in
    /home/claude)           ME=ceo;;
    /home/claude_C)         ME=hq_C;;
    /home/claude_P)         ME=hq_P;;
    /home/claude_B)         ME=hq_B;;
    /home/claude_T)         ME=hq_T;;
    /home/claude_U)         ME=hq_U;;
    /home/claude_S)         ME=hq_S;;
    /home/claude_I)         ME=hq_I;;
    /home/claude_R)         ME=hq_R;;
    /home/claude[0-9][0-9]) ME="seat${S4E#/home/claude}";;
    /home/claude[1-9])      ME="seat0${S4E#/home/claude}";;
    *)                      ME="$(basename "$S4E")";; esac; fi
ME="$(s4e_canon "$ME")"
# release-verbs-refuse-on-unfolded-authority-mail (hq_C 2026-08-30): s4e_unfolded_authority_mail /
# s4e_release_guard_note, used by unclaim/park below. Sourced relative to this script's own
# directory, not cwd, since these subcommands are invoked from anywhere.
. "$(dirname "${BASH_SOURCE[0]}")/lib_release_guard.sh"
# ⭐ SHARED BY next() AND banner() (fix-dispatch-bus-two-failure-modes, s266). Resolve a topic's QUEUE.tsv
# rank, or a large sentinel when the row is absent/orphaned. Hoisted to top level (was a next()-local
# function) so both commands sort candidates the SAME deterministic way instead of each falling back to
# glob (alphabetical) file order whenever a seat holds more than one claim.
qrow()  { grep -P "^[0-9]+\t\Q$1\E\t" "$PO/QUEUE.tsv" 2>/dev/null | head -1; }
qrank() { local row; row="$(qrow "$1")"; [ -n "$row" ] && printf '%s' "$row" | cut -f1 || echo 999999; }
# ⭐ next-tiebreak-by-mint-time-not-file-order (hq_C finding, ceo mint 2026-09-01): a topic's mint
# timestamp, read from its own task file's "minted via `mint` by ..., <ISO-8601>" LINKS line (the
# same line `mint` itself writes). Empty when the file or the line is absent -- PASS 3's sort then
# treats "unknown mint time" as OLDEST within its rank tier (a real timestamp always outranks no
# timestamp), never as a crash or a special case.
s4e_mint_ts() { grep -m1 'minted via `mint`' "$PO/tasks/$1.task.md" 2>/dev/null | grep -oE '[0-9]{4}-[0-9]{2}-[0-9]{2}T[0-9]{2}:[0-9]{2}:[0-9]{2}Z' | head -1; }
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

# ⭐ s4e-park-adds-a-blocker-and-refuses-an-unresolvable-topic — a state may now be a '+'-joined SET of
# blockers (BLOCKED-ON:B1+B2), each an independently-plausible reason recorded by a separate park rather
# than one overwriting the other. This answers "which one, if any, is done" so the self-heal (and the
# DOES-NOT-CLEAR check, which needs the SPECIFIC blocker) can treat "any member resolves" as sound self-clear.
s4e_first_resolved_blocker() {   # <possibly '+'-joined blockers> -> prints the first resolved one, rc 1 if none
    local blk="${1:-}" b save_IFS
    [ -n "$blk" ] || return 1
    save_IFS="$IFS"; IFS='+'
    for b in $blk; do
        IFS="$save_IFS"
        if s4e_blocker_done "$b"; then printf '%s' "$b"; return 0; fi
        IFS='+'
    done
    IFS="$save_IFS"
    return 1
}

# ⭐ s4e-done-does-not-clear-annotation (ceo 2026-08-29, hq_B's "your call" ask). SELF-CLEAR (above) earns
# its keep and stays; this is the ESCAPE HATCH for the case it measurably got wrong: hq_B found
# `snocone-parser-fixture-ast-drift-ruling` self-clear a dependent while its OWN ledger and the blocker's
# both said, in bold, "closing this row must not be read as clearing it" for that one dependent (the fix
# does not reach family 5) — a counter-finding written in PROSE changed nothing, because the dispatcher
# reads only the thin QUEUE.tsv record, never the baton text. So the annotation lives where the dispatcher
# already looks: the BLOCKER's own state column, spelled `DOES-NOT-CLEAR:<dep1>[,<dep2>,...]` appended
# after its base state (e.g. `DONE:DOES-NOT-CLEAR:snocone-parser-fixture-family5`). ⛔ Deliberately NOT a
# check of `s4e_blocker_done` above — that function's own law is "presence in the claim/QUEUE.done.tsv is
# the WHOLE done-ness signal, never the state column" (the state column is stale on ~57% of landed rows).
# This function answers a DIFFERENT question (did the blocker's author name ME as an exception), which is
# the one place the state column IS the authority per ceo's ruling — the two never overlap in what they
# certify, and neither should read the other's evidence.
s4e_does_not_clear() {   # <blocker-topic> <dependent-topic> -> rc 0 iff blocker's row names dependent
    local blk="${1:-}" dep="${2:-}" st names
    [ -n "$blk" ] && [ -n "$dep" ] || return 1
    st="$(s4e_row_state "$blk")"
    case "$st" in *DOES-NOT-CLEAR:*) ;; *) return 1;; esac
    names="${st#*DOES-NOT-CLEAR:}"          # everything after the marker...
    case "$names" in *:*) names="${names%%:*}";; esac   # ...up to a further ':'-clause, if any follows
    case ",$names," in *",$dep,"*) return 0;; esac
    return 1
}

# ⭐⭐ picker-dependency-and-boomerang-blindness (hq_B, 2026-08-28) — THREE CURES FOR ONE SYMPTOM: THE PICKER
# CANNOT SEE A DEPENDENCY A HUMAN KNOWS, so every seat re-pays the discovery cost. hq_P's census measured FOUR
# instances IN ONE DAY. All three cures below are MECHANISM-level; a per-row fix is precisely what we did four
# times already, and four hand-cures of one disease is the evidence that hand-curing is not the cure.
s4e_row_state() { qrow "${1:-}" | cut -f4; }

# ⭐ CURE 3 — A GRANT-WAIT IS NOT A PARK. `perf-nv-set-fn-o0-overhead` was re-served SEVEN times in one day
# (seat05 four times in NINE MINUTES) on a blocker no seat could ever clear: it needed a governance grant from
# Lon. Neither existing state fits. PARKED is routing, liftable by anyone with `park <topic> FREE`; the
# BLOCKED-ON:/PARKED-AWAITING: family SELF-CLEARS the instant its named topic goes DONE. A grant names no topic
# and never goes DONE, so both spellings are lies about the same wait. GRANT-NEEDED is its own state: the picker
# skips it, it never self-clears, and `park <topic> FREE` REFUSES on it unless S4E_GRANT_BY names the grant.
# ⛔ PARKED-LON-HOLD is honoured as a SYNONYM, not migrated away, because THREE LIVE ROWS ALREADY CARRY IT — the
# fleet invented this state by hand before the tool had one, exactly as it had invented PARKED-AWAITING before
# BLOCKED-ON existed. A canonical spelling that orphans the rows already using the ad-hoc one cures nothing.
# ⭐⭐ RESTRICTED:<x> — THE STATE THAT SURVIVES ITS OWN RELEASE (ceo ruling 2026-08-29, on hq_P's report; implemented hq_P).
# ⛔ WHY IT IS NOT JUST ASSIGNED WITH A DIFFERENT NAME: ASSIGNED:<x> was carrying TWO meanings with DIFFERENT LIFETIMES.
# (1) DISPATCH — HQ handed this work to x; when x lets go the assignment IS spent, and ceo's 2026-08-28 always-FREE
# unclaim ruling is exactly right for it. (2) RESTRICTION — this row may only EVER be worked by x (CEO-19 HQ-only rows):
# releasing a lock does not spend that, because the restriction belongs to the ROW and not to one episode of work on it.
# One spelling, two lifetimes, so no single rule about what release does could serve both -- and the one we had silently
# destroyed (2). MEASURED, not theorised: hq_P released defect-c-zop-flat-regime-depth-compensate with plain `unclaim`,
# the column went FREE, and seat16's `next` locked an HQ-only codegen row within minutes -- precisely the scenario that
# row's own header was minted to prevent. ⛔ THE PATCH WE REFUSED, and the refusal is the load-bearing part: special-case
# ASSIGNED:<hq> inside unclaim. That keys behaviour on WHO the assignee is rather than WHAT the state means, so the first
# restricted row assigned to a non-HQ breaks in silence -- the keyed-on-identity-not-behaviour shape banned elsewhere in
# this project for the same reason. A distinct spelling makes the lifetime a property of the STATE, checkable by machine.
s4e_restricted_to() { case "${1:-}" in RESTRICTED:?*) printf '%s' "${1#RESTRICTED:}"; return 0;; *) return 1;; esac; }
s4e_is_grant_wait() { case "${1:-}" in GRANT-NEEDED|GRANT-NEEDED:*|PARKED-LON-HOLD|PARKED-LON-HOLD:*) return 0;; *) return 1;; esac; }

# ⭐ CURE 2 — THE RELEASE BOOMERANG. seat01 released rank-0 `icon-n2` and the picker served THE SAME ROW BACK
# two minutes later, because a released row is instantly the top free row again and nothing remembered who let
# it go. That is a livelock, not a dispatch: the one seat guaranteed to refuse a row again is the seat that just
# judged it "not mine". `unclaim` now leaves a receipt under released/, and PASS 3 will not serve a row back to
# ITS OWN releaser inside the cooldown. ⛔ IT HIDES THE ROW FROM THE RELEASER ONLY — every other seat still sees
# it, which is the entire point: the row must stay live for the fleet, or this cure becomes the s265 disease it
# is modelled on. Any OTHER seat claiming it, or an HQ assigning it, DELETES the receipt — "without another
# seat/HQ touching it" is the row's own wording for when the cooldown ends early.
s4e_boomerang_hold() {   # <topic> -> rc 0 if THIS seat released it too recently to be served it back
    local r="$PO/released/${1:-}.release" who when now cool
    [ -f "$r" ] || return 1
    who="$(sed -n 1p "$r" 2>/dev/null)"; when="$(sed -n 2p "$r" 2>/dev/null)"
    [ "$who" = "$ME" ] || return 1
    case "$when" in ''|*[!0-9]*) return 1;; esac
    cool="${S4E_RELEASE_COOLDOWN:-3600}"; case "$cool" in ''|*[!0-9]*) cool=3600;; esac
    now="$(date -u +%s)"; [ "$((now - when))" -lt "$cool" ]
}
s4e_release_receipt() { mkdir -p "$PO/released" 2>/dev/null || return 0; printf '%s\n%s\n' "$ME" "$(date -u +%s)" > "$PO/released/${1}.release" 2>/dev/null || true; }
s4e_release_clear()   { rm -f "$PO/released/${1}.release" 2>/dev/null || true; }

# ⭐ CURE 1 — DEPENDENCY INVERSION: THE BLOCKER OUTRANKS THE BLOCKED, WHATEVER THE COLUMNS SAY.
# `prolog-call-n-user-predicate-segfault` (rank 1) blocked `polyglot-scrip-demos-10-working`, the umbrella
# outranked its own blocker, and so the rank-sorted picker served the BLOCKED work first — cured by hand that
# once, by a ceo re-rank 2->1. Re-ranking by hand does not scale and does not survive the next mint: rank is a
# human's GUESS at priority, a dependency is a FACT, and a fact outranks a guess. PASS 3 no longer merely skips
# a blocked row — it walks the chain and serves the blocker AT THE BLOCKED ROW'S OWN RANK POSITION.
# ⛔ Bounded at 8 hops with a seen-set: a dependency CYCLE must degrade to "skip", never spin. Every reason to
# decline promotion returns 1 and lets the caller fall through to the ordinary skip — this function can only
# ever ADD a serve the picker would not otherwise make, never suppress one it would.
s4e_servable_blocker() {   # <blocked-topic> -> prints the topic to serve INSTEAD, or nothing (rc 1)
    local cur="${1:-}" seen=" " blk st depth=0
    while [ "$depth" -lt 8 ]; do
      depth=$((depth+1))
      case "$(s4e_row_state "$cur")" in BLOCKED-ON:*|PARKED-AWAITING:*) st="$(s4e_row_state "$cur")"; blk="${st#*:}";; *) return 1;; esac
      [ -n "$blk" ] || return 1
      case "$seen" in *" $blk "*) return 1;; esac        # cycle -> skip, never spin
      seen="$seen$blk "
      s4e_blocker_done "$blk" && return 1                # resolved: the caller's self-heal owns this case
      [ -n "$(qrow "$blk")" ] || return 1                # dangling blocker: no row exists to serve
      [ -f "$PO/claims/$blk.claim" ] && return 1         # already someone's
      s4e_boomerang_hold "$blk" && return 1              # never promote a row I myself just released
      case "$(s4e_row_state "$blk")" in
        FREE|'')                        printf '%s' "$blk"; return 0;;   # servable -> promote it
        RESTRICTED:*)                   [ "$(s4e_restricted_to "$(s4e_row_state "$blk")")" = "$ME" ] && { printf '%s' "$blk"; return 0; }; return 1;;   # only its own owner
        BLOCKED-ON:*|PARKED-AWAITING:*) cur="$blk";;                     # transitive -> walk deeper
        *)                              return 1;;                       # parked/grant-gated -> nothing to promote
      esac
    done
    return 1
}
# ⭐⭐ PROMOTION ADMISSIBILITY -- next-dependency-promotion-walks-around-the-mode-lane-filter. MEASURED
# (seat01, MODE FLEET-8 ON SNOBOL4 ONLY): `next` served and LOCKED an Icon row via s4e_servable_blocker
# above with NO lane check and NO language-freeze check at all -- the ordinary FREE-row path below applies
# both before it ever claims a row; the dependency-inversion promotion above applied neither. Checked
# BEFORE $promo is ever claimed (not after, the way s4e_dispatch_gate runs post-claim) so a refusal never
# leaves an abandoned claim behind -- a refuse-then-unclaim two-step would race every other seat's own
# `next` against the brief window the claim existed. Two hard preconditions, checked in the same order
# and with the same semantics the ordinary path already uses for $topic, now applied to $promo too:
# (1) the MODE language freeze, never relaxed (see s4e_language_freeze_refuses); (2) the lane cut, relaxed
# only in the any-lane fallback pass, exactly like the ordinary path's own own-lane/any-lane split. rc 0 =
# admissible; rc 1 = refused, and the reason is already printed -- never a silent skip, per this row's own
# explicit requirement.
s4e_promotion_admissible() {   # <promo-topic> <blocked-topic> <rank>
    local _p="$1" _blocked="$2" _rank="$3" _tl
    if s4e_language_freeze_refuses "$_p"; then
      printf '⛔ REFUSED PROMOTION: rank-%s %s is BLOCKED-ON %s, but %s is %s and MODE freezes work to %s ONLY.\n' \
        "$_rank" "$_blocked" "$_p" "$_p" "$(s4e_topic_language "$_p")" "$(s4e_mode_language_freeze | tr '[:lower:]' '[:upper:]')"
      printf '   Not promoted, not served -- %s stays skipped this pass; a language freeze is never relaxed by a fallback.\n' "$_blocked"
      return 1
    fi
    if [ -n "${_my_lane:-}" ]; then
      _tl="$(s4e_topic_lane "$_p")"
      if [ -n "$_tl" ] && [ "$_tl" != "$_my_lane" ] && [ "${_lane_filter:-own-lane}" = own-lane ]; then
        printf '⛔ REFUSED PROMOTION: rank-%s %s is BLOCKED-ON %s, but %s is %s'"'"'s lane and yours is %s.\n' \
          "$_rank" "$_blocked" "$_p" "$_p" "$_tl" "$_my_lane"
        printf '   Not promoted (own-lane pass); %s stays skipped this pass -- retried cross-lane if your own lane has nothing else.\n' "$_blocked"
        return 1
      fi
    fi
    return 0
}
# ⭐ SAME SHAPE AS s4e_promotion_admissible ABOVE, ONE CHECK LATER IN THE CHAIN: a promoted blocker is
# claimed and served exactly like any ordinary row (row mint-dispatch-refuses-a-placeholder-done-when),
# so it needs the same pre-claim placeholder refusal the ordinary path gets -- calling
# s4e_predispatch_placeholder_check directly (defined below, safe: bash resolves calls at run time, and
# this is only ever invoked from deep inside the `next)` case, well after every function in this file is
# defined) rather than duplicating its text-match logic a second time.
s4e_promotion_placeholder_ok() {   # <promo-topic> <blocked-topic> <rank>; rc 0 = ok to promote+claim, 1 = refused (printed)
    s4e_predispatch_placeholder_check "$1" || return 0
    printf '⛔ REFUSING TO PROMOTE %s (blocker of %s, rank %s) — %s\n' "$1" "$2" "$3" "$_ppc_why"
    printf '   Not claimed. %s stays BLOCKED-ON %s until the blocker'"'"'s baton gets a real DONE-WHEN.\n' "$2" "$1"
    return 1
}
# ⛔⭐ ONE PROCESS PER IDENTITY (ceo RULING 2026-09-03, row bus-refuses-a-second-live-process-under-one-seat-identity).
# MEASURED by seat11, routed by hq_B 16:40: /home/claude11 held TWO live claude processes at once -- an interactive
# session and a scheduled routine (`claude --name Fleet #11 --model claude-sonnet-5 --effort max`, per
# .claude/scheduled_tasks.lock). LAW 6 above derives identity from the ROOT PATH, so BOTH are seat11 to the bus:
# one $ME, one claim namespace, one inbox. The routine left SCRIP dirty mid-edit while the session closed a row.
# ⛔⛔ AND THE RACE IS SILENT FROM BOTH SIDES, WHICH IS THE WHOLE DEFECT -- a claim HIDES its row from the other
# picker, so neither process can see the other and each reads a coherent, entirely wrong world. This is the
# 'non-empty is not alive' false-signal class the DRAINED and phantom-mailbox blocks above already convict,
# arriving one level down: not a stale FILE, a second live WRITER nobody declared.
# ⭐ THE RULE: one process per identity. A scheduled or cron routine never runs in a seat root -- it runs as its
# own identity in its own root, or not at all. The bus ENFORCES that here rather than trusting it.
# ⛔ IDENTITY IS THE PAIR (pid, starttime), NEVER THE PID ALONE. Linux recycles pids; a bare-pid lock eventually
# names a DIFFERENT live process and refuses a seat that is alone in its root -- a guard manufacturing the very
# false signal it exists to remove. /proc/<pid>/stat field 22 is that process's start time and is stable for its
# whole life, so the pair is unforgeable. ⛔ The comm field can contain BOTH spaces and parens, so it is parsed
# after the LAST ')', never by a positional split of the whole line.
s4e_proc_start() { local _s; _s="$(cat "/proc/$1/stat" 2>/dev/null)" || return 1; [ -n "$_s" ] || return 1
    printf '%s' "${_s##*') '}" | awk '{print $20}'; }
s4e_pid_live() { [ -n "${1:-}" ] && [ -n "${2:-}" ] && [ -d "/proc/$1" ] || return 1
    local _s; _s="$(s4e_proc_start "$1")" || return 1; [ -n "$_s" ] && [ "$_s" = "$2" ]; }
# ⛔ RELEASES ONLY OUR OWN LOCK. An unconditional rm would let a crashed verb's cleanup delete the lock of the
# process that legitimately took it next -- worse than never locking, because it fails in the direction of
# pretending the identity is free.
s4e_pid_release() { local _f="$PO/$ME/.pid" _p _t
    [ -f "$_f" ] || return 0; read -r _p _t _ < "$_f" 2>/dev/null || true
    [ "${_p:-}" = "$$" ] && rm -f "$_f"; return 0; }
s4e_pid_arm() { trap 's4e_pid_release' EXIT; trap 's4e_pid_release; exit 130' INT; trap 's4e_pid_release; exit 143' TERM HUP; }
s4e_pid_acquire() {
    local _f="$PO/$ME/.pid" _mine _hs _hp _ht _p _t _at _c _tries=0
    _mine="$(s4e_proc_start $$)" || _mine=""
    [ -n "$_mine" ] || { printf '⚠ one-process-per-identity guard INERT: /proc/%s/stat is unreadable, so process identity cannot be MEASURED. Proceeding UNGUARDED.\n' "$$" >&2; return 0; }
    # ⭐ A NESTED SELF-INVOCATION IS NOT A SECOND PROCESS. This script calls itself EIGHT times ("$0" banner /
    # claim / park / send, plus one `exec "$0" send`): a lock that could not tell a descendant from a stranger
    # would deadlock the bus against ITSELF on the very first `board`. The holder exports its (pid,starttime);
    # a descendant sees a LIVE holder for its own seat and passes straight through, taking nothing, releasing
    # nothing -- the parent's trap owns the lock for the whole tree.
    # ⛔ THE `exec` CASE IS THE SUBTLE ONE: exec REPLACES THE IMAGE BUT KEEPS THE PID *AND* THE STARTTIME, so the
    # new image IS the holder rather than a child -- and exec threw the release trap away with the old image.
    # It must RE-ARM or the lock leaks. Distinguished by pid == $$; a recycled pid cannot reach here, its
    # starttime differs. ⛔ A stale S4E_PID_LOCK inherited from a dead run simply fails the liveness test and
    # falls through to a normal acquire -- it can never wedge a fresh process.
    # ⛔ AND THE MARKER MUST AGREE WITH THE LOCK FILE, NOT MERELY NAME A LIVE PROCESS. S4E_PID_LOCK is inherited
    # environment: a stale one from an earlier run, or one leaking in from an unrelated s4e process that happens
    # to be an ancestor, would otherwise wave a genuine second writer straight through. A real descendant's
    # holder is BY CONSTRUCTION the process that wrote $PO/<seat>/.pid, so requiring the pair to match the file
    # costs a descendant nothing and closes the hole. MEASURED: without this the gate for this very row passed
    # standalone and FAILED under `done`, because `done` had exported the marker into it -- a verdict that
    # depended on the caller's environment, which is no verdict at all.
    if [ -n "${S4E_PID_LOCK:-}" ]; then
      IFS=: read -r _hs _hp _ht <<< "$S4E_PID_LOCK"
      if [ "${_hs:-}" = "$ME" ] && s4e_pid_live "${_hp:-}" "${_ht:-}" && [ -f "$_f" ]; then
        read -r _p _t _ < "$_f" 2>/dev/null || true
        if [ "${_p:-}" = "${_hp:-}" ] && [ "${_t:-}" = "${_ht:-}" ]; then
          [ "${_hp:-}" = "$$" ] && s4e_pid_arm; return 0; fi
      fi
    fi
    # ⛔ AN UNWRITABLE MAILBOX MAKES THE GUARD INERT, NOT FATAL. Refusing here would turn one broken permission
    # into a TOTAL bus outage for that seat, including read-only `check` -- the guard becoming a worse defect
    # than the one it polices. Every verb that actually writes still fails on its own write, loudly.
    if [ ! -w "$PO/$ME" ]; then
      printf '⚠ one-process-per-identity guard INERT: %s is not writable, so the lock cannot be taken. Proceeding UNGUARDED.\n' "$PO/$ME" >&2; return 0; fi
    while :; do
      # ⭐ noclobber makes this redirect O_CREAT|O_EXCL -- atomic on every POSIX filesystem, the same guarantee
      # `mint`'s mkdir lock and `claim`'s `ln` lean on. Two verbs racing here: exactly one creates the file.
      if ( set -o noclobber; printf '%s %s %s %s\n' "$$" "$_mine" "$(date -u +%FT%TZ)" "${cmd:-?}" > "$_f" ) 2>/dev/null; then
        export S4E_PID_LOCK="$ME:$$:$_mine"; s4e_pid_arm; return 0; fi
      _p=""; _t=""; _at=""; _c=""
      [ -f "$_f" ] && { read -r _p _t _at _c < "$_f" 2>/dev/null || true; }
      if s4e_pid_live "${_p:-}" "${_t:-}"; then
        printf '\n⛔⛔⛔ REFUSED: ANOTHER LIVE PROCESS ALREADY HOLDS THE IDENTITY %s ⛔⛔⛔\n' "$ME" >&2
        printf '    INCUMBENT  pid %s  -- running `%s`, lock taken %s\n' "$_p" "${_c:-?}" "${_at:-?}" >&2
        printf '    THIS ONE   pid %s  -- running `%s`\n' "$$" "${cmd:-?}" >&2
        printf '    lock file: %s\n' "$_f" >&2
        printf '\n    WHY THIS IS REFUSED RATHER THAN QUEUED: identity is derived from the ROOT PATH (LAW 6), so both\n' >&2
        printf '    processes in %s are "%s" to the bus -- ONE $ME, ONE claim namespace, ONE inbox. A claim HIDES its\n' "$S4E" "$ME" >&2
        printf '    row from the other picker, so the two would race SILENTLY and each would read a coherent, wrong world.\n' >&2
        printf '\n    ONE PROCESS PER IDENTITY (ceo 2026-09-03). A scheduled or cron routine NEVER runs in a seat root:\n' >&2
        printf '    it runs as its own identity in its own root, or not at all. If a routine is the incumbent here, stop\n' >&2
        printf '    it (check %s/.claude/scheduled_tasks.lock) rather than working around this refusal.\n' "$S4E" >&2
        printf '    If pid %s is a verb of YOURS that is legitimately still running (a long `done` DONE-WHEN holds the\n' "$_p" >&2
        printf '    lock for its whole run), wait for it. The lock self-clears the moment that process is gone.\n\n' >&2
        exit 2; fi
      # stale (holder dead, or a torn/empty file) -> self-clear and retry, bounded. Never loops forever.
      _tries=$((_tries+1))
      if [ "$_tries" -gt 3 ]; then
        printf '\n⛔ REFUSED (rc=2, COULD NOT MEASURE): %s exists, names no live process, and could not be cleared after %s attempts.\n' "$_f" "$_tries" >&2
        printf '    This is not a verdict about a second process -- it is the guard saying it cannot tell. Inspect the file by hand.\n\n' >&2
        exit 2; fi
      rm -f "$_f" 2>/dev/null || true
    done
}
cmd="${1:-check}"
# ⛔ THE LOCK IS TAKEN IN THE SAME CASE ARM AS THE IDENTITY ASSERTION, DELIBERATELY: the set of verbs that have
# an identity is exactly the set that can hold one, so the two can never drift apart into a verb that asserts a
# box but takes no lock. `mailbox` is excluded from both -- it is how a NEW identity is created, and it cannot
# lock a mailbox that does not exist yet. ⛔ `check` and `clear` ARE locked, unlike the unread-mail banner below:
# THE LOOP step 1 is `check`, and a `check` that quietly succeeds beside a second live writer is precisely the
# silent-race reading this row exists to abolish. The refusal IS the diagnostic that surfaces the second process.
case "$cmd" in mailbox|"") ;; *) s4e_assert_box "$ME" identity; s4e_pid_acquire;; esac
# ⛔ ORPHANED .msg.* ARE SWEPT ON EVERY RUN (LAW 6, second half). `send` writes the message to a mktemp
# $PO/.msg.XXXXXX and then mv's it into the destination inbox; when that mv failed the temp file just SAT there
# -- one rotted 46 hours at the postoffice root, a seat-to-seat brief neither end ever knew was lost. A message
# whose header still names a reachable mailbox is RE-DELIVERED here; one that does not is REPORTED loudly and
# left on disk. Never deleted: an undeliverable message is evidence, not garbage.
# ⛔⭐⭐ ROWD-CLOSE HANDS OFF OR REDS THE FLEET (CEO-69 batch 5a + CEO-70; row done-must-hand-off-manifest-cited-rows, hq_P).
# THREE IDENTICAL FLEET-WIDE REDS IN ONE DAY: closing a row cited as ROWD evidence in corpus_coverage_manifest.tsv reds
# `make test` for EVERY seat until someone re-points the manifest BY HAND (pas-display-revival 07:49 -> pascal-bench-quick-
# wrong-biggest 08:08 -> pascal-bool-family-truncated-output 14:35, the last caught mid-re-point). The manifest's own law,
# line 15: "ROWD evidence = a task topic that must exist and NOT be DONE." Nothing enforced it, so `done` could red the
# fleet and report success. ⭐ ONE check, placed BEFORE the done/OVERRIDE fork, so both close paths are covered BY
# CONSTRUCTION and cannot drift -- the emit_rec_fb / icn_gen_host_reserve lesson: two copies of one rule DO drift.
# ⛔ MATCHES ON $3=="ROWD" ALONE, never on $1, so a future kind= still trips the guard. FAILS CLOSED by design.
s4e_manifest_rowd_cite() {   # echo the citing manifest line(s) for topic $1; rc 0 = cited (refuse), rc 1 = not cited
    local t="$1" m="$S4E/SCRIP/scripts/corpus_coverage_manifest.tsv"
    [ -n "$t" ] && [ -f "$m" ] || return 1
    awk -F'\t' -v t="$t" '/^#/ {next} $3=="ROWD" && $4==t {print; f=1} END{exit !f}' "$m"
}
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
# ⛔⭐ THIS IS THE REVERSE OF THE IDENTITY MAP AT :92-97 AND THE TWO MUST AGREE. hq_T WAS MISSING HERE while the
# forward map had it, so `fleet` could not resolve hq_T's root and printed it as "0 STALLED (no root, retiring)"
# with a FAILURE banner WHILE hq_T WAS LANDING ROWS BY THE HOUR (ceo measured 2026-09-03 22:36). A health view
# that INVENTS a dead seat is worse than one that reports nothing: the ceo acts on it. The forward map gained
# hq_T when the fourth HQ opened; this one did not, because nothing checks that two hand-written tables of the
# same fact still agree -- the same class as the per-root digests drifting from RULES.md.
s4e_root() { case "$1" in ceo|hq) echo /home/claude;; hq_C) echo /home/claude_C;; hq_P) echo /home/claude_P;; hq_B) echo /home/claude_B;; hq_T) echo /home/claude_T;; hq_U) echo /home/claude_U;; hq_S) echo /home/claude_S;; hq_I) echo /home/claude_I;; hq_R) echo /home/claude_R;;
    seat0[1-9]|seat1[0-9]|seat20) echo "/home/claude${1#seat}";; *) echo "";; esac; }
s4e_hqboxes() { for _h in hq hq_C hq_P hq_B hq_T hq_U hq_S hq_I hq_R ceo; do [ -d "$PO/$_h/inbox" ] && echo "$_h"; done; }
s4e_is_hq() { case "$1" in hq|hq_C|hq_P|hq_B|hq_T|hq_U|hq_S|hq_I|hq_R|ceo) return 0;; *) return 1;; esac; }
# ⭐⭐ THE LANE — topic->HQ and identity->HQ, so `next` can restrict dispatch without inventing a second
# copy of MASTER-PLAN's THE LANES table (row next-serves-a-seat-only-rows-in-its-hqs-lane-and-no-row-
# carries-a-blank-owner-cell). Topic lane: the owner cell (QUEUE.tsv field 3) wins when it already names
# one of the four HQs — an explicit, already-made decision beats a guess; otherwise derived from the
# topic's LANGUAGE prefix, the same split THE LANES table uses. A topic naming no language (a postoffice/
# tooling/meta row — this very row is one) returns empty: UNDETERMINED, never defaulted, because guessing
# wrong here would starve or misroute a lane the mapping cannot see. Callers must treat empty as
# lane-neutral (servable from any lane), not as a fifth lane of its own.
s4e_topic_lane() {
    local _t="$1" _owner
    _owner="$(qrow "$_t" | cut -f3)"
    case "$_owner" in hq_C|hq_B|hq_P|hq_T|hq_U|hq_S|hq_I|hq_R) printf '%s' "$_owner"; return 0;; esac
    case "$_t" in
      prolog-*)                      printf 'hq_C';;
      icon-*)                        printf 'hq_B';;
      snobol4-*|snocone-*|pascal-*)  printf 'hq_P';;
      raku-*|rebus-*)                printf 'hq_T';;
    esac
}
# Identity lane: ceo is never restricted — checked by the CALLER against the identity SHAPE, the same
# precedent the MODE guard above already set (⛔ NOT s4e_is_hq(), which counts ceo as an HQ and would
# wrongly lane-restrict the one identity THE LANES table does not partition). An HQ's lane is itself. A
# seat's lane is read from ITS OWN postoffice HQ file — the same file `ask`'s routing already treats as
# authoritative — not a second hardcoded seat-number table that can drift from MASTER-PLAN while nobody
# is watching, the way seat07's own HQ file (checked 2026-09-04) still read hq_C after the 16-seat cut
# moved it to hq_T. An unreadable or unrecognized HQ file returns empty: UNDETERMINED, never restricted,
# so a stale or missing HQ file degrades to today's lane-blind behaviour rather than locking a seat out
# of every row in the queue.
s4e_my_lane() {
    case "$ME" in
      hq_C|hq_B|hq_P|hq_T|hq_U|hq_S|hq_I|hq_R) printf '%s' "$ME"; return 0;;
      seat*) local _f="$PO/$ME/HQ" _l
             [ -f "$_f" ] || return 0
             _l="$(head -1 "$_f" | tr -d '[:space:]')"
             case "$_l" in hq_C|hq_B|hq_P|hq_T|hq_U|hq_S|hq_I|hq_R) printf '%s' "$_l";; esac ;;
    esac
}
# ⭐⭐ THE MODE LANGUAGE FREEZE -- next-dependency-promotion-walks-around-the-mode-lane-filter, item (3):
# "add the MODE priority cut... as an explicit precondition alongside the lane cut, since today only the
# lane column is consulted and the freeze is enforced by seats remembering it." A Lon language freeze
# ("SNOBOL4 ONLY", later "ICON ONLY") is state written into MODE's newest entry, in prose, in the same
# ALL-CAPS convention every ceo mode announcement already uses for a binding clause -- parsed here rather
# than re-typed as a magic string, so the NEXT freeze (a different language) needs no code change. Line 2
# is the newest dated entry (line 1 is the bare mode value s4e_mode_line already reads); a freeze not
# stated there is not active, even if an older entry once had one -- each entry restates the CURRENT rule
# in full, never a diff against the previous line. Returns the lowercased language token, or empty when no
# freeze is stated -- empty means NO FREEZE, not "undetermined": unlike lane derivation there is no
# ambiguous middle state here.
s4e_mode_language_freeze() {
    sed -n 2p "$PO/MODE" 2>/dev/null | grep -oE 'ON [A-Z][A-Z0-9]* ONLY' | head -1 | awk '{print $2}' | tr '[:upper:]' '[:lower:]'
}
# Companion to s4e_topic_lane, same prefix table, different axis: LANGUAGE, not HQ. gimpel-*/snoflake-*
# are SNOBOL4-suite families with no snobol4- prefix of their own (this session's own aisnobol/snoflake
# census rows) -- named explicitly rather than left to fall through, the same reason s4e_topic_lane names
# every prefix instead of defaulting one. A topic naming no language (tooling/meta, like this row itself)
# returns empty: LANGUAGE-NEUTRAL, never frozen out, the same "empty is never a fifth category" rule
# s4e_topic_lane documents for lane.
s4e_topic_language() {
    case "${1:-}" in
      icon-*)                        printf 'icon';;
      prolog-*)                      printf 'prolog';;
      snobol4-*|gimpel-*|snoflake-*) printf 'snobol4';;
      snocone-*)                     printf 'snocone';;
      pascal-*)                      printf 'pascal';;
      raku-*)                        printf 'raku';;
      rebus-*)                       printf 'rebus';;
    esac
}
# rc 0 ("refuses") only when a freeze IS active, the topic's language IS determined, and they DIFFER --
# every other combination (no freeze, language-neutral topic, or topic matches the frozen language) is
# rc 1 ("does not refuse"). Deliberately UNCONDITIONAL, no own-lane/any-lane softening the way the lane
# cut has: Lon's freeze paused even standing HQ assignments fleet-wide (MODE 2026-09-04 17:38 CDT, "All
# HQ's and all Fleet workers are on SNOBOL4 only"), so an automatic pick honours it the same way
# regardless of an owner cell or a cross-lane fallback -- `claim <topic>` remains the deliberate override
# it already is everywhere else in this file, for the rare case a seat has an actual reason to work
# outside the freeze.
s4e_language_freeze_refuses() {
    local _fl _tl
    _fl="$(s4e_mode_language_freeze)"; [ -n "$_fl" ] || return 1
    _tl="$(s4e_topic_language "${1:-}")"; [ -n "$_tl" ] || return 1
    [ "$_tl" != "$_fl" ]
}
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
# ⛔⭐ RUN A ROW'S DONE-WHEN AT DISPATCH, BEFORE HANDING IT TO ANYONE (ceo ruling 2026-09-03 adopting seat09's
# suggestion; row next-and-assign-re-run-a-rows-done-when-before-serving-it). MEASURED by seat09
# (FINDING-2026-09-03-seat09-pascal-relop-array-field-row-was-already-cured-five-days-before-dispatch.md):
# pascal-relop-into-array-and-field-lvalues-loses-value sat FREE for FIVE DAYS describing a bug that a sibling
# row (pascal-restore-prezeta, ff1df778) had already cured, and its DONE-WHEN grepped an exact "N pass / 0 fail"
# string that corpus growth had made permanently unpassable. ⭐ THE DEFECT IS NOT THE DEAD ROW, IT IS THAT THE
# QUEUE HAD THE ANSWER THE WHOLE TIME AND NOBODY ASKED IT: the criterion that would have closed the row was
# sitting in the baton, one command away, through five days and every picker pass over it.
# THE THREE-WAY SPLIT (the ruling, verbatim in substance): green closes the row DONE with a ledger line
# "already satisfied at dispatch" and serves the next; a REFUSE (rc=2) or timeout serves the row WITH A WARNING;
# only RED serves it silently. ⛔ Red is the ONLY silent outcome, because red is the only one that means
# "there is real work here" -- the other two mean the queue does not know, and a picker that hides what it does
# not know is how a five-day-dead row reads as live work.
# ⛔⭐ THE TIMEOUT IS SHORT AND THAT IS THE POINT. `done`'s budget is 3600s because a close must be allowed to
# run `make test`; a DISPATCH probe must not stall the picker for twenty minutes to answer a question it asked
# on spec. A criterion too slow to probe is NOT a failure of the row -- it times out, which is rc=2, which
# serves the row with a warning. The safe direction is always SERVE.
s4e_dispatch_timeout() { printf '%s' "${S4E_DISPATCH_PROBE_TIMEOUT:-120}"; }
# ⛔ MIRRORS `done`'s vacuity blocklist and is NOT the authority for it -- `done` is (:898 and :916-922). Used
# only by `assign`, which cannot delegate to `done` (that verb requires the claim to be the RUNNER's, and assign
# writes ANOTHER seat's). `next` never calls this: it probes by invoking `done` itself, so its judgement is the
# real one by construction. ⛔ Two copies of one rule DO drift -- this file convicts that everywhere -- so the
# gate asserts the two agree over a table of criteria rather than trusting the comment.
s4e_donewhen_is_noop() {   # $1 = raw DONE-WHEN text; rc 0 = certifies nothing
    local dw="${1:-}" nc nrm first
    case "$(printf '%s' "$dw" | tr -d '[:space:]')" in true|:|exit0|/bin/true|/usr/bin/true|""|"#"*) return 0;; esac
    nc="$(printf '%s' "$dw" | s4e_strip_donewhen_comment)"
    nrm="$(printf '%s' "$nc" | sed -E 's/^[[:space:]]+//; s/[[:space:]]+$//; s/[[:space:]]+/ /g')"
    printf '%s' "$nrm" | grep -qE '[;|&`]|\$\(' && return 1
    case "$nrm" in ""|"exit 0"|exit0) return 0;; esac
    first="$(printf '%s' "$nrm" | awk '{for(i=1;i<=NF;i++){if($i !~ /^[A-Za-z_][A-Za-z0-9_]*=/){print $i; exit}}}')"
    case "$first" in true|:|/bin/true|/usr/bin/true|echo) return 0;; esac
    return 1; }
# ⛔⭐ S4E-GUARD-COMPILER-ABSENT -- A CRITERION THAT DRIVES AN UNBUILT COMPILER CANNOT BE MEASURED, SO IT IS
# REFUSED (rc=2), NEVER GRADED RED (rc=1). hq_P -> hq_B 2026-09-04, measured on pascal-m4-site1-forloop-backedge-
# 64byte-excess: ~88 batons carry seat05's 09-01 template whose compile step reads
#     timeout 300s SCRIP/scrip --compile ... || { echo "... compile failed"; exit 1; }
# so a MISSING or not-yet-built ./scrip reports rc=1 -- "there is real work here", which the dispatch probe serves
# SILENTLY -- instead of rc=2 -- "I could not measure", which it serves WITH a warning. The ceo root's binary was
# timestamped 21 minutes AFTER the assign that read that row red. ⭐ THE CURE IS ONE GUARD AT THE TWO RUNNER SITES,
# NOT A SWEEP OF 88 BATONS: a criterion is TEXT, and the property "this text drives the compiler" is readable from
# the text. Sweeping the batons would fix the ones that exist today and none of the ones written tomorrow.
# Returns 0 (and sets $_gca_why) when the criterion names the compiler AND that binary is absent.
s4e_donewhen_needs_compiler() {   # $1 = raw DONE-WHEN text
    local dw="${1:-}" b; _gca_why=""
    printf '%s' "$dw" | grep -qE '(^|[^A-Za-z0-9_./-])(\./)?(SCRIP/)?scrip([[:space:]]|$)' || return 1
    b="$S4E/SCRIP/scrip"
    [ -x "$b" ] && return 1
    _gca_why="the criterion drives the compiler and $b is not built (run make) -- a missing binary is COULD-NOT-MEASURE, never a red row"
    return 0; }
# ⛔⭐ IS-A-PLACEHOLDER, BY KNOWN SHAPE, NEVER BY BARE SUBSTRING (seat13, 2026-09-05, row fifty-seven-
# batons-are-unclosable-because-their-criterion-is-not-at-column-zero). The old test was `case "$dw" in
# *⛔*)` -- ANY occurrence of the glyph anywhere in the line -- which misread 34+ live, correct,
# substantive DONE-WHENs as unwritten placeholders solely because their OWN defensive guard clause
# prints a message containing ⛔, e.g. `[ -n "$S4E_HOME" ] || { echo "⛔ REFUSE(2): ..."; exit 2; };
# <the real check>`. MEASURED (whole postoffice, 2026-09-05): the bare-substring test false-positived on
# 34 S4E_HOME guard-clause batons plus 2 instrument/meta batons whose own DONE-WHEN must reference the
# placeholder text as DATA to detect placeholders in OTHER files -- 36 of 56 files the old detector named
# were never broken. Real placeholders come in exactly two shapes, both known in full: the `mint`
# boilerplate (always starts with this exact text) and the hand-rolled self-refusing stub `echo "⛔
# ..." >&2; false` pre-dating `mint` (6 live instances, e.g. error-code-parity, named-io-channels).
# Neither shape can appear as a SUFFIX of a real command the way the guard clause does, so anchoring
# beats searching.
s4e_donewhen_is_placeholder() {   # $1 = raw column-0 DONE-WHEN text (already extracted); rc 0 = still a placeholder
    local dw="${1:-}"
    [ -z "$dw" ] && return 0
    case "$dw" in "⛔ MUST BE MADE RUNNABLE"*) return 0;; esac
    printf '%s' "$dw" | grep -qE '^echo "⛔[^"]*"( *>&2)?; *(false|exit [1-9][0-9]*)$' && return 0
    return 1; }
# ⛔⭐⭐ THE ONE DONE-WHEN EXTRACTOR, AND IT READS THE WHOLE CRITERION (hq_T 2026-09-05, ceo rank-0 ruling on
# seat04's row s4e-msg-donewhen-truncation-false-closes-multiline-heredoc-batons; seat04 reproduced it end to end).
# It used to be `sed -n 's/^DONE-WHEN:...//p' "$b" | head -1` at THREE sites -- one physical line, discarding
# everything after it. ⛔ THAT IS A FALSE-GREEN ENGINE, NOT MERELY A TRUNCATION, and the direction is what makes it
# lethal: for the heredoc witness shape this project actively encourages ("creates its own repro at check-time
# rather than depending on this session's /tmp scratchpad"),
#     DONE-WHEN: cat > /tmp/w.sno <<'EOF'
#     <program>
#     EOF
#     out=$(scrip /tmp/w.sno); [ "$out" = want ] || exit 1; echo PASS
# the truncated text is `cat > /tmp/w.sno <<'EOF'` with no closing delimiter. bash WARNS ("here-document
# delimited by end-of-file"), treats the body as empty, writes a ZERO-BYTE file, and that cat -- the only command
# reached -- exits 0. So the whole criterion exits 0 having run NOTHING: the real check never executes, and a
# broken tree is byte-identical in verdict to a fixed one. ⭐ MEASURED CLOSURE: snobol4-pattern-primitive-as-
# function-argument-always-fails-in-callee closed DONE 2026-09-05T15:33Z through exactly this path, while the
# full untruncated text exits 1 on the same tree. 185 of 1128 live batons carry a multi-line DONE-WHEN.
# ⭐ THE CONTINUATION RULE IS THE FILE FORMAT'S OWN, not a new convention: a baton field runs to the next column-0
# field label or `## ` section -- which is exactly how GOAL: already carries paragraphs. The terminator set is the
# MEASURED label census of the live tasks tree, not a guess, and it is deliberately the STRUCTURAL labels only:
# prose labels that occur inside GOAL bodies (STEP 1:, Verified:, witness:) are NOT terminators, because a
# criterion's heredoc body is program text and must never be cut by a word that happens to end in a colon.
# Sets $_dw_backticked=1 when it stripped a markdown backtick pair, so a caller can still announce that.
s4e_donewhen_text() {   # $1 = baton path; prints the WHOLE criterion on stdout
    local b="$1" first rest line acc
    _dw_backticked=""
    first="$(sed -n 's/^DONE-WHEN:[[:space:]]*//p' "$b" | head -1)"
    [ -n "$first" ] || return 0
    acc="$first"
    # ⛔⭐⭐ CONTINUE ONLY WHILE THE TEXT IS INCOMPLETE SHELL, NEVER "UNTIL THE NEXT FIELD LABEL" -- and that
    # distinction is the whole design, measured the hard way. The obvious rule (take every line to the next
    # column-0 label, the way GOAL: carries paragraphs) is WRONG HERE and dangerous in the false-green direction
    # it is trying to cure: the live convention puts PROSE ANNOTATION after a criterion -- "⛔ DONE-WHEN REWRITTEN
    # 2026-08-24 (seat04): the line above used to be prose..." -- and 121 of the 122 DONE batons whose field spans
    # lines are exactly that shape. Swallowing their annotation into `bash -c` would break rows that close
    # correctly today, to fix one that does not.
    # ⭐ THE DISCRIMINATOR IS MEASURED, not a heuristic about indentation or glyphs: bash tells you whether text is
    # FINISHED. A complete line parses clean; a heredoc opener parses rc=0 WITH a "here-document ... delimited by
    # end-of-file" warning; an unclosed quote is rc=2 "unexpected EOF while looking for matching"; and PROSE is
    # rc=2 with a DIFFERENT error ("syntax error near unexpected token"), which must NOT pull the next line in.
    # So: a complete first line IS the criterion, byte for byte as before, for every baton in the tree today.
    # ⛔ RESIDUE, STATED RATHER THAN GUESSED AT: a criterion authored as two SYNTACTICALLY COMPLETE lines is
    # indistinguishable from an annotated one in this file format, so it still runs only its first line. That is
    # not curable by reading harder -- it needs a mint-time lint requiring one line, or an explicit continuation
    # (a trailing \\, && or a heredoc), which is what every real multi-line criterion in the tree already uses.
    if s4e_donewhen_incomplete "$acc"; then
        # ⭐ MULTI-LINE MODE, ENTERED ONLY BY EVIDENCE: the first line does not finish, so the criterion is the
        # WHOLE field block -- to the next column-0 field label or `## ` section, the baton format's own rule (it
        # is how GOAL: already carries paragraphs). Taking the whole block matters and a "keep adding lines until
        # it parses" loop is NOT enough: a heredoc becomes complete at its own delimiter, and seat04's measured
        # shape puts the REAL CHECK on the lines AFTER that delimiter -- stopping at first-complete would capture
        # the file write and drop the test, which is the original false green with two extra lines in it.
        # ⛔ And if the block ends still unfinished, the incompleteness guard at both runner sites REFUSES rc=2
        # rather than running it -- so a terminator that fires inside a heredoc body fails safe, never green.
        acc="$acc
$(awk '
            /^DONE-WHEN:/ && !seen { seen=1; next }
            seen && /^## / { exit }
            seen && /^(GOAL|LINKS|RANK|DONE-WHEN|DONE-WHEN-HISTORY|SCOPE|LEDGER|OWNER|BLOCKED-ON|FINDING|MINTED BY):/ { exit }
            seen { print }
        ' "$b")"
    fi
    # ⛔ ONE matched OUTER pair only, and never a one-sided backtick: a lone ` is either real substitution or a
    # typo, and both must fail LOUDLY rather than be silently rewritten. Normalised HERE, at the one extraction
    # point, so the vacuity probe and the real run cannot disagree about what the criterion IS.
    case "$acc" in '''`'''*'''`''') acc="${acc#\`}"; acc="${acc%\`}"; _dw_backticked=1;; esac
    printf '%s' "$acc"; }
# ⛔⭐ AND A SECOND, INDEPENDENT GUARD ON THE SAME FAILURE, because the extractor being right today is not a
# property anyone can keep proving: bash does NOT fail on an unterminated heredoc -- `bash -n -c "cat <<'EOF'"`
# exits 0 -- it only WARNS on stderr, which is precisely why the truncation could exit 0 for months. That warning
# is detectable, so any criterion whose text opens a heredoc it never closes is REFUSED (rc=2, could-not-measure),
# never graded. ⭐ This holds even if a future edit re-breaks the extractor, and it also catches a criterion an
# author simply mis-wrote -- the two cases a runner cannot tell apart and must not guess between.
# ⛔⭐⭐ MORE THAN ONE COLUMN-0 `DONE-WHEN:` LINE IS AN AMBIGUOUS CONTRACT, AND ONLY THE FIRST HAS EVER RUN
# (hq_T 2026-09-05, found while censusing the truncation class on the ceo's seed of six suspects). The extractor
# takes the first line by design; a baton carrying a SECOND column-0 DONE-WHEN: whose text is a real command has
# therefore been closing on HALF ITS OWN CONTRACT, silently, with the other half sitting in the file where every
# human reader counts it as part of the bar. ⭐ MEASURED: 39 live batons carry more than one such line; 10 extras
# survive the placeholder filters, but only THREE of those ten are genuinely a second criterion (see the
# byte-identical and prose rulings below -- the first census said ten and that number was wrong). Running the
# never-executed halves of the three: two are green (correctly closed anyway) and
# snobol4-xfail-class-setexit-errlimit-composition-2-entries is RED -- "cross-ref=0 (want 2)" -- a row closed on
# the half of its criterion that passed. That row is the ONE true false-closure this whole class ever contained.
# ⛔ REFUSES rather than running both: which line is the contract is a question about intent, and a tool that
# concatenates them invents a bar nobody wrote. Same call, and the same wording, as s4e_donewhen_hidden_elsewhere
# makes for its own case. The mint placeholder and the self-refusing stub are NOT counted -- a leftover placeholder
# under a real criterion is untidy, not ambiguous, and 4 of the 39 are exactly that.
# ⛔⭐⭐ A BYTE-IDENTICAL SECOND LINE IS NOT A SECOND CONTRACT (hq_T 2026-09-05, ceo's baton-repair order on the
# four open rows of this class). The refusal above rests entirely on "which line is the contract is a question
# about intent" -- and when the two texts are IDENTICAL there is no such question: the extractor taking the first
# loses nothing, no half of any bar went unrun, and the row was never in the false-closure hazard at all. Counting
# it would block a closable row for a COSMETIC reason, which is the exact ground on which the mint placeholder is
# already excluded four lines up. ⭐ MEASURED, whole postoffice, splitting the "10 real commands" this comment used
# to claim: 3 are byte-identical duplicates (prolog-failed-initialization-goal-exits-1-where-swipl-exits-0,
# raku-emitter-smx-coverage, dead-scrip-test-tree-three-raku-suites-report-green-on-nothing), 4 are PROSE at column
# zero, and only 3 are genuinely a second criterion -- all three snobol4-xfail-class rows. So the hidden-bar class
# is THREE, not ten, and exactly one of the three (setexit-errlimit) was ever red.
# ⛔ PROSE IS DELIBERATELY *NOT* DETECTED HERE, and that is a ruling, not an omission. Two of the four prose lines
# are an English spec full of backticks and one is a wrapped narrative sentence; every cheap discriminator I tried
# (shell metacharacters, a leading command word) misclassified at least one of them in BOTH directions. Telling
# prose from a command is the same intent question this function refuses to guess on, so the cure for those four is
# a RELABEL IN THE BATON, done in the same sitting -- never a heuristic here that would eventually eat a real bar.
# ⛔ The comparison is passed through the ENVIRONMENT, never `awk -v`: -v processes backslash escapes, and real
# criteria carry `\\` line continuations that would silently fail to compare equal.
# Sets $_dwm_lines for the message; rc 0 iff a second REAL, DIFFERENT criterion exists.
s4e_donewhen_multiple_contracts() {   # $1 = baton path
    local b="$1"
    _DWM_FIRST="$(sed -n 's/^DONE-WHEN:[[:space:]]*//p' "$b" | head -1)"; export _DWM_FIRST
    _dwm_lines="$(grep -n '^DONE-WHEN:' "$b" | sed -n '2,$p' \
        | grep -vE 'DONE-WHEN:[[:space:]]*⛔ MUST BE MADE RUNNABLE' \
        | grep -vE 'DONE-WHEN:[[:space:]]*echo "⛔[^"]*"( *>&2)?; *(false|exit [1-9][0-9]*)$' \
        | awk '{ txt = $0; sub(/^[0-9]+:DONE-WHEN:[[:space:]]*/, "", txt);
                 if (txt == ENVIRON["_DWM_FIRST"]) next;
                 num = $0; sub(/:.*$/, "", num); print num }' \
        | tr '\n' ' ' | sed 's/ $//')"
    unset _DWM_FIRST
    [ -n "$_dwm_lines" ]; }
s4e_donewhen_incomplete() {   # $1 = criterion text; rc 0 = bash cannot finish reading it
    printf '%s' "${1:-}" | bash -n /dev/stdin 2>&1 \
      | grep -qE 'here-document.*delimited by end-of-file|unexpected EOF while looking for matching|syntax error: unexpected end of file'; }
s4e_donewhen_unterminated_heredoc() { s4e_donewhen_incomplete "${1:-}"; }
# ⛔⭐ HIDDEN-ELSEWHERE: is there a DIFFERENT "DONE-WHEN:"-labeled line sitting in this baton's live text
# (GOAL/NEXT/QA), where a human but not the tool would read it as the contract? Scoped deliberately:
# the ## LEDGER section is excluded outright (it is historical narration -- this project's own ledgers
# routinely discuss "the DONE-WHEN as written" in prose, and a live contract would never legitimately
# live there), and backtick-quoted spans are stripped before matching (a GOAL sentence that says
# `` `sed -n 's/^DONE-WHEN:...'` `` to describe the extractor is a code REFERENCE, not a second
# candidate -- this is what the row's OWN GOAL paragraph and its sibling row's ledger both do, and
# both were false positives under the old whole-line scan). Sets $_dhe_lines (may be multi-line, numbered
# within the live text) for the refusal message; rc 0 iff at least one such line survives.
s4e_donewhen_hidden_elsewhere() {   # $1 = baton path
    local b="$1"
    _dhe_lines="$(awk '/^## LEDGER/{exit} {print}' "$b" \
        | sed -E 's/`[^`]*`//g' \
        | grep -n 'DONE-WHEN:' \
        | grep -vE '^[0-9]+:DONE-WHEN:' \
        | grep -vE '^[0-9]+:[[:space:]]*- ')"
    [ -n "$_dhe_lines" ]; }
# READ-ONLY probe: never closes, never claims, never writes. rc 0 green / 1 red / 2 could-not-measure.
# Sets $_dp_why (one line, why) and $_dp_out (the criterion's own output, which is the thing a reader needs --
# the mute-gate lesson from `done`: "DONE-WHEN exited 2" alone points nowhere).
s4e_dispatch_probe() {
    local t="$1" b="$PO/tasks/$1.task.md" dw to rc; _dp_why=""; _dp_out=""
    [ -f "$b" ] || { _dp_why="no task baton at $b"; return 2; }
    dw="$(s4e_donewhen_text "$b")"
    [ -n "$dw" ] || { _dp_why="the baton has no DONE-WHEN: line"; return 2; }
    if s4e_donewhen_is_placeholder "$dw"; then
        if s4e_donewhen_hidden_elsewhere "$b"; then
            _dp_why="column-0 is still a placeholder AND another DONE-WHEN: line sits elsewhere in the file (${_dhe_lines//$'\n'/ | }) -- refusing to guess which is the contract, hoist or relabel it"
        else
            _dp_why="the DONE-WHEN is still the mint placeholder, not a command"
        fi
        return 2
    fi
    s4e_donewhen_is_noop "$dw" && { _dp_why="the DONE-WHEN certifies nothing (a decorated shell no-op)"; return 2; }
    s4e_donewhen_multiple_contracts "$b" && { _dp_why="the baton carries a SECOND column-0 DONE-WHEN: (line(s) $_dwm_lines) that is a real command and has never run -- refusing to guess which line is the contract, hoist or relabel one"; return 2; }
    s4e_donewhen_unterminated_heredoc "$dw" && { _dp_why="the DONE-WHEN opens a heredoc it never closes -- bash would treat the body as EMPTY and exit 0 having run nothing, which is a false green, so this is COULD-NOT-MEASURE and never a verdict"; return 2; }
    s4e_donewhen_needs_compiler "$dw" && { _dp_why="$_gca_why"; return 2; }   # S4E-GUARD-COMPILER-ABSENT
    to="$(s4e_dispatch_timeout)"
    # ⛔ S4E_HOME EXPORTED, exactly as `done` does it: 11 live rows write `cd "$S4E_HOME/SCRIP" && ...` and
    # without the export that expands to the empty string and runs `cd /SCRIP`. A probe whose verdict depended
    # on the caller's environment would be worse than no probe -- it would close rows on some seats and not others.
    _dp_out="$(S4E_HOME="$S4E" timeout "$to" bash -c "$dw" 2>&1)"; rc=$?
    case "$rc" in 124|137) _dp_why="did not finish within ${to}s (probe budget, not the row's fault)"; return 2;; esac
    [ "$rc" = 0 ] && { _dp_why="exited 0"; return 0; }
    _dp_why="exited $rc"; return 1; }
# ⛔⭐ STATIC, PRE-CLAIM PLACEHOLDER REFUSAL (row mint-dispatch-refuses-a-placeholder-done-when, ceo
# CEO-286 2026-09-05): `next` and `assign` used to DISPATCH a row whose DONE-WHEN is still the mint
# boilerplate -- s4e_dispatch_probe already answers "could not measure" (rc=2) for it, correctly, but
# both sides then SERVED the row anyway, indistinguishable from any other rc=2 (a slow criterion, a
# missing compiler). A row opened onto an unmeasurable criterion can never be CLOSED by any amount of
# correct work (`done` executes the DONE-WHEN line whole, and prose is not a command), so serving it is
# strictly worse than refusing: two rows were opened onto exactly this shape on 2026-09-05 alone.
# ⭐ THIS CHECK IS SAFE BEFORE A CLAIM EXISTS, UNLIKE s4e_dispatch_probe: s4e_donewhen_is_placeholder is a
# pure TEXT match on the baton (no execution), so reading it twice or before anyone holds the row changes
# nothing about the answer -- there is no race to protect against, the way there is for actually RUNNING
# the criterion (s4e_dispatch_gate's own comment: probed after the claim, so two seats cannot race the
# same probe). Calling this before the claim is written is what makes "NO claim file written" possible.
s4e_predispatch_placeholder_check() {   # $1 = topic; rc 0 = placeholder (refuse, do not claim), 1 = proceed; sets $_ppc_why
    local t="$1" b="$PO/tasks/$1.task.md" dw
    _ppc_why=""
    [ -f "$b" ] || return 1
    dw="$(s4e_donewhen_text "$b")"
    [ -n "$dw" ] || return 1
    if s4e_donewhen_is_placeholder "$dw"; then
        _ppc_why="the DONE-WHEN is still the mint placeholder, not a command -- $b needs a real DONE-WHEN before this row can be dispatched"
        return 0
    fi
    return 1; }
# ⛔⭐ THE DISPATCH GATE, CALLED FROM EVERY PATH IN PASS 3 THAT SERVES A ROW -- rc 0 = serve it, 1 = it was
# closed, take the next one. ⛔ IT IS A FUNCTION BECAUSE PASS 3 SERVES FROM **TWO** PLACES AND THE FIRST DRAFT
# OF THIS CURE ONLY WIRED ONE: the ordinary claim-and-serve at the bottom, and the DEPENDENCY INVERSION
# promotion that serves a blocked row's blocker at the blocked row's rank. MEASURED, and it is why this is not
# inline: with only the ordinary path wired, test_gate_s4e_unclaim_keeps_park.sh still passed -- because the
# row it asserts on reaches the seat through the PROMOTION path, which sailed past the probe untouched. A gate
# passing for a reason unrelated to the change is exactly how a half-wired cure reads as a whole one.
# ⭐ THIS IS THE `park`/s4e_mark_row LESSON AGAIN, WRITTEN DOWN ONE FILE LATER: "a cure that enumerates its
# closing verbs silently reopens every time a new closing verb is minted." Same shape, serving side. If you add
# a third way to hand a row to a seat, it calls THIS -- do not copy the block.
s4e_dispatch_gate() {   # $1 = topic (already claimed by us), $2 = rank, for the message only
    local _dgt="$1" _dgr="$2" _dpb
    # ⛔⭐ DISPATCH PROBE (see s4e_dispatch_probe above for the measured cause). PROBED AFTER THE CLAIM,
    # DELIBERATELY: the claim is what makes the answer ours to act on. Probing first would let two seats
    # race the same probe and both close the same row, and closing a row we do not hold is the eviction
    # `park` already refuses to do. ⭐ AND BECAUSE THE CLAIM IS NOW OURS, THE PROBE IS `done` ITSELF --
    # not a reimplementation of it. That buys every guard `done` already carries, for free and without a
    # second copy to drift: the vacuity blocklist, the markdown-backtick strip, the no-baton refusal, the
    # ROWD-manifest guard that stops a close from redding `make test` fleet-wide, the S4E_HOME export,
    # and the claim/column mirroring. A close performed here is byte-for-byte a close performed by hand.
    _dp_out="$(S4E_NO_BANNER=1 S4E_DONE_TIMEOUT="$(s4e_dispatch_timeout)" "$0" done "$_dgt" 2>&1)"; _dp_rc=$?
    if [ "${S4E_NO_DISPATCH_PROBE:-0}" = "1" ]; then _dp_rc=1; _dp_out=""; fi
    case "$_dp_rc" in
      0) # GREEN -- the row was already satisfied. `done` has closed it; record WHY in the baton and move on.
         _dpb="$PO/tasks/$_dgt.task.md"
         [ -f "$_dpb" ] && printf '\n- [%s·%s] **ALREADY SATISFIED AT DISPATCH** — the picker ran this row'"'"'s own DONE-WHEN before serving it and it exited 0, so the row was closed rather than handed to a seat. Nobody worked it; if that is wrong, the DONE-WHEN is wrong (it is passing on a tree where the work is not done) and THAT is the finding. Probe budget %ss; tree %s.\n' "$(date -u +%Y-%m-%dT%H:%MZ)" "$ME" "$(s4e_dispatch_timeout)" "$S4E" >> "$_dpb"
         printf '✅ ALREADY SATISFIED AT DISPATCH — rank-%s %s was NOT served: its own DONE-WHEN exits 0 on this tree.\n' "$_dgr" "$_dgt"
         printf '   Closed DONE (via `done`, so every close guard applied) and a ledger line says why. Looking for the next row...\n'
         printf '   ⛔ If you believe this row is real work, the DONE-WHEN is the defect -- it passes where the work is not done.\n'
         # ⛔ return 1, NOT continue. `continue` inside a function does reach the caller's loop in bash, which
         # is exactly why it is wrong here: it would work by accident from PASS 3 and silently do nothing (or
         # worse) from any future caller that is not inside a loop. The caller spells the control flow.
         return 1;;
      2) # COULD NOT MEASURE -- serve it, but never silently.
         printf '⚠ DISPATCH PROBE COULD NOT MEASURE %s -- serving it anyway, unverified.\n' "$_dgt" >&2
         printf '%s\n' "$_dp_out" | grep -m2 -E '⛔|REFUSED|did NOT FINISH' | sed 's/^/   | /' >&2
         printf '   Probe budget was %ss (S4E_DISPATCH_PROBE_TIMEOUT). A criterion too slow to PROBE is not a failing row.\n' "$(s4e_dispatch_timeout)" >&2;;
      *) # RED is the only silent outcome: it is the only one that means there is real work here.
         # ⭐ Except when `done` refused rather than measured -- it exits 1 for a vacuous criterion too,
         # so rc alone cannot tell "no work needed" from "this row can never be closed". Say so.
         if printf '%s' "$_dp_out" | grep -q 'REFUSED'; then
           printf '⚠ %s has a DONE-WHEN that can never pass -- serving the row, but it cannot be CLOSED until that is fixed.\n' "$_dgt" >&2
           printf '%s\n' "$_dp_out" | grep -m1 'REFUSED' | sed 's/^/   | /' >&2; fi;;
    esac
    return 0; }
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
# ⛔⭐ THE ONE WRITER OF QUEUE.tsv's STATE COLUMN. Factored out of `park` (which was its only caller) so that
# `done` can mirror a close through the SAME code rather than growing a second, subtly-different rewriter.
# ⭐ WHY THIS EXISTS AT ALL: dispatch state has two homes -- claims/<topic>.claim and QUEUE.tsv's column --
# and until 2026-08-28 NO verb updated the column on completion. `done` latched the claim and left the column,
# so claim=DONE / queue=FREE was the DESIGNED steady state between a close and the next MANUAL sweep. Measured
# that day: 4 rows in that state, 0 in the mirror state. The asymmetry is the point -- the loud direction (a row
# that keeps coming back) gets noticed and fixed; this one is SILENT and produces no symptom until a ruling
# lands on the row and cannot be dispatched. See PROTOCOL-V2-DRAFT § A RESERVED QUESTION GETS AN ASSIGNED OWNER.
# ⛔⭐ THE LOCK IS NOT OPTIONAL, AND ITS NAME IS A LIE (hq_B 2026-08-29, queue-column-unwritten-by-acquiring-verbs).
# QUEUE.tsv is ONE file rewritten read-all/write-tmp/cat-back here, and the cure below puts that rewrite on the
# CLAIM path -- which up to 16 seats hit at every prompt, where before it ran only on a park or a close. That is
# exactly the shape `mint` already takes $PO/.mint.lock for, so this takes THE SAME lock: two differently-named
# locks over one file are not mutual exclusion at all. Reusing .mint.lock rather than renaming it is deliberate --
# a rename silently loses exclusion against any seat whose clone still has the old name, which is the stale-clone
# class this file convicts elsewhere. Renaming it to .queue.lock fleet-wide is a separate, later row.
# ⛔⭐ AND IT FAILS OPEN, NEVER CLOSED. A column write that cannot get the lock WARNS and returns 1; it must never
# abort a claim or a dispatch. The claim file is the authority and the picker reads THAT -- the column is only its
# readable twin, so a missed write costs legibility, while failing closed here would cost the fleet its dispatch.
s4e_set_row_state() {   # <topic> <state>  -- no-op (rc 1) when the topic has no live QUEUE.tsv row
  local _t="$1" _s="$2" _q="$PO/QUEUE.tsv" _tmp _lk="$PO/.mint.lock" _got=0 _i _rc=0
  # ⛔ STRUCTURAL INTEGRITY BELONGS AT THE PRIMITIVE, VOCABULARY AT THE VERB. `park` validates the state against
  # the known vocabulary; here we refuse only what would TEAR the file, because this function is reached by every
  # writer (claim/assign/unclaim/done/park) and a tab in the state column splits the row for every reader. A
  # vocabulary check here would instead risk refusing a legitimate value some future verb computes.
  case "$_s" in *$'\t'*|*$'\n'*) printf '⛔ REFUSED: state for %s contains a tab/newline; that would tear the QUEUE.tsv row. Not written.\n' "$_t" >&2; return 1;; esac
  grep -qP "^[0-9]+\t\Q$_t\E\t" "$_q" 2>/dev/null || return 1
  for _i in $(seq 1 20); do mkdir "$_lk" 2>/dev/null && { _got=1; break; }; sleep 0.1; done
  [ "$_got" = 1 ] || { printf '⚠ QUEUE.tsv state column NOT updated for %s (lock busy 2s). The CLAIM is the authority and dispatch is unaffected; only its readable twin is stale.\n' "$_t" >&2; return 1; }
  _tmp="$(mktemp)"
  if awk -F'\t' -v OFS='\t' -v t="$_t" -v s="$_s" '$2==t&&NF>3{$4=s} {print}' "$_q" > "$_tmp"; then cat "$_tmp" > "$_q" || _rc=1; else _rc=1; fi
  rm -f "$_tmp"; rmdir "$_lk" 2>/dev/null
  return $_rc
}
# ⭐⭐ NO BLANK OWNER CELL, THE OTHER HALF (row next-serves-a-seat-only-rows-in-its-hqs-lane-and-no-row-
# carries-a-blank-owner-cell). Column 3 stays the UMBRELLA-HQ/lane, never the working seat's own name --
# see the "COLUMN 3 IS DELIBERATELY NOT TOUCHED" note at claim's own acquisition site, which documents col3
# hq_B + claim seat12 as a MEANINGFUL pair (umbrella-HQ + working-seat), not drift to be cleaned. So this
# ONLY FILLS AN EMPTY CELL — never overwrites an existing value, deliberate or not; an already-set col3
# stays "a question routed to hq_P/ceo", exactly as that note already ruled. Writes the ASSIGNEE's OWN lane
# (its postoffice HQ file, or itself if the assignee IS an HQ) — never the assignee's bare name, which would
# be the exact drift the existing note warns against.
s4e_backfill_owner_lane() {
  local _t="$1" _seat="$2" _q="$PO/QUEUE.tsv" _row _cur _lane
  _row="$(qrow "$_t")"; [ -n "$_row" ] || return 1
  _cur="$(printf '%s' "$_row" | cut -f3)"
  case "$_cur" in ''|unassigned) : ;; *) return 0;; esac   # already set -- not this function's decision to change
  case "$_seat" in
    hq_C|hq_B|hq_P|hq_T|hq_U|hq_S|hq_I|hq_R) _lane="$_seat" ;;
    seat*) [ -f "$PO/$_seat/HQ" ] && _lane="$(head -1 "$PO/$_seat/HQ" | tr -d '[:space:]')" ;;
  esac
  case "${_lane:-}" in hq_C|hq_B|hq_P|hq_T|hq_U|hq_S|hq_I|hq_R) : ;; *) return 1;; esac   # undeterminable -- leave blank rather than guess
  local _lk="$PO/.mint.lock" _got=0 _i _tmp
  for _i in $(seq 1 20); do mkdir "$_lk" 2>/dev/null && { _got=1; break; }; sleep 0.1; done
  [ "$_got" = 1 ] || return 1
  _tmp="$(mktemp)"
  awk -F'\t' -v OFS='\t' -v t="$_t" -v o="$_lane" '$2==t&&NF>3&&($3==""||$3=="unassigned"){$3=o} {print}' "$_q" > "$_tmp" && cat "$_tmp" > "$_q"
  rm -f "$_tmp"; rmdir "$_lk" 2>/dev/null
}
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
# ⭐ ONE PROTOCOL NUMBER, HOISTED (ceo 2026-08-29, row s4e-clear-needs-protocol-version-guard): next() and clear() both
# guard on it now, and two inline copies of one value is the drift class this file convicts elsewhere. Bump it HERE and
# in the shared PROTOCOL-VERSION file together.
# ⛔ BUMPED 4 -> 5 (ceo ruling 2026-08-29, on hq_B's ask; "the word is given"). WHY A BUMP RATHER THAN A
# CADENCE: the state-column cure (d1ae2d30) lives in EACH SEAT'S OWN s4e_msg.sh, so a clone that has not
# pulled keeps writing lying FREE rows at every claim. Measured after the one-time reconcile of 18 rows: the
# A count went to 1 within minutes, and that one row was minted by a seat whose clone predates the cure. So
# reconciling on a cadence is symptom management -- the drift regenerates faster than a human sweeps it.
# Raising this number makes next() REFUSE to dispatch from any clone below it, which forces the pull before
# the next serve. That is what the guard was built for (seat09 79 commits behind, seat13 2, both silently
# running v1's picker).
# ⛔⛔ ORDERING IS LOAD-BEARING AND IS THE ONE WAY THIS GOES WRONG: this commit must be PUSHED BEFORE
# $PO/PROTOCOL-VERSION is raised to 5. The shared file is what seats compare against; raise it first and
# every seat is refused work while the commit that would fix them does not yet exist on origin.
S4E_PROTO=5
case "$cmd" in
  send)  to="$(s4e_canon "${2:?to}")"; topic="${3:?topic}"; shift 3; s4e_assert_box "$to" destination; s4e_assert_not_drained "$to"
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
         # ⛔⭐ SHAPE GUARD, NOT A DAMAGE DETECTOR (hq_T, task send-executes-backticks-in-a-message-body...,
         # on hq_C's + hq_P's same-hour hits above). A body that ALREADY lost text to an intervening shell's
         # command substitution arrives here with the backtick/$( already gone -- there is no residue to
         # catch, which is exactly why the --stdin heredoc path above exists as the safe INPUT route rather
         # than relying on a validator. This guard cannot undo that. What it CAN do: refuse every body that
         # still carries a literal backtick or $( by the time it reaches here, unconditionally -- whether
         # that survivor is inert (single-quoted, or a quoted heredoc) or is itself a landmine for the NEXT
         # shell hop that re-embeds this text unquoted. FAIL-ONCE BOTH WAYS is the spec: a literal backtick
         # or $( always refuses, an ordinary body always still sends -- no exception for --stdin, because a
         # sender who genuinely needs literal shell-like text has no safe way to prove it arrived undamaged.
         case "$_body" in
           *'`'*|*'$('*) echo "⛔ REFUSED: message body contains a backtick or \$(. An intervening shell (yours, or whatever built this command) silently command-substitutes these in transit, so send cannot tell safe literal text from the surviving half of an already-mangled message -- it refuses on the SHAPE either way, with no bypass. Rewrite the body without a literal backtick or \$(. For text that must contain shell-like syntax, describe it in words instead of pasting it live." >&2
             exit 1;;
         esac
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
         # ⛔⭐ N-guard (ceo 2026-08-29, row s4e-clear-needs-protocol-version-guard, on hq_P's measured hole): clear gets the
         # SAME refusal next has. The archive cure at the mv below protects only clones that carry it — hq_B's stale clone
         # ran the old rm path and destroyed 5 read messages with archive/=0 — and the seats most likely to destroy mail
         # are exactly the stale ones. This guard reaches them through the shared PROTOCOL-VERSION bump: their NEXT already
         # refuses on version, forcing the pull that brings both the archive cure and this guard. INTERIM LAW (until the
         # fleet is fully on protocol >= 4): archive/ counts are NOT ground truth for what was cleared.
         if [ -f "$PO/PROTOCOL-VERSION" ]; then
           need="$(head -1 "$PO/PROTOCOL-VERSION" | tr -cd '0-9')"; need="${need:-0}"
           if [ "$S4E_PROTO" -lt "$need" ]; then
             echo "⛔ REFUSING TO CLEAR — your s4e_msg.sh is PROTOCOL $S4E_PROTO, the fleet is on $need." >&2
             echo "   A stale clone's clear can DESTROY mail (the pre-archive rm path). Nothing was touched." >&2
             echo "   Fix, then re-run:  cd \"$S4E/SCRIP\" && git pull --rebase origin main" >&2
             exit 3; fi; fi
         [ -f "$lc" ] || { echo "⛔ REFUSED: nothing has been read in this seat. Run 'check' first -- clear only removes what check displayed." >&2; exit 2; }
         _cl=0; _kept=0; _fail=0
         for f in "$d"/*.msg; do [ -f "$f" ] || continue; _b="$(basename "$f")"
           if grep -qxF "$_b" "$lc" 2>/dev/null; then
             # ⛔⭐⭐ ARCHIVE, NEVER DESTROY (hq_P 2026-08-28, seat06's witness, ceo-granted).  `.last-check` is written by
             # check's ITERATION LOOP -- one basename per file, unconditionally -- so it records what check ITERATED, never
             # what the reader actually SAW.  This line used to be `rm -f "$f"`, so ANY truncation between check's `cat` and
             # the reader (a pipe through head/tail, a pager, a context clip) destroyed the body permanently while the seat
             # believed it had read its mail.  MEASURED, not hypothetical: it destroyed a message from hq_P to seat06, and
             # one of ceo's the same morning.  ⭐ Note the ORTHOGONAL axis was already guarded -- clear refuses to delete
             # post-check arrivals (KEPT UNREAD below) -- so the arrival race was thought about; this is the other axis.
             # ⛔ AND A FAILED ARCHIVE KEEPS THE MAIL: falling back to rm would reinstate the exact defect being cured.
             if mkdir -p "$PO/$ME/archive" 2>/dev/null && mv -f "$f" "$PO/$ME/archive/$_b" 2>/dev/null; then _cl=$((_cl+1))
             else _fail=$((_fail+1)); echo "⛔ COULD NOT ARCHIVE -- MESSAGE KEPT, NOT DESTROYED: $_b"; fi
           else _kept=$((_kept+1)); echo "⛔ KEPT UNREAD (arrived after your last check): $_b"; fi; done
         rm -f "$lc"
         if [ "$_fail" -gt 0 ]; then echo "[$ME] archived $_cl, KEPT $_kept UNREAD, ⛔ $_fail COULD NOT BE ARCHIVED (still in your inbox)"; exit 1
         elif [ "$_kept" -gt 0 ]; then echo "[$ME] archived $_cl, KEPT $_kept UNREAD -- run 'check' again before you stop"; exit 1
         else echo "[$ME] inbox cleared ($_cl archived to $ME/archive/ -- a truncated read is now recoverable, not lost)"; fi;;
  # ⭐ CREATION IS A DELIBERATE ACT WITH A NAME (V2-4). LAW 6 forbids mailboxes appearing as a side effect of a
  # typo, not mailboxes existing -- Lon adds seats, and a fleet that cannot enrol one is not operable. So the
  # capability survives as ONE explicit subcommand that says what it did, and every implicit mkdir is gone.
  mailbox) nm="$(s4e_canon "${2:?mailbox name}")"
         case "$nm" in ""|*/*|*$'\n'*|.*) echo "⛔ REFUSED: mailbox name must be a plain slug" >&2; exit 2;; esac
         if [ -d "$PO/$nm/inbox" ]; then echo "mailbox $nm already exists"; else mkdir -p "$PO/$nm/inbox" && echo "created mailbox $nm (deliberate, by $ME)"; fi;;
  claim) topic="${2:?topic}"; c="$PO/claims/$topic.claim"; mkdir -p "$PO/claims"
         if [ -f "$c" ]; then own="$(head -1 "$c")"; if [ "$own" = "$ME" ]; then echo "already yours"; else echo "CLAIMED by $own — pick other work"; exit 1; fi
         else t="$(mktemp "$PO/claims/.c.XXXXXX")"; echo "$ME" > "$t"
              if ln "$t" "$c" 2>/dev/null; then rm -f "$t"
                # ⭐ picker-dependency-and-boomerang-blindness CURE 2: an explicit claim IS the "another seat
                # touched it" event that ends the boomerang cooldown early. Cleared unconditionally, including
                # for the releaser's own deliberate re-claim — `claim` is a chosen act, `next` is a serve, and
                # the guard exists only to stop the SERVE. Never let this cure outlive a human decision.
                s4e_release_clear "$topic"
                # ⭐ queue-column-unwritten-by-acquiring-verbs (hq_B 2026-08-29, on hq_P's queue-wide audit).
                # THE COLUMN LEARNED ABOUT ENDINGS AND NEVER ABOUT BEGINNINGS: park, done and unclaim all wrote it,
                # while claim, `next` and assign -- every verb that TAKES a lock -- wrote only the claim file. Measured
                # before the cure: of 16 rows holding a live claim, 15 read FREE. That is not a column three verbs
                # forgot, it is a column no ACQUIRING verb ever wrote. Enforcing it HERE and not per-verb is hq_P's
                # sharpening and is the load-bearing part: `next` reaches its lock through this line (so does the
                # dependency-inversion promo), so one write covers the primary dispatch path and any future verb that
                # claims through the primitive instead of re-implementing it.
                # ⛔ COLUMN 3 IS DELIBERATELY NOT TOUCHED. serve() prints it as "owner", but on the live queue it
                # carries an HQ on rows a SEAT holds (corpus-crosscheck-probe-total-conversion: col3 hq_B, claim
                # seat12) -- which reads as umbrella-HQ + working-seat, not as drift. Writing the claimant there
                # would destroy that, so col3 stays a question routed to hq_P/ceo, not a field this cure assumes.
                # ⭐ REMEMBER A RESTRICTION BEFORE OVERWRITING IT. The column must still learn about beginnings
                # (hq_B's cure: 15 of 16 claimed rows read FREE before it), so a claimed RESTRICTED row still reads
                # CLAIMED:<x> -- but unclaim would then have no way to know the row was ever restricted, and would
                # correctly-but-destructively drive it to FREE. The claim file is the natural place: it already
                # carries the owner and the DONE/OVERRIDE-BY receipts, it is deleted with the lock, and it cannot
                # outlive the episode it describes. ⛔ ONLY RESTRICTED is remembered: an ASSIGNED dispatch really is
                # spent on release and must NOT be restored (that is ceo's 2026-08-28 ruling, still governing).
                { _pre="$(s4e_row_state "$topic")"; if s4e_restricted_to "$_pre" >/dev/null; then printf 'PRIOR-STATE %s\n' "$_pre" >> "$c"; fi; }
                s4e_set_row_state "$topic" "CLAIMED:$ME" || true
                echo "claimed $topic"; else rm -f "$t"; echo "RACE LOST: $(head -1 "$c" 2>/dev/null) owns it"; exit 1; fi; fi;;
  unclaim) # ⭐ s265 — RELEASE AN UNWORKED CLAIM. Minted because THREE seats hit its absence in one day (seat08,
         # seat09, seat13): a stale-clone picker mis-locked a row, the seat correctly refused to work it, and then had
         # NO WAY TO PUT IT BACK. `done` was the only exit and `done` is COMPUTED — closing a row you never worked
         # would have to defeat the DONE-WHEN gate, which is the false-green trap the gate exists to stop. So seats
         # held locks on rows they were not working, and a claimed row hides itself from the picker: one stale clone
         # silently removed a row from the whole fleet's reach. ⛔ REFUSES a claim that is DONE (that is a receipt, not
         # a lock) and one you do not own. The release is APPENDED to the baton's LEDGER, so a lock that was taken and
         # returned leaves a trace instead of vanishing.
         topic="${2:?topic}"; c="$PO/claims/$topic.claim"; q="$PO/QUEUE.tsv"
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
         # ⭐ unclaim-leaves-row-invisible-column-not-freed (ceo, 2026-08-28, on hq_P's icon-n2 measurement) —
         # unclaim removed only the claim FILE and never touched QUEUE.tsv's state column, so a released row
         # stayed ASSIGNED/ASSIGNED:<seat> and next()'s PASS 3 (which serves only FREE|'') never saw it again —
         # the exact "half the hiding" this case's own header already named. Mirrors `park`'s write, same awk
         # shape. The column is always driven to FREE, even over an ASSIGNED-BY state: an assignment is spent
         # the instant its lock is released, never restored to what the assigner intended (ruling: back to FREE).
         # A row whose column already reads FREE is simply rewritten FREE -> FREE: a no-op, not an error.
         # ⭐ WAS A SECOND, INLINE COPY of the awk in s4e_set_row_state -- the exact "second, subtly-different
         # rewriter" that function's own header says it was factored out to prevent, grown back four lines below it.
         # Identical behaviour (col4 -> FREE), now through the one writer, so it inherits the lock instead of racing.
         # ⭐ A RESTRICTION IS NOT SPENT BY LETTING GO OF THE LOCK -- restore it rather than freeing the row.
         # Everything else still goes to FREE, unchanged: ceo's 2026-08-28 ruling governs DISPATCH and is untouched.
         # ⭐ unclaim-clobbers-a-park-written-around-the-claim-so-next-serves-a-superseded-row (ceo MEASURED 2026-09-02 ~14:55,
         # cured hq_B the same day): ceo force-parked AROUND hq_B's live claim (S4E_PARK_FORCE=1 -> SUPERSEDED:folded-into-...),
         # hq_B's unclaim then drove the column to FREE by the unconditional write below, erasing the park, and ceo's own
         # `next` LOCKED the resurrected rank-0 superseded row as topmost work -- twice, because the second unclaim did it
         # again. ⛔ A PARK WRITTEN AROUND A CLAIM IS NOT SPENT BY THE CLAIM'S RELEASE. The 2026-08-28 "always FREE" ruling
         # governs DISPATCH of the CLAIM-SHAPED states only -- CLAIMED:<seat>, ASSIGNED, ASSIGNED:<seat>, FREE, empty -- and is
         # untouched for them. Any other column value (SUPERSEDED*, BLOCKED-ON:*, PARKED-AWAITING:*, PARKED*, RETIRED,
         # GRANT-NEEDED*, RESTRICTED:*) is a park somebody wrote deliberately around the lock: it is KEPT and the receipt
         # names it. Gate: test_gate_s4e_unclaim_keeps_park.sh (both arms, throwaway postoffice, fail-once on a mutated copy).
         { _rst="$(grep -m1 '^PRIOR-STATE ' "$c" 2>/dev/null | sed 's/^PRIOR-STATE //')"; _cur="$(s4e_row_state "$topic")"; _kept=""
           case "$_cur" in
             CLAIMED:*|ASSIGNED|ASSIGNED:*|FREE|"")
               if [ -n "$_rst" ] && s4e_restricted_to "$_rst" >/dev/null; then s4e_set_row_state "$topic" "$_rst" || true
               else s4e_set_row_state "$topic" "FREE" || true; fi;;
             *) _kept="$_cur"; echo "⭐ QUEUE.tsv column KEPT as '$_cur' — a park written around this claim survives its release (only a claim-shaped column is driven to FREE)";;
           esac
           [ -z "$_kept" ] || why="$why (QUEUE.tsv column kept as $_kept)"; }
         b="$PO/tasks/$topic.task.md"
         # release-verbs-refuse-on-unfolded-authority-mail: MUST run before anything below touches the
         # baton's own mtime -- the guard's whole signal is "was the baton written since the mail
         # arrived", so checking it after this verb's own RELEASED-line append would always see a
         # freshly-touched file and could never fire. WARN, never block (a stranded seat is worse than
         # a stale flag) -- but write the pointer INTO the baton, since a message in this terminal is
         # exactly what already failed to reach the artifact twice before.
         if s4e_unfolded_authority_mail "$ME" "$topic"; then
             echo "⚠ authority mail arrived after this claim was taken and the baton was not touched since -- appended a pointer to it, re-read before trusting this row's own NEXT block" >&2
             s4e_release_guard_note "$topic" unclaim "$ME"
         fi
         [ -f "$b" ] && printf '\n- %s **RELEASED** by %s — %s (claim removed; row returns to the picker)\n' "$(date -u +%Y-%m-%dT%H:%MZ)" "$ME" "$why" >> "$b"
         s4e_mark_row "$topic" RELEASED
         # ⭐ picker-dependency-and-boomerang-blindness CURE 2 — leave a RECEIPT so the picker can tell "this row
         # is free" from "this row is free BECAUSE I just let it go". Written after every refusal above has been
         # passed, so a REFUSED unclaim leaves no receipt and cannot hide a row from anyone.
         s4e_release_receipt "$topic"
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
         # ⛔⭐ ARG 3 IS THE STATE, NOT A REASON — AND IT USED TO ACCEPT ANY STRING AT ALL (hq_P, 2026-08-29,
         # reported against themselves). They ran `park <topic> "<a whole explanatory sentence>"` on the natural
         # assumption that the third argument was a reason, and the entire sentence — commas and all — was written
         # verbatim into QUEUE.tsv's load-bearing state column. They caught it on the verify line inside a minute,
         # so no harm; but the shape is the point. ⛔ THE NEAR MISS IS THE REAL FINDING: their prose happened to
         # contain no TAB. One that did would have SPLIT THE ROW and broken the TSV for every reader, not merely
         # uglified one cell — and a torn row in an unversioned file 16 seats dispatch from has no undo.
         # ⭐ AND AN UNVALIDATED STATE FAILS TOWARD INVISIBILITY: PASS 3 serves only FREE|'', so ANY typo — a
         # reason, a misspelling, a stray quote — silently removes the row from the picker while looking like a
         # deliberate park. The vocabulary below is the MEASURED one (every distinct value live in QUEUE.tsv),
         # not an invented one, plus CLAIMED: which the claim primitive now writes.
         case "$st" in
           *$'\t'*|*$'\n'*) printf '⛔ REFUSED: a state may not contain a tab or newline — that would TEAR the QUEUE.tsv row, not just mis-set it.\n' >&2; exit 2;;
         esac
         case "$st" in
           FREE|PARKED|BLOCKED|ASSIGNED|DONE|SUPERSEDED|RETIRED|GRANT-NEEDED|PARKED-LON-HOLD) : ;;
           PARKED-AWAITING:?*|BLOCKED-ON:?*|ASSIGNED:?*|CLAIMED:?*|RESTRICTED:?*|DONE:?*|SUPERSEDED:?*|GRANT-NEEDED:?*|PARKED-UMBRELLA:?*|PARKED-LON-HOLD:?*) : ;;
           *) printf '⛔ REFUSED: "%s" is not a state. ARG 3 IS THE STATE COLUMN, NOT A REASON — that is the mistake this guard exists for.\n' "$st" >&2
              printf '   Accepted: FREE PARKED BLOCKED ASSIGNED DONE SUPERSEDED RETIRED GRANT-NEEDED PARKED-LON-HOLD\n' >&2
              printf '             PARKED-AWAITING:<topic>  BLOCKED-ON:<topic>  ASSIGNED:<seat>  CLAIMED:<seat>\n' >&2
              printf '             RESTRICTED:<seat>   (only <seat> may ever work it; survives unclaim -- for CEO-19 HQ-only rows)\n' >&2
              printf '             DONE:<why>  SUPERSEDED:<why>  GRANT-NEEDED:<why>  PARKED-UMBRELLA:<why>\n' >&2
              printf '   ⭐ Put the REASON in the baton (%s/tasks/%s.task.md) — the queue is an INDEX, not a brief store.\n' "$PO" "$topic" >&2
              printf '   ⭐ To make a park SELF-CLEARING, spell it PARKED-AWAITING:<blocker> or BLOCKED-ON:<blocker>;\n' >&2
              printf '      next() re-asks whether that blocker is DONE and un-parks this row by itself.\n' >&2
              exit 2;;
         esac
         grep -qP "^[0-9]+\t\Q$topic\E\t" "$q" || { echo "⛔ no QUEUE.tsv row named $topic"; exit 1; }
         # ⛔⭐ picker-dangling-blocker-parks-a-row-forever-in-silence (hq_B mint, cured here). A BLOCKED-ON:/
         # PARKED-AWAITING: value naming a topic with NO row (live or swept-done) can never self-clear: the
         # promotion walk's own dangling branch ("no row exists to serve") and the self-heal check
         # (s4e_blocker_done, which needs a claim/QUEUE.done.tsv hit) both just silently return false forever
         # for a name nothing ever refers to. MEASURED (hq_B, queue-wide sweep): pz4 parked behind
         # "icon-n2-generator-activation-frames-items-3-4" — a sub-item gloss appended to a real row name,
         # itself not a row — with 3 more rows parked behind pz4, all silently permanent. Refuse the bad
         # state before it can be written, the same way `s4e_assert_box` refuses an invented mailbox instead
         # of creating one on the fly (LAW 6, same failure shape: an invented identity that silently succeeds).
         case "$st" in
           BLOCKED-ON:*|PARKED-AWAITING:*)
             _blk="${st#*:}"
             if [ -z "$(qrow "$_blk")" ] && ! s4e_blocker_done "$_blk"; then
               printf '\n⛔⛔⛔ REFUSED: %s names blocker "%s", which has no QUEUE.tsv row (live or swept-done) ⛔⛔⛔\n' "$st" "$_blk" >&2
               printf '    This state can never self-clear — s4e_blocker_done has nothing to ever find true for a\n' >&2
               printf '    name that is not a real row, so the block would be PERMANENT and SILENT.\n' >&2
               printf '    If you meant a SUB-ITEM of an existing row, name the ROW itself here and put the\n' >&2
               printf '    sub-item detail in the baton prose (## NEXT), not in the state string.\n' >&2
               printf '    If the blocker genuinely does not exist yet: mint it first (s4e_msg.sh mint).\n\n' >&2
               exit 2
             fi ;;
         esac
         # ⭐ s4e-park-adds-a-blocker-and-refuses-an-unresolvable-topic — A RE-PARK ADDS A BLOCKER, IT DOES NOT
         # REPLACE ONE. seat11's correct park (BLOCKED-ON:calling-convention-depth-tracked, the row that actually
         # cured it) was overwritten twice by later re-parks recording a DIFFERENT hypothesis, so when the true
         # blocker landed DONE nothing was left naming it and the row could not self-clear — eight seats released
         # it unworked. If this row is ALREADY BLOCKED-ON:/PARKED-AWAITING: something and the new state is ALSO
         # one of those, merge the new blocker into the existing '+'-joined set (deduped) instead of overwriting
         # it — the row then self-clears the moment ANY recorded blocker resolves (next()'s PASS 3), which is
         # sound: each '+' member is a real, independently-plausible reason, and one of them being true is
         # enough to re-open dispatch, exactly the case a wrong re-park used to erase.
         case "$st" in
           BLOCKED-ON:*|PARKED-AWAITING:*)
             _cur_st="$(s4e_row_state "$topic")"
             case "$_cur_st" in
               BLOCKED-ON:*|PARKED-AWAITING:*)
                 _old="${_cur_st#*:}"; _new="${st#*:}"
                 _dup=0; _save_IFS="$IFS"; IFS='+'
                 for _b in $_old; do [ "$_b" = "$_new" ] && _dup=1; done
                 IFS="$_save_IFS"
                 if [ "$_dup" = 0 ]; then st="BLOCKED-ON:${_old}+${_new}"; else st="BLOCKED-ON:${_old}"; fi
                 echo "  (additive park: was $_cur_st, now $st)"
                 ;;
             esac ;;
         esac
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
             # release-verbs-refuse-on-unfolded-authority-mail: check BEFORE the claim (and its timestamp)
             # is removed below. WARN, never block, same reasoning as unclaim's own wiring.
             if s4e_unfolded_authority_mail "$ME" "$topic"; then
                 echo "⚠ authority mail arrived after this claim was taken and the baton was not touched since -- appended a pointer to it, re-read before trusting this row's own NEXT block" >&2
                 s4e_release_guard_note "$topic" park "$ME"
             fi
             rm -f "$c"; echo "  (cleared my own holding claim — the state column carries this now, not a lock)"
             # ⛔ park-does-not-write-last-row-so-banner-misattributes-the-session (seat07 2026-09-01, cured hq_B same day).
             # s4e_mark_row was called from exactly two closing verbs, unclaim and done; park ALSO closes a row (it just
             # cleared the claim above) but was minted later and never joined that enumeration, so a session that ended
             # by parking got a Stop banner attributing it to whatever row this seat last RELEASED or DONE'd -- days old,
             # cumulative commit count and all. Measured live by seat07 (pass 39) and reproducible: park as the last act,
             # then banner. ⭐ THE CLASS, in seat07's words, is that "a cure that enumerates its closing verbs silently
             # reopens every time a new closing verb is minted" -- so if you add a verb that removes a claim file, it
             # must call s4e_mark_row too, and the durable shape is one s4e_close_claim writer that does both.
             s4e_mark_row "$topic" PARKED
         fi
         # ⭐ picker-dependency-and-boomerang-blindness CURE 3 — A GOVERNANCE GATE IS NOT LIFTED BY `park FREE`.
         # An ordinary park is routing and anyone may lift it. A GRANT-NEEDED/PARKED-LON-HOLD row waits on a
         # grant only Lon can give, and `perf-nv-set-fn-o0-overhead` proves what happens when the two are the
         # same verb: the row goes back to FREE, the picker serves it, and the next seat re-pays the discovery
         # cost against a wall that has not moved. The gate is lifted by the grant ARRIVING, so lifting it
         # REQUIRES NAMING THE GRANT — S4E_GRANT_BY is recorded into the baton, which is what makes the lift
         # auditable instead of a keystroke. ⛔ Moving BETWEEN grant-wait spellings is not a lift and is allowed.
         cur_st="$(s4e_row_state "$topic")"
         if s4e_is_grant_wait "$cur_st" && ! s4e_is_grant_wait "$st"; then
           if [ -z "${S4E_GRANT_BY:-}" ]; then
             printf '⛔ REFUSED: %s is GOVERNANCE-GATED (%s) — it waits on a GRANT, not on work.\n' "$topic" "$cur_st" >&2
             printf '   `park <topic> FREE` lifts an ordinary park. A grant-wait is lifted only when the grant ARRIVES,\n' >&2
             printf '   and whoever lifts it must say whose word lifted it (RULES.md: you are the only witness to what Lon told you):\n' >&2
             printf '     S4E_GRANT_BY="Lon, in-chat 2026-08-28" bash %s park %s %s\n' "$0" "$topic" "$st" >&2
             exit 1
           fi
           printf '⭐ GRANT RECORDED for %s (was %s) — granted by: %s\n' "$topic" "$cur_st" "$S4E_GRANT_BY"
           _gb="$PO/tasks/$topic.task.md"
           [ -f "$_gb" ] && printf '\n- %s **GOVERNANCE GATE LIFTED** by %s — gate was `%s`; grant: %s\n' "$(date -u +%Y-%m-%dT%H:%MZ)" "$ME" "$cur_st" "$S4E_GRANT_BY" >> "$_gb"
         fi
         s4e_set_row_state "$topic" "$st"
         b="$PO/tasks/$topic.task.md"
         [ -f "$b" ] && printf '\n- %s **STATE -> %s** by %s\n' "$(date -u +%Y-%m-%dT%H:%MZ)" "$st" "$ME" >> "$b"
         echo "$topic state -> $st";;
  done)  topic="${2:?topic}"; c="$PO/claims/$topic.claim"
         # ⛔ THE BANNER FIRES ITSELF HERE (HQ 2026-08-22, after seat4 finished its row and gave NO banner until Lon
         # asked for one). LAW 15 lived only as a step in the seat's CLAUDE.md -- and a step in a markdown file is a
         # hope, not a mechanism, exactly like the inbox before `check` was forced. A seat that closes a row runs
         # `done`, so `done` prints the banner. Same reason `board` does. Suppress with S4E_NO_BANNER=1.
         if [ -f "$c" ] && [ "$(head -1 "$c")" = "$ME" ]; then
              # ⛔⭐⭐ ROWD-CLOSE GUARD -- fires for BOTH the computed close and the S4E_DONE_OVERRIDE close, because it
              # sits ABOVE the fork. An override is an assertion by a human; it still may not red twelve other seats.
              if _cite="$(s4e_manifest_rowd_cite "$topic")"; then
                printf '⛔ REFUSED: "%s" is cited as ROWD evidence in the corpus coverage manifest, so closing it REDS\n' "$topic" >&2
                printf '   `make test` FOR EVERY SEAT until the manifest is re-pointed by hand. This has happened THREE times in one day.\n' >&2
                printf '   The citing line (SCRIP/scripts/corpus_coverage_manifest.tsv):\n       %s\n' "$_cite" >&2
                printf '   HAND OFF FIRST, then close -- in the SCRIP checkout, one of:\n' >&2
                printf '     (a) re-point the ROWD topic to a LIVE successor row that still covers that subtree, or\n' >&2
                printf '     (b) flip the subtree to GATED and name the harness script that now proves it.\n' >&2
                printf '   Commit and push that edit BEFORE re-running done.\n' >&2
                printf '   ⛔ THE MANIFEST LIVES IN SCRIP, NOT THE POSTOFFICE: this check reads YOUR checkout, so a stale\n' >&2
                printf '      clone reads a stale manifest. `git pull --rebase` in SCRIP before you close, or this guard\n' >&2
                printf '      can pass on a manifest that no longer matches origin.\n' >&2
                exit 1; fi
              # ⛔⭐⭐ DONE IS COMPUTED, NEVER DECLARED (ARCH-FLEET-CEO.md LAW 1 "NO HAND-TYPED VERDICTS", γ port).
              # Until now `done` appended the DONE marker UNCONDITIONALLY and never ran the task's DONE-WHEN --
              # so the one command whose entire job is to certify completion accepted the seat's word for it.
              # That is the exact shape of the v1 free-r10 inversion (an HQ ruling DONE on prose), sitting inside
              # the control plane written to forbid it. A law the tooling does not enforce is a hope.
              tf="$PO/tasks/$topic.task.md"
              if [ -f "$tf" ]; then
                dw="$(s4e_donewhen_text "$tf")"
                # ⛔⭐⭐ A MARKDOWN-STYLED DONE-WHEN WAS BEING RUN AS COMMAND SUBSTITUTION (seat06, 2026-08-28; FINDING
                # be4b2257). A criterion authored as `DONE-WHEN: \`bash scripts/gate.sh\`` -- backticks meant as CODE
                # STYLING, because this line lives in a MARKDOWN file -- reaches `bash -c "$dw"` as live substitution:
                # bash runs the gate, captures its stdout, and then EXECUTES THAT STDOUT AS A COMMAND. A gate printing
                # "GATE OK -- ..." on success dies `GATE: command not found`, rc=127, and the row cannot be closed.
                # ⛔ IT EXECUTES THE OUTPUT, so this is not merely a wrong exit code -- whatever a gate prints is fed
                # to a shell. ⭐ MEASURED BLAST RADIUS: 5 of 411 live task files carry a wrapped DONE-WHEN today.
                # ⭐ The SILENT case is SAFE and that was checked, not assumed: bash gives a command consisting only of
                # a substitution the SUBSTITUTION's exit status, so `\`false\`` still exits 1 -- no false green. The
                # defect is confined to criteria whose command prints on its success path.
                # ⭐ NORMALISED HERE, AT THE ONE EXTRACTION POINT, so the vacuity probe and the real run cannot disagree
                # about what the criterion IS -- two consumers reading one value, never two copies to drift.
                # ⛔ ONE matched OUTER pair only, and never a one-sided backtick: a lone ` is either real substitution
                # or a typo, and both must fail LOUDLY rather than be silently rewritten. Announced, never silent.
                [ -n "${_dw_backticked:-}" ] && printf '⚠ DONE-WHEN was wrapped in markdown backticks; stripped one matched pair before running.\n   Running: %s\n' "$dw" >&2
                if [ -z "$dw" ]; then
                  echo "⛔ REFUSED: $tf has no DONE-WHEN: line. A task with no computable completion test cannot be closed." >&2; exit 1; fi
                # ⛔⭐ REFUSE rc=2 ON AN UNTERMINATED HEREDOC -- the second, independent guard on the truncation
                # class (see s4e_donewhen_unterminated_heredoc). A criterion bash cannot finish reading is one
                # bash will happily run to a ZERO exit having executed nothing, so it must never reach the run.
                if s4e_donewhen_multiple_contracts "$tf"; then
                  printf '⛔ REFUSED(2): %s carries a SECOND column-0 DONE-WHEN: at line(s) %s.\n' "$tf" "$_dwm_lines" >&2
                  printf '   Only the FIRST has ever been executed, so closing here would certify half the contract while\n' >&2
                  printf '   the other half sits in the file where every human reader counts it as part of the bar.\n' >&2
                  printf '   Hoist the real criterion to the one DONE-WHEN: line, or relabel the other.\n' >&2
                  exit 2; fi
                if s4e_donewhen_unterminated_heredoc "$dw"; then
                  printf '⛔ REFUSED(2): %s\n' "$tf" >&2
                  printf '   Its DONE-WHEN opens a heredoc it never closes. bash does not fail on that -- it warns,\n' >&2
                  printf '   treats the body as EMPTY, and exits 0 having run nothing, so closing on it would certify\n' >&2
                  printf '   a criterion that never executed. Fix the criterion (close the delimiter, or move the\n' >&2
                  printf '   witness into a script file) before closing this row.\n' >&2
                  exit 2; fi
                # ⛔⭐ HIDDEN-CRITERION GUARD (seat13, row fifty-seven-batons-are-unclosable-because-their-
                # criterion-is-not-at-column-zero): mirrors s4e_dispatch_probe's check exactly -- two
                # consumers of one rule, never two copies to drift. See s4e_donewhen_is_placeholder /
                # s4e_donewhen_hidden_elsewhere above for the measured cause and the exact shapes.
                if s4e_donewhen_is_placeholder "$dw" && s4e_donewhen_hidden_elsewhere "$tf"; then
                  printf '⛔ REFUSED (rc=2): %s column-0 DONE-WHEN is still a placeholder, but another DONE-WHEN:-labeled\n' "$tf" >&2
                  printf '   line sits elsewhere in the same file -- the tool can never see it, so this row could stay open\n' >&2
                  printf '   forever even if that work is finished. Never guessed which one is the contract.\n' >&2
                  printf '   column 0  : %s\n' "$dw" >&2
                  printf '%s\n' "$_dhe_lines" | sed 's/^/   elsewhere : /' >&2
                  printf '   Fix: hoist the real criterion to column 0 (verified by RUNNING it, never by moving text), or\n' >&2
                  printf '   relabel the other line (e.g. "ACCEPTANCE, IN PROSE:") if it is not really a second candidate.\n' >&2
                  exit 2
                fi
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
                  # ⛔⭐ THE DEFAULT IS 3600s, AND A TIMEOUT IS A REFUSAL RATHER THAN A RED (hq_B 2026-09-03, row
                  # s4e-done-timeout-default-below-make-test-under-fleet-load, dispatched rank 0 because it blocked
                  # EVERY seat's close). The default was 900s while `make test` measures ~1100s on this box under
                  # twelve seats -- so every DONE-WHEN containing `make test` (every rung baton, most instrument rows)
                  # died at the default for a reason having NOTHING to do with the work, and said "NOT DONE -- exited
                  # 124", which reads as a red gate and sends the seat off to debug work that was already finished.
                  # ⭐ A timeout tuned to a job's MEASURED duration is not a tight bound, it is a FLAKY one: it belongs
                  # an order of magnitude above the measurement, never beside it -- the same rule the corpus runners
                  # learned when `timeout 30s` SIGTERMed a fully green board and the green board read as a hang.
                  # ⛔ AND THE TWO ANSWERS MAY NOT SHARE AN EXIT CODE. `timeout` exits 124 for a criterion that NEVER
                  # FINISHED -- "I could not measure" -- which the old arm folded into the same rc=1 as "I measured
                  # and it is red". That is precisely the fail-OPEN shape already cured in the vacuity probe twenty
                  # lines above, inside this very command: an instrument that cannot measure REFUSES rc=2, it does
                  # not report a verdict it never took. Now a caller scripting around `done` can tell them apart.
                  # ⭐ AND THE INSTRUMENT QUOTES ITS REFERENCE (RULES batch 14): every outcome prints the budget it ran
                  # under AND the wall-clock it used, so "did it just need more time?" is answered by the receipt
                  # instead of re-run and guessed at.
                  # S4E-GUARD-COMPILER-ABSENT -- ahead of the run, because there is nothing to learn from running it.
                  if s4e_donewhen_needs_compiler "$dw"; then
                    printf '  ⛔⛔ REFUSED (rc=2) — NOT MEASURED, so NOT a verdict: %s\n' "$_gca_why" >&2
                    printf '     The claim and the QUEUE column are UNCHANGED. Build, then re-run `done`.\n' >&2
                    exit 2; fi
                  _dwto="${S4E_DONE_TIMEOUT:-3600}"; _dwt0="$(date +%s)"
                  ( cd "$S4E" && S4E_HOME="$S4E" S4E_SEAT="$ME" timeout "$_dwto" bash -c "$dw" ) >"$_dwlog" 2>&1; rc=$?
                  _dwel="$(( $(date +%s) - _dwt0 ))"
                  if [ "$rc" -eq 0 ]; then
                    rm -f "$_dwlog"
                    printf '  ✅ DONE-WHEN exited 0 in %ss (timeout %ss) — completion is COMPUTED, not claimed.\n' "$_dwel" "$_dwto"
                  else
                    if [ "$rc" -eq 124 ]; then
                      printf '\n⛔ REFUSED (rc=2): the DONE-WHEN did NOT FINISH within %ss (elapsed %ss), so `done` COULD NOT MEASURE it.\n' "$_dwto" "$_dwel" >&2
                      printf '    This is NOT a red criterion, NOT a pass and NOT a skip. The claim is UNCHANGED and the row stays open.\n' >&2
                    else
                      printf '\n⛔⛔⛔ NOT DONE — the task DONE-WHEN exited %s after %ss (timeout %ss). The claim is UNCHANGED and the row stays open.\n' "$rc" "$_dwel" "$_dwto" >&2
                    fi
                    printf '    command : %s\n' "$dw" >&2
                    printf '    task    : %s\n' "$tf" >&2
                    if [ -s "$_dwlog" ]; then
                      printf '    ⭐ WHAT THE CRITERION ITSELF SAID (last 20 lines) -- read this BEFORE hypothesising:\n' >&2
                      sed -e 's/^/    | /' "$_dwlog" | tail -20 >&2
                    elif [ "$rc" -ne 124 ]; then
                      printf '    ⚠ the criterion produced NO output at all -- that is itself a clue: a silent non-zero is\n' >&2
                      printf '      usually a test that never ran (missing file, bad path, rc=127) rather than one that failed.\n' >&2
                    fi
                    rm -f "$_dwlog"
                    if [ "$rc" -eq 124 ]; then
                      printf '    Give it a real budget for THIS close:  S4E_DONE_TIMEOUT=<seconds> s4e_msg.sh done %s\n' "$topic" >&2
                      printf '    ⭐ Giving a check enough wall-clock is NOT weakening it -- raising the budget is always allowed;\n' >&2
                      printf '      editing the criterion to finish sooner is the false-green trap. A criterion that hangs FOREVER\n' >&2
                      printf '      is a finding about the criterion: say so in the LEDGER and fix the criterion, never the clock.\n\n' >&2
                      exit 2
                    fi
                    printf '    If the DONE-WHEN itself is WRONG, that is a real finding: fix it in the task file and say so\n' >&2
                    printf '    in the LEDGER, or re-run with S4E_DONE_OVERRIDE="why" which records the reason in the claim.\n' >&2
                    printf '    ⛔ Do NOT weaken a DONE-WHEN to make it pass -- that is the false-green trap this gate exists to stop.\n\n' >&2
                    exit 1; fi; fi
              else
                # ⛔⭐ HOLE A, CURED 2026-08-28 (hq_B; reproduced in a scratch postoffice before the fix). This arm
                # used to PRINT the law it was breaking -- "closing on the seat word alone, which LAW 1 forbids" --
                # and then FALL THROUGH to the unconditional DONE write below, closing the row with ZERO verification
                # and exit 0. A warning that does not change what happens is not a guard, it is a comment with
                # stdout. ⭐ AND IT WAS INVISIBLE FOR THE SAME REASON EVERY MUTE INSTRUMENT IS: the operator saw a
                # ⚠ line scroll past inside a command that then said "done", so the loud part and the wrong part
                # disagreed and the reassuring one was last. Now it REFUSES rc=2 -- a criterion that cannot be READ
                # cannot be PASSED, exactly as a test that cannot measure refuses rather than skipping as success.
                printf '⛔ REFUSED (rc=2): NO TASK BATON at %s — closing on the seat word alone is what LAW 1 forbids.\n' "$tf" >&2
                printf '   A row with no baton has no computable DONE-WHEN, so its completion cannot be verified, so it\n' >&2
                printf '   cannot be closed. This is NOT a pass and NOT a skip. Mint the baton for this topic, then close.\n' >&2
                exit 2
              fi
              grep -q '^DONE$' "$c" || echo DONE >> "$c"; echo "done $topic"
              # ⛔⭐ (a) DONE CLOSES ITS OWN WINDOW (hq_B scope, 2026-08-28, ratified by ceo as rank 1). Until now
              # this verb latched the CLAIM and never touched QUEUE.tsv -- s4e_mark_row() below writes only the
              # per-seat .last-row despite its name. So a verified close left the column reading FREE, and the row
              # became INVISIBLE IN ALL THREE PICKER PASSES (PASS 1/2 skip a DONE claim even for its owner; PASS 3
              # skips any topic that has a claim file at all) while still reading FREE to any human scanning the
              # queue. ⛔ THE FIX IS CLOSING THE WINDOW, NEVER WIDENING THE PICKER: making `next` serve DONE-claimed
              # rows would reopen landed work. The column is now MIRRORED FROM the claim, which is the authority.
              # ⭐ s4e-done-does-not-clear-annotation: S4E_DONE_DOES_NOT_CLEAR="dep1[,dep2,...]" appends the
              # machine-read exception list (see s4e_does_not_clear() above) to the state this DONE writes.
              # Optional and rare — most closes clear every dependent, which is why self-clear earns its keep.
              _dnc="${S4E_DONE_DOES_NOT_CLEAR:-}"; _dstate="DONE"
              [ -n "$_dnc" ] && _dstate="DONE:DOES-NOT-CLEAR:$_dnc"
              if s4e_set_row_state "$topic" "$_dstate"; then echo "  (QUEUE.tsv state -> $_dstate; claim and column now agree)"
              else echo "  (no live QUEUE.tsv row for $topic — nothing to mirror; the claim is the record)"; fi
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
         # ⛔⭐ PRE-CLAIM PLACEHOLDER REFUSAL (row mint-dispatch-refuses-a-placeholder-done-when, ceo CEO-286
         # 2026-09-05): STATIC and UNCONDITIONAL -- not gated behind S4E_NO_DISPATCH_PROBE the way the
         # dynamic probe below is, because that escape hatch exists for a seat who wants to dispatch a row
         # DESPITE an "already satisfied" reading they believe is wrong; there is no equivalent case for a
         # placeholder, since prose can never become a command no matter who overrides what. rc=2, matching
         # every other could-not-measure refusal in this file; no claim is written either way below this.
         if s4e_predispatch_placeholder_check "$topic"; then
           printf '\n⛔ REFUSED(2): %s HAS A PLACEHOLDER DONE-WHEN -- %s\n' "$topic" "$_ppc_why" >&2
           printf '   Fix the baton'"'"'s DONE-WHEN line to a real command, then assign again. No claim written.\n' >&2
           exit 2
         fi
         # ⛔⭐ DISPATCH PROBE, ASSIGN SIDE (same ruling as next's; see s4e_dispatch_probe above). An HQ dispatching
         # a row that is already satisfied is the seat09 five-day case with a person in the loop instead of a picker.
         # ⛔ THIS SIDE PROBES AND REFUSES; IT DOES NOT CLOSE. Two reasons, and both are about not overreaching:
         # (1) the ruling says the criterion runs "on the seat's own tree", and an HQ's tree is NOT the seat's -- a
         # green here proves the row is satisfied HERE, which is strong evidence and not the same statement; and
         # (2) `assign` cannot delegate to `done` the way next does (done requires the claim to be the RUNNER's, and
         # assign writes ANOTHER seat's), so closing from here would mean a SECOND copy of the close path, and the
         # close is the one thing in this file that must never exist twice. Refusing the dispatch already prevents
         # the whole measured defect: no seat is handed dead work. The HQ then closes it deliberately, or learns
         # that the DONE-WHEN is passing where the work is not done -- which is the more interesting finding anyway.
         if [ "${S4E_NO_DISPATCH_PROBE:-0}" != "1" ]; then
           # ⛔ rc CAPTURED IMMEDIATELY, never read as `$?` from inside an elif -- this file's own banner comment
           # convicts exactly that ("read its verdict line, not a pipeline's $?"), and a probe whose two failure
           # modes silently collapsed into one would serve back the ambiguity it exists to remove.
           s4e_dispatch_probe "$topic"; _dprc=$?
           if [ "$_dprc" = 0 ]; then
             printf '\n⛔ REFUSED: %s IS ALREADY SATISFIED — do not dispatch it.\n' "$topic" >&2
             printf '   Its own DONE-WHEN exits 0 on THIS tree (%s), so %s would be handed work that is already done.\n' "$S4E" "$seat" >&2
             printf '   That is the five-day dead row this check exists to stop (seat09, FINDING-2026-09-03).\n' >&2
             printf '   ⛔ NOT CLOSED FOR YOU: this is the HQ tree, not %s'"'"'s, and a close is a deliberate act. Either\n' "$seat" >&2
             printf '     close it here    : bash %s claim %s && bash %s done %s\n' "$0" "$topic" "$0" "$topic" >&2
             printf '     or fix the row   : if this row IS real work, its DONE-WHEN is the defect -- it passes where\n' >&2
             printf '                        the work is not done, which is exactly how seat09'"'"'s row died. Fix the\n' >&2
             printf '                        criterion in the baton first, then assign.\n' >&2
             printf '   To dispatch anyway, deliberately: S4E_NO_DISPATCH_PROBE=1 bash %s assign %s %s\n\n' "$0" "$seat" "$topic" >&2
             exit 1
           elif [ "$_dprc" = 2 ]; then
             printf '⚠ DISPATCH PROBE COULD NOT MEASURE %s -- assigning anyway, unverified: %s\n' "$topic" "${_dp_why:-?}" >&2
           fi
         fi
         t="$(mktemp "$PO/claims/.c.XXXXXX")"; { echo "$seat"; echo "ASSIGNED-BY $ME $(date -u +%FT%TZ)"; } > "$t"
         if ln "$t" "$c" 2>/dev/null; then rm -f "$t"; else rm -f "$t"; echo "⛔ RACE LOST: $(head -1 "$c" 2>/dev/null) owns '$topic'" >&2; exit 1; fi
         # ⭐ picker-dependency-and-boomerang-blindness CURE 2: an HQ assignment is the loudest possible "another
         # seat/HQ touched it" — it ends the boomerang cooldown outright. (PASS 1 serves assigned rows and never
         # consults the guard, so this is belt-and-braces: it keeps released/ from accumulating dead receipts.)
         s4e_release_clear "$topic"
         # ⭐ Same cure, second acquisition site. assign does NOT route through the `claim` verb (it writes ANOTHER
         # seat's name, which `claim` by construction cannot), so it is the one acquiring path the primitive does not
         # already cover -- an explicit call, not a missed one. ASSIGNED:<seat> is the spelling two live rows already
         # carry by hand; this makes it written. Col3 untouched here too, for the reason given at `claim`.
         s4e_set_row_state "$topic" "ASSIGNED:$seat" || true
         s4e_backfill_owner_lane "$topic" "$seat" || true   # fills col3 ONLY if blank -- see the function's own note
         # THE DOORBELL CARRIES NO CONTENT (ARCH-FLEET-CEO: "the mail never carries content that isn't also in a file").
         # A seat that never reads this message still resumes correctly, because the claim + task file are authoritative.
         if S4E_NO_BANNER=1 "$0" send "$seat" "task-$topic" "ASSIGNED: $topic. Run: bash SCRIP/scripts/s4e_msg.sh next — it serves this row FIRST, ahead of anything you picked yourself. The task file is authoritative; this message is only the doorbell." >/dev/null 2>&1
         then echo "assigned $topic -> $seat (claim written, doorbell sent)"
         else echo "assigned $topic -> $seat (claim written; ⛔ DOORBELL NOT SENT — the claim still governs, $seat gets it from next)"; fi;;
  # ⭐⭐ ceo RULING 2026-08-29 (row s4e-mint-subcommand, hq_C's find): A SEAT COULD NOT MINT A ROW — next|claim|
  # unclaim|park|done|assign|ask|send|check|clear|mailbox|sweep|board|banner|fleet, NONE of them creates one. That
  # makes "a finding without a row is a finding nobody had" unreachable from a seat by construction. Two seats left
  # real defects loose in one session for want of this path; hq_B hand-appended to QUEUE.tsv (no lock, 16 live
  # seats) and flagged the race themselves. The mail-your-HQ workaround only functions while an HQ is reading, and
  # a mint-request queue is just a second queue — so this is the real thing, open to every seat.
  # Usage: mint <topic> [rank] "GOAL text"   or   mint <topic> [rank] --stdin <<'EOF' ... EOF
  # rank is sniffed, not fixed-position: the token right after topic is consumed as rank ONLY if it is
  # ALL DIGITS (never true of real GOAL prose, even prose that happens to start with a number — that always
  # has a following space/letter); otherwise it defaults to 2 and the same token starts the goal text.
  mint)  topic="${2:?topic}"; shift 2
         rank=2
         if [ -n "${1:-}" ]; then case "$1" in *[!0-9]*|'') :;; *) rank="$1"; shift;; esac; fi
         # ⭐⭐ NO BLANK OWNER CELL, HALF 2 OF 2 (row next-serves-a-seat-only-rows-in-its-hqs-lane-and-no-
         # row-carries-a-blank-owner-cell) — a mint used to write field 3 as the literal string "unassigned"
         # UNCONDITIONALLY, which is exactly how this row's own baton was born with no lane a lane-blind
         # `next` could read. `--owner hq_X` is sniffed here, BEFORE the goal text is collected, the same
         # way rank already is, so `mint <topic> [rank] [--owner hq_X] "GOAL text"` never breaks a caller
         # who omits it: for a topic whose language prefix THE LANES table maps (prolog-/icon-/snobol4-/
         # snocone-/pascal-/raku-/rebus-*), s4e_topic_lane derives the owner for free and no flag is ever
         # needed — this is every mint this fleet has actually issued so far. Only a topic naming no
         # language (a postoffice/tooling/meta row, this fix's own first victim) needs the flag; asking for
         # it there, once, is cheaper than another blank cell nobody catches until a seat wanders into it.
         owner=""
         if [ "${1:-}" = "--owner" ]; then owner="${2:?--owner needs an hq_C|hq_B|hq_P|hq_T|hq_U|hq_S|hq_I|hq_R argument}"; shift 2
           case "$owner" in hq_C|hq_B|hq_P|hq_T|hq_U|hq_S|hq_I|hq_R) : ;; *) echo "⛔ REFUSED: --owner must be one of hq_C hq_B hq_P hq_T hq_U hq_S hq_I hq_R, not '$owner'." >&2; exit 2;; esac; fi
         if [ "${1:-}" = "--stdin" ] || [ "${1:-}" = "-" ]; then goal="$(cat)"; else goal="$*"; fi
         # ⛔ THE TOPIC BECOMES A FILENAME TWICE OVER (a QUEUE.tsv row AND tasks/<topic>.task.md) — same guard
         # as send (s191), checked before either write, not after.
         case "$topic" in ""|*/*|*$'\n'*) echo "⛔ REFUSED: topic must be a short filename-safe slug (no / and no newline). Usage: $0 mint <topic> [rank] \"GOAL text\"" >&2; exit 2;; esac
         [ -n "$goal" ] || { echo "⛔ REFUSED: empty GOAL text. Usage: $0 mint <topic> [rank] \"GOAL text\" (or --stdin)" >&2; exit 2; }
         [ -n "$owner" ] || owner="$(s4e_topic_lane "$topic")"
         if [ -z "$owner" ]; then
           printf '⛔ REFUSED: cannot derive an owner lane for "%s" -- its name matches no language THE LANES table maps\n' "$topic" >&2
           printf '   (prolog- icon- snobol4- snocone- pascal- raku- rebus-*). Supply one: mint %s %s --owner hq_X "GOAL"\n' "$topic" "$rank" >&2
           printf '   (hq_C correctness/Prolog · hq_B beautify/Icon+public face+postoffice tooling · hq_P speed/SNOBOL4+Snocone+Pascal+benchmarks · hq_T test suites/Raku+Rebus+the standard · hq_U unify/shared engine+cross-language regressions · hq_S SNOBOL4 runtime · hq_I Icon suites · hq_R Prolog builtins)\n' >&2
           exit 2; fi
         q="$PO/QUEUE.tsv"; d="$PO/QUEUE.done.tsv"; b="$PO/tasks/$topic.task.md"; mkdir -p "$PO/tasks"
         s4e_mint_dup() { grep -qP "^[0-9]+\t\Q$topic\E\t" "$q" 2>/dev/null && return 0
                           [ -f "$d" ] && grep -qP "^[0-9]+\t\Q$topic\E\t" "$d" 2>/dev/null && return 0
                           [ -f "$b" ]; }
         # Cheap pre-check OUTSIDE the lock — refuses the common case (an existing topic) without making
         # every mint wait on the lock for a duplicate that was always going to be refused. This is an
         # optimization, never the actual guard: the lock below re-checks for the real TOCTOU race.
         if s4e_mint_dup; then echo "⛔ REFUSED: '$topic' already exists (a live QUEUE.tsv row, a QUEUE.done.tsv row, or an existing task file) — pick a different name." >&2; exit 1; fi
         # ⭐ THE ATOMIC APPEND THE GOAL DEMANDS. QUEUE.tsv is ONE FILE every topic's row shares, unlike a
         # claim (one file per topic, where the `ln` hard-link trick above is already atomic per-target) — so
         # the lock is a DIRECTORY, not the row: mkdir is atomic on every POSIX filesystem, same guarantee
         # `ln` leans on, just scoped to the whole file instead of one name in it.
         lock="$PO/.mint.lock"; got=0
         for _i in $(seq 1 50); do mkdir "$lock" 2>/dev/null && { got=1; break; }; sleep 0.1; done
         [ "$got" = 1 ] || { echo "⛔ REFUSED: could not acquire the mint lock ($lock) after 5s — another mint is stuck or crashed holding it. Investigate before removing it by hand; do not rm -rf blindly." >&2; exit 3; }
         # ⛔ COMPOSED WITH THE IDENTITY LOCK, NOT LAYERED OVER IT. A bare `trap ... EXIT` here CLOBBERS the
         # one-process-per-identity release armed at startup, and the `trap - EXIT` below then removes it
         # outright -- so every successful `mint` would leak its own .pid. (Self-healing on the next verb via
         # the liveness check, but a lock file naming a dead process is exactly the stale evidence this row
         # exists to stop manufacturing.) Both handlers run, in order, from one trap.
         trap 'rmdir "$lock" 2>/dev/null; s4e_pid_release' EXIT
         if s4e_mint_dup; then echo "⛔ REFUSED: '$topic' was minted by someone else in the race just now — no torn row written, nothing lost." >&2; exit 1; fi
         # Baton BEFORE the queue row, deliberately: a crash between the two writes then leaves an orphan
         # task file (inert — nobody's picker ever finds a file next() never points at) rather than a live
         # QUEUE.tsv row with no baton behind it, which next()'s own "⛔ NO BATON" path can only catch AFTER
         # some seat has already been served the row.
         cat > "$b" <<TASKEOF
# TASK $topic
GOAL: $goal
DONE-WHEN: ⛔ MUST BE MADE RUNNABLE BEFORE done CAN EVER PASS — minted with no executable acceptance test; replace this line with a real command (see other tasks/*.task.md for the shape) before anyone can close this row.
LINKS: minted via \`mint\` by $ME, $(date -u +%FT%TZ)
## NEXT
Distill a real first step from the GOAL above (and a real DONE-WHEN — see the line above), then work it.
## QA
## LEDGER
- [$ME·$(date -u +%F)] Minted via \`s4e_msg.sh mint\`.
TASKEOF
         printf '%s\t%s\t%s\tFREE\n' "$rank" "$topic" "$owner" >> "$q"
         rmdir "$lock" 2>/dev/null; trap 's4e_pid_release' EXIT
         echo "minted $topic (rank $rank, owner $owner, state FREE) -> $b";;
  next)  q="$PO/QUEUE.tsv"; mkdir -p "$PO/claims"
         s4e_mode_line
         # ⛔⭐ MODE GATES DISPATCH -- IT IS NOT DECORATION (row next-refuses-to-dispatch-to-an-hq-seat-when-mode-
         # line-1-is-ceo; hq_C measured 2026-09-03 18:51). `next` printed "MODE: CEO" and dispatched in the SAME
         # PRINTOUT: it served and LOCKED prolog-bagof-setof-...-rung-8b to hq_C at a moment when no HQ was standing.
         # ⛔ The damage is not the wasted turn, it is the LOCK: a claimed row leaves the picker, so a stood-down
         # identity holding a claim HIDES THAT ROW FROM THE WHOLE FLEET -- and it arrives by a route nobody watches.
         # `fleet` is the health view for a seat that STALLS; it has nothing to say about one that was never meant to
         # be running at all. Reading a value only to PRINT it is the same class as the board that computed its
         # failure names and sent them to /dev/null: the information was produced and then not used.
         # ⛔ THE ceo IS NEVER REFUSED. s4e_is_hq() deliberately counts ceo as an HQ (it is an AUTHORITY test, used by
         # unclaim's force path), but under MODE CEO the ceo is precisely who works the rows. So this guard matches on
         # the IDENTITY SHAPE and must NOT be rewritten to call that predicate -- they answer different questions.
         # ⛔ AN ABSENT OR EMPTY MODE DOES NOT REFUSE HERE: s4e_mode_line above already says so loudly, and every
         # existing s4e gate fixture omits the file. Turning that into a refusal is a different row, not a free extra.
         _mode="$(head -1 "$PO/MODE" 2>/dev/null | tr -d '[:space:]')"
         _refuse_dispatch() {
             printf '⛔ REFUSING TO DISPATCH (rc=2): you are %s, %s, and MODE line 1 is %s.\n' "$ME" "$1" "$_mode" >&2
             printf '   Authority: %s/MODE (first line). %s\n' "$PO" "$2" >&2
             printf '   A claim taken by a stood-down identity HIDES that row from the whole fleet -- which is why this\n' >&2
             printf '   is a REFUSAL and not a warning. If the mode is wrong, ask ceo to publish the real one.\n' >&2
             exit 2; }
         case "$ME" in
           ceo) : ;;
           hq|hq_?) case "$_mode" in
                      CEO) _refuse_dispatch "an HQ" "Under CEO no HQ is standing -- the ceo works the rows itself.";; esac;;
           seat*)   case "$_mode" in
                      CEO|DUO|DUET|TRIO|QUARTET|QUINTET|OCTET) _refuse_dispatch "a fleet seat" "There is NO FLEET in $_mode -- only the ceo and the HQs work rows. (DUO is the pre-rename spelling of DUET and is refused too.)";; esac;;
         esac
         # ⛔⭐ s265 — A STALE CLONE SILENTLY REVERTS TO PRE-V2 DISPATCH, AND THAT IS NOW A REFUSAL, NOT A WARNING.
         # Measured the same day by TWO seats: seat09's clone was 79 commits behind and seat13's was 2, so both ran
         # v1's flat file-order picker — no rank sort, no assign-awareness. seat09 locked a rank-1 row while its own
         # HQ assignment sat unserved; seat13 locked rank 4 while ~30 rank-0 rows were FREE. Neither got any warning,
         # because the V2 fix only ever protected a seat whose clone ALREADY had it. ⛔ The check cannot be "git fetch
         # and compare" — a seat that never pulled has a stale origin/main too, so that test passes precisely when it
         # must fail. So the authority is the SHARED postoffice, which no clone can be behind: PROTOCOL-VERSION there
         # is the required protocol, S4E_PROTO below is this script's own, and next() REFUSES rather than dispatching
         # from a picker it cannot vouch for. A seat that mis-locks a row removes it from the whole fleet's reach.
         # (S4E_PROTO is hoisted above the case dispatch — one number, two guards: this one and clear's.)
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
             printf '      work THAT, then rewrite ## NEXT before you stop. Questions go in ## QA, receipts in ## LEDGER.\n'
             printf '      ⛔ the current block is the FIRST ^## NEXT — demote the one you replace to ## SUPERSEDED-NEXT\n'
             printf '      (baton-one-next-block-gate, ceo ruling 2026-08-29; a few live batons still predate this).\n'
             printf '      ⛔ a baton with NO ## NEXT block: distill one from GOAL + the latest ledger, write it, THEN work.\n'; fi
           # ⭐ V2-2: QUEUE.tsv is an INDEX (rank·topic·owner·state), not a brief store. Fields 3 and 4 are
           # owner and state now, so printing them as "brief:"/"first:" would announce "brief: unassigned".
           # The baton is the content; the index only says who owns it and what state it is in.
           printf '%s\n' "$srow" | awk -F'\t' 'NF>3{print "owner: " $3 "   state: " $4}
                                                  NF>1&&NF<4{print "brief: " $3; print "first: " $4}'
           [ -f "$PO/tasks/$st.task.md" ] || printf '⛔ NO BATON at %s/tasks/%s.task.md — under V2-2 every live row must have one. Tell your HQ; do not invent the work.\n' "$PO" "$st"; }
         # ⭐⭐ THE LANE FILTER LIVED ONLY IN PASS 3, AND A RE-SEATING WALKS STRAIGHT PAST IT (hq_B, ceo witness
         # 2026-09-03 22:55 CDT). MEASURED: seat07 held raku-roast-100-percent-compile (lane hq_T) while its own
         # lane's rank-0 SC4 snocone census sat FREE. The claim carried NO `ASSIGNED-BY` line, so no HQ dispatched
         # it across lanes; and the CONTROL ARM proves the free-row picker did not either -- with the claim removed,
         # PASS 3 skips that row unconditionally on its owner cell (`↩ skipped 4 free row(s) owned by another seat`).
         # What served it was PASS 2, which resumes ANY unfinished claim of mine with no lane check and no owner-cell
         # check at all. ⛔ A SEAT'S LANE IS MUTABLE AND ITS CLAIM IS NOT: seat07's HQ file was rewritten hq_T -> hq_P
         # at 22:45:05, ten minutes before the release, so a claim that was PERFECTLY IN-LANE when it was taken became
         # cross-lane while it was held. Nothing re-asks the question after a serve, so `next` re-served it every turn.
         # ⛔ THE CURE IS A NOTICE, NEVER AN AUTO-RELEASE. Dropping an in-flight claim to chase a lane would strand
         # real work, and a cross-lane hold is often DELIBERATE -- an HQ may assign across lanes on purpose (PASS 1
         # rows carry ASSIGNED-BY and are attributable already). So this says the mismatch out loud and names the
         # own-lane row that is waiting, and a human decides: keep working, or `unclaim` and let the owner have it.
         # ⭐ Same doctrine PASS 3 already runs on -- "a silent skip is indistinguishable from the row not existing",
         # and its cross-lane fallback is labelled rather than left to be reconstructed from the queue later. A serve
         # that cannot be told from an in-lane one is exactly how this went unseen until the ceo read two claims side
         # by side. ⛔ It NEVER refuses and never changes the exit status: a notice that can block is a notice that
         # gets worked around. Lane-undetermined topics and a seat with no readable HQ file stay silent (empty lane
         # is UNDETERMINED, never a fifth lane), so a missing HQ file degrades to today's behaviour, as everywhere else.
         # ⛔ THE TWO ARRIVAL PATHS ARE NOT THE SAME EVENT AND MUST NOT READ THE SAME. A PASS-1 row was DISPATCHED
         # across lanes by a named HQ (its claim carries ASSIGNED-BY): deliberate, attributable, and the notice only
         # makes it visible. A PASS-2 row is one the seat is holding, where the lane may have moved underneath it and
         # NOBODY decided anything -- that is the defect. Printing the PASS-2 explanation over a PASS-1 serve would
         # accuse an HQ of a slip it did not commit, which is the same class of false attribution as an unlabelled
         # cross-lane serve: a printout that describes a different event than the one that happened.
         s4e_cross_lane_notice() { local _t="$1" _via="$2" _tl _wait
           [ -n "${_my_lane:-}" ] || return 0
           _tl="$(s4e_topic_lane "$_t")"; [ -n "$_tl" ] || return 0; [ "$_tl" != "$_my_lane" ] || return 0
           printf '⛔ CROSS-LANE HOLD — %s is %s'"'"'s lane; your lane is %s (%s/%s/HQ).\n' "$_t" "$_tl" "$_my_lane" "$PO" "$ME"
           case "$_via" in
             assigned) printf '   An HQ DISPATCHED this to you across lanes on purpose -- the claim names who, above. Nothing is\n'
                       printf '   wrong here; you are told because a cross-lane serve must be legible in its own printout.\n';;
             *)        printf '   You are being served it because it is YOUR CLAIM, not because it is your work: the resume pass\n'
                       printf '   never re-asks the lane question, and a RE-SEATING CHANGES YOUR LANE UNDER A HELD CLAIM.\n';;
           esac
           _wait="$(awk -F'\t' -v me="$ME" -v lane="$_my_lane" '/^[0-9]+\t/ && ($4=="FREE"||$4=="") && ($3==lane||$3==me) {printf "%s\trank %s  %s  (owner %s)\n",$1,$1,$2,$3}' "$q" 2>/dev/null | sort -t$'\t' -s -k1,1n | head -1 | cut -f2-)"
           if [ -n "$_wait" ]; then printf '   ⭐ WAITING IN YOUR OWN LANE:  %s\n' "$_wait"
             printf '      A row owned by YOUR OWN HQ is now auto-served (cured 2026-09-04: the owner-cell skip treated an HQ\n'
             printf '      like a rival seat and idled six seats at once). A row owned by a DIFFERENT HQ still needs a\n'
             printf '      deliberate dispatch: that HQ or the ceo runs  s4e_msg.sh assign <topic> %s\n' "$ME"; fi
           printf '   KEEP WORKING IT if that was the intent -- this is a NOTICE, NOT A REFUSAL, and the exit status is\n'
           printf '   unchanged. Otherwise put it back for its owner:  s4e_msg.sh unclaim %s\n' "$_t"; }
         # ⭐ HOISTED: the lane is needed by PASS 1 and PASS 2 as well as PASS 3 now, so it is computed ONCE here
         # rather than re-derived per pass -- two readings of one seat's HQ file could disagree mid-run otherwise.
         _my_lane="$(s4e_my_lane)"
         # PASS 1 -- rows an HQ ASSIGNED to me that I have not started. These outrank anything I picked for myself.
         # ⭐⭐ TIE-BREAK MATCHES PASS 3: rank, THEN mint time (newest first; a topic with no readable mint
         # timestamp sorts last). MEASURED 2026-09-03 on seat11's FLEET-12 dispatch: their three assigned rows
         # were rank 2, 2 and 3, and PASS 1 sorted by RANK ALONE -- so the two rank-2 rows tied and the stable
         # sort fell back to the claims/*.claim GLOB ORDER, i.e. alphabetical. seat11 was served
         # raku-array-params-pass-by-copy where the dispatch had named raku-silent-wrong-answers first.
         # ⛔ TWO PASSES WITH TWO TIE-BREAKS, AND ONLY ONE OF THEM WAS EVER CHOSEN: PASS 3 fixed exactly this
         # for free rows (its own note: eight freshly-minted rank-1 slices sat 40+ minutes behind 15 older
         # ones) and the fix was simply never applied to assigned rows. Alphabetical order is not a priority
         # signal, it is an artifact of naming -- the same class as the file-order picking V2-1 removed.
         # ⭐ The deeper point, which the ceo took as the ruling for seat11: AN ORDER STATED IN PROSE ACROSS
         # EQUAL RANKS IS INVISIBLE TO THE PICKER. Rank is the only ordering the tool can see. This makes the
         # tie DETERMINISTIC and documented; it does not make prose orderable, and it should not be read as
         # doing so -- an intended order still has to be encoded as rank.
         # ⭐ RANK-SORTED even among MY OWN claims (fix-dispatch-bus-two-failure-modes, s266, seat07's
         # q-s4e-msg-banner-attribution-undercount): this used to be a bare glob loop, so a seat holding TWO
         # assigned-not-yet-running claims was served whichever topic sorted first ALPHABETICALLY, not the
         # fleet's own rank priority. Same defect shape V2-1 already fixed for Pass 3 (free rows); it had
         # just never been applied to "mine" too. Candidates are gathered first, THEN sorted by qrank, THEN
         # served -- one lowest-rank winner, deterministic regardless of claims/ directory order.
         while IFS=$'\t' read -r _rk t; do
           [ -n "${t:-}" ] || continue
           c="$PO/claims/$t.claim"; echo "RUNNING" >> "$c"
           s4e_cross_lane_notice "$t" assigned
           serve "$t" "ASSIGNED->RUNNING" "(dispatched by $(grep -m1 '^ASSIGNED-BY ' "$c" | cut -d' ' -f2))"; exit 0
         done < <(for c in "$PO"/claims/*.claim; do [ -f "$c" ] || continue
             [ "$(head -1 "$c")" = "$ME" ] || continue
             grep -q '^DONE$' "$c" && continue
             grep -q '^ASSIGNED-BY ' "$c" || continue
             grep -q '^RUNNING$' "$c" && continue
             t="$(basename "$c" .claim)"; printf '%s\t%s\t%s\n' "$(qrank "$t")" "$(s4e_mint_ts "$t")" "$t"
           done | sort -t$'\t' -s -k1,1n -k2,2r | cut -f1,3-)
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
           s4e_cross_lane_notice "$t" resume
           serve "$t" "RESUME" "(yours, unfinished — s4e_msg.sh done $t when the handoff clause is met)"; exit 0
         done < <(for c in "$PO"/claims/*.claim; do [ -f "$c" ] || continue
             [ "$(head -1 "$c")" = "$ME" ] || continue
             grep -q '^DONE$' "$c" && continue
             t="$(basename "$c" .claim)"; printf '%s\t%s\n' "$(qrank "$t")" "$t"
           done | sort -t$'\t' -s -k1,1n)
         [ -f "$q" ] || { echo "no QUEUE.tsv — ask hq"; exit 1; }
         # PASS 3 -- free rows, RANK-SORTED numerically; ties inside one rank now tie-break by MINT TIME,
         # newest first (next-tiebreak-by-mint-time-not-file-order, hq_C finding, ceo mint 2026-09-01) --
         # was file order (QUEUE.tsv line position, an accident of mint sequence, not a priority signal:
         # measured, eight freshly-minted rank-1 fleet slices sat unclaimed 40+ minutes behind 15 older
         # rank-1 rows). A topic with no readable mint timestamp sorts as OLDEST in its tier (s4e_mint_ts),
         # never specially -- so this degrades to the old behaviour only in the absence of data, not by design.
         _owned_skipped=0; _owned_first=""
         # ⛔ ONE line, not one per row: 18 of 178 FREE rows carry a named owner today, so per-row skip lines would
         # bury the serve they precede. But it is never SILENT -- a skip nobody can see is indistinguishable from
         # the row not existing, which is the failure this whole picker keeps being fixed for.
         s4e_report_owned_skips() {
           [ "${_owned_skipped:-0}" -gt 0 ] || return 0
           printf '↩ skipped %d free row(s) owned by another seat (topmost: %s).\n' "$_owned_skipped" "$_owned_first"
           printf '   The owner column constrains the pick (ceo 2026-09-03). To take one anyway: s4e_msg.sh claim <topic>.\n'
           printf '   To move ownership properly, an HQ or the ceo runs: s4e_msg.sh assign <topic> <seat>.\n'; }
         s4e_pass3_scan() {
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
               resolved_blk="$(s4e_first_resolved_blocker "$blk")"
               if [ -n "$resolved_blk" ] && s4e_does_not_clear "$resolved_blk" "$topic"; then
                 # ⭐ s4e-done-does-not-clear-annotation: the blocker IS done, but named THIS topic as an
                 # exception -- honour it instead of the ordinary self-clear. Print the annotation itself,
                 # not just a verdict, per the row's own DONE-WHEN ("prints the annotation text").
                 printf '⛔ %s reached DONE but its state explicitly excludes %s from self-clear (DOES-NOT-CLEAR) — staying %s.\n' "$resolved_blk" "$topic" "$step" >&2
                 printf '   %s state: %s\n' "$resolved_blk" "$(s4e_row_state "$resolved_blk")" >&2
               elif [ -n "$resolved_blk" ] && "$0" park "$topic" FREE >/dev/null 2>&1; then step=FREE
               # ⭐ CURE 1 — DEPENDENCY INVERSION (picker-dependency-and-boomerang-blindness). The blocker is
               # un-DONE, so the old code skipped this row and walked on down the rank order — which is how a
               # blocker RANKED BELOW the umbrella it blocks got served after the work it blocks. Reaching this
               # row at rank N is itself the proof that its blocker deserves rank N: serve the BLOCKER, here.
               elif promo="$(s4e_servable_blocker "$topic")" && [ -n "$promo" ] && s4e_promotion_admissible "$promo" "$topic" "$rank" \
                    && s4e_promotion_placeholder_ok "$promo" "$topic" "$rank" && "$0" claim "$promo" >/dev/null 2>&1; then
                 echo "RUNNING" >> "$PO/claims/$promo.claim"
                 # ⛔ THE PROMOTED BLOCKER IS A SERVED ROW LIKE ANY OTHER, so it is probed like any other. A
                 # blocker that is already satisfied is the WORST row to hand out unprobed: it is blocking
                 # something else, so closing it here also un-blocks the row that named it.
                 s4e_dispatch_gate "$promo" "$rank" || continue
                 printf '⭐ DEPENDENCY INVERSION — rank-%s %s is BLOCKED-ON %s, which is un-DONE, unclaimed and FREE.\n' "$rank" "$topic" "$blk"
                 printf '   Rank is a human guess at priority; a dependency is a fact, and a fact outranks a guess.\n'
                 printf '   You are being served THE BLOCKER at the blocked row'"'"'s own rank position.\n'
                 printf '   When %s lands DONE, %s un-blocks ITSELF — its state column is the self-clearing spelling.\n' "$promo" "$topic"
                 serve "$promo" "LOCKED" "(promoted over blocked $topic, rank $rank)"; exit 0
               fi ;;
           esac
           # ⭐ CURE 3 — GOVERNANCE-GATED ROWS SKIP HERE, ATTRIBUTABLY. The generic non-FREE skip below would
           # drop these too, but silently and indistinguishably from a park — and "indistinguishable from a
           # park" is the whole defect: a grant-wait that reads as a park gets `park FREE`d back into the
           # picker by the next tidy-minded seat, and the seven-re-serves-in-one-day loop restarts. Named here
           # so the state is legible to a -x trace, to the QUEUE-EMPTY report below, and to the park refusal.
           if s4e_is_grant_wait "$step"; then continue; fi
           # ⭐ RESTRICTED:<x> IS SERVABLE, BUT ONLY TO x. Unlike ASSIGNED (which hides a row from everyone, its owner
           # included, so the owner must reach for `claim`), a restriction names who MAY work it -- so the picker can and
           # should hand it to that seat. Everyone else falls through to the generic skip below, as before.
           # ⭐⭐ THE OWNER COLUMN CONSTRAINS THE PICK (ceo ruling 2026-09-03, on hq_B's measured case).
           # MEASURED: `next` served hq_B the rank-0 row port-exit-value-contract-untagged-rax-forges-dt-fail
           # whose owner column read hq_P. The picker ordered by rank and read only the STATE column, so
           # ownership was advisory AT THE MOMENT OF THE PICK -- a rank-0 row tagged to a seat in flight went to
           # whoever typed `next` first. That is fine when the owner is idle and wrong when they are working it,
           # and the picker cannot tell the difference, so it must not guess.
           # ⛔ THE COST OF GUESSING IS ASYMMETRIC AND THAT IS WHY THE DEFAULT IS SKIP: a wrongly-served row makes
           # TWO seats hold one piece of work (and a claim HIDES the row from its owner's own picker, so the owner
           # is locked out silently); a wrongly-skipped row costs one `claim` typed on purpose.
           # ⭐ `assign` remains the HQ/ceo verb that MOVES ownership, and `claim <topic>` is the deliberate
           # override -- neither is touched here. Only the automatic pick is constrained.
           _serve_reason="rank $rank"
           case "$step" in
             FREE|'')
               case "$brief" in
                 ''|unassigned) : ;;
                 "$ME") _serve_reason="rank $rank, your OWNER CELL" ;;
                 # ⛔⭐ A ROW OWNED BY THE SEAT'S OWN HQ IS IN THE SEAT'S LANE AND IS SERVED (ceo, URGENT
                 # 2026-09-04 19:52: SIX SEATS IDLE at load 0.65 with sixteen up, every lane reporting "queue
                 # empty" while QUEUE.tsv held 7 FREE rank<=1 rows per lane). The owner-cell skip treated an HQ
                 # exactly like a rival seat -- and under THE SNOBOL4 CUT every class row an HQ mints carries its
                 # OWN owner cell, so the picker skipped the entire body of assigned work in every lane at once.
                 # seat09's words, verbatim: "all in-lane FREE rows are owned by other seats or by hq_P itself".
                 # ⛔ THE SKIP'S OWN ASYMMETRY ARGUMENT DOES NOT REACH THIS CASE, and that is the whole cure.
                 # It reads: a wrongly-SERVED row makes two seats hold one piece of work, a wrongly-SKIPPED row
                 # costs one `claim` typed on purpose -- so when in doubt, skip. True of a row owned by another
                 # SEAT, which may be about to work it. NOT true of a row owned by an HQ: an HQ owner cell marks
                 # a LANE, not work in flight, and an HQ that is actually working a row has CLAIMED it -- and a
                 # claimed row is already hidden from every picker. So the doubt the skip exists to resolve does
                 # not exist here, and the cost it was weighing was one-sided all along.
                 # ⭐ THE GENERAL FORM, worth more than this line: a guard justified by a cost asymmetry must be
                 # re-checked wherever the asymmetry stops holding. This one was written about seats and applied
                 # to every owner cell, and it silently idled four lanes the day HQs started owning rows.
                 # ⛔ ONLY THE SEAT'S OWN HQ. Another HQ's row stays skipped -- that is a lane boundary, and the
                 # cure for wanting it is `claim`, which is still the deliberate override it has always been.
                 "$(s4e_hq)") _serve_reason="rank $rank, owned by your HQ $brief (your lane)" ;;
                 *) _owned_skipped=$((_owned_skipped+1))
                    [ -n "$_owned_first" ] || _owned_first="rank $rank  $topic  (owner $brief)"
                    continue;;
               esac ;;
             RESTRICTED:*) [ "$(s4e_restricted_to "$step")" = "$ME" ] || continue ;;
             *) continue;;
           esac
           # ⭐⭐ THE MODE LANGUAGE FREEZE -- see s4e_language_freeze_refuses above. Checked here too, not just
           # in the promotion path, so a FREE row that a park-sweep has not yet caught up to cannot be served
           # by the ordinary path either -- the two checks are companions, not alternatives (item (3) of this
           # row's own GOAL: "add the MODE priority cut... alongside the lane cut"). Unconditional: unlike the
           # lane filter below, there is no any-lane-style fallback for a language freeze, ever.
           if s4e_language_freeze_refuses "$topic"; then
             printf '⛔ SKIP %s (rank %s) — MODE freezes work to %s ONLY; this topic is %s. Not served automatically.\n' \
               "$topic" "$rank" "$(s4e_mode_language_freeze | tr '[:lower:]' '[:upper:]')" "$(s4e_topic_language "$topic")"
             printf '   Still live for a deliberate override: s4e_msg.sh claim %s\n' "$topic"
             continue
           fi
           # ⭐⭐ THE LANE FILTER — see s4e_topic_lane/s4e_my_lane above. An explicit OWNER-CELL match
           # (this row is tagged to me by name) always wins regardless of lane; a lane-undetermined topic
           # (s4e_topic_lane returns empty) is lane-neutral and never filtered by either pass. own-lane
           # pass: skip a row whose determined lane is not mine. any-lane pass (reached only when the
           # own-lane pass fell through the WHOLE sorted queue without serving anything): never skip on
           # lane, but relabel the reason so a cross-lane serve is visible in its own printout rather than
           # reconstructed later from the queue (GOAL's own requirement).
           # ⛔ AN OWNER-CELL REASON IS NOT RELABELLED BY THE LANE PASS -- neither the seat's own cell nor its HQ's.
           # "your OWN LANE (hq_P)" and "owned by your HQ hq_P" are both true of the same row, and the second is
           # the one that answers the question a reader actually has when six seats were told the queue was empty:
           # WHY did this one come through? The lane label would have hidden the very mechanism that was just
           # cured, which is how a fix gets quietly re-broken by someone who cannot see it working.
           if [ -n "${_my_lane:-}" ] && [ "$_serve_reason" != "rank $rank, your OWNER CELL" ] \
              && [ "${_serve_reason#rank $rank, owned by your HQ}" = "$_serve_reason" ]; then
             _tl="$(s4e_topic_lane "$topic")"
             if [ "$_lane_filter" = own-lane ]; then
               if [ -n "$_tl" ] && [ "$_tl" != "$_my_lane" ]; then continue; fi
               [ -n "$_tl" ] && _serve_reason="rank $rank, your OWN LANE ($_tl)"
             elif [ -n "$_tl" ] && [ "$_tl" != "$_my_lane" ]; then
               _serve_reason="rank $rank, CROSS-LANE FALLBACK (your lane $_my_lane had nothing servable; this row is $_tl's)"
             fi
           fi
           [ -f "$PO/claims/$topic.claim" ] && continue
           # ⭐ CURE 2 — RELEASE BOOMERANG. Do not hand a seat back the row it just put down; say so out loud,
           # because a silent skip here is indistinguishable from the row not existing.
           if s4e_boomerang_hold "$topic"; then
             printf '↩ SKIP %s (rank %s) — YOU released this row less than %ss ago; not serving it back to you.\n' "$topic" "$rank" "${S4E_RELEASE_COOLDOWN:-3600}"
             printf '   It is still live for every other seat. To take it back deliberately: s4e_msg.sh claim %s\n' "$topic"
             continue; fi
           if s4e_predispatch_placeholder_check "$topic"; then
             printf '⛔ REFUSING TO DISPATCH %s (rank %s) — %s\n' "$topic" "$rank" "$_ppc_why"
             printf '   Not claimed. An HQ or the mint author fixes the baton; this row stays free for everyone else.\n'
             continue; fi
           if "$0" claim "$topic" >/dev/null 2>&1; then
             echo "RUNNING" >> "$PO/claims/$topic.claim"
             # ⛔⭐ ONE CALL, TWO SERVE PATHS -- see s4e_dispatch_gate. Never inline this again.
             s4e_dispatch_gate "$topic" "$rank" || continue
             s4e_report_owned_skips
             serve "$topic" "LOCKED" "($_serve_reason)"; exit 0; fi
         done < <(grep -P '^[0-9]+\t' "$q" | while IFS=$'\t' read -r rk tp br st; do printf '%s\t%s\t%s\t%s\t%s\n' "$rk" "$(s4e_mint_ts "$tp")" "$tp" "$br" "$st"; done | sort -t$'\t' -s -k1,1n -k2,2r | cut -f1,3-)
         return 1
         }
         # ⭐⭐ TWO PASSES, ONE BODY — s4e_pass3_scan is called once per lane-filter mode so the own-lane
         # attempt and the cross-lane fallback can never disagree about what "servable" means (every other
         # skip rule -- blocked-on, grant-wait, restricted, claimed, boomeranged -- runs identically in
         # both calls; only the lane check inside toggles). A seat/HQ with no determinable lane
         # (s4e_my_lane empty: ceo, an unrecognized identity, or a seat whose HQ file is missing/stale)
         # never enters the own-lane pass at all -- lane-blind, exactly today's behaviour, is the safe
         # degradation for anything the lane mapping cannot see. GOAL's own fallback requirement ("a lane
         # guard that starves a seat whose lane is done is worse than the wandering it prevents") is why
         # the own-lane pass tries the WHOLE rank-sorted queue before giving up, not just rank<=1 -- a
         # strict rank<=1-only trigger would send a seat cross-lane while its own rank-2 work still sat
         # unclaimed.
         if [ -n "$_my_lane" ]; then _lane_filter=own-lane; s4e_pass3_scan; fi
         _lane_filter=any-lane; s4e_pass3_scan
         # ⭐ CURE 3, second half — WHEN NOTHING IS SERVABLE, SAY WHAT GOVERNANCE IS HOLDING. A bare "queue
         # empty" sent seats to ask HQ for work while rows sat waiting on a grant nobody had chased.
         _gw="$(awk -F'\t' '/^[0-9]+\t/ && ($4 ~ /^GRANT-NEEDED/ || $4 ~ /^PARKED-LON-HOLD/) {printf "     rank %s  %s  [%s]\n",$1,$2,$4}' "$q" 2>/dev/null)"
         s4e_report_owned_skips
         echo "QUEUE EMPTY — every row claimed. Ask hq: s4e_msg.sh ask work 'queue empty'"
         [ -n "$_gw" ] && { echo "   ⭐ NOT empty of WORK — these rows are GOVERNANCE-GATED, waiting on a grant, not on a seat:"; printf '%s\n' "$_gw"; echo "   Chase the grant (route via your HQ to ceo), do not re-park these to FREE."; }
         exit 1;;
  banner) # ⛔ FACTS ONLY -- NO PREDICTIONS (Lon 2026-08-22: "Why are you trying to predict the future. Quit saying
         # ⛔ FIRST THING IN THIS CASE, BEFORE ANY EXIT: the first placement was after the verdict line, which always `exit`s
         # (:1308), so the block was unreachable -- proven by deleting a hook and running banner: it did NOT come back. Falsify,
         # do not assume; a block that reads as coverage and cannot fire is the defect it was written to cure.
         # ⛔⭐ THE COMMIT-MSG HOOK IS (RE)INSTALLED FROM HERE TOO (seat04 2026-09-01; census hq_B same day). The installer rode
         # s4e_inbox_hook.sh, which fires on UserPromptSubmit -- and a census of every root's .claude/settings.json found
         # 16 of 19 wire ONLY Stop (all sixteen fleet seats; only the three HQs have UserPromptSubmit). So the row's claim
         # "rejected in every clone" was true in three roots and silently false in sixteen, and nothing on either end could
         # tell: the seat sees no install line (correct, nothing ran) and the HQ sees a landed row and a green gate. Stop is
         # the one event every root demonstrably has -- it is what prints this banner -- so coverage no longer depends on
         # which events a seat happened to wire. Same guard as the inbox hook: quiet, and never allowed to break the banner.
         # ($HERE is NOT a variable of this script -- the first draft of this block used it, tested false forever, and
         #  was caught by the verify line `grep -c HERE=` reading 0: the same silent class one paragraph up.)
         # ⭐⭐ THE SCORE LINE FIRST (Lon 2026-09-04 17:5x: "Ensure the banner is showing the score by each seat at every turn."):
         # measured from seat07's transcript the same hour -- 4 banners, 2 score lines: the line printed LAST, after ~9 s of
         # slower work, where a Stop-hook timeout or a truncated display loses it, and its refusal went to /dev/null. So it
         # prints HERE, 0.18 s, before anything slow, and a refusal prints VISIBLY. Never the verdict: this is a pipeline.
         if [ -n "${S4E_PROGRESS_PROBE_BROKEN:-}" ]; then _ph="/nonexistent-s4e-home"; else _ph="$S4E"; fi
         S4E_HOME="$_ph" python3 "$(dirname "${BASH_SOURCE[0]}")/util_score_row.py" progress 2>/dev/null | grep -m1 '^PROGRESS 09-10 |' \
           || printf 'PROGRESS: UNREADABLE -- util_score_row.py progress printed no score line under %s (SCORE.md missing or its grid unreadable); the verdict below is unaffected\n' "$_ph"
         _ih="$(dirname "${BASH_SOURCE[0]}")/install_commit_msg_hook.sh"
         if [ -x "$_ih" ]; then
           _h="$(bash "$_ih" --quiet 2>/dev/null || true)"; [ -n "$_h" ] && printf '%s\n' "$_h"
         fi
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
         # ⛔⭐ THIRD ATTRIBUTION DEFECT IN THIS BLOCK, and the first one the MESSAGE GREP CANNOT EVER FIX
         # (seat05 2026-08-29, msg q-bb-fixup-az-cleanup; verified + generalized by hq_P same session).
         # The grep above can only see a commit that NAMES this seat or its row. Nothing requires that:
         # the ONE-IDENTITY LAW makes every commit's author identical fleet-wide, so the message is the
         # only signal, and long-standing conventions carry neither field -- e.g. this repo's own
         # "FIXUP <file>: <change>" rung convention. seat05 landed 3 real, pushed, gate-verified FIXUP
         # commits and `board` printed "⚠ NOTHING LANDED ... produced NO commit and NO FINDING".
         # ⭐ MEASURED BLAST RADIUS, not just that row: of 20 commits in one 12h window, 15 (75%) named
         # neither a seat/hq id nor ANY QUEUE.tsv topic -- so this under-reports most fleet work, and
         # reports zero whenever a seat's whole session used such a convention.
         # ⭐ THE FIX USES A SIGNAL THAT NEEDS NO CONVENTION AT ALL: every seat has its OWN CLONE, so a
         # commit CREATED HERE is this seat's by construction. `git reflog` distinguishes exactly that --
         # locally-created commits are "commit:"/"commit (amend):" entries, while another seat's work
         # arriving via pull/rebase/merge is "pull ... (start): checkout"/"merge origin/main" and is
         # NEVER counted. (A `pull --rebase (pick)` replays THIS seat's own commit, also not counted --
         # the original "commit:" entry already covers it, and counting unique SUBJECTS de-duplicates
         # the rewritten hash.)
         # ⛔ STRICTLY ADDITIVE BY CONSTRUCTION: we take the MAX of the two counts, never the reflog
         # alone. This can only ever find MORE commits than before, never fewer, so it cannot turn a
         # correct banner red or a landed session into a false NOTHING LANDED -- the failure being cured
         # is a false NEGATIVE, and the fallback's own population ("commits made in this clone inside
         # the window") is this seat's work by definition. A repo with no reflog degrades to the old
         # behaviour silently.
         cmts=0; for r in "$S4E"/*/; do [ -d "$r/.git" ] || continue
           n=$(git -C "$r" log --since="$since" -i --grep="$ME" ${row1:+--grep="$row1"} --oneline 2>/dev/null | wc -l)
           n2=$(git -C "$r" reflog show --since="$since" --format='%gs' 2>/dev/null | sed -n 's/^commit[^:]*: //p' | sort -u | grep -c . 2>/dev/null || true); n2="${n2:-0}"
           [ "$n2" -gt "$n" ] 2>/dev/null && n="$n2"
           cmts=$((cmts+n)); done
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
         printf '\n%s\n  %s\n%s\n' "$b" "$line" "$b"
         # ⭐ THE PROGRESS LINE, LAST (Lon 2026-09-03 ~20:15: "each of the 7 main runners display a score of
         # percentage in a banner ... just to see a progress indicator of any kind"). It READS .github/SCORE.md
         # and runs no suite -- ~60ms, no network, no build -- so a Stop hook can afford it on every response.
         # ⛔ NEVER LET IT BREAK THE BANNER: the banner's exit status is the seat's computed verdict, so a
         # refusal here (a renamed grid column, a missing language row) must be VISIBLE and must not change
         # that verdict. Hence `|| true` and stderr kept -- a progress line that could turn a green handoff red
         # would be a reporting tool with veto power over the thing it reports on.
         S4E_HOME="$S4E" python3 "$(dirname "${BASH_SOURCE[0]}")/util_score_row.py" progress 2>&1 || printf 'PROGRESS: UNREADABLE at the foot of the banner -- see the score line at its head\n'
         printf '\n'
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
           # ⛔⭐ SECOND DISCRIMINATOR, ADDED BY hq_C 2026-09-05 AFTER "0 STALLED" FIRED FALSELY ON THE SAME LIVE
           # SEAT TWICE IN TWENTY MINUTES, from two different HQs (hq_B at 1h25m, ceo at 1h44m). seat05 was mid-cure
           # the whole time: four substantive messages and a baton LEDGER rewritten minutes before each alarm.
           # COMMITS SINCE LOCK cannot see the highest-value work a seat in a census/witness lane does, and under
           # FLEET-<n> -- where seats walk/census/witness while HQs cure, and are often told NOT to touch src/ --
           # that is the MAJORITY of seats, not an edge case. "0 commits" is reachable by "stalled" and by "doing
           # exactly what its HQ told it to do", and this column named only the first, in red (RULES.md § A SIGNAL
           # REACHABLE BY TWO CAUSES THAT NAMES ONLY ONE). The documented remedy for a stall is releasing the claim,
           # so a false positive here costs a working seat its in-flight row. The baton mtime is already on disk and
           # separates the two states for free.
           bmt=0; [ -f "$PO/tasks/$row.task.md" ] && bmt=$(stat -c %Y "$PO/tasks/$row.task.md" 2>/dev/null || echo 0)
           bage=$(( ( $(date +%s) - ${bmt:-0} ) / 60 ))
           # ⛔⭐ THIRD DISCRIMINATOR, ADDED BY seat01 2026-09-05 (row fleet-stalled-is-one-signal-reachable-by-two-
           # causes-and-names-only-one, minted by hq_B after seat05/seat07/seat14 all false-positived in one sitting):
           # baton mtime alone still misses a seat whose HQ lane produces outbound messages (census/witness/ask)
           # without rewriting its own baton in the same window. Same shape as the DONE-WHEN this row grades on:
           # count messages naming this seat, newer than the lock.
           msince=0; [ "$lockep" -gt 0 ] && msince=$(find "$PO" -name "*-$seat-*.msg" -newermt "@$lockep" 2>/dev/null | wc -l)
           if [ "$lockep" -eq 0 ]; then csh="-"
           elif [ "$csince" -eq 0 ] && [ "$(( $(date +%s) - lockep ))" -gt 3600 ]; then
             if [ "${bmt:-0}" -gt 0 ] && [ "$bage" -lt 60 ]; then csh="0 baton${bage}m"; elif [ "${msince:-0}" -gt 0 ]; then csh="0 msgs${msince}"; else csh="0 STALLED"; fi
           else csh="$csince"; fi
           printf '  %-8s %-30.30s  %-10.10s  %-10.10s  %-20.20s  %s  %-8.8s  %s\n' "$seat" "$row" "$lockage" "$csh" "$tree" "$q" "$mail" "$bl"; done
         free=0; tot=0
         while IFS=$'\t' read -r rank topic brief step; do case "$rank" in ''|\#*) continue;; esac
           tot=$((tot+1)); [ -f "$PO/claims/$topic.claim" ] || free=$((free+1)); done < "$PO/QUEUE.tsv" 2>/dev/null
         printf '\n  queue: %s rows, %s free for the picker (a row with ANY claim file, DONE or not, is hidden)\n' "$tot" "$free"
         printf '  Q = questions from that seat waiting on ANY HQ.  MAIL = unread in its inbox / age of the oldest.\n'
         printf '  ⛔ LOCK AGE = how long the CLAIM FILE has existed = when the lock was TAKEN. It is NOT a work signal:\n'
         printf '     a seat that claimed a row and stalled prints the same number as a seat mid-cure. COMMITS SINCE LOCK\n'
         printf '     measures work ONLY in a committing lane (attributed to the seat or its row, across that root every\n'
         printf '     repo) -- a census/witness lane reads 0 by design, so read it alongside baton age and outbound mail.\n'
         printf '     "0 STALLED" = locked over an hour, nothing attributed, the baton untouched for an hour, AND zero\n'
         printf '     messages sent since the lock -- a claimed row hides itself from the picker, so that blocks the row\n'
         printf '     for the WHOLE fleet until an HQ releases it. "0 batonNNm" = baton written NNm ago; "0 msgsN" = N\n'
         printf '     messages sent since lock: either means that seat is WORKING, in a lane whose output is\n'
         printf '     census/witness/LEDGER/asks rather than commits. DO NOT release it.\n'
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
         rm -f "$gone"; printf 'sweep: %s live rows kept, %s DONE rows moved to QUEUE.done.tsv (nothing deleted; buffer backed up)\n' "$nk" "$ng"
         # ⛔⭐ (b) GARBAGE-COLLECT CLAIMS WHOSE TOPIC HAS NO LIVE ROW (hq_B 2026-08-28; runs AFTER (a) by design --
         # `done` now mirrors its close into the column, so a claim reaching this point has already had every chance
         # to be recorded). ⭐ WHY IT MATTERS AND WHY IT IS NOT COSMETIC: PASS 3 skips ANY topic that has a claim
         # file, DONE or not. Measured that day: 227 claim files against 205 live rows, 195 of them residue for
         # topics already in QUEUE.done.tsv. Re-mint any one of those 195 NAMES and the new row is INVISIBLE ON
         # ARRIVAL -- it never reaches a seat, and the queue still reads FREE. That is a trap armed by ordinary
         # success: the more work the fleet lands, the more names become undispatchable.
         # ⛔ DELETES ONLY THE REDUNDANT ONES. A claim with no live row AND a DONE latch is duplicated by
         # QUEUE.done.tsv, so removing it loses nothing. A claim with no live row and NO DONE latch is NOT residue
         # -- it is a live lock whose row was renamed or dropped underneath it, i.e. possible work in flight -- so
         # it is REPORTED AND KEPT. Deleting that one would destroy the only trace of an in-flight claim, which is
         # the mirror of the bug this whole row exists to fix.
         _gc=0; _orph=""
         for _c in "$PO"/claims/*.claim; do
           [ -f "$_c" ] || continue
           _t="$(basename "$_c" .claim)"
           grep -qP "^[0-9]+\t\Q$_t\E\t" "$q" 2>/dev/null && continue      # still a live row -- leave it alone
           if grep -q '^DONE$' "$_c"; then rm -f "$_c"; _gc=$((_gc+1))
           else _orph="$_orph $_t"; fi
         done
         printf 'sweep: %s residue claim(s) garbage-collected (DONE + no live row; QUEUE.done.tsv keeps the record)\n' "$_gc"
         [ -n "$_orph" ] && printf '⚠ KEPT, NOT DELETED — claim with NO live row and NO DONE latch (a live lock whose row vanished; investigate, do not assume residue):%s\n' "$_orph"
         :;;
  board) if [ $# -gt 1 ]; then shift; grep -v "^$ME |" "$PO/BOARD.md" 2>/dev/null > "$PO/.b.$$" || true; printf '%s | %s | %s\n' "$ME" "$*" "$(date -u +%H:%M)" >> "$PO/.b.$$"; mv "$PO/.b.$$" "$PO/BOARD.md"; fi; cat "$PO/BOARD.md"
         # posting a board line IS the handoff gesture -- so the banner fires here too (see `done` above).
         [ "${S4E_NO_BANNER:-0}" = "1" ] || S4E_BANNER_NO_BOARD=1 "$0" banner;;
  *) echo "usage: next|claim|unclaim|park|done|assign|mint|ask|send|check|clear|mailbox|sweep|board|banner|fleet"; exit 2;;
esac
