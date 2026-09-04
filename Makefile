# Makefile — SCRIP unified build
#
# Primary targets:
#   make scrip        — build the unified scrip x86 executable
#   make all          — alias for scrip
#   make setup        — install system packages + CSNOBOL4 + SPITBOL oracle
#   make test         — THE blocking set: SNOBOL4 corpus (m3+m4) + the two live gates
#   make monitor-ipc  — build test/monitor/monitor_ipc.so
#   make clean        — remove build artefacts
#   make distclean    — clean + remove /tmp caches
#
# Runner wrappers (run a single .sno file):
#   make run SNO=file.sno              — default (mode-3 --run)
#   make run-jvm SNO=file.sno          — legacy JVM (until M-JITEM-JVM)
#   make run-net SNO=file.sno          — legacy .NET (until M-JITEM-NET)
#
# Note: run-asm retired — replaced by: scrip --native codegen-emit --x64 (M-JITEM-X64)
#
# Prerequisites:
#   apt-get install -y flex nasm build-essential libgmp-dev m4
#
# Authors: Lon Jones Cherryholmes · Claude Sonnet 4.6

ROOT    := $(shell pwd)
SRC     := $(ROOT)/src
RT      := $(SRC)/runtime
BOXES   := $(SRC)/ir
CORPUS  ?= $(ROOT)/../corpus
# PER-TREE objdir (s150): two checkouts NEVER share .o files — the HQ-27 ABI-mix class is structurally impossible. Override only deliberately. (Comment on own line: make keeps trailing spaces before an inline #.)
OBJ     ?= /tmp/si_objs$(subst /,-,$(ROOT))
# PER-ROOT pristine lock (make-pristine-per-root-flock-second-builder-waits): keyed by ROOT exactly like OBJ
# above, never global -- a second concurrent `make pristine`/`pristine-all` in THIS root waits on this file
# instead of racing the first one's rm -rf (seat08 measured 3+ concurrent pristines wiping objdir+out/ for
# 6+ minutes). This is a DIFFERENT lock from lib_build_governor.sh's fleet-wide governor.lock -- that one
# serializes builds against benchmarks ACROSS seats; this one serializes builds against each other WITHIN
# one checkout, which the shared governor's -s (shared) build mode deliberately does not do. The wait on
# THIS lock is bounded by scripts/lib_pristine_lock.sh (build-governor-holds-pristine-while-box-idles) --
# see the `pristine` target's own comment for why.
BUILD_LOCK ?= /tmp/si_pristine_lock$(subst /,-,$(ROOT))
CC      := gcc
CXX     := g++
WARN    := -w
RT_OPT  ?= -O0 -g -fno-strict-aliasing -fwrapv -fno-omit-frame-pointer  # ⛔⭐ NO -O2 BUILDS, EVER (Lon 2026-08-23 s262 FACT RULE, SUPERSEDES O0-DEV-O2-BENCH/s179 and O2-ALWAYS/s178): -O0 for development AND benchmarks AND demos. Never pass RT_OPT="-O2 ...", never build an -O2 RT_TAG, never quote an -O2 number as current state. Two reasons, the second stronger: (1) an -O2 template-touching rebuild is ~9m30 vs ~1m40, paid on every arm of a measure-and-cure loop; (2) it measures a compiler we are DELETING — the RT is moving to register-aware ASM (src/runtime/rtx/*.s, GOAL-RTCC.md), so an -O2 figure grades gcc's optimizer over code that will not exist. The LABELING duty survives: every perf number still names its RT_OPT, and it now reads -O0. Authority: .github/RULES.md § NO -O2 BUILDS.
DEPFLAGS := -MMD -MP
CBASE   := -O0 -g $(WARN) $(DEPFLAGS) -I$(SRC) -I$(SRC)/ir -I$(SRC)/lower -I$(SRC)/emitter -I$(SRC)/runtime/core -I$(RT)
ZCFLAGS ?=
CXXRT   := -O0 -g $(WARN) $(DEPFLAGS) -std=c++17 -finput-charset=UTF-8 -I$(SRC) -I$(SRC)/ir -I$(SRC)/lower -I$(SRC)/emitter -I$(SRC)/runtime/core -I$(SRC)/templates/bb -I$(SRC)/templates/xa -I$(SRC)/templates/x86 -I$(RT) $(ZCFLAGS)
CRT     := $(CBASE) $(ZCFLAGS)
LIBS    := -lm -lpthread

# Runner defaults
SNO          ?= $(error SNO is required — e.g. make run SNO=prog.sno)
INC          ?= $(CORPUS)/inc
JVM_CACHE    := /tmp/scrip_jvm_cache
NET_CACHE    := /tmp/scrip_net_cache
JASMIN       := $(ROOT)/miscellaneous/jasmin.jar
SCRIP_CC_BIN := $(ROOT)/scrip

.PHONY: all scrip setup hooks pristine pristine-all buildinfo FORCE \
        test test-postoffice \
        native codegen-emit-test \
        monitor-ipc \
        libscrip_rt libscrip_rt_static \
        run run-ir run-jvm run-net \
        clean distclean

# ── Primary target ────────────────────────────────────────────────────────────

all: scrip

pristine:  # A full rebuild, SCOPED TO THIS CONFIGURATION. ⛔ NO LONGER the per-landing gate law -- HQ-27 PRISTINE-BEFORE-VERDICT was loosened by Lon 2026-09-03 (RULES.md FACT RULE). This target is for the ceo's audits, release points, and whenever the stale-binary refusal fires; an ordinary landing grades on incremental `make`.
	# ⭐ WHY SCOPED IS STILL PRISTINE (Lon s258): it wipes every object THIS build can possibly link --
	# the compiler objdir, this RT_TAG's runtime objects, this tag's .so, the canonical symlink, and the
	# binary. Other tags live in directories this configuration never reads, so they cannot contaminate a
	# verdict; they are a cache, not state. That is what lets an -O2 gate run stop costing 9m30 every time.
	# Use `make pristine-all` for the old wipe-everything behaviour.
	# ⛔ AND IT NOW REMOVES ./scrip, WHICH THE OLD RECIPE DID NOT: `rm -rf $(OBJ) out` left the previous
	# binary in place, so a build that failed part-way left a stale, plausible ./scrip that every later
	# test would silently grade instead of failing loudly. Measured 2026-08-22: mid-rebuild the tree held a
	# 20:06 -O0 binary while an -O2 build was still running. That is the "non-empty is not alive" class.
	# ⛔ PER-ROOT FLOCK, EXCLUSIVE, BOUNDED (make-pristine-per-root-flock-second-builder-waits; bound added
	# by build-governor-holds-pristine-while-box-idles): a second `make pristine` in this same root now
	# WAITS here instead of racing this rm -rf, then runs its own full wipe+rebuild once the first is done.
	# ⭐ THE WAIT IS BOUNDED, NOT SILENT (scripts/lib_pristine_lock.sh, S4E_PRISTINE_LOCK_TIMEOUT, default
	# 1200s): a bare `flock` here previously waited FOREVER with zero output, indistinguishable from a
	# stuck build until it self-cleared -- hq_P measured a ~9.5h overnight loss to exactly that. Past the
	# bound it REFUSES (rc=99) naming the current holder instead of hanging; raise the bound for a
	# genuinely slower box, don't remove it. S4E_NO_BUILD_LOCK=1 bypasses the lock entirely -- control-arm
	# / measurement use ONLY (same shape as SCRIP_OPT=0: an emergency escape hatch nothing may depend on),
	# never for a real gate verdict.
	@if [ -n "$${S4E_NO_BUILD_LOCK:-}" ]; then \
		echo "⚠ S4E_NO_BUILD_LOCK set -- running pristine UNLOCKED (measurement/control-arm only)"; \
		rm -rf $(OBJ) $(RT_OBJDIR) $(RT_SO) $(ROOT)/out/libscrip_rt.so $(RT_A) $(ROOT)/out/libscrip_rt.a $(ROOT)/scrip; \
		$(MAKE) all; \
	else \
		bash scripts/lib_pristine_lock.sh $(BUILD_LOCK) -- sh -c '\
			rm -rf $(OBJ) $(RT_OBJDIR) $(RT_SO) $(ROOT)/out/libscrip_rt.so $(RT_A) $(ROOT)/out/libscrip_rt.a $(ROOT)/scrip && \
			$(MAKE) all \
		'; \
	fi

