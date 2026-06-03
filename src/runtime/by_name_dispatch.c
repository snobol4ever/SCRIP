#include "builtins/script_builtins.h"
#include "builtins/gen_value.h"
#include "builtins/gen_runtime.h"
#include "../driver/interp_private.h"
#include "../parser/raku/raku_re.h"
#include "core.h"
#include "pattern_match.h"
#include "rt/rt.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <gc/gc.h>
int script_try_call_builtin(tree_t *call, DESCR_t *out_descr) {
    if (!call || call->n < 1 || !call->c[0]) return 0;
    const char *fn = call->c[0]->v.sval;
    if (!fn) return 0;
    int nargs = call->n - 1;
            if (!strcmp(fn,"str_substr") || (!strcmp(fn,"substr") && nargs >= 2)) {
                DESCR_t sd = interp_eval(call->c[1]);
                DESCR_t id = interp_eval(call->c[2]);
                const char *s = VARVAL_fn(sd); if (!s) s = "";
                long slen = (long)strlen(s);
                long start = IS_INT_fn(id) ? id.i : 0;
                if (start < 0) start = slen + start;
                if (start < 0) start = 0;
                if (start > slen) start = slen;
                long len = slen - start;
                if (nargs >= 3) {
                    DESCR_t ld = interp_eval(call->c[3]);
                    len = IS_INT_fn(ld) ? ld.i : len;
                    if (len < 0) len = 0;
                    if (start + len > slen) len = slen - start;
                }
                char *out = GC_malloc((size_t)len + 1);
                memcpy(out, s + start, (size_t)len); out[len] = '\0';
                { *out_descr = STRVAL(out); return 1; }
            }
            if (!strcmp(fn,"str_index") || (!strcmp(fn,"index") && nargs >= 2)) {
                DESCR_t sd = interp_eval(call->c[1]);
                DESCR_t nd = interp_eval(call->c[2]);
                const char *s = VARVAL_fn(sd); if (!s) s = "";
                const char *needle = VARVAL_fn(nd); if (!needle) needle = "";
                long from = 0;
                if (nargs >= 3) { DESCR_t pd = interp_eval(call->c[3]); from = IS_INT_fn(pd)?pd.i:0; }
                if (from < 0) from = 0;
                if (*needle == '\0') { *out_descr = INTVAL(from); return 1; }
                const char *found = strstr(s + from, needle);
                { *out_descr = found ? INTVAL((long)(found - s)) : INTVAL(-1); return 1; }
            }
            if (!strcmp(fn,"str_rindex") || (!strcmp(fn,"rindex") && nargs >= 2)) {
                DESCR_t sd = interp_eval(call->c[1]);
                DESCR_t nd = interp_eval(call->c[2]);
                const char *s = VARVAL_fn(sd); if (!s) s = "";
                const char *needle = VARVAL_fn(nd); if (!needle) needle = "";
                long slen = (long)strlen(s);
                long from = slen;
                if (nargs >= 3) { DESCR_t pd = interp_eval(call->c[3]); from = IS_INT_fn(pd)?pd.i:slen; }
                size_t nlen = strlen(needle);
                if (nlen == 0) { *out_descr = INTVAL(from < slen ? from : slen); return 1; }
                long best = -1;
                for (long i = 0; i <= from - (long)nlen; i++) {
                    if (memcmp(s + i, needle, nlen) == 0) best = i;
                }
                { *out_descr = INTVAL(best); return 1; }
            }
            if (!strcmp(fn,"re_match") && nargs == 2) {
                DESCR_t sd = interp_eval(call->c[1]);
                DESCR_t pd = interp_eval(call->c[2]);
                const char *subj = VARVAL_fn(sd); if (!subj) subj = "";
                if (pd.v == DT_P) {
                    extern int exec_stmt(const char *, DESCR_t *, DESCR_t, DESCR_t *, int);
                    { *out_descr = exec_stmt(NULL, &sd, pd, NULL, 0) ? INTVAL(1) : FAILDESCR; return 1; }
                }
                const char *pat = VARVAL_fn(pd); if (!pat) pat = "";
                { Raku_nfa *nfa = raku_nfa_build(pat);
                  if (!nfa) { *out_descr = FAILDESCR; return 1; }
                  extern int raku_nfa_bb_match(const Raku_nfa *, const char *);
                  static int nfa_bb = -1;
                  if (nfa_bb < 0) { const char *e = getenv("RK_NFA_BB"); nfa_bb = (e && e[0]=='1') ? 1 : 0; }
                  raku_nfa_exec(nfa, subj, &g_raku_match);
                  g_raku_subject = subj;
                  int verdict = g_raku_match.matched ? 1 : 0;
                  if (nfa_bb) verdict = raku_nfa_bb_match(nfa, subj);
                  raku_nfa_free(nfa);
                  { *out_descr = verdict ? INTVAL(1) : FAILDESCR; return 1; }
                }
            }
            if (!strcmp(fn,"re_match_global") && nargs == 2) {
                DESCR_t sd = interp_eval(call->c[1]);
                DESCR_t pd = interp_eval(call->c[2]);
                const char *subj = VARVAL_fn(sd); if (!subj) subj = "";
                const char *pat  = VARVAL_fn(pd); if (!pat)  pat  = "";
                Raku_nfa *nfa = raku_nfa_build(pat);
                if (!nfa) { *out_descr = STRVAL(GC_strdup("")); return 1; }
                int slen = (int)strlen(subj);
                char *out = GC_malloc(slen * 4 + 4); out[0] = '\0';
                int pos = 0, count = 0;
                while (pos <= slen) {
                    Raku_match m;
                    raku_nfa_exec(nfa, subj + pos, &m);
                    if (!m.matched) break;
                    int mlen = m.full_end - m.full_start;
                    if (count > 0) { int ol=strlen(out); out[ol]='\x01'; out[ol+1]='\0'; }
                    strncat(out, subj + pos + m.full_start, (size_t)mlen);
                    g_raku_match = m;
                    g_raku_match.full_start += pos;
                    g_raku_match.full_end   += pos;
                    for (int g=0;g<m.ngroups;g++) {
                        if (m.group_start[g]>=0) g_raku_match.group_start[g]+=pos;
                        if (m.group_end[g]>=0)   g_raku_match.group_end[g]+=pos;
                    }
                    g_raku_subject = subj;
                    pos += m.full_start + (mlen > 0 ? mlen : 1);
                    count++;
                }
                raku_nfa_free(nfa);
                { *out_descr = count > 0 ? STRVAL(out) : FAILDESCR; return 1; }
            }
            if (!strcmp(fn,"re_subst") && nargs == 2) {
                DESCR_t sd = interp_eval(call->c[1]);
                DESCR_t td = interp_eval(call->c[2]);
                const char *subj = VARVAL_fn(sd); if (!subj) subj = "";
                const char *tok  = VARVAL_fn(td); if (!tok)  tok  = "";
                const char *sep1 = strchr(tok, '\x01');
                if (!sep1) { *out_descr = sd; return 1; }
                const char *sep2 = strchr(sep1+1, '\x01');
                if (!sep2) { *out_descr = sd; return 1; }
                int plen = (int)(sep1-tok);
                int rlen = (int)(sep2-(sep1+1));
                char *pat  = GC_malloc(plen+1); memcpy(pat, tok, plen); pat[plen]='\0';
                char *repl = GC_malloc(rlen+1); memcpy(repl, sep1+1, rlen); repl[rlen]='\0';
                int global = (*(sep2+1)=='g');
                Raku_nfa *nfa = raku_nfa_build(pat);
                if (!nfa) { *out_descr = sd; return 1; }
                int slen=(int)strlen(subj);
                char *res = GC_malloc(slen*4+rlen*8+4); res[0]='\0';
                int pos=0, did_one=0;
                while (pos<=slen) {
                    Raku_match m; raku_nfa_exec(nfa, subj+pos, &m);
                    if (!m.matched) { strncat(res, subj+pos, (size_t)(slen-pos)); break; }
                    strncat(res, subj+pos, (size_t)m.full_start);
                    strcat(res, repl);
                    g_raku_match=m; g_raku_subject=subj;
                    int advance=m.full_start+(m.full_end-m.full_start>0?m.full_end-m.full_start:1);
                    pos+=advance; did_one=1;
                    if (!global) { strncat(res, subj+pos, (size_t)(slen-pos)); break; }
                }
                raku_nfa_free(nfa);
                if (call->c[1]->t==TERM_VAR && call->c[1]->v.ival>=0 &&
                    call->c[1]->v.ival<FRAME.env_n && frame_depth>0)
                    FRAME.env[call->c[1]->v.ival] = STRVAL(res);
                { *out_descr = did_one ? STRVAL(res) : sd; return 1; }
            }
            if (!strcmp(fn,"open") && (nargs==1||nargs==2)) {
                DESCR_t pd=interp_eval(call->c[1]);
                const char *path=VARVAL_fn(pd); if(!path||!*path) { *out_descr = FAILDESCR; return 1; }
                const char *mode="r";
                if(nargs==2){
                    DESCR_t md=interp_eval(call->c[2]);
                    const char *ms=VARVAL_fn(md); if(!ms) ms="";
                    if(strstr(ms,":w")||strstr(ms,"w")) mode="w";
                    else if(strstr(ms,":a")||strstr(ms,"a")) mode="a";
                }
                FILE *fp=fopen(path,mode);
                if(!fp) { *out_descr = FAILDESCR; return 1; }
                int idx=fh_alloc(fp);
                if(idx<0){fclose(fp);{ *out_descr = FAILDESCR; return 1; }}
                { *out_descr = INTVAL(idx); return 1; }
            }
            if (!strcmp(fn,"close") && nargs==1) {
                DESCR_t fd=interp_eval(call->c[1]);
                int idx=(int)(IS_INT_fn(fd)?fd.i:0);
                FILE *fp=fh_get(idx);
                if(fp){fclose(fp);fh_free(idx);}
                { *out_descr = INTVAL(0); return 1; }
            }
            if (!strcmp(fn,"slurp") && nargs==1) {
                DESCR_t ad=interp_eval(call->c[1]);
                FILE *fp=NULL; int need_close=0;
                if(IS_INT_fn(ad)) {
                    fp=fh_get((int)ad.i);
                } else {
                    const char *path=VARVAL_fn(ad); if(!path||!*path) { *out_descr = STRVAL(GC_strdup("")); return 1; }
                    fp=fopen(path,"r"); need_close=1;
                }
                if(!fp) { *out_descr = STRVAL(GC_strdup("")); return 1; }
                fseek(fp,0,SEEK_END); long sz=ftell(fp); rewind(fp);
                char *buf=GC_malloc(sz+1);
                size_t nr=fread(buf,1,(size_t)sz,fp); buf[nr]='\0';
                if(need_close) fclose(fp);
                { *out_descr = STRVAL(buf); return 1; }
            }
            if (!strcmp(fn,"lines") && nargs==1) {
                DESCR_t ad=interp_eval(call->c[1]);
                FILE *fp=NULL; int need_close=0;
                if(IS_INT_fn(ad)) {
                    fp=fh_get((int)ad.i);
                } else {
                    const char *path=VARVAL_fn(ad); if(!path||!*path) { *out_descr = STRVAL(GC_strdup("")); return 1; }
                    fp=fopen(path,"r"); need_close=1;
                }
                if(!fp) { *out_descr = STRVAL(GC_strdup("")); return 1; }
                char *out=GC_malloc(65536); out[0]='\0'; size_t cap=65536, used=0;
                char line[4096]; int first=1;
                while(fgets(line,sizeof line,fp)){
                    size_t ll=strlen(line);
                    while(ll>0&&(line[ll-1]=='\n'||line[ll-1]=='\r')) line[--ll]='\0';
                    size_t need=used+ll+2;
                    if(need>cap){cap=need*2;char*nb=GC_malloc(cap);memcpy(nb,out,used);out=nb;}
                    if(!first){out[used++]='\x01';}
                    memcpy(out+used,line,ll); used+=ll; out[used]='\0'; first=0;
                }
                if(need_close) fclose(fp);
                { *out_descr = STRVAL(out); return 1; }
            }
            if ((!strcmp(fn,"print_fh")||!strcmp(fn,"say_fh")) && nargs==2) {
                DESCR_t fd=interp_eval(call->c[1]);
                DESCR_t vd=interp_eval(call->c[2]);
                int idx=(int)(IS_INT_fn(fd)?fd.i:1);
                FILE *fp=fh_get(idx); if(!fp) fp=stdout;
                const char *s=VARVAL_fn(vd); if(!s) s="";
                fputs(s,fp);
                if(!strcmp(fn,"say_fh")) fputc('\n',fp);
                { *out_descr = INTVAL(0); return 1; }
            }
            if (!strcmp(fn,"spurt") && nargs==2) {
                DESCR_t pd=interp_eval(call->c[1]);
                DESCR_t cd=interp_eval(call->c[2]);
                const char *path=VARVAL_fn(pd); if(!path||!*path) { *out_descr = FAILDESCR; return 1; }
                const char *content=VARVAL_fn(cd); if(!content) content="";
                FILE *fp=fopen(path,"w"); if(!fp) { *out_descr = FAILDESCR; return 1; }
                fputs(content,fp); fclose(fp);
                { *out_descr = INTVAL(0); return 1; }
            }
            if (!strcmp(fn,"raku_nfa_compile") && nargs == 1) {
                DESCR_t pd = interp_eval(call->c[1]);
                const char *pat = VARVAL_fn(pd); if (!pat) pat = "";
                { Raku_nfa *nfa = raku_nfa_build(pat);
                  if (!nfa) { printf("NFA:%s:ERROR\n", pat); { *out_descr = INTVAL(0); return 1; } }
                  printf("NFA:%s:states=%d\n", pat, raku_nfa_state_count(nfa));
                  raku_nfa_free(nfa);
                }
                { *out_descr = INTVAL(0); return 1; }
            }
            if (!strcmp(fn,"re_named_capture") && nargs == 1) {
                DESCR_t nd = interp_eval(call->c[1]);
                const char *name = VARVAL_fn(nd); if (!name) name = "";
                if (!g_raku_match.matched) { *out_descr = STRVAL(GC_strdup("")); return 1; }
                int g = -1;
                for (int i=0;i<g_raku_match.ngroups;i++)
                    if (strcmp(g_raku_match.group_name[i],name)==0){g=i;break;}
                if (g<0||g_raku_match.group_start[g]<0) { *out_descr = STRVAL(GC_strdup("")); return 1; }
                int gs=g_raku_match.group_start[g], ge=g_raku_match.group_end[g];
                if (ge<gs) { *out_descr = STRVAL(GC_strdup("")); return 1; }
                int len=ge-gs; char *out=GC_malloc(len+1);
                memcpy(out,g_raku_subject+gs,(size_t)len); out[len]='\0';
                { *out_descr = STRVAL(out); return 1; }
            }
            if ((!strcmp(fn,"fh_capture") || !strcmp(fn,"re_capture")) && nargs == 1) {
                DESCR_t nd = interp_eval(call->c[1]);
                int n = (int)(IS_INT_fn(nd) ? nd.i : 0);
                if (!g_raku_match.matched || n < 0 || n >= g_raku_match.ngroups
                    || g_raku_match.group_start[n] < 0) { *out_descr = STRVAL(GC_strdup("")); return 1; }
                int gs = g_raku_match.group_start[n];
                int ge = g_raku_match.group_end[n];
                if (ge < gs) { *out_descr = STRVAL(GC_strdup("")); return 1; }
                int len = ge - gs;
                char *out = GC_malloc(len + 1);
                memcpy(out, g_raku_subject + gs, (size_t)len);
                out[len] = '\0';
                { *out_descr = STRVAL(out); return 1; }
            }
            if (!strcmp(fn,"str_uc") || (!strcmp(fn,"uc") && nargs == 1)) {
                DESCR_t sd = interp_eval(call->c[1]);
                const char *s = VARVAL_fn(sd); if (!s) s = "";
                char *out = GC_strdup(s);
                for (char *p = out; *p; p++) *p = (char)toupper((unsigned char)*p);
                { *out_descr = STRVAL(out); return 1; }
            }
            if (!strcmp(fn,"str_lc") || (!strcmp(fn,"lc") && nargs == 1)) {
                DESCR_t sd = interp_eval(call->c[1]);
                const char *s = VARVAL_fn(sd); if (!s) s = "";
                char *out = GC_strdup(s);
                for (char *p = out; *p; p++) *p = (char)tolower((unsigned char)*p);
                { *out_descr = STRVAL(out); return 1; }
            }
            if (!strcmp(fn,"str_trim")) {
                DESCR_t sd = interp_eval(call->c[1]);
                const char *s = VARVAL_fn(sd); if (!s) s = "";
                while (*s == ' ' || *s == '\t' || *s == '\n' || *s == '\r') s++;
                size_t len = strlen(s);
                while (len > 0 && (s[len-1]==' '||s[len-1]=='\t'||s[len-1]=='\n'||s[len-1]=='\r')) len--;
                char *out = GC_malloc(len + 1); memcpy(out, s, len); out[len] = '\0';
                { *out_descr = STRVAL(out); return 1; }
            }
            if (!strcmp(fn,"chars") || !strcmp(fn,"length")) {
                if (nargs == 1) {
                    DESCR_t sd = interp_eval(call->c[1]);
                    const char *s = VARVAL_fn(sd); if (!s) s = "";
                    { *out_descr = INTVAL((long)strlen(s)); return 1; }
                }
            }
            if (!strcmp(fn,"script_die") && nargs >= 1) {
                DESCR_t md = interp_eval(call->c[1]);
                const char *msg = VARVAL_fn(md); if (!msg) msg = "Died";
                extern char g_script_exception[512];
                snprintf(g_script_exception, sizeof g_script_exception, "%s", msg);
                { *out_descr = FAILDESCR; return 1; }
            }
            if (!strcmp(fn,"script_try") && (nargs == 1 || nargs == 2)) {
                extern char g_script_exception[512];
                g_script_exception[0] = '\0';
                DESCR_t r = interp_eval(call->c[1]);
                int body_failed = IS_FAIL_fn(r);
                int real_die    = (g_script_exception[0] != '\0');
                if (!body_failed) { g_script_exception[0]='\0'; { *out_descr = r; return 1; } }
                if (nargs == 2 && real_die) {
                    tree_t *catch_blk = call->c[2];
                    int _sl2 = -1;
                    tree_t *_stk2[64]; int _sn2=0; _stk2[_sn2++]=catch_blk;
                    while (_sn2>0 && _sl2<0) {
                        tree_t *_n=_stk2[--_sn2]; if(!_n) continue;
                        if(_n->t==TERM_VAR && _n->v.sval &&
                           (strcmp(_n->v.sval,"$!")==0||strcmp(_n->v.sval,"!")==0))
                            _sl2=(int)_n->v.ival;
                        for(int _ci=0;_ci<_n->n&&_sn2<62;_ci++) _stk2[_sn2++]=_n->c[_ci];
                    }
                    DESCR_t exc_d = STRVAL(GC_strdup(g_script_exception));
                    if (_sl2 >= 0 && _sl2 < FRAME.env_n) FRAME.env[_sl2] = exc_d;
                    else NV_SET_fn("$!", exc_d);
                    g_script_exception[0] = '\0';
                    { *out_descr = interp_eval(catch_blk); return 1; }
                }
                g_script_exception[0] = '\0';
                { *out_descr = NULVCL; return 1; }
            }
#define SOH '\x01'
            if (!strcmp(fn,"array_map") && nargs == 2) {
                tree_t *blk = call->c[1];
                DESCR_t arrd = interp_eval(call->c[2]);
                const char *as = VARVAL_fn(arrd); if (!as) as = "";
                char *out = GC_strdup("");
                const char *seg = as;
                int first = 1;
                do {
                    const char *nx = strchr(seg, SOH);
                    size_t elen = nx ? (size_t)(nx - seg) : strlen(seg);
                    char *elem = GC_malloc(elen + 1);
                    memcpy(elem, seg, elen); elem[elen] = '\0';
                    {
                      char *_ep_ev; long _iv_ev = strtol(elem, &_ep_ev, 10);
                      DESCR_t _ev = (*_ep_ev == '\0' && _ep_ev > elem) ? INTVAL(_iv_ev) : STRVAL(elem);
                      int _sl = -1;
                      tree_t *_stk[64]; int _sn=0; _stk[_sn++]=blk;
                      while (_sn>0 && _sl<0) {
                          tree_t *_n=_stk[--_sn];
                          if (!_n) continue;
                          if (_n->t==TERM_VAR && _n->v.sval) {
                              const char *_sv = _n->v.sval;
                              if (strcmp(_sv,"$_")==0 || strcmp(_sv,"_")==0)
                                  _sl=(int)_n->v.ival;
                          }
                          for(int _ci=0;_ci<_n->n&&_sn<62;_ci++) _stk[_sn++]=_n->c[_ci];
                      }
                      if (_sl >= 0 && _sl < FRAME.env_n) FRAME.env[_sl] = _ev;
                      else NV_SET_fn("$_", _ev); }
                    DESCR_t r = interp_eval(blk);
                    if (!IS_FAIL_fn(r)) {
                        const char *rv; char rb[64];
                        if (IS_INT_fn(r))       { snprintf(rb,sizeof rb,"%lld",(long long)r.i); rv=rb; }
                        else if (IS_REAL_fn(r)) { snprintf(rb,sizeof rb,"%g",r.r); rv=rb; }
                        else                    { rv = VARVAL_fn(r); if (!rv) rv = ""; }
                        size_t ol = strlen(out), rl = strlen(rv);
                        char *nout = GC_malloc(ol + rl + 2);
                        memcpy(nout, out, ol);
                        if (!first) { nout[ol] = SOH; memcpy(nout+ol+1, rv, rl); nout[ol+1+rl]='\0'; }
                        else        { memcpy(nout+ol, rv, rl); nout[ol+rl]='\0'; first=0; }
                        out = nout;
                    }
                    seg = nx ? nx + 1 : NULL;
                } while (seg);
                { *out_descr = STRVAL(out); return 1; }
            }
            if (!strcmp(fn,"array_grep") && nargs == 2) {
                tree_t *blk = call->c[1];
                DESCR_t arrd = interp_eval(call->c[2]);
                const char *as = VARVAL_fn(arrd); if (!as) as = "";
                char *out = GC_strdup("");
                const char *seg = as;
                int first = 1;
                do {
                    const char *nx = strchr(seg, SOH);
                    size_t elen = nx ? (size_t)(nx - seg) : strlen(seg);
                    char *elem = GC_malloc(elen + 1);
                    memcpy(elem, seg, elen); elem[elen] = '\0';
                    {
                      char *_ep_ev; long _iv_ev = strtol(elem, &_ep_ev, 10);
                      DESCR_t _ev = (*_ep_ev == '\0' && _ep_ev > elem) ? INTVAL(_iv_ev) : STRVAL(elem);
                      int _sl = -1;
                      tree_t *_stk[64]; int _sn=0; _stk[_sn++]=blk;
                      while (_sn>0 && _sl<0) {
                          tree_t *_n=_stk[--_sn];
                          if (!_n) continue;
                          if (_n->t==TERM_VAR && _n->v.sval) {
                              const char *_sv = _n->v.sval;
                              if (strcmp(_sv,"$_")==0 || strcmp(_sv,"_")==0)
                                  _sl=(int)_n->v.ival;
                          }
                          for(int _ci=0;_ci<_n->n&&_sn<62;_ci++) _stk[_sn++]=_n->c[_ci];
                      }
                      if (_sl >= 0 && _sl < FRAME.env_n) FRAME.env[_sl] = _ev;
                      else NV_SET_fn("$_", _ev); }
                    DESCR_t r = interp_eval(blk);
                    int truthy = !IS_FAIL_fn(r);
                    if (truthy) {
                        size_t ol = strlen(out), el = strlen(elem);
                        char *nout = GC_malloc(ol + el + 2);
                        memcpy(nout, out, ol);
                        if (!first) { nout[ol] = SOH; memcpy(nout+ol+1,elem,el); nout[ol+1+el]='\0'; }
                        else        { memcpy(nout+ol,elem,el); nout[ol+el]='\0'; first=0; }
                        out = nout;
                    }
                    seg = nx ? nx + 1 : NULL;
                } while (seg);
                { *out_descr = STRVAL(out); return 1; }
            }
            if (!strcmp(fn,"array_sort") && (nargs == 1 || nargs == 2)) {
                DESCR_t arrd = interp_eval(call->c[nargs == 2 ? 2 : 1]);
                const char *as = VARVAL_fn(arrd); if (!as || !*as) { *out_descr = STRVAL(GC_strdup("")); return 1; }
                tree_t *blk = (nargs == 2) ? call->c[1] : NULL;
                int cnt = 1; for (const char *p=as;*p;p++) if(*p==SOH) cnt++;
                char **elems = GC_malloc((size_t)cnt * sizeof(char*));
                int idx = 0; const char *seg = as;
                do {
                    const char *nx = strchr(seg, SOH);
                    size_t elen = nx ? (size_t)(nx-seg) : strlen(seg);
                    char *elem = GC_malloc(elen+1); memcpy(elem,seg,elen); elem[elen]='\0';
                    elems[idx++] = elem;
                    seg = nx ? nx+1 : NULL;
                } while (seg && idx < cnt);
                if (blk) {
                    for (int i=1;i<cnt;i++) {
                        char *key = elems[i]; int j=i-1;
                        while (j>=0) {
                            NV_SET_fn("$a", STRVAL(elems[j]));
                            NV_SET_fn("$b", STRVAL(key));
                            DESCR_t r = interp_eval(blk);
                            long cmp = IS_INT_fn(r) ? r.i : 0;
                            if (cmp <= 0) break;
                            elems[j+1]=elems[j]; j--;
                        }
                        elems[j+1]=key;
                    }
                } else {
                    int all_int = 1;
                    for (int i=0;i<cnt&&all_int;i++) {
                        char *ep; strtol(elems[i],&ep,10);
                        if (*ep) all_int=0;
                    }
                    if (all_int) {
                        for (int i=1;i<cnt;i++) {
                            char *key=elems[i]; long kv=atol(key); int j=i-1;
                            while (j>=0 && atol(elems[j])>kv) { elems[j+1]=elems[j]; j--; }
                            elems[j+1]=key;
                        }
                    } else {
                        for (int i=1;i<cnt;i++) {
                            char *key=elems[i]; int j=i-1;
                            while (j>=0 && strcmp(elems[j],key)>0) { elems[j+1]=elems[j]; j--; }
                            elems[j+1]=key;
                        }
                    }
                }
                size_t total=0; for(int i=0;i<cnt;i++) total+=strlen(elems[i])+1;
                char *out=GC_malloc(total+1); out[0]='\0';
                for (int i=0;i<cnt;i++) {
                    if (i) { size_t ol=strlen(out); out[ol]=SOH; out[ol+1]='\0'; }
                    strcat(out,elems[i]);
                }
                { *out_descr = STRVAL(out); return 1; }
            }
#undef SOH
            if (!strcmp(fn,"obj_new")) {
                if (call->n < 2) { *out_descr = NULVCL; return 1; }
                DESCR_t cnameD = interp_eval(call->c[1]);
                const char *cname = VARVAL_fn(cnameD);
                if (!cname || !*cname) { *out_descr = FAILDESCR; return 1; }
                DatType *t = dat_find_type(cname);
                if (!t) { *out_descr = FAILDESCR; return 1; }
                DESCR_t fvals[64];
                for (int i=0;i<t->nfields && i<64;i++) fvals[i]=NULVCL;
                for (int ci=2; ci+1 < call->n; ci+=2) {
                    DESCR_t kD = interp_eval(call->c[ci]);
                    DESCR_t vD = interp_eval(call->c[ci+1]);
                    const char *kname = VARVAL_fn(kD);
                    if (!kname) continue;
                    for (int fi=0;fi<t->nfields;fi++) {
                        if (strcmp(t->fields[fi], kname)==0) { fvals[fi]=vD; break; }
                    }
                }
                { *out_descr = dat_construct(t, fvals, t->nfields); return 1; }
            }
            if (!strcmp(fn,"meth_call")) {
                if (call->n < 3) { *out_descr = FAILDESCR; return 1; }
                DESCR_t obj    = interp_eval(call->c[1]);
                DESCR_t mnameD = interp_eval(call->c[2]);
                const char *mname = VARVAL_fn(mnameD);
                if (!mname || !*mname) { *out_descr = FAILDESCR; return 1; }
                const char *cname = NULL;
                if (obj.v == DT_DATA && obj.u) {
                    DATINST_t *inst = (DATINST_t *)obj.u;
                    if (inst->type) cname = inst->type->name;
                }
                if (!cname) { *out_descr = FAILDESCR; return 1; }
                char procname[256];
                snprintf(procname, sizeof procname, "%s__%s", cname, mname);
                int pi;
                for (pi = 0; pi < g_stage2.proc_count; pi++)
                    if (strcmp(g_stage2.proc_table[pi].name, procname) == 0) break;
                if (pi >= g_stage2.proc_count) { *out_descr = FAILDESCR; return 1; }
                int nextra = call->n - 3;
                int total  = 1 + nextra;
                DESCR_t *callargs = GC_malloc((size_t)total * sizeof(DESCR_t));
                callargs[0] = obj;
                for (int i=0;i<nextra;i++) callargs[i+1] = interp_eval(call->c[3+i]);
                { *out_descr = proc_table_call(pi, callargs, total); return 1; }
            }
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
int scan_try_call_builtin(tree_t *call, DESCR_t *args, int nargs, DESCR_t *out)
{
    if (!call || call->n < 1 || !call->c[0]) return 0;
    const char *fn = call->c[0]->v.sval;
    if (!fn) return 0;
    if (!strcmp(fn, "any") && nargs >= 1 && (scan_pos > 0 || nargs >= 2)) {
        const char *cv; int clen;
        if (!cset_resolve(args[0], &cv, &clen)) { *out = FAILDESCR; return 1; }
        const char *s; int p, slen, end;
        if (nargs >= 2) {
            s = VARVAL_fn(args[1]); if (!s) s = "";
            slen = (int)strlen(s);
            int i1 = (nargs >= 3) ? (int)args[2].i : (scan_pos > 0 ? scan_pos : 1);
            int i2 = (nargs >= 4) ? (int)args[3].i : slen + 1;
            if (i1 <= 0 || i1 > slen) { *out = FAILDESCR; return 1; }
            if (i2 <= 0) i2 = slen + 1;
            p = i1 - 1; end = i2 - 1;
        } else {
            s = scan_subj; if (!s) { *out = FAILDESCR; return 1; }
            slen = (int)strlen(s); p = scan_pos - 1; end = slen;
        }
        if (p < 0 || p >= slen || p >= end || !cset_has(cv, clen, (unsigned char)s[p])) { *out = FAILDESCR; return 1; }
        if (nargs < 2) { *out = INTVAL(p + 2); return 1; }
        *out = INTVAL(p + 2);
        return 1;
    }
    if (!strcmp(fn, "many") && nargs >= 1 && (scan_pos > 0 || nargs >= 2)) {
        const char *cv; int clen;
        if (!cset_resolve(args[0], &cv, &clen)) { *out = FAILDESCR; return 1; }
        const char *s; int p, slen, end;
        if (nargs >= 2) {
            s = VARVAL_fn(args[1]); if (!s) s = "";
            slen = (int)strlen(s);
            int i1 = (nargs >= 3) ? (int)args[2].i : (scan_pos > 0 ? scan_pos : 1);
            int i2 = (nargs >= 4) ? (int)args[3].i : slen + 1;
            if (i1 <= 0 || i1 > slen) { *out = FAILDESCR; return 1; }
            if (i2 <= 0) i2 = slen + 1;
            p = i1 - 1; end = i2 - 1;
        } else {
            s = scan_subj; if (!s) { *out = FAILDESCR; return 1; }
            slen = (int)strlen(s); p = scan_pos - 1; end = slen;
        }
        if (p < 0 || p >= slen || p >= end || !cset_has(cv, clen, (unsigned char)s[p])) { *out = FAILDESCR; return 1; }
        while (p < end && p < slen && cset_has(cv, clen, (unsigned char)s[p])) p++;
        *out = INTVAL(p + 1);
        return 1;
    }
    if (!strcmp(fn, "upto") && nargs >= 1 && (scan_pos > 0 || nargs >= 2)) {
        const char *cv; int clen;
        if (!cset_resolve(args[0], &cv, &clen)) { *out = FAILDESCR; return 1; }
        const char *s; int p, slen, end;
        if (nargs >= 2) {
            s = VARVAL_fn(args[1]); if (!s) s = "";
            slen = (int)strlen(s);
            int i1 = (nargs >= 3) ? (int)args[2].i : (scan_pos > 0 ? scan_pos : 1);
            int i2 = (nargs >= 4) ? (int)args[3].i : slen + 1;
            if (i1 <= 0) i1 = 1;
            if (i2 <= 0) i2 = slen + 1;
            p = i1 - 1; end = i2 - 1;
        } else {
            s = scan_subj; if (!s) { *out = FAILDESCR; return 1; }
            slen = (int)strlen(s); p = scan_pos - 1; end = slen;
        }
        while (p < end && p < slen && !cset_has(cv, clen, (unsigned char)s[p])) p++;
        if (p >= end || p >= slen) { *out = FAILDESCR; return 1; }
        *out = INTVAL(p + 1);
        return 1;
    }
    if (!strcmp(fn, "move") && nargs >= 1 && scan_pos > 0) {
        int n = (int)args[0].i;
        int newp = scan_pos + n;
        if (!scan_subj || newp < 1 || newp > (int)strlen(scan_subj) + 1) { *out = FAILDESCR; return 1; }
        int old = scan_pos; scan_pos = newp;
        size_t len = (size_t)(n >= 0 ? n : -n); int start = (n >= 0 ? old : newp);
        char *buf = GC_malloc(len + 1); memcpy(buf, scan_subj + start - 1, len); buf[len] = '\0';
        *out = STRVAL(buf);
        return 1;
    }
    if (!strcmp(fn, "tab") && nargs >= 1 && scan_pos > 0) {
        if (IS_FAIL_fn(args[0])) { *out = FAILDESCR; return 1; }
        int slen = scan_subj ? (int)strlen(scan_subj) : 0;
        int newp = (int)args[0].i;
        if (newp == 0) newp = slen + 1;
        else if (newp < 0) newp = slen + 1 + newp;
        if (!scan_subj || newp < scan_pos || newp < 1 || newp > slen + 1) { *out = FAILDESCR; return 1; }
        int old = scan_pos; scan_pos = newp; size_t len = (size_t)(newp - old);
        char *buf = GC_malloc(len + 1); memcpy(buf, scan_subj + old - 1, len); buf[len] = '\0';
        *out = STRVAL(buf);
        return 1;
    }
    if (!strcmp(fn, "pos") && nargs >= 1 && scan_pos > 0) {
        if (IS_FAIL_fn(args[0])) { *out = FAILDESCR; return 1; }
        int slen = scan_subj ? (int)strlen(scan_subj) : 0;
        int p = (int)args[0].i;
        if (p == 0) p = slen + 1;
        else if (p < 0) p = slen + 1 + p;
        if (p < 1 || p > slen + 1) { *out = FAILDESCR; return 1; }
        *out = (scan_pos == p) ? INTVAL(scan_pos) : FAILDESCR;
        return 1;
    }
    if (!strcmp(fn, "rpos") && nargs >= 1 && scan_pos > 0) {
        if (IS_FAIL_fn(args[0])) { *out = FAILDESCR; return 1; }
        int slen = scan_subj ? (int)strlen(scan_subj) : 0;
        int p = slen + 1 - (int)args[0].i;
        if (p < 1 || p > slen + 1) { *out = FAILDESCR; return 1; }
        *out = (scan_pos == p) ? INTVAL(scan_pos) : FAILDESCR;
        return 1;
    }
    if (!strcmp(fn, "match") && nargs >= 1 && scan_pos > 0) {
        const char *needle = VARVAL_fn(args[0]);
        const char *hay = scan_subj ? scan_subj : "";
        if (!needle) { *out = FAILDESCR; return 1; }
        int p = scan_pos - 1, nl = (int)strlen(needle);
        if (strncmp(hay + p, needle, nl) != 0) { *out = FAILDESCR; return 1; }
        scan_pos += nl;
        *out = INTVAL(scan_pos);
        return 1;
    }
    if (!strcmp(fn, "bal") && nargs >= 1) {
        const char *c1; int c1len;
        if (!cset_resolve(args[0], &c1, &c1len)) { *out = FAILDESCR; return 1; }
        const char *c2 = "(", *c3 = ")";
        int c2len = 1, c3len = 1;
        if (nargs >= 2) {
            const char *v; int vlen;
            if (cset_resolve(args[1], &v, &vlen) && vlen > 0) { c2 = v; c2len = vlen; }
        }
        if (nargs >= 3) {
            const char *v; int vlen;
            if (cset_resolve(args[2], &v, &vlen) && vlen > 0) { c3 = v; c3len = vlen; }
        }
        const char *s; int slen, p, end;
        if (nargs >= 4) {
            s = VARVAL_fn(args[3]); if (!s) s = "";
            slen = (int)strlen(s);
            int i1 = (nargs >= 5) ? (int)args[4].i : 1;
            int i2 = (nargs >= 6) ? (int)args[5].i : slen + 1;
            if (i1 <= 0) i1 = 1; if (i2 <= 0) i2 = slen + 1;
            p = i1 - 1; end = i2 - 1;
        } else {
            s = scan_subj; if (!s) { *out = FAILDESCR; return 1; }
            slen = (int)strlen(s); p = scan_pos - 1; end = slen;
        }
        int depth = 0;
        while (p < end && p < slen) {
            unsigned char ch = (unsigned char)s[p];
            if (cset_has(c2, c2len, ch)) depth++;
            else if (cset_has(c3, c3len, ch) && depth > 0) depth--;
            else if (depth == 0 && cset_has(c1, c1len, ch)) { *out = INTVAL(p + 1); return 1; }
            p++;
        }
        *out = FAILDESCR;
        return 1;
    }
    if (!strcmp(fn, "find") && nargs >= 2) {
        long pos1; if (frame_lookup(call, &pos1)) { *out = INTVAL(pos1); return 1; }
        const char *needle = VARVAL_fn(args[0]);
        const char *hay = VARVAL_fn(args[1]);
        if (!needle || !hay) { *out = FAILDESCR; return 1; }
        char *p = strstr(hay, needle);
        *out = p ? INTVAL((long long)(p - hay) + 1) : FAILDESCR;
        return 1;
    }
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int builtin_is_generator(const char *name)
{
    if (!name) return 0;
    return !strcmp(name, "find") || !strcmp(name, "upto") || !strcmp(name, "any")
        || !strcmp(name, "many") || !strcmp(name, "bal") || !strcmp(name, "key")
        || !strcmp(name, "seq");
}
/*--------------------------------------------------------------------------------------------------------------------*/
int rt_builtin_is_known(const char *name)
{
    if (!name) return 0;
    if (rt_proc_is_registered(name)) return 0;
    if (builtin_is_generator(name))  return 0;
    static const char *known[] = {
        "write", "writes", "stop",
        "integer", "real", "string", "numeric", "char", "ord", "cset",
        "type", "image", "proc", "args", "copy",
        "abs", "sqrt", "sin", "cos", "tan", "exp", "log",
        "max", "min",
        "trim", "reverse", "repl", "map", "left", "center", "right",
        "table", "list", "set", "sort", "sortf", "get", "pop", "pull",
        "member", "insert", "delete", "key",
        "MAKELIST",
        NULL
    };
    for (int i = 0; known[i]; i++) if (!strcmp(known[i], name)) return 1;
    {
        if (dat_find_type(name)) return 1;
    }
    return 0;
}
