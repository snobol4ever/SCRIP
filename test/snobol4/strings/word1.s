                        .intel_syntax    noprefix
                        .text
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_PAT$0_α
proc_PAT$0_α:
                        .global          proc_PAT$0_α
                        .global          proc_PAT$0_β
                        .global          proc_PAT$0_γ
                        .global          proc_PAT$0_ω
                        sub              rsp, 208
                        mov              [rsp + 184], rcx
                        mov              [rsp + 192], rdx
                        mov              [rsp + 200], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp], 0
                        mov              qword ptr [rsp + 8], 0
                        mov              qword ptr [rsp + 80], 0
                        mov              qword ptr [rsp + 88], 0
                        mov              qword ptr [rsp + 144], 0
                        mov              qword ptr [rsp + 152], 0
                        mov              qword ptr [rsp + 160], 0
                        mov              qword ptr [rsp + 168], 0
                        mov              qword ptr [rsp + 176], r8
                        mov              dword ptr [rsp + 168], r14d
proc_PAT$0_attempt:
proc_PAT$0_α_body:
                        lea              rax, [rip + n0_match_sequence_β]
                        mov              qword ptr [rbp + 144], rax
#-----------------------------------------------------------------------------------------------------------------------
n0_match_sequence_α:
                                                                                        jmp   n7_match_lit_α
n0_match_sequence_as:
                                                                                        jmp   proc_PAT$0_γ
n0_match_sequence_β:
                                                                                        jmp   n1_match_alternate_β
n0_match_sequence_af:
                                                                                        jmp   proc_PAT$0_ω
#-----------------------------------------------------------------------------------------------------------------------
n1_match_alternate_α:
                        sub              rsp, 32
                        mov              dword ptr [rbp + 112], r14d
                        lea              rax, [rip + .Lx11_21]
                        mov              qword ptr [rbp + 128], rax
                                                                                        jmp   n3_match_lit_α
.Lx11_21:
                        lea              rax, [rip + .Lx11_19]
                        mov              qword ptr [rbp + 128], rax
                                                                                        jmp   n2_match_lit_α
n1_match_alternate_s0:
                        lea              rax, [rip + .Lx11_40]
                        mov              qword ptr [rbp + 120], rax
                                                                                        jmp   n1_match_alternate_as
n1_match_alternate_s1:
                        lea              rax, [rip + .Lx11_41]
                        mov              qword ptr [rbp + 120], rax
                                                                                        jmp   n1_match_alternate_as
.Lx11_40:
                                                                                        jmp   n3_match_lit_β
.Lx11_41:
                                                                                        jmp   n2_match_lit_β
n1_match_alternate_as:
                        add              rsp, 32
                                                                                        jmp   proc_PAT$0_γ
n1_match_alternate_β:
                        sub              rsp, 32
                        mov              rax, qword ptr [rbp + 120]
                                                                                        jmp   rax
n1_match_alternate_af:
                        mov              r14d, dword ptr [rbp + 112]
                        mov              rax, qword ptr [rbp + 128]
                                                                                        jmp   rax
.Lx11_19:
                        add              rsp, 32
                                                                                        jmp   n6_match_assign_cond_β
#-----------------------------------------------------------------------------------------------------------------------
n2_match_lit_α:
                        mov              eax, r14d
                        add              eax, 3
                        cmp              eax, r15d
                                                                                        jg    n1_match_alternate_af
                        movsxd           rcx, r14d
                        movzx            eax, byte ptr [r13+rcx]
                        cmp              eax, 32
                                                                                        jne   n1_match_alternate_af
                        movzx            eax, byte ptr [r13+rcx+1]
                        cmp              eax, 97
                                                                                        jne   n1_match_alternate_af
                        movzx            eax, byte ptr [r13+rcx+2]
                        cmp              eax, 32
                                                                                        jne   n1_match_alternate_af
                        add              r14d, 3
                                                                                        jmp   n1_match_alternate_s1
