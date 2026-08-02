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
                                                                                        jmp   main_α
#-----------------------------------------------------------------------------------------------------------------------
main_α:
main_α_body:
                        push             rbp
                        mov              rbp, rsp
                        sub              rsp, 8
#=======================================================================================================================
#       &TRIM    =  1
#-----------------------------------------------------------------------------------------------------------------------
n0_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx69_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n1_lit_integer_α
n0_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n3_var_α
.Lx69_0:
                        .quad            .Lx69_0_s
.Lx69_0_s:
                        .string          "TRIM"
#-----------------------------------------------------------------------------------------------------------------------
n1_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx70_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n2_call_α
n1_lit_integer_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n3_var_α
.Lx70_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n2_call_α:
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
.Lrkfnzd72:             .string          "SNO$KWSET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd72]                        # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 2                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 32
                        cmp              eax, 104
                                                                                        jne   .Lx71_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n3_var_α
.Lx71_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        add              rsp, 48
                                                                                        jmp   n3_var_α
n2_call_β:
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n3_var_α
#=======================================================================================================================
# AGAIN H        =  INPUT                            :F(END)
#-----------------------------------------------------------------------------------------------------------------------
n3_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx73_0]                 # name
                        call             NV_GET_fn@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx73_240
                        add              rsp, 16
                                                                                        jmp   main_γ
.Lx73_240:
                        mov              qword ptr [rsp + 0], rax                       # result
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
                        mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax                    # H
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
                        mov              rdi, qword ptr [rip + .Lx75_0]                 # name
                        call             NV_GET_fn@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx75_240
                        add              rsp, 16
                                                                                        jmp   main_γ
.Lx75_240:
                        mov              qword ptr [rsp + 0], rax                       # result
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
                        mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052304], rax                    # V
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
                        mov              rax, qword ptr [1879052288]                    # H
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n8_assign_α
n7_var_β:
                        add              rsp, 16
                                                                                        jmp   n9_var_α
#-----------------------------------------------------------------------------------------------------------------------
n8_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052320], rax                    # HC
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
                        sub              rsp, 320
                        mov              qword ptr [rsp + 0], 0                         # stmt_claim
                        mov              qword ptr [rsp + 8], 0
                        mov              qword ptr [rsp + 16], 0
                        mov              qword ptr [rsp + 24], 0
                        mov              qword ptr [rsp + 32], 0
                        mov              qword ptr [rsp + 40], 0
                        mov              qword ptr [rsp + 48], 0
                        mov              qword ptr [rsp + 56], 0
                        mov              qword ptr [rsp + 64], 0
                        mov              qword ptr [rsp + 72], 0
                        mov              qword ptr [rsp + 80], 0
                        mov              qword ptr [rsp + 88], 0
                        mov              qword ptr [rsp + 96], 0
                        mov              qword ptr [rsp + 104], 0
                        mov              qword ptr [rsp + 112], 0
                        mov              qword ptr [rsp + 120], 0
                        mov              qword ptr [rsp + 128], 0
                        mov              qword ptr [rsp + 136], 0
                        mov              qword ptr [rsp + 144], 0
                        mov              qword ptr [rsp + 152], 0
                        mov              qword ptr [rsp + 160], 0
                        mov              qword ptr [rsp + 168], 0
                        mov              qword ptr [rsp + 176], 0
                        mov              qword ptr [rsp + 184], 0
                        mov              qword ptr [rsp + 192], 0
                        mov              qword ptr [rsp + 200], 0
                        mov              qword ptr [rsp + 208], 0
                        mov              qword ptr [rsp + 216], 0
                        mov              qword ptr [rsp + 224], 0
                        mov              qword ptr [rsp + 232], 0
                        mov              qword ptr [rsp + 240], 0
                        mov              qword ptr [rsp + 248], 0
                        mov              qword ptr [rsp + 256], 0
                        mov              qword ptr [rsp + 264], 0
                        mov              qword ptr [rsp + 272], 0
                        mov              qword ptr [rsp + 280], 0
                        mov              qword ptr [rsp + 288], 0
                        mov              qword ptr [rsp + 296], 0
                        mov              qword ptr [rsp + 304], 0
                        mov              qword ptr [rsp + 312], 0
                        mov              rax, qword ptr [1879052320]                    # HC
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rsp + 304], rax                     # result
                        mov              qword ptr [rsp + 312], rdx
                                                                                        jmp   n10_var_α
n9_var_β:
                        add              rsp, 320
                                                                                        jmp   n3_var_α
#-----------------------------------------------------------------------------------------------------------------------
n10_var_α:
                        mov              rax, qword ptr [1879052304]                    # V
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 288], rax                     # result
                        mov              qword ptr [rsp + 296], rdx
                                                                                        jmp   n11_coerce_string_α
n10_var_β:
                        add              rsp, 320
                                                                                        jmp   n3_var_α
#-----------------------------------------------------------------------------------------------------------------------
n11_coerce_string_α:
                        lea              rdi, [rsp + 288]                               # in
                        lea              rsi, [rsp + 272]                               # out
                        mov              rdx, 3866683                                   # codes
                        call             rt_coerce_str_d@PLT
                                                                                        jmp   n12_match_begin_α
n11_coerce_string_β:
                        add              rsp, 320
                                                                                        jmp   n3_var_α
#-----------------------------------------------------------------------------------------------------------------------
n12_match_begin_α:
                        mov              qword ptr [rsp + 120], rbp                     # old_rbp
                        mov              rbp, rsp                                       # stmt_base
                        mov              qword ptr [rbp + 128], r13                     # outer_Σ
                        mov              qword ptr [rbp + 136], r14                     # outer_δ
                        mov              qword ptr [rbp + 144], r15                     # outer_Δ
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rbp + 152], rax                     # cap_gen
                        mov              rdi, qword ptr [rsp + 304]                     # lo
                        mov              rsi, qword ptr [rsp + 312]                     # hi
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              r10, qword ptr [1879048192]                    # cas_top
                        mov              qword ptr [r10 + 0], 0
                        mov              qword ptr [r10 + 8], rsp                       # cas_rsp_mark
                        lea              rcx, [rip + g_patstk_sp]
                        mov              rax, qword ptr [rcx + 0]
                        mov              qword ptr [r10 + 16], rax                      # cas_patstk
                        add              r10, 24
                        mov              qword ptr [1879048192], r10                    # cas_top
                        mov              rax, rsp
                        sub              rsp, 32
                        mov              qword ptr [rsp + 16], rax                      # rsp_mark
                        lea              rcx, [rip + g_patstk_sp]
                        mov              rax, qword ptr [rcx + 0]
                        mov              qword ptr [rsp + 8], rax                       # patstk_mark
                        mov              dword ptr [rsp + 0], 0                         # start_δ
