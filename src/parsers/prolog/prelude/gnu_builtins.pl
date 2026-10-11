/*  gnu_builtins.pl -- GNU Prolog's own builtins that SWI-Prolog lacks, as a Prolog layer (cfo 2026-10-11, row prolog-gnu-builtins-
    the-drivers-find-128-predicates-gnu-defines-and-scrip-lacks).  SCRIP's own text, AGPL like the rest of SCRIP: not a vendored file.
    The generator (util_gen_prolog_prelude_libs.py) embeds it in LOCAL mode: no SWI reader, no renames, its helpers '$gnu_b_'-named by
    hand.  It is parsed only when a program names one of its exports.

    FAMILY 1, TERMS TO AND FROM TEXT (BipsPl/write.pl, read.pl, atom.pl, gprolog 1.6.0 measured 2026-10-11): write, writeq, print,
    display and write_canonical into an atom, a code list or a char list; write_term into the same three; format into the same three;
    read_from_atom/codes/chars; number_atom/2 (the argument order of number_codes); lower_upper/2 over one-character atoms (a code
    is a type_error(character, Code)); new_atom/1,2 (an atom no program has made, here by a counter).  Every one is the SWI primitive
    the GNU name stands for -- with_output_to/2 and read_term_from_atom/3 -- so the text each writes is SCRIP's write, writeq, print,
    display and write_canonical, which already answer as gprolog's (print unquoted, display and write_canonical in dotted canonical
    form).  NAMED GAPS, later families of the row: read_token/1,2 and read_token_from_*, read_atom/integer/number (a tokenizer), the
    text streams (open_input_atom_stream and kin), the clocks and the OS, the debugger and the console.
*/
:- module(gnu_builtins, [ write_to_atom/2, writeq_to_atom/2, print_to_atom/2, display_to_atom/2, write_canonical_to_atom/2,
                          write_to_codes/2, writeq_to_codes/2, print_to_codes/2, display_to_codes/2, write_canonical_to_codes/2,
                          write_to_chars/2, writeq_to_chars/2, print_to_chars/2, display_to_chars/2, write_canonical_to_chars/2,
                          write_term_to_atom/3, write_term_to_codes/3, write_term_to_chars/3,
                          format_to_atom/3, format_to_codes/3, format_to_chars/3,
                          read_from_atom/2, read_from_codes/2, read_from_chars/2,
                          number_atom/2, lower_upper/2, new_atom/1, new_atom/2,
                          portray_clause/1, portray_clause/2, name_singleton_vars/1, name_query_vars/2, bind_variables/2 ]).
