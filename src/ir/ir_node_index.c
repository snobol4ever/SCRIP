#include "IR.h"
#include "ct_arena.h"
#include "ct_vec.h"
#include "g_lower.h"
#include "ir_node_index.h"
#include <string.h>
#include <stdint.h>
#include <stddef.h>
typedef struct { IR_t * key; int val; } nid_slot_t;
int g_m4_dense_nid = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int bb_node_id(IR_t * nd) {
    if (!nd) return 0;
    if (!g_m4_dense_nid) return (int)((uintptr_t)nd % 100000u);
    if ((uint64_t)(g_lower.ir.nid_count + 1) * 2 > g_lower.ir.nid.len) {
        uint32_t nc = g_lower.ir.nid.len ? g_lower.ir.nid.len * 2 : 8;
        cv_t o = g_lower.ir.nid, n = { 0, 0, 0, 0 };
        n.p = ct_zalloc(nc, sizeof(nid_slot_t));
        n.len = nc;
        n.cap = nc;
        n.esz = (uint32_t)sizeof(nid_slot_t);
        g_lower.ir.nid = n;
        for (uint32_t i = 0; i < o.len; i++) {
            nid_slot_t q = CV_AT(o, nid_slot_t, i);
            if (!q.key) continue;
            uintptr_t j = ((uintptr_t)q.key >> 4) & (g_lower.ir.nid.len - 1);
            while (CV_AT(g_lower.ir.nid, nid_slot_t, j).key) j = (j + 1u) & (g_lower.ir.nid.len - 1);
            CV_AT(g_lower.ir.nid, nid_slot_t, j) = q;
        }
    }
    uintptr_t h = ((uintptr_t)nd >> 4) & (g_lower.ir.nid.len - 1);
    while (CV_AT(g_lower.ir.nid, nid_slot_t, h).key) { if (CV_AT(g_lower.ir.nid, nid_slot_t, h).key == nd) return CV_AT(g_lower.ir.nid, nid_slot_t, h).val; h = (h + 1u) & (g_lower.ir.nid.len - 1); }
    CV_AT(g_lower.ir.nid, nid_slot_t, h).key = nd;
    return CV_AT(g_lower.ir.nid, nid_slot_t, h).val = ++g_lower.ir.nid_count;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void nidx_build(IR_t ** nodes, int n) {
    size_t want = 16;
    while (want < (size_t)n * 2 + 2) want <<= 1;
    if (want > g_lower.ir.nidx_cap) {
        ct_drop(g_lower.ir.nidx_keys);
        ct_drop(g_lower.ir.nidx_vals);
        g_lower.ir.nidx_keys = (IR_t **)ct_alloc(want * sizeof(IR_t *));
        g_lower.ir.nidx_vals = (int *)ct_alloc(want * sizeof(int));
        g_lower.ir.nidx_cap = want;
    }
    memset(g_lower.ir.nidx_keys, 0, g_lower.ir.nidx_cap * sizeof(IR_t *));
    for (int i = 0; i < n; i++) {
        IR_t *k = nodes[i];
        if (!k) continue;
        size_t h = ((size_t)(uintptr_t)k >> 4) & (g_lower.ir.nidx_cap - 1);
        while (g_lower.ir.nidx_keys[h] && g_lower.ir.nidx_keys[h] != k) h = (h + 1) & (g_lower.ir.nidx_cap - 1);
        if (!g_lower.ir.nidx_keys[h]) { g_lower.ir.nidx_keys[h] = k; g_lower.ir.nidx_vals[h] = i; }
    }
    g_lower.ir.nidx_nodes = nodes;
    g_lower.ir.nidx_n = n;
}
int nidx(IR_t ** nodes, int n, const IR_t * e) {
    if (!e) return -1;
    if (nodes != g_lower.ir.nidx_nodes || n != g_lower.ir.nidx_n) { int k = 0; while (k < n) { if (nodes[k] == e) return k; k++; } return -1; }
    size_t h = ((size_t)(uintptr_t)e >> 4) & (g_lower.ir.nidx_cap - 1);
    while (g_lower.ir.nidx_keys[h]) { if (g_lower.ir.nidx_keys[h] == e) return g_lower.ir.nidx_vals[h]; h = (h + 1) & (g_lower.ir.nidx_cap - 1); }
    return -1;
}
void nidx_reset(void) { g_lower.ir.nidx_nodes = 0; g_lower.ir.nidx_n = 0; }
