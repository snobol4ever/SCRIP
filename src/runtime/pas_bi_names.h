#ifndef PAS_BI_NAMES_H
#define PAS_BI_NAMES_H
enum pas_bi_id {
    PBI_abs, PBI_alloc, PBI_alloc_rec, PBI_alpha_str, PBI_append, PBI_arctan, PBI_arr_copy, PBI_arr_of, PBI_arrlit, PBI_assert, PBI_bound, PBI_ca_encode, PBI_ca_pack, PBI_ca_unpack, PBI_chr,
        PBI_chrlit, PBI_copy, PBI_cos, PBI_deref, PBI_deref_set, PBI_display, PBI_display_get, PBI_dispose, PBI_dispose_k, PBI_dynlen, PBI_dynset, PBI_elem_ref, PBI_enum_name, PBI_envcall, PBI_eof,
        PBI_eof_f, PBI_eoln, PBI_eoln_f, PBI_exp, PBI_fassign, PBI_fbuf_get, PBI_fbuf_set, PBI_fclose, PBI_feof_t, PBI_ferase, PBI_fget, PBI_field_idx_set, PBI_field_idx_update, PBI_field_set,
        PBI_fileexists, PBI_fill, PBI_fill_from, PBI_format, PBI_fput, PBI_frac, PBI_fread, PBI_fread_range, PBI_fwrite, PBI_fwrite_range, PBI_getbufch, PBI_getbufch_f, PBI_halt, PBI_heapfail, PBI_in,
        PBI_len, PBI_ln, PBI_mark, PBI_mathf, PBI_mkdir, PBI_move, PBI_nlf, PBI_nlg, PBI_now, PBI_open, PBI_ord_check, PBI_paramcount, PBI_paramstr, PBI_pcall, PBI_pchar_deref, PBI_pe, PBI_pos,
        PBI_pow, PBI_qchk, PBI_random, PBI_randomize, PBI_range_check, PBI_rarr, PBI_rdecode, PBI_read_c, PBI_read_c_f, PBI_read_i, PBI_read_i_f, PBI_read_s, PBI_read_s_f, PBI_readln, PBI_readln_f,
        PBI_rebase, PBI_ref_val, PBI_release, PBI_reset, PBI_resundef, PBI_rewrite, PBI_rmdir, PBI_round, PBI_rounde, PBI_rpatch, PBI_rterr, PBI_s2wide, PBI_set, PBI_setdif, PBI_seteq, PBI_setint,
        PBI_setlength, PBI_setne, PBI_setrange, PBI_setuni, PBI_sin, PBI_sqr, PBI_sqrt, PBI_std_mute, PBI_stdfile, PBI_str_delete, PBI_str_insert, PBI_str_to_int, PBI_strcmp, PBI_strf, PBI_strfill,
        PBI_stringofchar, PBI_strmove, PBI_subset, PBI_super, PBI_swapendian, PBI_tagmark, PBI_tagset_chk, PBI_tagwhole, PBI_tbuf_get, PBI_tbuf_set, PBI_tget, PBI_ticks, PBI_tput, PBI_treset,
        PBI_trewrite, PBI_trunc, PBI_udiv, PBI_umod, PBI_undefined_value, PBI_unhandled, PBI_unresolved_target, PBI_upcase, PBI_ustr, PBI_vcheck, PBI_view, PBI_wide2s, PBI_write, PBI_writeln,
        PBI_COUNT_
};
static const char g_pas_bi_name[][24] = { "__pas_abs", "__pas_alloc", "__pas_alloc_rec", "__pas_alpha_str", "__pas_append", "__pas_arctan", "__pas_arr_copy", "__pas_arr_of", "__pas_arrlit",
    "__pas_assert", "__pas_bound", "__pas_ca_encode", "__pas_ca_pack", "__pas_ca_unpack", "__pas_chr", "__pas_chrlit", "__pas_copy", "__pas_cos", "__pas_deref", "__pas_deref_set", "__pas_display",
    "__pas_display_get", "__pas_dispose", "__pas_dispose_k", "__pas_dynlen", "__pas_dynset", "__pas_elem_ref", "__pas_enum_name", "__pas_envcall", "__pas_eof", "__pas_eof_f", "__pas_eoln",
    "__pas_eoln_f", "__pas_exp", "__pas_fassign", "__pas_fbuf_get", "__pas_fbuf_set", "__pas_fclose", "__pas_feof_t", "__pas_ferase", "__pas_fget", "__pas_field_idx_set", "__pas_field_idx_update",
    "__pas_field_set", "__pas_fileexists", "__pas_fill", "__pas_fill_from", "__pas_format", "__pas_fput", "__pas_frac", "__pas_fread", "__pas_fread_range", "__pas_fwrite", "__pas_fwrite_range",
    "__pas_getbufch", "__pas_getbufch_f", "__pas_halt", "__pas_heapfail", "__pas_in", "__pas_len", "__pas_ln", "__pas_mark", "__pas_mathf", "__pas_mkdir", "__pas_move", "__pas_nlf", "__pas_nlg",
    "__pas_now", "__pas_open", "__pas_ord_check", "__pas_paramcount", "__pas_paramstr", "__pas_pcall", "__pas_pchar_deref", "__pas_pe", "__pas_pos", "__pas_pow", "__pas_qchk", "__pas_random",
    "__pas_randomize", "__pas_range_check", "__pas_rarr", "__pas_rdecode", "__pas_read_c", "__pas_read_c_f", "__pas_read_i", "__pas_read_i_f", "__pas_read_s", "__pas_read_s_f", "__pas_readln",
    "__pas_readln_f", "__pas_rebase", "__pas_ref_val", "__pas_release", "__pas_reset", "__pas_resundef", "__pas_rewrite", "__pas_rmdir", "__pas_round", "__pas_rounde", "__pas_rpatch", "__pas_rterr",
    "__pas_s2wide", "__pas_set", "__pas_setdif", "__pas_seteq", "__pas_setint", "__pas_setlength", "__pas_setne", "__pas_setrange", "__pas_setuni", "__pas_sin", "__pas_sqr", "__pas_sqrt",
    "__pas_std_mute", "__pas_stdfile", "__pas_str_delete", "__pas_str_insert", "__pas_str_to_int", "__pas_strcmp", "__pas_strf", "__pas_strfill", "__pas_stringofchar", "__pas_strmove", "__pas_subset",
    "__pas_super", "__pas_swapendian", "__pas_tagmark", "__pas_tagset_chk", "__pas_tagwhole", "__pas_tbuf_get", "__pas_tbuf_set", "__pas_tget", "__pas_ticks", "__pas_tput", "__pas_treset",
    "__pas_trewrite", "__pas_trunc", "__pas_udiv", "__pas_umod", "__pas_undefined_value", "__pas_unhandled", "__pas_unresolved_target", "__pas_upcase", "__pas_ustr", "__pas_vcheck", "__pas_view",
    "__pas_wide2s", "__pas_write", "__pas_writeln", };
#define PAS_BI_COUNT (sizeof(g_pas_bi_name) / sizeof(g_pas_bi_name[0]))
#endif
