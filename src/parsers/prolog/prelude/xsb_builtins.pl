/*  xsb_builtins.pl -- XSB BUILTINS THAT SCRIP LACKS, AS A PROLOG LAYER (cfo 2026-10-24, row prolog-xsbtests-to-100-the-58-failing-programs-ceo-1926; ceo CEO-1958: XSB 5.0.0 is the
    xsb_tests oracle, CEO-1927, and an XSB-only predicate is debt implemented as XSB does). SCRIP's own text, AGPL like the rest of SCRIP; embedded by the generator in LOCAL mode and
    parsed only when a program names one of its exports. import/1 and export/1 answer XSB's module directives ':- import P from M.' and ':- export P.' (the package prelude declares
    XSB's operators import fy 1050, export fx 1100, from xfx 1040): SCRIP has one predicate namespace, so a predicate an XSB system module exports is already visible and the
    directive succeeds having changed nothing (an import from a module whose M.P lies beside the program loads it at compile time, prolog_driver.c pl_import_one). index/1 answers
    XSB's ':- index Name/Arity-Arg.' (the package prelude declares XSB's index fy 1100), an indexing hint that changes no answer. compiler_options/1 answers XSB's
    ':- compiler_options(Options).' (consult.P proc_directive: options for compiling the current file -- spec_off, optimize, sysmod, allow_redefinition, singleton_warnings_off,
    xpp_on): none changes what SCRIP's compile answers, and a file that uses XSB's preprocessor (#define, #include) under xpp_on is refused by SCRIP's reader before the
    directive runs, so succeeding on a list hides nothing (ai_tests/read, hq_icon 2026-10-24). term_depth/2 is XSB's builtin: 1 for a variable or an
    atomic, 2 for a float (XSB boxes a float, measured on XSB 5.0.0: term_depth(1.5,D) gives 2), 1 + the deepest argument for a compound (prolog_tests/test_term_depth). dynamic_pred_has_clauses/2 is XSB assert.P's: 1 when the dynamic
    predicate of Head has a clause, else 0; a static predicate raises permission_error(access, non_dynamic, Head) and a non-callable type_error(callable, Head), as XSB's
    dynamic_code_function reports them (prolog_tests/test_dphc). set_arg/3 is XSB machine's set_arg(Term, Index, Arg) :- term_set_arg(Term, Index, Arg, -1), the backtrackable destructive
    assignment (XSB syslib/machine.P; constraint_tests/testsetarg's ref shows the old argument back after a failure), which is SWI's setarg/3 with its arguments reordered. load_dyn/1 is XSB consult.P's load_dyn(File) :- load_dyn(File, 1, usermod): the file is
    found as named when the name has an extension, else as File.P, File.pl or File (search_module's order), each clause is asserted at the end (direction 1, z) and each
    directive is run; float_tests/assert_test reads its data file assert_test_old so (cfo 2026-10-24, CEO-1960). gc_strings/0 (a no-op: SCRIP's strings are not collected by the program) and set_xsb_param/2 (float_display_format with precision(N) sets the digits write prints a float with, the flag $xsb_float_precision that the writer reads; other parameters are accepted and ignored) are XSB machine routines prolog_tests/test_float_io_precision calls. file_open/3 (a numeric or r, w, a mode; 3 opens the text as a stream), file_close/1, file_read/3 (the term and, in an open list as XSB leaves it, vv(Name, Var) for each variable, an anonymous one named _, syslib/xsb_read.P) and str_cat/3 are the XSB file_io and machine routines prolog_tests/readstrtest calls. read_canonical/1,2 are XSB syslib/consult.P's read of a term the canonical writer wrote (hq_packages 2026-10-29, prolog_tests/test_write_read), answered by SCRIP's reader, a superset of the canonical syntax. load_dync/1 is consult/1 and ensure_dync_loaded/1 consults a file once, both down the dialect(xsb) load_files road that reads
    the file's table directives (ARCH-PROLOG-WFS-TABLING.md section 8 R1, hq_packages 2026-10-28, CEO-2005). edb/1, ti/1, thread_shared/1 and document_export/1 are XSB's declarations (the package prelude declares their operators) that change nothing SCRIP answers, so each directive succeeds silently (hq_packages 2026-10-28). */
