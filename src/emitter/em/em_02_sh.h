#ifndef EMITTER_EM_EM_02_SH_H
#define EMITTER_EM_EM_02_SH_H
#include "emitter/em/em_head.h"
extern "C" { __attribute__((visibility("hidden"))) void xa_pair_push(struct bb_label_t * d, struct bb_label_t * j); }
extern "C" { int emit_resumable_frame(void); }
extern "C" { int bb_ab_sym_is_thunk_stem(const char * sym); }
extern "C" { int bb_call_route_classify(IR_t * nd); }
extern "C" { void call_prepare(IR_t * nd); }
extern "C" { __attribute__((visibility("hidden"))) void flat_drive_cleanup(IR_t **nodes, int n, int i, bb_label_t **lbls, bb_label_t **betas, bb_label_t *node_γ, bb_label_t *node_ω, int zread); }
extern "C" { void drive_arg_slots_reserve(int n); }
extern "C" { void * bb_thunk_rec_addr(const char * name); }
extern "C" { void * bb_dstar_rec_addr(const char * star); }
extern "C" { __attribute__((visibility("hidden"))) std::string icn_gen_line_save(void); }
extern "C" { __attribute__((visibility("hidden"))) std::string icn_gen_line_resume(void); }
extern "C" { void display_copy(IR_t *nd); }
void defer_prepare(void);
uint64_t emit_double_bits(double d);
#endif
