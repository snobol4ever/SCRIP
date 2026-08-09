                        .intel_syntax    noprefix
                        .text
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_PAT$0_α
proc_PAT$0_α:
                        sub              rsp, 304
                        mov              qword ptr [rsp + 280], rcx
                        mov              qword ptr [rsp + 288], rdx
                        mov              qword ptr [rsp + 296], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp + 272], r8
                        mov              dword ptr [rsp + 264], r14d
proc_PAT$0_attempt:
proc_PAT$0_α_body:
                        lea              rax, [rip + proc_PAT$0_ω]
                        mov              qword ptr [rbp + 240], rax
#-----------------------------------------------------------------------------------------------------------------------
n0_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx16_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n1_match_pos_α
.Lx16_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n1_match_pos_α:
                        mov              rax, 0
                        cmp              r14d, eax
                                                                                        jne   proc_PAT$0_scanfail
                                                                                        jmp   n2_match_assign_save_α
n1_match_pos_β:
                                                                                        jmp   proc_PAT$0_scanfail
#-----------------------------------------------------------------------------------------------------------------------
n2_match_assign_save_α:
                        sub              rsp, 16
                        mov              dword ptr [rbp + 80], r14d
                                                                                        jmp   n3_match_len_α
n2_match_assign_save_β:
                        add              rsp, 16
                                                                                        jmp   proc_PAT$0_scanfail
#-----------------------------------------------------------------------------------------------------------------------
n3_match_len_α:
                        mov              eax, r14d
                        add              eax, 4
                        cmp              eax, r15d
                                                                                        jle   .Lx20_240
                        add              rsp, 16
                                                                                        jmp   proc_PAT$0_scanfail
.Lx20_240:
                        add              r14d, 4
                                                                                        jmp   n4_match_assign_cond_α
n3_match_len_β:
                        sub              r14d, 4
                        add              rsp, 16
                                                                                        jmp   proc_PAT$0_scanfail
#-----------------------------------------------------------------------------------------------------------------------
n4_match_assign_cond_α:
                        mov              eax, dword ptr [rbp + 80]
                        lea              rcx, [rip + .S0]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24
                                                                                        jmp   n5_lit_integer_α
n4_match_assign_cond_β:
                        sub              r12, 24
                                                                                        jmp   n3_match_len_β
#-----------------------------------------------------------------------------------------------------------------------
n5_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx23_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n6_match_tab_α
.Lx23_0:
                        .quad            6
#-----------------------------------------------------------------------------------------------------------------------
n6_match_tab_α:
                        sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d
                        mov              rax, qword ptr [rsp + 24]                      # lit_integer
                        cmp              r14d, eax
                                                                                        jle   .Lx24_239
                        add              rsp, 16
                                                                                        jmp   n4_match_assign_cond_β
.Lx24_239:
                        cmp              r15d, eax
                                                                                        jge   .Lx24_240
                        add              rsp, 16
                                                                                        jmp   n4_match_assign_cond_β
.Lx24_240:
                        mov              r14d, eax
                                                                                        jmp   n7_match_assign_save_α
n6_match_tab_β:
                        mov              r14d, dword ptr [rsp + 0]
                        add              rsp, 16
                                                                                        jmp   n4_match_assign_cond_β
#-----------------------------------------------------------------------------------------------------------------------
n7_match_assign_save_α:
                        sub              rsp, 16
                        mov              dword ptr [rbp + 144], r14d
                                                                                        jmp   n8_match_arb_α
n7_match_assign_save_β:
                        add              rsp, 16
                                                                                        jmp   n6_match_tab_β
#-----------------------------------------------------------------------------------------------------------------------
n8_match_arb_α:
                        sub              rsp, 16
                        mov              dword ptr [rsp + 0], 0
                        mov              eax, r14d
                        mov              dword ptr [rsp + 4], eax
                                                                                        jmp   n9_match_assign_cond_α
n8_match_arb_β:
                        add              dword ptr [rsp + 0], 1
                        mov              eax, dword ptr [rsp + 4]
                        add              eax, dword ptr [rsp + 0]
                        cmp              eax, r15d
                                                                                        jg    .Lx28_0
                        mov              r14d, eax
                                                                                        jmp   n9_match_assign_cond_α
