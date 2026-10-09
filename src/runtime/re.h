/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef NFA_RE_H
#define NFA_RE_H
typedef struct { int matched; int full_start; int full_end; int ngroups; int ncaplog; int nev; char *blk; } Match;
#define MATCH_GRP(m, g) (((int *)(m)->blk) + 4 * (g))
#define MATCH_LOG(m, k) (((int *)(m)->blk) + 4 * (m)->ngroups + 3 * (k))
#define match_group_start(m, g) (MATCH_GRP(m, g)[0])
#define match_group_end(m, g) (MATCH_GRP(m, g)[1])
#define match_group_repeatable(m, g) (MATCH_GRP(m, g)[2])
#define match_group_name(m, g) ((const char *)(m)->blk + MATCH_GRP(m, g)[3])
#define match_caplog_group(m, k) (MATCH_LOG(m, k)[0])
#define match_caplog_start(m, k) (MATCH_LOG(m, k)[1])
#define match_caplog_end(m, k) (MATCH_LOG(m, k)[2])
#endif
