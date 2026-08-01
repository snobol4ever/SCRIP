                        .intel_syntax    noprefix
                        .text
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_PAT$0_α
proc_PAT$0_α:
                        mov              qword ptr [rsp + 272], r8
                        mov              dword ptr [rsp + 264], r14d
proc_PAT$0_attempt:
proc_PAT$0_α_body:
                        lea              rax, [rip + n0_match_sequence_β]
                        mov              qword ptr [rbp + 240], rax
#-----------------------------------------------------------------------------------------------------------------------
n0_match_sequence_α:
                                                                                        jmp   n16_lit_integer_α
n0_match_sequence_as:
                                                                                        jmp   proc_PAT$0_γ
n0_match_sequence_β:
                                                                                        jmp   n3_match_assign_cond_β
n0_match_sequence_af:
                                                                                        jmp   proc_PAT$0_ω
#-----------------------------------------------------------------------------------------------------------------------
n1_match_assign_save_α:
                        sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d
                                                                                        jmp   n2_match_rem_α
n1_match_assign_save_β:
                        add              rsp, 16
                                                                                        jmp   n4_match_sequence_β
#-----------------------------------------------------------------------------------------------------------------------
n2_match_rem_α:
                        sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d
                        mov              r14d, r15d
                                                                                        jmp   n3_match_assign_cond_α
n2_match_rem_β:
                        mov              r14d, dword ptr [rsp + 0]
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n4_match_sequence_β
#-----------------------------------------------------------------------------------------------------------------------
n3_match_assign_cond_α:
                        mov              eax, dword ptr [rsp + 16]
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
                                                                                        jmp   proc_PAT$0_γ
n3_match_assign_cond_β:
                        mov              rax, qword ptr [1879048192]
                        sub              rax, 24
                        mov              qword ptr [1879048192], rax
                                                                                        jmp   n2_match_rem_β
#-----------------------------------------------------------------------------------------------------------------------
n4_match_sequence_α:
                                                                                        jmp   n6_match_lit_α
n4_match_sequence_as:
                                                                                        jmp   n1_match_assign_save_α
n4_match_sequence_β:
                                                                                        jmp   n5_match_span_β
n4_match_sequence_af:
                                                                                        jmp   n9_match_assign_cond_β
#-----------------------------------------------------------------------------------------------------------------------
n5_match_span_α:
                        sub              rsp, 16
                        movsxd           rcx, r14d
.Lx28_0:
                        cmp              ecx, r15d
                                                                                        jge   .Lx28_1
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 32
                                                                                        je    .Lx28_10
                        cmp              esi, 58
                                                                                        je    .Lx28_10
                                                                                        jmp   .Lx28_1
.Lx28_10:
                        add              ecx, 1
                        cmp              ecx, r15d
                                                                                        jge   .Lx28_1
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 32
                                                                                        je    .Lx28_11
                        cmp              esi, 58
                                                                                        je    .Lx28_11
                                                                                        jmp   .Lx28_1
.Lx28_11:
                        add              ecx, 1
                        cmp              ecx, r15d
                                                                                        jge   .Lx28_1
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 32
                                                                                        je    .Lx28_12
                        cmp              esi, 58
                                                                                        je    .Lx28_12
                                                                                        jmp   .Lx28_1
.Lx28_12:
                        add              ecx, 1
                        cmp              ecx, r15d
                                                                                        jge   .Lx28_1
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 32
                                                                                        je    .Lx28_13
                        cmp              esi, 58
                                                                                        je    .Lx28_13
                                                                                        jmp   .Lx28_1
.Lx28_13:
                        add              ecx, 1
                                                                                        jmp   .Lx28_0
.Lx28_1:
                        cmp              ecx, r14d
                                                                                        jg    .Lx28_240
                        add              rsp, 16
                                                                                        jmp   n6_match_lit_β
.Lx28_240:
                        mov              dword ptr [rsp + 4], r14d
                        mov              r14d, ecx
                                                                                        jmp   n1_match_assign_save_α
