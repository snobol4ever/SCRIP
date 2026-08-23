/* ==================================================================================================================== */
/* sm_interp_test_native.sc — SI-5 native counterpart.                                                                   */
/* Natural Snocone source equivalent to the AST that sm_interp_test.sc hand-builds.                                       */
/* Native scrip --run on this must produce the same stdout as the self-hosted pipeline running sm_interp_test.sc.      */
/* ==================================================================================================================== */
OUTPUT = '--- interp ---';
X = 2 + 3;
OUTPUT = X * 4;
OUTPUT = '--- done ---';
