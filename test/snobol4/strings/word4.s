                        .intel_syntax    noprefix
                        .text
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_PAT$0_α
proc_PAT$0_α:
                        sub              rsp, 256
                        mov              qword ptr [rsp + 232], rcx
                        mov              qword ptr [rsp + 240], rdx
                        mov              qword ptr [rsp + 248], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rsp + 224], r8
                        mov              dword ptr [rsp + 216], r14d
proc_PAT$0_attempt:
proc_PAT$0_α_body:
                        lea              rax, [rip + proc_PAT$0_ω]
                        mov              qword ptr [rbp + 192], rax
#-----------------------------------------------------------------------------------------------------------------------
n0_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx15_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n1_match_pos_α
.Lx15_0:
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
                        mov              dword ptr [rsp + 0], r14d
                                                                                        jmp   n3_match_break_α
n2_match_assign_save_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   proc_PAT$0_scanfail
#-----------------------------------------------------------------------------------------------------------------------
n3_match_break_α:
                        sub              rsp, 16
                        movsxd           rcx, r14d
.Lx20_0:
                        cmp              ecx, r15d
                                                                                        jl    .Lx20_237
                        add              rsp, 16
                        add              rsp, 48
                                                                                        jmp   proc_PAT$0_scanfail
.Lx20_237:
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 32
                                                                                        je    .Lx20_1
                        add              ecx, 1
                        cmp              ecx, r15d
                                                                                        jl    .Lx20_238
                        add              rsp, 16
                        add              rsp, 48
                                                                                        jmp   proc_PAT$0_scanfail
.Lx20_238:
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 32
                                                                                        je    .Lx20_1
                        add              ecx, 1
                        cmp              ecx, r15d
                                                                                        jl    .Lx20_239
                        add              rsp, 16
                        add              rsp, 48
                                                                                        jmp   proc_PAT$0_scanfail
.Lx20_239:
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 32
                                                                                        je    .Lx20_1
                        add              ecx, 1
                        cmp              ecx, r15d
                                                                                        jl    .Lx20_240
                        add              rsp, 16
                        add              rsp, 48
                                                                                        jmp   proc_PAT$0_scanfail
.Lx20_240:
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 32
                                                                                        je    .Lx20_1
                        add              ecx, 1
                                                                                        jmp   .Lx20_0
.Lx20_1:
                        mov              dword ptr [rsp + 0], r14d
                        mov              r14d, ecx
                                                                                        jmp   n4_match_assign_cond_α
n3_match_break_β:
                        mov              r14d, dword ptr [rsp + 0]
                        add              rsp, 16
                        add              rsp, 48
                                                                                        jmp   proc_PAT$0_scanfail
#-----------------------------------------------------------------------------------------------------------------------
n4_match_assign_cond_α:
                        mov              eax, dword ptr [rsp + 16]
                        lea              rcx, [rip + .S0]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24
                                                                                        jmp   n5_match_lit_α
n4_match_assign_cond_β:
                        sub              r12, 24
                                                                                        jmp   n3_match_break_β
#-----------------------------------------------------------------------------------------------------------------------
n5_match_lit_α:
                        mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d
                                                                                        jg    n4_match_assign_cond_β
                        movsxd           rcx, r14d
                        movzx            eax, byte ptr [r13+rcx]
                        cmp              eax, 32
                                                                                        jne   n4_match_assign_cond_β
                        add              r14d, 1
                                                                                        jmp   n6_match_span_α
n5_match_lit_β:
                        sub              r14d, 1
                                                                                        jmp   n4_match_assign_cond_β
#-----------------------------------------------------------------------------------------------------------------------
n6_match_span_α:
                        sub              rsp, 16
                        movsxd           rcx, r14d
.Lx26_0:
                        cmp              ecx, r15d
                                                                                        jge   .Lx26_1
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 32
                                                                                        je    .Lx26_10
                                                                                        jmp   .Lx26_1
.Lx26_10:
                        add              ecx, 1
                        cmp              ecx, r15d
                                                                                        jge   .Lx26_1
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 32
                                                                                        je    .Lx26_11
                                                                                        jmp   .Lx26_1
