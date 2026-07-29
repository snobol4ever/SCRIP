                        .intel_syntax    noprefix
                        .text
                        .section         .rodata
.Lgvan0:                .string          "H"
.Lgvan1:                .string          "V"
.Lgvan2:                .string          "HC"
.Lgvan3:                .string          "NH"
.Lgvan4:                .string          "CROSS"
.Lgvan5:                .string          "VC"
.Lgvan6:                .string          "NV"
.Lgvan7:                .string          "PRINTV"
.Lgvan8:                .string          "C"
                        .align           8
__gva_names:
                        .quad            .Lgvan0
                        .quad            .Lgvan1
                        .quad            .Lgvan2
                        .quad            .Lgvan3
                        .quad            .Lgvan4
                        .quad            .Lgvan5
                        .quad            .Lgvan6
                        .quad            .Lgvan7
                        .quad            .Lgvan8
                        .section         .text
                        .intel_syntax    noprefix
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        mov              edi, 9
                        call             rt_gva_island@PLT
                        mov              rsi, rax
                        lea              rdi, [rip + __gva_names]
                        mov              edx, 9
                        call             gva_register@PLT
                        xor              esi, esi
                        call             main_α
                        xor              eax, eax
                        add              rsp, 24
                        ret
#-----------------------------------------------------------------------------------------------------------------------
main_α:
                        .global          main_α
                        .global          main_β
                        .global          main_γ
                        .global          main_ω
                        sub              rsp, 1336
                        mov              rdi, rsp
                        mov              ecx, 1336
                        xor              eax, eax
                        rep stosb
                        mov              [rsp + 1328], rbp
                        mov              rbp, rsp
main_α_body:
#=======================================================================================================================
#       &TRIM    =  1
#-----------------------------------------------------------------------------------------------------------------------
n0_lit_string_α:
                        mov              qword ptr [rbp + 64], 1
                        mov              rax, qword ptr [rip + .Lx69_0]
                        mov              qword ptr [rbp + 72], rax
                                                                                        jmp   n5_lit_integer_α
n0_lit_string_β:
                                                                                        jmp   n6_var_α
.Lx69_0:
                        .quad            .Lx69_0_s
.Lx69_0_s:
                        .string          "TRIM"
#-----------------------------------------------------------------------------------------------------------------------
n1_goto_α:
                                                                                        jmp   n6_var_α
n1_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n2_goto_α:
                                                                                        jmp   n7_var_α
n2_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n3_goto_α:
                                                                                        jmp   n8_var_α
n3_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n4_goto_α:
                                                                                        jmp   n9_var_α
n4_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n5_lit_integer_α:
                        mov              qword ptr [rbp + 80], 6
                        mov              rax, qword ptr [rip + .Lx74_0]
                        mov              qword ptr [rbp + 88], rax
                                                                                        jmp   n10_call_α
n5_lit_integer_β:
                                                                                        jmp   n6_var_α
.Lx74_0:
                        .quad            1
#=======================================================================================================================
# AGAIN H        =  INPUT                            :F(END)
#-----------------------------------------------------------------------------------------------------------------------
n6_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx75_0]
                        call             NV_GET_fn@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx75_240
                        add              rsp, 16
                                                                                        jmp   main_γ
.Lx75_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n11_assign_α
n6_var_β:
                        add              rsp, 16
                                                                                        jmp   main_γ
.Lx75_0:
                        .quad            .Lx75_0_s
.Lx75_0_s:
                        .string          "INPUT"
#=======================================================================================================================
# NEXTH HC       ?  @NH ANY(V) . CROSS = '*'         :F(AGAIN)
#-----------------------------------------------------------------------------------------------------------------------
n7_var_α:
                        mov              rax, qword ptr [1879052320]
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rbp + 416], rax
                        mov              qword ptr [rbp + 424], rdx
                                                                                        jmp   n12_var_α
n7_var_β:
                                                                                        jmp   n6_var_α
#=======================================================================================================================
# NEXTV VC       ?  @NV CROSS = '#'                  :F(NEXTH)
#-----------------------------------------------------------------------------------------------------------------------
n8_var_α:
                        mov              rax, qword ptr [1879052368]
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rbp + 624], rax
                        mov              qword ptr [rbp + 632], rdx
                                                                                        jmp   n13_match_head_α
n8_var_β:
                                                                                        jmp   n7_var_α
#=======================================================================================================================
# PRINT PRINTV   ?  LEN(1) . C =                     :F(NEXTV)
#-----------------------------------------------------------------------------------------------------------------------
n9_var_α:
                        mov              rax, qword ptr [1879052400]
                        mov              rdx, qword ptr [1879052408]
                        mov              qword ptr [rbp + 1056], rax
                        mov              qword ptr [rbp + 1064], rdx
                                                                                        jmp   n14_match_head_α
n9_var_β:
                                                                                        jmp   n8_var_α
#-----------------------------------------------------------------------------------------------------------------------
n10_call_α:
                        mov              rax, qword ptr [rbp + 64]
                        mov              qword ptr [rbp + 16], rax
                        mov              rax, qword ptr [rbp + 72]
                        mov              qword ptr [rbp + 24], rax
                        mov              rax, qword ptr [rbp + 80]
                        mov              qword ptr [rbp + 32], rax
                        mov              rax, qword ptr [rbp + 88]
                        mov              qword ptr [rbp + 40], rax
                        .section         .rodata
