% the Prolog trail's cap (PL_TR_ARENA_LG2 27: 4194303 conditional bindings) read at cap+1 -- 4800000 variables older than a
% choicepoint, each bound once, in chunks of 4000; length/2 over 4800000 cells needs the 4 GB virtual stack the row declares. A legal program: swipl and gprolog print done.
% SCRIP refuses at the cap, loudly (rc 2).
:- initialization(main).
main :- length(L, 4800000), ( true ; true ), bind_all(L), write(done), nl, halt.
bind_all([]) :- !.
bind_all(L) :- take(4000, L, Rest), bind_all(Rest).
take(0, R, R) :- !.
take(_, [], []) :- !.
take(N, [a|T], R) :- N1 is N - 1, take(N1, T, R).
