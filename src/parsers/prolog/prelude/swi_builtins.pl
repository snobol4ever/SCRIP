/*  swi_builtins.pl -- SWI-PROLOG BUILTINS THAT SCRIP LACKS, AS A PROLOG LAYER (hq_prolog 2026-10-15, row prolog-swi-tabling-the-xsb-loader-runs-the-xsb-test-drivers and the SWI census).
    SCRIP's own text, AGPL like the rest of SCRIP: not a vendored file; the generator (util_gen_prolog_prelude_libs.py) embeds it in LOCAL mode, no SWI reader, no renames, its helpers
    '$swb_'-named by hand. It is parsed only when a program names one of its exports. directory_file_path/3 and make_directory_path/1 left for the vendored
    library(filesex) itself (cao 2026-10-30); every export here is written from the documented behaviour and graded against swipl by
    test_gate_pl_swi_file_and_text_builtins_answer_as_swipl.sh. fast_term_serialized/2, fast_write/2 and fast_read/2 (cfo 2026-10-17) write SCRIP's own form, a length-prefixed
    canonical text of term_factorized's skeleton and substitution with every string carried beside it as an atom, so no reader flag changes what comes back; graded by
    test_gate_pl_maplist_include_and_exclude_exit_deterministically_and_fast_term_io_round_trips_as_swipl.sh. maplist/6 and maplist/7 (hq_prolog 2026-10-24) are what swipl's
    library(apply_macros) expansion gives every program whose load reaches library(clpfd), list-first like the base prelude's maplist/2..5; set_prolog_stack/2 checks the stack
    name and the parameter as swipl 9.0.4 does, and limit(Size) sets the stack_limit flag, the combined limit swipl's own warning names. */
:- module(swi_builtins, [ '$clause'/4, open_string/2, exists_file/1, exists_directory/1, size_file/2, time_file/2, same_file/2, working_directory/2,
                          setenv/2, unsetenv/1, wait_for_input/3, text_to_string/2, fast_term_serialized/2, fast_write/2, fast_read/2, maplist/6, maplist/7, set_prolog_stack/2 , call_with_depth_limit/3, '$dl_findall'/3, '$dl_f2'/3,
                          set_time_file/3, link_file/3, file_mode_/2, chmod_/2, read_link/3, prolog_to_os_filename/2, copy_stream_data/2, copy_stream_data/3, get_time/1, expand_file_name/2,
                          current_arithmetic_function/1]).