.Lx26_11:
                        add              ecx, 1
                        cmp              ecx, r15d
                                                                                        jge   .Lx26_1
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 32
                                                                                        je    .Lx26_12
                                                                                        jmp   .Lx26_1
.Lx26_12:
                        add              ecx, 1
                        cmp              ecx, r15d
                                                                                        jge   .Lx26_1
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 32
                                                                                        je    .Lx26_13
                                                                                        jmp   .Lx26_1
.Lx26_13:
                        add              ecx, 1
                                                                                        jmp   .Lx26_0
.Lx26_1:
                        cmp              ecx, r14d
                                                                                        jg    .Lx26_240
                        add              rsp, 16
                        add              rsp, 48
                                                                                        jmp   n5_match_lit_β
.Lx26_240:
                        mov              dword ptr [rsp + 4], r14d
                        mov              r14d, ecx
                                                                                        jmp   n7_match_assign_save_α
n6_match_span_β:
                        mov              r14d, dword ptr [rsp + 4]
                        add              rsp, 16
                        add              rsp, 48
                                                                                        jmp   n5_match_lit_β
#-----------------------------------------------------------------------------------------------------------------------
n7_match_assign_save_α:
                        sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d
                                                                                        jmp   n8_match_breakx_α
n7_match_assign_save_β:
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n5_match_lit_β
#-----------------------------------------------------------------------------------------------------------------------
n8_match_breakx_α:
                        sub              rsp, 16
                        mov              dword ptr [rsp + 4], r14d
                        movsxd           rcx, r14d
.Lx30_0:
                        cmp              ecx, r15d
                                                                                        jl    .Lx30_237
                        add              rsp, 16
                        add              rsp, 96
                                                                                        jmp   n5_match_lit_β
.Lx30_237:
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 32
                                                                                        je    .Lx30_1
                        add              ecx, 1
                        cmp              ecx, r15d
                                                                                        jl    .Lx30_238
                        add              rsp, 16
                        add              rsp, 96
                                                                                        jmp   n5_match_lit_β
.Lx30_238:
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 32
                                                                                        je    .Lx30_1
                        add              ecx, 1
                        cmp              ecx, r15d
                                                                                        jl    .Lx30_239
                        add              rsp, 16
                        add              rsp, 96
                                                                                        jmp   n5_match_lit_β
.Lx30_239:
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 32
                                                                                        je    .Lx30_1
                        add              ecx, 1
                        cmp              ecx, r15d
                                                                                        jl    .Lx30_240
                        add              rsp, 16
                        add              rsp, 96
                                                                                        jmp   n5_match_lit_β
.Lx30_240:
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 32
                                                                                        je    .Lx30_1
                        add              ecx, 1
                                                                                        jmp   .Lx30_0
.Lx30_1:
                        mov              r14d, ecx
                                                                                        jmp   n9_match_assign_cond_α
n8_match_breakx_β:
                        movsxd           rcx, r14d
                        add              ecx, 1
.Lx30_2:
                        cmp              ecx, r15d
                                                                                        jge   .Lx30_4
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 32
                                                                                        je    .Lx30_3
                        add              ecx, 1
                        cmp              ecx, r15d
                                                                                        jge   .Lx30_4
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 32
                                                                                        je    .Lx30_3
                        add              ecx, 1
                        cmp              ecx, r15d
                                                                                        jge   .Lx30_4
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 32
                                                                                        je    .Lx30_3
                        add              ecx, 1
                        cmp              ecx, r15d
                                                                                        jge   .Lx30_4
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 32
                                                                                        je    .Lx30_3
                        add              ecx, 1
                                                                                        jmp   .Lx30_2
.Lx30_3:
                        mov              r14d, ecx
                                                                                        jmp   n9_match_assign_cond_α
.Lx30_4:
                        mov              r14d, dword ptr [rsp + 4]
                        add              rsp, 16
                        add              rsp, 96
                                                                                        jmp   n5_match_lit_β
#-----------------------------------------------------------------------------------------------------------------------
n9_match_assign_cond_α:
                        mov              eax, dword ptr [rsp + 16]
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
                                                                                        jmp   n8_match_breakx_β
