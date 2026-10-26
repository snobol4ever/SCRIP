#ifdef __cplusplus
#include <unordered_map>
#include "g_runtime.h"
#include "ct_arena.h"
#include "ct_vec.h"
extern "C" int prolog_atom_intern_lit(const char *);
extern "C" int prolog_functor_arity(int);
#include <string>
#include <climits>
#include "emit.h"
#include "gc_frame_map.h"
#include "dtp.h"
#include "ir_index.h"
#include "ir_node_index.h"
#include "templates/x86/x86_asm.h"
#include "templates/bb/bb_templates.h"
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <algorithm>
#endif
#ifdef __cplusplus
extern "C" {
#endif
#include "driver/scrip_sm.h"
#include <string>
#include "stage2.h"
#include "templates/bb/bb_templates.h"
#include "templates/xa/xa_templates.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "g_emitter.h"
extern g_emitter_t g_emitter;
extern const char * bb_src_of(const IR_t * nd);
extern int bb_line_of(const IR_t * nd);
extern const char * stmt_src_get_file(void);
extern int stmt_src_nlines(void);
extern bb_emit_mode_t bb_emit_mode;
extern int g_sm_native_unsupported;
extern FILE *bb_emit_out;
extern bb_medium_t g_medium;
extern void (*g_emit_chain_posthook)(void);
extern int g_use_sm_macros;
extern int g_use_bb_macros;
extern sm_emit_t g_emit;
extern IR_graph_t * g_emit_cfg;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern "C" const char * bb_ab_sym_name(const char * nm);
