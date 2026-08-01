                        .intel_syntax    noprefix
                        .text
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_LBL__MATCHIT_α
proc_LBL__MATCHIT_α:
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
                        mov              qword ptr [rsp + 288], rax
                        mov              qword ptr [rsp + 296], rdx
                                                                                        jmp   n2_match_head_α
n1_var_β:
                                                                                        jmp   n15_lit_string_α
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
                                                                                        jmp   n3_match_sequence_α
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
                                                                                        jmp   n15_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n3_match_sequence_α:
                                                                                        jmp   n13_lit_integer_α
n3_match_sequence_as:
                                                                                        jmp   n4_match_release_α
n3_match_sequence_β:
                                                                                        jmp   n8_match_rpos_β
n3_match_sequence_af:
                                                                                        jmp   n2_match_head_β
#-----------------------------------------------------------------------------------------------------------------------
n4_match_release_α:
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
.Lx25_5:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx25_5
                        lea              rdi, [r10 + 24]
                        mov              rdx, r13
                        call             rt_dcap_end_ok_open@PLT
.Lx25_1:
                        test             rax, rax
                                                                                        je    .Lx25_2
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx25_3]
                        lea              rdx, [rip + .Lx25_4]
                                                                                        jmp   rax
.Lx25_3:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx25_1
.Lx25_4:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx25_1
.Lx25_2:
                        call             rt_dcap_end_ok_close@PLT
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              r10, qword ptr [1879048192]
.Lx25_6:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx25_6
                        mov              qword ptr [1879048192], r10
                        mov              r13, qword ptr [rbp + 80]
                        mov              r14, qword ptr [rbp + 88]
                        mov              r15, qword ptr [rbp + 96]
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              rdx, qword ptr [rbp + 104]
                        call             rt_match_ctx_restore@PLT
                        mov              rbp, qword ptr [rbp + 72]
                                                                                        jmp   n5_var_α
#=======================================================================================================================
# MI_YES  MATCHIT = V                                                 :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n5_var_α:
                        mov              rax, qword ptr [1879052320]
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rsp + 320], rax
                        mov              qword ptr [rsp + 328], rdx
                                                                                        jmp   n6_assign_α
n5_var_β:
                                                                                        jmp   n17_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n6_assign_α:
                        mov              rax, qword ptr [rsp + 320]
                        mov              rdx, qword ptr [rsp + 328]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n17_save_restore_α
n6_assign_β:
                                                                                        jmp   n17_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n7_lit_integer_α:
                        mov              qword ptr [rsp + 160], 6
                        mov              rax, qword ptr [rip + .Lx28_0]
                        mov              qword ptr [rsp + 168], rax
                                                                                        jmp   n8_match_rpos_α
n7_lit_integer_β:
                                                                                        jmp   n11_match_assign_cond_β
.Lx28_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n8_match_rpos_α:
                        mov              rax, 0
                        mov              ecx, r15d
                        sub              ecx, eax
                        cmp              r14d, ecx
                                                                                        jne   n11_match_assign_cond_β
                                                                                        jmp   n4_match_release_α
n8_match_rpos_β:
                                                                                        jmp   n11_match_assign_cond_β
#-----------------------------------------------------------------------------------------------------------------------
n9_match_assign_save_α:
                        sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d
                                                                                        jmp   n10_match_arbno_α
n9_match_assign_save_β:
                        add              rsp, 16
                                                                                        jmp   n2_match_head_β
#-----------------------------------------------------------------------------------------------------------------------
n10_match_arbno_α:
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
                                                                                        jmp   n11_match_assign_cond_α
n10_match_arbno_β:
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
                                                                                        jmp   n12_match_lit_α
n10_match_arbno_as:
                        mov              eax, dword ptr [rsp + 176]
                        cmp              r14d, eax
                                                                                        je    n12_match_lit_β
                        mov              dword ptr [rsp + 180], r14d
                                                                                        jmp   n11_match_assign_cond_α
n10_match_arbno_af:
                        mov              eax, dword ptr [rsp + 184]
                        test             eax, eax
                                                                                        jnz   .Lx33_2
                        add              rsp, 224
                                                                                        jmp   n12_match_lit_β
.Lx33_2:
                        mov              r14d, dword ptr [rsp + 176]
                        add              rsp, 224
                                                                                        jmp   n9_match_assign_save_β
#-----------------------------------------------------------------------------------------------------------------------
n11_match_assign_cond_α:
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
                                                                                        jmp   n7_lit_integer_α
