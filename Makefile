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
CC      := gcc
CXX     := g++
WARN    := -w
RT_OPT  ?= -O0 -g -fno-strict-aliasing -fwrapv -fno-omit-frame-pointer  # ⛔⭐ NO -O2 BUILDS, EVER (Lon 2026-08-23 s262 FACT RULE, SUPERSEDES O0-DEV-O2-BENCH/s179 and O2-ALWAYS/s178): -O0 for development AND benchmarks AND demos. Never pass RT_OPT="-O2 ...", never build an -O2 RT_TAG, never quote an -O2 number as current state. Two reasons, the second stronger: (1) an -O2 template-touching rebuild is ~9m30 vs ~1m40, paid on every arm of a measure-and-cure loop; (2) it measures a compiler we are DELETING — the RT is moving to register-aware ASM (src/runtime/rtx/*.S, GOAL-RTCC.md), so an -O2 figure grades gcc's optimizer over code that will not exist. The LABELING duty survives: every perf number still names its RT_OPT, and it now reads -O0. Authority: .github/RULES.md § NO -O2 BUILDS.
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
JASMIN       := $(ROOT)/backends/jasmin.jar
SCRIP_CC_BIN := $(ROOT)/scrip

.PHONY: all scrip setup pristine pristine-all buildinfo FORCE \
        test \
        native codegen-emit-test \
        monitor-ipc \
        libscrip_rt \
        run run-ir run-jvm run-net \
        clean distclean

# ── Primary target ────────────────────────────────────────────────────────────

all: scrip

pristine:  # THE gate-law incantation (HQ-27 PRISTINE-BUILD-BEFORE-VERDICT), now SCOPED TO THIS CONFIGURATION.
	# ⭐ WHY SCOPED IS STILL PRISTINE (Lon s258): it wipes every object THIS build can possibly link --
	# the compiler objdir, this RT_TAG's runtime objects, this tag's .so, the canonical symlink, and the
	# binary. Other tags live in directories this configuration never reads, so they cannot contaminate a
	# verdict; they are a cache, not state. That is what lets an -O2 gate run stop costing 9m30 every time.
	# Use `make pristine-all` for the old wipe-everything behaviour.
	# ⛔ AND IT NOW REMOVES ./scrip, WHICH THE OLD RECIPE DID NOT: `rm -rf $(OBJ) out` left the previous
	# binary in place, so a build that failed part-way left a stale, plausible ./scrip that every later
	# test would silently grade instead of failing loudly. Measured 2026-08-22: mid-rebuild the tree held a
	# 20:06 -O0 binary while an -O2 build was still running. That is the "non-empty is not alive" class.
	rm -rf $(OBJ) $(RT_OBJDIR) $(RT_SO) $(ROOT)/out/libscrip_rt.so $(ROOT)/scrip
	$(MAKE) all

test: scrip  # ⭐ WAS THE FALSE-GREEN TRAP (cured hq_P s268): `test`, `test-ir` and `test-all` were named in .PHONY with NO RECIPE ANYWHERE, so each exited 0 having run NOTHING ("Nothing to be done for 'test'") while reading as a full green suite. `test-ir` and `test-all` are DELETED rather than wired — nothing behind them ever existed. This target now runs THE blocking set named in CLAUDE.md and fails loudly on the first red. ⛔ Gate VERDICTS still require `make pristine` first (HQ-27); this target only builds what is missing.
	bash scripts/test_corpus_snobol4.sh
	bash scripts/test_gate_emit_no_lang.sh
	bash scripts/test_gate_template_medium_invisible.sh
	bash scripts/test_gate_corpus_coverage_classified.sh

pristine-all:  # wipe EVERY cached configuration, not just this one (the pre-s258 behaviour)
	rm -rf $(OBJ) $(ROOT)/out $(ROOT)/scrip
	$(MAKE) all

buildinfo:  # ⭐ what am I actually about to link? print it rather than assume it (LAW 0)
	@printf 'RT_OPT     : %s\n' '$(RT_OPT)'
	@printf 'ZCFLAGS    : %s\n' '$(ZCFLAGS)'
	@printf 'RT_TAG     : %s\n' '$(RT_TAG)'
	@printf 'RT_OBJDIR  : %s  (%s objects cached)\n' '$(RT_OBJDIR)' "$$(ls $(RT_OBJDIR)/*.o 2>/dev/null | wc -l)"
	@printf 'RT_SO      : %s\n' '$(RT_SO)'
	@printf 'canonical  : out/libscrip_rt.so -> %s\n' "$$(readlink out/libscrip_rt.so 2>/dev/null || echo '(none)')"
	@printf 'compiler   : %s  (hardcoded -O0; RT_OPT does NOT affect it)\n' '$(OBJ)'
	@printf 'cached tags:\n'; for d in out/rt_pic-*; do [ -d "$$d" ] && printf '   %s  %s objects\n' "$$d" "$$(ls $$d/*.o 2>/dev/null|wc -l)"; done; true

# ── libscrip_rt.so — runtime support library for --native codegen-emit --x64 ────────────
# EM-6: full SNOBOL4 runtime compiled -fPIC and linked into the .so.
# Emitted x86-64 binaries link against this .so for all language-level
# semantics (pattern matcher, NV table, exec_stmt, builtins, GC).
libscrip_rt: out/libscrip_rt.so

