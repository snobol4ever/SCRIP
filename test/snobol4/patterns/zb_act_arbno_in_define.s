                        .intel_syntax    noprefix
                        .text
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__MATCHIT_α:
proc_LBL__MATCHIT_α_body:
#=======================================================================================================================
# MATCHIT S POS(0) ARBNO('a') . V RPOS(0)                            :S(MI_YES)
#-----------------------------------------------------------------------------------------------------------------------
n0_statement_begin_α:
                                                                                        jmp   n1_var_α
n0_statement_begin_β:
                                                                                        jmp   n17_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n1_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]                    # S
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n2_match_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n2_match_begin_α:
                        mov              rdi, qword ptr [rsp + 0]                       # var
                        mov              rsi, qword ptr [rsp + 8]
                        mov              qword ptr [rsp + 88], rbp                      # old_rbp
                        mov              rbp, rsp                                       # stmt_base
                        mov              qword ptr [rsp + 48], r13                      # outer_Σ
                        mov              qword ptr [rsp + 56], r14                      # outer_δ
                        mov              qword ptr [rsp + 64], r15                      # outer_Δ
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rsp + 72], rax                      # cap_gen
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
.Lx26_0:
                        mov              r14d, dword ptr [rsp + 0]
                                                                                        jmp   n3_lit_integer_α
n2_match_begin_β:
                        mov              rsp, qword ptr [r12 + -16]                     # rsp_restore
                        sub              rsp, 32
                        add              dword ptr [rsp + 0], 1                         # start_δ
                        mov              eax, dword ptr [rsp + 0]
                        cmp              eax, r15d
                                                                                        jg    .Lx26_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                                                                                        jne   .Lx26_1
                                                                                        jmp   .Lx26_0
.Lx26_1:
n2_match_begin_af:
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
                                                                                        jmp   n17_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n3_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx27_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n4_match_pos_α
.Lx27_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n4_match_pos_α:
                        mov              rax, 0
                        cmp              r14d, eax
                                                                                        je    .Lx28_240
                        add              rsp, 16
                                                                                        jmp   n2_match_begin_β
.Lx28_240:
                                                                                        jmp   n5_match_assign_save_α
n4_match_pos_β:
                        add              rsp, 16
                                                                                        jmp   n2_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n5_match_assign_save_α:
                        sub              rsp, 16
                        mov              dword ptr [rbp + 192], r14d
                                                                                        jmp   n6_match_arbno_α
n5_match_assign_save_β:
                        add              rsp, 16
                                                                                        jmp   n2_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n6_match_arbno_α:
                        sub              rsp, 80
                        mov              eax, 0
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        mov              qword ptr [rsp + 72], rax
                        mov              dword ptr [rsp + 32], r14d
                        mov              dword ptr [rsp + 36], r14d
                        mov              dword ptr [rsp + 40], 1
                        mov              rax, qword ptr [rsp + 120]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [rsp + 128]
                        mov              qword ptr [rsp + 56], rax
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [rsp + 64], rax
                                                                                        jmp   n7_match_assign_cond_α
n6_match_arbno_β:
                        add              rsp, 16
                        mov              r14d, dword ptr [rsp + 36]
                        mov              rax, qword ptr [rsp + 48]
                        mov              rcx, qword ptr [rsp + 56]
                        sub              rsp, 80
                        mov              edx, 0
                        mov              qword ptr [rsp + 0], rdx
                        mov              qword ptr [rsp + 8], rdx
                        mov              qword ptr [rsp + 16], rdx
                        mov              qword ptr [rsp + 24], rdx
                        mov              dword ptr [rsp + 32], r14d
                        mov              dword ptr [rsp + 36], r14d
                        mov              dword ptr [rsp + 40], 0
                        mov              qword ptr [rsp + 48], rax
                        mov              qword ptr [rsp + 56], rcx
                        mov              qword ptr [rsp + 72], rdx
                        mov              rax, qword ptr [rsp + 144]
                        mov              qword ptr [rsp + 64], rax
                                                                                        jmp   n16_match_lit_α
n6_match_arbno_as:
                        mov              eax, dword ptr [rsp + 32]
                        cmp              r14d, eax
                                                                                        je    n16_match_lit_β
                        mov              dword ptr [rsp + 36], r14d
                                                                                        jmp   n7_match_assign_cond_α
n6_match_arbno_af:
                        mov              eax, dword ptr [rsp + 40]
                        test             eax, eax
                                                                                        jnz   .Lx32_2
                        add              rsp, 80
                                                                                        jmp   n16_match_lit_β
