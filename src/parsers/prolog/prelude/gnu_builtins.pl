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
                          portray_clause/1, portray_clause/2, name_singleton_vars/1, name_query_vars/2, bind_variables/2,
                          partial_list/1, list_or_partial_list/1, generic_var/1, non_generic_var/1, non_fd_var/1, current_atom/1, atom_property/2,
                          call_with_args/1, call_with_args/2, call_with_args/3, call_with_args/4, call_with_args/5, call_with_args/6, call_with_args/7,
                          call_with_args/8, call_with_args/9, call_with_args/10, call_with_args/11, call_det/2, setarg/4, term_variables/3,
                          term_hash/4, term_ref/2,
                          shell/1, shell/2, system/1, system/2, spawn/2, spawn/3, unlink/1, rename_file/2, delete_directory/1, copy_file/2,
                          file_permission/2, sleep/1, host_name/1, os_version/1, architecture/1, date_time/1, send_signal/2, temporary_name/2,
                          temporary_file/3, is_absolute_file_name/1, cpu_time/1, real_time/1, user_time/1, system_time/1,
                          read_token_from_atom/2, read_token_from_chars/2, read_token_from_codes/2, get_print_stream/1,
                          popen/3, exec/4, exec/5, wait/2, create_pipe/2, set_bip_name/2, current_bip_name/2, syntax_error_info/4,
                          read_token/1, read_token/2, read_atom/1, read_atom/2, read_integer/1, read_integer/2, read_number/1, read_number/2,
                          get_key/1, get_key/2, get_key_no_echo/1, get_key_no_echo/2,
                          get_linedit_prompt/1, set_linedit_prompt/1, add_linedit_completion/1, find_linedit_completion/2,
                          leash/1, spy/1, nospy/1, nospyall/0, spypoint_condition/3, debugging/0, notrace/0,
                          socket/2, socket_close/1, socket_bind/2, socket_listen/2, socket_accept/3, socket_accept/4, socket_connect/4, hostname_address/2,
                          open_input_atom_stream/2, open_input_chars_stream/2, open_input_codes_stream/2, open_output_atom_stream/1, open_output_chars_stream/1,
                          open_output_codes_stream/1, close_input_atom_stream/1, close_input_chars_stream/1, close_input_codes_stream/1, close_output_atom_stream/2,
                          close_output_chars_stream/2, close_output_codes_stream/2, add_stream_alias/2, current_alias/2, add_stream_mirror/2, remove_stream_mirror/2,
                          current_mirror/2, current_stream/1,
                          set_stream_type/2, set_stream_eof_action/2, set_stream_buffering/2, stream_position/2, stream_line_column/3, set_stream_line_column/3,
                          seek/4 ]).
