/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef POLYGLOT_H
#define POLYGLOT_H
#include <stdint.h>
#include "parsers/snobol4/scrip_cc.h"
#include "driver/driver.h"
#include "stage2.h"
extern int g_fi8_gen_init_count;
void     polyglot_init   (stage2_t *s2, const tree_t *prog);
int      polyglot_module_open  (stage2_t *s2, const tree_t *s);
void     polyglot_module_extend(stage2_t *s2, int mod_idx, const tree_t *s);
tree_t   *parse_scrip_polyglot(const char *src, const char *filename, lower_seg_t *segs, int *nsegs, int max_segs);
#endif