get_time(T) :- '$gnu_os'(get_time, 0, T).
maplist(G, L1, L2, L3, L4, L5) :- '$swb_maplist'(L1, L2, L3, L4, L5, G).
'$clause'(H, B, R, _) :- nonvar(R), !, ( clause(H0, B0, R) -> true ; nb_current('$tx_cur', tx(Rs)), '$swb_tx_erased'(Rs, R, (H0 :- B0)) ), B = B0, '$swb_unmangle'(H0, H).
'$swb_tx_erased'([E|Es], R, C) :- ( E = '$txdb'(1, _, _, R0, C0), R0 == R -> C = C0 ; '$swb_tx_erased'(Es, R, C) ).
'$clause'(H, B, R, _) :- clause(H, B, R).
'$swb_unmangle'(H0, M:H) :- callable(H0), functor(H0, F, _), sub_atom(F, Bf, 1, Af, ':'), Bf > 0, Af > 0, !, sub_atom(F, 0, Bf, _, M), sub_atom(F, _, Af, 0, F1), H0 =.. [_|As], H =.. [F1|As].
'$swb_unmangle'(H, H).
'$swb_maplist'([], [], [], [], [], _).
'$swb_maplist'([A|As], [B|Bs], [C|Cs], [D|Ds], [E|Es], G) :- call(G, A, B, C, D, E), '$swb_maplist'(As, Bs, Cs, Ds, Es, G).
maplist(G, L1, L2, L3, L4, L5, L6) :- '$swb_maplist'(L1, L2, L3, L4, L5, L6, G).
'$swb_maplist'([], [], [], [], [], [], _).
'$swb_maplist'([A|As], [B|Bs], [C|Cs], [D|Ds], [E|Es], [F|Fs], G) :- call(G, A, B, C, D, E, F), '$swb_maplist'(As, Bs, Cs, Ds, Es, Fs, G).
set_prolog_stack(S, P) :- ( atom(S), memberchk(S, [local, global, trail, argument]) -> true ; throw(error(existence_error(stack, S), context(system:'$set_prolog_stack'/4, _))) ), '$swb_stack_kv'(P).
'$swb_stack_kv'(P) :- var(P), !, throw(error(instantiation_error, context(system:(=..)/2, _))).
'$swb_stack_kv'(P) :- compound(P), P =.. [K, V0], memberchk(K, [limit, min_free, low, factor, spare]), !, ( var(V0) -> throw(error(instantiation_error, context('$syspreds':set_prolog_stack/2, _))) ; true ), catch(V is V0, error(Err, _), throw(error(Err, context('$syspreds':set_prolog_stack/2, _)))), ( K == limit -> set_prolog_flag(stack_limit, V) ; true ).
'$swb_stack_kv'(P) :- ( compound(P) -> functor(P, K, _) ; K = P ), throw(error(domain_error(stack_parameter, K), context(system:'$set_prolog_stack'/4, _))).
open_string(S, Stream) :- ( var(S) -> throw(error(instantiation_error, context(open_string/2, _))) ; true ), text_to_string(S, Str), atom_string(A, Str), open_input_atom_stream(A, Stream), '$gnu_os'(ts_unmark, Stream, _).
text_to_string(T, S) :- var(T), !, throw(error(instantiation_error, context(system:text_to_string/2, _))).
text_to_string(T, S) :- T == [], !, S = "".
text_to_string(T, S) :- string(T), !, S = T.
text_to_string(T, S) :- atom(T), !, atom_string(T, S).
text_to_string(T, S) :- is_list(T), T = [C|_], !, ( integer(C) -> string_codes(S, T) ; string_chars(S, T) ).
text_to_string(T, _) :- throw(error(type_error(text, T), context(system:text_to_string/2, _))).
'$swb_arg'(X, P, PI) :- ( var(X) -> throw(error(instantiation_error, context(system:PI, _))) ; atom(X) -> P = X ; string(X) -> atom_string(P, X) ; throw(error(type_error(atom, X), context(system:PI, _))) ).
exists_file(F) :- '$swb_arg'(F, P, exists_file/1), '$gnu_os'(st_kind, P, K), K == file.
exists_directory(D) :- '$swb_arg'(D, P, exists_directory/1), '$gnu_os'(st_kind, P, K), K == directory.
size_file(F, S) :- '$swb_arg'(F, P, size_file/2), ( '$gnu_os'(st_size, P, S0) -> S = S0 ; throw(error(existence_error(file, F), context(system:size_file/2, 'No such file or directory'))) ).
time_file(F, T) :- '$swb_arg'(F, P, time_file/2), ( '$gnu_os'(st_mtime, P, T0) -> T = T0 ; throw(error(existence_error(file, F), context(system:time_file/2, 'No such file or directory'))) ).
same_file(A, B) :- '$swb_arg'(A, PA, same_file/2), '$swb_arg'(B, PB, same_file/2), ( PA == PB -> true ; '$gnu_os'(st_same, PA, PB) ).
working_directory(Old, New) :- '$gnu_os'(cwd, x, C0), ( atom_concat(_, /, C0) -> Old0 = C0 ; atom_concat(C0, /, Old0) ), Old = Old0, ( var(New) -> New = Old0 ; New == Old0 -> true ; '$swb_arg'(New, P, working_directory/2), ( '$gnu_os'(chdir, P, _) -> true ; throw(error(existence_error(directory, New), context(system:working_directory/2, 'No such file or directory'))) ) ).
setenv(N, V) :- '$swb_arg'(N, PN, setenv/2), ( var(V) -> throw(error(instantiation_error, context(system:setenv/2, _))) ; number(V) -> atom_number(PV, V) ; '$swb_arg'(V, PV, setenv/2) ), '$gnu_os'(setenv, PN, PV).
unsetenv(N) :- '$swb_arg'(N, PN, unsetenv/1), '$gnu_os'(unsetenv, PN, x).
wait_for_input(L, R, T) :- ( var(L) ; var(T) ), !, throw(error(instantiation_error, context(system:wait_for_input/3, _))).
wait_for_input(L, R, T) :- '$gnu_os'(poll, L-T, R0), R = R0.
fast_term_serialized(T, S) :- var(S), !, '$swb_ftser'(T, S, fast_term_serialized/2).
fast_term_serialized(T, S) :- ( string(S) -> true ; throw(error(type_error(string, S), context(system:fast_term_serialized/2, _))) ), '$swb_ftdes'(S, T).
fast_write(Out, T) :- '$swb_ftser'(T, S, fast_write/2), string_codes(S, Cs), '$swb_u8e'(Cs, Bs), length(Bs, N), number_codes(N, NCs), '$swb_putb'(NCs, Out), put_byte(Out, 46),
    '$swb_putb'(Bs, Out).
