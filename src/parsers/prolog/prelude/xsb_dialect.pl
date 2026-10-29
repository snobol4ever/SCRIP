/*  xsb_dialect.pl -- XSB'S BUILTINS THAT ONLY THE XSB DIALECT HAS OR WHOSE NAMES SWI AND GNU ALSO DEFINE (hq_icon 2026-10-28, row prolog-xsbtests-to-100-the-58-failing-programs-ceo-1926;
    ARCH-PROLOG-DIALECTS.md sec 5, the conflicting builtins). SCRIP's own text, AGPL like the rest of SCRIP. The injector selects this library ONLY when the program is in the XSB dialect (prolog_parse.c
    pl_lib_dialect_ok: an XSB marker in its own clauses, or --dialect=xsb), so a SWI, GNU or ISO program never sees these names and keeps its own: xsb_builtins, selected in every dialect, keeps the no-op
    directives (import/1, export/1, index/1, compiler_options/1) that SWI programs in the corpus also call. do_all/1,2 are XSB's (cmplib/parse.P trans_goals): do_all(G) is (call(G), fail ; true), and
    do_all(Gen, Do) runs Do once for each DISTINCT, sorted binding of the variables Gen and Do share (findall, sort, member), do_all(Gen, if_none(Do, Else)) calls Else when Gen has no solution; index/2 and
    index/3 answer XSB's ':- index(Name/Arity, Arg).' hints, which change no answer. term_size/2 is XSB's term_sizew (emu/std_pred_xsb_i.h, measured on XSB 5.0.0): an atom, an integer, a variable or [] is 1,
    a float 4, a compound or list cell 1 plus the sizes of its arguments; library(terms)' term_size/2 counts SWI's cells and is shadowed when this library is selected (the generator's SHADOWS row;
    prolog_tests/test_term_size). number_digits/2 is XSB's number_chars/2 with the digit characters as the integers 0 to 9 and '-' '.' 'e' '+' as atoms (measured on XSB 5.0.0: number_digits(-23, L) gives
    [-,2,3], number_digits(N, [1,'2']) gives 12, [1,'.'] gives 1.0); invalid text raises syntax_error(List), an integer outside 0 to 9 type_error(digit, List), a non-list type_error(list, X), a non-number
    first argument type_error(number, X), a partial list instantiation_error (prolog_tests/n2c). compile/1,2 check that the file exists (as F, F.P or F.pl, load_dyn's search) and LOAD NOTHING: XSB's compile writes an object
    file that SCRIP has no use for, and the program's consult/1 of the same name loads the source (ARCH-PROLOG-DIALECTS sec 5; prolog_tests/altindex, inline). min_ed_dis/3 is the xeddis foreign module's Levenshtein distance between two atoms (measured on XSB 5.0.0: frog and dog are 2 apart,
    frog and antidisestablishmentarianism 27; foreign_tests/cinter1). import/1 is xsb_builtins' no-op directive plus XSB's renaming form ':- import member/2 from basics as mymember/2.', which defines mymember(A, B) :- member(A, B) (the generator's
    SHADOWS row removes xsb_builtins' own import/1 when this library is selected; foreign_tests/test_importas). is_acyclic/1 is XSB's name for ISO acyclic_term/1 (prolog_tests/large_arity_prolog).
    term_to_atom/2,3 and term_to_codes/2,3 are XSB's (syslib/string.P) and WRITE ONLY: the term is copied and written as codes, the default options ignore_ops(canonical), quoted(false), numbervars(false), so a
    compound is f(a,b) whatever its operators, a proper list [a,b], an unbound variable _h0, _h1, ... in order of first occurrence (a repeated variable keeps its number), a float as number_codes writes it and
    an atom as its text (quoted(true) quotes what XSB's if_not_need_quote refuses); SWI's term_to_atom/2 parses text, so it stays SWI's outside the XSB dialect (prolog_tests/large_arity_prolog). gc_dynamic/1 (syslib/standard.P: check_var, then the count of retracted clauses the delayed reclamation freed) answers 0, SCRIP frees a retracted clause at once so none is pending, and a bound argument is type_error(variable, N); XSB's own count is its storage's timing and no answer (CEO-1934, CEO-2012 (c)), so the retract_tests *_gc_* programs print it on lines the package mask replaces. retract_nr/1 is retract/1 (assert.P: the same retraction without reclaiming the clause; XSB prints an obsolete warning on stderr; retract_tests *_gc_*). */