test-postoffice:  # ⭐ THE HERMETIC POSTOFFICE GATES, WIRED 2026-09-03 (hq_B, ceo ruling on row `postoffice-gates-red-on-origin-because-no-s4e-gate-is-in-make-test`). ⛔ WHY THIS TARGET EXISTS: three of these gates were RED ON ORIGIN and nobody knew, because NO s4e_* gate was in any runner -- a gate in no runner is not measuring, the same false-green shape `test` itself was cured of at s268. Each red was a stale FIXTURE, never a broken tool: the picker grew an owner-column constraint and a baton requirement, and the fixtures kept asserting against the older contract. ⭐ HERMETIC IS THE MEMBERSHIP TEST, AND IT IS MEASURED, NOT ASSUMED: every gate below builds its own scratch postoffice under mktemp, and the set was verified to leave /home/resources/postoffice byte-identical (QUEUE.tsv, QUEUE.done.tsv, claims/, tasks/) across a full run -- a gate that mutated live fleet state would be unrunnable here at any speed. The gates that read the LIVE postoffice (baton_donewhen_runnable{,_live}, baton_next_blocks, baton_one_next_block, baton_state_header_single_record, queue_is_an_index, s4e_release_verbs_mark_last_row) are deliberately OUT: they grade rows sixteen seats are editing right now, so they red on a dirty fleet BY DESIGN -- exactly the reason test_gate_preflight_complete.sh is out too (ceo, same ruling). Keep this target seconds-cheap; anything needing a build or a live read does not belong in it.
	bash scripts/test_gate_s4e_picker_v2.sh
	bash scripts/test_gate_s4e_next_honours_owner.sh
	bash scripts/test_gate_s4e_next_tiebreak_by_mint_time.sh
	bash scripts/test_gate_s4e_unclaim_keeps_park.sh
	bash scripts/test_gate_s4e_done_timeout_is_a_refusal.sh
	bash scripts/test_gate_s4e_next_refuses_hq_under_ceo_mode.sh
	bash scripts/test_gate_picker_autounblock.sh
	bash scripts/test_gate_dispatch_claim_single_authority.sh
	bash scripts/test_gate_dispatch_gc_safepoint_inline.sh
	bash scripts/test_gate_dispatch_bus_failure_modes.sh
	@echo "  ✅ test-postoffice: 10 hermetic s4e_* gates green"

