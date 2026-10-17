/*  xsb_ordsets.pl -- XSB'S ordsets ADDITION TO library(ordsets) (hq_prolog 2026-10-17, row prolog-swi-tabling-the-xsb-loader-runs-the-xsb-test-drivers, SWI xsb/ai_tests).
    ord_setproduct/3 is swipl 9.0.4's library(dialect/xsb/ordsets) clauses (BSD, Jan Wielemaker, VU University Amsterdam), which SWI's xsb dialect loads for
    ':- import ... from ordsets'; the rest of XSB's ordsets is library(ordsets). Embedded by the generator in LOCAL mode; linked for a program that references
    load_files (prolog_parse.c's load_files block), since a file loaded at run time can call only what the static program linked. */
:- module(xsb_ordsets, [ ord_setproduct/3 ]).
ord_setproduct([], _, []).
ord_setproduct([Head|Tail], Set, SetProduct) :- '$xos_product'(Set, Head, SetProduct, Rest), ord_setproduct(Tail, Set, Rest).
'$xos_product'([], _, Set, Set).
'$xos_product'([Head|Tail], X, [X-Head|TailX], Tl) :- '$xos_product'(Tail, X, TailX, Tl).
