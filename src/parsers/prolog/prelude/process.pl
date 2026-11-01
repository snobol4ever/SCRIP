/*  process.pl -- SWI-Prolog 9.0.4's library(process) as a Prolog layer over SCRIP's process leaves (the cao, 2026-11-01; ARCH-PROLOG-DATABASE
    section 18.3).  SCRIP's own text, AGPL like the rest of SCRIP, in the generator's LOCAL mode: parsed only when a program names an export.

    THE ORDER, measured on swipl 9.0.4: the executable is resolved BEFORE the options are read, so a bare name that is no file raises
    existence_error(source_sink, Exe) even when an option is bad; path(Name) searches PATH for an executable file; an atom or string is a
    file name taken as given, NEVER searched.  Then the options: stdin/stdout/stderr(std | null | pipe(S) | pipe(S, [type(T), encoding(E)])
    | stream(S)), process(Pid), detached(Bool), cwd(Dir), env(List) (the whole environment) and environment(List) (added to the inherited
    one), window(_) ignored; an unknown pipe option is domain_error(pipe_option, O), a bad type domain_error(stream_type, T).  The leaf
    '$proc_create' forks; in the child each channel is dup2'ed onto 0, 1 or 2 and every other descriptor of the parent closes at the exec
    (the pipes are made close-on-exec), so a child reading its stdin sees end of file when the parent closes its end.  WITHOUT process(Pid)
    and without a pipe the call waits and a status other than exit(0) raises process_error(Exe, Status).  process_wait/2,3 answer exit(N),
    killed(Signal) or, under timeout(T), timeout; the wait takes the goal interrupt of call_with_time_limit/2 at EINTR.  */
:- module(process, [ process_create/3, process_wait/2, process_wait/3, process_id/1, process_id/2, is_process/1, process_release/1,
                     process_kill/1, process_kill/2, process_group_kill/1, process_group_kill/2 ]).

process_create(Exe, Args, Options) :-
    '$pc_exe'(Exe, Path),
    '$pc_args'(Args, Argv),
    ( is_list(Options) -> true ; throw(error(type_error(list, Options), _)) ),
    '$pc_opts'(Options, pc(ch(std, _), ch(std, _), ch(std, _), none, '', [], false), pc(I, O, E, Pid, Cwd, Env, Det)),
    I = ch(IS, IV), O = ch(OS, OV), E = ch(ES, EV),
    '$proc_create'(Path, Argv, io(IS, OS, ES), Cwd, Env, Det, R),
    ( R = '$oserr'(_, M) -> throw(error(process_error(Exe, M), _)) ; true ),
    R = pr(P, SI, SO, SE),
    '$pc_bind'(IS, IV, SI), '$pc_bind'(OS, OV, SO), '$pc_bind'(ES, EV, SE),
    (   Pid = pid(P0) -> P0 = P
    ;   '$pc_piped'(IS, OS, ES) -> true
    ;   '$proc_wait'(P, -1, St), ( St == exit(0) -> true ; throw(error(process_error(Exe, St), _)) )
    ).
'$pc_piped'(I, O, E) :- ( I = pipe(_) ; O = pipe(_) ; E = pipe(_) ), !.
'$pc_bind'(pipe(_), V, S) :- !, V = S.
'$pc_bind'(_, _, _).

'$pc_exe'(E, _) :- var(E), !, throw(error(instantiation_error, _)).
'$pc_exe'(path(N), P) :- !, ( '$pc_search'(N, P) -> true ; throw(error(existence_error(source_sink, path(N)), _)) ).
'$pc_exe'(E, P) :- ( atom(E) -> P = E ; string(E) -> atom_string(P, E) ; throw(error(type_error(source_sink, E), _)) ),
    ( exists_file(P), access_file(P, execute) -> true ; throw(error(existence_error(source_sink, E), _)) ).
'$pc_search'(N, P) :-
    ( atom(N) -> NA = N ; string(N) -> atom_string(NA, N) ; fail ),
    getenv('PATH', PS), atomic_list_concat(Ds, ':', PS), member(D, Ds), D \== '',
    atomic_list_concat([D, '/', NA], P), exists_file(P), access_file(P, execute), !.

'$pc_args'(A, _) :- var(A), !, throw(error(instantiation_error, _)).
'$pc_args'(A, Argv) :- is_list(A), !, '$pc_texts'(A, Ts), Argv =.. [argv|Ts].
'$pc_args'(A, _) :- throw(error(type_error(list, A), _)).
'$pc_texts'([], []).
'$pc_texts'([X|Xs], [T|Ts]) :- '$pc_text'(X, T), '$pc_texts'(Xs, Ts).
'$pc_text'(X, _) :- var(X), !, throw(error(instantiation_error, _)).
'$pc_text'(file(F), T) :- !, '$pc_text'(F, T).
'$pc_text'(X, X) :- atom(X), !.
'$pc_text'(X, T) :- string(X), !, atom_string(T, X).
'$pc_text'(X, T) :- number(X), !, atom_number(T, X).
'$pc_text'(X, T) :- is_list(X), !, '$pc_texts'(X, Ps), atomic_list_concat(Ps, T).
'$pc_text'(X, _) :- throw(error(type_error(text, X), _)).

