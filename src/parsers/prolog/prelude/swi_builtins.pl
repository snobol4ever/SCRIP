/*  swi_builtins.pl -- SWI-PROLOG BUILTINS THAT SCRIP LACKS, AS A PROLOG LAYER (hq_prolog 2026-10-15, row prolog-swi-tabling-the-xsb-loader-runs-the-xsb-test-drivers and the SWI census).
    SCRIP's own text, AGPL like the rest of SCRIP: not a vendored file; the generator (util_gen_prolog_prelude_libs.py) embeds it in LOCAL mode, no SWI reader, no renames, its helpers
    '$swb_'-named by hand. It is parsed only when a program names one of its exports. directory_file_path/3 and make_directory_path/1 are swipl 9.0.4's library(filesex) clauses
    (BSD, Jan Wielemaker, VU University Amsterdam) with their foreign-library dependency removed; the rest are written here from the documented behaviour and graded against swipl by
    test_gate_pl_swi_file_and_text_builtins_answer_as_swipl.sh. */
:- module(swi_builtins, [ open_string/2, directory_file_path/3, make_directory_path/1, exists_file/1, exists_directory/1, size_file/2, time_file/2, same_file/2, working_directory/2,
                          setenv/2, unsetenv/1, text_to_string/2 ]).
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
