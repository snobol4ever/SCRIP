#ifndef EMITTER_EMIT_KNOBS_H
#define EMITTER_EMIT_KNOBS_H
#define CSET_CHAIN_MAX 3
#ifdef __cplusplus
extern "C" {
#endif
int sn4_cset32(void);
int sn4_defer_beta_guard(void);
int sn4_define_fold(void);
int sn4_descr_stamp(void);
int sn4_u2_fence(void);
int sn4_nret_cap(void);
int sn4_cap_gva(void);
int sn4_opt_binimm_off(void);
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
int emit_knob_unless_zero(const char * name);
int emit_knob_nonzero(const char * name);
int emit_knob_if_one(const char * name);
#endif
#endif
