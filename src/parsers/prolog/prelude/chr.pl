:- module(chr, [op(1180, xfx, ==>), op(1180, xfx, <=>), op(1150, fx, constraints), op(1150, fx, chr_constraint), op(1150, fx, chr_preprocessor), op(1150, fx, handler), op(1150, fx, rules), op(1100, xfx, \), op(1200, xfx, @), op(1190, xfx, pragma), op(500, yfx, #), op(1150, fx, chr_type), op(1150, fx, chr_declaration), op(1130, xfx, --->), op(1150, fx, ?), '$chr_attach'/5, '$chr_detach'/5, '$enumerate_constraints'/1, '$chr_unify'/3]).
:- use_module(chr/chr_runtime).
'$chr_attach'(_, _, _, [], _).
'$chr_attach'(M, I, K, [V|Vs], S) :-
    Bit is 1 << (I - 1),
    (   get_attr(V, M, Att)
    ->  Att =.. [v, Mask|Ls],
        (   Mask /\ Bit =:= Bit
        ->  '$chr_nth_cons'(I, Ls, S, Ls1),
            Att1 =.. [v, Mask|Ls1]
        ;   Mask1 is Mask \/ Bit,
            '$chr_nth_put'(I, Ls, [S], Ls1),
            Att1 =.. [v, Mask1|Ls1]
        )
    ;   '$chr_fresh_lists'(1, K, I, S, Ls1),
        Att1 =.. [v, Bit|Ls1]
    ),
    put_attr(V, M, Att1),
    '$chr_attach'(M, I, K, Vs, S).
'$chr_detach'(_, _, _, [], _).
'$chr_detach'(M, I, K, [V|Vs], S) :-
    Bit is 1 << (I - 1),
    (   get_attr(V, M, Att)
    ->  Att =.. [v, Mask|Ls],
        (   Mask /\ Bit =:= Bit
        ->  '$chr_nth_get'(I, Ls, L),
            'chr sbag_del_element'(L, S, L1),
            (   L1 == []
            ->  Mask1 is Mask /\ \ Bit,
                (   Mask1 =:= 0
                ->  del_attr(V, M)
                ;   '$chr_nth_put'(I, Ls, [], Ls1),
                    Att1 =.. [v, Mask1|Ls1],
                    put_attr(V, M, Att1)
                )
            ;   '$chr_nth_put'(I, Ls, L1, Ls1),
                Att1 =.. [v, Mask|Ls1],
                put_attr(V, M, Att1)
            )
        ;   true
        )
    ;   true
    ),
    '$chr_detach'(M, I, K, Vs, S).
'$chr_fresh_lists'(J, K, _, _, []) :- J > K, !.
'$chr_fresh_lists'(J, K, I, S, [L|Ls]) :-
    (   J =:= I
    ->  L = [S]
    ;   L = []
    ),
    J1 is J + 1,
    '$chr_fresh_lists'(J1, K, I, S, Ls).
'$chr_nth_get'(1, [L|_], L) :- !.
'$chr_nth_get'(I, [_|Ls], L) :- I1 is I - 1, '$chr_nth_get'(I1, Ls, L).
'$chr_nth_put'(1, [_|Ls], X, [X|Ls]) :- !.
'$chr_nth_put'(I, [L|Ls], X, [L|Ls1]) :- I1 is I - 1, '$chr_nth_put'(I1, Ls, X, Ls1).
'$chr_nth_cons'(1, [L|Ls], S, [[S|L]|Ls]) :- !.
'$chr_nth_cons'(I, [L|Ls], S, [L|Ls1]) :- I1 is I - 1, '$chr_nth_cons'(I1, Ls, S, Ls1).
'$enumerate_constraints'(C) :-
    (   nonvar(C)
    ->  functor(C, N, A),
        '$chr_constraint'(N/A, _, Key),
        '$chr_enumerate_store'(Key, N, C)
    ;   '$chr_constraint'(N/A, _, Key),
        '$chr_enumerate_store'(Key, N, C)
    ).
'$chr_enumerate_store'(Key, N, C) :-
    nb_current(Key, Store),
    member(S, Store),
    S =.. [_, _, _, _, _, _, _|Args],
    C =.. [N|Args].
'$chr_unify'(M, Att, Other) :-
    Att =.. [v, Mask|Ls],
    '$chr_sort_all'(Ls, Ls1),
    (   var(Other)
    ->  (   get_attr(Other, M, Att2)
        ->  Att2 =.. [v, Mask2|Ls2],
            '$chr_sort_all'(Ls2, Ls2s),
            '$chr_merge_all'(Ls1, Ls2s, Ls3),
            Mask3 is Mask \/ Mask2,
            Att3 =.. [v, Mask3|Ls3],
            put_attr(Other, M, Att3),
            '$chr_run_all'(Ls3)
        ;   Att3 =.. [v, Mask|Ls1],
            put_attr(Other, M, Att3),
            '$chr_run_all'(Ls1)
        )
    ;   (   compound(Other)
        ->  term_variables(Other, Vs),
            Att3 =.. [v, Mask|Ls1],
            '$chr_attach_increment'(M, Vs, Att3)
        ;   true
        ),
        '$chr_run_all'(Ls1)
    ).
'$chr_attach_increment'(_, [], _).
'$chr_attach_increment'(M, [V|Vs], Att) :-
    'chr not_locked'(V),
    (   get_attr(V, M, Att2)
    ->  Att =.. [v, Mask|Ls],
        Att2 =.. [v, Mask2|Ls2],
        '$chr_sort_all'(Ls2, Ls2s),
        '$chr_merge_all'(Ls, Ls2s, Ls3),
        Mask3 is Mask \/ Mask2,
        Att3 =.. [v, Mask3|Ls3],
        put_attr(V, M, Att3)
    ;   put_attr(V, M, Att)
    ),
    '$chr_attach_increment'(M, Vs, Att).
'$chr_sort_all'([], []).
'$chr_sort_all'([L|Ls], [S|Ss]) :- sort(L, S), '$chr_sort_all'(Ls, Ss).
'$chr_merge_all'([], [], []).
'$chr_merge_all'([A|As], [B|Bs], [C|Cs]) :- 'chr merge_attributes'(A, B, C), '$chr_merge_all'(As, Bs, Cs).
'$chr_run_all'([]).
'$chr_run_all'([L|Ls]) :- '$chr_run_suspensions'(L), '$chr_run_all'(Ls).
'$chr_run_suspensions'([]).
'$chr_run_suspensions'([S|Ss]) :-
    arg(2, S, State),
    (   State == active
    ->  setarg(2, S, triggered),
        arg(4, S, G0),
        G is G0 + 1,
        setarg(4, S, G),
        arg(5, S, Cont),
        call(Cont),
        arg(2, S, State1),
        (   State1 == triggered
        ->  setarg(2, S, active)
        ;   true
        )
    ;   true
    ),
    '$chr_run_suspensions'(Ss).
