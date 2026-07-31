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
                        sub              rsp, 1304
                        mov              rdi, rsp
                        mov              ecx, 1304
                        xor              eax, eax
                        rep stosb
                        mov              [rsp + 1296], rbp
                        mov              rbp, rsp
main_α_body:
#=======================================================================================================================
#       &TRIM    =  1
#-----------------------------------------------------------------------------------------------------------------------
n0_lit_string_α:
                        mov              qword ptr [rsp + 64], 1
                        mov              dword ptr [rsp + 68], 4
                        mov              rax, qword ptr [rip + .Lx69_0]
                        mov              qword ptr [rsp + 72], rax
                                                                                        jmp   n1_lit_integer_α
n0_lit_string_β:
                                                                                        jmp   n3_var_α
.Lx69_0:
                        .quad            .Lx69_0_s
.Lx69_0_s:
                        .string          "TRIM"
#-----------------------------------------------------------------------------------------------------------------------
n1_lit_integer_α:
                        mov              qword ptr [rsp + 80], 6
                        mov              rax, qword ptr [rip + .Lx70_0]
                        mov              qword ptr [rsp + 88], rax
                                                                                        jmp   n2_call_α
n1_lit_integer_β:
                                                                                        jmp   n3_var_α
.Lx70_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n2_call_α:
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 24], rax
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [rsp + 40], rax
                        .section         .rodata
.Lrkfn72:               .string          "SNO$KWSET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn72]
                        lea              rsi, [rsp + 16]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 99
                                                                                        je    n3_var_α
                                                                                        jmp   n3_var_α
n2_call_β:
                                                                                        jmp   n3_var_α
#=======================================================================================================================
# AGAIN H        =  INPUT                            :F(END)
#-----------------------------------------------------------------------------------------------------------------------
n3_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx73_0]
                        call             NV_GET_fn@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx73_240
                        add              rsp, 16
                                                                                        jmp   main_γ
.Lx73_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n4_assign_α
n3_var_β:
                        add              rsp, 16
                                                                                        jmp   main_γ
.Lx73_0:
                        .quad            .Lx73_0_s
.Lx73_0_s:
                        .string          "INPUT"
#-----------------------------------------------------------------------------------------------------------------------
n4_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   n5_var_α
n4_assign_β:
                        add              rsp, 16
                                                                                        jmp   main_γ
#=======================================================================================================================
#       V        =  INPUT                            :F(END)
#-----------------------------------------------------------------------------------------------------------------------
n5_var_α:
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
                                                                                        jmp   n6_assign_α
n5_var_β:
                        add              rsp, 16
                                                                                        jmp   main_γ
.Lx75_0:
                        .quad            .Lx75_0_s
.Lx75_0_s:
                        .string          "INPUT"
#-----------------------------------------------------------------------------------------------------------------------
n6_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                        add              rsp, 16
                                                                                        jmp   n7_var_α
n6_assign_β:
                        add              rsp, 16
                                                                                        jmp   main_γ
#=======================================================================================================================
#       HC       =  H
#-----------------------------------------------------------------------------------------------------------------------
n7_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n8_assign_α
n7_var_β:
                        add              rsp, 16
                                                                                        jmp   n9_var_α
#-----------------------------------------------------------------------------------------------------------------------
n8_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052320], rax
                        mov              qword ptr [1879052328], rdx
                        add              rsp, 16
                                                                                        jmp   n9_var_α
n8_assign_β:
                        add              rsp, 16
                                                                                        jmp   n9_var_α
#=======================================================================================================================
# NEXTH HC       ?  @NH ANY(V) . CROSS = '*'         :F(AGAIN)
#-----------------------------------------------------------------------------------------------------------------------
n9_var_α:
                        mov              rax, qword ptr [1879052320]
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rsp + 400], rax
                        mov              qword ptr [rsp + 408], rdx
                                                                                        jmp   n10_var_α
n9_var_β:
                                                                                        jmp   n3_var_α
#-----------------------------------------------------------------------------------------------------------------------
n10_var_α:
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 384], rax
                        mov              qword ptr [rsp + 392], rdx
                                                                                        jmp   n11_coerce_string_α
n10_var_β:
                                                                                        jmp   n3_var_α
#-----------------------------------------------------------------------------------------------------------------------
n11_coerce_string_α:
                        sub              rsp, 16
                        lea              rdi, [rsp + 400]
                        lea              rsi, [rsp + 384]
                        mov              rdx, 3866683
                        call             rt_coerce_str_d@PLT
                        add              rsp, 16
                                                                                        jmp   n12_match_head_α
n11_coerce_string_β:
                        sub              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n3_var_α