.Lx84_0:
                        mov              r14d, dword ptr [rsp + 0]
                                                                                        jmp   n13_match_sequence_α
n12_match_begin_β:
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
                        mov              r10, qword ptr [1879048192]
.Lx84_2:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx84_2
                        mov              rax, qword ptr [r10 + 16]                      # cas_patstk
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              rsp, qword ptr [r10 + 8]                       # cas_rsp_mark
                        mov              qword ptr [1879048192], r10
                        mov              r13, qword ptr [rbp + 128]                     # outer_Σ
                        mov              r14, qword ptr [rbp + 136]                     # outer_δ
                        mov              r15, qword ptr [rbp + 144]                     # outer_Δ
                        mov              rdi, r13                                       # sig
                        mov              rsi, r15                                       # len
                        mov              rdx, qword ptr [rbp + 152]                     # cap_gen
                        call             rt_match_ctx_restore@PLT
                        mov              rbp, qword ptr [rbp + 24]                      # old_rbp
                        add              rsp, 320
                                                                                        jmp   n3_var_α
#-----------------------------------------------------------------------------------------------------------------------
n13_match_sequence_α:
                                                                                        jmp   n64_match_atp_α
n13_match_sequence_as:
                                                                                        jmp   n14_match_end_α
n13_match_sequence_β:
                                                                                        jmp   n63_match_assign_cond_β
n13_match_sequence_af:
                                                                                        jmp   n12_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n14_match_end_α:
                        mov              r10, qword ptr [1879048192]
.Lx88_9:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx88_9
                        mov              rax, qword ptr [r10 + 16]
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              eax, dword ptr [rsp + 16]
                        mov              dword ptr [rsp + 224], eax
                        mov              qword ptr [rsp + 248], r14
                        mov              rsp, qword ptr [r10 + 8]
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
                        mov              r13, qword ptr [rbp + 128]                     # outer_Σ
                        mov              r14, qword ptr [rbp + 136]                     # outer_δ
                        mov              r15, qword ptr [rbp + 144]                     # outer_Δ
                        mov              rdi, r13                                       # sig
                        mov              rsi, r15                                       # len
                        mov              rdx, qword ptr [rbp + 152]                     # cap_gen
                        call             rt_match_ctx_restore@PLT
                                                                                        jmp   n15_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n15_lit_string_α:
                        mov              qword ptr [rsp + 176], 2                       # result
                        mov              dword ptr [rsp + 180], 1
                        mov              rax, qword ptr [rip + .Lx89_0]
                        mov              qword ptr [rsp + 184], rax
                                                                                        jmp   n16_match_replace_α
n15_lit_string_β:
                        mov              rbp, qword ptr [rbp + 120]                     # old_rbp
                        add              rsp, 320
                                                                                        jmp   n3_var_α
.Lx89_0:
                        .quad            .Lx89_0_s
.Lx89_0_s:
                        .string          "*"
#-----------------------------------------------------------------------------------------------------------------------
n16_match_replace_α:
                        mov              rdi, qword ptr [rip + .Lx91_0]                 # name
                        mov              rsi, qword ptr [rbp + 304]                     # sub_lo
                        mov              rdx, qword ptr [rbp + 312]                     # sub_hi
                        mov              ecx, dword ptr [rbp + 80]                      # start
                        mov              r8, qword ptr [rbp + 104]                      # end
                        lea              r9, [rbp + 176]                                # replp
                        call             rt_match_replace@PLT
                                                                                        jmp   .Lx91_1
.Lx91_0:
                        .quad            .Lx91_0_s
.Lx91_0_s:
                        .string          "HC"
.Lx91_1:
                        mov              rbp, qword ptr [rbp + 24]                      # old_rbp
                        add              rsp, 320
                                                                                        jmp   n17_var_α
#=======================================================================================================================
#       VC       =  V
#-----------------------------------------------------------------------------------------------------------------------
n17_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]                    # V
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n18_assign_α
n17_var_β:
                        add              rsp, 16
                                                                                        jmp   n19_var_α
#-----------------------------------------------------------------------------------------------------------------------
n18_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052368], rax                    # VC
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
                        sub              rsp, 192
                        mov              qword ptr [rsp + 0], 0                         # stmt_claim
                        mov              qword ptr [rsp + 8], 0
                        mov              qword ptr [rsp + 16], 0
                        mov              qword ptr [rsp + 24], 0
                        mov              qword ptr [rsp + 32], 0
                        mov              qword ptr [rsp + 40], 0
                        mov              qword ptr [rsp + 48], 0
                        mov              qword ptr [rsp + 56], 0
                        mov              qword ptr [rsp + 64], 0
                        mov              qword ptr [rsp + 72], 0
                        mov              qword ptr [rsp + 80], 0
                        mov              qword ptr [rsp + 88], 0
                        mov              qword ptr [rsp + 96], 0
                        mov              qword ptr [rsp + 104], 0
                        mov              qword ptr [rsp + 112], 0
                        mov              qword ptr [rsp + 120], 0
                        mov              qword ptr [rsp + 128], 0
                        mov              qword ptr [rsp + 136], 0
                        mov              qword ptr [rsp + 144], 0
                        mov              qword ptr [rsp + 152], 0
                        mov              qword ptr [rsp + 160], 0
                        mov              qword ptr [rsp + 168], 0
                        mov              qword ptr [rsp + 176], 0
                        mov              qword ptr [rsp + 184], 0
                        mov              rax, qword ptr [1879052368]                    # VC
                        mov              rdx, qword ptr [1879052376]
                        mov              qword ptr [rsp + 176], rax                     # result
                        mov              qword ptr [rsp + 184], rdx
                                                                                        jmp   n20_match_begin_α
