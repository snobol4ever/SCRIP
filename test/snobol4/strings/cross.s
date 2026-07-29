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
                        sub              rsp, 1208
                        mov              rdi, rsp
                        mov              ecx, 1208
                        xor              eax, eax
                        rep stosb
main_α_body:
#=======================================================================================================================
#       &TRIM    =  1
#-----------------------------------------------------------------------------------------------------------------------
n0_lit_string_α:
                        mov              qword ptr [rsp + 64], 1
                        mov              rax, qword ptr [rip + .Lx69_0]
                        mov              qword ptr [rsp + 72], rax
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
                        mov              qword ptr [rsp + 80], 6
                        mov              rax, qword ptr [rip + .Lx74_0]
                        mov              qword ptr [rsp + 88], rax
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
                        mov              qword ptr [rsp + 384], rax
                        mov              qword ptr [rsp + 392], rdx
                                                                                        jmp   n12_var_α
n7_var_β:
                                                                                        jmp   n6_var_α
#=======================================================================================================================
# NEXTV VC       ?  @NV CROSS = '#'                  :F(NEXTH)
#-----------------------------------------------------------------------------------------------------------------------
n8_var_α:
                        mov              rax, qword ptr [1879052368]
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rsp + 560], rax
                        mov              qword ptr [rsp + 568], rdx
                                                                                        jmp   n13_match_head_α
n8_var_β:
                                                                                        jmp   n7_var_α
#=======================================================================================================================
# PRINT PRINTV   ?  LEN(1) . C =                     :F(NEXTV)
#-----------------------------------------------------------------------------------------------------------------------
n9_var_α:
                        mov              rax, qword ptr [1879052400]
                        mov              rdx, qword ptr [1879052408]
                        mov              qword ptr [rsp + 928], rax
                        mov              qword ptr [rsp + 936], rdx
                                                                                        jmp   n14_match_head_α
n9_var_β:
                                                                                        jmp   n8_var_α
#-----------------------------------------------------------------------------------------------------------------------
n10_call_α:
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [rsp + 40], rax
                        .section         .rodata
.Lrkfn80:               .string          "SNO$KWSET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn80]
                        lea              rsi, [rsp + 16]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
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
                        mov              qword ptr [rsp + 368], rax
                        mov              qword ptr [rsp + 376], rdx
                                                                                        jmp   n16_op73_α
n12_var_β:
                                                                                        jmp   n6_var_α
#-----------------------------------------------------------------------------------------------------------------------
n13_match_head_α:
                        mov              rdi, qword ptr [rsp + 560]
                        mov              rsi, qword ptr [rsp + 568]
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              rax, qword ptr [1879048192]
                        mov              qword ptr [rsp + 464], rax
                        mov              qword ptr [rsp + 448], rsp
                        lea              rcx, [rip + g_patstk_sp]
                        mov              rax, qword ptr [rcx + 0]
                        mov              qword ptr [rsp + 440], rax
                        mov              dword ptr [rsp + 432], 0
.Lx84_0:
                        mov              r14d, dword ptr [rsp + 432]
                                                                                        jmp   n17_match_sequence_α
n13_match_head_β:
                        add              dword ptr [rsp + 432], 1
                        mov              eax, dword ptr [rsp + 432]
                        cmp              eax, r15d
                                                                                        jg    .Lx84_1
                        lea              rcx, [rip + g_anchor]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                                                                                        jne   .Lx84_1
                                                                                        jmp   .Lx84_0
.Lx84_1:
                        mov              rax, qword ptr [rsp + 440]
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              rsp, qword ptr [rsp + 448]
                        mov              rax, qword ptr [rsp + 464]
                        mov              qword ptr [1879048192], rax
                                                                                        jmp   n7_var_α
#-----------------------------------------------------------------------------------------------------------------------
n14_match_head_α:
                        mov              rdi, qword ptr [rsp + 928]
                        mov              rsi, qword ptr [rsp + 936]
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              rax, qword ptr [1879048192]
                        mov              qword ptr [rsp + 816], rax
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
                        mov              rax, qword ptr [rsp + 816]
                        mov              qword ptr [1879048192], rax
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
                        lea              rdi, [rsp + 368]
                        lea              rsi, [rsp + 352]
                        mov              rdx, 3866683
                        call             rt_coerce_str_d@PLT
                                                                                        jmp   n20_match_head_α
n16_op73_β:
                                                                                        jmp   n6_var_α
#-----------------------------------------------------------------------------------------------------------------------
n17_match_sequence_α:
                        mov              dword ptr [rsp + 512], r14d
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
                        mov              rdi, qword ptr [rsp + 384]
                        mov              rsi, qword ptr [rsp + 392]
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              rax, qword ptr [1879048192]
                        mov              qword ptr [rsp + 208], rax
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
                        mov              rax, qword ptr [rsp + 208]
                        mov              qword ptr [1879048192], rax
                                                                                        jmp   n6_var_α