#-----------------------------------------------------------------------------------------------------------------------
n12_match_head_α:
                        mov              qword ptr [rbp + 224], r13
                        mov              qword ptr [rbp + 232], r14
                        mov              qword ptr [rbp + 240], r15
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rbp + 248], rax
                        mov              qword ptr [rbp + 216], rbp
                        mov              rdi, qword ptr [rbp + 400]
                        mov              rsi, qword ptr [rbp + 408]
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
.Lx84_0:
                        mov              r14d, dword ptr [rsp + 0]
                                                                                        jmp   n13_match_sequence_α
n12_match_head_β:
                        add              dword ptr [rsp + 0], 1
                        mov              eax, dword ptr [rsp + 0]
                        cmp              eax, r15d
                                                                                        jg    .Lx84_1
                        lea              rcx, [rip + g_anchor]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                                                                                        jne   .Lx84_1
                                                                                        jmp   .Lx84_0
.Lx84_1:
                        mov              rax, qword ptr [rsp + 8]
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              rsp, qword ptr [rsp + 16]
                        mov              r10, qword ptr [1879048192]
.Lx84_2:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx84_2
                        mov              qword ptr [1879048192], r10
                        mov              r13, qword ptr [rbp + 224]
                        mov              r14, qword ptr [rbp + 232]
                        mov              r15, qword ptr [rbp + 240]
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              rdx, qword ptr [rbp + 248]
                        call             rt_match_ctx_restore@PLT
                        mov              rbp, qword ptr [rbp + 216]
                                                                                        jmp   n3_var_α
#-----------------------------------------------------------------------------------------------------------------------
n13_match_sequence_α:
                                                                                        jmp   n64_match_atp_α
n13_match_sequence_as:
                                                                                        jmp   n14_match_release_α
n13_match_sequence_β:
                                                                                        jmp   n63_match_assign_cond_β
n13_match_sequence_af:
                                                                                        jmp   n12_match_head_β
#-----------------------------------------------------------------------------------------------------------------------
n14_match_release_α:
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
.Lx88_5:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx88_5
                        lea              rdi, [r10 + 24]
                        mov              rdx, r13
                        call             rt_dcap_end_ok_open@PLT
.Lx88_1:
                        test             rax, rax
                                                                                        je    .Lx88_2
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx88_3]
                        lea              rdx, [rip + .Lx88_4]
                                                                                        jmp   rax
.Lx88_3:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx88_1
.Lx88_4:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx88_1
.Lx88_2:
                        call             rt_dcap_end_ok_close@PLT
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              r10, qword ptr [1879048192]
.Lx88_6:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx88_6
                        mov              qword ptr [1879048192], r10
                        mov              r13, qword ptr [rbp + 224]
                        mov              r14, qword ptr [rbp + 232]
                        mov              r15, qword ptr [rbp + 240]
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              rdx, qword ptr [rbp + 248]
                        call             rt_match_ctx_restore@PLT
                                                                                        jmp   n15_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n15_lit_string_α:
                        mov              qword ptr [rsp + 272], 1
                        mov              dword ptr [rsp + 276], 1
                        mov              rax, qword ptr [rip + .Lx89_0]
                        mov              qword ptr [rsp + 280], rax
                                                                                        jmp   n16_match_replace_α
n15_lit_string_β:
                                                                                        jmp   n3_var_α
.Lx89_0:
                        .quad            .Lx89_0_s
.Lx89_0_s:
                        .string          "*"
#-----------------------------------------------------------------------------------------------------------------------
n16_match_replace_α:
                        mov              rdi, qword ptr [rip + .Lx91_0]
                        mov              rsi, qword ptr [rbp + 400]
                        mov              rdx, qword ptr [rbp + 408]
                        mov              ecx, dword ptr [rbp + 176]
                        mov              r8, qword ptr [rbp + 200]
                        lea              r9, [rbp + 272]
                        call             rt_match_replace@PLT
                                                                                        jmp   .Lx91_1
.Lx91_0:
                        .quad            .Lx91_0_s
.Lx91_0_s:
                        .string          "HC"
.Lx91_1:
                        mov              rbp, qword ptr [rbp + 216]
                                                                                        jmp   n17_var_α
#=======================================================================================================================
#       VC       =  V
#-----------------------------------------------------------------------------------------------------------------------
n17_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n18_assign_α
n17_var_β:
                        add              rsp, 16
                                                                                        jmp   n19_var_α
#-----------------------------------------------------------------------------------------------------------------------
n18_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052368], rax
                        mov              qword ptr [1879052376], rdx
                        add              rsp, 16
                                                                                        jmp   n19_var_α
n18_assign_β:
                        add              rsp, 16
                                                                                        jmp   n19_var_α
#=======================================================================================================================
# NEXTV VC       ?  @NV CROSS = '#'                  :F(NEXTH)
#-----------------------------------------------------------------------------------------------------------------------
n19_var_α:
                        mov              rax, qword ptr [1879052368]
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rsp + 608], rax
                        mov              qword ptr [rsp + 616], rdx
                                                                                        jmp   n20_match_head_α
