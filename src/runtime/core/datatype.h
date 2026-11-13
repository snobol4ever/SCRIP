#ifndef DATATYPE_H
#define DATATYPE_H
#include "ir/descr.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct _DATINST_tType { char *name; int nfields; int nhidden; char **fields; struct _DATINST_tType *next; long serial_next; } DATBLK_t;
typedef struct _DATINST_t { DATBLK_t *type; DESCR_t *fields; long id; long dumpno; } DATINST_t;
void DEFDAT_fn(const char *spec);
DESCR_t DATCON_fn(const char *type_name, ...);
DESCR_t FIELD_GET_fn(DESCR_t obj, const char *field);
void FIELD_SET_fn(DESCR_t obj, const char *field, DESCR_t val);
void FIELD_SET_fn(DESCR_t obj, const char *field, DESCR_t val);
typedef DESCR_t (*FNCPTR_t)(DESCR_t *args, int nargs);
typedef struct _FNCBLK_t {
    char *name;
    char *spec;
    char *entry_label;
    FNCPTR_t fn;
    int nparams;
    char **params;
    int nlocals;
    char **locals;
    int min_args;
    int max_args;
    int max_set;
    struct _FNCBLK_t *next;
} FNCBLK_t;
void DEFINE_fn(const char *spec, FNCPTR_t fn);
void DEFINE_fn_entry(const char *spec, FNCPTR_t fn, const char *entry_label);
void register_fn_alias(const char *newname, const char *oldname);
int core_call_registered_fn(const char *name, DESCR_t *args, int nargs, DESCR_t *out);
DESCR_t APPLY_fn(const char *name, DESCR_t *args, int nargs);
int FNCEX_fn(const char *name);
int FUNC_NPARAMS_fn(const char *fname);
int FUNC_NLOCALS_fn(const char *fname);
const char *FUNC_PARAM_fn(const char *fname, int i);
const char *FUNC_LOCAL_fn(const char *fname, int i);
const char *FUNC_ENTRY_fn(const char *fname);
int FUNC_IS_ENTRY_LABEL(const char *label);
#define FRETURN 4
#define NRETURN 5
#ifdef __cplusplus
}
#endif
#endif
