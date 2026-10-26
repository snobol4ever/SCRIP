#ifndef PLANNER_HEAD_H
#define PLANNER_HEAD_H
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <climits>
#include <algorithm>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <math.h>
extern "C" {
#include "ct_arena.h"
#include "ct_vec.h"
#include "IR.h"
#include "stage2.h"
#include "gc_frame_map.h"
#include "dtp.h"
#include "ir_index.h"
#include "ir_node_index.h"
#include "ir_query.h"
#include "call_forms.h"
#include "zeta_depth.h"
}
#endif
extern "C" {
#include "g_planner.h"