n19_var_β:
                                                                                        jmp   n9_var_α
#-----------------------------------------------------------------------------------------------------------------------
n20_match_head_α:
                        mov              qword ptr [rbp + 496], r13
                        mov              qword ptr [rbp + 504], r14
                        mov              qword ptr [rbp + 512], r15
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rbp + 520], rax
                        mov              qword ptr [rbp + 488], rbp
                        mov              rdi, qword ptr [rbp + 608]
                        mov              rsi, qword ptr [rbp + 616]
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              r10, qword ptr [1879048192]
                        mov              qword ptr [r10 + 0], 0
                        add              r10, 24
                        mov              qword ptr [1879048192], r10
                        mov              qword ptr [rbp + 464], rsp
                        lea              rcx, [rip + g_patstk_sp]
                        mov              rax, qword ptr [rcx + 0]
                        mov              qword ptr [rbp + 456], rax
                        mov              dword ptr [rbp + 448], 0
.Lx96_0:
                        mov              r14d, dword ptr [rbp + 448]
                                                                                        jmp   n21_match_sequence_α
n20_match_head_β:
                        add              dword ptr [rbp + 448], 1
                        mov              eax, dword ptr [rbp + 448]
                        cmp              eax, r15d
                                                                                        jg    .Lx96_1
                        lea              rcx, [rip + g_anchor]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                                                                                        jne   .Lx96_1
                                                                                        jmp   .Lx96_0
.Lx96_1:
                        mov              rax, qword ptr [rbp + 456]
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              rsp, qword ptr [rbp + 464]
                        mov              r10, qword ptr [1879048192]
.Lx96_2:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx96_2
                        mov              qword ptr [1879048192], r10
                        mov              r13, qword ptr [rbp + 496]
                        mov              r14, qword ptr [rbp + 504]
                        mov              r15, qword ptr [rbp + 512]
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              rdx, qword ptr [rbp + 520]
                        call             rt_match_ctx_restore@PLT
                        mov              rbp, qword ptr [rbp + 488]
                                                                                        jmp   n9_var_α
#-----------------------------------------------------------------------------------------------------------------------
n21_match_sequence_α:
                        mov              dword ptr [rbp + 560], r14d
                                                                                        jmp   n60_match_atp_α
n21_match_sequence_as:
                                                                                        jmp   n22_match_release_α
n21_match_sequence_β:
                                                                                        jmp   n59_match_patref_β
n21_match_sequence_af:
                                                                                        jmp   n20_match_head_β
#-----------------------------------------------------------------------------------------------------------------------
n22_match_release_α:
                        mov              rax, qword ptr [rbp + 456]
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              qword ptr [rbp + 472], r14
                        mov              rsp, qword ptr [rbp + 464]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, qword ptr [1879048192]
                        mov              r10, rsi
.Lx100_5:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx100_5
                        lea              rdi, [r10 + 24]
                        mov              rdx, r13
                        call             rt_dcap_end_ok_open@PLT
.Lx100_1:
                        test             rax, rax
                                                                                        je    .Lx100_2
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx100_3]
                        lea              rdx, [rip + .Lx100_4]
                                                                                        jmp   rax
.Lx100_3:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx100_1
.Lx100_4:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx100_1
.Lx100_2:
                        call             rt_dcap_end_ok_close@PLT
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              r10, qword ptr [1879048192]
.Lx100_6:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx100_6
                        mov              qword ptr [1879048192], r10
                        mov              r13, qword ptr [rbp + 496]
                        mov              r14, qword ptr [rbp + 504]
                        mov              r15, qword ptr [rbp + 512]
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              rdx, qword ptr [rbp + 520]
                        call             rt_match_ctx_restore@PLT
                                                                                        jmp   n23_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n23_lit_string_α:
                        mov              qword ptr [rsp + 544], 1
                        mov              dword ptr [rsp + 548], 1
                        mov              rax, qword ptr [rip + .Lx101_0]
                        mov              qword ptr [rsp + 552], rax
                                                                                        jmp   n24_match_replace_α
n23_lit_string_β:
                                                                                        jmp   n9_var_α
.Lx101_0:
                        .quad            .Lx101_0_s
.Lx101_0_s:
                        .string          "#"
#-----------------------------------------------------------------------------------------------------------------------
n24_match_replace_α:
                        mov              rdi, qword ptr [rip + .Lx103_0]
                        mov              rsi, qword ptr [rbp + 608]
                        mov              rdx, qword ptr [rbp + 616]
                        mov              ecx, dword ptr [rbp + 448]
                        mov              r8, qword ptr [rbp + 472]
                        lea              r9, [rbp + 544]
                        call             rt_match_replace@PLT
                                                                                        jmp   .Lx103_1
.Lx103_0:
                        .quad            .Lx103_0_s
.Lx103_0_s:
                        .string          "VC"
