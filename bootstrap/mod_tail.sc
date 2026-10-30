/* mod_tail.sc -- the last statements of the LINKED parser module (see mod_head.sc): the first run ends here and returns to C;    */
/* every later run arrives at ModParse from mod_head.sc and goes to ModEntry, the one line the Makefile writes after this file: a */
/* wired language jumps to its parser's parse label (ModSrc is the text; the parser leaves with ModTree set through its ModEnd,   */
/* which falls into the return above), an unwired one returns with ModTree null. The line sits after this file so that the first  */
/* run, which ends at the return above, never falls into it. The build inserts ModPrefix markers between the files, and scrip     */
/* --module qualifies every name by its file's prefix (Lon 2026-10-14: one global space, a 3-letter prefix per parser: sno_ snc_  */
/* rak_ reb_ icn_ pro_ pas_; the library sc_).                                                                                    */
if (DIFFER(ModLinked)) { ModInited = 1; return; }
ModParse: ModTree = ;
ModErrPos = -1;
goto ModEntry;
