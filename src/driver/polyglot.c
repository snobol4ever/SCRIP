#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parsers/snobol4/scrip_cc.h"
#include "parsers/prolog/prolog_driver.h"
#include "parsers/prolog/prolog_atom.h"
#include "parsers/icon/icon_driver.h"
#include "parsers/raku/raku_driver.h"
#include "parsers/rebus/rebus_lower.h"
#include "runtime/builtins/gen_runtime.h"
#include "runtime/builtins/resolution.h"
#include "driver/driver.h"
#include "driver/polyglot.h"
#include "lower.h"
#include "SM.h"
int g_fi8_gen_init_count = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void polyglot_init(stage2_t *s2, const tree_t *prog)
{
    if (!prog) return;
    label_table_build(s2, prog);
    prescan_defines(prog);
    g_fi8_gen_init_count++;
    s2->proc_count = 0; global_count = 0;
    frame_depth = 0;
    memset(frame_stack, 0, sizeof frame_stack);
    scan_subj = ""; scan_pos = 1; scan_depth = 0;
    g_root = NULL;
    prolog_atom_init();
    memset(&s2->resolve_pred_table, 0, sizeof s2->resolve_pred_table);
    g_resolve_cut_flag = 0;
    g_resolve_active   = 0;
    memset(&s2->module_registry, 0, sizeof s2->module_registry);
    s2->module_registry.main_mod = -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int polyglot_module_open(stage2_t *s2, const tree_t *s)
{
    if (s2->module_registry.nmod >= SCRIP_MOD_MAX) return -1;
    int mod_idx = s2->module_registry.nmod++;
    ScripModule *m = &s2->module_registry.mods[mod_idx];
    m->name             = NULL;
    m->first            = s;
    m->last             = s;
    m->nstmts           = 0;
    m->core_label_start = s2->label_count;
    m->core_label_count = 0;
    m->proc_start       = s2->proc_count;
    m->nprocs           = 0;
    return mod_idx;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void polyglot_module_extend(stage2_t *s2, int mod_idx, const tree_t *s)
{
    if (mod_idx < 0) return;
    s2->module_registry.mods[mod_idx].last = s;
    s2->module_registry.mods[mod_idx].nstmts++;
}
extern tree_t *sno_parse_string_ast(const char *src, CODE_t **code_out);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *parse_scrip_polyglot(const char *src, const char *filename, lower_seg_t *segs, int *nsegs, int max_segs)
{
    tree_t *result = calloc(1, sizeof(tree_t));
    if (!result) return NULL;
    result->t = TT_PROGRAM;
    const char *p = src;
    while (*p) {
        const char *fence = strstr(p, "```");
        if (!fence) break;
        const char *tag_start = fence + 3;
        const char *tag_end   = tag_start;
        while (*tag_end && *tag_end != '\n' && *tag_end != '\r') tag_end++;
        while (tag_end > tag_start && (tag_end[-1] == ' ' || tag_end[-1] == '\t')) tag_end--;
        int tag_len = (int)(tag_end - tag_start);
        p = tag_end;
        while (*p && *p != '\n') p++;
        if (*p == '\n') p++;
        const char *block_start = p;
        const char *close = strstr(p, "```");
        if (!close) break;
        int   blen = (int)(close - block_start);
        char *block = malloc(blen + 1);
        if (!block) { p = close + 3; continue; }
        memcpy(block, block_start, blen);
        block[blen] = '\0';
        p = close + 3;
        while (*p && *p != '\n') p++;
        if (*p == '\n') p++;
        tree_t *sub_ast = NULL;
        lower_entry_fn fence_fn = lower_sno_stage2;
        if ((tag_len == 7 && strncmp(tag_start, "SNOBOL4", 7) == 0) || (tag_len == 5 && strncmp(tag_start, "Scrip", 5) == 0) || (tag_len == 5 && strncmp(tag_start, "SCRIP", 5) == 0)) {
            sub_ast = sno_parse_string_ast(block, NULL);
        } else if (tag_len == 4 && strncmp(tag_start, "Icon", 4) == 0) {
            icon_compile(block, filename, &sub_ast);
            fence_fn = lower_icon_stage2;
        } else if (tag_len == 6 && strncmp(tag_start, "Prolog", 6) == 0) {
            prolog_compile(block, filename, &sub_ast);
            fence_fn = lower_pl_stage2;
        } else if (tag_len == 4 && strncmp(tag_start, "Raku", 4) == 0) {
            raku_compile(block, filename, &sub_ast);
            fence_fn = lower_raku_stage2;
        } else if (tag_len == 5 && strncmp(tag_start, "Rebus", 5) == 0) {
            rebus_compile(block, filename, &sub_ast);
        } else { free(block); continue; }
        free(block);
        if (!sub_ast || sub_ast->n == 0) { free(sub_ast); continue; }
        if (segs && nsegs && *nsegs < max_segs) {
            tree_t *_sp = calloc(1, sizeof(tree_t));
            if (_sp) { _sp->t = TT_PROGRAM;
                for (int _si = 0; _si < sub_ast->n; _si++) if (sub_ast->c[_si]) ast_push(_sp, sub_ast->c[_si]);
                segs[*nsegs].prog = _sp; segs[*nsegs].fn = fence_fn; (*nsegs)++; }
        }
        for (int _i = 0; _i < sub_ast->n; _i++) {
            tree_t *ch = sub_ast->c[_i];
            if (!ch) continue;
            ast_push(result, ch);
        }
        if (sub_ast->c) free((char *)sub_ast->c - sizeof(size_t)); free(sub_ast);
    }
    return result;
}