.Lx32_2:
                        mov              r14d, dword ptr [rsp + 32]
                        add              rsp, 80
                        mov              rbp, qword ptr [rsp + 104]                     # old_rbp
                                                                                        jmp   n5_match_assign_save_β
#-----------------------------------------------------------------------------------------------------------------------
n7_match_assign_cond_α:
                        mov              eax, dword ptr [rbp + 192]
                        lea              rcx, [rip + .S0]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24
                                                                                        jmp   n8_lit_integer_α
n7_match_assign_cond_β:
                        sub              r12, 24
                                                                                        jmp   n6_match_arbno_β
#-----------------------------------------------------------------------------------------------------------------------
n8_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx35_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n9_match_rpos_α
.Lx35_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n9_match_rpos_α:
                        mov              rax, 0
                        mov              ecx, r15d
                        sub              ecx, eax
                        cmp              r14d, ecx
                                                                                        je    .Lx36_240
                        add              rsp, 16
                                                                                        jmp   n7_match_assign_cond_β
.Lx36_240:
                                                                                        jmp   n10_match_end_α
#-----------------------------------------------------------------------------------------------------------------------
n10_match_end_α:
                        mov              r10, r12
.Lx38_8:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx38_8
                        mov              rsp, qword ptr [r10 + 8]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, r12
                        mov              r10, rsi
.Lx38_5:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx38_5
                        lea              rdi, [r10 + 24]
                        mov              rdx, r13
                        call             rt_dcap_end_ok_open@PLT
.Lx38_1:
                        test             rax, rax
                                                                                        je    .Lx38_2
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx38_3]
                        lea              rdx, [rip + .Lx38_4]
                                                                                        jmp   rax
.Lx38_3:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx38_1
.Lx38_4:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx38_1
.Lx38_2:
                        call             rt_dcap_end_ok_close@PLT
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
.Lx38_10:
                        sub              r12, 24                                        # cas_mark
                        mov              rax, qword ptr [r12 + 0]
                        test             rax, rax
                                                                                        jne   .Lx38_10
                        mov              r13, qword ptr [rsp + 48]                      # outer_Σ
                        mov              r14, qword ptr [rsp + 56]                      # outer_δ
                        mov              r15, qword ptr [rsp + 64]                      # outer_Δ
                        mov              rdi, r13                                       # sig
                        mov              rsi, r15                                       # len
                        call             rt_match_ctx_restore@PLT
                        mov              rbp, qword ptr [rsp + 40]                      # old_rbp
                                                                                        jmp   n11_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n11_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n12_statement_begin_α
#=======================================================================================================================
# MI_YES  MATCHIT = V                                                 :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n12_statement_begin_α:
                                                                                        jmp   n13_var_α
n12_statement_begin_β:
                                                                                        jmp   n21_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n13_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]                    # V
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n14_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n14_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax                    # MATCHIT
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n15_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n15_statement_end_α:
                                                                                        jmp   n21_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n16_match_lit_α:
                        mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d
                                                                                        jg    n6_match_arbno_af
                        movsxd           rcx, r14d
                        movzx            eax, byte ptr [r13+rcx]
                        cmp              eax, 97
                                                                                        jne   n6_match_arbno_af
                        add              r14d, 1
                                                                                        jmp   n6_match_arbno_as
n16_match_lit_β:
                        sub              r14d, 1
                                                                                        jmp   n6_match_arbno_af
#=======================================================================================================================
#         MATCHIT = 'fail'                                            :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n17_statement_begin_α:
                                                                                        jmp   n18_lit_string_α
n17_statement_begin_β:
                                                                                        jmp   n21_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n18_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx51_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n19_assign_α
.Lx51_0:
                        .quad            .Lx51_0_s
.Lx51_0_s:
                        .string          "fail"
#-----------------------------------------------------------------------------------------------------------------------
n19_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax                    # MATCHIT
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n20_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n20_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n21_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n21_save_restore_α:
                        mov              rcx, qword ptr [1879048216]
                        test             rcx, rcx
                                                                                        je    .Lx56_0
                        mov              cl, 0
                        mov              rax, qword ptr [rcx + -64]
                                                                                        jmp   rax
.Lx56_0:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__MATCHIT_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__MATCHIT_β:
                                                                                        jmp   proc_LBL__MATCHIT_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__MATCHIT_γ:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__MATCHIT_ω:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_MATCHIT_α
