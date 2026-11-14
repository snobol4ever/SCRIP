#ifndef SCRIP_PLANNER_H
#define SCRIP_PLANNER_H
#include "ir/ir_fwd.h"
#include "ir/stage2.h"
#include "ir/stage2_planner_road.h"
struct zeta_plan_t;
void planner_dump_homes(const IR_graph_t * g, IR_t ** nodes, int n, const unsigned char * zon);
void planner_dump_plan(const IR_graph_t * g, const zeta_plan_t * pl, const char * prefix);
void planner_dump_homes_on(void);
void planner_dump_plan_on(void);
void planner_road_none(planner_road_t * rd);
void planner_road_named(const char * pname, int dyn_scope, int is_generator, const IR_graph_t * g, planner_road_t * rd);
void planner_road_pattern(planner_road_t * rd);
void planner_road_proc(stage2_t * s2, int pi, planner_road_t * rd, int rt);
void planner_proc_road(stage2_t * s2, int pi, int rt, planner_road_t * rd);
const planner_road_t * planner_road_of(const IR_t * entry);
void planner_road_floor_dc(stage2_t * s2, int pi, int rt, int * floor, int * dc_np);
int planner_port_exit_promotes(const stage2_t * s2, int pi);
void planner_chain_operands(IR_t * entry);
int planner_blob_casmark(void);
int planner_blob_head_bytes(void);
int planner_road_blob_scope(const planner_road_t * rd);
void planner_stage_chain(IR_graph_t * g, IR_t * entry, const char * prefix, const planner_road_t * rd);
void planner_graph_facts(IR_graph_t * g);
void planner_stage_proc(stage2_t * s2, int pi, int mode);
void planner_stage_procs(stage2_t * s2, int from_pi, int mode);
void planner_stage_main(IR_graph_t * bbg, const char * prefix);
int proc_role3_kind(const IR_graph_t * g);
int planner_stage_rt(IR_graph_t * g, IR_t * entry, const char * prefix, const planner_road_t * rd);
void planner_stage_release(IR_t * entry);
#endif