.Lx103_1:
                        mov              rbp, qword ptr [rbp + 488]
                                                                                        jmp   n25_lit_string_α
#=======================================================================================================================
#       OUTPUT   =
#-----------------------------------------------------------------------------------------------------------------------
n25_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx104_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n26_assign_α
n25_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n27_var_α
.Lx104_0:
                        .quad            .Lx104_0_s
.Lx104_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n26_assign_α:
                        mov              rsi, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx105_0]
                        call             NV_SET_fn@PLT
                        add              rsp, 16
                                                                                        jmp   n27_var_α
n26_assign_β:
                        add              rsp, 16
                                                                                        jmp   n27_var_α
.Lx105_0:
                        .quad            .Lx105_0_s
.Lx105_0_s:
                        .string          "OUTPUT"
#=======================================================================================================================
#       PRINTV   =  V
#-----------------------------------------------------------------------------------------------------------------------
n27_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n28_assign_α
n27_var_β:
                        add              rsp, 16
                                                                                        jmp   n29_var_α
#-----------------------------------------------------------------------------------------------------------------------
n28_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052400], rax
                        mov              qword ptr [1879052408], rdx
                        add              rsp, 16
                                                                                        jmp   n29_var_α
n28_assign_β:
                        add              rsp, 16
                                                                                        jmp   n29_var_α
#=======================================================================================================================
#       PRINTV   ?  POS(NV) LEN(1) = '#'
#-----------------------------------------------------------------------------------------------------------------------
n29_var_α:
                        mov              rax, qword ptr [1879052400]
                        mov              rdx, qword ptr [1879052408]
                        mov              qword ptr [rsp + 832], rax
                        mov              qword ptr [rsp + 840], rdx
                                                                                        jmp   n30_var_α
n29_var_β:
                                                                                        jmp   n39_var_α
#-----------------------------------------------------------------------------------------------------------------------
n30_var_α:
                        mov              rax, qword ptr [1879052384]
                        mov              rdx, qword ptr [1879052392]
                        mov              qword ptr [rsp + 816], rax
                        mov              qword ptr [rsp + 824], rdx
                                                                                        jmp   n31_coerce_integer_α
n30_var_β:
                                                                                        jmp   n39_var_α
#-----------------------------------------------------------------------------------------------------------------------
n31_coerce_integer_α:
                        sub              rsp, 16
                        lea              rdi, [rsp + 832]
                        lea              rsi, [rsp + 816]
                        mov              rdx, 10682530
                        call             rt_coerce_int_d@PLT
                        add              rsp, 16
                                                                                        jmp   n32_match_head_α
n31_coerce_integer_β:
                        sub              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n39_var_α
#-----------------------------------------------------------------------------------------------------------------------
n32_match_head_α:
                        mov              qword ptr [rbp + 720], r13
                        mov              qword ptr [rbp + 728], r14
                        mov              qword ptr [rbp + 736], r15
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rbp + 744], rax
                        mov              qword ptr [rbp + 712], rbp
                        mov              rdi, qword ptr [rbp + 832]
                        mov              rsi, qword ptr [rbp + 840]
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
.Lx113_0:
                        mov              r14d, dword ptr [rsp + 0]
                                                                                        jmp   n33_match_sequence_α
n32_match_head_β:
                        add              dword ptr [rsp + 0], 1
                        mov              eax, dword ptr [rsp + 0]
                        cmp              eax, r15d
                                                                                        jg    .Lx113_1
                        lea              rcx, [rip + g_anchor]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                                                                                        jne   .Lx113_1
                                                                                        jmp   .Lx113_0
.Lx113_1:
                        mov              rax, qword ptr [rsp + 8]
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              rsp, qword ptr [rsp + 16]
                        mov              r10, qword ptr [1879048192]
.Lx113_2:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx113_2
                        mov              qword ptr [1879048192], r10
                        mov              r13, qword ptr [rbp + 720]
                        mov              r14, qword ptr [rbp + 728]
                        mov              r15, qword ptr [rbp + 736]
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              rdx, qword ptr [rbp + 744]
                        call             rt_match_ctx_restore@PLT
                        mov              rbp, qword ptr [rbp + 712]
                                                                                        jmp   n39_var_α
#-----------------------------------------------------------------------------------------------------------------------
n33_match_sequence_α:
                                                                                        jmp   n38_match_pos_α
n33_match_sequence_as:
                                                                                        jmp   n34_match_release_α
n33_match_sequence_β:
                                                                                        jmp   n37_match_len_β
n33_match_sequence_af:
                                                                                        jmp   n32_match_head_β
#-----------------------------------------------------------------------------------------------------------------------
n34_match_release_α:
                        mov              rax, qword ptr [rsp + 8]
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              eax, dword ptr [rsp + 0]
                        mov              dword ptr [rsp + 704], eax
                        mov              qword ptr [rsp + 728], r14
                        mov              rsp, qword ptr [rsp + 16]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, qword ptr [1879048192]
                        mov              r10, rsi
