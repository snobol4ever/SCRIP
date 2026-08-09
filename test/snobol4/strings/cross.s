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
                        mov              r12, qword ptr [0x70000000]
                        xor              esi, esi
                                                                                        jmp   main_α
#-----------------------------------------------------------------------------------------------------------------------
main_α:
main_α_body:
                        push             rbp
                        mov              rbp, rsp
                        sub              rsp, 104
#-----------------------------------------------------------------------------------------------------------------------
n0_statement_begin_α:
                                                                                        jmp   n1_statement_end_α
n0_statement_begin_β:
                                                                                        jmp   n2_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n1_statement_end_α:
                                                                                        jmp   n2_statement_begin_α
#=======================================================================================================================
#       &TRIM    =  1
#-----------------------------------------------------------------------------------------------------------------------
n2_statement_begin_α:
                                                                                        jmp   n3_lit_string_α
n2_statement_begin_β:
                                                                                        jmp   n7_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n3_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx102_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n4_lit_integer_α
.Lx102_0:
                        .quad            .Lx102_0_s
.Lx102_0_s:
                        .string          "TRIM"
#-----------------------------------------------------------------------------------------------------------------------
n4_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx103_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n5_call_α
n4_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n2_statement_begin_β
.Lx103_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n5_call_α:
                        sub              rsp, 16
                        sub              rsp, 32
                        mov              r10, qword ptr [rsp + 64]
                        mov              r11, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        mov              r10, qword ptr [rsp + 48]
                        mov              r11, qword ptr [rsp + 56]
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        .section         .rodata
.Lrkfnzd105:            .string          "SNO$KWSET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd105]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 2                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 32
                        cmp              eax, 104
                                                                                        jne   .Lx104_240
                        add              rsp, 16
                                                                                        jmp   n4_lit_integer_β
.Lx104_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n6_statement_end_α
n5_call_β:
                        add              rsp, 16
                                                                                        jmp   n4_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n6_statement_end_α:
                        add              rsp, 48
                                                                                        jmp   n7_statement_begin_α
#=======================================================================================================================
# AGAIN H        =  INPUT                            :F(END)
#-----------------------------------------------------------------------------------------------------------------------
n7_statement_begin_α:
                                                                                        jmp   n8_var_α
n7_statement_begin_β:
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n8_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx110_0]                # name
                        call             NV_GET_fn@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx110_240
                        add              rsp, 16
                                                                                        jmp   n7_statement_begin_β
.Lx110_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n9_assign_α
.Lx110_0:
                        .quad            .Lx110_0_s
.Lx110_0_s:
                        .string          "INPUT"
#-----------------------------------------------------------------------------------------------------------------------
n9_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax                    # H
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n10_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n10_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n11_statement_begin_α
#=======================================================================================================================
#       V        =  INPUT                            :F(END)
#-----------------------------------------------------------------------------------------------------------------------
n11_statement_begin_α:
                                                                                        jmp   n12_var_α
n11_statement_begin_β:
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n12_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx116_0]                # name
                        call             NV_GET_fn@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx116_240
                        add              rsp, 16
                                                                                        jmp   n11_statement_begin_β
.Lx116_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n13_assign_α
.Lx116_0:
                        .quad            .Lx116_0_s
.Lx116_0_s:
                        .string          "INPUT"
#-----------------------------------------------------------------------------------------------------------------------
n13_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052304], rax                    # V
                        mov              qword ptr [1879052312], rdx
                                                                                        jmp   n14_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n14_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n15_statement_begin_α
#=======================================================================================================================
#       HC       =  H
#-----------------------------------------------------------------------------------------------------------------------
n15_statement_begin_α:
                                                                                        jmp   n16_var_α
n15_statement_begin_β:
                                                                                        jmp   n19_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n16_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052288]                    # H
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n17_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n17_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052320], rax                    # HC
                        mov              qword ptr [1879052328], rdx
                                                                                        jmp   n18_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n18_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n19_statement_begin_α
#=======================================================================================================================
# NEXTH HC       ?  @NH ANY(V) . CROSS = '*'         :F(AGAIN)
#-----------------------------------------------------------------------------------------------------------------------
n19_statement_begin_α:
                                                                                        jmp   n20_var_α
n19_statement_begin_β:
                                                                                        jmp   n7_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n20_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]                    # HC
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n21_var_α
#-----------------------------------------------------------------------------------------------------------------------
n21_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]                    # V
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n22_coerce_string_α
n21_var_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n7_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n22_coerce_string_α:
                        sub              rsp, 16
                        lea              rdi, [rsp + 16]                                # var
                        lea              rsi, [rsp + 0]                                 # result
                        mov              rdx, 3866683                                   # codes
                        call             rt_coerce_str_d@PLT
                                                                                        jmp   n23_match_begin_α