test: scrip  # ⭐ WAS THE FALSE-GREEN TRAP (cured hq_P s268): `test`, `test-ir` and `test-all` were named in .PHONY with NO RECIPE ANYWHERE, so each exited 0 having run NOTHING ("Nothing to be done for 'test'") while reading as a full green suite. `test-ir` and `test-all` are DELETED rather than wired — nothing behind them ever existed. This target now runs THE blocking set named in CLAUDE.md and fails loudly on the first red. ⛔ HQ-27 PRISTINE-BEFORE-VERDICT IS VOID as a per-landing requirement (Lon 2026-09-03 ~15:58, RULES.md FACT RULE: "It's time to loosen this pristine build that keeps preventing forward progress and causes 20 minute wait times."). A landing verdict runs on an INCREMENTAL make -- never on a stale binary, and the stale-binary refusal stays as the guard. `make pristine` survives for the ceo's audits, release points, and whenever that refusal fires; the receipt names the build it graded on. This comment asserted the retired law for hours after it was retired, which is the failure mode the FACT RULE exists to prevent: law that lives in a second unversioned place cannot be retired in one edit.
	python3 scripts/strip_comments.py --check   # ~0.8s MEASURED (0.79/0.83/0.80 over three runs, 384 files), the cheapest arm here and the only one needing no build at all, so a style red fails in under a second instead of after a 6-minute board. ⭐ WIRED 2026-09-03 (hq_P, row strip-comments-check-is-not-in-make-test-so-src-style-goes-red-on-origin, minted by hq_C): RULES.md § C style -- src/ carries ZERO comments but the sanctioned 200-char /*---*/ and /*===*/ separators -- was enforced ONLY by a clause each Prolog rung's DONE-WHEN opted into BY HAND. A seat that omitted it pushed a RED gate to origin while `make test` still said green for everyone, and the redness stayed invisible until some LATER seat's DONE-WHEN ran on the merged tree. ⛔ TWICE IN ONE EVENING, hq_B's 92d300f07 and hq_P's own 84e02570, NEITHER SEAT AT FAULT in any way a reviewer would catch: a long explanatory comment is the NATURAL way to make a deletion or a shared-box change legible, and this codebase has deliberately closed that door and routed the explanation to the FINDING. A rule people must REMEMBER to copy into their own acceptance test is a hope, not a mechanism -- the same false-green shape this very target was cured of at s268.
	bash scripts/test_gate_our_files_are_lf.sh   # <1s, pure census (no build): OUR FILES ARE LF (Lon 2026-09-03 "Do not use CRLF, use LF."; 2026-09-04 "Fix the CRLF to LF problem. Why am I still hearing about that?"). Every tracked file in SCRIP, corpus, .github minus corpus/packages/ (vendor fixtures keep the oracle's own CRLF, s183) and generated flex/bison outputs; a CR at end of line is red, a CR byte inside a string literal is data. WIRED 2026-09-04 (ceo, Lon order): the csv.writer default lineterminator re-minted every ALL.csv as CRLF after each rebuild and no gate saw it.
	$(MAKE) --no-print-directory test-postoffice   # ~13s MEASURED, no build needed: the hermetic s4e_* gates. SECOND arm by ceo ruling 2026-09-03 (row postoffice-gates-red-on-origin-because-no-s4e-gate-is-in-make-test) -- every seat's next/done/assign rides on this one tool, so a red in it is a red for all sixteen seats at once, and it belongs beside strip_comments as a cheap arm that fails in seconds rather than after a 6-minute board.
	bash scripts/test_gate_capture_stdin_and_red_exit.sh   # ~15s, mktemp-only: first because a cheap self-contained gate belongs before a 6-minute board (ceo grant 2026-08-30; move it if the order should be authored elsewhere)
	bash scripts/test_gate_term_wordref_ratchet.sh   # ~0.13s, pure source census (no build needed): per-file `Term` word-ref RATCHET for the T-slice cell migration. Cheap+self-contained so it sits beside the stdin gate, ahead of the boards (same ceo grant 2026-08-30). Pins are LOWERED in the landing commit of each T slice; see the file header for the re-pin one-liner.
	bash scripts/test_gate_score_row_rewrites_in_place.sh   # <1s MEASURED, offline (--no-fetch), no build needed: THE ONE LEADERBOARD's write path. Sits with the cheap no-build arms above the boards for the same reason they do. ⭐ WIRED 2026-09-03 (hq_T, row score-md-rows-are-rewritten-by-the-runner-that-measured-them): Lon's FACT RULE that every suite run rewrites its .github/SCORE.md row is only as good as the helper every runner calls, and the one property that decides whether the board is worth reading -- a row is REWRITTEN IN PLACE, never appended beside a stale twin -- fails INVISIBLY. An append bug yields a file that is longer, fuller, holds every number anyone ever measured, and hands a reader grepping for a language the OLDEST one; it would pass any test that only asked 'did the new number show up'. So this gate asserts the new number arrived AND the old one did not survive AND the row count did not move, plus all five refusal paths, on a SCRATCH COPY of the board (a gate that edits the artifact it grades is how a green board and a corrupted file coexist).
	bash scripts/test_gate_seat_identity_one_map.sh   # ~0.8s MEASURED (0.88/0.77/0.81 over three runs, 15 arms), offline, no build: seat identity is ONE map, and no runner may sign a board row with a placeholder. Sits beside the write-path gate above because it grades the other half of the same row -- that one asks whether the number landed in place, this one asks whether anyone is actually behind it. ⭐ WIRED 2026-09-04 (hq_T, row vendor-runners-stamp-unknown-seat-into-the-leaderboard-when-s4e-seat-is-unset, GOAL by hq_C routed by ceo CEO-175): every runner spelled its measurer default ${S4E_SEAT:-unknown-seat}, S4E_SEAT is unset in a plain shell, and so the vendor boards signed THE ONE LEADERBOARD 'unknown-seat' -- which defeated the helper's own 'an unattributed row is a claim with nobody behind it' guard BY BEING NON-EMPTY. The cure DERIVES the identity from the root path rather than refusing harder, because the identity was never unknown: it is a fact on disk. ⛔ THE GATE EXISTS BECAUSE THE CURE LEFT THREE HAND-SYNCED COPIES of that root->seat map (s4e_msg.sh, s4e_inbox_hook.sh, util_score_row.py's derive_measurer). They agree today; nothing MADE them agree, and a drifted map does not crash -- it signs somebody else's name to a measurement, which is the one failure a leaderboard cannot survive and cannot detect. It also PINS the single deliberate divergence (on an unrecognised root the bus NAMES, because a seat with no name cannot be mailed, while the board REFUSES, because a guessed signature is the very claim this row kills), so collapsing them later has to be a deliberate act with a reason instead of a tidy-up. ⛔ THE MAP IS LIFTED FROM THE FILES, NEVER RETYPED IN THE GATE -- a gate carrying its own copy of the expected map would be a FOURTH copy, and would pass while the file it grades was wrong. All 8 arms plus the rc=2 refusal path fail-once proven on a scratch copy of scripts/, never the real tree.
	bash scripts/test_gate_score_column_semantics.sh   # ~0.1s, pure file read (no build): every runner a SCORE.md grid cell cites must be of that COLUMN'S kind — M master · L ladder · V vendor. ⭐ WIRED 2026-09-03 (hq_T, ceo ruling after `reb 100%` went out on the live board): a "|"-split off-by-one put the Rebus LADDER census in the MASTER column, and the readability guard beside it could not object because "22/22" is a well-formed population in a cell allowed to contain one. READABLE AND WRONG is the one shape a parse check can never catch — it is precisely what passing the parse means. Blocking from the start rather than REPORTED, because it is green today: the ramp exists for gates that are already red, not for ones that cost nothing to keep green.
	-bash scripts/test_gate_score_tables_agree.sh   # ~0.1s, pure file read: SCORE.md's September-10 grid and its standardized display must agree BY VALUE. ⭐ WIRED 2026-09-03 (hq_T, ceo approval): `util_score_row.py write` updates only the display, so a grid cell goes stale the moment a runner records a row -- measured four times in one sitting, each found only by a hand edit. ⛔ VALUES, NEVER DATES: same-day staleness (today's date beside a superseded number) is invisible to any freshness check. ⛔ REPORTED, NOT BLOCKING, because it is RED TODAY (2 same-denominator conflicts in Icon, 21 one-sided populations): the ramp exists for gates that are already red, where blocking would stop the very landings that clear them -- the same call made for the xfail census, and the opposite of the column-semantics gate above, which was green on arrival and so went blocking at once.
	bash scripts/test_gate_no_o2_arm_in_scripts.sh   # ~0.05s MEASURED (0.04/0.05/0.04 over three runs, 620 scripts), pure source census (no build): the s262 FACT RULE (NO -O2 BUILDS, EVER) enforced where it is EXECUTED rather than where it is written down. ⭐ WIRED 2026-09-03 (hq_B, row jcon-selfhost-build-carries-a-live-o2-runtime-arm-contradicting-the-s262-fact-rule): test_gate_digest_matches_rules.sh polices the per-root CLAUDE.md DIGESTS, so it answers "does the digest say the right thing", which was read as "is the rule obeyed" -- and TWO live -O2 runtime arms sat in scripts/ for weeks while every digest read clean (jcon_selfhost_build.sh's PERF=1, routed by hq_P; and build_o2_working_snobol4.sh, which nobody had reported and whose whole purpose was `make libscrip_rt RT_OPT="$$O2"`). Both deleted in that landing. ⛔ SCOPED TO RT_OPT/RTOPT ON PURPOSE, not to any -O2 in scripts/: 26 scripts carry a legitimate one (gcc -O2 for tools/bench_rusage, the measurement harness -- building the stopwatch at -O0 adds its own cost to every benchmark number; and `fpc -O2`, the rival at its released default, which IS the fair-comparison basis). A gate broader than its rule gets disabled by the first person it blocks for a good reason.
	bash scripts/test_gate_cross_suite_ref_agreement.sh   # ~0.16s MEASURED (0.13/0.17/0.18 over three runs, 70 byte-identical program pairs across 9 language dirs), pure file parse, no build. ⭐ WIRED 2026-09-03 (hq_B, ceo CEO-177 ruling after the ICN4 FALSE GREEN): two suites held CONTRADICTORY REFS for a BYTE-IDENTICAL program -- ALL.ref said Icon's x[-3+:6] fails, rung36_all.ref said it yields the section, icont agreed with the second. So the master board's two reds were CORRECT OUTPUT FAILING A BAD REF, and the cure changed the COMPILER to match the bad ref: a real regression plus a false green at 380/381 that five green arms AND a ceo audit all missed, because not one of them asked the ORACLE. ⛔ This gate cannot say WHICH ref is right -- only an oracle run can -- but proving the disagreement EXISTS is the cheap half, and it was available for free the whole time. A ref is evidence about a PAST oracle run, not about the oracle.
	bash scripts/test_gate_same_suite_ref_agreement.sh   # ~0.2s MEASURED, pure file parse via corpus_suite_harness's own reader, no build. ⭐ WIRED 2026-09-04 (seat15, task same-suite-contradictory-refs-gate-two-entries-one-program-two-answers, hq_B's find routed by ceo): the SAME-SUITE twin of the gate above -- two entries can share one master (never linked via the cross-suite gate's ALL.csv `origin` mechanism) and still hold contradictory refs for a byte-identical program. Found live wiring this in: Icon entries 318/719 (`procedure_every_to_17`/`_48`) disagreed `3.0/2.0/1.0` vs `3/2/1` -- oracle-verified against real `icont`/`iconx` (`3/2/1` is right; the ref fix EXPOSES a genuine SCRIP defect, `every write(N.0 to M.0 by S.0)` printing a spurious `.0` real Icon does not -- FINDING-2026-09-04-seat15-icon-float-to-by-generator-prints-trailing-decimal-oracle-does-not.md, flagged not fixed, src/ codegen is hq_B's Icon lane not this row's) -- plus a second, unrelated stale ref (`procedure_scan_write_5` missing a legitimate second blank-line write). Both cured in ALL.ref before this gate went blocking; wiring it in RED on day one would have punished the wrong thing. Same "cannot say which side is right, only that a disagreement exists" limit as its sibling -- run the ORACLE, never the compiler, to settle one.
	bash scripts/test_gate_no_xfail_survives.sh   # ~0.4s, pure census (no build): THERE IS NO SUCH THING AS XFAIL (Lon 2026-09-03 21:30, in-chat to ceo: "there is no such thing now as XFAIL. We are shooting for 100%. So if an XFAIL is a faulty test than lets fix all those tests."). ⭐ WIRED 2026-09-03 (hq_T, row test-gate-no-xfail-survives-...): ONE gate for all seven languages, not seven copies, because the suites spell an xfail THREE different ways -- ALL.csv `xfail` column, ALL.xfail lines, and *.xfail marker files -- and snobol4 uses all three (68+136+1) while icon uses two (6+27) and raku only the first (156). A single-source census reads pascal and rebus 0 CORRECTLY and icon 6-instead-of-43 WRONGLY, and both look equally like an answer. ⛔ REPORTED, NOT BLOCKING, and that is a ramp rather than a softening: the gate genuinely returns rc=1 while any xfail survives (XFAIL_GATE_BLOCKING=1 proves it), but wiring it blocking TODAY would red the landing verdict of every seat -- including the landings that reduce the count -- so the gate would prevent the work it exists to cause. Same REPORTED-then-BLOCKING ramp board_packages.sh took; the flip is a date decision for ceo/Lon, not a script author's. It REFUSES rc=2 on an unreachable suite, because a census that cannot see its population must never print 0.
	bash scripts/test_gate_icn_scan_argtype.sh   # ~2.5s MEASURED (2.69/2.43 over two runs, 11 witnesses x 2 modes), and it needs the build the arms above do not: the Icon scan-node builtin argument-type class, re-cut from the icont/iconx oracle EVERY run rather than from a stored .ref -- a .ref only proves "unchanged since someone cut it", which cannot tell a cured builtin from a .ref cut while it was broken. Placed ahead of the ~28s SNOBOL4 board so this class costs 2.5s to falsify, per the cheap-arms-first design of this recipe. Negative-tested by injection (deleting bb_scan_any's check makes it exit 1 naming that one builtin, other 10 green).
	bash scripts/test_corpus_snobol4.sh
	bash scripts/test_gate_emit_no_lang.sh
	bash scripts/test_gate_template_medium_invisible.sh
	bash scripts/test_gate_corpus_coverage_classified.sh
	bash scripts/test_gate_pl_quad_regs.sh   # ~45s (166 Prolog compiles, most refusing during the rebuild): THE QUAD GATE -- no r12-r15 write reachable from a Prolog graph outside the TR/B/ROOT/BALL enrolment, mode-4 TEXT + reachable rtx (hq_P 2026-09-02, ARCH-PROLOG-BYRD-BOX-TRANSLATION.md § E instrument lane). Last in the recipe so every cheaper gate reports first.
	-bash scripts/board_packages.sh   # REPORTED, NOT BLOCKING (MASTER-PLAN I21, task board-packages-into-make-test-reported-then-blocking, D1 stage): runs every vendored package/third-party suite and prints the denominator (suites=/graded=/unproven=). The leading `-` is belt-and-suspenders -- the script's own exit code is already 0 whenever it successfully measured, red constituent suites included, per this stage's own DONE-WHEN -- so a red package suite reports here without failing `make test`; D5 turns the measured floor into a real gate.