write_to_atom(A, T) :- '$gnu_b_oa'(A, write_to_atom/2), with_output_to(atom(A), write(T)).
writeq_to_atom(A, T) :- '$gnu_b_oa'(A, writeq_to_atom/2), with_output_to(atom(A), writeq(T)).
print_to_atom(A, T) :- '$gnu_b_oa'(A, print_to_atom/2), with_output_to(atom(A), print(T)).
display_to_atom(A, T) :- '$gnu_b_oa'(A, display_to_atom/2), with_output_to(atom(A), display(T)).
write_canonical_to_atom(A, T) :- '$gnu_b_oa'(A, write_canonical_to_atom/2), with_output_to(atom(A), write_canonical(T)).
write_to_codes(C, T) :- '$gnu_b_ol'(C, write_to_codes/2), with_output_to(codes(C), write(T)).
writeq_to_codes(C, T) :- '$gnu_b_ol'(C, writeq_to_codes/2), with_output_to(codes(C), writeq(T)).
print_to_codes(C, T) :- '$gnu_b_ol'(C, print_to_codes/2), with_output_to(codes(C), print(T)).
display_to_codes(C, T) :- '$gnu_b_ol'(C, display_to_codes/2), with_output_to(codes(C), display(T)).
write_canonical_to_codes(C, T) :- '$gnu_b_ol'(C, write_canonical_to_codes/2), with_output_to(codes(C), write_canonical(T)).
write_to_chars(C, T) :- '$gnu_b_ol'(C, write_to_chars/2), with_output_to(chars(C), write(T)).
writeq_to_chars(C, T) :- '$gnu_b_ol'(C, writeq_to_chars/2), with_output_to(chars(C), writeq(T)).
print_to_chars(C, T) :- '$gnu_b_ol'(C, print_to_chars/2), with_output_to(chars(C), print(T)).
display_to_chars(C, T) :- '$gnu_b_ol'(C, display_to_chars/2), with_output_to(chars(C), display(T)).
write_canonical_to_chars(C, T) :- '$gnu_b_ol'(C, write_canonical_to_chars/2), with_output_to(chars(C), write_canonical(T)).
write_term_to_atom(A, T, O) :- '$gnu_b_oa'(A, write_term_to_atom/3), with_output_to(atom(A), write_term(T, O)).
write_term_to_codes(C, T, O) :- '$gnu_b_ol'(C, write_term_to_codes/3), with_output_to(codes(C), write_term(T, O)).
write_term_to_chars(C, T, O) :- '$gnu_b_ol'(C, write_term_to_chars/3), with_output_to(chars(C), write_term(T, O)).
format_to_atom(A, F, Args) :- '$gnu_b_oa'(A, format_to_atom/3), with_output_to(atom(A), format(F, Args)).
format_to_codes(C, F, Args) :- '$gnu_b_ol'(C, format_to_codes/3), with_output_to(codes(C), format(F, Args)).
format_to_chars(C, F, Args) :- '$gnu_b_ol'(C, format_to_chars/3), with_output_to(chars(C), format(F, Args)).
read_from_atom(A, T) :- ( var(A) -> throw(error(instantiation_error, read_from_atom/2)) ; '$gnu_b_read'(A, T, read_from_atom/2) ).
read_from_codes(C, T) :- atom_codes(A, C), '$gnu_b_read'(A, T, read_from_codes/2).
read_from_chars(C, T) :- atom_chars(A, C), '$gnu_b_read'(A, T, read_from_chars/2).
'$gnu_b_oa'(A, _) :- ( var(A) ; atom(A) ), !.
'$gnu_b_oa'(A, PI) :- throw(error(type_error(atom, A), PI)).
'$gnu_b_ol'(L, _) :- '$gnu_b_pl'(L), !.
'$gnu_b_ol'(L, PI) :- throw(error(type_error(list, L), PI)).
'$gnu_b_pl'(L) :- var(L), !.
'$gnu_b_pl'([]).
'$gnu_b_pl'([_|T]) :- '$gnu_b_pl'(T).
'$gnu_b_read'(A, T, _) :- atom_codes(A, Cs), '$gnu_b_blank'(Cs), !, T = end_of_file.
'$gnu_b_read'(A, T, PI) :- '$gnu_os'(ts_in, A, S), current_input(I), set_input(S), catch(read(T0), E, true), set_input(I), close_input_atom_stream(S), ( var(E) -> T = T0 ; E = error(F, _) -> throw(error(F, PI)) ; throw(E) ).
'$gnu_b_blank'([]).
'$gnu_b_blank'([C|Cs]) :- code_type(C, space), '$gnu_b_blank'(Cs).
number_atom(N, A) :- ( var(A) -> number_codes(N, C), atom_codes(A, C) ; atom_codes(A, C), number_codes(N, C) ).
lower_upper(L, U) :- nonvar(L), !, '$gnu_b_char'(L), upcase_atom(L, U0), U = U0.
lower_upper(L, U) :- '$gnu_b_char'(U), downcase_atom(U, L0), L = L0.
'$gnu_b_char'(C) :- var(C), !, throw(error(instantiation_error, lower_upper/2)).
'$gnu_b_char'(C) :- atom(C), atom_length(C, 1), !.
'$gnu_b_char'(C) :- throw(error(type_error(character, C), lower_upper/2)).
new_atom(A) :- nonvar(A), !, throw(error(uninstantiation_error(A), new_atom/1)).
new_atom(A) :- new_atom('$atom_', A).
new_atom(_, A) :- nonvar(A), !, throw(error(uninstantiation_error(A), new_atom/2)).
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
partial_list(L) :- '$gnu_b_tail'(L, T), var(T).
list_or_partial_list(L) :- '$gnu_b_tail'(L, T), ( var(T) -> true ; T == [] ).
'$gnu_b_tail'(L, T) :- ( nonvar(L), L = [_|R] -> '$gnu_b_tail'(R, T) ; T = L ).
generic_var(X) :- var(X).
non_generic_var(X) :- nonvar(X).
non_fd_var(X) :- \+ ( var(X), get_attr(X, clpfd, _) ).
current_atom(A) :- atom(A).
atom_property(A, _) :- var(A), !, throw(error(instantiation_error, atom_property/2)).
atom_property(A, _) :- \+ atom(A), !, throw(error(type_error(atom, A), atom_property/2)).
atom_property(A, P) :- nonvar(P), \+ '$gnu_b_aprop'(P), !, throw(error(domain_error(atom_property, P), atom_property/2)).
atom_property(A, length(N)) :- atom_length(A, N).
atom_property(A, prefix_op) :- '$gnu_b_op'(A, [fx, fy]).
atom_property(A, infix_op) :- '$gnu_b_op'(A, [xfx, xfy, yfx]).
atom_property(A, postfix_op) :- '$gnu_b_op'(A, [xf, yf]).
atom_property(A, needs_quotes) :- with_output_to(atom(Q), writeq(A)), Q \== A.
atom_property(A, hash(H)) :- term_hash(A, H).
'$gnu_b_aprop'(length(_)).
'$gnu_b_aprop'(prefix_op).
'$gnu_b_aprop'(infix_op).
'$gnu_b_aprop'(postfix_op).
'$gnu_b_aprop'(needs_quotes).
'$gnu_b_aprop'(hash(_)).
'$gnu_b_op'(A, Ts) :- current_op(_, T, A), memberchk(T, Ts), !.
call_with_args(P) :- '$gnu_b_cwa'(P, [], 1).
call_with_args(P, A1) :- '$gnu_b_cwa'(P, [A1], 2).
call_with_args(P, A1, A2) :- '$gnu_b_cwa'(P, [A1, A2], 3).
call_with_args(P, A1, A2, A3) :- '$gnu_b_cwa'(P, [A1, A2, A3], 4).
call_with_args(P, A1, A2, A3, A4) :- '$gnu_b_cwa'(P, [A1, A2, A3, A4], 5).
call_with_args(P, A1, A2, A3, A4, A5) :- '$gnu_b_cwa'(P, [A1, A2, A3, A4, A5], 6).
call_with_args(P, A1, A2, A3, A4, A5, A6) :- '$gnu_b_cwa'(P, [A1, A2, A3, A4, A5, A6], 7).
call_with_args(P, A1, A2, A3, A4, A5, A6, A7) :- '$gnu_b_cwa'(P, [A1, A2, A3, A4, A5, A6, A7], 8).
call_with_args(P, A1, A2, A3, A4, A5, A6, A7, A8) :- '$gnu_b_cwa'(P, [A1, A2, A3, A4, A5, A6, A7, A8], 9).
call_with_args(P, A1, A2, A3, A4, A5, A6, A7, A8, A9) :- '$gnu_b_cwa'(P, [A1, A2, A3, A4, A5, A6, A7, A8, A9], 10).
call_with_args(P, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10) :- '$gnu_b_cwa'(P, [A1, A2, A3, A4, A5, A6, A7, A8, A9, A10], 11).
'$gnu_b_cwa'(P, _, N) :- var(P), !, throw(error(instantiation_error, call_with_args/N)).
'$gnu_b_cwa'(P, _, N) :- \+ atom(P), !, throw(error(type_error(atom, P), call_with_args/N)).
'$gnu_b_cwa'(P, As, _) :- G =.. [P|As], call(G).
call_det(G, _) :- var(G), !, throw(error(instantiation_error, call_det/2)).
call_det(_, D) :- nonvar(D), D \== true, D \== false, !, throw(error(type_error(boolean, D), call_det/2)).
call_det(G, D) :- call_cleanup(G, Det = true), ( Det == true -> D = true ; D = false ).
setarg(_, _, _, U) :- var(U), !, throw(error(instantiation_error, setarg/4)).
setarg(_, _, _, U) :- U \== true, U \== false, !, throw(error(type_error(boolean, U), setarg/4)).
setarg(N, T, V, true) :- setarg(N, T, V).
setarg(_, _, V, false) :- \+ atomic(V), !, throw(error(type_error(atomic, V), setarg/4)).
setarg(N, T, V, false) :- nb_setarg(N, T, V).
term_variables(T, Vs, Tail) :- term_variables(T, Vs0), append(Vs0, Tail, Vs).
term_hash(_, D, _, _) :- var(D), !, throw(error(instantiation_error, term_hash/4)).
term_hash(_, D, _, _) :- \+ integer(D), !, throw(error(type_error(integer, D), term_hash/4)).
term_hash(_, _, R, _) :- var(R), !, throw(error(instantiation_error, term_hash/4)).
term_hash(_, _, R, _) :- \+ integer(R), !, throw(error(type_error(integer, R), term_hash/4)).
term_hash(_, _, R, _) :- R < 0, !, throw(error(domain_error(not_less_than_zero, R), term_hash/4)).
term_hash(T, _, R, H) :- ( ground(T) -> term_hash(T, H0), ( R =:= 0 -> H = H0 ; H is H0 mod R ) ; true ).
term_ref(T, R) :- var(R), !, ( var(T) -> throw(error(instantiation_error, term_ref/2)) ; '$gnu_b_tref_add'(T, R) ).
term_ref(_, R) :- \+ integer(R), !, throw(error(type_error(integer, R), term_ref/2)).
term_ref(_, R) :- R < 0, !, throw(error(domain_error(not_less_than_zero, R), term_ref/2)).
term_ref(T, R) :- '$gnu_b_tref_key'(R, K), b_getval(K, T0), T = T0.
'$gnu_b_tref_key'(R, K) :- atom_number(A, R), atom_concat('$gnu_b_tref_', A, K).
'$gnu_b_tref_add'(T, R) :- ( nb_current('$gnu_b_tref_n', N0) -> true ; N0 = 0 ), R is N0 + 1, nb_setval('$gnu_b_tref_n', R), '$gnu_b_tref_key'(R, K), b_setval(K, T).
shell(C) :- shell(C, 0).
shell(C, S) :- '$gnu_b_cmd'(C, shell/2), '$gnu_os'(shell, C, S0), S = S0.
system(C) :- system(C, 0).
system(C, S) :- '$gnu_b_cmd'(C, system/2), '$gnu_os'(system, C, S0), S = S0.
'$gnu_b_cmd'(C, PI) :- var(C), !, throw(error(instantiation_error, PI)).
'$gnu_b_cmd'(C, PI) :- \+ atom(C), !, throw(error(type_error(atom, C), PI)).
'$gnu_b_cmd'(_, _).
spawn(Cmd, Args) :- spawn(Cmd, Args, 0).
spawn(Cmd, _, _) :- var(Cmd), !, throw(error(instantiation_error, spawn/3)).
spawn(_, Args, _) :- var(Args), !, throw(error(instantiation_error, spawn/3)).
spawn(Cmd, Args, S) :- '$gnu_b_quote'(Cmd, Q), '$gnu_b_quotes'(Args, Qs), atomic_list_concat(['exec', Q|Qs], ' ', Line), '$gnu_os'(shell, Line, S0), S = S0.
popen(C, M, S) :- '$gnu_b_cmd'(C, popen/3), '$gnu_b_io_mode'(M, F), '$gnu_os'(popen, C-F, S0), S = S0.
'$gnu_b_io_mode'(M, _) :- var(M), !, throw(error(instantiation_error, popen/3)).
'$gnu_b_io_mode'(read, r) :- !.
'$gnu_b_io_mode'(write, w) :- !.
'$gnu_b_io_mode'(M, _) :- atom(M), !, throw(error(domain_error(io_mode, M), popen/3)).
'$gnu_b_io_mode'(M, _) :- throw(error(type_error(atom, M), popen/3)).
exec(C, I, O, E, P) :- '$gnu_b_cmd'(C, exec/5), '$gnu_os'(exec, C, s(I0, O0, E0, P0)), I = I0, O = O0, E = E0, P = P0.
exec(C, I, O, E) :- '$gnu_b_cmd'(C, exec/4), '$gnu_os'(exec, C, s(I0, O0, E0, _)), I = I0, O = O0, E = E0.
wait(P, _) :- var(P), !, throw(error(instantiation_error, wait/2)).
wait(P, _) :- \+ integer(P), !, throw(error(type_error(integer, P), wait/2)).
wait(P, S) :- '$gnu_os'(wait, P, S0), S = S0.
create_pipe(R, W) :- pipe(R, W).
'$gnu_b_quotes'([], []).
'$gnu_b_quotes'([A|As], [Q|Qs]) :- '$gnu_b_quote'(A, Q), '$gnu_b_quotes'(As, Qs).
'$gnu_b_quote'(A, Q) :- atom_codes(A, Cs), '$gnu_b_qcodes'(Cs, Qs), append([39|Qs], [39], QC), atom_codes(Q, QC).
'$gnu_b_qcodes'([], []).
'$gnu_b_qcodes'([39|Cs], Qs) :- !, append([39, 92, 39, 39], Qs1, Qs), '$gnu_b_qcodes'(Cs, Qs1).
'$gnu_b_qcodes'([C|Cs], [C|Qs]) :- '$gnu_b_qcodes'(Cs, Qs).
unlink(P) :- var(P), !, throw(error(instantiation_error, unlink/1)).
unlink(P) :- catch('$gnu_os'(unlink, P, _), error(system_error(_), _), true).
delete_directory(P) :- '$gnu_os'(rmdir, P, _).
rename_file(F, T) :- '$gnu_os'(rename, F, T).
copy_file(F, T) :- '$gnu_os'(copy, F, T).
file_permission(_, M) :- var(M), !, throw(error(instantiation_error, file_permission/2)).
file_permission(P, M) :- '$gnu_b_perm'(M, 0, Mask), '$gnu_os'(access, P, Mask).
'$gnu_b_perm'([], K, K) :- !.
'$gnu_b_perm'([M|Ms], K0, K) :- !, '$gnu_b_perm1'(M, B), K1 is K0 \/ B, '$gnu_b_perm'(Ms, K1, K).
'$gnu_b_perm'(M, K0, K) :- '$gnu_b_perm1'(M, B), K is K0 \/ B.
'$gnu_b_perm1'(M, _) :- var(M), !, throw(error(instantiation_error, file_permission/2)).
'$gnu_b_perm1'(read, 4) :- !.
'$gnu_b_perm1'(write, 2) :- !.
'$gnu_b_perm1'(execute, 1) :- !.
'$gnu_b_perm1'(search, 1) :- !.
'$gnu_b_perm1'(M, _) :- throw(error(domain_error(os_file_permission, M), file_permission/2)).
sleep(S) :- var(S), !, throw(error(instantiation_error, sleep/1)).
sleep(S) :- \+ number(S), !, throw(error(type_error(number, S), sleep/1)).
sleep(S) :- S < 0, !, throw(error(domain_error(not_less_than_zero, S), sleep/1)).
sleep(S) :- '$gnu_os'(sleep, S, _).
host_name(H) :- '$gnu_os'(hostname, 0, H).
os_version(V) :- '$gnu_os'(os_version, 0, V).
architecture(A) :- '$gnu_os'(architecture, 0, A).
date_time(DT) :- '$gnu_os'(date_time, 0, DT).
send_signal(P, _) :- var(P), !, throw(error(instantiation_error, send_signal/2)).
send_signal(_, S) :- var(S), !, throw(error(instantiation_error, send_signal/2)).
send_signal(P, S) :- '$gnu_os'(kill, P, S).
temporary_name(T, _) :- var(T), !, throw(error(instantiation_error, temporary_name/2)).
temporary_name(T, P) :- atom_codes(T, Cs), atom_codes('XXXXXX', X6), ( append(Pre, X6, Cs) -> true ; Pre = Cs ), '$gnu_b_tmp_suffix'(Suf), append(Pre, Suf, Ps), atom_codes(P, Ps).
temporary_file(D, _, _) :- var(D), !, throw(error(instantiation_error, temporary_file/3)).
temporary_file(D, Pre, P) :- atom_codes(Pre, PCs), ( length(P5, 5), append(P5, _, PCs) -> true ; P5 = PCs ), '$gnu_b_tmp_suffix'(Suf), atom_codes(D, DCs),
    ( append(_, [47], DCs) -> Sep = [] ; Sep = [47] ), append([DCs, Sep, P5, Suf], Ps), atom_codes(P, Ps).
