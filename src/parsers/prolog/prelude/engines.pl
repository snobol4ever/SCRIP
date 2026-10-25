/*  engines.pl -- SWI-Prolog's engines as a prelude library (hq_packages 2026-10-25, ceo CEO-1960/1964/1966; the cto's design: ARCH-PROLOG-THREADS.md sec 4).  SCRIP's own text, AGPL like the rest,
    not a vendored file: no SWI reader, no renames, helpers '$eng_'-named by hand (the generator embeds it in LOCAL mode).  An engine IS a Prolog generator context: engine_create copies Templ-Goal and
    starts a coroutine whose body is '$engine_body'/2 on the generator road ('$co_*' road's cousin, rt_proc_call_gen_h), on its OWN trail arena, so a caller that backtracks never undoes an engine's
    bindings and an engine that backtracks into its goal never undoes the caller's.  Five leaves ('$engine_new', '$engine_next', '$engine_yield', '$engine_destroy', '$engine_self') drive it; everything
    else is here.  The registry, the aliases and the mailboxes are global variables (root cells): '$eng_<Id>' holds e(Addr, Tr, Slab, Alias, Limit, State), '$engal_<Alias>' an alias's Id, '$engmb_<Id>' the
    mailbox, '$engine_live' the live ids, '$engine_seq' the next id.  An engine handle is the alias atom or '$engine'(Id).  A body answer or yield is copied to the heap before it crosses the switch
    (findall's own rule); an exception in the body crosses as the data throw(Ball) and is re-thrown by engine_next on the caller's stack.
*/
:- module(engines, [ engine_create/3, engine_create/4, engine_next/2, engine_next_reified/2, engine_post/2, engine_post/3, engine_yield/1, engine_fetch/1, engine_destroy/1, engine_self/1,
                     current_engine/1, is_engine/1 ]).
engine_create(Templ, Goal, E) :-
    engine_create(Templ, Goal, E, []).
engine_create(Templ, Goal, E, Options) :-
    '$eng_options'(Options, Alias, Limit),
    (   Alias \== none, '$eng_alias_get'(Alias, _)
    ->  throw(error(permission_error(create, engine, Alias), context(engine_create/4, _)))
    ;   true
    ),
    copy_term(Templ-Goal, T2-G2),
    '$engine_new'('$engine_body', T2, G2, Addr, Tr, Slab),
    '$eng_next_id'(Id),
    '$eng_put'(Id, e(Addr, Tr, Slab, Alias, Limit, live)),
    '$eng_live_add'(Id),
    (   Alias == none
    ->  E = '$engine'(Id)
    ;   E = Alias,
        '$eng_alias_put'(Alias, Id)
    ).
'$eng_options'(Options, _, _) :-
    var(Options),
    !,
    throw(error(instantiation_error, context(engine_create/4, _))).
'$eng_options'([], none, none) :-
    !.
'$eng_options'([O|Os], Alias, Limit) :-
    !,
    '$eng_options'(Os, Alias0, Limit0),
    '$eng_option'(O, Alias0, Limit0, Alias, Limit).
'$eng_options'(Options, _, _) :-
    throw(error(type_error(list, Options), context(engine_create/4, _))).
'$eng_option'(O, _, _, _, _) :-
    var(O),
    !,
    throw(error(instantiation_error, context(engine_create/4, _))).
'$eng_option'(alias(A), _, L, A, L) :-
    !,
    (   atom(A)
    ->  true
    ;   throw(error(type_error(atom, A), context(engine_create/4, _)))
    ).
'$eng_option'(stack_limit(L), A, _, A, L) :-
    !.
'$eng_option'(stack(L), A, _, A, L) :-
    !.
'$eng_option'(_, A, L, A, L).
engine_next(E, Term) :-
    '$eng_rec'(E, Id, e(Addr, _, _, _, Limit, State)),
    (   State == live
    ->  '$eng_step'(Addr, Limit, R),
        '$eng_after'(R, Id, Term)
    ;   throw(error(existence_error(engine, E), context(system:engine_next/2, _)))
    ).
'$eng_step'(Addr, none, R) :-
    !,
    '$engine_next'(Addr, R).
'$eng_step'(Addr, Limit, R) :-
    current_prolog_flag(stack_limit, Old),
    set_prolog_flag(stack_limit, Limit),
    '$engine_next'(Addr, R),
    set_prolog_flag(stack_limit, Old).
'$eng_after'(answer(C), _, T) :-
    !,
    T = C.
'$eng_after'(yield(C), _, T) :-
    !,
    T = C.
'$eng_after'(throw(B), Id, _) :-
    !,
    '$eng_finish'(Id),
    throw(B).
'$eng_after'(_, Id, _) :-
    '$eng_finish'(Id),
    fail.
'$eng_finish'(Id) :-
    '$eng_get'(Id, e(Addr, Tr, Slab, Alias, Limit, _)),
    '$engine_destroy'(Addr, Tr, Slab),
    '$eng_put'(Id, e(0, 0, 0, Alias, Limit, done)).
engine_next_reified(E, T) :-
    (   catch(engine_next(E, A), B, T = exception(B))
    ->  (   nonvar(T)
        ->  true
        ;   T = the(A)
        )
    ;   T = no
    ).
engine_post(E, Term) :-
    '$eng_rec'(E, Id, _),
    (   '$eng_mb_full'(Id)
    ->  throw(error(permission_error(post_to, engine, E), context(engine_post/2, 'Already has a term')))
    ;   copy_term(Term, C),
        '$eng_mb_put'(Id, C)
    ).
engine_post(E, Term, Reply) :-
    engine_post(E, Term),
    engine_next(E, Reply).