n22_coerce_string_β:
                        add              rsp, 16
                                                                                        jmp   n21_var_β
#-----------------------------------------------------------------------------------------------------------------------
n23_match_begin_α:
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              qword ptr [rbp + 384], rdi
                        mov              qword ptr [rbp + 392], rsi
                        mov              qword ptr [rsp + 48], r13                      # outer_Σ
                        mov              qword ptr [rsp + 56], r14                      # outer_δ
                        mov              qword ptr [rsp + 64], r15                      # outer_Δ
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rsp + 72], rax                      # cap_gen
                        mov              qword ptr [rsp + 40], rbp                      # old_rbp
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              qword ptr [r12 + 0], 0                         # cas_top
                        mov              qword ptr [r12 + 8], rsp                       # cas_rsp_mark
                        mov              qword ptr [r12 + 16], 0
                        add              r12, 24                                        # cas_top
                        mov              rax, rsp
                        sub              rsp, 32
                        mov              qword ptr [rsp + 16], rax                      # rsp_mark
                        mov              dword ptr [rsp + 0], 0                         # start_δ
.Lx133_0:
                        mov              r14d, dword ptr [rsp + 0]
                                                                                        jmp   n24_match_atp_α
n23_match_begin_β:
                        mov              rsp, qword ptr [r12 + -16]                     # rsp_restore
                        sub              rsp, 32
                        add              dword ptr [rsp + 0], 1                         # start_δ
                        mov              eax, dword ptr [rsp + 0]
                        cmp              eax, r15d
                                                                                        jg    .Lx133_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                                                                                        jne   .Lx133_1
                                                                                        jmp   .Lx133_0
.Lx133_1:
n23_match_begin_af:
                        sub              r12, 24                                        # cas_mark
                        mov              rsp, qword ptr [r12 + 8]                       # cas_rsp_mark
                        mov              r13, qword ptr [rsp + 48]                      # outer_Σ
                        mov              r14, qword ptr [rsp + 56]                      # outer_δ
                        mov              r15, qword ptr [rsp + 64]                      # outer_Δ
                        mov              rdi, r13                                       # sig
                        mov              rsi, r15                                       # len
                        mov              rdx, qword ptr [rsp + 72]                      # cap_gen
                        call             rt_match_ctx_restore@PLT
                        mov              rbp, qword ptr [rsp + 40]                      # old_rbp
                                                                                        jmp   n22_coerce_string_β
#-----------------------------------------------------------------------------------------------------------------------
n24_match_atp_α:
                        sub              rsp, 16
                        mov              esi, r14d                                      # cur_delta
                        lea              rdi, [rip + .S0]                               # varname
                        call             rt_at_cursor@PLT
                                                                                        jmp   n25_match_assign_save_α
n24_match_atp_β:
                        add              rsp, 16
                                                                                        jmp   n23_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n25_match_assign_save_α:
                        sub              rsp, 16
                        mov              dword ptr [rbp + -80], r14d
                                                                                        jmp   n26_match_any_α
n25_match_assign_save_β:
                        add              rsp, 16
                                                                                        jmp   n24_match_atp_β
#-----------------------------------------------------------------------------------------------------------------------
n26_match_any_α:
                        mov              eax, r14d
                        cmp              eax, r15d
                                                                                        jl    .Lx138_239
                        add              rsp, 16
                                                                                        jmp   n24_match_atp_β
.Lx138_239:
                        movsxd           rcx, r14d
                        movzx            esi, byte ptr [r13+rcx]                        # needle_ptr
                        mov              edi, esi                                       # char
                        mov              rsi, qword ptr [rsp + 40]                      # coerce_string
                        mov              edx, dword ptr [rsp + 36]                      # needle_len
                        call             rt_sg_member@PLT
                        test             eax, eax
                                                                                        jne   .Lx138_240
                        add              rsp, 16
                                                                                        jmp   n24_match_atp_β
.Lx138_240:
                        add              r14d, 1
                                                                                        jmp   n27_match_assign_cond_α
n26_match_any_β:
                        sub              r14d, 1
                        add              rsp, 16
                                                                                        jmp   n24_match_atp_β
#-----------------------------------------------------------------------------------------------------------------------
n27_match_assign_cond_α:
                        mov              eax, dword ptr [rbp + -80]
                        lea              rcx, [rip + .S1]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24
                                                                                        jmp   n28_match_end_α
n27_match_assign_cond_β:
                        sub              r12, 24
                                                                                        jmp   n26_match_any_β
