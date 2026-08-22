# Makefile — SCRIP unified build
#
# Primary targets:
#   make scrip        — build the unified scrip x86 executable
#   make all          — alias for scrip
#   make setup        — install system packages + CSNOBOL4 + SPITBOL oracle
#   make test         — run corpus (mode-4 gate)
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
BOXES   := $(SRC)/machine
CORPUS  ?= $(ROOT)/../corpus
# PER-TREE objdir (s150): two checkouts NEVER share .o files — the HQ-27 ABI-mix class is structurally impossible. Override only deliberately. (Comment on own line: make keeps trailing spaces before an inline #.)
OBJ     ?= /tmp/si_objs$(subst /,-,$(ROOT))
CC      := gcc
CXX     := g++
WARN    := -w
RT_OPT  ?= -O0 -g -fno-strict-aliasing -fwrapv -fno-omit-frame-pointer  # ⭐ O0-DEV-O2-BENCH (Lon 2026-08-20 in-chat, s179, SUPERSEDES O2-ALWAYS/s178): -O0 for ALL normal development — -O2 is reserved for benchmark and demo runs, passed explicitly (RT_OPT="-O2 -g -fno-strict-aliasing -fwrapv -fno-omit-frame-pointer" make). Label any perf number with its RT_OPT.
DEPFLAGS := -MMD -MP
CBASE   := -O0 -g $(WARN) $(DEPFLAGS) -I$(SRC) -I$(SRC)/include -I$(SRC)/contracts -I$(SRC)/lower -I$(SRC)/machine -I$(SRC)/emitter -I$(SRC)/runtime/core -I$(RT)
ZCFLAGS ?=
CXXRT   := -O0 -g $(WARN) $(DEPFLAGS) -std=c++17 -finput-charset=UTF-8 -I$(SRC) -I$(SRC)/include -I$(SRC)/contracts -I$(SRC)/lower -I$(SRC)/machine -I$(SRC)/emitter -I$(SRC)/runtime/core -I$(RT) -DDYN_ENGINE_LINKED $(ZCFLAGS)
CRT     := $(CBASE) -DDYN_ENGINE_LINKED $(ZCFLAGS)
LIBS    := -lm -lpthread

# Runner defaults
SNO          ?= $(error SNO is required — e.g. make run SNO=prog.sno)
INC          ?= $(CORPUS)/programs/inc
JVM_CACHE    := /tmp/scrip_jvm_cache
NET_CACHE    := /tmp/scrip_net_cache
JASMIN       := $(SRC)/backends/jasmin.jar
SCRIP_CC_BIN := $(ROOT)/scrip

.PHONY: all scrip setup pristine \
        test test-ir test-all \
        native codegen-emit-test \
        monitor-ipc \
        libscrip_rt \
        run run-ir run-jvm run-net \
        clean distclean

# ── Primary target ────────────────────────────────────────────────────────────

all: scrip