.Lrkfn80:               .string          "SNO$KWSET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn80]
                        lea              rsi, [rbp + 16]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 0], rax
                        mov              qword ptr [rbp + 8], rdx
                        cmp              eax, 99
                                                                                        je    n6_var_α
                                                                                        jmp   n6_var_α
n10_call_β:
                                                                                        jmp   n6_var_α
#-----------------------------------------------------------------------------------------------------------------------
n11_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n15_var_α
n11_assign_β:
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n12_var_α:
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rbp + 400], rax
                        mov              qword ptr [rbp + 408], rdx
                                                                                        jmp   n16_op73_α
n12_var_β:
                                                                                        jmp   n6_var_α
#-----------------------------------------------------------------------------------------------------------------------
n13_match_head_α:
                        mov              qword ptr [rbp + 512], r13
                        mov              qword ptr [rbp + 520], r14
                        mov              qword ptr [rbp + 528], r15
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rbp + 536], rax
                        mov              qword ptr [rbp + 504], rbp
                        mov              rdi, qword ptr [rbp + 624]
                        mov              rsi, qword ptr [rbp + 632]
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              r10, qword ptr [1879048192]
                        mov              qword ptr [r10 + 0], 0
                        add              r10, 24
                        mov              qword ptr [1879048192], r10
                        mov              qword ptr [rbp + 480], rsp
                        lea              rcx, [rip + g_patstk_sp]
                        mov              rax, qword ptr [rcx + 0]
                        mov              qword ptr [rbp + 472], rax
                        mov              dword ptr [rbp + 464], 0
.Lx84_0:
                        mov              r14d, dword ptr [rbp + 464]
                                                                                        jmp   n17_match_sequence_α
n13_match_head_β:
                        add              dword ptr [rbp + 464], 1
                        mov              eax, dword ptr [rbp + 464]
                        cmp              eax, r15d
                                                                                        jg    .Lx84_1
                        lea              rcx, [rip + g_anchor]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                                                                                        jne   .Lx84_1
                                                                                        jmp   .Lx84_0
.Lx84_1:
                        mov              rax, qword ptr [rbp + 472]
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              rsp, qword ptr [rbp + 480]
                        mov              r10, qword ptr [1879048192]
.Lx84_2:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx84_2
                        mov              qword ptr [1879048192], r10
                        mov              r13, qword ptr [rbp + 512]
                        mov              r14, qword ptr [rbp + 520]
                        mov              r15, qword ptr [rbp + 528]
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              rdx, qword ptr [rbp + 536]
                        call             rt_match_ctx_restore@PLT
                        mov              rbp, qword ptr [rbp + 504]
                                                                                        jmp   n7_var_α
#-----------------------------------------------------------------------------------------------------------------------
n14_match_head_α:
                        mov              qword ptr [rbp + 928], r13
                        mov              qword ptr [rbp + 936], r14
                        mov              qword ptr [rbp + 944], r15
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rbp + 952], rax
                        mov              qword ptr [rbp + 920], rbp
                        mov              rdi, qword ptr [rbp + 1056]
                        mov              rsi, qword ptr [rbp + 1064]
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              r10, qword ptr [1879048192]
                        mov              qword ptr [r10 + 0], 0
                        add              r10, 24
                        mov              qword ptr [1879048192], r10
                        mov              rax, rsp
                        sub              rsp, 32
                        mov              qword ptr [rsp + 16], rax
                        lea              rcx, [rip + g_patstk_sp]
                        mov              rax, qword ptr [rcx + 0]
                        mov              qword ptr [rsp + 8], rax
                        mov              dword ptr [rsp + 0], 0
.Lx86_0:
                        mov              r14d, dword ptr [rsp + 0]
                                                                                        jmp   n18_match_assign_save_α
n14_match_head_β:
                        add              dword ptr [rsp + 0], 1
                        mov              eax, dword ptr [rsp + 0]
                        cmp              eax, r15d
                                                                                        jg    .Lx86_1
                        lea              rcx, [rip + g_anchor]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                                                                                        jne   .Lx86_1
                                                                                        jmp   .Lx86_0
.Lx86_1:
                        mov              rax, qword ptr [rsp + 8]
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              rsp, qword ptr [rsp + 16]
                        mov              r10, qword ptr [1879048192]
.Lx86_2:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx86_2
                        mov              qword ptr [1879048192], r10
                        mov              r13, qword ptr [rbp + 928]
                        mov              r14, qword ptr [rbp + 936]
                        mov              r15, qword ptr [rbp + 944]
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              rdx, qword ptr [rbp + 952]
                        call             rt_match_ctx_restore@PLT
                        mov              rbp, qword ptr [rbp + 920]
                                                                                        jmp   n8_var_α
#=======================================================================================================================
#       V        =  INPUT                            :F(END)
#-----------------------------------------------------------------------------------------------------------------------
n15_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx87_0]
                        call             NV_GET_fn@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx87_240
                        add              rsp, 16
                                                                                        jmp   main_γ
