/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef POLYGLOT_H
#define POLYGLOT_H
#include <stdint.h>
#include "parser/snobol4/scrip_cc.h"
#include "driver/driver.h"
#include "stage2.h"
extern int g_fi8_gen_init_count;
extern int g_fi8_pl_init_count;
uint32_t polyglot_lang_mask(const tree_t *prog);
void     polyglot_init   (stage2_t *s2, const tree_t *prog, uint32_t lang_mask);
tree_t   *parse_scrip_polyglot(const char *src, const char *filename);
#endif
