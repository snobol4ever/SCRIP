/*  gnu_fd.pl -- GNU Prolog's FD spelling as a Prolog layer over library(clpfd) (cfo 2026-10-10, CEO-1596, ARCH-PROLOG-CLPFD.md
    section 5 point 3; row prolog-clpfd-the-gnu-fd-spelling-fd-domain-fd-labeling-fd-all-different-and-the-hash-hash-operators-
    over-the-same-solver-equal-to-gprolog-ceo-1596).  SCRIP's own text, AGPL like the rest of SCRIP: not a vendored file.  The
    generator (util_gen_prolog_prelude_libs.py) embeds it in LOCAL mode: no SWI reader, no renames, its helpers '$gnu_'-named by hand.

    THE SUPERSET.  A program in GNU spelling -- fd_domain/3, fd_labeling/1,2, fd_all_different/1, the full-lookahead operators
    #=# #\=# #<# #># #=<# #>=# and the boolean #<=> #\<=> #\==> ## #\\/ #\/\ -- is accepted beside SWI's and answered by the
    vendored clpfd solver; gprolog's documented difference for the #..# forms is pruning strength, never the labelled solutions.
    It is graded against gprolog (gplc), one oracle per dialect.  gprolog declares its FD operators always (EnginePl/oper.c:158-181)
    and imports nothing, so SCRIP's base operator table carries exactly that family at gprolog's priorities; SWI's own operators
    (in, ins, .., #<==>, #<==, the binary #\) arrive only at use_module(library(clpfd)).  THIS TEXT IS PARSED WHEN A PROGRAM NAMES
    ONE OF ITS EXPORTS, before clpfd is selected through it, so every SWI-only operator below is written in functional notation:
    in(X, '..'(L, U)) for X in L..U, '#<==>'(B, C) for B #<==> C, '#\\'(A, B) for the xor A #\ B.

    MEASURED against gprolog 1.6.0 on 2026-10-10: fd_dom/2 answers the LIST of the domain's values ([1,2,4,5]) where SWI's
    fd_dom/2 answers the domain term, so this library SHADOWS clpfd's fd_dom/2 when it is selected (the injector drops clpfd's
    clauses for it) and is selected only by a GNU-only name (fd_dom/2 is QUIET: it never selects this library by itself); the
    domain term it converts comes from clpfd's own fd_get/3 and domain_to_drep/2 under the generator's '$clpfd_' renames.
    fd_not_prime/1 keeps 1 ([1,4,6,8,9,10,12,14,15,16,18,20] on 1..20); fd_has_vector/1 is false on an interval and true once the
    domain has a hole; fd_has_extra_cstr/1 read false on an interval and on a bounded variable (two cases); fd_max_integer/1 is
    268435455 and fd_vector_max/1 127; fd_labeling/2's value_method(max) enumerates from the top and value_method(middle) takes
    the lower middle first; backtracks(B) is unified with 0 -- SCRIP counts no labeling backtracks (named gap); fd_minimize/2 and
    fd_maximize/2 are gprolog's own branch-and-bound (BipsFD/fd_optim.pl) over nb_setval.  NAMED GAPS: fd_dom/2 of a variable
    with no domain (gprolog gives it 0..fd_max_integer), the reorder(_) labeling option, value_method(limits).
*/
:- module(gnu_fd, [ op(700, xfx, #=#), op(700, xfx, #\=#), op(700, xfx, #<#), op(700, xfx, #>#), op(700, xfx, #=<#), op(700, xfx, #>=#),
                    op(750, xfy, #<=>), op(750, xfy, #\<=>), op(740, xfy, #\==>), op(730, xfy, ##), op(730, yfx, #\\/), op(720, yfx, #\/\),
                    (#=#)/2, (#\=#)/2, (#<#)/2, (#>#)/2, (#=<#)/2, (#>=#)/2, (#<=>)/2, (#\<=>)/2, (#\==>)/2, (##)/2, (#\\/)/2, (#\/\)/2,
                    fd_domain/3, fd_domain/2, fd_domain_bool/1, fd_labeling/1, fd_labeling/2, fd_labelingff/1, fd_all_different/1,
                    fd_element/3, fd_element_var/3, fd_atleast/3, fd_atmost/3, fd_exactly/3, fd_cardinality/2, fd_cardinality/3,
                    fd_relation/2, fd_relationc/2, fd_minimize/2, fd_maximize/2, fd_min/2, fd_max/2, fd_dom/2,
                    fd_has_extra_cstr/1, fd_has_vector/1, fd_use_vector/1, fd_vector_max/1, fd_set_vector_max/1, fd_max_integer/1,
                    fd_prime/1, fd_not_prime/1, fd_at_least_one/1, fd_at_most_one/1, fd_only_one/1, fd_reified_in/4 ]).
:- use_module(library(clpfd)).
'#=#'(X, Y) :- X #= Y.
'#\\=#'(X, Y) :- X #\= Y.
'#<#'(X, Y) :- X #< Y.
'#>#'(X, Y) :- X #> Y.
'#=<#'(X, Y) :- X #=< Y.
'#>=#'(X, Y) :- X #>= Y.
'#<=>'(X, Y) :- '$gnu_plain'(X, X1), '$gnu_plain'(Y, Y1), '#<==>'(X1, Y1).
'#\\<=>'(X, Y) :- '$gnu_plain'(X, X1), '$gnu_plain'(Y, Y1), #\ '#<==>'(X1, Y1).
'#\\==>'(X, Y) :- '$gnu_plain'(X, X1), '$gnu_plain'(Y, Y1), #\ (X1 #==> Y1).
'##'(X, Y) :- '$gnu_plain'(X, X1), '$gnu_plain'(Y, Y1), '#\\'(X1, Y1).
'#\\\\/'(X, Y) :- '$gnu_plain'(X, X1), '$gnu_plain'(Y, Y1), #\ (X1 #\/ Y1).
'#\\/\\'(X, Y) :- '$gnu_plain'(X, X1), '$gnu_plain'(Y, Y1), #\ (X1 #/\ Y1).
'$gnu_plain'(V, V) :- var(V), !.
'$gnu_plain'('#=#'(X, Y), X #= Y) :- !.
'$gnu_plain'('#\\=#'(X, Y), X #\= Y) :- !.
'$gnu_plain'('#<#'(X, Y), X #< Y) :- !.
'$gnu_plain'('#>#'(X, Y), X #> Y) :- !.
'$gnu_plain'('#=<#'(X, Y), X #=< Y) :- !.
'$gnu_plain'('#>=#'(X, Y), X #>= Y) :- !.
'$gnu_plain'('#<=>'(X, Y), '#<==>'(X1, Y1)) :- !, '$gnu_plain'(X, X1), '$gnu_plain'(Y, Y1).
'$gnu_plain'('#\\<=>'(X, Y), #\ '#<==>'(X1, Y1)) :- !, '$gnu_plain'(X, X1), '$gnu_plain'(Y, Y1).
'$gnu_plain'('#\\==>'(X, Y), #\ (X1 #==> Y1)) :- !, '$gnu_plain'(X, X1), '$gnu_plain'(Y, Y1).
'$gnu_plain'('##'(X, Y), '#\\'(X1, Y1)) :- !, '$gnu_plain'(X, X1), '$gnu_plain'(Y, Y1).
'$gnu_plain'('#\\\\/'(X, Y), #\ (X1 #\/ Y1)) :- !, '$gnu_plain'(X, X1), '$gnu_plain'(Y, Y1).
'$gnu_plain'('#\\/\\'(X, Y), #\ (X1 #/\ Y1)) :- !, '$gnu_plain'(X, X1), '$gnu_plain'(Y, Y1).
'$gnu_plain'(X #==> Y, X1 #==> Y1) :- !, '$gnu_plain'(X, X1), '$gnu_plain'(Y, Y1).
'$gnu_plain'(X #/\ Y, X1 #/\ Y1) :- !, '$gnu_plain'(X, X1), '$gnu_plain'(Y, Y1).
'$gnu_plain'(X #\/ Y, X1 #\/ Y1) :- !, '$gnu_plain'(X, X1), '$gnu_plain'(Y, Y1).
'$gnu_plain'(#\ X, #\ X1) :- !, '$gnu_plain'(X, X1).
'$gnu_plain'(C, C).
fd_domain(X, L, U) :- ( var(X) -> in(X, '..'(L, U)) ; integer(X) -> in(X, '..'(L, U)) ; ins(X, '..'(L, U)) ).
fd_domain(X, List) :- '$gnu_list_dom'(List, D), ( var(X) -> in(X, D) ; integer(X) -> in(X, D) ; ins(X, D) ).
fd_domain_bool(X) :- fd_domain(X, 0, 1).
'$gnu_list_dom'([V], V) :- !.
'$gnu_list_dom'([V|Vs], V \/ D) :- '$gnu_list_dom'(Vs, D).
fd_labeling(X) :- '$gnu_labeling'(X, []).
fd_labelingff(X) :- '$gnu_labeling'(X, [variable_method(first_fail)]).
fd_labeling(X, Options) :- '$gnu_labeling'(X, Options).
'$gnu_labeling'(X, Options) :-
    ( ( var(X) ; integer(X) ) -> Vs = [X] ; Vs = X ),
    '$gnu_label_opts_list'(Options, leftmost, up, Sel, Ord),
    labeling([Sel, Ord], Vs),
    '$gnu_label_backtracks'(Options).
'$gnu_label_backtracks'([]).
'$gnu_label_backtracks'([O|Os]) :- ( O = backtracks(B) -> B = 0 ; true ), '$gnu_label_backtracks'(Os).
'$gnu_label_opts_list'([], S, V, S, V).
'$gnu_label_opts_list'([O|Os], S0, V0, S, V) :-
    ( O = variable_method(M) -> '$gnu_var_method'(M, S1), V1 = V0
    ; O = value_method(M) -> '$gnu_val_method'(M, V1), S1 = S0
    ; S1 = S0, V1 = V0 ),
    '$gnu_label_opts_list'(Os, S1, V1, S, V).
'$gnu_var_method'(standard, leftmost).
'$gnu_var_method'(first_fail, ff).
'$gnu_var_method'(ff, ff).
'$gnu_var_method'(most_constrained, ffc).
'$gnu_var_method'(smallest, min).
'$gnu_var_method'(largest, max).
'$gnu_var_method'(max_regret, ff).
'$gnu_var_method'(random, leftmost).
'$gnu_val_method'(min, up).
'$gnu_val_method'(max, down).
'$gnu_val_method'(middle, bisect).
'$gnu_val_method'(bisect, bisect).
'$gnu_val_method'(random, up).
fd_all_different(L) :- all_different(L).
fd_element(I, List, V) :- element(I, List, V).
fd_element_var(I, List, V) :- element(I, List, V).
fd_atleast(N, List, V) :- '$gnu_count'(List, V, C), C #>= N.
fd_atmost(N, List, V) :- '$gnu_count'(List, V, C), C #=< N.
fd_exactly(N, List, V) :- '$gnu_count'(List, V, C), C #= N.
'$gnu_count'([], _, 0).
'$gnu_count'([X|Xs], V, C) :- '$gnu_count'(Xs, V, C0), '#<==>'(B, X #= V), C #= C0 + B.
fd_cardinality(List, Count) :- fd_max_integer(Inf), in(Count, '..'(0, Inf)), '$gnu_card'(List, Count).
fd_cardinality(L, List, U) :- in(Count, '..'(L, U)), '$gnu_card'(List, Count).
'$gnu_card'([], 0).
'$gnu_card'([C|Cs], Count) :- '$gnu_card'(Cs, Count0), '$gnu_plain'(C, C1), '#<==>'(B, C1), Count #= Count0 + B.
fd_at_least_one(List) :- '$gnu_card'(List, Count), Count #>= 1.
fd_at_most_one(List) :- '$gnu_card'(List, Count), Count #=< 1.
fd_only_one(List) :- '$gnu_card'(List, 1).
fd_relation(Tuples, Vars) :- tuples_in([Vars], Tuples).
fd_relationc(CTuples, Vars) :- transpose(CTuples, Tuples), tuples_in([Vars], Tuples).
fd_minimize(Goal, Var) :-
    fd_max_integer(Inf), nb_setval('$gnu_cur_min', Inf),
    repeat,
    nb_getval('$gnu_cur_min', B), B1 is B - 1,
    (   in(Var, '..'(0, B1)), call(Goal) -> fd_min(Var, C), nb_setval('$gnu_cur_min', C), fail
    ;   !, Var = B, call(Goal)
    ).
fd_maximize(Goal, Var) :-
    fd_max_integer(Inf), nb_setval('$gnu_cur_max', 0),
    repeat,
    nb_getval('$gnu_cur_max', B), B1 is B + 1,
    (   in(Var, '..'(B1, Inf)), call(Goal) -> fd_max(Var, C), nb_setval('$gnu_cur_max', C), fail
    ;   !, Var = B, call(Goal)
    ).
fd_min(X, Min) :- fd_inf(X, Min).
fd_max(X, Max) :- fd_sup(X, Max).
'$gnu_drep'(X, D) :- '$clpfd_fd_get'(X, XD, _), '$clpfd_domain_to_drep'(XD, D).
fd_dom(X, List) :- ( integer(X) -> List = [X] ; '$gnu_drep'(X, D), '$gnu_dom_list'(D, List, []) ).
'$gnu_dom_list'(D1 \/ D2, L0, L) :- !, '$gnu_dom_list'(D1, L0, L1), '$gnu_dom_list'(D2, L1, L).
'$gnu_dom_list'('..'(A, B), L0, L) :- !, numlist(A, B, Vs), append(Vs, L, L0).
'$gnu_dom_list'(V, [V|L], L).
fd_has_extra_cstr(X) :- fd_var(X), fail.
fd_has_vector(X) :- fd_var(X), '$gnu_drep'(X, D), D = (_ \/ _).
fd_use_vector(X) :- fd_var(X).
fd_vector_max(Max) :- ( nb_current('$gnu_vector_max', M) -> Max = M ; Max = 127 ).
fd_set_vector_max(Max) :- integer(Max), nb_setval('$gnu_vector_max', Max).
fd_max_integer(268435455).
fd_prime(X) :- fd_vector_max(M), '$gnu_primes'(2, M, Ps), '$gnu_list_dom'(Ps, D), in(X, D).
fd_not_prime(X) :- fd_vector_max(M), '$gnu_primes'(2, M, Ps), '$gnu_list_dom'(Ps, D), in(X, '..'(0, M)), #\ in(X, D).
'$gnu_primes'(N, M, []) :- N > M, !.
'$gnu_primes'(N, M, Ps) :- N1 is N + 1, '$gnu_primes'(N1, M, Ps0), ( '$gnu_is_prime'(N, 2) -> Ps = [N|Ps0] ; Ps = Ps0 ).
'$gnu_is_prime'(N, D) :- D * D > N, !.
'$gnu_is_prime'(N, D) :- N mod D =\= 0, D1 is D + 1, '$gnu_is_prime'(N, D1).
fd_reified_in(X, L, U, B) :- '#<==>'(B, in(X, '..'(L, U))).
