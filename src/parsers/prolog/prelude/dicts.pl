/*  dicts.pl -- SWI-Prolog 7's dicts as a Prolog layer (cfo 2026-10-11, row prolog-swi-dicts-and-their-functional-notation-d-get-
    key-are-a-parse-error-advent-demo, CEO-1660; the class is the cfo's, CEO-1664).  SCRIP's own text, AGPL like the rest of SCRIP:
    not a vendored file.  The generator (util_gen_prolog_prelude_libs.py) embeds it in LOCAL mode: no SWI reader, no renames, its
    helpers '$dict_'-named by hand.  It is parsed only when a program names one of its exports -- or holds a dict dot, which the
    lowerer turns into a call of '$dict_eval'/3 (prolog_inject_prelude adds that key when the program holds one).
    is_dict/1,2 live in the base prelude (prolog_parse.c), which library(error) and library(option) reach without this file.

    THE TERM.  The reader reads Tag{K1:V1, ...} as '$dict_lit'(Tag, K1, V1, ...) in source order, and the lowerer's first pass
    (lower_prolog.c pl_dict_rewrite) places it as '$dict'(Tag, V1, K1, ..., Vn, Kn), keys in the standard order of terms (small
    integers before atoms), the layout of SWI's C'dict' -- so two dicts unify exactly when their tags and their key sets and values
    do.  The writer prints '$dict'/odd as Tag{K:V,...}.  Every predicate here takes that term apart with =../2 and builds it with
    keysort/2, so a dict built at run time has the same layout as one the lowerer placed.

    THE DOT.  Obj.Func in a clause is read as '$dot'(Obj, Func) and expanded by the lowerer into '$dict_eval'(Obj, Func, V) ahead of
    the goal that holds it (in a head, ahead of the body): a key answers its value or raises existence_error(key, Key, Dict);
    get(K) fails on a missing key; get(K, Default); put(New) and put(K, V) answer a new dict.  A non-dict raises type_error(dict, X).

    MEASURED AGAINST swipl 9.0.4 (2026-10-11): printing and dict_pairs/3 order keys as here; get_dict/3 with an unbound key
    enumerates in SWI's internal atom order (a, z, m for z, a, m), here in standard order (a, m, z) -- the one named difference.
    NOT HERE: user-defined dict methods (Tag:Func := Body), the :< and >:< operators, dicts in read_term at run time.
*/
:- module(dicts, [ get_dict/3, get_dict/5, put_dict/3, put_dict/4, del_dict/4, dict_pairs/3, dict_create/3, select_dict/3, (:<)/2, (>:<)/2,
                   b_set_dict/3, nb_set_dict/3, '$dict_eval'/3 ]).
'$dict_need'(D) :- is_dict(D), !.
'$dict_need'(D) :- var(D), !, throw(error(instantiation_error, _)).
'$dict_need'(D) :- throw(error(type_error(dict, D), _)).
'$dict_conv'(D, D) :- is_dict(D), !.
'$dict_conv'(L, D) :- is_list(L), !, dict_create(D, _, L).
'$dict_conv'(D, _) :- '$dict_need'(D).
'$dict_vk'([], []).
'$dict_vk'([V, K|R], [K-V|P]) :- '$dict_vk'(R, P).
'$dict_kvs'(D, T, P) :- D =.. ['$dict', T|VK], '$dict_vk'(VK, P).
'$dict_build'(T, P, D) :- '$dict_keys_ok'(P), keysort(P, S), '$dict_nodup'(S), '$dict_vk'(VK, S), D =.. ['$dict', T|VK].
'$dict_nodup'([]).
'$dict_nodup'([K-_|R]) :- '$dict_nodup'(R, K).
'$dict_nodup'([], _).
'$dict_nodup'([K-_|R], K0) :- ( K == K0 -> throw(error(duplicate_key(K), _)) ; '$dict_nodup'(R, K) ).
'$dict_norm'(L, _) :- var(L), !, throw(error(instantiation_error, _)).
'$dict_norm'([], []) :- !.
'$dict_norm'([E|Es], [K-V|R]) :- !, '$dict_kv'(E, K, V), '$dict_norm'(Es, R).
'$dict_norm'(L, _) :- throw(error(type_error(list, L), _)).
'$dict_kv'(E, _, _) :- var(E), !, throw(error(instantiation_error, _)).
'$dict_kv'(K-V, K, V) :- !.
'$dict_kv'(K=V, K, V) :- !.
'$dict_kv'(K:V, K, V) :- !.
'$dict_kv'(E, K, V) :- compound(E), E =.. [K, V], !.
'$dict_kv'(E, _, _) :- throw(error(type_error(pair, E), _)).
'$dict_keys_ok'([]).
'$dict_keys_ok'([K-_|R]) :- '$dict_key_ok'(K), '$dict_keys_ok'(R).
'$dict_key_ok'(K) :- var(K), !, throw(error(instantiation_error, _)).
'$dict_key_ok'(K) :- ( atom(K) ; integer(K) ), !.
'$dict_key_ok'(K) :- throw(error(type_error('dict-key', K), _)).
dict_pairs(D, T, P) :- is_dict(D), !, '$dict_kvs'(D, T, P).
dict_pairs(_, T, P) :- var(T), var(P), !, throw(error(instantiation_error, _)).
dict_pairs(D, T, P) :- '$dict_norm'(P, P1), '$dict_build'(T, P1, D).
dict_create(D, T, P) :- '$dict_norm'(P, P1), '$dict_build'(T, P1, D).
get_dict(K, D, V) :- '$dict_need'(D), '$dict_kvs'(D, _, P), ( var(K) -> '$dict_member'(P, K, V) ; '$dict_get'(P, K, V0), V = V0 ).
'$dict_member'([K0-V0|R], K, V) :- ( K = K0, V = V0 ; '$dict_member'(R, K, V) ).
'$dict_get'([K0-V0|R], K, V) :- ( K0 == K -> V = V0 ; '$dict_get'(R, K, V) ).
get_dict(K, D, V, ND, NV) :- '$dict_conv'(D, D1), get_dict(K, D1, V), put_dict(K, D1, NV, ND).
put_dict(K, D, V, ND) :- '$dict_need'(D), '$dict_kvs'(D, T, P), '$dict_put'(P, K, V, P1), '$dict_build'(T, P1, ND).
'$dict_put'([], K, V, [K-V]).
'$dict_put'([K0-V0|R], K, V, O) :- ( K0 == K -> O = [K-V|R] ; O = [K0-V0|O1], '$dict_put'(R, K, V, O1) ).
put_dict(New, D, ND) :- '$dict_need'(D), ( is_dict(New) -> '$dict_kvs'(New, _, NP) ; '$dict_norm'(New, NP) ), '$dict_kvs'(D, T, P),
    '$dict_putall'(NP, P, P1), '$dict_build'(T, P1, ND).
