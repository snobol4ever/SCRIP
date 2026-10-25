/*  mctx.pl -- THE CONTEXT-MODULE HELPERS OF A CLAUSE GOAL WHOSE ARGUMENT IS A VARIABLE (hq_prolog 2026-10-15; SCRIP's own text, AGPL like the rest of SCRIP: not a vendored
    file; the generator embeds it in LOCAL mode).  pl_module_flatten (prolog_lower.c) renames a predicate a module or plunit unit defines or declares dynamic to M:name and
    qualifies every goal that names it; an assert, asserta, assertz, retract, retractall or clause goal whose first argument is a variable bound at run time names the bare
    name, so pl_mod_rt_clause rewrites it to '$mctx_<goal>'(M1, M2, ...) and these clauses rename the head to M1:name or M2:name when that predicate is declared or defined,
    and strip the prefix from a head they hand back.  Parsed only when the program holds such a goal (prolog_parse.c pl_tree_var_clause_goal): a module program that
    never asserts a variable pays nothing, and the base prelude carries none of it (test_gate_pl_prelude_libs_cost.sh). '$mctx_call'(M, G) runs a goal the lowerer could not resolve, or a
    variable goal, inside a plunit unit M and qualifies the existence_error of the goal's own procedure with the module that called it (swipl: plunit_<unit>:name/arity,
    or Q:name/arity for call(Q:name, ...)); an error from a deeper call passes unchanged. */
:- module(mctx, ['$mctx_assert'/3, '$mctx_asserta'/3, '$mctx_assertz'/3, '$mctx_assert'/4, '$mctx_asserta'/4, '$mctx_assertz'/4, '$mctx_retract'/3, '$mctx_retractall'/3,
                 '$mctx_clause'/4, '$mctx_clause'/5, '$mctx_call'/2]).
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
'$mctx_own'(H,Own):-(nonvar(H),callable(H),functor(H,N,_),atom(N),'$mod_split'(N,Own0,_)->Own=Own0;Own=user).
'$mctx_unb'(M1,M2,H1,B0,B):-(nonvar(B0),B0=Q:B1,atom(Q),(Q==M1;Q==M2)->B2=B1;B2=B0),'$mctx_own'(H1,Own),'$clause_unmangle'(Own,B2,B).
'$mctx_clause'(M1,M2,H0,B):-(var(H0)->clause(H1,B0),'$mctx_unh'(M1,M2,H1,H0);'$mctx_head'(M1,M2,H0,H),clause(H,B0),H1=H),'$mctx_unb'(M1,M2,H1,B0,B).
'$mctx_clause'(M1,M2,H0,B,R):-(var(H0)->clause(H1,B0,R),'$mctx_unh'(M1,M2,H1,H0);'$mctx_head'(M1,M2,H0,H),clause(H,B0,R),H1=H),'$mctx_unb'(M1,M2,H1,B0,B).
'$mctx_call'(M,G):-catch(G,error(existence_error(procedure,PI),Ctx),'$mctx_unk'(M,G,PI,Ctx)).
'$mctx_unk'(M,G,N/A,_):-nonvar(G),callable(G),functor(G,N,A),'$mctx_pick'([M],N,A,QN),!,G=..[_|As],G2=..[QN|As],call(G2).
'$mctx_unk'(M,G,N/A,Ctx):-atom(N),'$mctx_lf'(M,G,Q,E),callable(E),functor(E,N,A),Q\==user,!,throw(error(existence_error(procedure,Q:N/A),Ctx)).
'$mctx_lf'(M,G,Q,E):-nonvar(G),G=(A,B),!,('$mctx_lf'(M,A,Q,E);'$mctx_lf'(M,B,Q,E)).
'$mctx_lf'(M,G,Q,E):-nonvar(G),G=(A;B),!,('$mctx_lf'(M,A,Q,E);'$mctx_lf'(M,B,Q,E)).
'$mctx_lf'(M,G,Q,E):-nonvar(G),G=(A->B),!,('$mctx_lf'(M,A,Q,E);'$mctx_lf'(M,B,Q,E)).
'$mctx_lf'(M,G,Q,E):-nonvar(G),G=(A*->B),!,('$mctx_lf'(M,A,Q,E);'$mctx_lf'(M,B,Q,E)).
'$mctx_lf'(M,G,Q,E):-nonvar(G),G=(\+A),!,'$mctx_lf'(M,A,Q,E).
'$mctx_lf'(M,G,Q,E):-'$mctx_eff'(M,G,Q,E).
'$mctx_unk'(_,_,PI,Ctx):-throw(error(existence_error(procedure,PI),Ctx)).
'$mctx_eff'(M,G,Q,E):-nonvar(G),G=..[call,C|As],!,'$mctx_eff1'(M,C,As,Q,E).
'$mctx_eff'(M,G,Q,E):-nonvar(G),G=Q0:G1,atom(Q0),!,Q=Q0,E=G1.
'$mctx_eff'(M,G,M,G).
'$mctx_eff1'(M,C,As,Q,E):-nonvar(C),C=Q0:C1,atom(Q0),!,Q=Q0,'$mctx_ext'(C1,As,E).
'$mctx_eff1'(M,C,As,M,E):-'$mctx_ext'(C,As,E).
'$mctx_ext'(C,As,E):-nonvar(C),callable(C),C=..L0,append(L0,As,L1),E=..L1,!.
'$mctx_ext'(C,_,C).