#-----------------------------------------------------------------------------------------------------------------------
n28_match_end_α:
                        mov              r10, r12
.Lx142_9:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx142_9
                        mov              eax, dword ptr [rsp + 16]
                        mov              dword ptr [rsp + 224], eax
                        mov              qword ptr [rsp + 248], r14
                        mov              rsp, qword ptr [r10 + 8]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, r12
                        mov              r10, rsi
.Lx142_5:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx142_5
                        lea              rdi, [r10 + 24]
                        mov              rdx, r13
                        call             rt_dcap_end_ok_open@PLT
.Lx142_1:
                        test             rax, rax
                                                                                        je    .Lx142_2
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx142_3]
                        lea              rdx, [rip + .Lx142_4]
                                                                                        jmp   rax
.Lx142_3:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx142_1
.Lx142_4:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx142_1
.Lx142_2:
                        call             rt_dcap_end_ok_close@PLT
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        sub              r12, 24                                        # cas_mark
                        mov              r13, qword ptr [rsp + 48]                      # outer_Σ
                        mov              r14, qword ptr [rsp + 56]                      # outer_δ
                        mov              r15, qword ptr [rsp + 64]                      # outer_Δ
                        mov              rdi, r13                                       # sig
                        mov              rsi, r15                                       # len
                        call             rt_match_ctx_restore@PLT
                                                                                        jmp   n29_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n29_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx143_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n30_match_replace_α
.Lx143_0:
                        .quad            .Lx143_0_s
.Lx143_0_s:
                        .string          "*"
#-----------------------------------------------------------------------------------------------------------------------
n30_match_replace_α:
                        mov              rdi, qword ptr [rip + .Lx145_0]                # name
                        mov              rsi, qword ptr [rbp + 384]                     # sub_lo
                        mov              rdx, qword ptr [rbp + 392]                     # sub_hi
                        mov              ecx, dword ptr [rbp + 176]                     # start
                        mov              r8, qword ptr [rbp + 200]                      # end
                        lea              r9, [rsp + 0]                                  # lit_string
                        call             rt_match_replace@PLT
                                                                                        jmp   .Lx145_1
.Lx145_0:
                        .quad            .Lx145_0_s
.Lx145_0_s:
                        .string          "HC"
.Lx145_1:
                        mov              rbp, qword ptr [rbp + 216]                     # old_rbp
                                                                                        jmp   n31_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n31_statement_end_α:
                        add              rsp, 48
                                                                                        jmp   n32_statement_begin_α
#=======================================================================================================================
#       VC       =  V
#-----------------------------------------------------------------------------------------------------------------------
n32_statement_begin_α:
                                                                                        jmp   n33_var_α
n32_statement_begin_β:
                                                                                        jmp   n36_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n33_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]                    # V
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n34_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n34_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052368], rax                    # VC
                        mov              qword ptr [1879052376], rdx
                                                                                        jmp   n35_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n35_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n36_statement_begin_α
#=======================================================================================================================
# NEXTV VC       ?  @NV CROSS = '#'                  :F(NEXTH)
#-----------------------------------------------------------------------------------------------------------------------
n36_statement_begin_α:
                                                                                        jmp   n37_var_α
n36_statement_begin_β:
                                                                                        jmp   n19_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n37_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052368]                    # VC
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n38_var_α
#-----------------------------------------------------------------------------------------------------------------------
n38_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # CROSS
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n39_assign_α
n38_var_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n19_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n39_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx158_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n40_match_begin_α
n39_assign_β:
                                                                                        jmp   n38_var_β
.Lx158_0:
                        .quad            .Lx158_0_s
.Lx158_0_s:
                        .string          "PATV$0"
#-----------------------------------------------------------------------------------------------------------------------
n40_match_begin_α:
                        mov              rdi, qword ptr [rsp + 16]                      # var
                        mov              rsi, qword ptr [rsp + 24]
                        mov              qword ptr [rbp + 592], rdi
                        mov              qword ptr [rbp + 600], rsi
                        mov              qword ptr [rbp + 480], r13                     # outer_Σ
                        mov              qword ptr [rbp + 488], r14                     # outer_δ
                        mov              qword ptr [rbp + 496], r15                     # outer_Δ
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rbp + 504], rax                     # cap_gen
                        mov              qword ptr [rbp + 472], rbp                     # old_rbp
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              qword ptr [r12 + 0], 0                         # cas_top
                        mov              qword ptr [r12 + 8], rsp                       # cas_rsp_mark
                        mov              qword ptr [r12 + 16], 0
                        add              r12, 24                                        # cas_top
                        mov              qword ptr [rbp + 448], rsp                     # zls2_mark
                        mov              dword ptr [rbp + 432], 0                       # start_δ