proc_MATCHIT_α:
proc_MATCHIT_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n57_save_restore_α:
                        mov              rdi, rcx                                       # gw
                        mov              rsi, rdx                                       # ww
                        lea              rdx, [rsp + 0]                                 # rsp
                        mov              rcx, rbp                                       # rbp
                        call             rt_flat_wire_adopt@PLT
                                                                                        jmp   n58_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n58_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx62_0]                 # name
                        call             rt_goto_transfer@PLT
                                                                                        jmp   .Lx62_1
.Lx62_0:
                        .quad            .Lx62_0_s
.Lx62_0_s:
                        .string          "MATCHIT"
.Lx62_1:
                                                                                        jmp   proc_MATCHIT_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_MATCHIT_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_MATCHIT_β:
                                                                                        jmp   proc_MATCHIT_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_MATCHIT_γ:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_MATCHIT_ω:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 8]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
proc_startup:
                        sub              rsp, 8
                        .section         .rodata
.Lstartup_pname0:       .string          "LBL__MATCHIT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname0]
                        lea              rsi, [rip + proc_LBL__MATCHIT_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 416
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname1:       .string          "MATCHIT"
.Lstartup_pp1_0:        .string          "S"
                        .align           8
.Lstartup_pnames1:
                        .quad            .Lstartup_pp1_0
                        .quad            0
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname1]
                        lea              rsi, [rip + .Lstartup_pnames1]
                        mov              edx, 1
                        call             rt_proc_register@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 1
                        call             rt_proc_set_dyn_scope@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        lea              rsi, [rip + proc_MATCHIT_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 1
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 1
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 16
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname1]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        add              rsp, 8
                        ret
                        .section         .rodata
.Lgvan0:                .string          "MATCHIT"
.Lgvan1:                .string          "S"
.Lgvan2:                .string          "V"
                        .align           8
__gva_names:
                        .quad            .Lgvan0
                        .quad            .Lgvan1
                        .quad            .Lgvan2
                        .section         .text
                        .intel_syntax    noprefix
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        call             proc_startup
                        mov              edi, 3
                        call             rt_gva_island@PLT
                        mov              rsi, rax
                        lea              rdi, [rip + __gva_names]
                        mov              edx, 3
                        call             gva_register@PLT
                        mov              r12, qword ptr [0x70000000]
                        xor              esi, esi
                                                                                        jmp   main_α
#-----------------------------------------------------------------------------------------------------------------------
main_α:
main_α_body:
                        push             rbp
                        mov              rbp, rsp
                        sub              rsp, 88
#=======================================================================================================================
#         DEFINE('MATCHIT(S)') :(MATCHIT_END)
#-----------------------------------------------------------------------------------------------------------------------
n63_statement_begin_α:
                                                                                        jmp   n64_func_activate_α
n63_statement_begin_β:
                                                                                        jmp   n66_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n64_func_activate_α:
                        lea              rax, [rip + MATCHIT_act_α]
                        mov              r11, qword ptr [rip + fn_cell$MATCHIT@GOTPCREL]
                        mov              qword ptr [r11 + 0], rax
                                                                                        jmp   n65_statement_end_α
n64_func_activate_β:
                                                                                        jmp   n63_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n65_statement_end_α:
                                                                                        jmp   n66_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n66_statement_begin_α:
                                                                                        jmp   n67_statement_end_α
n66_statement_begin_β:
                                                                                        jmp   n68_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n67_statement_end_α:
                                                                                        jmp   n68_statement_begin_α
#=======================================================================================================================
#         OUTPUT = MATCHIT('aaa')
#-----------------------------------------------------------------------------------------------------------------------
n68_statement_begin_α:
                                                                                        jmp   n69_lit_string_α
n68_statement_begin_β:
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n69_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 3
                        mov              rax, qword ptr [rip + .Lx110_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n70_call_α
.Lx110_0:
                        .quad            .Lx110_0_s
.Lx110_0_s:
                        .string          "aaa"
#-----------------------------------------------------------------------------------------------------------------------
n70_call_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [rsp + 16]                      # lit_string
                        mov              qword ptr [1879052304], rax                    # S
                        mov              rax, qword ptr [rsp + 24]                      # lit_string
                        mov              qword ptr [1879052312], rax                    # S
                        sub              rsp, 32
                        mov              rax, qword ptr [1879052304]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052288]                    # MATCHIT
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 24], rax
                        lea              rcx, [rip + .Lx112_10]
                        lea              rdx, [rip + .Lx112_11]
                        mov              rax, qword ptr [rip + fn_cell$MATCHIT@GOTPCREL]
                                                                                        jmp   rax