.Lx117_5:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx117_5
                        lea              rdi, [r10 + 24]
                        mov              rdx, r13
                        call             rt_dcap_end_ok_open@PLT
.Lx117_1:
                        test             rax, rax
                                                                                        je    .Lx117_2
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx117_3]
                        lea              rdx, [rip + .Lx117_4]
                                                                                        jmp   rax
.Lx117_3:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx117_1
.Lx117_4:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx117_1
.Lx117_2:
                        call             rt_dcap_end_ok_close@PLT
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              r10, qword ptr [1879048192]
.Lx117_6:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx117_6
                        mov              qword ptr [1879048192], r10
                        mov              r13, qword ptr [rbp + 720]
                        mov              r14, qword ptr [rbp + 728]
                        mov              r15, qword ptr [rbp + 736]
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              rdx, qword ptr [rbp + 744]
                        call             rt_match_ctx_restore@PLT
                                                                                        jmp   n35_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n35_lit_string_α:
                        mov              qword ptr [rsp + 768], 1
                        mov              dword ptr [rsp + 772], 1
                        mov              rax, qword ptr [rip + .Lx118_0]
                        mov              qword ptr [rsp + 776], rax
                                                                                        jmp   n36_match_replace_α
n35_lit_string_β:
                                                                                        jmp   n39_var_α
.Lx118_0:
                        .quad            .Lx118_0_s
.Lx118_0_s:
                        .string          "#"
#-----------------------------------------------------------------------------------------------------------------------
n36_match_replace_α:
                        mov              rdi, qword ptr [rip + .Lx120_0]
                        mov              rsi, qword ptr [rbp + 832]
                        mov              rdx, qword ptr [rbp + 840]
                        mov              ecx, dword ptr [rbp + 672]
                        mov              r8, qword ptr [rbp + 696]
                        lea              r9, [rbp + 768]
                        call             rt_match_replace@PLT
                                                                                        jmp   .Lx120_1
.Lx120_0:
                        .quad            .Lx120_0_s
.Lx120_0_s:
                        .string          "PRINTV"
.Lx120_1:
                        mov              rbp, qword ptr [rbp + 712]
                                                                                        jmp   n39_var_α
#-----------------------------------------------------------------------------------------------------------------------
n37_match_len_α:
                        mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d
                                                                                        jg    n32_match_head_β
                        add              r14d, 1
                                                                                        jmp   n34_match_release_α
n37_match_len_β:
                        sub              r14d, 1
                                                                                        jmp   n32_match_head_β
#-----------------------------------------------------------------------------------------------------------------------
n38_match_pos_α:
                        mov              rax, qword ptr [rbp + 808]
                        cmp              r14d, eax
                                                                                        jne   n32_match_head_β
                                                                                        jmp   n37_match_len_α
n38_match_pos_β:
                                                                                        jmp   n32_match_head_β
#=======================================================================================================================
# PRINT PRINTV   ?  LEN(1) . C =                     :F(NEXTV)
#-----------------------------------------------------------------------------------------------------------------------
n39_var_α:
                        mov              rax, qword ptr [1879052400]
                        mov              rdx, qword ptr [1879052408]
                        mov              qword ptr [rsp + 1024], rax
                        mov              qword ptr [rsp + 1032], rdx
                                                                                        jmp   n40_match_head_α
n39_var_β:
                                                                                        jmp   n19_var_α
#-----------------------------------------------------------------------------------------------------------------------
n40_match_head_α:
                        mov              qword ptr [rbp + 912], r13
                        mov              qword ptr [rbp + 920], r14
                        mov              qword ptr [rbp + 928], r15
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rbp + 936], rax
                        mov              qword ptr [rbp + 904], rbp
                        mov              rdi, qword ptr [rbp + 1024]
                        mov              rsi, qword ptr [rbp + 1032]
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
.Lx125_0:
                        mov              r14d, dword ptr [rsp + 0]
                                                                                        jmp   n41_match_assign_save_α
n40_match_head_β:
                        add              dword ptr [rsp + 0], 1
                        mov              eax, dword ptr [rsp + 0]
                        cmp              eax, r15d
                                                                                        jg    .Lx125_1
                        lea              rcx, [rip + g_anchor]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                                                                                        jne   .Lx125_1
                                                                                        jmp   .Lx125_0
.Lx125_1:
                        mov              rax, qword ptr [rsp + 8]
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              rsp, qword ptr [rsp + 16]
                        mov              r10, qword ptr [1879048192]
.Lx125_2:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx125_2
                        mov              qword ptr [1879048192], r10
                        mov              r13, qword ptr [rbp + 912]
                        mov              r14, qword ptr [rbp + 920]
                        mov              r15, qword ptr [rbp + 928]
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              rdx, qword ptr [rbp + 936]
                        call             rt_match_ctx_restore@PLT
                        mov              rbp, qword ptr [rbp + 904]
                                                                                        jmp   n19_var_α