pristine-all:  # wipe EVERY cached configuration, not just this one (the pre-s258 behaviour)
	# Same race, same cure, same bounded BUILD_LOCK as `pristine` above -- see its comment block for why
	# the wait is bounded, not silent (make-pristine-per-root-flock-second-builder-waits;
	# build-governor-holds-pristine-while-box-idles).
	@if [ -n "$${S4E_NO_BUILD_LOCK:-}" ]; then \
		echo "⚠ S4E_NO_BUILD_LOCK set -- running pristine-all UNLOCKED (measurement/control-arm only)"; \
		rm -rf $(OBJ) $(ROOT)/out $(ROOT)/scrip; \
		$(MAKE) all; \
	else \
		bash scripts/lib_pristine_lock.sh $(BUILD_LOCK) -- sh -c '\
			rm -rf $(OBJ) $(ROOT)/out $(ROOT)/scrip && \
			$(MAKE) all \
		'; \
	fi

buildinfo:  # ⭐ what am I actually about to link? print it rather than assume it (LAW 0)
	@printf 'RT_OPT     : %s\n' '$(RT_OPT)'
	@printf 'ZCFLAGS    : %s\n' '$(ZCFLAGS)'
	@printf 'RT_TAG     : %s\n' '$(RT_TAG)'
	@printf 'RT_OBJDIR  : %s  (%s objects cached)\n' '$(RT_OBJDIR)' "$$(ls $(RT_OBJDIR)/*.o 2>/dev/null | wc -l)"
	@printf 'RT_SO      : %s\n' '$(RT_SO)'
	@printf 'canonical  : out/libscrip_rt.so -> %s\n' "$$(readlink out/libscrip_rt.so 2>/dev/null || echo '(none)')"
	@printf 'static arm : out/libscrip_rt.a -> %s  (opt-in, m4-static-link-arm; never canonical)\n' "$$(readlink out/libscrip_rt.a 2>/dev/null || echo '(not built)')"
	@printf 'compiler   : %s  (hardcoded -O0; RT_OPT does NOT affect it)\n' '$(OBJ)'
	@printf 'cached tags:\n'; for d in out/rt_pic-*; do [ -d "$$d" ] && printf '   %s  %s objects\n' "$$d" "$$(ls $$d/*.o 2>/dev/null|wc -l)"; done; true

# ── libscrip_rt.so — runtime support library for --native codegen-emit --x64 ────────────
# EM-6: full SNOBOL4 runtime compiled -fPIC and linked into the .so.
# Emitted x86-64 binaries link against this .so for all language-level
# semantics (pattern matcher, NV table, exec_stmt, builtins, GC).
libscrip_rt: out/libscrip_rt.so