.Lx28_0:
                        mov              r14d, dword ptr [rsp + 4]
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n6_match_tab_β
#-----------------------------------------------------------------------------------------------------------------------
n9_match_assign_cond_α:
                        mov              eax, dword ptr [rbp + 144]
                        lea              rcx, [rip + .S1]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24
                                                                                        jmp   n10_match_lit_α
n9_match_assign_cond_β:
                        sub              r12, 24
                                                                                        jmp   n8_match_arb_β
#-----------------------------------------------------------------------------------------------------------------------
n10_match_lit_α:
                        mov              eax, r14d
                        add              eax, 2
                        cmp              eax, r15d
                                                                                        jg    n9_match_assign_cond_β
                        movsxd           rcx, r14d
                        movzx            eax, byte ptr [r13+rcx]
                        cmp              eax, 32
                                                                                        jne   n9_match_assign_cond_β
                        movzx            eax, byte ptr [r13+rcx+1]
                        cmp              eax, 58
                                                                                        jne   n9_match_assign_cond_β
                        add              r14d, 2
                                                                                        jmp   n11_lit_integer_α
n10_match_lit_β:
                        sub              r14d, 2
                                                                                        jmp   n9_match_assign_cond_β
#-----------------------------------------------------------------------------------------------------------------------
n11_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx33_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n12_match_tab_α
.Lx33_0:
                        .quad            24
#-----------------------------------------------------------------------------------------------------------------------
n12_match_tab_α:
                        sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d
                        mov              rax, qword ptr [rsp + 24]                      # lit_integer
                        cmp              r14d, eax
                                                                                        jle   .Lx34_239
                        add              rsp, 16
                                                                                        jmp   n10_match_lit_β
.Lx34_239:
                        cmp              r15d, eax
                                                                                        jge   .Lx34_240
                        add              rsp, 16
                                                                                        jmp   n10_match_lit_β
.Lx34_240:
                        mov              r14d, eax
                                                                                        jmp   n13_match_assign_save_α
n12_match_tab_β:
                        mov              r14d, dword ptr [rsp + 0]
                        add              rsp, 16
                                                                                        jmp   n10_match_lit_β
#-----------------------------------------------------------------------------------------------------------------------
n13_match_assign_save_α:
                        sub              rsp, 16
                        mov              dword ptr [rbp + 224], r14d
                                                                                        jmp   n14_match_rem_α
n13_match_assign_save_β:
                        add              rsp, 16
                                                                                        jmp   n12_match_tab_β
#-----------------------------------------------------------------------------------------------------------------------
n14_match_rem_α:
                        sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d
                        mov              r14d, r15d
                                                                                        jmp   n15_match_assign_cond_α
n14_match_rem_β:
                        mov              r14d, dword ptr [rsp + 0]
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n12_match_tab_β
#-----------------------------------------------------------------------------------------------------------------------
n15_match_assign_cond_α:
                        mov              eax, dword ptr [rbp + 224]
                        lea              rcx, [rip + .S2]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24
                        add              rsp, 160
                                                                                        jmp   proc_PAT$0_scanhit
n15_match_assign_cond_β:
                        sub              r12, 24
                                                                                        jmp   n14_match_rem_β
proc_PAT$0_scanhit:
                        cmp              qword ptr [rbp + 272], 1
                                                                                        jne   .Lx40_0
                        mov              ecx, dword ptr [rbp + 264]
                        lea              rdx, [rip + g_scan_hit_start]
                        mov              dword ptr [rdx + 0], ecx
.Lx40_0:
                                                                                        jmp   proc_PAT$0_γ
proc_PAT$0_scanfail:
                        cmp              qword ptr [rbp + 272], 1
                                                                                        jne   .Lx41_0
                        mov              eax, dword ptr [rbp + 264]
                        inc              eax
                        cmp              eax, r15d
                                                                                        jg    .Lx41_0
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        cmp              qword ptr [rcx + 0], 0
                                                                                        jne   .Lx41_0
                        mov              dword ptr [rbp + 264], eax
                        mov              r14d, eax
                        mov              rsp, rbp
                                                                                        jmp   proc_PAT$0_attempt
