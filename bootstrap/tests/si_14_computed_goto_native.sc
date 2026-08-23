/* ================================================================================================================================ */
/* si_14_computed_goto_native.sc — SI-14 native counterpart.                                                                       */
/* Snocone source has no native `:($L)` syntax — host parser does not produce TT_GOU as expr.                                       */
/* The native simulates the observable behavior: a forward jump to a labeled section.                                              */
/* ================================================================================================================================ */
OUTPUT = '--- interp ---';
L = 'TARGET';                        /* not needed for native simulation, kept for symmetry */
goto target;
OUTPUT = 'miss';                     /* unreachable */
target: OUTPUT = 'hit';
OUTPUT = '--- done ---';
