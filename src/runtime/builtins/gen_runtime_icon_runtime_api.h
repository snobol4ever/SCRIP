#ifndef GEN_RUNTIME_ICON_RUNTIME_API_H
#define GEN_RUNTIME_ICON_RUNTIME_API_H
#ifdef __cplusplus
extern "C" {
#endif
const char *cset_union(const char *a, int alen, int ablk, const char *b, int blen, int bblk, int *outlen);
const char *cset_diff(const char *a, int alen, int ablk, const char *b, int blen, int bblk, int *outlen);
const char *cset_inter(const char *a, int alen, int ablk, const char *b, int blen, int bblk, int *outlen);
const char *cset_canonical(const char *cs, int len);
#ifdef __cplusplus
}
#endif
#endif