.Lx87_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n19_assign_α
n15_var_β:
                        add              rsp, 16
                                                                                        jmp   main_γ
.Lx87_0:
                        .quad            .Lx87_0_s
.Lx87_0_s:
                        .string          "INPUT"
#-----------------------------------------------------------------------------------------------------------------------
n16_op73_α:
                        lea              rdi, [rbp + 400]
                        lea              rsi, [rbp + 384]
                        mov              rdx, 3866683
                        call             rt_coerce_str_d@PLT
                                                                                        jmp   n20_match_head_α
n16_op73_β:
                                                                                        jmp   n6_var_α
#-----------------------------------------------------------------------------------------------------------------------
n17_match_sequence_α:
                        mov              dword ptr [rbp + 576], r14d
                                                                                        jmp   n22_match_atp_α
n17_match_sequence_as:
                                                                                        jmp   n21_match_release_α
n17_match_sequence_β:
                                                                                        jmp   n23_match_defer_β
n17_match_sequence_af:
                                                                                        jmp   n13_match_head_β
#-----------------------------------------------------------------------------------------------------------------------
n18_match_assign_save_α:
                        sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d
                                                                                        jmp   n24_match_len_α
n18_match_assign_save_β:
                        add              rsp, 16
                                                                                        jmp   n14_match_head_β
#-----------------------------------------------------------------------------------------------------------------------
n19_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                                                                                        jmp   n25_var_α
n19_assign_β:
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n20_match_head_α:
                        mov              qword ptr [rbp + 224], r13
                        mov              qword ptr [rbp + 232], r14
                        mov              qword ptr [rbp + 240], r15
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rbp + 248], rax
                        mov              qword ptr [rbp + 216], rbp
                        mov              rdi, qword ptr [rbp + 416]
                        mov              rsi, qword ptr [rbp + 424]
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              r10, qword ptr [1879048192]
                        mov              qword ptr [r10 + 0], 0
                        add              r10, 24
                        mov              qword ptr [1879048192], r10
                        mov              rax, rsp
                        sub              rsp, 32
                        mov              qword ptr [rsp + 16], rax
                        lea              rcx, [rip + g_patstk_sp]
                        mov              rax, qword ptr [rcx + 0]
                        mov              qword ptr [rsp + 8], rax
                        mov              dword ptr [rsp + 0], 0
.Lx96_0:
                        mov              r14d, dword ptr [rsp + 0]
                                                                                        jmp   n26_match_sequence_α
n20_match_head_β:
                        add              dword ptr [rsp + 0], 1
                        mov              eax, dword ptr [rsp + 0]
                        cmp              eax, r15d
                                                                                        jg    .Lx96_1
                        lea              rcx, [rip + g_anchor]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                                                                                        jne   .Lx96_1
                                                                                        jmp   .Lx96_0
.Lx96_1:
                        mov              rax, qword ptr [rsp + 8]
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              rsp, qword ptr [rsp + 16]
                        mov              r10, qword ptr [1879048192]
.Lx96_2:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx96_2
                        mov              qword ptr [1879048192], r10
                        mov              r13, qword ptr [rbp + 224]
                        mov              r14, qword ptr [rbp + 232]
                        mov              r15, qword ptr [rbp + 240]
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              rdx, qword ptr [rbp + 248]
                        call             rt_match_ctx_restore@PLT
                        mov              rbp, qword ptr [rbp + 216]
                                                                                        jmp   n6_var_α
#-----------------------------------------------------------------------------------------------------------------------
n21_match_release_α:
                        mov              rax, qword ptr [rbp + 472]
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              qword ptr [rbp + 488], r14
                        mov              rsp, qword ptr [rbp + 480]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, qword ptr [1879048192]
                        mov              r10, rsi
.Lx98_5:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx98_5
                        lea              rdi, [r10 + 24]
                        mov              rdx, r13
                        call             rt_dcap_end_ok_open@PLT
.Lx98_1:
                        test             rax, rax
                                                                                        je    .Lx98_2
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx98_3]
                        lea              rdx, [rip + .Lx98_4]
                                                                                        jmp   rax
.Lx98_3:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx98_1
.Lx98_4:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx98_1
.Lx98_2:
                        call             rt_dcap_end_ok_close@PLT
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              r10, qword ptr [1879048192]
.Lx98_6:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx98_6
                        mov              qword ptr [1879048192], r10
                        mov              r13, qword ptr [rbp + 512]
                        mov              r14, qword ptr [rbp + 520]
                        mov              r15, qword ptr [rbp + 528]
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              rdx, qword ptr [rbp + 536]
                        call             rt_match_ctx_restore@PLT
                                                                                        jmp   n27_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n22_match_atp_α:
                        mov              esi, r14d
                        lea              rdi, [rip + .S0]
                        call             rt_at_cursor@PLT
                                                                                        jmp   n23_match_defer_α
n22_match_atp_β:
                                                                                        jmp   n13_match_head_β
#-----------------------------------------------------------------------------------------------------------------------
n23_match_defer_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        cmp              eax, 3
                                                                                        jne   .Lx100_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx100_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx100_10
.Lx100_9:
                        xor              eax, eax
