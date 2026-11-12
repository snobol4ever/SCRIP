# Makefile — SCRIP unified build
#
# Primary targets:
#   make scrip        — build the unified scrip x86 executable
#   make all          — alias for scrip
#   make setup        — install system packages + CSNOBOL4 + SPITBOL oracle
#   make test         — THE blocking set, LOOP AND REPORT (R-582): every arm runs, reds and refusals print
#                       as separate classes, the summary sums to the arm count, non-zero on any blocking red
#                       or refusal. The arms are DECLARED as the recipe lines of test-sequential below.
#   make test-sequential — the same arms the old way, one shell per line, make stopping at the first red.
#                       The declaration `make test` reads, kept runnable: use it to BISECT, never to land.
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
# THIS lock is bounded by ../harness/scripts/lib_pristine_lock.sh (build-governor-holds-pristine-while-box-idles) --
# see the `pristine` target's own comment for why.
BUILD_LOCK ?= /tmp/si_pristine_lock$(subst /,-,$(ROOT))
CC_SLOT := $(if $(wildcard $(ROOT)/../harness/scripts/util_cc_slot.sh),bash $(abspath $(ROOT)/../harness/scripts/util_cc_slot.sh) ,)
CC      := $(CC_SLOT)gcc
CXX     := $(CC_SLOT)g++
# ⛔⭐ ONE COMPILE CACHE FOR THE WHOLE FLEET (R-1879; Lon 2026-10-21 18:5x CDT, in-chat, verbatim: "Why is the load so high? Why do recompiles re-compile everything all the
# time?"; measured R-1876: sixteen trees compiled every commit separately, no cache on the box). ccache keys on the preprocessed source and the flags, so a hit is the same compile and
# s150's per-tree objdir guard still holds. It enters through its wrapper directory on PATH, so every printed command still reads gcc/g++ (test_gate_no_implicit_declarations greps them).
# CCACHE_BASEDIR, the parent of this SCRIP where its four repos are siblings, rewrites each tree's absolute paths relative to its own SCRIP, and DBGMAP maps the tree to "." in the debug info, so one tree's hit is another's and gdb reads the
# tree it runs in. S4E_NO_CCACHE=1 on the command line or in the environment builds without it (the control arm). Since 2026-11-11 the base is the tree root, not /home or / (the coo, measured on a scratch pair at different depths: 0 of 2 hits with base /home and with base /, which also rewrites /usr/include relative to the tree, 1 of 2 with each tree its own base), so every root and every worktree shares one cache; test_gate_ccache_shares_every_tree.sh holds it.
ifeq ($(S4E_NO_CCACHE)$(wildcard /usr/lib/ccache/gcc),/usr/lib/ccache/gcc)
export PATH := /usr/lib/ccache:$(PATH)
export CCACHE_BASEDIR := $(abspath $(ROOT)/..)
export CCACHE_NOHASHDIR := true
# ⛔⭐ A CACHE HIT TAKES NO COMPILE SLOT (R-2085). MEASURED 2026-10-31 11:08-14:33 CDT by progress/SLOT_WAITS.tsv: 9,728 compiles waited
# for one of the 8 slots, about 17 s each, 45 compiler-hours in 3.4 h, while ccache answered 81 percent of calls from the cache: the slot wrapper
# sat IN FRONT of ccache, so a hit that costs milliseconds queued behind real compiles. ccache's prefix_command runs only when ccache itself
# invokes the compiler (a miss, a link, an uncacheable call), so the slot moves there: proved on one object with the only slot held, a hit
# through the prefix returns in 0.03 s, the same hit through the old wrapper blocks until its 20 s timeout, and a link still waits for the slot.
ifneq ($(CC_SLOT),)
export CCACHE_PREFIX := $(abspath $(ROOT)/../harness/scripts/util_cc_slot.sh)
CC      := gcc
CXX     := g++
endif
endif
DBGMAP  := -fdebug-prefix-map=$(ROOT)=.
WARN    := -w
RT_OPT  ?= -O0 -g -fno-strict-aliasing -fwrapv -fno-omit-frame-pointer  # ⛔⭐ NO -O2 BUILDS, EVER (Lon 2026-08-23 s262 FACT RULE, SUPERSEDES O0-DEV-O2-BENCH/s179 and O2-ALWAYS/s178): -O0 for development AND benchmarks AND demos. Never pass RT_OPT="-O2 ...", never build an -O2 RT_TAG, never quote an -O2 number as current state. Two reasons, the second stronger: (1) an -O2 template-touching rebuild is ~9m30 vs ~1m40, paid on every arm of a measure-and-cure loop; (2) it measures a compiler we are DELETING — the RT is moving to register-aware ASM (src/runtime/rtx/*.s, GOAL-RTCC.md), so an -O2 figure grades gcc's optimizer over code that will not exist. The LABELING duty survives: every perf number still names its RT_OPT, and it now reads -O0. Authority: .github/RULES.md § NO -O2 BUILDS.
# ⛔ THE FAN-OUT WIDTH OF `make test` READS THE LOAD FIRST (R-1333, Lon's ECONOMY word, 2026-09-27 17:2x: cap concurrent children at
# max(2, min(4, cores minus load1)); at load 34 that is two, never six). TEST_SHARDS=N on the command line still overrides; TEST_SHARDS=1 is the
# sequential shape. The width is read from ../harness/scripts/lib_fanout.sh (fanout_width), the one instrument of the ceiling, and run_blocking_set.sh
# clamps a requested count above it as well. Was `TEST_SHARDS ?= 6` (the 2026-09-20 measurement: the wall is the declared-serial phase plus the slowest shard).
TEST_SHARDS ?= $(shell bash ../harness/scripts/lib_fanout.sh width)
# ⛔⭐⭐⭐⭐ THE DEFAULT ARENA IS 128 KB AND IT IS COMPILED IN, NOT EXPORTED (Lon 2026-09-21 17:3x, in-chat: "So let's take the default GC arena down to 128 KB."; R-1095). GC_HEAP_KB in gc_heap.c is the default, so it travels with the BINARY and needs no make variable -- which also retires the blanket `export SCRIP_HEAP_MB ?= 1` this line used to carry, the one test_gate_gc_the_tiny_arena_is_the_default_of_gc_testing.sh arm 1 refuses by name because it changed what every existing gate measured and masked standing reds. MEASURED on two allocating witnesses: 128 KB is the PEAK of collector exasperation, not the floor -- icon concat 1613 collections at 1 MB, 7533 at 256 KB, 21362 at 128 KB, then DOWN to 5429 at 64 KB because the live set no longer fits and the window GROWS. Below the peak, smaller is LESS exasperating.
SCRIP_HEAP_KB_TINY ?= 128  # kept as the knob `make arena` raises; the old note follows. THE TINY ARENA IS THE EXERCISER OF ALL GC TESTING, AND IT WAS ITS OWN PASS UNTIL R-1075 (`make test-arena`), NOT THE BLOCKING SET'S ENVIRONMENT (Lon 2026-09-19 16:1x, in-chat, verbatim: "Actually, for all GC testing all seats should use a tiny arena to exasperate all the problems all the time."; R-931/934/938/939, RULES.md § THE INSTRUMENT LAWS, TWENTY-EIGHTH BATCH CLAUSE 2). SCRIP_HEAP_MB is the COMMITTED WINDOW in MB (rt_gcheap_init); at 1 MB a program collects wherever it allocates a megabyte -- 1614 collections against SIX at the shipped 512 on corpus/benchmarks/icon/bench_icnstr_concat_table.icn, stdout identical -- and the reserve is the larger of 8x the window and the shipped default's, so a tiny window REFUSES NO LIVE SET (the cap used to be 8x the window and manufactured three phantom SIGABRTs on the SNOBOL4 rungs; cured at SCRIP e6aab28c7). ⛔ MEASURED AND REVERSED THE SAME SITTING (R-939): this was first landed as a blanket `export SCRIP_HEAP_MB ?= 1` here, so every arm of `make test` inherited it. The full 360-arm set then read 326 green / 32 red / 2 refused, and the classification says why that is the wrong shape -- gates that grade the SHIPPED configuration's behaviour flipped red for an arena nobody ships (gc2's Arizona witness and the co-expression frame image are green at 512 and red at 1 MB), while other reds were standing reds the export got blamed for. A landing gate must grade what we ship; the tiny arena is the SECOND pass whose every red is a row. Same logic as CLAUSE 2's published-row refinement.
DEPFLAGS := -MMD -MP
CBASE   := -O0 -g $(DBGMAP) $(WARN) $(DEPFLAGS) -I$(SRC)
ZCFLAGS ?=
CXXRT   := -O0 -g $(DBGMAP) $(WARN) $(DEPFLAGS) -std=c++17 -finput-charset=UTF-8 -I$(SRC) $(ZCFLAGS)
CRT     := $(CBASE) $(ZCFLAGS)
LIBS    := -lm -lpthread

