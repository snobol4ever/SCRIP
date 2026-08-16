/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#pragma once
#include "core.h"
#include "SM.h"
DESCR_t descr_to_str(DESCR_t d);
DESCR_t descr_to_str_fracdigit(DESCR_t d);
DESCR_t shared_arith(DESCR_t l, DESCR_t r, SM_op_t op);
