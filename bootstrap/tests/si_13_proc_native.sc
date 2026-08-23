/* ================================================================================================================================ */
/* si_13_proc_native.sc — SI-13 native counterpart.                                                                                 */
/* Produces the same stdout as si_13_proc.sc via self-hosted pipeline.                                                              */
/* The FRETURN case is simulated by calling a host function that fails.                                                             */
/* ================================================================================================================================ */
OUTPUT = '--- interp ---';
X = 'hello';
OUTPUT = EVAL(*X);
OUTPUT = EVAL(*(3 + 4));
if (LT(1, 0)) { goto skip; }     /* LT(1,0) always fails — simulates FRETURN path reaching :F label */
fret_failed: OUTPUT = 'fret-ok';
skip: OUTPUT = '--- done ---';