# Runner defaults
SNO          ?= $(error SNO is required — e.g. make run SNO=prog.sno)
INC          ?= $(CORPUS)/inc
JVM_CACHE    := /tmp/scrip_jvm_cache
NET_CACHE    := /tmp/scrip_net_cache
JASMIN       := $(ROOT)/miscellaneous/jasmin.jar
SCRIP_CC_BIN := $(ROOT)/scrip

.PHONY: preflight all build-all prelude-prolog scrip setup hooks pristine pristine-all buildinfo arena test-arena parsers-sc FORCE \
        test test-sequential test-boards \
        native codegen-emit-test \
        monitor-ipc \
        libscrip_rt libscrip_rt_static \
        run run-ir run-jvm run-net \
        clean distclean

# ── Primary target ────────────────────────────────────────────────────────────

# ⛔⭐ ONE BUILD PER TREE (the cto, ARCH-SHARED-DEVELOPMENT.md section 3.2, R-2041 step 6; Lon 2026-10-31 15:1x CDT, verbatim: "Everyone is compiling at the same time."): `make`
# first asks the build cache (/home/resources/builds/scrip/<key>, key = sha256 of HEAD^{tree}, RT_TAG, the toolchain) and on a hit with a CLEAN tree copies the artifacts in, prints
# FETCHED <key> and compiles nothing; on a miss or a dirty tree it builds as before and, when the tree is clean, publishes the entry for every other root. MEASURED before this: 19
# roots rebuilt every landing (3,687 compiles waited for the 8 slots in one hour on 2026-10-31). `make scrip` and the other targets stay the raw build; S4E_NO_BUILD_CACHE=1 is the control arm.
BUILD_CACHE := $(if $(wildcard $(ROOT)/../harness/scripts/util_build_cache.sh),bash $(abspath $(ROOT)/../harness/scripts/util_build_cache.sh),)
# ⛔⭐ ONE BOX-WIDE COMPILE BUDGET (DESIGN-PLAN box-scheduler step 2, the cto CTO-496; Lon 2026-11-04, verbatim: "There has been talk, and only talk, about using industry
# recognized ways of doing make and build load balancing and management."): `make` (any -j) runs build-all through harness/scripts/util_jobserver.sh make, which joins the
# build to the one GNU make jobserver every root shares (the pipe /home/resources/progress/jobserver.fifo held by the unit s4e-jobserver, N tokens declared in
# /home/resources/progress/JOBSERVER.tsv), so nineteen roots never run more than N+makes compiles at once, by mechanism; no holder, and it is the plain $(MAKE) as before.
JS_MAKE := $(if $(wildcard $(ROOT)/../harness/scripts/util_jobserver.sh),bash $(abspath $(ROOT)/../harness/scripts/util_jobserver.sh) make,$(MAKE))
all:
ifneq ($(BUILD_CACHE),)
	@if $(BUILD_CACHE) fetch --root $(ROOT) --tag $(RT_TAG); then :; else $(JS_MAKE) --no-print-directory build-all && $(BUILD_CACHE) publish --root $(ROOT) --tag $(RT_TAG); fi
else
	@$(JS_MAKE) --no-print-directory build-all
endif
build-all: scrip out/scrip-ipp
prelude-prolog: out/libscrip_prelude_prolog.so

pristine:  # A full rebuild, SCOPED TO THIS CONFIGURATION. ⛔ NO LONGER the per-landing gate law -- HQ-27 PRISTINE-BEFORE-VERDICT was loosened by Lon 2026-09-03 (RULES.md FACT RULE). This target is for the officer's audits, release points, and whenever the stale-binary refusal fires; an ordinary landing grades on incremental `make`.
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
	# ⭐ THE WAIT IS BOUNDED, NOT SILENT (../harness/scripts/lib_pristine_lock.sh, S4E_PRISTINE_LOCK_TIMEOUT, default
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
		bash ../harness/scripts/lib_pristine_lock.sh $(BUILD_LOCK) -- sh -c '\
			rm -rf $(OBJ) $(RT_OBJDIR) $(RT_SO) $(ROOT)/out/libscrip_rt.so $(RT_A) $(ROOT)/out/libscrip_rt.a $(ROOT)/scrip && \
			$(MAKE) all \
		'; \
	fi


SHELL := /bin/bash
preflight:  # ⭐ THE CHEAP HERMETIC ARMS, WITH NO BUILD (hq_T's ask 2026-09-06, measured by them: the Makefile had all/pristine/test/test-postoffice/buildinfo/scrip/clean and NO cheap target of any kind, so EVERY sub-second hermetic arm was trapped behind a full build -- a STRUCTURAL disincentive, not anyone's discipline). ⛔ THE MISSING `scrip` PREREQUISITE IS THE ENTIRE POINT OF THIS TARGET AND MUST NOT BE ADDED. Population DECLARED in ../harness/scripts/preflight_arms.txt because a static grep cannot classify it in either direction (it convicts strip_comments.py, which says "make test" in its prose, and it clears test_gate_pl_quad_regs.sh, which reaches the compiler through a lib_ helper without naming it); ../harness/scripts/test_gate_preflight_arms_stay_cheap.sh PINS EVERY ARM BY MEASURED DURATION so this cannot quietly acquire a 45-second arm and stop being worth running.
	@: R-1893 Lon 2026-10-22 the R-1671 coo-only refusal is released, every seat may run preflight; \
	bad=0; n=0; \
	while read -r arm; do \
	  n=$$((n + 1)); \
	  case "$$arm" in *.py*) out=$$(cd "$(ROOT)" && python3 $$arm </dev/null 2>&1) ;; *) out=$$(cd "$(ROOT)" && bash $$arm </dev/null 2>&1) ;; esac; \
	  rc=$$?; \
	  if [ $$rc -ne 0 ]; then bad=$$((bad + 1)); printf '  RED  rc=%-3s %s\n' "$$rc" "$${arm%% *}"; printf '%s\n' "$$out" | tail -3 | sed 's/^/       /'; fi; \
	done < <(grep -E '^../harness/scripts/' "$(ROOT)/../harness/scripts/preflight_arms.txt"); \
	echo "preflight: $$n arm(s), $$bad red -- NO BUILD, hermetic. NOT a substitute for the test target: it cannot see a codegen defect."; \
	[ $$bad -eq 0 ]
fleet-smoke:  # ⭐ TIER 1 OF THE VERIFIER LOOP (Lon 2026-10-11 12:4x CDT: "The COO should run a set of super-fast test, a so-called smoke test, often."; R-1673): the eight per-language smokes, about 8-20 s in all, the coo's to run after every origin landing; every other seat is refused rc 2 (R-1671). No build: a stale ./scrip is the freshness guard's refusal, never a verdict.
	@bash "$(ROOT)/../harness/scripts/test_fleet_smoke.sh"

arena: export SCRIP_HEAP_KB := $(SCRIP_HEAP_KB_TINY)
arena:  # ⭐ WHAT ARENA WILL THE TINY PASS USE? print it rather than assume it (LAW 0), and it is the one place test-arena's arena is decided.
	@printf 'ARENA SCRIP_HEAP_KB=%s  (the committed window for the tiny-arena pass; raise it with SCRIP_HEAP_KB_TINY=N. The reserve is the larger of 8x this and the shipped default, so a tiny window refuses no live set)\n' "$$SCRIP_HEAP_KB"

