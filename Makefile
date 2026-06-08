# Makefile — SCRIP unified build
#
# Primary targets:
#   make scrip        — build the unified scrip x86 executable
#   make all          — alias for scrip
#   make setup        — install system packages + CSNOBOL4 + SPITBOL oracle
#   make test         — run corpus (--interp, PASS=178 gate)
#   make test-ir      — run corpus (--interp mode)
#   make test-all     — both passes back-to-back
#   make monitor-ipc  — build test/monitor/monitor_ipc.so
#   make clean        — remove build artefacts
#   make distclean    — clean + remove /tmp caches
#
# Runner wrappers (run a single .sno file):
#   make run SNO=file.sno              — default (--interp)
#   make run-ir SNO=file.sno           — --interp (IR tree-walk)
#   make run-jvm SNO=file.sno          — legacy JVM (until M-JITEM-JVM)
#   make run-net SNO=file.sno          — legacy .NET (until M-JITEM-NET)
#
# Note: run-asm retired — replaced by: scrip --native codegen-emit --x64 (M-JITEM-X64)
#
# Prerequisites:
#   apt-get install -y libgc-dev flex nasm build-essential libgmp-dev m4
#
# Authors: Lon Jones Cherryholmes · Claude Sonnet 4.6

ROOT    := $(shell pwd)
SRC     := $(ROOT)/src
RT      := $(SRC)/runtime
BOXES   := $(SRC)/machine
CORPUS  ?= $(ROOT)/../corpus
OBJ     := /tmp/si_objs
CC      := gcc
CXX     := g++
CXXRT   := -O0 -g $(WARN) -std=c++17 -finput-charset=UTF-8 -I$(SRC) -I$(SRC)/include -I$(SRC)/contracts -I$(SRC)/lower -I$(SRC)/interp -I$(SRC)/machine -I$(SRC)/emitter -I$(SRC)/runtime/core -I$(RT) -DDYN_ENGINE_LINKED
WARN    := -w
CBASE   := -O0 -g $(WARN) -I$(SRC) -I$(SRC)/include -I$(SRC)/contracts -I$(SRC)/lower -I$(SRC)/interp -I$(SRC)/machine -I$(SRC)/emitter -I$(SRC)/runtime/core -I$(RT)
CRT     := $(CBASE) -DDYN_ENGINE_LINKED
LIBS    := -lgc -lm

# Runner defaults
SNO          ?= $(error SNO is required — e.g. make run SNO=prog.sno)
INC          ?= $(CORPUS)/programs/inc
JVM_CACHE    := /tmp/scrip_jvm_cache
NET_CACHE    := /tmp/scrip_net_cache
JASMIN       := $(SRC)/backends/jasmin.jar
SCRIP_CC_BIN := $(ROOT)/scrip

.PHONY: all scrip scrip-interp scrip setup \
        test test-ir test-all \
        native codegen-emit-test \
        monitor-ipc \
        libscrip_rt \
        run run-ir run-jvm run-net \
        clean distclean

# ── Primary target ────────────────────────────────────────────────────────────

all: scrip

# ── libscrip_rt.so — runtime support library for --native codegen-emit --x64 ────────────
# EM-6: full SNOBOL4 runtime compiled -fPIC and linked into the .so.
# Emitted x86-64 binaries link against this .so for all language-level
# semantics (pattern matcher, NV table, exec_stmt, builtins, GC).
libscrip_rt: out/libscrip_rt.so

