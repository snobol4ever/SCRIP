/* ==================================================================================================================== */
/* si_12_call_builtin_native.sc — SI-12 native counterpart: same stdout as si_12_call_builtin.sc via self-hosted pipe.   */
/* Calls SIZE, SUBSTR, TRIM directly via host Snocone — no AST building, no sm_interp machinery.                         */
/* ==================================================================================================================== */
OUTPUT = '--- interp ---';
OUTPUT = SIZE('hello');
OUTPUT = SUBSTR('hello', 2, 3);
OUTPUT = TRIM('  hi  ');
OUTPUT = '--- done ---';