# EM-6 runtime objects (all compiled -fPIC so they can go into the .so)
RT_PIC_SRCS := \
    $(RT)/rtx/rtx_misc.s \
    $(RT)/rtx/rtx_zdp.s \
    $(RT)/rtx/rtx_alloc.s \
    $(RT)/rtx/rtx_str.s \
    $(RT)/rtx/rtx_arith.s \
    $(RT)/rtx/rtx_icnvar.s \
    $(RT)/rtx/rtx_icnnum.s \
    $(RT)/rtx/rtx_icnrel.s \
    $(RT)/rtx/rtx_icnagg.s \
    $(RT)/rtx/rtx_match.s \
    $(RT)/rtx/rtx_icngen.s \
    $(RT)/rtx/rtx_icncall.s \
    $(RT)/rtx/rtx_icnsub.s \
    $(RT)/rtx/rtx_plunify.s \
    $(RT)/rtx/rtx_table.s \
    $(RT)/rtx/rtx_init.c \
    $(RT)/rtx/rtcc_init.c \
    $(RT)/rt/rt_slab.c \
    $(RT)/rt/rt_pl_trail.c \
    $(RT)/rt/rt_arena.c \
    $(RT)/rt/rt.c \
    $(RT)/rt/zeta_alloc.c \
    $(RT)/rt/gc_heap.c \
    $(RT)/rt/rt_protected.c \
    $(RT)/rt/pat_pool.c \
    $(RT)/rt/rt_coexpr.c \
    $(RT)/rt/bbprof.c \
    $(RT)/rt/portcount.c \
    $(RT)/rt/porttrace.cpp \
    $(RT)/rt/rt_stack_overflow.c \
    $(SRC)/runtime/core/core.c \
    $(SRC)/runtime/core/argval.c \
    $(SRC)/runtime/core/name_save.c \
    $(SRC)/runtime/core/stmt_exec.c \
    $(SRC)/runtime/runtime_eval.c \
    $(SRC)/runtime/runtime_init.c \
    $(SRC)/runtime/io_format.c \
    $(SRC)/runtime/arithmetic.c \
    $(SRC)/runtime/pattern_match.c \
    $(SRC)/runtime/keywords.c \
    $(SRC)/runtime/string_ops.c \
    $(SRC)/runtime/name_binding.c \
    $(SRC)/runtime/values.c \
    $(SRC)/runtime/invocation.c \
    $(SRC)/runtime/aggregates.c \
    $(SRC)/runtime/string_builtins.c \
    $(SRC)/ir/bb_pool.c \
    $(SRC)/emitter/emit.cpp \
    $(SRC)/emitter/emit_str.cpp \
    $(SRC)/templates/x86/x86_arg_roles.cpp \
    $(SRC)/templates/bb/bb_glue_flat.cpp \
    $(SRC)/templates/bb/bb_main.cpp \
    $(SRC)/templates/bb/bb_glue_framed.cpp \
    $(SRC)/templates/bb/bb_match_lambda.cpp \
    $(SRC)/templates/bb/bb_lit.cpp \
    $(SRC)/templates/bb/bb_keyword_assign_snobol4.cpp \
    $(SRC)/templates/bb/bb_define.cpp \
    $(SRC)/runtime/rt/bb_pat_build.cpp \
    $(SRC)/templates/bb/bb_conjunction.cpp \
    $(SRC)/templates/bb/bb_subscript.cpp \
    $(SRC)/templates/bb/bb_subscript2.cpp \
    $(SRC)/templates/bb/bb_deref.cpp \
    $(SRC)/templates/bb/bb_random.cpp \
    $(SRC)/templates/bb/bb_assign_var.cpp \
    $(SRC)/templates/bb/bb_assign_var_sub.cpp \
    $(SRC)/templates/bb/bb_rev_assign_var.cpp \
    $(SRC)/templates/bb/bb_var_ref.cpp \
    $(SRC)/templates/bb/bb_goto.cpp \
    $(SRC)/templates/bb/bb_bound.cpp \
    $(SRC)/templates/bb/bb_statement.cpp \
    $(SRC)/templates/bb/bb_disjunction.cpp \
    $(SRC)/templates/bb/bb_goto_deferred.cpp \
    $(SRC)/templates/bb/bb_fail.cpp \
    $(SRC)/templates/bb/bb_cut.cpp \
    $(SRC)/templates/bb/bb_lit_scalar.cpp \
    $(SRC)/templates/bb/bb_var.cpp \
    $(SRC)/templates/bb/bb_var_global.cpp \
    $(SRC)/templates/bb/bb_return.cpp \
    $(SRC)/templates/bb/bb_keyword_icon.cpp \
    $(SRC)/templates/bb/bb_keyword_snobol4.cpp \
    $(SRC)/templates/bb/bb_keyword_assign.cpp \
    $(SRC)/templates/bb/bb_unop.cpp \
    $(SRC)/templates/bb/bb_succeed.cpp \
    $(SRC)/templates/bb/bb_every.cpp \
    $(SRC)/templates/bb/bb_to.cpp \
    $(SRC)/templates/bb/bb_match_len.cpp \
    $(SRC)/templates/bb/bb_match_lit.cpp \
    $(SRC)/templates/bb/bb_match_any.cpp \
    $(SRC)/templates/bb/bb_match_notany.cpp \
    $(SRC)/templates/bb/bb_match_span.cpp \
    $(SRC)/templates/bb/bb_match_break.cpp \
    $(SRC)/templates/bb/bb_match_breakx.cpp \
    $(SRC)/templates/bb/bb_match_tab.cpp \
    $(SRC)/templates/bb/bb_coerce_string.cpp \
    $(SRC)/templates/bb/bb_coerce_numeric.cpp \
    $(SRC)/templates/bb/bb_cmp_test.cpp \
    $(SRC)/templates/bb/bb_ident.cpp \
    $(SRC)/templates/bb/bb_differ.cpp \
    $(SRC)/templates/bb/bb_coerce_integer.cpp \
    $(SRC)/templates/bb/bb_coerce_real.cpp \
    $(SRC)/templates/bb/bb_match_rtab.cpp \
    $(SRC)/templates/bb/bb_match_pos.cpp \
    $(SRC)/templates/bb/bb_match_rpos.cpp \
    $(SRC)/templates/bb/bb_match_rem.cpp \
    $(SRC)/templates/bb/bb_match_atp.cpp \
    $(SRC)/templates/bb/bb_match_arb.cpp \
    $(SRC)/templates/bb/bb_match_arbno.cpp \
    $(SRC)/templates/bb/bb_match_bal.cpp \
    $(SRC)/templates/bb/bb_match_fence0.cpp \
    $(SRC)/templates/bb/bb_match_fence1.cpp \
    $(SRC)/templates/bb/bb_match_abort.cpp \
    $(SRC)/templates/bb/bb_zdp_anchor.cpp \
    $(SRC)/templates/bb/bb_match_begin.cpp \
    $(SRC)/templates/bb/bb_match_end.cpp \
    $(SRC)/templates/bb/bb_match_replace.cpp \
    $(SRC)/templates/bb/bb_match_capture.cpp \
    $(SRC)/templates/bb/bb_match_alternate.cpp \
    $(SRC)/templates/bb/bb_match_defer.cpp \
    $(SRC)/templates/bb/bb_match_value.cpp \
    $(SRC)/templates/bb/bb_to_by.cpp \
    $(SRC)/templates/bb/bb_make_list.cpp \
    $(SRC)/templates/bb/bb_activate.cpp \
    $(SRC)/templates/bb/bb_create.cpp \
    $(SRC)/templates/bb/bb_coret.cpp \
    $(SRC)/templates/bb/bb_cofail.cpp \
    $(SRC)/templates/bb/bb_move_label.cpp \
    $(SRC)/templates/bb/bb_indirect_goto.cpp \
    $(SRC)/templates/xa/xa_coexpr_entry.cpp \
    $(SRC)/templates/bb/bb_call.cpp \
    $(SRC)/templates/bb/bb_call_proc_staged.cpp \
    $(SRC)/templates/bb/bb_call_write_slot.cpp \
    $(SRC)/templates/bb/bb_call_bool.cpp \
    $(SRC)/templates/bb/bb_call_fn.cpp \
    $(SRC)/templates/bb/bb_binop_relop.cpp \
    $(SRC)/templates/bb/bb_binop_relop_val.cpp \
    $(SRC)/templates/bb/bb_binop_arith.cpp \
    $(SRC)/templates/bb/bb_binop_concat_slot.cpp \
    $(SRC)/templates/bb/bb_binop_xrep_slot.cpp \
    $(SRC)/templates/bb/bb_assign_local.cpp \
    $(SRC)/templates/bb/bb_assign_global.cpp \
    $(SRC)/templates/bb/bb_field_get.cpp \
    $(SRC)/templates/bb/bb_suspend.cpp \
    $(SRC)/templates/bb/bb_iterate.cpp \
    $(SRC)/templates/bb/bb_repalt.cpp \
    $(SRC)/templates/bb/bb_limit.cpp \
    $(SRC)/templates/bb/bb_section.cpp \
    $(SRC)/templates/bb/bb_rev_assign.cpp \
    $(SRC)/templates/bb/bb_rev_swap.cpp \
    $(SRC)/templates/bb/bb_swap.cpp \
    $(SRC)/templates/bb/bb_swap_var.cpp \
    $(SRC)/templates/bb/bb_proc_value.cpp \
    $(SRC)/templates/bb/bb_call_value.cpp \
    $(SRC)/templates/bb/bb_enter_init.cpp \
    $(SRC)/templates/bb/bb_gen_scan.cpp \
    $(SRC)/templates/bb/bb_scan_tab.cpp \
    $(SRC)/templates/bb/bb_scan_move.cpp \
    $(SRC)/templates/bb/bb_scan_upto.cpp \
    $(SRC)/templates/bb/bb_scan_any.cpp \
    $(SRC)/templates/bb/bb_scan_many.cpp \
    $(SRC)/templates/bb/bb_scan_find.cpp \
    $(SRC)/templates/bb/bb_scan_match.cpp \
    $(SRC)/templates/bb/bb_scan_pos.cpp \
    $(SRC)/templates/bb/bb_scan_bal.cpp \
    $(SRC)/templates/bb/bb_scan_sequence.cpp \
    $(SRC)/templates/bb/bb_scan_alternate.cpp \
    $(SRC)/templates/bb/bb_glit.cpp \
    $(SRC)/templates/bb/bb_gcc.cpp \
    $(SRC)/templates/bb/bb_galt.cpp \
    $(SRC)/runtime/rt_gram_trampoline.S \
    $(SRC)/runtime/rt/rt_sg_scan.S \
    $(SRC)/runtime/rt/rt_asm_helpers.S \
    $(SRC)/templates/xa/xa_stubs.cpp \
    $(SRC)/templates/xa/xa_macro_library.cpp \
    $(SRC)/templates/xa/xa_bb_macro_library.cpp \
    $(SRC)/templates/xa/xa_exec_stmt_blob.cpp \
    $(SRC)/templates/xa/xa_file_header.cpp \
    $(SRC)/templates/xa/xa_bb_ptr_slot.cpp \
    $(SRC)/templates/xa/xa_flat.cpp \
    $(SRC)/templates/xa/xa_prologue.cpp \
    $(SRC)/templates/xa/xa_epilogue.cpp \
    $(SRC)/templates/xa/xa_wasm_main.cpp \
    $(SRC)/templates/xa/xa_js_label_register.cpp \
    $(SRC)/templates/xa/xa_expression_registry.cpp \
    $(SRC)/templates/xa/xa_strtab_rodata.cpp \
    $(SRC)/templates/xa/xa_csettab_rodata.cpp \
    $(SRC)/templates/xa/xa_cap_fixup.cpp \
    $(SRC)/templates/xa/xa_pattern_blobs.cpp \
    $(SRC)/optimizer/gva_collect.c \
    $(SRC)/optimizer/proc_collect.c \
    $(SRC)/optimizer/ir_query.c \
    $(SRC)/optimizer/region_report.c \
    $(SRC)/optimizer/branch_chain.c \
    $(SRC)/optimizer/const_fold.c \
    $(SRC)/optimizer/copy_prop.c \
    $(SRC)/optimizer/pat_fold.c \
    $(SRC)/optimizer/dead_pure.c \
    $(SRC)/optimizer/dead_goto.c \
    $(SRC)/optimizer/optimizer.c \
    \
    $(SRC)/ir/sm_prog.c \
    $(SRC)/lower/lower_common.c \
    $(SRC)/lower/tree_to_sno.c \
    $(SRC)/lower/lower_icon.c \
    $(SRC)/lower/lower_snobol4.c \
    $(SRC)/lower/lower_prolog.c \
    $(SRC)/lower/lower_raku.c \
    $(SRC)/lower/lower_pascal.c \
    $(SRC)/runtime/builtins/gen_runtime.c \
    $(SRC)/runtime/by_name_dispatch.c \
    $(SRC)/runtime/unification.c \
    $(SRC)/runtime/core/coerce.c \
    $(SRC)/ir/scrip_ir.c \
    $(SRC)/ir/zeta_storage.c \
    $(SRC)/ir/zeta_depth.c \
    $(SRC)/runtime/rt_runtime.c \
    $(SRC)/driver/driver_globals.c \
    $(SRC)/driver/driver_label.c \
    $(SRC)/driver/driver_hooks.c \
    $(SRC)/driver/driver_data.c \
    $(SRC)/driver/driver_call.c \
    $(SRC)/driver/scrip_sm.c \
    $(SRC)/driver/stmt_ast.c \
    $(SRC)/driver/polyglot.c \
    $(SRC)/ir/ast_print.c \
    $(SRC)/parsers/snobol4/snobol4.tab.c \
    $(SRC)/parsers/snobol4/snobol4.lex.c \
    $(SRC)/parsers/icon/icon_runtime.c \
    $(SRC)/parsers/icon/icon_parse.c \
    $(SRC)/parsers/icon/icon_lex.c \
    $(SRC)/parsers/icon/icon_driver.c \
    $(SRC)/parsers/prolog/prolog_lex.c \
    $(SRC)/parsers/prolog/prolog_parse.c \
    $(SRC)/parsers/prolog/prolog_atom.c \
    $(SRC)/parsers/prolog/prolog_driver.c \
    $(SRC)/parsers/prolog/prolog_lower.c \
    $(SRC)/parsers/snocone/snocone_lex.c \
    $(SRC)/parsers/snocone/snocone_parse.tab.c \
    $(SRC)/parsers/snocone/snocone_driver.c \
    $(SRC)/parsers/raku/raku.tab.c \
    $(SRC)/parsers/raku/raku.lex.c \
    $(SRC)/parsers/raku/raku_driver.c \
    $(SRC)/parsers/raku/re.c \
    $(SRC)/parsers/rebus/rebus.tab.c \
    $(SRC)/parsers/rebus/lex.rebus.c \
    $(SRC)/parsers/rebus/rebus_lower.c \
    $(SRC)/parsers/pascal/pascal.tab.c \
    $(SRC)/parsers/pascal/pascal.lex.c \
    $(SRC)/parsers/pascal/pascal_driver.c \
    $(SRC)/templates/bb/bb_arith.cpp \
    $(SRC)/templates/bb/bb_binop_gvar_arith.cpp \
    $(SRC)/templates/bb/bb_binop_gvar_arith_slot.cpp \
    $(SRC)/templates/bb/bb_call_define.cpp \
    $(SRC)/templates/bb/bb_case_arm.cpp \
    $(SRC)/templates/bb/bb_det_nl.cpp \
    $(SRC)/templates/bb/bb_idx_get.cpp \
    $(SRC)/templates/bb/bb_idx_set.cpp \
    $(SRC)/templates/bb/bb_indirect_assign_lit_s.cpp \
    $(SRC)/templates/bb/bb_indirect_assign_var.cpp \
    $(SRC)/templates/bb/bb_initial.cpp \
    $(SRC)/templates/bb/bb_key_gen.cpp \
    $(SRC)/templates/bb/bb_ref_invariant.cpp \
    $(SRC)/templates/bb/bb_subject.cpp \
    $(SRC)/templates/bb/bb_unop_gvar_slot.cpp \
    $(SRC)/templates/bb/bb_assign_frame.cpp \
    $(SRC)/templates/bb/bb_var_frame.cpp \
    $(SRC)/templates/bb/bb_var_frame_ref.cpp