.Lx100_10:
                        test             rax, rax
                                                                                        jz    .Lx100_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx100_4]
                        lea              rdx, [rip + .Lx100_5]
                                                                                        jmp   rax
.Lx100_4:
                                                                                        jmp   n21_match_release_α
.Lx100_5:
                                                                                        jmp   n22_match_atp_β
.Lx100_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S1]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx100_2:
                        test             rax, rax
                                                                                        je    .Lx100_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx100_7]
                        lea              rdx, [rip + .Lx100_8]
                                                                                        jmp   rax
.Lx100_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx100_2
.Lx100_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx100_2
.Lx100_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n22_match_atp_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx100_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n21_match_release_α
.Lx100_6:
                        add              rsp, 16
                                                                                        jmp   n22_match_atp_β
n23_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n24_match_len_α:
                        mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d
                                                                                        jle   .Lx101_240
                        add              rsp, 16
                                                                                        jmp   n14_match_head_β
.Lx101_240:
                        add              r14d, 1
                                                                                        jmp   n28_match_assign_cond_α
n24_match_len_β:
                        sub              r14d, 1
                        add              rsp, 16
                                                                                        jmp   n14_match_head_β
#=======================================================================================================================
#       HC       =  H
#-----------------------------------------------------------------------------------------------------------------------
n25_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n29_assign_α
n25_var_β:
                        add              rsp, 16
                                                                                        jmp   n7_var_α
#-----------------------------------------------------------------------------------------------------------------------
n26_match_sequence_α:
                                                                                        jmp   n31_match_atp_α
n26_match_sequence_as:
                                                                                        jmp   n30_match_release_α
n26_match_sequence_β:
                                                                                        jmp   n33_match_assign_cond_β
n26_match_sequence_af:
                                                                                        jmp   n20_match_head_β
#-----------------------------------------------------------------------------------------------------------------------
n27_lit_string_α:
                        mov              qword ptr [rbp + 560], 1
                        mov              rax, qword ptr [rip + .Lx105_0]
                        mov              qword ptr [rbp + 568], rax
                                                                                        jmp   n34_match_replace_α
n27_lit_string_β:
                                                                                        jmp   n7_var_α
.Lx105_0:
                        .quad            .Lx105_0_s
.Lx105_0_s:
                        .string          "#"
#-----------------------------------------------------------------------------------------------------------------------
n28_match_assign_cond_α:
                        mov              eax, dword ptr [rsp + 0]
                        lea              rcx, [rip + .S2]
                        mov              r10, qword ptr [1879048192]
                        mov              qword ptr [r10 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r10 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r10 + 16], rdx
                        add              r10, 24
                        mov              qword ptr [1879048192], r10
                                                                                        jmp   n35_match_release_α
n28_match_assign_cond_β:
                        mov              rax, qword ptr [1879048192]
                        sub              rax, 24
                        mov              qword ptr [1879048192], rax
                                                                                        jmp   n24_match_len_β
#-----------------------------------------------------------------------------------------------------------------------
n29_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052320], rax
                        mov              qword ptr [1879052328], rdx
                                                                                        jmp   n7_var_α
n29_assign_β:
                                                                                        jmp   n7_var_α
#-----------------------------------------------------------------------------------------------------------------------
n30_match_release_α:
                        mov              rax, qword ptr [rsp + 24]
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              eax, dword ptr [rsp + 16]
                        mov              dword ptr [rsp + 224], eax
                        mov              qword ptr [rsp + 248], r14
                        mov              rsp, qword ptr [rsp + 32]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, qword ptr [1879048192]
                        mov              r10, rsi
.Lx110_5:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx110_5
                        lea              rdi, [r10 + 24]
                        mov              rdx, r13
                        call             rt_dcap_end_ok_open@PLT
.Lx110_1:
                        test             rax, rax
                                                                                        je    .Lx110_2
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx110_3]
                        lea              rdx, [rip + .Lx110_4]
                                                                                        jmp   rax
.Lx110_3:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx110_1
.Lx110_4:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx110_1
.Lx110_2:
                        call             rt_dcap_end_ok_close@PLT
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              r10, qword ptr [1879048192]
.Lx110_6:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx110_6
                        mov              qword ptr [1879048192], r10
                        mov              r13, qword ptr [rbp + 224]
                        mov              r14, qword ptr [rbp + 232]
                        mov              r15, qword ptr [rbp + 240]
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              rdx, qword ptr [rbp + 248]
                        call             rt_match_ctx_restore@PLT
                                                                                        jmp   n36_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n31_match_atp_α:
                        mov              esi, r14d
                        lea              rdi, [rip + .S3]
                        call             rt_at_cursor@PLT
                                                                                        jmp   n32_match_assign_save_α
n31_match_atp_β:
                                                                                        jmp   n20_match_head_β
#-----------------------------------------------------------------------------------------------------------------------
n32_match_assign_save_α:
                        sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d
                                                                                        jmp   n37_match_any_α
n32_match_assign_save_β:
                        add              rsp, 16
                                                                                        jmp   n31_match_atp_β
#-----------------------------------------------------------------------------------------------------------------------
n33_match_assign_cond_α:
                        mov              eax, dword ptr [rsp + 0]
                        lea              rcx, [rip + .S1]
                        mov              r10, qword ptr [1879048192]
                        mov              qword ptr [r10 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r10 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r10 + 16], rdx
                        add              r10, 24
                        mov              qword ptr [1879048192], r10
                                                                                        jmp   n30_match_release_α
