                        .intel_syntax    noprefix
                        .text
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_LBL__MATCHIT_α
proc_LBL__MATCHIT_α:
                        .global          proc_LBL__MATCHIT_α
                        .global          proc_LBL__MATCHIT_β
                        .global          proc_LBL__MATCHIT_γ
                        .global          proc_LBL__MATCHIT_ω
                        sub              rsp, 432
                        mov              [rsp + 408], rcx
                        mov              [rsp + 416], rdx
                        mov              [rsp + 424], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp], 0
                        mov              qword ptr [rsp + 8], 0
                        mov              qword ptr [rsp + 192], 0
                        mov              qword ptr [rsp + 200], 0
proc_LBL__MATCHIT_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n0_goto_α:
                                                                                        jmp   n1_var_α
n0_goto_β:
                                                                                        jmp   proc_LBL__MATCHIT_ω
#=======================================================================================================================
# MATCHIT S POS(0) ARBNO('a') . V RPOS(0)                            :S(MI_YES)
#-----------------------------------------------------------------------------------------------------------------------
n1_var_α:
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rbp + 288], rax
                        mov              qword ptr [rbp + 296], rdx
                                                                                        jmp   n2_match_head_α
n1_var_β:
                                                                                        jmp   n3_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n2_match_head_α:
                        mov              qword ptr [rbp + 80], r13
                        mov              qword ptr [rbp + 88], r14
                        mov              qword ptr [rbp + 96], r15
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rbp + 104], rax
                        mov              qword ptr [rbp + 72], rbp
                        mov              rdi, qword ptr [rbp + 288]
                        mov              rsi, qword ptr [rbp + 296]
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
.Lx21_0:
                        mov              r14d, dword ptr [rsp + 0]
                                                                                        jmp   n4_match_sequence_α
n2_match_head_β:
                        add              dword ptr [rsp + 0], 1
                        mov              eax, dword ptr [rsp + 0]
                        cmp              eax, r15d
                                                                                        jg    .Lx21_1
                        lea              rcx, [rip + g_anchor]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                                                                                        jne   .Lx21_1
                                                                                        jmp   .Lx21_0
.Lx21_1:
                        mov              rax, qword ptr [rsp + 8]
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              rsp, qword ptr [rsp + 16]
                        mov              r10, qword ptr [1879048192]
.Lx21_2:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx21_2
                        mov              qword ptr [1879048192], r10
                        mov              r13, qword ptr [rbp + 80]
                        mov              r14, qword ptr [rbp + 88]
                        mov              r15, qword ptr [rbp + 96]
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              rdx, qword ptr [rbp + 104]
                        call             rt_match_ctx_restore@PLT
                        mov              rbp, qword ptr [rbp + 72]
                                                                                        jmp   n3_lit_string_α
#=======================================================================================================================
#         MATCHIT = 'fail'                                            :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n3_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx22_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n5_assign_α
n3_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n13_op14_α
.Lx22_0:
                        .quad            .Lx22_0_s
.Lx22_0_s:
                        .string          "fail"
#-----------------------------------------------------------------------------------------------------------------------
n4_match_sequence_α:
                                                                                        jmp   n7_lit_integer_α
n4_match_sequence_as:
                                                                                        jmp   n6_match_release_α
n4_match_sequence_β:
                                                                                        jmp   n12_match_rpos_β
n4_match_sequence_af:
                                                                                        jmp   n2_match_head_β
#-----------------------------------------------------------------------------------------------------------------------
n5_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n13_op14_α
n5_assign_β:
                                                                                        jmp   n13_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n6_match_release_α:
                        mov              rax, qword ptr [rsp + 192]
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              rsp, qword ptr [rsp + 200]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, qword ptr [1879048192]
                        mov              r10, rsi
.Lx27_5:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx27_5
                        lea              rdi, [r10 + 24]
                        mov              rdx, r13
                        call             rt_dcap_end_ok_open@PLT
.Lx27_1:
                        test             rax, rax
                                                                                        je    .Lx27_2
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx27_3]
                        lea              rdx, [rip + .Lx27_4]
                                                                                        jmp   rax
