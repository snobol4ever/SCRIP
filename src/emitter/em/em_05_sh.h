#ifndef EMITTER_EM_EM_05_SH_H
#define EMITTER_EM_EM_05_SH_H
#include "emitter/em/em_head.h"
extern "C" { __attribute__((visibility("hidden"))) void emit_gc_rsp_reset(void); }
extern "C" { __attribute__((visibility("hidden"))) const char * sr3_gamma_label(const IR_t * g); }
extern "C" { __attribute__((visibility("hidden"))) int gc_sites_report_on(void); }
extern "C" { __attribute__((visibility("hidden"))) void emit_gc_sites_data(const char * fam, int frameless); }
#endif
