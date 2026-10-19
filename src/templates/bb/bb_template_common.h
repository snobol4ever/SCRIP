/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#pragma once
#define CSET_CHAIN_MAX 3
#include "emit.h"
#include "emit_ir.h"
#include "XA.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int zls_off(const IR_t *nd);
int zls_argv_off(const IR_t *nd);
int zls_act_off(const IR_t *nd);
#define _ g_emit
#define GZ_CELL_OFF(slot) (8 + 16 * (slot))