.Lx27_3:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx27_1
.Lx27_4:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx27_1
.Lx27_2:
                        call             rt_dcap_end_ok_close@PLT
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              r10, qword ptr [1879048192]
.Lx27_6:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx27_6
                        mov              qword ptr [1879048192], r10
                        mov              r13, qword ptr [rbp + 80]
                        mov              r14, qword ptr [rbp + 88]
                        mov              r15, qword ptr [rbp + 96]
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              rdx, qword ptr [rbp + 104]
                        call             rt_match_ctx_restore@PLT
                        mov              rbp, qword ptr [rbp + 72]
                                                                                        jmp   n14_var_α
#-----------------------------------------------------------------------------------------------------------------------
n7_lit_integer_α:
                        mov              qword ptr [rbp + 144], 6
                        mov              rax, qword ptr [rip + .Lx28_0]
                        mov              qword ptr [rbp + 152], rax
                                                                                        jmp   n8_match_pos_α
n7_lit_integer_β:
                                                                                        jmp   n2_match_head_β
.Lx28_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n8_match_pos_α:
                        mov              rax, 0
                        cmp              r14d, eax
                                                                                        jne   n2_match_head_β
                                                                                        jmp   n9_match_assign_save_α
n8_match_pos_β:
                                                                                        jmp   n2_match_head_β
#-----------------------------------------------------------------------------------------------------------------------
n9_match_assign_save_α:
                        sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d
                                                                                        jmp   n15_match_arbno_α
n9_match_assign_save_β:
                        add              rsp, 16
                                                                                        jmp   n2_match_head_β
#-----------------------------------------------------------------------------------------------------------------------
n10_match_assign_cond_α:
                        mov              eax, dword ptr [rsp + 208]
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
                                                                                        jmp   n11_lit_integer_α
n10_match_assign_cond_β:
                        mov              rax, qword ptr [1879048192]
                        sub              rax, 24
                        mov              qword ptr [1879048192], rax
                                                                                        jmp   n15_match_arbno_β
#-----------------------------------------------------------------------------------------------------------------------
n11_lit_integer_α:
                        mov              qword ptr [rbp + 272], 6
                        mov              rax, qword ptr [rip + .Lx34_0]
                        mov              qword ptr [rbp + 280], rax
                                                                                        jmp   n12_match_rpos_α
n11_lit_integer_β:
                                                                                        jmp   n10_match_assign_cond_β
.Lx34_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n12_match_rpos_α:
                        mov              rax, 0
                        mov              ecx, r15d
                        sub              ecx, eax
                        cmp              r14d, ecx
                                                                                        jne   n10_match_assign_cond_β
                                                                                        jmp   n6_match_release_α
n12_match_rpos_β:
                                                                                        jmp   n10_match_assign_cond_β
#-----------------------------------------------------------------------------------------------------------------------
n13_op14_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#=======================================================================================================================
# MI_YES  MATCHIT = V                                                 :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n14_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n16_assign_α
n14_var_β:
                        add              rsp, 16
                                                                                        jmp   n13_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n15_match_arbno_α:
                        sub              rsp, 224
                        mov              eax, 0
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        mov              qword ptr [rsp + 32], rax
                        mov              qword ptr [rsp + 40], rax
                        mov              qword ptr [rsp + 48], rax
                        mov              qword ptr [rsp + 56], rax
                        mov              qword ptr [rsp + 64], rax
                        mov              qword ptr [rsp + 72], rax
                        mov              qword ptr [rsp + 80], rax
                        mov              qword ptr [rsp + 88], rax
                        mov              qword ptr [rsp + 96], rax
                        mov              qword ptr [rsp + 104], rax
                        mov              qword ptr [rsp + 112], rax
                        mov              qword ptr [rsp + 120], rax
                        mov              qword ptr [rsp + 128], rax
                        mov              qword ptr [rsp + 136], rax
                        mov              qword ptr [rsp + 144], rax
                        mov              qword ptr [rsp + 152], rax
                        mov              qword ptr [rsp + 160], rax
                        mov              qword ptr [rsp + 168], rax
                        mov              qword ptr [rsp + 216], rax
                        mov              dword ptr [rsp + 176], r14d
                        mov              dword ptr [rsp + 180], r14d
                        mov              dword ptr [rsp + 184], 1
                        mov              rax, qword ptr [rsp + 248]
                        mov              qword ptr [rsp + 192], rax
                        mov              rax, qword ptr [rsp + 256]
                        mov              qword ptr [rsp + 200], rax
                        mov              rax, qword ptr [rsp + 224]
                        mov              qword ptr [rsp + 208], rax
                                                                                        jmp   n10_match_assign_cond_α
