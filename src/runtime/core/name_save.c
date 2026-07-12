#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "core.h"
#include "sil_macros.h"
#include "name_t.h"
typedef struct {
    int         live;
    NAME_t      name;
    const char *substr;
    int         slen;
} NAME_entry_t;
static NAME_ctx_t  g_root_ctx  = { NULL, 0, 0, NULL };
static NAME_ctx_t *g_ctx_current = &g_root_ctx;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void NAME_ctx_enter(NAME_ctx_t *ctx)
{
    if (!ctx) return;
    ctx->entries = NULL;
    ctx->cap     = 0;
    ctx->top     = 0;
    ctx->parent  = g_ctx_current;
    g_ctx_current = ctx;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void NAME_ctx_leave(void)
{
    NAME_ctx_t *ctx = g_ctx_current;
    if (!ctx || ctx == &g_root_ctx) return;
    g_ctx_current = ctx->parent;
    ctx->entries = NULL;
    ctx->cap     = 0;
    ctx->top     = 0;
    ctx->parent  = NULL;
}