# EM-6 runtime objects (all compiled -fPIC so they can go into the .so)
RT_PIC_SRCS := \
    $(RT)/rt/rt.c \
    $(RT)/rt/rt_protected.c \
    $(RT)/rt/pat_pool.c \
    $(SRC)/runtime/core/core.c \
    $(SRC)/runtime/core/invoke.c \
    $(SRC)/runtime/core/argval.c \
    $(SRC)/runtime/core/name_save.c \
    $(SRC)/runtime/core/name_t.c \
    $(SRC)/runtime/core/stmt_exec.c \
    $(SRC)/runtime/runtime_eval.c \
    $(SRC)/runtime/unification.c \
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
    $(SRC)/runtime/tree.c \
    $(SRC)/runtime/string_builtins.c \
    $(SRC)/machine/bb_pool.c \
    $(SRC)/emitter/emit_core.cpp \
    $(SRC)/emitter/emit_str.cpp \
    $(SRC)/emitter/emit_globals.c \
    $(SRC)/emitter/emit_io.c \
    $(SRC)/emitter/BB_templates/bb_match_defer.cpp \
    $(SRC)/emitter/BB_templates/bb_lit.cpp \
    $(SRC)/emitter/BB_templates/bb_unify.cpp \
    $(SRC)/emitter/BB_templates/bb_arith.cpp \
    $(SRC)/emitter/BB_templates/bb_conj.cpp \
    $(SRC)/emitter/BB_templates/bb_disj.cpp \
    $(SRC)/emitter/BB_templates/bb_ite.cpp \
    $(SRC)/emitter/BB_templates/bb_catch.cpp \
    $(SRC)/emitter/BB_templates/bb_cut.cpp \
    $(SRC)/emitter/BB_templates/bb_goal.cpp \
    $(SRC)/emitter/BB_templates/bb_resolve.cpp \
    $(SRC)/emitter/BB_templates/bb_io.cpp \
    $(SRC)/emitter/BB_templates/bb_is_cmp.cpp \
    $(SRC)/emitter/BB_templates/bb_type_test.cpp \
    $(SRC)/emitter/BB_templates/bb_term_inspect.cpp \
    $(SRC)/emitter/BB_templates/bb_aggregate_nb.cpp \
    $(SRC)/emitter/BB_templates/bb_atom_string.cpp \
    $(SRC)/emitter/BB_templates/bb_term_io.cpp \
    $(SRC)/emitter/BB_templates/bb_findall.cpp \
    $(SRC)/emitter/BB_templates/bb_succ_plus.cpp \
    $(SRC)/emitter/BB_templates/bb_list.cpp \
    $(SRC)/emitter/BB_templates/bb_retract_throw.cpp \
    $(SRC)/emitter/BB_templates/bb_choice.cpp \
    $(SRC)/emitter/BB_templates/bb_atom.cpp \
    $(SRC)/emitter/BB_templates/bb_logicvar.cpp \
    $(SRC)/emitter/BB_templates/bb_fail.cpp \
    $(SRC)/emitter/BB_templates/bb_lit_scalar.cpp \
    $(SRC)/emitter/BB_templates/bb_var.cpp \
    $(SRC)/emitter/BB_templates/bb_var_global.cpp \
    $(SRC)/emitter/BB_templates/bb_return.cpp \
    $(SRC)/emitter/BB_templates/bb_gvar_assign.cpp \
    $(SRC)/emitter/BB_templates/bb_var_frame.cpp \
    $(SRC)/emitter/BB_templates/bb_assign_frame.cpp \
    $(SRC)/emitter/BB_templates/bb_var_frame_ref.cpp \
    $(SRC)/emitter/BB_templates/bb_assign_frame_ref.cpp \
    $(SRC)/emitter/BB_templates/bb_scan_stmt.cpp \
    $(SRC)/emitter/BB_templates/bb_gen_scan.cpp \
    $(SRC)/emitter/BB_templates/bb_keyword.cpp \
    $(SRC)/emitter/BB_templates/bb_unop.cpp \
    $(SRC)/emitter/BB_templates/bb_succeed.cpp \
    $(SRC)/emitter/BB_templates/bb_every.cpp \
    $(SRC)/emitter/BB_templates/bb_to.cpp \
    $(SRC)/emitter/BB_templates/bb_alt.cpp \
    $(SRC)/emitter/BB_templates/bb_scan_pos.cpp \
    $(SRC)/emitter/BB_templates/bb_scan_any.cpp \
    $(SRC)/emitter/BB_templates/bb_scan_match.cpp \
    $(SRC)/emitter/BB_templates/bb_scan_many.cpp \
    $(SRC)/emitter/BB_templates/bb_scan_tab.cpp \
    $(SRC)/emitter/BB_templates/bb_scan_move.cpp \
    $(SRC)/emitter/BB_templates/bb_scan_upto.cpp \
    $(SRC)/emitter/BB_templates/bb_scan_find.cpp \
    $(SRC)/emitter/BB_templates/bb_scan_bal.cpp \
    $(SRC)/emitter/BB_templates/bb_iterate.cpp \
    $(SRC)/emitter/BB_templates/bb_call.cpp \
    $(SRC)/emitter/BB_templates/bb_call_proc_staged.cpp \
    $(SRC)/emitter/BB_templates/bb_call_write_slot.cpp \
    $(SRC)/emitter/BB_templates/bb_call_userproc.cpp \
    $(SRC)/emitter/BB_templates/bb_call_fn.cpp \
    $(SRC)/emitter/BB_templates/bb_binop_relop.cpp \
    $(SRC)/emitter/BB_templates/bb_binop_arith.cpp \
    $(SRC)/emitter/BB_templates/bb_binop_gvar_arith.cpp \
    $(SRC)/emitter/BB_templates/bb_binop_gvar_relop.cpp \
    $(SRC)/emitter/BB_templates/bb_binop_gvar_arith_slot.cpp \
    $(SRC)/emitter/BB_templates/bb_binop_concat_slot.cpp \
    $(SRC)/emitter/BB_templates/bb_match_any.cpp \
    $(SRC)/emitter/BB_templates/bb_match_notany.cpp \
    $(SRC)/emitter/BB_templates/bb_match_span.cpp \
    $(SRC)/emitter/BB_templates/bb_match_break.cpp \
    $(SRC)/emitter/BB_templates/bb_match_breakx.cpp \
    $(SRC)/emitter/BB_templates/bb_match_rtab.cpp \
    $(SRC)/emitter/BB_templates/bb_match_span_var.cpp \
    $(SRC)/emitter/BB_templates/bb_gvar_assign_lit_s.cpp \
    $(SRC)/emitter/BB_templates/bb_gvar_assign_lit_i.cpp \
    $(SRC)/emitter/BB_templates/bb_gvar_assign_var.cpp \
    $(SRC)/emitter/BB_templates/bb_gvar_assign_concat.cpp \
    $(SRC)/emitter/BB_templates/bb_gvar_assign_call.cpp \
    $(SRC)/emitter/BB_templates/bb_gvar_assign_descr.cpp \
    $(SRC)/emitter/BB_templates/bb_match_pos.cpp \
    $(SRC)/emitter/BB_templates/bb_query_frame.cpp \
    $(SRC)/emitter/BB_templates/bb_det_write.cpp \
    $(SRC)/emitter/BB_templates/bb_det_nl.cpp \
    $(SRC)/emitter/BB_templates/bb_det_is.cpp \
    $(SRC)/emitter/BB_templates/bb_det_cmp.cpp \
    $(SRC)/emitter/BB_templates/bb_cell_unify.cpp \
    $(SRC)/emitter/BB_templates/bb_cell_choice.cpp \
    $(SRC)/emitter/BB_templates/bb_cell_call.cpp \
    $(SRC)/emitter/BB_templates/bb_callee_frame.cpp \
    $(SRC)/emitter/BB_templates/bb_cell_cut.cpp \
    $(SRC)/emitter/BB_templates/bb_cell_ite.cpp \
    $(SRC)/emitter/BB_templates/bb_match_tab.cpp \
    $(SRC)/emitter/BB_templates/bb_match_atp.cpp \
    $(SRC)/emitter/BB_templates/bb_match_len.cpp \
    $(SRC)/emitter/BB_templates/bb_match_rem.cpp \
    $(SRC)/emitter/BB_templates/bb_match_arb.cpp \
    $(SRC)/emitter/BB_templates/bb_match_arbno.cpp \
    $(SRC)/emitter/BB_templates/bb_match_abort.cpp \
    $(SRC)/emitter/BB_templates/bb_match_fence.cpp \
    $(SRC)/emitter/BB_templates/bb_match_alt.cpp \
    $(SRC)/emitter/BB_templates/bb_match_cat.cpp \
    $(SRC)/emitter/BB_templates/bb_subject.cpp \
    $(SRC)/emitter/BB_templates/bb_match_head.cpp \
    $(SRC)/emitter/BB_templates/bb_match_retry.cpp \
    $(SRC)/emitter/BB_templates/bb_match_advance.cpp \
    $(SRC)/emitter/BB_templates/bb_match_capture.cpp \
    $(SRC)/emitter/BB_templates/bb_pattern_lit.cpp \
    $(SRC)/emitter/BB_templates/bb_pattern_alt.cpp \
    $(SRC)/emitter/BB_templates/bb_pattern_unary_i.cpp \
    $(SRC)/emitter/BB_templates/bb_pattern_unary_s.cpp \
    $(SRC)/emitter/BB_templates/bb_dtp_assign.cpp \
    $(SRC)/emitter/BB_templates/bb_pattern_stub.cpp \
    $(SRC)/emitter/BB_templates/bb_gather.cpp \
    $(SRC)/emitter/BB_templates/bb_assign_local.cpp \
    $(SRC)/emitter/XA_templates/xa_stubs.cpp \
    $(SRC)/emitter/XA_templates/xa_macro_library.cpp \
    $(SRC)/emitter/XA_templates/xa_bb_macro_library.cpp \
    $(SRC)/emitter/XA_templates/xa_exec_stmt_blob.cpp \
    $(SRC)/emitter/XA_templates/xa_file_header.cpp \
    $(SRC)/emitter/XA_templates/xa_bb_ptr_slot.cpp \
    $(SRC)/emitter/XA_templates/xa_flat.cpp \
    $(SRC)/emitter/XA_templates/xa_prologue.cpp \
    $(SRC)/emitter/XA_templates/xa_epilogue.cpp \
    $(SRC)/emitter/XA_templates/xa_wasm_main.cpp \
    $(SRC)/emitter/XA_templates/xa_js_label_register.cpp \
    $(SRC)/emitter/XA_templates/xa_expression_registry.cpp \
    $(SRC)/emitter/XA_templates/xa_strtab_rodata.cpp \
    $(SRC)/emitter/XA_templates/xa_cap_fixup.cpp \
    $(SRC)/emitter/XA_templates/xa_pattern_blobs.cpp \
    $(SRC)/emitter/emit_bb.c \
    \
    $(SRC)/machine/bb_boxes.c \
    $(SRC)/attic/smx_dead_stubs.c \
    $(SRC)/machine/sm_prog.c \
    $(SRC)/lower/lower.c \
    $(SRC)/lower/lower_icon.c \
    $(SRC)/lower/lower_raku.c \
    $(SRC)/lower/lower_pascal.c \
    $(SRC)/lower/lower_prolog.c \
    $(SRC)/lower/lower_program.c \
    $(SRC)/lower/lower_sno.c \
    $(SRC)/runtime/builtins/gen_runtime.c \
    $(SRC)/runtime/by_name_dispatch.c \
    $(SRC)/runtime/builtins/resolution.c \
    $(SRC)/runtime/core/coerce.c \
    $(SRC)/lower/ast_clone.c \
    $(SRC)/contracts/scrip_ir.c \
    $(SRC)/interp/IR_interp.c \
    $(SRC)/driver/interp_globals.c \
    $(SRC)/driver/interp_label.c \
    $(SRC)/driver/interp_hooks.c \
    $(SRC)/driver/interp_data.c \
    $(SRC)/driver/interp_call.c \
    $(SRC)/driver/interp_ref.c \
    $(SRC)/driver/interp_ast_stubs.c \
    $(SRC)/driver/scrip_sm.c \
    $(SRC)/driver/stmt_ast.c \
    $(SRC)/driver/sync_monitor.c \
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
    $(SRC)/parser/prolog/prolog_lower.c \
    $(SRC)/parser/prolog/prolog_atom.c \
    $(SRC)/parser/prolog/prolog_builtin.c \
    $(SRC)/parser/prolog/prolog_unify.c \
    $(SRC)/parser/prolog/prolog_driver.c \
    $(SRC)/parser/snocone/snocone_lex.c \
    $(SRC)/parser/snocone/snocone_parse.tab.c \
    $(SRC)/parser/snocone/snocone_driver.c \
    $(SRC)/parser/raku/raku.tab.c \
    $(SRC)/parser/raku/raku.lex.c \
    $(SRC)/parser/raku/raku_driver.c \
    $(SRC)/parser/raku/raku_re.c \
    $(SRC)/parser/raku/raku_nfa_bb.c \
    $(SRC)/parser/rebus/rebus.tab.c \
    $(SRC)/parser/rebus/lex.rebus.c \
    $(SRC)/parser/rebus/rebus_lower.c \
    $(SRC)/parser/rebus/rebus_emit.c \
    $(SRC)/parser/rebus/rebus_print.c \
    $(SRC)/parser/pascal/pascal.tab.c \
    $(SRC)/parser/pascal/pascal.lex.c \
    $(SRC)/parser/pascal/pascal_driver.c

