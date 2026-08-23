/* ==================================================================================================================== */
/* si_11_compare_native.sc — SI-11 native counterpart: produces the same stdout as si_11_compare.sc via the              */
/* self-hosted pipeline.  Snocone has no infix `<` / `LGT` operators in source, so the native counterpart simulates the  */
/* same control-flow effect — print 'lt-ok' on the LT branch, then 'lgt-ok' on the LGT branch — using host builtins      */
/* directly.  Both LT(3,5) and LGT('beta','alpha') succeed, so both lines print.                                         */
/* ==================================================================================================================== */
OUTPUT = '--- interp ---';
if (LT(3, 5))             OUTPUT = 'lt-ok';
if (LGT('beta','alpha'))  OUTPUT = 'lgt-ok';
OUTPUT = '--- done ---';