n33_match_assign_cond_β:
                        mov              rax, qword ptr [1879048192]
                        sub              rax, 24
                        mov              qword ptr [1879048192], rax
                                                                                        jmp   n37_match_any_β
#-----------------------------------------------------------------------------------------------------------------------
n34_match_replace_α:
                        mov              rdi, qword ptr [rip + .Lx117_0]
                        mov              rsi, qword ptr [rbp + 624]
                        mov              rdx, qword ptr [rbp + 632]
                        mov              ecx, dword ptr [rbp + 464]
                        mov              r8, qword ptr [rbp + 488]
                        lea              r9, [rbp + 560]
                        call             rt_match_replace@PLT
                                                                                        jmp   .Lx117_1
.Lx117_0:
                        .quad            .Lx117_0_s
.Lx117_0_s:
                        .string          "VC"
.Lx117_1:
                        mov              rbp, qword ptr [rbp + 504]
                                                                                        jmp   n38_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n35_match_release_α:
                        mov              rax, qword ptr [rsp + 24]
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              eax, dword ptr [rsp + 16]
                        mov              dword ptr [rsp + 928], eax
                        mov              qword ptr [rsp + 952], r14
                        mov              rsp, qword ptr [rsp + 32]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, qword ptr [1879048192]
                        mov              r10, rsi
.Lx119_5:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx119_5
                        lea              rdi, [r10 + 24]
                        mov              rdx, r13
                        call             rt_dcap_end_ok_open@PLT
.Lx119_1:
                        test             rax, rax
                                                                                        je    .Lx119_2
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx119_3]
                        lea              rdx, [rip + .Lx119_4]
                                                                                        jmp   rax
.Lx119_3:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx119_1
.Lx119_4:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx119_1
.Lx119_2:
                        call             rt_dcap_end_ok_close@PLT
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              r10, qword ptr [1879048192]
.Lx119_6:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx119_6
                        mov              qword ptr [1879048192], r10
                        mov              r13, qword ptr [rbp + 928]
                        mov              r14, qword ptr [rbp + 936]
                        mov              r15, qword ptr [rbp + 944]
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              rdx, qword ptr [rbp + 952]
                        call             rt_match_ctx_restore@PLT
                                                                                        jmp   n39_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n36_lit_string_α:
                        mov              qword ptr [rbp + 272], 1
                        mov              rax, qword ptr [rip + .Lx120_0]
                        mov              qword ptr [rbp + 280], rax
                                                                                        jmp   n40_match_replace_α
n36_lit_string_β:
                                                                                        jmp   n6_var_α
.Lx120_0:
                        .quad            .Lx120_0_s
.Lx120_0_s:
                        .string          "*"
#-----------------------------------------------------------------------------------------------------------------------
n37_match_any_α:
                        mov              eax, r14d
                        cmp              eax, r15d
                                                                                        jl    .Lx122_239
                        add              rsp, 16
                                                                                        jmp   n31_match_atp_β
.Lx122_239:
                        movsxd           rcx, r14d
                        movzx            esi, byte ptr [r13+rcx]
                        mov              r8, qword ptr [rbp + 392]
                        mov              r9d, dword ptr [rbp + 388]
                        mov              edx, 0
.Lx122_5:
                        cmp              edx, r9d
                                                                                        jl    .Lx122_240
                        add              rsp, 16
                                                                                        jmp   n31_match_atp_β
.Lx122_240:
                        movzx            edi, byte ptr [r8 + rdx]
                        cmp              esi, edi
                                                                                        je    .Lx122_6
                        add              edx, 1
                                                                                        jmp   .Lx122_5
.Lx122_6:
                        add              r14d, 1
                                                                                        jmp   n33_match_assign_cond_α
n37_match_any_β:
                        sub              r14d, 1
                        add              rsp, 16
                                                                                        jmp   n31_match_atp_β