:- module(xsb_dialect, [ index/2, index/3, do_all/1, do_all/2, term_size/2, number_digits/2, is_acyclic/1, term_to_atom/2, term_to_atom/3, term_to_codes/2, term_to_codes/3, compile/1, compile/2, min_ed_dis/3, gc_dynamic/1, retract_nr/1, import/1 ]).
gc_dynamic(N) :- ( var(N) -> N = 0 ; throw(error(type_error(variable, N), gc_dynamic/1)) ).
retract_nr(C) :- retract(C).
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
term_to_atom(T, A) :- term_to_codes(T, Cs), atom_codes(A, Cs).
term_to_atom(T, A, Opts) :- term_to_codes(T, Opts, Cs), atom_codes(A, Cs).
term_to_codes(T, Cs) :- term_to_codes(T, [ignore_ops(canonical)], Cs).
term_to_codes(T, Opts, Cs) :- '$xsb_ttc_opts'(Opts, opt(I, Q, N)), ( var(I) -> I = canonical ; true ), ( var(Q) -> Q = false ; true ), ( var(N) -> N = false ; true ),
    copy_term(T, T1), ( N == true -> numbervars(T1, 0, _) ; true ), '$xsb_ttc'(T1, opt(I, Q, N), 0, _, Cs, []).
'$xsb_ttc_opts'(Opts, _) :- var(Opts), !, throw(error(instantiation_error, term_to_atom/3)).
'$xsb_ttc_opts'([], _) :- !.
'$xsb_ttc_opts'([O|Os], S) :- !, '$xsb_ttc_opt'(O, S), '$xsb_ttc_opts'(Os, S).
'$xsb_ttc_opts'(Opts, _) :- throw(error(type_error(list, Opts), term_to_atom/3)).
'$xsb_ttc_opt'(ignore_ops(F), opt(F, _, _)) :- ( F == true ; F == canonical ), !.
'$xsb_ttc_opt'(quoted(F), opt(_, F, _)) :- ( F == true ; F == false ), !.
'$xsb_ttc_opt'(numbervars(F), opt(_, _, F)) :- ( F == true ; F == false ), !.
'$xsb_ttc_opt'(O, _) :- throw(error(domain_error(write_option, O), term_to_atom/3)).
'$xsb_ttc_var'(N, Cs0, Cs) :- number_codes(N, Ds), Cs0 = [95, 104|R], append(Ds, Cs, R).
'$xsb_ttc'(T, O, V0, V, Cs0, Cs) :- ( var(T) -> T = '$xsb_tv'(V0), V is V0 + 1, '$xsb_ttc_var'(V0, Cs0, Cs)
    ; T = '$xsb_tv'(N) -> V = V0, '$xsb_ttc_var'(N, Cs0, Cs)
    ; T = '$VAR'(N) -> '$xsb_ttc_dollar'(N, O, V0, V, Cs0, Cs)
    ; integer(T) -> V = V0, number_codes(T, Ds), append(Ds, Cs, Cs0)
    ; atom(T) -> V = V0, '$xsb_ttc_atom'(T, O, Cs0, Cs)
    ; float(T) -> V = V0, number_codes(T, Ds), append(Ds, Cs, Cs0)
    ; is_list(T) -> T = [H|Tl], '$xsb_ttc_list'(H, Tl, O, V0, V, Cs0, Cs)
    ; compound(T) -> T =.. [F0|As], ( F0 == '[|]' -> F = '.' ; F = F0 ), '$xsb_ttc_atom'(F, O, Cs0, [40|C1]), '$xsb_ttc_args'(As, O, V0, V, C1, Cs) ).
'$xsb_ttc_args'([], _, V, V, Cs0, Cs) :- Cs0 = [41|Cs].
'$xsb_ttc_args'([A|As], O, V0, V, Cs0, Cs) :- '$xsb_ttc'(A, O, V0, V1, Cs0, C1), '$xsb_ttc_rest'(As, O, V1, V, C1, Cs).
'$xsb_ttc_rest'([], _, V, V, Cs0, Cs) :- Cs0 = [41|Cs].
'$xsb_ttc_rest'([A|As], O, V0, V, [44|Cs0], Cs) :- '$xsb_ttc'(A, O, V0, V1, Cs0, C1), '$xsb_ttc_rest'(As, O, V1, V, C1, Cs).
'$xsb_ttc_list'(H, Tl, opt(canonical, Q, N), V0, V, [91|Cs0], Cs) :- !, '$xsb_ttc'(H, opt(canonical, Q, N), V0, V1, Cs0, C1), '$xsb_ttc_lrest'(Tl, opt(canonical, Q, N), V1, V, C1, Cs).
'$xsb_ttc_list'(H, Tl, O, V0, V, Cs0, Cs) :- O = opt(_, Q, _), ( Q == true -> Cs0 = [39, 46, 39, 40|C1] ; Cs0 = [46, 40|C1] ), '$xsb_ttc'(H, O, V0, V1, C1, C2), C2 = [44|C3], '$xsb_ttc'(Tl, O, V1, V, C3, [41|Cs]).
'$xsb_ttc_lrest'([], _, V, V, Cs0, Cs) :- Cs0 = [93|Cs].
'$xsb_ttc_lrest'([A|As], O, V0, V, [44|Cs0], Cs) :- '$xsb_ttc'(A, O, V0, V1, Cs0, C1), '$xsb_ttc_lrest'(As, O, V1, V, C1, Cs).
'$xsb_ttc_dollar'(N, O, V0, V, Cs0, Cs) :- O = opt(_, Q, Nv), ( Nv == true -> '$xsb_ttc_letters'(N, O, V0, V, Cs0, Cs)
    ; '$xsb_ttc'(N, O, V0, V, C1, [41|Cs]), ( Q == true -> append([39, 36, 86, 65, 82, 39, 40], C1, Cs0) ; append([36, 86, 65, 82, 40], C1, Cs0) ) ).
