                        .intel_syntax    noprefix
                        .text
                        .section         .rodata
.Lgvan0:                .string          "TBAL"
                        .align           8
__gva_names:
                        .quad            .Lgvan0
                        .section         .text
                        .intel_syntax    noprefix
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        mov              edi, 1
                        call             rt_gva_island@PLT
                        mov              rsi, rax
                        lea              rdi, [rip + __gva_names]
                        mov              edx, 1
                        call             gva_register@PLT
                        mov              r12, qword ptr [0x70000000]
                        call             rtcc_load_all@PLT
                        xor              esi, esi
                                                                              jmp   main_α
#-----------------------------------------------------------------------------------------------------------------------
main_α:
main_α_body:
#=======================================================================================================================
#         <stmt 1, line 1: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n0_statement_begin_α:                                                         jmp   n1_statement_end_α
n0_statement_begin_β:                                                         jmp   n2_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n1_statement_end_α:                                                           jmp   n2_statement_begin_α
#=======================================================================================================================
#         TBAL = 'the real tbal'
#-----------------------------------------------------------------------------------------------------------------------
n2_statement_begin_α:                                                         jmp   n3_lit_string_α
n2_statement_begin_β:                                                         jmp   n6_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n3_lit_string_α:        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 13
                        mov              rax, qword ptr [rip + .Lx44_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n4_assign_α
.Lx44_0:                .quad            .Lx44_0_s
.Lx44_0_s:              .string          "the real tbal"
#-----------------------------------------------------------------------------------------------------------------------
n4_assign_α:            mov              rax, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax              # TBAL
                        mov              qword ptr [r9 + 8], rdx;             jmp   n5_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n5_statement_end_α:     add              rsp, 16;                             jmp   n6_statement_begin_α
#=======================================================================================================================
#         TBAL = 'the real tbal'
#-----------------------------------------------------------------------------------------------------------------------
n6_statement_begin_α:                                                         jmp   n7_statement_end_α
n6_statement_begin_β:                                                         jmp   n8_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n7_statement_end_α:                                                           jmp   n8_statement_begin_α
#=======================================================================================================================
#         DIFFER($'TBAL', TBAL)                   :f(e001)
#-----------------------------------------------------------------------------------------------------------------------
n8_statement_begin_α:                                                         jmp   n9_lit_string_α
n8_statement_begin_β:                                                         jmp   n19_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n9_lit_string_α:        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx54_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n10_call_α
.Lx54_0:                .quad            .Lx54_0_s
.Lx54_0_s:              .string          "TBAL"
#-----------------------------------------------------------------------------------------------------------------------
n10_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd56:             .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd56]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 16
                        cmp              eax, 104;                            jne   .Lx55_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n8_statement_begin_β
.Lx55_240:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n11_deref_α
n10_call_β:             add              rsp, 16
                        add              rsp, 16;                             jmp   n8_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n11_deref_α:            sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]            # call
                        mov              rsi, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_deref@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        cmp              eax, 104;                            jne   .Lx57_240
                        add              rsp, 16
                        add              rsp, 32;                             jmp   n8_statement_begin_β
.Lx57_240:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n12_var_α
n11_deref_β:            add              rsp, 16
                        add              rsp, 32;                             jmp   n8_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n12_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 0]              # TBAL
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n13_call_α
n12_var_β:              add              rsp, 16;                             jmp   n11_deref_β
#-----------------------------------------------------------------------------------------------------------------------
n13_call_α:             sub              rsp, 16
                        sub              rsp, 32
                        mov              r8, qword ptr [rsp + 64]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 8], r8
                        mov              r8, qword ptr [rsp + 48]
                        mov              qword ptr [rsp + 16], r8
                        mov              r8, qword ptr [rsp + 56]
                        mov              qword ptr [rsp + 24], r8
                        .section         .rodata
.Lrkfnzd60:             .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd60]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 32
                        cmp              eax, 104;                            jne   .Lx59_240
                        add              rsp, 16;                             jmp   n12_var_β