#=======================================================================================================================
#       OUTPUT   =
#-----------------------------------------------------------------------------------------------------------------------
n38_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              rax, qword ptr [rip + .Lx123_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n41_assign_α
n38_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n44_var_α
.Lx123_0:
                        .quad            .Lx123_0_s
.Lx123_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n39_lit_string_α:
                        mov              qword ptr [rbp + 976], 1
                        mov              rax, qword ptr [rip + .Lx124_0]
                        mov              qword ptr [rbp + 984], rax
                                                                                        jmp   n42_match_replace_α
n39_lit_string_β:
                                                                                        jmp   n8_var_α
.Lx124_0:
                        .quad            .Lx124_0_s
.Lx124_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n40_match_replace_α:
                        mov              rdi, qword ptr [rip + .Lx126_0]
                        mov              rsi, qword ptr [rbp + 416]
                        mov              rdx, qword ptr [rbp + 424]
                        mov              ecx, dword ptr [rbp + 176]
                        mov              r8, qword ptr [rbp + 200]
                        lea              r9, [rbp + 272]
                        call             rt_match_replace@PLT
                                                                                        jmp   .Lx126_1
.Lx126_0:
                        .quad            .Lx126_0_s
.Lx126_0_s:
                        .string          "HC"
.Lx126_1:
                        mov              rbp, qword ptr [rbp + 216]
                                                                                        jmp   n43_var_α
#-----------------------------------------------------------------------------------------------------------------------
n41_assign_α:
                        mov              rsi, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx127_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   n44_var_α
n41_assign_β:
                                                                                        jmp   n44_var_α
.Lx127_0:
                        .quad            .Lx127_0_s
.Lx127_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n42_match_replace_α:
                        mov              rdi, qword ptr [rip + .Lx129_0]
                        mov              rsi, qword ptr [rbp + 1056]
                        mov              rdx, qword ptr [rbp + 1064]
                        mov              ecx, dword ptr [rbp + 880]
                        mov              r8, qword ptr [rbp + 904]
                        lea              r9, [rbp + 976]
                        call             rt_match_replace@PLT
                                                                                        jmp   .Lx129_1
.Lx129_0:
                        .quad            .Lx129_0_s
.Lx129_0_s:
                        .string          "PRINTV"
.Lx129_1:
                        mov              rbp, qword ptr [rbp + 920]
                                                                                        jmp   n45_var_α
#=======================================================================================================================
#       VC       =  V
#-----------------------------------------------------------------------------------------------------------------------
n43_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n46_assign_α
n43_var_β:
                        add              rsp, 16
                                                                                        jmp   n8_var_α
#=======================================================================================================================
#       PRINTV   =  V
#-----------------------------------------------------------------------------------------------------------------------
n44_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n47_assign_α
n44_var_β:
                        add              rsp, 16
                                                                                        jmp   n48_var_α
#=======================================================================================================================
#       OUTPUT   =  DIFFER(C, '#') DUPL(' ', NH) C   :S(PRINT)
#-----------------------------------------------------------------------------------------------------------------------
n45_var_α:
                        mov              rax, qword ptr [1879052416]
                        mov              rdx, qword ptr [1879052424]
                        mov              qword ptr [rbp + 1168], rax
                        mov              qword ptr [rbp + 1176], rdx
                                                                                        jmp   n49_lit_string_α
n45_var_β:
                                                                                        jmp   n50_var_α
#-----------------------------------------------------------------------------------------------------------------------
n46_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052368], rax
                        mov              qword ptr [1879052376], rdx
                                                                                        jmp   n8_var_α
n46_assign_β:
                                                                                        jmp   n8_var_α
#-----------------------------------------------------------------------------------------------------------------------
n47_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052400], rax
                        mov              qword ptr [1879052408], rdx
                                                                                        jmp   n48_var_α
n47_assign_β:
                                                                                        jmp   n48_var_α
#=======================================================================================================================
#       PRINTV   ?  POS(NV) LEN(1) = '#'
#-----------------------------------------------------------------------------------------------------------------------
n48_var_α:
                        mov              rax, qword ptr [1879052400]
                        mov              rdx, qword ptr [1879052408]
                        mov              qword ptr [rbp + 848], rax
                        mov              qword ptr [rbp + 856], rdx
                                                                                        jmp   n51_var_α
n48_var_β:
                                                                                        jmp   n9_var_α
#-----------------------------------------------------------------------------------------------------------------------
n49_lit_string_α:
                        mov              qword ptr [rbp + 1184], 1
                        mov              rax, qword ptr [rip + .Lx136_0]
                        mov              qword ptr [rbp + 1192], rax
                                                                                        jmp   n52_call_α
n49_lit_string_β:
                                                                                        jmp   n50_var_α
.Lx136_0:
                        .quad            .Lx136_0_s
.Lx136_0_s:
                        .string          "#"
#=======================================================================================================================
#       OUTPUT   =  H                                :(PRINT)
#-----------------------------------------------------------------------------------------------------------------------
n50_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n53_assign_α
n50_var_β:
                        add              rsp, 16
                                                                                        jmp   n9_var_α
#-----------------------------------------------------------------------------------------------------------------------
n51_var_α:
                        mov              rax, qword ptr [1879052384]
                        mov              rdx, qword ptr [1879052392]
                        mov              qword ptr [rbp + 832], rax
                        mov              qword ptr [rbp + 840], rdx
                                                                                        jmp   n54_op74_α
n51_var_β:
                                                                                        jmp   n9_var_α
#-----------------------------------------------------------------------------------------------------------------------
n52_call_α:
                        mov              rax, qword ptr [rbp + 1168]
                        mov              qword ptr [rbp + 1120], rax
                        mov              rax, qword ptr [rbp + 1176]
                        mov              qword ptr [rbp + 1128], rax
                        mov              rax, qword ptr [rbp + 1184]
                        mov              qword ptr [rbp + 1136], rax
                        mov              rax, qword ptr [rbp + 1192]
                        mov              qword ptr [rbp + 1144], rax
                        .section         .rodata
.Lrkfn140:              .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn140]
                        lea              rsi, [rbp + 1120]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1104], rax
                        mov              qword ptr [rbp + 1112], rdx
                        cmp              eax, 99
                                                                                        je    n50_var_α
                                                                                        jmp   n55_lit_string_α
