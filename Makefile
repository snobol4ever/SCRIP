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
BOXES   := $(SRC)/processor
CORPUS  ?= $(ROOT)/../corpus
OBJ     := /tmp/si_objs
CC      := gcc
CXX     := g++
CXXRT   := -O0 -g $(WARN) -std=c++17 -finput-charset=UTF-8 -I$(SRC) -I$(SRC)/include -I$(SRC)/lower -I$(SRC)/processor -I$(SRC)/emitter -I$(SRC)/runtime/core -I$(RT) -DDYN_ENGINE_LINKED
WARN    := -w
CBASE   := -O0 -g $(WARN) -I$(SRC) -I$(SRC)/include -I$(SRC)/lower -I$(SRC)/processor -I$(SRC)/emitter -I$(SRC)/runtime/core -I$(RT)
CRT     := $(CBASE) -DDYN_ENGINE_LINKED
LIBS    := -lgc -lm

# Runner defaults
SNO          ?= $(error SNO is required — e.g. make run SNO=prog.sno)
INC          ?= $(CORPUS)/programs/inc
JVM_CACHE    := /tmp/scrip_jvm_cache
NET_CACHE    := /tmp/scrip_net_cache
JASMIN       := $(SRC)/backend/jasmin.jar
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
    $(SRC)/runtime/core/core.c \
    $(SRC)/runtime/core/pattern.c \
    $(SRC)/runtime/core/invoke.c \
    $(SRC)/runtime/core/argval.c \
    $(SRC)/runtime/core/name_save.c \
    $(SRC)/runtime/core/name_t.c \
    $(SRC)/runtime/core/stmt_exec.c \
    $(SRC)/runtime/core/eval_code.c \
    $(SRC)/runtime/core/eval_pat.c \
    $(SRC)/processor/bb_pool.c \
    $(SRC)/emitter/emit_core.cpp \
    $(SRC)/emitter/emit_str.cpp \
    $(SRC)/emitter/emit_globals.c \
    $(SRC)/emitter/emit_io.c \
    $(SRC)/emitter/BB_templates/bb_arbno.cpp \
    $(SRC)/emitter/BB_templates/bb_capture.cpp \
    $(SRC)/emitter/BB_templates/bb_pat_defer.cpp \
    $(SRC)/emitter/BB_templates/bb_eps.cpp \
    $(SRC)/emitter/BB_templates/bb_nfa_passthrough.cpp \
    $(SRC)/emitter/BB_templates/bb_nfa_char.cpp \
    $(SRC)/emitter/BB_templates/bb_nfa_any.cpp \
    $(SRC)/emitter/BB_templates/bb_nfa_class.cpp \
    $(SRC)/emitter/BB_templates/bb_nfa_bol.cpp \
    $(SRC)/emitter/BB_templates/bb_nfa_eol.cpp \
    $(SRC)/emitter/BB_templates/bb_nfa_accept.cpp \
    $(SRC)/emitter/BB_templates/bb_lit.cpp \
    $(SRC)/emitter/BB_templates/bb_clause.cpp \
    $(SRC)/emitter/BB_templates/bb_builtin.cpp \
    $(SRC)/emitter/BB_templates/bb_logicvar.cpp \
    $(SRC)/emitter/BB_templates/bb_atom.cpp \
    $(SRC)/emitter/BB_templates/bb_unify.cpp \
    $(SRC)/emitter/BB_templates/bb_arith.cpp \
    $(SRC)/emitter/BB_templates/bb_conj.cpp \
    $(SRC)/emitter/BB_templates/bb_goal.cpp \
    $(SRC)/emitter/BB_templates/bb_choice.cpp \
    $(SRC)/emitter/BB_templates/bb_disj.cpp \
    $(SRC)/emitter/BB_templates/bb_ite.cpp \
    $(SRC)/emitter/BB_templates/bb_catch.cpp \
    $(SRC)/emitter/BB_templates/bb_cut.cpp \
    $(SRC)/emitter/BB_templates/bb_lit_scalar.cpp \
    $(SRC)/emitter/BB_templates/bb_var.cpp \
    $(SRC)/emitter/BB_templates/bb_assign.cpp \
    $(SRC)/emitter/BB_templates/bb_gvar_assign.cpp \
    $(SRC)/emitter/BB_templates/bb_scan_stmt.cpp \
    $(SRC)/emitter/BB_templates/bb_subject.cpp \
    $(SRC)/emitter/BB_templates/bb_ref_invariant.cpp \
    $(SRC)/emitter/BB_templates/bb_match.cpp \
    $(SRC)/emitter/BB_templates/bb_swap.cpp \
    $(SRC)/emitter/BB_templates/bb_return.cpp \
    $(SRC)/emitter/BB_templates/bb_unop.cpp \
    $(SRC)/emitter/BB_templates/bb_field.cpp \
    $(SRC)/emitter/BB_templates/bb_list_bang.cpp \
    $(SRC)/emitter/BB_templates/bb_idx.cpp \
    $(SRC)/emitter/BB_templates/bb_stub.cpp \
    $(SRC)/emitter/BB_templates/bb_fail.cpp \
    $(SRC)/emitter/BB_templates/bb_succeed.cpp \
    $(SRC)/emitter/BB_templates/bb_to.cpp \
    $(SRC)/emitter/BB_templates/bb_alt.cpp \
    $(SRC)/emitter/BB_templates/bb_every.cpp \
    $(SRC)/emitter/BB_templates/bb_call.cpp \
    $(SRC)/emitter/BB_templates/bb_program.cpp \
    $(SRC)/emitter/BB_templates/bb_proc.cpp \
    $(SRC)/emitter/BB_templates/bb_binop_gen.cpp \
    $(SRC)/emitter/BB_templates/bb_binop.cpp \
    $(SRC)/emitter/BB_templates/bb_binop_lit_arith.cpp \
    $(SRC)/emitter/BB_templates/bb_binop_jct_relop.cpp \
    $(SRC)/emitter/BB_templates/bb_binop_relop.cpp \
    $(SRC)/emitter/BB_templates/bb_binop_arith.cpp \
    $(SRC)/emitter/BB_templates/bb_binop_gvar_arith.cpp \
    $(SRC)/emitter/BB_templates/bb_binop_concat_slot.cpp \
    $(SRC)/emitter/BB_templates/bb_binop_concat_lit.cpp \
    $(SRC)/emitter/BB_templates/bb_binop_agpure.cpp \
    $(SRC)/emitter/BB_templates/bb_if.cpp \
    $(SRC)/emitter/BB_templates/bb_initial.cpp \
    $(SRC)/emitter/BB_templates/bb_case.cpp \
    $(SRC)/emitter/BB_templates/bb_gen_alt.cpp \
    $(SRC)/emitter/BB_templates/bb_proc_gen.cpp \
    $(SRC)/emitter/BB_templates/bb_limit.cpp \
    $(SRC)/emitter/BB_templates/bb_upto.cpp \
    $(SRC)/emitter/BB_templates/bb_suspend.cpp \
    $(SRC)/emitter/BB_templates/bb_seq.cpp \
    $(SRC)/emitter/BB_templates/bb_seq_gather.cpp \
    $(SRC)/emitter/BB_templates/bb_seq_flat.cpp \
    $(SRC)/emitter/BB_templates/bb_seq_passthrough.cpp \
    $(SRC)/emitter/BB_templates/bb_iterate.cpp \
    $(SRC)/emitter/BB_templates/bb_gen_scan.cpp \
    $(SRC)/emitter/BB_templates/bb_keyword.cpp \
    $(SRC)/emitter/BB_templates/bb_to_by.cpp \
    $(SRC)/emitter/BB_templates/bb_cset.cpp \
    $(SRC)/emitter/BB_templates/bb_pat_any.cpp \
    $(SRC)/emitter/BB_templates/bb_pat_notany.cpp \
    $(SRC)/emitter/BB_templates/bb_pat_span.cpp \
    $(SRC)/emitter/BB_templates/bb_pat_break.cpp \
    $(SRC)/emitter/BB_templates/bb_pat_pos.cpp \
    $(SRC)/emitter/BB_templates/bb_pat_tab.cpp \
    $(SRC)/emitter/BB_templates/bb_pat_atp.cpp \
    $(SRC)/emitter/BB_templates/bb_pat_len.cpp \
    $(SRC)/emitter/BB_templates/bb_pat_rem.cpp \
    $(SRC)/emitter/BB_templates/bb_pat_arb.cpp \
    $(SRC)/emitter/BB_templates/bb_pat_abort.cpp \
    $(SRC)/emitter/BB_templates/bb_pat_fence.cpp \
    $(SRC)/emitter/BB_templates/bb_pat_alt.cpp \
    $(SRC)/emitter/BB_templates/bb_pat_cat.cpp \
    $(SRC)/emitter/BB_templates/bb_rk_gather.cpp \
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
    $(SRC)/processor/bb_boxes.c \
    $(SRC)/processor/smx_dead_stubs.c \
    $(SRC)/lower/sm_prog.c \
    $(SRC)/lower/lower.c \
    $(SRC)/lower/lower_program.c \
    $(SRC)/lower/lower_sno.c \
    $(SRC)/runtime/interp/gen_runtime.c \
    $(SRC)/runtime/interp/scan_builtins.c \
    $(SRC)/runtime/interp/script_builtins.c \
    $(SRC)/runtime/interp/script_builtins_byname.c \
    $(SRC)/runtime/interp/resolve_runtime.c \
    $(SRC)/runtime/core/coerce.c \
    $(SRC)/lower/ast_clone.c \
    $(SRC)/lower/scrip_ir.c \
    $(SRC)/lower/bb_exec.c \
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
    $(SRC)/ast/ast_print.c \
    $(SRC)/frontend/snobol4/snobol4.tab.c \
    $(SRC)/frontend/snobol4/snobol4.lex.c \
    $(SRC)/frontend/icon/icon_runtime.c \
    $(SRC)/frontend/icon/icon_parse.c \
    $(SRC)/frontend/icon/icon_lex.c \
    $(SRC)/frontend/icon/icon_driver.c \
    $(SRC)/frontend/prolog/prolog_lex.c \
    $(SRC)/frontend/prolog/prolog_parse.c \
    $(SRC)/frontend/prolog/prolog_lower.c \
    $(SRC)/frontend/prolog/prolog_atom.c \
    $(SRC)/frontend/prolog/prolog_builtin.c \
    $(SRC)/frontend/prolog/prolog_unify.c \
    $(SRC)/frontend/prolog/prolog_driver.c \
    $(SRC)/frontend/snocone/snocone_lex.c \
    $(SRC)/frontend/snocone/snocone_parse.tab.c \
    $(SRC)/frontend/snocone/snocone_driver.c \
    $(SRC)/frontend/raku/raku.tab.c \
    $(SRC)/frontend/raku/raku.lex.c \
    $(SRC)/frontend/raku/raku_driver.c \
    $(SRC)/frontend/raku/raku_re.c \
    $(SRC)/frontend/raku/raku_nfa_bb.c \
    $(SRC)/frontend/rebus/rebus.tab.c \
    $(SRC)/frontend/rebus/lex.rebus.c \
    $(SRC)/frontend/rebus/rebus_lower.c \
    $(SRC)/frontend/rebus/rebus_emit.c \
    $(SRC)/frontend/rebus/rebus_print.c