#-----------------------------------------------------------------------------------------------------------------------
n10_match_lit_α:
                        mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d
                                                                                        jg    n9_match_assign_cond_β
                        movsxd           rcx, r14d
                        movzx            eax, byte ptr [r13+rcx]
                        cmp              eax, 32
                                                                                        jne   n9_match_assign_cond_β
                        add              r14d, 1
                                                                                        jmp   n11_match_span_α
n10_match_lit_β:
                        sub              r14d, 1
                                                                                        jmp   n9_match_assign_cond_β
#-----------------------------------------------------------------------------------------------------------------------
n11_match_span_α:
                        sub              rsp, 16
                        movsxd           rcx, r14d
.Lx36_0:
                        cmp              ecx, r15d
                                                                                        jge   .Lx36_1
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 32
                                                                                        je    .Lx36_10
                        cmp              esi, 58
                                                                                        je    .Lx36_10
                                                                                        jmp   .Lx36_1
.Lx36_10:
                        add              ecx, 1
                        cmp              ecx, r15d
                                                                                        jge   .Lx36_1
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 32
                                                                                        je    .Lx36_11
                        cmp              esi, 58
                                                                                        je    .Lx36_11
                                                                                        jmp   .Lx36_1
.Lx36_11:
                        add              ecx, 1
                        cmp              ecx, r15d
                                                                                        jge   .Lx36_1
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 32
                                                                                        je    .Lx36_12
                        cmp              esi, 58
                                                                                        je    .Lx36_12
                                                                                        jmp   .Lx36_1
.Lx36_12:
                        add              ecx, 1
                        cmp              ecx, r15d
                                                                                        jge   .Lx36_1
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 32
                                                                                        je    .Lx36_13
                        cmp              esi, 58
                                                                                        je    .Lx36_13
                                                                                        jmp   .Lx36_1
.Lx36_13:
                        add              ecx, 1
                                                                                        jmp   .Lx36_0
.Lx36_1:
                        cmp              ecx, r14d
                                                                                        jg    .Lx36_240
                        add              rsp, 16
                        add              rsp, 96
                                                                                        jmp   n10_match_lit_β
.Lx36_240:
                        mov              dword ptr [rsp + 4], r14d
                        mov              r14d, ecx
                                                                                        jmp   n12_match_assign_save_α
n11_match_span_β:
                        mov              r14d, dword ptr [rsp + 4]
                        add              rsp, 16
                        add              rsp, 96
                                                                                        jmp   n10_match_lit_β
#-----------------------------------------------------------------------------------------------------------------------
n12_match_assign_save_α:
                        sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d
                                                                                        jmp   n13_match_rem_α
n12_match_assign_save_β:
                        add              rsp, 16
                        add              rsp, 112
                                                                                        jmp   n10_match_lit_β
#-----------------------------------------------------------------------------------------------------------------------
n13_match_rem_α:
                        sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d
                        mov              r14d, r15d
                                                                                        jmp   n14_match_assign_cond_α
n13_match_rem_β:
                        mov              r14d, dword ptr [rsp + 0]
                        add              rsp, 16
                        add              rsp, 144
                                                                                        jmp   n10_match_lit_β
#-----------------------------------------------------------------------------------------------------------------------
n14_match_assign_cond_α:
                        mov              eax, dword ptr [rsp + 16]
                        lea              rcx, [rip + .S2]
                        mov              qword ptr [r12 + 0], rcx
                        mov              esi, eax
                        mov              qword ptr [r12 + 8], rsi
                        mov              edx, r14d
                        sub              edx, eax
                        mov              qword ptr [r12 + 16], rdx
                        add              r12, 24
                        add              rsp, 144
                                                                                        jmp   proc_PAT$0_scanhit
n14_match_assign_cond_β:
                        sub              r12, 24
                                                                                        jmp   n13_match_rem_β
proc_PAT$0_scanhit:
                        cmp              qword ptr [rbp + 224], 1
                                                                                        jne   .Lx42_0
                        mov              ecx, dword ptr [rbp + 216]
                        lea              rdx, [rip + g_scan_hit_start]
                        mov              dword ptr [rdx + 0], ecx
.Lx42_0:
                                                                                        jmp   proc_PAT$0_γ