n5_match_span_β:
                        mov              r14d, dword ptr [rsp + 4]
                        add              rsp, 16
                                                                                        jmp   n6_match_lit_β
#-----------------------------------------------------------------------------------------------------------------------
n6_match_lit_α:
                        mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d
                                                                                        jg    n9_match_assign_cond_β
                        movsxd           rcx, r14d
                        movzx            eax, byte ptr [r13+rcx]
                        cmp              eax, 32
                                                                                        jne   n9_match_assign_cond_β
                        add              r14d, 1
                                                                                        jmp   n5_match_span_α
n6_match_lit_β:
                        sub              r14d, 1
                                                                                        jmp   n9_match_assign_cond_β
#-----------------------------------------------------------------------------------------------------------------------
n7_match_assign_save_α:
                        sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d
                                                                                        jmp   n8_match_breakx_α
n7_match_assign_save_β:
                        add              rsp, 16
                                                                                        jmp   n10_match_sequence_β
#-----------------------------------------------------------------------------------------------------------------------
n8_match_breakx_α:
                        sub              rsp, 16
                        mov              dword ptr [rsp + 4], r14d
                        movsxd           rcx, r14d
.Lx34_0:
                        cmp              ecx, r15d
                                                                                        jl    .Lx34_237
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n10_match_sequence_β
.Lx34_237:
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 32
                                                                                        je    .Lx34_1
                        add              ecx, 1
                        cmp              ecx, r15d
                                                                                        jl    .Lx34_238
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n10_match_sequence_β
.Lx34_238:
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 32
                                                                                        je    .Lx34_1
                        add              ecx, 1
                        cmp              ecx, r15d
                                                                                        jl    .Lx34_239
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n10_match_sequence_β
.Lx34_239:
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 32
                                                                                        je    .Lx34_1
                        add              ecx, 1
                        cmp              ecx, r15d
                                                                                        jl    .Lx34_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n10_match_sequence_β
.Lx34_240:
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 32
                                                                                        je    .Lx34_1
                        add              ecx, 1
                                                                                        jmp   .Lx34_0
.Lx34_1:
                        mov              r14d, ecx
                                                                                        jmp   n9_match_assign_cond_α
n8_match_breakx_β:
                        movsxd           rcx, r14d
                        add              ecx, 1
.Lx34_2:
                        cmp              ecx, r15d
                                                                                        jge   .Lx34_4
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 32
                                                                                        je    .Lx34_3
                        add              ecx, 1
                        cmp              ecx, r15d
                                                                                        jge   .Lx34_4
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 32
                                                                                        je    .Lx34_3
                        add              ecx, 1
                        cmp              ecx, r15d
                                                                                        jge   .Lx34_4
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 32
                                                                                        je    .Lx34_3
                        add              ecx, 1
                        cmp              ecx, r15d
                                                                                        jge   .Lx34_4
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 32
                                                                                        je    .Lx34_3
                        add              ecx, 1
                                                                                        jmp   .Lx34_2
.Lx34_3:
                        mov              r14d, ecx
                                                                                        jmp   n9_match_assign_cond_α
.Lx34_4:
                        mov              r14d, dword ptr [rsp + 4]
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n10_match_sequence_β
#-----------------------------------------------------------------------------------------------------------------------
n9_match_assign_cond_α:
                        mov              eax, dword ptr [rsp + 16]
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
                                                                                        jmp   n4_match_sequence_α
n9_match_assign_cond_β:
                        mov              rax, qword ptr [1879048192]
                        sub              rax, 24
                        mov              qword ptr [1879048192], rax
                                                                                        jmp   n8_match_breakx_β
#-----------------------------------------------------------------------------------------------------------------------
n10_match_sequence_α:
                                                                                        jmp   n12_match_lit_α
n10_match_sequence_as:
                                                                                        jmp   n7_match_assign_save_α