n2_match_lit_β:
                        sub              r14d, 3
                                                                                        jmp   n1_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n3_match_lit_α:
                        mov              eax, r14d
                        add              eax, 4
                        cmp              eax, r15d
                                                                                        jg    n1_match_alternate_af
                        movsxd           rcx, r14d
                        mov              edx, dword ptr [r13+rcx]
                        cmp              edx, 543584032
                                                                                        jne   n1_match_alternate_af
                        add              r14d, 4
                                                                                        jmp   n1_match_alternate_s0
n3_match_lit_β:
                        sub              r14d, 4
                                                                                        jmp   n1_match_alternate_af
#-----------------------------------------------------------------------------------------------------------------------
n4_match_assign_save_α:
                        sub              rsp, 32
                        lea              rdi, [rbp + 80]
                        mov              esi, r14d
                        call             rt_cap_push@PLT
                        add              rsp, 32
                                                                                        jmp   n5_match_arb_α
n4_match_assign_save_β:
                        sub              rsp, 32
                        lea              rdi, [rbp + 80]
                        call             rt_cap_pop@PLT
                        add              rsp, 32
                                                                                        jmp   n7_match_lit_β
#-----------------------------------------------------------------------------------------------------------------------
n5_match_arb_α:
                        sub              rsp, 16
                        mov              dword ptr [rsp + 0], 0
                        mov              eax, r14d
                        mov              dword ptr [rsp + 4], eax
                                                                                        jmp   n6_match_assign_cond_α
n5_match_arb_β:
                        add              dword ptr [rsp + 0], 1
                        mov              eax, dword ptr [rsp + 4]
                        add              eax, dword ptr [rsp + 0]
                        cmp              eax, r15d
                                                                                        jg    .Lx19_0
                        mov              r14d, eax
                                                                                        jmp   n6_match_assign_cond_α
.Lx19_0:
                        mov              r14d, dword ptr [rsp + 4]
                        add              rsp, 16
                                                                                        jmp   n4_match_assign_save_β
#-----------------------------------------------------------------------------------------------------------------------
n6_match_assign_cond_α:
                        lea              rdi, [rbp + 80]
                        call             rt_cap_top@PLT
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
                                                                                        jmp   n1_match_alternate_α
n6_match_assign_cond_β:
                        mov              rax, qword ptr [1879048192]
                        sub              rax, 24
                        mov              qword ptr [1879048192], rax
                                                                                        jmp   n5_match_arb_β
#-----------------------------------------------------------------------------------------------------------------------
n7_match_lit_α:
                        mov              eax, r14d
                        add              eax, 5
                        cmp              eax, r15d
                                                                                        jg    proc_PAT$0_ω
                        movsxd           rcx, r14d
                        mov              edx, dword ptr [r13+rcx]
                        cmp              edx, 1701344288
                                                                                        jne   proc_PAT$0_ω
                        movzx            eax, byte ptr [r13+rcx+4]
                        cmp              eax, 32
                                                                                        jne   proc_PAT$0_ω
                        add              r14d, 5
                                                                                        jmp   n4_match_assign_save_α
n7_match_lit_β:
                        sub              r14d, 5
                                                                                        jmp   proc_PAT$0_ω
proc_PAT$0_scanhit:
                        cmp              qword ptr [rbp + 176], 1
                                                                                        jne   7f
                        mov              ecx, dword ptr [rbp + 168]
                        lea              rdx, [rip + g_scan_hit_start]
                        mov              dword ptr [rdx], ecx
7:
                                                                                        jmp   proc_PAT$0_γ
proc_PAT$0_scanfail:
                        cmp              qword ptr [rbp + 176], 1
                                                                                        jne   8f
                        mov              eax, dword ptr [rbp + 168]
                        inc              eax
                        cmp              eax, r15d
                                                                                        jg    8f
                        lea              rcx, [rip + g_anchor]
                        cmp              qword ptr [rcx], 0
                                                                                        jne   8f
                        mov              dword ptr [rbp + 168], eax
                        mov              r14d, eax
                        mov              rsp, rbp
                                                                                        jmp   proc_PAT$0_attempt