'$xsb_ttc_letters'(N, O, V0, V, Cs0, Cs) :- ( integer(N), N >= 0 -> V = V0, C is N mod 26 + 65, ( N >= 26 -> N1 is N // 26, Cs0 = [C|C1], '$xsb_ttc_letters'(N1, O, V0, _, C1, Cs) ; Cs0 = [C|Cs] )
    ; integer(N) -> V = V0, Cs0 = [95|Cs] ; atom(N) -> V = V0, atom_codes(N, Ac), append(Ac, Cs, Cs0)
    ; N = [_|_], '$xsb_ttc_numlist'(N) -> V = V0, append(N, Cs, Cs0) ; '$xsb_ttc'(N, O, V0, V, Cs0, Cs) ).
'$xsb_ttc_numlist'([]).
'$xsb_ttc_numlist'([H|T]) :- integer(H), '$xsb_ttc_numlist'(T).
'$xsb_ttc_atom'(A, opt(_, Q, _), Cs0, Cs) :- atom_codes(A, Ac), ( Q == true -> ( '$xsb_ttc_plain'(Ac) -> append(Ac, Cs, Cs0) ; Cs0 = [39|C1], '$xsb_ttc_dq'(Ac, C1, [39|Cs]) ) ; append(Ac, Cs, Cs0) ).
'$xsb_ttc_plain'([91, 93]) :- !.
'$xsb_ttc_plain'([C|Cs]) :- C >= 97, C =< 122, '$xsb_ttc_plain1'(Cs).
'$xsb_ttc_plain1'([]).
'$xsb_ttc_plain1'([C|Cs]) :- ( C >= 97, C =< 122 -> true ; C >= 65, C =< 90 -> true ; C =:= 95 -> true ; C >= 48, C =< 57 ), '$xsb_ttc_plain1'(Cs).
'$xsb_ttc_dq'([], Cs, Cs).
'$xsb_ttc_dq'([C|Cs], [C|Cs0], Cs1) :- ( C =:= 39 -> Cs0 = [39|Cs0a], '$xsb_ttc_dq'(Cs, Cs0a, Cs1) ; '$xsb_ttc_dq'(Cs, Cs0, Cs1) ).
compile(F) :- compile(F, []).
compile(F, Opts) :- ( var(F) -> throw(error(instantiation_error, compile/2)) ; \+ atom(F) -> throw(error(type_error(atom, F), compile/2)) ; true ),
    ( is_list(Opts) -> true ; throw(error(type_error(list, Opts), compile/2)) ), '$xsb_ld_path'(F, _).
min_ed_dis(S, T, D) :- atom_codes(S, Cs), atom_codes(T, Ds), length(Ds, N), numlist(0, N, Row0), '$xsb_med'(Cs, Ds, 1, Row0, Row), last(Row, D).
'$xsb_med'([], _, _, Row, Row).
'$xsb_med'([C|Cs], Ds, I, Prev, Row) :- '$xsb_med_row'(Ds, C, I, Prev, Cur), I1 is I + 1, '$xsb_med'(Cs, Ds, I1, Cur, Row).
'$xsb_med_row'(Ds, C, I, Prev, [I|Cur]) :- '$xsb_med_cells'(Ds, C, Prev, I, Cur).
'$xsb_med_cells'([], _, _, _, []).
'$xsb_med_cells'([D|Ds], C, [Diag, Up|Prev], Left, [X|Xs]) :- ( C =:= D -> Sub = Diag ; Sub is Diag + 1 ), U1 is Up + 1, L1 is Left + 1, X is min(Sub, min(U1, L1)), '$xsb_med_cells'(Ds, C, [Up|Prev], X, Xs).
import(as(from(Spec, _), Spec2)) :- !, '$xsb_import_as'(Spec, Spec2).
import(_).
'$xsb_import_as'(N/A, N2/A) :- functor(H1, N, A), H1 =.. [_|Args], H2 =.. [N2|Args], assertz((H2 :- H1)).