test-arena: export SCRIP_HEAP_KB := $(SCRIP_HEAP_KB_TINY)
test-arena: scrip parsers-sc arena  # ⛔⭐ LON'S TINY-ARENA PASS, MANDATORY PER COLLECTOR LANDING (Lon 2026-09-19; R-938/939; RULES.md batch 28 CLAUSE 2; gate test_gate_gc_the_tiny_arena_is_the_default_of_gc_testing.sh). Every arm below runs with a 1 MB committed window, which collects wherever the program allocates a megabyte -- the exasperation Lon asked for, and on its first afternoon it found nine SNOBOL4 programs and two Icon programs that the shipped arena passes. ⛔ THIS IS A SECOND PASS, NOT `make test`'s ENVIRONMENT: a landing gate grades the configuration we ship (R-939 measured the alternative -- the blanket export took the 360-arm set to 32 red, including gates that grade shipped behaviour correctly at 512), and every red HERE is a row, never a blocked fleet. ⛔ AND IT IS RED BY DESIGN TODAY: two arms below are the collector findings of 2026-09-19 (gc2's Arizona witness and the co-expression frame image, both green at 512 and red at 1 MB), each on a row of its own; a seat reads the NAMES, not the exit code, until those rows close.
	@bad=0; ref=0; n=0; \
	for l in snobol4 icon prolog pascal raku rebus snocone; do \
	  n=$$((n + 1)); \
	  if bash ../harness/scripts/test_smoke_$$l.sh >/dev/null 2>&1; then printf '  GREEN  smoke %s\n' "$$l"; else bad=$$((bad + 1)); printf '  RED    smoke %s\n' "$$l"; fi; \
	done; \
	printf 'test-arena: arms=%s red=%s refused=%s at SCRIP_HEAP_KB=%s -- every RED here is a collector finding and owes a ROW; a REFUSED arm COULD NOT MEASURE (a ceiling that fired, a missing prerequisite) and owes its REASON, never a collector row (coo 2026-09-20 on the cfo CFO-121/122: the mark-walk arm needs 651s at 1 MB against a 300s ceiling, and rc=124 was being tallied and read as a lost root). Both still fail this target -- unmeasured must not read green -- but a reader can now tell a clock from a collector (the arena is named on the line above; a suite board run under this pass names it too)\n' "$$n" "$$bad" "$$ref" "$$SCRIP_HEAP_KB"; \
	[ $$bad -eq 0 ] && [ $$ref -eq 0 ]

test-boards:  # ⭐ THE PACKAGE BOARDS, SPLIT OUT OF `make test` 2026-09-08 (cfo, ECONOMY, on R-433/431 in the archived ruling record): board_packages.sh runs every vendored package/third-party suite and prints the denominator (suites=/graded=/unproven=). It was the REPORTED, NOT BLOCKING arm of `make test` (MASTER-PLAN I21, D1 stage): its exit code is 0 whenever it measured, red constituent suites included, so it could never fail the build -- and it cost every seat ~10 minutes per blocking run. The coo's measure runs this target; any seat runs it on demand. D5 turns the measured floor into a real gate, and that gate goes back into `test` when it exists.
	$(MAKE) scrip
	bash ../harness/scripts/board_packages.sh

test: scrip parsers parsers-sc  # ⭐⛔ LOOP AND REPORT, RULED R-582 (2026-09-11) ON THE cfo'S FIRST FULL CENSUS OF THIS TARGET. It used to be ONE target of ~150 recipe lines, and make ABORTS ON THE FIRST FAILING LINE: when arm 15 went red the set died 24 seconds in and 131 ARMS NEVER EXECUTED, for all 13 seats, for as long as that arm stayed red. Not red, not green -- UNMEASURED, which our own denominator law calls the worst of the three. `make -k` does NOT rescue it (measured, 149s, same single red): -k continues across TARGETS, never across the lines of ONE recipe, so the set could only ever reveal ONE red per cure cycle and each cycle paid a build. The cfo's loop-and-continue census found EIGHT non-green where this target reported ONE -- five reds and THREE REFUSALS, one of which would have passed vacuously. WE REPORTED 1 OF 8. ⛔ NOTHING IS WEAKENED: a blocking red still exits non-zero (1), a blocking refusal exits 2, reds and refusals print as SEPARATE classes because rc=1 FAILED and rc=2 COULD-NOT-MEASURE are different facts, and the summary satisfies its own denominator (green+red+refused == arms, printed) so a run that dies partway can never again read as a clean one. ⭐ THE OBJECTION AND ITS ANSWER: loop-and-report lets a seat land while a red stands elsewhere -- but that was ALREADY TRUE and worse, because seats were landing with 131 arms unmeasured and no way to know they were. ⛔ THE ARMS ARE NOT HERE: they are the recipe lines of `test-sequential` below, which is the DECLARATION this driver reads and an executable twin of it, so `grep <gate-name> Makefile` still finds an arm and the two forms cannot diverge silently (test_gate_make_test_loops_and_reports.sh pins the agreement).
	@bash ../harness/scripts/run_blocking_set.sh $(if $(filter 1,$(TEST_SHARDS)),,--shards $(TEST_SHARDS))   # ⭐⛔ PARALLEL BY DEFAULT since 2026-09-20 (coo, Lon's TENET condition 2 through the officer). MEASURED ON ONE TREE, BOTH SHAPES, origin e58d97dfb, 372 arms: SEQUENTIAL wall=2239s at load 8.6->3.1 and FAN-OUT/6 wall=1007s at load 9.2->9.1, which is 2.22x -- and the arithmetic closes rather than being asserted, 3899 arm-seconds inside 1007s of wall is 3.87x concurrency divided by the 1.74x each arm slows down under six-way load. 370 OF 372 ARMS AGREE ARM FOR ARM, class counts identical in both shapes (355 green, 16 red, 1 refused) and the same exit code; the two that disagree are duration-pinned arms that disagree in OPPOSITE directions, one favouring each shape, both now declared serial or named there. raced=0 over 230 recorded arm windows here and independently raced=0 over 373 arms on the cfo's tree. TEST_SHARDS=1 restores the one-at-a-time shape for a set that has gone strange, and `make test-sequential` is still the abort-on-first-red form. ⛔ WHAT MAKES IT SAFE TO BELIEVE IS NOT THE SPEED: two arms that ran concurrently and had the same path move under both are BOTH rc=2 COULD-NOT-MEASURE, named with each other and with the path, and NEVER a red -- a collision reported as a red is an unattributable finding and ten seats cannot afford one. The arms that CANNOT be parallel are DECLARED in ../harness/scripts/blocking_set_serial_arms.txt with the reason each cannot, and the parent runs them one at a time before any fan-out. the parallel-agrees-with-serial gate (retired by gates-zero) compares the whole arm-to-rc map between the two shapes and measures both failure directions of the race rule in every run.

test-sequential: scrip parsers parsers-sc  # ⛔⭐ THE DECLARATION OF THE BLOCKING SET, AND THE LEGACY ABORT-ON-FIRST-RED FORM. `make test` LOOPS these lines through ../harness/scripts/run_blocking_set.sh and reports every one of them; this target still runs them the old way -- one shell per line, make stopping at the first non-green -- which is what you want when bisecting a set that has gone strange, and never what you want for a landing verdict. ⛔ EDIT THE ARMS HERE. One arm per line, a leading `-` means REPORTED (run and classified, cannot fail the run) exactly as it always meant to make, and the trailing `#` comment is the arm's measured provenance. ⭐ WAS THE FALSE-GREEN TRAP (cured hq_P s268): `test`, `test-ir` and `test-all` were named in .PHONY with NO RECIPE ANYWHERE, so each exited 0 having run NOTHING ("Nothing to be done for 'test'") while reading as a full green suite. `test-ir` and `test-all` are DELETED rather than wired — nothing behind them ever existed. This target now runs THE blocking set named in CLAUDE.md and fails loudly on the first red. ⛔ HQ-27 PRISTINE-BEFORE-VERDICT IS VOID as a per-landing requirement (Lon 2026-09-03 ~15:58, RULES.md FACT RULE: "It's time to loosen this pristine build that keeps preventing forward progress and causes 20 minute wait times."). A landing verdict runs on an INCREMENTAL make -- never on a stale binary, and the stale-binary refusal stays as the guard. `make pristine` survives for the officer's audits, release points, and whenever that refusal fires; the receipt names the build it graded on. This comment asserted the retired law for hours after it was retired, which is the failure mode the FACT RULE exists to prevent: law that lives in a second unversioned place cannot be retired in one edit.
	bash ../harness/scripts/test_smoke_snobol4.sh   # THE BLOCKING SET IS THE EIGHT SMOKES (DESIGN-PLAN gates-zero, CTO-637; Lon 2026-11-12 13:1x-13:2x CST, verbatim: "I'm thinking I do not want any STINKING GATES." / "take those almost 2000 scripts to ZERO."): what tells right from wrong is a program graded against its oracle, the container entries the cycle grades every lap and these smokes; a lander also runs test_feature_reps.sh <lang> <feature> for what its diff touches (skill gates-and-witnesses section 0)
	bash ../harness/scripts/test_smoke_icon.sh
	bash ../harness/scripts/test_smoke_pascal.sh
	bash ../harness/scripts/test_smoke_prolog.sh
	bash ../harness/scripts/test_smoke_raku.sh
	bash ../harness/scripts/test_smoke_rebus.sh
	bash ../harness/scripts/test_smoke_snocone.sh
	bash ../harness/scripts/test_smoke_scrip_all_modes.sh