n10_match_sequence_β:
                                                                                        jmp   n11_match_span_β
n10_match_sequence_af:
                                                                                        jmp   n15_match_assign_cond_β
#-----------------------------------------------------------------------------------------------------------------------
n11_match_span_α:
                        sub              rsp, 16
                        movsxd           rcx, r14d
.Lx40_0:
                        cmp              ecx, r15d
                                                                                        jge   .Lx40_1
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 32
                                                                                        je    .Lx40_10
                                                                                        jmp   .Lx40_1
.Lx40_10:
                        add              ecx, 1
                        cmp              ecx, r15d
                                                                                        jge   .Lx40_1
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 32
                                                                                        je    .Lx40_11
                                                                                        jmp   .Lx40_1
.Lx40_11:
                        add              ecx, 1
                        cmp              ecx, r15d
                                                                                        jge   .Lx40_1
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 32
                                                                                        je    .Lx40_12
                                                                                        jmp   .Lx40_1
.Lx40_12:
                        add              ecx, 1
                        cmp              ecx, r15d
                                                                                        jge   .Lx40_1
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 32
                                                                                        je    .Lx40_13
                                                                                        jmp   .Lx40_1
.Lx40_13:
                        add              ecx, 1
                                                                                        jmp   .Lx40_0
.Lx40_1:
                        cmp              ecx, r14d
                                                                                        jg    .Lx40_240
                        add              rsp, 16
                                                                                        jmp   n12_match_lit_β
.Lx40_240:
                        mov              dword ptr [rsp + 4], r14d
                        mov              r14d, ecx
                                                                                        jmp   n7_match_assign_save_α
n11_match_span_β:
                        mov              r14d, dword ptr [rsp + 4]
                        add              rsp, 16
                                                                                        jmp   n12_match_lit_β
#-----------------------------------------------------------------------------------------------------------------------
n12_match_lit_α:
                        mov              eax, r14d
                        add              eax, 1
                        cmp              eax, r15d
                                                                                        jg    n15_match_assign_cond_β
                        movsxd           rcx, r14d
                        movzx            eax, byte ptr [r13+rcx]
                        cmp              eax, 32
                                                                                        jne   n15_match_assign_cond_β
                        add              r14d, 1
                                                                                        jmp   n11_match_span_α
n12_match_lit_β:
                        sub              r14d, 1
                                                                                        jmp   n15_match_assign_cond_β
#-----------------------------------------------------------------------------------------------------------------------
n13_match_assign_save_α:
                        sub              rsp, 16
                        mov              dword ptr [rsp + 0], r14d
                                                                                        jmp   n14_match_break_α
n13_match_assign_save_β:
                        add              rsp, 16
                                                                                        jmp   proc_PAT$0_ω
#-----------------------------------------------------------------------------------------------------------------------
n14_match_break_α:
                        sub              rsp, 16
                        movsxd           rcx, r14d
.Lx46_0:
                        cmp              ecx, r15d
                                                                                        jl    .Lx46_237
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   proc_PAT$0_ω
.Lx46_237:
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 32
                                                                                        je    .Lx46_1
                        add              ecx, 1
                        cmp              ecx, r15d
                                                                                        jl    .Lx46_238
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   proc_PAT$0_ω
.Lx46_238:
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 32
                                                                                        je    .Lx46_1
                        add              ecx, 1
                        cmp              ecx, r15d
                                                                                        jl    .Lx46_239
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   proc_PAT$0_ω
.Lx46_239:
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 32
                                                                                        je    .Lx46_1
                        add              ecx, 1
                        cmp              ecx, r15d
                                                                                        jl    .Lx46_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   proc_PAT$0_ω
.Lx46_240:
                        movzx            esi, byte ptr [r13+rcx]
                        cmp              esi, 32
                                                                                        je    .Lx46_1
                        add              ecx, 1
                                                                                        jmp   .Lx46_0
