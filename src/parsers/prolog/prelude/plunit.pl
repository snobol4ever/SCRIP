/*  plunit.pl -- library(plunit) for a program that carries its own unit tests (the cto's ARCH-PROLOG-DATABASE.md section 10
    item 1; ProRosetta catamorphism-6, json-pointer-1, sort-disjoint-sublist-3, test-a-function).  SCRIP's own text, AGPL like the
    rest of SCRIP: not a vendored file.  The generator embeds it in LOCAL mode: no SWI reader, no renames, its helpers pj_-named.
    It is parsed only when a program names one of its exports.

    THE MECHANISM is the SWI runner's shim (corpus/tests/prolog/plunit.pl): the parse stage turns a unit's test/1 and test/2
    clauses between begin_tests/1,2 and end_tests/1 into pj_test(Unit, Name, Options, Body) facts, and run_tests/0,1 grades each
    test alone, its bindings undone before the next, by plunit's options (fail, true(E), A Cmp B, all(T Cmp L), error(E),
    throws(B), forall(G), setup(S), cleanup(C), condition(C), blocked(W)).  THE CONVENTIONS are swipl 9.0.4's, the oracle of its
    output: the report goes to user_error, never to the program's output, and run_tests/0,1 FAILS when a test failed (measured:
    a unit with one failing test of three prints "% 1 test failed" on stderr and run_tests(u) fails).  The shim stays the SWI
    runner's own: it grades by its stdout lines, and a file consulted before the program defines these names itself.
*/
:- module(plunit, [begin_tests/1, begin_tests/2, end_tests/1, run_tests/0, run_tests/1, set_test_options/1]).
:- dynamic pj_test/4.
begin_tests(Suite) :- pj_unit_open(Suite), pj_suites_add(Suite, []).
begin_tests(Suite, Opts) :- pj_unit_open(Suite), pj_suites_add(Suite, Opts).
end_tests(_) :- '$set_source_module'(user).
pj_unit_open(Suite) :- atom_concat(plunit_, Suite, M), '$set_source_module'(M).
pj_suites(L) :- ( nb_current(pj_suites, L0) -> L = L0 ; L = [] ).
pj_suites_add(Suite, Opts) :- pj_suites(Old), nb_setval(pj_suites, [Suite-Opts|Old]).
set_test_options(_).
run_tests :- pj_suites(Sx), pj_reverse(Sx, Ss), pj_pairs_names(Ss, Names), pj_run(Names).
run_tests(L) :- is_list(L), !, pj_run(L).
run_tests(S) :- pj_run([S]).
pj_pairs_names([], []).
pj_pairs_names([S-_|T], [S|R]) :- pj_pairs_names(T, R).
pj_run(Suites) :-
    nb_setval(pj_p, 0), nb_setval(pj_f, 0),
    current_output(Out),
    set_output(user_error),
    catch(pj_run_list(Suites), E, (set_output(Out), throw(E))),
    nb_getval(pj_p, P), nb_getval(pj_f, F),
    pj_summary(P, F),
    set_output(Out),
    F =:= 0.
pj_summary(P, 0) :- !, ( P =:= 1 -> format('% test passed~n') ; format('% All ~w tests passed~n', [P]) ).
pj_summary(P, F) :- pj_s(F, SF), pj_s(P, SP), format('% ~w test~w failed~n% ~w test~w passed~n', [F, SF, P, SP]).
pj_s(1, '') :- !.
pj_s(_, s).
pj_run_list([]).
pj_run_list([S|T]) :- ( pj_run_suite(S) -> true ; true ), !, pj_run_list(T).
pj_run_suite(Suite) :-
    format('% PL-Unit: ~w ', [Suite]),
    pj_unit_opt(Suite, setup),
    findall(t(N, O, G), pj_test(Suite, N, O, G), Tests),
    ( pj_run_tests(Suite, Tests) -> true ; true ),
    pj_unit_opt(Suite, cleanup),
    format(' done~n'), !.
pj_unit_opt(Suite, Kind) :-
    pj_suites(L),
    ( memberchk(Suite-Opts0, L) -> true ; Opts0 = [] ),
    ( is_list(Opts0) -> Opts = Opts0 ; Opts = [Opts0] ),
    Opt =.. [Kind, G],
    ( memberchk(Opt, Opts) -> ( catch(G, E, (format('~nunit ~w ~w raised ~q~n', [Suite, Kind, E]), fail)) -> true ; format('~nunit ~w ~w failed~n', [Suite, Kind]) ) ; true ).
