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
	bash scripts/test_gate_next_honours_the_lane_cut.sh   # ~1.07s MEASURED (1.07/1.08, 10 arms), hermetic (its own scratch postoffice under mktemp; the live QUEUE.tsv is byte-identical across a run, checked). The LANE half of the picker: dependency-promotion honours the lane cut and the MODE language freeze, AND a FREE row owned by the seat's OWN HQ is served. ⭐ WIRED 2026-09-04 (hq_T, ceo URGENT: six seats idle at load 0.65 with sixteen up, every lane reporting "queue empty" while the queue held seven FREE rank<=1 rows per lane -- the owner-cell skip treated an HQ exactly like a rival seat, and under THE SNOBOL4 CUT every class row carries its HQ's owner cell). ⛔ WIRED HERE RATHER THAN AS A SECOND GATE FILE: the ceo's row asked for a new test_gate_next_never_serves_a_row_outside_the_seats_lane.sh, and writing one would have duplicated coverage this gate already holds -- the lane filter lives here, so its arms do too. A second gate over one property is the copy problem wearing a test's clothes. Sits beside test_gate_s4e_next_honours_owner.sh, which owns the other half (the owner cell itself); together they are the picker's two axes.
	bash scripts/test_gate_s4e_next_honours_owner.sh
	bash scripts/test_gate_s4e_next_tiebreak_by_mint_time.sh
	bash scripts/test_gate_s4e_unclaim_keeps_park.sh
	bash scripts/test_gate_s4e_done_timeout_is_a_refusal.sh
	bash scripts/test_gate_s4e_next_refuses_hq_under_ceo_mode.sh
	bash scripts/test_gate_s4e_donewhen_refuses_when_compiler_absent.sh
	bash scripts/test_gate_picker_autounblock.sh
	bash scripts/test_gate_dispatch_claim_single_authority.sh
	bash scripts/test_gate_dispatch_gc_safepoint_inline.sh
	bash scripts/test_gate_dispatch_bus_failure_modes.sh
	bash scripts/test_gate_dispatch_refuses_placeholder_donewhen.sh
	@echo "  ✅ test-postoffice: hermetic s4e_* gates green (count them in this recipe, never from a remembered number)"