8:
                                                                                        jmp   proc_PAT$0_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$0_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$0_β:
                                                                                        jmp   qword ptr [rbp + 144]
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$0_γ:
                        push             rbp
                        lea              rax, [rip + proc_PAT$0_res]
                        push             rax
                        mov              rax, [rbp + 184]
                        mov              rbp, [rbp + 200]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$0_ω:
                        mov              rax, [rbp + 192]
                        lea              rsp, [rbp + 208]
                        mov              rbp, [rbp + 200]
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
                        mov              esi, 160
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
.Lgvan1:                .string          "LINE"
                        .align           8
__gva_names:
                        .quad            .Lgvan0
                        .quad            .Lgvan1
                        .section         .text
                        .intel_syntax    noprefix
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        call             proc_startup
                        mov              edi, 2
                        call             rt_gva_island@PLT
                        mov              rsi, rax
                        lea              rdi, [rip + __gva_names]
                        mov              edx, 2
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
                        sub              rsp, 248
                        mov              rdi, rsp
                        mov              ecx, 248
                        xor              eax, eax
                        rep stosb
                        mov              [rsp + 240], rbp
                        mov              rbp, rsp
main_α_body:
#=======================================================================================================================
#       PAT      =  " the " ARB . OUTPUT (" of " | " a ")
#-----------------------------------------------------------------------------------------------------------------------
n24_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx34_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n25_call_α
.Lx34_0:
                        .quad            .Lx34_0_s
.Lx34_0_s:
                        .string          "PAT$0"
#-----------------------------------------------------------------------------------------------------------------------
n25_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd36:             .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd36]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 99
                                                                                        jne   .Lx35_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n27_var_α
.Lx35_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n26_assign_α
n25_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n27_var_α
#-----------------------------------------------------------------------------------------------------------------------
n26_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 32
                                                                                        jmp   n27_var_α
#=======================================================================================================================
# LOOP  LINE     =  INPUT                            :F(END)
#-----------------------------------------------------------------------------------------------------------------------
n27_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx38_0]
                        call             NV_GET_fn@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx38_240
                        add              rsp, 16
                                                                                        jmp   main_γ
.Lx38_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n28_assign_α
.Lx38_0:
                        .quad            .Lx38_0_s
.Lx38_0_s:
                        .string          "INPUT"
#-----------------------------------------------------------------------------------------------------------------------
n28_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                        add              rsp, 16
                                                                                        jmp   n29_var_α
#=======================================================================================================================
#       LINE     ?  PAT                              :(LOOP)
#-----------------------------------------------------------------------------------------------------------------------
n29_var_α:
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 224], rax
                        mov              qword ptr [rsp + 232], rdx
                                                                                        jmp   n30_match_head_α
#-----------------------------------------------------------------------------------------------------------------------
n30_match_head_α:
                        mov              qword ptr [rbp + 160], r13
                        mov              qword ptr [rbp + 168], r14
                        mov              qword ptr [rbp + 176], r15
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rbp + 184], rax
                        mov              qword ptr [rbp + 152], rbp
                        mov              rdi, qword ptr [rbp + 224]
                        mov              rsi, qword ptr [rbp + 232]
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              r10, qword ptr [1879048192]
                        mov              qword ptr [r10 + 0], 0
                        add              r10, 24
                        mov              qword ptr [1879048192], r10
                        mov              qword ptr [rbp + 128], rsp
                        lea              rcx, [rip + g_patstk_sp]
                        mov              rax, qword ptr [rcx + 0]
                        mov              qword ptr [rbp + 120], rax
                        mov              dword ptr [rbp + 112], 0
.Lx42_0:
                        mov              r14d, dword ptr [rbp + 112]
                                                                                        jmp   n31_match_patref_α
n30_match_head_β:
                        add              dword ptr [rbp + 112], 1
                        mov              eax, dword ptr [rbp + 112]
                        cmp              eax, r15d
                                                                                        jg    .Lx42_1
                        lea              rcx, [rip + g_anchor]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                                                                                        jne   .Lx42_1
                                                                                        jmp   .Lx42_0