'$dict_putall'([], P, P).
'$dict_putall'([K-V|R], P, O) :- '$dict_put'(P, K, V, P1), '$dict_putall'(R, P1, O).
del_dict(K, D, V, ND) :- '$dict_need'(D), '$dict_kvs'(D, T, P), '$dict_del'(P, K, V, P1), '$dict_build'(T, P1, ND).
'$dict_del'([K0-V0|R], K, V, O) :- ( K0 == K -> V = V0, O = R ; O = [K0-V0|O1], '$dict_del'(R, K, V, O1) ).
'$dict_eval'(D, F, V) :- '$dict_conv'(D, D1), '$dict_fn'(F, D1, V).
select_dict(S, F, R) :- '$dict_conv'(S, S1), '$dict_conv'(F, F1), '$dict_kvs'(S1, T, SP), '$dict_kvs'(F1, T, FP), '$dict_sel'(SP, FP, RP), '$dict_build'(_, RP, R).
'$dict_sel'([], R, R).
'$dict_sel'([K-V|S], F, R) :- '$dict_take'(F, K, V0, F1), V = V0, '$dict_sel'(S, F1, R).
'$dict_take'([K0-V0|R], K, V, O) :- ( K0 == K -> V = V0, O = R ; O = [K0-V0|O1], '$dict_take'(R, K, V, O1) ).
':<'(S, F) :- select_dict(S, F, _).
'>:<'(A, B) :- '$dict_conv'(A, A1), '$dict_conv'(B, B1), '$dict_kvs'(A1, T, AP), '$dict_kvs'(B1, T, BP), '$dict_join'(AP, BP).
'$dict_join'([], _).
'$dict_join'([K-V|R], P) :- ( '$dict_get'(P, K, V0) -> V = V0 ; true ), '$dict_join'(R, P).
b_set_dict(K, D, V) :- '$dict_need'(D), '$dict_slot'(K, D, I), setarg(I, D, V).
nb_set_dict(K, D, V) :- '$dict_need'(D), '$dict_slot'(K, D, I), nb_setarg(I, D, V).
'$dict_slot'(K, D, I) :- D =.. [_, _|VK], ( '$dict_slot_'(VK, K, 2, I0) -> I = I0 ; throw(error(existence_error(key, K, D), _)) ).
'$dict_slot_'([_, K0|R], K, I0, I) :- ( K0 == K -> I = I0 ; I1 is I0 + 2, '$dict_slot_'(R, K, I1, I) ).
'$dict_fn'(F, D, V) :- var(F), !, get_dict(F, D, V).
'$dict_fn'(get(K), D, V) :- !, get_dict(K, D, V).
'$dict_fn'(get(K, Df), D, V) :- !, ( get_dict(K, D, V0) -> V = V0 ; V = Df ).
'$dict_fn'(put(N), D, V) :- !, put_dict(N, D, V).
'$dict_fn'(put(K, X), D, V) :- !, put_dict(K, D, X, V).
'$dict_fn'(K, D, V) :- atomic(K), !, ( get_dict(K, D, V0) -> V = V0 ; throw(error(existence_error(key, K, D), context(system:'$get_dict_ex'/3, _))) ).
'$dict_fn'(F, D, _) :- throw(error(existence_error(dict_function, F), D)).