n19_var_β:
                        add              rsp, 192
                                                                                        jmp   n9_var_α
#-----------------------------------------------------------------------------------------------------------------------
n20_match_begin_α:
                        mov              qword ptr [rsp + 56], rbp                      # old_rbp
                        mov              rbp, rsp                                       # stmt_base
                        mov              qword ptr [rbp + 64], r13                      # outer_Σ
                        mov              qword ptr [rbp + 72], r14                      # outer_δ
                        mov              qword ptr [rbp + 80], r15                      # outer_Δ
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rbp + 88], rax                      # cap_gen
                        mov              rdi, qword ptr [rsp + 176]                     # lo
                        mov              rsi, qword ptr [rsp + 184]                     # hi
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              r10, qword ptr [1879048192]                    # cas_top
                        mov              qword ptr [r10 + 0], 0
                        mov              qword ptr [r10 + 8], rsp                       # cas_rsp_mark
                        lea              rcx, [rip + g_patstk_sp]
                        mov              rax, qword ptr [rcx + 0]
                        mov              qword ptr [r10 + 16], rax                      # cas_patstk
                        add              r10, 24
                        mov              qword ptr [1879048192], r10                    # cas_top
                        mov              qword ptr [rbp + 32], rsp                      # zls2_mark
                        lea              rcx, [rip + g_patstk_sp]
                        mov              rax, qword ptr [rcx + 0]
                        mov              qword ptr [rbp + 24], rax                      # patstk_mark
                        mov              dword ptr [rbp + 16], 0                        # start_δ
.Lx96_0:
                        mov              r14d, dword ptr [rbp + 16]
                                                                                        jmp   n21_match_sequence_α
n20_match_begin_β:
                        add              dword ptr [rbp + 16], 1
                        mov              eax, dword ptr [rbp + 16]
                        cmp              eax, r15d
                                                                                        jg    .Lx96_1
                        lea              rcx, [rip + g_anchor]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                                                                                        jne   .Lx96_1
                                                                                        jmp   .Lx96_0
.Lx96_1:
                        mov              rax, qword ptr [rbp + 24]                      # patstk_mark
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              rsp, qword ptr [rbp + 32]
                        mov              r10, qword ptr [1879048192]
.Lx96_2:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx96_2
                        mov              qword ptr [1879048192], r10
                        mov              r13, qword ptr [rbp + 64]                      # outer_Σ
                        mov              r14, qword ptr [rbp + 72]                      # outer_δ
                        mov              r15, qword ptr [rbp + 80]                      # outer_Δ
                        mov              rdi, r13                                       # sig
                        mov              rsi, r15                                       # len
                        mov              rdx, qword ptr [rbp + 88]                      # cap_gen
                        call             rt_match_ctx_restore@PLT
                        mov              rbp, qword ptr [rbp + 56]                      # old_rbp
                        add              rsp, 192
                                                                                        jmp   n9_var_α
#-----------------------------------------------------------------------------------------------------------------------
n21_match_sequence_α:
                        mov              dword ptr [rbp + 560], r14d
                                                                                        jmp   n60_match_atp_α
n21_match_sequence_as:
                                                                                        jmp   n22_match_end_α
n21_match_sequence_β:
                                                                                        jmp   n59_match_patref_β
n21_match_sequence_af:
                                                                                        jmp   n20_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n22_match_end_α:
                        mov              rax, qword ptr [rbp + 24]
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              qword ptr [rbp + 40], r14
                        mov              rsp, qword ptr [rbp + 32]
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
                        mov              r13, qword ptr [rbp + 64]                      # outer_Σ
                        mov              r14, qword ptr [rbp + 72]                      # outer_δ
                        mov              r15, qword ptr [rbp + 80]                      # outer_Δ
                        mov              rdi, r13                                       # sig
                        mov              rsi, r15                                       # len
                        mov              rdx, qword ptr [rbp + 88]                      # cap_gen
                        call             rt_match_ctx_restore@PLT
                                                                                        jmp   n23_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n23_lit_string_α:
                        mov              qword ptr [rsp + 112], 2                       # result
                        mov              dword ptr [rsp + 116], 1
                        mov              rax, qword ptr [rip + .Lx101_0]
                        mov              qword ptr [rsp + 120], rax
                                                                                        jmp   n24_match_replace_α
n23_lit_string_β:
                        mov              rbp, qword ptr [rbp + 56]                      # old_rbp
                        add              rsp, 192
                                                                                        jmp   n9_var_α
.Lx101_0:
                        .quad            .Lx101_0_s
.Lx101_0_s:
                        .string          "#"
#-----------------------------------------------------------------------------------------------------------------------
n24_match_replace_α:
                        mov              rdi, qword ptr [rip + .Lx103_0]                # name
                        mov              rsi, qword ptr [rbp + 176]                     # sub_lo
                        mov              rdx, qword ptr [rbp + 184]                     # sub_hi
                        mov              ecx, dword ptr [rbp + 16]                      # start
                        mov              r8, qword ptr [rbp + 40]                       # end
                        lea              r9, [rbp + 112]                                # replp
                        call             rt_match_replace@PLT
                                                                                        jmp   .Lx103_1
.Lx103_0:
                        .quad            .Lx103_0_s
.Lx103_0_s:
                        .string          "VC"
.Lx103_1:
                        mov              rbp, qword ptr [rbp + 56]                      # old_rbp
                        add              rsp, 192
                                                                                        jmp   n25_lit_string_α
#=======================================================================================================================
#       OUTPUT   =
#-----------------------------------------------------------------------------------------------------------------------
n25_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
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
                        mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx105_0]                # name
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
                        mov              rax, qword ptr [1879052304]                    # V
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n28_assign_α
n27_var_β:
                        add              rsp, 16
                                                                                        jmp   n29_var_α
