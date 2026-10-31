/*  arithmetic.pl -- SWI-Prolog's library(arithmetic) as SCRIP runs it (cqo 2026-10-31, SWI library/test_arithfunc). SCRIP's own text,
    AGPL like the rest of SCRIP: written from swipl 9.0.4's documented behaviour (/usr/lib/swi-prolog/library/arithmetic.pl), not a
    vendored copy, because swipl's library declares a function through a system:term_expansion of the :- arithmetic_function/1
    directive and SCRIP runs a directive in place instead. The generator embeds it in LOCAL mode: no SWI reader, no renames, its
    helpers '$ar_'-named by hand. It is parsed only when a program names one of its exports.
    THE MODEL. :- arithmetic_function(Name/Arity) records '$ar_fn'(Term, Module) for a Term of that name and arity; the predicate
    Name/Arity+1 is the function, its last argument the result. The expansion is swipl's: goal_expansion/2 rewrites A is E and the
    six comparisons whose expressions name a declared function into the calls that compute it followed by the native goal, so a
    declaration must precede the clauses that use it. A file that loads this library takes the load road from its use_module line
    (the clauses after it are expanded as they are loaded). arithmetic_expression_value/2 evaluates such an expression at run time.
    A function is found by its NAME and arity: the module of the declaring context is recorded but not used to hide it from another
    module. Left out: roundtoward/2 over a declared function. */
:- module(arithmetic, [ arithmetic_function/1, arithmetic_expression_value/2 ]).
:- dynamic '$ar_fn'/2.

arithmetic_function(Spec) :-
    strip_module(Spec, M, NameArity),
    (   var(NameArity)
    ->  throw(error(instantiation_error, _))
    ;   NameArity = Name/Arity, atom(Name), integer(Arity)
    ->  functor(Term, Name, Arity),
        (   '$ar_fn'(Term, _) -> true ; assertz('$ar_fn'(Term, M)) )
    ;   throw(error(type_error(predicate_indicator, NameArity), _))
    ).

arithmetic_expression_value(Spec, Result) :-
    strip_module(Spec, _, Expression),
    '$ar_eval'(Expression, Result).

'$ar_eval'(Number, Result) :-
    number(Number),
    !,
    Result = Number.
'$ar_eval'(Term, Result) :-
    callable(Term),
    '$ar_fn'(Term, _),
    !,
    Term =.. [Name|Args0],
    '$ar_eval_args'(Args0, Args),
    append(Args, [Result], PredArgs),
    Goal =.. [Name|PredArgs],
    call(Goal).
'$ar_eval'(Term, Result) :-
    compound(Term),
    !,
    Term =.. [Name|Args0],
    '$ar_eval_args'(Args0, Args),
    Native =.. [Name|Args],
    Result is Native.
'$ar_eval'(Term, Result) :-
    Result is Term.

'$ar_eval_args'([], []).
'$ar_eval_args'([E|Es], [V|Vs]) :-
    '$ar_eval'(E, V),
    '$ar_eval_args'(Es, Vs).

'$ar_goal'(A is Expr, Goal) :-
    '$ar_expand'(Expr, Native, Pre),
    '$ar_tidy'((Pre, A is Native), Goal).
'$ar_goal'(ExprA =:= ExprB, Goal) :-
    '$ar_expand2'(ExprA, ExprB, NA, NB, Pre),
    '$ar_tidy'((Pre, NA =:= NB), Goal).
'$ar_goal'(ExprA =\= ExprB, Goal) :-
    '$ar_expand2'(ExprA, ExprB, NA, NB, Pre),
    '$ar_tidy'((Pre, NA =\= NB), Goal).
'$ar_goal'(ExprA > ExprB, Goal) :-
    '$ar_expand2'(ExprA, ExprB, NA, NB, Pre),
    '$ar_tidy'((Pre, NA > NB), Goal).
'$ar_goal'(ExprA < ExprB, Goal) :-
    '$ar_expand2'(ExprA, ExprB, NA, NB, Pre),
    '$ar_tidy'((Pre, NA < NB), Goal).
'$ar_goal'(ExprA >= ExprB, Goal) :-
    '$ar_expand2'(ExprA, ExprB, NA, NB, Pre),
    '$ar_tidy'((Pre, NA >= NB), Goal).
'$ar_goal'(ExprA =< ExprB, Goal) :-
    '$ar_expand2'(ExprA, ExprB, NA, NB, Pre),
    '$ar_tidy'((Pre, NA =< NB), Goal).

'$ar_expand2'(ExprA, ExprB, NA, NB, (PreA, PreB)) :-
    '$ar_expand'(ExprA, NA, PreA),
    '$ar_expand'(ExprB, NB, PreB).

'$ar_expand'(Expression, Native, Goal) :-
    '$ar_do'(Expression, Native, Goal0),
    '$ar_tidy'(Goal0, Goal).

'$ar_do'(X, X, true) :-
    '$ar_native'(X),
    !.
'$ar_do'(Function, Result, ArgCode) :-
    current_arithmetic_function(Function),
    !,
    Function =.. [Name|Args],
    '$ar_do_args'(Args, ArgResults, ArgCode),
    Result =.. [Name|ArgResults].
'$ar_do'(Function, Result, (ArgCode, Pred)) :-
    callable(Function),
    '$ar_fn'(Function, _),
    !,
    Function =.. [Name|Args],
    '$ar_pred_args'(Args, ArgResults, ArgCode),
    append(ArgResults, [Result], PredArgs),
    Pred =.. [Name|PredArgs].
'$ar_do'(Function, _, _) :-
    throw(error(type_error(evaluable, Function), _)).

'$ar_do_args'([], [], true).
'$ar_do_args'([H0|T0], [H|T], (A, B)) :-
    '$ar_do'(H0, H, A),
    '$ar_do_args'(T0, T, B).

'$ar_pred_args'([], [], true).
'$ar_pred_args'([H0|T0], [H|T], (A, B)) :-
    '$ar_do'(H0, H1, A0),
    (   callable(H1),
        current_arithmetic_function(H1)
    ->  A = (A0, H is H1)
    ;   A = A0,
        H = H1
    ),
    '$ar_pred_args'(T0, T, B).

'$ar_native'(F) :-
    var(F),
    !.
'$ar_native'(F) :-
    number(F),
    !.
'$ar_native'([_]) :- !.
'$ar_native'(F) :-
    string(F),
    !,
    string_length(F, 1).
'$ar_native'(F) :-
    current_arithmetic_function(F),
    (   compound(F)
    ->  forall(arg(_, F, A), '$ar_native'(A))
    ;   true
    ).

'$ar_tidy'(A, A) :-
    var(A),
    !.
'$ar_tidy'(((A, B), C), R) :-
    !,
    '$ar_tidy'((A, B, C), R).
'$ar_tidy'((true, A), R) :-
    !,
    '$ar_tidy'(A, R).
'$ar_tidy'((A, true), R) :-
    !,
    '$ar_tidy'(A, R).
'$ar_tidy'((A, X is Y), R) :-
    var(X), var(Y),
    !,
    '$ar_tidy'(A, R),
    X = Y.
'$ar_tidy'((A, B), (TA, TB)) :-
    !,
    '$ar_tidy'(A, TA),
    '$ar_tidy'(B, TB).
'$ar_tidy'(A, A).

goal_expansion(Math, Goal) :-
    '$ar_goal'(Math, Goal).