#-----------------------------------------------------------------------------------------------------------------------
n41_match_assign_save_α:
                        sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d
                                                                                        jmp   n42_match_len_α
n41_match_assign_save_β:
                        add              rsp, 16
                                                                                        jmp   n40_match_head_β
#-----------------------------------------------------------------------------------------------------------------------
n42_match_len_α:
                        mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d
                                                                                        jle   .Lx128_240
                        add              rsp, 16
                                                                                        jmp   n40_match_head_β
.Lx128_240:
                        add              r14d, 1
                                                                                        jmp   n43_match_assign_cond_α
n42_match_len_β:
                        sub              r14d, 1
                        add              rsp, 16
                                                                                        jmp   n40_match_head_β
#-----------------------------------------------------------------------------------------------------------------------
n43_match_assign_cond_α:
                        mov              eax, dword ptr [rsp + 0]
                        lea              rcx, [rip + .S0]
                        mov              r10, qword ptr [1879048192]
                        mov              qword ptr [r10 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r10 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r10 + 16], rdx
                        add              r10, 24
                        mov              qword ptr [1879048192], r10
                                                                                        jmp   n44_match_release_α
n43_match_assign_cond_β:
                        mov              rax, qword ptr [1879048192]
                        sub              rax, 24
                        mov              qword ptr [1879048192], rax
                                                                                        jmp   n42_match_len_β
#-----------------------------------------------------------------------------------------------------------------------
n44_match_release_α:
                        mov              rax, qword ptr [rsp + 24]
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              eax, dword ptr [rsp + 16]
                        mov              dword ptr [rsp + 912], eax
                        mov              qword ptr [rsp + 936], r14
                        mov              rsp, qword ptr [rsp + 32]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, qword ptr [1879048192]
                        mov              r10, rsi
.Lx132_5:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx132_5
                        lea              rdi, [r10 + 24]
                        mov              rdx, r13
                        call             rt_dcap_end_ok_open@PLT
.Lx132_1:
                        test             rax, rax
                                                                                        je    .Lx132_2
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx132_3]
                        lea              rdx, [rip + .Lx132_4]
                                                                                        jmp   rax
.Lx132_3:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx132_1
.Lx132_4:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx132_1
.Lx132_2:
                        call             rt_dcap_end_ok_close@PLT
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              r10, qword ptr [1879048192]
.Lx132_6:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx132_6
                        mov              qword ptr [1879048192], r10
                        mov              r13, qword ptr [rbp + 912]
                        mov              r14, qword ptr [rbp + 920]
                        mov              r15, qword ptr [rbp + 928]
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              rdx, qword ptr [rbp + 936]
                        call             rt_match_ctx_restore@PLT
                                                                                        jmp   n45_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n45_lit_string_α:
                        mov              qword ptr [rsp + 960], 1
                        mov              dword ptr [rsp + 964], 0
                        mov              rax, qword ptr [rip + .Lx133_0]
                        mov              qword ptr [rsp + 968], rax
                                                                                        jmp   n46_match_replace_α
n45_lit_string_β:
                                                                                        jmp   n19_var_α
.Lx133_0:
                        .quad            .Lx133_0_s
.Lx133_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n46_match_replace_α:
                        mov              rdi, qword ptr [rip + .Lx135_0]
                        mov              rsi, qword ptr [rbp + 1024]
                        mov              rdx, qword ptr [rbp + 1032]
                        mov              ecx, dword ptr [rbp + 864]
                        mov              r8, qword ptr [rbp + 888]
                        lea              r9, [rbp + 960]
                        call             rt_match_replace@PLT
                                                                                        jmp   .Lx135_1
.Lx135_0:
                        .quad            .Lx135_0_s
.Lx135_0_s:
                        .string          "PRINTV"
.Lx135_1:
                        mov              rbp, qword ptr [rbp + 904]
                                                                                        jmp   n47_var_α
#=======================================================================================================================
#       OUTPUT   =  DIFFER(C, '#') DUPL(' ', NH) C   :S(PRINT)
#-----------------------------------------------------------------------------------------------------------------------
n47_var_α:
                        mov              rax, qword ptr [1879052416]
                        mov              rdx, qword ptr [1879052424]
                        mov              qword ptr [rsp + 1136], rax
                        mov              qword ptr [rsp + 1144], rdx
                                                                                        jmp   n48_lit_string_α
n47_var_β:
                                                                                        jmp   n57_var_α
#-----------------------------------------------------------------------------------------------------------------------
n48_lit_string_α:
                        mov              qword ptr [rsp + 1152], 1
                        mov              dword ptr [rsp + 1156], 1
                        mov              rax, qword ptr [rip + .Lx137_0]
                        mov              qword ptr [rsp + 1160], rax
                                                                                        jmp   n49_call_α
