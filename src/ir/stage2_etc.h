#ifndef STAGE2_ETC_H
#define STAGE2_ETC_H
#include <stdint.h>
#include "ir/stage2.h"
#include "ir/stage2_planner_road.h"
struct IR_graph_t;
#ifdef __cplusplus
extern "C" {
#endif
void emit_proc_prepare(stage2_t *s2, int pi, const planner_road_t *rd, int *isp_out);
int emit_jmp_entry_for_road(const planner_road_t *rd, IR_graph_t *g, const char *pname);
void emit_flat_road(planner_road_t *rd);
#ifdef __cplusplus
}
#endif
#endif
