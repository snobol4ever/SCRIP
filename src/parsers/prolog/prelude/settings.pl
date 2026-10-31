/*  settings.pl -- SWI-Prolog's library(settings) as SCRIP runs it (cqo 2026-10-31, SWI library/test_settings). SCRIP's own text, AGPL like
    the rest of SCRIP: written from swipl 9.0.4's documented behaviour (/usr/lib/swi-prolog/library/settings.pl), not a vendored copy,
    because swipl's library declares a setting through a system:term_expansion of the :- setting/4 directive and SCRIP runs a directive
    in place instead. The generator embeds it in LOCAL mode: no SWI reader, no renames, its helpers '$set_'-named by hand. It is parsed
    only when a program names one of its exports.
    THE MODEL. :- setting(Name, Type, Default, Comment) records the declaration; setting(Name, Value) answers the value set by
    set_setting/2, else the default (one set_setting_default/2 replaced) evaluated: a variable or an atomic default is itself,
    env(Var) and env(Var, Def) read the environment, setting(Other) reads another setting, a numeric type evaluates an arithmetic
    default whose env/1 and env/2 are swipl's arithmetic functions, A+B under type atom concatenates, list(T) evaluates element by
    element. The dynamic facts keep swipl's names and arguments (st_value(Name, Module, Value), st_default(Name, Module, Value),
    local_file(Path), st_modified) so a program that retracts settings:st_value/3 or settings:st_default/3 resets them as in swipl.
    A setting is found by its NAME: the module of the declaring context is recorded but not used to separate two settings of one
    name. The vendored libraries' own :- setting declarations are dropped by the generator, so the ones they read are declared here
    (library(listing)'s five, at swipl 9.0.4's defaults; ARCH-PROLOG-DATABASE section 4). save_settings/1 writes setting(Module:Name, Value) for each value set; load_settings/1 reads such a file back. Left out:
    broadcast/1 of a change, list_settings/0, the setting cache. */
:- module(settings, [ setting/4, setting/2, set_setting/2, set_setting_default/2, restore_setting/1, current_setting/1, save_settings/0,
                      save_settings/1, load_settings/1, load_settings/2 ]).
:- dynamic st_value/3, st_default/3, local_file/1, st_modified/0, '$set_decl'/5.

'$set_builtin'(body_indentation, listing, nonneg, 4, 'Indentation used goals in the body').
'$set_builtin'(tab_distance, listing, nonneg, 0, 'Distance between tab-stops.  0 uses only spaces').
'$set_builtin'(cut_on_same_line, listing, boolean, false, 'Place cuts (!) on the same line').
'$set_builtin'(line_width, listing, nonneg, 78, 'Width of a line.  0 is infinite').
'$set_builtin'(comment_ansi_attributes, listing, list, [fg(green)], 'ansi_format/3 attributes to print comments').

setting(QName, Type, Default, Comment) :-
    strip_module(QName, M, N),
    must_be(atom, N),
    ( '$set_find'(N, _, _, _, _) -> true ; assertz('$set_decl'(N, M, Type, Default, Comment)) ).

'$set_find'(N, M, T, D, C) :- '$set_decl'(N, M, T, D, C).
'$set_find'(N, M, T, D, C) :- '$set_builtin'(N, M, T, D, C).

setting(QName, Value) :-
    strip_module(QName, _, N),
    (   nonvar(N)
    ->  (   st_value(N, _, V0)
        ->  Value = V0
        ;   '$set_curr'(N, M, T, D)
        ->  '$set_eval'(D, M, T, Value)
        ;   throw(error(existence_error(setting, N), _))
        )
    ;   '$set_find'(N, _, _, _, _),
        setting(N, Value)
    ).

current_setting(QName) :- strip_module(QName, _, N), '$set_find'(N, _, _, _, _).

'$set_curr'(N, M, T, D) :- '$set_find'(N, M, T, D0, _), !, ( st_default(N, _, D1) -> D = D1 ; D = D0 ).

set_setting(QName, Value) :-
    strip_module(QName, _, N),
    must_be(atom, N),
    (   '$set_curr'(N, M, T, D0)
    ->  '$set_eval'(D0, M, T, D),
        (   Value == D
        ->  '$set_retract'(N)
        ;   st_value(N, _, V0), V0 == Value
        ->  true
        ;   must_be(T, Value),
            '$set_retract'(N),
            '$set_modified',
            assertz(st_value(N, M, Value))
        )
    ;   throw(error(existence_error(setting, N), _))
    ).

'$set_retract'(N) :- '$set_modified', retractall(st_value(N, _, _)).
'$set_modified' :- st_modified, !.
'$set_modified' :- assertz(st_modified).

restore_setting(QName) :- strip_module(QName, _, N), must_be(atom, N), ( st_value(N, _, _) -> '$set_retract'(N) ; true ).

