% the Prolog trail's cap (PL_TR_ARENA_LG2 25: 1048575 conditional bindings) read at cap+1 -- 1200000 variables older than a
% choicepoint, each bound once. A legal program: swipl and gprolog print done. SCRIP refuses at the cap, loudly (rc 2).
:- initialization(main).
main :- length(L, 1200000), ( true ; true ), bind(L), write(done), nl, halt.
bind([]).
bind([a|T]) :- bind(T).
