#ifndef RT_PROTECTED_H
#define RT_PROTECTED_H
#ifdef __cplusplus
extern "C" {
#endif
int is_protected_pat_name(const char *name);
int protected_pat_name_to_sm_op(const char *name);
#ifdef __cplusplus
}
/*--------------------------------------------------------------------------------------------------------------------*/
#endif
#endif