fast_read(In, T) :- get_byte(In, B0), ( B0 < 0 -> T = end_of_file ; '$swb_getlen'(In, B0, 0, N), length(Bs, N), '$swb_getb'(Bs, In), '$swb_u8d'(Bs, Cs), string_codes(S, Cs), '$swb_ftdes'(S, T) ).
'$swb_ftser'(T, S, PI) :- term_factorized(T, K, Sb), '$swb_ftblob'(K-Sb, PI), '$swb_ftstr'(K-Sb, KS, [], Ps), with_output_to(string(S), write_term('$fts'(KS, Ps), [quoted(true), ignore_ops(true), '$lists'(true)])).
'$swb_ftdes'(S, T) :- term_string(R, S), R = '$fts'(K-Sb, Ps), '$swb_ftstrb'(Ps), '$swb_ftbind'(Sb), T = K.
'$swb_ftstr'(X, X, P, P) :- var(X), !.
'$swb_ftstr'(X, V, P, [V-A|P]) :- string(X), !, atom_string(A, X).
'$swb_ftstr'(X, X, P, P) :- atomic(X), !.
'$swb_ftstr'(X, Y, P0, P) :- X =.. [F|As], '$swb_ftstrl'(As, Bs, P0, P), Y =.. [F|Bs].
'$swb_ftstrl'([], [], P, P).
'$swb_ftstrl'([A|As], [B|Bs], P0, P) :- '$swb_ftstr'(A, B, P0, P1), '$swb_ftstrl'(As, Bs, P1, P).
'$swb_ftstrb'([]).
'$swb_ftstrb'([V-A|Ps]) :- atom_string(A, V), '$swb_ftstrb'(Ps).
'$swb_ftbind'([]).
'$swb_ftbind'([V=X|R]) :- V = X, '$swb_ftbind'(R).
'$swb_ftblob'(X, _) :- var(X), !.
'$swb_ftblob'(X, _) :- atomic(X), !.
'$swb_ftblob'(X, PI) :- X = '$stream'(_), !, throw(error(permission_error(fast_serialize, blob, X), context(system:PI, _))).
'$swb_ftblob'(X, PI) :- X =.. [_|As], '$swb_ftblobl'(As, PI).
'$swb_ftblobl'([], _).
'$swb_ftblobl'([A|As], PI) :- '$swb_ftblob'(A, PI), '$swb_ftblobl'(As, PI).
'$swb_getlen'(_, 46, N, N) :- !.
'$swb_getlen'(In, B, A, N) :- B >= 48, B =< 57, A1 is A * 10 + B - 48, get_byte(In, B1), '$swb_getlen'(In, B1, A1, N).
'$swb_putb'([], _).
'$swb_putb'([B|Bs], O) :- put_byte(O, B), '$swb_putb'(Bs, O).
'$swb_getb'([], _).
'$swb_getb'([B|Bs], I) :- get_byte(I, B), '$swb_getb'(Bs, I).
'$swb_u8e'([], []).
'$swb_u8e'([C|Cs], Bs) :- '$swb_u8c'(C, Bs, R), '$swb_u8e'(Cs, R).
'$swb_u8c'(C, [C|R], R) :- C < 128, !.
'$swb_u8c'(C, [B1, B2|R], R) :- C < 2048, !, B1 is 192 \/ (C >> 6), B2 is 128 \/ (C /\ 63).
'$swb_u8c'(C, [B1, B2, B3|R], R) :- C < 65536, !, B1 is 224 \/ (C >> 12), B2 is 128 \/ ((C >> 6) /\ 63), B3 is 128 \/ (C /\ 63).
'$swb_u8c'(C, [B1, B2, B3, B4|R], R) :- B1 is 240 \/ (C >> 18), B2 is 128 \/ ((C >> 12) /\ 63), B3 is 128 \/ ((C >> 6) /\ 63), B4 is 128 \/ (C /\ 63).
'$swb_u8d'([], []).
'$swb_u8d'([B|Bs], [C|Cs]) :- '$swb_u8b'(B, Bs, C, R), '$swb_u8d'(R, Cs).
'$swb_u8b'(B, Bs, B, Bs) :- B < 128, !.
'$swb_u8b'(B, [B2|R], C, R) :- B < 224, !, C is ((B /\ 31) << 6) \/ (B2 /\ 63).
'$swb_u8b'(B, [B2, B3|R], C, R) :- B < 240, !, C is ((B /\ 15) << 12) \/ ((B2 /\ 63) << 6) \/ (B3 /\ 63).
'$swb_u8b'(B, [B2, B3, B4|R], C, R) :- C is ((B /\ 7) << 18) \/ ((B2 /\ 63) << 12) \/ ((B3 /\ 63) << 6) \/ (B4 /\ 63).

