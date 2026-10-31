#ifndef SETEXIT_H
#define SETEXIT_H
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
void core_set_label_exists_hook(int (*fn)(const char *));
const char *setexit_label_get(char *buf, size_t bufsz);
void sno_setexit_fire_on_end(void);
void sno_setexit_resume(const char *which);
#ifdef __cplusplus
}
#endif
#endif