.Lx160_0:
                        mov              r14d, dword ptr [rbp + 432]
                                                                                        jmp   n41_match_atp_α
n40_match_begin_β:
                        add              dword ptr [rbp + 432], 1
                        mov              eax, dword ptr [rbp + 432]
                        cmp              eax, r15d
                                                                                        jg    .Lx160_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                                                                                        jne   .Lx160_1
                                                                                        jmp   .Lx160_0
.Lx160_1:
n40_match_begin_af:
                        sub              r12, 24                                        # cas_mark
                        mov              rsp, qword ptr [rbp + 448]
                        mov              r13, qword ptr [rbp + 480]                     # outer_Σ
                        mov              r14, qword ptr [rbp + 488]                     # outer_δ
                        mov              r15, qword ptr [rbp + 496]                     # outer_Δ
                        mov              rdi, r13                                       # sig
                        mov              rsi, r15                                       # len
                        mov              rdx, qword ptr [rbp + 504]                     # cap_gen
                        call             rt_match_ctx_restore@PLT
                        mov              rbp, qword ptr [rbp + 472]                     # old_rbp
                                                                                        jmp   n39_assign_β
#-----------------------------------------------------------------------------------------------------------------------
n41_match_atp_α:
                        sub              rsp, 16
                        mov              esi, r14d                                      # cur_delta
                        lea              rdi, [rip + .S2]                               # varname
                        call             rt_at_cursor@PLT
                                                                                        jmp   n42_match_defer_α
n41_match_atp_β:
                        add              rsp, 16
                                                                                        jmp   n40_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n42_match_defer_α:
                        lea              rdi, [rip + .S3]                               # varname
                        xor              esi, esi                                       # ival_flag
                        call             rt_defer_get_pat_fn@PLT
                        test             rax, rax
                                                                                        jz    .Lx162_0
                        mov              r8d, 0
                        lea              rcx, [rip + .Lx162_4]
                        lea              rdx, [rip + .Lx162_5]
                                                                                        jmp   rax
.Lx162_4:
                                                                                        jmp   n43_match_end_α
.Lx162_5:
                                                                                        jmp   n41_match_atp_β
.Lx162_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S3]                               # varname
                        xor              esi, esi                                       # ival_flag
                        call             rt_defer_open@PLT
.Lx162_2:
                        test             rax, rax
                                                                                        je    .Lx162_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx162_7]
                        lea              rdx, [rip + .Lx162_8]
                                                                                        jmp   rax
.Lx162_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx162_2
.Lx162_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx162_2
.Lx162_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d                                      # cur_delta
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n41_match_atp_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx162_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n43_match_end_α
.Lx162_6:
                        add              rsp, 16
                                                                                        jmp   n41_match_atp_β
n42_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n43_match_end_α:
                        mov              r10, r12
.Lx164_9:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx164_9
                        mov              qword ptr [rbp + 456], r14
                        mov              rsp, qword ptr [rbp + 448]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, r12
                        mov              r10, rsi
.Lx164_5:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx164_5
                        lea              rdi, [r10 + 24]
                        mov              rdx, r13
                        call             rt_dcap_end_ok_open@PLT
.Lx164_1:
                        test             rax, rax
                                                                                        je    .Lx164_2
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx164_3]
                        lea              rdx, [rip + .Lx164_4]
                                                                                        jmp   rax
.Lx164_3:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx164_1
.Lx164_4:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx164_1
.Lx164_2:
                        call             rt_dcap_end_ok_close@PLT
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        sub              r12, 24                                        # cas_mark
                        mov              r13, qword ptr [rbp + 480]                     # outer_Σ
                        mov              r14, qword ptr [rbp + 488]                     # outer_δ
                        mov              r15, qword ptr [rbp + 496]                     # outer_Δ
                        mov              rdi, r13                                       # sig
                        mov              rsi, r15                                       # len
                        call             rt_match_ctx_restore@PLT
                                                                                        jmp   n44_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n44_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx165_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n45_match_replace_α
.Lx165_0:
                        .quad            .Lx165_0_s
.Lx165_0_s:
                        .string          "#"
#-----------------------------------------------------------------------------------------------------------------------
n45_match_replace_α:
                        mov              rdi, qword ptr [rip + .Lx167_0]                # name
                        mov              rsi, qword ptr [rbp + 592]                     # sub_lo
                        mov              rdx, qword ptr [rbp + 600]                     # sub_hi
                        mov              ecx, dword ptr [rbp + 432]                     # start
                        mov              r8, qword ptr [rbp + 456]                      # end
                        lea              r9, [rsp + 0]                                  # lit_string
                        call             rt_match_replace@PLT
                                                                                        jmp   .Lx167_1
