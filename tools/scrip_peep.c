/* scrip_peep -- a STANDALONE .s -> .s peephole for mode-4 output (Lon s262: "we make a stand-alone executable to convert S -> S").
   ⛔ WHY STANDALONE AND NOT A PASS IN THE EMITTER.  CLAUDE.md makes m3 == m4 output a design INVARIANT: modes 3 and 4 share one codegen, the BINARY medium and the TEXT medium must agree instruction for instruction.  A peephole living inside the emitter would have to be medium-blind to keep that, which is a much larger job than finding out whether a peephole is worth building at all.  A separate binary that rewrites a .s AFTER emission leaves the invariant untouched -- the compiler still emits identical code in both media, and the pass is something you OPT INTO on a benchmark.  It is also trivially A/B-able, which is what makes it a measurement instrument rather than a leap of faith.
   ⛔ WHAT IT IS NOT: it is not on any build path, no gate runs it, and RULES.md's ".s artifacts are honest current compiler output, never pinned goldens" is unaffected because nothing regenerates artifacts through it.
   ⭐ WHAT THE RULES WERE CHOSEN FROM -- measured, not guessed.  roman.sno at -O0, callgrind, every EXECUTED emitted instruction categorised by role (32,146,227 Ir = 29.5% of the program at N=6000):
       branch 18.9% | mov-other 15.3% | compare 11.3% | arith 8.0% | reg<-imm 7.7% | reg<-reg 7.3%
       frame reload 6.9% | lea 6.6% | frame spill 5.5% | global-via-rip 5.4% | GOT load 2.9% | push/pop 2.7%
     The two peephole-reachable peaks inside that: the hottest reg<-imm pair is `mov r11, 44` / `mov r11, 45` at 777,656 Ir -- the DIAG node-id stamp, pure crash-dump telemetry -- and the hottest GOT loads are rtccb@GOTPCREL (512,828) and rt_anchor_g@GOTPCREL (401,228), reloaded on every trip of the unanchored retry loop.  Frame spill+reload is 12.4% of emitted code but that is REGISTER ALLOCATION, which a peephole cannot fix and which this tool deliberately does not pretend to.
   BUILD: gcc -O2 -o scrip-peep tools/scrip_peep.c
   USE:   scrip-peep [--stats] [--no-RULE ...] [--diag] in.s -o out.s        (stdin/stdout when no file given) */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAXL 262144
