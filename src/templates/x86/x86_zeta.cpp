#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include "templates/x86/x86_zeta.h"
#include "templates/x86/x86_zverify.h"
#include "emitter/emit_state_.h"
#include "templates/x86/x86_ops.h"
extern "C" const char * bb_kind_name(int op);
static const char * ZLOC(int o);
static const char * ZLOCD(int o);
static const char * ZLOC_B(int o);
static const char * ZOPFS(int off, int sp);
static const char * ZOPAN();
static const char * zone_ref(int rbp_off, int spine_base, int d, int w);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char * ZCELL(int w) { return x86_zref(w + _.op_zrel, 1); }
const char * ZCELLD(int w) { return x86_zref(w + _.op_zrel, 0); }
const char * ZRES(int w) { return _.op_xf_off != -1 ? RDQ("rbp", _.op_xf_off + w) : ZCELL(w); }
const char * ZRESD(int w) { return _.op_xf_off != -1 ? RDD("rbp", _.op_xf_off + w) : ZCELLD(w); }
const char * ZRES_SPINE(int w) { return ZCELL(w); }
static const char * ZLOC(int o) { return ZCELL(16 + o); }
static const char * ZLOCD(int o) { return ZCELLD(16 + o); }
static const char * ZLOC_B(int o) { return ZCELL(_.op_ztail + 16 + o); }
const char * ZOPQ(int k, int w) { int in = k >= 0 && k < _.op_zcap; if (in && _.op_zread_fr[k] >= 0) return x86_zop(_.op_zread_fr[k] + w, 1, 0);
    return (in && _.op_zread_xf[k] != -1) ? RDQ("rbp", _.op_zread_xf[k] + w) : x86_zref((in ? _.op_zread[k] : 0) + w, 1); }
static const char * ZOPFS(int off, int sp) {
    const char * a = x86_zop(off, 1, 0), * p = strrchr(a, '+');
    if (!strncmp(a, "qword ptr [rbp ", 15) || !p || p[1] != ' ') return a;
    return x86_strkeep(std::string(a, (size_t)(p - a)) + "+ " + std::to_string(atoi(p + 2) + sp) + "]");
}
const char * ZOPQS(int k, int sp, int w) { int in = k >= 0 && k < _.op_zcap; if (in && _.op_zread_fr[k] >= 0) return ZOPFS(_.op_zread_fr[k] + w, sp);
    return (in && _.op_zread_xf[k] != -1) ? RDQ("rbp", _.op_zread_xf[k] + w) : x86_zref((in ? _.op_zread[k] : 0) + sp + w, 1); }
const char * ZOPD(int k, int w) { int in = k >= 0 && k < _.op_zcap; if (in && _.op_zread_fr[k] >= 0) return x86_zop(_.op_zread_fr[k] + w, 0, 0);
    return (in && _.op_zread_xf[k] != -1) ? RDD("rbp", _.op_zread_xf[k] + w) : x86_zref((in ? _.op_zread[k] : 0) + w, 0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * ZOPAN() { if (_.op_a_node_kind < 0) return ""; static char b[8][48]; static int i; i = (i + 1) & 7; const char * n = bb_kind_name(_.op_a_node_kind);
    snprintf(b[i], 48, "%s", n ? n : ""); return b[i]; }
const char * ZRESN() { return "result"; }
const char * ZOPN(int k) { if (k < 0 || k >= _.op_zcap) return ""; int kk = _.op_zkind[k]; if (kk < 0 && k == 0) kk = _.op_a_node_kind; if (kk < 0) return ""; static char b[8][48];
    static int i; i = (i + 1) & 7; const char * n = bb_kind_name(kk); snprintf(b[i], 48, "%s", n ? n : ""); return b[i]; }
const char * HKN(int k) { static const char * const n[6] = { "old____", "outer_Σ", "outer_δ", "outer_Δ", "cap_gen", "rsp_mark" }; return (k >= 0 && k < 6) ? n[k] : ""; }
const char * XSAQ(int d) { return (_.op_zcap > 0 && _.op_zread_xf[0] != -1) ? RDQ("rbp", _.op_zread_xf[0] + d) : FRQ(_.op_sa + d); }
const char * XSAD(int d) { return (_.op_zcap > 0 && _.op_zread_xf[0] != -1) ? RDD("rbp", _.op_zread_xf[0] + d) : FR(_.op_sa + d); }
static const char * zone_ref(int rbp_off, int spine_base, int d, int w) { return (rbp_off != -1) ? ((w == 8) ? RDQ("rbp", rbp_off + d) : RDD("rbp", rbp_off + d)) :
    ((w == 8) ? FRQ(spine_base + d) : FR(spine_base + d)); }
