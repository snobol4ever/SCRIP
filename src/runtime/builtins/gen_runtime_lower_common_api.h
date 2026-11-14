#ifndef GEN_RUNTIME_LOWER_COMMON_API_H
#define GEN_RUNTIME_LOWER_COMMON_API_H
#ifdef __cplusplus
extern "C" {
#endif
int is_global(const char *name);
void global_register(const char *name);
#ifdef __cplusplus
}
#endif
#endif
