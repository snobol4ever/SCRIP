/* mod_tail.sc -- the last statements of the LINKED parser module (see mod_head.sc): the first run ends here and returns to C;    */
/* every later run arrives at ModParse from mod_head.sc, parses ModSrc through the parser ModLang names and returns with ModTree. */
/* The build inserts ModPrefix markers between the files, and scrip --module qualifies every name by its file's prefix (Lon      */
/* 2026-10-14: one global space, a 3-letter prefix per parser: sno_ snc_ rak_ reb_ icn_ pro_ pas_; the library sc_).             */
if (DIFFER(ModLinked)) { ModInited = 1; return; }
ModParse: ModTree = ;
ModErrPos = -1;
if (IDENT(ModLang, 'snobol4')) ModTree = sno_ParseTree(ModSrc, ModPath);
return;
