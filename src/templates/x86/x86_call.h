#ifndef X86_CALL_H
#define X86_CALL_H
#include <string>
#include "emitter/bb_label.h"
#include "templates/x86/x86_jump.h"
extern "C++" {
const char * x86_portname(int p);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
struct bb_label_t * x86_portlbl(int p);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int x86_diag_regs_on();
std::string x86_port_hook(int site, int port, const char * lbl = NULL);;
int x86_pl_trace_on();
int x86_fc_on();
std::string x86_fc_jcc_omega(const char * mnem);
std::string x86_fc_jcc_gamma(const char * mnem);
int x86_fc_hit(int off);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int x86_port_of(const char * s);
#define X86_PL_TR_ENTRY_BYTES 32L
#define X86_PL_TR_ARENA_MASK (-536870912L)
std::string x86_pl_tr_pop_head();
std::string x86_pl_tr_pop_restore();
std::string x86_pl_tr_top_sync();
#define X86_PL_TR_HEADER_BYTES 160L
#define X86_PL_TR_WAKE_OFF 48L
#define X86_PL_TR_RESIDUE_OFF 56L
#define X86_PL_TR_FLOOR_OFF 64L
#define X86_PL_TR_CATCH_OFF 112L
std::string x86_pl_choice_mark(const std::string & mark_mem, int l0);
std::string x86_pl_tr_tidy(int l0);
std::string x86_pl_header_open(int hoff, int prev_off);
std::string x86_pl_catch_link(int rec);
std::string x86_pl_catch_open(int rec, struct bb_label_t * resume);
std::string x86_pl_catch_unlink(int rec);
std::string x86_pl_frame_choice_open(const char * fb, int kt, int l_take, int l_done, int l_mark);
std::string x86_release_acts(bool below_rsp, int kind = 4);
std::string x86_pl_scc_release_check(const char * fb, int kt, int l_ok);
std::string x86_pl_ball_rule(int l_go, int l_skip);
std::string x86_pl_ball_rule_omega(int l_go);
std::string x86_pl_alarm_probe();
std::string x86_pl_tr_unwind_at(const std::string & mark_mem, int l_loop, int l_done);
int x86_trace_hooks_on();
}
#endif