'$pc_opts'([], S, S).
'$pc_opts'([O|Os], S0, S) :- '$pc_opt'(O, S0, S1), '$pc_opts'(Os, S1, S).
'$pc_opt'(O, _, _) :- var(O), !, throw(error(instantiation_error, _)).
'$pc_opt'(stdin(X), pc(_, O, E, P, C, V, D), pc(I, O, E, P, C, V, D)) :- !, '$pc_chan'(X, I).
'$pc_opt'(stdout(X), pc(I, _, E, P, C, V, D), pc(I, O, E, P, C, V, D)) :- !, '$pc_chan'(X, O).
'$pc_opt'(stderr(X), pc(I, O, _, P, C, V, D), pc(I, O, E, P, C, V, D)) :- !, '$pc_chan'(X, E).
'$pc_opt'(process(P), pc(I, O, E, _, C, V, D), pc(I, O, E, pid(P), C, V, D)) :- !.
'$pc_opt'(detached(D), pc(I, O, E, P, C, V, _), pc(I, O, E, P, C, V, D)) :- !.
'$pc_opt'(cwd(X), pc(I, O, E, P, _, V, D), pc(I, O, E, P, C, V, D)) :- !, '$pc_text'(X, C).
'$pc_opt'(env(L), pc(I, O, E, P, C, _, D), pc(I, O, E, P, C, replace(T), D)) :- !, '$pc_env'(L, T).
'$pc_opt'(environment(L), pc(I, O, E, P, C, _, D), pc(I, O, E, P, C, add(T), D)) :- !, '$pc_env'(L, T).
'$pc_opt'(window(_), S, S) :- !.
'$pc_opt'(priority(_), S, S) :- !.
'$pc_opt'(O, _, _) :- throw(error(domain_error(process_option, O), _)).
'$pc_env'(L, T) :- '$pc_kvs'(L, As), T =.. [env|As].
'$pc_kvs'([], []).
'$pc_kvs'([K=V|L], [A|As]) :- !, '$pc_text'(K, KT), '$pc_text'(V, VT), atomic_list_concat([KT, '=', VT], A), '$pc_kvs'(L, As).
'$pc_kvs'([X|_], _) :- throw(error(type_error(environment_variable, X), _)).

'$pc_chan'(X, _) :- var(X), !, throw(error(instantiation_error, _)).
'$pc_chan'(std, ch(std, _)) :- !.
'$pc_chan'(null, ch(null, _)) :- !.
'$pc_chan'(pipe(S), ch(pipe(text), S)) :- !.
'$pc_chan'(pipe(S, Os), ch(pipe(T), S)) :- !, '$pc_popts'(Os, text, T).
'$pc_chan'(stream(S), ch(fd(S), _)) :- !.
'$pc_chan'(X, _) :- throw(error(domain_error(process_stream, X), _)).
'$pc_popts'(Os, _, _) :- var(Os), !, throw(error(instantiation_error, _)).
'$pc_popts'([], T, T) :- !.
'$pc_popts'([type(X)|Os], _, T) :- !, ( X == text -> T1 = text ; X == binary -> T1 = binary ; throw(error(domain_error(stream_type, X), _)) ), '$pc_popts'(Os, T1, T).
'$pc_popts'([encoding(_)|Os], T0, T) :- !, '$pc_popts'(Os, T0, T).
'$pc_popts'([O|_], _, _) :- !, throw(error(domain_error(pipe_option, O), _)).
'$pc_popts'(Os, _, _) :- throw(error(type_error(list, Os), _)).

process_wait(P, S) :- process_wait(P, S, []).
process_wait(P, S, Os) :-
    ( var(P) -> throw(error(instantiation_error, _)) ; integer(P) -> true ; throw(error(type_error(process, P), _)) ),
    ( memberchk(timeout(T), Os) -> ( T == infinite -> T1 = -1 ; number(T) -> T1 = T ; throw(error(type_error(timeout, T), _)) ) ; T1 = -1 ),
    '$proc_wait'(P, T1, R),
    ( R = '$oserr'(echild, _) -> throw(error(existence_error(process, P), _)) ; R = '$oserr'(_, M) -> throw(error(system_error(M), _)) ; S = R ).

process_kill(P) :- process_kill(P, term).
process_kill(P, Sig) :-
    '$pc_sig'(Sig, N), '$proc_kill'(P, N, R),
    ( R == ok -> true ; R = '$oserr'(esrch, _) -> throw(error(existence_error(process, P), _)) ; R = '$oserr'(_, M) -> throw(error(system_error(M), _)) ).
process_group_kill(P) :- process_group_kill(P, term).
process_group_kill(P, Sig) :- '$pc_sig'(Sig, N), G is -P, '$proc_kill'(G, N, R), ( R == ok -> true ; throw(error(existence_error(process, P), _)) ).
'$pc_sig'(S, _) :- var(S), !, throw(error(instantiation_error, _)).
'$pc_sig'(S, S) :- integer(S), !.
'$pc_sig'(S, N) :- '$pc_signo'(S, N), !.
'$pc_sig'(S, _) :- throw(error(domain_error(signal, S), _)).
'$pc_signo'(hup, 1).
'$pc_signo'(int, 2).
'$pc_signo'(quit, 3).
'$pc_signo'(kill, 9).
'$pc_signo'(usr1, 10).
'$pc_signo'(usr2, 12).
'$pc_signo'(pipe, 13).
'$pc_signo'(alrm, 14).
'$pc_signo'(term, 15).
'$pc_signo'(cont, 18).
'$pc_signo'(stop, 19).

process_id(P) :- current_prolog_flag(pid, P).
process_id(P, P) :- integer(P).
is_process(P) :- integer(P), P > 0, '$proc_kill'(P, 0, ok).
process_release(_).