test: scrip  # ⭐ WAS THE FALSE-GREEN TRAP (cured hq_P s268): `test`, `test-ir` and `test-all` were named in .PHONY with NO RECIPE ANYWHERE, so each exited 0 having run NOTHING ("Nothing to be done for 'test'") while reading as a full green suite. `test-ir` and `test-all` are DELETED rather than wired — nothing behind them ever existed. This target now runs THE blocking set named in CLAUDE.md and fails loudly on the first red. ⛔ HQ-27 PRISTINE-BEFORE-VERDICT IS VOID as a per-landing requirement (Lon 2026-09-03 ~15:58, RULES.md FACT RULE: "It's time to loosen this pristine build that keeps preventing forward progress and causes 20 minute wait times."). A landing verdict runs on an INCREMENTAL make -- never on a stale binary, and the stale-binary refusal stays as the guard. `make pristine` survives for the ceo's audits, release points, and whenever that refusal fires; the receipt names the build it graded on. This comment asserted the retired law for hours after it was retired, which is the failure mode the FACT RULE exists to prevent: law that lives in a second unversioned place cannot be retired in one edit.
	python3 scripts/strip_comments.py --check   # ~0.8s MEASURED (0.79/0.83/0.80 over three runs, 384 files), the cheapest arm here and the only one needing no build at all, so a style red fails in under a second instead of after a 6-minute board. ⭐ WIRED 2026-09-03 (hq_P, row strip-comments-check-is-not-in-make-test-so-src-style-goes-red-on-origin, minted by hq_C): RULES.md § C style -- src/ carries ZERO comments but the sanctioned 200-char /*---*/ and /*===*/ separators -- was enforced ONLY by a clause each Prolog rung's DONE-WHEN opted into BY HAND. A seat that omitted it pushed a RED gate to origin while `make test` still said green for everyone, and the redness stayed invisible until some LATER seat's DONE-WHEN ran on the merged tree. ⛔ TWICE IN ONE EVENING, hq_B's 92d300f07 and hq_P's own 84e02570, NEITHER SEAT AT FAULT in any way a reviewer would catch: a long explanatory comment is the NATURAL way to make a deletion or a shared-box change legible, and this codebase has deliberately closed that door and routed the explanation to the FINDING. A rule people must REMEMBER to copy into their own acceptance test is a hope, not a mechanism -- the same false-green shape this very target was cured of at s268.
	bash scripts/test_gate_our_files_are_lf.sh   # <1s, pure census (no build): OUR FILES ARE LF (Lon 2026-09-03 "Do not use CRLF, use LF."; 2026-09-04 "Fix the CRLF to LF problem. Why am I still hearing about that?"). Every tracked file in SCRIP, corpus, .github minus corpus/packages/ (vendor fixtures keep the oracle's own CRLF, s183) and generated flex/bison outputs; a CR at end of line is red, a CR byte inside a string literal is data. WIRED 2026-09-04 (ceo, Lon order): the csv.writer default lineterminator re-minted every ALL.csv as CRLF after each rebuild and no gate saw it.
	bash scripts/test_gate_parser_generated_files_in_sync.sh   # ~1.0s MEASURED (0.91/0.92/0.98 over three runs, 14 generated files from 9 grammar sources across 5 parser dirs), no build needed -- it runs the pinned bison/flex itself: EVERY COMMITTED bison/flex OUTPUT MATCHES A FRESH REGENERATION FROM ITS .y/.l. ⭐ WIRED 2026-09-04 (hq_T, row parser-sources-and-generated-files-in-sync-gate-in-make-test, minted by ceo after TWO sessions lost a build to the same trap in one day -- ceo 16:25 on raku.y, hq_C 18:1x on snobol4.l). The generated parsers are TRACKED and this Makefile compiles them as-is with NO bison/flex rule, so editing a grammar changes NOTHING: make succeeds, the binary relinks, every board stays green, and the edit is simply not in the program. Neither session was careless -- editing the .y IS the correct way to change the grammar, and the build cannot warn because from make's point of view nothing it knows about changed. ⛔ IT DIFFS, IT DOES NOT COMPARE mtimes: a fresh clone stamps every file with one checkout time and a stray touch would red a consistent tree, so the only question with a true answer is whether regeneration reproduces the committed bytes (#line excluded, being the one legitimately path-dependent line). ⛔ REFUSES rc=2 -- never a false red -- when bison/flex cannot generate OR report a version other than the pinned 3.8.2/2.6.4, because a different bison emits different tables from identical input and would red all five grammars while blaming the tree. ⭐ WHY IT SURVIVED THIS LONG: nothing in this recipe regenerated a grammar, so the whole regen path sat DEAD for ten days (bison could not run at all until seat03 cured it earlier today) without one board going red -- a capability nobody exercises is not a capability anybody has, and this arm is what exercises it. Blocking from the start rather than REPORTED because it is green on arrival, the same call the column-semantics gate got.
	-bash scripts/test_gate_umbrella_done_whens_anchor_on_the_board_line.sh   # ~0.1s MEASURED (0.04/0.04/0.04 over three runs, 12 umbrella batons censused), hermetic baton-text read: no build, no suite run, no oracle. EVERY UMBRELLA DONE-WHEN READS ITS SCORE FROM THE RUNNER'S OWN LABELLED BOARD LINE. ⭐ WIRED 2026-09-04 (hq_T, row umbrella-done-whens-anchor-on-the-runners-own-board-line-never-on-any-line-with-pass, ceo mint on hq_B + seat02's flag): the ceo's umbrella template grepped a runner's WHOLE stdout for PASS=[1-9] to prove it measured and for (FAIL|REJECT|CRASH|HANG)=[1-9] to red it, which assumes every score-shaped token in the stream belongs to the score. ⛔ FALSE IN BOTH DIRECTIONS OUT OF ONE ASSUMPTION, which is why curing the arm that bit you leaves the class alive: CEO-251 REQUIRED informational arms beside the score, so snoflake's sbl-vs-@expect line and csnobol4's triangulation line carry FAIL= BY DESIGN and red a PERFECT suite -- while dotnet, which spells its count lowercase m3_pass=, read as 'printed no PASS count' and refused. EXTRACTION ANCHORS ON IDENTITY, NEVER ON POSITION. ⛔ THE GATE ACCEPTS AN ANCHOR IN THE PATTERN, not only one in a prior extraction -- a single grep for "^CSNOBOL4_SUITE_BOARD total=... m3_FAIL=0" names the line it means and cannot match an informational arm, and judging by the piped-in variable alone flagged that correctly-pinned baton on the first run; flagging a baton that already did the right thing is how a gate teaches its lane to stop reading it. ⛔ SCOPED TO THE RULING AND NO WIDER: inria's whole-stdout grep for 445/445 is the same FAMILY and is printed as a NAMED OBSERVATION, never counted in the verdict, because a gate broader than its rule gets switched off by the first person it blocks for a good reason. REPORTED, NOT BLOCKING, because it is RED TODAY (3 of 12 -- pascal-pat, prolog-gnu, prolog-swi, all non-SNOBOL4 lanes parked under QUARTET): the ramp exists for gates that are already red, where blocking would stop the very landings that clear them, the same call the xfail census and board_packages took. Was 5 of 12; hq_T re-pinned snobol4-aisnobol-and-dotnet on measured board lines this sitting, and it now reds for a REAL defect (aisnobol m3_CRASH=2/m4_CRASH=2) instead of refusing with 'no PASS count'.
	$(MAKE) --no-print-directory test-postoffice   # ~13s MEASURED, no build needed: the hermetic s4e_* gates. SECOND arm by ceo ruling 2026-09-03 (row postoffice-gates-red-on-origin-because-no-s4e-gate-is-in-make-test) -- every seat's next/done/assign rides on this one tool, so a red in it is a red for all sixteen seats at once, and it belongs beside strip_comments as a cheap arm that fails in seconds rather than after a 6-minute board.
	bash scripts/test_gate_capture_stdin_and_red_exit.sh   # ~15s, mktemp-only: first because a cheap self-contained gate belongs before a 6-minute board (ceo grant 2026-08-30; move it if the order should be authored elsewhere)
	bash scripts/test_gate_term_wordref_ratchet.sh   # ~0.13s, pure source census (no build needed): per-file `Term` word-ref RATCHET for the T-slice cell migration. Cheap+self-contained so it sits beside the stdin gate, ahead of the boards (same ceo grant 2026-08-30). Pins are LOWERED in the landing commit of each T slice; see the file header for the re-pin one-liner.
	bash scripts/test_gate_score_row_rewrites_in_place.sh   # <1s MEASURED, offline (--no-fetch), no build needed: THE ONE LEADERBOARD's write path. Sits with the cheap no-build arms above the boards for the same reason they do. ⭐ WIRED 2026-09-03 (hq_T, row score-md-rows-are-rewritten-by-the-runner-that-measured-them): Lon's FACT RULE that every suite run rewrites its .github/SCORE.md row is only as good as the helper every runner calls, and the one property that decides whether the board is worth reading -- a row is REWRITTEN IN PLACE, never appended beside a stale twin -- fails INVISIBLY. An append bug yields a file that is longer, fuller, holds every number anyone ever measured, and hands a reader grepping for a language the OLDEST one; it would pass any test that only asked 'did the new number show up'. So this gate asserts the new number arrived AND the old one did not survive AND the row count did not move, plus all five refusal paths, on a SCRATCH COPY of the board (a gate that edits the artifact it grades is how a green board and a corrupted file coexist).
	bash scripts/test_gate_seat_identity_one_map.sh   # ~0.8s MEASURED (0.88/0.77/0.81 over three runs, 15 arms), offline, no build: seat identity is ONE map, and no runner may sign a board row with a placeholder. Sits beside the write-path gate above because it grades the other half of the same row -- that one asks whether the number landed in place, this one asks whether anyone is actually behind it. ⭐ WIRED 2026-09-04 (hq_T, row vendor-runners-stamp-unknown-seat-into-the-leaderboard-when-s4e-seat-is-unset, GOAL by hq_C routed by ceo CEO-175): every runner spelled its measurer default ${S4E_SEAT:-unknown-seat}, S4E_SEAT is unset in a plain shell, and so the vendor boards signed THE ONE LEADERBOARD 'unknown-seat' -- which defeated the helper's own 'an unattributed row is a claim with nobody behind it' guard BY BEING NON-EMPTY. The cure DERIVES the identity from the root path rather than refusing harder, because the identity was never unknown: it is a fact on disk. ⛔ THE GATE EXISTS BECAUSE THE CURE LEFT THREE HAND-SYNCED COPIES of that root->seat map (s4e_msg.sh, s4e_inbox_hook.sh, util_score_row.py's derive_measurer). They agree today; nothing MADE them agree, and a drifted map does not crash -- it signs somebody else's name to a measurement, which is the one failure a leaderboard cannot survive and cannot detect. It also PINS the single deliberate divergence (on an unrecognised root the bus NAMES, because a seat with no name cannot be mailed, while the board REFUSES, because a guessed signature is the very claim this row kills), so collapsing them later has to be a deliberate act with a reason instead of a tidy-up. ⛔ THE MAP IS LIFTED FROM THE FILES, NEVER RETYPED IN THE GATE -- a gate carrying its own copy of the expected map would be a FOURTH copy, and would pass while the file it grades was wrong. All 8 arms plus the rc=2 refusal path fail-once proven on a scratch copy of scripts/, never the real tree.
	bash scripts/test_gate_ladder_walk_v4_exempts_package_class_rows.sh   # <1s MEASURED, hermetic (scratch plan + scratch postoffice under mktemp; never reads the live one), no build: the ladder walk's V4 RANK INVERSION exempts <lang>-<package>-class-* rows BY SHAPE and COUNTS them on one line. ⭐ WIRED 2026-09-04 (hq_T, row ladder-walk-v4-exempts-package-class-rows, ceo CEO-230 on RULES.md § THE PACKAGE-CLASS RULE): class rows minted from a vendored-package census are ladder-rank by law since Lon 13:20, and the walk the ceo reads every tick printed 27 of them among 62 V4 lines -- a report that is half noise is a report nobody reads. Exempt by shape rather than by a list of package names because a list drifts silently the day a new suite is vendored; the control arm proves a genuine off-ladder rank-1 row is STILL flagged, and a two-token -master-red-class- row (a board class, not a package class) stays a candidate. Sits with the cheap no-build arms for the same reason they do.
	bash scripts/test_gate_harness_refusal_is_rc2.sh   # ~10.5s MEASURED (11.01/10.08 over two runs, 8 arms), needs the build (it grades the real rebus master twice, deliberately): the suite harness has ONE refusal code and it is rc=2. ⭐ WIRED 2026-09-04 (hq_T, row harness-refusal-exit-code-unified-on-rc-2, ceo CEO-233 on hq_T's own receipt): corpus_suite_harness.py refuse() exited 3 by local convention while lib_gate.sh and every bash gate say a refusal is 2, and the stale-binary preflight landing INSIDE that harness left one tool carrying two refusal codes -- a caller could not ask "did it refuse?" without knowing which refusal it hit. ⛔ THE ARMS THAT MATTER MOST ARE 3 AND 4, which assert rc=1 (measured-and-red) and rc=0 did NOT move: unifying refusals downward is one edit away from collapsing could-not-measure into measured-and-red, and a gate proving only "refusals are 2" would pass on a harness that returned 2 for everything, so arm 3 grades a REAL master with a REAL injected red. ⭐ Writing it found a live defect one layer below the row: a missing ALL.<ext>/ALL.ref reached read_text() and died with a FileNotFoundError traceback -- Python exits 1 for that, and rc=1 here means "some entries are RED", so the one case where NOTHING was graded returned the code for a measured red board. Cured in cmd_run; arm 1 keeps it dead.
	bash scripts/test_gate_runners_refuse_on_a_stale_binary.sh   # ~8s MEASURED (7.7s), needs the built binary (this target already depends on scrip): EVERY suite runner refuses rc=2 on a stale binary -- the ladder body (all seven languages), the Python harness, and all 20 $$SCRIP-grading test_*_suite.sh runners, censused with a printed denominator so a new grader added without the preflight reds HERE rather than on its first false board. ⭐ WIRED 2026-09-04 (hq_T, row harness-and-ladder-runner-refuse-on-a-stale-binary-like-the-artifact-regen-does, minted by ceo after TWO false-red audits in one day: a fetched-not-merged clone at 12:23, and a 10:57 binary at 14:03 that read RED and then GREEN after nothing but an incremental make). Only test_corpus_snobol4.sh refused on this before; every other runner graded whatever binary was in the tree and stamped the verdict with a git SHA that was not evidence about it. ⛔ THE ARM THAT MATTERS MOST IS THE ONE-COPY INVARIANT (arm 10): the staleness rule is gate_require_fresh in lib_gate.sh and NOTHING ELSE -- the Python harness reaches it through util_require_fresh.sh, a shim with zero logic, because the identical rule already existed twice in bash and a cure to one copy (3d12ca54) never reached the other, so the same defect was cured a second time nine days later (4c7253e99). A third copy, in a second language, is how the class survives its own cure a third time. Also cured while wiring: four skip-as-success arms (exit 0 from "SKIP ... not built") in gc_stress, snobol4_pat_rung (x2) and prolog_rung (x2), and a cwd-relative ./scrip default in test_swi_suite.sh; arm 11 keeps them dead.
	bash scripts/test_gate_modes_declaration_travels.sh   # ~8s MEASURED (7.1/8.2s, 15 arms across all 7 languages, 6 of them declaring ast entries; needs ./scrip for its one grading arm): THE `modes` DECLARATION TRAVELS WITH THE SUITE. ⭐ WIRED 2026-09-05 (hq_T, umbrella row test-suite-consistency-seven-languages-one-standard; the cross-language census hq_B asked for after util_census_optimizer_bypass.py was caught grading 28 modes=ast entries by EXECUTION). An `ast` entry's .ref is a --dump-ast dump; execute it and the red means nothing. The harness refuses such a run -- but its evidence was a SIBLING ALL.csv, so the guard's activation depended on WHERE THE CALLER PUT THE SUITE. Every runner grading an EXTRACTED family in a tempdir had no csv beside it, and a guard that cannot see its subject says nothing, which is indistinguishable from a pass. MEASURED BOTH WAYS ON ONE PAIR OF COMMANDS (pascal's 5 modes=ast entries): in place rc=2 REFUSING; extracted, rc=1 with a full plausible board, total=5 m3_fail=5 m4_fail=5 -- and all five PASS when graded by the instrument they declare. Blocking from the start because it is green on arrival; fail-once proven across all seven languages (8 violations on the pre-cure harness).
	bash scripts/test_gate_score_column_semantics.sh   # ~0.1s, pure file read (no build): every runner a SCORE.md grid cell cites must be of that COLUMN'S kind — M master · L ladder · V vendor. ⭐ WIRED 2026-09-03 (hq_T, ceo ruling after `reb 100%` went out on the live board): a "|"-split off-by-one put the Rebus LADDER census in the MASTER column, and the readability guard beside it could not object because "22/22" is a well-formed population in a cell allowed to contain one. READABLE AND WRONG is the one shape a parse check can never catch — it is precisely what passing the parse means. Blocking from the start rather than REPORTED, because it is green today: the ramp exists for gates that are already red, not for ones that cost nothing to keep green.
	-bash scripts/test_gate_score_tables_agree.sh   # ~0.1s, pure file read: SCORE.md's September-10 grid and its standardized display must agree BY VALUE. ⭐ WIRED 2026-09-03 (hq_T, ceo approval): `util_score_row.py write` updates only the display, so a grid cell goes stale the moment a runner records a row -- measured four times in one sitting, each found only by a hand edit. ⛔ VALUES, NEVER DATES: same-day staleness (today's date beside a superseded number) is invisible to any freshness check. ⛔ REPORTED, NOT BLOCKING, because it is RED TODAY (2 same-denominator conflicts in Icon, 21 one-sided populations): the ramp exists for gates that are already red, where blocking would stop the very landings that clear them -- the same call made for the xfail census, and the opposite of the column-semantics gate above, which was green on arrival and so went blocking at once.
	bash scripts/test_gate_no_o2_arm_in_scripts.sh   # ~0.05s MEASURED (0.04/0.05/0.04 over three runs, 620 scripts), pure source census (no build): the s262 FACT RULE (NO -O2 BUILDS, EVER) enforced where it is EXECUTED rather than where it is written down. ⭐ WIRED 2026-09-03 (hq_B, row jcon-selfhost-build-carries-a-live-o2-runtime-arm-contradicting-the-s262-fact-rule): test_gate_digest_matches_rules.sh polices the per-root CLAUDE.md DIGESTS, so it answers "does the digest say the right thing", which was read as "is the rule obeyed" -- and TWO live -O2 runtime arms sat in scripts/ for weeks while every digest read clean (jcon_selfhost_build.sh's PERF=1, routed by hq_P; and build_o2_working_snobol4.sh, which nobody had reported and whose whole purpose was `make libscrip_rt RT_OPT="$$O2"`). Both deleted in that landing. ⛔ SCOPED TO RT_OPT/RTOPT ON PURPOSE, not to any -O2 in scripts/: 26 scripts carry a legitimate one (gcc -O2 for tools/bench_rusage, the measurement harness -- building the stopwatch at -O0 adds its own cost to every benchmark number; and `fpc -O2`, the rival at its released default, which IS the fair-comparison basis). A gate broader than its rule gets disabled by the first person it blocks for a good reason.
	bash scripts/test_gate_cross_suite_ref_agreement.sh   # ~0.16s MEASURED (0.13/0.17/0.18 over three runs, 70 byte-identical program pairs across 9 language dirs), pure file parse, no build. ⭐ WIRED 2026-09-03 (hq_B, ceo CEO-177 ruling after the ICN4 FALSE GREEN): two suites held CONTRADICTORY REFS for a BYTE-IDENTICAL program -- ALL.ref said Icon's x[-3+:6] fails, rung36_all.ref said it yields the section, icont agreed with the second. So the master board's two reds were CORRECT OUTPUT FAILING A BAD REF, and the cure changed the COMPILER to match the bad ref: a real regression plus a false green at 380/381 that five green arms AND a ceo audit all missed, because not one of them asked the ORACLE. ⛔ This gate cannot say WHICH ref is right -- only an oracle run can -- but proving the disagreement EXISTS is the cheap half, and it was available for free the whole time. A ref is evidence about a PAST oracle run, not about the oracle.
	bash scripts/test_gate_same_suite_ref_agreement.sh   # ~0.2s MEASURED, pure file parse via corpus_suite_harness's own reader, no build. ⭐ WIRED 2026-09-04 (seat15, task same-suite-contradictory-refs-gate-two-entries-one-program-two-answers, hq_B's find routed by ceo): the SAME-SUITE twin of the gate above -- two entries can share one master (never linked via the cross-suite gate's ALL.csv `origin` mechanism) and still hold contradictory refs for a byte-identical program. Found live wiring this in: Icon entries 318/719 (`procedure_every_to_17`/`_48`) disagreed `3.0/2.0/1.0` vs `3/2/1` -- oracle-verified against real `icont`/`iconx` (`3/2/1` is right; the ref fix EXPOSES a genuine SCRIP defect, `every write(N.0 to M.0 by S.0)` printing a spurious `.0` real Icon does not -- FINDING-2026-09-04-seat15-icon-float-to-by-generator-prints-trailing-decimal-oracle-does-not.md, flagged not fixed, src/ codegen is hq_B's Icon lane not this row's) -- plus a second, unrelated stale ref (`procedure_scan_write_5` missing a legitimate second blank-line write). Both cured in ALL.ref before this gate went blocking; wiring it in RED on day one would have punished the wrong thing. Same "cannot say which side is right, only that a disagreement exists" limit as its sibling -- run the ORACLE, never the compiler, to settle one.
	bash scripts/test_gate_xfail_marker_and_index_agree.sh   # ~0.6s MEASURED, pure file parse, no build: THE OTHER HALF OF THE PROMOTION CLASS. An XFAIL lives in three places -- the banner in the master, ALL.csv's xfail column, and the reason in ALL.xfail -- and a promotion rewrites the master and leaves the INDEX at 1. Measured at corpus 5df255b01: 46 banners against 49 index rows, with every single-file gate green because each compared a file with itself. Its sibling below catches the ORDER half of the same commit and is blind to this one. ARM 3 grades the real 5df255b01 tree, so the gate must red on the defect that motivated it.
	bash scripts/test_gate_master_order_is_the_builders_order.sh   # ~0.9s MEASURED, pure file parse via the builder's own master_file_key import (no build): THE COMMITTED MASTER IS SORTED THE WAY ITS BUILDER SORTS, for every language carrying an ALL.csv. ⭐ WIRED 2026-09-05 (seat06/hq_T, row three-master-builder-gates-are-red-at-head-and-none-is-in-make-test): pascal/prolog/raku/snobol4 had drifted from the level-ordering law (ceo 2026-09-03, "rank <= N must select the greenest N") since their last promotion and were resorted (util_build_master_suite.py --resort) to cure it, content-invariant. SNOBOL4 additionally needed a new master_file_key -- kind-partitioned, lines before blocks -- because plain master_sort_key names an order write_suite() cannot physically store for a mixed format-A/format-B master (a block ends only at the next banner or EOF, so a trailing one-liner is silently swallowed into it); every single-kind master is byte-identical under either key.
	bash scripts/test_gate_master_builder_reindex_only.sh   # ~2.4s MEASURED, hermetic (every arm runs against a scratch copy of corpus, never the real tree): util_build_master_suite.py --reindex reproduces the committed prolog ALL.csv byte-identically, ACTUALLY RECOMPUTES (not copies) a deliberately corrupted derived cell, touches ALL.csv only, preserves CRLF, and refuses rc=2 on an unacknowledged loose pair or an absorption flag. ⭐ WIRED 2026-09-05 (seat06/hq_T, same row as above): was red only because the prolog master it grades against was the same stale-order data the sibling gate above cures -- green since that resort, including the recompute arm, which a stale baseline had made look like a copy-not-recompute bug.
	bash scripts/test_gate_master_suite_builder_contract.sh   # ~1.5s MEASURED, hermetic (runs in a scratch tree, never opens the real master for writing): util_build_master_suite.py honours the PENDING.md/KEEP.md deferral contract and scopes absorption EXACTLY via --absorb-only. ⭐ WIRED 2026-09-05 (seat06/hq_T, same row): arm D's population (one loose absorbable icon pair) had gone empty a THIRD time from ordinary corpus absorption -- the identical false-red shape this gate's own header already names happening twice before -- so a synthetic pair is now planted fresh into the scratch tree on every reset, immune to how much of the real corpus has been absorbed by the time this runs. A genuine `added`-referenced-before-set bug under `set -u` (arm E read it unconditionally, but arm D's else-branch is the only place that sets it) is cured alongside.
	bash scripts/test_gate_no_xfail_survives.sh   # ~0.4s, pure census (no build): THERE IS NO SUCH THING AS XFAIL (Lon 2026-09-03 21:30, in-chat to ceo: "there is no such thing now as XFAIL. We are shooting for 100%. So if an XFAIL is a faulty test than lets fix all those tests."). ⭐ WIRED 2026-09-03 (hq_T, row test-gate-no-xfail-survives-...): ONE gate for all seven languages, not seven copies, because the suites spell an xfail THREE different ways -- ALL.csv `xfail` column, ALL.xfail lines, and *.xfail marker files -- and snobol4 uses all three (68+136+1) while icon uses two (6+27) and raku only the first (156). A single-source census reads pascal and rebus 0 CORRECTLY and icon 6-instead-of-43 WRONGLY, and both look equally like an answer. ⛔ REPORTED, NOT BLOCKING, and that is a ramp rather than a softening: the gate genuinely returns rc=1 while any xfail survives (XFAIL_GATE_BLOCKING=1 proves it), but wiring it blocking TODAY would red the landing verdict of every seat -- including the landings that reduce the count -- so the gate would prevent the work it exists to cause. Same REPORTED-then-BLOCKING ramp board_packages.sh took; the flip is a date decision for ceo/Lon, not a script author's. It REFUSES rc=2 on an unreachable suite, because a census that cannot see its population must never print 0.
	bash scripts/test_gate_icn_scan_argtype.sh   # ~2.5s MEASURED (2.69/2.43 over two runs, 11 witnesses x 2 modes), and it needs the build the arms above do not: the Icon scan-node builtin argument-type class, re-cut from the icont/iconx oracle EVERY run rather than from a stored .ref -- a .ref only proves "unchanged since someone cut it", which cannot tell a cured builtin from a .ref cut while it was broken. Placed ahead of the ~28s SNOBOL4 board so this class costs 2.5s to falsify, per the cheap-arms-first design of this recipe. Negative-tested by injection (deleting bb_scan_any's check makes it exit 1 naming that one builtin, other 10 green).
	bash scripts/test_gate_kw_integer_hex_refused.sh   # ~2.5s MEASURED (2.46/2.53 over two runs, 26 arms x 2 modes), needs the build and the correctness oracle: an integer-required keyword assigned a STRING must refuse hex, infinity and NaN with ERROR 208, and must still coerce a real. ⭐ WIRED 2026-09-05 IN THE COMMIT THAT CURED IT (hq_P, row snobol4-integer-keyword-accepts-hex-string-oracle-raises-error-208), never before -- a red in this blocking set stops EVERY seat's landing, not just the owner's. ⛔ THE DEFECT CLASS IS 'A WRONG NUMBER, RETURNED WITH rc=0': &ANCHOR = "0x10" stored 16 and printed plausible output where SPITBOL raises ERROR 208, so no board could ever see it -- there was no hex-string keyword witness in the master, which is exactly why it survived on a green board. ⛔ THE ARMS ARE WIDER THAN THE ROW'S OWN BRIEF, MEASURED AT CURE TIME: the brief said the defect was ONLY the unsigned 0x/0X form and that "1e400" was 'correctly refused by both'. It was not -- "inf", "infinity", "nan", "nan(chars)" and the 1e400 overflow were all refused with ERROR 210 (negative or too large) where the oracle raises 208, the same wrong-reason refusal already documented for "-0x2": strtod parses the value and an unrelated RANGE guard catches it downstream. A gate carrying only the hex arms would have gone green on a narrow cure and left the rest of the class live underneath. ⭐ BOTH CONTROL ARMS ARE LOAD-BEARING AND BOTH WERE NEGATIVE-TESTED BY INJECTION, not merely asserted: the coercion arm ("3.7"->3, "1.5e3"->1500) fails if someone 'fixes' this by reverting 0fa9c4cb4, whose intent is CORRECT; the 210 arm ("1e30", "99999999", "16777217") fails if the cure OVER-rejects, turning a finite-but-too-large number into 'not integer' -- which would trade one wrong error code for another and still match no oracle.
	bash scripts/test_gate_ref_cutters_refuse_a_dead_oracle.sh   # ~58.48s MEASURED (58.48/71.02 over two runs, 14 arms), needs the build only for the bench cutter arm: EVERY path that mints a .ref from a live oracle reads the oracle's STATUS and refuses when it died mid-cut. ⭐ WIRED 2026-09-04 (hq_T, row every-ref-cutting-path-refuses-when-the-oracle-dies-mid-cut, ceo on seat07's rung04 finding that `sbl -bf` SIGSEGVs on about half its ERROR 212 runs WHILE PRINTING the diagnostic -- so a ref cut on an unlucky run is silently TRUNCATED, which is the worst artifact this project can make: a plausible pin that grades every future run against a lie, indistinguishable downstream from a good one). ⛔ THE PROBE PRINTS OUTPUT AND *THEN* SEGFAULTS, and that is the whole difficulty -- an oracle that dies silently is caught by any "did it produce bytes" check, so a gate built on one would pass on a cutter with no status check at all. ⛔⛔ WRITING THIS GATE FOUND THE DEFECT A SOURCE-LEVEL CENSUS HAD DECLARED ABSENT hours earlier: run_oracle() returned kind="RAN" for a signal-killed child (the signal is a NEGATIVE returncode, which "RAN" does not consider), and cmd_capture_oracle_refs -- the one command here that mints ground truth from a live oracle -- guards on `kind != "RAN"` alone. Cured in the shared reader so both callers inherit it, rather than at the call site, since util_build_package_suite.py was safe ONLY by re-deriving the same rule itself. ⭐ ARM 7 is the control and is not optional: with a HEALTHY synthetic oracle the same paths must still mint, or every arm above would pass on a cutter that refuses everything. ARM 6 censuses with a PRINTED DENOMINATOR so the SIXTH cutter, written by someone who never read this finding, reds HERE rather than by pinning a truncated ref nobody notices for weeks -- and it strips comments first, because its first run named a script as an unguarded cutter on the strength of four header lines describing ref-diffing.
	bash scripts/test_gate_sno_setexit_resume_matches_oracle.sh   # ~1.5s MEASURED, needs the build: the SETEXIT/ERRLIMIT RESUME mechanism -- :(CONTINUE) resumes at the interrupted statement's own FAILURE EXIT, the trap is one-shot unless re-armed, a handler that falls off its end continues in source order, and the END trap honours &ERRLIMIT. ⭐ WIRED 2026-09-04 IN THE COMMIT THAT CURED IT (hq_P, row setexit-not-invoked-under-errlimit-survival), following the kw_integer_hex precedent above. ⛔ EVERY FACE IS A CONTRACT THE TWO ORACLES AGREE ON -- each was measured on SPITBOL (x64/bin/sbl -bf) AND on CSNOBOL4, and the faces where they DISAGREE are deliberately absent: the error NUMBERING (SPITBOL 14/22/37 vs CSNOBOL4 2/5/35 for the same errors -- SCRIP carries CSNOBOL4's table, so a gate pinning SPITBOL numbers would red a correct tree) and the CSNOBOL4-only trap-fires-on-normal-END extension. A gate that pins a dialect-divergent face grades the oracle, not the compiler. ⛔ NON-VACUOUS BY CONSTRUCTION: it REFUSES rc=2 unless the killswitch SCRIP_SETEXIT=0 moves all 4 resume faces, so a cure that quietly stops discriminating reds HERE instead of passing forever -- the vacuous-test class Lon flagged across ~14 rung harnesses.
	bash scripts/test_corpus_snobol4.sh
	bash scripts/test_gate_emit_no_lang.sh
	bash scripts/test_gate_template_medium_invisible.sh
	bash scripts/test_gate_corpus_coverage_classified.sh
	bash scripts/test_gate_pl_quad_regs.sh   # ~45s (166 Prolog compiles, most refusing during the rebuild): THE QUAD GATE -- no r12-r15 write reachable from a Prolog graph outside the TR/B/ROOT/BALL enrolment, mode-4 TEXT + reachable rtx (hq_P 2026-09-02, ARCH-PROLOG-BYRD-BOX-TRANSLATION.md § E instrument lane). Last in the recipe so every cheaper gate reports first.
	bash scripts/test_gate_sno_runtime_define.sh   # ~15s: runtime DEFINE (the gimpel DEXP idiom, a runtime prototype over a compiled body, a CODE() body) matches SPITBOL in both modes
	bash scripts/test_gate_sno_deferred_replacement_keeps_its_capture_mark.sh   # ~15s: a replacement over a pattern-valued variable with a capture keeps its deferred-capture mark (both modes vs SPITBOL)
	bash scripts/test_gate_sno_io_assoc_channel_and_filespec.sh   # ~1.3s MEASURED, needs the build: INPUT/OUTPUT's association form must reject a non-integer channel and a space-containing filespec like SPITBOL (ERROR 116/160), not silently create a stray file or accept a null channel. ⭐ WIRED 2026-09-05 IN THE COMMIT THAT CURED IT (hq_P/seat09, row snobol4-output-third-argument-is-a-format-not-a-file-name). ⛔ DIALECT-GATED, NOT AN OUTRIGHT BAN: SCRIP's old acceptance matched CSNOBOL4 exactly, so the gate proves BOTH directions on the live oracle -- SPITBOL default raises the error, `--compat=csnobol4` still accepts -- plus regression faces for the corpus's 8 real bracketed-options-only sites this cure must never touch.
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
