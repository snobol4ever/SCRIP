#ifndef ZETA_DEPTH_H
#define ZETA_DEPTH_H
#include "IR.h"
#ifdef __cplusplus
extern "C" {
#endif
/*===================================================================================================================================================================================================*/
/* ZETA-DEPTH (ZDP) -- THE ONE STATIC-STACK-HEIGHT LATTICE.  Replaces the hand-written admission estimators (leaf_frame_candidate, blob_choice_rbp_scan, the s128 5-term gate, the s130 8-byte window   */
/* law, the s131 BAL decline) with the predicate every one of them was approximating: is the rsp depth at this program point statically known?  Lon's model (s132) is the standard result -- offsets are */
/* (depth at use) - (depth at allocation), total wherever depth is path-invariant, failing in exactly two ways: (a) an unknown COUNT of allocations between a RESULT's write and its read, (b) a beta    */
/* label reached with an unknown accumulation.  BOTH ARE ONE PREDICATE: the meet at a join point disagreed.  Case (a) is a backedge join; case (b) is the omega fan-in join.  Same lattice, same TOP,    */
/* same cure (address off a base that does not move).  The failure list stops being a list and becomes the TOP set -- the list becomes the code, which is the whole point of the rung.                   */
/*===================================================================================================================================================================================================*/
#define ZDP_BOT ((int)0x80000000)
#define ZDP_TOP ((int)0x7fffffff)
enum { ZDP_OK = 0, ZDP_UNREACHED = 1, ZDP_JOIN = 2, ZDP_DYN = 3, ZDP_RESUME = 4 };
int  zdp_mode(void);
int  zdp_analyze(IR_graph_t * g);
int  zdp_alpha(const IR_t * nd);
int  zdp_beta(const IR_t * nd);
int  zdp_known_alpha(const IR_t * nd);
int  zdp_known_beta(const IR_t * nd);
int  zdp_resume_sound(const IR_t * nd);
int  zdp_why(const IR_t * nd);
void zdp_report(IR_graph_t * g, const char * tag);
void zdp_port_census(IR_graph_t * g);
#ifdef __cplusplus
}
#endif
#endif
