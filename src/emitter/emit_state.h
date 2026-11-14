#ifndef EMITTER_EMIT_STATE_H
#define EMITTER_EMIT_STATE_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
void sig_prepare(const char * fname, int nargs);
void sig_prepare_define(const char * fname, const char * entry, int role);
void sig_prepare_chain(const char * fname);
#ifdef __cplusplus
}
#endif
#endif