.Lx112_10:
                        add              rsp, 32
                                                                                        jmp   .Lx112_2
.Lx112_11:
                        add              rsp, 32
                                                                                        jmp   .Lx112_2
.Lx112_2:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104
                                                                                        jne   .Lx112_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n68_statement_begin_β
.Lx112_240:
                                                                                        jmp   n71_assign_α
n70_call_β:
                                                                                        jmp   n68_statement_begin_β
.Lx112_0:
                        .quad            .Lx112_0_s
.Lx112_0_s:
                        .string          "MATCHIT"
#-----------------------------------------------------------------------------------------------------------------------
n71_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx113_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n72_statement_end_α
.Lx113_0:
                        .quad            .Lx113_0_s
.Lx113_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n72_statement_end_α:
                        add              rsp, 32
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n73_goto_α:
                                                                                        jmp   n74_statement_begin_α
n73_goto_β:
                                                                                        jmp   main_ω
#=======================================================================================================================
# MATCHIT S POS(0) ARBNO('a') . V RPOS(0)                            :S(MI_YES)
#-----------------------------------------------------------------------------------------------------------------------
n74_statement_begin_α:
                                                                                        jmp   n75_var_α
n74_statement_begin_β:
                                                                                        jmp   n91_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n75_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]                    # S
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n76_match_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n76_match_begin_α:
                        mov              rdi, qword ptr [rsp + 0]                       # var
                        mov              rsi, qword ptr [rsp + 8]
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
.Lx121_0:
                        mov              r14d, dword ptr [rsp + 0]
                                                                                        jmp   n77_lit_integer_α
n76_match_begin_β:
                        mov              rsp, qword ptr [r12 + -16]                     # rsp_restore
                        sub              rsp, 32
                        add              dword ptr [rsp + 0], 1                         # start_δ
                        mov              eax, dword ptr [rsp + 0]
                        cmp              eax, r15d
                                                                                        jg    .Lx121_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                                                                                        jne   .Lx121_1
                                                                                        jmp   .Lx121_0
.Lx121_1:
n76_match_begin_af:
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
                                                                                        jmp   n91_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n77_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx122_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n78_match_pos_α
.Lx122_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n78_match_pos_α:
                        mov              rax, 0
                        cmp              r14d, eax
                                                                                        je    .Lx123_240
                        add              rsp, 16
                                                                                        jmp   n76_match_begin_β
.Lx123_240:
                                                                                        jmp   n79_match_assign_save_α
n78_match_pos_β:
                        add              rsp, 16
                                                                                        jmp   n76_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n79_match_assign_save_α:
                        sub              rsp, 16
                        mov              dword ptr [rbp + -80], r14d
                                                                                        jmp   n80_match_arbno_α
n79_match_assign_save_β:
                        add              rsp, 16
                                                                                        jmp   n76_match_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n80_match_arbno_α:
                        sub              rsp, 80
                        mov              eax, 0
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        mov              qword ptr [rsp + 72], rax
                        mov              dword ptr [rsp + 32], r14d
                        mov              dword ptr [rsp + 36], r14d
                        mov              dword ptr [rsp + 40], 1
                        mov              rax, qword ptr [rsp + 120]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [rsp + 128]
                        mov              qword ptr [rsp + 56], rax
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [rsp + 64], rax
                                                                                        jmp   n81_match_assign_cond_α
n80_match_arbno_β:
                        add              rsp, 16
                        mov              r14d, dword ptr [rsp + 36]
                        mov              rax, qword ptr [rsp + 48]
                        mov              rcx, qword ptr [rsp + 56]
                        sub              rsp, 80
                        mov              edx, 0
                        mov              qword ptr [rsp + 0], rdx
                        mov              qword ptr [rsp + 8], rdx
                        mov              qword ptr [rsp + 16], rdx
                        mov              qword ptr [rsp + 24], rdx
                        mov              dword ptr [rsp + 32], r14d
                        mov              dword ptr [rsp + 36], r14d
                        mov              dword ptr [rsp + 40], 0
                        mov              qword ptr [rsp + 48], rax
                        mov              qword ptr [rsp + 56], rcx
                        mov              qword ptr [rsp + 72], rdx
                        mov              rax, qword ptr [rsp + 144]
                        mov              qword ptr [rsp + 64], rax
                                                                                        jmp   n90_match_lit_α
