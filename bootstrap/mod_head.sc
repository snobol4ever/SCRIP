/* mod_head.sc -- the first statement of a LINKED parser module (stage 1 of the bootstrap, Lon 2026-10-14: "get all 7 parsers   */
/* linked into SCRIP and running"; ARCH-PARSER-SC-BOOTSTRAP.md section 3.4). The build concatenates mod_head.sc, the fourteen    */
/* library files, parser_<lang>.sc and mod_tail.sc, and scrip --module <lang> --compile makes the object the runtime links.       */
/* The C entry (src/runtime/rt/rt_sc_module.c) runs the top level once -- the library and the parser build their patterns and     */
/* the tail returns -- and after that every entry jumps straight to ModParse with ModSrc and ModPath set (the tail goes on to      */
/* ModEntry, the line the Makefile writes last); ModTree carries the tree back. ModLinked is set by the C entry before the first   */
/* run, so the parser's own stdin driver is skipped (its goto ModEnd).                                                              */
if (DIFFER(ModInited)) goto ModParse;