# ⛔ RT_OPT IS DEFINED ONCE, AT LINE 34. A SECOND `RT_OPT ?=` stood here carrying the RETIRED O0-DEV-O2-BENCH text (s179) — inert by `?=` but read as law by anyone who greps for the flag and lands on the wrong one, which is how a NO-O2-BUILDS violation gets written in good faith. Deleted hq_P s269; the s262 FACT RULE lives at :34 and nowhere else.
RT_INCS := -I$(SRC) -I$(SRC)/ir -I$(SRC)/lower -I$(SRC)/emitter -I$(SRC)/runtime/core -I$(SRC)/runtime/builtins -I$(RT) -I$(RT)/rt \
    -I$(SRC)/parsers/snobol4 -I$(SRC)/parsers/raku -I$(SRC)/optimizer -I$(SRC)/templates/bb -I$(SRC)/templates/xa -I$(SRC)/templates/x86
# ⭐⭐ BUILD CACHE KEYED BY THE FLAGS THAT PRODUCED IT (Lon 2026-08-22 s258, in-chat: "can we enforce a
# pure incremental build and keep these objects around longer ... That is killing us concerning optimized
# tests"). MEASURED CAUSE, and it is NOT header fan-out (that was the hypothesis; transitive-include census
# over 274 TUs says NO header invalidates the whole set -- worst is descr.h at 80.3%):
#   CBASE/CXXRT hardcode -O0, so ONLY this runtime honours RT_OPT. RT_OBJDIR was ONE directory shared by
#   every RT_OPT value, and `pristine` deletes it -- so -O0 and -O2 objects could NEVER coexist and every
#   switch threw the other away and paid full price again (1m40 at -O0 vs 9m30 at -O2).
# ⛔ AND THERE WAS A SILENT WRONG BUILD UNDERNEATH IT, which is why the culture was "always pristine":
#   with no flag stamp, a plain `RT_OPT="-O2 ..." make` after an -O0 build found every .o newer than its
#   .c, did NOTHING, and handed back an -O0 runtime while the caller believed it was -O2. That is the
#   HQ-27 ABI-mix class through a different door, and it is why nobody could trust incremental.
# Keying the directory by the flags fixes both at once: mixing is now structurally impossible (different
# directories), so incremental is trustworthy, so pristine stops being mandatory for a flag switch.
# Pay -O2 ONCE, then switch arms in seconds, forever.
# ⛔ NORMALISE BEFORE HASHING. `RT_OPT ?= -O0 ... # comment` leaves TRAILING SPACES in the value (make
# keeps everything up to the #), so a bare `make` and an explicit RT_OPT="<the same flags>" would hash
# differently and silently keep two identical caches. `tr -s` + strip makes the key depend on the FLAGS,
# not on their spacing.
RT_TAG    := $(shell printf '%s|%s' '$(strip $(RT_OPT))' '$(strip $(ZCFLAGS))' | tr -s ' ' | md5sum | cut -c1-10)
RT_OBJDIR := out/rt_pic-$(RT_TAG)
RT_SO     := out/libscrip_rt-$(RT_TAG).so
RT_A      := out/libscrip_rt-$(RT_TAG).a
RT_PIC_OBJS := $(addprefix $(RT_OBJDIR)/,$(addsuffix .o,$(basename $(notdir $(RT_PIC_SRCS)))))
vpath %.c $(sort $(dir $(RT_PIC_SRCS)))
vpath %.cpp $(sort $(dir $(RT_PIC_SRCS)))
vpath %.S $(sort $(dir $(RT_PIC_SRCS)))
vpath %.s $(sort $(dir $(RT_PIC_SRCS)))
$(RT_OBJDIR):
	@mkdir -p $(RT_OBJDIR)