.Lx46_1:
                        mov              dword ptr [rsp + 0], r14d
                        mov              r14d, ecx
                                                                                        jmp   n15_match_assign_cond_α
n14_match_break_β:
                        mov              r14d, dword ptr [rsp + 0]
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   proc_PAT$0_ω
#-----------------------------------------------------------------------------------------------------------------------
n15_match_assign_cond_α:
                        mov              eax, dword ptr [rsp + 16]
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
                                                                                        jmp   n10_match_sequence_α
n15_match_assign_cond_β:
                        mov              rax, qword ptr [1879048192]
                        sub              rax, 24
                        mov              qword ptr [1879048192], rax
                                                                                        jmp   n14_match_break_β
#-----------------------------------------------------------------------------------------------------------------------
n16_lit_integer_α:
                        mov              qword ptr [rbp + 48], 6
                        mov              rax, qword ptr [rip + .Lx49_0]
                        mov              qword ptr [rbp + 56], rax
                                                                                        jmp   n17_match_pos_α
n16_lit_integer_β:
                                                                                        jmp   proc_PAT$0_ω
.Lx49_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n17_match_pos_α:
                        mov              rax, 0
                        cmp              r14d, eax
                                                                                        jne   proc_PAT$0_ω
                                                                                        jmp   n13_match_assign_save_α
n17_match_pos_β:
                                                                                        jmp   proc_PAT$0_ω
proc_PAT$0_scanhit:
                        cmp              qword ptr [rbp + 272], 1
                                                                                        jne   7f
                        mov              ecx, dword ptr [rbp + 264]
                        lea              rdx, [rip + g_scan_hit_start]
                        mov              dword ptr [rdx], ecx
7:
                                                                                        jmp   proc_PAT$0_γ
proc_PAT$0_scanfail:
                        cmp              qword ptr [rbp + 272], 1
                                                                                        jne   8f
                        mov              eax, dword ptr [rbp + 264]
                        inc              eax
                        cmp              eax, r15d
                                                                                        jg    8f
                        lea              rcx, [rip + g_anchor]
                        cmp              qword ptr [rcx], 0
                                                                                        jne   8f
                        mov              dword ptr [rbp + 264], eax
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
                                                                                        jmp   qword ptr [rbp + 240]
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$0_γ:
                        mov              rsp, rbp
                        pop              rbp
                        xor              edi, edi
                        call             exit@PLT
#-----------------------------------------------------------------------------------------------------------------------
proc_PAT$0_ω:
                        mov              rsp, rbp
                        pop              rbp
                        mov              edi, 1
                        call             exit@PLT
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
                        xor              esi, esi
                                                                                        jmp   main_α
#-----------------------------------------------------------------------------------------------------------------------
main_α:
main_α_body:
                        push             rbp
                        mov              rbp, rsp
                        sub              rsp, 8
#=======================================================================================================================
#       PAT      =  POS(0) BREAK(' ') . WHEN (' ' SPAN(' '))
#-----------------------------------------------------------------------------------------------------------------------
n51_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx71_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n52_call_α
.Lx71_0:
                        .quad            .Lx71_0_s
.Lx71_0_s:
                        .string          "PAT$0"
#-----------------------------------------------------------------------------------------------------------------------
n52_call_α:
                        sub              rsp, 16
                        sub              rsp, 16
                        mov              r10, qword ptr [rsp + 32]
                        mov              r11, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 0], r10
                        mov              qword ptr [rsp + 8], r11
                        .section         .rodata
.Lrkfnzd73:             .string          "SNO$MKPAT"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd73]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        add              rsp, 16
                        cmp              eax, 99
                                                                                        jne   .Lx72_240
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n54_var_α
.Lx72_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n53_assign_α
n52_call_β:
                        add              rsp, 16
                        add              rsp, 16
                                                                                        jmp   n54_var_α
#-----------------------------------------------------------------------------------------------------------------------
n53_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 32
                                                                                        jmp   n54_var_α
