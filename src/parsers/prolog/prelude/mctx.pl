/*  mctx.pl -- THE CONTEXT-MODULE HELPERS OF A CLAUSE GOAL WHOSE ARGUMENT IS A VARIABLE (hq_prolog 2026-10-15; SCRIP's own text, AGPL like the rest of SCRIP: not a vendored
    file; the generator embeds it in LOCAL mode).  pl_module_flatten (prolog_lower.c) renames a predicate a module or plunit unit defines or declares dynamic to M:name and
    qualifies every goal that names it; an assert, asserta, assertz, retract, retractall or clause goal whose first argument is a variable bound at run time names the bare
    name, so pl_mod_rt_clause rewrites it to '$mctx_<goal>'(M1, M2, ...) and these clauses rename the head to M1:name or M2:name when that predicate is declared or defined,
    and strip the prefix from a head they hand back.  Parsed only when the program holds such a goal (prolog_parse.c pl_tree_var_clause_goal): a module program that
    never asserts a variable pays nothing, and the base prelude carries none of it (test_gate_pl_prelude_libs_cost.sh). */
:- module(mctx, ['$mctx_assert'/3, '$mctx_asserta'/3, '$mctx_assertz'/3, '$mctx_assert'/4, '$mctx_asserta'/4, '$mctx_assertz'/4, '$mctx_retract'/3, '$mctx_retractall'/3,
                 '$mctx_clause'/4, '$mctx_clause'/5]).
'$mctx_pick'([M|_],N,A,QN):-atom_concat(M,':',P),atom_concat(P,N,Q0),current_predicate(Q0/A),!,QN=Q0.
'$mctx_pick'([_|Ms],N,A,QN):-'$mctx_pick'(Ms,N,A,QN).
'$mctx_head'(M1,M2,H0,H):-nonvar(H0),callable(H0),H0\=(_:_),functor(H0,N,A),'$mctx_pick'([M1,M2],N,A,QN),!,H0=..[_|As],H=..[QN|As].
'$mctx_head'(_,_,H,H).
'$mctx_term'(M1,M2,T0,T):-nonvar(T0),T0=(H0:-B),!,'$mctx_head'(M1,M2,H0,H),T=(H:-B).
'$mctx_term'(M1,M2,T0,T):-'$mctx_head'(M1,M2,T0,T).
'$mctx_assert'(M1,M2,C):-'$mctx_term'(M1,M2,C,C1),assert(C1).
'$mctx_asserta'(M1,M2,C):-'$mctx_term'(M1,M2,C,C1),asserta(C1).
'$mctx_assertz'(M1,M2,C):-'$mctx_term'(M1,M2,C,C1),assertz(C1).
'$mctx_assert'(M1,M2,C,R):-'$mctx_term'(M1,M2,C,C1),assert(C1,R).
'$mctx_asserta'(M1,M2,C,R):-'$mctx_term'(M1,M2,C,C1),asserta(C1,R).
'$mctx_assertz'(M1,M2,C,R):-'$mctx_term'(M1,M2,C,C1),assertz(C1,R).
'$mctx_strip'([M|Ms],N,N1):-(atom_concat(M,':',P),atom_concat(P,N1,N)->true;'$mctx_strip'(Ms,N,N1)).
'$mctx_unh'(M1,M2,H0,H):-(nonvar(H0),callable(H0),functor(H0,N,_),atom(N),'$mctx_strip'([M1,M2],N,N1)->H0=..[_|As],H=..[N1|As];H=H0).
'$mctx_un'(M1,M2,T0,T):-(nonvar(T0),T0=(H0:-B)->'$mctx_unh'(M1,M2,H0,H),T=(H:-B);'$mctx_unh'(M1,M2,T0,T)).
'$mctx_retract'(M1,M2,C):-(var(C)->retract(C1),'$mctx_un'(M1,M2,C1,C);'$mctx_term'(M1,M2,C,C1),retract(C1)).
'$mctx_retractall'(M1,M2,H0):-'$mctx_head'(M1,M2,H0,H),retractall(H).
'$mctx_clause'(M1,M2,H0,B):-(var(H0)->clause(H1,B),'$mctx_unh'(M1,M2,H1,H0);'$mctx_head'(M1,M2,H0,H),clause(H,B)).
'$mctx_clause'(M1,M2,H0,B,R):-(var(H0)->clause(H1,B,R),'$mctx_unh'(M1,M2,H1,H0);'$mctx_head'(M1,M2,H0,H),clause(H,B,R)).