out/libscrip_rt.so: $(RT_PIC_SRCS) $(RT)/rt/rt.h
	@mkdir -p out
	$(CC) -O0 -g $(WARN) -fPIC -shared \
	    -I$(SRC) -I$(SRC)/include -I$(SRC)/lower -I$(SRC)/processor -I$(SRC)/emitter -I$(SRC)/runtime/core -I$(RT) -I$(RT)/rt \
	    -I$(SRC)/frontend/snobol4 -I$(SRC)/frontend/raku \
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
	$(CC) $(CBASE) -c $(SRC)/frontend/snobol4/snobol4.lex.c -o $(OBJ)/snobol4.lex.o
	$(CC) $(CBASE) -c $(SRC)/frontend/snobol4/snobol4.tab.c -o $(OBJ)/snobol4.tab.o
	$(CC) $(CRT)   -c $(SRC)/runtime/core/core.c               -o $(OBJ)/snobol4.o
	$(CC) $(CRT)   -c $(SRC)/runtime/core/pattern.c        -o $(OBJ)/snobol4_pattern.o
	$(CC) $(CRT)   -c $(SRC)/runtime/core/invoke.c                 -o $(OBJ)/snobol4_invoke.o
	$(CC) $(CRT)   -c $(SRC)/runtime/core/argval.c                 -o $(OBJ)/snobol4_argval.o
	$(CC) $(CRT)   -c $(SRC)/runtime/core/name_save.c                    -o $(OBJ)/snobol4_nmd.o
	$(CC) $(CRT)   -c $(SRC)/runtime/core/name_t.c                         -o $(OBJ)/name_t.o
	$(CC) $(CRT)   -c $(SRC)/runtime/core/stmt_exec.c                  -o $(OBJ)/stmt_exec.o
	$(CC) $(CRT)   -c $(SRC)/runtime/core/eval_code.c                  -o $(OBJ)/eval_code.o
	$(CC) $(CRT)   -c $(SRC)/runtime/core/eval_pat.c                   -o $(OBJ)/eval_pat.o
	$(CC) $(CRT)   -c $(SRC)/processor/bb_pool.c                    -o $(OBJ)/bb_pool.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/emit_core.cpp              -o $(OBJ)/emit_core.o
	$(CC) $(CRT)   -c $(SRC)/emitter/emit_globals.c            -o $(OBJ)/emit_globals.o
	$(CC) $(CRT)   -c $(SRC)/emitter/emit_io.c                 -o $(OBJ)/emit_io.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_arbno.cpp      -o $(OBJ)/bb_arbno.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_capture.cpp     -o $(OBJ)/bb_capture.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_pat_defer.cpp   -o $(OBJ)/bb_pat_defer.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_eps.cpp         -o $(OBJ)/bb_eps.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_nfa_passthrough.cpp -o $(OBJ)/bb_nfa_passthrough.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_nfa_char.cpp    -o $(OBJ)/bb_nfa_char.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_nfa_any.cpp     -o $(OBJ)/bb_nfa_any.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_nfa_class.cpp   -o $(OBJ)/bb_nfa_class.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_nfa_bol.cpp     -o $(OBJ)/bb_nfa_bol.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_nfa_eol.cpp     -o $(OBJ)/bb_nfa_eol.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_nfa_accept.cpp  -o $(OBJ)/bb_nfa_accept.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_lit.cpp        -o $(OBJ)/bb_lit.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_clause.cpp           -o $(OBJ)/bb_clause.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_builtin.cpp   -o $(OBJ)/bb_builtin.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_logicvar.cpp       -o $(OBJ)/bb_logicvar.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_atom.cpp      -o $(OBJ)/bb_atom.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_unify.cpp     -o $(OBJ)/bb_unify.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_arith.cpp     -o $(OBJ)/bb_arith.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_conj.cpp       -o $(OBJ)/bb_conj.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_goal.cpp      -o $(OBJ)/bb_goal.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_choice.cpp    -o $(OBJ)/bb_choice.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_disj.cpp       -o $(OBJ)/bb_disj.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_ite.cpp       -o $(OBJ)/bb_ite.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_catch.cpp     -o $(OBJ)/bb_catch.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_cut.cpp       -o $(OBJ)/bb_cut.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_lit_scalar.cpp        -o $(OBJ)/bb_lit_scalar.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_var.cpp               -o $(OBJ)/bb_var.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_assign.cpp            -o $(OBJ)/bb_assign.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_gvar_assign.cpp        -o $(OBJ)/bb_gvar_assign.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_scan_stmt.cpp          -o $(OBJ)/bb_scan_stmt.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_subject.cpp       -o $(OBJ)/bb_subject.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_ref_invariant.cpp     -o $(OBJ)/bb_ref_invariant.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_match.cpp         -o $(OBJ)/bb_match.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_swap.cpp              -o $(OBJ)/bb_swap.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_return.cpp            -o $(OBJ)/bb_return.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_unop.cpp              -o $(OBJ)/bb_unop.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_field.cpp             -o $(OBJ)/bb_field.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_list_bang.cpp         -o $(OBJ)/bb_list_bang.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_idx.cpp               -o $(OBJ)/bb_idx.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_stub.cpp         -o $(OBJ)/bb_stub.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_fail.cpp              -o $(OBJ)/bb_fail.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_succeed.cpp           -o $(OBJ)/bb_succeed.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_to.cpp           -o $(OBJ)/bb_to.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_alt.cpp          -o $(OBJ)/bb_alt.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_every.cpp        -o $(OBJ)/bb_every.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_call.cpp         -o $(OBJ)/bb_call.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_program.cpp      -o $(OBJ)/bb_program.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_proc.cpp         -o $(OBJ)/bb_proc.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_to_by.cpp        -o $(OBJ)/bb_to_by.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_binop_gen.cpp    -o $(OBJ)/bb_binop_gen.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_binop.cpp        -o $(OBJ)/bb_binop.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_binop_lit_arith.cpp   -o $(OBJ)/bb_binop_lit_arith.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_binop_jct_relop.cpp   -o $(OBJ)/bb_binop_jct_relop.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_binop_relop.cpp       -o $(OBJ)/bb_binop_relop.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_binop_arith.cpp       -o $(OBJ)/bb_binop_arith.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_binop_gvar_arith.cpp  -o $(OBJ)/bb_binop_gvar_arith.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_binop_concat_slot.cpp -o $(OBJ)/bb_binop_concat_slot.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_binop_concat_lit.cpp  -o $(OBJ)/bb_binop_concat_lit.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_binop_agpure.cpp      -o $(OBJ)/bb_binop_agpure.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_if.cpp           -o $(OBJ)/bb_if.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_initial.cpp      -o $(OBJ)/bb_initial.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_case.cpp         -o $(OBJ)/bb_case.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_proc_gen.cpp -o $(OBJ)/bb_proc_gen.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_upto.cpp -o $(OBJ)/bb_upto.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_suspend.cpp -o $(OBJ)/bb_suspend.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_seq.cpp -o $(OBJ)/bb_seq.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_seq_gather.cpp -o $(OBJ)/bb_seq_gather.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_seq_flat.cpp -o $(OBJ)/bb_seq_flat.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_seq_passthrough.cpp -o $(OBJ)/bb_seq_passthrough.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_gen_alt.cpp -o $(OBJ)/bb_gen_alt.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_limit.cpp -o $(OBJ)/bb_limit.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_iterate.cpp -o $(OBJ)/bb_iterate.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_gen_scan.cpp -o $(OBJ)/bb_gen_scan.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_keyword.cpp -o $(OBJ)/bb_keyword.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_cset.cpp              -o $(OBJ)/bb_cset.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_pat_any.cpp          -o $(OBJ)/bb_pat_any.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_pat_notany.cpp  -o $(OBJ)/bb_pat_notany.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_pat_span.cpp    -o $(OBJ)/bb_pat_span.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_pat_break.cpp   -o $(OBJ)/bb_pat_break.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_pat_pos.cpp          -o $(OBJ)/bb_pat_pos.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_pat_tab.cpp          -o $(OBJ)/bb_pat_tab.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_pat_atp.cpp          -o $(OBJ)/bb_pat_atp.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_pat_len.cpp          -o $(OBJ)/bb_pat_len.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_pat_rem.cpp          -o $(OBJ)/bb_pat_rem.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_pat_arb.cpp          -o $(OBJ)/bb_pat_arb.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_pat_abort.cpp     -o $(OBJ)/bb_pat_abort.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/emit_str.cpp                      -o $(OBJ)/emit_str.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_pat_fence.cpp     -o $(OBJ)/bb_pat_fence.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_pat_alt.cpp    -o $(OBJ)/bb_pat_alt.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_pat_cat.cpp    -o $(OBJ)/bb_pat_cat.o
	$(CXX) $(CXXRT) -c $(SRC)/emitter/BB_templates/bb_rk_gather.cpp  -o $(OBJ)/bb_rk_gather.o
	$(CXX) $(CRT)   -I$(SRC)/emitter/XA_templates -I$(SRC)/include -c $(SRC)/emitter/XA_templates/xa_stubs.cpp -o $(OBJ)/xa_stubs.o
	$(CXX) $(CRT)   -I$(SRC)/emitter/XA_templates -I$(SRC)/include -I$(SRC)/emitter/SM_templates -c $(SRC)/emitter/XA_templates/xa_macro_library.cpp -o $(OBJ)/xa_macro_library.o
	$(CXX) $(CRT)   -I$(SRC)/emitter/XA_templates -I$(SRC)/include -c $(SRC)/emitter/XA_templates/xa_bb_macro_library.cpp -o $(OBJ)/xa_bb_macro_library.o
	$(CXX) $(CRT)   -I$(SRC)/emitter/XA_templates -I$(SRC)/include -I$(SRC)/emitter/SM_templates -c $(SRC)/emitter/XA_templates/xa_exec_stmt_blob.cpp -o $(OBJ)/xa_exec_stmt_blob.o
	$(CXX) $(CRT)   -I$(SRC)/emitter/XA_templates -I$(SRC)/include -I$(SRC)/emitter           -c $(SRC)/emitter/XA_templates/xa_file_header.cpp     -o $(OBJ)/xa_file_header.o
	$(CXX) $(CRT)   -I$(SRC)/emitter/XA_templates -I$(SRC)/include -I$(SRC)/emitter           -c $(SRC)/emitter/XA_templates/xa_bb_ptr_slot.cpp      -o $(OBJ)/xa_bb_ptr_slot.o
	$(CXX) $(CRT)   -I$(SRC)/emitter/XA_templates -I$(SRC)/include -I$(SRC)/emitter           -c $(SRC)/emitter/XA_templates/xa_flat.cpp             -o $(OBJ)/xa_flat.o
	$(CXX) $(CRT)   -I$(SRC)/emitter/XA_templates -I$(SRC)/include -I$(SRC)/emitter           -c $(SRC)/emitter/XA_templates/xa_prologue.cpp         -o $(OBJ)/xa_prologue.o
	$(CXX) $(CRT)   -I$(SRC)/emitter/XA_templates -I$(SRC)/include -I$(SRC)/emitter           -c $(SRC)/emitter/XA_templates/xa_epilogue.cpp         -o $(OBJ)/xa_epilogue.o
	$(CXX) $(CRT)   -I$(SRC)/emitter/XA_templates -I$(SRC)/include -I$(SRC)/emitter           -c $(SRC)/emitter/XA_templates/xa_wasm_main.cpp        -o $(OBJ)/xa_wasm_main.o
	$(CXX) $(CRT)   -I$(SRC)/emitter/XA_templates -I$(SRC)/include -I$(SRC)/emitter           -c $(SRC)/emitter/XA_templates/xa_js_label_register.cpp -o $(OBJ)/xa_js_label_register.o
	$(CXX) $(CRT)   -I$(SRC)/emitter/XA_templates -I$(SRC)/include -I$(SRC)/emitter           -c $(SRC)/emitter/XA_templates/xa_expression_registry.cpp -o $(OBJ)/xa_expression_registry.o
	$(CXX) $(CRT)   -I$(SRC)/emitter/XA_templates -I$(SRC)/include -I$(SRC)/emitter           -c $(SRC)/emitter/XA_templates/xa_strtab_rodata.cpp     -o $(OBJ)/xa_strtab_rodata.o
	$(CXX) $(CRT)   -I$(SRC)/emitter/XA_templates -I$(SRC)/include -I$(SRC)/emitter           -c $(SRC)/emitter/XA_templates/xa_cap_fixup.cpp         -o $(OBJ)/xa_cap_fixup.o
	$(CXX) $(CRT)   -I$(SRC)/emitter/XA_templates -I$(SRC)/include -I$(SRC)/emitter           -c $(SRC)/emitter/XA_templates/xa_pattern_blobs.cpp     -o $(OBJ)/xa_pattern_blobs.o
	$(CC) $(CRT) -c $(SRC)/processor/bb_boxes.c -o $(OBJ)/bb_boxes.o
	$(CC) $(CRT) -c $(SRC)/processor/smx_dead_stubs.c -o $(OBJ)/smx_dead_stubs.o
	$(CC) $(CBASE) -I$(SRC)/frontend/snobol4 -DIR_DEFINE_NAMES \
	    -c $(SRC)/ast/ast_print.c -o $(OBJ)/ast_print.o
	$(CC) $(CBASE) -I$(SRC)/frontend/snobol4 -c $(SRC)/frontend/snocone/snocone_lex.c        -o $(OBJ)/snocone_lex.o
	$(CC) $(CBASE) -I$(SRC)/frontend/snobol4 -c $(SRC)/frontend/snocone/snocone_parse.tab.c  -o $(OBJ)/snocone_parse.tab.o
	$(CC) $(CBASE) -I$(SRC)/frontend/snobol4 -c $(SRC)/frontend/snocone/snocone_driver.c     -o $(OBJ)/snocone_driver.o
	$(CC) $(CBASE) -I$(SRC)/frontend/snobol4 -c $(SRC)/frontend/prolog/prolog_lex.c      -o $(OBJ)/prolog_lex.o
	$(CC) $(CBASE) -I$(SRC)/frontend/snobol4 -c $(SRC)/frontend/prolog/prolog_parse.c    -o $(OBJ)/prolog_parse.o
	$(CC) $(CBASE) -I$(SRC)/frontend/snobol4 -c $(SRC)/frontend/prolog/prolog_lower.c    -o $(OBJ)/prolog_lower.o
	$(CC) $(CBASE) -I$(SRC)/frontend/snobol4 -c $(SRC)/frontend/prolog/prolog_atom.c     -o $(OBJ)/prolog_atom.o
	$(CC) $(CBASE) -I$(SRC)/frontend/snobol4 -c $(SRC)/frontend/prolog/prolog_builtin.c  -o $(OBJ)/prolog_builtin.o
	$(CC) $(CBASE) -I$(SRC)/frontend/snobol4 -c $(SRC)/frontend/prolog/prolog_unify.c    -o $(OBJ)/prolog_unify.o
	$(CC) $(CBASE) -I$(SRC)/frontend/snobol4 -c $(SRC)/frontend/prolog/prolog_driver.c   -o $(OBJ)/prolog_driver.o
	$(CC) $(CBASE) -I$(SRC)/frontend/snobol4 -c $(SRC)/frontend/icon/icon_lex.c         -o $(OBJ)/icon_lex.o
	$(CC) $(CBASE) -I$(SRC)/frontend/snobol4 -c $(SRC)/frontend/icon/icon_parse.c       -o $(OBJ)/icon_parse.o
	$(CC) $(CBASE) -I$(SRC)/frontend/snobol4 -c $(SRC)/frontend/icon/icon_runtime.c     -o $(OBJ)/icon_runtime.o

	$(CC) $(CBASE) -I$(SRC)/frontend/snobol4 -c $(SRC)/frontend/icon/icon_driver.c      -o $(OBJ)/icon_driver.o
	$(CC) $(CBASE) -I$(SRC)/frontend/snobol4 -I$(SRC)/frontend/raku -c $(SRC)/frontend/raku/raku.tab.c    -o $(OBJ)/raku.tab.o
	$(CC) $(CBASE) -I$(SRC)/frontend/snobol4 -I$(SRC)/frontend/raku -c $(SRC)/frontend/raku/raku.lex.c    -o $(OBJ)/raku.lex.o
	$(CC) $(CBASE) -I$(SRC)/frontend/snobol4 -I$(SRC)/frontend/raku -c $(SRC)/frontend/raku/raku_driver.c -o $(OBJ)/raku_driver.o
	$(CC) $(CBASE) -I$(SRC)/frontend/snobol4 -I$(SRC)/frontend/raku -c $(SRC)/frontend/raku/raku_re.c      -o $(OBJ)/raku_re.o
	$(CC) $(CBASE) -I$(SRC)/frontend/snobol4 -I$(SRC)/frontend/raku -c $(SRC)/frontend/raku/raku_nfa_bb.c  -o $(OBJ)/raku_nfa_bb.o
	$(CC) $(CBASE) -I$(SRC)/frontend/snobol4 -c $(SRC)/frontend/rebus/rebus.tab.c    -o $(OBJ)/rebus.tab.o
	$(CC) $(CBASE) -I$(SRC)/frontend/snobol4 -c $(SRC)/frontend/rebus/lex.rebus.c    -o $(OBJ)/lex.rebus.o
	$(CC) $(CBASE) -I$(SRC)/frontend/snobol4 -c $(SRC)/frontend/rebus/rebus_lower.c  -o $(OBJ)/rebus_lower.o
	$(CC) $(CBASE) -I$(SRC)/frontend/snobol4 -c $(SRC)/frontend/rebus/rebus_emit.c   -o $(OBJ)/rebus_emit.o
	$(CC) $(CBASE) -I$(SRC)/frontend/snobol4 -c $(SRC)/frontend/rebus/rebus_print.c  -o $(OBJ)/rebus_print.o
	$(CC) $(CRT)   -c $(SRC)/runtime/interp/gen_runtime.c -o $(OBJ)/gen_runtime.o
	$(CC) $(CRT)   -c $(SRC)/runtime/interp/script_builtins.c -o $(OBJ)/script_builtins.o
	$(CC) $(CRT)   -c $(SRC)/runtime/interp/script_builtins_byname.c -o $(OBJ)/script_builtins_byname.o
	$(CC) $(CRT)   -c $(SRC)/runtime/interp/scan_builtins.c -o $(OBJ)/scan_builtins.o
	$(CC) $(CRT)   -c $(SRC)/runtime/interp/resolve_runtime.c  -o $(OBJ)/resolve_runtime.o
	$(CC) $(CRT)   -c $(SRC)/runtime/core/coerce.c      -o $(OBJ)/coerce.o
	$(CC) $(CRT)   -c $(SRC)/lower/ast_clone.c    -o $(OBJ)/ast_clone.o
	$(CC) $(CRT)   -c $(SRC)/lower/scrip_ir.c     -o $(OBJ)/scrip_ir.o
	$(CC) $(CRT)   -c $(SRC)/lower/bb_exec.c      -o $(OBJ)/bb_exec.o
	$(CC) $(CRT)   -c $(SRC)/lower/sm_prog.c    -o $(OBJ)/sm_prog.o
	$(CC) $(CRT)   -c $(SRC)/lower/lower.c      -o $(OBJ)/lower.o
	$(CC) $(CRT)   -c $(SRC)/lower/lower_program.c -o $(OBJ)/lower_program.o
	$(CC) $(CRT)   -c $(SRC)/lower/lower_sno.c     -o $(OBJ)/lower_sno.o
	$(CC) $(CRT)   -c $(SRC)/emitter/emit_bb.c -o $(OBJ)/emit_bb.o
	$(CC) $(CRT)   -c $(SRC)/runtime/rt/rt.c   -o $(OBJ)/rt.o
	$(CC) $(CRT)   -c $(SRC)/runtime/rt/rt_protected.c -o $(OBJ)/rt_protected.o
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