pj_run_tests(_, []).
pj_run_tests(Suite, [t(N0, O, G)|Rest]) :- pj_name_text(N0, N), pj_run_verdict(Suite, N, O, G), pj_run_tests(Suite, Rest).
pj_run_verdict(Suite, N, O, G) :- \+ \+ once(pj_run_one(Suite, N, O, G)), !.
pj_run_verdict(Suite, N, _, _) :- pj_fail(Suite, N, 'no verdict').
pj_pass :- nb_getval(pj_p, N), N1 is N + 1, nb_setval(pj_p, N1), format('.').
pj_fail(Suite, Name, Why) :- nb_getval(pj_f, N), N1 is N + 1, nb_setval(pj_f, N1), format('~n~ttest ~w:~w: ~w~n', [Suite, Name, Why]).
pj_fail_err(Suite, Name, E) :- pj_err_text(E, T), format(atom(W), 'received error: ~q', [T]), pj_fail(Suite, Name, W).
pj_name_text(N0, N) :- pj_is_codes(N0), !, atom_codes(N, N0).
pj_name_text(N, N).
pj_is_codes([C|T]) :- integer(C), C >= 0, pj_is_codes_t(T).
pj_is_codes_t([]).
pj_is_codes_t([C|T]) :- integer(C), C >= 0, pj_is_codes_t(T).
pj_has_blocked([blocked(_)|_]).
pj_has_blocked([_|T]) :- pj_has_blocked(T).
pj_has_blocked(blocked(_)).
pj_wants_fail([fail|_]).
pj_wants_fail([false|_]).
pj_wants_fail([_|T]) :- pj_wants_fail(T).
pj_wants_fail(fail).
pj_wants_fail(false).
pj_has_error([error(E)|_], E).
pj_has_error([_|T], E) :- pj_has_error(T, E).
pj_has_error(error(E), E).
pj_has_throws([throws(T)|_], T).
pj_has_throws([error(F, C)|_], error(F, C)).
pj_has_throws([_|T2], T) :- pj_has_throws(T2, T).
pj_has_throws(throws(T), T).
pj_has_throws(error(F, C), error(F, C)).
pj_has_true([true(E)|_], E).
pj_has_true([C|_], C) :- compound(C), functor(C, Op, 2), pj_all_op(Op).
pj_has_true([_|T], E) :- pj_has_true(T, E).
pj_has_true(true(E), E).
pj_has_true(C, C) :- compound(C), functor(C, Op, 2), pj_all_op(Op).
pj_has_all([all(E)|_], E).
pj_has_all([_|T], E) :- pj_has_all(T, E).
pj_has_all(all(E), E).
pj_has_forall([forall(G)|_], G).
pj_has_forall([_|T], G) :- pj_has_forall(T, G).
pj_take_setup(Opts0, S, C, Rest) :-
    ( is_list(Opts0) -> Opts = Opts0 ; Opts = [Opts0] ),
    ( memberchk(setup(_), Opts) ; memberchk(cleanup(_), Opts) ), !,
    ( memberchk(setup(S0), Opts) -> S = S0 ; S = true ),
    ( memberchk(cleanup(C0), Opts) -> C = C0 ; C = true ),
    pj_del_setup(Opts, Rest).
pj_del_setup([], []).
pj_del_setup([setup(_)|T], R) :- !, pj_del_setup(T, R).
pj_del_setup([cleanup(_)|T], R) :- !, pj_del_setup(T, R).
pj_del_setup([H|T], [H|R]) :- pj_del_setup(T, R).
pj_del_forall([], []).
pj_del_forall([forall(_)|T], R) :- !, pj_del_forall(T, R).
pj_del_forall([H|T], [H|R]) :- pj_del_forall(T, R).
pj_skip_cond(Opts) :- is_list(Opts), member(condition(C), Opts), \+ catch(C, _, fail).
pj_run_one(Suite, Name, Opts, Goal) :- pj_has_forall(Opts, Gen), !,
    pj_del_forall(Opts, Rest),
    catch(forall(Gen, once(pj_run_one(Suite, Name, Rest, Goal))), E, pj_fail_err(Suite, Name, E)).
pj_run_one(_, _, Opts, _) :- pj_has_blocked(Opts), !.
pj_run_one(_, _, Opts, _) :- pj_skip_cond(Opts), !.
pj_run_one(Suite, Name, Opts, Goal) :- pj_take_setup(Opts, S, C, Rest), !,
    catch((once(S) -> R = ok ; R = failed), E, R = raised(E)),
    (   R == ok
    ->  pj_run_one(Suite, Name, Rest, Goal)
    ;   R = raised(E1)
    ->  pj_err_text(E1, T), format(atom(W), 'setup failed: received error: ~q', [T]), pj_fail(Suite, Name, W)
    ;   pj_fail(Suite, Name, 'setup failed')
    ),
    ignore(catch(C, _, true)).
