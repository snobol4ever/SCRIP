/*  tabling.pl -- PROLOG TABLING AS SCRIP RUNS IT (cfo 2026-10-22, CFO-324, row prolog-tabling-is-boxes-the-evaluation-state-is-the-table-header-ceo-1885;
    ARCH-PROLOG-TABLING-BOXES.md; the design ruled at CEO-1885). SCRIP's own text, AGPL like the rest of SCRIP: it was the tabling block of PL_PRELUDE_SRC in
    prolog_parse.c and leaves the C string by CEO-1885 (a Prolog prelude file, not C text); the generator (util_gen_prolog_prelude_libs.py) embeds it in LOCAL mode,
    no SWI reader, no renames. It is parsed only for a program that tables, loads files at run time, or names one of its exports (undefined/0 is QUIET: five vendored
    libraries carry the atom as data, and prolog_parse.c selects this library for a program whose source names it).
    THE EVALUATION STATE IS THE BOX'S HEADER: '$tbl_hdr' lowers to IR_TABLE_HDR, a box whose frame cells hold one evaluation's state (cell 1 the tables in progress,
    2 the raise to the alternating fixpoint, 3 the phase, 4 the new-answer flag of a WFS pass, 5 the dependency depth, 6 the semi-naive context, 7 the consumer
    position, 8 the highest position, 9 the tables a pass consumed); '$tbh_own' writes this header's own cell, '$tbh_set' and '$tbh_bset' (trailed) the nearest header
    that owns the cell, '$tbh_get' reads it, '$tbh_close' restores the banked header. No predicate here keeps state anywhere else: the answer tables are tries on the
    heap (they outlive the call), the current delay list is one trailed root word ('$tbl_dlget'/'$tbl_dlset', the cto's 2026-10-14 ruling), and the incremental
    predicates are '$tbl_incrp'/1 facts. */