.Lx167_0:
                        .quad            .Lx167_0_s
.Lx167_0_s:
                        .string          "VC"
.Lx167_1:
                        mov              rbp, qword ptr [rbp + 472]                     # old_rbp
                                                                                        jmp   n46_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n46_statement_end_α:
                        add              rsp, 32
                                                                                        jmp   n47_statement_begin_α
#=======================================================================================================================
#       OUTPUT   =
#-----------------------------------------------------------------------------------------------------------------------
n47_statement_begin_α:
                                                                                        jmp   n48_lit_string_α
n47_statement_begin_β:
                                                                                        jmp   n51_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n48_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx172_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n49_assign_α
.Lx172_0:
                        .quad            .Lx172_0_s
.Lx172_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n49_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx173_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n50_statement_end_α
.Lx173_0:
                        .quad            .Lx173_0_s
.Lx173_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n50_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n51_statement_begin_α
#=======================================================================================================================
#       PRINTV   =  V
#-----------------------------------------------------------------------------------------------------------------------
n51_statement_begin_α:
                                                                                        jmp   n52_var_α
n51_statement_begin_β:
                                                                                        jmp   n55_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n52_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]                    # V
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n53_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n53_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052400], rax                    # PRINTV
                        mov              qword ptr [1879052408], rdx
                                                                                        jmp   n54_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n54_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n55_statement_begin_α
#=======================================================================================================================
#       PRINTV   ?  POS(NV) LEN(1) = '#'
#-----------------------------------------------------------------------------------------------------------------------
n55_statement_begin_α:
                                                                                        jmp   n56_var_α
n55_statement_begin_β:
                                                                                        jmp   n66_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n56_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052400]                    # PRINTV
                        mov              rdx, qword ptr [1879052408]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n57_var_α
#-----------------------------------------------------------------------------------------------------------------------
n57_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052384]                    # NV
                        mov              rdx, qword ptr [1879052392]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n58_coerce_integer_α
n57_var_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n66_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n58_coerce_integer_α:
                        sub              rsp, 16
                        lea              rdi, [rsp + 16]                                # var
                        lea              rsi, [rsp + 0]                                 # result
                        mov              rdx, 10682530                                  # codes
                        call             rt_coerce_int_d@PLT
                                                                                        jmp   n59_match_begin_α
n58_coerce_integer_β:
                        add              rsp, 16
                                                                                        jmp   n57_var_β
#-----------------------------------------------------------------------------------------------------------------------
n59_match_begin_α:
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              qword ptr [rbp + 800], rdi
                        mov              qword ptr [rbp + 808], rsi
                        mov              qword ptr [rsp + 48], r13                      # outer_Σ
                        mov              qword ptr [rsp + 56], r14                      # outer_δ
                        mov              qword ptr [rsp + 64], r15                      # outer_Δ
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rsp + 72], rax                      # cap_gen
                        mov              qword ptr [rsp + 40], rbp                      # old_rbp
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              qword ptr [r12 + 0], 0                         # cas_top
                        mov              qword ptr [r12 + 8], rsp                       # cas_rsp_mark
                        mov              qword ptr [r12 + 16], 0
                        add              r12, 24                                        # cas_top
                        mov              rax, rsp
                        sub              rsp, 32
                        mov              qword ptr [rsp + 16], rax                      # rsp_mark
                        mov              dword ptr [rsp + 0], 0                         # start_δ
.Lx189_0:
                        mov              r14d, dword ptr [rsp + 0]
                                                                                        jmp   n60_match_pos_α
n59_match_begin_β:
                        mov              rsp, qword ptr [r12 + -16]                     # rsp_restore
                        sub              rsp, 32
                        add              dword ptr [rsp + 0], 1                         # start_δ
                        mov              eax, dword ptr [rsp + 0]
                        cmp              eax, r15d
                                                                                        jg    .Lx189_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                                                                                        jne   .Lx189_1
                                                                                        jmp   .Lx189_0
.Lx189_1:
n59_match_begin_af:
                        sub              r12, 24                                        # cas_mark
                        mov              rsp, qword ptr [r12 + 8]                       # cas_rsp_mark
                        mov              r13, qword ptr [rsp + 48]                      # outer_Σ
                        mov              r14, qword ptr [rsp + 56]                      # outer_δ
                        mov              r15, qword ptr [rsp + 64]                      # outer_Δ
                        mov              rdi, r13                                       # sig
                        mov              rsi, r15                                       # len
                        mov              rdx, qword ptr [rsp + 72]                      # cap_gen
                        call             rt_match_ctx_restore@PLT
                        mov              rbp, qword ptr [rsp + 40]                      # old_rbp
                                                                                        jmp   n58_coerce_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n60_match_pos_α:
                        mov              rax, qword ptr [rsp + 8]
                        cmp              r14d, eax
                                                                                        jne   n59_match_begin_β
                                                                                        jmp   n61_match_len_α
