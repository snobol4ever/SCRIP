%  WITNESS for HB_PLJ (209) -- the Prolog-side collected-heap block.
:- initialization(main).
churn(0).
churn(N) :- N > 0, atom_codes(_, [122,122,122,122,122,122,122,122,122,122]), M is N - 1, churn(M).
main :- L = [87,73,84,78,69,83,83], churn(300), atom_codes(A, L), write(A), nl.