#-----------------------------------------------------------------------------------------------------------------------
n28_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052400], rax                    # PRINTV
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
                        sub              rsp, 752
                        mov              qword ptr [rsp + 0], 0                         # stmt_claim
                        mov              qword ptr [rsp + 8], 0
                        mov              qword ptr [rsp + 16], 0
                        mov              qword ptr [rsp + 24], 0
                        mov              qword ptr [rsp + 32], 0
                        mov              qword ptr [rsp + 40], 0
                        mov              qword ptr [rsp + 48], 0
                        mov              qword ptr [rsp + 56], 0
                        mov              qword ptr [rsp + 64], 0
                        mov              qword ptr [rsp + 72], 0
                        mov              qword ptr [rsp + 80], 0
                        mov              qword ptr [rsp + 88], 0
                        mov              qword ptr [rsp + 96], 0
                        mov              qword ptr [rsp + 104], 0
                        mov              qword ptr [rsp + 112], 0
                        mov              qword ptr [rsp + 120], 0
                        mov              qword ptr [rsp + 128], 0
                        mov              qword ptr [rsp + 136], 0
                        mov              qword ptr [rsp + 144], 0
                        mov              qword ptr [rsp + 152], 0
                        mov              qword ptr [rsp + 160], 0
                        mov              qword ptr [rsp + 168], 0
                        mov              qword ptr [rsp + 176], 0
                        mov              qword ptr [rsp + 184], 0
                        mov              qword ptr [rsp + 192], 0
                        mov              qword ptr [rsp + 200], 0
                        mov              qword ptr [rsp + 208], 0
                        mov              qword ptr [rsp + 216], 0
                        mov              qword ptr [rsp + 224], 0
                        mov              qword ptr [rsp + 232], 0
                        mov              qword ptr [rsp + 240], 0
                        mov              qword ptr [rsp + 248], 0
                        mov              qword ptr [rsp + 256], 0
                        mov              qword ptr [rsp + 264], 0
                        mov              qword ptr [rsp + 272], 0
                        mov              qword ptr [rsp + 280], 0
                        mov              qword ptr [rsp + 288], 0
                        mov              qword ptr [rsp + 296], 0
                        mov              qword ptr [rsp + 304], 0
                        mov              qword ptr [rsp + 312], 0
                        mov              qword ptr [rsp + 320], 0
                        mov              qword ptr [rsp + 328], 0
                        mov              qword ptr [rsp + 336], 0
                        mov              qword ptr [rsp + 344], 0
                        mov              qword ptr [rsp + 352], 0
                        mov              qword ptr [rsp + 360], 0
                        mov              qword ptr [rsp + 368], 0
                        mov              qword ptr [rsp + 376], 0
                        mov              qword ptr [rsp + 384], 0
                        mov              qword ptr [rsp + 392], 0
                        mov              qword ptr [rsp + 400], 0
                        mov              qword ptr [rsp + 408], 0
                        mov              qword ptr [rsp + 416], 0
                        mov              qword ptr [rsp + 424], 0
                        mov              qword ptr [rsp + 432], 0
                        mov              qword ptr [rsp + 440], 0
                        mov              qword ptr [rsp + 448], 0
                        mov              qword ptr [rsp + 456], 0
                        mov              qword ptr [rsp + 464], 0
                        mov              qword ptr [rsp + 472], 0
                        mov              qword ptr [rsp + 480], 0
                        mov              qword ptr [rsp + 488], 0
                        mov              qword ptr [rsp + 496], 0
                        mov              qword ptr [rsp + 504], 0
                        mov              qword ptr [rsp + 512], 0
                        mov              qword ptr [rsp + 520], 0
                        mov              qword ptr [rsp + 528], 0
                        mov              qword ptr [rsp + 536], 0
                        mov              qword ptr [rsp + 544], 0
                        mov              qword ptr [rsp + 552], 0
                        mov              qword ptr [rsp + 560], 0
                        mov              qword ptr [rsp + 568], 0
                        mov              qword ptr [rsp + 576], 0
                        mov              qword ptr [rsp + 584], 0
                        mov              qword ptr [rsp + 592], 0
                        mov              qword ptr [rsp + 600], 0
                        mov              qword ptr [rsp + 608], 0
                        mov              qword ptr [rsp + 616], 0
                        mov              qword ptr [rsp + 624], 0
                        mov              qword ptr [rsp + 632], 0
                        mov              qword ptr [rsp + 640], 0
                        mov              qword ptr [rsp + 648], 0
                        mov              qword ptr [rsp + 656], 0
                        mov              qword ptr [rsp + 664], 0
                        mov              qword ptr [rsp + 672], 0
                        mov              qword ptr [rsp + 680], 0
                        mov              qword ptr [rsp + 688], 0
                        mov              qword ptr [rsp + 696], 0
                        mov              qword ptr [rsp + 704], 0
                        mov              qword ptr [rsp + 712], 0
                        mov              qword ptr [rsp + 720], 0
                        mov              qword ptr [rsp + 728], 0
                        mov              qword ptr [rsp + 736], 0
                        mov              qword ptr [rsp + 744], 0
                        mov              rax, qword ptr [1879052400]                    # PRINTV
                        mov              rdx, qword ptr [1879052408]
                        mov              qword ptr [rsp + 736], rax                     # result
                        mov              qword ptr [rsp + 744], rdx
                                                                                        jmp   n30_var_α
n29_var_β:
                        add              rsp, 752
                                                                                        jmp   n39_var_α
#-----------------------------------------------------------------------------------------------------------------------
n30_var_α:
                        mov              rax, qword ptr [1879052384]                    # NV
                        mov              rdx, qword ptr [1879052392]
                        mov              qword ptr [rsp + 720], rax                     # result
                        mov              qword ptr [rsp + 728], rdx
                                                                                        jmp   n31_coerce_integer_α
n30_var_β:
                        add              rsp, 752
                                                                                        jmp   n39_var_α
#-----------------------------------------------------------------------------------------------------------------------
n31_coerce_integer_α:
                        lea              rdi, [rsp + 720]                               # in
                        lea              rsi, [rsp + 704]                               # out
                        mov              rdx, 10682530                                  # codes
                        call             rt_coerce_int_d@PLT
                                                                                        jmp   n32_match_begin_α