n48_lit_string_β:
                                                                                        jmp   n57_var_α
.Lx137_0:
                        .quad            .Lx137_0_s
.Lx137_0_s:
                        .string          "#"
#-----------------------------------------------------------------------------------------------------------------------
n49_call_α:
                        mov              rax, qword ptr [rsp + 1136]
                        mov              qword ptr [rsp + 1088], rax
                        mov              rax, qword ptr [rsp + 1144]
                        mov              qword ptr [rsp + 1096], rax
                        mov              rax, qword ptr [rsp + 1152]
                        mov              qword ptr [rsp + 1104], rax
                        mov              rax, qword ptr [rsp + 1160]
                        mov              qword ptr [rsp + 1112], rax
                        .section         .rodata
.Lrkfn139:              .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn139]
                        lea              rsi, [rsp + 1088]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1072], rax
                        mov              qword ptr [rsp + 1080], rdx
                        cmp              eax, 99
                                                                                        je    n57_var_α
                                                                                        jmp   n50_lit_string_α
n49_call_β:
                                                                                        jmp   n57_var_α
#-----------------------------------------------------------------------------------------------------------------------
n50_lit_string_α:
                        mov              qword ptr [rsp + 1232], 1
                        mov              dword ptr [rsp + 1236], 1
                        mov              rax, qword ptr [rip + .Lx140_0]
                        mov              qword ptr [rsp + 1240], rax
                                                                                        jmp   n51_var_α
n50_lit_string_β:
                                                                                        jmp   n57_var_α
.Lx140_0:
                        .quad            .Lx140_0_s
.Lx140_0_s:
                        .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n51_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 1248], rax
                        mov              qword ptr [rsp + 1256], rdx
                                                                                        jmp   n52_call_α
n51_var_β:
                                                                                        jmp   n57_var_α
#-----------------------------------------------------------------------------------------------------------------------
n52_call_α:
                        mov              rax, qword ptr [rsp + 1232]
                        mov              qword ptr [rsp + 1184], rax
                        mov              rax, qword ptr [rsp + 1240]
                        mov              qword ptr [rsp + 1192], rax
                        mov              rax, qword ptr [rsp + 1248]
                        mov              qword ptr [rsp + 1200], rax
                        mov              rax, qword ptr [rsp + 1256]
                        mov              qword ptr [rsp + 1208], rax
                        .section         .rodata
.Lrkfn143:              .string          "DUPL"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn143]
                        lea              rsi, [rsp + 1184]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 1168], rax
                        mov              qword ptr [rsp + 1176], rdx
                        cmp              eax, 99
                                                                                        je    n57_var_α
                                                                                        jmp   n53_binop_α
n52_call_β:
                                                                                        jmp   n57_var_α
#-----------------------------------------------------------------------------------------------------------------------
n53_binop_α:
                        mov              rdi, qword ptr [rsp + 1072]
                        mov              rsi, qword ptr [rsp + 1080]
                        mov              rdx, qword ptr [rsp + 1168]
                        mov              rcx, qword ptr [rsp + 1176]
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 1056], rax
                        mov              qword ptr [rsp + 1064], rdx
                                                                                        jmp   n54_var_α
n53_binop_β:
                                                                                        jmp   n57_var_α
#-----------------------------------------------------------------------------------------------------------------------
n54_var_α:
                        mov              rax, qword ptr [1879052416]
                        mov              rdx, qword ptr [1879052424]
                        mov              qword ptr [rsp + 1264], rax
                        mov              qword ptr [rsp + 1272], rdx
                                                                                        jmp   n55_binop_α
n54_var_β:
                                                                                        jmp   n57_var_α
#-----------------------------------------------------------------------------------------------------------------------
n55_binop_α:
                        mov              rdi, qword ptr [rsp + 1056]
                        mov              rsi, qword ptr [rsp + 1064]
                        mov              rdx, qword ptr [rsp + 1264]
                        mov              rcx, qword ptr [rsp + 1272]
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 1040], rax
                        mov              qword ptr [rsp + 1048], rdx
                                                                                        jmp   n56_assign_α
n55_binop_β:
                                                                                        jmp   n57_var_α
#-----------------------------------------------------------------------------------------------------------------------
n56_assign_α:
                        mov              rsi, qword ptr [rsp + 1040]
                        mov              rdx, qword ptr [rsp + 1048]
                        mov              rdi, qword ptr [rip + .Lx147_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   n39_var_α
n56_assign_β:
                                                                                        jmp   n57_var_α
.Lx147_0:
                        .quad            .Lx147_0_s
.Lx147_0_s:
                        .string          "OUTPUT"
#=======================================================================================================================
#       OUTPUT   =  H                                :(PRINT)
#-----------------------------------------------------------------------------------------------------------------------
n57_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n58_assign_α
n57_var_β:
                        add              rsp, 16
                                                                                        jmp   n39_var_α
#-----------------------------------------------------------------------------------------------------------------------
n58_assign_α:
                        mov              rsi, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx149_0]
                        call             NV_SET_fn@PLT
                        add              rsp, 16
                                                                                        jmp   n39_var_α
