/*  xsb_builtins.pl -- XSB BUILTINS THAT SCRIP LACKS, AS A PROLOG LAYER (cfo 2026-10-24, row prolog-xsbtests-to-100-the-58-failing-programs-ceo-1926; ceo CEO-1958: XSB 5.0.0 is the
    xsb_tests oracle, CEO-1927, and an XSB-only predicate is debt implemented as XSB does). SCRIP's own text, AGPL like the rest of SCRIP; embedded by the generator in LOCAL mode and
    parsed only when a program names one of its exports. import/1 and export/1 answer XSB's module directives ':- import P from M.' and ':- export P.' (the package prelude declares
    XSB's operators import fy 1050, export fx 1100, from xfx 1040): SCRIP has one predicate namespace, so a predicate an XSB system module exports is already visible and the
    directive succeeds having changed nothing. set_arg/3 is XSB machine's set_arg(Term, Index, Arg) :- term_set_arg(Term, Index, Arg, -1), the backtrackable destructive
    assignment (XSB syslib/machine.P; constraint_tests/testsetarg's ref shows the old argument back after a failure), which is SWI's setarg/3 with its arguments reordered. */
:- module(xsb_builtins, [ import/1, export/1, set_arg/3 ]).
import(_).
export(_).
set_arg(Term, Index, Arg) :- setarg(Index, Term, Arg).