:- module(tabling, [ '$tbl_call'/2, '$tbl_icall'/2, '$tbl_isincr'/1, '$tbl_incrs'/1, '$incr_inval'/0, '$incr_guard'/2, '$incr_drop'/3, subgoal_property/2, '$tbl_vmap'/2, incr_assert/1, incr_asserta/1, incr_assertz/1, incr_retract/1, incr_retractall/1, incr_invalidate_calls/1, incr_invalidate_call/1, xwam_state/2, '$tbl_mcall'/3, '$tbl_get'/4, '$tbl_valid'/4, '$tbl_noattv'/1, '$tbh_v'/3, '$tbl_dl'/1, '$tbl_msplit'/7, '$tbl_mindex'/1, '$tbl_mcheck'/1, '$tbl_mterm'/2, '$tbl_enter'/6, '$tbl_leading'/0, '$tbl_inprog'/1, '$tbl_lead'/1, '$tbl_eval1'/1, '$tbl_cycg'/1, '$tbl_restrain'/2, '$tbl_rs'/1, '$tbl_rsa'/2, '$tbl_gdeeper'/2, '$tbl_deeper'/2, '$tbl_atrip'/1, '$tbl_trip'/1, '$tbl_trip'/2, '$tbl_ans2'/3, '$tbl_chk'/2, '$tbl_chk2'/3, '$tbl_eval1n'/1, '$tbl_nest'/4, '$tbl_dup'/1, '$tbl_scc'/4, '$tbl_indep'/2, '$tbl_srounds'/1, '$tbl_upto'/4, '$tbl_fin_all'/1, '$tbl_depn'/1, '$tbl_pos'/2, '$tbl_idx'/4, '$tbl_dmin'/3, '$tbl_done'/1, '$tbl_wfsrun'/0, '$tbl_alt'/1, '$tbl_alt2'/2, '$tbl_reset'/1, '$tbl_renew'/3, '$tbl_count'/3, '$tbl_snapw'/2, '$tbl_snapins'/2, '$tbl_final'/1, '$tbl_final1'/1, '$tbl_wdrop'/1, '$tbl_finvals'/2, tnot/1, '$tnot_split'/2, '$tnot_impl'/2, '$tnot_st'/3, '$tnot_u'/2, '$tnot_i'/3, undefined/0, '$tbl undefined'/0, not_exists/1, tabled_call/1, '$tbl tabled_call'/1, get_residual/2, call_residual_program/2, '$call_residual_m'/3, '$tbl_rprog'/4, '$tbl_rshare'/3, '$tbl_rcls'/4, '$tbl_rneg'/2, '$tbl_ratoms'/2, '$tbl_resq'/2, '$tbl_rsk'/6, '$tbl_rval'/2, '$tbl_answered'/1, '$tbl_resid'/3, sk_not/1, get_calls/3, get_returns/2, get_call/3, get_returns_and_dls/3, '$grd'/3, '$grd_u'/3, excess_vars/4, '$xs_q'/4, '$xs_minus'/3, '$xs_memv'/2, get_returns/3, '$gr_idx'/4, get_returns_for_call/2, '$tbl_unql'/2, '$tbl_unqe'/2, call_delays/2, '$call_delays_m'/3, '$tbl_unqlm'/3, '$tbl_unqem'/3, '$tbl_conj'/2, '$tbl_del'/3, '$tbl_rounds'/0, '$tbl_round'/3, '$tbl_eval'/2, '$tbl_gshort'/2, '$tbl_runs'/5, '$tbl_run'/5, '$tbl_app'/3, '$tbl_snap'/2, '$tbl_snapat'/3, '$tbl_slice'/2, '$tbl_use'/1, '$tbl_range'/3, '$tbl_addk'/5, '$tbl_mcond'/3, '$tbl_unpair'/2, '$tbl_radd'/5, '$tbl_rput'/5, '$tbl_rhas'/2, '$tbl_fa'/3, '$tbl_ph'/1, '$tbl_raise'/0, '$tbl_dlpush'/1, '$tbl_w'/2, '$tbl_wany'/2, '$tbl_wtin'/2, '$tbl_add'/5, '$tbl_answer'/2, '$tbl_aval'/4, '$tbl_ucons'/2, '$tbl_manswer'/4, '$tbl_madd'/6, '$tbl_mput'/6, '$tbl_same'/2, '$tbl_mupd'/4, '$tbl_mupds'/4, '$tbl_mupd1'/4, '$tbl_mclos'/2, '$tbl_complete'/1, '$tbl_abort'/0, '$tbl_drop'/2, '$tbl_dgoal'/2, abolish_all_tables/0, '$tbl_drop_all'/1, '$tbl_local_variant_table'/1, current_table/2, '$tbl_strip'/3, '$tbl_lookup'/4, '$tbl_qual'/3, '$tbl_unqual'/3, '$tbl_table_status'/4 ]).
'$tbl_call'(G,I):-'$tbl_get'(G,I,plain,T),'$tbl_answer'(T,G).
'$tbl_icall'(G,I):-functor(G,F,N),('$tbl_isincr'(F/N)->true;assertz('$tbl_incrp'(F/N))),'$tbl_call'(G,I).
'$tbl_isincr'(X):-catch(call('$tbl_incrp'(X)),_,fail).
'$tbl_incrs'(L):-findall(X,'$tbl_isincr'(X),L).
'$incr_inval':-'$tbl_incrs'(L),L\==[],!,'$tbl_vt'(VT),'$trie_entries'(VT,Es),'$incr_drop'(Es,L,VT).
'$incr_inval'.
'$incr_guard'(C,P):-'$tbl_inprog'(Ts),Ts\==[],'$tbl_incrs'(L),member(T,Ts),'$tbl_dgoal'(T,G),functor(G,F,N),memberchk(F/N,L),!,throw(error(permission_error(update,incremental_table,C),P/1)).
'$incr_guard'(_,_).
'$incr_drop'([],_,_).
'$incr_drop'([e(_,G,T)|Es],L,VT):-(functor(G,F,N),memberchk(F/N,L),'$trie_meta_get'(T,0,complete)->'$trie_meta_set'(T,0,invalid);true),'$incr_drop'(Es,L,VT).
subgoal_property(MG,[incremental,V]):-get_calls(MG,T,_),'$trie_id'(T,I),'$tbl_dgoal'(I,Gv),functor(Gv,F,N),'$tbl_isincr'(F/N),'$tbl_table_status'(T,S,_,_),'$tbl_vmap'(S,V).
'$tbl_vmap'(invalid,invalid).
'$tbl_vmap'(complete,valid).
incr_assert(C):-'$incr_guard'(C,incr_assert),assertz(C),'$incr_inval'.
incr_asserta(C):-'$incr_guard'(C,incr_asserta),asserta(C),'$incr_inval'.
incr_assertz(C):-'$incr_guard'(C,incr_assertz),assertz(C),'$incr_inval'.
incr_retract(C):-'$incr_guard'(C,incr_retract),retract(C),'$incr_inval'.
incr_retractall(C):-'$incr_guard'(C,incr_retractall),retractall(C),'$incr_inval'.
incr_invalidate_calls(_):-'$incr_inval'.
incr_invalidate_call(_):-'$incr_inval'.
xwam_state(2,R):-'$tbl_dl'(DL),(DL==[]->R=0;R=1).
'$tbl_mcall'(G,I,S):-G=..[F|As],I=..[Fi|_],S=..[_|Ms0],'$tbl_msplit'(Ms0,As,Gs,Xs,Cs,Vs,Ms),'$tbl_mcheck'(Cs),Gv=..[F|Gs],Iv=..[Fi|Gs],'$tbl_mterm'(Cs,Ma),'$tbl_mterm'(Vs,Mv),'$tbl_get'(Gv,Iv,moded(Ms,Xs,Mv),T),'$tbl_manswer'(T,Xs,Ma,G).
'$tbl_get'(G,I,K,T1):-'$tbl_noattv'(G),'$tbl_cycg'(G),'$tbl_vt'(VT),('$trie_lookup'(VT,G,Tq)->'$tbl_valid'(Tq,VT,G,T);T=none),'$tbl_restrain'(T,G),'$tbl_enter'(T,VT,G,I,K,T0),('$trie_lookup'(VT,G,T1)->true;T1=T0),'$trie_meta_get'(T1,0,S),(S==complete->true;'$tbl_depn'(T1)).
'$tbl_valid'(T0,VT,G,none):-'$trie_meta_get'(T0,0,invalid),!,'$trie_delete'(VT,G,_).
'$tbl_valid'(T,_,_,T).
'$tbl_noattv'(G):-term_attvars(G,[]),!.
'$tbl_noattv'(G):-throw(error(type_error(free_of_attvar,G),_)).
'$tbh_v'(F,D,V):-('$tbh_get'(F,V0)->V=V0;V=D).
'$tbl_dl'(L):-('$tbl_dlget'(L0)->L=L0;L=[]).
'$tbl_msplit'([],[],[],[],[],[],[]).
'$tbl_msplit'([M|Ms],[A|As],[A|Gs],[A|Xs],Cs,Vs,Mo):-'$tbl_mindex'(M),!,'$tbl_msplit'(Ms,As,Gs,Xs,Cs,Vs,Mo).
'$tbl_msplit'([M|Ms],[A|As],[V|Gs],Xs,[A|Cs],[V|Vs],[M|Mo]):-'$tbl_msplit'(Ms,As,Gs,Xs,Cs,Vs,Mo).
'$tbl_mindex'(M):-var(M),!.
'$tbl_mindex'(index).
'$tbl_mindex'('+').
'$tbl_mcheck'([]).
'$tbl_mcheck'([C|Cs]):-(var(C)->'$tbl_mcheck'(Cs);throw(error(uninstantiation_error(C),_))).
'$tbl_mterm'([X],X):-!.
'$tbl_mterm'(L,T):-T=..[s|L].
'$tbl_enter'(none,VT,G,I,K,T):-!,'$trie_new'(T),'$trie_meta_set'(T,0,incomplete),'$trie_meta_set'(T,1,d(G,I,K,none,none)),'$trie_insert'(VT,0,G,T,_),('$tbh_leading'->'$tbl_inprog'(Ts),'$tbh_set'(1,[T|Ts]),'$tbl_eval1n'(T);'$tbl_lead'(T)).
'$tbl_enter'(T,_,_,_,_,T).
'$tbl_leading':-'$tbh_leading'.
'$tbl_inprog'(Ts):-'$tbh_v'(1,[],Ts).
'$tbl_lead'(T):-'$tbl_hdr','$tbh_own'(1,[T]),'$tbh_own'(2,false),'$tbh_own'(3,0),'$tbh_own'(4,false),catch(('$tbl_eval1'(T),'$tbl_rounds','$tbl_wfsrun'),E,('$tbl_abort',throw(E))),'$tbl_inprog'(Ts),'$tbl_complete'(Ts),'$tbh_close'.
'$tbl_eval1'(T):-'$tbl_hdr','$tbh_own'(5,false),'$tbl_eval'(T,_),'$tbh_v'(5,false,D),'$tbh_close',(D==false->'$tbl_done'(T);true).
'$tbl_cycg'(G):-acyclic_term(G),!.
'$tbl_cycg'(G):-catch(current_prolog_flag(max_table_subgoal_size,N),_,fail),integer(N),!,'$tbl_trip'(G).
'$tbl_cycg'(_).
'$tbl_restrain'(none,G):-!,'$tbl_rs'(G).
'$tbl_restrain'(_,_).
'$tbl_rs'(G):-catch(current_prolog_flag(max_table_subgoal_size,N),_,fail),integer(N),'$tbl_rsa'(max_table_subgoal_size_action,A),'$tbl_gdeeper'(G,N),!,'$tbl_trip'(A,G).
'$tbl_rs'(_).
'$tbl_rsa'(F,A):-(catch(current_prolog_flag(F,A0),_,fail)->A=A0;A=error),(A==error;A==failure),!.
'$tbl_gdeeper'(G,N):-G=..[_|As],M is N+2,member(A,As),'$tbl_deeper'(A,M),!.
'$tbl_deeper'(_,L):-L=<0,!.
'$tbl_deeper'(T,L):-compound(T),T=..[_|As],L1 is L-1,member(A,As),'$tbl_deeper'(A,L1),!.
'$tbl_atrip'(T):-throw(error(resource_error(tripwire(max_table_answer_size,'$trie'(T))),_)).
'$tbl_trip'(G):-(catch(current_prolog_flag(max_table_subgoal_size_action,A),_,fail)->true;A=error),'$tbl_trip'(A,G).
'$tbl_trip'(failure,_):-!,fail.
'$tbl_trip'(A,_):-A\==error,!.
'$tbl_trip'(_,G):-'$tbl_unqual'(G,M,G1),functor(G1,F,N),functor(MG,F,N),throw(error(resource_error(tripwire(max_table_subgoal_size,M:MG)),_)).
'$tbl_ans2'(A,T,_):- \+ acyclic_term(A),!,'$tbl_atrip'(T).
'$tbl_ans2'(A,T,N):-'$tbl_gdeeper'(A,N),!,'$tbl_atrip'(T).
'$tbl_ans2'(_,_,_).
'$tbl_chk'(Fs,T):-catch(current_prolog_flag(max_table_answer_size,N),_,fail),integer(N),'$tbl_rsa'(max_table_answer_size_action,error),!,'$tbl_chk2'(Fs,T,N).
'$tbl_chk'(_,_).
'$tbl_chk2'([],_,_).
'$tbl_chk2'([A|As],T,N):-'$tbl_ans2'(A,T,N),'$tbl_chk2'(As,T,N).
'$tbl_eval1n'(T):-'$tbl_hdr','$tbh_own'(5,false),'$tbl_eval'(T,_),'$tbh_v'(5,false,D),'$tbl_pos'(T,P),'$tbl_nest'(D,P,T,Up),'$tbh_close','$tbl_dup'(Up).
'$tbl_nest'(false,_,T,none):-!,'$tbl_done'(T).
'$tbl_nest'(D,P,T,Up):-D>=P,'$tbl_ph'(0),!,'$tbh_set'(5,false),'$tbl_srounds'(T),'$tbh_v'(5,false,Dr),'$tbl_scc'(Dr,P,T,Up).
'$tbl_nest'(D,_,_,D).
'$tbl_dup'(none):-!.
'$tbl_dup'(D):-'$tbh_v'(5,false,D0),'$tbl_dmin'(D0,D,D1),'$tbh_set'(5,D1).
'$tbl_scc'(Dr,P,T,none):-'$tbl_indep'(Dr,P),!,'$tbl_inprog'(Ts),'$tbl_upto'(Ts,T,Us,Rest),'$tbl_fin_all'(Us),'$tbh_set'(1,Rest).
'$tbl_indep'(false,_):-!.
'$tbl_indep'(Dr,P):-Dr>=P.
'$tbl_scc'(Dr,_,_,Dr).
'$tbl_srounds'(T):-repeat,'$tbl_inprog'(Ts0),'$tbl_upto'(Ts0,T,Us0,_),length(Us0,N0),'$tbl_round'(Us0,0,C),'$tbl_inprog'(Ts1),'$tbl_upto'(Ts1,T,Us1,_),length(Us1,N1),(C+N1-N0>0->fail;!).
'$tbl_upto'([],_,[],[]).
'$tbl_upto'([K|Ks],T,Us,Rest):-(K==T->Us=[K],Rest=Ks;Us=[K|Us1],'$tbl_upto'(Ks,T,Us1,Rest)).
'$tbl_fin_all'([]).
'$tbl_fin_all'([K|Ks]):-('$tbl_ph'(r)->'$tbl_final1'(K);true),'$trie_meta_set'(K,0,complete),'$tbl_fin_all'(Ks).
'$tbl_depn'(T):-'$tbl_pos'(T,P),'$tbh_v'(5,false,D0),'$tbl_dmin'(D0,P,D),'$tbh_set'(5,D).
'$tbl_pos'(T,P):-'$tbl_inprog'(Ts),length(Ts,L),'$tbl_idx'(Ts,T,0,I),!,P is L-I.
'$tbl_pos'(_,0).
'$tbl_idx'([K|Ks],T,I0,I):-(K==T->I=I0;I1 is I0+1,'$tbl_idx'(Ks,T,I1,I)).
'$tbl_dmin'(false,B,B):-!.
'$tbl_dmin'(A,false,A):-!.
'$tbl_dmin'(A,B,C):-C is min(A,B).
'$tbl_done'(T):-('$tbl_ph'(r)->'$tbl_final1'(T);true),'$trie_meta_set'(T,0,complete),'$tbl_inprog'(Ts),'$tbl_del'(Ts,T,Ts1),'$tbh_set'(1,Ts1).
'$tbl_wfsrun':-'$tbh_v'(2,false,F),(F==true->'$tbl_alt'(0);true).
'$tbl_alt'(N0):-'$tbl_inprog'(Ts),'$tbl_snapw'(Ts,u),'$tbh_set'(4,false),'$tbl_reset'(t),'$tbl_rounds','$tbh_v'(4,false,WN),'$tbl_alt2'(WN,N0).
'$tbl_alt2'(true,N0):-!,'$tbl_reset'(u),'$tbl_rounds','$tbl_alt'(N0).
'$tbl_alt2'(_,N0):-'$tbl_inprog'(Ts2),'$tbl_count'(Ts2,0,N1),'$tbl_snapw'(Ts2,t),(N1=:=N0->'$tbl_reset'(r),'$tbl_rounds','$tbl_inprog'(Ts3),'$tbl_final'(Ts3);'$tbl_reset'(u),'$tbl_rounds','$tbl_alt'(N1)).
'$tbl_reset'(P):-'$tbh_set'(3,P),'$tbl_inprog'(Ts),'$tbl_vt'(VT),'$tbl_renew'(Ts,VT,Ts2),'$tbh_set'(1,Ts2).
'$tbl_renew'([],_,[]).
'$tbl_renew'([T|Ts],VT,[T2|Ts2]):-'$trie_meta_get'(T,1,d(G,I,K,_,W)),'$trie_new'(T2),'$trie_meta_set'(T2,0,incomplete),'$trie_meta_set'(T2,1,d(G,I,K,none,W)),'$trie_delete'(VT,G,_),'$trie_insert'(VT,0,G,T2,_),'$trie_destroy'(T),'$tbl_renew'(Ts,VT,Ts2).
'$tbl_count'([],N,N).
'$tbl_count'([T|Ts],N0,N):-'$trie_entries'(T,L),length(L,K),N1 is N0+K,'$tbl_count'(Ts,N1,N).
'$tbl_snapw'([],_).
'$tbl_snapw'([T|Ts],Which):-'$trie_meta_get'(T,1,d(G,I,K,Sn,W0)),(W0==none->W1=w(none,none);W1=W0),W1=w(TS,US),(Which==t->Old=TS;Old=US),(Old==none->true;'$trie_destroy'(Old)),'$trie_new'(S),'$trie_entries'(T,L),'$tbl_snapins'(L,S),(Which==t->W2=w(S,US);W2=w(TS,S)),'$trie_meta_set'(T,1,d(G,I,K,Sn,W2)),'$tbl_snapw'(Ts,Which).
'$tbl_snapins'([],_).
'$tbl_snapins'([e(_,Sk,_)|L],S):-'$trie_insert'(S,0,Sk,true,_),'$tbl_snapins'(L,S).
'$tbl_final'([]).
'$tbl_final'([T|Ts]):-'$tbl_final1'(T),'$tbl_final'(Ts).
'$tbl_final1'(T):-'$trie_entries'(T,L),'$tbl_finvals'(L,T),'$trie_meta_get'(T,1,d(G,I,K,Sn,W)),(W=w(TS,US)->'$tbl_wdrop'(TS),'$tbl_wdrop'(US);true),'$trie_meta_set'(T,1,d(G,I,K,Sn,none)).
'$tbl_wdrop'(S):-(S==none->true;'$trie_destroy'(S)).
'$tbl_finvals'([],_).
'$tbl_finvals'([e(_,Sk,V)|L],T):-(nonvar(V),V=r(P)->(member(_-[],P)->NV=true;NV=u(P)),'$trie_insert'(T,1,Sk,NV,_);true),'$tbl_finvals'(L,T).
tnot(G):-'$tnot_split'(G,G1),'$tbl_vt'(VT),'$trie_lookup'(VT,G1,T),\+ '$trie_meta_get'(T,0,invalid),!,'$trie_meta_get'(T,0,S),'$tnot_st'(S,T,G1).
tnot(G):-'$tnot_split'(G,G1),'$tnot_impl'(G1,I),!,'$tbl_get'(G1,I,plain,T),'$trie_meta_get'(T,0,S),'$tnot_st'(S,T,G1).
tnot(G):- \+ G.
'$tnot_split'(G,_):-var(G),!,throw(error(instantiation_error,tnot/1)).
'$tnot_split'(M:G,G1):-atom(M),!,('$mod_cl'(M,G,0,G1)->true;G1=G).
'$tnot_split'(G,G).
'$tnot_impl'(undefined,'$tbl undefined'):-!.
'$tnot_impl'(G,I):-callable(G),G=..[F|As],atom(F),atom_concat('$tbl ',F,TF),length(As,N),current_predicate(TF/N),I=..[TF|As].
'$tnot_st'(complete,T,G):-!,'$trie_entries'(T,L),(member(e(_,_,true),L)->fail;L==[]->true;'$tbl_ph'(P),'$tnot_u'(P,G)).
'$tnot_st'(_,T,G):-'$tbl_raise','$tbl_ph'(P),'$tnot_i'(P,T,G).
'$tnot_u'(t,_):-!,'$tbl_raise',fail.
'$tnot_u'(r,G):-!,'$tbl_dlpush'(tnot(G)).
'$tnot_u'(_,G):-('$tbl_leading'->'$tbl_raise';'$tbl_dlpush'(tnot(G))).
'$tnot_i'(t,T,_):-!,('$tbl_w'(T,w(_,US)),US\==none-> \+ '$tbl_wany'(T,u);'$tbh_set'(4,true)).
'$tnot_i'(r,T,G):-!,\+ '$tbl_wany'(T,t),('$tbl_wany'(T,u)->'$tbl_dlpush'(tnot(G));true).
'$tnot_i'(_,T,_):- \+ '$tbl_wany'(T,t).
undefined:-'$tbl_call'(undefined,'$tbl undefined').
'$tbl undefined':-tnot(undefined).
not_exists(G):-'$tnot_split'(G,G1),ground(G1),'$tnot_impl'(G1,_),!,tnot(G1).
not_exists(G):-'$tnot_split'(G,G1),'$tbl_unqual'(G1,M,G2),(tabled_call(M:G2),fail;tnot(tabled_call(M:G2))).
tabled_call(G):-'$tbl_call'(tabled_call(G),'$tbl tabled_call'(G)).
'$tbl tabled_call'(G):-call(G).
get_residual(G0,DL):-'$tnot_split'(G0,G),'$tbl_vt'(VT),'$trie_entries'(VT,L),member(e(_,V,T),L),'$tbl_answered'(T),\+ \+ V=G,copy_term(V,Vc),Vc=G,term_variables(Vc,Vs),Sk=..[ret|Vs],'$trie_entries'(T,As),member(e(_,Sk,Val),As),'$tbl_resid'(Val,Sk,D),'$tbl_unql'(D,DL).
call_residual_program(G,P):-'$call_residual_m'(user,G,P).
'$call_residual_m'(M,G,P):-'$call_delays_m'(M,G,_),'$tbl_rprog'([G],M,[],P).
'$tbl_rprog'([],_,_,[]).
'$tbl_rprog'([A|As],M,Seen,P):-member(S,Seen),S=@=A,!,'$tbl_rprog'(As,M,Seen,P).
'$tbl_rprog'([A|As],M,Seen,P):-findall(A-D,('$tbl_resq'(A,D),D\==[]),Rs0),'$tbl_rshare'(Rs0,A,Rs),'$tbl_rcls'(Rs,M,Cs,New),append(As,New,As2),'$tbl_rprog'(As2,M,[A|Seen],P2),append(Cs,P2,P).
'$tbl_rshare'([],_,[]).
'$tbl_rshare'([A2-D|R],A,[A2-D|R2]):-(A2=@=A->A2=A;true),'$tbl_rshare'(R,A,R2).
'$tbl_rcls'([],_,[],[]).
'$tbl_rcls'([A-D0|Rs],M,[(H:-B)|Cs],New):-'$tbl_rneg'(D0,D),'$tbl_unqem'(A,M,H),'$tbl_unqlm'(D,M,Du),'$tbl_conj'(Du,B),'$tbl_ratoms'(D,N1),'$tbl_rcls'(Rs,M,Cs,N2),append(N1,N2,New).
'$tbl_rneg'([],[]).
'$tbl_rneg'([L|Ls],[L2|Ls2]):-(L=tnot(G)->copy_term(G,G2),L2=tnot(G2);L2=L),'$tbl_rneg'(Ls,Ls2).
'$tbl_ratoms'([],[]).
'$tbl_ratoms'([tnot(A)|Ls],[A|As]):-!,'$tbl_ratoms'(Ls,As).
'$tbl_ratoms'([A|Ls],[A|As]):-'$tbl_ratoms'(Ls,As).
'$tbl_resq'(G,D):-'$tbl_vt'(VT),'$trie_entries'(VT,L),member(e(_,V,T),L),'$tbl_answered'(T),\+ \+ V=G,'$trie_meta_get'(T,1,d(G0,_,K,_,_)),'$tbl_rsk'(K,G0,V,G,Sk,Mv),'$trie_entries'(T,As),member(e(_,Sk,Val),As),'$tbl_rval'(Mv,Val),'$tbl_resid'(Val,Sk,D).
'$tbl_rsk'(moded(_,Xs0,Mv0),G0,_,G,Sk,Mv):-!,copy_term(G0-Xs0-Mv0,G-Xs-Mv),term_variables(Xs,Vs),Sk=..[ret|Vs].
'$tbl_rsk'(_,_,V,G,Sk,none):-copy_term(V,G),term_variables(G,Vs),Sk=..[ret|Vs].
'$tbl_rval'(Mv,_):-Mv==none,!.
'$tbl_rval'(Mv,Val):-(nonvar(Val),Val=mu(V,_)->Mv=V;Mv=Val).
'$tbl_answered'(T):-'$trie_meta_get'(T,0,S),(S==complete->true;S==invalid).
'$tbl_resid'(true,_,[]).
'$tbl_resid'(u(P),Sk,D):-member(Sk2-D,P),Sk2=Sk.
'$tbl_resid'(mu(_,D),_,D).
sk_not(G):-not_exists(G).
get_calls(MG,T,R):-'$tbl_strip'(MG,M,G),'$tbl_vt'(VT),'$trie_entries'(VT,L),member(e(_,_,I),L),T='$trie'(I),'$tbl_table_status'(T,_,M:G,R).
get_returns(T,R):-'$trie_id'(T,I),'$trie_entries'(I,As),member(e(_,R,_),As).
get_call(MG,T,R):-'$tbl_strip'(MG,M,G),get_calls(M:G0,T,R0),G0=@=G,!,G0=G,R=R0.
get_returns_and_dls(T,R,DLs):-'$trie_id'(T,I),'$trie_entries'(I,As),member(e(_,R,V),As),'$grd'(V,R,DLs).
'$grd'(u(P),R,DLs):-!,'$grd_u'(P,R,DLs).
'$grd'(_,_,[]).
'$grd_u'([],_,[]).
'$grd_u'([Sk-D|P],R,Out):-(Sk=R->'$tbl_unql'(D,DL),Out=[DL|O2];Out=O2),'$grd_u'(P,R,O2).
excess_vars(T,E,A,V):-'$xs_q'(T,T2,[],Q),term_variables(E-Q,EV),term_variables(T2,TV),'$xs_minus'(TV,EV,R),append(A,R,V).
'$xs_q'(T,T,Q,Q):-var(T),!.
'$xs_q'(X^G,G2,Q0,Q):-!,'$xs_q'(G,G2,[X|Q0],Q).
'$xs_q'(T,T,Q,Q).
'$xs_minus'([],_,[]).
'$xs_minus'([V|Vs],E,R):-'$xs_memv'(V,E),!,'$xs_minus'(Vs,E,R).
'$xs_minus'([V|Vs],E,[V|R]):-'$xs_minus'(Vs,E,R).
'$xs_memv'(V,[W|Ws]):-(V==W->true;'$xs_memv'(V,Ws)).
get_returns(T,R,N):-'$trie_id'(T,I),'$trie_entries'(I,As),'$gr_idx'(As,1,R,N).
'$gr_idx'([e(_,R,_)|_],N,R,N).
'$gr_idx'([_|As],N0,R,N):-N1 is N0+1,'$gr_idx'(As,N1,R,N).
get_returns_for_call(MV,A):-current_table(MV,T),'$tbl_table_status'(T,_,_:A,Sk),'$trie_id'(T,I),'$trie_entries'(I,As),member(e(_,Sk,_),As).
'$tbl_unql'([],[]).
'$tbl_unql'([E|Es],[F|Fs]):-'$tbl_unqe'(E,F),'$tbl_unql'(Es,Fs).
'$tbl_unqe'(tnot(G),tnot(G2)):-!,'$tbl_unqual'(G,_,G2).
'$tbl_unqe'(G,G2):-'$tbl_unqual'(G,_,G2).
call_delays(G,D):-'$call_delays_m'(user,G,D).
'$call_delays_m'(M,G,D):-'$tbl_dl'(D0),'$tbl_dlset'([]),call(G),'$tbl_dl'(DL),reverse(DL,DL1),'$tbl_unqlm'(DL1,M,DL2),'$tbl_conj'(DL2,D),append(DL,D0,D1),'$tbl_dlset'(D1).
'$tbl_unqlm'([],_,[]).
'$tbl_unqlm'([E|Es],M,[F|Fs]):-'$tbl_unqem'(E,M,F),'$tbl_unqlm'(Es,M,Fs).
'$tbl_unqem'(tnot(G),M,tnot(G2)):-!,'$tbl_unqem'(G,M,G2).
'$tbl_unqem'(G,M,G3):-'$tbl_unqual'(G,Mg,G2),(Mg==M->G3=G2;Mg==user->G3=G2;G3=Mg:G2).
'$tbl_conj'([],true):-!.
'$tbl_conj'([X],X):-!.
'$tbl_conj'([X|Xs],(X,C)):-'$tbl_conj'(Xs,C).
'$tbl_del'([],_,[]).
'$tbl_del'([K|R],T,S):-(K==T->S=R;S=[K|S1],'$tbl_del'(R,T,S1)).
'$tbl_rounds':-repeat,'$tbl_inprog'(Ks0),(Ks0==[]->!;length(Ks0,N0),'$tbl_round'(Ks0,0,C),'$tbl_inprog'(Ks1),length(Ks1,N1),(C+N1-N0>0->fail;!)).
'$tbl_round'([],C,C).
'$tbl_round'([K|T],C0,C):-'$tbl_eval'(K,N),C1 is C0+N,'$tbl_round'(T,C1,C).
'$tbl_eval'(T,N):-'$trie_meta_get'(T,1,d(G,_,_,_,_)),ground(G),'$tbl_gshort'(T,N),!.
'$tbl_gshort'(T,0):- \+ '$tbl_ph'(r),'$trie_entries'(T,[_|_]),!.
'$tbl_gshort'(T,N):-'$tbl_ph'(r),'$tbl_wtin'(T,ret),('$trie_entries'(T,[_|_])->N=0;'$trie_insert'(T,0,ret,r([ret-[]]),_),N=1).
'$tbl_eval'(T,N):-'$trie_meta_get'(T,1,d(G,I,K,Sn,W)),'$tbl_hdr','$tbh_own'(6,all),'$tbh_own'(7,0),'$tbh_own'(8,0),'$tbh_own'(9,[]),'$tbl_runs'(Sn,G,I,Fs,Mx),'$tbh_v'(9,[],U),'$tbh_close','$tbl_snap'(U,Ps),'$trie_meta_set'(T,1,d(G,I,K,s(Mx,Ps),W)),'$tbl_addk'(K,Fs,T,G,N).
'$tbl_runs'(none,G,I,Fs,Mx):-!,'$tbh_bset'(6,all),'$tbh_bset'(7,0),'$tbh_set'(8,0),'$tbl_fa'(G,I,Fs),'$tbh_v'(8,0,Mx).
'$tbl_runs'(s(Mx0,Ps),G,I,Fs,Mx):-'$tbh_set'(8,Mx0),'$tbl_run'(1,Ps,G,I,Fs),'$tbh_v'(8,0,Mx).
'$tbl_run'(R,Ps,G,I,Fs):-'$tbh_v'(8,0,M),(R>M->Fs=[];'$tbh_bset'(6,sn(R,Ps)),'$tbh_bset'(7,0),'$tbl_fa'(G,(I,'$tbh_v'(7,0,J),J>=R),F1),R1 is R+1,'$tbl_run'(R1,Ps,G,I,F2),'$tbl_app'(F1,F2,Fs)).
'$tbl_app'([],L,L).
'$tbl_app'([X|Xs],L,[X|Ys]):-'$tbl_app'(Xs,L,Ys).
'$tbl_snap'([],[]).
'$tbl_snapat'([],_,0).
'$tbl_snapat'([K-V|Ps],C,E):-(K==C->E=V;'$tbl_snapat'(Ps,C,E)).
'$tbl_snap'([C|Cs],[C-E|Ps]):-'$trie_slice'(C,0,0,E-_),'$tbl_snap'(Cs,Ps).
'$tbl_slice'(T,L):-'$trie_meta_get'(T,0,S),(S==complete->'$trie_entries'(T,L);'$tbl_use'(T),'$tbl_range'(T,F,To),'$trie_slice'(T,F,To,_-L)).
'$tbl_use'(T):-'$tbh_v'(9,[],U),('$var_memberchk'(T,U)->true;'$tbh_set'(9,[T|U])).
'$tbl_range'(C,0,-1):-'$tbh_v'(6,all,X),X=sn(_,Ps),'$tbl_snapat'(Ps,C,E),E>=1,'$trie_meta_get'(C,1,d(G,_,_,_,_)),ground(G),!.
'$tbl_range'(C,F,To):-'$tbh_v'(7,0,J0),J is J0+1,'$tbh_bset'(7,J),'$tbh_v'(8,0,M),(J>M->'$tbh_set'(8,J);true),'$tbh_v'(6,all,X),(X=sn(R,Ps)->'$tbl_snapat'(Ps,C,E),(J<R->F=0,To=E;J=:=R->F=E,To= -1;F=0,To= -1);F=0,To= -1).
'$tbl_addk'(plain,Fs,T,G,N):-'$tbl_ph'(r),!,'$tbl_unpair'(Fs,As),'$tbl_chk'(As,T),'$tbl_radd'(Fs,T,G,0,N).
'$tbl_addk'(plain,Fs,T,G,N):-'$tbl_chk'(Fs,T),'$tbl_add'(Fs,T,G,0,N).
'$tbl_addk'(moded(Ms,Xs,Mv),Fs0,T,G,N):-('$tbl_ph'(r)->'$tbl_unpair'(Fs0,Fs);Fs=Fs0),'$tbl_madd'(Fs,T,G-Xs-Mv,Ms,0,N),('$tbl_ph'(r)->'$tbl_mcond'(Fs0,T,G-Xs-Mv);true).
'$tbl_mcond'([],_,_).
'$tbl_mcond'([A-D|R],T,P):-(D==[]->true;copy_term(P,Gc-Xc-_),term_variables(Xc,Vs),Gc=A,Sk=..[ret|Vs],('$trie_lookup'(T,Sk,V),(var(V);V\=mu(_,_))->'$trie_delete'(T,Sk,_),'$trie_insert'(T,0,Sk,mu(V,D),_);true)),'$tbl_mcond'(R,T,P).
'$tbl_unpair'([],[]).
'$tbl_unpair'([A-_|R],[A|S]):-'$tbl_unpair'(R,S).
'$tbl_radd'([],_,_,N,N).
'$tbl_radd'([A-D|As],T,G,N0,N):-copy_term(G,Gc),term_variables(Gc,Vs),Gc=A,Sk=..[ret|Vs],reverse(D,D1),'$tbl_rput'(T,Sk,D1,N0,N1),'$tbl_radd'(As,T,G,N1,N).
'$tbl_rput'(T,Sk,D,N0,N):-'$trie_lookup'(T,Sk,r(P0)),!,('$tbl_rhas'(P0,Sk-D)->N=N0;'$trie_insert'(T,1,Sk,r([Sk-D|P0]),_),N is N0+1).
'$tbl_rput'(T,Sk,D,N0,N):-'$trie_insert'(T,0,Sk,r([Sk-D]),_),N is N0+1.
'$tbl_rhas'(P,X):-member(Y,P),'$tbl_same'(Y,X),!.
'$tbl_fa'(G,Goal,Fs):-'$tbl_ph'(r),!,'$tbl_dl'(D0),findall(G-D,('$tbl_dlset'([]),Goal,'$tbl_dl'(D)),Fs),'$tbl_dlset'(D0).
'$tbl_fa'(G,Goal,Fs):-ground(G),!,(once(Goal)->Fs=[G];Fs=[]).
'$tbl_fa'(G,Goal,Fs):-findall(G,Goal,Fs).
'$tbl_ph'(P):-'$tbh_v'(3,0,P0),P=P0.
'$tbl_raise':-'$tbh_set'(2,true),'$tbh_set'(5,0).
'$tbl_dlpush'(E):-'$tbl_dl'(L),'$tbl_dlset'([E|L]).
'$tbl_w'(T,W):-'$trie_meta_get'(T,1,d(_,_,_,_,W0)),(W0==none->W=w(none,none);W=W0).
'$tbl_wany'(T,Which):-'$tbl_w'(T,w(TS,US)),(Which==t->S=TS;S=US),S\==none,'$trie_entries'(S,[_|_]),!.
'$tbl_wtin'(T,Sk):-'$tbl_w'(T,w(TS,_)),TS\==none,'$trie_lookup'(TS,Sk,_),!.
'$tbl_add'([],_,_,N,N).
'$tbl_add'([A|As],T,G,N0,N):-copy_term(G,Gc),term_variables(Gc,Vs),Gc=A,Sk=..[ret|Vs],'$trie_insert'(T,0,Sk,true,R),(R>=0->N1 is N0+1;N1=N0),'$tbl_add'(As,T,G,N1,N).
'$tbl_answer'(T,G):-term_variables(G,Vs),Sk=..[ret|Vs],'$tbl_slice'(T,L),member(e(_,Sk,V),L),'$tbl_aval'(V,T,G,Sk).
'$tbl_aval'(true,_,_,_):-!.
'$tbl_aval'(r(_),T,G,Sk):-!,('$tbl_wtin'(T,Sk)->true;'$tbl_dlpush'(G)).
'$tbl_aval'(u(_),_,G,_):-'$tbl_ph'(P),'$tbl_ucons'(P,G).
'$tbl_ucons'(t,_):-!,'$tbl_raise',fail.
'$tbl_ucons'(r,G):-!,'$tbl_dlpush'(G).
'$tbl_ucons'(_,G):-('$tbl_leading'->'$tbl_raise';'$tbl_dlpush'(G)).
'$tbl_manswer'(T,Xs,Ma,Gv):-term_variables(Xs,Vs),Sk=..[ret|Vs],'$tbl_slice'(T,L),member(e(_,Sk,V),L),(nonvar(V),V=mu(V0,_)->Ma=V0,'$tbl_ph'(P),'$tbl_ucons'(P,Gv);Ma=V).
'$tbl_madd'([],_,_,_,N,N).
'$tbl_madd'([A|As],T,P,Ms,N0,N):-copy_term(P,Gc-Xc-Mc),term_variables(Xc,Vs),Gc=A,Sk=..[ret|Vs],'$tbl_mput'(T,Sk,Ms,Mc,N0,N1),'$tbl_madd'(As,T,P,Ms,N1,N).
'$tbl_mput'(T,Sk,Ms,M,N0,N):-'$trie_lookup'(T,Sk,Old0),!,(nonvar(Old0),Old0=mu(Old,DL)->W=1;Old=Old0,W=0),('$tbl_mupd'(Ms,Old,M,New)->true;New=Old),('$tbl_same'(New,Old)->N=N0;'$trie_delete'(T,Sk,_),(W==1->NV=mu(New,DL);NV=New),'$trie_insert'(T,0,Sk,NV,_),N is N0+1).
'$tbl_mput'(T,Sk,_,M,N0,N):-'$trie_insert'(T,0,Sk,M,Q),(Q>=0->N is N0+1;N=N0).
'$tbl_same'(A,B):-copy_term(A-B,A1-B1),numbervars(A1,0,N),numbervars(B1,0,N),A1==B1.
'$tbl_mupd'([M],O,N,R):-!,'$tbl_mupd1'(M,O,N,R).
'$tbl_mupd'(Ms,O,N,R):-O=..[s|Os],N=..[s|Ns],'$tbl_mupds'(Ms,Os,Ns,Rs),R=..[s|Rs].
'$tbl_mupds'([],[],[],[]).
'$tbl_mupds'([M|Ms],[O|Os],[N|Ns],[R|Rs]):-'$tbl_mupd1'(M,O,N,R),'$tbl_mupds'(Ms,Os,Ns,Rs).
'$tbl_mupd1'(first,O,_,O):-!.
'$tbl_mupd1'('-',O,_,O):-!.
'$tbl_mupd1'(last,_,N,N):-!.
'$tbl_mupd1'(min,O,N,R):-!,(O@<N->R=O;R=N).
'$tbl_mupd1'(max,O,N,R):-!,(O@>N->R=O;R=N).
'$tbl_mupd1'(sum,O,N,R):-!,R is O+N.
'$tbl_mupd1'(lattice(P),O,N,R):-!,'$tbl_mclos'(P,C),call(C,O,N,R),!.
'$tbl_mupd1'(po(P),O,N,R):-!,'$tbl_mclos'(P,C),(call(C,O,N)->R=O;R=N).
'$tbl_mupd1'(M,_,_,_):-throw(error(domain_error(tabled_mode,M),_)).
'$tbl_mclos'(M:P,M:C):-!,'$tbl_mclos'(P,C).
'$tbl_mclos'(N/_,N):-!.
'$tbl_mclos'(H,N):-compound(H),!,functor(H,N,_).
'$tbl_mclos'(N,N).
'$tbl_complete'([]).
'$tbl_complete'([T|Ts]):-'$trie_meta_set'(T,0,complete),'$tbl_complete'(Ts).
'$tbl_abort':-'$tbl_inprog'(Ts),'$tbl_vt'(VT),'$tbl_drop'(Ts,VT),'$tbh_set'(1,[]).
'$tbl_drop'([],_).
'$tbl_drop'([T|Ts],VT):-('$tbl_dgoal'(T,G),'$trie_delete'(VT,G,_)->true;true),'$trie_destroy'(T),'$tbl_drop'(Ts,VT).
'$tbl_dgoal'(T,G):-'$trie_meta_get'(T,1,d(G,_,_,_,_)).
abolish_all_tables:-'$tbl_leading',!,throw(error(permission_error(abolish,table,all),abolish_all_tables/0)).
abolish_all_tables:-'$tbl_vt'(VT),'$trie_entries'(VT,L),'$tbl_drop_all'(L),'$trie_destroy'(VT).
'$tbl_drop_all'([]).
'$tbl_drop_all'([e(_,_,T)|L]):-'$trie_destroy'(T),'$tbl_drop_all'(L).
'$tbl_local_variant_table'('$trie'(VT)):-'$tbl_vt'(VT).
current_table(MV,T):-'$tbl_strip'(MV,M,V),'$tbl_vt'(VT),(nonvar(V)->'$tbl_lookup'(VT,M,V,I);'$trie_entries'(VT,L),member(e(_,QV,I),L),'$tbl_unqual'(QV,M,V)),T='$trie'(I).
'$tbl_strip'(MV,M,V):-nonvar(MV),MV=M0:V0,!,(var(M)->M=M0;true),'$tbl_strip'(V0,M,V).
'$tbl_strip'(MV,M,V):-nonvar(MV),MV='$lfx_mc'(M0,V0),!,(var(M)->M=M0;true),'$tbl_strip'(V0,M,V).
'$tbl_strip'(V,_,V).
'$tbl_lookup'(VT,M,V,I):-atom(M),M\==user,'$tbl_qual'(M,V,QV),'$trie_lookup'(VT,QV,I),!.
'$tbl_lookup'(VT,_,V,I):-'$trie_lookup'(VT,V,I).
'$tbl_qual'(M,V,QV):-callable(V),V=..[F|As],atom_concat(M,':',P),atom_concat(P,F,QF),QV=..[QF|As].
'$tbl_unqual'(QV,M,V):-callable(QV),QV=..[QF|As],atom(QF),sub_atom(QF,B,1,A,':'),B>0,!,sub_atom(QF,0,B,_,M),B1 is B+1,sub_atom(QF,B1,A,0,F),V=..[F|As].
'$tbl_unqual'(V,user,V).
'$tbl_table_status'(T,S,M:W,Sk):-'$trie_id'(T,I),'$trie_meta_get'(I,0,S),'$tbl_dgoal'(I,G),'$tbl_unqual'(G,M0,W0),term_variables(W0,Vs),Sk=..[ret|Vs],M=M0,W=W0.
