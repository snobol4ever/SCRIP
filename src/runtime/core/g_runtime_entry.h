#ifndef G_RUNTIME_ENTRY_H
#define G_RUNTIME_ENTRY_H
#ifdef __ASSEMBLER__
#define RT_ENTRY_DECL(sym, conv, transfers)
#else
enum { RT_ENTRY_CONV_leaf = 1, RT_ENTRY_CONV_gen = 2, RT_ENTRY_CONV_open = 3 };
#ifdef __cplusplus
#define RT_ENTRY_DECL(sym, conv, transfers) static_assert(RT_ENTRY_CONV_##conv >= 1 && (transfers) >= 0 && (transfers) <= 1, "RT_ENTRY_DECL " #sym)
#else
#define RT_ENTRY_DECL(sym, conv, transfers) _Static_assert(RT_ENTRY_CONV_##conv >= 1 && (transfers) >= 0 && (transfers) <= 1, "RT_ENTRY_DECL " #sym)
#endif
#endif
#endif