pristine-all:  # wipe EVERY cached configuration, not just this one (the pre-s258 behaviour)
	# Same race, same cure, same bounded BUILD_LOCK as `pristine` above -- see its comment block for why
	# the wait is bounded, not silent (make-pristine-per-root-flock-second-builder-waits;
	# build-governor-holds-pristine-while-box-idles).
	@if [ -n "$${S4E_NO_BUILD_LOCK:-}" ]; then \
		echo "⚠ S4E_NO_BUILD_LOCK set -- running pristine-all UNLOCKED (measurement/control-arm only)"; \
		rm -rf $(OBJ) $(ROOT)/out $(ROOT)/scrip; \
		$(MAKE) all; \
	else \
		bash ../harness/scripts/lib_pristine_lock.sh $(BUILD_LOCK) -- sh -c '\
			rm -rf $(OBJ) $(ROOT)/out $(ROOT)/scrip && \
			$(MAKE) all \
		'; \
	fi

buildinfo:  # ⭐ what am I actually about to link? print it rather than assume it (LAW 0)
	@printf 'RT_OPT     : %s\n' '$(RT_OPT)'
	@printf 'ARENA      : shipped default %s KB committed window, READ OUT OF src/runtime/rt/gc_heap.c (#define GC_HEAP_KB) rather than repeated here -- it is COMPILED IN and travels with the binary (R-1095, Lon 2026-09-21); the tiny-arena pass (make test-arena) uses SCRIP_HEAP_KB_TINY=%s; this shell inherits SCRIP_HEAP_KB=%s SCRIP_HEAP_MB=%s (both empty means the shipped default)\n' "$$(grep -E '^#define GC_HEAP_KB[[:space:]]+[0-9]+' $(ROOT)/src/runtime/rt/gc_heap.c | awk '{print $$3}')" '$(SCRIP_HEAP_KB_TINY)' "$${SCRIP_HEAP_KB:-}" "$${SCRIP_HEAP_MB:-}"
	@printf '           : \342\233\224 AN MB PIN NOW RAISES THE ARENA. SCRIP_HEAP_MB=1 is 1024 KB = 8x the shipped default and collects 13x LESS -- 1613 collections against 21362, measured by the cfo 2026-09-21 on corpus/benchmarks/icon/bench_icnstr_concat_table.icn, one binary through the env switch. Before R-1095 the default WAS 1 MB and =1 merely applied it; today it overrides it upward, so a gate pinning SCRIP_HEAP_MB=1 for exasperation gets less of it than pinning nothing at all\n'
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
    $(RT)/rtx/rtx_gc.s \
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
    $(RT)/rt/rt_pl_engine.c \
    $(RT)/rt/rt_arena.c \
    $(RT)/rt/rt.c \
    $(RT)/rt/rt_sc_module.c \
    $(RT)/rt/gc_heap.c \
    $(RT)/rt/gc_alloc_lookup.c \
    $(RT)/rt/gc_slot_table.c \
    $(RT)/rt/gc_audit_b.c \
    $(RT)/rt/rt_protected.c \
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
    $(SRC)/runtime/g_runtime.c \
    $(SRC)/runtime/io_format.c \
    $(SRC)/runtime/arithmetic.c \
    $(SRC)/runtime/bignum.c \
    $(SRC)/runtime/pattern_match.c \
    $(SRC)/runtime/keywords.c \
    $(SRC)/runtime/string_ops.c \
    $(SRC)/runtime/name_binding.c \
    $(SRC)/runtime/values.c \
    $(SRC)/runtime/invocation.c \
    $(SRC)/runtime/aggregates.c \
    $(SRC)/runtime/string_builtins.c \
    $(SRC)/ir/bb_pool.c \
    $(SRC)/ir/ct_arena.c \
    $(SRC)/ir/descr_asserts.c \
    $(sort $(wildcard $(SRC)/emitter/em/em_*.cpp)) \
    $(sort $(wildcard $(SRC)/planner/zeta_plan/zp_*.cpp)) \
    $(SRC)/planner/act_words.cpp \
    $(SRC)/planner/plan_facts.cpp \
    $(SRC)/planner/planner_report.cpp \
    $(SRC)/planner/planner_road.cpp \
    $(SRC)/planner/planner_operands.cpp \
    $(SRC)/planner/planner_stage_chains.cpp \
    $(SRC)/emitter/emit_str.cpp \
    $(SRC)/emitter/emit_call_probe.cpp \
    $(SRC)/templates/x86/x86_arg_roles.cpp \
    $(SRC)/templates/x86/x86_asm.cpp \
    $(SRC)/templates/x86/x86_call.cpp \
    $(SRC)/templates/x86/x86_frame.cpp \
    $(SRC)/templates/x86/x86_gc.cpp \
    $(SRC)/templates/x86/x86_icn.cpp \
    $(SRC)/templates/x86/x86_jump.cpp \
    $(SRC)/templates/x86/x86_rsp_tracker.cpp \
    $(SRC)/templates/x86/x86_rtcc.cpp \
    $(SRC)/templates/x86/x86_rodata.cpp \
    $(SRC)/templates/x86/x86_zeta.cpp \
    $(SRC)/templates/x86/x86_zverify.cpp \
    $(SRC)/templates/bb/bb_glue_flat.cpp \
    $(SRC)/templates/bb/bb_main.cpp \
    $(SRC)/templates/bb/bb_match_lambda.cpp \
    $(SRC)/templates/bb/bb_keyword_assign_snobol4.cpp \
    $(SRC)/templates/bb/bb_define_bind.cpp \
    $(SRC)/templates/bb/bb_define_shim.cpp \
    $(SRC)/templates/bb/bb_define_floater.cpp \
    $(SRC)/templates/bb/bb_define_adopt.cpp \
    $(SRC)/templates/bb/bb_define_call.cpp \
    $(SRC)/runtime/rt/bb_pat_build.cpp \
    $(SRC)/templates/bb/bb_conjunction.cpp \
    $(SRC)/templates/bb/bb_subscript.cpp \
    $(SRC)/templates/bb/bb_subscript2.cpp \
    $(SRC)/templates/bb/bb_subscript_lvck.cpp \
    $(SRC)/templates/bb/bb_deref.cpp \
    $(SRC)/templates/bb/bb_unify_const.cpp \
    $(SRC)/templates/bb/bb_unify_struct.cpp \
    $(SRC)/templates/bb/bb_unify_first.cpp \
    $(SRC)/templates/bb/bb_unify_value.cpp \
    $(SRC)/templates/bb/bb_random.cpp \
    $(SRC)/templates/bb/bb_assign_var.cpp \
    $(SRC)/templates/bb/bb_assign_var_sub.cpp \
    $(SRC)/templates/bb/bb_rev_assign_var.cpp \
    $(SRC)/templates/bb/bb_var_ref.cpp \
    $(SRC)/templates/bb/bb_goto.cpp \
    $(SRC)/templates/bb/bb_bound.cpp \
    $(SRC)/templates/bb/bb_statement.cpp \
    $(SRC)/templates/bb/bb_stmt_mark.cpp \
    $(SRC)/templates/bb/bb_setexit_test.cpp \
    $(SRC)/templates/bb/bb_cleanup.cpp \
    $(SRC)/templates/bb/bb_table_hdr.cpp \
    $(SRC)/templates/bb/bb_disjunction.cpp \
    $(SRC)/templates/bb/bb_goto_deferred.cpp \
    $(SRC)/templates/bb/bb_fail.cpp \
    $(SRC)/templates/bb/bb_cut.cpp \
    $(SRC)/templates/bb/bb_lit_integer.cpp \
    $(SRC)/templates/bb/bb_lit_string.cpp \
    $(SRC)/templates/bb/bb_lit_dstar.cpp \
    $(SRC)/templates/bb/bb_lit_thunk.cpp \
    $(SRC)/templates/bb/bb_lit_atom.cpp \
    $(SRC)/templates/bb/bb_lit_name.cpp \
    $(SRC)/templates/bb/bb_lit_charset.cpp \
    $(SRC)/templates/bb/bb_lit_real.cpp \
    $(SRC)/templates/bb/bb_var.cpp \
    $(SRC)/templates/bb/bb_var_global.cpp \
    $(SRC)/templates/bb/bb_return.cpp \
    $(SRC)/templates/bb/bb_keyword_icon.cpp \
    $(SRC)/templates/bb/bb_keyword_snobol4.cpp \
    $(SRC)/templates/bb/bb_keyword_assign.cpp \
    $(SRC)/templates/bb/bb_unop.cpp \
    $(SRC)/templates/bb/bb_succeed.cpp \
    $(SRC)/templates/bb/bb_to.cpp \
    $(SRC)/templates/bb/bb_match_len.cpp \
    $(SRC)/templates/bb/bb_match_lit.cpp \
    $(SRC)/templates/bb/bb_match_rkrun.cpp \
    $(SRC)/templates/bb/bb_match_rkcap.cpp \
    $(SRC)/templates/bb/bb_match_rkland.cpp \
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
    $(SRC)/templates/bb/bb_match_flush.cpp \
    $(SRC)/templates/bb/bb_match_casmark.cpp \
    $(SRC)/templates/bb/bb_match_fence1.cpp \
    $(SRC)/templates/bb/bb_match_abort.cpp \
    $(SRC)/templates/bb/bb_match_trap.cpp \
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
    $(SRC)/templates/bb/bb_gate_arm.cpp \
    $(SRC)/templates/bb/bb_gate.cpp \
    $(SRC)/templates/xa/xa_coexpr_entry.cpp \
    $(SRC)/templates/bb/bb_call.cpp \
    $(SRC)/templates/bb/bb_call_pl_leaf.cpp \
    $(SRC)/templates/bb/bb_call_pas_elem.cpp \
    $(SRC)/templates/bb/bb_call_proc_staged.cpp \
    $(SRC)/templates/bb/bb_call_pl_block.cpp \
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
    $(SRC)/templates/bb/bb_rev_assign_global.cpp \
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
    $(SRC)/templates/bb/bb_scan_alternate.cpp \
    $(SRC)/runtime/rt/rt_sg_scan.S \
    $(SRC)/runtime/rt/rt_asm_helpers.S \
    $(SRC)/templates/xa/xa_stubs.cpp \
    $(SRC)/templates/xa/xa_macro_library.cpp \
    $(SRC)/templates/xa/xa_bb_macro_library.cpp \
    $(SRC)/templates/xa/xa_exec_stmt_blob.cpp \
    $(SRC)/templates/xa/xa_file_header.cpp \
    $(SRC)/templates/xa/xa_bb_ptr_slot.cpp \
    $(SRC)/templates/xa/xa_flat.cpp \
    $(SRC)/templates/xa/xa_call_tail.cpp \
    $(SRC)/templates/xa/xa_icn_act_restore.cpp \
    $(SRC)/templates/xa/xa_to_helpers.cpp \
    $(SRC)/templates/xa/xa_prologue.cpp \
    $(SRC)/templates/xa/xa_epilogue.cpp \
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
    $(SRC)/optimizer/dead_pure.c \
    $(SRC)/optimizer/dead_goto.c \
    $(SRC)/optimizer/optimizer.c \
    \
    $(SRC)/ir/sm_prog.c \
    $(SRC)/ir/ir_node_index.c \
    $(SRC)/lower/lower_common.c \
    $(SRC)/lower/lower_call_forms.c \
    $(SRC)/lower/tree_to_sno.c \
    $(sort $(wildcard $(SRC)/lower/lower_icon/lic_*.c)) \
    $(SRC)/lower/lower_icon_class.c \
    $(sort $(wildcard $(SRC)/lower/lower_snobol4/lsn_*.c)) \
    $(SRC)/lower/lower_snocone.c \
    $(sort $(wildcard $(SRC)/lower/lower_prolog/lpl_*.c)) \
    $(SRC)/lower/lower_prolog_chr.c \
    $(sort $(wildcard $(SRC)/lower/lower_raku/lrk_*.c)) \
    $(SRC)/lower/lower_pascal.c \
    $(sort $(wildcard $(SRC)/lower/lower_pascal_tree/lpt_*.c)) \
    $(SRC)/lower/lower_pascal_class.c \
    $(SRC)/lower/lower_pascal_generics.c \
    $(SRC)/lower/lower_pascal_types.c \
    $(SRC)/runtime/builtins/gen_runtime.c \
    $(sort $(wildcard $(SRC)/runtime/lib/bn_*.c)) \
    $(SRC)/runtime/builtin_table.c \
    $(SRC)/runtime/builtin_leaf.c \
    $(SRC)/runtime/pas_xr.c \
    $(SRC)/runtime/posix_leaves.c \
    $(SRC)/runtime/posix_info.c \
    $(SRC)/runtime/posix_files.c \
    $(SRC)/runtime/posix_proc.c \
    $(SRC)/runtime/unicon_leaves.c \
    $(SRC)/runtime/rt_dir_snapshot.c \
    $(SRC)/runtime/icn_extfn.c \
    $(sort $(wildcard $(SRC)/runtime/unification/un_*.c)) \
    $(SRC)/runtime/core/coerce.c \
    $(SRC)/ir/scrip_ir.c \
    $(SRC)/planner/frame_layout.c \
    $(SRC)/planner/zeta_depth.c \
    $(SRC)/planner/planner_stage.c \
    $(SRC)/runtime/rt_runtime.c \
    $(SRC)/runtime/rx.c \
    $(SRC)/runtime/rk_rat.c \
    $(SRC)/runtime/pl_rat.c \
    $(SRC)/driver/driver_globals.c \
    $(SRC)/driver/driver_label.c \
    $(SRC)/driver/driver_hooks.c \
    $(SRC)/driver/driver_data.c \
    $(SRC)/driver/driver_call.c \
    $(SRC)/driver/driver_procs.c \
    $(SRC)/driver/scrip_sm.c \
    $(SRC)/driver/stmt_ast.c \
    $(SRC)/driver/sno_parse_stage.c \
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
    $(SRC)/runtime/rt/prolog_atom.c \
    $(SRC)/parsers/prolog/prolog_driver.c \
    $(SRC)/parsers/prolog/prolog_lower.c \
    $(SRC)/parsers/snocone/snocone_lex.c \
    $(SRC)/parsers/snocone/snocone_parse.tab.c \
    $(SRC)/parsers/snocone/snocone_driver.c \
    $(SRC)/parsers/raku/raku_driver.c \
    $(sort $(wildcard $(SRC)/parsers/raku/rk_syntax/rks_*.c)) \
    $(sort $(wildcard $(SRC)/parsers/raku/rk_tree/rkt_*.c)) \
    $(SRC)/parsers/raku/rk_regex.c \
    $(SRC)/parsers/rebus/rebus.tab.c \
    $(SRC)/parsers/rebus/lex.rebus.c \
    $(SRC)/parsers/rebus/rebus_driver.c \
    $(SRC)/lower/lower_rebus.c \
    $(SRC)/parsers/pascal/pascal.tab.c \
    $(SRC)/parsers/pascal/pascal.lex.c \
    $(SRC)/parsers/pascal/pascal_driver.c \
    $(SRC)/templates/bb/bb_assign_frame.cpp \
    $(SRC)/templates/bb/bb_var_frame.cpp