pj_run_one(Suite, Name, Opts, Goal) :- pj_has_error(Opts, E), !, pj_do_error(Suite, Name, Goal, E).
pj_run_one(Suite, Name, Opts, Goal) :- pj_has_throws(Opts, T), !, pj_do_throw(Suite, Name, Goal, T).
pj_run_one(Suite, Name, Opts, Goal) :- pj_wants_fail(Opts), !, pj_do_fail(Suite, Name, Goal).
pj_run_one(Suite, Name, Opts, Goal) :- pj_has_true(Opts, Expr), !, pj_do_true(Suite, Name, Goal, Expr).
pj_run_one(Suite, Name, Opts, Goal) :- pj_has_all(Opts, AE), !, pj_do_all(Suite, Name, Goal, AE).
pj_run_one(Suite, Name, _, Goal) :- pj_do_succeed(Suite, Name, Goal).
pj_do_succeed(Suite, Name, Goal) :- catch((Goal, R = ok), E, R = err(E)), !, pj_do_succeed_r(Suite, Name, R).
pj_do_succeed(Suite, Name, _) :- pj_fail(Suite, Name, failed).
pj_do_succeed_r(_, _, ok) :- pj_pass.
pj_do_succeed_r(Suite, Name, err(E)) :- pj_fail_err(Suite, Name, E).
pj_do_fail(Suite, Name, Goal) :- catch((Goal, R = ok), E, R = err(E)), !, pj_do_fail_r(Suite, Name, R).
pj_do_fail(_, _, _) :- pj_pass.
pj_do_fail_r(Suite, Name, ok) :- pj_fail(Suite, Name, 'succeeded, should have failed').
pj_do_fail_r(Suite, Name, err(E)) :- pj_fail_err(Suite, Name, E).
pj_err_text(error(F, _), T) :- !, T = F.
pj_err_text(E, E).
pj_do_error(Suite, Name, Goal, Exp) :-
    catch((Goal, Ok = true), B, pj_err_ball(Suite, Name, Exp, B)), !,
    ( Ok == true -> pj_fail(Suite, Name, 'no exception') ; true ).
pj_do_error(Suite, Name, _, _) :- pj_fail(Suite, Name, 'no exception').
pj_err_ball(Suite, Name, Exp, B) :-
    (   nonvar(B), B = error(Act, _)
    ->  copy_term(Exp, ExpC), ( subsumes_term(ExpC, Act) -> pj_pass ; format(atom(W), 'wrong error: ~q', [Act]), pj_fail(Suite, Name, W) )
    ;   format(atom(W), 'received ~q', [B]), pj_fail(Suite, Name, W)
    ).
pj_do_throw(Suite, Name, Goal, Exp) :-
    catch((Goal, Ok = true), Act, ( Act = Exp -> pj_pass ; format(atom(W), 'wrong exception: ~q', [Act]), pj_fail(Suite, Name, W) )), !,
    ( Ok == true -> pj_fail(Suite, Name, 'no exception') ; true ).
pj_do_throw(Suite, Name, _, _) :- pj_fail(Suite, Name, 'no exception').
pj_do_true(Suite, Name, Goal, Expr) :-
    catch(Goal, E, (pj_fail_err(Suite, Name, E), fail)), !,
    ( catch(Expr, _, fail) -> pj_pass ; format(atom(W), 'wrong answer: ~q', [Expr]), pj_fail(Suite, Name, W) ).
pj_do_true(Suite, Name, _, _) :- pj_fail(Suite, Name, failed).
pj_do_all(Suite, Name, Goal, Cmp) :-
    Cmp =.. [Op, Var, Expected], pj_all_op(Op), !,
    catch((findall(Var, Goal, Actual), R = ok), E, R = err(E)),
    pj_do_all_r(Suite, Name, R, Op, Actual, Expected).
pj_all_op('==').
pj_all_op('=:=').
pj_all_op('=').
pj_all_op('=@=').
pj_all_cmp([], [], _).
pj_all_cmp([E|ET], [V|VT], Op) :- G =.. [Op, E, V], G, pj_all_cmp(ET, VT, Op).
pj_do_all_r(Suite, Name, ok, Op, Actual, Expected) :-
    ( catch(pj_all_cmp(Expected, Actual, Op), _, fail) -> pj_pass ; format(atom(W), 'wrong answer: ~q', [Actual]), pj_fail(Suite, Name, W) ).
pj_do_all_r(Suite, Name, err(E), _, _, _) :- pj_fail_err(Suite, Name, E).
pj_reverse(L, R) :- pj_rev(L, [], R).
pj_rev([], R, R).
pj_rev([H|T], A, R) :- pj_rev(T, [H|A], R).
