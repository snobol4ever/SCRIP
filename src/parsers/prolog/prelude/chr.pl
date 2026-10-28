:- module(chr, [op(1180, xfx, ==>), op(1180, xfx, <=>), op(1150, fx, constraints), op(1150, fx, chr_constraint), op(1150, fx, chr_preprocessor), op(1150, fx, handler), op(1150, fx, rules), op(1100, xfx, \), op(1200, xfx, @), op(1190, xfx, pragma), op(500, yfx, #), op(1150, fx, chr_type), op(1150, fx, chr_declaration), op(1130, xfx, --->), op(1150, fx, ?), '$chr_attach'/5, '$chr_detach'/5, '$enumerate_constraints'/1]).
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
