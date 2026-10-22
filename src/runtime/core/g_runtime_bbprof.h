#ifndef G_RUNTIME_BBPROF_H
#define G_RUNTIME_BBPROF_H
#include <stdint.h>
#include <time.h>
#define BBPROF_PC_CAP 2048
struct g_bbprof_e;
struct g_bbprof_pc { uintptr_t pc; uint64_t n; };
typedef struct {
    struct g_bbprof_e *tab;
    int n;
    int cap;
    int sorted;
    int armed;
    struct g_bbprof_e *live_tab;
    int live_n;
    int late_n;
    uint64_t c_samples;
    uint64_t unattr;
    uint64_t total;
    struct g_bbprof_pc pcs[BBPROF_PC_CAP];
    char altstack[65536];
    timer_t timer;
    int timer_ok;
} g_bbprof_t;
#endif