# ⛔ RT_OPT IS DEFINED ONCE, NEAR THE TOP (`grep -n "^RT_OPT" Makefile` names its line). A SECOND `RT_OPT ?=` stood here carrying the RETIRED O0-DEV-O2-BENCH text (s179) — inert by `?=` but read as law by anyone who greps for the flag and lands on the wrong one, which is how a NO-O2-BUILDS violation gets written in good faith. Deleted hq_P s269; the s262 FACT RULE lives at :34 and nowhere else.
RT_INCS := -I$(SRC)
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
	$(CC) $(RT_OPT) -g $(DBGMAP) $(WARN) $(DEPFLAGS) -fPIC $(RT_INCS) $(ZCFLAGS) -c $< -o $@
$(RT_OBJDIR)/%.o: %.cpp $(RT)/rt/rt.h | $(RT_OBJDIR)
	$(CC) $(RT_OPT) -g $(DBGMAP) $(WARN) $(DEPFLAGS) -fPIC -fno-gnu-unique -std=c++17 -finput-charset=UTF-8 $(RT_INCS) $(ZCFLAGS) -c $< -o $@
$(RT_OBJDIR)/%.o: %.S | $(RT_OBJDIR)
	$(CC) $(RT_OPT) -g $(DBGMAP) $(WARN) $(DEPFLAGS) -fPIC $(RT_INCS) $(ZCFLAGS) -c $< -o $@
