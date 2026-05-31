#pragma once
#ifndef SCRIP_BB_PROGRAM_H
#define SCRIP_BB_PROGRAM_H
#include "BB.h"
typedef struct bb_program_t {
    BB_graph_t **table;
    int          count;
    int          cap;
} bb_program_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int  bb_program_add (bb_program_t *p, BB_graph_t *bbg);
void bb_program_free(bb_program_t *p);
#endif
