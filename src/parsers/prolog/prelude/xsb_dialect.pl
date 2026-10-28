/*  xsb_dialect.pl -- XSB'S BUILTINS THAT ONLY THE XSB DIALECT HAS OR WHOSE NAMES SWI AND GNU ALSO DEFINE (hq_icon 2026-10-28, row prolog-xsbtests-to-100-the-58-failing-programs-ceo-1926;
    ARCH-PROLOG-DIALECTS.md sec 5, the conflicting builtins). SCRIP's own text, AGPL like the rest of SCRIP. The injector selects this library ONLY when the program is in the XSB dialect (prolog_parse.c
    pl_lib_dialect_ok: an XSB marker in its own clauses, or --dialect=xsb), so a SWI, GNU or ISO program never sees these names and keeps its own: xsb_builtins, selected in every dialect, keeps the no-op
    directives (import/1, export/1, index/1, compiler_options/1) that SWI programs in the corpus also call. do_all/1,2 are XSB's (cmplib/parse.P trans_goals): do_all(G) is (call(G), fail ; true), and
    do_all(Gen, Do) runs Do once for each DISTINCT, sorted binding of the variables Gen and Do share (findall, sort, member), do_all(Gen, if_none(Do, Else)) calls Else when Gen has no solution; index/2 and
    index/3 answer XSB's ':- index(Name/Arity, Arg).' hints, which change no answer. term_size/2 is XSB's term_sizew (emu/std_pred_xsb_i.h, measured on XSB 5.0.0): an atom, an integer, a variable or [] is 1,
    a float 4, a compound or list cell 1 plus the sizes of its arguments; library(terms)' term_size/2 counts SWI's cells and is shadowed when this library is selected (the generator's SHADOWS row;
    prolog_tests/test_term_size). number_digits/2 is XSB's number_chars/2 with the digit characters as the integers 0 to 9 and '-' '.' 'e' '+' as atoms (measured on XSB 5.0.0: number_digits(-23, L) gives
    [-,2,3], number_digits(N, [1,'2']) gives 12, [1,'.'] gives 1.0); invalid text raises syntax_error(List), an integer outside 0 to 9 type_error(digit, List), a non-list type_error(list, X), a non-number
    first argument type_error(number, X), a partial list instantiation_error (prolog_tests/n2c). is_acyclic/1 is XSB's name for ISO acyclic_term/1 (prolog_tests/large_arity_prolog). */
:- module(xsb_dialect, [ index/2, index/3, do_all/1, do_all/2, term_size/2, number_digits/2, is_acyclic/1 ]).
index(_, _).
index(_, _, _).
do_all(G) :- ( call(G), fail ; true ).
do_all(Gen, Do0) :- ( nonvar(Do0), Do0 = if_none(Do, Else) -> '$xsb_da_vars'(Gen, Do, Vs), findall(Vs, Gen, Vals), ( Vals == [] -> call(Else), fail ; '$xsb_da_each'(Vs, Vals, Do) )
    ; '$xsb_da_vars'(Gen, Do0, Vs), findall(Vs, Gen, Vals), '$xsb_da_each'(Vs, Vals, Do0) ).
do_all(_, _).
'$xsb_da_each'(Vs, Vals, Do) :- sort(Vals, Sorted), member(Vs, Sorted), call(Do), fail.
'$xsb_da_vars'(Gen, Do, Vs) :- term_variables(Gen, GV), term_variables(Do, DV), '$xsb_da_shared'(GV, DV, Vs).
'$xsb_da_shared'([], _, []).
'$xsb_da_shared'([V|Vs], DV, R) :- ( '$xsb_da_memq'(V, DV) -> R = [V|R1] ; R = R1 ), '$xsb_da_shared'(Vs, DV, R1).
'$xsb_da_memq'(V, [W|Ws]) :- ( V == W -> true ; '$xsb_da_memq'(V, Ws) ).
term_size(T, S) :- '$xsb_tsz'(T, 0, S).
'$xsb_tsz'(T, A0, A) :- ( var(T) -> A is A0 + 1 ; float(T) -> A is A0 + 4 ; compound(T) -> functor(T, _, N), A1 is A0 + 1, '$xsb_tsz_args'(1, N, T, A1, A) ; A is A0 + 1 ).
'$xsb_tsz_args'(I, N, T, A0, A) :- ( I > N -> A = A0 ; I =:= N -> arg(I, T, X), '$xsb_tsz'(X, A0, A) ; arg(I, T, X), '$xsb_tsz'(X, A0, A1), I1 is I + 1, '$xsb_tsz_args'(I1, N, T, A1, A) ).
number_digits(N, L) :- ( number(N) -> number_chars(N, Cs), '$xsb_nd_out'(Cs, Ds), L = Ds
    ; nonvar(N) -> throw(error(type_error(number, N), number_digits/2))
    ; var(L) -> throw(error(instantiation_error, number_digits/2))
    ; '$xsb_nd_in'(L, L, Cs0), '$xsb_nd_dot'(Cs0, Cs), catch(number_chars(N, Cs), error(syntax_error(_), _), throw(error(syntax_error(L), number_digits/2))) ).
'$xsb_nd_dot'(Cs0, Cs) :- ( append(_, ['.'], Cs0) -> append(Cs0, ['0'], Cs) ; Cs = Cs0 ).
'$xsb_nd_out'([], []).
'$xsb_nd_out'([C|Cs], [D|Ds]) :- ( char_type(C, digit(W)) -> D = W ; D = C ), '$xsb_nd_out'(Cs, Ds).
'$xsb_nd_in'(L, W, Cs) :- ( var(L) -> throw(error(instantiation_error, number_digits/2)) ; L == [] -> Cs = [] ; L = [D|Ds] -> '$xsb_nd_char'(D, W, C), Cs = [C|Cs1], '$xsb_nd_in'(Ds, W, Cs1)
    ; throw(error(type_error(list, W), number_digits/2)) ).
'$xsb_nd_char'(D, W, C) :- ( integer(D) -> ( D >= 0, D =< 9 -> number_codes(D, [K]), char_code(C, K) ; throw(error(type_error(digit, W), number_digits/2)) )
    ; atom(D), atom_length(D, 1) -> C = D ; throw(error(syntax_error(W), number_digits/2)) ).
is_acyclic(T) :- acyclic_term(T).