n31_coerce_integer_β:
                        add              rsp, 752
                                                                                        jmp   n39_var_α
#-----------------------------------------------------------------------------------------------------------------------
n32_match_begin_α:
                        mov              qword ptr [rsp + 616], rbp                     # old_rbp
                        mov              rbp, rsp                                       # stmt_base
                        mov              qword ptr [rbp + 624], r13                     # outer_Σ
                        mov              qword ptr [rbp + 632], r14                     # outer_δ
                        mov              qword ptr [rbp + 640], r15                     # outer_Δ
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rbp + 648], rax                     # cap_gen
                        mov              rdi, qword ptr [rsp + 736]                     # lo
                        mov              rsi, qword ptr [rsp + 744]                     # hi
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              r10, qword ptr [1879048192]                    # cas_top
                        mov              qword ptr [r10 + 0], 0
                        mov              qword ptr [r10 + 8], rsp                       # cas_rsp_mark
                        lea              rcx, [rip + g_patstk_sp]
                        mov              rax, qword ptr [rcx + 0]
                        mov              qword ptr [r10 + 16], rax                      # cas_patstk
                        add              r10, 24
                        mov              qword ptr [1879048192], r10                    # cas_top
                        mov              rax, rsp
                        sub              rsp, 32
                        mov              qword ptr [rsp + 16], rax                      # rsp_mark
                        lea              rcx, [rip + g_patstk_sp]
                        mov              rax, qword ptr [rcx + 0]
                        mov              qword ptr [rsp + 8], rax                       # patstk_mark
                        mov              dword ptr [rsp + 0], 0                         # start_δ
.Lx113_0:
                        mov              r14d, dword ptr [rsp + 0]
                                                                                        jmp   n33_match_sequence_α
n32_match_begin_β:
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
                        mov              r10, qword ptr [1879048192]
.Lx113_2:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx113_2
                        mov              rax, qword ptr [r10 + 16]                      # cas_patstk
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              rsp, qword ptr [r10 + 8]                       # cas_rsp_mark
                        mov              qword ptr [1879048192], r10
                        mov              r13, qword ptr [rbp + 624]                     # outer_Σ
                        mov              r14, qword ptr [rbp + 632]                     # outer_δ
                        mov              r15, qword ptr [rbp + 640]                     # outer_Δ
                        mov              rdi, r13                                       # sig
                        mov              rsi, r15                                       # len
                        mov              rdx, qword ptr [rbp + 648]                     # cap_gen
                        call             rt_match_ctx_restore@PLT
                        mov              rbp, qword ptr [rbp + 520]                     # old_rbp
                        add              rsp, 752
                                                                                        jmp   n39_var_α
#-----------------------------------------------------------------------------------------------------------------------
n33_match_sequence_α:
                                                                                        jmp   n38_match_pos_α
n33_match_sequence_as:
                                                                                        jmp   n34_match_end_α
n33_match_sequence_β:
                                                                                        jmp   n37_match_len_β
n33_match_sequence_af:
                                                                                        jmp   n32_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n34_match_end_α:
                        mov              r10, qword ptr [1879048192]
.Lx117_9:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx117_9
                        mov              rax, qword ptr [r10 + 16]
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              eax, dword ptr [rsp + 0]
                        mov              dword ptr [rsp + 704], eax
                        mov              qword ptr [rsp + 728], r14
                        mov              rsp, qword ptr [r10 + 8]
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
                        mov              r13, qword ptr [rbp + 624]                     # outer_Σ
                        mov              r14, qword ptr [rbp + 632]                     # outer_δ
                        mov              r15, qword ptr [rbp + 640]                     # outer_Δ
                        mov              rdi, r13                                       # sig
                        mov              rsi, r15                                       # len
                        mov              rdx, qword ptr [rbp + 648]                     # cap_gen
                        call             rt_match_ctx_restore@PLT
                                                                                        jmp   n35_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n35_lit_string_α:
                        mov              qword ptr [rsp + 672], 2                       # result
                        mov              dword ptr [rsp + 676], 1
                        mov              rax, qword ptr [rip + .Lx118_0]
                        mov              qword ptr [rsp + 680], rax
                                                                                        jmp   n36_match_replace_α
n35_lit_string_β:
                        mov              rbp, qword ptr [rbp + 616]                     # old_rbp
                        add              rsp, 752
                                                                                        jmp   n39_var_α
.Lx118_0:
                        .quad            .Lx118_0_s
.Lx118_0_s:
                        .string          "#"
#-----------------------------------------------------------------------------------------------------------------------
n36_match_replace_α:
                        mov              rdi, qword ptr [rip + .Lx120_0]                # name
                        mov              rsi, qword ptr [rbp + 736]                     # sub_lo
                        mov              rdx, qword ptr [rbp + 744]                     # sub_hi
                        mov              ecx, dword ptr [rbp + 576]                     # start
                        mov              r8, qword ptr [rbp + 600]                      # end
                        lea              r9, [rbp + 672]                                # replp
                        call             rt_match_replace@PLT
                                                                                        jmp   .Lx120_1
.Lx120_0:
                        .quad            .Lx120_0_s
.Lx120_0_s:
                        .string          "PRINTV"
.Lx120_1:
                        mov              rbp, qword ptr [rbp + 520]                     # old_rbp
                        add              rsp, 752
                                                                                        jmp   n39_var_α
#-----------------------------------------------------------------------------------------------------------------------
n37_match_len_α:
                        mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d
                                                                                        jg    n32_match_begin_β
                        add              r14d, 1
                                                                                        jmp   n34_match_end_α
n37_match_len_β:
                        sub              r14d, 1
                                                                                        jmp   n32_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n38_match_pos_α:
                        mov              rax, qword ptr [rbp + 712]
                        cmp              r14d, eax
                                                                                        jne   n32_match_begin_β
                                                                                        jmp   n37_match_len_α
n38_match_pos_β:
                                                                                        jmp   n32_match_begin_β
