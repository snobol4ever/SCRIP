/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef PL_PARSE_H
#define PL_PARSE_H
#include "runtime/rt_pl_reader.h"
void prolog_preprocess(PlProgram *prog);
PlProgram *prolog_parse(const char *src, const char *filename);
void prolog_inject_prelude(PlProgram *prog, const char *user_src);
void prolog_rewrite_incremental_dynamics(PlProgram *prog);
void prolog_dcg_expand(PlClause *cl);
int pl_prelude_defines(const char *nm, int ar);
const char *pl_prelude_lib_private(const char *lib, const char *nm, int ar);
void prolog_program_free(PlProgram *prog);
int prolog_op_user_count(void);
int prolog_op_user_get(int i, const char **name_out, int *prec_out, const char **type_out);
#endif
