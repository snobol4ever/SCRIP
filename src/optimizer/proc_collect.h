/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef PROC_COLLECT_H
#define PROC_COLLECT_H
#include "IR.h"
void proc_collect_reset(void);
int proc_slot_of(const char *name);
int proc_collect_add(const char *name);
int proc_slot_count(void);
const char *proc_slot_name(int k);
int proc_direct_eligible(const char *name);
void proc_collect_graph(IR_graph_t *g);
#endif