pristine:  # THE gate-law incantation (HQ-27 PRISTINE-BUILD-BEFORE-VERDICT): wipe THIS tree's objdir + out, rebuild everything. Supersedes the hand-typed `rm -rf /tmp/si_objs out && make`.
	rm -rf $(OBJ) $(ROOT)/out
	$(MAKE) all

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
    $(SRC)/machine/bb_pool.c \
    $(SRC)/emitter/emit.cpp \
    $(SRC)/emitter/emit_str.cpp \
    $(SRC)/templates/bb_glue_flat.cpp \
    $(SRC)/templates/bb_main.cpp \
    $(SRC)/templates/bb_glue_framed.cpp \
    $(SRC)/templates/bb_lit.cpp \
    $(SRC)/templates/bb_keyword_assign_snobol4.cpp \
    $(SRC)/templates/bb_define.cpp \
    $(SRC)/runtime/rt/bb_pat_build.cpp \
    $(SRC)/templates/bb_conjunction.cpp \
    $(SRC)/templates/bb_subscript.cpp \
    $(SRC)/templates/bb_deref.cpp \
    $(SRC)/templates/bb_random.cpp \
    $(SRC)/templates/bb_assign_var.cpp \
    $(SRC)/templates/bb_rev_assign_var.cpp \
    $(SRC)/templates/bb_var_ref.cpp \
    $(SRC)/templates/bb_goto.cpp \
    $(SRC)/templates/bb_bound.cpp \
    $(SRC)/templates/bb_statement.cpp \
    $(SRC)/templates/bb_disjunction.cpp \
    $(SRC)/templates/bb_goto_deferred.cpp \
    $(SRC)/templates/bb_fail.cpp \
    $(SRC)/templates/bb_cut.cpp \
    $(SRC)/templates/bb_lit_scalar.cpp \
    $(SRC)/templates/bb_var.cpp \
    $(SRC)/templates/bb_var_global.cpp \
    $(SRC)/templates/bb_return.cpp \
    $(SRC)/templates/bb_keyword_icon.cpp \
    $(SRC)/templates/bb_keyword_snobol4.cpp \
    $(SRC)/templates/bb_keyword_assign.cpp \
    $(SRC)/templates/bb_unop.cpp \
    $(SRC)/templates/bb_succeed.cpp \
    $(SRC)/templates/bb_every.cpp \
    $(SRC)/templates/bb_to.cpp \
    $(SRC)/templates/bb_match_len.cpp \
    $(SRC)/templates/bb_match_lit.cpp \
    $(SRC)/templates/bb_match_any.cpp \
    $(SRC)/templates/bb_match_notany.cpp \
    $(SRC)/templates/bb_match_span.cpp \
    $(SRC)/templates/bb_match_break.cpp \
    $(SRC)/templates/bb_match_breakx.cpp \
    $(SRC)/templates/bb_match_tab.cpp \
    $(SRC)/templates/bb_coerce_string.cpp \
    $(SRC)/templates/bb_coerce_numeric.cpp \
    $(SRC)/templates/bb_cmp_test.cpp \
    $(SRC)/templates/bb_ident.cpp \
    $(SRC)/templates/bb_differ.cpp \
    $(SRC)/templates/bb_coerce_integer.cpp \
    $(SRC)/templates/bb_coerce_real.cpp \
    $(SRC)/templates/bb_match_rtab.cpp \
    $(SRC)/templates/bb_match_pos.cpp \
    $(SRC)/templates/bb_match_rpos.cpp \
    $(SRC)/templates/bb_match_rem.cpp \
    $(SRC)/templates/bb_match_atp.cpp \
    $(SRC)/templates/bb_match_arb.cpp \
    $(SRC)/templates/bb_match_arbno.cpp \
    $(SRC)/templates/bb_match_bal.cpp \
    $(SRC)/templates/bb_match_fence0.cpp \
    $(SRC)/templates/bb_match_fence1.cpp \
    $(SRC)/templates/bb_match_abort.cpp \
    $(SRC)/templates/bb_zdp_anchor.cpp \
    $(SRC)/templates/bb_match_begin.cpp \
    $(SRC)/templates/bb_match_end.cpp \
    $(SRC)/templates/bb_match_replace.cpp \
    $(SRC)/templates/bb_match_capture.cpp \
    $(SRC)/templates/bb_match_alternate.cpp \
    $(SRC)/templates/bb_match_defer.cpp \
    $(SRC)/templates/bb_match_value.cpp \
    $(SRC)/templates/bb_to_by.cpp \
    $(SRC)/templates/bb_make_list.cpp \
    $(SRC)/templates/bb_activate.cpp \
    $(SRC)/templates/bb_create.cpp \
    $(SRC)/templates/bb_coret.cpp \
    $(SRC)/templates/bb_cofail.cpp \
    $(SRC)/templates/bb_move_label.cpp \
    $(SRC)/templates/bb_indirect_goto.cpp \
    $(SRC)/templates/xa_coexpr_entry.cpp \
    $(SRC)/templates/bb_call.cpp \
    $(SRC)/templates/bb_call_proc_staged.cpp \
    $(SRC)/templates/bb_call_write_slot.cpp \
    $(SRC)/templates/bb_call_bool.cpp \
    $(SRC)/templates/bb_call_fn.cpp \
    $(SRC)/templates/bb_binop_relop.cpp \
    $(SRC)/templates/bb_binop_arith.cpp \
    $(SRC)/templates/bb_binop_concat_slot.cpp \
    $(SRC)/templates/bb_binop_xrep_slot.cpp \
    $(SRC)/templates/bb_assign_local.cpp \
    $(SRC)/templates/bb_assign_global.cpp \
    $(SRC)/templates/bb_field_get.cpp \
    $(SRC)/templates/bb_suspend.cpp \
    $(SRC)/templates/bb_iterate.cpp \
    $(SRC)/templates/bb_repalt.cpp \
    $(SRC)/templates/bb_limit.cpp \
    $(SRC)/templates/bb_section.cpp \
    $(SRC)/templates/bb_rev_assign.cpp \
    $(SRC)/templates/bb_rev_swap.cpp \
    $(SRC)/templates/bb_swap.cpp \
    $(SRC)/templates/bb_swap_var.cpp \
    $(SRC)/templates/bb_proc_value.cpp \
    $(SRC)/templates/bb_call_value.cpp \
    $(SRC)/templates/bb_enter_init.cpp \
    $(SRC)/templates/bb_gen_scan.cpp \
    $(SRC)/templates/bb_scan_tab.cpp \
    $(SRC)/templates/bb_scan_move.cpp \
    $(SRC)/templates/bb_scan_upto.cpp \
    $(SRC)/templates/bb_scan_any.cpp \
    $(SRC)/templates/bb_scan_many.cpp \
    $(SRC)/templates/bb_scan_find.cpp \
    $(SRC)/templates/bb_scan_match.cpp \
    $(SRC)/templates/bb_scan_pos.cpp \
    $(SRC)/templates/bb_scan_bal.cpp \
    $(SRC)/templates/bb_scan_sequence.cpp \
    $(SRC)/templates/bb_scan_alternate.cpp \
    $(SRC)/templates/bb_glit.cpp \
    $(SRC)/templates/bb_gcc.cpp \
    $(SRC)/templates/bb_galt.cpp \
    $(SRC)/runtime/rt_gram_trampoline.S \
    $(SRC)/runtime/rt/rt_sg_scan.S \
    $(SRC)/runtime/rt/rt_asm_helpers.S \
    $(SRC)/templates/xa_stubs.cpp \
    $(SRC)/templates/xa_macro_library.cpp \
    $(SRC)/templates/xa_bb_macro_library.cpp \
    $(SRC)/templates/xa_exec_stmt_blob.cpp \
    $(SRC)/templates/xa_file_header.cpp \
    $(SRC)/templates/xa_bb_ptr_slot.cpp \
    $(SRC)/templates/xa_flat.cpp \
    $(SRC)/templates/xa_prologue.cpp \
    $(SRC)/templates/xa_epilogue.cpp \
    $(SRC)/templates/xa_wasm_main.cpp \
    $(SRC)/templates/xa_js_label_register.cpp \
    $(SRC)/templates/xa_expression_registry.cpp \
    $(SRC)/templates/xa_strtab_rodata.cpp \
    $(SRC)/templates/xa_csettab_rodata.cpp \
    $(SRC)/templates/xa_cap_fixup.cpp \
    $(SRC)/templates/xa_pattern_blobs.cpp \
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
    $(SRC)/machine/sm_prog.c \
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
    $(SRC)/contracts/scrip_ir.c \
    $(SRC)/contracts/zeta_storage.c \
    $(SRC)/contracts/zeta_depth.c \
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
    $(SRC)/contracts/ast_print.c \
    $(SRC)/parser/snobol4/snobol4.tab.c \
    $(SRC)/parser/snobol4/snobol4.lex.c \
    $(SRC)/parser/icon/icon_runtime.c \
    $(SRC)/parser/icon/icon_parse.c \
    $(SRC)/parser/icon/icon_lex.c \
    $(SRC)/parser/icon/icon_driver.c \
    $(SRC)/parser/prolog/prolog_lex.c \
    $(SRC)/parser/prolog/prolog_parse.c \
    $(SRC)/parser/prolog/prolog_atom.c \
    $(SRC)/parser/prolog/prolog_builtin.c \
    $(SRC)/parser/prolog/prolog_unify.c \
    $(SRC)/parser/prolog/prolog_driver.c \
    $(SRC)/parser/prolog/prolog_lower.c \
    $(SRC)/parser/snocone/snocone_lex.c \
    $(SRC)/parser/snocone/snocone_parse.tab.c \
    $(SRC)/parser/snocone/snocone_driver.c \
    $(SRC)/parser/raku/raku.tab.c \
    $(SRC)/parser/raku/raku.lex.c \
    $(SRC)/parser/raku/raku_driver.c \
    $(SRC)/parser/raku/re.c \
    $(SRC)/parser/rebus/rebus.tab.c \
    $(SRC)/parser/rebus/lex.rebus.c \
    $(SRC)/parser/rebus/rebus_lower.c \
    $(SRC)/parser/pascal/pascal.tab.c \
    $(SRC)/parser/pascal/pascal.lex.c \
    $(SRC)/parser/pascal/pascal_driver.c \
    $(SRC)/templates/bb_arith.cpp \
    $(SRC)/templates/bb_binop_gvar_arith.cpp \
    $(SRC)/templates/bb_binop_gvar_arith_slot.cpp \
    $(SRC)/templates/bb_call_define.cpp \
    $(SRC)/templates/bb_case_arm.cpp \
    $(SRC)/templates/bb_cell_cut.cpp \
    $(SRC)/templates/bb_cell_ite.cpp \
    $(SRC)/templates/bb_det_nl.cpp \
    $(SRC)/templates/bb_idx_get.cpp \
    $(SRC)/templates/bb_idx_set.cpp \
    $(SRC)/templates/bb_indirect_assign_lit_s.cpp \
    $(SRC)/templates/bb_indirect_assign_var.cpp \
    $(SRC)/templates/bb_initial.cpp \
    $(SRC)/templates/bb_key_gen.cpp \
    $(SRC)/templates/bb_ref_invariant.cpp \
    $(SRC)/templates/bb_subject.cpp \
    $(SRC)/templates/bb_unop_gvar_slot.cpp \
    $(SRC)/templates/bb_var_frame.cpp \
    $(SRC)/templates/bb_var_frame_ref.cpp

