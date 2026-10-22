/*  xsb_tables.pl -- XSB TABLE-MAINTENANCE PREDICATES SWI'S xsb DIALECT DEFINES (hq_prolog 2026-10-17, row prolog-swi-tabling-the-xsb-loader-runs-the-xsb-test-drivers, SWI
    xsb/table_tests abol_test3*). gc_tables(-Remaining) counts the abolished tables that are still waiting to be reclaimed. SCRIP destroys a table when it is abolished
    (abolish_table_pred/1, abolish_all_tables/0 free its trie at once), so none is ever waiting and the count is 0, the value swipl 9.0.4 reports once its atom collector
    has run. abolish_table_subgoals(:Goal) (2026-10-18, xsb/table_tests abolish_dag, abolish_cycle, abolish_neg_*, atc_test) abolishes every table whose call unifies with
    Goal and then, transitively, every table holding a conditional answer whose delay list names an abolished table (a positive literal it subsumes, or tnot of one): a
    caller whose answers are unconditional keeps its table, and a callee is never touched -- as swipl 9.0.4 measured on p01 :- p11 :- u1 :- undef (tnot) and q0 :- q1 :- q2.
    abolish_table_call/1,2 is the same predicate (SWI library(tables)); the /2 forms accept [], abolish_tables_transitively and abolish_tables_singly as swipl does.
    abolish_table_pred(:NameArityOrHead) abolishes the subgoals of the predicate's most general head, as SWI's library(tables) defines it -- so the same cascade (it was
    the base prelude's own drop of the matching tables alone, which left xsb/table_tests pred_abolish_dag/cycle's dependents standing).
    abolish_table_info/0 (2026-10-21, hq_packages, xsb/ptq tran/1) is XSB's reset of the tabling statistics: SCRIP keeps none, so it succeeds.
    abolish_module_tables(+Module) (xsb/table_tests abol_test2, abol_test2a) destroys every table whose variant is in Module, as boot/tabling.pl of swipl 9.0.4 does (no cascade); usermod is user.
    Embedded by the generator in LOCAL mode; linked for a program that references load_files or names one of these predicates (prolog_parse.c). */
:- module(xsb_tables, [ gc_tables/1, abolish_table_info/0, abolish_module_tables/1, '$amt_drop'/3, abolish_table_subgoals/1, abolish_table_subgoals/2, abolish_table_call/1, abolish_table_call/2, abolish_table_pred/1, '$ats_goal'/2,
                        '$ats_split'/4, '$ats_close'/3, '$ats_dep'/4, '$ats_hits'/2, '$ats_lit'/2, '$ats_drop'/2, '$ats_opts'/1, '$atp_goal'/2 ]).
gc_tables(0).
abolish_table_info.
abolish_module_tables(M0):-'$must_be_atom_mod'(M0),(M0==usermod->M=user;M=M0),'$tbl_vt'(VT),'$trie_entries'(VT,L),'$amt_drop'(L,M,VT).
'$must_be_atom_mod'(M):-var(M),!,throw(error(instantiation_error,abolish_module_tables/1)).
'$must_be_atom_mod'(M):-atom(M),!.
'$must_be_atom_mod'(M):-throw(error(type_error(atom,M),abolish_module_tables/1)).
'$amt_drop'([],_,_).
'$amt_drop'([e(_,QV,T)|L],M,VT):-('$tbl_unqual'(QV,M0,_),M0==M->('$trie_delete'(VT,QV,_)->true;true),'$trie_destroy'(T);true),'$amt_drop'(L,M,VT).
abolish_table_pred(P):-'$atp_goal'(P,G),abolish_table_subgoals(G).
'$atp_goal'(P,_):-var(P),!,throw(error(instantiation_error,abolish_table_pred/1)).
'$atp_goal'(M:P,M:G):-atom(M),!,'$atp_goal'(P,G).
'$atp_goal'(F/N,G):-atom(F),integer(N),!,functor(G,F,N).
'$atp_goal'(H,G):-callable(H),!,functor(H,F,N),functor(G,F,N).
'$atp_goal'(P,_):-throw(error(type_error(callable_or_predicate_indicator,P),abolish_table_pred/1)).
abolish_table_subgoals(MG):-'$ats_goal'(MG,QG),'$tbl_vt'(VT),'$trie_entries'(VT,L),'$ats_split'(L,QG,A,R),'$ats_close'(A,R,All),'$ats_drop'(All,VT).
abolish_table_subgoals(G,O):-'$ats_opts'(O),abolish_table_subgoals(G).
abolish_table_call(G):-abolish_table_subgoals(G).
abolish_table_call(G,O):-abolish_table_subgoals(G,O).
'$ats_goal'(MG,_):-var(MG),!,throw(error(instantiation_error,abolish_table_subgoals/1)).
'$ats_goal'(MG,QG):-'$tbl_strip'(MG,M,G),(atom(M),M\==user->'$tbl_qual'(M,G,QG);QG=G).
'$ats_split'([],_,[],[]).
'$ats_split'([E|L],G,A,R):-E=e(_,V,_),(\+ \+ V=G->A=[E|A1],R=R1;A=A1,R=[E|R1]),'$ats_split'(L,G,A1,R1).
'$ats_close'(A,R,All):-'$ats_dep'(R,A,N,Rest),(N==[]->All=A;append(A,N,A2),'$ats_close'(A2,Rest,All)).
'$ats_dep'([],_,[],[]).
'$ats_dep'([E|R],A,N,Rest):-E=e(_,_,T),('$ats_hits'(T,A)->N=[E|N1],Rest=Rest1;N=N1,Rest=[E|Rest1]),'$ats_dep'(R,A,N1,Rest1).
'$ats_hits'(T,A):-'$trie_entries'(T,As),member(e(_,_,u(P)),As),member(_-D,P),member(L,D),'$ats_lit'(L,G),member(e(_,V,_),A),subsumes_term(V,G),!.
'$ats_lit'(tnot(G),G):-!.
'$ats_lit'(G,G).
'$ats_drop'([],_).
'$ats_drop'([e(_,V,T)|L],VT):-('$trie_delete'(VT,V,_)->true;true),'$trie_destroy'(T),'$ats_drop'(L,VT).
'$ats_opts'(O):-var(O),!,throw(error(instantiation_error,abolish_table_subgoals/2)).
'$ats_opts'([]):-!.
'$ats_opts'(O):-is_list(O),(memberchk(abolish_tables_transitively,O);memberchk(abolish_tables_singly,O)),!.
'$ats_opts'(O):-is_list(O),!,throw(error(domain_error([abolish_tables_transitively,abolish_tables_singly],O),abolish_table_subgoals/2)).
'$ats_opts'(O):-throw(error(type_error(list,O),abolish_table_subgoals/2)).