out/libscrip_rt.so: $(RT_PIC_SRCS) $(RT)/rt/rt.h
	@mkdir -p out
	$(CC) -O0 -g $(WARN) -fPIC -shared \
	    -I$(SRC) -I$(SRC)/include -I$(SRC)/contracts -I$(SRC)/lower -I$(SRC)/interp -I$(SRC)/machine -I$(SRC)/emitter -I$(SRC)/runtime/core -I$(RT) -I$(RT)/rt \
	    -I$(SRC)/parser/snobol4 -I$(SRC)/parser/raku \
	    -DDYN_ENGINE_LINKED -DIR_DEFINE_NAMES \
	    $(RT_PIC_SRCS) \
	    -lgc -lm -lstdc++ \
	    -o out/libscrip_rt.so
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

scrip:
	@mkdir -p $(OBJ)
	@rm -f $(OBJ)/*.o
	$(CC) $(CBASE) -c $(SRC)/parser/snobol4/snobol4.lex.c -o $(OBJ)/snobol4.lex.o
	$(CC) $(CBASE) -c $(SRC)/parser/snobol4/snobol4.tab.c -o $(OBJ)/snobol4.tab.o
	$(CC) $(CRT)   -c $(SRC)/runtime/core/core.c               -o $(OBJ)/snobol4.o
	$(CC) $(CRT)   -c $(SRC)/runtime/core/invoke.c                 -o $(OBJ)/snobol4_invoke.o
	$(CC) $(CRT)   -c $(SRC)/runtime/core/argval.c                 -o $(OBJ)/snobol4_argval.o
	$(CC) $(CRT)   -c $(SRC)/runtime/core/name_save.c                    -o $(OBJ)/snobol4_nmd.o
	$(CC) $(CRT)   -c $(SRC)/runtime/core/name_t.c                         -o $(OBJ)/name_t.o
	$(CC) $(CRT)   -c $(SRC)/runtime/core/stmt_exec.c                  -o $(OBJ)/stmt_exec.o
	$(CC) $(CRT)   -c $(SRC)/runtime/runtime_eval.c                  -o $(OBJ)/eval_code.o
	$(CC) $(CRT)   -c $(SRC)/runtime/unification.c                  -o $(OBJ)/unification.o
	$(CC) $(CRT)   -c $(SRC)/runtime/runtime_init.c                  -o $(OBJ)/runtime_init.o
	$(CC) $(CRT)   -c $(SRC)/runtime/io_format.c                  -o $(OBJ)/io_format.o
	$(CC) $(CRT)   -c $(SRC)/runtime/arithmetic.c                  -o $(OBJ)/arithmetic.o
	$(CC) $(CRT)   -c $(SRC)/runtime/pattern_match.c              -o $(OBJ)/pattern_match.o
	$(CC) $(CRT)   -c $(SRC)/runtime/keywords.c                   -o $(OBJ)/keywords.o
	$(CC) $(CRT)   -c $(SRC)/runtime/string_ops.c                 -o $(OBJ)/string_ops.o
	$(CC) $(CRT)   -c $(SRC)/runtime/name_binding.c               -o $(OBJ)/name_binding.o
	$(CC) $(CRT)   -c $(SRC)/runtime/values.c                     -o $(OBJ)/values.o
	$(CC) $(CRT)   -c $(SRC)/runtime/invocation.c                 -o $(OBJ)/invocation.o
	$(CC) $(CRT)   -c $(SRC)/runtime/aggregates.c                 -o $(OBJ)/aggregates.o
	$(CC) $(CRT)   -c $(SRC)/runtime/tree.c                       -o $(OBJ)/tree.o
	$(CC) $(CRT)   -c $(SRC)/runtime/string_builtins.c           -o $(OBJ)/string_builtins.o
	$(CC) $(CRT)   -c $(SRC)/machine/bb_pool.c                    -o $(OBJ)/bb_pool.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/emit_core.cpp              -o $(OBJ)/emit_core.o
	$(CC) $(CRT)   -c $(SRC)/emitter/emit_globals.c            -o $(OBJ)/emit_globals.o
	$(CC) $(CRT)   -c $(SRC)/emitter/emit_io.c                 -o $(OBJ)/emit_io.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_match_defer.cpp   -o $(OBJ)/bb_match_defer.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_lit.cpp        -o $(OBJ)/bb_lit.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_unify.cpp     -o $(OBJ)/bb_unify.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_arith.cpp     -o $(OBJ)/bb_arith.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_conj.cpp       -o $(OBJ)/bb_conj.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_disj.cpp       -o $(OBJ)/bb_disj.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_ite.cpp       -o $(OBJ)/bb_ite.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_catch.cpp     -o $(OBJ)/bb_catch.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_cut.cpp       -o $(OBJ)/bb_cut.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_goal.cpp      -o $(OBJ)/bb_goal.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_resolve.cpp   -o $(OBJ)/bb_resolve.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_io.cpp -o $(OBJ)/bb_io.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_is_cmp.cpp -o $(OBJ)/bb_is_cmp.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_type_test.cpp -o $(OBJ)/bb_type_test.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_term_inspect.cpp -o $(OBJ)/bb_term_inspect.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_aggregate_nb.cpp -o $(OBJ)/bb_aggregate_nb.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_atom_string.cpp -o $(OBJ)/bb_atom_string.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_term_io.cpp -o $(OBJ)/bb_term_io.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_findall.cpp -o $(OBJ)/bb_findall.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_succ_plus.cpp -o $(OBJ)/bb_succ_plus.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_list.cpp -o $(OBJ)/bb_list.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_retract_throw.cpp -o $(OBJ)/bb_retract_throw.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_choice.cpp    -o $(OBJ)/bb_choice.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_atom.cpp      -o $(OBJ)/bb_atom.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_logicvar.cpp  -o $(OBJ)/bb_logicvar.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_fail.cpp      -o $(OBJ)/bb_fail.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_lit_scalar.cpp        -o $(OBJ)/bb_lit_scalar.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_var.cpp               -o $(OBJ)/bb_var.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_var_global.cpp        -o $(OBJ)/bb_var_global.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_return.cpp             -o $(OBJ)/bb_return.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_gvar_assign.cpp        -o $(OBJ)/bb_gvar_assign.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_var_frame.cpp          -o $(OBJ)/bb_var_frame.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_assign_frame.cpp       -o $(OBJ)/bb_assign_frame.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_var_frame_ref.cpp      -o $(OBJ)/bb_var_frame_ref.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_assign_frame_ref.cpp   -o $(OBJ)/bb_assign_frame_ref.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_scan_stmt.cpp               -o $(OBJ)/bb_scan_stmt.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_gen_scan.cpp          -o $(OBJ)/bb_gen_scan.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_keyword.cpp           -o $(OBJ)/bb_keyword.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_unop.cpp              -o $(OBJ)/bb_unop.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_succeed.cpp           -o $(OBJ)/bb_succeed.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_every.cpp        -o $(OBJ)/bb_every.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_to.cpp           -o $(OBJ)/bb_to.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_alt.cpp          -o $(OBJ)/bb_alt.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_scan_pos.cpp     -o $(OBJ)/bb_scan_pos.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_scan_any.cpp     -o $(OBJ)/bb_scan_any.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_scan_match.cpp   -o $(OBJ)/bb_scan_match.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_scan_many.cpp    -o $(OBJ)/bb_scan_many.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_scan_tab.cpp     -o $(OBJ)/bb_scan_tab.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_scan_move.cpp    -o $(OBJ)/bb_scan_move.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_scan_upto.cpp    -o $(OBJ)/bb_scan_upto.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_scan_find.cpp    -o $(OBJ)/bb_scan_find.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_scan_bal.cpp     -o $(OBJ)/bb_scan_bal.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_iterate.cpp      -o $(OBJ)/bb_iterate.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_call.cpp         -o $(OBJ)/bb_call.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_call_proc_staged.cpp  -o $(OBJ)/bb_call_proc_staged.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_call_write_slot.cpp   -o $(OBJ)/bb_call_write_slot.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_call_userproc.cpp     -o $(OBJ)/bb_call_userproc.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_call_fn.cpp      -o $(OBJ)/bb_call_fn.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_binop_relop.cpp       -o $(OBJ)/bb_binop_relop.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_binop_arith.cpp       -o $(OBJ)/bb_binop_arith.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_binop_gvar_arith.cpp  -o $(OBJ)/bb_binop_gvar_arith.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_binop_gvar_relop.cpp  -o $(OBJ)/bb_binop_gvar_relop.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_binop_gvar_arith_slot.cpp -o $(OBJ)/bb_binop_gvar_arith_slot.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_binop_concat_slot.cpp -o $(OBJ)/bb_binop_concat_slot.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_match_any.cpp          -o $(OBJ)/bb_match_any.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_match_notany.cpp  -o $(OBJ)/bb_match_notany.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_match_span.cpp    -o $(OBJ)/bb_match_span.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_match_break.cpp   -o $(OBJ)/bb_match_break.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_match_breakx.cpp  -o $(OBJ)/bb_match_breakx.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_match_rtab.cpp    -o $(OBJ)/bb_match_rtab.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_match_span_var.cpp -o $(OBJ)/bb_match_span_var.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_gvar_assign_lit_s.cpp -o $(OBJ)/bb_gvar_assign_lit_s.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_gvar_assign_lit_i.cpp -o $(OBJ)/bb_gvar_assign_lit_i.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_gvar_assign_var.cpp    -o $(OBJ)/bb_gvar_assign_var.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_gvar_assign_concat.cpp -o $(OBJ)/bb_gvar_assign_concat.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_gvar_assign_call.cpp   -o $(OBJ)/bb_gvar_assign_call.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_gvar_assign_descr.cpp  -o $(OBJ)/bb_gvar_assign_descr.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_match_pos.cpp          -o $(OBJ)/bb_match_pos.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_query_frame.cpp     -o $(OBJ)/bb_query_frame.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_det_write.cpp        -o $(OBJ)/bb_det_write.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_det_nl.cpp           -o $(OBJ)/bb_det_nl.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_det_is.cpp           -o $(OBJ)/bb_det_is.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_det_cmp.cpp          -o $(OBJ)/bb_det_cmp.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_cell_unify.cpp       -o $(OBJ)/bb_cell_unify.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_cell_choice.cpp      -o $(OBJ)/bb_cell_choice.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_cell_call.cpp        -o $(OBJ)/bb_cell_call.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_callee_frame.cpp     -o $(OBJ)/bb_callee_frame.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_cell_cut.cpp         -o $(OBJ)/bb_cell_cut.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_cell_ite.cpp         -o $(OBJ)/bb_cell_ite.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_match_tab.cpp          -o $(OBJ)/bb_match_tab.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_match_atp.cpp          -o $(OBJ)/bb_match_atp.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_match_len.cpp          -o $(OBJ)/bb_match_len.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_match_rem.cpp          -o $(OBJ)/bb_match_rem.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_match_arb.cpp          -o $(OBJ)/bb_match_arb.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_match_arbno.cpp             -o $(OBJ)/bb_match_arbno.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_match_abort.cpp     -o $(OBJ)/bb_match_abort.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/emit_str.cpp                      -o $(OBJ)/emit_str.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_match_fence.cpp     -o $(OBJ)/bb_match_fence.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_match_alt.cpp    -o $(OBJ)/bb_match_alt.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_match_cat.cpp    -o $(OBJ)/bb_match_cat.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_subject.cpp   -o $(OBJ)/bb_subject.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_match_head.cpp    -o $(OBJ)/bb_match_head.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_match_retry.cpp   -o $(OBJ)/bb_match_retry.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_match_advance.cpp -o $(OBJ)/bb_match_advance.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_match_capture.cpp -o $(OBJ)/bb_match_capture.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_pattern_lit.cpp  -o $(OBJ)/bb_pattern_lit.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_pattern_alt.cpp  -o $(OBJ)/bb_pattern_alt.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_pattern_unary_i.cpp  -o $(OBJ)/bb_pattern_unary_i.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_pattern_unary_s.cpp  -o $(OBJ)/bb_pattern_unary_s.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_dtp_assign.cpp  -o $(OBJ)/bb_dtp_assign.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_pattern_stub.cpp -o $(OBJ)/bb_pattern_stub.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_gather.cpp  -o $(OBJ)/bb_gather.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_assign_local.cpp -o $(OBJ)/bb_assign_local.o
	$(CXX) $(CRT)   -I$(SRC)/emitter/XA_templates -I$(SRC)/include -I$(SRC)/contracts -c $(SRC)/emitter/XA_templates/xa_stubs.cpp -o $(OBJ)/xa_stubs.o
	$(CXX) $(CRT)   -I$(SRC)/emitter/XA_templates -I$(SRC)/include -I$(SRC)/contracts -I$(SRC)/emitter/SM_templates -c $(SRC)/emitter/XA_templates/xa_macro_library.cpp -o $(OBJ)/xa_macro_library.o
	$(CXX) $(CRT)   -I$(SRC)/emitter/XA_templates -I$(SRC)/include -I$(SRC)/contracts -c $(SRC)/emitter/XA_templates/xa_bb_macro_library.cpp -o $(OBJ)/xa_bb_macro_library.o
	$(CXX) $(CRT)   -I$(SRC)/emitter/XA_templates -I$(SRC)/include -I$(SRC)/contracts -I$(SRC)/emitter/SM_templates -c $(SRC)/emitter/XA_templates/xa_exec_stmt_blob.cpp -o $(OBJ)/xa_exec_stmt_blob.o
	$(CXX) $(CRT)   -I$(SRC)/emitter/XA_templates -I$(SRC)/include -I$(SRC)/contracts -I$(SRC)/emitter           -c $(SRC)/emitter/XA_templates/xa_file_header.cpp     -o $(OBJ)/xa_file_header.o
	$(CXX) $(CRT)   -I$(SRC)/emitter/XA_templates -I$(SRC)/include -I$(SRC)/contracts -I$(SRC)/emitter           -c $(SRC)/emitter/XA_templates/xa_bb_ptr_slot.cpp      -o $(OBJ)/xa_bb_ptr_slot.o
	$(CXX) $(CRT)   -I$(SRC)/emitter/XA_templates -I$(SRC)/include -I$(SRC)/contracts -I$(SRC)/emitter           -c $(SRC)/emitter/XA_templates/xa_flat.cpp             -o $(OBJ)/xa_flat.o
	$(CXX) $(CRT)   -I$(SRC)/emitter/XA_templates -I$(SRC)/include -I$(SRC)/contracts -I$(SRC)/emitter           -c $(SRC)/emitter/XA_templates/xa_prologue.cpp         -o $(OBJ)/xa_prologue.o
	$(CXX) $(CRT)   -I$(SRC)/emitter/XA_templates -I$(SRC)/include -I$(SRC)/contracts -I$(SRC)/emitter           -c $(SRC)/emitter/XA_templates/xa_epilogue.cpp         -o $(OBJ)/xa_epilogue.o
	$(CXX) $(CRT)   -I$(SRC)/emitter/XA_templates -I$(SRC)/include -I$(SRC)/contracts -I$(SRC)/emitter           -c $(SRC)/emitter/XA_templates/xa_wasm_main.cpp        -o $(OBJ)/xa_wasm_main.o
	$(CXX) $(CRT)   -I$(SRC)/emitter/XA_templates -I$(SRC)/include -I$(SRC)/contracts -I$(SRC)/emitter           -c $(SRC)/emitter/XA_templates/xa_js_label_register.cpp -o $(OBJ)/xa_js_label_register.o
	$(CXX) $(CRT)   -I$(SRC)/emitter/XA_templates -I$(SRC)/include -I$(SRC)/contracts -I$(SRC)/emitter           -c $(SRC)/emitter/XA_templates/xa_expression_registry.cpp -o $(OBJ)/xa_expression_registry.o
	$(CXX) $(CRT)   -I$(SRC)/emitter/XA_templates -I$(SRC)/include -I$(SRC)/contracts -I$(SRC)/emitter           -c $(SRC)/emitter/XA_templates/xa_strtab_rodata.cpp     -o $(OBJ)/xa_strtab_rodata.o
	$(CXX) $(CRT)   -I$(SRC)/emitter/XA_templates -I$(SRC)/include -I$(SRC)/contracts -I$(SRC)/emitter           -c $(SRC)/emitter/XA_templates/xa_cap_fixup.cpp         -o $(OBJ)/xa_cap_fixup.o
	$(CXX) $(CRT)   -I$(SRC)/emitter/XA_templates -I$(SRC)/include -I$(SRC)/contracts -I$(SRC)/emitter           -c $(SRC)/emitter/XA_templates/xa_pattern_blobs.cpp     -o $(OBJ)/xa_pattern_blobs.o
	$(CC) $(CRT) -c $(SRC)/machine/bb_boxes.c -o $(OBJ)/bb_boxes.o
	$(CC) $(CRT) -c $(SRC)/attic/smx_dead_stubs.c -o $(OBJ)/smx_dead_stubs.o
	$(CC) $(CBASE) -I$(SRC)/parser/snobol4 -DIR_DEFINE_NAMES \
	    -c $(SRC)/contracts/ast_print.c -o $(OBJ)/ast_print.o
	$(CC) $(CBASE) -I$(SRC)/parser/snobol4 -c $(SRC)/parser/snocone/snocone_lex.c        -o $(OBJ)/snocone_lex.o
	$(CC) $(CBASE) -I$(SRC)/parser/snobol4 -c $(SRC)/parser/snocone/snocone_parse.tab.c  -o $(OBJ)/snocone_parse.tab.o
	$(CC) $(CBASE) -I$(SRC)/parser/snobol4 -c $(SRC)/parser/snocone/snocone_driver.c     -o $(OBJ)/snocone_driver.o
	$(CC) $(CBASE) -I$(SRC)/parser/snobol4 -c $(SRC)/parser/prolog/prolog_lex.c      -o $(OBJ)/prolog_lex.o
	$(CC) $(CBASE) -I$(SRC)/parser/snobol4 -c $(SRC)/parser/prolog/prolog_parse.c    -o $(OBJ)/prolog_parse.o
	$(CC) $(CBASE) -I$(SRC)/parser/snobol4 -c $(SRC)/parser/prolog/prolog_lower.c    -o $(OBJ)/prolog_lower.o
	$(CC) $(CBASE) -I$(SRC)/parser/snobol4 -c $(SRC)/parser/prolog/prolog_atom.c     -o $(OBJ)/prolog_atom.o
	$(CC) $(CBASE) -I$(SRC)/parser/snobol4 -c $(SRC)/parser/prolog/prolog_builtin.c  -o $(OBJ)/prolog_builtin.o
	$(CC) $(CBASE) -I$(SRC)/parser/snobol4 -c $(SRC)/parser/prolog/prolog_unify.c    -o $(OBJ)/prolog_unify.o
	$(CC) $(CBASE) -I$(SRC)/parser/snobol4 -c $(SRC)/parser/prolog/prolog_driver.c   -o $(OBJ)/prolog_driver.o
	$(CC) $(CBASE) -I$(SRC)/parser/snobol4 -c $(SRC)/parser/icon/icon_lex.c         -o $(OBJ)/icon_lex.o
	$(CC) $(CBASE) -I$(SRC)/parser/snobol4 -c $(SRC)/parser/icon/icon_parse.c       -o $(OBJ)/icon_parse.o
	$(CC) $(CBASE) -I$(SRC)/parser/snobol4 -c $(SRC)/parser/icon/icon_runtime.c     -o $(OBJ)/icon_runtime.o

	$(CC) $(CBASE) -I$(SRC)/parser/snobol4 -c $(SRC)/parser/icon/icon_driver.c      -o $(OBJ)/icon_driver.o
	$(CC) $(CBASE) -I$(SRC)/parser/snobol4 -I$(SRC)/parser/raku -c $(SRC)/parser/raku/raku.tab.c    -o $(OBJ)/raku.tab.o
	$(CC) $(CBASE) -I$(SRC)/parser/snobol4 -I$(SRC)/parser/raku -c $(SRC)/parser/raku/raku.lex.c    -o $(OBJ)/raku.lex.o
	$(CC) $(CBASE) -I$(SRC)/parser/snobol4 -I$(SRC)/parser/raku -c $(SRC)/parser/raku/raku_driver.c -o $(OBJ)/raku_driver.o
	$(CC) $(CBASE) -I$(SRC)/parser/snobol4 -I$(SRC)/parser/raku -c $(SRC)/parser/raku/raku_re.c      -o $(OBJ)/raku_re.o
	$(CC) $(CBASE) -I$(SRC)/parser/snobol4 -I$(SRC)/parser/raku -c $(SRC)/parser/raku/raku_nfa_bb.c  -o $(OBJ)/raku_nfa_bb.o
	$(CC) $(CBASE) -I$(SRC)/parser/snobol4 -c $(SRC)/parser/pascal/pascal.tab.c    -o $(OBJ)/pascal.tab.o
	$(CC) $(CBASE) -I$(SRC)/parser/snobol4 -c $(SRC)/parser/pascal/pascal.lex.c    -o $(OBJ)/pascal.lex.o
	$(CC) $(CBASE) -I$(SRC)/parser/snobol4 -c $(SRC)/parser/pascal/pascal_driver.c -o $(OBJ)/pascal_driver.o
	$(CC) $(CBASE) -I$(SRC)/parser/snobol4 -c $(SRC)/parser/rebus/rebus.tab.c    -o $(OBJ)/rebus.tab.o
	$(CC) $(CBASE) -I$(SRC)/parser/snobol4 -c $(SRC)/parser/rebus/lex.rebus.c    -o $(OBJ)/lex.rebus.o
	$(CC) $(CBASE) -I$(SRC)/parser/snobol4 -c $(SRC)/parser/rebus/rebus_lower.c  -o $(OBJ)/rebus_lower.o
	$(CC) $(CBASE) -I$(SRC)/parser/snobol4 -c $(SRC)/parser/rebus/rebus_emit.c   -o $(OBJ)/rebus_emit.o
	$(CC) $(CBASE) -I$(SRC)/parser/snobol4 -c $(SRC)/parser/rebus/rebus_print.c  -o $(OBJ)/rebus_print.o
	$(CC) $(CRT)   -c $(SRC)/runtime/builtins/gen_runtime.c -o $(OBJ)/gen_runtime.o
	$(CC) $(CRT)   -c $(SRC)/runtime/by_name_dispatch.c -o $(OBJ)/by_name_dispatch.o
	$(CC) $(CRT)   -c $(SRC)/runtime/builtins/resolution.c  -o $(OBJ)/resolution.o
	$(CC) $(CRT)   -c $(SRC)/runtime/core/coerce.c      -o $(OBJ)/coerce.o
	$(CC) $(CRT)   -c $(SRC)/lower/ast_clone.c    -o $(OBJ)/ast_clone.o
	$(CC) $(CRT)   -c $(SRC)/contracts/scrip_ir.c     -o $(OBJ)/scrip_ir.o
	$(CC) $(CRT)   -c $(SRC)/interp/IR_interp.c      -o $(OBJ)/IR_interp.o
	$(CC) $(CRT)   -c $(SRC)/machine/sm_prog.c    -o $(OBJ)/sm_prog.o
	$(CC) $(CRT)   -c $(SRC)/lower/lower.c      -o $(OBJ)/lower.o
	$(CC) $(CRT)   -c $(SRC)/lower/lower_icon.c   -o $(OBJ)/lower_icon.o
	$(CC) $(CRT)   -c $(SRC)/lower/lower_raku.c   -o $(OBJ)/lower_raku.o
	$(CC) $(CRT)   -c $(SRC)/lower/lower_pascal.c -o $(OBJ)/lower_pascal.o
	$(CC) $(CRT)   -c $(SRC)/lower/lower_prolog.c -o $(OBJ)/lower_prolog.o
	$(CC) $(CRT)   -c $(SRC)/lower/lower_program.c -o $(OBJ)/lower_program.o
	$(CC) $(CRT)   -c $(SRC)/lower/lower_sno.c     -o $(OBJ)/lower_sno.o
	$(CC) $(CRT)   -c $(SRC)/emitter/emit_bb.c -o $(OBJ)/emit_bb.o
	$(CC) $(CRT)   -c $(SRC)/runtime/rt/rt.c   -o $(OBJ)/rt.o
	$(CC) $(CRT)   -c $(SRC)/runtime/rt/rt_protected.c -o $(OBJ)/rt_protected.o
	$(CC) $(CRT)   -c $(SRC)/runtime/rt/pat_pool.c -o $(OBJ)/pat_pool.o
	$(CC) $(CRT)   -c $(SRC)/driver/interp_globals.c -o $(OBJ)/interp_globals.o
	$(CC) $(CRT)   -c $(SRC)/driver/interp_label.c   -o $(OBJ)/interp_label.o
	$(CC) $(CRT)   -c $(SRC)/driver/interp_hooks.c   -o $(OBJ)/interp_hooks.o
	$(CC) $(CRT)   -c $(SRC)/driver/interp_data.c    -o $(OBJ)/interp_data.o
	$(CC) $(CRT)   -c $(SRC)/driver/interp_call.c    -o $(OBJ)/interp_call.o
	$(CC) $(CRT)   -c $(SRC)/driver/interp_ref.c     -o $(OBJ)/interp_ref.o
	$(CC) $(CRT)   -c $(SRC)/driver/interp_ast_stubs.c -o $(OBJ)/interp_ast_stubs.o
	$(CC) $(CRT)   -c $(SRC)/driver/scrip_sm.c       -o $(OBJ)/scrip_sm.o
	$(CC) $(CRT)   -c $(SRC)/driver/stmt_ast.c       -o $(OBJ)/stmt_ast.o
	$(CC) $(CRT)   -c $(SRC)/driver/sync_monitor.c -o $(OBJ)/sync_monitor.o
	$(CC) $(CRT)   -c $(SRC)/driver/polyglot.c -o $(OBJ)/polyglot.o
	$(CC) $(CRT)   -c $(SRC)/driver/scrip.c  -o $(OBJ)/scrip_driver.o
	$(CXX) -m64 -no-pie $(OBJ)/*.o $(LIBS) -o scrip
	@echo "Built: scrip"

# backward-compat symlink
scrip-interp: scrip
	@ln -sf scrip scrip-interp

# test_emit_io retired (2026-05-25): it existed only to keep the dead g_text_buf/g_bin_buf
# buffered path exercised. NO-BUFFERS ruling removed that path; emit_io.c is now pure passthrough.

# ── scrip-monitor: scrip with CSNOBOL4 4th executor linked in (IM-15b) ───────
# Build: make scrip-monitor CSN_A=/home/claude/csnobol4/libcsnobol4.a
# Requires: bash scripts/build_csnobol4_archive.sh first
CSN_A   ?= /home/claude/csnobol4/libcsnobol4.a
CSN_INC ?= /home/claude/csnobol4

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

test: scrip
	CORPUS=$(CORPUS) bash test/run_interp_broad.sh

test-ir: scrip
	INTERP="./scrip --interp" CORPUS=$(CORPUS) bash test/run_interp_broad.sh

test-all: test test-ir

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

run-ir: scrip
	./scrip --interp $(SNO)

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
	rm -rf $(OBJ) scrip scrip-interp

distclean: clean
	rm -rf $(JVM_CACHE) $(NET_CACHE) /tmp/snobol4_asm_* /tmp/scrip_cc_*