.Lx42_1:
                        mov              rax, qword ptr [rbp + 120]
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              rsp, qword ptr [rbp + 128]
                        mov              r10, qword ptr [1879048192]
.Lx42_2:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx42_2
                        mov              qword ptr [1879048192], r10
                        mov              r13, qword ptr [rbp + 160]
                        mov              r14, qword ptr [rbp + 168]
                        mov              r15, qword ptr [rbp + 176]
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              rdx, qword ptr [rbp + 184]
                        call             rt_match_ctx_restore@PLT
                        mov              rbp, qword ptr [rbp + 152]
                                                                                        jmp   n27_var_α
#-----------------------------------------------------------------------------------------------------------------------
n31_match_patref_α:
                        lea              rsi, [rip + g_sno_defer_cells+0]
                        mov              rax, qword ptr [rsi + 0]
                        test             rax, rax
                                                                                        jne   .Lx43_11
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        cmp              eax, 3
                                                                                        jne   .Lx43_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx43_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx43_10
.Lx43_9:
                        xor              eax, eax
.Lx43_10:
                        lea              rsi, [rip + g_sno_defer_cells+0]
                        mov              qword ptr [rsi + 0], rax
.Lx43_11:
                        test             rax, rax
                                                                                        jz    .Lx43_0
                        mov              r8d, 1
                        lea              rcx, [rip + .Lx43_4]
                        lea              rdx, [rip + .Lx43_5]
                                                                                        jmp   rax
.Lx43_4:
                        lea              rcx, [rip + g_scan_hit_start]
                        mov              rax, qword ptr [rcx]
                        mov              dword ptr [rbp + 112], eax
                                                                                        jmp   n32_match_release_α
.Lx43_5:
                                                                                        jmp   n30_match_head_β
.Lx43_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S1]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx43_2:
                        test             rax, rax
                                                                                        je    .Lx43_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx43_7]
                        lea              rdx, [rip + .Lx43_8]
                                                                                        jmp   rax
.Lx43_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx43_2
.Lx43_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx43_2
.Lx43_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n30_match_head_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx43_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n32_match_release_α
.Lx43_6:
                        add              rsp, 16
                                                                                        jmp   n30_match_head_β
n31_match_patref_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n32_match_release_α:
                        mov              rax, qword ptr [rbp + 120]
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              rsp, qword ptr [rbp + 128]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, qword ptr [1879048192]
                        mov              r10, rsi
.Lx45_5:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx45_5
                        lea              rdi, [r10 + 24]
                        mov              rdx, r13
                        call             rt_dcap_end_ok_open@PLT
.Lx45_1:
                        test             rax, rax
                                                                                        je    .Lx45_2
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx45_3]
                        lea              rdx, [rip + .Lx45_4]
                                                                                        jmp   rax
.Lx45_3:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx45_1
.Lx45_4:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx45_1
.Lx45_2:
                        call             rt_dcap_end_ok_close@PLT
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              r10, qword ptr [1879048192]
.Lx45_6:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx45_6
                        mov              qword ptr [1879048192], r10
                        mov              r13, qword ptr [rbp + 160]
                        mov              r14, qword ptr [rbp + 168]
                        mov              r15, qword ptr [rbp + 176]
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              rdx, qword ptr [rbp + 184]
                        call             rt_match_ctx_restore@PLT
                        mov              rbp, qword ptr [rbp + 152]
                                                                                        jmp   n27_var_α
#-----------------------------------------------------------------------------------------------------------------------
n33_goto_α:
                                                                                        jmp   n27_var_α
n33_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
                        mov              eax, 1
                        xor              edx, edx
                        mov              rsp, rbp
                        mov              rbp, [rsp + 240]
                        add              rsp, 248
                        ret
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              rsp, rbp
                        mov              eax, 99
                        xor              edx, edx
                        mov              rbp, [rsp + 240]
                        add              rsp, 248
                        ret
                        .section         .rodata
.S0:                    .string          "OUTPUT"
.S1:                    .string          "PAT"
                        .text
                        .section         .note.GNU-stack,"",@progbits