$(RT_OBJDIR)/%.o: %.c $(RT)/rt/rt.h | $(RT_OBJDIR)
	$(CC) $(RT_OPT) -g $(WARN) $(DEPFLAGS) -fPIC $(RT_INCS) $(ZCFLAGS) -c $< -o $@
$(RT_OBJDIR)/%.o: %.cpp $(RT)/rt/rt.h | $(RT_OBJDIR)
	$(CC) $(RT_OPT) -g $(WARN) $(DEPFLAGS) -fPIC -std=c++17 -finput-charset=UTF-8 $(RT_INCS) $(ZCFLAGS) -c $< -o $@
$(RT_OBJDIR)/%.o: %.S | $(RT_OBJDIR)
	$(CC) $(RT_OPT) -g $(WARN) $(DEPFLAGS) -fPIC $(RT_INCS) $(ZCFLAGS) -c $< -o $@
$(RT_OBJDIR)/%.o: %.s | $(RT_OBJDIR)
	$(CC) $(RT_OPT) -g $(WARN) $(DEPFLAGS) -fPIC $(RT_INCS) $(ZCFLAGS) -x assembler-with-cpp -c $< -o $@
$(RT_SO): $(RT_PIC_OBJS)
	@mkdir -p out
	$(CC) -shared $(RT_PIC_OBJS) -lm -lstdc++ -lpthread -o $@
	@echo "Built: $@   RT_OPT=$(RT_OPT)"
# out/libscrip_rt.so REMAINS THE CANONICAL PATH -- 73 scripts reference it by that exact name, so it must
# never move. It is a symlink to whichever configuration was actually built.
# ⛔ THE FORCE PREREQUISITE IS LOAD-BEARING, NOT A TIC: make resolves a symlink to its TARGET's timestamp,
# so switching back to a previously-built (older) tag would leave the link pointing at the newer wrong .so
# and make would call it up to date. That is precisely the silent-wrong-build class this rung exists to
# kill, so the link is refreshed unconditionally. It costs one symlink call.
out/libscrip_rt.so: $(RT_SO) FORCE
	@ln -sfn $(notdir $(RT_SO)) out/libscrip_rt.so
	@echo "out/libscrip_rt.so -> $(notdir $(RT_SO))   RT_OPT=$(RT_OPT)"

# ── libscrip_rt.a — static-archive TWIN of libscrip_rt.so, for the OPT-IN m4 STATIC link arm ────
# Row m4-static-link-arm (Lon 2026-08-28, FLEET-6). ADDITIVE ONLY: out/libscrip_rt.so above REMAINS
# canonical -- this archives the SAME $(RT_PIC_OBJS) already built for the .so (no separate compile,
# no separate flags) so `ar` is the only new cost. FINDING f4f6292c measured the shared .so's mere
# page-in as a fixed floor (430 faults / 7.4MB / ~1.8ms before any program work); static linking
# against this archive is the one lever there that actually worked (-51% faults on a do-nothing
# program, -29% on treebank-match, byte-identical output) -- at a real price (~27-30MB/binary) that
# is why it is a SEPARATE opt-in target and never wired into `scrip`/`libscrip_rt` above.
$(RT_A): $(RT_PIC_OBJS)
	@mkdir -p out
	ar rcs $(RT_A) $(RT_PIC_OBJS)
	@echo "Built: $@   RT_OPT=$(RT_OPT)"
# Same FORCE-symlink-refresh reasoning as out/libscrip_rt.so above -- a stale symlink surviving a
# RT_TAG switch would silently statically-link the WRONG configuration.
out/libscrip_rt.a: $(RT_A) FORCE
	@ln -sfn $(notdir $(RT_A)) out/libscrip_rt.a
	@echo "out/libscrip_rt.a -> $(notdir $(RT_A))   RT_OPT=$(RT_OPT)"
libscrip_rt_static: out/libscrip_rt.a
FORCE:

# ── EM-2 synthetic-program harness — RETIRED (SMX-4, 2026-05-30): sm_codegen_x64_emit
# and its test driver were part of the now-deleted Stack-Machine native emitter.


# ── EM-7a/EM-7b unit tests retired 2026-05-19 (EC-BB-UNIFY-2): the underlying
# compile-time PATND_t API converted to IR_t*. The tests asserted XKIND_t
# fields (kind/nchildren/children) and the patnd_is_fully_invariant(PATND_t*)
# signature, both of which no longer exist for the compile-time walker.
# Coverage subsumed by GATE-2 (broker) and the mode-4 compile gate.

# ── scrip — unified driver (all modes, all frontends) ────────────────────────
# WASM removed from scrip build (2026-04-08): --native codegen-emit --wasm / emit_wasm.c
# dropped. Use scrip legacy driver if WASM emission is ever needed.

