/*  xsb_builtins.pl -- XSB BUILTINS THAT SCRIP LACKS, AS A PROLOG LAYER (cfo 2026-10-24, row prolog-xsbtests-to-100-the-58-failing-programs-ceo-1926; ceo CEO-1958: XSB 5.0.0 is the
    xsb_tests oracle, CEO-1927, and an XSB-only predicate is debt implemented as XSB does). SCRIP's own text, AGPL like the rest of SCRIP; embedded by the generator in LOCAL mode and
    parsed only when a program names one of its exports. import/1 and export/1 answer XSB's module directives ':- import P from M.' and ':- export P.' (the package prelude declares
    XSB's operators import fy 1050, export fx 1100, from xfx 1040): SCRIP has one predicate namespace, so a predicate an XSB system module exports is already visible and the
    directive succeeds having changed nothing. set_arg/3 is XSB machine's set_arg(Term, Index, Arg) :- term_set_arg(Term, Index, Arg, -1), the backtrackable destructive
    assignment (XSB syslib/machine.P; constraint_tests/testsetarg's ref shows the old argument back after a failure), which is SWI's setarg/3 with its arguments reordered. load_dyn/1 is XSB consult.P's load_dyn(File) :- load_dyn(File, 1, usermod): the file is
    found as named when the name has an extension, else as File.P, File.pl or File (search_module's order), each clause is asserted at the end (direction 1, z) and each
    directive is run; float_tests/assert_test reads its data file assert_test_old so (cfo 2026-10-24, CEO-1960). */
:- module(xsb_builtins, [ import/1, export/1, set_arg/3, load_dyn/1 ]).
import(_).
export(_).
set_arg(Term, Index, Arg) :- setarg(Index, Term, Arg).
load_dyn(File) :- '$xsb_ld_path'(File, Path), setup_call_cleanup(open(Path, read, S), '$xsb_ld_read'(S), close(S)).
'$xsb_ld_path'(F, P) :- ( var(F) -> throw(error(instantiation_error, load_dyn/1)) ; \+ atom(F) -> throw(error(type_error(atom, F), load_dyn/1)) ; true ),
    ( sub_atom(F, _, _, _, '.'), exists_file(F) -> P = F ; atom_concat(F, '.P', P), exists_file(P) -> true ; atom_concat(F, '.pl', P), exists_file(P) -> true ; exists_file(F) -> P = F
    ; throw(error(existence_error('file/module', F), load_dyn/1)) ).
'$xsb_ld_read'(S) :- repeat, read_term(S, T, []), ( T == end_of_file -> ! ; '$xsb_ld_one'(T), fail ).
'$xsb_ld_one'((:- G)) :- !, ( catch(G, _, true) -> true ; true ).
'$xsb_ld_one'(C) :- assertz(C).
