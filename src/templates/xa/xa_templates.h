/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#pragma once
#include "emit.h"
#include "XA.h"
void xa_macro_library_open    (void);
void xa_macro_library_close   (void);
void xa_bb_macro_library      (void);
void xa_exec_stmt_blob        (void);
void xa_file_header           (void);
void xa_file_footer           (void);
void xa_bb_ptr_slot           (void);
void xa_entry_dispatch        (void);
void xa_flat_data_section     (void);
void xa_prologue              (void);
void xa_epilogue              (void);
void xa_wasm_main_open        (void);
void xa_wasm_main_close       (void);
void xa_js_label_register     (void);
void xa_expression_registry   (void);
void xa_strtab_rodata         (void);
void xa_csettab_rodata        (void);
void xa_cap_fixup             (void);
void xa_pattern_blobs         (void);