#=======================================================================================================================
# LOOP  LINE     =  INPUT                            :F(END)
#-----------------------------------------------------------------------------------------------------------------------
n54_var_α:
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
                                                                                        jmp   n55_assign_α
.Lx75_0:
                        .quad            .Lx75_0_s
.Lx75_0_s:
                        .string          "INPUT"
#-----------------------------------------------------------------------------------------------------------------------
n55_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052352], rax
                        mov              qword ptr [1879052360], rdx
                        add              rsp, 16
                                                                                        jmp   n56_var_α
#=======================================================================================================================
#       LINE     ?  PAT                              :F(LOOP)
#-----------------------------------------------------------------------------------------------------------------------
n56_var_α:
                        mov              rax, qword ptr [1879052352]
                        mov              rdx, qword ptr [1879052360]
                        mov              qword ptr [rsp + 224], rax
                        mov              qword ptr [rsp + 232], rdx
                                                                                        jmp   n57_match_head_α
#-----------------------------------------------------------------------------------------------------------------------
n57_match_head_α:
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
.Lx79_0:
                        mov              r14d, dword ptr [rbp + 112]
                                                                                        jmp   n58_match_patref_α
n57_match_head_β:
                        add              dword ptr [rbp + 112], 1
                        mov              eax, dword ptr [rbp + 112]
                        cmp              eax, r15d
                                                                                        jg    .Lx79_1
                        lea              rcx, [rip + g_anchor]
                        mov              rax, qword ptr [rcx]
                        cmp              rax, 0
                                                                                        jne   .Lx79_1
                                                                                        jmp   .Lx79_0
.Lx79_1:
                        mov              rax, qword ptr [rbp + 120]
                        lea              rcx, [rip + g_patstk_sp]
                        mov              qword ptr [rcx + 0], rax
                        mov              rsp, qword ptr [rbp + 128]
                        mov              r10, qword ptr [1879048192]
.Lx79_2:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx79_2
                        mov              qword ptr [1879048192], r10
                        mov              r13, qword ptr [rbp + 160]
                        mov              r14, qword ptr [rbp + 168]
                        mov              r15, qword ptr [rbp + 176]
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              rdx, qword ptr [rbp + 184]
                        call             rt_match_ctx_restore@PLT
                        mov              rbp, qword ptr [rbp + 152]
                                                                                        jmp   n54_var_α
#-----------------------------------------------------------------------------------------------------------------------
n58_match_patref_α:
                        lea              rsi, [rip + g_sno_defer_cells+0]
                        mov              rax, qword ptr [rsi + 0]
                        test             rax, rax
                                                                                        jne   .Lx80_11
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        cmp              eax, 3
                                                                                        jne   .Lx80_9
                        mov              rax, qword ptr [rdx + 0]
                        test             rax, rax
                                                                                        jne   .Lx80_10
                        mov              rdi, rdx
                        call             dtp_fn_of@PLT
                                                                                        jmp   .Lx80_10
.Lx80_9:
                        xor              eax, eax
.Lx80_10:
                        lea              rsi, [rip + g_sno_defer_cells+0]
                        mov              qword ptr [rsi + 0], rax
.Lx80_11:
                        test             rax, rax
                                                                                        jz    .Lx80_0
                        mov              r8d, 1
                        lea              rcx, [rip + .Lx80_4]
                        lea              rdx, [rip + .Lx80_5]
                                                                                        jmp   rax
.Lx80_4:
                        lea              rcx, [rip + g_scan_hit_start]
                        mov              rax, qword ptr [rcx]
                        mov              dword ptr [rbp + 112], eax
                                                                                        jmp   n59_match_release_α
.Lx80_5:
                                                                                        jmp   n57_match_head_β
.Lx80_0:
                        push             r14
                        push             r15
                        push             r13
                        sub              rsp, 8
                        lea              rdi, [rip + .S3]
                        xor              esi, esi
                        call             rt_defer_open@PLT