#=======================================================================================================================
# PRINT PRINTV   ?  LEN(1) . C =                     :F(NEXTV)
#-----------------------------------------------------------------------------------------------------------------------
n39_var_α:
                        sub              rsp, 944
                        mov              qword ptr [rsp + 0], 0                         # stmt_claim
                        mov              qword ptr [rsp + 8], 0
                        mov              qword ptr [rsp + 16], 0
                        mov              qword ptr [rsp + 24], 0
                        mov              qword ptr [rsp + 32], 0
                        mov              qword ptr [rsp + 40], 0
                        mov              qword ptr [rsp + 48], 0
                        mov              qword ptr [rsp + 56], 0
                        mov              qword ptr [rsp + 64], 0
                        mov              qword ptr [rsp + 72], 0
                        mov              qword ptr [rsp + 80], 0
                        mov              qword ptr [rsp + 88], 0
                        mov              qword ptr [rsp + 96], 0
                        mov              qword ptr [rsp + 104], 0
                        mov              qword ptr [rsp + 112], 0
                        mov              qword ptr [rsp + 120], 0
                        mov              qword ptr [rsp + 128], 0
                        mov              qword ptr [rsp + 136], 0
                        mov              qword ptr [rsp + 144], 0
                        mov              qword ptr [rsp + 152], 0
                        mov              qword ptr [rsp + 160], 0
                        mov              qword ptr [rsp + 168], 0
                        mov              qword ptr [rsp + 176], 0
                        mov              qword ptr [rsp + 184], 0
                        mov              qword ptr [rsp + 192], 0
                        mov              qword ptr [rsp + 200], 0
                        mov              qword ptr [rsp + 208], 0
                        mov              qword ptr [rsp + 216], 0
                        mov              qword ptr [rsp + 224], 0
                        mov              qword ptr [rsp + 232], 0
                        mov              qword ptr [rsp + 240], 0
                        mov              qword ptr [rsp + 248], 0
                        mov              qword ptr [rsp + 256], 0
                        mov              qword ptr [rsp + 264], 0
                        mov              qword ptr [rsp + 272], 0
                        mov              qword ptr [rsp + 280], 0
                        mov              qword ptr [rsp + 288], 0
                        mov              qword ptr [rsp + 296], 0
                        mov              qword ptr [rsp + 304], 0
                        mov              qword ptr [rsp + 312], 0
                        mov              qword ptr [rsp + 320], 0
                        mov              qword ptr [rsp + 328], 0
                        mov              qword ptr [rsp + 336], 0
                        mov              qword ptr [rsp + 344], 0
                        mov              qword ptr [rsp + 352], 0
                        mov              qword ptr [rsp + 360], 0
                        mov              qword ptr [rsp + 368], 0
                        mov              qword ptr [rsp + 376], 0
                        mov              qword ptr [rsp + 384], 0
                        mov              qword ptr [rsp + 392], 0
                        mov              qword ptr [rsp + 400], 0
                        mov              qword ptr [rsp + 408], 0
                        mov              qword ptr [rsp + 416], 0
                        mov              qword ptr [rsp + 424], 0
                        mov              qword ptr [rsp + 432], 0
                        mov              qword ptr [rsp + 440], 0
                        mov              qword ptr [rsp + 448], 0
                        mov              qword ptr [rsp + 456], 0
                        mov              qword ptr [rsp + 464], 0
                        mov              qword ptr [rsp + 472], 0
                        mov              qword ptr [rsp + 480], 0
                        mov              qword ptr [rsp + 488], 0
                        mov              qword ptr [rsp + 496], 0
                        mov              qword ptr [rsp + 504], 0
                        mov              qword ptr [rsp + 512], 0
                        mov              qword ptr [rsp + 520], 0
                        mov              qword ptr [rsp + 528], 0
                        mov              qword ptr [rsp + 536], 0
                        mov              qword ptr [rsp + 544], 0
                        mov              qword ptr [rsp + 552], 0
                        mov              qword ptr [rsp + 560], 0
                        mov              qword ptr [rsp + 568], 0
                        mov              qword ptr [rsp + 576], 0
                        mov              qword ptr [rsp + 584], 0
                        mov              qword ptr [rsp + 592], 0
                        mov              qword ptr [rsp + 600], 0
                        mov              qword ptr [rsp + 608], 0
                        mov              qword ptr [rsp + 616], 0
                        mov              qword ptr [rsp + 624], 0
                        mov              qword ptr [rsp + 632], 0
                        mov              qword ptr [rsp + 640], 0
                        mov              qword ptr [rsp + 648], 0
                        mov              qword ptr [rsp + 656], 0
                        mov              qword ptr [rsp + 664], 0
                        mov              qword ptr [rsp + 672], 0
                        mov              qword ptr [rsp + 680], 0
                        mov              qword ptr [rsp + 688], 0
                        mov              qword ptr [rsp + 696], 0
                        mov              qword ptr [rsp + 704], 0
                        mov              qword ptr [rsp + 712], 0
                        mov              qword ptr [rsp + 720], 0
                        mov              qword ptr [rsp + 728], 0
                        mov              qword ptr [rsp + 736], 0
                        mov              qword ptr [rsp + 744], 0
                        mov              qword ptr [rsp + 752], 0
                        mov              qword ptr [rsp + 760], 0
                        mov              qword ptr [rsp + 768], 0
                        mov              qword ptr [rsp + 776], 0
                        mov              qword ptr [rsp + 784], 0
                        mov              qword ptr [rsp + 792], 0
                        mov              qword ptr [rsp + 800], 0
                        mov              qword ptr [rsp + 808], 0
                        mov              qword ptr [rsp + 816], 0
                        mov              qword ptr [rsp + 824], 0
                        mov              qword ptr [rsp + 832], 0
                        mov              qword ptr [rsp + 840], 0
                        mov              qword ptr [rsp + 848], 0
                        mov              qword ptr [rsp + 856], 0
                        mov              qword ptr [rsp + 864], 0
                        mov              qword ptr [rsp + 872], 0
                        mov              qword ptr [rsp + 880], 0
                        mov              qword ptr [rsp + 888], 0
                        mov              qword ptr [rsp + 896], 0
                        mov              qword ptr [rsp + 904], 0
                        mov              qword ptr [rsp + 912], 0
                        mov              qword ptr [rsp + 920], 0
                        mov              qword ptr [rsp + 928], 0
                        mov              qword ptr [rsp + 936], 0
                        mov              rax, qword ptr [1879052400]                    # PRINTV
                        mov              rdx, qword ptr [1879052408]
                        mov              qword ptr [rsp + 928], rax                     # result
                        mov              qword ptr [rsp + 936], rdx
                                                                                        jmp   n40_match_begin_α
