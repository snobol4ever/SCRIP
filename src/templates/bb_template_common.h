/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#pragma once
#include "emit.h"
#include "emit_ir.h"
#include "XA.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
void jvm_class_hdr(FILE *out, const char *name);
void net_class_hdr(FILE *out, int sid, int nid);
void net_α_hdr(FILE *out);
void net_β_hdr(FILE *out);
void net_cursor_load(FILE *out);
void net_spec_of(FILE *out);
void net_fail_ret(FILE *out);
void net_push_i4(FILE *out, int v);
void net_charset_class(FILE *out, int sid, int nid, const char *tag);
void net_escape_ldstr(FILE *out, const char *s);
void js_escape(FILE *out, const char *s);
int zls_off(const IR_t *nd);
#define _ g_emit
#define GZ_CELL_OFF(slot) (8 + 16 * (slot))
