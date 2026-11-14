#ifndef GC_HEAP_HB_AGGB_H
#define GC_HEAP_HB_AGGB_H
#include <stdint.h>
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
#define HB_ZCOL 200
#define HB_FILL 202
#define HB_ZBLK 204
#define HB_WSC 205
#define HB_AGGV 206
#define HB_AGGP 207
#define HB_AGGT 208
#define HB_AGGB 211
#define HB_ARR 212
#define HB_DINST 213
#define HB_DVEC 214
#define HB_WSB 215
#define HB_PLDB 216
#define HB_PLDBS 217
#define HB_PLDBR 218
#define HB_PLDBK 219
#define HB_PVEC 220
#define HB_DTP 221
#define HB_DTPRCP 222
#define HB_TRIE 223
#define HB_TRIEN 224
#define HB_NVAR 225
#define HB_FNCB 226
#define HB_DATB 227
#define HB_EVCH 228
#define HB_LBLT 229
#define HB_IOCH 230
#define HB_TRCE 231
#define HB_PROC 232
#define HB_PSEG 233
#define HB_RVEC 234
#define HB_KIND_NAME(t_) ((t_) == HB_ZCOL ? "HB_ZCOL" : (t_) == HB_FILL ? "HB_FILL" : (t_) == HB_ZBLK ? "HB_ZBLK" : (t_) == HB_WSC ? "HB_WSC" : (t_) == HB_AGGV ? "HB_AGGV" : (t_) == HB_AGGP ? \
    "HB_AGGP" : (t_) == HB_AGGT ? "HB_AGGT" : (t_) == HB_AGGB ? "HB_AGGB" : (t_) == HB_ARR ? "HB_ARR" : (t_) == HB_DINST ? "HB_DINST" : (t_) == HB_DVEC ? "HB_DVEC" : (t_) == HB_WSB ? "HB_WSB" : (t_) \
    == HB_PLDB ? "HB_PLDB" : (t_) == HB_PLDBS ? "HB_PLDBS" : (t_) == HB_PLDBR ? "HB_PLDBR" : (t_) == HB_PLDBK ? "HB_PLDBK" : (t_) == HB_PVEC ? "HB_PVEC" : (t_) == HB_DTP ? "HB_DTP" : (t_) == \
    HB_DTPRCP ? "HB_DTPRCP" : (t_) == HB_TRIE ? "HB_TRIE" : (t_) == HB_TRIEN ? "HB_TRIEN" : (t_) == HB_NVAR ? "HB_NVAR" : (t_) == HB_FNCB ? "HB_FNCB" : (t_) == HB_DATB ? "HB_DATB" : (t_) == HB_EVCH \
    ? "HB_EVCH" : (t_) == HB_LBLT ? "HB_LBLT" : (t_) == HB_IOCH ? "HB_IOCH" : (t_) == HB_TRCE ? "HB_TRCE" : (t_) == HB_PROC ? "HB_PROC" : (t_) == HB_PSEG ? "HB_PSEG" : (t_) == HB_RVEC ? "HB_RVEC" : \
    "HB_?")
#define HB_IS_AGG(t_) (((t_) >= HB_AGGV && (t_) <= HB_AGGT) || (t_) == HB_AGGB)
#ifdef __cplusplus
}
#endif
#endif