engine_yield(Term) :-
    (   '$engine_self'(_)
    ->  copy_term(Term, C),
        '$engine_yield'(yield(C))
    ;   throw(error(permission_error(execute, vmi, 'I_YIELD'), context(system:engine_yield/1, _)))
    ).
engine_fetch(Term) :-
    (   '$engine_self'(Addr)
    ->  true
    ;   throw(error(existence_error(term, delivery, main), context(system:engine_fetch/1, _)))
    ),
    '$eng_by_addr'(Addr, Id),
    (   '$eng_mb_take'(Id, T0)
    ->  Term = T0
    ;   '$eng_handle'(Id, E),
        throw(error(existence_error(term, delivery, E), context(engine_fetch/1, _)))
    ).
engine_destroy(E) :-
    '$eng_rec'(E, Id, e(Addr, Tr, Slab, Alias, _, State)),
    (   State == live
    ->  '$engine_destroy'(Addr, Tr, Slab)
    ;   true
    ),
    '$eng_forget'(Id, Alias).
engine_self(E) :-
    '$engine_self'(Addr),
    '$eng_by_addr'(Addr, Id),
    '$eng_handle'(Id, E).
current_engine(E) :-
    '$eng_live'(Ids),
    '$eng_member'(Id, Ids),
    '$eng_get'(Id, _),
    '$eng_handle'(Id, E).
is_engine(E) :-
    nonvar(E),
    '$eng_try_id'(E, Id),
    '$eng_get'(Id, _).
'$eng_member'(X, [X|_]).
'$eng_member'(X, [_|T]) :-
    '$eng_member'(X, T).
'$eng_rec'(E, Id, Rec) :-
    '$eng_id'(E, Id),
    (   '$eng_get'(Id, Rec0)
    ->  Rec = Rec0
    ;   throw(error(existence_error(engine, E), _))
    ).
'$eng_id'(E, _) :-
    var(E),
    !,
    throw(error(instantiation_error, _)).
'$eng_id'(E, Id) :-
    (   '$eng_try_id'(E, Id0)
    ->  Id = Id0
    ;   atom(E)
    ->  throw(error(existence_error(engine, E), _))
    ;   throw(error(type_error(engine, E), _))
    ).
'$eng_try_id'('$engine'(Id), Id) :-
    integer(Id),
    !.
'$eng_try_id'(A, Id) :-
    atom(A),
    '$eng_alias_get'(A, Id).
'$eng_handle'(Id, E) :-
    '$eng_get'(Id, e(_, _, _, Alias, _, _)),
    (   Alias == none
    ->  E = '$engine'(Id)
    ;   E = Alias
    ).
'$eng_by_addr'(Addr, Id) :-
    '$eng_live'(Ids),
    '$eng_member'(Id, Ids),
    '$eng_get'(Id, e(Addr, _, _, _, _, _)),
    !.
'$eng_key'(Prefix, Id, Key) :-
    (   integer(Id)
    ->  number_codes(Id, Cs),
        atom_codes(IdA, Cs)
    ;   IdA = Id
    ),
    atom_concat(Prefix, IdA, Key).
'$eng_get'(Id, Rec) :-
    '$eng_key'('$eng_', Id, K),
    catch(nb_getval(K, V), _, fail),
    V \== none,
    Rec = V.
'$eng_put'(Id, Rec) :-
    '$eng_key'('$eng_', Id, K),
    nb_setval(K, Rec).
'$eng_next_id'(Id) :-
    catch(nb_getval('$engine_seq', N0), _, N0 = 0),
    Id is N0 + 1,
    nb_setval('$engine_seq', Id).
'$eng_live'(Ids) :-
    catch(nb_getval('$engine_live', Ids0), _, Ids0 = []),
    Ids = Ids0.
'$eng_live_add'(Id) :-
    '$eng_live'(Ids),
    nb_setval('$engine_live', [Id|Ids]).
'$eng_live_del'(Id, Ids0, Ids) :-
    '$eng_del'(Ids0, Id, Ids).
'$eng_del'([], _, []).
'$eng_del'([X|T], Id, R) :-
    (   X == Id
    ->  R = T
    ;   R = [X|R1],
        '$eng_del'(T, Id, R1)
    ).
'$eng_alias_get'(Alias, Id) :-
    '$eng_key'('$engal_', Alias, K),
    catch(nb_getval(K, V), _, fail),
    V \== none,
    Id = V.
'$eng_alias_put'(Alias, Id) :-
    '$eng_key'('$engal_', Alias, K),
    nb_setval(K, Id).
'$eng_forget'(Id, Alias) :-
    '$eng_key'('$eng_', Id, K),
    nb_setval(K, none),
    '$eng_key'('$engmb_', Id, M),
    nb_setval(M, empty),
    (   Alias == none
    ->  true
    ;   '$eng_key'('$engal_', Alias, A),
        nb_setval(A, none)
    ),
    '$eng_live'(Ids0),
    '$eng_live_del'(Id, Ids0, Ids),
    nb_setval('$engine_live', Ids).
'$eng_mb_full'(Id) :-
    '$eng_key'('$engmb_', Id, K),
    catch(nb_getval(K, V), _, fail),
    V = full(_).
'$eng_mb_put'(Id, T) :-
    '$eng_key'('$engmb_', Id, K),
    nb_setval(K, full(T)).
'$eng_mb_take'(Id, T) :-
    '$eng_key'('$engmb_', Id, K),
    catch(nb_getval(K, V), _, fail),
    V = full(T0),
    nb_setval(K, empty),
    T = T0.
'$engine_body'(T, G) :-
    '$engine_yield'(created),
    catch(( call(G), copy_term(T, C), '$engine_yield'(answer(C)), fail ; true ), B, ( copy_term(B, CB), '$engine_yield'(throw(CB)) )),
    fail.