n58_assign_β:
                        add              rsp, 16
                                                                                        jmp   n39_var_α
.Lx149_0:
                        .quad            .Lx149_0_s
.Lx149_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n59_match_patref_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        cmp              eax, 3
                                                                                        jne   .Lx150_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx150_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx150_10
.Lx150_9:
                        xor              eax, eax
.Lx150_10:
                        test             rax, rax
                                                                                        jz    .Lx150_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx150_4]
                        lea              rdx, [rip + .Lx150_5]
                                                                                        jmp   rax
.Lx150_4:
                                                                                        jmp   n22_match_release_α
.Lx150_5:
                                                                                        jmp   n60_match_atp_β
.Lx150_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S1]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx150_2:
                        test             rax, rax
                                                                                        je    .Lx150_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx150_7]
                        lea              rdx, [rip + .Lx150_8]
                                                                                        jmp   rax
.Lx150_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx150_2
.Lx150_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx150_2
.Lx150_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n60_match_atp_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx150_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n22_match_release_α
.Lx150_6:
                        add              rsp, 16
                                                                                        jmp   n60_match_atp_β
n59_match_patref_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n60_match_atp_α:
                        mov              esi, r14d
                        lea              rdi, [rip + .S2]
                        call             rt_at_cursor@PLT
                                                                                        jmp   n59_match_patref_α
n60_match_atp_β:
                                                                                        jmp   n20_match_head_β
#-----------------------------------------------------------------------------------------------------------------------
n61_match_assign_save_α:
                        sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d
                                                                                        jmp   n62_match_any_α
n61_match_assign_save_β:
                        add              rsp, 16
                                                                                        jmp   n64_match_atp_β
#-----------------------------------------------------------------------------------------------------------------------
n62_match_any_α:
                        mov              eax, r14d
                        cmp              eax, r15d
                                                                                        jl    .Lx155_239
                        add              rsp, 16
                                                                                        jmp   n64_match_atp_β
.Lx155_239:
                        movsxd           rcx, r14d
                        movzx            esi, byte ptr [r13+rcx]
                        mov              r8, qword ptr [rbp + 376]
                        mov              r9d, dword ptr [rbp + 372]
                        mov              edx, 0
.Lx155_5:
                        cmp              edx, r9d
                                                                                        jl    .Lx155_240
                        add              rsp, 16
                                                                                        jmp   n64_match_atp_β
.Lx155_240:
                        movzx            edi, byte ptr [r8 + rdx]
                        cmp              esi, edi
                                                                                        je    .Lx155_6
                        add              edx, 1
                                                                                        jmp   .Lx155_5
.Lx155_6:
                        add              r14d, 1
                                                                                        jmp   n63_match_assign_cond_α
n62_match_any_β:
                        sub              r14d, 1
                        add              rsp, 16
                                                                                        jmp   n64_match_atp_β
#-----------------------------------------------------------------------------------------------------------------------
n63_match_assign_cond_α:
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
                                                                                        jmp   n14_match_release_α
n63_match_assign_cond_β:
                        mov              rax, qword ptr [1879048192]
                        sub              rax, 24
                        mov              qword ptr [1879048192], rax
                                                                                        jmp   n62_match_any_β
#-----------------------------------------------------------------------------------------------------------------------
n64_match_atp_α:
                        mov              esi, r14d
                        lea              rdi, [rip + .S3]
                        call             rt_at_cursor@PLT
                                                                                        jmp   n61_match_assign_save_α
n64_match_atp_β:
                                                                                        jmp   n12_match_head_β
#-----------------------------------------------------------------------------------------------------------------------
n65_goto_α:
                                                                                        jmp   n3_var_α
n65_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n66_goto_α:
                                                                                        jmp   n9_var_α
n66_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n67_goto_α:
                                                                                        jmp   n19_var_α
n67_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n68_goto_α:
                                                                                        jmp   n39_var_α
n68_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
                        mov              eax, 1
                        xor              edx, edx
                        mov              rsp, rbp
                        mov              rbp, [rsp + 1296]
                        add              rsp, 1304
                        ret
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              rsp, rbp
                        mov              eax, 99
                        xor              edx, edx
                        mov              rbp, [rsp + 1296]
                        add              rsp, 1304
                        ret
                        .section         .rodata
.S0:                    .string          "C"
.S1:                    .string          "CROSS"
.S2:                    .string          "NV"
.S3:                    .string          "NH"
                        .text
                        .section         .note.GNU-stack,"",@progbits
