#ifndef RT_SC_MODULE_H
#define RT_SC_MODULE_H
#include <stdint.h>
typedef struct {
    void *module_init;
    void *main_alpha;
    const char **gva_names;
    long n_gva;
    void **alpha_tab;
    const void *gc_maps;
    const void *gc_sites;
    int64_t *proc_tab;
    const char **proc_names;
    long n_proc;
    const char *name;
} sc_module_desc_t;
#endif