proc_PAT$0_scanfail:
                        cmp              qword ptr [rbp + 224], 1
                                                                                        jne   .Lx43_0
                        mov              eax, dword ptr [rbp + 216]
                        inc              eax
                        cmp              eax, r15d
                                                                                        jg    .Lx43_0
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        cmp              qword ptr [rcx + 0], 0
                                                                                        jne   .Lx43_0
                        mov              dword ptr [rbp + 216], eax
                        mov              r14d, eax
                        mov              rsp, rbp
                                                                                        jmp   proc_PAT$0_attempt
.Lx43_0:
                                                                                        jmp   proc_PAT$0_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$0_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$0_β:
                                                                                        jmp   qword ptr [rbp + 192]
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$0_γ:
                        push             rbp
                        lea              rax, [rip + proc_PAT$0_res]
                        push             rax
                        mov              rax, qword ptr [rbp + 232]
                        mov              rbp, qword ptr [rbp + 248]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$0_ω:
                        mov              rax, qword ptr [rbp + 240]
                        lea              rsp, [rbp + 256]
                        mov              rbp, qword ptr [rbp + 248]
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
                        mov              esi, 208
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
n44_statement_begin_α:
                                                                                        jmp   n45_statement_end_α
n44_statement_begin_β:
                                                                                        jmp   n46_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n45_statement_end_α:
                                                                                        jmp   n46_statement_begin_α
#=======================================================================================================================
#       PAT      =  POS(0) BREAK(' ') . WHEN (' ' SPAN(' '))
#-----------------------------------------------------------------------------------------------------------------------
n46_statement_begin_α:
                                                                                        jmp   n47_lit_string_α
n46_statement_begin_β:
                                                                                        jmp   n51_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n47_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx82_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n48_call_α
.Lx82_0:
                        .quad            .Lx82_0_s
.Lx82_0_s:
                        .string          "PAT$0"
#-----------------------------------------------------------------------------------------------------------------------
n48_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd84:             .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd84]                        # fn
                        lea              rsi, [rsp + 0]                                 # args
                        mov              edx, 1                                         # nargs
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 104
                                                                                        jne   .Lx83_240
                                                                                        jmp   n50_statement_end_α
.Lx83_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n49_assign_α
n48_call_β:
                                                                                        jmp   n50_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n49_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax                    # PAT
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n50_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n50_statement_end_α:
                        add              rsp, 32
                                                                                        jmp   n51_statement_begin_α
#=======================================================================================================================
# LOOP  LINE     =  INPUT                            :F(END)
#-----------------------------------------------------------------------------------------------------------------------
n51_statement_begin_α:
                                                                                        jmp   n52_var_α
n51_statement_begin_β:
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n52_var_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx90_0]                 # name
                        call             NV_GET_fn@PLT
                        cmp              eax, 104
                                                                                        jne   .Lx90_240
                        add              rsp, 16
                                                                                        jmp   main_γ
.Lx90_240:
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n53_assign_α
.Lx90_0:
                        .quad            .Lx90_0_s
.Lx90_0_s:
                        .string          "INPUT"
#-----------------------------------------------------------------------------------------------------------------------
n53_assign_α:
                        mov              rax, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052352], rax                    # LINE
                        mov              qword ptr [1879052360], rdx
                                                                                        jmp   n54_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n54_statement_end_α:
                        add              rsp, 16
                                                                                        jmp   n55_statement_begin_α
#=======================================================================================================================
#       LINE     ?  PAT                              :F(LOOP)
#-----------------------------------------------------------------------------------------------------------------------
n55_statement_begin_α:
                                                                                        jmp   n56_var_α
n55_statement_begin_β:
                                                                                        jmp   n51_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n56_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052352]                    # LINE
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n57_var_α
#-----------------------------------------------------------------------------------------------------------------------
n57_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052288]                    # PAT
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n58_assign_α
n57_var_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n51_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n58_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # var
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx98_0]                 # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n59_match_begin_α
n58_assign_β:
                                                                                        jmp   n57_var_β
.Lx98_0:
                        .quad            .Lx98_0_s
.Lx98_0_s:
                        .string          "PATV$0"