RT_OPT ?= -O0 -g -fno-strict-aliasing -fwrapv -fno-omit-frame-pointer  # ⭐ O0-DEV-O2-BENCH (Lon 2026-08-20 s179): -O0 for development; -O2 explicitly for benchmark/demo runs only
RT_INCS := -I$(SRC) -I$(SRC)/include -I$(SRC)/contracts -I$(SRC)/lower -I$(SRC)/machine -I$(SRC)/emitter -I$(SRC)/runtime/core -I$(SRC)/runtime/builtins -I$(RT) -I$(RT)/rt \
    -I$(SRC)/parser/snobol4 -I$(SRC)/parser/raku -I$(SRC)/optimizer
RT_OBJDIR := out/rt_pic
RT_PIC_OBJS := $(addprefix $(RT_OBJDIR)/,$(addsuffix .o,$(basename $(notdir $(RT_PIC_SRCS)))))
vpath %.c $(sort $(dir $(RT_PIC_SRCS)))
vpath %.cpp $(sort $(dir $(RT_PIC_SRCS)))
vpath %.S $(sort $(dir $(RT_PIC_SRCS)))
$(RT_OBJDIR):
	@mkdir -p $(RT_OBJDIR)
$(RT_OBJDIR)/%.o: %.c $(RT)/rt/rt.h | $(RT_OBJDIR)
	$(CC) $(RT_OPT) -g $(WARN) $(DEPFLAGS) -fPIC $(RT_INCS) -DDYN_ENGINE_LINKED -DIR_DEFINE_NAMES $(ZCFLAGS) -c $< -o $@
$(RT_OBJDIR)/%.o: %.cpp $(RT)/rt/rt.h | $(RT_OBJDIR)
	$(CC) $(RT_OPT) -g $(WARN) $(DEPFLAGS) -fPIC -std=c++17 -finput-charset=UTF-8 $(RT_INCS) -DDYN_ENGINE_LINKED -DIR_DEFINE_NAMES $(ZCFLAGS) -c $< -o $@
$(RT_OBJDIR)/%.o: %.S | $(RT_OBJDIR)
	$(CC) $(RT_OPT) -g $(WARN) $(DEPFLAGS) -fPIC $(RT_INCS) -DDYN_ENGINE_LINKED -DIR_DEFINE_NAMES $(ZCFLAGS) -c $< -o $@
out/libscrip_rt.so: $(RT_PIC_OBJS)
	@mkdir -p out
	$(CC) -shared $(RT_PIC_OBJS) -lm -lstdc++ -lpthread -o out/libscrip_rt.so
	@echo "Built: out/libscrip_rt.so"

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