n11_match_assign_cond_β:
                        mov              rax, qword ptr [1879048192]
                        sub              rax, 24
                        mov              qword ptr [1879048192], rax
                                                                                        jmp   n10_match_arbno_β
#-----------------------------------------------------------------------------------------------------------------------
n12_match_lit_α:
                        mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d
                                                                                        jg    n10_match_arbno_af
                        movsxd           rcx, r14d
                        movzx            eax, byte ptr [r13+rcx]
                        cmp              eax, 97
                                                                                        jne   n10_match_arbno_af
                        add              r14d, 1
                                                                                        jmp   n10_match_arbno_as
n12_match_lit_β:
                        sub              r14d, 1
                                                                                        jmp   n10_match_arbno_af
#-----------------------------------------------------------------------------------------------------------------------
n13_lit_integer_α:
                        mov              qword ptr [rsp + 176], 6
                        mov              rax, qword ptr [rip + .Lx38_0]
                        mov              qword ptr [rsp + 184], rax
                                                                                        jmp   n14_match_pos_α
n13_lit_integer_β:
                                                                                        jmp   n2_match_head_β
.Lx38_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n14_match_pos_α:
                        mov              rax, 0
                        cmp              r14d, eax
                                                                                        jne   n2_match_head_β
                                                                                        jmp   n9_match_assign_save_α
n14_match_pos_β:
                                                                                        jmp   n2_match_head_β
#=======================================================================================================================
#         MATCHIT = 'fail'                                            :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n15_lit_string_α:
                        mov              qword ptr [rsp + 304], 1
                        mov              dword ptr [rsp + 308], 4
                        mov              rax, qword ptr [rip + .Lx40_0]
                        mov              qword ptr [rsp + 312], rax
                                                                                        jmp   n16_assign_α
n15_lit_string_β:
                                                                                        jmp   n17_save_restore_α
.Lx40_0:
                        .quad            .Lx40_0_s
.Lx40_0_s:
                        .string          "fail"
#-----------------------------------------------------------------------------------------------------------------------
n16_assign_α:
                        mov              rax, qword ptr [rsp + 304]
                        mov              rdx, qword ptr [rsp + 312]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n17_save_restore_α
n16_assign_β:
                                                                                        jmp   n17_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n17_save_restore_α:
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
                        mov              rsp, rbp
                        pop              rbp
                        xor              edi, edi
                        call             exit@PLT
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__MATCHIT_ω:
                        mov              rsp, rbp
                        pop              rbp
                        mov              edi, 1
                        call             exit@PLT
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_MATCHIT_α
proc_MATCHIT_α:
proc_MATCHIT_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n44_save_restore_α:
                        mov              rdi, qword ptr [rsp + 40]
                        mov              rsi, qword ptr [rsp + 48]
                        lea              rdx, [rsp + 64]
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
                        mov              rsp, rbp
                        pop              rbp
                        xor              edi, edi
                        call             exit@PLT
#-----------------------------------------------------------------------------------------------------------------------
proc_MATCHIT_ω:
                        mov              rsp, rbp
                        pop              rbp
                        mov              edi, 1
                        call             exit@PLT
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
                                                                                        jmp   main_α
#-----------------------------------------------------------------------------------------------------------------------
main_α:
main_α_body:
                        push             rbp
                        mov              rbp, rsp
                        sub              rsp, 8
#=======================================================================================================================
#         DEFINE('MATCHIT(S)') :(MATCHIT_END)
#         OUTPUT = MATCHIT('aaa')
#-----------------------------------------------------------------------------------------------------------------------
n50_lit_string_α:
                        mov              qword ptr [rsp + 384], 1
                        mov              dword ptr [rsp + 388], 3
                        mov              rax, qword ptr [rip + .Lx73_0]
                        mov              qword ptr [rsp + 392], rax
                                                                                        jmp   n51_call_α
n50_lit_string_β:
                                                                                        jmp   main_γ
.Lx73_0:
                        .quad            .Lx73_0_s
.Lx73_0_s:
                        .string          "aaa"
#-----------------------------------------------------------------------------------------------------------------------
n51_call_α:
                        sub              rsp, 32
                        mov              rax, qword ptr [1879052304]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052288]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx75_0]
                        mov              esi, 1
                        mov              edx, 1
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx75_5
                        mov              rax, qword ptr [rsp + 416]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 424]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx75_6]
                        lea              rdx, [rip + .Lx75_7]
                                                                                        jmp   rax
.Lx75_6:
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
                                                                                        jmp   .Lx75_2
.Lx75_7:
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
                                                                                        jmp   .Lx75_2