#-----------------------------------------------------------------------------------------------------------------------
n21_match_release_α:
                        mov              rax, qword ptr [rsp + 440]
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              qword ptr [rsp + 456], r14
                        mov              rsp, qword ptr [rsp + 448]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rdi, qword ptr [rsp + 496]
                        mov              rsi, qword ptr [1879048192]
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
                        mov              rax, qword ptr [rsp + 464]
                        mov              qword ptr [1879048192], rax
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
                        mov              qword ptr [rsp + 496], 1
                        mov              rax, qword ptr [rip + .Lx105_0]
                        mov              qword ptr [rsp + 504], rax
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
                        mov              rdi, qword ptr [rsp + 240]
                        mov              rsi, qword ptr [1879048192]
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
                        mov              rax, qword ptr [rsp + 208]
                        mov              qword ptr [1879048192], rax
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
                        mov              rsi, qword ptr [rsp + 560]
                        mov              rdx, qword ptr [rsp + 568]
                        mov              ecx, dword ptr [rsp + 432]
                        mov              r8, qword ptr [rsp + 456]
                        lea              r9, [rsp + 496]
                        call             rt_match_replace@PLT
                                                                                        jmp   .Lx117_1
.Lx117_0:
                        .quad            .Lx117_0_s
.Lx117_0_s:
                        .string          "VC"
.Lx117_1:
                                                                                        jmp   n38_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n35_match_release_α:
                        mov              rax, qword ptr [rsp + 24]
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              eax, dword ptr [rsp + 16]
                        mov              dword ptr [rsp + 832], eax
                        mov              qword ptr [rsp + 856], r14
                        mov              rsp, qword ptr [rsp + 32]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rdi, qword ptr [rsp + 848]
                        mov              rsi, qword ptr [1879048192]
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
                        mov              rax, qword ptr [rsp + 816]
                        mov              qword ptr [1879048192], rax
                                                                                        jmp   n39_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n36_lit_string_α:
                        mov              qword ptr [rsp + 240], 1
                        mov              rax, qword ptr [rip + .Lx120_0]
                        mov              qword ptr [rsp + 248], rax
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
                        mov              r8, qword ptr [rsp + 456]
                        mov              r9d, dword ptr [rsp + 452]
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
                        mov              qword ptr [rsp + 848], 1
                        mov              rax, qword ptr [rip + .Lx124_0]
                        mov              qword ptr [rsp + 856], rax
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
                        mov              rsi, qword ptr [rsp + 384]
                        mov              rdx, qword ptr [rsp + 392]
                        mov              ecx, dword ptr [rsp + 176]
                        mov              r8, qword ptr [rsp + 200]
                        lea              r9, [rsp + 240]
                        call             rt_match_replace@PLT
                                                                                        jmp   .Lx126_1
.Lx126_0:
                        .quad            .Lx126_0_s
.Lx126_0_s:
                        .string          "HC"
.Lx126_1:
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
                        mov              rsi, qword ptr [rsp + 928]
                        mov              rdx, qword ptr [rsp + 936]
                        mov              ecx, dword ptr [rsp + 784]
                        mov              r8, qword ptr [rsp + 808]
                        lea              r9, [rsp + 848]
                        call             rt_match_replace@PLT
                                                                                        jmp   .Lx129_1
.Lx129_0:
                        .quad            .Lx129_0_s
.Lx129_0_s:
                        .string          "PRINTV"
.Lx129_1:
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
                        mov              qword ptr [rsp + 1040], rax
                        mov              qword ptr [rsp + 1048], rdx
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
                        mov              qword ptr [rsp + 752], rax
                        mov              qword ptr [rsp + 760], rdx
                                                                                        jmp   n51_var_α
n48_var_β:
                                                                                        jmp   n9_var_α
#-----------------------------------------------------------------------------------------------------------------------
n49_lit_string_α:
                        mov              qword ptr [rsp + 1056], 1
                        mov              rax, qword ptr [rip + .Lx136_0]
                        mov              qword ptr [rsp + 1064], rax
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
                        mov              qword ptr [rsp + 736], rax
                        mov              qword ptr [rsp + 744], rdx
                                                                                        jmp   n54_op74_α
n51_var_β:
                                                                                        jmp   n9_var_α
#-----------------------------------------------------------------------------------------------------------------------
n52_call_α:
                        mov              rax, qword ptr [rsp + 1040]
                        mov              qword ptr [rsp + 992], rax
                        mov              rax, qword ptr [rsp + 1048]
                        mov              qword ptr [rsp + 1000], rax
                        mov              rax, qword ptr [rsp + 1056]
                        mov              qword ptr [rsp + 1008], rax
                        mov              rax, qword ptr [rsp + 1064]
                        mov              qword ptr [rsp + 1016], rax
                        .section         .rodata
.Lrkfn140:              .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn140]
                        lea              rsi, [rsp + 992]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 976], rax
                        mov              qword ptr [rsp + 984], rdx
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
                        lea              rdi, [rsp + 736]
                        lea              rsi, [rsp + 720]
                        mov              rdx, 10682530
                        call             rt_coerce_int_d@PLT
                                                                                        jmp   n56_match_head_α
