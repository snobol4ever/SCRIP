/* ==================================================================================================================== */
/* si_09_pat_fns_native.sc — SI-9 native counterpart: pattern function calls (LEN + SPAN with capture).                  */
/* ==================================================================================================================== */
&ANCHOR = 0;
&FULLSCAN = 1;
OUTPUT = '--- interp ---';
S = 'abc123';
S ? LEN(3) . LET SPAN('0123456789') . NUM;
OUTPUT = 'LET=' LET;
OUTPUT = 'NUM=' NUM;
OUTPUT = '--- done ---';