n80_match_arbno_as:
                        mov              eax, dword ptr [rsp + 32]
                        cmp              r14d, eax
                                                                                        je    n90_match_lit_β
                        mov              dword ptr [rsp + 36], r14d
                                                                                        jmp   n81_match_assign_cond_α
n80_match_arbno_af:
                        mov              eax, dword ptr [rsp + 40]
                        test             eax, eax
                                                                                        jnz   .Lx127_2
                        add              rsp, 80
                                                                                        jmp   n90_match_lit_β
.Lx127_2:
                        mov              r14d, dword ptr [rsp + 32]
                        add              rsp, 80
                        mov              rbp, qword ptr [rsp + 104]                     # old_rbp
                                                                                        jmp   n79_match_assign_save_β
#-----------------------------------------------------------------------------------------------------------------------
n81_match_assign_cond_α:
                        mov              eax, dword ptr [rbp + -80]
                        lea              rcx, [rip + .S0]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24
                                                                                        jmp   n82_lit_integer_α
n81_match_assign_cond_β:
                        sub              r12, 24
                                                                                        jmp   n80_match_arbno_β
#-----------------------------------------------------------------------------------------------------------------------
n82_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx130_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n83_match_rpos_α
.Lx130_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n83_match_rpos_α:
                        mov              rax, 0
                        mov              ecx, r15d
                        sub              ecx, eax
                        cmp              r14d, ecx
                                                                                        je    .Lx131_240
                        add              rsp, 16
                                                                                        jmp   n81_match_assign_cond_β
.Lx131_240:
                                                                                        jmp   n84_match_end_α
#-----------------------------------------------------------------------------------------------------------------------
n84_match_end_α:
                        mov              r10, r12
.Lx133_8:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx133_8
                        mov              rsp, qword ptr [r10 + 8]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, r12
                        mov              r10, rsi
.Lx133_5:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx133_5
                        lea              rdi, [r10 + 24]
                        mov              rdx, r13
                        call             rt_dcap_end_ok_open@PLT
.Lx133_1:
                        test             rax, rax
                                                                                        je    .Lx133_2
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx133_3]
                        lea              rdx, [rip + .Lx133_4]
                                                                                        jmp   rax
.Lx133_3:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx133_1
.Lx133_4:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx133_1
.Lx133_2:
                        call             rt_dcap_end_ok_close@PLT
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
.Lx133_10:
                        sub              r12, 24                                        # cas_mark
                        mov              rax, qword ptr [r12 + 0]
                        test             rax, rax
                                                                                        jne   .Lx133_10
                        mov              r13, qword ptr [rsp + 48]                      # outer_Σ
                        mov              r14, qword ptr [rsp + 56]                      # outer_δ
                        mov              r15, qword ptr [rsp + 64]                      # outer_Δ
                        mov              rdi, r13                                       # sig
                        mov              rsi, r15                                       # len
                        call             rt_match_ctx_restore@PLT
                        mov              rbp, qword ptr [rsp + 40]                      # old_rbp
                                                                                        jmp   n85_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n85_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n86_statement_begin_α
#=======================================================================================================================
# MI_YES  MATCHIT = V                                                 :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n86_statement_begin_α:
                                                                                        jmp   n87_var_α
n86_statement_begin_β:
                                                                                        jmp   n95_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n87_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]                    # V
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n88_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n88_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax                    # MATCHIT
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n89_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n89_statement_end_α:
                                                                                        jmp   n95_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n90_match_lit_α:
                        mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d
                                                                                        jg    n80_match_arbno_af
                        movsxd           rcx, r14d
                        movzx            eax, byte ptr [r13+rcx]
                        cmp              eax, 97
                                                                                        jne   n80_match_arbno_af
                        add              r14d, 1
                                                                                        jmp   n80_match_arbno_as
n90_match_lit_β:
                        sub              r14d, 1
                                                                                        jmp   n80_match_arbno_af
#=======================================================================================================================
#         MATCHIT = 'fail'                                            :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n91_statement_begin_α:
                                                                                        jmp   n92_lit_string_α
n91_statement_begin_β:
                                                                                        jmp   n95_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n92_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx146_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n93_assign_α
.Lx146_0:
                        .quad            .Lx146_0_s
.Lx146_0_s:
                        .string          "fail"