n15_match_arbno_β:
                        mov              r14d, dword ptr [rsp + 180]
                        mov              rax, qword ptr [rsp + 192]
                        mov              rcx, qword ptr [rsp + 200]
                        sub              rsp, 224
                        mov              edx, 0
                        mov              qword ptr [rsp + 0], rdx
                        mov              qword ptr [rsp + 8], rdx
                        mov              qword ptr [rsp + 16], rdx
                        mov              qword ptr [rsp + 24], rdx
                        mov              qword ptr [rsp + 32], rdx
                        mov              qword ptr [rsp + 40], rdx
                        mov              qword ptr [rsp + 48], rdx
                        mov              qword ptr [rsp + 56], rdx
                        mov              qword ptr [rsp + 64], rdx
                        mov              qword ptr [rsp + 72], rdx
                        mov              qword ptr [rsp + 80], rdx
                        mov              qword ptr [rsp + 88], rdx
                        mov              qword ptr [rsp + 96], rdx
                        mov              qword ptr [rsp + 104], rdx
                        mov              qword ptr [rsp + 112], rdx
                        mov              qword ptr [rsp + 120], rdx
                        mov              qword ptr [rsp + 128], rdx
                        mov              qword ptr [rsp + 136], rdx
                        mov              qword ptr [rsp + 144], rdx
                        mov              qword ptr [rsp + 152], rdx
                        mov              qword ptr [rsp + 160], rdx
                        mov              qword ptr [rsp + 168], rdx
                        mov              dword ptr [rsp + 176], r14d
                        mov              dword ptr [rsp + 180], r14d
                        mov              dword ptr [rsp + 184], 0
                        mov              qword ptr [rsp + 192], rax
                        mov              qword ptr [rsp + 200], rcx
                        mov              qword ptr [rsp + 216], rdx
                        mov              rax, qword ptr [rsp + 432]
                        mov              qword ptr [rsp + 208], rax
                                                                                        jmp   n17_match_lit_α
n15_match_arbno_as:
                        mov              eax, dword ptr [rsp + 176]
                        cmp              r14d, eax
                                                                                        je    n17_match_lit_β
                        mov              dword ptr [rsp + 180], r14d
                                                                                        jmp   n10_match_assign_cond_α
n15_match_arbno_af:
                        mov              eax, dword ptr [rsp + 184]
                        test             eax, eax
                                                                                        jnz   .Lx40_2
                        add              rsp, 224
                                                                                        jmp   n17_match_lit_β
.Lx40_2:
                        mov              r14d, dword ptr [rsp + 176]
                        add              rsp, 224
                                                                                        jmp   n9_match_assign_save_β
#-----------------------------------------------------------------------------------------------------------------------
n16_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n13_op14_α
n16_assign_β:
                                                                                        jmp   n13_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n17_match_lit_α:
                        mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d
                                                                                        jg    n15_match_arbno_af
                        movsxd           rcx, r14d
                        movzx            eax, byte ptr [r13+rcx]
                        cmp              eax, 97
                                                                                        jne   n15_match_arbno_af
                        add              r14d, 1
                                                                                        jmp   n15_match_arbno_as
n17_match_lit_β:
                        sub              r14d, 1
                                                                                        jmp   n15_match_arbno_af
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__MATCHIT_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__MATCHIT_β:
                                                                                        jmp   proc_LBL__MATCHIT_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__MATCHIT_γ:
                        mov              rdi, [rbp]
                        mov              rsi, [rbp + 8]
                        mov              rax, [rbp + 408]
                        lea              rsp, [rbp + 432]
                        mov              rbp, [rbp + 424]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__MATCHIT_ω:
                        mov              rax, [rbp + 416]
                        lea              rsp, [rbp + 432]
                        mov              rbp, [rbp + 424]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_MATCHIT_α