.Lx75_5:
                        add              rsp, 32
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx75_20
                        mov              rax, qword ptr [rsp + 384]
                        mov              rdx, qword ptr [rsp + 392]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx75_21
.Lx75_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 384]
                        mov              rdx, qword ptr [rsp + 392]
                        call             rt_arg_stage@PLT
.Lx75_21:
                        mov              rdi, qword ptr [rip + .Lx75_0]
                        mov              esi, 1
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx75_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx75_3]
                        lea              rdx, [rip + .Lx75_4]
                                                                                        jmp   rax
.Lx75_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx75_2
.Lx75_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx75_2
.Lx75_1:
                        call             rt_faildescr@PLT
.Lx75_2:
                        mov              qword ptr [rsp + 336], rax
                        mov              qword ptr [rsp + 344], rdx
                        cmp              eax, 99
                                                                                        je    main_γ
                                                                                        jmp   n52_assign_α
n51_call_β:
                                                                                        jmp   main_γ
.Lx75_0:
                        .quad            .Lx75_0_s
.Lx75_0_s:
                        .string          "MATCHIT"
#-----------------------------------------------------------------------------------------------------------------------
n52_assign_α:
                        mov              rsi, qword ptr [rsp + 336]
                        mov              rdx, qword ptr [rsp + 344]
                        mov              rdi, qword ptr [rip + .Lx76_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   main_γ
n52_assign_β:
                                                                                        jmp   main_γ
.Lx76_0:
                        .quad            .Lx76_0_s
.Lx76_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n53_goto_α:
                                                                                        jmp   n54_var_α
n53_goto_β:
                                                                                        jmp   main_ω
#=======================================================================================================================
# MATCHIT S POS(0) ARBNO('a') . V RPOS(0)                            :S(MI_YES)
#-----------------------------------------------------------------------------------------------------------------------
n54_var_α:
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 288], rax
                        mov              qword ptr [rsp + 296], rdx
                                                                                        jmp   n55_match_head_α
n54_var_β:
                                                                                        jmp   n68_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n55_match_head_α:
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
.Lx80_0:
                        mov              r14d, dword ptr [rsp + 0]
                                                                                        jmp   n56_match_sequence_α
n55_match_head_β:
                        add              dword ptr [rsp + 0], 1
                        mov              eax, dword ptr [rsp + 0]
                        cmp              eax, r15d
                                                                                        jg    .Lx80_1
                        lea              rcx, [rip + g_anchor]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                                                                                        jne   .Lx80_1
                                                                                        jmp   .Lx80_0
.Lx80_1:
                        mov              rax, qword ptr [rsp + 8]
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              rsp, qword ptr [rsp + 16]
                        mov              r10, qword ptr [1879048192]
.Lx80_2:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx80_2
                        mov              qword ptr [1879048192], r10
                        mov              r13, qword ptr [rbp + 80]
                        mov              r14, qword ptr [rbp + 88]
                        mov              r15, qword ptr [rbp + 96]
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              rdx, qword ptr [rbp + 104]
                        call             rt_match_ctx_restore@PLT
                        mov              rbp, qword ptr [rbp + 72]
                                                                                        jmp   n68_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n56_match_sequence_α:
                                                                                        jmp   n66_lit_integer_α
n56_match_sequence_as:
                                                                                        jmp   n57_match_release_α
n56_match_sequence_β:
                                                                                        jmp   n61_match_rpos_β
n56_match_sequence_af:
                                                                                        jmp   n55_match_head_β
#-----------------------------------------------------------------------------------------------------------------------
n57_match_release_α:
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
.Lx84_5:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx84_5
                        lea              rdi, [r10 + 24]
                        mov              rdx, r13
                        call             rt_dcap_end_ok_open@PLT
.Lx84_1:
                        test             rax, rax
                                                                                        je    .Lx84_2
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx84_3]
                        lea              rdx, [rip + .Lx84_4]
                                                                                        jmp   rax
.Lx84_3:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx84_1
.Lx84_4:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx84_1
.Lx84_2:
                        call             rt_dcap_end_ok_close@PLT
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              r10, qword ptr [1879048192]
.Lx84_6:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx84_6
                        mov              qword ptr [1879048192], r10
                        mov              r13, qword ptr [rbp + 80]
                        mov              r14, qword ptr [rbp + 88]
                        mov              r15, qword ptr [rbp + 96]
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              rdx, qword ptr [rbp + 104]
                        call             rt_match_ctx_restore@PLT
                        mov              rbp, qword ptr [rbp + 72]
                                                                                        jmp   n58_var_α
