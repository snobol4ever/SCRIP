#ifndef SCRIP_PLAN_FACTS_H
#define SCRIP_PLAN_FACTS_H
#ifndef __cplusplus
#error plan_facts.h is C++ only (it names the plan record)
#endif
#include "ir/ir_fwd.h"
struct zeta_plan_t;
void planner_facts(zeta_plan_t * rec, IR_t ** nodes, int n, const IR_graph_t * g);
void planner_blob_facts(zeta_plan_t * rec);
#endif