n52_call_β:
                                                                                        jmp   n50_var_α
#-----------------------------------------------------------------------------------------------------------------------
n53_assign_α:
                        mov              rsi, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx141_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   n9_var_α
n53_assign_β:
                                                                                        jmp   n9_var_α
.Lx141_0:
                        .quad            .Lx141_0_s
.Lx141_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n54_op74_α:
                        lea              rdi, [rbp + 832]
                        lea              rsi, [rbp + 816]
                        mov              rdx, 10682530
                        call             rt_coerce_int_d@PLT
                                                                                        jmp   n56_match_head_α
n54_op74_β:
                                                                                        jmp   n9_var_α
#-----------------------------------------------------------------------------------------------------------------------
n55_lit_string_α:
                        mov              qword ptr [rbp + 1264], 1
                        mov              rax, qword ptr [rip + .Lx144_0]
                        mov              qword ptr [rbp + 1272], rax
                                                                                        jmp   n57_var_α
n55_lit_string_β:
                                                                                        jmp   n50_var_α
.Lx144_0:
                        .quad            .Lx144_0_s
.Lx144_0_s:
                        .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n56_match_head_α:
                        mov              qword ptr [rbp + 736], r13
                        mov              qword ptr [rbp + 744], r14
                        mov              qword ptr [rbp + 752], r15
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rbp + 760], rax
                        mov              qword ptr [rbp + 728], rbp
                        mov              rdi, qword ptr [rbp + 848]
                        mov              rsi, qword ptr [rbp + 856]
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              r10, qword ptr [1879048192]
                        mov              qword ptr [r10 + 0], 0
                        add              r10, 24
                        mov              qword ptr [1879048192], r10
                        mov              rax, rsp
                        sub              rsp, 32
                        mov              qword ptr [rsp + 16], rax
                        lea              rcx, [rip + g_patstk_sp]
                        mov              rax, qword ptr [rcx + 0]
                        mov              qword ptr [rsp + 8], rax
                        mov              dword ptr [rsp + 0], 0
.Lx146_0:
                        mov              r14d, dword ptr [rsp + 0]
                                                                                        jmp   n58_match_sequence_α
n56_match_head_β:
                        add              dword ptr [rsp + 0], 1
                        mov              eax, dword ptr [rsp + 0]
                        cmp              eax, r15d
                                                                                        jg    .Lx146_1
                        lea              rcx, [rip + g_anchor]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                                                                                        jne   .Lx146_1
                                                                                        jmp   .Lx146_0
.Lx146_1:
                        mov              rax, qword ptr [rsp + 8]
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              rsp, qword ptr [rsp + 16]
                        mov              r10, qword ptr [1879048192]
.Lx146_2:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx146_2
                        mov              qword ptr [1879048192], r10
                        mov              r13, qword ptr [rbp + 736]
                        mov              r14, qword ptr [rbp + 744]
                        mov              r15, qword ptr [rbp + 752]
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              rdx, qword ptr [rbp + 760]
                        call             rt_match_ctx_restore@PLT
                        mov              rbp, qword ptr [rbp + 728]
                                                                                        jmp   n9_var_α
#-----------------------------------------------------------------------------------------------------------------------
n57_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rbp + 1280], rax
                        mov              qword ptr [rbp + 1288], rdx
                                                                                        jmp   n59_call_α
n57_var_β:
                                                                                        jmp   n50_var_α
#-----------------------------------------------------------------------------------------------------------------------
n58_match_sequence_α:
                                                                                        jmp   n61_match_pos_α
n58_match_sequence_as:
                                                                                        jmp   n60_match_release_α
n58_match_sequence_β:
                                                                                        jmp   n62_match_len_β
n58_match_sequence_af:
                                                                                        jmp   n56_match_head_β
#-----------------------------------------------------------------------------------------------------------------------
n59_call_α:
                        mov              rax, qword ptr [rbp + 1264]
                        mov              qword ptr [rbp + 1216], rax
                        mov              rax, qword ptr [rbp + 1272]
                        mov              qword ptr [rbp + 1224], rax
                        mov              rax, qword ptr [rbp + 1280]
                        mov              qword ptr [rbp + 1232], rax
                        mov              rax, qword ptr [rbp + 1288]
                        mov              qword ptr [rbp + 1240], rax
                        .section         .rodata
.Lrkfn151:              .string          "DUPL"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn151]
                        lea              rsi, [rbp + 1216]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1200], rax
                        mov              qword ptr [rbp + 1208], rdx
                        cmp              eax, 99
                                                                                        je    n50_var_α
                                                                                        jmp   n63_binop_α
n59_call_β:
                                                                                        jmp   n50_var_α
#-----------------------------------------------------------------------------------------------------------------------
n60_match_release_α:
                        mov              rax, qword ptr [rsp + 8]
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              eax, dword ptr [rsp + 0]
                        mov              dword ptr [rsp + 720], eax
                        mov              qword ptr [rsp + 744], r14
                        mov              rsp, qword ptr [rsp + 16]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, qword ptr [1879048192]
                        mov              r10, rsi
.Lx153_5:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx153_5
                        lea              rdi, [r10 + 24]
                        mov              rdx, r13
                        call             rt_dcap_end_ok_open@PLT