#-----------------------------------------------------------------------------------------------------------------------
n59_match_begin_α:
                        mov              rdi, qword ptr [rsp + 16]                      # var
                        mov              rsi, qword ptr [rsp + 24]
                        mov              qword ptr [rbp + 160], r13                     # outer_Σ
                        mov              qword ptr [rbp + 168], r14                     # outer_δ
                        mov              qword ptr [rbp + 176], r15                     # outer_Δ
                        lea              rcx, [rip + g_cap_gen]
                        mov              eax, dword ptr [rcx + 0]
                        mov              qword ptr [rbp + 184], rax                     # cap_gen
                        mov              qword ptr [rbp + 152], rbp                     # old_rbp
                        call             rt_match_enter@PLT
                        mov              r13, rax
                        mov              r15, rdx
                        mov              qword ptr [r12 + 0], 0                         # cas_top
                        mov              qword ptr [r12 + 8], rsp                       # cas_rsp_mark
                        mov              qword ptr [r12 + 16], 0
                        add              r12, 24                                        # cas_top
                        mov              qword ptr [rbp + 128], rsp                     # zls2_mark
                        mov              dword ptr [rbp + 112], 0                       # start_δ
.Lx100_0:
                        mov              r14d, dword ptr [rbp + 112]
                                                                                        jmp   n60_match_defer_α
n59_match_begin_β:
                        add              dword ptr [rbp + 112], 1
                        mov              eax, dword ptr [rbp + 112]
                        cmp              eax, r15d
                                                                                        jg    .Lx100_1
                        mov              rcx, qword ptr [rip + rt_anchor_g@GOTPCREL]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                                                                                        jne   .Lx100_1
                                                                                        jmp   .Lx100_0
.Lx100_1:
n59_match_begin_af:
                        sub              r12, 24                                        # cas_mark
                        mov              rsp, qword ptr [rbp + 128]
                        mov              r13, qword ptr [rbp + 160]                     # outer_Σ
                        mov              r14, qword ptr [rbp + 168]                     # outer_δ
                        mov              r15, qword ptr [rbp + 176]                     # outer_Δ
                        mov              rdi, r13                                       # sig
                        mov              rsi, r15                                       # len
                        mov              rdx, qword ptr [rbp + 184]                     # cap_gen
                        call             rt_match_ctx_restore@PLT
                        mov              rbp, qword ptr [rbp + 152]                     # old_rbp
                                                                                        jmp   n58_assign_β
#-----------------------------------------------------------------------------------------------------------------------
n60_match_defer_α:
                        lea              rdi, [rip + .S3]                               # varname
                        xor              esi, esi                                       # ival_flag
                        call             rt_defer_get_pat_fn@PLT
                        test             rax, rax
                                                                                        jz    .Lx101_0
                        mov              r8d, 1
                        lea              rcx, [rip + .Lx101_4]
                        lea              rdx, [rip + .Lx101_5]
                                                                                        jmp   rax
.Lx101_4:
                        lea              rcx, [rip + g_scan_hit_start]
                        mov              rax, qword ptr [rcx]
                        mov              dword ptr [rbp + 112], eax
                                                                                        jmp   n61_match_end_α
.Lx101_5:
                                                                                        jmp   n59_match_begin_β
.Lx101_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S3]                               # varname
                        xor              esi, esi                                       # ival_flag
                        call             rt_defer_open@PLT
.Lx101_2:
                        test             rax, rax
                                                                                        je    .Lx101_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx101_7]
                        lea              rdx, [rip + .Lx101_8]
                                                                                        jmp   rax
.Lx101_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx101_2
.Lx101_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax                                       # fret
                        mov              rsi, rdx                                       # fret
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx101_2
.Lx101_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d                                      # cur_delta
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n59_match_begin_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx101_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n61_match_end_α
.Lx101_6:
                        add              rsp, 16
                                                                                        jmp   n59_match_begin_β
n60_match_defer_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n61_match_end_α:
                        mov              r10, r12
.Lx103_9:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx103_9
                        mov              rsp, qword ptr [rbp + 128]
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        mov              rsi, r12
                        mov              r10, rsi
.Lx103_5:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx103_5
                        lea              rdi, [r10 + 24]
                        mov              rdx, r13
                        call             rt_dcap_end_ok_open@PLT
.Lx103_1:
                        test             rax, rax
                                                                                        je    .Lx103_2
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx103_3]
                        lea              rdx, [rip + .Lx103_4]
                                                                                        jmp   rax