proc_MATCHIT_α:
                        .global          proc_MATCHIT_α
                        .global          proc_MATCHIT_β
                        .global          proc_MATCHIT_γ
                        .global          proc_MATCHIT_ω
                        sub              rsp, 432
                        mov              [rsp + 408], rcx
                        mov              [rsp + 416], rdx
                        mov              rdi, rsp
                        mov              ecx, 400
                        xor              eax, eax
                        rep stosb
proc_MATCHIT_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n44_op14_α:
                        mov              rdi, qword ptr [rsp + 408]
                        mov              rsi, qword ptr [rsp + 416]
                        lea              rdx, [rsp + 432]
                        mov              rcx, rbp
                        call             rt_flat_wire_adopt@PLT
                                                                                        jmp   n45_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n45_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx49_0]
                        call             rt_goto_transfer@PLT
                                                                                        jmp   .Lx49_1
.Lx49_0:
                        .quad            .Lx49_0_s
.Lx49_0_s:
                        .string          "MATCHIT"
.Lx49_1:
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
                        mov              rdi, [rsp]
                        mov              rsi, [rsp + 8]
                        mov              rax, [rsp + 408]
                        add              rsp, 432
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_MATCHIT_ω:
                        mov              rax, [rsp + 416]
                        add              rsp, 432
                                                                                        jmp   rax
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
                        mov              esi, 400
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
                        sub              rsp, 408
                        mov              rdi, rsp
                        mov              ecx, 408
                        xor              eax, eax
                        rep stosb
                        mov              [rsp + 400], rbp
                        mov              rbp, rsp
main_α_body:
#=======================================================================================================================
#         DEFINE('MATCHIT(S)') :(MATCHIT_END)
#         OUTPUT = MATCHIT('aaa')
#-----------------------------------------------------------------------------------------------------------------------
n50_lit_string_α:
                        mov              qword ptr [rbp + 384], 1
                        mov              dword ptr [rbp + 388], 3
                        mov              rax, qword ptr [rip + .Lx73_0]
                        mov              qword ptr [rbp + 392], rax
                                                                                        jmp   n54_call_α
n50_lit_string_β:
                                                                                        jmp   main_γ
.Lx73_0:
                        .quad            .Lx73_0_s
.Lx73_0_s:
                        .string          "aaa"
#-----------------------------------------------------------------------------------------------------------------------
n51_goto_α:
                                                                                        jmp   n55_var_α
n51_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n52_goto_α:
                                                                                        jmp   n56_var_α
n52_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n53_goto_α:
                                                                                        jmp   n50_lit_string_α
n53_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n54_call_α:
                        sub              rsp, 32
                        mov              rax, qword ptr [1879052304]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052288]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx78_0]
                        mov              esi, 1
                        mov              edx, 1
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx78_5
                        mov              rax, qword ptr [rbp + 384]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rbp + 392]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx78_6]
                        lea              rdx, [rip + .Lx78_7]
                                                                                        jmp   rax
.Lx78_6:
                        mov              rdi, qword ptr [1879052288]
                        mov              rsi, qword ptr [1879052296]
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_γ@PLT
                                                                                        jmp   .Lx78_2
.Lx78_7:
                        mov              rax, qword ptr [rsp + 16]
                        mov              qword ptr [1879052288], rax
                        mov              rax, qword ptr [rsp + 24]
                        mov              qword ptr [1879052296], rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [1879052312], rax
                        add              rsp, 32
                        call             rt_proc_call_epilogue_slim_ω@PLT
                                                                                        jmp   .Lx78_2
.Lx78_5:
                        add              rsp, 32
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx78_20
                        mov              rax, qword ptr [rbp + 384]
                        mov              rdx, qword ptr [rbp + 392]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx78_21
.Lx78_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rbp + 384]
                        mov              rdx, qword ptr [rbp + 392]
                        call             rt_arg_stage@PLT
.Lx78_21:
                        mov              rdi, qword ptr [rip + .Lx78_0]
                        mov              esi, 1
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx78_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx78_3]
                        lea              rdx, [rip + .Lx78_4]
                                                                                        jmp   rax
