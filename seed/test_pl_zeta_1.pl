:- initialization(main).
f(a).
f(b).
g(a) :- !.
g(b).
main :- ( f(X), write(X), nl, fail ; true ), ( g(Y), write(Y), nl, fail ; true ).