n39_var_β:
                        add              rsp, 944
                                                                                        jmp   n19_var_α
#-----------------------------------------------------------------------------------------------------------------------
n40_match_begin_α:
                        mov              qword ptr [rsp + 808], rbp                     # old_rbp
                        mov              rbp, rsp                                       # stmt_base
                        mov              qword ptr [rbp + 816], r13                     # outer_Σ
                        mov              qword ptr [rbp + 824], r14                     # outer_δ
                        mov              qword ptr [rbp + 832], r15                     # outer_Δ
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rbp + 840], rax                     # cap_gen
                        mov              rdi, qword ptr [rsp + 928]                     # lo
                        mov              rsi, qword ptr [rsp + 936]                     # hi
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              r10, qword ptr [1879048192]                    # cas_top
                        mov              qword ptr [r10 + 0], 0
                        mov              qword ptr [r10 + 8], rsp                       # cas_rsp_mark
                        lea              rcx, [rip + g_patstk_sp]
                        mov              rax, qword ptr [rcx + 0]
                        mov              qword ptr [r10 + 16], rax                      # cas_patstk
                        add              r10, 24
                        mov              qword ptr [1879048192], r10                    # cas_top
                        mov              rax, rsp
                        sub              rsp, 32
                        mov              qword ptr [rsp + 16], rax                      # rsp_mark
                        lea              rcx, [rip + g_patstk_sp]
                        mov              rax, qword ptr [rcx + 0]
                        mov              qword ptr [rsp + 8], rax                       # patstk_mark
                        mov              dword ptr [rsp + 0], 0                         # start_δ
.Lx125_0:
                        mov              r14d, dword ptr [rsp + 0]
                                                                                        jmp   n41_match_assign_save_α
n40_match_begin_β:
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
                        mov              r10, qword ptr [1879048192]
.Lx125_2:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx125_2
                        mov              rax, qword ptr [r10 + 16]                      # cas_patstk
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              rsp, qword ptr [r10 + 8]                       # cas_rsp_mark
                        mov              qword ptr [1879048192], r10
                        mov              r13, qword ptr [rbp + 816]                     # outer_Σ
                        mov              r14, qword ptr [rbp + 824]                     # outer_δ
                        mov              r15, qword ptr [rbp + 832]                     # outer_Δ
                        mov              rdi, r13                                       # sig
                        mov              rsi, r15                                       # len
                        mov              rdx, qword ptr [rbp + 840]                     # cap_gen
                        call             rt_match_ctx_restore@PLT
                        mov              rbp, qword ptr [rbp + 712]                     # old_rbp
                        add              rsp, 944
                                                                                        jmp   n19_var_α
#-----------------------------------------------------------------------------------------------------------------------
n41_match_assign_save_α:
                        sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d
                                                                                        jmp   n42_match_len_α
n41_match_assign_save_β:
                        add              rsp, 16
                                                                                        jmp   n40_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n42_match_len_α:
                        mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d
                                                                                        jle   .Lx128_240
                        add              rsp, 16
                                                                                        jmp   n40_match_begin_β
.Lx128_240:
                        add              r14d, 1
                                                                                        jmp   n43_match_assign_cond_α
n42_match_len_β:
                        sub              r14d, 1
                        add              rsp, 16
                                                                                        jmp   n40_match_begin_β
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
                                                                                        jmp   n44_match_end_α
n43_match_assign_cond_β:
                        mov              rax, qword ptr [1879048192]
                        sub              rax, 24
                        mov              qword ptr [1879048192], rax
                                                                                        jmp   n42_match_len_β
#-----------------------------------------------------------------------------------------------------------------------
n44_match_end_α:
                        mov              r10, qword ptr [1879048192]
.Lx132_9:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx132_9
                        mov              rax, qword ptr [r10 + 16]
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              eax, dword ptr [rsp + 16]
                        mov              dword ptr [rsp + 912], eax
                        mov              qword ptr [rsp + 936], r14
                        mov              rsp, qword ptr [r10 + 8]
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
                        mov              r13, qword ptr [rbp + 816]                     # outer_Σ
                        mov              r14, qword ptr [rbp + 824]                     # outer_δ
                        mov              r15, qword ptr [rbp + 832]                     # outer_Δ
                        mov              rdi, r13                                       # sig
                        mov              rsi, r15                                       # len
                        mov              rdx, qword ptr [rbp + 840]                     # cap_gen
                        call             rt_match_ctx_restore@PLT
                                                                                        jmp   n45_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n45_lit_string_α:
                        mov              qword ptr [rsp + 864], 2                       # result
                        mov              dword ptr [rsp + 868], 0
                        mov              rax, qword ptr [rip + .Lx133_0]
                        mov              qword ptr [rsp + 872], rax
                                                                                        jmp   n46_match_replace_α
n45_lit_string_β:
                        mov              rbp, qword ptr [rbp + 808]                     # old_rbp
                        add              rsp, 944
                                                                                        jmp   n19_var_α
.Lx133_0:
                        .quad            .Lx133_0_s
.Lx133_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n46_match_replace_α:
                        mov              rdi, qword ptr [rip + .Lx135_0]                # name
                        mov              rsi, qword ptr [rbp + 928]                     # sub_lo
                        mov              rdx, qword ptr [rbp + 936]                     # sub_hi
                        mov              ecx, dword ptr [rbp + 768]                     # start
                        mov              r8, qword ptr [rbp + 792]                      # end
                        lea              r9, [rbp + 864]                                # replp
                        call             rt_match_replace@PLT
                                                                                        jmp   .Lx135_1
