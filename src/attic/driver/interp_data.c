/* ATTIC: dead code excised from src/driver/interp_data.c (commit base 2a35216) — unreachable from main per --gc-sections oracle. Provenance only; not compiled. */
/*--------------------------------------------------------------------------------------------------------------------*/
DESCR_t dat_field_call(const char *name, DESCR_t *args, int nargs) {
    if (!name || nargs < 1) return FAILDESCR;
    DatType *_dt = dat_find_type(name);
    if (_dt) return dat_construct(_dt, args, nargs);
    int _fi = 0;
    DatType *_ft = dat_find_field(name, &_fi);
    if (_ft) return dat_field_get(name, args[0]);
    size_t _nlen = strlen(name);
    if (_nlen > 4 && strcmp(name + _nlen - 4, "_SET") == 0 && nargs >= 2) {
        char _fname[128];
        size_t _flen = _nlen - 4;
        if (_flen >= sizeof(_fname)) _flen = sizeof(_fname) - 1;
        memcpy(_fname, name, _flen); _fname[_flen] = '\0';
        DESCR_t *_cell = data_field_ptr(_fname, args[1]);
        if (_cell) { *_cell = args[0]; return args[0]; }
    }
    return FAILDESCR;
}