$(RT_OBJDIR)/%.o: %.s | $(RT_OBJDIR)
	$(CC) $(RT_OPT) -g $(DBGMAP) $(WARN) $(DEPFLAGS) -fPIC $(RT_INCS) $(ZCFLAGS) -x assembler-with-cpp -c $< -o $@
# ⛔⭐ THE PREVIOUS .so IS KEPT, BY CONTENT HASH, BEFORE THIS LINK OVERWRITES IT (hq_T 2026-09-09, on the officer's rank-0 row;
# asked for by name in FINDING-2026-09-09-hq_P item 3). THE MEASURED CASE: hq_P caught a completed `make` whose binary
# emitted code its own templates do not contain, wanted to settle it by diffing the two libraries, and could not --
# "the evidence I would most want is the one thing the next build destroys". The .so is ALREADY content-addressable by
# flags ($(RT_TAG)), so a same-flags rebuild silently replaces it and the prior compiler ceases to exist. Keeping it
# costs one cp of a file that is regenerated in seconds; not keeping it costs the only diff that can settle the class.
# ⛔ KEYED BY CONTENT, NOT BY TIME: two identical rebuilds collapse onto one attic entry, so a build loop cannot flush
# a genuinely different predecessor out of the window. The window is the 8 most recent by mtime -- deliberately small,
# and pruned here rather than by a cron nobody runs (THE PACE RULES: "Do not depend on cron").
# ⛔ EVERY FILE UNDER src/ IS A PREREQUISITE OF THE RUNTIME (the coo 2026-09-27, on the cfo's finding): lib_build_currency.sh, the one
# authority on "is the artifact built from src/?", compares it with the NEWEST FILE under src/, compiled or not, while this rule relinked
# only when an object changed. 41 files under src/ reach no compile (READMEs, unit tests, sub-Makefiles, grammar sources and
# src/templates/x86/gc_allocating_table.inc, which util_gen_gc_allocating_table.py rewrites and only util_emitted_alloc_cost.py and a gate
# read). So touching one left every runner, gate and census refusing rc 2 ("the runtime predates src/") until a make pristine -- six
# minutes at load 45, three times in one cfo sitting. Now the relink is this recipe over objects already built: seconds.
RT_SRC_ALL := $(shell find $(SRC) -type f 2>/dev/null)
# ⛔ AND THIS Makefile IS A PREREQUISITE (the coo 2026-10-04): the link line below is the runtime's build input too, and a change to its
# flags with no object changed left every seat's `make` answering "up to date" on the library linked the old way -- measured on the
# landing that moved libstdc++ into it (make rc 0 in 0 s, ldd still naming libstdc++.so.6). A relink over unchanged objects with
# unchanged flags is byte-identical (measured: the plain relink cmp-equal to the shipped .so) and costs seconds.
# ── the linked parser module (stage 1 of the bootstrap, Lon 2026-10-14 "get all 7 parsers linked into SCRIP and running", and the same hour: ONE global
# space, a 3-letter prefix per parser -- sno_ snc_ rak_ reb_ icn_ pro_ pas_; ARCH-PARSER-SC-BOOTSTRAP.md section 3.8): a stage-0 runtime and driver (no
# module) compile ONE chain -- mod_head.sc, the library, every parser_<lang>.sc in SC_MOD_LANGS, mod_tail.sc, with a ModPrefix marker before each file --
# through scrip --module, which qualifies every name by its file's prefix (the library's sc_) and emits a descriptor instead of main. ONE SHARED OBJECT
# PER LANGUAGE PARSER (cto 2026-10-17, R-1823 shaped by Lon: "I like the idea of a shared object for each language parser."): each language's
# chain out/sc_mod/scmod_<lang>.sc (mod_head, the library, its parser_<lang>.sc, mod_tail; never named parser_<lang>.sc, which the importer reads as
# a self-import and skips) compiles as module parser_<lang> to out/libscrip_parser_<lang>.so, linked by no
# runtime and no program; rt_sc_module.c dlopens the one its parse asks for, beside the runtime it finds by dladdr. The user program's GVA slots start
# after the reserved 741 (SC_GVA_RESERVE, the SNOBOL4 parser's count; a parser with more cells refuses its load until each module has its own island).
SC_MOD_LANGS := snobol4 snocone rebus icon prolog pascal raku
SC_MOD_LIB := global case assign match counter stack tree ShiftReduce tdump gen qize semantic omega trace
# the parse label of each WIRED language's parser, the target of the ONE line the chain ends with (ModEntry, after mod_tail.sc: mod_tail.sc names only ModEntry and stays language-neutral);
# a language with no entry here ends its chain with ModEntry: return, a parse that yields no tree (cto 2026-10-29 22:1x CDT, ARCH-PARSER-SC-BOOTSTRAP.md section 7.x, D3 road 2)
SC_MOD_ENTRY_snobol4 := sno_SnoParse
sc_mod_pfx = $(if $(filter snobol4,$(1)),sno_,$(if $(filter snocone,$(1)),snc_,$(if $(filter raku,$(1)),rak_,$(if $(filter rebus,$(1)),reb_,$(if $(filter icon,$(1)),icn_,$(if $(filter prolog,$(1)),pro_,pas_))))))
SC_MOD_OBJS := $(SC_MOD_LANGS:%=out/sc_mod/scmod_%.o)
PARSER_SOS := $(SC_MOD_LANGS:%=out/libscrip_parser_%.so)
PARSER_TAGGED := $(SC_MOD_LANGS:%=out/libscrip_parser_%-$(RT_TAG).so)
# ⛔⭐ THE SEVEN BOOTSTRAP PARSER MODULES ARE NOT PART OF `all` (Lon 2026-10-21 19:0x CDT, in-chat, verbatim: "We have not instituted the boot strap parser into SCRIP, so why
# do we have a dependency. Fix that."; R-1877). They load only at a SCRIP_PARSE_STAGE=sc parse (R-1823) and hung off libscrip_rt, so every compiler commit rebuilt all seven in every tree
# (scrip0 relink, 330 MB of .s, about 2 core-minutes, measured R-1876). Build them on purpose with `make parsers-sc`; the blocking set's targets below ask for them, since one wired gate parses with sc.
parsers-sc: $(PARSER_SOS)
RT_SO0 := out/libscrip_rt0.so
# the stage-0 library and the shipped one link no parser module; a parser's descriptor is reached only by dlsym on its own object (R-1823)
$(RT_SO0): $(RT_PIC_OBJS) Makefile
	@mkdir -p out
	$(CC) -shared $(RT_PIC_OBJS) -Wl,-Bstatic -lstdc++ -Wl,-Bdynamic -static-libgcc -lm -lpthread -Wl,--exclude-libs,ALL -o $@
out/scrip0: $(RT_SO0) $(SRC)/driver/scrip.c $(SRC)/driver/kernel.c
	@mkdir -p $(OBJ)
	$(CC) $(CRT) -fPIC -c $(SRC)/driver/scrip.c -o $(OBJ)/scrip_driver0.o
	$(CC) $(CRT) -fPIC -c $(SRC)/driver/kernel.c -o $(OBJ)/scrip_kernel0.o
	$(CXX) -m64 -no-pie -rdynamic $(OBJ)/scrip_driver0.o $(OBJ)/scrip_kernel0.o -Wl,-rpath,'$$ORIGIN' -L$(abspath out) -lscrip_rt0 $(LIBS) -o $@
$(SC_MOD_LANGS:%=out/sc_mod/scmod_%.sc): out/sc_mod/scmod_%.sc: bootstrap/mod_head.sc bootstrap/mod_tail.sc $(SC_MOD_LIB:%=bootstrap/%.sc) bootstrap/parser_%.sc Makefile
	@mkdir -p out/sc_mod
	@{ printf "ModPrefix = 'mod_';\nimport mod_head;\nModPrefix = 'sc_';\n"; for f in $(SC_MOD_LIB); do printf "import '%s';\n" $$f; done; \
	   printf "ModPrefix = '$(call sc_mod_pfx,$*)';\nimport parser_$*;\n"; printf "ModPrefix = 'mod_';\nimport mod_tail;\nModEntry: $(if $(SC_MOD_ENTRY_$*),goto $(SC_MOD_ENTRY_$*),return);\n"; } > $@