write_to_atom(A, T) :- with_output_to(atom(A), write(T)).
writeq_to_atom(A, T) :- with_output_to(atom(A), writeq(T)).
print_to_atom(A, T) :- with_output_to(atom(A), print(T)).
display_to_atom(A, T) :- with_output_to(atom(A), display(T)).
write_canonical_to_atom(A, T) :- with_output_to(atom(A), write_canonical(T)).
write_to_codes(C, T) :- with_output_to(codes(C), write(T)).
writeq_to_codes(C, T) :- with_output_to(codes(C), writeq(T)).
print_to_codes(C, T) :- with_output_to(codes(C), print(T)).
display_to_codes(C, T) :- with_output_to(codes(C), display(T)).
write_canonical_to_codes(C, T) :- with_output_to(codes(C), write_canonical(T)).
write_to_chars(C, T) :- with_output_to(chars(C), write(T)).
writeq_to_chars(C, T) :- with_output_to(chars(C), writeq(T)).
print_to_chars(C, T) :- with_output_to(chars(C), print(T)).
display_to_chars(C, T) :- with_output_to(chars(C), display(T)).
write_canonical_to_chars(C, T) :- with_output_to(chars(C), write_canonical(T)).
write_term_to_atom(A, T, O) :- with_output_to(atom(A), write_term(T, O)).
write_term_to_codes(C, T, O) :- with_output_to(codes(C), write_term(T, O)).
write_term_to_chars(C, T, O) :- with_output_to(chars(C), write_term(T, O)).
format_to_atom(A, F, Args) :- with_output_to(atom(A), format(F, Args)).
format_to_codes(C, F, Args) :- with_output_to(codes(C), format(F, Args)).
format_to_chars(C, F, Args) :- with_output_to(chars(C), format(F, Args)).
read_from_atom(A, T) :- read_term_from_atom(A, T, []).
read_from_codes(C, T) :- atom_codes(A, C), read_term_from_atom(A, T, []).
read_from_chars(C, T) :- atom_chars(A, C), read_term_from_atom(A, T, []).
number_atom(N, A) :- ( var(A) -> number_codes(N, C), atom_codes(A, C) ; atom_codes(A, C), number_codes(N, C) ).
lower_upper(L, U) :- nonvar(L), !, '$gnu_b_char'(L), upcase_atom(L, U0), U = U0.
lower_upper(L, U) :- '$gnu_b_char'(U), downcase_atom(U, L0), L = L0.
'$gnu_b_char'(C) :- var(C), !, throw(error(instantiation_error, lower_upper/2)).
'$gnu_b_char'(C) :- atom(C), atom_length(C, 1), !.
'$gnu_b_char'(C) :- throw(error(type_error(character, C), lower_upper/2)).
new_atom(A) :- new_atom('$atom_', A).
new_atom(P, A) :- ( nb_current('$gnu_b_new_atom', N0) -> true ; N0 = 0 ), N is N0 + 1, nb_setval('$gnu_b_new_atom', N), atom_number(S, N), atom_concat(P, S, A).
portray_clause(T) :- current_output(S), '$gnu_b_portray'(S, T, portray_clause/1).
portray_clause(S, T) :- '$gnu_b_portray'(S, T, portray_clause/2).
'$gnu_b_portray'(_, T, PI) :- var(T), !, throw(error(instantiation_error, PI)).
'$gnu_b_portray'(_, T, PI) :- \+ callable(T), !, throw(error(type_error(callable, T), PI)).
'$gnu_b_portray'(S, T, _) :- \+ \+ ( '$gnu_b_lit'(T, T1, [], Rs), '$gnu_b_singles'(T1), '$gnu_b_number'(T1, 0, Rs, _, numbervars), '$gnu_b_pc'(S, T1) ).
'$gnu_b_lit'(T, T, Rs, Rs) :- var(T), !.
'$gnu_b_lit'('$VAR'(N), '$VARNAME'(A), Rs, [N|Rs]) :- integer(N), !, with_output_to(atom(A), write_term('$VAR'(N), [quoted(true), numbervars(false)])).
'$gnu_b_lit'(T, T1, Rs0, Rs) :- compound(T), !, T =.. [F|As], '$gnu_b_lits'(As, As1, Rs0, Rs), T1 =.. [F|As1].
'$gnu_b_lit'(T, T, Rs, Rs).
'$gnu_b_lits'([], [], Rs, Rs).
'$gnu_b_lits'([A|As], [B|Bs], Rs0, Rs) :- '$gnu_b_lit'(A, B, Rs0, Rs1), '$gnu_b_lits'(As, Bs, Rs1, Rs).
'$gnu_b_pc'(S, (H :- B)) :- !, '$gnu_b_wt'(S, H, 1199), ( B == true -> true ; write(S, ' :-'), '$gnu_b_nl'(S, 0, 0' ), '$gnu_b_body'(S, 0, 0, B) ), '$gnu_b_stop'(S).
'$gnu_b_pc'(S, (H --> B)) :- !, '$gnu_b_wt'(S, H, 1199), ( B == true -> true ; write(S, ' -->'), '$gnu_b_nl'(S, 0, 0' ), '$gnu_b_body'(S, 0, 0, B) ), '$gnu_b_stop'(S).
'$gnu_b_pc'(S, (:- B)) :- !, write(S, ':-'), put_char(S, '\t'), '$gnu_b_body'(S, 0, 0, B), '$gnu_b_stop'(S).
'$gnu_b_pc'(S, T) :- '$gnu_b_wt'(S, T, 1200), '$gnu_b_stop'(S).
'$gnu_b_stop'(S) :- write(S, '.'), nl(S).
'$gnu_b_wt'(S, T, P) :- write_term(S, T, [quoted(true), numbervars(true), space_args(true), priority(P)]).
'$gnu_b_body'(S, L, _, (A, B)) :- !, '$gnu_b_body'(S, L, 1, A), write(S, ','), ( '$gnu_b_is_cut'(B) -> write(S, ' ') ; '$gnu_b_nl'(S, L, 0' ) ), '$gnu_b_body'(S, L, 2, B).
'$gnu_b_body'(S, L, C, (A ; B)) :- !, ( C =\= 4 -> write(S, '(   '), L1 is L + 1 ; L1 = L ), '$gnu_b_body'(S, L1, 3, A), '$gnu_b_nl'(S, L1, 0';), '$gnu_b_body'(S, L1, 4, B),
    ( C =\= 4 -> L0 is L1 - 1, '$gnu_b_nl'(S, L0, 0' ), write(S, ')') ; true ).
'$gnu_b_body'(S, L, C, (A -> B)) :- !, '$gnu_b_if'(S, L, C, A, B, ' ->').
'$gnu_b_body'(S, L, C, (A *-> B)) :- !, '$gnu_b_if'(S, L, C, A, B, ' *-> ').
'$gnu_b_body'(S, _, C, G) :- '$gnu_b_prec'(C, P), '$gnu_b_wt'(S, G, P).
'$gnu_b_if'(S, L, C, A, B, Arrow) :- ( C =\= 3, C =\= 4 -> write(S, '(   '), L1 is L + 1 ; L1 = L ), '$gnu_b_body'(S, L1, 5, A), write(S, Arrow), '$gnu_b_nl'(S, L1, 0' ),
    '$gnu_b_body'(S, L1, 6, B), ( C =\= 3, C =\= 4 -> L0 is L1 - 1, '$gnu_b_nl'(S, L0, 0' ), write(S, ')') ; true ).
'$gnu_b_is_cut'(B) :- nonvar(B), B = (X, _), !, '$gnu_b_is_cut'(X).
'$gnu_b_is_cut'(B) :- B == !.
'$gnu_b_prec'(0, 1199).
'$gnu_b_prec'(1, 999).
'$gnu_b_prec'(2, 1000).
'$gnu_b_prec'(3, 1099).
'$gnu_b_prec'(4, 1100).
'$gnu_b_prec'(5, 1049).
'$gnu_b_prec'(6, 1050).
'$gnu_b_nl'(S, L, Ch) :- nl(S), put_char(S, '\t'), N is 4 * (L - 1), '$gnu_b_spaces'(S, N), ( L =\= 0 -> put_code(S, Ch), write(S, '   ') ; true ).
'$gnu_b_spaces'(S, N) :- ( N > 0 -> put_char(S, ' '), N1 is N - 1, '$gnu_b_spaces'(S, N1) ; true ).
name_singleton_vars(T) :- '$gnu_b_singles'(T).
'$gnu_b_singles'(T) :- ( acyclic_term(T) -> '$gnu_b_occ'(T, [], Occ), term_variables(T, Vs), '$gnu_b_bind_single'(Vs, Occ) ; true ).
'$gnu_b_occ'(T, O, [T|O]) :- var(T), !.
'$gnu_b_occ'(T, O0, O) :- compound(T), !, T =.. [_|As], '$gnu_b_occs'(As, O0, O).
'$gnu_b_occ'(_, O, O).
'$gnu_b_occs'([], O, O).
'$gnu_b_occs'([A|As], O0, O) :- '$gnu_b_occ'(A, O0, O1), '$gnu_b_occs'(As, O1, O).
'$gnu_b_bind_single'([], _).
'$gnu_b_bind_single'([V|Vs], Occ) :- '$gnu_b_count'(Occ, V, 0, N), ( N =:= 1 -> V = '$VARNAME'('_') ; true ), '$gnu_b_bind_single'(Vs, Occ).
'$gnu_b_count'([], _, N, N).
'$gnu_b_count'([X|Xs], V, N0, N) :- ( X == V -> N1 is N0 + 1 ; N1 = N0 ), '$gnu_b_count'(Xs, V, N1, N).
name_query_vars(L, _) :- var(L), !, throw(error(instantiation_error, name_query_vars/2)).
name_query_vars(L, Rest) :- '$gnu_b_nqv'(L, Rest).
'$gnu_b_nqv'([], []).
'$gnu_b_nqv'([N=V|L], Rest) :- !, ( var(V) -> V = '$VARNAME'(N), Rest = Rest1 ; Rest = [N=V|Rest1] ), '$gnu_b_nqv'(L, Rest1).
'$gnu_b_nqv'([_|L], Rest) :- '$gnu_b_nqv'(L, Rest).
bind_variables(_, Os) :- var(Os), !, throw(error(instantiation_error, bind_variables/2)).
bind_variables(T, Os) :- '$gnu_b_bv_opts'(Os, [], Ex, 0, From, _, Next, numbervars, Mode), '$gnu_b_reserved'(Ex, [], Rs), '$gnu_b_number'(T, From, Rs, Next0, Mode), Next = Next0.
'$gnu_b_bv_opts'(Os, _, _, _, _, _, _, _, _) :- var(Os), !, throw(error(instantiation_error, bind_variables/2)).
'$gnu_b_bv_opts'([], Ex, Ex, F, F, N, N, M, M) :- !.
'$gnu_b_bv_opts'([O|Os], Ex0, Ex, F0, F, N0, N, M0, M) :- !, '$gnu_b_bv_opt'(O, Ex0, Ex1, F0, F1, N0, N1, M0, M1), '$gnu_b_bv_opts'(Os, Ex1, Ex, F1, F, N1, N, M1, M).
'$gnu_b_bv_opts'(Os, _, _, _, _, _, _, _, _) :- throw(error(type_error(list, Os), bind_variables/2)).
'$gnu_b_bv_opt'(O, _, _, _, _, _, _, _, _) :- var(O), !, throw(error(instantiation_error, bind_variables/2)).
'$gnu_b_bv_opt'(exclude(Ex), _, Ex, F, F, N, N, M, M) :- !.
'$gnu_b_bv_opt'(from(F), Ex, Ex, _, F, N, N, M, M) :- integer(F), !.
'$gnu_b_bv_opt'(next(N), Ex, Ex, F, F, _, N, M, M) :- ( var(N) ; integer(N) ), !.
'$gnu_b_bv_opt'(numbervars, Ex, Ex, F, F, N, N, _, numbervars) :- !.
'$gnu_b_bv_opt'(namevars, Ex, Ex, F, F, N, N, _, namevars) :- !.
'$gnu_b_bv_opt'(O, _, _, _, _, _, _, _, _) :- throw(error(domain_error(var_binding_option, O), bind_variables/2)).
'$gnu_b_reserved'(T, Rs, Rs) :- var(T), !.
'$gnu_b_reserved'('$VAR'(N), Rs, [N|Rs]) :- integer(N), !.
'$gnu_b_reserved'(T, Rs0, Rs) :- compound(T), !, T =.. [_|As], '$gnu_b_reserveds'(As, Rs0, Rs).
'$gnu_b_reserved'(_, Rs, Rs).
'$gnu_b_reserveds'([], Rs, Rs).
'$gnu_b_reserveds'([A|As], Rs0, Rs) :- '$gnu_b_reserved'(A, Rs0, Rs1), '$gnu_b_reserveds'(As, Rs1, Rs).
'$gnu_b_number'(T, From, Rs, Next, Mode) :- term_variables(T, Vs), '$gnu_b_bindn'(Vs, From, Rs, Next, Mode).
'$gnu_b_bindn'([], N, _, N, _).
'$gnu_b_bindn'([V|Vs], N0, Rs, N, Mode) :- '$gnu_b_free'(N0, Rs, K), ( Mode == namevars -> '$gnu_b_letter'(K, A), V = '$VARNAME'(A) ; V = '$VAR'(K) ), K1 is K + 1,
    '$gnu_b_bindn'(Vs, K1, Rs, N, Mode).
'$gnu_b_free'(N0, Rs, K) :- ( memberchk(N0, Rs) -> N1 is N0 + 1, '$gnu_b_free'(N1, Rs, K) ; K = N0 ).
'$gnu_b_letter'(K, A) :- L is K mod 26, S is K // 26, C is 0'A + L, ( S =:= 0 -> atom_codes(A, [C]) ; number_codes(S, Ds), atom_codes(A, [C|Ds]) ).
