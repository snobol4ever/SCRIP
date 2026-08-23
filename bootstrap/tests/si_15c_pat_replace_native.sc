/* si_15c_pat_replace_native.sc — native counterpart. */
&ANCHOR = 0;
&FULLSCAN = 1;
OUTPUT = '--- interp ---';
S = 'hello world';
S ? 'world' = 'there';
OUTPUT = S;
OUTPUT = '--- done ---';