.Lx41_0:
                                                                                        jmp   proc_PAT$0_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$0_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$0_β:
                                                                                        jmp   qword ptr [rbp + 240]
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$0_γ:
                        push             rbp
                        lea              rax, [rip + proc_PAT$0_res]
                        push             rax
                        mov              rax, qword ptr [rbp + 280]
                        mov              rbp, qword ptr [rbp + 296]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$0_ω:
                        mov              rax, qword ptr [rbp + 288]
                        lea              rsp, [rbp + 304]
                        mov              rbp, qword ptr [rbp + 296]
                                                                                        jmp   rax
proc_startup:
                        sub              rsp, 8
                        .section         .rodata
.Lstartup_pname0:       .string          "PAT$0"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname0]
                        lea              rsi, [rip + proc_PAT$0_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 256
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 1
                        call             rt_proc_set_zstatic@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        add              rsp, 8
                        ret
                        .section         .rodata
.Lgvan0:                .string          "PAT"
.Lgvan1:                .string          "WHEN"
.Lgvan2:                .string          "WHO"
.Lgvan3:                .string          "WHAT"
.Lgvan4:                .string          "LINE"
                        .align           8
__gva_names:
                        .quad            .Lgvan0
                        .quad            .Lgvan1
                        .quad            .Lgvan2
                        .quad            .Lgvan3
                        .quad            .Lgvan4
                        .section         .text
                        .intel_syntax    noprefix
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        call             proc_startup
                        mov              edi, 5
                        call             rt_gva_island@PLT
                        mov              rsi, rax
                        lea              rdi, [rip + __gva_names]
                        mov              edx, 5
                        call             gva_register@PLT
                        mov              r12, qword ptr [0x70000000]
                        xor              esi, esi
                                                                                        jmp   main_α
#-----------------------------------------------------------------------------------------------------------------------
main_α:
main_α_body:
                        push             rbp
                        mov              rbp, rsp
                        sub              rsp, 8
#-----------------------------------------------------------------------------------------------------------------------
n42_statement_begin_α:
                                                                                        jmp   n43_statement_end_α
n42_statement_begin_β:
                                                                                        jmp   n44_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n43_statement_end_α:
                                                                                        jmp   n44_statement_begin_α
#=======================================================================================================================
#       PAT      =  POS(0) LEN(4) . WHEN
#-----------------------------------------------------------------------------------------------------------------------
n44_statement_begin_α:
                                                                                        jmp   n45_lit_string_α
n44_statement_begin_β:
                                                                                        jmp   n49_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n45_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx80_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n46_call_α
.Lx80_0:
                        .quad            .Lx80_0_s
.Lx80_0_s:
                        .string          "PAT$0"
#-----------------------------------------------------------------------------------------------------------------------
n46_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd82:             .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd82]                        # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx81_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n44_statement_begin_β
.Lx81_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n47_assign_α
n46_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n44_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n47_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax                    # PAT
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n48_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n48_statement_end_α:
                        add              rsp, 32
                                                                                        jmp   n49_statement_begin_α
#=======================================================================================================================
# LOOP  LINE     =  INPUT                            :F(END)
#-----------------------------------------------------------------------------------------------------------------------
n49_statement_begin_α:
                                                                                        jmp   n50_var_α
n49_statement_begin_β:
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n50_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx88_0]                 # name
                        call             NV_GET_fn@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx88_240
                        add              rsp, 16
                                                                                        jmp   n49_statement_begin_β
.Lx88_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n51_assign_α
.Lx88_0:
                        .quad            .Lx88_0_s
.Lx88_0_s:
                        .string          "INPUT"
#-----------------------------------------------------------------------------------------------------------------------
n51_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052352], rax                    # LINE
                        mov              qword ptr [1879052360], rdx
                                                                                        jmp   n52_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n52_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n53_statement_begin_α
