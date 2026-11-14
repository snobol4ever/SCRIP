#include <string>
#include <cstdint>
extern "C" {
#include "emitter/emit_state_.h"
#include "bb_templates.h"
}
#include "templates/x86/x86_ops.h"
#define CAP_FROM() RDD("rbp", _.op_off + 8 + 8 * (int)_.op_ival)
#define CAP_TO() RDD("rbp", _.op_off + 12 + 8 * (int)_.op_ival)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_rkcaps() {
    x86_begin();
    return _.op_off > -16
         ? x86_alpha() + x86_bomb("IR_MATCH_RKCAPS: the landing node's capture block was not granted") + x86_beta() + x86_omega()
         : x86("comment", "IR_MATCH_RKCAPS: open capture group, from = delta, to = none")
         + x86_alpha()
         + x86("mov", CAP_FROM(), "r14d")
         + x86("mov", CAP_TO(), (long)-1)
         + x86_gamma()
         + x86_beta()
         + x86_omega();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
std::string bb_match_rkcape() {
    x86_begin();
    return _.op_off > -16
         ? x86_alpha() + x86_bomb("IR_MATCH_RKCAPE: the landing node's capture block was not granted") + x86_beta() + x86_omega()
         : x86("comment", "IR_MATCH_RKCAPE: close capture group, to = delta; beta reopens it")
         + x86_alpha()
         + x86("mov", CAP_TO(), "r14d")
         + x86_gamma()
         + x86_beta()
         + x86("mov", CAP_TO(), (long)-1)
         + x86_omega();
}
