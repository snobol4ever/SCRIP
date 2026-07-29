/* util_rtx_defer_arm_census.c — ARM-LEVEL 0(d) for the defer family (s215).
 * The s214 census (util_rtx_defer_census.c) counts CALLS. That is necessary and NOT sufficient: s212's
 * WRONG-TEMPLATE-ARM lesson says a discharged 0(d) proves a SYMBOL executes, never WHICH ARM. A port of
 * rt_defer_open's plain-NV_GET arm grades a guaranteed 1.000x if the hot traffic is the '*' star arm, and
 * vice versa. This interposer splits the traffic by the SAME predicates the C body branches on, in order:
 *   varname==NULL | varname[0]=='*' | strcmp(varname,"FAIL")==0 | plain
 * and additionally reports ival_flag!=0 (the template emits xor esi,esi, so this is expected to be 0) and
 * the star sub-split (latch-hit vs rt_proc_call_open owed-call), which is where g_star_peek lives.
 * Build/run: see util_rtx_defer_census.c header conventions; LD_PRELOAD against mode-3 or mode-4. */
#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
typedef struct { int32_t v; uint32_t slen; union { long i; double r; void *p; } u; } D16;
static long (*o_open)(const char *, int); static int (*o_close)(int); static long (*o_step)(D16);
static void *(*o_getpat)(const char *, int);
static unsigned long a_null, a_star, a_fail, a_plain, a_ival, a_star_owed, a_star_res, a_close, a_step, a_gp_star, a_gp_plain, a_gp_hit, a_gp_miss;
long rt_defer_open(const char *n, int iv)
{
    if (!o_open) o_open = dlsym(RTLD_NEXT, "rt_defer_open");
    if (iv) a_ival++;
    if (!n) a_null++;
    else if (n[0] == '*') a_star++;
    else if (!strcmp(n, "FAIL")) a_fail++;
    else a_plain++;
    long r = o_open(n, iv);
    if (n && n[0] == '*') { if (r) a_star_owed++; else a_star_res++; }
    return r;
}
static unsigned long a_cl_fail, a_cl_adv0, a_cl_adv1, a_cl_advs, a_cl_advl;
int rt_defer_close(int c)
{
    if (!o_close) o_close = dlsym(RTLD_NEXT, "rt_defer_close");
    a_close++; int r = o_close(c);
    if (r < 0) a_cl_fail++; else { int d = r - c; if (d == 0) a_cl_adv0++; else if (d == 1) a_cl_adv1++; else if (d <= 8) a_cl_advs++; else a_cl_advl++; }
    return r;
}
long rt_defer_step(D16 d) { if (!o_step) o_step = dlsym(RTLD_NEXT, "rt_defer_step"); a_step++; return o_step(d); }
void *rt_defer_get_pat_fn(const char *n, int iv)
{
    if (!o_getpat) o_getpat = dlsym(RTLD_NEXT, "rt_defer_get_pat_fn");
    if (n && n[0] == '*') a_gp_star++; else a_gp_plain++;
    void *r = o_getpat(n, iv);
    if (r) a_gp_hit++; else a_gp_miss++;
    return r;
}
__attribute__((destructor)) static void rpt(void)
{
    fprintf(stderr, "[0d-arm] open: plain=%lu star=%lu fail=%lu null=%lu ival_nonzero=%lu\n", a_plain, a_star, a_fail, a_null, a_ival);
    fprintf(stderr, "[0d-arm] star split: owed_call=%lu resolved_inline=%lu\n", a_star_owed, a_star_res);
    fprintf(stderr, "[0d-arm] get_pat_fn: star=%lu plain=%lu -> fn=%lu null=%lu\n", a_gp_star, a_gp_plain, a_gp_hit, a_gp_miss);
    fprintf(stderr, "[0d-arm] close=%lu step=%lu\n", a_close, a_step);
    fprintf(stderr, "[0d-arm] close arms: fail=%lu adv0(SNUL)=%lu adv1=%lu adv2-8=%lu adv>8=%lu\n", a_cl_fail, a_cl_adv0, a_cl_adv1, a_cl_advs, a_cl_advl);
}