#=======================================================================================================================
#       LINE     ?  PAT                              :F(LOOP)
#-----------------------------------------------------------------------------------------------------------------------
n53_statement_begin_α:
                                                                                        jmp   n54_var_α
n53_statement_begin_β:
                                                                                        jmp   n49_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n54_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # LINE
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n55_var_α
#-----------------------------------------------------------------------------------------------------------------------
n55_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052288]                    # PAT
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n56_assign_α
n55_var_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n49_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n56_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx96_0]                 # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n57_match_begin_α
n56_assign_β:
                                                                                        jmp   n55_var_β
.Lx96_0:
                        .quad            .Lx96_0_s
.Lx96_0_s:
                        .string          "PATV$0"
#-----------------------------------------------------------------------------------------------------------------------
n57_match_begin_α:
                        mov              rdi, qword ptr [rsp + 16]                      # var
                        mov              rsi, qword ptr [rsp + 24]
                        sub              rsp, 64
                        mov              qword ptr [rbp + -40], r13                     # outer_Σ
                        mov              qword ptr [rbp + -32], r14                     # outer_δ
                        mov              qword ptr [rbp + -24], r15                     # outer_Δ
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rbp + -16], rax                     # cap_gen
                        mov              qword ptr [rbp + -48], rbp                     # old_rbp
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              qword ptr [r12 + 0], 0                         # cas_top
                        mov              qword ptr [r12 + 8], rsp                       # cas_rsp_mark
                        mov              qword ptr [r12 + 16], 0
                        add              r12, 24                                        # cas_top
                        mov              qword ptr [rbp + -56], rsp                     # zls2_mark
                        mov              dword ptr [rbp + -64], 0                       # start_δ
.Lx98_0:
                        mov              r14d, dword ptr [rbp + -64]
                                                                                        jmp   n58_match_defer_α
n57_match_begin_β:
                        add              dword ptr [rbp + -64], 1
                        mov              eax, dword ptr [rbp + -64]
                        cmp              eax, r15d
                                                                                        jg    .Lx98_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                                                                                        jne   .Lx98_1
                                                                                        jmp   .Lx98_0
.Lx98_1:
n57_match_begin_af:
                        sub              r12, 24                                        # cas_mark
                        mov              rsp, qword ptr [rbp + -56]
                        mov              r13, qword ptr [rbp + -40]                     # outer_Σ
                        mov              r14, qword ptr [rbp + -32]                     # outer_δ
                        mov              r15, qword ptr [rbp + -24]                     # outer_Δ
                        mov              rdi, r13                                       # sig
                        mov              rsi, r15                                       # len
                        mov              rdx, qword ptr [rbp + -16]                     # cap_gen
                        call             rt_match_ctx_restore@PLT
                        mov              rbp, qword ptr [rbp + -48]                     # old_rbp
                                                                                        jmp   n56_assign_β
#-----------------------------------------------------------------------------------------------------------------------
n58_match_defer_α:
                        lea              rdi, [rip + .S3]                               # varname
                        xor              esi, esi                                       # ival_flag
                        call             rt_defer_get_pat_fn@PLT
                        test             rax, rax
                                                                                        jz    .Lx99_0
                        mov              r8d, 1
                        lea              rcx, [rip + .Lx99_4]
                        lea              rdx, [rip + .Lx99_5]
                                                                                        jmp   rax
.Lx99_4:
                        lea              rcx, [rip + g_scan_hit_start]
                        mov              rax, qword ptr [rcx]
                        mov              dword ptr [rbp + 112], eax
                                                                                        jmp   n59_match_end_α
.Lx99_5:
                                                                                        jmp   n57_match_begin_β
.Lx99_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S3]                               # varname
                        xor              esi, esi                                       # ival_flag
                        call             rt_defer_open@PLT
.Lx99_2:
                        test             rax, rax
                                                                                        je    .Lx99_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx99_7]
                        lea              rdx, [rip + .Lx99_8]
                                                                                        jmp   rax
.Lx99_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx99_2
.Lx99_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx99_2
.Lx99_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d                                      # cur_delta
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n57_match_begin_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx99_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n59_match_end_α
.Lx99_6:
                        add              rsp, 16
                                                                                        jmp   n57_match_begin_β
