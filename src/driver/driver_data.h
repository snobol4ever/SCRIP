#ifndef DRIVER_DATA_H
#define DRIVER_DATA_H
#include <stddef.h>
#include <stdint.h>
#include "ir/descr.h"
DESCR_t *data_field_ptr(const char *fname, DESCR_t inst);
typedef struct {
    char * name;
    int nfields;
    char ** fields;
    char * parent;
    DESCR_t * defaults;
    char * has_default;
    char * required;
    char * rw;
    char * sigil;
    char * priv;
    char ** mro;
    int mro_len, mro_cap;
    char ** parents;
    int nparents, parents_cap;
    char ** roles;
    int nroles, roles_cap;
    char ** methods;
    int nmethods, methods_cap;
    char ** multis;
    int nmultis, multis_cap;
    char has_build;
    char ** build_keys;
    int nbuild_keys, build_keys_cap;
    char ** handles_meth;
    char ** handles_fld;
    int nhandles, handles_cap;
    struct _DATINST_tType *blk;
    char live;
    char foreign;
    char nohook;
    char nhidden;
    int fcap;
} DatType;
DatType *dat_register(const char *spec);
DatType *dat_find_type(const char *name);
DatType *dat_find_type_n(const char *name, size_t n);
DatType *dat_find_field(const char *name, int *fidx);
void dat_set_live(const char *name, int live);
void dat_set_nohook(const char *name);
void dat_set_hidden(const char *name, int n);
int dat_type_hidden(int i);
DESCR_t dat_construct(DatType *t, DESCR_t *args, int nargs);
void class_inherit_multi(const char *child, const char **parents, int nparents);
void class_compose_role(const char *child, const char *role);
void dat_add_method(const char *type, const char *mname);
void dat_mark_method_multi(const char *type, const char *mname);
int dat_type_nmultis(int idx);
const char *dat_type_multi_at(int idx, int j);
void dat_set_field_default_i(const char *cls, const char *field, int64_t v);
void dat_set_field_priv(const char *cls, const char *field);
int dat_field_is_private(const char *cls, const char *field);
void dat_set_field_default_s(const char *cls, const char *field, const char *v);
void dat_set_field_default_r(const char *cls, const char *field, double v);
DESCR_t c_dat_field_get(const char *fname, DESCR_t obj);
#endif