.Lx153_1:
                        test             rax, rax
                                                                                        je    .Lx153_2
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx153_3]
                        lea              rdx, [rip + .Lx153_4]
                                                                                        jmp   rax
.Lx153_3:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx153_1
.Lx153_4:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx153_1
.Lx153_2:
                        call             rt_dcap_end_ok_close@PLT
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              r10, qword ptr [1879048192]
.Lx153_6:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx153_6
                        mov              qword ptr [1879048192], r10
                        mov              r13, qword ptr [rbp + 736]
                        mov              r14, qword ptr [rbp + 744]
                        mov              r15, qword ptr [rbp + 752]
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              rdx, qword ptr [rbp + 760]
                        call             rt_match_ctx_restore@PLT
                                                                                        jmp   n64_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n61_match_pos_α:
                        mov              rax, qword ptr [rbp + 824]
                        cmp              r14d, eax
                                                                                        jne   n56_match_head_β
                                                                                        jmp   n62_match_len_α
n61_match_pos_β:
                                                                                        jmp   n56_match_head_β
#-----------------------------------------------------------------------------------------------------------------------
n62_match_len_α:
                        mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d
                                                                                        jg    n56_match_head_β
                        add              r14d, 1
                                                                                        jmp   n60_match_release_α
n62_match_len_β:
                        sub              r14d, 1
                                                                                        jmp   n56_match_head_β
#-----------------------------------------------------------------------------------------------------------------------
n63_binop_α:
                        mov              rdi, qword ptr [rbp + 1104]
                        mov              rsi, qword ptr [rbp + 1112]
                        mov              rdx, qword ptr [rbp + 1200]
                        mov              rcx, qword ptr [rbp + 1208]
                        call             str_concat_d@PLT
                        mov              qword ptr [rbp + 1088], rax
                        mov              qword ptr [rbp + 1096], rdx
                                                                                        jmp   n65_var_α
n63_binop_β:
                                                                                        jmp   n50_var_α
#-----------------------------------------------------------------------------------------------------------------------
n64_lit_string_α:
                        mov              qword ptr [rbp + 784], 1
                        mov              rax, qword ptr [rip + .Lx157_0]
                        mov              qword ptr [rbp + 792], rax
                                                                                        jmp   n66_match_replace_α
n64_lit_string_β:
                                                                                        jmp   n9_var_α
.Lx157_0:
                        .quad            .Lx157_0_s
.Lx157_0_s:
                        .string          "#"
#-----------------------------------------------------------------------------------------------------------------------
n65_var_α:
                        mov              rax, qword ptr [1879052416]
                        mov              rdx, qword ptr [1879052424]
                        mov              qword ptr [rbp + 1296], rax
                        mov              qword ptr [rbp + 1304], rdx
                                                                                        jmp   n67_binop_α
n65_var_β:
                                                                                        jmp   n50_var_α
#-----------------------------------------------------------------------------------------------------------------------
n66_match_replace_α:
                        mov              rdi, qword ptr [rip + .Lx160_0]
                        mov              rsi, qword ptr [rbp + 848]
                        mov              rdx, qword ptr [rbp + 856]
                        mov              ecx, dword ptr [rbp + 688]
                        mov              r8, qword ptr [rbp + 712]
                        lea              r9, [rbp + 784]
                        call             rt_match_replace@PLT
                                                                                        jmp   .Lx160_1
.Lx160_0:
                        .quad            .Lx160_0_s
.Lx160_0_s:
                        .string          "PRINTV"
.Lx160_1:
                        mov              rbp, qword ptr [rbp + 728]
                                                                                        jmp   n9_var_α
#-----------------------------------------------------------------------------------------------------------------------
n67_binop_α:
                        mov              rdi, qword ptr [rbp + 1088]
                        mov              rsi, qword ptr [rbp + 1096]
                        mov              rdx, qword ptr [rbp + 1296]
                        mov              rcx, qword ptr [rbp + 1304]
                        call             str_concat_d@PLT
                        mov              qword ptr [rbp + 1072], rax
                        mov              qword ptr [rbp + 1080], rdx
                                                                                        jmp   n68_assign_α
n67_binop_β:
                                                                                        jmp   n50_var_α
#-----------------------------------------------------------------------------------------------------------------------
n68_assign_α:
                        mov              rsi, qword ptr [rbp + 1072]
                        mov              rdx, qword ptr [rbp + 1080]
                        mov              rdi, qword ptr [rip + .Lx162_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   n9_var_α
n68_assign_β:
                                                                                        jmp   n50_var_α
.Lx162_0:
                        .quad            .Lx162_0_s
.Lx162_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
                        mov              eax, 1
                        xor              edx, edx
                        mov              rsp, rbp
                        mov              rbp, [rsp + 1328]
                        add              rsp, 1336
                        ret
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              rsp, rbp
                        mov              eax, 99
                        xor              edx, edx
                        mov              rbp, [rsp + 1328]
                        add              rsp, 1336
                        ret
                        .section         .rodata
.S0:                    .string          "NV"
.S1:                    .string          "CROSS"
.S2:                    .string          "C"
.S3:                    .string          "NH"
                        .text
                        .section         .note.GNU-stack,"",@progbits