#define LSZ  1024
static char  g_raw[MAXL][LSZ];
static char  g_lab[MAXL][160];
static char  g_ins[MAXL][LSZ];
static int   g_fused[MAXL];
static int   g_dead[MAXL];
static char* g_splice[MAXL];
static int   g_n;
static long  g_hit[8];
static const char * g_rule[8] = { "got-cse", "store-reload", "dead-jmp", "self-mov", "diag-stamp", "inline-single-use", 0, 0 };
static int   g_on[8] = { 1, 1, 1, 1, 0, 1, 0, 0 };
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void trimr(char * s) { size_t n = strlen(s); while (n && (s[n-1] == '\n' || s[n-1] == '\r' || s[n-1] == ' ' || s[n-1] == '\t')) s[--n] = 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Strip the trailing `# ...` the emitter uses for provenance notes.  It never carries semantics -- x86("note",...) and x86("comment",...) both land here -- so ignoring it for matching is safe, and the ORIGINAL line is what gets printed, so notes survive into the output untouched. */
static void body_of(const char * ins, char * out, size_t cap) { size_t j = 0; for (size_t i = 0; ins[i] && j + 1 < cap; i++) { if (ins[i] == '#') break; out[j++] = ins[i]; } while (j && (out[j-1] == ' ' || out[j-1] == '\t')) j--; out[j] = 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void canon(const char * in, char * out, size_t cap) { size_t j = 0; int sp = 0; for (size_t i = 0; in[i] && j + 1 < cap; i++) { char c = in[i]; if (c == ' ' || c == '\t') { sp = 1; continue; } if (sp && j) out[j++] = ' '; sp = 0; if (j + 1 < cap) out[j++] = c; } out[j] = 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void mnem_of(const char * c, char * out, size_t cap) { size_t j = 0; for (size_t i = 0; c[i] && c[i] != ' ' && j + 1 < cap; i++) out[j++] = c[i]; out[j] = 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Two operands, comma-split at depth 0 (an operand may itself contain a comma-free bracket expression; the emitter never nests commas inside [] so depth tracking on [] is enough). */
static int ops2(const char * c, char * a, size_t ca, char * b, size_t cb) { const char * p = strchr(c, ' '); if (!p) return 0; p++; int d = 0; const char * comma = 0; for (const char * q = p; *q; q++) { if (*q == '[') d++; else if (*q == ']') d--; else if (*q == ',' && d == 0) { comma = q; break; } } if (!comma) return 0; size_t la = (size_t)(comma - p); if (la >= ca) return 0; memcpy(a, p, la); a[la] = 0; while (la && (a[la-1] == ' ')) a[--la] = 0; const char * s = comma + 1; while (*s == ' ') s++; if (strlen(s) >= cb) return 0; strcpy(b, s); return 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* A basic-block boundary.  Deliberately BLUNT: a label definition, any branch or call, any directive, and any fused line (which hides a branch in its second half).  Every rule below only ever looks WITHIN one block, so being blunt here costs optimisation opportunities and cannot cost correctness. */
static int is_barrier(int i) { if (g_lab[i][0]) return 1; if (g_fused[i]) return 1; char c[LSZ], m[32]; canon(g_ins[i], c, sizeof c); if (!c[0]) return 0; if (c[0] == '.') return 1; mnem_of(c, m, sizeof m); if (m[0] == 'j' || !strcmp(m, "call") || !strcmp(m, "ret") || !strcmp(m, "leave")) return 1; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* Does instruction i WRITE register r?  Conservative: any mnemonic whose first operand is exactly r, plus pop.  Anything it cannot parse is reported as a write, so an unknown instruction ends the run rather than being assumed harmless. */
static int writes_reg(int i, const char * r) { char c[LSZ], m[32], a[256], b[256]; canon(g_ins[i], c, sizeof c); if (!c[0]) return 0; mnem_of(c, m, sizeof m); if (!strcmp(m, "pop")) return 1; if (!ops2(c, a, sizeof a, b, sizeof b)) { if (!strcmp(m, "cqo") || !strcmp(m, "nop")) return 0; return 1; } return strcmp(a, r) == 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int main(int argc, char ** argv) {
    const char * inp = 0, * outp = 0; int stats = 0;
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--stats")) stats = 1;
        else if (!strcmp(argv[i], "--diag")) g_on[4] = 1;
        else if (!strncmp(argv[i], "--no-", 5)) { for (int k = 0; k < 8; k++) if (g_rule[k] && !strcmp(argv[i] + 5, g_rule[k])) g_on[k] = 0; }
        else if (!strcmp(argv[i], "-o") && i + 1 < argc) outp = argv[++i];
        else if (argv[i][0] != '-') inp = argv[i];
        else { fprintf(stderr, "scrip-peep: unknown option %s\n", argv[i]); return 2; }
    }
    FILE * fi = inp ? fopen(inp, "r") : stdin;
    if (!fi) { fprintf(stderr, "scrip-peep: cannot open %s\n", inp); return 1; }
    while (g_n < MAXL && fgets(g_raw[g_n], LSZ, fi)) {
        char t[LSZ]; strcpy(t, g_raw[g_n]); trimr(t);
        char * p = t; while (*p == ' ' || *p == '\t') p++;
        g_lab[g_n][0] = 0; g_ins[g_n][0] = 0; g_fused[g_n] = strchr(p, ';') ? 1 : 0;
        if (*p && *p != '#') {
            char * col = 0; for (char * q = p; *q; q++) { if (*q == ':') { col = q; break; } if (!(isalnum((unsigned char)*q) || *q == '_' || *q == '.' || *q == '$')) break; }
            if (col) { size_t nl = (size_t)(col - p); if (nl < sizeof g_lab[0]) { memcpy(g_lab[g_n], p, nl); g_lab[g_n][nl] = 0; } p = col + 1; while (*p == ' ' || *p == '\t') p++; }
            if (strlen(p) < LSZ) strcpy(g_ins[g_n], p);
        }
        g_n++;
    }
    if (inp) fclose(fi);
    for (int i = 0; i < g_n; i++) {
        if (g_dead[i] || !g_ins[i][0] || g_fused[i]) continue;
        char c[LSZ], m[32], a[256], b[256], bod[LSZ];
        body_of(g_ins[i], bod, sizeof bod); canon(bod, c, sizeof c); mnem_of(c, m, sizeof m);
        /* RULE self-mov: `mov X, X` is a no-op in every addressing mode the emitter produces. */
        if (g_on[3] && !strcmp(m, "mov") && ops2(c, a, sizeof a, b, sizeof b) && !strcmp(a, b)) { g_dead[i] = 1; g_hit[3]++; continue; }
        /* RULE diag-stamp (OPT-IN, --diag): r10 carries the SNOBOL4 statement number and r11 the BB node id, for crash dumps only (x86_asm.h's own note).  Dropping them cannot change an ANSWER; it degrades a diagnostic, which is why it is off by default and must be asked for by name. */
        if (g_on[4] && !strcmp(m, "mov") && ops2(c, a, sizeof a, b, sizeof b) && (!strcmp(a, "r10") || !strcmp(a, "r11")) && (b[0] == '-' || isdigit((unsigned char)b[0]))) { g_dead[i] = 1; g_hit[4]++; continue; }
        /* RULE store-reload: `mov MEM, R` immediately followed by `mov R, MEM` -- the reload re-reads the word just written.  Adjacency is required, so nothing can have touched either side. */
        if (g_on[1] && !strcmp(m, "mov") && ops2(c, a, sizeof a, b, sizeof b) && a[0] == '[') {
            int j = i + 1; while (j < g_n && !g_ins[j][0] && !g_lab[j][0]) j++;
            if (j < g_n && !g_fused[j] && !g_lab[j][0] && g_ins[j][0]) { char c2[LSZ], m2[32], a2[256], b2[256], bod2[LSZ]; body_of(g_ins[j], bod2, sizeof bod2); canon(bod2, c2, sizeof c2); mnem_of(c2, m2, sizeof m2);
                if (!strcmp(m2, "mov") && ops2(c2, a2, sizeof a2, b2, sizeof b2) && !strcmp(a2, b) && !strcmp(b2, a)) { g_dead[j] = 1; g_hit[1]++; } }
        }
        /* RULE got-cse: a second `mov R, [rip + SYM@GOTPCREL]` for the same SYM into the same R, inside one block, with no write to R in between.  A GOT slot is fixed after relocation, so the value is the same; the run stops at the first barrier, so no call or branch can be crossed. */
        if (g_on[0] && !strcmp(m, "mov") && ops2(c, a, sizeof a, b, sizeof b) && strstr(b, "@GOTPCREL")) {
            for (int j = i + 1; j < g_n; j++) {
                if (g_dead[j]) continue;
                if (!g_ins[j][0]) { if (g_lab[j][0]) break; continue; }
                if (is_barrier(j)) break;
                char c2[LSZ], m2[32], a2[256], b2[256], bod2[LSZ]; body_of(g_ins[j], bod2, sizeof bod2); canon(bod2, c2, sizeof c2); mnem_of(c2, m2, sizeof m2);
                if (!strcmp(m2, "mov") && ops2(c2, a2, sizeof a2, b2, sizeof b2) && !strcmp(a2, a) && !strcmp(b2, b)) { g_dead[j] = 1; g_hit[0]++; continue; }
                if (writes_reg(j, a)) break;
            }
        }
    }
    /* RULE dead-jmp: `jmp L` whose next surviving line is `L:` itself.  Run after the others so a jump exposed by a deletion is still caught. */
    if (g_on[2]) for (int i = 0; i < g_n; i++) {
        if (g_dead[i] || !g_ins[i][0] || g_fused[i]) continue;
        char c[LSZ], m[32], bod[LSZ]; body_of(g_ins[i], bod, sizeof bod); canon(bod, c, sizeof c); mnem_of(c, m, sizeof m);
        if (strcmp(m, "jmp")) continue;
        const char * tgt = strchr(c, ' '); if (!tgt) continue; tgt++;
        for (int j = i + 1; j < g_n; j++) { if (g_dead[j]) continue; if (g_lab[j][0]) { if (!strcmp(g_lab[j], tgt)) { g_dead[i] = 1; g_hit[2]++; } break; } if (g_ins[j][0]) break; }
    }
    /* RULE inline-single-use -- ⭐ THIS IS THE LITERAL-BOX COLLAPSE (Lon s262: "Did we ever collapse the literal boxes into an option of their operator boxes?"), expressed generically so it is not a per-op filter.  A literal box compiles to `jmp nK_lit_integer_α` / `nK_lit_integer_α: <a few stores> jmp <γ port>`; when that label is named exactly ONCE in the whole file, the box has a single predecessor and its body can move to the call site, retiring two jumps and a label.  Nothing here mentions `lit` -- any single-entry straight-line box collapses, which is the same cure for the whole class.
       ⛔ SAFETY IS A WHOLE-FILE REFERENCE COUNT, NOT A PATTERN MATCH.  Byrd-box wiring puts label addresses in places a jump-target scan would miss -- `lea rax, [rip + .Lx348_13]` installs a continuation, `.quad body$FN` seeds a cell -- so a label taken by ADDRESS anywhere must never be inlined-and-deleted.  The count below is over every textual occurrence of the token in the file except its own definition, so an address-taken label is refused by construction.  Additional refusals: a body containing a label, a conditional branch, a call, a directive, or a fused line; a body not ending in an unconditional jmp; a body longer than the cap.
       ⛔ IT DOES NOT REORDER OR RE-TIME ANYTHING: the instructions execute in the same order, at the same point, with two unconditional jumps removed. */
    if (g_on[5]) for (int i = 0; i < g_n; i++) {
        if (g_dead[i] || !g_ins[i][0] || g_fused[i]) continue;
        char c[LSZ], m[32], bod[LSZ]; body_of(g_ins[i], bod, sizeof bod); canon(bod, c, sizeof c); mnem_of(c, m, sizeof m);
        if (strcmp(m, "jmp")) continue;
        const char * t = strchr(c, ' '); if (!t) continue; t++;
        if (!t[0] || t[0] == '*' || t[0] == '%') continue;
        int def = -1; for (int j = 0; j < g_n; j++) if (!g_dead[j] && g_lab[j][0] && !strcmp(g_lab[j], t)) { def = j; break; }
        if (def < 0 || def == i) continue;
        long refs = 0; size_t tl = strlen(t);
        for (int j = 0; j < g_n; j++) { const char * h = g_raw[j]; while ((h = strstr(h, t)) != 0) { char before = (h == g_raw[j]) ? ' ' : h[-1]; char after = h[tl]; int wordy = !(isalnum((unsigned char)after) || after == '_' || after == '.' || after == '$') && !(isalnum((unsigned char)before) || before == '_' || before == '.' || before == '$'); if (wordy) { if (!(j == def && after == ':')) refs++; } h += tl; } }
        if (refs != 1) continue;
        /* ⛔⛔ FALLTHROUGH IS AN UNNAMED REFERENCE, AND MISSING IT PRODUCED A WRONG ANSWER (measured: roman printed `check: 441` instead of `check: 1102`).  A whole-file symbol count proves nobody NAMES the label; it says nothing about the block physically above it running off its end into this one.  Byrd-box wiring lays boxes out contiguously, so falling in is the common case, not the exotic one.  Refuse unless the previous instruction-bearing line ENDS the flow -- an unconditional jmp or a ret, fused or not. */
        { int prev = -1; for (int j = def - 1; j >= 0; j--) { if (g_dead[j] || !g_ins[j][0]) continue; prev = j; break; }
          if (prev < 0) continue;
          char cp[LSZ], bp[LSZ]; body_of(g_ins[prev], bp, sizeof bp); canon(bp, cp, sizeof cp);
          const char * last = cp; for (const char * q = cp; *q; q++) if (*q == ';') { last = q + 1; while (*last == ' ') last++; }
          char mp[32]; mnem_of(last, mp, sizeof mp);
          if (strcmp(mp, "jmp") && strcmp(mp, "ret")) continue; }
        int end = -1, len = 0, ok = 1;
        for (int j = def; j < g_n && ok; j++) {
            if (g_dead[j]) continue;
            if (j > def && g_lab[j][0]) { ok = 0; break; }
            if (!g_ins[j][0]) continue;
            char c2[LSZ], m2[32], bod2[LSZ]; body_of(g_ins[j], bod2, sizeof bod2); canon(bod2, c2, sizeof c2); mnem_of(c2, m2, sizeof m2);
            /* ⭐ A FUSED LINE IS THE NORMAL TERMINATOR HERE, not an exotic case: the emitter routinely writes the box's last store and its outgoing wire on one line -- `mov qword ptr [rsp + 8], rax;  jmp n2_keyword_assign_snobol4_α`.  Refusing every fused line made this rule fire ZERO times on roman.  A fused line is accepted ONLY as the final line, ONLY when its last component is an unconditional jmp and no component is a call, a directive, or a conditional branch; the raw line is then spliced verbatim, so nothing is re-encoded. */
            if (g_fused[j]) {
                char tmp[LSZ]; canon(bod2, tmp, sizeof tmp); int bad = 0; const char * seg = tmp; int nseg = 0, lastjmp = 0;
                while (seg && *seg) { const char * semi = strchr(seg, ';'); char one[LSZ]; size_t sl = semi ? (size_t)(semi - seg) : strlen(seg); if (sl >= sizeof one) { bad = 1; break; } memcpy(one, seg, sl); one[sl] = 0; char * q = one; while (*q == ' ') q++; if (*q) { char mm2[32]; mnem_of(q, mm2, sizeof mm2); nseg++; lastjmp = !strcmp(mm2, "jmp"); if (mm2[0] == '.' || !strcmp(mm2, "call") || !strcmp(mm2, "ret") || (mm2[0] == 'j' && strcmp(mm2, "jmp"))) { bad = 1; break; } }
                    seg = semi ? semi + 1 : 0; }
                if (bad || !lastjmp || nseg < 1) { ok = 0; break; }
                end = j; break;
            }
            if (c2[0] == '.' || !strcmp(m2, "call") || !strcmp(m2, "ret")) { ok = 0; break; }
            if (m2[0] == 'j') { if (strcmp(m2, "jmp")) { ok = 0; break; } end = j; break; }
            if (++len > 12) { ok = 0; break; }
        }
        if (!ok || end < 0) continue;
        /* splice: the jmp site becomes the body, the body's own trailing jmp stays, the original block goes */
        char buf[LSZ * 16]; size_t bl = 0; buf[0] = 0;
        for (int j = def; j <= end; j++) { if (g_dead[j] || !g_ins[j][0]) continue; size_t need = strlen(g_raw[j]); if (bl + need + 1 >= sizeof buf) { ok = 0; break; } memcpy(buf + bl, g_raw[j], need); bl += need; buf[bl] = 0; }
        if (!ok) continue;
        if (bl + 1 >= LSZ * 16) continue;
        strcpy(g_raw[i], "");
        g_splice[i] = strdup(buf);
        for (int j = def; j <= end; j++) g_dead[j] = 2;   /* ⛔ 2, NOT 1: state 1 means "instruction deleted, KEEP the label" (a self-mov on a labelled line must not take its label with it); state 2 means "line removed entirely", which is what a splice needs because the spliced text ALREADY carries the label.  Using 1 here emitted the label twice and `as` refused the file with `symbol '.Lx328_0' is already defined`.  Two different removals, two different states. */
        g_hit[5]++;
    }
    FILE * fo = outp ? fopen(outp, "w") : stdout;
    if (!fo) { fprintf(stderr, "scrip-peep: cannot write %s\n", outp); return 1; }
    long kept = 0, killed = 0;
    for (int i = 0; i < g_n; i++) {
        if (!g_dead[i]) { if (g_splice[i]) { fputs(g_splice[i], fo); kept++; killed += 2; } else { fputs(g_raw[i], fo); if (g_ins[i][0]) kept++; } continue; }
        killed++;
        /* A deleted instruction on a labelled line would take the label with it, so the label is re-emitted alone.  The parser above never attaches a label to a line it deletes except here, and this is the one place that can happen. */
        if (g_dead[i] == 1 && g_lab[i][0]) fprintf(fo, "%s:\n", g_lab[i]);
    }
    if (outp) fclose(fo);
    if (stats) { fprintf(stderr, "scrip-peep: %ld instructions in, %ld removed (%.2f%%)\n", kept + killed, killed, (kept + killed) ? 100.0 * (double)killed / (double)(kept + killed) : 0.0); for (int k = 0; k < 8; k++) if (g_rule[k]) fprintf(stderr, "  %-14s %s  %ld\n", g_rule[k], g_on[k] ? "on " : "off", g_hit[k]); }
    return 0;
}