.Lx135_0:
                        .quad            .Lx135_0_s
.Lx135_0_s:
                        .string          "PRINTV"
.Lx135_1:
                        mov              rbp, qword ptr [rbp + 712]                     # old_rbp
                        add              rsp, 944
                                                                                        jmp   n47_var_α
#=======================================================================================================================
#       OUTPUT   =  DIFFER(C, '#') DUPL(' ', NH) C   :S(PRINT)
#-----------------------------------------------------------------------------------------------------------------------
n47_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052416]                    # C
                        mov              rdx, qword ptr [1879052424]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n48_lit_string_α
n47_var_β:
                        add              rsp, 16
                                                                                        jmp   n57_var_α
#-----------------------------------------------------------------------------------------------------------------------
n48_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx137_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n49_call_α
n48_lit_string_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n57_var_α
.Lx137_0:
                        .quad            .Lx137_0_s
.Lx137_0_s:
                        .string          "#"
#-----------------------------------------------------------------------------------------------------------------------
n49_call_α:
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
.Lrkfnzd139:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd139]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 2                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 32
                        cmp              eax, 104
                                                                                        jne   .Lx138_240
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n57_var_α
.Lx138_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n50_lit_string_α
n49_call_β:
                        add              rsp, 16
                        add              rsp, 32
                                                                                        jmp   n57_var_α
#-----------------------------------------------------------------------------------------------------------------------
n50_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx140_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n51_var_α
n50_lit_string_β:
                        add              rsp, 16
                        add              rsp, 48
                                                                                        jmp   n57_var_α
.Lx140_0:
                        .quad            .Lx140_0_s
.Lx140_0_s:
                        .string          " "
#-----------------------------------------------------------------------------------------------------------------------
n51_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # NH
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n52_call_α
n51_var_β:
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n57_var_α
#-----------------------------------------------------------------------------------------------------------------------
n52_call_α:
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
.Lrkfnzd143:            .string          "DUPL"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd143]                       # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 2                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 32
                        cmp              eax, 104
                                                                                        jne   .Lx142_240
                        add              rsp, 16
                        add              rsp, 80
                                                                                        jmp   n57_var_α
.Lx142_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n53_binop_α
n52_call_β:
                        add              rsp, 16
                        add              rsp, 80
                                                                                        jmp   n57_var_α
#-----------------------------------------------------------------------------------------------------------------------
n53_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 64]                      # call
                        mov              rsi, qword ptr [rsp + 72]                      # a
                        mov              rdx, qword ptr [rsp + 16]                      # b
                        mov              rcx, qword ptr [rsp + 24]                      # b
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n54_var_α
n53_binop_β:
                        add              rsp, 16
                        add              rsp, 96
                                                                                        jmp   n57_var_α
#-----------------------------------------------------------------------------------------------------------------------
n54_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052416]                    # C
                        mov              rdx, qword ptr [1879052424]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n55_binop_α
n54_var_β:
                        add              rsp, 16
                        add              rsp, 112
                                                                                        jmp   n57_var_α
#-----------------------------------------------------------------------------------------------------------------------
n55_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # binop
                        mov              rsi, qword ptr [rsp + 40]                      # a
                        mov              rdx, qword ptr [rsp + 16]                      # var
                        mov              rcx, qword ptr [rsp + 24]                      # b
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n56_assign_α
n55_binop_β:
                        add              rsp, 16
                        add              rsp, 128
                                                                                        jmp   n57_var_α
#-----------------------------------------------------------------------------------------------------------------------
n56_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx147_0]                # name
                        call             NV_SET_fn@PLT
                        add              rsp, 144
                                                                                        jmp   n39_var_α
n56_assign_β:
                        add              rsp, 144
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
                        mov              rax, qword ptr [1879052288]                    # H
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n58_assign_α
n57_var_β:
                        add              rsp, 16
                                                                                        jmp   n39_var_α
#-----------------------------------------------------------------------------------------------------------------------
n58_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx149_0]                # name
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
                        mov              rax, qword ptr [1879052352]                    # CROSS
                        mov              rdx, qword ptr [1879052360]
                        cmp              eax, 8
                                                                                        jne   .Lx150_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx150_10
                        mov              rdi, rdx                                       # headv
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
                                                                                        jmp   n22_match_end_α
.Lx150_5:
                                                                                        jmp   n60_match_atp_β
.Lx150_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S1]                               # varname
                        xor              esi, esi                                       # ival_flag
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
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx150_2
.Lx150_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx150_2
.Lx150_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d                                      # cur_delta
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n60_match_atp_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx150_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n22_match_end_α
.Lx150_6:
                        add              rsp, 16
                                                                                        jmp   n60_match_atp_β
n59_match_patref_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n60_match_atp_α:
                        mov              esi, r14d                                      # cur_delta
                        lea              rdi, [rip + .S2]                               # varname
                        call             rt_at_cursor@PLT
                                                                                        jmp   n59_match_patref_α
n60_match_atp_β:
                                                                                        jmp   n20_match_begin_β
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
                                                                                        jmp   n14_match_end_α
n63_match_assign_cond_β:
                        mov              rax, qword ptr [1879048192]
                        sub              rax, 24
                        mov              qword ptr [1879048192], rax
                                                                                        jmp   n62_match_any_β
#-----------------------------------------------------------------------------------------------------------------------
n64_match_atp_α:
                        mov              esi, r14d                                      # cur_delta
                        lea              rdi, [rip + .S3]                               # varname
                        call             rt_at_cursor@PLT
                                                                                        jmp   n61_match_assign_save_α
n64_match_atp_β:
                                                                                        jmp   n12_match_begin_β
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
                        mov              rsp, rbp
                        pop              rbp
                        xor              edi, edi
                        call             exit@PLT
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              rsp, rbp
                        pop              rbp
                        mov              edi, 1
                        call             exit@PLT
                        .section         .rodata
.S0:                    .string          "C"
.S1:                    .string          "CROSS"
.S2:                    .string          "NV"
.S3:                    .string          "NH"
                        .text
                        .section         .note.GNU-stack,"",@progbits