n54_op74_β:
                                                                                        jmp   n9_var_α
#-----------------------------------------------------------------------------------------------------------------------
n55_lit_string_α:
                        mov              qword ptr [rsp + 1136], 1
                        mov              rax, qword ptr [rip + .Lx144_0]
                        mov              qword ptr [rsp + 1144], rax
                                                                                        jmp   n57_var_α
n55_lit_string_β:
                                                                                        jmp   n50_var_α
.Lx144_0:
                        .quad            .Lx144_0_s
.Lx144_0_s:
                        .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n56_match_head_α:
                        mov              rdi, qword ptr [rsp + 752]
                        mov              rsi, qword ptr [rsp + 760]
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              rax, qword ptr [1879048192]
                        mov              qword ptr [rsp + 656], rax
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
                        mov              rax, qword ptr [rsp + 656]
                        mov              qword ptr [1879048192], rax
                                                                                        jmp   n9_var_α
#-----------------------------------------------------------------------------------------------------------------------
n57_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 1152], rax
                        mov              qword ptr [rsp + 1160], rdx
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
                        mov              rax, qword ptr [rsp + 1136]
                        mov              qword ptr [rsp + 1088], rax
                        mov              rax, qword ptr [rsp + 1144]
                        mov              qword ptr [rsp + 1096], rax
                        mov              rax, qword ptr [rsp + 1152]
                        mov              qword ptr [rsp + 1104], rax
                        mov              rax, qword ptr [rsp + 1160]
                        mov              qword ptr [rsp + 1112], rax
                        .section         .rodata
.Lrkfn151:              .string          "DUPL"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn151]
                        lea              rsi, [rsp + 1088]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1072], rax
                        mov              qword ptr [rsp + 1080], rdx
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
                        mov              dword ptr [rsp + 656], eax
                        mov              qword ptr [rsp + 680], r14
                        mov              rsp, qword ptr [rsp + 16]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rdi, qword ptr [rsp + 688]
                        mov              rsi, qword ptr [1879048192]
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
                        mov              rax, qword ptr [rsp + 656]
                        mov              qword ptr [1879048192], rax
                                                                                        jmp   n64_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n61_match_pos_α:
                        mov              rax, qword ptr [rsp + 792]
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
                        mov              rdi, qword ptr [rsp + 976]
                        mov              rsi, qword ptr [rsp + 984]
                        mov              rdx, qword ptr [rsp + 1072]
                        mov              rcx, qword ptr [rsp + 1080]
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 960], rax
                        mov              qword ptr [rsp + 968], rdx
                                                                                        jmp   n65_var_α
n63_binop_β:
                                                                                        jmp   n50_var_α
#-----------------------------------------------------------------------------------------------------------------------
n64_lit_string_α:
                        mov              qword ptr [rsp + 688], 1
                        mov              rax, qword ptr [rip + .Lx157_0]
                        mov              qword ptr [rsp + 696], rax
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
                        mov              qword ptr [rsp + 1168], rax
                        mov              qword ptr [rsp + 1176], rdx
                                                                                        jmp   n67_binop_α
n65_var_β:
                                                                                        jmp   n50_var_α
#-----------------------------------------------------------------------------------------------------------------------
n66_match_replace_α:
                        mov              rdi, qword ptr [rip + .Lx160_0]
                        mov              rsi, qword ptr [rsp + 752]
                        mov              rdx, qword ptr [rsp + 760]
                        mov              ecx, dword ptr [rsp + 624]
                        mov              r8, qword ptr [rsp + 648]
                        lea              r9, [rsp + 688]
                        call             rt_match_replace@PLT
                                                                                        jmp   .Lx160_1
.Lx160_0:
                        .quad            .Lx160_0_s
.Lx160_0_s:
                        .string          "PRINTV"
.Lx160_1:
                                                                                        jmp   n9_var_α
#-----------------------------------------------------------------------------------------------------------------------
n67_binop_α:
                        mov              rdi, qword ptr [rsp + 960]
                        mov              rsi, qword ptr [rsp + 968]
                        mov              rdx, qword ptr [rsp + 1168]
                        mov              rcx, qword ptr [rsp + 1176]
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 944], rax
                        mov              qword ptr [rsp + 952], rdx
                                                                                        jmp   n68_assign_α
n67_binop_β:
                                                                                        jmp   n50_var_α
#-----------------------------------------------------------------------------------------------------------------------
n68_assign_α:
                        mov              rsi, qword ptr [rsp + 944]
                        mov              rdx, qword ptr [rsp + 952]
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
                        add              rsp, 1208
                        ret
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              eax, 99
                        xor              edx, edx
                        add              rsp, 1208
                        ret
                        .section         .rodata
.S0:                    .string          "NV"
.S1:                    .string          "CROSS"
.S2:                    .string          "C"
.S3:                    .string          "NH"
                        .text
                        .section         .note.GNU-stack,"",@progbits