n60_match_pos_β:
                                                                                        jmp   n59_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n61_match_len_α:
                        mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d
                                                                                        jg    n59_match_begin_β
                        add              r14d, 1
                                                                                        jmp   n62_match_end_α
n61_match_len_β:
                        sub              r14d, 1
                                                                                        jmp   n59_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n62_match_end_α:
                        mov              r10, r12
.Lx193_9:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx193_9
                        mov              eax, dword ptr [rsp + 0]
                        mov              dword ptr [rsp + 688], eax
                        mov              qword ptr [rsp + 712], r14
                        mov              rsp, qword ptr [r10 + 8]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, r12
                        mov              r10, rsi
.Lx193_5:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx193_5
                        lea              rdi, [r10 + 24]
                        mov              rdx, r13
                        call             rt_dcap_end_ok_open@PLT
.Lx193_1:
                        test             rax, rax
                                                                                        je    .Lx193_2
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx193_3]
                        lea              rdx, [rip + .Lx193_4]
                                                                                        jmp   rax
.Lx193_3:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx193_1
.Lx193_4:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx193_1
.Lx193_2:
                        call             rt_dcap_end_ok_close@PLT
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        sub              r12, 24                                        # cas_mark
                        mov              r13, qword ptr [rsp + 48]                      # outer_Σ
                        mov              r14, qword ptr [rsp + 56]                      # outer_δ
                        mov              r15, qword ptr [rsp + 64]                      # outer_Δ
                        mov              rdi, r13                                       # sig
                        mov              rsi, r15                                       # len
                        call             rt_match_ctx_restore@PLT
                                                                                        jmp   n63_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n63_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx194_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n64_match_replace_α
.Lx194_0:
                        .quad            .Lx194_0_s
.Lx194_0_s:
                        .string          "#"
#-----------------------------------------------------------------------------------------------------------------------
n64_match_replace_α:
                        mov              rdi, qword ptr [rip + .Lx196_0]                # name
                        mov              rsi, qword ptr [rbp + 800]                     # sub_lo
                        mov              rdx, qword ptr [rbp + 808]                     # sub_hi
                        mov              ecx, dword ptr [rbp + 656]                     # start
                        mov              r8, qword ptr [rbp + 680]                      # end
                        lea              r9, [rsp + 0]                                  # lit_string
                        call             rt_match_replace@PLT
                                                                                        jmp   .Lx196_1
.Lx196_0:
                        .quad            .Lx196_0_s
.Lx196_0_s:
                        .string          "PRINTV"
.Lx196_1:
                        mov              rbp, qword ptr [rbp + 696]                     # old_rbp
                                                                                        jmp   n65_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n65_statement_end_α:
                        add              rsp, 48
                                                                                        jmp   n66_statement_begin_α
#=======================================================================================================================
# PRINT PRINTV   ?  LEN(1) . C =                     :F(NEXTV)
#-----------------------------------------------------------------------------------------------------------------------
n66_statement_begin_α:
                                                                                        jmp   n67_var_α
n66_statement_begin_β:
                                                                                        jmp   n36_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n67_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052400]                    # PRINTV
                        mov              rdx, qword ptr [1879052408]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n68_match_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n68_match_begin_α:
                        mov              rdi, qword ptr [rsp + 0]                       # var
                        mov              rsi, qword ptr [rsp + 8]
                        mov              qword ptr [rbp + 992], rdi
                        mov              qword ptr [rbp + 1000], rsi
                        mov              qword ptr [rsp + 48], r13                      # outer_Σ
                        mov              qword ptr [rsp + 56], r14                      # outer_δ
                        mov              qword ptr [rsp + 64], r15                      # outer_Δ
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rsp + 72], rax                      # cap_gen
                        mov              qword ptr [rsp + 40], rbp                      # old_rbp
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              qword ptr [r12 + 0], 0                         # cas_top
                        mov              qword ptr [r12 + 8], rsp                       # cas_rsp_mark
                        mov              qword ptr [r12 + 16], 0
                        add              r12, 24                                        # cas_top
                        mov              rax, rsp
                        sub              rsp, 32
                        mov              qword ptr [rsp + 16], rax                      # rsp_mark
                        mov              dword ptr [rsp + 0], 0                         # start_δ