$(SC_MOD_OBJS): out/sc_mod/scmod_%.o: out/sc_mod/scmod_%.sc out/scrip0
	./out/scrip0 --module parser_$* --compile --out=out/sc_mod/scmod_$*.s out/sc_mod/scmod_$*.sc < /dev/null
	$(CC) -c out/sc_mod/scmod_$*.s -o $@
$(RT_SO): $(RT_PIC_OBJS) $(RT_SRC_ALL) $(SRC)/runtime/rt_interposable.list Makefile
	@mkdir -p out out/attic
	@if [ -f $@ ]; then cp -pn $@ out/attic/$(notdir $(basename $(RT_SO)))-$$(md5sum $@ | cut -c1-12).so 2>/dev/null || true; fi
	@ls -1t out/attic/*.so 2>/dev/null | tail -n +9 | xargs -r rm -f
	$(CC) -shared $(RT_PIC_OBJS) $(RT_BIND) -Wl,-Bstatic -lstdc++ -Wl,-Bdynamic -static-libgcc -lm -lpthread -Wl,--exclude-libs,ALL -o $@   # ⛔⭐ libstdc++ AND libgcc ARE LINKED INTO THE RUNTIME, HIDDEN (the coo 2026-10-04, row instruments-a-mode-4-process-spends-1-76m-of-its-3-73m-instructions-in-ld-so-..., R-1501 on hq_prolog's finding). MEASURED on SCRIP 9557a201d, callgrind, a mode-4 hello world (OUTPUT = 'hi'), the runtime relinked from the same 259 objects (the plain relink is byte-identical to the shipped .so): shared -lstdc++ 3,732,883 Ir (ld.so 1.76M of it) | + -Bsymbolic-functions 3,284,379 | THIS LINE 1,819,873 (PIE) / 1,814,168 (-no-pie) | this line + -Bsymbolic-functions 1,370,109 | + -Bsymbolic 1,248,271. An empty C main() reads 156,840 Ir and 1,873,917 with -lstdc++ -lm -lpthread loaded: LOADING libstdc++.so IS THE COST (its own 4,300 symbolic relocations and its initialisers), not this library's relocations. scrip and every mode-4 binary NEED no libstdc++ of their own (readelf -d scrip: libscrip_rt.so, libc.so.6), so the process holds ONE copy, compiled from the same GCC 13 headers as these objects. SUPERSEDED 2026-10-15 BY RT_BIND (cto, R-1795): it binds FUNCTIONS only and keeps every datum interposable, so a COPY relocation stays the one copy; no executable SCRIP builds or emits carries a canonical-PLT function (scrip, scrip-ipp, the parser tools, mode-4 programs; the 122 corpus .s artifacts take no runtime function address rip-relatively), the C test drivers take one only to call it; rt_time_ns and the lexer entries the parser tools clock stay interposable by name, and every other LD_PRELOAD instrument runs on out/interposable/libscrip_rt.so; the runtime-binds-its-own-functions gates (retired by gates-zero) holds it.
	@echo "Built: $@   RT_OPT=$(RT_OPT)"
	@printf 'kept previous: %s   (out/attic/, 8 newest by content)\n' "$$(ls -1t out/attic/*.so 2>/dev/null | head -1 | xargs -r basename)"
# THE RUNTIME BINDS ITS OWN FUNCTIONS AND ITS OWN DATA (cto 2026-10-15, row startup-the-runtime-binds-its-own-symbols-..., R-1795; data
# 2026-10-17, row startup-an-empty-mode-4-program-costs-...): --dynamic-list names the only symbols that stay interposable -- the functions an
# instrument interposes (rt_time_ns for util_parser_phase_profile.sh, the lexer entries parser_main.c clocks) and the data an executable
# COPIES, so the runtime reads the executable's one copy: rtccb (every mode-4 program and scrip-ipp), sno_nerrors and rebus_nerrors (the
# parser tools), g_gc_pending (emitted code reads it); every other function and datum binds inside the
# library, and its relative relocations are plain RELA (-z nopack-relative-relocs): ld.so applies RELA's at about 6.5 Ir each, while RELR's bitmap walk measured 12.7K Ir more for the same 7,103 (hq_icon 2026-10-17). The gate holds the copy relocations of every executable SCRIP
# builds or emits to that list. MEASURED, mode-4 Icon hello under callgrind: 2,535,562 Ir shipped, 1,715,355 with the functions bound.
# An instrument that interposes any other runtime symbol by LD_PRELOAD links out/interposable/libscrip_rt.so (every symbol preemptible,
# the same objects) and runs under LD_LIBRARY_PATH (the executables carry RUNPATH).
RT_BIND := -Wl,--dynamic-list=$(SRC)/runtime/rt_interposable.list -Wl,-z,nopack-relative-relocs
out/interposable/libscrip_rt.so: $(RT_PIC_OBJS) Makefile
	@mkdir -p out/interposable
	$(CC) -shared $(RT_PIC_OBJS) -Wl,-Bstatic -lstdc++ -Wl,-Bdynamic -static-libgcc -lm -lpthread -Wl,--exclude-libs,ALL -o $@
# out/libscrip_rt.so REMAINS THE CANONICAL PATH -- 73 scripts reference it by that exact name, so it must
# never move. It is a real COPY of whichever configuration was actually built -- never a symlink (Lon 2026-10-17, R-1820:
# "Do not EVER use symlinks."). ⛔ THE FORCE PREREQUISITE IS LOAD-BEARING: switching back to a previously-built (older) tag must
# still replace the canonical file, so the copy is refreshed unconditionally. It costs one copy.
out/libscrip_rt.so: $(RT_SO) FORCE
	@rm -f out/libscrip_rt.so && cp -f $(RT_SO) out/libscrip_rt.so
	@echo "out/libscrip_rt.so = copy of $(notdir $(RT_SO))   RT_OPT=$(RT_OPT)"
# EACH PARSER IS ITS OWN SHARED OBJECT (cto 2026-10-17, R-1823): out/libscrip_parser_<lang>.so, linked alone from its module object, its
# undefined names resolved against the runtime the process already holds; the copy follows the runtime's RT_TAG discipline.
$(PARSER_TAGGED): out/libscrip_parser_%-$(RT_TAG).so: out/sc_mod/scmod_%.o
	@mkdir -p out
	$(CC) -shared $< -o $@
$(PARSER_SOS): out/libscrip_parser_%.so: out/libscrip_parser_%-$(RT_TAG).so FORCE
	@rm -f $@ && cp -f $< $@
# ── THE PROLOG PRELUDE IS ONE SHARED OBJECT (Lon 2026-11-09, COO-500, verbatim: "Make Prolog run much faster by using a shared object for all the prelude
# code." / "So you compile using mode 4 the *.pl into a shared object and make that code available to SCRIP executable."): ./scrip --prolog-prelude-unit compiles
# the whole base prelude ONCE through the mode-4 road (an empty program, every base clause kept, no library appended) as module prolog_prelude, its g_rtccb loads
# through the GOT; the object is linked alone like a parser module, its undefined names resolved against the runtime the process holds. A mode-3 Prolog run
# dlopens it beside libscrip_rt.so (rt_pl_prelude_so_load, rt_sc_module.c), installs its atoms, functors, keys, procedures and GC tables before the program is
# parsed, and compiles only the program's own clauses (the object is loaded when SCRIP_PL_PRELUDE_SO is 1 or a path; unset or 0 takes the source road, until design prelude-object step 6). MEASURED (the coo, SCRIP 4d89b1093 + this landing): a
# Prolog hello 4,735,032,215 instructions on the source road, 36,910,735 on this one; the unit compile is about 60 s and 546 MB, the object 46.7 MB.
# The unit depends on what it is made of (the tagged runtime, which holds the compiler and the embedded prelude, and the driver that emits it), never on scrip or
# out/libscrip_rt.so, which every make relinks or recopies: those are order-only, so a make with nothing changed does not recompile the unit.
out/prelude/prolog_prelude.o: $(RT_SO) $(SRC)/driver/scrip.c $(SRC)/driver/kernel.c | scrip out/libscrip_rt.so
	@mkdir -p out/prelude
	@: > out/prelude/prolog_prelude_empty.pl
	SCRIP_PL_PRELUDE_SO=0 ./scrip --prolog-prelude-unit --compile --out=out/prelude/prolog_prelude.s out/prelude/prolog_prelude_empty.pl < /dev/null
	$(CC) -c out/prelude/prolog_prelude.s -o $@
	@rm -f out/prelude/prolog_prelude.s
out/libscrip_prelude_prolog-$(RT_TAG).so: out/prelude/prolog_prelude.o
	$(CC) -shared $< -o $@
out/libscrip_prelude_prolog.so: out/libscrip_prelude_prolog-$(RT_TAG).so FORCE
	@rm -f $@ && cp -f $< $@

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
	rm -f $(RT_A) && ar rcs $(RT_A) $(RT_PIC_OBJS)
	@echo "Built: $@   RT_OPT=$(RT_OPT)"
# Same FORCE-copy-refresh reasoning as out/libscrip_rt.so above -- a stale copy surviving a
# RT_TAG switch would silently statically-link the WRONG configuration.
out/libscrip_rt.a: $(RT_A) FORCE
	@rm -f out/libscrip_rt.a && cp -f $(RT_A) out/libscrip_rt.a
	@echo "out/libscrip_rt.a = copy of $(notdir $(RT_A))   RT_OPT=$(RT_OPT)"
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
# ⛔⭐ THE RPATH IS $ORIGIN-RELATIVE, NEVER THE BUILDING ROOT'S ABSOLUTE out/ (the cto, 2026-10-31 16:5x CDT, found on the cqo's root: a build FETCHED from the cache
# (R-2041 step 6) carried its PRODUCER's absolute rpath, so the cqo's scrip loaded a runtime from another seat's scratch worktree). scrip sits at the root and finds
# out/ as $ORIGIN/out; scrip0, scrip-ipp and the parser tools sit in out/ and find it as $ORIGIN. A binary then belongs to the tree it is copied into, which is what
# one build per tree needs; util_build_cache.sh refuses to publish or fetch an artifact whose rpath names an absolute directory.
scrip: libscrip_rt
	@mkdir -p $(OBJ)
	$(CC) $(CRT) -fPIC -c $(SRC)/driver/scrip.c -o $(OBJ)/scrip_driver.o
	$(CC) $(CRT) -fPIC -c $(SRC)/driver/kernel.c -o $(OBJ)/scrip_kernel.o
	$(CXX) -m64 -no-pie -rdynamic $(OBJ)/scrip_driver.o $(OBJ)/scrip_kernel.o \
	    -Wl,-rpath,'$$ORIGIN/out' -L$(abspath out) -lscrip_rt \
	    $(LIBS) -o scrip
	@echo "Built: scrip (dynamic, links out/libscrip_rt.so)"

# ── parsers: the seven stand-alone parser executables (Lon 2026-09-27, R-1340): src/parsers/parser_main.c compiled once per frontend,
# each linking out/libscrip_rt.so exactly as scrip does, printing the tree in the one line-per-node form (ir_dump_tree) the
# Snocone TreeDump twin prints. The frontend is chosen by the build, never by a runtime switch.
PARSER_LANGS := snobol4 snocone icon prolog rebus raku pascal
parsers: $(PARSER_LANGS:%=out/parser_%)
	@echo "Built: $(PARSER_LANGS:%=out/parser_%)"
# ── scrip-ipp: the Icon Pre-Processor as SCRIP's own pre-step beside scrip (Lon 2026-09-30, RULES.md FACT RULE SCRIP DOES NOT PREPROCESS, R-1366):
# src/parsers/icon/ipp.icn is JCON's complete Icon preprocessor written in Icon, compiled by scrip itself in mode 4 and linked against the runtime.
# $define $undef $include $ifdef $ifndef $else $endif $line $error resolved to plain text on stdout, LPATH for includes; it inserts no semicolon, ever.
out/scrip-ipp: $(SRC)/parsers/icon/ipp.icn scrip libscrip_rt
	@mkdir -p $(OBJ) out
	./scrip --compile --out=$(OBJ)/ipp.s $(SRC)/parsers/icon/ipp.icn < /dev/null
	$(CC) -m64 -no-pie $(OBJ)/ipp.s -Wl,-rpath,'$$ORIGIN' -L$(abspath out) -lscrip_rt -lm -lpthread -o $@
out/parser_%: $(SRC)/parsers/parser_main.c libscrip_rt
	@mkdir -p $(OBJ) out
	$(CC) $(CRT) -DPARSER_LANG_$(shell echo $* | tr a-z A-Z) -c $(SRC)/parsers/parser_main.c -o $(OBJ)/parser_$*.o
	$(CXX) -m64 -no-pie -rdynamic $(OBJ)/parser_$*.o -Wl,-rpath,'$$ORIGIN' -L$(abspath out) -lscrip_rt $(LIBS) -o $@


# test_emit_io retired (2026-05-25): it existed only to keep the dead g_text_buf/g_bin_buf
# buffered path exercised. NO-BUFFERS ruling removed that path; emit_io.c is now pure passthrough.

# ── scrip-monitor: scrip with CSNOBOL4 4th executor linked in (IM-15b) ───────
# Build: make scrip-monitor CSN_A=$(ROOT)/../csnobol4/libcsnobol4.a  (CSN_INC=$(ROOT)/../csnobol4 supplies data.h; no root path is hardcoded)
# Requires: bash ../harness/scripts/build_csnobol4_archive.sh first
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
# live path is the per-prompt session hook (../harness/scripts/s4e_inbox_hook.sh calls the installer
# --quiet); this target is the explicit manual entry point and the one `make setup` runs.
hooks:
	@bash $(ROOT)/../harness/scripts/install_commit_msg_hook.sh

# ⛔ MEASURED 2026-08-30 (hq_B): $(ROOT)/setup.sh DOES NOT EXIST in this tree, so `make setup`
# was dying at the bash line having done nothing -- a target that looks like a bootstrap and
# cannot be one. That is why hook installation was NOT wired here despite the row's brief
# offering it: wiring into a dead path installs nothing, forever. `hooks` runs FIRST so the one
# thing this target can actually do is done, and the missing script is now named out loud
# instead of surfacing as a bare rc=127.
setup: hooks
	@# setup.sh never existed in this tree (hq_B measured the dead path 2026-08-30); the bootstrap IS these two scripts, called directly so the README's `make setup` line is true rather than a rc=127 grave.
	bash $(ROOT)/../harness/scripts/install_system_packages.sh
	bash $(ROOT)/../harness/scripts/build_official_oracles.sh

# ── Test targets ──────────────────────────────────────────────────────────────
# (the old --run corpus runners were removed with the interpreter; the live
#  gates are the ../harness/scripts/ suite — see GOAL-SNOBOL4-BB.md Session Setup.)

# ── EM-9: native codegen-emit-test — smoke + em8 gate for --native codegen-emit --x64 ──────────────
# Runs: test_smoke_snobol4.sh (7/7) + test_gate_em8_snocone_jit_emit.sh (5/5).
# Requires: libscrip_rt (built into out/libscrip_rt.so).
native codegen-emit-test: scrip libscrip_rt
	@bash ../harness/scripts/test_smoke_snobol4.sh

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
# A SOURCE THAT MOVED leaves every older objdir a .d naming its old path as the object's first prerequisite, and -MP
# phony-targets headers only, so the next incremental make dies "No rule to make target" on a file nobody should build.
# Such a .d is left out: its object rebuilds from the pattern rule when its source changes, and the .d is rewritten.
# re.c moved from src/parsers/raku to src/runtime when the flex/bison Raku parser was deleted (R-1289 phase 2, cto
# ruling), prolog_atom.c from src/parsers/prolog to src/runtime/rt (hq_prolog 481a8cd96, the second instance, R-1352).
# An empty rule per vanished source does NOT work: vpath then resolves the stem to the phantom target.
# BY RULE, NOT BY NAME (the coo 2026-09-28, R-1352): ../harness/scripts/live_dep_files.awk keeps a .d only while the source it names
# as its object's first prerequisite exists, so the next move needs no edit here; test_gate_make_leaves_out_a_dep_file_whose_
# source_is_gone.sh proves it end to end (a real make -n over a moved-source fixture) and fails once with the old by-name filter.
-include $(shell find $(OBJ) $(RT_OBJDIR) -name "*.d" 2>/dev/null | xargs -r awk -f $(ROOT)/../harness/scripts/live_dep_files.awk 2>/dev/null)