.Lx78_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx78_2
.Lx78_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx78_2
.Lx78_1:
                        call             rt_faildescr@PLT
.Lx78_2:
                        mov              qword ptr [rbp + 336], rax
                        mov              qword ptr [rbp + 344], rdx
                        cmp              eax, 99
                                                                                        je    main_γ
                                                                                        jmp   n57_assign_α
n54_call_β:
                                                                                        jmp   main_γ
.Lx78_0:
                        .quad            .Lx78_0_s
.Lx78_0_s:
                        .string          "MATCHIT"
#=======================================================================================================================
# MATCHIT S POS(0) ARBNO('a') . V RPOS(0)                            :S(MI_YES)
#-----------------------------------------------------------------------------------------------------------------------
n55_var_α:
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rbp + 288], rax
                        mov              qword ptr [rbp + 296], rdx
                                                                                        jmp   n58_match_head_α
n55_var_β:
                                                                                        jmp   n59_lit_string_α
#=======================================================================================================================
# MI_YES  MATCHIT = V                                                 :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n56_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n60_assign_α
n56_var_β:
                        add              rsp, 16
                                                                                        jmp   n61_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n57_assign_α:
                        mov              rsi, qword ptr [rbp + 336]
                        mov              rdx, qword ptr [rbp + 344]
                        mov              rdi, qword ptr [rip + .Lx81_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   main_γ
n57_assign_β:
                                                                                        jmp   main_γ
.Lx81_0:
                        .quad            .Lx81_0_s
.Lx81_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n58_match_head_α:
                        mov              qword ptr [rbp + 80], r13
                        mov              qword ptr [rbp + 88], r14
                        mov              qword ptr [rbp + 96], r15
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rbp + 104], rax
                        mov              qword ptr [rbp + 72], rbp
                        mov              rdi, qword ptr [rbp + 288]
                        mov              rsi, qword ptr [rbp + 296]
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
.Lx83_0:
                        mov              r14d, dword ptr [rsp + 0]
                                                                                        jmp   n62_match_sequence_α
n58_match_head_β:
                        add              dword ptr [rsp + 0], 1
                        mov              eax, dword ptr [rsp + 0]
                        cmp              eax, r15d
                                                                                        jg    .Lx83_1
                        lea              rcx, [rip + g_anchor]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                                                                                        jne   .Lx83_1
                                                                                        jmp   .Lx83_0
.Lx83_1:
                        mov              rax, qword ptr [rsp + 8]
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              rsp, qword ptr [rsp + 16]
                        mov              r10, qword ptr [1879048192]
.Lx83_2:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx83_2
                        mov              qword ptr [1879048192], r10
                        mov              r13, qword ptr [rbp + 80]
                        mov              r14, qword ptr [rbp + 88]
                        mov              r15, qword ptr [rbp + 96]
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              rdx, qword ptr [rbp + 104]
                        call             rt_match_ctx_restore@PLT
                        mov              rbp, qword ptr [rbp + 72]
                                                                                        jmp   n59_lit_string_α
#=======================================================================================================================
#         MATCHIT = 'fail'                                            :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n59_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx84_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n63_assign_α
n59_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n61_op14_α
.Lx84_0:
                        .quad            .Lx84_0_s
.Lx84_0_s:
                        .string          "fail"
#-----------------------------------------------------------------------------------------------------------------------
n60_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n61_op14_α
n60_assign_β:
                                                                                        jmp   n61_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n61_op14_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n62_match_sequence_α:
                                                                                        jmp   n65_lit_integer_α
n62_match_sequence_as:
                                                                                        jmp   n64_match_release_α
n62_match_sequence_β:
                                                                                        jmp   n70_match_rpos_β
n62_match_sequence_af:
                                                                                        jmp   n58_match_head_β
#-----------------------------------------------------------------------------------------------------------------------
n63_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n61_op14_α
n63_assign_β:
                                                                                        jmp   n61_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n64_match_release_α:
                        mov              rax, qword ptr [rsp + 192]
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              rsp, qword ptr [rsp + 200]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, qword ptr [1879048192]
                        mov              r10, rsi
.Lx92_5:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx92_5
                        lea              rdi, [r10 + 24]
                        mov              rdx, r13
                        call             rt_dcap_end_ok_open@PLT