.Lx203_0:
                        mov              r14d, dword ptr [rsp + 0]
                                                                                        jmp   n69_match_assign_save_α
n68_match_begin_β:
                        mov              rsp, qword ptr [r12 + -16]                     # rsp_restore
                        sub              rsp, 32
                        add              dword ptr [rsp + 0], 1                         # start_δ
                        mov              eax, dword ptr [rsp + 0]
                        cmp              eax, r15d
                                                                                        jg    .Lx203_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                                                                                        jne   .Lx203_1
                                                                                        jmp   .Lx203_0
.Lx203_1:
n68_match_begin_af:
                        sub              r12, 24                                        # cas_mark
                        mov              rsp, qword ptr [r12 + 8]                       # cas_rsp_mark
                        mov              r13, qword ptr [rsp + 48]                      # outer_Σ
                        mov              r14, qword ptr [rsp + 56]                      # outer_δ
                        mov              r15, qword ptr [rsp + 64]                      # outer_Δ
                        mov              rdi, r13                                       # sig
                        mov              rsi, r15                                       # len
                        mov              rdx, qword ptr [rsp + 72]                      # cap_gen
                        call             rt_match_ctx_restore@PLT
                        mov              rbp, qword ptr [rsp + 40]                      # old_rbp
                        add              rsp, 16
                                                                                        jmp   n36_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n69_match_assign_save_α:
                        sub              rsp, 16
                        mov              dword ptr [rbp + -96], r14d
                                                                                        jmp   n70_match_len_α
n69_match_assign_save_β:
                        add              rsp, 16
                                                                                        jmp   n68_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n70_match_len_α:
                        mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d
                                                                                        jle   .Lx206_240
                        add              rsp, 16
                                                                                        jmp   n68_match_begin_β
.Lx206_240:
                        add              r14d, 1
                                                                                        jmp   n71_match_assign_cond_α
n70_match_len_β:
                        sub              r14d, 1
                        add              rsp, 16
                                                                                        jmp   n68_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n71_match_assign_cond_α:
                        mov              eax, dword ptr [rbp + -96]
                        lea              rcx, [rip + .S4]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24
                                                                                        jmp   n72_match_end_α
n71_match_assign_cond_β:
                        sub              r12, 24
                                                                                        jmp   n70_match_len_β
#-----------------------------------------------------------------------------------------------------------------------
n72_match_end_α:
                        mov              r10, r12
.Lx210_9:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx210_9
                        mov              eax, dword ptr [rsp + 16]
                        mov              dword ptr [rsp + 880], eax
                        mov              qword ptr [rsp + 904], r14
                        mov              rsp, qword ptr [r10 + 8]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, r12
                        mov              r10, rsi
.Lx210_5:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx210_5
                        lea              rdi, [r10 + 24]
                        mov              rdx, r13
                        call             rt_dcap_end_ok_open@PLT
.Lx210_1:
                        test             rax, rax
                                                                                        je    .Lx210_2
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx210_3]
                        lea              rdx, [rip + .Lx210_4]
                                                                                        jmp   rax
.Lx210_3:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx210_1
.Lx210_4:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx210_1
.Lx210_2:
                        call             rt_dcap_end_ok_close@PLT
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        sub              r12, 24                                        # cas_mark
                        mov              r13, qword ptr [rsp + 48]                      # outer_Σ
                        mov              r14, qword ptr [rsp + 56]                      # outer_δ
                        mov              r15, qword ptr [rsp + 64]                      # outer_Δ
                        mov              rdi, r13                                       # sig
                        mov              rsi, r15                                       # len
                        call             rt_match_ctx_restore@PLT
                                                                                        jmp   n73_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n73_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx211_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n74_match_replace_α
.Lx211_0:
                        .quad            .Lx211_0_s
.Lx211_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n74_match_replace_α:
                        mov              rdi, qword ptr [rip + .Lx213_0]                # name
                        mov              rsi, qword ptr [rbp + 992]                     # sub_lo
                        mov              rdx, qword ptr [rbp + 1000]                    # sub_hi
                        mov              ecx, dword ptr [rbp + 832]                     # start
                        mov              r8, qword ptr [rbp + 856]                      # end
                        lea              r9, [rsp + 0]                                  # lit_string
                        call             rt_match_replace@PLT
                                                                                        jmp   .Lx213_1
.Lx213_0:
                        .quad            .Lx213_0_s
.Lx213_0_s:
                        .string          "PRINTV"
.Lx213_1:
                        mov              rbp, qword ptr [rbp + 872]                     # old_rbp
                                                                                        jmp   n75_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n75_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n76_statement_begin_α
