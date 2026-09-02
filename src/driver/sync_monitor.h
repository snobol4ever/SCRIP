/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef SYNC_MONITOR_H
#define SYNC_MONITOR_H
#include "core.h"
#include "runtime/builtins/gen_runtime.h"
typedef struct {
    NvPair  *nv_pairs;
    int      nv_count;
    int64_t  kw_stcount;
    int64_t  kw_stlimit;
    int64_t  kw_anchor;
    int      frame_depth;
    int      resolve_trail_mark;
    int      last_ok;
    const char **label_path;
    int          label_path_n;
    int          label_path_cap;
    NvPair  *frame_locals;
    int      frame_locals_count;
    struct PlLocalPair {
        char *name;
        char *val_str;
    } *resolve_locals;
    int      resolve_locals_count;
} ExecSnapshot;
void exec_snapshot_take(ExecSnapshot *s);
void exec_snapshot_restore(const ExecSnapshot *s);
void exec_snapshot_free(ExecSnapshot *s);
int sync_monitor_run(const tree_t *prog, int verbose, const char *core_path);
#endif