set_setting_default(QName, Default) :-
    strip_module(QName, _, N),
    must_be(atom, N),
    (   '$set_find'(N, M, T, D0, _)
    ->  retractall(st_default(N, _, _)),
        '$set_retract'(N),
        ( Default == D0 -> true ; assertz(st_default(N, M, Default)) ),
        '$set_eval'(Default, M, T, V),
        set_setting(N, V)
    ;   throw(error(existence_error(setting, N), _))
    ).

'$set_eval'(D, _, _, V) :- var(D), !, V = D.
'$set_eval'(D, _, _, V) :- atomic(D), !, V = D.
'$set_eval'(env(Name), _, T, V) :- !, ( getenv(Name, Text) -> '$set_text'(T, Text, V) ; throw(error(existence_error(environment_variable, Name), _)) ).
'$set_eval'(env(Name, Def), _, T, V) :- !, ( getenv(Name, Text) -> '$set_text'(T, Text, V) ; V = Def ).
'$set_eval'(setting(Name), _, T, V) :- !, setting(Name, V), must_be(T, V).
'$set_eval'(E, _, T, V) :-
    '$set_numeric'(T, B),
    !,
    '$set_arith'(E, E1),
    V0 is E1,
    ( B == float -> V is float(V0) ; B == integer -> V is round(V0) ; V = V0 ).
'$set_eval'(A+B, M, atom, V) :- !, '$set_parts'(A+B, M, L, []), atomic_list_concat(L, V).
'$set_eval'(L, M, list(T), V) :- !, '$set_list'(L, M, T, V).
'$set_eval'(D, _, _, D).

'$set_list'([], _, _, []).
'$set_list'([H0|T0], M, T, [H|R]) :- '$set_eval'(H0, M, T, H), '$set_list'(T0, M, T, R).

'$set_parts'(A+B, M, L0, L) :- !, '$set_parts'(A, M, L0, L1), '$set_parts'(B, M, L1, L).
'$set_parts'(env(Name), _, [X|L], L) :- !, ( getenv(Name, X) -> true ; throw(error(existence_error(environment_variable, Name), _)) ).
'$set_parts'(env(Name, Def), _, [X|L], L) :- !, ( getenv(Name, X) -> true ; X = Def ).
'$set_parts'(setting(Name), _, [X|L], L) :- !, setting(Name, X).
'$set_parts'(A, _, [A|L], L).

'$set_arith'(E, E) :- var(E), !.
'$set_arith'(env(Name), V) :- !, ( getenv(Name, Text) -> '$set_text'(number, Text, V) ; throw(error(existence_error(environment_variable, Name), _)) ).
'$set_arith'(env(Name, Def), V) :- !, ( getenv(Name, Text) -> '$set_text'(number, Text, V) ; V = Def ).
'$set_arith'(E, R) :- compound(E), !, E =.. [F|As], '$set_arith_l'(As, Bs), R =.. [F|Bs].
'$set_arith'(E, E).
'$set_arith_l'([], []).
'$set_arith_l'([A|As], [B|Bs]) :- '$set_arith'(A, B), '$set_arith_l'(As, Bs).

'$set_numeric'(integer, integer).
'$set_numeric'(nonneg, integer).
'$set_numeric'(float, float).
'$set_numeric'(between(L, _), T) :- ( integer(L) -> T = integer ; T = float ).

'$set_text'(T, Text, V) :- '$set_numeric'(T, _), !, atom_number(Text, V).
'$set_text'(number, Text, V) :- !, atom_number(Text, V).
'$set_text'(atom, Text, V) :- !, atom_string(V, Text).
'$set_text'(_, Text, V) :- ( catch(term_to_atom(V0, Text), _, fail) -> V = V0 ; V = Text ).

save_settings :- ( local_file(F) -> save_settings(F) ; save_settings('settings.db') ).
save_settings(File) :-
    setup_call_cleanup(open(File, write, Out), '$set_save'(Out), close(Out)),
    retractall(st_modified).
'$set_save'(Out) :-
    format(Out, '/*  Saved settings~n~n    Do not edit this file manually~n*/~n~n', []),
    forall(st_value(N, M, V), format(Out, '~q.~n', [setting(M:N, V)])).

load_settings(File) :- load_settings(File, []).
load_settings(File, _) :-
    retractall(local_file(_)),
    assertz(local_file(File)),
    (   exists_file(File)
    ->  setup_call_cleanup(open(File, read, In), '$set_load'(In), close(In))
    ;   true
    ).
'$set_load'(In) :- read_term(In, T, []), ( T == end_of_file -> true ; '$set_load1'(T), '$set_load'(In) ).
'$set_load1'(setting(Q, V)) :- !, strip_module(Q, M, N), retractall(st_value(N, _, _)), assertz(st_value(N, M, V)).
'$set_load1'(_).