#=======================================================================================================================
#       OUTPUT   =  DIFFER(C, '#') DUPL(' ', NH) C   :S(PRINT)
#-----------------------------------------------------------------------------------------------------------------------
n76_statement_begin_α:
                                                                                        jmp   n77_var_α
n76_statement_begin_β:
                                                                                        jmp   n88_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n77_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052416]                    # C
                        mov              rdx, qword ptr [1879052424]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n78_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n78_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx219_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n79_call_α
n78_lit_string_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n76_statement_begin_β
.Lx219_0:
                        .quad            .Lx219_0_s
.Lx219_0_s:
                        .string          "#"
#-----------------------------------------------------------------------------------------------------------------------
n79_call_α:
                        sub              rsp, 16
                        sub              rsp, 32
                        mov              r10, qword ptr [rsp + 64]
                        mov              r11, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        mov              r10, qword ptr [rsp + 48]
                        mov              r11, qword ptr [rsp + 56]
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        .section         .rodata
.Lrkfnzd221:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd221]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 2                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 32
                        cmp              eax, 104
                                                                                        jne   .Lx220_240
                        add              rsp, 16
                                                                                        jmp   n78_lit_string_β
.Lx220_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n80_lit_string_α
n79_call_β:
                        add              rsp, 16
                                                                                        jmp   n78_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n80_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx222_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n81_var_α
n80_lit_string_β:
                        add              rsp, 32
                                                                                        jmp   n78_lit_string_β
.Lx222_0:
                        .quad            .Lx222_0_s
.Lx222_0_s:
                        .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n81_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # NH
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n82_call_α
n81_var_β:
                        add              rsp, 16
                                                                                        jmp   n80_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n82_call_α:
                        sub              rsp, 16
                        sub              rsp, 32
                        mov              r10, qword ptr [rsp + 64]
                        mov              r11, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        mov              r10, qword ptr [rsp + 48]
                        mov              r11, qword ptr [rsp + 56]
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        .section         .rodata
.Lrkfnzd225:            .string          "DUPL"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd225]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 2                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 32
                        cmp              eax, 104
                                                                                        jne   .Lx224_240
                        add              rsp, 16
                                                                                        jmp   n81_var_β
.Lx224_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n83_binop_α
n82_call_β:
                        add              rsp, 16
                                                                                        jmp   n81_var_β
#-----------------------------------------------------------------------------------------------------------------------
n83_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 64]                      # call
                        mov              rsi, qword ptr [rsp + 72]                      # a
                        mov              rdx, qword ptr [rsp + 16]                      # b
                        mov              rcx, qword ptr [rsp + 24]                      # b
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n84_var_α
n83_binop_β:
                        add              rsp, 32
                                                                                        jmp   n81_var_β
#-----------------------------------------------------------------------------------------------------------------------
n84_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052416]                    # C
                        mov              rdx, qword ptr [1879052424]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n85_binop_α
n84_var_β:
                        add              rsp, 16
                                                                                        jmp   n83_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n85_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # binop
                        mov              rsi, qword ptr [rsp + 40]                      # a
                        mov              rdx, qword ptr [rsp + 16]                      # var
                        mov              rcx, qword ptr [rsp + 24]                      # b
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n86_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n86_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx229_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n87_statement_end_α
.Lx229_0:
                        .quad            .Lx229_0_s
.Lx229_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n87_statement_end_α:
                        add              rsp, 144
                                                                                        jmp   n66_statement_begin_α
#=======================================================================================================================
#       OUTPUT   =  H                                :(PRINT)
#-----------------------------------------------------------------------------------------------------------------------
n88_statement_begin_α:
                                                                                        jmp   n89_var_α
n88_statement_begin_β:
                                                                                        jmp   n66_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n89_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052288]                    # H
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n90_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n90_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx235_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n91_statement_end_α
.Lx235_0:
                        .quad            .Lx235_0_s
.Lx235_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n91_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n66_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n92_goto_α:
                                                                                        jmp   n7_statement_begin_α
n92_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n93_goto_α:
                                                                                        jmp   n19_statement_begin_α
n93_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n94_goto_α:
                                                                                        jmp   n36_statement_begin_α
n94_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n95_goto_α:
                                                                                        jmp   n66_statement_begin_α
n95_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              rsp, rbp
                        pop              rbp
                        mov              edi, 1
                        call             exit@PLT
                        .section         .rodata
.S0:                    .string          "NH"
.S1:                    .string          "CROSS"
.S2:                    .string          "NV"
.S3:                    .string          "PATV$0"
.S4:                    .string          "C"
                        .text
                        .section         .note.GNU-stack,"",@progbits