call_with_depth_limit(G, L, R) :-
    '$dl_check'(L),
    '$dl_push'(L),
    (   call(G), '$dl_reached'(R), ( true ; '$dl_reset'(0), fail )
    ;   '$dl_false'(R)
    ).
'$dl_findall'(T, G, L) :- '$dl_f2'(T, G, L).
'$dl_f2'(T, G, L) :- '$dl_f3'(T, G, L).
'$dl_f3'(T, G, L) :- '$dl_f4'(T, G, L).
'$dl_f4'(T, G, L) :- '$dl_f5'(T, G, L).
'$dl_f5'(T, G, L) :- '$dl_tick'(1), findall(T, G, L).
'$dl_check'(L) :- ( var(L) -> throw(error(instantiation_error, call_with_depth_limit/3)) ; integer(L) -> true ; throw(error(type_error(integer, L), call_with_depth_limit/3)) ).
'$swb_fx_text'(X, P, Ctx) :- ( var(X) -> throw(error(instantiation_error, context(Ctx, _))) ; atom(X) -> P = X ; string(X) -> atom_string(P, X) ; is_list(X), catch(text_to_string(X, S), _, fail) -> atom_string(P, S) ; throw(error(type_error(text, X), context(Ctx, _))) ).
'$swb_fx_err'(e(enoent, M), _, O) :- !, throw(error(existence_error(file, O), context(_, M))).
'$swb_fx_err'(e(eperm, M), A, O) :- !, throw(error(permission_error(A, file, O), context(_, M))).
'$swb_fx_err'(e(_, M), _, _) :- throw(error(system_error, context(_, M))).
set_time_file(F, Old, New) :- '$swb_fx_text'(F, P, files_ex:set_time_file/3), '$fx_stat'(P, R), ( R = st(_, A, M, C) -> true ; '$swb_fx_err'(R, stat, P) ), '$swb_fx_old'(Old, access, A), '$swb_fx_old'(Old, modified, M), '$swb_fx_old'(Old, changed, C), '$swb_fx_close'(Old), ( New == [] -> true ; '$swb_fx_new'(New, access, A, A1), '$swb_fx_new'(New, modified, M, M1), '$fx_utime'(P, A1, M1, U), ( U == ok -> true ; '$swb_fx_err'(U, set_time, P) ) ).
'$swb_fx_old'(L, N, T) :- var(L), !, X =.. [N, T], L = [X|_].
'$swb_fx_old'([X|L], N, T) :- !, ( compound(X), functor(X, N, 1) -> arg(1, X, V), ( V = T -> true ; true ) ; '$swb_fx_old'(L, N, T) ).
'$swb_fx_old'(_, _, _).
'$swb_fx_close'(L) :- var(L), !, L = [].
'$swb_fx_close'([_|L]) :- !, '$swb_fx_close'(L).
'$swb_fx_close'(_).
'$swb_fx_new'(L, N, Cur, T) :- ( '$swb_fx_find'(L, N, X) -> '$swb_fx_time'(X, T) ; T is truncate(Cur) ).
'$swb_fx_find'(L, N, X) :- nonvar(L), L = [E|R], ( nonvar(E), compound(E), functor(E, N, 1) -> arg(1, E, X) ; '$swb_fx_find'(R, N, X) ).
'$swb_fx_time'(X, _) :- var(X), !, throw(error(instantiation_error, _)).
'$swb_fx_time'(now, T) :- !, get_time(T0), T is truncate(T0).
'$swb_fx_time'(X, T) :- number(X), !, T is truncate(X).
'$swb_fx_time'(X, _) :- throw(error(type_error(time, X), _)).
link_file(O, N, T) :- '$swb_fx_text'(O, OP, files_ex:link_file/3), '$swb_fx_text'(N, NP, files_ex:link_file/3), ( var(T) -> throw(error(instantiation_error, _)) ; \+ atom(T) -> throw(error(type_error(atom, T), _)) ; T == symbolic -> true ; T == hard -> true ; throw(error(domain_error(link_type, T), _)) ), ( T == symbolic -> '$fx_symlink'(OP, NP, R) ; '$fx_link'(OP, NP, R) ), ( R == ok -> true ; '$swb_fx_err'(R, link, NP) ).
file_mode_(F, M) :- '$swb_fx_text'(F, P, files_ex:file_mode_/2), '$fx_stat'(P, R), ( R = st(M0, _, _, _) -> M = M0 ; '$swb_fx_err'(R, stat, P) ).
chmod_(F, M) :- '$swb_fx_text'(F, P, files_ex:chmod_/2), ( var(M) -> throw(error(instantiation_error, context(files_ex:chmod_/2, _))) ; integer(M) -> true ; throw(error(type_error(integer, M), context(files_ex:chmod_/2, _))) ), '$fx_chmod'(P, M, R), ( R == ok -> true ; '$swb_fx_err'(R, chmod, M) ).
read_link(F, L, T) :- '$swb_fx_text'(F, P, system:read_link/3), '$fx_readlink'(P, l(L0, T0)), L = L0, T = T0.
prolog_to_os_filename(P, O) :- P = O.
copy_stream_data(I, O) :- '$swb_csd'(I, O, -1).
copy_stream_data(I, O, N) :- ( var(N) -> throw(error(instantiation_error, context(system:copy_stream_data/3, _))) ; integer(N) -> true ; throw(error(type_error(integer, N), context(system:copy_stream_data/3, _))) ), '$swb_csd'(I, O, N).
'$swb_csd'(I, O, N) :- ( stream_property(I, type(binary)) -> '$swb_csdb'(I, O, N) ; '$swb_csdt'(I, O, N) ).
'$swb_csdt'(_, _, 0) :- !.
'$swb_csdt'(I, O, N) :- get_char(I, C), ( C == end_of_file -> true ; put_char(O, C), N1 is N - 1, '$swb_csdt'(I, O, N1) ).
'$swb_csdb'(_, _, 0) :- !.
'$swb_csdb'(I, O, N) :- get_byte(I, C), ( C =:= -1 -> true ; put_byte(O, C), N1 is N - 1, '$swb_csdb'(I, O, N1) ).
% expand_file_name/2 (hq_icon 2026-10-31, row prolog-swi-to-100-the-failing-cases-r-1930, files/test_glob): swipl 9.0.4's answers, measured on a scratch tree and graded by
% test_gate_pl_swi_expand_file_name_answers_as_swipl.sh.  A name with no ? * [ { comes back as the one-element list it was (after $VAR and ~ expansion), existing or not; a name with one is
% split at '/', each segment with a wildcard lists its directory (a leading '.' is matched only by a segment that starts with '.'; '.' and '..' are entries) and matches it with
% wildcard_match/2, a literal segment after a wildcard must exist at the end, a trailing '/' is dropped, the whole strings are msort-ed, an unmatched [ or { raises syntax_error.
expand_file_name(Spec, Files) :-
    '$swb_efn_text'(Spec, A0), '$swb_efn_dollar'(A0, A1), '$swb_efn_tilde'(A1, A2),
    (   '$swb_efn_meta'(A2)
    ->  atomic_list_concat(Segs, '/', A2), '$swb_efn_check'(Segs),
        findall(F, '$swb_efn_gen'(Segs, '', no, F), Found), msort(Found, Files)
    ;   Files = [A2] ).
'$swb_efn_err'(E) :- throw(error(E, context(system:expand_file_name/2, _))).
'$swb_efn_text'(S, _) :- var(S), !, '$swb_efn_err'(instantiation_error).
'$swb_efn_text'(S, A) :- atom(S), !, A = S.
'$swb_efn_text'(S, A) :- string(S), !, atom_string(A, S).
'$swb_efn_text'(S, A) :- number(S), !, number_codes(S, Cs), atom_codes(A, Cs).
'$swb_efn_text'(S, A) :- is_list(S), catch(text_to_string(S, Str), error(_, _), fail), !, atom_string(A, Str).
'$swb_efn_text'(S, _) :- '$swb_efn_err'(type_error(text, S)).
'$swb_efn_namechar'(C) :- ( C >= 0'a, C =< 0'z -> true ; C >= 0'A, C =< 0'Z -> true ; C >= 0'0, C =< 0'9 -> true ; C =:= 0'_ ).
'$swb_efn_dollar'(A0, A) :- sub_atom(A0, _, 1, _, '$'), !, atom_codes(A0, Cs0), '$swb_efn_dollar_'(Cs0, Cs), atom_codes(A, Cs).
'$swb_efn_dollar'(A, A).
'$swb_efn_dollar_'([], []).
'$swb_efn_dollar_'([0'$, C|T], Out) :- '$swb_efn_namechar'(C), !, '$swb_efn_name'([C|T], NameCs, Rest), atom_codes(Name, NameCs),
    (   getenv(Name, Val) -> atom_codes(Val, VCs) ; '$swb_efn_err'(existence_error(variable, Name)) ),
    '$swb_efn_dollar_'(Rest, Out0), append(VCs, Out0, Out).
'$swb_efn_dollar_'([C|T], [C|Out]) :- '$swb_efn_dollar_'(T, Out).
'$swb_efn_name'([C|T], [C|Ns], Rest) :- '$swb_efn_namechar'(C), !, '$swb_efn_name'(T, Ns, Rest).
'$swb_efn_name'(Rest, [], Rest).
'$swb_efn_tilde'(A0, A) :- sub_atom(A0, 0, 1, _, '~'), !, atom_codes(A0, [_|Cs0]), '$swb_efn_upto_slash'(Cs0, UCs, Rest),
    (   UCs == [] -> ( getenv('HOME', H) -> true ; H = '' ) ; atom_codes(U, UCs), '$swb_efn_home'(U, H) ),
    atom_codes(H, HCs), append(HCs, Rest, Cs), atom_codes(A, Cs).
'$swb_efn_tilde'(A, A).
'$swb_efn_upto_slash'([C|T], [C|Us], Rest) :- C =\= 0'/, !, '$swb_efn_upto_slash'(T, Us, Rest).
'$swb_efn_upto_slash'(Rest, [], Rest).
'$swb_efn_home'(U, H) :- catch(setup_call_cleanup(open('/etc/passwd', read, S), '$swb_efn_pw'(S, U, H0), close(S)), error(_, _), fail), !, H = H0.
'$swb_efn_home'(U, _) :- '$swb_efn_err'(existence_error(user, U)).
'$swb_efn_pw'(S, U, H) :- read_line_to_codes(S, Cs), Cs \== end_of_file, atom_codes(L, Cs), atomic_list_concat(Fs, ':', L),
    (   Fs = [U, _, _, _, _, H0|_] -> H = H0 ; '$swb_efn_pw'(S, U, H) ).
'$swb_efn_meta'(A) :- sub_atom(A, _, 1, _, C), memberchk(C, ['*', '?', '[', '{']), !.
'$swb_efn_check'([]).
'$swb_efn_check'([S|T]) :- atom_codes(S, Cs), '$swb_efn_bal'(Cs, 0, no), '$swb_efn_check'(T).
'$swb_efn_bal'([], 0, no) :- !.
'$swb_efn_bal'([], _, yes) :- !, '$swb_efn_err'(syntax_error('Unmatched \'[\'')).
'$swb_efn_bal'([], _, no) :- '$swb_efn_err'(syntax_error('Unmatched \'{\'')).
'$swb_efn_bal'([0']|T], D, yes) :- !, '$swb_efn_bal'(T, D, no).
'$swb_efn_bal'([_|T], D, yes) :- !, '$swb_efn_bal'(T, D, yes).
'$swb_efn_bal'([0'[|T], D, no) :- !, '$swb_efn_bal'(T, D, yes).
'$swb_efn_bal'([0'{|T], D, no) :- !, D1 is D + 1, '$swb_efn_bal'(T, D1, no).
'$swb_efn_bal'([0'}|T], D, no) :- D > 0, !, D1 is D - 1, '$swb_efn_bal'(T, D1, no).
'$swb_efn_bal'([_|T], D, no) :- '$swb_efn_bal'(T, D, no).
'$swb_efn_gen'([S], P, Wild, F) :- !, '$swb_efn_last'(S, P, Wild, F).
'$swb_efn_gen'([S|Rest], P, Wild, F) :- '$swb_efn_seg'(S, P, Wild, E, Wild1), atomic_list_concat([P, E, '/'], P1), '$swb_efn_gen'(Rest, P1, Wild1, F).
'$swb_efn_last'(S, P, _, F) :- '$swb_efn_meta'(S), !, '$swb_efn_entry'(S, P, E), atom_concat(P, E, F).
'$swb_efn_last'('', P, _, F) :- !, atom_concat(F, '/', P).
'$swb_efn_last'(S, P, _, F) :- atom_concat(P, S, F), ( exists_file(F) -> true ; exists_directory(F) ).
'$swb_efn_seg'(S, P, _, E, yes) :- '$swb_efn_meta'(S), !, '$swb_efn_entry'(S, P, E).
'$swb_efn_seg'(S, _, Wild, S, Wild).
'$swb_efn_entry'(S, P, E) :- ( P == '' -> D = '.' ; D = P ), catch(directory_files(D, Es), error(_, _), fail), member(E, Es), '$swb_efn_visible'(S, E), wildcard_match(S, E).
'$swb_efn_visible'(S, E) :- ( sub_atom(E, 0, 1, _, '.') -> sub_atom(S, 0, 1, _, '.') ; true ).

current_arithmetic_function(F) :-
    var(F),
    !,
    '$swb_evaluables'(L),
    member(N/A, L),
    functor(F, N, A).
current_arithmetic_function(F) :-
    callable(F),
    !,
    functor(F, N, A),
    '$swb_evaluables'(L),
    memberchk(N/A, L).
current_arithmetic_function(F) :-
    throw(error(type_error(callable, F), context(current_arithmetic_function/1, _))).
'$swb_evaluables'([(*)/2,(**)/2,(+)/1,(+)/2,(-)/1,(-)/2,(/)/2,(//)/2,(/\)/2,(<<)/2,(>>)/2,(\)/1,(\/)/2,(^)/2,abs/1,acos/1,acosh/1,asin/1,asinh/1,atan/1,atan/2,atan2/2,atanh/1,ceil/1,ceiling/1,copysign/2,cos/1,cosh/1,cputime/0,denominator/1,(div)/2,e/0,epsilon/0,erf/1,erfc/1,eval/1,exp/1,float/1,float_fractional_part/1,float_integer_part/1,floor/1,gcd/2,getbit/2,inf/0,integer/1,lcm/2,lgamma/1,log/1,log10/1,lsb/1,max/2,min/2,(mod)/2,msb/1,nan/0,nexttoward/2,numerator/1,pi/0,popcount/1,powm/3,random/1,random_float/0,rational/1,rationalize/1,(rdiv)/2,(rem)/2,round/1,roundtoward/2,sign/1,sin/1,sinh/1,sqrt/1,tan/1,tanh/1,truncate/1,(xor)/2]).