.Lx103_3:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx103_1
.Lx103_4:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx103_1
.Lx103_2:
                        call             rt_dcap_end_ok_close@PLT
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        sub              r12, 24                                        # cas_mark
                        mov              r13, qword ptr [rbp + 160]                     # outer_Σ
                        mov              r14, qword ptr [rbp + 168]                     # outer_δ
                        mov              r15, qword ptr [rbp + 176]                     # outer_Δ
                        mov              rdi, r13                                       # sig
                        mov              rsi, r15                                       # len
                        mov              rdx, qword ptr [rbp + 184]                     # cap_gen
                        call             rt_match_ctx_restore@PLT
                        mov              rbp, qword ptr [rbp + 152]                     # old_rbp
                                                                                        jmp   n62_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n62_statement_end_α:
                        add              rsp, 32
                                                                                        jmp   n63_statement_begin_α
#=======================================================================================================================
#       OUTPUT   =  WHO " invented the " WHAT " in " WHEN  :(LOOP)
#-----------------------------------------------------------------------------------------------------------------------
n63_statement_begin_α:
                                                                                        jmp   n64_var_α
n63_statement_begin_β:
                                                                                        jmp   n51_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n64_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]                    # WHO
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n65_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n65_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 14
                        mov              rax, qword ptr [rip + .Lx109_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n66_binop_α
n65_lit_string_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n51_statement_begin_α
.Lx109_0:
                        .quad            .Lx109_0_s
.Lx109_0_s:
                        .string          " invented the "
#-----------------------------------------------------------------------------------------------------------------------
n66_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # var
                        mov              rsi, qword ptr [rsp + 40]                      # a
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # b
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n67_var_α
n66_binop_β:
                        add              rsp, 16
                                                                                        jmp   n65_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n67_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]                    # WHAT
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n68_binop_α
n67_var_β:
                        add              rsp, 16
                                                                                        jmp   n66_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n68_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # binop
                        mov              rsi, qword ptr [rsp + 40]                      # a
                        mov              rdx, qword ptr [rsp + 16]                      # var
                        mov              rcx, qword ptr [rsp + 24]                      # b
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n69_lit_string_α
n68_binop_β:
                        add              rsp, 16
                                                                                        jmp   n67_var_β
#-----------------------------------------------------------------------------------------------------------------------
n69_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx113_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n70_binop_α
n69_lit_string_β:
                        add              rsp, 16
                                                                                        jmp   n68_binop_β
.Lx113_0:
                        .quad            .Lx113_0_s
.Lx113_0_s:
                        .string          " in "
#-----------------------------------------------------------------------------------------------------------------------
n70_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # binop
                        mov              rsi, qword ptr [rsp + 40]                      # a
                        mov              rdx, qword ptr [rsp + 16]                      # lit_string
                        mov              rcx, qword ptr [rsp + 24]                      # b
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n71_var_α
n70_binop_β:
                        add              rsp, 16
                                                                                        jmp   n69_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n71_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]                    # WHEN
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n72_binop_α
n71_var_β:
                        add              rsp, 16
                                                                                        jmp   n70_binop_β
#-----------------------------------------------------------------------------------------------------------------------
n72_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # binop
                        mov              rsi, qword ptr [rsp + 40]                      # a
                        mov              rdx, qword ptr [rsp + 16]                      # var
                        mov              rcx, qword ptr [rsp + 24]                      # b
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n73_assign_α
n72_binop_β:
                        add              rsp, 16
                                                                                        jmp   n71_var_β
#-----------------------------------------------------------------------------------------------------------------------
n73_assign_α:
                        mov              rsi, qword ptr [rsp + 0]                       # binop
                        mov              rdx, qword ptr [rsp + 8]                       # val
                        mov              rdi, qword ptr [rip + .Lx117_0]                # name
                        call             NV_SET_fn@PLT
                                                                                        jmp   n74_statement_end_α
.Lx117_0:
                        .quad            .Lx117_0_s
.Lx117_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n74_statement_end_α:
                        add              rsp, 144
                                                                                        jmp   n51_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n75_goto_α:
                                                                                        jmp   n51_statement_begin_α
n75_goto_β:
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
