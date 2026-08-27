/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef GVA_COLLECT_H
#define GVA_COLLECT_H
int gva_name_eligible(const char *name);
void gva_collect_reset(void);
int gva_index_of(const char *name);
int gva_collect_var(const char *name);
int gva_count(void);
const char *gva_name(int k);
void gva_io_refuse_name(const char *name);
struct IR_graph_t;
void gva_io_refuse_scan_graph(struct IR_graph_t *g);
#endif