.Lx92_1:
                        test             rax, rax
                                                                                        je    .Lx92_2
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx92_3]
                        lea              rdx, [rip + .Lx92_4]
                                                                                        jmp   rax
.Lx92_3:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx92_1
.Lx92_4:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx92_1
.Lx92_2:
                        call             rt_dcap_end_ok_close@PLT
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              r10, qword ptr [1879048192]
.Lx92_6:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx92_6
                        mov              qword ptr [1879048192], r10
                        mov              r13, qword ptr [rbp + 80]
                        mov              r14, qword ptr [rbp + 88]
                        mov              r15, qword ptr [rbp + 96]
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              rdx, qword ptr [rbp + 104]
                        call             rt_match_ctx_restore@PLT
                        mov              rbp, qword ptr [rbp + 72]
                                                                                        jmp   n56_var_α
#-----------------------------------------------------------------------------------------------------------------------
n65_lit_integer_α:
                        mov              qword ptr [rbp + 144], 6
                        mov              rax, qword ptr [rip + .Lx93_0]
                        mov              qword ptr [rbp + 152], rax
                                                                                        jmp   n66_match_pos_α
n65_lit_integer_β:
                                                                                        jmp   n58_match_head_β
.Lx93_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n66_match_pos_α:
                        mov              rax, 0
                        cmp              r14d, eax
                                                                                        jne   n58_match_head_β
                                                                                        jmp   n67_match_assign_save_α
n66_match_pos_β:
                                                                                        jmp   n58_match_head_β
#-----------------------------------------------------------------------------------------------------------------------
n67_match_assign_save_α:
                        sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d
                                                                                        jmp   n71_match_arbno_α
n67_match_assign_save_β:
                        add              rsp, 16
                                                                                        jmp   n58_match_head_β
#-----------------------------------------------------------------------------------------------------------------------
n68_match_assign_cond_α:
                        mov              eax, dword ptr [rsp + 208]
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
                                                                                        jmp   n69_lit_integer_α
n68_match_assign_cond_β:
                        mov              rax, qword ptr [1879048192]
                        sub              rax, 24
                        mov              qword ptr [1879048192], rax
                                                                                        jmp   n71_match_arbno_β
#-----------------------------------------------------------------------------------------------------------------------
n69_lit_integer_α:
                        mov              qword ptr [rbp + 272], 6
                        mov              rax, qword ptr [rip + .Lx99_0]
                        mov              qword ptr [rbp + 280], rax
                                                                                        jmp   n70_match_rpos_α
n69_lit_integer_β:
                                                                                        jmp   n68_match_assign_cond_β
.Lx99_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n70_match_rpos_α:
                        mov              rax, 0
                        mov              ecx, r15d
                        sub              ecx, eax
                        cmp              r14d, ecx
                                                                                        jne   n68_match_assign_cond_β
                                                                                        jmp   n64_match_release_α
n70_match_rpos_β:
                                                                                        jmp   n68_match_assign_cond_β
#-----------------------------------------------------------------------------------------------------------------------
n71_match_arbno_α:
                        sub              rsp, 224
                        mov              eax, 0
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        mov              qword ptr [rsp + 32], rax
                        mov              qword ptr [rsp + 40], rax
                        mov              qword ptr [rsp + 48], rax
                        mov              qword ptr [rsp + 56], rax
                        mov              qword ptr [rsp + 64], rax
                        mov              qword ptr [rsp + 72], rax
                        mov              qword ptr [rsp + 80], rax
                        mov              qword ptr [rsp + 88], rax
                        mov              qword ptr [rsp + 96], rax
                        mov              qword ptr [rsp + 104], rax
                        mov              qword ptr [rsp + 112], rax
                        mov              qword ptr [rsp + 120], rax
                        mov              qword ptr [rsp + 128], rax
                        mov              qword ptr [rsp + 136], rax
                        mov              qword ptr [rsp + 144], rax
                        mov              qword ptr [rsp + 152], rax
                        mov              qword ptr [rsp + 160], rax
                        mov              qword ptr [rsp + 168], rax
                        mov              qword ptr [rsp + 216], rax
                        mov              dword ptr [rsp + 176], r14d
                        mov              dword ptr [rsp + 180], r14d
                        mov              dword ptr [rsp + 184], 1
                        mov              rax, qword ptr [rsp + 248]
                        mov              qword ptr [rsp + 192], rax
                        mov              rax, qword ptr [rsp + 256]
                        mov              qword ptr [rsp + 200], rax
                        mov              rax, qword ptr [rsp + 224]
                        mov              qword ptr [rsp + 208], rax
                                                                                        jmp   n68_match_assign_cond_α