# scrip: links against out/libscrip_rt.so (dynamic) so that runtime edits only require
# `make libscrip_rt` + rerun, not a full 118s `make scrip`.  Lon directive 2026-07-25.
# For consistency, --run (mode-3) and --compile (mode-4) both use the same libscrip_rt.so.
# RPATH baked in so the binary finds the .so at out/libscrip_rt.so relative to the repo root.
scrip: libscrip_rt
	@mkdir -p $(OBJ)
	$(CC) $(CRT) -c $(SRC)/driver/scrip.c -o $(OBJ)/scrip_driver.o
	$(CXX) -m64 -no-pie -rdynamic $(OBJ)/scrip_driver.o \
	    -Wl,-rpath,$(abspath out) -L$(abspath out) -lscrip_rt \
	    $(LIBS) -o scrip
	@echo "Built: scrip (dynamic, links out/libscrip_rt.so)"


# test_emit_io retired (2026-05-25): it existed only to keep the dead g_text_buf/g_bin_buf
# buffered path exercised. NO-BUFFERS ruling removed that path; emit_io.c is now pure passthrough.

# ── scrip-monitor: scrip with CSNOBOL4 4th executor linked in (IM-15b) ───────
# Build: make scrip-monitor CSN_A=/home/claude/csnobol4/libcsnobol4.a
# Requires: bash scripts/build_csnobol4_archive.sh first
CSN_A   ?= $(ROOT)/../csnobol4/libcsnobol4.a
CSN_INC ?= $(ROOT)/../csnobol4

scrip-monitor:
	@# Build all scrip objects, then relink with CSNOBOL4 4th executor
	$(MAKE) -f Makefile scrip
	$(CC) $(CRT) -DWITH_CSNOBOL4=1 -I$(CSN_INC) \
	      -c $(SRC)/driver/csnobol4_shim.c -o $(OBJ)/csnobol4_shim_csn.o
	$(CC) $(CRT) -DWITH_CSNOBOL4=1 \
	      -c $(SRC)/driver/sync_monitor.c -o $(OBJ)/sync_monitor_csn.o
	$(CC) -m64 -no-pie \
	      $(OBJ)/csnobol4_shim_csn.o $(OBJ)/sync_monitor_csn.o \
	      $(filter-out $(OBJ)/sync_monitor.o $(OBJ)/sync_monitor_csn.o $(OBJ)/csnobol4_shim.o $(OBJ)/csnobol4_shim_csn.o $(OBJ)/scrip_driver.o, $(wildcard $(OBJ)/*.o)) \
	      $(OBJ)/scrip_driver.o \
	      $(CSN_A) $(LIBS) -lutil -ldl -lz -lbz2 -o scrip-monitor
	@echo "Built: scrip-monitor (with CSNOBOL4 4th executor)"

# ── monitor_ipc.so ────────────────────────────────────────────────────────────

monitor-ipc:
	gcc -shared -fPIC \
	    -o test/monitor/monitor_ipc.so \
	    test/monitor/monitor_ipc.c
	@echo "Built: test/monitor/monitor_ipc.so"

# ── Environment setup (idempotent) ────────────────────────────────────────────

# ── Git hooks ─────────────────────────────────────────────────────────────────
# ⛔⭐ GIT HOOKS DO NOT PROPAGATE THROUGH CLONE, so every seat must install them locally. The
# live path is the per-prompt session hook (scripts/s4e_inbox_hook.sh calls the installer
# --quiet); this target is the explicit manual entry point and the one `make setup` runs.
hooks:
	@bash $(ROOT)/scripts/install_commit_msg_hook.sh

# ⛔ MEASURED 2026-08-30 (hq_B): $(ROOT)/setup.sh DOES NOT EXIST in this tree, so `make setup`
# was dying at the bash line having done nothing -- a target that looks like a bootstrap and
# cannot be one. That is why hook installation was NOT wired here despite the row's brief
# offering it: wiring into a dead path installs nothing, forever. `hooks` runs FIRST so the one
# thing this target can actually do is done, and the missing script is now named out loud
# instead of surfacing as a bare rc=127.
setup: hooks
	@# setup.sh never existed in this tree (hq_B measured the dead path 2026-08-30); the bootstrap IS these two scripts, called directly so the README's `make setup` line is true rather than a rc=127 grave.
	bash $(ROOT)/scripts/install_system_packages.sh
	bash $(ROOT)/scripts/build_official_oracles.sh

# ── Test targets ──────────────────────────────────────────────────────────────
# (the old --run corpus runners were removed with the interpreter; the live
#  gates are the scripts/ suite — see GOAL-SNOBOL4-BB.md Session Setup.)

# ── EM-9: native codegen-emit-test — smoke + em8 gate for --native codegen-emit --x64 ──────────────
# Runs: test_smoke_snobol4.sh (7/7) + test_gate_em8_snocone_jit_emit.sh (5/5).
# Requires: libscrip_rt (built into out/libscrip_rt.so).
native codegen-emit-test: scrip libscrip_rt
	@bash scripts/test_smoke_snobol4.sh
	@bash scripts/test_gate_em8_snocone_jit_emit.sh

.PHONY: native codegen-emit-test

# ── Runner wrappers ───────────────────────────────────────────────────────────

run: scrip
	./scrip $(SNO)

# Legacy JVM runner — uses old scrip text emitter until M-JITEM-JVM lands
run-jvm: scrip
	@mkdir -p $(JVM_CACHE); \
	base=$$(basename $(SNO) .sno); \
	hash=$$(echo $(SNO) | md5sum | cut -c1-8); \
	key=$${base}_$${hash}; \
	jfile=$(JVM_CACHE)/$${key}.j; \
	stamp=$(JVM_CACHE)/$${key}.stamp; \
	$(SCRIP_CC_BIN) -jvm $(SNO) > $$jfile; \
	classname=$$(grep '\.class' $$jfile | head -1 | awk '{print $$NF}'); \
	j_md5=$$(md5sum $$jfile | cut -d' ' -f1); \
	cached_md5=$$(cat $$stamp 2>/dev/null || echo ''); \
	if [ "$$j_md5" != "$$cached_md5" ] || [ ! -f $(JVM_CACHE)/$$classname.class ]; then \
	    java -jar $(JASMIN) $$jfile -d $(JVM_CACHE) >/dev/null; \
	    echo "$$j_md5" > $$stamp; \
	fi; \
	java -cp $(JVM_CACHE) $$classname

# Legacy .NET runner — uses old scrip text emitter until M-JITEM-NET lands
run-net: scrip
	@mkdir -p $(NET_CACHE); \
	base=$$(basename $(SNO) .sno); \
	hash=$$(echo $(SNO) | md5sum | cut -c1-8); \
	key=$${base}_$${hash}; \
	il=$(NET_CACHE)/$${key}.il; \
	exe=$(NET_CACHE)/$${key}.exe; \
	stamp=$(NET_CACHE)/$${key}.stamp; \
	$(SCRIP_CC_BIN) -net $(SNO) > $$il; \
	il_md5=$$(md5sum $$il | cut -d' ' -f1); \
	cached_md5=$$(cat $$stamp 2>/dev/null || echo ''); \
	if [ "$$il_md5" != "$$cached_md5" ] || [ ! -f $$exe ]; then \
	    ilasm $$il /output:$$exe >/dev/null; \
	    echo "$$il_md5" > $$stamp; \
	fi; \
	mono $$exe

# ── Clean ─────────────────────────────────────────────────────────────────────

clean:
	rm -rf $(OBJ) out scrip

distclean: clean
	rm -rf $(JVM_CACHE) $(NET_CACHE) /tmp/snobol4_asm_* /tmp/scrip_cc_*

# ── Header dependency tracking (s159) ─────────────────────────────────────────
# -MMD -MP emits a .d beside every .o; including them makes a header edit rebuild
# every TU that includes it.  Before this, editing emit.h rebuilt NOTHING and make
# silently linked an ABI-mismatched binary (the s157 bisect produced impossible
# results for exactly this reason).  find, not wildcard: objects nest in subdirs.
-include $(shell find $(OBJ) $(RT_OBJDIR) -name "*.d" 2>/dev/null)
