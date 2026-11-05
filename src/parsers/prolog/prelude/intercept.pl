/*  intercept.pl -- SWI-Prolog's library(intercept) as a prelude library (hq_prolog 2026-11-04, row prolog-swi-to-100-the-failing-cases-r-1930, swi_tests library/test_intercept). SCRIP's own text, AGPL like the rest of
    SCRIP: not a vendored file (no SWI reader, no renames, helpers '$icpt_'-named by hand; the generator embeds it in LOCAL mode). swipl's send_signal/1 finds its interceptor by walking the call stack with
    prolog_frame_attribute(Frame, parent_goal(_), intercept:do_intercept(...)); SCRIP has no frame visitor, so the active interceptors are a backtrackable global variable, a list with the innermost first.
    '$icpt_do'/4 pushes i(Ball, Handler, Context) with b_setval, runs Goal and pops with b_setval: the interceptor is visible exactly while Goal runs, is visible again when backtracking re-enters Goal, and an
    exception unwinds it. send_signal/1 takes the first entry whose Ball unifies with a COPY of the signal, copy_term(Ball0+Handler0, Signal+Handler), swipl's own test, and calls the handler (with the
    context, uncopied, for intercept/4). intercept_all/4 and nb_intercept_all/4 are swipl's, with the handler and state helpers renamed.
*/
:- module(intercept, [ intercept/3, intercept/4, intercept_all/4, nb_intercept_all/4, send_signal/1, send_silent_signal/1 ]).
intercept(Goal, Ball, Handler) :-
    '$icpt_do'(Goal, Ball, Handler, args).
intercept(Goal, Ball, Handler, Context) :-
    '$icpt_do'(Goal, Ball, Handler, args(Context)).
'$icpt_do'(Goal, Ball, Handler, Context) :-
    '$icpt_get'(Old),
    b_setval('$intercept', [i(Ball, Handler, Context)|Old]),
    call(Goal),
    b_setval('$intercept', Old).
'$icpt_get'(Stack) :-
    catch(b_getval('$intercept', Stack), _, Stack = []).
intercept_all(Template, Goal, Ball, List) :-
    List0 = [_],
    State = list(List0, List0),
    intercept(Goal, Ball, '$icpt_add'(Template), State),
    arg(1, State, [_|List]).
'$icpt_add'(Elem, State) :-
    Tail = [Elem],
    arg(2, State, List),
    setarg(2, List, Tail),
    setarg(2, State, Tail).
nb_intercept_all(Template, Goal, Ball, List) :-
    List0 = [_],
    State = list(List0, List0),
    intercept(Goal, Ball, '$icpt_nb_add'(Template), State),
    arg(1, State, [_|List]).
'$icpt_nb_add'(Elem, State) :-
    duplicate_term(Elem, Copy),
    Tail = [Copy],
    arg(2, State, List),
    nb_linkarg(2, List, Tail),
    nb_linkarg(2, State, Tail).
send_signal(Signal) :-
    '$icpt_nonvar'(Signal),
    '$icpt_get'(Stack),
    (   '$icpt_find'(Stack, Signal, Handler, Context)
    ->  '$icpt_call'(Context, Handler)
    ;   throw(error(unintercepted_signal(Signal), _))
    ).
send_silent_signal(Signal) :-
    '$icpt_nonvar'(Signal),
    '$icpt_get'(Stack),
    (   '$icpt_find'(Stack, Signal, Handler, Context)
    ->  '$icpt_call'(Context, Handler)
    ;   true
    ).
'$icpt_nonvar'(Signal) :-
    (   nonvar(Signal)
    ->  true
    ;   throw(error(instantiation_error, _))
    ).
'$icpt_call'(args, Handler) :-
    call(Handler).
'$icpt_call'(args(A0), Handler) :-
    call(Handler, A0).
'$icpt_find'([i(Ball0, Handler0, Context0)|Rest], Signal, Handler, Context) :-
    (   copy_term(Ball0+Handler0, Signal+Handler)
    ->  Context = Context0
    ;   '$icpt_find'(Rest, Signal, Handler, Context)
    ).