:- module(xsb_builtins, [ import/1, export/1, index/1, compiler_options/1, term_depth/2, dynamic_pred_has_clauses/2, set_arg/3, load_dyn/1, load_dync/1, ensure_dync_loaded/1, edb/1, ti/1, thread_shared/1, document_export/1, read_canonical/1, read_canonical/2, file_open/3, file_close/1, file_read/3, str_cat/3, gc_strings/0, set_xsb_param/2 ]).
import(_).
export(_).
index(_).
edb(_).
ti(_).
thread_shared(_).
document_export(_).
compiler_options(L) :- ( is_list(L) -> true ; throw(error(type_error(list, L), compiler_options/1)) ).
term_depth(T, D) :- ( compound(T) -> functor(T, _, N), '$xsb_td_args'(N, T, 0, M), D is M + 1 ; float(T) -> D = 2 ; D = 1 ).
'$xsb_td_args'(0, _, M, M) :- !.
'$xsb_td_args'(I, T, M0, M) :- arg(I, T, A), term_depth(A, DA), M1 is max(M0, DA), I1 is I - 1, '$xsb_td_args'(I1, T, M1, M).
dynamic_pred_has_clauses(H, F) :- ( var(H) -> throw(error(instantiation_error, dynamic_pred_has_clauses/2)) ; \+ callable(H) -> throw(error(type_error(callable, H), dynamic_pred_has_clauses/2))
    ; predicate_property(H, dynamic) -> ( \+ \+ clause(H, _) -> F = 1 ; F = 0 ) ; throw(error(permission_error(access, non_dynamic, H), dynamic_pred_has_clauses/2)) ).
set_arg(Term, Index, Arg) :- setarg(Index, Term, Arg).
load_dyn(File) :- '$xsb_ld_path'(File, Path), setup_call_cleanup(open(Path, read, S), '$xsb_ld_read'(S), close(S)).
load_dync(File) :- consult(File).
read_canonical(Term) :- read_term(Term, []).
read_canonical(Stream, Term) :- read_term(Stream, Term, []).
file_open(Name, Mode, Stream) :- '$xsb_fo_mode'(Mode, M), ( M == string -> open_string(Name, Stream) ; open(Name, M, Stream) ).
'$xsb_fo_mode'(0, read) :- !.
'$xsb_fo_mode'(1, write) :- !.
'$xsb_fo_mode'(2, append) :- !.
'$xsb_fo_mode'(3, string) :- !.
'$xsb_fo_mode'(M, read) :- ( M == r ; M == rb ), !.
'$xsb_fo_mode'(M, write) :- ( M == w ; M == wb ), !.
'$xsb_fo_mode'(M, append) :- ( M == a ; M == ab ), !.
'$xsb_fo_mode'(M, _) :- throw(error(domain_error(file_mode, M), file_open/3)).
file_close(Stream) :- close(Stream).
file_read(Stream, Term, Vars) :- read_term(Stream, Term, [variable_names(Bs), variables(Ws)]), '$xsb_vv'(Ws, Bs, Vars).
'$xsb_vv'([], _, _).
'$xsb_vv'([W|Ws], Bs, [vv(N, W)|Vs]) :- ( member(N0 = V0, Bs), V0 == W -> N = N0 ; N = '_' ), '$xsb_vv'(Ws, Bs, Vs).
str_cat(A, B, C) :- atom_concat(A, B, C).
gc_strings.
set_xsb_param(float_display_format, Opts) :- !, ( memberchk(precision(N), Opts) -> true ; N = 15 ), create_prolog_flag('$xsb_float_precision', N, []).
set_xsb_param(_, _).
ensure_dync_loaded(File) :- '$lf_list'(File, Fs), '$xsb_edl'(Fs).
'$xsb_edl'([]).
'$xsb_edl'([F|Fs]) :- catch(load_files(F, [dialect(xsb), if(not_loaded)]), error(E, load_files/2), throw(error(E, ensure_dync_loaded/1))), '$xsb_edl'(Fs).
'$xsb_ld_path'(F, P) :- ( var(F) -> throw(error(instantiation_error, load_dyn/1)) ; \+ atom(F) -> throw(error(type_error(atom, F), load_dyn/1)) ; true ),
    ( sub_atom(F, _, _, _, '.'), exists_file(F) -> P = F ; atom_concat(F, '.P', P), exists_file(P) -> true ; atom_concat(F, '.pl', P), exists_file(P) -> true ; exists_file(F) -> P = F
    ; throw(error(existence_error('file/module', F), load_dyn/1)) ).
'$xsb_ld_read'(S) :- repeat, read_term(S, T, []), ( T == end_of_file -> ! ; '$xsb_ld_one'(T), fail ).
'$xsb_ld_one'((:- G)) :- !, ( catch(G, _, true) -> true ; true ).
'$xsb_ld_one'(C) :- assertz(C).
