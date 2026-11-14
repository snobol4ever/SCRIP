#ifndef DESCR_TAGS_DESCR_SASSERT_H
#define DESCR_TAGS_DESCR_SASSERT_H
#ifdef __cplusplus
#define DESCR_SASSERT(c, m) static_assert(c, m)
#else
#define DESCR_SASSERT(c, m) _Static_assert(c, m)
#endif
#endif
