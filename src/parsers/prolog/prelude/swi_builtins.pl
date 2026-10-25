/*  swi_builtins.pl -- SWI-PROLOG BUILTINS THAT SCRIP LACKS, AS A PROLOG LAYER (hq_prolog 2026-10-15, row prolog-swi-tabling-the-xsb-loader-runs-the-xsb-test-drivers and the SWI census).
    SCRIP's own text, AGPL like the rest of SCRIP: not a vendored file; the generator (util_gen_prolog_prelude_libs.py) embeds it in LOCAL mode, no SWI reader, no renames, its helpers
    '$swb_'-named by hand. It is parsed only when a program names one of its exports. directory_file_path/3 and make_directory_path/1 are swipl 9.0.4's library(filesex) clauses
    (BSD, Jan Wielemaker, VU University Amsterdam) with their foreign-library dependency removed; the rest are written here from the documented behaviour and graded against swipl by
    test_gate_pl_swi_file_and_text_builtins_answer_as_swipl.sh. fast_term_serialized/2, fast_write/2 and fast_read/2 (cfo 2026-10-17) write SCRIP's own form, a length-prefixed
    canonical text of term_factorized's skeleton and substitution with every string carried beside it as an atom, so no reader flag changes what comes back; graded by
    test_gate_pl_maplist_include_and_exclude_exit_deterministically_and_fast_term_io_round_trips_as_swipl.sh. maplist/6 and maplist/7 (hq_prolog 2026-10-24) are what swipl's
    library(apply_macros) expansion gives every program whose load reaches library(clpfd), list-first like the base prelude's maplist/2..5; set_prolog_stack/2 checks the stack
    name and the parameter as swipl 9.0.4 does, and limit(Size) sets the stack_limit flag, the combined limit swipl's own warning names. */
:- module(swi_builtins, [ open_string/2, directory_file_path/3, make_directory_path/1, exists_file/1, exists_directory/1, size_file/2, time_file/2, same_file/2, working_directory/2,
                          setenv/2, unsetenv/1, text_to_string/2, fast_term_serialized/2, fast_write/2, fast_read/2, maplist/6, maplist/7, set_prolog_stack/2 ]).
maplist(G, L1, L2, L3, L4, L5) :- '$swb_maplist'(L1, L2, L3, L4, L5, G).
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
directory_file_path(Dir, File, Path) :- nonvar(Dir), nonvar(File), !, ( ( is_absolute_file_name(File) ; Dir == '.' ) -> Path = File ; sub_atom(Dir, _, _, 0, /) -> atom_concat(Dir, File, Path) ; atomic_list_concat([Dir, /, File], Path) ).
directory_file_path(Dir, File, Path) :- nonvar(Path), !, ( nonvar(Dir) -> ( Dir == '.', \+ is_absolute_file_name(Path) -> File = Path ; sub_atom(Dir, _, _, 0, /) -> atom_concat(Dir, File, Path) ; atom_concat(Dir, /, TheDir) -> atom_concat(TheDir, File, Path) ) ; nonvar(File) -> atom_concat(Dir0, File, Path), '$swb_strip_slash'(Dir0, Dir) ; file_directory_name(Path, Dir), file_base_name(Path, File) ).
directory_file_path(_, _, _) :- throw(error(instantiation_error(_), _)).
'$swb_strip_slash'(Dir0, Dir) :- ( atom_concat(D, /, Dir0), D \== '' -> Dir = D ; Dir = Dir0 ).
make_directory_path(Dir) :- '$swb_mdp'(Dir), !.
make_directory_path(Dir) :- throw(error(permission_error(create, directory, Dir), _)).
'$swb_mdp'(Dir) :- exists_directory(Dir), !.
'$swb_mdp'(Dir) :- atom_concat(RealDir, '/', Dir), RealDir \== '', !, '$swb_mdp'(RealDir).
'$swb_mdp'(Dir) :- Dir \== (/), !, file_directory_name(Dir, Parent), '$swb_mdp'(Parent), E = error(existence_error(directory, _), _), catch(make_directory(Dir), E, ( exists_directory(Dir) -> true ; throw(E) )).
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
fast_term_serialized(T, S) :- var(S), !, '$swb_ftser'(T, S, fast_term_serialized/2).
fast_term_serialized(T, S) :- ( string(S) -> true ; throw(error(type_error(string, S), context(system:fast_term_serialized/2, _))) ), '$swb_ftdes'(S, T).
fast_write(Out, T) :- '$swb_ftser'(T, S, fast_write/2), string_codes(S, Cs), '$swb_u8e'(Cs, Bs), length(Bs, N), number_codes(N, NCs), '$swb_putb'(NCs, Out), put_byte(Out, 46),
    '$swb_putb'(Bs, Out).
fast_read(In, T) :- get_byte(In, B0), ( B0 < 0 -> T = end_of_file ; '$swb_getlen'(In, B0, 0, N), length(Bs, N), '$swb_getb'(Bs, In), '$swb_u8d'(Bs, Cs), string_codes(S, Cs), '$swb_ftdes'(S, T) ).
'$swb_ftser'(T, S, PI) :- term_factorized(T, K, Sb), '$swb_ftblob'(K-Sb, PI), '$swb_ftstr'(K-Sb, KS, [], Ps), with_output_to(string(S), write_canonical('$fts'(KS, Ps))).
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
