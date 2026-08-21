#include "core.h"
#include "../rtx/rtcc.h"
#include "../rt/rt_arena.h"
#include "sil_macros.h"
#include "utf8.h"
#include "../../parser/snobol4/scrip_cc.h"
#include "../rt/rt_protected.h"
extern int g_protected_pat_vars_armed;
int g_call_fastpath_off = 0;
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <stdarg.h>
#include <ctype.h>
#include <fcntl.h>
#include <inttypes.h>
#include <unistd.h>
#include <sys/resource.h>
#include <time.h>
#define MON_RS "\x1e"
#define MON_US "\x1f"
#include <sys/uio.h>
#include "../../../scripts/monitor/monitor_wire.h"
int monitor_fd  = -1;
int monitor_ack_fd = -1;
int monitor_ready = 0;
int monitor_quiet_depth = 0;
int g_monitor_bin = 0;
int g_mon_max_stno = 0;
static void  mon_at_exit(void);
static uint32_t intern_name_bin(const char *p, int len);
static void  mon_send_bin(uint32_t kind, uint32_t name_id, uint8_t type,
                          const void *value, uint32_t value_len);
#define TRACE_SET_CAP 256
static const char *trace_set[TRACE_SET_CAP];
static const char *trace_callback[TRACE_SET_CAP];
static int trace_set_n = 0;
static int g_comm_dbg = -1;
static int trace_recursion_depth = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int trace_slot_lookup(const char *name) {
    if (!name || !*name) return -1;
    unsigned h = 5381;
    for (const char *p = name; *p; p++) h = h * 33 ^ (unsigned char)*p;
    for (int i = 0; i < TRACE_SET_CAP; i++) {
        int slot = (h + i) & (TRACE_SET_CAP - 1);
        if (!trace_set[slot]) return -1;
        if (strcmp(trace_set[slot], name) == 0) return slot;
    }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void trace_register(const char *name) {
    if (!name || !*name) return;
    unsigned h = 5381;
    for (const char *p = name; *p; p++) h = h * 33 ^ (unsigned char)*p;
    for (int i = 0; i < TRACE_SET_CAP; i++) {
        int slot = (h + i) & (TRACE_SET_CAP - 1);
        if (!trace_set[slot]) {
            trace_set[slot] = rt_ws_strdup(name);
            trace_callback[slot] = NULL;
            trace_set_n++;
            return;
        }
        if (strcmp(trace_set[slot], name) == 0) return;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void trace_register_callback(const char *name, const char *cbfn) {
    if (!name || !*name) return;
    unsigned h = 5381;
    for (const char *p = name; *p; p++) h = h * 33 ^ (unsigned char)*p;
    for (int i = 0; i < TRACE_SET_CAP; i++) {
        int slot = (h + i) & (TRACE_SET_CAP - 1);
        if (!trace_set[slot]) {
            trace_set[slot] = rt_ws_strdup(name);
            trace_callback[slot] = (cbfn && *cbfn) ? rt_ws_strdup(cbfn) : NULL;
            trace_set_n++;
            return;
        }
        if (strcmp(trace_set[slot], name) == 0) {
            trace_callback[slot] = (cbfn && *cbfn) ? rt_ws_strdup(cbfn) : NULL;
            return;
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void trace_unregister(const char *name) {
    if (!name || !*name) return;
    unsigned h = 5381;
    for (const char *p = name; *p; p++) h = h * 33 ^ (unsigned char)*p;
    for (int i = 0; i < TRACE_SET_CAP; i++) {
        int slot = (h + i) & (TRACE_SET_CAP - 1);
        if (!trace_set[slot]) return;
        if (strcmp(trace_set[slot], name) == 0) {
            trace_set[slot] = NULL;
            trace_callback[slot] = NULL;
            if (trace_set_n > 0) trace_set_n--;
            return;
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int trace_registered(const char *name) {
    return trace_slot_lookup(name) >= 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *trace_get_callback(const char *name) {
    int slot = trace_slot_lookup(name);
    if (slot < 0) return NULL;
    return trace_callback[slot];
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int trace_is_active(const char *name) { return trace_registered(name); }
int64_t kw_stcount = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void mon_send(const char *kind, const char *name, const char *value) {
    if (monitor_fd < 0) return;
    if (!value) value = "";
    struct iovec iov[6];
    iov[0].iov_base = (void*)kind;   iov[0].iov_len = strlen(kind);
    iov[1].iov_base = MON_RS;        iov[1].iov_len = 1;
    iov[2].iov_base = (void*)name;   iov[2].iov_len = strlen(name);
    iov[3].iov_base = MON_US;        iov[3].iov_len = 1;
    iov[4].iov_base = (void*)value;  iov[4].iov_len = strlen(value);
    iov[5].iov_base = MON_RS;        iov[5].iov_len = 1;
    writev(monitor_fd, iov, 6);
    if (monitor_ack_fd >= 0) {
        char ack[1];
        ssize_t r = read(monitor_ack_fd, ack, 1);
        if (r != 1 || ack[0] == 'S') exit(0);
    }
}
static char  **g_bin_names      = NULL;
static int    *g_bin_name_lens  = NULL;
static int     g_bin_n_names    = 0;
static int     g_bin_names_cap  = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int load_names_file_bin(const char *path) {
    FILE *f = fopen(path, "r");
    if (!f) return -1;
    int   cap = 64;
    char **names = (char **)rt_ws_alloc(cap * sizeof(char *));
    int   *lens  = (int  *)rt_ws_alloc(cap * sizeof(int));
    if (!names || !lens) { fclose(f); return -1; }
    int n = 0;
    char *line = NULL; size_t lcap = 0;
    ssize_t got;
    while ((got = getline(&line, &lcap, f)) >= 0) {
        if (got > 0 && line[got-1] == '\n') { line[got-1] = '\0'; got--; }
        if (got > 0 && line[got-1] == '\r') { line[got-1] = '\0'; got--; }
        if (n == cap) {
            cap *= 2;
            names = (char **)rt_ws_realloc(names, cap * sizeof(char *));
            lens  = (int  *)rt_ws_realloc(lens,  cap * sizeof(int));
            if (!names || !lens) { fclose(f); free(line); return -1; }
        }
        char *copy = (char *)rt_ws_alloc((size_t)got + 1);
        if (!copy) { fclose(f); free(line); return -1; }
        memcpy(copy, line, (size_t)got + 1);
        names[n] = copy;
        lens[n]  = (int)got;
        n++;
    }
    free(line);
    fclose(f);
    g_bin_names     = names;
    g_bin_name_lens = lens;
    g_bin_n_names   = n;
    g_bin_names_cap = cap;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static uint32_t lookup_name_id_bin(const char *p, int len) {
    if (!g_bin_names || !p) return MW_NAME_ID_NONE;
    for (int i = 0; i < g_bin_n_names; i++) {
        if (g_bin_name_lens[i] == len && memcmp(g_bin_names[i], p, (size_t)len) == 0)
            return (uint32_t)i;
    }
    return MW_NAME_ID_NONE;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static uint32_t intern_name_bin(const char *p, int len) {
    if (!p || len < 0) return MW_NAME_ID_NONE;
    for (int i = 0; i < g_bin_n_names; i++) {
        if (g_bin_name_lens[i] == len && memcmp(g_bin_names[i], p, (size_t)len) == 0)
            return (uint32_t)i;
    }
    if (g_bin_n_names == g_bin_names_cap) {
        int new_cap = g_bin_names_cap ? g_bin_names_cap * 2 : 64;
        char **nn = (char **)rt_ws_realloc(g_bin_names, (size_t)new_cap * sizeof(char *));
        int   *nl = (int  *)rt_ws_realloc(g_bin_name_lens, (size_t)new_cap * sizeof(int));
        if (!nn || !nl) {
            if (nn) g_bin_names = nn;
            if (nl) g_bin_name_lens = nl;
            return MW_NAME_ID_NONE;
        }
        g_bin_names     = nn;
        g_bin_name_lens = nl;
        g_bin_names_cap = new_cap;
    }
    char *copy = (char *)rt_ws_alloc((size_t)len + 1);
    if (!copy) return MW_NAME_ID_NONE;
    if (len > 0) memcpy(copy, p, (size_t)len);
    copy[len] = '\0';
    int id = g_bin_n_names;
    g_bin_names[id]     = copy;
    g_bin_name_lens[id] = len;
    g_bin_n_names       = id + 1;
    if (g_monitor_bin && monitor_fd >= 0) {
        mon_send_bin(MWK_NAME_DEF, (uint32_t)id, MWT_STRING,
                     (len > 0) ? (const void *)copy : NULL, (uint32_t)len);
    }
    return (uint32_t)id;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void mon_at_exit(void) {
    static int already = 0;
    if (already) return;
    already = 1;
    if (monitor_fd >= 0 && g_monitor_bin) {
        if (g_mon_max_stno > 0 && monitor_ready) {
            int64_t es = (int64_t)g_mon_max_stno + 1; unsigned char lhdr[MW_HDR_BYTES]; unsigned char lbuf[8];
            for (int k = 0; k < 8; k++) lbuf[k] = (unsigned char)(((uint64_t)es >> (k*8)) & 0xff);
            mw_pack_hdr(lhdr, MWK_LABEL, MW_NAME_ID_NONE, MWT_INTEGER, 8);
            struct iovec iov[2]; iov[0].iov_base = lhdr; iov[0].iov_len = MW_HDR_BYTES; iov[1].iov_base = lbuf; iov[1].iov_len = 8;
            ssize_t lw = writev(monitor_fd, iov, 2); (void)lw;
            if (monitor_ack_fd >= 0) { char ack[1]; ssize_t r = read(monitor_ack_fd, ack, 1); (void)r; }
        }
        unsigned char hdr[MW_HDR_BYTES];
        mw_pack_hdr(hdr, MWK_END, MW_NAME_ID_NONE, MWT_NULL, 0);
        ssize_t w = write(monitor_fd, hdr, MW_HDR_BYTES);
        (void)w;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static uint8_t scrip_tag_to_wire(int v) {
    switch (v) {
        case DT_SNUL:  return MWT_STRING;
        case DT_S:     return MWT_STRING;
        case DT_I:     return MWT_INTEGER;
        case DT_R:     return MWT_REAL;
        case DT_P:     return MWT_PATTERN;
        case DT_A:     return MWT_ARRAY;
        case DT_T:     return MWT_TABLE;
        case DT_C:     return MWT_CODE;
        case DT_E:     return MWT_EXPRESSION;
        case DT_DATA:  return MWT_DATA;
        default:       return MWT_UNKNOWN;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void mon_send_bin(uint32_t kind, uint32_t name_id, uint8_t type,
                         const void *value, uint32_t value_len) {
    if (monitor_fd < 0) return;
    unsigned char hdr[MW_HDR_BYTES];
    mw_pack_hdr(hdr, kind, name_id, type, value_len);
    struct iovec iov[2];
    int niov = 1;
    iov[0].iov_base = hdr;
    iov[0].iov_len  = MW_HDR_BYTES;
    if (value_len > 0 && value) {
        iov[1].iov_base = (void *)value;
        iov[1].iov_len  = (size_t)value_len;
        niov = 2;
    }
    ssize_t total = (ssize_t)MW_HDR_BYTES + (ssize_t)value_len;
    ssize_t got   = writev(monitor_fd, iov, niov);
    if (got != total) return;
    if (monitor_ack_fd >= 0) {
        char ack[1];
        ssize_t r = read(monitor_ack_fd, ack, 1);
        if (r != 1 || ack[0] == 'S') exit(0);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void mon_emit_label_bin(int64_t stno) {
    kw_stcount = stno;   /* s189 (HQ Fable): the dead counter becomes the CURRENT-STNO latch -- nothing incremented it anywhere and nothing reads it back into the language, so this is instrument state riding an existing cell; the ZSM ring stamps it per port so a crash window names its statements without a monitor run */
    if (monitor_fd < 0 || !g_monitor_bin) return;
    unsigned char buf[8];
    for (int k = 0; k < 8; k++) buf[k] = (unsigned char)(((uint64_t)stno >> (k*8)) & 0xff);
    mon_send_bin(MWK_LABEL, MW_NAME_ID_NONE, MWT_INTEGER, buf, 8);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ⭐ s112 MON-CAP — IS THIS NAME A COMPILER-MINTED INTERNAL?  ONE AUTHORITY for both VALUE fire-points (mon_emit_value_bin and comm_var's binary arm); they are separate code paths and this seat
 * measured the cost of fixing only one — the filter went into mon_emit_value_bin, the rebuild was clean, and beauty.sno's divergence did not move ONE STEP, because the events came out of the other path.
 * Every name the SNOBOL4 lowerer mints for itself carries an embedded '$' after an identifier head: PAT$n, PAT$n$A<i>, PAT$n$V<i>, PATV$k, PATTMP$n, EXPR$n, IGT$n, SNO$*.  The oracle has no such
 * variables, so each emitted a VALUE event with no counterpart and desynced the trace (beauty step 951: `spl @663 VALUE DQ` vs `scr @663 VALUE PAT$9$A0`).
 * ⛔ THE HEAD TEST IS LOAD-BEARING, AND A NAIVE strchr(name,'$') IS WRONG — THIS SEAT SHIPPED THAT VERSION AND BEAUTY FALSIFIED IT IN ONE RUN.  `$` is SPITBOL's indirect-reference OPERATOR, so a name
 * cannot be WRITTEN with one — but indirect assignment CREATES variables named by arbitrary strings, and beauty.sno:104 is literally `$'$' = *White '$' *White`, i.e. a user variable whose whole name is
 * "$".  Suppressing on '$' anywhere killed that legitimate event and moved the divergence to step 1007 instead of past it.  Requiring an identifier HEAD ([A-Za-z_] then [A-Za-z0-9_]* then '$') keeps
 * every minted form and spares punctuation-named user variables ($, !, **) — the exact shapes beauty builds its operator table from.
 * RESIDUAL RISK, STATED RATHER THAN PAPERED OVER: a user CAN still collide via `$('A$B')`, an identifier-headed name containing '$'.  Nothing in the corpus does, and no name written in source can
 * reach this shape, but this filter is a HEURISTIC over a namespace the lowerer shares with the user — not a proof.  The durable fix is a mint-time marker on compiler names; until then, keep it here. */
static int mon_name_is_internal(const char *name)
{
    if (!name || !name[0] || name[0] == '<') return 0;
    if (!((name[0] >= 'A' && name[0] <= 'Z') || (name[0] >= 'a' && name[0] <= 'z') || name[0] == '_')) return 0;
    { const char *q = name + 1; while (*q && ((*q >= 'A' && *q <= 'Z') || (*q >= 'a' && *q <= 'z') || (*q >= '0' && *q <= '9') || *q == '_')) q++; return *q == '$'; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void mon_emit_value_bin(const char *name, DESCR_t val) {
    if (monitor_fd < 0 || !g_monitor_bin || !monitor_ready) return;
    if (!name || !name[0] || name[0] == '_' || name[0] == '&' || NV_PTR_fn(name) == NULL) return;
    if (mon_name_is_internal(name)) return;
    uint32_t name_id = intern_name_bin(name, (int)strlen(name));
    if (name_id == MW_NAME_ID_NONE) return;
    uint8_t type = scrip_tag_to_wire(val.v);
    const void *vp = NULL; uint32_t vlen = 0;
    int64_t i_buf; double r_buf;
    switch (type) {
        case MWT_STRING: case MWT_NAME:
            if (val.s) { vlen = val.slen ? val.slen : (uint32_t)strlen(val.s); vp = vlen ? (const void *)val.s : NULL; } break;
        case MWT_INTEGER: { int64_t iv = val.i; unsigned char *p = (unsigned char *)&i_buf;
            for (int k = 0; k < 8; k++) p[k] = (unsigned char)((iv >> (k*8)) & 0xff); vp = &i_buf; vlen = 8; break; }
        case MWT_REAL: { memcpy(&r_buf, &val.r, sizeof(r_buf)); vp = &r_buf; vlen = 8; break; }
        default: break;
    }
    mon_send_bin(MWK_VALUE, name_id, type, vp, vlen);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void _arg_str(DESCR_t a, const char **out_p, int *out_len) {
    if (a.v == DT_S && a.s) {
        *out_p   = a.s;
        *out_len = a.slen ? (int)a.slen : (int)strlen(a.s);
    } else {
        *out_p = NULL; *out_len = 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _mon_put_helper(DESCR_t *args, int nargs, uint32_t kind, int opaque) {
    if (nargs < 2) return FAILDESCR;
    const char *np; int nlen;
    _arg_str(args[0], &np, &nlen);
    if (!np) return FAILDESCR;
    uint32_t name_id = lookup_name_id_bin(np, nlen);
    if (name_id == MW_NAME_ID_NONE) return FAILDESCR;
    uint8_t type;
    const void *vp = NULL;
    uint32_t    vlen = 0;
    int64_t i_buf;
    double  r_buf;
    if (opaque) {
        const char *tp; int tlen;
        _arg_str(args[1], &tp, &tlen);
        type = MWT_DATA;
        if (tp) {
            if      (tlen == 7  && memcmp(tp, "PATTERN", 7) == 0) type = MWT_PATTERN;
            else if (tlen == 5  && memcmp(tp, "ARRAY",   5) == 0) type = MWT_ARRAY;
            else if (tlen == 5  && memcmp(tp, "TABLE",   5) == 0) type = MWT_TABLE;
            else if (tlen == 4  && memcmp(tp, "CODE",    4) == 0) type = MWT_CODE;
            else if (tlen == 4  && memcmp(tp, "FILE",    4) == 0) type = MWT_FILE;
            else if (tlen == 10 && memcmp(tp, "EXPRESSION", 10) == 0) type = MWT_EXPRESSION;
            else if (tlen == 4  && memcmp(tp, "NAME",    4) == 0) type = MWT_NAME;
            else if (tlen == 0)                                    type = MWT_NULL;
        }
    } else {
        type = scrip_tag_to_wire(args[1].v);
        switch (type) {
            case MWT_STRING:
            case MWT_NAME:
                if (args[1].s) {
                    vlen = args[1].slen ? (uint32_t)args[1].slen : (uint32_t)strlen(args[1].s);
                    vp   = (vlen > 0) ? (const void *)args[1].s : NULL;
                }
                break;
            case MWT_INTEGER: {
                int64_t iv = args[1].i;
                unsigned char *p = (unsigned char *)&i_buf;
                for (int k = 0; k < 8; k++) p[k] = (unsigned char)((iv >> (k*8)) & 0xff);
                vp = &i_buf; vlen = 8;
                break;
            }
            case MWT_REAL: {
                double rv = args[1].r;
                memcpy(&r_buf, &rv, sizeof(r_buf));
                vp = &r_buf; vlen = 8;
                break;
            }
            default: break;
        }
    }
    mon_send_bin(kind, name_id, type, vp, vlen);
    return (DESCR_t){ .v = DT_I, .i = 0 };
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _b_MON_OPEN(DESCR_t *args, int nargs) {
    if (nargs < 3) return FAILDESCR;
    if (monitor_fd < 0) return FAILDESCR;
    if (g_bin_names == NULL) {
        const char *np; int nl;
        _arg_str(args[2], &np, &nl);
        if (np && nl > 0) {
            char buf[4096];
            int  cp = nl < (int)sizeof(buf)-1 ? nl : (int)sizeof(buf)-1;
            memcpy(buf, np, cp); buf[cp] = '\0';
            if (load_names_file_bin(buf) < 0) return FAILDESCR;
        }
    }
    return (DESCR_t){ .v = DT_I, .i = 0 };
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _b_MON_PUT_S_VALUE (DESCR_t *a, int n) { return _mon_put_helper(a, n, MWK_VALUE,  0); }
static DESCR_t _b_MON_PUT_I_VALUE (DESCR_t *a, int n) { return _mon_put_helper(a, n, MWK_VALUE,  0); }
static DESCR_t _b_MON_PUT_R_VALUE (DESCR_t *a, int n) { return _mon_put_helper(a, n, MWK_VALUE,  0); }
static DESCR_t _b_MON_PUT_O_VALUE (DESCR_t *a, int n) { return _mon_put_helper(a, n, MWK_VALUE,  1); }
static DESCR_t _b_MON_PUT_S_RETURN(DESCR_t *a, int n) { return _mon_put_helper(a, n, MWK_RETURN, 0); }
static DESCR_t _b_MON_PUT_I_RETURN(DESCR_t *a, int n) { return _mon_put_helper(a, n, MWK_RETURN, 0); }
static DESCR_t _b_MON_PUT_R_RETURN(DESCR_t *a, int n) { return _mon_put_helper(a, n, MWK_RETURN, 0); }
static DESCR_t _b_MON_PUT_O_RETURN(DESCR_t *a, int n) { return _mon_put_helper(a, n, MWK_RETURN, 1); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _b_MON_PUT_CALL(DESCR_t *args, int nargs) {
    if (nargs < 1) return FAILDESCR;
    const char *np; int nlen;
    _arg_str(args[0], &np, &nlen);
    if (!np) return FAILDESCR;
    uint32_t name_id = lookup_name_id_bin(np, nlen);
    if (name_id == MW_NAME_ID_NONE) return FAILDESCR;
    mon_send_bin(MWK_CALL, name_id, MWT_NULL, NULL, 0);
    return (DESCR_t){ .v = DT_I, .i = 0 };
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _b_MON_CLOSE(DESCR_t *args, int nargs) {
    (void)args; (void)nargs;
    if (monitor_fd >= 0) {
        mon_send_bin(MWK_END, MW_NAME_ID_NONE, MWT_NULL, NULL, 0);
    }
    return (DESCR_t){ .v = DT_I, .i = 0 };
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _b_LOAD_stub(DESCR_t *args, int nargs) {
    if (nargs < 1) return FAILDESCR;
    const char *p; int len;
    _arg_str(args[0], &p, &len);
    if (!p || len < 4) return FAILDESCR;
    if (memcmp(p, "MON_", 4) == 0) {
        return (DESCR_t){ .v = DT_I, .i = 0 };
    }
    return FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int mon_synth_name(const char *n) { static int keep = -1; if (keep < 0) { const char *e = getenv("SCRIP_MON_SYNTH"); keep = (e && *e == '1') ? 1 : 0; } return keep ? 0 : mon_name_is_internal(n); }   /* ⭐ s189 (HQ Fable, autobug unblock; delegates to mon_name_is_internal, the s112 MON-CAP ONE AUTHORITY -- identifier-head test, so the user variable named "$" survives): compiler-SYNTHETIC frames (EXPR$n defer thunks, PAT$n blobs) never cross the trace/monitor surface -- the oracle structurally cannot emit them ($ is not a name character in source), so every such CALL/RETURN/VALUE event is a GUARANTEED false divergence that kills the 2-way sync-step at the first deferred evaluation and blinds Lon's automatic bug finder exactly where beauty's bug lives (step 1568: spl CALL PushCounter vs scr CALL EXPR$173, the *Command thunk).  Filtering here also moves &FTRACE toward oracle behavior (SPITBOL user tracing has no synthetic frames).  SCRIP_MON_SYNTH=1 restores the old stream for A/B.  NAMESPACE-LAW-consistent (ARCH-PASSTHRU: synthetic names are pollution and die); function-local static cache per the tree's killswitch idiom, ZERO new file-scope state. */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void comm_var(const char *name, DESCR_t val) {
    if (!name || name[0] == '_') return;
    if (mon_synth_name(name)) return;
    if (monitor_quiet_depth > 0) return;
    if (g_comm_dbg < 0) g_comm_dbg = getenv("SCRIP_DEBUG_TRACE") ? 1 : 0;
    const int dbg = g_comm_dbg;
    if (!dbg && trace_set_n == 0 && monitor_fd < 0) return;
    const char *cbfn = trace_get_callback(name);
    if (dbg)
        fprintf(stderr, "[scrip-trace] comm_var name=%s cb=%s recur=%d\n",
                name, cbfn ? cbfn : "(none)", trace_recursion_depth);
    if (cbfn && trace_recursion_depth == 0) {
        trace_recursion_depth++;
        DESCR_t cbargs[2];
        cbargs[0] = STRVAL(rt_ws_strdup_c(name));
        cbargs[1] = STRVAL("");
        (void)APPLY_fn(cbfn, cbargs, 2);
        trace_recursion_depth--;
        return;
    }
    if (monitor_fd < 0) return;
    if (!monitor_ready) return;
    if (kw_trace <= 0 && !trace_registered(name)) return;
    if (g_monitor_bin && mon_name_is_internal(name)) return;   /* s112 MON-CAP: same suppression as mon_emit_value_bin — comm_var is the SECOND VALUE fire-point (the &TRACE path) and is the one beauty.sno's step-951 events were actually coming out of.  Scoped to the monitor regime so ordinary &TRACE output is byte-unchanged. */
    if (g_monitor_bin) {
        uint32_t name_id = intern_name_bin(name, (int)strlen(name));
        if (name_id == MW_NAME_ID_NONE) return;
        uint8_t type = scrip_tag_to_wire(val.v);
        const void *vp = NULL;
        uint32_t    vlen = 0;
        int64_t i_buf;
        double  r_buf;
        switch (type) {
            case MWT_STRING:
            case MWT_NAME:
                if (val.s) {
                    vlen = val.slen ? (uint32_t)val.slen : (uint32_t)strlen(val.s);
                    vp   = (vlen > 0) ? (const void *)val.s : NULL;
                }
                break;
            case MWT_INTEGER: {
                int64_t iv = val.i;
                unsigned char *p = (unsigned char *)&i_buf;
                for (int k = 0; k < 8; k++) p[k] = (unsigned char)((iv >> (k*8)) & 0xff);
                vp = &i_buf; vlen = 8;
                break;
            }
            case MWT_REAL: {
                double rv = val.r;
                memcpy(&r_buf, &rv, sizeof(r_buf));
                vp = &r_buf; vlen = 8;
                break;
            }
            default: break;
        }
        mon_send_bin(MWK_VALUE, name_id, type, vp, vlen);
        return;
    }
    const char *s = VARVAL_fn(val);
    mon_send("VALUE", name, s ? s : "(undef)");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void comm_call(const char *fname) {
    if (!fname || !*fname) return;
    if (mon_synth_name(fname)) return;
    if (kw_ftrace > 0 && !g_monitor_bin) {
        fprintf(stdout, "****%-7lld  %s()\n",
                (long long)kw_stcount, fname);
        fflush(stdout);
    }
    if (monitor_fd < 0) return;
    if (!monitor_ready) return;
    if (kw_ftrace <= 0 && !trace_registered(fname)) return;
    if (g_monitor_bin) {
        uint32_t name_id = intern_name_bin(fname, (int)strlen(fname));
        if (name_id == MW_NAME_ID_NONE) return;
        mon_send_bin(MWK_CALL, name_id, MWT_NULL, NULL, 0);
        return;
    }
    mon_send("CALL", fname, "");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void comm_return(const char *fname, DESCR_t retval) {
    if (!fname || !*fname) return;
    if (mon_synth_name(fname)) return;
    if (kw_ftrace > 0 && !g_monitor_bin) {
        const char *s = VARVAL_fn(retval);
        fprintf(stdout, "****%-7lld  RETURN %s = '%s'\n",
                (long long)kw_stcount, fname, s ? s : "");
        fflush(stdout);
    }
    if (monitor_fd < 0) return;
    if (!monitor_ready) return;
    if (kw_ftrace <= 0 && !trace_registered(fname)) return;
    if (g_monitor_bin) {
        uint32_t name_id = intern_name_bin(fname, (int)strlen(fname));
        if (name_id == MW_NAME_ID_NONE) return;
        const char *rt = kw_rtntype[0] ? kw_rtntype : "RETURN";
        uint32_t rtlen = (uint32_t)strlen(rt);
        mon_send_bin(MWK_RETURN, name_id, MWT_STRING,
                     rtlen ? (const void *)rt : NULL, rtlen);
        (void)retval;
        return;
    }
    const char *s = VARVAL_fn(retval);
    mon_send("RETURN", fname, s ? s : "(fail)");
}
int64_t kw_fullscan = 0;
int64_t kw_maxlngth = 524288;
int64_t kw_anchor   = 0;
int64_t kw_trim     = 0;
int64_t kw_stlimit  = -1;
int64_t kw_ftrace   = 0;
int64_t kw_trace    = 0;
int64_t kw_errlimit = 0;
int64_t kw_code     = 0;
int64_t kw_fnclevel = 0;
char    kw_rtntype[16] = "";
char ucase[27]    = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
char lcase[27]    = "abcdefghijklmnopqrstuvwxyz";
char digits[11]   = "0123456789";
char alphabet[257];
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int is_numeric_like(DESCR_t d) {
    if (IS_INT(d) || IS_REAL(d) || IS_NULL(d)) return 1;
    if (IS_STR(d)) {
        const char *s = d.s ? d.s : "";
        while (*s == ' ' || *s == '\t') s++;
        if (!*s) return 1;
        char *end;
        strtod(s, &end);
        if (end == s) return 0;
        while (*end == ' ' || *end == '\t') end++;
        return (*end == '\0');
    }
    if (IS_KW(d)) return is_numeric_like(NV_GET_fn(d.s));
    return 0;
}
#define NUM_GUARD(fn)                                                      \
    do {                                                                   \
        if (!is_numeric_like(a[0])) {                                      \
            core_runtime_error(1, fn " first argument is not numeric");    \
            return FAILDESCR;                                              \
        }                                                                  \
        if (!is_numeric_like(a[1])) {                                      \
            core_runtime_error(1, fn " second argument is not numeric");   \
            return FAILDESCR;                                              \
        }                                                                  \
    } while (0)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _GT_(DESCR_t *a, int n) {
    if (n < 2) return FAILDESCR;
    NUM_GUARD("GT");
    return gt(a[0], a[1]) ? NULVCL : FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _LT_(DESCR_t *a, int n) {
    if (n < 2) return FAILDESCR;
    NUM_GUARD("LT");
    return lt(a[0], a[1]) ? NULVCL : FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _GE_(DESCR_t *a, int n) {
    if (n < 2) return FAILDESCR;
    NUM_GUARD("GE");
    return ge(a[0], a[1]) ? NULVCL : FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _LE_(DESCR_t *a, int n) {
    if (n < 2) return FAILDESCR;
    NUM_GUARD("LE");
    return le(a[0], a[1]) ? NULVCL : FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _EQ_(DESCR_t *a, int n) {
    if (n < 2) return FAILDESCR;
    NUM_GUARD("EQ");
    if (IS_INT(a[0]) && IS_INT(a[1]))
        return (a[0].i == a[1].i) ? NULVCL : FAILDESCR;
    return (to_real(a[0]) == to_real(a[1])) ? NULVCL : FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _NE_(DESCR_t *a, int n) {
    if (n < 2) return FAILDESCR;
    NUM_GUARD("NE");
    if (IS_INT(a[0]) && IS_INT(a[1]))
        return (a[0].i != a[1].i) ? NULVCL : FAILDESCR;
    return (to_real(a[0]) != to_real(a[1])) ? NULVCL : FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _b_add(DESCR_t *a, int n) {
    if (n < 2) return FAILDESCR;
    return add(a[0], a[1]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _b_sub(DESCR_t *a, int n) {
    if (n < 2) return FAILDESCR;
    return sub(a[0], a[1]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _b_mul(DESCR_t *a, int n) {
    if (n < 2) return FAILDESCR;
    return mul(a[0], a[1]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _b_div(DESCR_t *a, int n) {
    if (n < 2) return FAILDESCR;
    return DIVIDE_fn(a[0], a[1]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _b_pow(DESCR_t *a, int n) {
    if (n < 2) return FAILDESCR;
    return POWER_fn(a[0], a[1]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _b_neg(DESCR_t *a, int n) {
    if (n < 1) return FAILDESCR;
    return neg(a[0]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _b_pos(DESCR_t *a, int n) {
    if (n < 1) return FAILDESCR;
    return pos(a[0]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _INTEGER_(DESCR_t *a, int n) {
    if (n < 1) return FAILDESCR;
    if (IS_INT(a[0])) return a[0];
    if (IS_STR(a[0]) && a[0].s) {
        char *end;
        long long v = strtoll(a[0].s, &end, 10);
        if (end != a[0].s && *end == '\0') return INTVAL(v);
    }
    return FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _REAL_(DESCR_t *a, int n) {
    if (n < 1) return FAILDESCR;
    if (IS_REAL(a[0])) return a[0];
    if (IS_INT(a[0]))  return REALVAL((double)a[0].i);
    if (IS_STR(a[0]) && a[0].s) {
        char *end;
        double v = strtod(a[0].s, &end);
        if (end != a[0].s && *end == '\0') return REALVAL(v);
    }
    return FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _SIZE_(DESCR_t *a, int n) {
    if (n < 1) return INTVAL(0);
    if (IS_STR(a[0]) && a[0].slen) return INTVAL((int64_t)a[0].slen);
    const char *s = VARVAL_fn(a[0]);
    return INTVAL((int64_t)(s ? utf8_strlen(s) : 0));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _IDENT_(DESCR_t *a, int n) {
    DESCR_t x = (n > 0) ? a[0] : NULVCL;
    DESCR_t y = (n > 1) ? a[1] : NULVCL;
    return ident(x, y) ? NULVCL : FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _DIFFER_(DESCR_t *a, int n) {
    DESCR_t x = (n > 0) ? a[0] : NULVCL;
    DESCR_t y = (n > 1) ? a[1] : NULVCL;
    return differ(x, y) ? NULVCL : FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _VDIFFER_(DESCR_t *a, int n) {
    if (n < 2) return (n == 1) ? a[0] : FAILDESCR;
    DESCR_t x = a[0], y = a[1];
    if (x.v == DT_SNUL) { x.v = DT_S; x.s = ""; }
    if (y.v == DT_SNUL) { y.v = DT_S; y.s = ""; }
    int equal;
    if (x.v != y.v) {
        equal = 0;
    } else {
        switch (x.v) {
            case DT_I: equal = (x.i == y.i); break;
            case DT_R: equal = (x.r == y.r); break;
            case DT_S: {
                           const char *xs = x.s ? x.s : "";
                           const char *ys = y.s ? y.s : "";
                           equal = (strcmp(xs, ys) == 0); break; }
            default:   equal = (x.s == y.s); break;
        }
    }
    return equal ? FAILDESCR : a[0];
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _NUMERIC_(DESCR_t *a, int n) {
    if (n < 1) return FAILDESCR;
    DESCR_t val = a[0];
    if (IS_INT(val))  return val;
    if (IS_REAL(val)) return val;
    if (IS_STR(val) || val.v == DT_SNUL) {
        const char *s = val.s ? val.s : "";
        while (*s == ' ') s++;
        if (!*s) return INTVAL(0);
        char *end = NULL;
        long long iv = strtoll(s, &end, 10);
        while (*end == ' ') end++;
        if (*end == '\0') return INTVAL((int64_t)iv);
        double rv = strtod(s, &end);
        while (*end == ' ') end++;
        if (*end == '\0') return REALVAL(rv);
    }
    return FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _NAME_(DESCR_t *a, int n) {
    if (n < 1) return FAILDESCR;
    DESCR_t val = a[0];
    if (IS_NAME(val)) {
        const char *nm = val.s ? val.s : "";
        return STRVAL(rt_ws_strdup_c(nm));
    }
    const char *s = VARVAL_fn(val);
    return STRVAL(rt_ws_strdup_c(s ? s : ""));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _LGT_(DESCR_t *a, int n) {
    if (n < 2) return FAILDESCR;
    const char *x = VARVAL_fn(a[0]); const char *y = VARVAL_fn(a[1]);
    if (!x) x = ""; if (!y) y = "";
    return strcmp(x, y) > 0 ? NULVCL : FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _LLT_(DESCR_t *a, int n) {
    if (n < 2) return FAILDESCR;
    const char *x = VARVAL_fn(a[0]); const char *y = VARVAL_fn(a[1]);
    if (!x) x = ""; if (!y) y = "";
    return strcmp(x, y) < 0 ? NULVCL : FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _LGE_(DESCR_t *a, int n) {
    if (n < 2) return FAILDESCR;
    const char *x = VARVAL_fn(a[0]); const char *y = VARVAL_fn(a[1]);
    if (!x) x = ""; if (!y) y = "";
    return strcmp(x, y) >= 0 ? NULVCL : FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _LLE_(DESCR_t *a, int n) {
    if (n < 2) return FAILDESCR;
    const char *x = VARVAL_fn(a[0]); const char *y = VARVAL_fn(a[1]);
    if (!x) x = ""; if (!y) y = "";
    return strcmp(x, y) <= 0 ? NULVCL : FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _LEQ_(DESCR_t *a, int n) {
    if (n < 2) return FAILDESCR;
    const char *x = VARVAL_fn(a[0]); const char *y = VARVAL_fn(a[1]);
    if (!x) x = ""; if (!y) y = "";
    return strcmp(x, y) == 0 ? NULVCL : FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _LNE_(DESCR_t *a, int n) {
    if (n < 2) return FAILDESCR;
    const char *x = VARVAL_fn(a[0]); const char *y = VARVAL_fn(a[1]);
    if (!x) x = ""; if (!y) y = "";
    return strcmp(x, y) != 0 ? NULVCL : FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _HOST_(DESCR_t *a, int n) {
    if (n < 1) return NULVCL;
    int64_t selector = to_int(a[0]);
    if (selector == 0) return STRVAL(rt_ws_strdup_c(""));
    if (selector == 1) {
        char buf[32]; snprintf(buf, sizeof(buf), "%d", (int)getpid());
        return STRVAL(rt_ws_strdup_c(buf));
    }
    if (selector == 3) return INTVAL(0);
    if (selector == 4 && n >= 2) {
        const char *envname = VARVAL_fn(a[1]);
        if (!envname || !*envname) return NULVCL;
        const char *val = getenv(envname);
        if (!val) return NULVCL;
        return STRVAL(rt_ws_strdup_c(val));
    }
    return NULVCL;
}
#define IO_CHAN_MAX 32
typedef struct {
    FILE  *fp;
    char  *varname;
    int    is_output;
    char  *buf;
    size_t cap;
} io_chan_t;
static io_chan_t _io_chan[IO_CHAN_MAX];
static int _io_chan_init = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void _io_chan_setup(void) {
    if (_io_chan_init) return;
    memset(_io_chan, 0, sizeof(_io_chan));
    _io_chan_init = 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int _io_chan_find_by_var(const char *name) {
    _io_chan_setup();
    for (int i = 0; i < IO_CHAN_MAX; i++)
        if (_io_chan[i].varname && strcmp(_io_chan[i].varname, name) == 0) return i;
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void _io_chan_close(int ch) {
    _io_chan_setup();
    if (ch < 0 || ch >= IO_CHAN_MAX) return;
    if (_io_chan[ch].fp) { fclose(_io_chan[ch].fp); _io_chan[ch].fp = NULL; }
    if (_io_chan[ch].varname) { _io_chan[ch].varname = NULL; }
    if (_io_chan[ch].buf)  { free(_io_chan[ch].buf); _io_chan[ch].buf = NULL; }
    _io_chan[ch].cap = 0;
    _io_chan[ch].is_output = 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _ENDFILE_(DESCR_t *a, int n) {
    if (n < 1) return NULVCL;
    int ch = (int)a[0].i;
    if (ch >= 0 && ch < IO_CHAN_MAX) _io_chan_close(ch);
    return NULVCL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _APPLY_(DESCR_t *a, int n) {
    if (n < 1) return NULVCL;
    const char *fname = NULL;
    if (a[0].v == DT_N) {
        if (a[0].slen == 0 && a[0].s && *a[0].s)
            fname = a[0].s;
        else if (a[0].slen == 1 && a[0].ptr)
            fname = NV_name_from_ptr((const DESCR_t *)a[0].ptr);
    }
    if (!fname) fname = VARVAL_fn(a[0]);
    return APPLY_fn(fname, a + 1, n - 1);
}
static DESCR_t _ARG_(DESCR_t *a, int n);
static DESCR_t _DEFINE_(DESCR_t *a, int n);
static DESCR_t _FIELD_(DESCR_t *a, int n);
static DESCR_t _LOCAL_(DESCR_t *a, int n);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _LPAD_(DESCR_t *a, int n) {
    if (n < 2) return n > 0 ? a[0] : NULVCL;
    return lpad_fn(a[0], a[1], n > 2 ? a[2] : STRVAL(" "));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _RPAD_(DESCR_t *a, int n) {
    if (n < 2) return n > 0 ? a[0] : NULVCL;
    return rpad_fn(a[0], a[1], n > 2 ? a[2] : STRVAL(" "));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _CHAR_(DESCR_t *a, int n) {
    if (n < 1) return NULVCL;
    return BCHAR_fn(a[0]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _DUPL_(DESCR_t *a, int n) {
    if (n < 2) return NULVCL;
    return DUPL_fn(a[0], a[1]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _REMDR_(DESCR_t *a, int n) {
    if (n < 2) return FAILDESCR;
    extern int operand_is_real_str(DESCR_t v);
    /* .cmth extends remdr to reals in the SAME sentence as the 8 math fns (sbl.min:260); the real/int decision is arithmetic.c's, reused not respelled.  REMDR-by-zero stays a FAIL here:      */
    /* the oracle raises 167/312 instead, a SEPARATE measured divergence with its own blast radius over every program relying on the failure signal, and NOT part of this family.             */
    if (IS_REAL_fn(a[0]) || IS_REAL_fn(a[1]) || operand_is_real_str(a[0]) || operand_is_real_str(a[1])) {
        double ry = to_real(a[1]);
        if (ry == 0.0) return FAILDESCR;
        double rr = fmod(to_real(a[0]), ry);
        if (isinf(rr)) { core_runtime_error(312, "remdr caused real overflow"); return FAILDESCR; }
        return REALVAL(rr);
    }
    int64_t x = to_int(a[0]), y = to_int(a[1]);
    if (y == 0) return FAILDESCR;
    return INTVAL(x % y);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* SPITBOL extended math (.cmth, sbl.min:1658): ATAN CHOP COS EXP LN SIN SQRT TAN.  Each takes ONE arg, coerces INT/REAL/STRING via to_real, returns REAL ALWAYS -- CHOP(7) is 7. and         */
/* DATATYPE(SQRT(4)) is REAL, both measured against the live oracle.  Domain/overflow arms carry SPITBOL's own numbered errors (sbl.min:13223-15753): hard errors, not statement failures.   */
#define MATH_ARG(ECODE, EMSG)                                              \
    if (n < 1) return FAILDESCR;                                           \
    if (!is_numeric_like(a[0])) { core_runtime_error((ECODE), (EMSG)); return FAILDESCR; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _ATAN_(DESCR_t *a, int n) {
    MATH_ARG(301, "atan argument not numeric");
    return REALVAL(atan(to_real(a[0])));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _CHOP_(DESCR_t *a, int n) {
    MATH_ARG(302, "chop argument not numeric");
    return REALVAL(trunc(to_real(a[0])));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _COS_(DESCR_t *a, int n) {
    MATH_ARG(303, "cos argument not numeric");
    return REALVAL(cos(to_real(a[0])));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _EXP_(DESCR_t *a, int n) {
    MATH_ARG(304, "exp argument not numeric");
    double _r = exp(to_real(a[0]));
    if (isinf(_r)) { core_runtime_error(305, "exp produced real overflow"); return FAILDESCR; }
    return REALVAL(_r);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _LN_(DESCR_t *a, int n) {
    MATH_ARG(306, "ln argument not numeric");
    double _x = to_real(a[0]);
    if (_x < 0.0)  { core_runtime_error(315, "ln argument negative"); return FAILDESCR; }
    if (_x == 0.0) { core_runtime_error(307, "ln produced real overflow"); return FAILDESCR; }
    return REALVAL(log(_x));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _SIN_(DESCR_t *a, int n) {
    MATH_ARG(308, "sin argument not numeric");
    return REALVAL(sin(to_real(a[0])));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _SQRT_(DESCR_t *a, int n) {
    MATH_ARG(313, "sqrt argument not numeric");
    double _x = to_real(a[0]);
    if (_x < 0.0) { core_runtime_error(314, "sqrt argument negative"); return FAILDESCR; }
    return REALVAL(sqrt(_x));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _TAN_(DESCR_t *a, int n) {
    MATH_ARG(309, "tan argument not numeric");
    double _r = tan(to_real(a[0]));
    if (isinf(_r)) { core_runtime_error(310, "tan produced real overflow or argument is out of range"); return FAILDESCR; }
    return REALVAL(_r);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _REPLACE_(DESCR_t *a, int n) {
    if (n < 3) return NULVCL;
    return REPLACE_fn(a[0], a[1], a[2]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _TRIM_(DESCR_t *a, int n) {
    if (n < 1) return NULVCL;
    return TRIM_fn(a[0]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _SUBSTR_(DESCR_t *a, int n) {
    if (n < 2) return NULVCL;
    if (n < 3) {
        DESCR_t big = { .v = DT_I, .slen = 0, .i = 999999999 };
        return SUBSTR_fn(a[0], a[1], big);
    }
    return SUBSTR_fn(a[0], a[1], a[2]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _REVERSE_(DESCR_t *a, int n) {
    if (n < 1) return NULVCL;
    return REVERS_fn(a[0]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _DATATYPE_(DESCR_t *a, int n) {
    if (n < 1) return STRVAL("STRING");
    return STRVAL((char*)datatype(a[0]));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _LCASE_(DESCR_t *a, int n) {
    if (n < 1) return NULVCL;
    const char *s = VARVAL_fn(a[0]);
    if (!s) return NULVCL;
    char *r = rt_ws_strdup_c(s);
    for (int i = 0; r[i]; i++) r[i] = (char)tolower((unsigned char)r[i]);
    return STRVAL(r);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _UCASE__fn(DESCR_t *a, int n) {
    if (n < 1) return NULVCL;
    const char *s = VARVAL_fn(a[0]);
    if (!s) return NULVCL;
    char *r = rt_ws_strdup_c(s);
    for (int i = 0; r[i]; i++) r[i] = (char)toupper((unsigned char)r[i]);
    return STRVAL(r);
}
extern DESCR_t EVAL_fn(DESCR_t);
extern DESCR_t code(const char *src);
extern DESCR_t opsyn(DESCR_t, DESCR_t, DESCR_t);
extern DESCR_t sort_fn(DESCR_t);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _EVAL_(DESCR_t *a, int n)  { return EVAL_fn(n>0?a[0]:NULVCL); }
static DESCR_t _CODE_(DESCR_t *a, int n)  { return code(n>0?VARVAL_fn(a[0]):""); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _OPSYN_(DESCR_t *a, int n) {
    return opsyn(n>0?a[0]:NULVCL,n>1?a[1]:NULVCL,n>2?a[2]:NULVCL); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _SORT_(DESCR_t *a, int n)  { return sort_fn(n>0?a[0]:NULVCL); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _DATE_(DESCR_t *a, int n) {
    (void)a; (void)n;
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char *buf = rt_ws_alloc_c(20);
    strftime(buf, 20, "%m/%d/%Y %H:%M:%S", tm);
    return STRVAL(buf);
}
static int64_t _g_start_ms = -1;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _TIME_(DESCR_t *a, int n) {
    (void)a; (void)n;
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    int64_t now_ms = (int64_t)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
    if (_g_start_ms < 0) _g_start_ms = now_ms;
    return REALVAL((double)(now_ms - _g_start_ms));
}
static DESCR_t _INPUT_(DESCR_t *a, int n);
static DESCR_t _OUTPUT_(DESCR_t *a, int n);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _ARRAY_(DESCR_t *a, int n) {
    if (n < 1) return FAILDESCR;
    const char *proto = VARVAL_fn(a[0]);
    if (proto && strchr(proto, ':')) {
        int lo = 1, hi = 1, lo2 = 1, hi2 = 1;
        const char *comma = strchr(proto, ',');
        if (comma) {
            sscanf(proto,  "%d:%d", &lo, &hi);
            if (strchr(comma+1, ':')) {
                sscanf(comma+1, "%d:%d", &lo2, &hi2);
            } else {
                lo2 = 1; hi2 = atoi(comma+1);
                if (hi2 < 1) hi2 = 1;
            }
            ARBLK_t *arr2d = array_new2d(lo, hi, lo2, hi2);
            arr2d->proto_bare = 0;
            return ARRAY_VAL(arr2d);
        }
        sscanf(proto, "%d:%d", &lo, &hi);
        ARBLK_t *arr1d = array_new(lo, hi);
        arr1d->proto_bare = 0;
        return ARRAY_VAL(arr1d);
    }
    if (proto && strchr(proto, ',')) {
        int r = 1, c = 1;
        sscanf(proto, "%d,%d", &r, &c);
        if (r < 1) r = 1;
        if (c < 1) c = 1;
        ARBLK_t *arrrc = array_new2d(1, r, 1, c);
        arrrc->proto_bare = 1;
        return ARRAY_VAL(arrrc);
    }
    int sz = (int)to_int(a[0]);
    if (sz < 1) return FAILDESCR;
    ARBLK_t *arr = array_new(1, sz);
    arr->proto_bare = 1;
    if (n >= 2) {
        for (int i = 0; i < sz; i++) arr->data[i] = a[1];
    }
    return ARRAY_VAL(arr);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _TABLE_(DESCR_t *a, int n) {
    int init = (n >= 1) ? (int)to_int(a[0]) : 0;
    int inc  = (n >= 2) ? (int)to_int(a[1]) : 0;
    return TABLE_VAL(table_new_args(init, inc));
}   /* ⛔ THE 10s WERE A THIRD COPY OF THE TABLE DEFAULTS AND THEY WERE THE STALE COPY: this face passed 10 for an absent header count while the oracle's default is 11, so a table born here would have prototyped 10.  Passing 0 says "not supplied" and lets table_new own the answer, which is the same discipline the PROTOTYPE faces now follow.  Live only above nargs 3 today (BID_TABLE claims 0..3), and that is exactly why it must not be allowed to drift unobserved. */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _CONVERT_(DESCR_t *a, int n) {
    if (n < 2) return FAILDESCR;
    DESCR_t val  = a[0];
    const char *type = VARVAL_fn(a[1]);
    if (!type) return FAILDESCR;
    if (strcasecmp(type, "STRING")  == 0) {
        const char *s = VARVAL_fn(val);
        return s ? STRVAL(rt_ws_strdup_c(s)) : NULVCL;
    }
    if (strcasecmp(type, "INTEGER") == 0) {
        if (!IS_STR(val) && !IS_INT(val) && !IS_REAL(val)) return FAILDESCR;
        return INTVAL((int64_t)to_int(val));
    }
    if (strcasecmp(type, "REAL")    == 0) {
        if (!IS_STR(val) && !IS_INT(val) && !IS_REAL(val)) return FAILDESCR;
        return REALVAL(to_real(val));
    }
    if (strcasecmp(type, "ARRAY")   == 0) {
        if (IS_ARR(val)) return val;
        if (IS_TBL(val) && val.tbl) {
            TBBLK_t *tbl = val.tbl;
            int n = tbl->size;
            if (n == 0) return FAILDESCR;
            ARBLK_t *a = array_new2d(1, n, 1, 2);
            a->proto_bare = 1;
            int row = 1;
            for (int b = 0; b < TABLE_BUCKETS && row <= n; b++) {
                for (TBPAIR_t *e = tbl->buckets[b]; e && row <= n; e = e->next) {
                    DESCR_t kd = (e->key_descr.v != DT_SNUL)
                                 ? e->key_descr
                                 : STRVAL(e->key ? e->key : "");
                    array_set2(a, row, 1, kd);
                    array_set2(a, row, 2, e->val);
                    row++;
                }
            }
            return ARRAY_VAL(a);
        }
        return FAILDESCR;
    }
    if (strcasecmp(type, "TABLE")   == 0) {
        if (IS_TBL(val)) return val;
        if (IS_ARR(val) && val.arr) {
            ARBLK_t *a = val.arr;
            int rows = a->hi - a->lo + 1;
            int cols = a->hi2 - a->lo2 + 1;
            if (cols != 2) return FAILDESCR;
            TBBLK_t *tbl = table_new_args(rows > 0 ? rows : 10, 10);
            for (int i = a->lo; i <= a->hi; i++) {
                DESCR_t kd = array_get2(a, i, a->lo2);
                DESCR_t vd = array_get2(a, i, a->lo2 + 1);
                char kb[64];
                table_set_descr(tbl, tbl_key_str(kd, kb, sizeof kb), kd, vd);
            }
            return TABLE_VAL(tbl);   /* ⛔ THE KEY ENCODING IS SPELLED ONCE, IN tbl_key_str, AND THIS ARM WAS THE SECOND SPELLING.  It stored under VARVAL_fn(kd) -- the PLAIN string -- while every table read and write in the runtime (rt_subscript_var, subscript_set, the pattern paths) keys through tbl_key_str, which TYPE-TAGS the key: INTEGER 7 hashes as "\001i7", not "7".  So an integer key survived CONVERT(a,'TABLE') into a bucket no subscript could ever find, and crosscheck rung11 1113/006 `DIFFER(ata<7>, 45)` was measuring it -- vacuously, because the CONVERT above it had already failed and taken the same :f branch.  Dropping the `if (!key) continue` with it is not a widening: tbl_key_str answers for every datatype including the null string ("\001n"), so the guard could only ever have discarded a key the table is required to hold. */
        }
        return FAILDESCR;
    }
    if (strcasecmp(type, "PATTERN") == 0) {
        if (IS_PAT(val)) return val;
        if (IS_STR(val) || val.v == DT_SNUL) {
            const char *s = VARVAL_fn(val);
            return s ? pat_lit(s) : FAILDESCR;
        }
        return FAILDESCR;
    }
    if (strcasecmp(type, "CODE")       == 0) {
        const char *s = VARVAL_fn(val);
        if (!s || !*s) return FAILDESCR;
        return code(s);
    }
    if (strcasecmp(type, "EXPRESSION") == 0) {
        const char *s = VARVAL_fn(val);
        if (!s || !*s) return FAILDESCR;
        return compile_to_expression(s);
    }
    if (strcasecmp(type, "NAME") == 0) {
        const char *s = VARVAL_fn(val);
        if (!s || !*s) return FAILDESCR;
        return NAMEVAL(rt_ws_strdup_c(s));
    }
    if (strcasecmp(type, "NUMERIC")    == 0) {
        if (IS_INT(val)) return val;
        if (IS_REAL(val)) return val;
        if (IS_STR(val) || val.v == DT_SNUL) {
            const char *s = val.s ? val.s : "";
            while (*s == ' ') s++;
            if (!*s) return INTVAL(0);
            char *end = NULL;
            long long iv = strtoll(s, &end, 10);
            while (*end == ' ') end++;
            if (*end == '\0') return INTVAL((int64_t)iv);
            double rv = strtod(s, &end);
            while (*end == ' ') end++;
            if (*end == '\0') return REALVAL(rv);
        }
        return FAILDESCR;
    }
    return FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _COPY_(DESCR_t *a, int n) {
    if (n < 1) return FAILDESCR;
    DESCR_t v = a[0];
    if (IS_ARR(v)) {
        if (!v.arr) return v;
        int sz = v.arr->hi - v.arr->lo + 1;
        ARBLK_t *copy = array_new(v.arr->lo, v.arr->hi);
        copy->proto_bare = v.arr->proto_bare;
        for (int i = 0; i < sz; i++) copy->data[i] = v.arr->data[i];
        return ARRAY_VAL(copy);
    }
    return v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _b_nPush(DESCR_t *a, int n) {
    (void)a; (void)n;
    NPUSH_fn();
    return NULVCL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _b_nInc(DESCR_t *a, int n) {
    (void)a; (void)n;
    NINC_fn();
    return INTVAL(ntop());
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _b_nDec(DESCR_t *a, int n) {
    (void)a; (void)n;
    NDEC_fn();
    return INTVAL(ntop());
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _b_nTop(DESCR_t *a, int n) {
    (void)a; (void)n;
    return INTVAL(ntop());
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _b_nPop(DESCR_t *a, int n) {
    (void)a; (void)n;
    int64_t val = ntop();
    NPOP_fn();
    return INTVAL(val);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _b_tree_n(DESCR_t *a, int n) {
    if (n < 1) return INTVAL(0);
    return FIELD_GET_fn(a[0], "n");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _b_tree_t(DESCR_t *a, int n) {
    if (n < 1) return NULVCL;
    return FIELD_GET_fn(a[0], "t");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _b_tree_v(DESCR_t *a, int n) {
    if (n < 1) return NULVCL;
    return FIELD_GET_fn(a[0], "v");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _b_tree_c(DESCR_t *a, int n) {
    if (n < 1) return NULVCL;
    return FIELD_GET_fn(a[0], "c");
}
extern DESCR_t rsort_fn(DESCR_t t);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _RSORT_(DESCR_t *a, int n) { return rsort_fn(n>0?a[0]:NULVCL); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _CLEAR_(DESCR_t *a, int n) {
    (void)a; (void)n;
    NV_CLEAR_fn();
    return NULVCL;
}
static char _setexit_label[256];
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _SETEXIT_(DESCR_t *a, int n) {
    if (n < 1 || a[0].v == DT_FAIL) {
        _setexit_label[0] = '\0';
        return NULVCL;
    }
    const char *lbl = VARVAL_fn(a[0]);
    if (lbl) strncpy(_setexit_label, lbl, sizeof(_setexit_label)-1);
    return NULVCL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _FUNCTION_(DESCR_t *a, int n) {
    if (n < 1) return FAILDESCR;
    const char *name = VARVAL_fn(a[0]);
    if (!name || !*name) return FAILDESCR;
    return FNCEX_fn(name) ? STRVAL(rt_ws_strdup_c(name)) : FAILDESCR;
}
static int (*_label_exists_hook)(const char *) = NULL;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void core_set_label_exists_hook(int (*fn)(const char *)) { _label_exists_hook = fn; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _LABEL_(DESCR_t *a, int n) {
    if (n < 1) return FAILDESCR;
    const char *name = VARVAL_fn(a[0]);
    if (!name || !*name) return FAILDESCR;
    if (_label_exists_hook && _label_exists_hook(name))
        return STRVAL(rt_ws_strdup_c(name));
    return FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _COLLECT_(DESCR_t *a, int n) {
    (void)a; (void)n;
    { extern long rt_gc_collect(void); extern long rt_gcheap_free(void); rt_gc_collect(); return INTVAL((int64_t)rt_gcheap_free()); }
}
static void var_dump(void);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _DUMP_(DESCR_t *a, int n) {
    (void)a; (void)n;
    var_dump();
    return NULVCL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _TRACE_(DESCR_t *a, int n) {
    if (n < 1) return FAILDESCR;
    const char *varname = VARVAL_fn(a[0]);
    if (!varname || !*varname) return FAILDESCR;
    if (getenv("SCRIP_DEBUG_TRACE"))
        fprintf(stderr, "[scrip-trace] _TRACE_ entry n=%d varname=%s\n", n, varname);
    const char *type = (n >= 2) ? VARVAL_fn(a[1]) : "VALUE";
    if (!type || !*type) type = "VALUE";
    if (getenv("SCRIP_DEBUG_TRACE"))
        fprintf(stderr, "[scrip-trace] _TRACE_ type=%s\n", type);
    const char *cbfn = (n >= 4) ? VARVAL_fn(a[3]) : "";
    if (type && (strcmp(type,"VALUE")==0 || strcmp(type,"value")==0)) {
        if (cbfn && *cbfn) {
            trace_register_callback(varname, cbfn);
        } else {
            trace_register(varname);
        }
    }
    return STRVAL(rt_ws_strdup_c(varname));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _STOPTR_(DESCR_t *a, int n) {
    if (n < 1) return FAILDESCR;
    const char *varname = VARVAL_fn(a[0]);
    if (!varname || !*varname) return FAILDESCR;
    trace_unregister(varname);
    return STRVAL(rt_ws_strdup_c(varname));
}
static DATBLK_t *_udef_lookup(const char *name);
typedef struct { char *typename; int nfields; char **fields; } DataClosure;
typedef struct { char *typename; char *fieldname; } FieldClosure;
#define DATA_MAX_TYPES 64
#define DATA_MAX_FIELDS 16
static struct {
    char *typename;
    int   nfields;
    char *fields[DATA_MAX_FIELDS];
} _data_types[DATA_MAX_TYPES];
static int _data_ntypes = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _make_ctor(int tidx, DESCR_t *args, int nargs) {
    if (tidx < 0 || tidx >= _data_ntypes) return NULVCL;
    DATBLK_t *t = _udef_lookup(_data_types[tidx].typename);
    if (!t) return NULVCL;
    DATINST_t *u = rt_ws_alloc(sizeof(DATINST_t));
    u->type   = t;
    u->id     = t->serial_next++;
    u->fields = rt_ws_alloc(t->nfields * sizeof(DESCR_t));
    for (int i = 0; i < t->nfields; i++)
        u->fields[i] = (i < nargs) ? args[i] : NULVCL;
    return (DESCR_t){ .v = DT_DATA, .u = u };
}
#define CTOR_FN(idx) \
static DESCR_t _ctor_##idx(DESCR_t *a, int n) { return _make_ctor(idx, a, n); }
CTOR_FN(0)  CTOR_FN(1)  CTOR_FN(2)  CTOR_FN(3)
CTOR_FN(4)  CTOR_FN(5)  CTOR_FN(6)  CTOR_FN(7)
CTOR_FN(8)  CTOR_FN(9)  CTOR_FN(10) CTOR_FN(11)
CTOR_FN(12) CTOR_FN(13) CTOR_FN(14) CTOR_FN(15)
CTOR_FN(16) CTOR_FN(17) CTOR_FN(18) CTOR_FN(19)
CTOR_FN(20) CTOR_FN(21) CTOR_FN(22) CTOR_FN(23)
CTOR_FN(24) CTOR_FN(25) CTOR_FN(26) CTOR_FN(27)
CTOR_FN(28) CTOR_FN(29) CTOR_FN(30) CTOR_FN(31)
CTOR_FN(32) CTOR_FN(33) CTOR_FN(34) CTOR_FN(35)
CTOR_FN(36) CTOR_FN(37) CTOR_FN(38) CTOR_FN(39)
CTOR_FN(40) CTOR_FN(41) CTOR_FN(42) CTOR_FN(43)
CTOR_FN(44) CTOR_FN(45) CTOR_FN(46) CTOR_FN(47)
CTOR_FN(48) CTOR_FN(49) CTOR_FN(50) CTOR_FN(51)
CTOR_FN(52) CTOR_FN(53) CTOR_FN(54) CTOR_FN(55)
CTOR_FN(56) CTOR_FN(57) CTOR_FN(58) CTOR_FN(59)
CTOR_FN(60) CTOR_FN(61) CTOR_FN(62) CTOR_FN(63)
static DESCR_t (*_ctor_fns[DATA_MAX_TYPES])(DESCR_t *, int) = {
    _ctor_0,  _ctor_1,  _ctor_2,  _ctor_3,
    _ctor_4,  _ctor_5,  _ctor_6,  _ctor_7,
    _ctor_8,  _ctor_9,  _ctor_10, _ctor_11,
    _ctor_12, _ctor_13, _ctor_14, _ctor_15,
    _ctor_16, _ctor_17, _ctor_18, _ctor_19,
    _ctor_20, _ctor_21, _ctor_22, _ctor_23,
    _ctor_24, _ctor_25, _ctor_26, _ctor_27,
    _ctor_28, _ctor_29, _ctor_30, _ctor_31,
    _ctor_32, _ctor_33, _ctor_34, _ctor_35,
    _ctor_36, _ctor_37, _ctor_38, _ctor_39,
    _ctor_40, _ctor_41, _ctor_42, _ctor_43,
    _ctor_44, _ctor_45, _ctor_46, _ctor_47,
    _ctor_48, _ctor_49, _ctor_50, _ctor_51,
    _ctor_52, _ctor_53, _ctor_54, _ctor_55,
    _ctor_56, _ctor_57, _ctor_58, _ctor_59,
    _ctor_60, _ctor_61, _ctor_62, _ctor_63,
};
#define FIELD_ACCESSOR_MAX (DATA_MAX_TYPES * DATA_MAX_FIELDS)
static struct { int tidx; int fidx; } _facc_slots[FIELD_ACCESSOR_MAX];
static int _facc_n = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _make_fget(int slot, DESCR_t obj) {
    if (slot < 0 || slot >= _facc_n) return FAILDESCR;
    int tidx = _facc_slots[slot].tidx;
    int fidx = _facc_slots[slot].fidx;
    if (!IS_DATA(obj) || !obj.u) return FAILDESCR;
    if (fidx < 0 || fidx >= obj.u->type->nfields) return FAILDESCR;
    return obj.u->fields[fidx];
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void _make_fset(int slot, DESCR_t obj, DESCR_t val) {
    if (slot < 0 || slot >= _facc_n) return;
    int fidx = _facc_slots[slot].fidx;
    if (!IS_DATA(obj) || !obj.u) return;
    if (fidx < 0 || fidx >= obj.u->type->nfields) return;
    obj.u->fields[fidx] = val;
}
#define FACC_FN(idx) \
static DESCR_t _facc_get_##idx(DESCR_t *a, int n) { \
    return n>=1 ? _make_fget(idx, a[0]) : NULVCL; }
#define FACC_SET_FN(idx) \
static DESCR_t _facc_set_##idx(DESCR_t *a, int n) { \
    if (n>=2) _make_fset(idx, a[1], a[0]); \
    return n>=1 ? a[0] : NULVCL; }
FACC_FN(0)   FACC_FN(1)   FACC_FN(2)   FACC_FN(3)
FACC_FN(4)   FACC_FN(5)   FACC_FN(6)   FACC_FN(7)
FACC_FN(8)   FACC_FN(9)   FACC_FN(10)  FACC_FN(11)
FACC_FN(12)  FACC_FN(13)  FACC_FN(14)  FACC_FN(15)
FACC_FN(16)  FACC_FN(17)  FACC_FN(18)  FACC_FN(19)
FACC_FN(20)  FACC_FN(21)  FACC_FN(22)  FACC_FN(23)
FACC_FN(24)  FACC_FN(25)  FACC_FN(26)  FACC_FN(27)
FACC_FN(28)  FACC_FN(29)  FACC_FN(30)  FACC_FN(31)
FACC_FN(32)  FACC_FN(33)  FACC_FN(34)  FACC_FN(35)
FACC_FN(36)  FACC_FN(37)  FACC_FN(38)  FACC_FN(39)
FACC_FN(40)  FACC_FN(41)  FACC_FN(42)  FACC_FN(43)
FACC_FN(44)  FACC_FN(45)  FACC_FN(46)  FACC_FN(47)
FACC_FN(48)  FACC_FN(49)  FACC_FN(50)  FACC_FN(51)
FACC_FN(52)  FACC_FN(53)  FACC_FN(54)  FACC_FN(55)
FACC_FN(56)  FACC_FN(57)  FACC_FN(58)  FACC_FN(59)
FACC_FN(60)  FACC_FN(61)  FACC_FN(62)  FACC_FN(63)
FACC_FN(64)  FACC_FN(65)  FACC_FN(66)  FACC_FN(67)
FACC_FN(68)  FACC_FN(69)  FACC_FN(70)  FACC_FN(71)
FACC_FN(72)  FACC_FN(73)  FACC_FN(74)  FACC_FN(75)
FACC_FN(76)  FACC_FN(77)  FACC_FN(78)  FACC_FN(79)
FACC_FN(80)  FACC_FN(81)  FACC_FN(82)  FACC_FN(83)
FACC_FN(84)  FACC_FN(85)  FACC_FN(86)  FACC_FN(87)
FACC_FN(88)  FACC_FN(89)  FACC_FN(90)  FACC_FN(91)
FACC_FN(92)  FACC_FN(93)  FACC_FN(94)  FACC_FN(95)
FACC_FN(96)  FACC_FN(97)  FACC_FN(98)  FACC_FN(99)
FACC_FN(100) FACC_FN(101) FACC_FN(102) FACC_FN(103)
FACC_FN(104) FACC_FN(105) FACC_FN(106) FACC_FN(107)
FACC_FN(108) FACC_FN(109) FACC_FN(110) FACC_FN(111)
FACC_FN(112) FACC_FN(113) FACC_FN(114) FACC_FN(115)
FACC_FN(116) FACC_FN(117) FACC_FN(118) FACC_FN(119)
FACC_FN(120) FACC_FN(121) FACC_FN(122) FACC_FN(123)
FACC_FN(124) FACC_FN(125) FACC_FN(126) FACC_FN(127)
FACC_SET_FN(0)   FACC_SET_FN(1)   FACC_SET_FN(2)   FACC_SET_FN(3)
FACC_SET_FN(4)   FACC_SET_FN(5)   FACC_SET_FN(6)   FACC_SET_FN(7)
FACC_SET_FN(8)   FACC_SET_FN(9)   FACC_SET_FN(10)  FACC_SET_FN(11)
FACC_SET_FN(12)  FACC_SET_FN(13)  FACC_SET_FN(14)  FACC_SET_FN(15)
FACC_SET_FN(16)  FACC_SET_FN(17)  FACC_SET_FN(18)  FACC_SET_FN(19)
FACC_SET_FN(20)  FACC_SET_FN(21)  FACC_SET_FN(22)  FACC_SET_FN(23)
FACC_SET_FN(24)  FACC_SET_FN(25)  FACC_SET_FN(26)  FACC_SET_FN(27)
FACC_SET_FN(28)  FACC_SET_FN(29)  FACC_SET_FN(30)  FACC_SET_FN(31)
FACC_SET_FN(32)  FACC_SET_FN(33)  FACC_SET_FN(34)  FACC_SET_FN(35)
FACC_SET_FN(36)  FACC_SET_FN(37)  FACC_SET_FN(38)  FACC_SET_FN(39)
FACC_SET_FN(40)  FACC_SET_FN(41)  FACC_SET_FN(42)  FACC_SET_FN(43)
FACC_SET_FN(44)  FACC_SET_FN(45)  FACC_SET_FN(46)  FACC_SET_FN(47)
FACC_SET_FN(48)  FACC_SET_FN(49)  FACC_SET_FN(50)  FACC_SET_FN(51)
FACC_SET_FN(52)  FACC_SET_FN(53)  FACC_SET_FN(54)  FACC_SET_FN(55)
FACC_SET_FN(56)  FACC_SET_FN(57)  FACC_SET_FN(58)  FACC_SET_FN(59)
FACC_SET_FN(60)  FACC_SET_FN(61)  FACC_SET_FN(62)  FACC_SET_FN(63)
FACC_SET_FN(64)  FACC_SET_FN(65)  FACC_SET_FN(66)  FACC_SET_FN(67)
FACC_SET_FN(68)  FACC_SET_FN(69)  FACC_SET_FN(70)  FACC_SET_FN(71)
FACC_SET_FN(72)  FACC_SET_FN(73)  FACC_SET_FN(74)  FACC_SET_FN(75)
FACC_SET_FN(76)  FACC_SET_FN(77)  FACC_SET_FN(78)  FACC_SET_FN(79)
FACC_SET_FN(80)  FACC_SET_FN(81)  FACC_SET_FN(82)  FACC_SET_FN(83)
FACC_SET_FN(84)  FACC_SET_FN(85)  FACC_SET_FN(86)  FACC_SET_FN(87)
FACC_SET_FN(88)  FACC_SET_FN(89)  FACC_SET_FN(90)  FACC_SET_FN(91)
FACC_SET_FN(92)  FACC_SET_FN(93)  FACC_SET_FN(94)  FACC_SET_FN(95)
FACC_SET_FN(96)  FACC_SET_FN(97)  FACC_SET_FN(98)  FACC_SET_FN(99)
FACC_SET_FN(100) FACC_SET_FN(101) FACC_SET_FN(102) FACC_SET_FN(103)
FACC_SET_FN(104) FACC_SET_FN(105) FACC_SET_FN(106) FACC_SET_FN(107)
FACC_SET_FN(108) FACC_SET_FN(109) FACC_SET_FN(110) FACC_SET_FN(111)
FACC_SET_FN(112) FACC_SET_FN(113) FACC_SET_FN(114) FACC_SET_FN(115)
FACC_SET_FN(116) FACC_SET_FN(117) FACC_SET_FN(118) FACC_SET_FN(119)
FACC_SET_FN(120) FACC_SET_FN(121) FACC_SET_FN(122) FACC_SET_FN(123)
FACC_SET_FN(124) FACC_SET_FN(125) FACC_SET_FN(126) FACC_SET_FN(127)
static DESCR_t (*_facc_set_fns[FIELD_ACCESSOR_MAX])(DESCR_t *, int) = {
    _facc_set_0,   _facc_set_1,   _facc_set_2,   _facc_set_3,
    _facc_set_4,   _facc_set_5,   _facc_set_6,   _facc_set_7,
    _facc_set_8,   _facc_set_9,   _facc_set_10,  _facc_set_11,
    _facc_set_12,  _facc_set_13,  _facc_set_14,  _facc_set_15,
    _facc_set_16,  _facc_set_17,  _facc_set_18,  _facc_set_19,
    _facc_set_20,  _facc_set_21,  _facc_set_22,  _facc_set_23,
    _facc_set_24,  _facc_set_25,  _facc_set_26,  _facc_set_27,
    _facc_set_28,  _facc_set_29,  _facc_set_30,  _facc_set_31,
    _facc_set_32,  _facc_set_33,  _facc_set_34,  _facc_set_35,
    _facc_set_36,  _facc_set_37,  _facc_set_38,  _facc_set_39,
    _facc_set_40,  _facc_set_41,  _facc_set_42,  _facc_set_43,
    _facc_set_44,  _facc_set_45,  _facc_set_46,  _facc_set_47,
    _facc_set_48,  _facc_set_49,  _facc_set_50,  _facc_set_51,
    _facc_set_52,  _facc_set_53,  _facc_set_54,  _facc_set_55,
    _facc_set_56,  _facc_set_57,  _facc_set_58,  _facc_set_59,
    _facc_set_60,  _facc_set_61,  _facc_set_62,  _facc_set_63,
    _facc_set_64,  _facc_set_65,  _facc_set_66,  _facc_set_67,
    _facc_set_68,  _facc_set_69,  _facc_set_70,  _facc_set_71,
    _facc_set_72,  _facc_set_73,  _facc_set_74,  _facc_set_75,
    _facc_set_76,  _facc_set_77,  _facc_set_78,  _facc_set_79,
    _facc_set_80,  _facc_set_81,  _facc_set_82,  _facc_set_83,
    _facc_set_84,  _facc_set_85,  _facc_set_86,  _facc_set_87,
    _facc_set_88,  _facc_set_89,  _facc_set_90,  _facc_set_91,
    _facc_set_92,  _facc_set_93,  _facc_set_94,  _facc_set_95,
    _facc_set_96,  _facc_set_97,  _facc_set_98,  _facc_set_99,
    _facc_set_100, _facc_set_101, _facc_set_102, _facc_set_103,
    _facc_set_104, _facc_set_105, _facc_set_106, _facc_set_107,
    _facc_set_108, _facc_set_109, _facc_set_110, _facc_set_111,
    _facc_set_112, _facc_set_113, _facc_set_114, _facc_set_115,
    _facc_set_116, _facc_set_117, _facc_set_118, _facc_set_119,
    _facc_set_120, _facc_set_121, _facc_set_122, _facc_set_123,
    _facc_set_124, _facc_set_125, _facc_set_126, _facc_set_127,
};
static DESCR_t (*_facc_fns[FIELD_ACCESSOR_MAX])(DESCR_t *, int) = {
    _facc_get_0,   _facc_get_1,   _facc_get_2,   _facc_get_3,
    _facc_get_4,   _facc_get_5,   _facc_get_6,   _facc_get_7,
    _facc_get_8,   _facc_get_9,   _facc_get_10,  _facc_get_11,
    _facc_get_12,  _facc_get_13,  _facc_get_14,  _facc_get_15,
    _facc_get_16,  _facc_get_17,  _facc_get_18,  _facc_get_19,
    _facc_get_20,  _facc_get_21,  _facc_get_22,  _facc_get_23,
    _facc_get_24,  _facc_get_25,  _facc_get_26,  _facc_get_27,
    _facc_get_28,  _facc_get_29,  _facc_get_30,  _facc_get_31,
    _facc_get_32,  _facc_get_33,  _facc_get_34,  _facc_get_35,
    _facc_get_36,  _facc_get_37,  _facc_get_38,  _facc_get_39,
    _facc_get_40,  _facc_get_41,  _facc_get_42,  _facc_get_43,
    _facc_get_44,  _facc_get_45,  _facc_get_46,  _facc_get_47,
    _facc_get_48,  _facc_get_49,  _facc_get_50,  _facc_get_51,
    _facc_get_52,  _facc_get_53,  _facc_get_54,  _facc_get_55,
    _facc_get_56,  _facc_get_57,  _facc_get_58,  _facc_get_59,
    _facc_get_60,  _facc_get_61,  _facc_get_62,  _facc_get_63,
    _facc_get_64,  _facc_get_65,  _facc_get_66,  _facc_get_67,
    _facc_get_68,  _facc_get_69,  _facc_get_70,  _facc_get_71,
    _facc_get_72,  _facc_get_73,  _facc_get_74,  _facc_get_75,
    _facc_get_76,  _facc_get_77,  _facc_get_78,  _facc_get_79,
    _facc_get_80,  _facc_get_81,  _facc_get_82,  _facc_get_83,
    _facc_get_84,  _facc_get_85,  _facc_get_86,  _facc_get_87,
    _facc_get_88,  _facc_get_89,  _facc_get_90,  _facc_get_91,
    _facc_get_92,  _facc_get_93,  _facc_get_94,  _facc_get_95,
    _facc_get_96,  _facc_get_97,  _facc_get_98,  _facc_get_99,
    _facc_get_100, _facc_get_101, _facc_get_102, _facc_get_103,
    _facc_get_104, _facc_get_105, _facc_get_106, _facc_get_107,
    _facc_get_108, _facc_get_109, _facc_get_110, _facc_get_111,
    _facc_get_112, _facc_get_113, _facc_get_114, _facc_get_115,
    _facc_get_116, _facc_get_117, _facc_get_118, _facc_get_119,
    _facc_get_120, _facc_get_121, _facc_get_122, _facc_get_123,
    _facc_get_124, _facc_get_125, _facc_get_126, _facc_get_127,
};
static int fn_has_builtin(const char *name);
static void _func_init(void);
DESCR_t core_DATA_register(DESCR_t *a, int n);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _DATA_(DESCR_t *a, int n) { return core_DATA_register(a, n); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t core_DATA_register(DESCR_t *a, int n) {
    if (n < 1) return NULVCL;
    const char *raw_spec = VARVAL_fn(a[0]);
    if (!raw_spec || !*raw_spec) return NULVCL;
    char *spec = rt_ws_strdup(raw_spec);
    DEFDAT_fn(spec);
    char *s = rt_ws_strdup(spec);
    char *paren = strchr(s, '(');
    if (!paren) return NULVCL;
    *paren = '\0';
    char *tname = s;
    char *fstr = paren + 1;
    char *close = strchr(fstr, ')');
    if (close) *close = '\0';
    if (_data_ntypes >= DATA_MAX_TYPES) return NULVCL;
    int tidx = _data_ntypes++;
    char *uname = rt_ws_strdup(tname);
    _data_types[tidx].typename = uname;
    int nf = 0;
    char *tmp = rt_ws_strdup(fstr);
    char *tok = strtok(tmp, ",");
    while (tok && nf < DATA_MAX_FIELDS) {
        while (*tok == ' ') tok++;
        char *end = tok + strlen(tok) - 1;
        while (end > tok && *end == ' ') *end-- = '\0';
        char *fld = rt_ws_strdup(tok);
        _data_types[tidx].fields[nf] = fld;
        nf++;
        tok = strtok(NULL, ",");
    }
    _data_types[tidx].nfields = nf;
    extern void register_fn(const char *, DESCR_t (*)(DESCR_t*, int), int, int);
    register_fn(uname, _ctor_fns[tidx], 0, nf);
    for (int fi = 0; fi < nf; fi++) {
        if (_facc_n >= FIELD_ACCESSOR_MAX) break;
        int slot = _facc_n++;
        _facc_slots[slot].tidx = tidx;
        _facc_slots[slot].fidx = fi;
        const char *fname = _data_types[tidx].fields[fi];
        register_fn(fname, _facc_fns[slot], 1, 1);
        char setname[256];
        snprintf(setname, sizeof(setname), "%s_SET", fname);
        register_fn(setname, _facc_set_fns[slot], 2, 2);
    }
    return NULVCL;
}
extern DESCR_t pat_span(const char *);
extern DESCR_t pat_break_(const char *);
extern DESCR_t pat_breakx(const char *);
extern DESCR_t pat_any_cs(const char *);
extern DESCR_t pat_notany(const char *);
extern DESCR_t pat_len(int64_t);
extern DESCR_t pat_pos(int64_t);
extern DESCR_t pat_rpos(int64_t);
extern DESCR_t pat_tab(int64_t);
extern DESCR_t pat_rtab(int64_t);
extern DESCR_t pat_arb(void);
extern DESCR_t pat_rem(void);
extern DESCR_t pat_fail(void);
extern DESCR_t pat_abort(void);
extern DESCR_t pat_succeed(void);
extern DESCR_t pat_bal(void);
extern DESCR_t pat_arbno(DESCR_t);
extern DESCR_t pat_fence(void);
extern DESCR_t pat_fence_p(DESCR_t);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _PAT_SPAN_(DESCR_t *a, int n)    { return n>=1 ? pat_span(VARVAL_fn(a[0]))    : FAILDESCR; }
static DESCR_t _PAT_BREAK_(DESCR_t *a, int n)   { return n>=1 ? pat_break_(VARVAL_fn(a[0]))  : FAILDESCR; }
static DESCR_t _PAT_BREAKX_(DESCR_t *a, int n)  { return n>=1 ? pat_breakx(VARVAL_fn(a[0]))  : FAILDESCR; }
static DESCR_t _PAT_ANY_(DESCR_t *a, int n)     { return n>=1 ? pat_any_cs(VARVAL_fn(a[0]))  : FAILDESCR; }
static DESCR_t _PAT_NOTANY_(DESCR_t *a, int n)  { return n>=1 ? pat_notany(VARVAL_fn(a[0]))  : FAILDESCR; }
static DESCR_t _PAT_LEN_(DESCR_t *a, int n)     { return n>=1 ? pat_len(to_int(a[0]))   : FAILDESCR; }
static DESCR_t _PAT_POS_(DESCR_t *a, int n)     { return n>=1 ? pat_pos(to_int(a[0]))   : FAILDESCR; }
static DESCR_t _PAT_RPOS_(DESCR_t *a, int n)    { return n>=1 ? pat_rpos(to_int(a[0]))  : FAILDESCR; }
static DESCR_t _PAT_TAB_(DESCR_t *a, int n)     { return n>=1 ? pat_tab(to_int(a[0]))   : FAILDESCR; }
static DESCR_t _PAT_RTAB_(DESCR_t *a, int n)    { return n>=1 ? pat_rtab(to_int(a[0]))  : FAILDESCR; }
static DESCR_t _PAT_ARB_(DESCR_t *a, int n)     { (void)a;(void)n; return pat_arb();     }
static DESCR_t _PAT_REM_(DESCR_t *a, int n)     { (void)a;(void)n; return pat_rem();     }
static DESCR_t _PAT_FAIL_(DESCR_t *a, int n)    { (void)a;(void)n; return pat_fail();    }
static DESCR_t _PAT_ABORT_(DESCR_t *a, int n)   { (void)a;(void)n; return pat_abort();   }
static DESCR_t _PAT_SUCCEED_(DESCR_t *a, int n) { (void)a;(void)n; return pat_succeed(); }
static DESCR_t _PAT_BAL_(DESCR_t *a, int n)     { (void)a;(void)n; return pat_bal();     }
static DESCR_t _PAT_ARBNO_(DESCR_t *a, int n)   { return n>=1 ? pat_arbno(a[0])  : FAILDESCR; }
static DESCR_t _PAT_FENCE_(DESCR_t *a, int n)   { return n>=1 ? pat_fence_p(a[0]) : pat_fence(); }
static DESCR_t _PAT_ALT_(DESCR_t *a, int n)     { return n>=2 ? pat_alt(a[0], a[1])  : (n>=1 ? a[0] : FAILDESCR); }
static DESCR_t _PAT_CONCAT_(DESCR_t *a, int n)  { return n>=2 ? pat_cat(a[0], a[1])  : (n>=1 ? a[0] : FAILDESCR); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _PROTOTYPE_(DESCR_t *a, int n) { return agg_prototype(n >= 1 ? a[0] : NULVCL); }   /* ⛔ THIS IS A FACE, NOT AN IMPLEMENTATION -- see agg_prototype (aggregates.c) for the one authority and the oracle receipts.  The body that stood here was the SECOND spelling of PROTOTYPE and the one every non-1-argument call reached, and it was wrong three ways at once: it stringified answers the oracle gives as INTEGERs, it answered the null string for a TABLE where the oracle answers the hash-header count, and it failed the statement for an invalid object where the oracle raises ERROR 164.  It is kept as a face rather than deleted because the registry it is registered in is a REACHABLE entry point -- APPLY and OPSYN's register_fn_alias both resolve through it, and a runtime OPSYN('P','PROTOTYPE') alias was measurably answering '1:3' where the direct call answered 3.  n >= 1 is argument PRESENCE, not an argument COUNT: a call written with no argument passes the null string, which is an invalid object and takes the 164 like any other. */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _ITEM_(DESCR_t *a, int n) {
    if (n < 2) return FAILDESCR;
    DESCR_t arr = a[0];
    if (IS_TBL(arr)) {
        const char *k = VARVAL_fn(a[1]);
        return table_get(arr.tbl, k ? k : "");
    }
    if (IS_ARR(arr)) {
        int i = (int)to_int(a[1]);
        if (n == 2) return array_get(arr.arr, i);
        int j = (int)to_int(a[2]);
        return array_get2(arr.arr, i, j);
    }
    return FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _VALUE_(DESCR_t *a, int n) {
    if (n < 1) return FAILDESCR;
    const char *name = VARVAL_fn(a[0]);
    if (!name) return FAILDESCR;
    char *fname = rt_ws_strdup(name);
    return NV_GET_fn(fname);
}
int core_stack_floor_raised = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void core_lib_init(void) {
    { extern void rt_gcheap_warmup(void); rt_gcheap_warmup(); }   /* W1-GC-WARMUP (mode-4 twin): mode-4's generated main calls core_lib_init directly and never calls
     * rt_gcheap_warmup itself (only scrip.c's mode-3 driver did, at two call sites before rt_outer_call/rt_outer_call_delta0). gc_static_segs_init's dl_iterate_phdr
     * uses movaps and requires 16B-aligned rsp; core_lib_init runs from a genuine C prologue (called via plain `call` from generated main, before any JIT blob),
     * so driving the one-time census here gives mode-4 the same guarantee mode-3 already has. Idempotent via gc_static_segs_init's own g_gc_nseg>=0 guard. */
    if (!core_stack_floor_raised) {
        core_stack_floor_raised = 1;
        long floor = 64L * 1024 * 1024;
        const char *e = getenv("SCRIP_STACK");
        if (e && e[0]) { char *ep = NULL; long ev = strtol(e, &ep, 10);
            if (ev > 0) { if (ep && (*ep == 'k' || *ep == 'K')) ev *= 1024L; else if (ep && (*ep == 'm' || *ep == 'M')) ev *= 1024L * 1024L; floor = ev; } }
        struct rlimit rl;
        if (getrlimit(RLIMIT_STACK, &rl) == 0) {
            if (rl.rlim_max != RLIM_INFINITY && (rlim_t)floor > rl.rlim_max) floor = (long)rl.rlim_max;
            if (rl.rlim_cur == RLIM_INFINITY || (rlim_t)floor > rl.rlim_cur) { rl.rlim_cur = (rlim_t)floor; setrlimit(RLIMIT_STACK, &rl); }
        }
    }
    for (int i = 0; i < 256; i++) alphabet[i] = (char)i;
    alphabet[256] = '\0';
    { extern void rt_kw_seed_defaults(void); rt_kw_seed_defaults(); }   /* KW-5b: the keyword block's oracle-true initials must be live BEFORE statement one, or a program that never mentions a keyword runs on the C initializers -- measured as a wrong &TRIM answer on a bare `L = INPUT` (see rt_kw_seed_defaults). Unconditional since KW-4 deleted the killswitch. */
    { struct timespec _ts; clock_gettime(CLOCK_MONOTONIC, &_ts);
      _g_start_ms = (int64_t)_ts.tv_sec * 1000 + _ts.tv_nsec / 1000000; }
    const char *mon_fifo = getenv("MONITOR_READY_PIPE");
    if (mon_fifo && mon_fifo[0]) {
        monitor_fd = open(mon_fifo, O_WRONLY | O_NONBLOCK);
        if (monitor_fd < 0) monitor_fd = open(mon_fifo, O_WRONLY);
        const char *go_pipe = getenv("MONITOR_GO_PIPE");
        if (go_pipe && go_pipe[0])
            monitor_ack_fd = open(go_pipe, O_RDONLY);
        const char *names_path = getenv("MONITOR_NAMES_FILE");
        if (names_path && names_path[0])
            (void)load_names_file_bin(names_path);
    } else {
        const char *mon = getenv("MONITOR");
        if (mon && mon[0] == '1') monitor_fd = 2;
    }
    {
        const char *ev_ftr = getenv("SCRIP_FTRACE");
        if (ev_ftr && ev_ftr[0]) {
            int64_t v = (int64_t)strtoll(ev_ftr, NULL, 10);
            if (v > 0) kw_ftrace = v;
        }
        const char *ev_tr = getenv("SCRIP_TRACE");
        if (ev_tr && ev_tr[0]) {
            int64_t v = (int64_t)strtoll(ev_tr, NULL, 10);
            if (v > 0) kw_trace = v;
        }
    }
    {
        const char *ev_bin = getenv("MONITOR_BIN");
        if (ev_bin && ev_bin[0] && ev_bin[0] != '0') {
            g_monitor_bin = 1;
            atexit(mon_at_exit);
        }
    }
    extern void register_fn(const char *, DESCR_t (*)(DESCR_t*, int), int, int);
    register_fn("GT",       _GT_,       2, 2);
    register_fn("LT",       _LT_,       2, 2);
    register_fn("GE",       _GE_,       2, 2);
    register_fn("LE",       _LE_,       2, 2);
    register_fn("EQ",       _EQ_,       2, 2);
    register_fn("NE",       _NE_,       2, 2);
    register_fn("add",      _b_add,      2, 2);
    register_fn("sub",      _b_sub,      2, 2);
    register_fn("mul",      _b_mul,      2, 2);
    register_fn("DIVIDE_fn",_b_div,      2, 2);
    register_fn("POWER_fn", _b_pow,      2, 2);
    register_fn("neg",      _b_neg,      1, 1);
    register_fn("__num_pos", _b_pos,      1, 1);
    register_fn("PLS",       _b_pos,      1, 1);
    register_fn("INTEGER",  _INTEGER_,  1, 1);
    register_fn("REAL",     _REAL_,     1, 1);
    register_fn("SIZE",        _SIZE_,     1, 1);
    register_fn("IDENT",    _IDENT_,    0, 2);
    register_fn("DIFFER",   _DIFFER_,   0, 2);
    register_fn("VDIFFER",  _VDIFFER_,  0, 2);
    register_fn("NUMERIC",  _NUMERIC_,  1, 1);
    register_fn("NAME",     _NAME_,     1, 1);
    register_fn("LGT",      _LGT_,      2, 2);
    register_fn("LLT",      _LLT_,      2, 2);
    register_fn("LGE",      _LGE_,      2, 2);
    register_fn("LLE",      _LLE_,      2, 2);
    register_fn("LEQ",      _LEQ_,      2, 2);
    register_fn("LNE",      _LNE_,      2, 2);
    register_fn("HOST",     _HOST_,     1, 4);
    register_fn("ENDFILE",  _ENDFILE_,  1, 1);
    register_fn("APPLY",    _APPLY_,    1, 9);
    register_fn("LPAD",     _LPAD_,     2, 3);
    register_fn("RPAD",     _RPAD_,     2, 3);
    register_fn("CHAR",     _CHAR_,     1, 1);
    register_fn("DUPL",        _DUPL_,     2, 2);
    register_fn("REPLACE",  _REPLACE_,  3, 3);
    register_fn("REMDR",    _REMDR_,    2, 2);
    register_fn("ATAN",     _ATAN_,     1, 1);
    register_fn("CHOP",     _CHOP_,     1, 1);
    register_fn("COS",      _COS_,      1, 1);
    register_fn("EXP",      _EXP_,      1, 1);
    register_fn("LN",       _LN_,       1, 1);
    register_fn("SIN",      _SIN_,      1, 1);
    register_fn("SQRT",     _SQRT_,     1, 1);
    register_fn("TAN",      _TAN_,      1, 1);
    register_fn("TRIM",        _TRIM_,     1, 1);
    register_fn("SUBSTR",      _SUBSTR_,   2, 3);
    register_fn("REVERSE",  _REVERSE_,  1, 1);
    register_fn("DATATYPE", _DATATYPE_, 1, 1);
    register_fn("LCASE",    _LCASE_,    1, 1);
    register_fn("UCASE",    _UCASE__fn, 1, 1);
    register_fn("DATA",        _DATA_,     1, 1);
    register_fn("ARRAY",   _ARRAY_,   1, 2);
    register_fn("TABLE",   _TABLE_,   0, 2);
    register_fn("CONVERT", _CONVERT_, 2, 2);
    register_fn("PROTOTYPE", _PROTOTYPE_, 1, 1);
    register_fn("ITEM",    _ITEM_,    2, 9);
    register_fn("VALUE",   _VALUE_,   1, 1);
    register_fn("COPY",    _COPY_,    1, 1);
    register_fn("EVAL",  _EVAL_,  1, 1);
    register_fn("CODE",  _CODE_,  1, 1);
    register_fn("DEFINE", _DEFINE_, 1, 2);
    register_fn("FIELD",  _FIELD_,  2, 2);
    register_fn("OPSYN", _OPSYN_, 2, 3);
    register_fn("ARG",   _ARG_,   2, 2);
    register_fn("LOCAL", _LOCAL_, 2, 2);
    register_fn("SORT",  _SORT_,  1, 1);
    register_fn("INPUT",  _INPUT_,  1, 4);
    register_fn("OUTPUT", _OUTPUT_, 1, 4);
    register_fn("nPush",    _b_nPush,    0, 0);
    register_fn("nInc",     _b_nInc,     0, 0);
    register_fn("nDec",     _b_nDec,     0, 0);
    register_fn("nTop",     _b_nTop,     0, 0);
    register_fn("nPop",     _b_nPop,     0, 0);
    register_fn("n",        _b_tree_n,      1, 1);
    register_fn("t",        _b_tree_t,      1, 1);
    register_fn("v",        _b_tree_v,      1, 1);
    register_fn("c",        _b_tree_c,      1, 1);
    register_fn("DATE",     _DATE_,        0, 0);
    register_fn("TIME",     _TIME_,        0, 0);
    register_fn("DUMP",     _DUMP_,        0, 1);
    register_fn("TRACE",    _TRACE_,       1, 4);
    register_fn("STOPTR",   _STOPTR_,      1, 2);
    register_fn("LOAD",            _b_LOAD_stub,        1, 2);
    register_fn("MON_OPEN",        _b_MON_OPEN,         3, 3);
    register_fn("MON_PUT_S_VALUE", _b_MON_PUT_S_VALUE,  2, 2);
    register_fn("MON_PUT_I_VALUE", _b_MON_PUT_I_VALUE,  2, 2);
    register_fn("MON_PUT_R_VALUE", _b_MON_PUT_R_VALUE,  2, 2);
    register_fn("MON_PUT_O_VALUE", _b_MON_PUT_O_VALUE,  2, 2);
    register_fn("MON_PUT_S_RETURN",_b_MON_PUT_S_RETURN, 2, 2);
    register_fn("MON_PUT_I_RETURN",_b_MON_PUT_I_RETURN, 2, 2);
    register_fn("MON_PUT_R_RETURN",_b_MON_PUT_R_RETURN, 2, 2);
    register_fn("MON_PUT_O_RETURN",_b_MON_PUT_O_RETURN, 2, 2);
    register_fn("MON_PUT_CALL",    _b_MON_PUT_CALL,     1, 1);
    register_fn("MON_CLOSE",       _b_MON_CLOSE,        0, 0);
    register_fn("DATE",     _DATE_,        0, 0);
    register_fn("TIME",     _TIME_,        0, 0);
    register_fn("RSORT",    _RSORT_,       1, 1);
    register_fn("CLEAR",    _CLEAR_,       0, 0);
    register_fn("SETEXIT",  _SETEXIT_,     0, 1);
    register_fn("FUNCTION", _FUNCTION_,    1, 1);
    register_fn("LABEL",    _LABEL_,       1, 1);
    register_fn("COLLECT",  _COLLECT_,     0, 1);
    register_fn("SPAN",    _PAT_SPAN_,    1, 1);
    register_fn("BREAK",   _PAT_BREAK_,   1, 1);
    register_fn("BREAKX",  _PAT_BREAKX_,  1, 1);
    register_fn("ANY",     _PAT_ANY_,     1, 1);
    register_fn("NOTANY",  _PAT_NOTANY_,  1, 1);
    register_fn("LEN",     _PAT_LEN_,     1, 1);
    register_fn("POS",     _PAT_POS_,     1, 1);
    register_fn("RPOS",    _PAT_RPOS_,    1, 1);
    register_fn("TAB",     _PAT_TAB_,     1, 1);
    register_fn("RTAB",    _PAT_RTAB_,    1, 1);
    register_fn("ARB",     _PAT_ARB_,     0, 0);
    register_fn("REM",     _PAT_REM_,     0, 0);
    register_fn("FAIL",       _PAT_FAIL_,    0, 0);
    register_fn("ABORT",   _PAT_ABORT_,   0, 0);
    register_fn("SUCCEED", _PAT_SUCCEED_, 0, 0);
    register_fn("BAL",     _PAT_BAL_,     0, 0);
    register_fn("ARBNO",   _PAT_ARBNO_,   1, 1);
    register_fn("FENCE",   _PAT_FENCE_,   0, 1);
    register_fn("ALT",     _PAT_ALT_,     2, 2);
    register_fn("CONCAT",  _PAT_CONCAT_,  2, 2);
    NV_SET_fn("ARB",     pat_arb());
    NV_SET_fn("BAL",     pat_bal());
    NV_SET_fn("FENCE",   pat_fence());
    NV_SET_fn("ABORT",   pat_abort());
    NV_SET_fn("FAIL",    pat_fail());
    NV_SET_fn("REM",     pat_rem());
    NV_SET_fn("SUCCEED", pat_succeed());
    monitor_ready = 1;
    DEFDAT_fn("tree(t,v,n,c)");
    register_fn("c", _b_tree_c, 1, 1);
    register_fn("t", _b_tree_t, 1, 1);
    register_fn("v", _b_tree_v, 1, 1);
    register_fn("n", _b_tree_n, 1, 1);
    g_protected_pat_vars_armed = 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
char *c_VARVAL_fn(DESCR_t v) {
    char buf[64];
    switch (v.v) {
        case DT_SNUL:    return rt_ws_strdup_c("");
        case DT_S:     return v.s ? v.s : rt_ws_strdup_c("");
        case DT_I: {
            int64_t _x = v.i; int _p = (int)sizeof(buf);
            buf[--_p] = 0;
            if (_x == 0) buf[--_p] = '0';
            else { uint64_t _u = (_x < 0) ? (uint64_t)(-(_x + 1)) + 1u : (uint64_t)_x;
                   while (_u) { buf[--_p] = (char)('0' + (int)(_u % 10u)); _u /= 10u; }
                   if (_x < 0) buf[--_p] = '-'; }
            return rt_ws_strdup_c(buf + _p);
        }
        case DT_R: {
            /* SPITBOL standard representation via the one authority (string_ops.c). */
            extern const char *real_str(double r, char *b, int bufsz);
            real_str(v.r, buf, sizeof(buf));
            return rt_ws_strdup_c(buf);
        }
        case DT_DATA:
            return v.u ? rt_ws_strdup_c(v.u->type->name) : rt_ws_strdup_c("");
        case DT_P:
            return rt_ws_strdup_c("PATTERN");
        case DT_A: {
            if (!v.arr) return rt_ws_strdup_c("ARRAY");
            ARBLK_t *arr = v.arr;
            char buf[128];
            if (arr->ndim > 1) {
                int cols = arr->hi2 - arr->lo2 + 1;
                if (arr->proto_bare)
                    snprintf(buf, sizeof(buf), "ARRAY('%d,%d')",
                             arr->hi - arr->lo + 1, cols);
                else
                    snprintf(buf, sizeof(buf), "ARRAY('%d:%d,%d:%d')",
                             arr->lo, arr->hi, arr->lo2, arr->hi2);
            } else {
                if (arr->proto_bare)
                    snprintf(buf, sizeof(buf), "ARRAY('%d')", arr->hi);
                else
                    snprintf(buf, sizeof(buf), "ARRAY('%d:%d')", arr->lo, arr->hi);
            }
            return rt_ws_strdup_c(buf);
        }
        case DT_T: {
            if (!v.tbl) return rt_ws_strdup_c("TABLE");
            char buf[64];
            snprintf(buf, sizeof(buf), "TABLE(%d,%d)", v.tbl->init, v.tbl->inc);
            return rt_ws_strdup_c(buf);
        }
        case DT_N:
            if (v.slen == 2 && v.ptr) { extern DESCR_t rt_deref(DESCR_t); return VARVAL_fn(rt_deref(v)); }
            if (v.slen == 0 && v.s) return rt_ws_strdup_c(v.s);
            if (v.ptr) {
                const char *nm = NV_name_from_ptr((const DESCR_t *)v.ptr);
                if (nm) return rt_ws_strdup_c(nm);
                return VARVAL_fn(*(DESCR_t *)v.ptr);
            }
            return rt_ws_strdup_c("");
        case DT_K:
            if (v.s) return VARVAL_fn(NV_GET_fn(v.s));
            return rt_ws_strdup_c("");
        case DT_E:
            return rt_ws_strdup_c("EXPRESSION");
        case DT_C:
            return rt_ws_strdup_c("");
        default:
            return rt_ws_strdup_c("");
    }
}
#include <setjmp.h>
jmp_buf g_core_err_jmp;
int     g_core_err_active = 0;
int     g_core_err_stmt   = 0;
int     g_kw_ctx         = 0;
static const char *core_err_msgs[40] = {
     NULL,
     "Illegal data type",
     "Error in arithmetic operation",
     "Erroneous array or table reference",
     "Null string in illegal context",
     "Undefined function or operation",
     "Erroneous prototype",
     "Unknown keyword",
     "Variable not present where required",
     "Entry point of function not label",
     "Illegal argument to primitive function",
     "Reading error",
     "Illegal i/o unit",
     "Limit on defined data types exceeded",
     "Negative number in illegal context",
     "String overflow",
     "Overflow during pattern matching",
     "Error in SNOBOL4 system",
     "Return from level zero",
     "Failure during goto evaluation",
     "Insufficient storage to continue",
     "Stack overflow",
     "Limit on statement execution exceeded",
     "Object exceeds size limit",
     "Undefined or erroneous goto",
     "Incorrect number of arguments",
     "Limit on compilation errors exceeded",
     "Erroneous END statement",
     "Execution of statement with compilation error",
     "Erroneous INCLUDE statement",
     "Cannot open INCLUDE file",
     "Erroneous LINE statement",
     "Missing END statement",
     "Output error",
     "User interrupt",
     "Not in a SETEXIT handler",
     "Error in BLOCKS",
     "Too many warnings in BLOCKS",
     "Mystery error in BLOCKS",
     "Cannot CONTINUE from FATAL error",
};
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void core_runtime_error(int code, const char *msg) {
    if (!msg && code >= 1 && code <= 39)
        msg = core_err_msgs[code];
    { extern jmp_buf g_core_errjmp_stk[64]; extern int g_core_errjmp_n;
      extern long g_icn_errnumber; extern const char *g_icn_errtext; extern DESCR_t g_icn_errvalue; extern int g_icn_err_valid;
      extern long g_error;
      if (g_error != 0 && g_core_errjmp_n > 0) {
          if (g_error > 0) g_error--;
          extern void rt_kw_publish_error(int code, const char *msg);
          g_icn_errnumber = code; g_icn_errtext = msg ? msg : ""; memset(&g_icn_errvalue, 0, sizeof g_icn_errvalue); g_icn_err_valid = 1;
          rt_kw_publish_error(code, msg);
          longjmp(g_core_errjmp_stk[g_core_errjmp_n - 1], code);
      } }   /* ⭐⭐⭐ CN-EVAL-FAILS (this seat) -- THE CONVERSION TEST MOVED ABOVE THE REPORT AND ABOVE THE TWO exit(1)s, AND BOTH MOVES ARE LOAD-BEARING, NOT TIDYING. (1) ABOVE THE PRINT: a converted error is one somebody CAUGHT, and a caught error has no business on stderr -- the codebase already spells that rule in KW-5's &ERRLIMIT arm ("no message is displayed", manual Ch.16 verbatim) and the live oracle spells it too (EVAL('&NEVERSET') takes :F in total silence). Leaving the fprintf first made every EVAL failure print an error the oracle never prints, which is a .ref divergence on any witness that merely EXERCISES the path. (2) ABOVE core_err_is_terminal: error 22 "undefined function called" is ON that list, and the oracle FAILS `EVAL('UNDEFINEDFN(3)')` rather than dying -- so a boundary that catches behind the terminal exit cannot implement p.131 at all, because the most ordinary fragment error of the lot exits before the handler is consulted. ⛔ THE GATE IS NARROWED IN THE SAME BREATH, `g_error != 0` -> `g_error != 0 && g_core_errjmp_n > 0`, SO NOTHING ELSE MOVES: with no handler pushed the old code set the icn_err* state and fell to exit(1), which is unobservable, so every path that is not actively being caught still prints and still exits exactly as before -- terminal, fatal and ordinary alike. Only a frame that has BOTH armed g_error AND pushed a jmp_buf sees new behaviour, and today that is the EVAL boundary alone. ⛔ AND THE PUBLISH HAPPENS *HERE*, NOT AT THE CATCH SITE, FOR A LIFETIME REASON THAT COST A DEBUG CYCLE: nearly every caller builds its message in a STACK buffer (`char eb[192]; snprintf(eb, ...); core_runtime_error(342, eb)` is the tier-3 keyword arm's exact shape), and longjmp unwinds that frame -- so a handler that read g_icn_errtext AFTER landing was reading freed stack, and &ERRTEXT came back null. rt_kw_publish_error copies the text while msg is still live, one frame before the jump. g_icn_errtext keeps pointing at the caller's buffer exactly as it always has, because the Icon path consumes it before unwinding; this line does not change that contract, it just stops the SNOBOL4 path from depending on it. */
    fprintf(stderr, "\n** Error %d in statement %d\n   %s\n",
            code, g_core_err_stmt, msg ? msg : "");
    if (core_err_is_terminal(code)) exit(1);
    if (core_err_is_fatal(code))    exit(1);
    exit(1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
jmp_buf g_core_errjmp_stk[64]; int g_core_errjmp_n = 0;
long g_icn_errnumber = 0; const char *g_icn_errtext = ""; DESCR_t g_icn_errvalue; int g_icn_err_valid = 0;
static const char *icn_errmsg(int n) {
    switch (n) { case 101: return "integer expected or out of range"; case 102: return "numeric expected"; case 103: return "string expected"; case 104: return "cset expected";
                 case 106: return "procedure or integer expected"; case 107: return "record expected"; case 108: return "list expected"; case 110: return "file expected"; case 115: return "structure expected"; case 210: return "invalid tab stop"; }
    return "run-time error";
}
int core_icn_error(int code, DESCR_t val) {
    extern long g_error;
    if (g_error != 0) {
        if (g_error > 0) g_error--;
        g_icn_errnumber = code; g_icn_errtext = icn_errmsg(code); g_icn_errvalue = val; g_icn_err_valid = 1;
        if (g_core_errjmp_n > 0) longjmp(g_core_errjmp_stk[g_core_errjmp_n - 1], code);
        return 1;
    }
    fprintf(stderr, "\nRun-time error %d\n%s\n", code, icn_errmsg(code));
    exit(1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int64_t to_int_slow(DESCR_t v) {
    switch (v.v) {
        case DT_I:  return v.i;
        case DT_R: return (int64_t)v.r;
        case DT_S:
        case DT_SNUL: {
            const char *s = v.s ? v.s : "";
            while (*s == ' ') s++;
            if (!*s) return 0;
            return (int64_t)strtoll(s, NULL, 10);
        }
        case DT_K:
            return to_int(NV_GET_fn(v.s));
        default:
            core_runtime_error(1, NULL);
            return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
double to_real(DESCR_t v) {
    switch (v.v) {
        case DT_R: return v.r;
        case DT_I:  return (double)v.i;
        case DT_S:
        case DT_SNUL: {
            const char *s = v.s ? v.s : "";
            return strtod(s, NULL);
        }
        case DT_K:
            return to_real(NV_GET_fn(v.s));
        default:
            core_runtime_error(1, NULL);
            return 0.0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *datatype(DESCR_t v) {
    switch (v.v) {
        case DT_SNUL:    return "STRING";
        case DT_S:       return "STRING";
        case DT_I:       return "INTEGER";
        case DT_R:       return "REAL";
        case DT_DATA:    return v.u ? v.u->type->name : "DATA";
        case DT_P:       return "PATTERN";
        case DT_A:       return "ARRAY";
        case DT_T:       return "TABLE";
        case DT_C:       return "CODE";
        case DT_E:       return "EXPRESSION";
        case DT_N:       return "NAME";
        case DT_K:       return "NAME";
        default:         return "STRING";
    }
}
static DATBLK_t *_udef_types = NULL;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void DEFDAT_fn(const char *spec) {
    char *s = rt_ws_strdup(spec);
    char *paren = strchr(s, '(');
    if (!paren) return;
    *paren = '\0';
    char *name = s;
    char *fields_str = paren + 1;
    char *close = strchr(fields_str, ')');
    if (close) *close = '\0';
    DATBLK_t *t = rt_ws_alloc(sizeof(DATBLK_t));
    t->name = rt_ws_strdup(name);
    int nfields = 0;
    char *tmp = rt_ws_strdup(fields_str);
    char *tok = strtok(tmp, ",");
    while (tok) { nfields++; tok = strtok(NULL, ","); }
    t->nfields = nfields;
    t->fields  = rt_ws_alloc(nfields * sizeof(char *));
    tmp = rt_ws_strdup(fields_str);
    tok = strtok(tmp, ",");
    for (int i = 0; i < nfields && tok; i++) {
        while (*tok == ' ') tok++;
        char *end = tok + strlen(tok) - 1;
        while (end > tok && *end == ' ') *end-- = '\0';
        char *fld = rt_ws_strdup(tok);
        t->fields[i] = fld;
        tok = strtok(NULL, ",");
    }
    t->next    = _udef_types;
    t->serial_next = 1;
    _udef_types = t;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DATBLK_t *_udef_lookup(const char *name) {
    for (DATBLK_t *t = _udef_types; t; t = t->next)
        if (strcasecmp(t->name, name) == 0) return t;
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t DATCON_fn(const char *typename, ...) {
    DATBLK_t *t = _udef_lookup(typename);
    if (!t) return NULVCL;
    DATINST_t *u = rt_ws_alloc(sizeof(DATINST_t));
    u->type   = t;
    u->id     = t->serial_next++;
    u->fields = rt_ws_alloc(t->nfields * sizeof(DESCR_t));
    for (int i = 0; i < t->nfields; i++) u->fields[i] = NULVCL;
    va_list ap;
    va_start(ap, typename);
    for (int i = 0; i < t->nfields; i++) {
        DESCR_t v = va_arg(ap, DESCR_t);
        if (IS_NULL(v) && v.s == NULL) break;
        u->fields[i] = v;
    }
    va_end(ap);
    return (DESCR_t){ .v = DT_DATA, .u = u };
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t FIELD_GET_fn(DESCR_t obj, const char *field) {
    if (!IS_DATA(obj) || !obj.u) return NULVCL;
    DATBLK_t *t = obj.u->type;
    for (int i = 0; i < t->nfields; i++)
        if (strcasecmp(t->fields[i], field) == 0)
            return obj.u->fields[i];
    return NULVCL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void FIELD_SET_fn(DESCR_t obj, const char *field, DESCR_t val) {
    { extern void rt_sxt_break(const char *); if (val.v == DT_S) rt_sxt_break(val.s); }
    if (!IS_DATA(obj) || !obj.u) return;
    DATBLK_t *t = obj.u->type;
    for (int i = 0; i < t->nfields; i++)
        if (strcasecmp(t->fields[i], field) == 0) {
            obj.u->fields[i] = val;
            return;
        }
}
#define VAR_BUCKETS 512
typedef struct _VarEntry {
    char   *name;
    DESCR_t  val;
    DESCR_t *cell;
    int      is_gva;
    int      is_const;   /* SN4-CONSTANTS (s145, Lon's bit grant): 1 = sealed &constant cell -- one-time assignment, any later write = error 341 */
    struct _VarEntry *next;
} NV_t;
static NV_t *_var_buckets[VAR_BUCKETS];
static int _var_init_done = 0;

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void _var_init(void) {
    if (_var_init_done) return;
    memset(_var_buckets, 0, sizeof(_var_buckets));
    _var_init_done = 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static unsigned _var_hash(const char *name) {
    unsigned h = 5381;
    while (*name) h = h * 33 ^ (unsigned char)*name++;
    return h % VAR_BUCKETS;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static NV_t *_var_bucket_find(const char *name) {
    for (NV_t *e = _var_buckets[_var_hash(name)]; e; e = e->next)
        if (strcmp(e->name, name) == 0) return e;
    return (NV_t *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int _nv_kwsplit(void) { static int _k = -1; if (_k < 0) { const char *e = getenv("SCRIP_KWSPACE_SPLIT"); _k = (e && *e == '0') ? 0 : 1; } return _k; }   /* ⭐⭐⭐ CN-DOLLAR-ORACLE killswitch (queue row `cn-oracle-rulings`, HQ-61 "ORACLE-FAITHFUL CONFIRMED"). SCRIP_KWSPACE_SPLIT=0 restores the pre-ruling single-namespace regime EXACTLY -- every guard below is written as `is_const && _nv_kwsplit()` and every seal as `'&' && !_nv_kwsplit()`, so the OFF arm reads byte-for-byte like the code this ruling replaced and the revert probe can prove the flip is not vacuous. */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int _nv_ordinary(const NV_t *e) { return !(e->is_const && _nv_kwsplit()); }   /* ⭐⭐⭐ THE TWO NAMESPACES, MEASURED ON THE ORACLE AND ENFORCED IN ONE PREDICATE. Live `x64/bin/sbl -b`: `&ANCHOR = 1` then `$('&ANCHOR')` reads NULL, an indirect WRITE leaves the keyword at 1, `$('&NEVERSET') = 99` is accepted silently, and a second and third indirect write are accepted too (w1/w2/w3 = 1/2/3, DATATYPE INTEGER). So in SPITBOL `$('&X')` names an ORDINARY VARIABLE literally spelled "&X" in a namespace WHOLLY DISJOINT from the keyword &X, in BOTH directions. SCRIP kept ONE hash table for both, so a user-declared constant `&N` and the indirect variable `$('&N')` shared a cell: the read answered 42 where the oracle answers null, and the write raised 341 where the oracle assigns. `is_const` is ALREADY exactly the set of keyword-space (tier-3) cells -- NV_SET_fn seals on the leading '&' and nothing else sets the bit -- so it becomes the NAMESPACE TAG and no key mangling is needed: a mangled string key could always be collided with by `$()`, which names variables with ARBITRARY strings (beauty.sno:104 is literally `$'$' = ...`), whereas a per-entry tag cannot be. Two entries may now share a name in one bucket chain -- that IS the two cells -- so every ORDINARY walker filters through here and the keyword walkers (NV_KW_GET_fn/NV_KW_SET_fn/NV_CONST_ASSIGNED_fn) take the complement. */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t NV_GET_fn(const char *name) {
    _var_init();
    if (!name) return NULVCL;
    if (!g_call_fastpath_off && name[0] != '&' && (name[0] != 'I' || strcmp(name, "INPUT") != 0)) { NV_t *e = _var_bucket_find(name); if (e) return e->is_gva ? *e->cell : e->val; }
    if (strcmp(name, "INPUT") == 0) return input_read();
    _io_chan_setup();
    int ch = _io_chan_find_by_var(name);
    if (ch >= 0 && !_io_chan[ch].is_output && _io_chan[ch].fp) {
        ssize_t nread = getline(&_io_chan[ch].buf, &_io_chan[ch].cap, _io_chan[ch].fp);
        if (nread < 0) return FAILDESCR;
        if (nread > 0 && _io_chan[ch].buf[nread-1] == '\n') { _io_chan[ch].buf[nread-1] = '\0'; nread--; }
        if (kw_trim) {
            while (nread > 0 && (_io_chan[ch].buf[nread-1] == ' ' || _io_chan[ch].buf[nread-1] == '\t')) {
                _io_chan[ch].buf[--nread] = '\0';
            }
        }
        return STRVAL(rt_ws_strdup_c(_io_chan[ch].buf));
    }
    if (strcmp   (name, "&subject") == 0) {
        extern const char *scan_subj;
        return scan_subj ? STRVAL(scan_subj) : NULVCL;
    }
    if (strcmp   (name, "&pos") == 0) {
        extern int scan_pos;
        return INTVAL(scan_pos);
    }
    unsigned h = _var_hash(name);
    for (NV_t *e = _var_buckets[h]; e; e = e->next)
        if (strcmp(e->name, name) == 0 && _nv_ordinary(e)) return e->is_gva ? *e->cell : e->val;
    { extern DESCR_t rt_proc_value(const char *);
      if (name && (!strcmp(name, "write") || !strcmp(name, "writes"))) return rt_proc_value(name); }
    return NULVCL;
}
int g_protected_pat_vars_armed = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t NV_SET_fn(const char *name, DESCR_t val) {
    _var_init();
    { extern void rt_sxt_break(const char *); if (val.v == DT_S) rt_sxt_break(val.s); }
    if (g_protected_pat_vars_armed && name && is_protected_pat_name(name)) {
        core_runtime_error(42, NULL);
        return val;
    }
    if (!name) return val;
    if (!g_call_fastpath_off && name[0] != '&') { NV_t *e = _var_bucket_find(name); if (e) { if (e->is_gva) *e->cell = val; else e->val = val; if (g_comm_dbg != 0 || trace_set_n != 0 || monitor_fd >= 0) comm_var(name, val); return val; } }
    _io_chan_setup();
    int ch = _io_chan_find_by_var(name);
    if (ch >= 0 && _io_chan[ch].is_output && _io_chan[ch].fp) {
        char *s = VARVAL_fn(val);
        fprintf(_io_chan[ch].fp, "%s\n", s ? s : "");
        return val;
    }
    if (strcmp(name, "OUTPUT") == 0) { output_val(val); return val; }
    if (strcmp(name, "TERMINAL") == 0) {
        const char *s = IS_STR(val) ? val.s : "";
        fprintf(stderr, "%s\n", s);
        return val;
    }
    if (strcmp   (name, "&subject") == 0) {
        extern const char *scan_subj;
        const char *s = (val.v == DT_S) ? val.s : VARVAL_fn(val);
        scan_subj = s ? rt_ws_strdup_c(s) : ""; return val;
    }
    if (strcmp   (name, "&pos") == 0) {
        extern int scan_pos;
        scan_pos = (int)((val.v==DT_I) ? val.i : (int64_t)to_real(val)); return val;
    }
    if (g_kw_ctx) {
        static const char *known_kw[] = {
            "STLIMIT","ANCHOR","TRIM","FULLSCAN","CASE","MAXLNGTH",
            "FTRACE","ERRLIMIT","CODE","FNCLEVEL","RTNTYPE",
            "ALPHABET","UCASE","LCASE","DIGITS","PI","PARM",
            "STEXEC","STCOUNT","STNO","DUMP","ABEND",
            "TRACE","GTRACE","FATALLIMIT","ERRLIMIT","ERRTYPE","ERRTEXT",
            "INPUT","OUTPUT","TERMINAL","PUNCHAR",
            NULL
        };
        int found = 0;
        for (int _ki = 0; known_kw[_ki]; _ki++)
            if (strcmp(name, known_kw[_ki]) == 0) { found = 1; break; }
        if (!found) {
            core_runtime_error(7, NULL);
            return FAILDESCR;
        }
    }
    unsigned h = _var_hash(name);
    for (NV_t *e = _var_buckets[h]; e; e = e->next) {
        if (strcmp(e->name, name) == 0 && _nv_ordinary(e)) {
            if (e->is_const) { char eb[192]; snprintf(eb, sizeof eb, "re-assignment of a sealed &constant: %s", e->name); core_runtime_error(341, eb); return val; }
            if (e->is_gva) *e->cell = val; else e->val = val;
            if (name[0] == '&' && !_nv_kwsplit()) e->is_const = 1;   /* ⭐⭐⭐ CN-DOLLAR-ORACLE NARROWED THIS TO THE KILLSWITCH-OFF ARM, AND THAT IS THE HALF OF THE RULING THE READ SIDE CANNOT DO ALONE. Reached only through the ORDINARY namespace now (the walk above filters keyword-space entries out), so leaving it armed would re-seal the very cell `$('&X') = ...` just created and the SECOND indirect write would raise 341 -- measured on live sbl as w1/w2/w3 = 1/2/3, three accepted writes. The tier-3 seal did not move: it lives in NV_KW_SET_fn, which owns keyword space outright. Original CN-10 note kept below because its predicate lesson still holds. ⭐ SN4-CONSTANTS CN-10 -- SEAL AT THE FIRST REAL STORE, NOT AT ENTRY BIRTH. Reachable only for an `&`-entry born WITHOUT a value (NV_PTR_fn's intern, which no longer pre-seals -- see its create site): that entry's first store IS the one-time assignment, so it stores and seals here; the second store then meets is_const above and raises 341 exactly as a SET-created constant does. `is_const` is thereby made truthful -- 1 iff the one-time assignment has EXECUTED -- which is what lets NV_CONST_ASSIGNED_fn below answer the 342 read-side predicate (s148 item 3: NV_EXISTS_fn tested entry EXISTENCE, so a valueless intern suppressed 342 and pre-armed 341 against the first legitimate write). */
            comm_var(name, val);
            return val;   /* CN-10 drive-by, same region: this was a BARE `return;` in a DESCR_t-returning function -- any caller consuming the update-path result read garbage. All sibling paths already return val. */
        }
    }
    { static long _nvc = -1; if (_nvc < 0) { const char *ev = getenv("SCRIP_NV_TRACE"); _nvc = (ev && *ev && *ev != '0') ? 0 : -2; } if (_nvc >= 0) { _nvc++; fprintf(stderr, "[NVC] SET %ld new-var '%s' h=%u\n", _nvc, name, h); fflush(stderr); } }
    NV_t *e = rt_ws_alloc(sizeof(NV_t));
    e->name = rt_ws_strdup(name);
    e->val  = val;
    e->cell = (DESCR_t *)0;
    e->is_gva = 0;
    e->is_const = (name[0] == '&' && !_nv_kwsplit()) ? 1 : 0;   /* ⭐⭐⭐ CN-DOLLAR-ORACLE: THE ORDINARY CREATOR NO LONGER MINTS KEYWORD-SPACE CELLS. `$('&NEVERSET') = 99` arrives here and the oracle accepts it silently as an ordinary variable (measured; and reads back 99), so tagging it is_const would both hide it from every later ordinary read and refuse the next write with 341. Keyword space is minted in NV_KW_SET_fn and nowhere else -- which is what keeps `is_const` a truthful namespace tag. */
    e->next = _var_buckets[h];
    _var_buckets[h] = e;
    comm_var(name, val);
    return val;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int NV_EXISTS_fn(const char *name) { _var_init(); if (!name) return 0; unsigned h = _var_hash(name); for (NV_t *e = _var_buckets[h]; e; e = e->next) if (strcmp(e->name, name) == 0 && _nv_ordinary(e)) return 1; return 0; }   /* CN-DOLLAR-ORACLE: an ORDINARY-namespace existence test, filtered like every other ordinary walker so it cannot answer 1 for a keyword-space twin. */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int NV_CONST_ASSIGNED_fn(const char *name) { _var_init(); if (!name) return 0; unsigned h = _var_hash(name); for (NV_t *e = _var_buckets[h]; e; e = e->next) if (strcmp(e->name, name) == 0 && e->is_const) return 1; return 0; }   /* ⭐ CN-DOLLAR-ORACLE MADE THIS A KEYWORD-SPACE SCAN INSTEAD OF A FIRST-NAME-MATCH. Two entries may now share a name (the constant &N and the indirect variable $('&N')), and the bucket chain prepends, so `return e->is_const` on the first match would answer 0 the moment an indirect write to "&N" landed in front of the constant -- the 342 predicate would then say a declared constant was never assigned. Identical when only one entry exists, which is every pre-ruling program. SN4-CONSTANTS CN-10 -- the 342 read-side predicate: 1 iff the one-time assignment has EXECUTED (is_const is sealed at SET-create or at the first store into a PTR-interned cell, never at valueless birth). NV_EXISTS_fn answers a different question (entry existence) and keywords.c asking IT was s148 item 3. */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t NV_KW_GET_fn(const char *name) { _var_init(); if (!name) return NULVCL; if (!_nv_kwsplit()) return NV_GET_fn(name); unsigned h = _var_hash(name); for (NV_t *e = _var_buckets[h]; e; e = e->next) if (strcmp(e->name, name) == 0 && e->is_const) return e->is_gva ? *e->cell : e->val; return NULVCL; }   /* ⭐⭐⭐ THE KEYWORD-SPACE READER (CN-DOLLAR-ORACLE). rt_keyword_read_snobol4's tier-3 arms called NV_GET_fn, which is now the ORDINARY reader -- pointing them here is what keeps a declared constant readable while `$('&N')` reads the disjoint ordinary cell. Deliberately NOT routed through NV_GET_fn's preamble: that preamble is the ORDINARY namespace's (INPUT, the I/O-channel association scan, &subject/&pos, the write/writes proc values), and none of it can own a tier-3 name -- kw_read answers &subject/&pos long before the tier-3 arm is reached, so consulting it here would only add ways for keyword space to leak into ordinary state. The killswitch-OFF arm delegates to NV_GET_fn verbatim, which IS the pre-ruling call, so SCRIP_KWSPACE_SPLIT=0 is an exact revert and not an approximation. */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t NV_KW_SET_fn(const char *name, DESCR_t val) { _var_init(); if (!name) return val; if (!_nv_kwsplit()) return NV_SET_fn(name, val); { extern void rt_sxt_break(const char *); if (val.v == DT_S) rt_sxt_break(val.s); } unsigned h = _var_hash(name); for (NV_t *e = _var_buckets[h]; e; e = e->next) if (strcmp(e->name, name) == 0 && e->is_const) { char eb[192]; snprintf(eb, sizeof eb, "re-assignment of a sealed &constant: %s", e->name); core_runtime_error(341, eb); return val; } NV_t *e = rt_ws_alloc(sizeof(NV_t)); e->name = rt_ws_strdup(name); e->val = val; e->cell = (DESCR_t *)0; e->is_gva = 0; e->is_const = 1; e->next = _var_buckets[h]; _var_buckets[h] = e; comm_var(name, val); return val; }   /* ⭐⭐⭐ THE KEYWORD-SPACE WRITER, AND THE ONLY MINTER OF SEALED CELLS LEFT IN THE FILE (CN-DOLLAR-ORACLE). The one-time-assignment seal is UNCHANGED in force and in wording -- second tier-3 store still raises 341 with the same text -- it merely stopped being a property of the leading '&' in an ordinary store and became a property of the namespace the store came from, which is what the oracle measures: `$('&NEVERSET') = 99` is silent, `&C = "one"` then `&C = "two"` is not. It creates UNCONDITIONALLY rather than reusing an ordinary same-named entry, because reusing one would re-merge the two cells this ruling separates. It skips NV_SET_fn's I/O-association and OUTPUT/TERMINAL preamble for the same reason NV_KW_GET_fn skips its twin -- a tier-3 constant is not an ordinary variable and must not be able to fire an I/O association -- but keeps rt_sxt_break (the GC/string bookkeeping every store owes) and comm_var (tracing, which the sealed cell owes exactly as before). */
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t *NV_PTR_fn(const char *name) {
    _var_init();
    if (!name) return NULL;
    if (strcmp(name, "INPUT")  == 0) return NULL;
    if (strcmp(name, "OUTPUT") == 0) return NULL;
    if (strcmp(name, "STLIMIT")  == 0) return NULL;
    if (strcmp(name, "ANCHOR")   == 0) return NULL;
    if (strcmp(name, "TRIM")     == 0) return NULL;
    if (strcmp(name, "FULLSCAN") == 0) return NULL;
    if (strcmp(name, "CASE")     == 0) return NULL;
    if (strcmp(name, "MAXLNGTH") == 0) return NULL;
    if (strcmp(name, "FTRACE")   == 0) return NULL;
    if (strcmp(name, "TRACE")    == 0) return NULL;
    if (strcmp(name, "ERRLIMIT") == 0) return NULL;
    if (strcmp(name, "CODE")     == 0) return NULL;
    if (strcmp(name, "FNCLEVEL") == 0) return NULL;
    if (strcmp(name, "RTNTYPE")  == 0) return NULL;
    unsigned h = _var_hash(name);
    for (NV_t *e = _var_buckets[h]; e; e = e->next)
        if (strcmp(e->name, name) == 0 && _nv_ordinary(e)) return e->is_gva ? e->cell : &e->val;
    { static long _nvc = -1; if (_nvc < 0) { const char *ev = getenv("SCRIP_NV_TRACE"); _nvc = (ev && *ev && *ev != '0') ? 0 : -2; } if (_nvc >= 0) { _nvc++; fprintf(stderr, "[NVC] PTR %ld new-var '%s' h=%u\n", _nvc, name, h); fflush(stderr); } }
    NV_t *e = rt_ws_alloc(sizeof(NV_t));
    e->name = rt_ws_strdup(name);
    e->val  = NULVCL;
    e->cell = (DESCR_t *)0;
    e->is_gva = 0;
    e->is_const = 0;   /* ⭐ SN4-CONSTANTS CN-10 -- A POINTER INTERN IS NOT AN ASSIGNMENT. This creator mints a VALUELESS cell; pre-sealing it (the old `name[0]=='&'` arm, twin of NV_SET_fn's create) made is_const mean "an `&`-entry exists" rather than "the one-time assignment executed", which is precisely the wrong predicate s148 item 3 names: 342 suppressed (entry exists) AND 341 pre-armed (first legitimate write refused). No `&`-name reaches this function today (census 2026-08-19: mon_emit_value_bin excludes them, gva_collect refuses them, rt_cell_for serves call-plumbing locals), so this is prophylactic like the rc-recording -- the seal now lands at the first real store in NV_SET_fn's update path. */
    e->next = _var_buckets[h];
    _var_buckets[h] = e;
    return &e->val;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int NV_bind_gva(const char *name, DESCR_t *cell) {
    if (!name || !cell) return 0;
    DESCR_t *p = NV_PTR_fn(name);
    if (!p) return 0;
    *cell = *p;
    _var_init();
    unsigned h = _var_hash(name);
    for (NV_t *e = _var_buckets[h]; e; e = e->next)
        if (strcmp(e->name, name) == 0 && _nv_ordinary(e)) { e->cell = cell; e->is_gva = 1; return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *NV_name_from_ptr(const DESCR_t *ptr) {
    if (!ptr) return NULL;
    for (int i = 0; i < VAR_BUCKETS; i++)
        for (NV_t *e = _var_buckets[i]; e; e = e->next)
            if (&e->val == ptr) return e->name;
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void NV_CLEAR_fn(void) {
    _var_init();
    for (int _i = 0; _i < VAR_BUCKETS; _i++) {
        for (NV_t *_e = _var_buckets[_i]; _e; _e = _e->next)
            if (!_e->is_const && !is_protected_pat_name(_e->name)) { if (_e->is_gva) *_e->cell = NULVCL; else _e->val = NULVCL; }   /* ⭐⭐⭐ SN4-CONSTANTS CN-6 + CN-7 — CLEAR SKIPS WHAT IS PROTECTED AND STORES THROUGH THE BINDING THE ENTRY ALREADY CARRIES. Manual Ch.19 CLEAR: "the null string is assigned to all user variables in the system", and SPITBOL "differs from SNOBOL4 by not clearing protected variables, such as ARB". ORACLE-MEASURED, not inferred: DATATYPE(ARB) reads PATTERN both before and after CLEAR() while an ordinary variable goes null in the same program. CN-6 (is_const): a sealed &constant raises 341 on any write, so it is protected under SCRIP's own regime -- nulling it drove PATTERN->NULL while leaving the ENTRY alive, so 342 could not fire (NV_EXISTS_fn stays true) and 341 then refused every restore, leaving the cell permanently null AND permanently unwritable. CN-7 (is_gva): this was the ONLY writer in the file that ignored the GVA binding -- 2185/2227/2242/2302/2345 all ask `is_gva ? *cell : val`, CLEAR alone stored to `val` unconditionally, so it nulled a copy the compiled program never reads and every CLEAR() was silently a no-op for ordinary variables. Deliberately NOT routed through NV_SET_fn even though that is the store authority: an I/O association sets g_call_fastpath_off (2912/2940), which would send every entry down the slow path into _io_chan_find_by_var -- a bulk reset must not fire I/O associations or raise 42 on a protected name the way an ordinary assignment does. CN-8 DELETED _var_reg outright: its producer NV_REG_fn and consumer NV_SYNC_fn were both removed as GC-proven-dead by the 2026-06-15 sweep 1308f790, leaving three write-through loops and a GC root visit bounded by a counter nothing incremented -- a modes-1/2 name-to-DESCR* registry with no meaning in modes 3/4, where the compiled program reads GVA cells and NV entries directly. Deletion is identity by reachability, not by measurement. */
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int ASGNIC_fn(const char *kw_name, DESCR_t val) {
    if (!kw_name) return 0;
    int64_t iv = IS_INT(val) ? val.i : (int64_t)to_real(val);
    if (strcmp(kw_name, "STLIMIT")  == 0) { kw_stlimit  = iv; return 1; }
    if (strcmp(kw_name, "ANCHOR")   == 0) { kw_anchor   = iv; if (g_rtcc_on) rtccb[RTCC_SLOT_R8] = (uint64_t)(int64_t)iv; return 1; }
    if (strcmp(kw_name, "TRIM")     == 0) { kw_trim     = iv; return 1; }
    if (strcmp(kw_name, "FULLSCAN") == 0) { kw_fullscan = iv; return 1; }
    if (strcmp(kw_name, "CASE")     == 0) {
        (void)iv;
        core_runtime_error(10, "&CASE is read-only; SCRIP is case-sensitive only");
        return 1;
    }
    if (strcmp(kw_name, "MAXLNGTH") == 0) { kw_maxlngth = iv; return 1; }
    if (strcmp(kw_name, "FTRACE")   == 0) { kw_ftrace   = iv; return 1; }
    if (strcmp(kw_name, "ERRLIMIT") == 0) { kw_errlimit = iv; return 1; }
    if (strcmp(kw_name, "CODE")     == 0) { kw_code     = iv; return 1; }
    if (strcmp(kw_name, "STCOUNT")  == 0) return 1;
    if (strcmp(kw_name, "STNO")     == 0) return 1;
    if (strcmp(kw_name, "ALPHABET") == 0) return 1;
    if (strcmp(kw_name, "FNCLEVEL") == 0) return 1;
    if (strcmp(kw_name, "RTNTYPE")  == 0) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void var_dump(void) {
    fprintf(stderr, "[DUMP start]\n");
    for (int i = 0; i < VAR_BUCKETS; i++) {
        for (NV_t *e = _var_buckets[i]; e; e = e->next) {
            const char *tname;
            switch(e->val.v) {
                case 0: tname="NULL"; break;
                case 1: tname="STR"; break;
                case 2: tname="INT"; break;
                case 3: tname="REAL"; break;
                case 5: tname="PATTERN"; break;
                case 6: tname="DT_A"; break;
                case 7: tname="TABLE"; break;
                case 8: tname="DT_DATA"; break;
                case 9: tname="DT_FAIL"; break;
                default: tname="OTHER"; break;
            }
            if (IS_STR(e->val)) {
                const char *s = e->val.s ? e->val.s : "(null)";
                int len = (int)strlen(s);
                fprintf(stderr, "  %s = STR(%.*s)\n", e->name, len > 40 ? 40 : len, s);
            } else {
                fprintf(stderr, "  %s = %s\n", e->name, tname);
            }
        }
    }
    fprintf(stderr, "[DUMP end]\n");
}
#define NSTACK_MAX 256
static int64_t _nstack[NSTACK_MAX];
static int      _ntop = -1;
#define NHOME_MAX 256
static int _nhome[NHOME_MAX];
static int _nhome_top = -1;
int _nseq = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void NPUSH_fn(void) {
    if (_ntop < NSTACK_MAX - 1) {
        ++_ntop;
        _nstack[_ntop] = 0;
        if (_nhome_top < NHOME_MAX - 1) {
            _nhome[++_nhome_top] = _ntop;
        }
    }
    fprintf(stderr, "SEQ%04d NPUSH depth=%d top=%lld\n",
            ++_nseq, _ntop, (long long)(_ntop >= 0 ? _nstack[_ntop] : 0));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void NINC_fn(void) {
    if (_ntop >= 0) _nstack[_ntop]++;
    fprintf(stderr, "SEQ%04d NINC  depth=%d top=%lld\n",
            ++_nseq, _ntop, (long long)(_ntop >= 0 ? _nstack[_ntop] : 0));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void NDEC_fn(void) { if (_ntop >= 0) _nstack[_ntop]--; }
int64_t ntop(void) { return (_ntop >= 0) ? _nstack[_ntop] : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void NPOP_fn(void) {
    fprintf(stderr, "SEQ%04d NPOP  depth=%d top=%lld\n",
            ++_nseq, _ntop, (long long)(_ntop >= 0 ? _nstack[_ntop] : 0));
    if (_ntop >= 0) _ntop--;
}
#define VSTACK_MAX 1024
static DESCR_t _vstack[VSTACK_MAX];
static int    _vstop = -1;
#define FUNC_BUCKETS 128
typedef struct _FNCBLK_t {
    char   *name;
    char   *spec;
    char   *entry_label;
    FNCPTR_t fn;
    int     nparams;
    char  **params;
    int     nlocals;
    char  **locals;
    struct _FuncEntry *next;
} FNCBLK_t;
static FNCBLK_t *_func_buckets[FUNC_BUCKETS];
static int        _func_init_done = 0;
static unsigned _func_hash(const char *name);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void _func_init(void) {
    if (_func_init_done) return;
    memset(_func_buckets, 0, sizeof(_func_buckets));
    _func_init_done = 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static unsigned _func_hash(const char *name) {
    unsigned h = 5381;
    while (*name) h = h * 33 ^ (unsigned char)*name++;
    return h % FUNC_BUCKETS;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static FNCBLK_t *_parse_define_spec(const char *spec) {
    FNCBLK_t *fe = rt_ws_alloc(sizeof(FNCBLK_t));
    char *s = rt_ws_strdup(spec);
    fe->spec = rt_ws_strdup(spec);
    char *paren = strchr(s, '(');
    if (!paren) {
        char *comma = strchr(s, ',');
        if (comma) {
            *comma = '\0';
            fe->name = rt_ws_strdup(s);
            fe->entry_label = fe->name;
            char *lstr = rt_ws_strdup(comma + 1);
            int nl = 0;
            char *tok = strtok(lstr, ",");
            while (tok) { nl++; tok = strtok(NULL, ","); }
            fe->nlocals = nl;
            fe->locals  = rt_ws_alloc(nl * sizeof(char *));
            lstr = rt_ws_strdup(comma + 1);
            tok  = strtok(lstr, ",");
            for (int i = 0; i < nl && tok; i++) {
                while (*tok == ' ') tok++;
                fe->locals[i] = rt_ws_strdup(tok);
                tok = strtok(NULL, ",");
            }
        } else {
            fe->name = rt_ws_strdup(s);
            fe->entry_label = fe->name;
        }
        fe->nparams = 0;
        fe->params  = NULL;
        return fe;
    }
    *paren = '\0';
    fe->name = rt_ws_strdup(s);
    fe->entry_label = fe->name;
    char *close = strchr(paren + 1, ')');
    char *locals_str = NULL;
    if (close) {
        locals_str = close + 1;
        if (*locals_str == ',') locals_str++;
        *close = '\0';
    }
    char *pstr = rt_ws_strdup(paren + 1);
    int np = 0;
    if (*pstr) {
        char *tok = strtok(pstr, ",");
        while (tok) { np++; tok = strtok(NULL, ","); }
    }
    fe->nparams = np;
    fe->params  = np ? rt_ws_alloc(np * sizeof(char *)) : NULL;
    if (np) {
        pstr = rt_ws_strdup(paren + 1);
        char *tok = strtok(pstr, ",");
        for (int i = 0; i < np && tok; i++) {
            while (*tok == ' ') tok++;
            fe->params[i] = rt_ws_strdup(tok);
            tok = strtok(NULL, ",");
        }
    }
    int nl = 0;
    fe->nlocals = 0;
    fe->locals  = NULL;
    if (locals_str && *locals_str) {
        char *lstr = rt_ws_strdup(locals_str);
        char *tok  = strtok(lstr, ",");
        while (tok) { nl++; tok = strtok(NULL, ","); }
        fe->nlocals = nl;
        fe->locals  = rt_ws_alloc(nl * sizeof(char *));
        lstr = rt_ws_strdup(locals_str);
        tok  = strtok(lstr, ",");
        for (int i = 0; i < nl && tok; i++) {
            while (*tok == ' ') tok++;
            fe->locals[i] = rt_ws_strdup(tok);
            tok = strtok(NULL, ",");
        }
    }
    return fe;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void DEFINE_fn(const char *spec, FNCPTR_t fn) {
    _func_init();
    FNCBLK_t *fe = _parse_define_spec(spec);
    fe->fn = fn;
    unsigned h = _func_hash(fe->name);
    for (FNCBLK_t *e = _func_buckets[h]; e; e = e->next) {
        if (strcmp(e->name, fe->name) == 0) {
            e->spec    = fe->spec;
            e->fn      = fe->fn;
            e->nparams = fe->nparams;
            e->params  = fe->params;
            e->nlocals = fe->nlocals;
            e->locals  = fe->locals;
            return;
        }
    }
    fe->next = _func_buckets[h];
    _func_buckets[h] = fe;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void DEFINE_fn_entry(const char *spec, FNCPTR_t fn, const char *entry_label) {
    DEFINE_fn(spec, fn);
    if (!entry_label) return;
    _func_init();
    FNCBLK_t *fe = _parse_define_spec(spec);
    unsigned h = _func_hash(fe->name);
    for (FNCBLK_t *e = _func_buckets[h]; e; e = e->next) {
        if (strcmp(e->name, fe->name) == 0) {
            char *el = rt_ws_strdup(entry_label);
            e->entry_label = el;
            return;
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* OPSYN operator dispatch (s10): the by-name chain consults the core fn table for operator-symbol names
 * (non-identifier lead char) so an OPSYN-aliased operator like `@` reaches the aliased C builtin.  Returns
 * 0 when the name is absent or carries no C fn (SNOBOL-defined synonym targets are a follow-on). */
int core_call_registered_fn(const char *name, DESCR_t *args, int nargs, DESCR_t *out) {
    if (!name || !*name) return 0;
    _func_init();
    unsigned h = _func_hash(name);
    for (FNCBLK_t *e = _func_buckets[h]; e; e = e->next)
        if (strcmp(e->name, name) == 0) {
            if (!e->fn) return 0;
            *out = e->fn(args, nargs);
            return 1;
        }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void register_fn_alias(const char *newname, const char *oldname) {
    _func_init();
    char *nn = rt_ws_strdup(newname);
    char *on = rt_ws_strdup(oldname);
    newname = nn; oldname = on;
    FNCBLK_t *old_entry = NULL;
    unsigned ho = _func_hash(oldname);
    for (FNCBLK_t *e = _func_buckets[ho]; e; e = e->next) {
        if (strcmp(e->name, oldname) == 0) { old_entry = e; break; }
    }
    FNCBLK_t *fe = rt_ws_alloc(sizeof(FNCBLK_t));
    fe->name    = rt_ws_strdup(newname);
    if (old_entry) {
        fe->spec        = old_entry->spec;
        fe->entry_label = old_entry->entry_label;
        fe->fn          = old_entry->fn;
        fe->nparams = old_entry->nparams;
        fe->params  = old_entry->params;
        fe->nlocals = old_entry->nlocals;
        fe->locals  = old_entry->locals;
    } else {
        fe->spec = rt_ws_strdup(newname); fe->fn = NULL;
        fe->entry_label = fe->name;
        fe->nparams = 0; fe->params = NULL;
        fe->nlocals = 0; fe->locals = NULL;
    }
    fe->next = NULL;
    unsigned hn = _func_hash(newname);
    for (FNCBLK_t *e = _func_buckets[hn]; e; e = e->next) {
        if (strcmp(e->name, newname) == 0) {
            e->spec = fe->spec; e->fn = fe->fn;
            e->entry_label = fe->entry_label;
            e->nparams = fe->nparams; e->params = fe->params;
            e->nlocals = fe->nlocals; e->locals = fe->locals;
            return;
        }
    }
    fe->next = _func_buckets[hn];
    _func_buckets[hn] = fe;
}
DESCR_t (*g_user_call_hook)(const char *name, DESCR_t *args, int nargs) = NULL;
DESCR_t (*g_eval_pat_hook)(DESCR_t pat) = NULL;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t APPLY_fn(const char *name, DESCR_t *args, int nargs) {
    _func_init();
    if (!name) return NULVCL;
    unsigned h = _func_hash(name);
    for (FNCBLK_t *e = _func_buckets[h]; e; e = e->next) {
        if (strcmp(e->name, name) == 0) {
            if (e->fn) {
                return e->fn(args, nargs);
            }
            if (g_user_call_hook) return g_user_call_hook(name, args, nargs);
            return NULVCL;
        }
    }
    if (g_user_call_hook) {
        DESCR_t r = g_user_call_hook(name, args, nargs);
        if (!IS_FAIL_fn(r)) return r;
    }
    if (getenv("SCRIP_DEBUG_APPLY"))
        fprintf(stderr, "[apply-err5] unresolved '%s' (nargs=%d)\n", name ? name : "(null)", nargs);
    core_runtime_error(5, NULL);
    return FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _ARG_(DESCR_t *a, int n) {
    if (n < 2) return FAILDESCR;
    const char *fname = VARVAL_fn(a[0]);
    if (!fname) return FAILDESCR;
    int64_t idx = to_int(a[1]);
    _func_init();
    unsigned h = _func_hash(fname);
    for (FNCBLK_t *e = _func_buckets[h]; e; e = e->next) {
        if (strcmp(e->name, fname) == 0) {
            if (idx < 1 || idx > (int64_t)e->nparams) return FAILDESCR;
            return STRVAL(rt_ws_strdup_c(e->params[idx - 1]));
        }
    }
    /* Fallback: the compiled (mode-4) backend registers DEFINE'd functions in the
     * rt_proc registry (rt_proc_register carries the param-name table), NOT in
     * _func_buckets, which only the in-process (mode-3) runtime DEFINE fills. Consult
     * rt_proc so ARG introspection has the same result in both backends — same class
     * of two-registry parity gap as the proc/1 fix. Fixes 1017 mode-4 (SZ-7). */
    { extern int rt_proc_nparams(const char *); extern const char *rt_proc_pname(const char *, int);
      int np = rt_proc_nparams(fname);
      if (np > 0 && idx >= 1 && idx <= (int64_t)np) {
          const char *pn = rt_proc_pname(fname, (int)(idx - 1));
          if (pn) return STRVAL(rt_ws_strdup_c(pn));
      } }
    return FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _LOCAL_(DESCR_t *a, int n) {
    if (n < 2) return FAILDESCR;
    const char *fname = VARVAL_fn(a[0]);
    if (!fname) return FAILDESCR;
    int64_t idx = to_int(a[1]);
    _func_init();
    unsigned h = _func_hash(fname);
    for (FNCBLK_t *e = _func_buckets[h]; e; e = e->next) {
        if (strcmp(e->name, fname) == 0) {
            if (idx < 1 || idx > (int64_t)e->nlocals) return FAILDESCR;
            return STRVAL(rt_ws_strdup_c(e->locals[idx - 1]));
        }
    }
    return FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _DEFINE_(DESCR_t *a, int n) {
    if (n < 1) return FAILDESCR;
    const char *proto = VARVAL_fn(a[0]);
    if (!proto || !*proto) return FAILDESCR;
    const char *entry = (n >= 2) ? VARVAL_fn(a[1]) : NULL;
    if (entry && !*entry) entry = NULL;
    if (entry)
        DEFINE_fn_entry(proto, NULL, entry);
    else
        DEFINE_fn(proto, NULL);
    return NULVCL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _FIELD_(DESCR_t *a, int n) {
    if (n < 2) return FAILDESCR;
    const char *fname = VARVAL_fn(a[0]);
    if (!fname) return FAILDESCR;
    int64_t idx = to_int(a[1]);
    _func_init();
    unsigned h = _func_hash(fname);
    for (FNCBLK_t *e = _func_buckets[h]; e; e = e->next) {
        if (strcmp(e->name, fname) == 0) {
            if (idx < 1 || idx > (int64_t)e->nparams) return FAILDESCR;
            return STRVAL(rt_ws_strdup_c(e->params[idx - 1]));
        }
    }
    return FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int FNCEX_fn(const char *name) {
    _func_init();
    if (!name) return 0;
    unsigned h = _func_hash(name);
    for (FNCBLK_t *e = _func_buckets[h]; e; e = e->next)
        if (strcmp(e->name, name) == 0) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int FUNC_NPARAMS_fn(const char *fname) {
    _func_init();
    if (!fname) return 0;
    unsigned h = _func_hash(fname);
    for (FNCBLK_t *e = _func_buckets[h]; e; e = e->next)
        if (strcmp(e->name, fname) == 0) return e->nparams;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int FUNC_NLOCALS_fn(const char *fname) {
    _func_init();
    if (!fname) return 0;
    unsigned h = _func_hash(fname);
    for (FNCBLK_t *e = _func_buckets[h]; e; e = e->next)
        if (strcmp(e->name, fname) == 0) return e->nlocals;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *FUNC_PARAM_fn(const char *fname, int i) {
    _func_init();
    if (!fname) return NULL;
    unsigned h = _func_hash(fname);
    for (FNCBLK_t *e = _func_buckets[h]; e; e = e->next)
        if (strcmp(e->name, fname) == 0)
            return (i >= 0 && i < e->nparams) ? e->params[i] : NULL;
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *FUNC_LOCAL_fn(const char *fname, int i) {
    _func_init();
    if (!fname) return NULL;
    unsigned h = _func_hash(fname);
    for (FNCBLK_t *e = _func_buckets[h]; e; e = e->next)
        if (strcmp(e->name, fname) == 0)
            return (i >= 0 && i < e->nlocals) ? e->locals[i] : NULL;
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *FUNC_ENTRY_fn(const char *fname) {
    _func_init();
    if (!fname) return NULL;
    unsigned h = _func_hash(fname);
    for (FNCBLK_t *e = _func_buckets[h]; e; e = e->next)
        if (strcmp(e->name, fname) == 0)
            return e->entry_label ? e->entry_label : e->name;
    return NULL;
}
static FILE *_input_fp = NULL;
static char *_input_buf = NULL;
static size_t _input_cap = 0;
static long _input_rlen = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t input_read(void) {
    if (!_input_fp) _input_fp = stdin;
    if (_input_rlen > 0) {
        extern char *rt_str_alloc(long n);
        char *b = rt_str_alloc(_input_rlen);
        size_t got = fread(b, 1, (size_t)_input_rlen, _input_fp);
        if (got == 0) return FAILDESCR;
        b[got] = '\0';
        { DESCR_t r; r.v = DT_S; r.slen = (uint32_t)got; r.s = b; return r; }
    }
    ssize_t nread = getline(&_input_buf, &_input_cap, _input_fp);
    if (nread < 0) return FAILDESCR;
    if (nread > 0 && _input_buf[nread-1] == '\n') { _input_buf[nread-1] = '\0'; nread--; }
    if (kw_trim) {
        while (nread > 0 && (_input_buf[nread-1] == ' ' || _input_buf[nread-1] == '\t')) {
            _input_buf[--nread] = '\0';
        }
    }
    return STRVAL(rt_ws_strdup_c(_input_buf));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *_io_extract_fname(const char *opts_str, char *buf, size_t bufsz) {
    if (!opts_str || !opts_str[0]) return NULL;
    const char *bracket = strchr(opts_str, '[');
    if (bracket) {
        size_t len = (size_t)(bracket - opts_str);
        while (len > 0 && opts_str[len-1] == ' ') len--;
        if (len > 0 && len < bufsz) {
            memcpy(buf, opts_str, len);
            buf[len] = '\0';
            return buf;
        }
        return NULL;
    }
    return opts_str;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *_io_varname(DESCR_t d) {
    if (IS_STR(d)) return d.s;
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void _io_parse_opts(const char *spec, long *fd, long *rlen) {
    const char *p = spec ? strchr(spec, '[') : NULL;
    if (!p) return;
    for (p++; *p && *p != ']'; p++) {
        if (*p == '-' && (p[1] == 'f' || p[1] == 'r' || p[1] == 'q')) {
            char k = p[1]; char *e = NULL; long v = strtol(p + 2, &e, 10);
            if (e && e > p + 2) { if (k == 'f') *fd = v; else *rlen = v; p = e - 1; }
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _INPUT_(DESCR_t *a, int n) {
    _io_chan_setup();
    char fname_buf[4096];
    const char *fname = NULL;
    if (n >= 4) {
        fname = VARVAL_fn(a[3]);
    } else if (n >= 3) {
        fname = _io_extract_fname(VARVAL_fn(a[2]), fname_buf, sizeof(fname_buf));
    }
    int ch = (n >= 2 && IS_INT(a[1])) ? (int)a[1].i : -1;
    if (!fname || !fname[0]) {
        extern int dup(int);
        long fd = -1, rlen = 0;
        _io_parse_opts(n >= 3 ? VARVAL_fn(a[2]) : NULL, &fd, &rlen);
        if (_input_fp && _input_fp != stdin) fclose(_input_fp);
        _input_fp = stdin;
        if (fd >= 0) { FILE *nf = fdopen(dup((int)fd), "r"); if (!nf) return FAILDESCR; _input_fp = nf; }
        _input_rlen = rlen;
        return NULVCL;
    }
    FILE *f = fopen(fname, "r");
    if (!f) return FAILDESCR;
    if (ch >= 0 && ch < IO_CHAN_MAX) {
        _io_chan_close(ch);
        _io_chan[ch].fp = f;
        _io_chan[ch].is_output = 0;
        const char *vn = (n >= 1) ? _io_varname(a[0]) : NULL;
        _io_chan[ch].varname = vn ? rt_ws_strdup(vn) : NULL; if (vn) g_call_fastpath_off = 1;
    } else {
        if (_input_fp && _input_fp != stdin) fclose(_input_fp);
        _input_fp = f;
    }
    return NULVCL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t _OUTPUT_(DESCR_t *a, int n) {
    _io_chan_setup();
    char fname_buf[4096];
    const char *fname = NULL;
    if (n >= 4) {
        fname = VARVAL_fn(a[3]);
    } else if (n >= 3) {
        fname = _io_extract_fname(VARVAL_fn(a[2]), fname_buf, sizeof(fname_buf));
    } else if (n >= 1) {
        return NULVCL;
    }
    int ch = (n >= 2 && IS_INT(a[1])) ? (int)a[1].i : -1;
    if (!fname || !fname[0]) return FAILDESCR;
    FILE *f = fopen(fname, "w");
    if (!f) return FAILDESCR;
    if (ch >= 0 && ch < IO_CHAN_MAX) {
        _io_chan_close(ch);
        _io_chan[ch].fp = f;
        _io_chan[ch].is_output = 1;
        const char *vn = (n >= 1) ? _io_varname(a[0]) : NULL;
        _io_chan[ch].varname = vn ? rt_ws_strdup(vn) : NULL; if (vn) g_call_fastpath_off = 1;
    } else {
        fclose(f);
        return FAILDESCR;
    }
    return NULVCL;
}
int _x4_pending_parent_frame = -1;
int _command_pending_parent_frame = -1;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void core_gc_roots(void)
{
    extern void rt_gc_visit_descr(DESCR_t *d);
    for (int b = 0; b < VAR_BUCKETS; b++) for (NV_t *e = _var_buckets[b]; e; e = e->next) { rt_gc_visit_descr(&e->val); if (e->cell) rt_gc_visit_descr(e->cell); }
}
