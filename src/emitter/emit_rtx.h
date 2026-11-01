#ifndef EMITTER_EMIT_RTX_H
#define EMITTER_EMIT_RTX_H
#ifdef __cplusplus
extern "C" {
#endif
int emit_rtx_entry_is(const char * sym);
unsigned emit_rtx_clob_mask(const char * sym);
void * emit_rtx_twin(const char * sym, const char ** cname, int * kind);
const char * emit_rtx_twin_sym(const char * sym);
void * emit_rtx_twin_addr(const char * sym, void * addr);
#ifdef __cplusplus
}
#endif
#endif