'$gnu_b_tmp_suffix'(Suf) :- prolog_pid(Pid), ( nb_current('$gnu_b_tmp_n', N0) -> true ; N0 = 0 ), N is N0 + 1, nb_setval('$gnu_b_tmp_n', N),
    K is (Pid * 1000 + N) mod 1000000 + 1000000, number_codes(K, [_|Suf]).
is_absolute_file_name(F) :- var(F), !, throw(error(instantiation_error, is_absolute_file_name/1)).
is_absolute_file_name(F) :- atom(F), sub_atom(F, 0, 1, _, '/').
cpu_time(T) :- '$gnu_b_clock'(T, cpu_time/1), statistics(cpu_time, [T0, _]), T = T0.
real_time(T) :- '$gnu_b_clock'(T, real_time/1), statistics(real_time, [T0, _]), T = T0.
user_time(T) :- '$gnu_b_clock'(T, user_time/1), statistics(user_time, [T0, _]), T = T0.
system_time(T) :- '$gnu_b_clock'(T, system_time/1), statistics(system_time, [T0, _]), T = T0.
'$gnu_b_clock'(T, _) :- var(T), !.
'$gnu_b_clock'(T, _) :- integer(T), !.
'$gnu_b_clock'(T, PI) :- throw(error(type_error(integer, T), PI)).
read_token_from_atom(A, T) :- var(A), !, throw(error(instantiation_error, read_token_from_atom/2)).
read_token_from_atom(A, T) :- \+ atom(A), !, throw(error(type_error(atom, A), read_token_from_atom/2)).
read_token_from_atom(A, T) :- atom_codes(A, Cs), '$gnu_b_tok'(Cs, T0), T = T0.
read_token_from_chars(L, T) :- atom_chars(A, L), atom_codes(A, Cs), '$gnu_b_tok'(Cs, T0), T = T0.
read_token_from_codes(Cs, T) :- atom_codes(A, Cs), atom_codes(A, Cs1), '$gnu_b_tok'(Cs1, T0), T = T0.
'$gnu_b_tok'(Cs0, T) :- '$gnu_b_layout'(Cs0, Cs), '$gnu_b_tok1'(Cs, T).
'$gnu_b_layout'([C|Cs], R) :- code_type(C, space), !, '$gnu_b_layout'(Cs, R).
'$gnu_b_layout'([0'%|Cs], R) :- !, '$gnu_b_eol'(Cs, Cs1), '$gnu_b_layout'(Cs1, R).
'$gnu_b_layout'([0'/, 0'*|Cs], R) :- !, '$gnu_b_eoc'(Cs, Cs1), '$gnu_b_layout'(Cs1, R).
'$gnu_b_layout'(Cs, Cs).
'$gnu_b_eol'([], []).
'$gnu_b_eol'([0'\n|Cs], Cs) :- !.
'$gnu_b_eol'([_|Cs], R) :- '$gnu_b_eol'(Cs, R).
'$gnu_b_eoc'([], []).
'$gnu_b_eoc'([0'*, 0'/|Cs], Cs) :- !.
'$gnu_b_eoc'([_|Cs], R) :- '$gnu_b_eoc'(Cs, R).
'$gnu_b_tok1'([], punct(end_of_file)) :- !.
'$gnu_b_tok1'([C|Cs], T) :- code_type(C, digit(_)), !, '$gnu_b_num'([C|Cs], T).
'$gnu_b_tok1'([C|Cs], T) :- ( code_type(C, upper) ; C == 0'_ ), !, '$gnu_b_alnum'(Cs, Ns), atom_codes(N, [C|Ns]), T = var(N).
'$gnu_b_tok1'([C|Cs], T) :- code_type(C, lower), !, '$gnu_b_alnum'(Cs, Ns), atom_codes(T, [C|Ns]).
'$gnu_b_tok1'([0'"|Cs], string(A)) :- !, '$gnu_b_quoted'(Cs, 0'", Q), append([0'"|Q], [0'"], L), atom_codes(LA, L), read_term_from_atom(LA, S, [double_quotes(codes)]), atom_codes(A, S).
'$gnu_b_tok1'([0'`|Cs], back_quotes(A)) :- !, '$gnu_b_quoted'(Cs, 0'`, Q), append([0''|Q], [0''], L), atom_codes(LA, L), read_term_from_atom(LA, A, []).
'$gnu_b_tok1'([0''|Cs], A) :- !, '$gnu_b_quoted'(Cs, 0'', Q), append([0''|Q], [0''], L), atom_codes(LA, L), read_term_from_atom(LA, A, []).
'$gnu_b_tok1'([C|_], punct(P)) :- '$gnu_b_in'(C, '()[]{}|'), !, atom_codes(P, [C]).
'$gnu_b_tok1'([0',|_], ',') :- !.
'$gnu_b_tok1'([0'!|_], !) :- !.
'$gnu_b_tok1'([0';|_], ;) :- !.
'$gnu_b_tok1'([0'.|Cs], punct(full_stop)) :- ( Cs == [] ; Cs = [C|_], ( code_type(C, space) ; C == 0'% ) ), !.
'$gnu_b_tok1'([C|Cs], T) :- '$gnu_b_sym'(C), !, '$gnu_b_syms'(Cs, Ss), atom_codes(T, [C|Ss]).
'$gnu_b_alnum'([C|Cs], [C|Ns]) :- ( code_type(C, alnum) ; C == 0'_ ), !, '$gnu_b_alnum'(Cs, Ns).
'$gnu_b_alnum'(_, []).
'$gnu_b_sym'(C) :- '$gnu_b_in'(C, '+-*/\\^<>=~:.?@#&$').
'$gnu_b_syms'([C|Cs], [C|Ss]) :- '$gnu_b_sym'(C), !, '$gnu_b_syms'(Cs, Ss).
'$gnu_b_syms'(_, []).
'$gnu_b_quoted'([Q, Q|Cs], Q, [Q, Q|R]) :- !, '$gnu_b_quoted'(Cs, Q, R).
'$gnu_b_quoted'([Q|_], Q, []) :- !.
'$gnu_b_quoted'([0'\\, C|Cs], Q, [0'\\, C|R]) :- !, '$gnu_b_quoted'(Cs, Q, R).
'$gnu_b_quoted'([C|Cs], Q, [C|R]) :- '$gnu_b_quoted'(Cs, Q, R).
'$gnu_b_num'(Cs, T) :- '$gnu_b_numlex'(Cs, L), atom_codes(A, L), atom_number(A, T).
'$gnu_b_numlex'([0'0, 0'', C|_], [0'0, 0'', C]) :- !.
'$gnu_b_numlex'([0'0, X|Cs], [0'0, X|Ds]) :- '$gnu_b_in'(X, xob), Cs = [D|_], code_type(D, xdigit(_)), !, '$gnu_b_alnum'(Cs, Ds).
'$gnu_b_numlex'(Cs, L) :- '$gnu_b_digits'(Cs, Ds, R), ( R = [0'., D|R1], code_type(D, digit(_)) -> '$gnu_b_digits'([D|R1], Fs, R2), '$gnu_b_exp'(R2, Es), append(Ds, [0'.|Fs], L0), append(L0, Es, L) ; L = Ds ).
'$gnu_b_digits'([C|Cs], [C|Ds], R) :- code_type(C, digit(_)), !, '$gnu_b_digits'(Cs, Ds, R).
'$gnu_b_digits'(R, [], R).
'$gnu_b_exp'([E|Cs], [E|Es]) :- '$gnu_b_in'(E, eE), ( Cs = [S|Cs1], '$gnu_b_in'(S, '+-') -> Es = [S|Ds], Cs2 = Cs1 ; Es = Ds, Cs2 = Cs ), Cs2 = [D|_], code_type(D, digit(_)), !, '$gnu_b_digits'(Cs2, Ds, _).
'$gnu_b_exp'(_, []).
'$gnu_b_in'(C, A) :- atom_codes(A, L), memberchk(C, L).
get_print_stream(S) :- ( var(S) -> true ; is_stream(S) -> true ; atom(S), stream_property(_, alias(S)) -> true ; throw(error(domain_error(stream, S), get_print_stream/1)) ), current_output(P), ( atom(S) -> stream_property(P, alias(S)) ; S = P ).
set_bip_name(F, _) :- var(F), !, throw(error(instantiation_error, set_bip_name/2)).
set_bip_name(F, _) :- \+ atom(F), !, throw(error(type_error(atom, F), set_bip_name/2)).
set_bip_name(_, A) :- var(A), !, throw(error(instantiation_error, set_bip_name/2)).
set_bip_name(_, A) :- \+ integer(A), !, throw(error(type_error(integer, A), set_bip_name/2)).
set_bip_name(F, A) :- ( A > 255 -> A1 = -1 ; A1 = A ), nb_setval('$gnu_b_bip', F/A1).
current_bip_name(F, A) :- '$gnu_b_oa'(F, current_bip_name/2), '$gnu_b_oi'(A, current_bip_name/2), nb_current('$gnu_b_bip', F0/A0), F = F0, A = A0.
syntax_error_info(F, L, C, M) :- '$gnu_b_oa'(F, syntax_error_info/4), '$gnu_b_oi'(L, syntax_error_info/4), '$gnu_b_oi'(C, syntax_error_info/4), '$gnu_b_oa'(M, syntax_error_info/4),
    '$gnu_os'(synerr, x, '$gnu_b_synerr'(F0, L0, C0, M0)), F = F0, L = L0, C = C0, M = M0.
'$gnu_b_oi'(I, _) :- ( var(I) ; integer(I) ), !.
'$gnu_b_oi'(I, PI) :- throw(error(type_error(integer, I), PI)).
read_token(T) :- current_input(S), '$gnu_b_rd'(S, read_token, T, read_token/1).
read_token(S, T) :- '$gnu_b_rd'(S, read_token, T, read_token/2).
read_atom(A) :- '$gnu_b_oa'(A, read_atom/1), current_input(S), '$gnu_b_rd'(S, read_atom, A, read_atom/1).
read_atom(S, A) :- '$gnu_b_oa'(A, read_atom/2), '$gnu_b_rd'(S, read_atom, A, read_atom/2).
read_integer(I) :- '$gnu_b_oi'(I, read_integer/1), current_input(S), '$gnu_b_rd'(S, read_integer, I, read_integer/1).
read_integer(S, I) :- '$gnu_b_oi'(I, read_integer/2), '$gnu_b_rd'(S, read_integer, I, read_integer/2).
read_number(N) :- '$gnu_b_on'(N, read_number/1), current_input(S), '$gnu_b_rd'(S, read_number, N, read_number/1).
read_number(S, N) :- '$gnu_b_on'(N, read_number/2), '$gnu_b_rd'(S, read_number, N, read_number/2).
'$gnu_b_on'(N, _) :- ( var(N) ; number(N) ), !.
'$gnu_b_on'(N, PI) :- throw(error(type_error(number, N), PI)).
'$gnu_b_rd'(S, Op, X, PI) :- current_input(O), set_input(S), '$gnu_os'(Op, S, R), set_input(O), '$gnu_b_rdres'(R, X, PI).
'$gnu_b_rdres'(ok(T), X, _) :- X = T.
'$gnu_b_rdres'(err(A), _, PI) :- throw(error(syntax_error(A), PI)).
get_key(C) :- current_input(S), '$gnu_b_key'(current_input_stream, S, C, get_key/1).
get_key(S, C) :- '$gnu_b_key'(S, S, C, get_key/2).
get_key_no_echo(C) :- current_input(S), '$gnu_b_key'(current_input_stream, S, C, get_key_no_echo/1).
get_key_no_echo(S, C) :- '$gnu_b_key'(S, S, C, get_key_no_echo/2).
'$gnu_b_key'(W, S, C, PI) :- current_input(O), set_input(S), set_input(O), ( stream_property(S, type(binary)) -> throw(error(permission_error(input, binary_stream, W), PI)) ; true ),
    '$gnu_b_oi'(C, PI), set_input(S), '$gnu_os'(get_key, x, K), set_input(O), C = K.
get_linedit_prompt(P) :- '$gnu_b_oa'(P, get_linedit_prompt/1), ( nb_current('$gnu_b_prompt', P0) -> true ; P0 = '' ), P = P0.
set_linedit_prompt(P) :- '$gnu_b_ia'(P, set_linedit_prompt/1), nb_setval('$gnu_b_prompt', P).
add_linedit_completion(W) :- '$gnu_b_ia'(W, add_linedit_completion/1), atom_codes(W, Cs), Cs \== [], '$gnu_b_wordcs'(Cs),
    ( nb_current('$gnu_b_compl', L) -> true ; L = [] ), ( memberchk(W, L) -> true ; nb_setval('$gnu_b_compl', [W|L]) ).
find_linedit_completion(P, C) :- '$gnu_b_ia'(P, find_linedit_completion/2), '$gnu_b_oa'(C, find_linedit_completion/2), '$gnu_os'(compl, P, L0),
    ( nb_current('$gnu_b_compl', Ad) -> true ; Ad = [] ), atom_codes(P, Pc), findall(W, ( member(W, Ad), atom_codes(W, Wc), append(Pc, _, Wc) ), L1), append(L0, L1, L2), sort(L2, L), member(C, L).
'$gnu_b_ia'(A, PI) :- var(A), !, throw(error(instantiation_error, PI)).
'$gnu_b_ia'(A, PI) :- \+ atom(A), !, throw(error(type_error(atom, A), PI)).
'$gnu_b_ia'(_, _).
'$gnu_b_wordcs'([]).
'$gnu_b_wordcs'([C|Cs]) :- ( C >= 0'a, C =< 0'z ; C >= 0'A, C =< 0'Z ; C >= 0'0, C =< 0'9 ; C =:= 0'_ ), !, '$gnu_b_wordcs'(Cs).
notrace :- nodebug.
leash(L) :- var(L), !, throw(error(instantiation_error, leash/1)).
leash(full) :- !, leash([call, exit, redo, fail, exception]).
leash(half) :- !, leash([call, redo]).
leash(loose) :- !, leash([call]).
leash(none) :- !, leash([]).
leash(tight) :- !, leash([call, redo, fail, exception]).
leash(L) :- '$gnu_b_lmask'(L, M), !, nb_setval('$gnu_b_leash', M), '$gnu_b_show_leash'.
leash(L) :- throw(error(domain_error(leash_ports, L), leash/1)).
'$gnu_b_lmask'([], 0).
'$gnu_b_lmask'([P|L], M1) :- '$gnu_b_lmask'(L, M), '$gnu_b_port'(P, B), M1 is M \/ B.
'$gnu_b_port'(call, 1).
'$gnu_b_port'(exit, 2).
'$gnu_b_port'(redo, 4).
'$gnu_b_port'(fail, 8).
'$gnu_b_port'(exception, 16).
'$gnu_b_show_leash' :- ( nb_current('$gnu_b_leash', M) -> true ; M = 31 ),
    ( M =:= 0 -> write(user_output, 'No leashing')
    ; write(user_output, 'Using leashing stopping at '), findall(P, ( '$gnu_b_port'(P, B), M /\ B > 0 ), Ps), '$gnu_b_ports'(Ps, 0'[), write(user_output, '] ports') ),
    nl(user_output).
'$gnu_b_ports'([], _).
'$gnu_b_ports'([P|Ps], C) :- format(user_output, '~c~a', [C, P]), '$gnu_b_ports'(Ps, 0',).
'$gnu_b_show_mode' :- ( current_prolog_flag(debug, on) -> write(user_output, 'The debugger will first leap -- showing spypoints (debug)') ; write(user_output, 'The debugger is switched off') ), nl(user_output).
debugging :- '$gnu_b_show_mode', '$gnu_b_show_leash', current_prolog_flag(unknown, A), write(user_output, 'Undefined predicates will '),
    ( A == error -> write(user_output, 'raise an existence_error') ; A == warning -> write(user_output, 'display a warning message and fail') ; write(user_output, fail) ), nl(user_output),
    write(user_output, 'Spypoints:'), nl(user_output), '$gnu_b_spies'(S), forall(member(N/Ar, S), format(user_output, '   ~a/~d~n', [N, Ar])).
'$gnu_b_spies'(S) :- ( nb_current('$gnu_b_spy', S) -> true ; S = [] ).
spy(Spec) :- '$gnu_b_preds'(Spec, spy/1, L), '$gnu_b_spy_set'(L), L \== [], current_prolog_flag(debug, off), debug, fail.
spy(_).
nospy(Spec) :- '$gnu_b_preds'(Spec, nospy/1, L), '$gnu_b_spy_reset'(L), fail.
nospy(_).
nospyall :- nb_setval('$gnu_b_spy', []), write(user_output, 'All spypoints removed'), nl(user_output).
spypoint_condition(G, _, _) :- var(G), !, throw(error(instantiation_error, spypoint_condition/3)).
spypoint_condition(G, P, T) :- callable(G), !, '$gnu_b_spc'(G, P, T).
spypoint_condition(G, _, _) :- throw(error(type_error(callable, G), spypoint_condition/3)).
'$gnu_b_spc'(G, _, _) :- functor(G, N, A),
    ( current_predicate(N/A) -> '$gnu_b_spy_set'([N/A]), current_prolog_flag(debug, off), debug ; format(user_output, 'Warning: The predicate ~a/~d is undefined~n', [N, A]) ), fail.
'$gnu_b_spc'(_, _, _).
'$gnu_b_spy_set'([]).
'$gnu_b_spy_set'([N/A|L]) :- '$gnu_b_spies'(S), ( select(N/A, S, S1) -> Msg = 'There is already a spypoint on' ; S1 = S, Msg = 'Spypoint placed on' ), append(S1, [N/A], S2),
    nb_setval('$gnu_b_spy', S2), format(user_output, '~a ~a/~d~n', [Msg, N, A]), '$gnu_b_spy_set'(L).
'$gnu_b_spy_reset'([]).
'$gnu_b_spy_reset'([N/A|L]) :- '$gnu_b_spies'(S), ( select(N/A, S, S1) -> Msg = 'Spypoint removed from' ; S1 = S, Msg = 'There is no spypoint on' ),
    nb_setval('$gnu_b_spy', S1), format(user_output, '~a ~a/~d~n', [Msg, N, A]), '$gnu_b_spy_reset'(L).
'$gnu_b_preds'(Spec, PI, _) :- var(Spec), !, throw(error(instantiation_error, PI)).
'$gnu_b_preds'([], _, []) :- !.
'$gnu_b_preds'([S1|S2], PI, L) :- !, '$gnu_b_preds'(S1, PI, L1), '$gnu_b_preds'(S2, PI, L2), append(L1, L2, L).
'$gnu_b_preds'(N/A1-A2, PI, L) :- !, '$gnu_b_preds1'(N, A1, A2, PI, L).
'$gnu_b_preds'(N, PI, L) :- atom(N), !, current_prolog_flag(max_arity, Max), '$gnu_b_preds1'(N, 0, Max, PI, L).
'$gnu_b_preds'(S, PI, L) :- '$gnu_b_pi'(S, PI, N, A), '$gnu_b_preds1'(N, A, A, PI, L).
'$gnu_b_pi'(S, PI, _, _) :- S \= _/_, !, throw(error(type_error(predicate_indicator, S), PI)).
'$gnu_b_pi'(N/_, PI, _, _) :- var(N), !, throw(error(instantiation_error, PI)).
'$gnu_b_pi'(_/A, PI, _, _) :- var(A), !, throw(error(instantiation_error, PI)).
'$gnu_b_pi'(N/_, PI, _, _) :- \+ atom(N), !, throw(error(type_error(atom, N), PI)).
'$gnu_b_pi'(_/A, PI, _, _) :- \+ integer(A), !, throw(error(type_error(integer, A), PI)).
'$gnu_b_pi'(_/A, PI, _, _) :- A < 0, !, throw(error(domain_error(not_less_than_zero, A), PI)).
'$gnu_b_pi'(N/A, _, N, A).
'$gnu_b_preds1'(N, A1, A2, PI, _) :- ( var(N) ; var(A1) ; var(A2) ), !, throw(error(instantiation_error, PI)).
'$gnu_b_preds1'(N, A1, A2, _, L) :- atom(N), current_prolog_flag(max_arity, Max), integer(A1), integer(A2), A1 >= 0, A1 =< Max, A2 >= 0, A2 =< Max,
    findall(Ln-(N/A), ( current_predicate(N/A), A >= A1, A =< A2, functor(H, N, A), ( predicate_property(H, prolog_line(Ln)) -> true ; Ln = 0 ) ), K0), keysort(K0, K1),
    findall(P, member(_-P, K1), L0), reverse(L0, L),
    ( L == [] -> ( A1 == A2 -> Z = A1 ; A1 =:= 0, A2 =:= Max -> Z = any ; Z = A1-A2 ), format(user_output, 'Warning: spy ~a/~w - no matching predicate~n', [N, Z]) ; true ).
socket(D, S) :- '$gnu_b_ia'(D, socket/2), ( D == 'AF_UNIX' ; D == 'AF_INET' ), !, '$gnu_b_unbound'(S, socket/2), '$gnu_os'(sk_socket, D, S).
socket(D, _) :- throw(error(domain_error(socket_domain, D), socket/2)).
socket_close(S) :- '$gnu_b_int'(S, socket_close/1), '$gnu_os'(sk_sclose, S, _).
socket_bind(S, A) :- '$gnu_b_int'(S, socket_bind/2), ( var(A) -> throw(error(instantiation_error, socket_bind/2)) ; '$gnu_b_sbind'(A, S) ).
'$gnu_b_sbind'('AF_UNIX'(P), S) :- !, '$gnu_b_ia'(P, socket_bind/2), '$gnu_os'(sk_sbind_unix, u(S, P), _).
'$gnu_b_sbind'('AF_INET'(H, P), S) :- !, ( var(H) -> '$gnu_os'(hostname, x, H) ; '$gnu_b_ia'(H, socket_bind/2) ),
    ( var(P) -> '$gnu_os'(sk_sbind_inet, i(S, 0), P) ; '$gnu_b_int'(P, socket_bind/2), '$gnu_os'(sk_sbind_inet, i(S, P), _) ).
'$gnu_b_sbind'(A, _) :- throw(error(domain_error(socket_address, A), socket_bind/2)).
socket_listen(S, N) :- '$gnu_b_int'(S, socket_listen/2), '$gnu_b_int'(N, socket_listen/2), '$gnu_os'(sk_slisten, l(S, N), _).
socket_accept(S, I, O) :- '$gnu_b_unbound'(I, socket_accept/3), '$gnu_b_unbound'(O, socket_accept/3), '$gnu_b_int'(S, socket_accept/3),
    '$gnu_os'(sk_saccept, S, acc(I0, O0, _)), I = I0, O = O0.
socket_accept(S, C, I, O) :- '$gnu_b_unbound'(C, socket_accept/4), '$gnu_b_unbound'(I, socket_accept/4), '$gnu_b_unbound'(O, socket_accept/4), '$gnu_b_int'(S, socket_accept/4),
    '$gnu_os'(sk_saccept, S, acc(I0, O0, C0)), C = C0, I = I0, O = O0.
socket_connect(S, A, I, O) :- '$gnu_b_unbound'(I, socket_connect/4), '$gnu_b_unbound'(O, socket_connect/4), '$gnu_b_int'(S, socket_connect/4),
    ( var(A) -> throw(error(instantiation_error, socket_connect/4)) ; '$gnu_b_sconn'(A, S, st(I0, O0)) ), I = I0, O = O0.
'$gnu_b_sconn'('AF_UNIX'(P), S, St) :- !, '$gnu_b_ia'(P, socket_connect/4), '$gnu_os'(sk_sconn_unix, c(S, P), St).
'$gnu_b_sconn'('AF_INET'(H, P), S, St) :- !, '$gnu_b_ia'(H, socket_connect/4), '$gnu_b_int'(P, socket_connect/4), '$gnu_os'(sk_sconn_inet, c(S, H, P), St).
'$gnu_b_sconn'(A, _, _) :- throw(error(domain_error(socket_address, A), socket_connect/4)).
hostname_address(N, A) :- '$gnu_b_ia'(N, hostname_address/2), '$gnu_b_oa'(A, hostname_address/2), '$gnu_os'(sk_hostaddr, N, A0), A = A0.
'$gnu_b_unbound'(X, PI) :- ( var(X) -> true ; throw(error(uninstantiation_error(X), PI)) ).
'$gnu_b_int'(X, PI) :- var(X), !, throw(error(instantiation_error, PI)).
'$gnu_b_int'(X, PI) :- \+ integer(X), !, throw(error(type_error(integer, X), PI)).
'$gnu_b_int'(_, _).
open_input_atom_stream(A, S) :- '$gnu_b_unbound'(S, open_input_atom_stream/2), '$gnu_b_ia'(A, open_input_atom_stream/2), '$gnu_os'(ts_in, A, S).
open_input_chars_stream(L, S) :- '$gnu_b_unbound'(S, open_input_chars_stream/2), '$gnu_b_chars'(L, L, open_input_chars_stream/2), atom_chars(A, L), '$gnu_os'(ts_in, A, S).
open_input_codes_stream(L, S) :- '$gnu_b_unbound'(S, open_input_codes_stream/2), '$gnu_b_codes'(L, L, open_input_codes_stream/2), atom_codes(A, L), '$gnu_os'(ts_in, A, S).
open_output_atom_stream(S) :- '$gnu_b_unbound'(S, open_output_atom_stream/1), '$gnu_os'(ts_out, x, S).
open_output_chars_stream(S) :- '$gnu_b_unbound'(S, open_output_chars_stream/1), '$gnu_os'(ts_out, x, S).
open_output_codes_stream(S) :- '$gnu_b_unbound'(S, open_output_codes_stream/1), '$gnu_os'(ts_out, x, S).
'$gnu_b_chars'(L, _, PI) :- var(L), !, throw(error(instantiation_error, PI)).
'$gnu_b_chars'([], _, _) :- !.
'$gnu_b_chars'([C|T], W, PI) :- !, ( var(C) -> throw(error(instantiation_error, PI)) ; atom(C), atom_length(C, 1) -> true ; throw(error(type_error(character, C), PI)) ), '$gnu_b_chars'(T, W, PI).
'$gnu_b_chars'(_, W, PI) :- throw(error(type_error(list, W), PI)).
'$gnu_b_codes'(L, _, PI) :- var(L), !, throw(error(instantiation_error, PI)).
'$gnu_b_codes'([], _, _) :- !.
'$gnu_b_codes'([C|T], W, PI) :- !, ( var(C) -> throw(error(instantiation_error, PI)) ; \+ integer(C) -> throw(error(type_error(integer, C), PI))
    ; C >= 1, C =< 255 -> true ; throw(error(representation_error(character_code), PI)) ), '$gnu_b_codes'(T, W, PI).
'$gnu_b_codes'(_, W, PI) :- throw(error(type_error(list, W), PI)).
close_input_atom_stream(X) :- '$gnu_b_tclose'(X, close_input_atom_stream/1).
close_input_chars_stream(X) :- '$gnu_b_tclose'(X, close_input_chars_stream/1).
close_input_codes_stream(X) :- '$gnu_b_tclose'(X, close_input_codes_stream/1).
'$gnu_b_tclose'(X, PI) :- '$gnu_b_sora'(X, PI, S), '$gnu_os'(ts_kind, S, K),
    ( K == n -> throw(error(domain_error(term_stream_or_alias, X), PI)) ; K == o -> throw(error(permission_error(close, stream, X), PI)) ; '$gnu_os'(ts_unmark, S, _), close(S) ).
close_output_atom_stream(X, A) :- '$gnu_b_tclose_out'(X, A, atom, close_output_atom_stream/2).
close_output_chars_stream(X, A) :- '$gnu_b_tclose_out'(X, A, chars, close_output_chars_stream/2).
close_output_codes_stream(X, A) :- '$gnu_b_tclose_out'(X, A, codes, close_output_codes_stream/2).
'$gnu_b_tclose_out'(X, Sink, Kind, PI) :- '$gnu_b_sora'(X, PI, S), '$gnu_os'(ts_kind, S, K),
    ( K == n -> throw(error(domain_error(term_stream_or_alias, X), PI)) ; K == i -> throw(error(permission_error(close, stream, X), PI)) ; true ),
    ( Kind == atom -> '$gnu_b_oa'(Sink, PI) ; '$gnu_b_plist'(Sink) -> true ; throw(error(type_error(list, Sink), PI)) ),
    '$gnu_os'(ts_take, S, T), ( Kind == atom -> V = T ; Kind == chars -> atom_chars(T, V) ; atom_codes(T, V) ), Sink = V, '$gnu_os'(ts_unmark, S, _), close(S).
'$gnu_b_plist'(L) :- var(L), !.
'$gnu_b_plist'([]) :- !.
'$gnu_b_plist'([_|T]) :- '$gnu_b_plist'(T).
'$gnu_b_sora'(X, PI, _) :- var(X), !, throw(error(instantiation_error, PI)).
'$gnu_b_sora'(X, PI, S) :- atom(X), !, ( stream_property(S0, alias(X)) -> S = S0 ; throw(error(existence_error(stream, X), PI)) ).
'$gnu_b_sora'(X, PI, S) :- X = '$stream'(_), !, ( catch(stream_property(X, mode(_)), _, fail) -> S = X ; throw(error(existence_error(stream, X), PI)) ).
'$gnu_b_sora'(X, PI, _) :- throw(error(domain_error(stream_or_alias, X), PI)).
add_stream_alias(X, A) :- '$gnu_b_sora'(X, add_stream_alias/2, S), '$gnu_b_ia'(A, add_stream_alias/2),
    ( stream_property(S1, alias(A)) -> ( S1 == S -> true ; throw(error(permission_error(add_alias, source_sink, alias(A)), add_stream_alias/2)) ) ; '$gnu_os'(ts_alias, al(S, A), _) ).
current_alias(S, A) :- '$gnu_b_ostream'(S, current_alias/2), '$gnu_b_oa'(A, current_alias/2), stream_property(S, alias(A)).
add_stream_mirror(X, M) :- '$gnu_b_sora'(X, add_stream_mirror/2, S), '$gnu_b_sora'(M, add_stream_mirror/2, Ms),
    ( stream_property(Ms, output) -> true ; throw(error(permission_error(output, stream, M), add_stream_mirror/2)) ), '$gnu_os'(ts_mir_add, mm(S, Ms), _).
remove_stream_mirror(X, M) :- '$gnu_b_sora'(X, remove_stream_mirror/2, S), '$gnu_b_sora'(M, remove_stream_mirror/2, Ms), '$gnu_os'(ts_mir_del, mm(S, Ms), _).
current_mirror(S, M) :- '$gnu_b_ostream'(S, current_mirror/2), '$gnu_b_ostream'(M, current_mirror/2), stream_property(S, mirror(M)).
'$gnu_b_ostream'(S, _) :- var(S), !.
'$gnu_b_ostream'('$stream'(_), _) :- !.
'$gnu_b_ostream'(S, PI) :- throw(error(domain_error(stream, S), PI)).
current_stream(S) :- '$gnu_b_ostream'(S, current_stream/1), catch(stream_property(S, mode(_)), error(existence_error(_, _), _), fail).
set_stream_type(S, T) :- PI = set_stream_type/2, ( var(T) -> throw(error(instantiation_error, PI)) ; T == text -> K = t ; T == binary -> K = b ; throw(error(domain_error(stream_type, T), PI)) ),
    '$gnu_b_sora'(S, PI, St), '$gnu_os'(ts_info, St, si(K0, _, _, IO)),
    ( K0 == K -> true ; IO =:= 1 -> throw(error(permission_error(modify, stream, S), PI)) ; '$gnu_os'(ts_settype, st(St, T), _) ).
set_stream_eof_action(S, A) :- PI = set_stream_eof_action/2,
    ( var(A) -> throw(error(instantiation_error, PI)) ; memberchk(A, [error, eof_code, reset]) -> true ; throw(error(domain_error(eof_action, A), PI)) ),
    '$gnu_b_sora'(S, PI, St), '$gnu_os'(ts_info, St, si(_, M, _, _)), ( M == w -> throw(error(permission_error(modify, stream, S), PI)) ; '$gnu_os'(ts_seteof, se(St, A), _) ).
set_stream_buffering(S, B) :- PI = set_stream_buffering/2,
    ( var(B) -> throw(error(instantiation_error, PI)) ; memberchk(B, [none, line, block]) -> true ; throw(error(domain_error(buffering_mode, B), PI)) ),
    '$gnu_b_sora'(S, PI, St), '$gnu_os'(ts_setbuf, sb(St, B), _).
stream_position(S, P) :- PI = stream_position/2, '$gnu_b_sora'(S, PI, St),
    ( var(P) -> true ; P = '$stream_position'(A, B, C, D), '$gnu_b_vint'([A, B, C, D]) -> true ; throw(error(domain_error(stream_position, P), PI)) ),
    '$gnu_os'(ts_pos, St, P0), P = P0.
'$gnu_b_vint'([]).
'$gnu_b_vint'([X|Xs]) :- ( var(X) ; integer(X) ), !, '$gnu_b_vint'(Xs).
stream_line_column(S, L, C) :- PI = stream_line_column/3, '$gnu_b_sora'(S, PI, St), '$gnu_os'(ts_info, St, si(K, _, _, _)),
    ( K == b -> throw(error(permission_error(access, binary_stream, S), PI)) ; true ), '$gnu_b_oi'(L, PI), '$gnu_b_oi'(C, PI),
    '$gnu_os'(ts_linecol, St, lc(L0, C0)), L = L0, C = C0.
set_stream_line_column(S, L, C) :- PI = set_stream_line_column/3, '$gnu_b_sora'(S, PI, St), '$gnu_os'(ts_info, St, si(K, _, R, _)),
    ( R == false -> throw(error(permission_error(reposition, stream, S), PI)) ; K == b -> throw(error(permission_error(reposition, binary_stream, S), PI)) ; true ),
    '$gnu_b_int'(L, PI), '$gnu_b_int'(C, PI), L >= 1, C >= 1, '$gnu_os'(ts_setlinecol, slc(St, L, C), _).
seek(S, W, O, N) :- ( \+ current_prolog_flag('$dialect', gnu) ; integer(W), atom(O) ), !, '$gnu_b_seek'(S, O, W, N, seek/4).
seek(S, W, O, N) :- '$gnu_b_seek'(S, W, O, N, seek/4).
'$gnu_b_seek'(S, W, O, N, PI) :- '$gnu_b_sora'(S, PI, St), '$gnu_os'(ts_info, St, si(K, _, R, _)),
    ( R == false -> throw(error(permission_error(reposition, stream, S), PI)) ; K == t -> throw(error(permission_error(reposition, text_stream, S), PI)) ; true ),
    '$gnu_b_ia'(W, PI), ( memberchk(W, [bof, current, eof]) -> true ; throw(error(domain_error(stream_seek_method, W), PI)) ),
    '$gnu_b_int'(O, PI), '$gnu_b_oi'(N, PI), '$gnu_os'(ts_seek, sk(St, W, O), N0), N = N0.
