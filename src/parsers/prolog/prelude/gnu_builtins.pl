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
                          number_atom/2, lower_upper/2, new_atom/1, new_atom/2 ]).
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
