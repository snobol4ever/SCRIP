/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef KEYWORDS_H
#define KEYWORDS_H
#include "../contracts/descr.h"
extern long g_error;
extern long g_trace;
extern long g_dump;
extern long g_random;
extern int  g_jcon;
int         kw_assign(const char *kw, DESCR_t val);
int         kw_can_assign(const char *kw, DESCR_t val);
DESCR_t     kw_read(const char *kw);
DESCR_t     rt_keyword_read(const char *sval);
DESCR_t     rt_keyword_read_snobol4(const char *sval);
DESCR_t     rt_keyword_gen(const char *sval, long idx);
int         rt_kw_direct_on(void);
const char *rt_kw_direct_sym(int idx, int *soff, const void **base);
void        rt_kw_seed_defaults(void);
void        rt_kw_bind(void *block, int n);
int         rt_kw_index(const char *kw);
DESCR_t     rt_kw_read_idx(int64_t idx);
DESCR_t     rt_kw_write_idx(int64_t idx, DESCR_t v);
const char *kw_cset_name(const char *ptr);
int         kw_cset_len(const char *ptr);
const char *kw_cset_const_str(const char *kw);
void        rt_kw_publish_error(int code, const char *msg);
#endif
