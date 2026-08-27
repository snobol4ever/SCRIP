/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef ZETA_DEPTH_H
#define ZETA_DEPTH_H
#include "IR.h"
#ifdef __cplusplus
extern "C" {
#endif
#define ZDP_BOT ((int)0x80000000)
#define ZDP_TOP ((int)0x7fffffff)
enum { ZDP_OK = 0, ZDP_UNREACHED = 1, ZDP_JOIN = 2, ZDP_DYN = 3, ZDP_RESUME = 4, ZDP_ALTRET = 5 };
enum { ZDP_TIER_SPINE = 0, ZDP_TIER_ACTIVATION = 1, ZDP_TIER_STANDING = 2 };
int  zdp_mode(void);
int  zdp_analyze(IR_graph_t * g);
int  zdp_alpha(const IR_t * nd);
int  zdp_beta(const IR_t * nd);
int  zdp_known_alpha(const IR_t * nd);
int  zdp_known_beta(const IR_t * nd);
int  zdp_resume_sound(const IR_t * nd);
int  zdp_why(const IR_t * nd);
int  zdp_tier(const IR_t * nd);
int  zdp_inside(const IR_t * nd);
int  zdp_scratch_cell(const IR_t * nd);
int  zdp_scan_pure(const IR_t * nd);
int  zdp_seam_tier(const IR_t * nd);
void zdp_report(IR_graph_t * g, const char * tag);
void zdp_port_census(IR_graph_t * g);
void zdp_unwind_census(IR_graph_t * g);
void zdp_bomb_census(IR_graph_t * g, const char * tag);
int  zzone_plan(IR_graph_t * g);
int  zzone_tier_of(const IR_t * nd);
int  zzone_off_of(const IR_t * nd);
#ifdef __cplusplus
}
#endif
#endif