.Lx59_240:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n14_statement_end_α
n13_call_β:             add              rsp, 16;                             jmp   n12_var_β
#-----------------------------------------------------------------------------------------------------------------------
n14_statement_end_α:    add              rsp, 80;                             jmp   n15_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 210/001: $string lookup'        :(END)
#-----------------------------------------------------------------------------------------------------------------------
n15_statement_begin_α:                                                        jmp   n16_lit_string_α
n15_statement_begin_β:                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n16_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 28
                        mov              rax, qword ptr [rip + .Lx65_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n17_assign_α
.Lx65_0:                .quad            .Lx65_0_s
.Lx65_0_s:              .string          "FAIL 210/001: $string lookup"
#-----------------------------------------------------------------------------------------------------------------------
n17_assign_α:           mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx66_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n18_statement_end_α
.Lx66_0:                .quad            .Lx66_0_s
.Lx66_0_s:              .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n18_statement_end_α:    add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e001  <stmt 6, line 10: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n19_statement_begin_α:                                                        jmp   n20_statement_end_α
n19_statement_begin_β:                                                        jmp   n21_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n20_statement_end_α:                                                          jmp   n21_statement_begin_α
#=======================================================================================================================
#         DIFFER($.TBAL, TBAL)                   :f(e002)
#-----------------------------------------------------------------------------------------------------------------------
n21_statement_begin_α:                                                        jmp   n22_lit_string_α
n21_statement_begin_β:                                                        jmp   n32_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n22_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx75_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n23_call_α
.Lx75_0:                .quad            .Lx75_0_s
.Lx75_0_s:              .string          "TBAL"
#-----------------------------------------------------------------------------------------------------------------------
n23_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd77:             .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd77]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 16
                        cmp              eax, 104;                            jne   .Lx76_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n21_statement_begin_β
.Lx76_240:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n24_deref_α
n23_call_β:             add              rsp, 16
                        add              rsp, 16;                             jmp   n21_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n24_deref_α:            sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 16]            # call
                        mov              rsi, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_deref@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        cmp              eax, 104;                            jne   .Lx78_240
                        add              rsp, 16
                        add              rsp, 32;                             jmp   n21_statement_begin_β
.Lx78_240:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n25_var_α
n24_deref_β:            add              rsp, 16
                        add              rsp, 32;                             jmp   n21_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n25_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 0]              # TBAL
                        mov              rdx, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n26_call_α
n25_var_β:              add              rsp, 16;                             jmp   n24_deref_β
#-----------------------------------------------------------------------------------------------------------------------
n26_call_α:             sub              rsp, 16
                        sub              rsp, 32
                        mov              r8, qword ptr [rsp + 64]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 8], r8
                        mov              r8, qword ptr [rsp + 48]
                        mov              qword ptr [rsp + 16], r8
                        mov              r8, qword ptr [rsp + 56]
                        mov              qword ptr [rsp + 24], r8
                        .section         .rodata
.Lrkfnzd81:             .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd81]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 32
                        cmp              eax, 104;                            jne   .Lx80_240
                        add              rsp, 16;                             jmp   n25_var_β
.Lx80_240:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n27_statement_end_α
n26_call_β:             add              rsp, 16;                             jmp   n25_var_β
#-----------------------------------------------------------------------------------------------------------------------
n27_statement_end_α:    add              rsp, 80;                             jmp   n28_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 210/002: $.var lookup'          :(END)
#-----------------------------------------------------------------------------------------------------------------------
n28_statement_begin_α:                                                        jmp   n29_lit_string_α
n28_statement_begin_β:                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n29_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 26
                        mov              rax, qword ptr [rip + .Lx86_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n30_assign_α
.Lx86_0:                .quad            .Lx86_0_s
.Lx86_0_s:              .string          "FAIL 210/002: $.var lookup"
#-----------------------------------------------------------------------------------------------------------------------
n30_assign_α:           mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx87_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n31_statement_end_α
.Lx87_0:                .quad            .Lx87_0_s
.Lx87_0_s:              .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n31_statement_end_α:    add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e002  <stmt 9, line 15: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n32_statement_begin_α:                                                        jmp   n33_statement_end_α
n32_statement_begin_β:                                                        jmp   n34_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n33_statement_end_α:                                                          jmp   n34_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'PASS 210_indirect_ref (2/2)'
#-----------------------------------------------------------------------------------------------------------------------
n34_statement_begin_α:                                                        jmp   n35_lit_string_α
n34_statement_begin_β:                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n35_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 27
                        mov              rax, qword ptr [rip + .Lx96_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n36_assign_α
.Lx96_0:                .quad            .Lx96_0_s
.Lx96_0_s:              .string          "PASS 210_indirect_ref (2/2)"
#-----------------------------------------------------------------------------------------------------------------------
n36_assign_α:           mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx97_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n37_statement_end_α
.Lx97_0:                .quad            .Lx97_0_s
.Lx97_0_s:              .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n37_statement_end_α:    add              rsp, 16;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                              jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
                        xor              edi, edi
                        call             exit@PLT
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              edi, 1
                        call             exit@PLT
                        .section         .note.GNU-stack,"",@progbits