n71_match_arbno_β:
                        mov              r14d, dword ptr [rsp + 180]
                        mov              rax, qword ptr [rsp + 192]
                        mov              rcx, qword ptr [rsp + 200]
                        sub              rsp, 224
                        mov              edx, 0
                        mov              qword ptr [rsp + 0], rdx
                        mov              qword ptr [rsp + 8], rdx
                        mov              qword ptr [rsp + 16], rdx
                        mov              qword ptr [rsp + 24], rdx
                        mov              qword ptr [rsp + 32], rdx
                        mov              qword ptr [rsp + 40], rdx
                        mov              qword ptr [rsp + 48], rdx
                        mov              qword ptr [rsp + 56], rdx
                        mov              qword ptr [rsp + 64], rdx
                        mov              qword ptr [rsp + 72], rdx
                        mov              qword ptr [rsp + 80], rdx
                        mov              qword ptr [rsp + 88], rdx
                        mov              qword ptr [rsp + 96], rdx
                        mov              qword ptr [rsp + 104], rdx
                        mov              qword ptr [rsp + 112], rdx
                        mov              qword ptr [rsp + 120], rdx
                        mov              qword ptr [rsp + 128], rdx
                        mov              qword ptr [rsp + 136], rdx
                        mov              qword ptr [rsp + 144], rdx
                        mov              qword ptr [rsp + 152], rdx
                        mov              qword ptr [rsp + 160], rdx
                        mov              qword ptr [rsp + 168], rdx
                        mov              dword ptr [rsp + 176], r14d
                        mov              dword ptr [rsp + 180], r14d
                        mov              dword ptr [rsp + 184], 0
                        mov              qword ptr [rsp + 192], rax
                        mov              qword ptr [rsp + 200], rcx
                        mov              qword ptr [rsp + 216], rdx
                        mov              rax, qword ptr [rsp + 432]
                        mov              qword ptr [rsp + 208], rax
                                                                                        jmp   n72_match_lit_α
n71_match_arbno_as:
                        mov              eax, dword ptr [rsp + 176]
                        cmp              r14d, eax
                                                                                        je    n72_match_lit_β
                        mov              dword ptr [rsp + 180], r14d
                                                                                        jmp   n68_match_assign_cond_α
n71_match_arbno_af:
                        mov              eax, dword ptr [rsp + 184]
                        test             eax, eax
                                                                                        jnz   .Lx102_2
                        add              rsp, 224
                                                                                        jmp   n72_match_lit_β
.Lx102_2:
                        mov              r14d, dword ptr [rsp + 176]
                        add              rsp, 224
                                                                                        jmp   n67_match_assign_save_β
#-----------------------------------------------------------------------------------------------------------------------
n72_match_lit_α:
                        mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d
                                                                                        jg    n71_match_arbno_af
                        movsxd           rcx, r14d
                        movzx            eax, byte ptr [r13+rcx]
                        cmp              eax, 97
                                                                                        jne   n71_match_arbno_af
                        add              r14d, 1
                                                                                        jmp   n71_match_arbno_as
n72_match_lit_β:
                        sub              r14d, 1
                                                                                        jmp   n71_match_arbno_af
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
                        mov              eax, 1
                        xor              edx, edx
                        mov              rsp, rbp
                        mov              rbp, [rsp + 400]
                        add              rsp, 408
                        ret
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              rsp, rbp
                        mov              eax, 99
                        xor              edx, edx
                        mov              rbp, [rsp + 400]
                        add              rsp, 408
                        ret
                        .section         .rodata
.S0:                    .string          "V"
                        .text
                        .section         .note.GNU-stack,"",@progbits
