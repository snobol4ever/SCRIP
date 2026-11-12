#ifndef RUNTIME_LIB_BN_12_RK_H
#define RUNTIME_LIB_BN_12_RK_H
#include "runtime/lib/bn_head.h"
__attribute__((visibility("hidden"))) int gram_get_flavor(const char *qname);
__attribute__((visibility("hidden"))) const char *gram_get(const char *qname);
int rt_grammar_has_top(const char *gname);
__attribute__((visibility("hidden"))) int rk_match_is(DESCR_t d);
__attribute__((visibility("hidden"))) const char *rk_match_text(DESCR_t d);
__attribute__((visibility("hidden"))) int rk_match_is_nil(DESCR_t d);
__attribute__((visibility("hidden"))) int rk_agg_truth(DESCR_t v);
int rk_is_truthy(DESCR_t v);
__attribute__((visibility("hidden"))) DESCR_t rk_match_obj_in(const Match *m, const char *subj, DESCR_t orig);
__attribute__((visibility("hidden"))) DESCR_t rk_match_obj(const Match *m, const char *subj);
__attribute__((visibility("hidden"))) int grammar_parse_core(const char *gname, const char *subj, DESCR_t *out, int partial, DESCR_t actions);
__attribute__((visibility("hidden"))) int rk_mu_method(const char *m, DESCR_t self, int nmargs, DESCR_t *out);
__attribute__((visibility("hidden"))) int rk_uni_is_name(const char *nm);
__attribute__((visibility("hidden"))) DESCR_t rk_hash_list(DESCR_t h, int what);
__attribute__((visibility("hidden"))) int rk_pre_value(const char *nm, DESCR_t *out);
__attribute__((visibility("hidden"))) int rk_list_coercion_meth(const char *m, int nargs);
__attribute__((visibility("hidden"))) int rk_match_method(const char *m, DESCR_t *args, int nargs, DESCR_t *out);
const char *rt_main_progname(void);
void rt_main_args_gc_root(void);
int rt_main_args_count(void);
const char *rt_main_arg_at(int i);
__attribute__((visibility("hidden"))) const char *rk_match_render(DESCR_t d, int use_gist);
__attribute__((visibility("hidden"))) const char *rk_dyn_text(DESCR_t d, int use_gist);
__attribute__((visibility("hidden"))) const char *rk_obj_default_raku(DESCR_t d);
#endif
