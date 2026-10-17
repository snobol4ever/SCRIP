/*  xsb_tables.pl -- XSB TABLE-MAINTENANCE PREDICATES SWI'S xsb DIALECT DEFINES (hq_prolog 2026-10-17, row prolog-swi-tabling-the-xsb-loader-runs-the-xsb-test-drivers, SWI
    xsb/table_tests abol_test3*). gc_tables(-Remaining) counts the abolished tables that are still waiting to be reclaimed. SCRIP destroys a table when it is abolished
    (abolish_table_pred/1, abolish_all_tables/0 free its trie at once), so none is ever waiting and the count is 0, the value swipl 9.0.4 reports once its atom collector
    has run. Embedded by the generator in LOCAL mode; linked for a program that references load_files (prolog_parse.c's load_files block). */
:- module(xsb_tables, [ gc_tables/1 ]).
gc_tables(0).