#-----------------------------------------------------------------------------------------------------------------------
n93_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax                    # MATCHIT
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n94_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n94_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n95_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n95_save_restore_α:
                        mov              rcx, qword ptr [1879048216]
                        test             rcx, rcx
                                                                                        je    .Lx151_0
                        mov              cl, 0
                        mov              rax, qword ptr [rcx + -64]
                                                                                        jmp   rax
.Lx151_0:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n96_goto_α:
                                                                                        jmp   n86_statement_begin_α
n96_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n97_goto_α:
                                                                                        jmp   n66_statement_begin_α
n97_goto_β:
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
                        .section         .data
                        .align           8
fn_cell$MATCHIT:
                        .quad            rt_ab_undef_fn_stub
                        .section         .text
                        .intel_syntax    noprefix
MATCHIT_act_α:
                        push             rbp
                        mov              rbp, rsp
                        sub              rsp, 136
                        mov              qword ptr [rbp + -16], rcx
                        mov              qword ptr [rbp + -24], rdx
                        lea              rax, [rbp + 8]
                        mov              qword ptr [rbp + -32], rax
                        mov              rax, qword ptr [1879048216]
                        mov              qword ptr [rbp + -40], rax
                        mov              qword ptr [1879048216], rbp
                        lea              rax, [rip + .Lx154_1]
                        mov              qword ptr [rbp + -64], rax
                        mov              rdi, rbp
                        call             rt_ab_enter_env@PLT
                        mov              rax, qword ptr [1879052288]                    # MATCHIT
                        mov              qword ptr [rbp + -112], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rbp + -104], rax
                        mov              rax, qword ptr [1879052304]                    # S
                        mov              qword ptr [rbp + -128], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rbp + -120], rax
                        xor              eax, eax                                       # MATCHIT
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rax
                        xor              eax, eax                                       # S
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rax
                        movabs           rax, 140069861334412
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx154_2
                        mov              rdi, qword ptr [rip + .Lx154_0]
                        call             mon_emit_call_bin@PLT
.Lx154_2:
                                                                                        jmp   proc_MATCHIT_α
                                                                                        jmp   MATCHIT_act_γ
.Lx154_1:
                        movzx            r9, cl
                        cmp              r9d, 2
                                                                                        je    .Lx154_3
                        mov              qword ptr [rbp + -88], rax
                        mov              qword ptr [rbp + -96], rdx
                        mov              rdi, rbp
                        mov              rsi, qword ptr [rbp + -88]
                        mov              rdx, qword ptr [rbp + -96]
                        xor              ecx, ecx
                        call             rt_ab_leave_env@PLT
                        mov              qword ptr [rbp + -88], rax
                        mov              qword ptr [rbp + -96], rdx
                                                                                        jmp   .Lx154_4
.Lx154_3:
                        mov              rdi, rbp
                        xor              esi, esi
                        xor              edx, edx
                        mov              ecx, 1
                        call             rt_ab_leave_env@PLT
                        mov              qword ptr [rbp + -88], rax
                        mov              qword ptr [rbp + -96], rdx
.Lx154_4:
                        mov              rcx, qword ptr [rbp + -112]                    # MATCHIT
                        mov              qword ptr [1879052288], rcx
                        mov              rcx, qword ptr [rbp + -104]
                        mov              qword ptr [1879052296], rcx
                        mov              rcx, qword ptr [rbp + -128]                    # S
                        mov              qword ptr [1879052304], rcx
                        mov              rcx, qword ptr [rbp + -120]
                        mov              qword ptr [1879052312], rcx
                        mov              r10, qword ptr [rbp + -16]
                        mov              r11, qword ptr [rbp + -24]
                        mov              rcx, qword ptr [rbp + -40]
                        movabs           rax, 140069861334412
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx154_5
                        mov              rdi, qword ptr [rip + .Lx154_0]
                        mov              rsi, qword ptr [rbp + -88]
                        mov              rdx, qword ptr [rbp + -96]
                        call             mon_emit_return_bin@PLT
.Lx154_5:
                        mov              rax, qword ptr [rbp + -88]
                        mov              rdx, qword ptr [rbp + -96]
                        mov              qword ptr [1879048216], rcx
                        cmp              r9d, 2
                                                                                        je    .Lx154_6
                                                                                        jmp   r10
.Lx154_6:
                                                                                        jmp   r11
MATCHIT_act_γ:
MATCHIT_act_ω:
.Lx154_0:
                        .quad            .Lx154_0_s
.Lx154_0_s:
                        .string          "MATCHIT"
                        .section         .rodata
.S0:                    .string          "V"
                        .text
                        .section         .note.GNU-stack,"",@progbits