.Lx80_2:
                        test             rax, rax
                                                                                        je    .Lx80_3
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx80_7]
                        lea              rdx, [rip + .Lx80_8]
                                                                                        jmp   rax
.Lx80_7:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx80_2
.Lx80_8:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_defer_step@PLT
                                                                                        jmp   .Lx80_2
.Lx80_3:
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              edi, r14d
                        call             rt_defer_close@PLT
                        test             eax, eax
                                                                                        js    n57_match_head_β
                        mov              r14d, eax
                        lea              rax, [rip + .Lx80_6]
                        sub              rsp, 8
                        push             rax
                                                                                        jmp   n59_match_release_α
.Lx80_6:
                        add              rsp, 16
                                                                                        jmp   n57_match_head_β
n58_match_patref_β:
                                                                                        jmp   qword ptr [rsp]
#-----------------------------------------------------------------------------------------------------------------------
n59_match_release_α:
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
.Lx82_5:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx82_5
                        lea              rdi, [r10 + 24]
                        mov              rdx, r13
                        call             rt_dcap_end_ok_open@PLT
.Lx82_1:
                        test             rax, rax
                                                                                        je    .Lx82_2
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx82_3]
                        lea              rdx, [rip + .Lx82_4]
                                                                                        jmp   rax
.Lx82_3:
                        call             rt_proc_call_epilogue_γ@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx82_1
.Lx82_4:
                        call             rt_proc_call_epilogue_ω@PLT
                        mov              rdi, rax
                        mov              rsi, rdx
                        call             rt_dcap_step@PLT
                                                                                        jmp   .Lx82_1
.Lx82_2:
                        call             rt_dcap_end_ok_close@PLT
                        add              rsp, 8
                        pop              r13
                        pop              r15
                        pop              r14
                        mov              r10, qword ptr [1879048192]
.Lx82_6:
                        sub              r10, 24
                        mov              rax, qword ptr [r10 + 0]
                        test             rax, rax
                                                                                        jne   .Lx82_6
                        mov              qword ptr [1879048192], r10
                        mov              r13, qword ptr [rbp + 160]
                        mov              r14, qword ptr [rbp + 168]
                        mov              r15, qword ptr [rbp + 176]
                        mov              rdi, r13
                        mov              rsi, r15
                        mov              rdx, qword ptr [rbp + 184]
                        call             rt_match_ctx_restore@PLT
                        mov              rbp, qword ptr [rbp + 152]
                                                                                        jmp   n60_var_α
#=======================================================================================================================
#       OUTPUT   =  WHO " invented the " WHAT " in " WHEN  :(LOOP)
#-----------------------------------------------------------------------------------------------------------------------
n60_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n61_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n61_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 14
                        mov              rax, qword ptr [rip + .Lx84_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n62_binop_α
.Lx84_0:
                        .quad            .Lx84_0_s
.Lx84_0_s:
                        .string          " invented the "
#-----------------------------------------------------------------------------------------------------------------------
n62_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n63_var_α
#-----------------------------------------------------------------------------------------------------------------------
n63_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n64_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n64_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n65_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n65_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx88_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n66_binop_α
.Lx88_0:
                        .quad            .Lx88_0_s
.Lx88_0_s:
                        .string          " in "
#-----------------------------------------------------------------------------------------------------------------------
n66_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n67_var_α
#-----------------------------------------------------------------------------------------------------------------------
n67_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n68_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n68_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n69_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n69_assign_α:
                        mov              rsi, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx92_0]
                        call             NV_SET_fn@PLT
                        add              rsp, 144
                                                                                        jmp   n54_var_α
.Lx92_0:
                        .quad            .Lx92_0_s
.Lx92_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n70_goto_α:
                                                                                        jmp   n54_var_α
n70_goto_β:
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
.S0:                    .string          "WHAT"
.S1:                    .string          "WHO"
.S2:                    .string          "WHEN"
.S3:                    .string          "PAT"
                        .text
                        .section         .note.GNU-stack,"",@progbits