n58_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n59_match_end_α:
                        mov              r10, r12
.Lx101_9:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx101_9
                        mov              rsp, qword ptr [rbp + -56]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, r12
                        mov              r10, rsi
.Lx101_5:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx101_5
                        lea              rdi, [r10 + 24]
                        mov              rdx, r13
                        call             rt_dcap_end_ok_open@PLT
.Lx101_1:
                        test             rax, rax
                                                                                        je    .Lx101_2
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx101_3]
                        lea              rdx, [rip + .Lx101_4]
                                                                                        jmp   rax
.Lx101_3:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx101_1
.Lx101_4:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx101_1
.Lx101_2:
                        call             rt_dcap_end_ok_close@PLT
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        sub              r12, 24                                        # cas_mark
                        mov              r13, qword ptr [rbp + -40]                     # outer_Σ
                        mov              r14, qword ptr [rbp + -32]                     # outer_δ
                        mov              r15, qword ptr [rbp + -24]                     # outer_Δ
                        mov              rdi, r13                                       # sig
                        mov              rsi, r15                                       # len
                        call             rt_match_ctx_restore@PLT
                        mov              rbp, qword ptr [rbp + -48]                     # old_rbp
                                                                                        jmp   n60_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n60_statement_end_α:
                        add              rsp, 32
                                                                                        jmp   n61_statement_begin_α
#=======================================================================================================================
#       OUTPUT   =  WHO " invented the " WHAT " in " WHEN  :(LOOP)
#-----------------------------------------------------------------------------------------------------------------------
n61_statement_begin_α:
                                                                                        jmp   n62_var_α
n61_statement_begin_β:
                                                                                        jmp   n49_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n62_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]                    # WHO
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n63_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n63_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 14
                        mov              rax, qword ptr [rip + .Lx107_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n64_binop_α
n63_lit_string_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n61_statement_begin_β
.Lx107_0:
                        .quad            .Lx107_0_s
.Lx107_0_s:
                        .string          " invented the "
#-----------------------------------------------------------------------------------------------------------------------
n64_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # a
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # b
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n65_var_α
n64_binop_β:
                        add              rsp, 16
                                                                                        jmp   n63_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n65_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # WHAT
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n66_binop_α
n65_var_β:
                        add              rsp, 16
                                                                                        jmp   n64_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n66_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # binop
                        mov              rsi, qword ptr [rsp + 40]                      # a
                        mov              rdx, qword ptr [rsp + 16]                      # var
                        mov              rcx, qword ptr [rsp + 24]                      # b
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n67_lit_string_α
n66_binop_β:
                        add              rsp, 16
                                                                                        jmp   n65_var_β
#-----------------------------------------------------------------------------------------------------------------------
n67_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx111_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n68_binop_α
n67_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n66_binop_β
.Lx111_0:
                        .quad            .Lx111_0_s
.Lx111_0_s:
                        .string          " in "
#-----------------------------------------------------------------------------------------------------------------------
n68_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # binop
                        mov              rsi, qword ptr [rsp + 40]                      # a
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # b
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n69_var_α
n68_binop_β:
                        add              rsp, 16
                                                                                        jmp   n67_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n69_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]                    # WHEN
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n70_binop_α
n69_var_β:
                        add              rsp, 16
                                                                                        jmp   n68_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n70_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # binop
                        mov              rsi, qword ptr [rsp + 40]                      # a
                        mov              rdx, qword ptr [rsp + 16]                      # var
                        mov              rcx, qword ptr [rsp + 24]                      # b
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n71_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n71_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx115_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n72_statement_end_α
.Lx115_0:
                        .quad            .Lx115_0_s
.Lx115_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n72_statement_end_α:
                        add              rsp, 144
                                                                                        jmp   n49_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n73_goto_α:
                                                                                        jmp   n49_statement_begin_α
n73_goto_β:
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
.S0:                    .string          "WHEN"
.S1:                    .string          "WHO"
.S2:                    .string          "WHAT"
.S3:                    .string          "PATV$0"
                        .text
                        .section         .note.GNU-stack,"",@progbits
