#ifndef SCRIP_ACT_WORDS_H
#define SCRIP_ACT_WORDS_H
#include "planner/zeta_plan.h"
enum {
    ACT_NONE = 0, ACT_DEFERRED_PATTERN = 1, ACT_STATIC_PATTERN = 2, ACT_PATTERN_FUNCTION = 3, ACT_ARBNO_ITERATION = 4, ACT_RESUME_ROAD = 5, ACT_BOUNDED_ARM = 6, ACT_BOUNDED_OTHER = 7, ACT_HOME = 8,
        ACT_REGEX_KIT = 9
};
enum { ACT_CLASS_NONE = 0, ACT_CLASS_ARBNO = 1, ACT_CLASS_CAPTURE = 2, ACT_CLASS_FENCE = 3, ACT_CLASS_CHOICE = 5, ACT_CLASS_VALUE = 6, ACT_CLASS_HOME = 7, ACT_CLASS_REGEX = 8 };
const char * zeta_name(int z);
int planner_frame_slot(const IR_graph_t * g, const IR_t * nd);
int planner_frame_slot_at(const IR_graph_t * g, const IR_t * nd, IR_t ** nodes, int n, const int * claim, const int * xb);
int planner_read_distance(const IR_graph_t * cg, IR_t ** nodes, int n, const int * zarm, int i, int k, const int * zout, const int * xb);
int planner_graph_retains(const IR_graph_t * g);
void planner_consumers_build(const IR_graph_t * g);
int planner_consumers_of(const IR_graph_t * g, const IR_t * nd, const int ** con);
int planner_rec_home_g(const IR_graph_t * cg, const IR_t * nd);
int planner_rec_home(const IR_t * nd);
const zrd_t * planner_rec_read(int i, int j);
extern "C" int planner_act_cause(const IR_graph_t * g, const IR_t * nd, int cls, int lo, int hi, int in_arm, int in_repeat, int is_root);
extern "C" const char * planner_act_cause_name(int cause);
extern "C" const char * planner_act_class_name(int cls);
extern "C" void planner_act_build(const IR_graph_t * g, int blob, int head);
extern "C" int planner_act_slot(const IR_t * nd);
extern "C" int planner_act_value_slot(const IR_t * nd);
extern "C" int planner_act_class_of(const IR_t * nd);
extern "C" int planner_act_cause_of(const IR_t * nd);
extern "C" int planner_act_extra(const IR_t * mb);
extern "C" int planner_blob_slot_bytes(void);
extern "C" int planner_act_window(const IR_t * arbno, int * lo, int * bytes);
extern "C" void planner_act_visit(void (*fn)(const IR_t *, int, int, int));
extern "C" int planner_body_live(const IR_graph_t * g, const IR_t * first, const IR_t * last, const IR_t * self);
extern "C" int planner_alt_arm_exits(const IR_graph_t * g, const IR_t * alt, int j);
extern "C" int planner_alt_arm_live(const IR_graph_t * g, const IR_t * alt, int j);
extern "C" int planner_alt_exit(const IR_graph_t * g, const IR_t * alt, int * M);
extern "C" int planner_fence_k(const IR_graph_t * g, const IR_t * nd);
extern "C" int planner_read_extra(const IR_graph_t * cg, IR_t ** nodes, int n, const int * zarm, int i, int k);
extern "C" int planner_alt_k(const IR_graph_t * g, const IR_t * nd);
extern "C" int planner_blob_exit_live(const IR_graph_t * g);
#endif
