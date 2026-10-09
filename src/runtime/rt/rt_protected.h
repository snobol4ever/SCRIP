/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef RT_PROTECTED_H
#define RT_PROTECTED_H
#ifdef __cplusplus
extern "C" {
#endif
static inline __attribute__((always_inline)) int is_protected_pat_lead(char c) { return c == 'A' || c == 'B' || c == 'F' || c == 'R' || c == 'S'; }
int is_protected_pat_name(const char *name);
int protected_pat_name_to_sm_op(const char *name);
#ifdef __cplusplus
}
#endif
#endif