# EM-6 runtime objects (all compiled -fPIC so they can go into the .so)
RT_PIC_SRCS := \
    $(RT)/rtx/rtx_misc.S \
    $(RT)/rtx/rtx_zdp.S \
    $(RT)/rtx/rtx_alloc.S \
    $(RT)/rtx/rtx_str.S \
    $(RT)/rtx/rtx_arith.S \
    $(RT)/rtx/rtx_icnvar.S \
    $(RT)/rtx/rtx_icnnum.S \
    $(RT)/rtx/rtx_icnrel.S \
    $(RT)/rtx/rtx_icnagg.S \
    $(RT)/rtx/rtx_match.S \
    $(RT)/rtx/rtx_icngen.S \
    $(RT)/rtx/rtx_icncall.S \
    $(RT)/rtx/rtx_icnsub.S \
    $(RT)/rtx/rtx_plunify.S \
    $(RT)/rtx/rtx_table.S \
    $(RT)/rtx/rtx_init.c \
    $(RT)/rtx/rtcc_init.c \
    $(RT)/rt/rt_slab.c \
    $(RT)/rt/rt_arena.c \
    $(RT)/rt/rt.c \
    $(RT)/rt/zeta_alloc.c \
    $(RT)/rt/zeta_heap.c \
    $(RT)/rt/gc_heap.c \
    $(RT)/rt/rt_protected.c \
    $(RT)/rt/pat_pool.c \
    $(RT)/rt/rt_coexpr.c \
    $(RT)/rt/bbprof.c \
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
    $(SRC)/runtime/builtins/resolution.c \
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
    $(SRC)/driver/driver_ast_stubs.c \
    $(SRC)/driver/scrip_sm.c \
    $(SRC)/driver/stmt_ast.c \
    $(SRC)/driver/polyglot.c \
    $(SRC)/ir/ast_print.c \
    $(SRC)/frontend/snobol4/snobol4.tab.c \
    $(SRC)/frontend/snobol4/snobol4.lex.c \
    $(SRC)/frontend/icon/icon_runtime.c \
    $(SRC)/frontend/icon/icon_parse.c \
    $(SRC)/frontend/icon/icon_lex.c \
    $(SRC)/frontend/icon/icon_driver.c \
    $(SRC)/frontend/prolog/prolog_lex.c \
    $(SRC)/frontend/prolog/prolog_parse.c \
    $(SRC)/frontend/prolog/prolog_atom.c \
    $(SRC)/frontend/prolog/prolog_builtin.c \
    $(SRC)/frontend/prolog/prolog_unify.c \
    $(SRC)/frontend/prolog/prolog_driver.c \
    $(SRC)/frontend/prolog/prolog_lower.c \
    $(SRC)/frontend/snocone/snocone_lex.c \
    $(SRC)/frontend/snocone/snocone_parse.tab.c \
    $(SRC)/frontend/snocone/snocone_driver.c \
    $(SRC)/frontend/raku/raku.tab.c \
    $(SRC)/frontend/raku/raku.lex.c \
    $(SRC)/frontend/raku/raku_driver.c \
    $(SRC)/frontend/raku/re.c \
    $(SRC)/frontend/rebus/rebus.tab.c \
    $(SRC)/frontend/rebus/lex.rebus.c \
    $(SRC)/frontend/rebus/rebus_lower.c \
    $(SRC)/frontend/pascal/pascal.tab.c \
    $(SRC)/frontend/pascal/pascal.lex.c \
    $(SRC)/frontend/pascal/pascal_driver.c \
    $(SRC)/templates/bb/bb_arith.cpp \
    $(SRC)/templates/bb/bb_binop_gvar_arith.cpp \
    $(SRC)/templates/bb/bb_binop_gvar_arith_slot.cpp \
    $(SRC)/templates/bb/bb_call_define.cpp \
    $(SRC)/templates/bb/bb_case_arm.cpp \
    $(SRC)/templates/bb/bb_cell_cut.cpp \
    $(SRC)/templates/bb/bb_cell_ite.cpp \
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
    -I$(SRC)/frontend/snobol4 -I$(SRC)/frontend/raku -I$(SRC)/optimizer -I$(SRC)/templates/bb -I$(SRC)/templates/xa -I$(SRC)/templates/x86
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
RT_PIC_OBJS := $(addprefix $(RT_OBJDIR)/,$(addsuffix .o,$(basename $(notdir $(RT_PIC_SRCS)))))
vpath %.c $(sort $(dir $(RT_PIC_SRCS)))
vpath %.cpp $(sort $(dir $(RT_PIC_SRCS)))
vpath %.S $(sort $(dir $(RT_PIC_SRCS)))
$(RT_OBJDIR):
	@mkdir -p $(RT_OBJDIR)
$(RT_OBJDIR)/%.o: %.c $(RT)/rt/rt.h | $(RT_OBJDIR)
	$(CC) $(RT_OPT) -g $(WARN) $(DEPFLAGS) -fPIC $(RT_INCS) $(ZCFLAGS) -c $< -o $@
$(RT_OBJDIR)/%.o: %.cpp $(RT)/rt/rt.h | $(RT_OBJDIR)
	$(CC) $(RT_OPT) -g $(WARN) $(DEPFLAGS) -fPIC -std=c++17 -finput-charset=UTF-8 $(RT_INCS) $(ZCFLAGS) -c $< -o $@
$(RT_OBJDIR)/%.o: %.S | $(RT_OBJDIR)
	$(CC) $(RT_OPT) -g $(WARN) $(DEPFLAGS) -fPIC $(RT_INCS) $(ZCFLAGS) -c $< -o $@
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

setup:
	bash $(ROOT)/setup.sh

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