#=======================================================================================================================
# MI_YES  MATCHIT = V                                                 :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n58_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n59_assign_α
n58_var_β:
                        add              rsp, 16
                                                                                        jmp   n70_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n59_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n70_save_restore_α
n59_assign_β:
                                                                                        jmp   n70_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n60_lit_integer_α:
                        mov              qword ptr [rsp + 160], 6
                        mov              rax, qword ptr [rip + .Lx87_0]
                        mov              qword ptr [rsp + 168], rax
                                                                                        jmp   n61_match_rpos_α
n60_lit_integer_β:
                                                                                        jmp   n64_match_assign_cond_β
.Lx87_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n61_match_rpos_α:
                        mov              rax, 0
                        mov              ecx, r15d
                        sub              ecx, eax
                        cmp              r14d, ecx
                                                                                        jne   n64_match_assign_cond_β
                                                                                        jmp   n57_match_release_α
n61_match_rpos_β:
                                                                                        jmp   n64_match_assign_cond_β
#-----------------------------------------------------------------------------------------------------------------------
n62_match_assign_save_α:
                        sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d
                                                                                        jmp   n63_match_arbno_α
n62_match_assign_save_β:
                        add              rsp, 16
                                                                                        jmp   n55_match_head_β
#-----------------------------------------------------------------------------------------------------------------------
n63_match_arbno_α:
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
                                                                                        jmp   n64_match_assign_cond_α
n63_match_arbno_β:
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
                                                                                        jmp   n65_match_lit_α
n63_match_arbno_as:
                        mov              eax, dword ptr [rsp + 176]
                        cmp              r14d, eax
                                                                                        je    n65_match_lit_β
                        mov              dword ptr [rsp + 180], r14d
                                                                                        jmp   n64_match_assign_cond_α
n63_match_arbno_af:
                        mov              eax, dword ptr [rsp + 184]
                        test             eax, eax
                                                                                        jnz   .Lx92_2
                        add              rsp, 224
                                                                                        jmp   n65_match_lit_β
.Lx92_2:
                        mov              r14d, dword ptr [rsp + 176]
                        add              rsp, 224
                                                                                        jmp   n62_match_assign_save_β
#-----------------------------------------------------------------------------------------------------------------------
n64_match_assign_cond_α:
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
                                                                                        jmp   n60_lit_integer_α
n64_match_assign_cond_β:
                        mov              rax, qword ptr [1879048192]
                        sub              rax, 24
                        mov              qword ptr [1879048192], rax
                                                                                        jmp   n63_match_arbno_β
#-----------------------------------------------------------------------------------------------------------------------
n65_match_lit_α:
                        mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d
                                                                                        jg    n63_match_arbno_af
                        movsxd           rcx, r14d
                        movzx            eax, byte ptr [r13+rcx]
                        cmp              eax, 97
                                                                                        jne   n63_match_arbno_af
                        add              r14d, 1
                                                                                        jmp   n63_match_arbno_as
n65_match_lit_β:
                        sub              r14d, 1
                                                                                        jmp   n63_match_arbno_af
#-----------------------------------------------------------------------------------------------------------------------
n66_lit_integer_α:
                        mov              qword ptr [rsp + 176], 6
                        mov              rax, qword ptr [rip + .Lx97_0]
                        mov              qword ptr [rsp + 184], rax
                                                                                        jmp   n67_match_pos_α
n66_lit_integer_β:
                                                                                        jmp   n55_match_head_β
.Lx97_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n67_match_pos_α:
                        mov              rax, 0
                        cmp              r14d, eax
                                                                                        jne   n55_match_head_β
                                                                                        jmp   n62_match_assign_save_α
n67_match_pos_β:
                                                                                        jmp   n55_match_head_β
#=======================================================================================================================
#         MATCHIT = 'fail'                                            :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n68_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx99_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n69_assign_α
n68_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n70_save_restore_α
.Lx99_0:
                        .quad            .Lx99_0_s
.Lx99_0_s:
                        .string          "fail"
#-----------------------------------------------------------------------------------------------------------------------
n69_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 16
                                                                                        jmp   n70_save_restore_α
n69_assign_β:
                        add              rsp, 16
                                                                                        jmp   n70_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n70_save_restore_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n71_goto_α:
                                                                                        jmp   n58_var_α
n71_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n72_goto_α:
                                                                                        jmp   n50_lit_string_α
n72_goto_β:
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
.S0:                    .string          "V"
                        .text
                        .section         .note.GNU-stack,"",@progbits
