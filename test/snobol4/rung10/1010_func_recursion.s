                        .intel_syntax    noprefix
                        .text
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        call             module_init
                        mov              edi, 4
                        call             rt_gva_island@PLT
                        mov              rsi, rax
                        lea              rdi, [rip + __gva_names]
                        mov              edx, 4
                        call             gva_register@PLT
                        mov              r12, qword ptr [0x70000000]
                        call             rtcc_load_all@PLT
                        xor              esi, esi
                                                                              jmp   main_α
                        .section         .rodata
.Lgvan0:                .string          "fact"
.Lgvan1:                .string          "n"
.Lgvan2:                .string          "fact2"
.Lgvan3:                .string          "facto"
                        .align           8
__gva_names:
                        .quad            .Lgvan0
                        .quad            .Lgvan1
                        .quad            .Lgvan2
                        .quad            .Lgvan3
                        .section         .text
                        .intel_syntax    noprefix
#-----------------------------------------------------------------------------------------------------------------------
main_α:
main_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n0_statement_begin_α:   mov              r11, 1
                        mov              r10, 0;                              jmp   n1_statement_end_α
n0_statement_begin_β:   mov              r11, 1;                              jmp   n2_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n1_statement_end_α:     mov              r11, 2
                        mov              r10, 1;                              jmp   n2_statement_begin_α
#=======================================================================================================================
#         DEFINE('fact(n)')                              :(fact_end)
#-----------------------------------------------------------------------------------------------------------------------
n2_statement_begin_α:   mov              r11, 3
                        mov              r10, 2;                              jmp   n3_define_α
n2_statement_begin_β:   mov              r11, 3;                              jmp   n23_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n3_define_α:            mov              r11, 4
                        mov              rdi, qword ptr [rip + .Lx118_0]
                        mov              rsi, qword ptr [rip + .Lx118_1]
                        mov              edx, 1
                        mov              ecx, 1
                        mov              r8d, 0
                        lea              r9, [rip + n5_statement_begin_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n4_statement_end_α
n3_define_β:            mov              r11, 4;                              jmp   n2_statement_begin_β
.Lx118_0:               .quad            .Lx118_0_s
.Lx118_0_s:             .string          "fact"
.Lx118_1:               .quad            .Lx118_1_s
.Lx118_1_s:             .string          "n"
#-----------------------------------------------------------------------------------------------------------------------
n4_statement_end_α:     mov              r11, 5
                        mov              r10, 2;                              jmp   n23_statement_begin_α
#=======================================================================================================================
# fact    fact = EQ(n, 1) 1                              :s(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n5_statement_begin_α:   mov              r11, 6
                        mov              r10, 3;                              jmp   n6_var_α
n5_statement_begin_β:   mov              r11, 6;                              jmp   n14_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n6_var_α:               sub              rsp, 16
                        mov              r11, 7
                        mov              rax, qword ptr [r9 + 16]             # n
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n7_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n7_lit_integer_α:       sub              rsp, 16
                        mov              r11, 8
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx125_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n8_coerce_numeric_α
n7_lit_integer_β:       mov              r11, 8
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n5_statement_begin_β
.Lx125_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n8_coerce_numeric_α:    sub              rsp, 16
                        mov              r11, 9
                        mov              eax, dword ptr [rsp + 32]            # var
                        cmp              al, 5;                               je    .Lx127_1
                        cmp              al, 3;                               jne   .Lx127_0
                        mov              eax, dword ptr [rsp + 16]            # lit_integer
                        cmp              al, 3;                               jne   .Lx127_0
.Lx127_1:               mov              rax, qword ptr [rsp + 32]            # var
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # var
                        mov              qword ptr [rsp + 8], rax;            jmp   n9_coerce_numeric_α
.Lx127_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]                      # lit_integer
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 101
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n9_coerce_numeric_α
n8_coerce_numeric_β:    mov              r11, 9
                        add              rsp, 16;                             jmp   n7_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n9_coerce_numeric_α:    sub              rsp, 16
                        mov              r11, 10
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              al, 5;                               je    .Lx129_1
                        cmp              al, 3;                               jne   .Lx129_0
                        mov              eax, dword ptr [rsp + 48]            # var
                        cmp              al, 3;                               jne   .Lx129_0
.Lx129_1:               mov              rax, qword ptr [rsp + 32]            # lit_integer
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n10_cmp_test_α
.Lx129_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 48]                      # var
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 102
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n10_cmp_test_α
n9_coerce_numeric_β:    mov              r11, 10
                        add              rsp, 16;                             jmp   n8_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n10_cmp_test_α:         sub              rsp, 16
                        mov              r11, 11
                        mov              eax, dword ptr [rsp + 32]            # coerce_numeric
                        mov              ecx, dword ptr [rsp + 16]
                        mov              edx, eax
                        and              edx, ecx
                        cmp              dl, 3;                               jne   .Lx131_0
                        mov              rax, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 24]
                        cmp              rax, rdx;                            je    .Lx131_239
                        add              rsp, 16;                             jmp   n9_coerce_numeric_β
.Lx131_239:                                                                   jmp   n11_lit_integer_α
.Lx131_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_cmp_d@PLT
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        test             eax, eax;                            je    .Lx131_240
                        add              rsp, 16;                             jmp   n9_coerce_numeric_β
.Lx131_240:                                                                   jmp   n11_lit_integer_α
n10_cmp_test_β:         mov              r11, 11
                        add              rsp, 16;                             jmp   n9_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n11_lit_integer_α:      sub              rsp, 16
                        mov              r11, 12
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx132_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n12_assign_α
.Lx132_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n12_assign_α:           mov              r11, 13
                        mov              rax, qword ptr [rsp + 0]             # lit_integer
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax              # fact
                        mov              qword ptr [r9 + 8], rdx;             jmp   n13_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n13_statement_end_α:    mov              r11, 14
                        mov              r10, 3
                        add              rsp, 96;                             jmp   RETURN
#=======================================================================================================================
#         fact = n * fact(n - 1)                         :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n14_statement_begin_α:  mov              r11, 15
                        mov              r10, 4;                              jmp   n15_var_α
n14_statement_begin_β:  mov              r11, 15;                             jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n15_var_α:              sub              rsp, 16
                        mov              r11, 16
                        mov              rax, qword ptr [r9 + 16]             # n
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n16_var_α
#-----------------------------------------------------------------------------------------------------------------------
n16_var_α:              sub              rsp, 16
                        mov              r11, 17
                        mov              rax, qword ptr [r9 + 16]             # n
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n17_lit_integer_α
n16_var_β:              mov              r11, 17
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n14_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n17_lit_integer_α:      sub              rsp, 16
                        mov              r11, 18
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx140_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n18_binop_α
n17_lit_integer_β:      mov              r11, 18
                        add              rsp, 16;                             jmp   n16_var_β
.Lx140_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n18_binop_α:            sub              rsp, 16
                        mov              r11, 19
                        mov              ecx, dword ptr [rsp + 32]            # var
                        mov              rax, qword ptr [rsp + 40]
                        cmp              cl, 3;                               jne   .Lx141_2
                        sub              rax, 1
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              qword ptr [rsp + 8], rax;            jmp   .Lx141_7
.Lx141_2:               mov              eax, ecx
                        mov              edx, ecx
                        and              edx, 1;                              jz    .Lx141_0
                        mov              rsi, qword ptr [rsp + 40]            # var
                        mov              rdi, 1
                        cmp              al, 5;                               je    .Lx141_3
                        cvtsi2sd         xmm0, rsi;                           jmp   .Lx141_4
.Lx141_3:               movq             xmm0, rsi
.Lx141_4:               cvtsi2sd         xmm1, rdi
                        subsd            xmm0, xmm1
                        movq             rax, xmm0
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              qword ptr [rsp + 8], rax
.Lx141_7:                                                                     jmp   n19_call_α
.Lx141_0:               mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_sub@PLT
                        cmp              al, 104;                             jne   .Lx141_240
                        add              rsp, 16;                             jmp   n17_lit_integer_β
.Lx141_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n19_call_α
n18_binop_β:            mov              r11, 19
                        add              rsp, 16;                             jmp   n17_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n19_call_α:             sub              rsp, 16
                        mov              r11, 20
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 16]            # binop
                        mov              rdx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_arg_stage@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdi, qword ptr [rip + .Lx143_0]
                        mov              esi, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_call_open@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        test             rax, rax;                            je    .Lx143_1
                        mov              rdi, qword ptr [rip + .Lx143_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        lea              rcx, [rip + .Lx143_4]                # s111 floater pair (ZD twin NON-SLIM fallback): THE arm GVA-off actually reaches — MONITOR_BIN forces n_gva_m3=0, the slim tail at ~:403 that s110 patched refuses, and the site falls through to rt_proc_call_open here with flat rcx/rdx wires and NO pair.  Push omega then gamma = [rsp+0]=gamma [rsp+8]=omega; the fnrbp2 floater consumes 16 so L(3)/L(4) arrive at today's depth.  SCRIP_SLIM_PAIR=0 restores prior bytes.
                        push             rcx
                        lea              rcx, [rip + .Lx143_3]
                        push             rcx
                        lea              rcx, [rip + .Lx143_3]
                        lea              rdx, [rip + .Lx143_4];               jmp   rax
.Lx143_3:               mov              rdi, qword ptr [rip + .Lx143_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_call_epilogue_named_γ@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx143_2
.Lx143_4:               mov              rdi, qword ptr [rip + .Lx143_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_call_epilogue_named_ω@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx143_2
.Lx143_1:               call             rt_faildescr@PLT
.Lx143_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx143_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx143_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx143_240
                        add              rsp, 16;                             jmp   n18_binop_β
.Lx143_240:                                                                   jmp   n20_binop_α
n19_call_β:             mov              r11, 20;                             jmp   n18_binop_β
.Lx143_0:               .quad            .Lx143_0_s
.Lx143_0_s:             .string          "fact"
#-----------------------------------------------------------------------------------------------------------------------
n20_binop_α:            sub              rsp, 16
                        mov              r11, 21
                        mov              eax, dword ptr [rsp + 80]            # var
                        mov              ecx, dword ptr [rsp + 16]            # call
                        mov              edx, eax
                        and              edx, ecx
                        cmp              dl, 3;                               jne   .Lx144_2
                        mov              rax, qword ptr [rsp + 88]            # var
                        mov              rdx, qword ptr [rsp + 24]            # call
                        imul             rax, rdx
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              qword ptr [rsp + 8], rax;            jmp   .Lx144_7
.Lx144_2:               and              edx, 1;                              jz    .Lx144_0
                        mov              rsi, qword ptr [rsp + 88]            # var
                        mov              rdi, qword ptr [rsp + 24]            # call
                        cmp              al, 5;                               je    .Lx144_3
                        cvtsi2sd         xmm0, rsi;                           jmp   .Lx144_4
.Lx144_3:               movq             xmm0, rsi
.Lx144_4:               cmp              cl, 5;                               je    .Lx144_5
                        cvtsi2sd         xmm1, rdi;                           jmp   .Lx144_6
.Lx144_5:               movq             xmm1, rdi
.Lx144_6:               mulsd            xmm0, xmm1
                        movq             rax, xmm0
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              qword ptr [rsp + 8], rax
.Lx144_7:                                                                     jmp   n21_assign_α
.Lx144_0:               mov              rdi, qword ptr [rsp + 80]            # var
                        mov              rsi, qword ptr [rsp + 88]
                        mov              rdx, qword ptr [rsp + 16]            # call
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_mul@PLT
                        cmp              al, 104;                             jne   .Lx144_240
                        add              rsp, 32;                             jmp   n18_binop_β
.Lx144_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n21_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n21_assign_α:           mov              r11, 22
                        mov              rax, qword ptr [rsp + 0]             # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax              # fact
                        mov              qword ptr [r9 + 8], rdx;             jmp   n22_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n22_statement_end_α:    mov              r11, 23
                        mov              r10, 4
                        add              rsp, 96;                             jmp   RETURN
#=======================================================================================================================
# fact_end  <stmt 5, line 8: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n23_statement_begin_α:  mov              r11, 24
                        mov              r10, 5;                              jmp   n24_statement_end_α
n23_statement_begin_β:  mov              r11, 24;                             jmp   n25_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n24_statement_end_α:    mov              r11, 25
                        mov              r10, 5;                              jmp   n25_statement_begin_α
#=======================================================================================================================
#         NE(fact(5), 120)           :f(e001)
#-----------------------------------------------------------------------------------------------------------------------
n25_statement_begin_α:  mov              r11, 26
                        mov              r10, 6;                              jmp   n26_lit_integer_α
n25_statement_begin_β:  mov              r11, 26;                             jmp   n37_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n26_lit_integer_α:      sub              rsp, 16
                        mov              r11, 27
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx154_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n27_call_α
.Lx154_0:               .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n27_call_α:             sub              rsp, 16
                        mov              r11, 28
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 16]            # lit_integer
                        mov              rdx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_arg_stage@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdi, qword ptr [rip + .Lx156_0]
                        mov              esi, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_call_open@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        test             rax, rax;                            je    .Lx156_1
                        mov              rdi, qword ptr [rip + .Lx156_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        lea              rcx, [rip + .Lx156_4]                # s111 floater pair (ZD twin NON-SLIM fallback): THE arm GVA-off actually reaches — MONITOR_BIN forces n_gva_m3=0, the slim tail at ~:403 that s110 patched refuses, and the site falls through to rt_proc_call_open here with flat rcx/rdx wires and NO pair.  Push omega then gamma = [rsp+0]=gamma [rsp+8]=omega; the fnrbp2 floater consumes 16 so L(3)/L(4) arrive at today's depth.  SCRIP_SLIM_PAIR=0 restores prior bytes.
                        push             rcx
                        lea              rcx, [rip + .Lx156_3]
                        push             rcx
                        lea              rcx, [rip + .Lx156_3]
                        lea              rdx, [rip + .Lx156_4];               jmp   rax
.Lx156_3:               mov              rdi, qword ptr [rip + .Lx156_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_call_epilogue_named_γ@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx156_2
.Lx156_4:               mov              rdi, qword ptr [rip + .Lx156_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_call_epilogue_named_ω@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx156_2
.Lx156_1:               call             rt_faildescr@PLT
.Lx156_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx156_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx156_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx156_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n25_statement_begin_β
.Lx156_240:                                                                   jmp   n28_lit_integer_α
n27_call_β:             mov              r11, 28;                             jmp   n25_statement_begin_β
.Lx156_0:               .quad            .Lx156_0_s
.Lx156_0_s:             .string          "fact"
#-----------------------------------------------------------------------------------------------------------------------
n28_lit_integer_α:      sub              rsp, 16
                        mov              r11, 29
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx157_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n29_coerce_numeric_α
n28_lit_integer_β:      mov              r11, 29
                        add              rsp, 16
                        add              rsp, 32;                             jmp   n25_statement_begin_β
.Lx157_0:               .quad            120
#-----------------------------------------------------------------------------------------------------------------------
n29_coerce_numeric_α:   sub              rsp, 16
                        mov              r11, 30
                        mov              eax, dword ptr [rsp + 32]            # call
                        cmp              al, 5;                               je    .Lx159_1
                        cmp              al, 3;                               jne   .Lx159_0
                        mov              eax, dword ptr [rsp + 16]            # lit_integer
                        cmp              al, 3;                               jne   .Lx159_0
.Lx159_1:               mov              rax, qword ptr [rsp + 32]            # call
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # call
                        mov              qword ptr [rsp + 8], rax;            jmp   n30_coerce_numeric_α
.Lx159_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]                      # lit_integer
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 149
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n30_coerce_numeric_α
n29_coerce_numeric_β:   mov              r11, 30
                        add              rsp, 16;                             jmp   n28_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n30_coerce_numeric_α:   sub              rsp, 16
                        mov              r11, 31
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              al, 5;                               je    .Lx161_1
                        cmp              al, 3;                               jne   .Lx161_0
                        mov              eax, dword ptr [rsp + 48]            # call
                        cmp              al, 3;                               jne   .Lx161_0
.Lx161_1:               mov              rax, qword ptr [rsp + 32]            # lit_integer
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n31_cmp_test_α
.Lx161_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 48]                      # call
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 150
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n31_cmp_test_α
n30_coerce_numeric_β:   mov              r11, 31
                        add              rsp, 16;                             jmp   n29_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n31_cmp_test_α:         sub              rsp, 16
                        mov              r11, 32
                        mov              eax, dword ptr [rsp + 32]            # coerce_numeric
                        mov              ecx, dword ptr [rsp + 16]
                        mov              edx, eax
                        and              edx, ecx
                        cmp              dl, 3;                               jne   .Lx163_0
                        mov              rax, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 24]
                        cmp              rax, rdx;                            jne   .Lx163_239
                        add              rsp, 16;                             jmp   n30_coerce_numeric_β
.Lx163_239:                                                                   jmp   n32_statement_end_α
.Lx163_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_cmp_d@PLT
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        test             eax, eax;                            jne   .Lx163_240
                        add              rsp, 16;                             jmp   n30_coerce_numeric_β
.Lx163_240:                                                                   jmp   n32_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n32_statement_end_α:    mov              r11, 33
                        mov              r10, 6
                        add              rsp, 96;                             jmp   n33_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1010/001: fact(5)=120'          :(END)
#-----------------------------------------------------------------------------------------------------------------------
n33_statement_begin_α:  mov              r11, 34
                        mov              r10, 7;                              jmp   n34_lit_string_α
n33_statement_begin_β:  mov              r11, 34;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n34_lit_string_α:       sub              rsp, 16
                        mov              r11, 35
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 26
                        mov              rax, qword ptr [rip + .Lx168_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n35_assign_α
.Lx168_0:               .quad            .Lx168_0_s
.Lx168_0_s:             .string          "FAIL 1010/001: fact(5)=120"
#-----------------------------------------------------------------------------------------------------------------------
n35_assign_α:           mov              r11, 36
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx169_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n36_statement_end_α
.Lx169_0:               .quad            .Lx169_0_s
.Lx169_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n36_statement_end_α:    mov              r11, 37
                        mov              r10, 7
                        add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e001  <stmt 8, line 12: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n37_statement_begin_α:  mov              r11, 38
                        mov              r10, 8;                              jmp   n38_statement_end_α
n37_statement_begin_β:  mov              r11, 38;                             jmp   n39_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n38_statement_end_α:    mov              r11, 39
                        mov              r10, 8;                              jmp   n39_statement_begin_α
#=======================================================================================================================
#         DIFFER(OPSYN(.facto, 'fact'))                   :f(e002)
#-----------------------------------------------------------------------------------------------------------------------
n39_statement_begin_α:  mov              r11, 40
                        mov              r10, 9;                              jmp   n40_lit_string_α
n39_statement_begin_β:  mov              r11, 40;                             jmp   n51_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n40_lit_string_α:       sub              rsp, 16
                        mov              r11, 41
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx178_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n41_call_α
.Lx178_0:               .quad            .Lx178_0_s
.Lx178_0_s:             .string          "facto"
#-----------------------------------------------------------------------------------------------------------------------
n41_call_α:             sub              rsp, 16
                        mov              r11, 42
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd180:            .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd180]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx179_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n39_statement_begin_β
.Lx179_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n42_lit_string_α
n41_call_β:             mov              r11, 42
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n39_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n42_lit_string_α:       sub              rsp, 16
                        mov              r11, 43
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx181_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n43_call_α
n42_lit_string_β:       mov              r11, 43
                        add              rsp, 16
                        add              rsp, 32;                             jmp   n39_statement_begin_β
.Lx181_0:               .quad            .Lx181_0_s
.Lx181_0_s:             .string          "fact"
#-----------------------------------------------------------------------------------------------------------------------
n43_call_α:             sub              rsp, 16
                        mov              r11, 44
                        sub              rsp, 32
                        mov              r8, qword ptr [rsp + 64]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 8], r8
                        mov              r8, qword ptr [rsp + 48]
                        mov              qword ptr [rsp + 16], r8
                        mov              r8, qword ptr [rsp + 56]
                        mov              qword ptr [rsp + 24], r8
                        .section         .rodata
.Lrkfnzd183:            .string          "OPSYN"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd183]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        add              rsp, 32
                        cmp              al, 104;                             jne   .Lx182_240
                        add              rsp, 16;                             jmp   n42_lit_string_β
.Lx182_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n44_lit_string_α
n43_call_β:             mov              r11, 44
                        add              rsp, 16;                             jmp   n42_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n44_lit_string_α:       sub              rsp, 16
                        mov              r11, 45
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx184_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n45_differ_α
n44_lit_string_β:       mov              r11, 45
                        add              rsp, 32;                             jmp   n42_lit_string_β
.Lx184_0:               .quad            .Lx184_0_s
.Lx184_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n45_differ_α:           sub              rsp, 16
                        mov              r11, 46
                        mov              rdi, qword ptr [rsp + 32]            # call
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_string
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             descr_identical@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        test             eax, eax;                            je    .Lx186_240
                        add              rsp, 16;                             jmp   n44_lit_string_β
.Lx186_240:                                                                   jmp   n46_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n46_statement_end_α:    mov              r11, 47
                        mov              r10, 9
                        add              rsp, 96;                             jmp   n47_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1010/002: opsyn alias'          :(END)
#-----------------------------------------------------------------------------------------------------------------------
n47_statement_begin_α:  mov              r11, 48
                        mov              r10, 10;                             jmp   n48_lit_string_α
n47_statement_begin_β:  mov              r11, 48;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n48_lit_string_α:       sub              rsp, 16
                        mov              r11, 49
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 26
                        mov              rax, qword ptr [rip + .Lx191_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n49_assign_α
.Lx191_0:               .quad            .Lx191_0_s
.Lx191_0_s:             .string          "FAIL 1010/002: opsyn alias"
#-----------------------------------------------------------------------------------------------------------------------
n49_assign_α:           mov              r11, 50
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx192_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n50_statement_end_α
.Lx192_0:               .quad            .Lx192_0_s
.Lx192_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n50_statement_end_α:    mov              r11, 51
                        mov              r10, 10
                        add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e002  <stmt 11, line 17: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n51_statement_begin_α:  mov              r11, 52
                        mov              r10, 11;                             jmp   n52_statement_end_α
n51_statement_begin_β:  mov              r11, 52;                             jmp   n53_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n52_statement_end_α:    mov              r11, 53
                        mov              r10, 11;                             jmp   n53_statement_begin_α
#=======================================================================================================================
#         NE(facto(4), 24)           :f(e003)
#-----------------------------------------------------------------------------------------------------------------------
n53_statement_begin_α:  mov              r11, 54
                        mov              r10, 12;                             jmp   n54_lit_integer_α
n53_statement_begin_β:  mov              r11, 54;                             jmp   n65_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n54_lit_integer_α:      sub              rsp, 16
                        mov              r11, 55
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx201_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n55_call_α
.Lx201_0:               .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n55_call_α:             sub              rsp, 16
                        mov              r11, 56
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 16]            # lit_integer
                        mov              rdx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_arg_stage@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdi, qword ptr [rip + .Lx203_0]
                        mov              esi, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_call_open@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        test             rax, rax;                            je    .Lx203_1
                        mov              rdi, qword ptr [rip + .Lx203_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        lea              rcx, [rip + .Lx203_4]                # s111 floater pair (ZD twin NON-SLIM fallback): THE arm GVA-off actually reaches — MONITOR_BIN forces n_gva_m3=0, the slim tail at ~:403 that s110 patched refuses, and the site falls through to rt_proc_call_open here with flat rcx/rdx wires and NO pair.  Push omega then gamma = [rsp+0]=gamma [rsp+8]=omega; the fnrbp2 floater consumes 16 so L(3)/L(4) arrive at today's depth.  SCRIP_SLIM_PAIR=0 restores prior bytes.
                        push             rcx
                        lea              rcx, [rip + .Lx203_3]
                        push             rcx
                        lea              rcx, [rip + .Lx203_3]
                        lea              rdx, [rip + .Lx203_4];               jmp   rax
.Lx203_3:               mov              rdi, qword ptr [rip + .Lx203_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_call_epilogue_named_γ@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx203_2
.Lx203_4:               mov              rdi, qword ptr [rip + .Lx203_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_call_epilogue_named_ω@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx203_2
.Lx203_1:               call             rt_faildescr@PLT
.Lx203_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx203_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx203_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx203_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n53_statement_begin_β
.Lx203_240:                                                                   jmp   n56_lit_integer_α
n55_call_β:             mov              r11, 56;                             jmp   n53_statement_begin_β
.Lx203_0:               .quad            .Lx203_0_s
.Lx203_0_s:             .string          "facto"
#-----------------------------------------------------------------------------------------------------------------------
n56_lit_integer_α:      sub              rsp, 16
                        mov              r11, 57
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx204_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n57_coerce_numeric_α
n56_lit_integer_β:      mov              r11, 57
                        add              rsp, 16
                        add              rsp, 32;                             jmp   n53_statement_begin_β
.Lx204_0:               .quad            24
#-----------------------------------------------------------------------------------------------------------------------
n57_coerce_numeric_α:   sub              rsp, 16
                        mov              r11, 58
                        mov              eax, dword ptr [rsp + 32]            # call
                        cmp              al, 5;                               je    .Lx206_1
                        cmp              al, 3;                               jne   .Lx206_0
                        mov              eax, dword ptr [rsp + 16]            # lit_integer
                        cmp              al, 3;                               jne   .Lx206_0
.Lx206_1:               mov              rax, qword ptr [rsp + 32]            # call
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # call
                        mov              qword ptr [rsp + 8], rax;            jmp   n58_coerce_numeric_α
.Lx206_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]                      # lit_integer
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 149
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n58_coerce_numeric_α
n57_coerce_numeric_β:   mov              r11, 58
                        add              rsp, 16;                             jmp   n56_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n58_coerce_numeric_α:   sub              rsp, 16
                        mov              r11, 59
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              al, 5;                               je    .Lx208_1
                        cmp              al, 3;                               jne   .Lx208_0
                        mov              eax, dword ptr [rsp + 48]            # call
                        cmp              al, 3;                               jne   .Lx208_0
.Lx208_1:               mov              rax, qword ptr [rsp + 32]            # lit_integer
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n59_cmp_test_α
.Lx208_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 48]                      # call
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 150
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n59_cmp_test_α
n58_coerce_numeric_β:   mov              r11, 59
                        add              rsp, 16;                             jmp   n57_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n59_cmp_test_α:         sub              rsp, 16
                        mov              r11, 60
                        mov              eax, dword ptr [rsp + 32]            # coerce_numeric
                        mov              ecx, dword ptr [rsp + 16]
                        mov              edx, eax
                        and              edx, ecx
                        cmp              dl, 3;                               jne   .Lx210_0
                        mov              rax, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 24]
                        cmp              rax, rdx;                            jne   .Lx210_239
                        add              rsp, 16;                             jmp   n58_coerce_numeric_β
.Lx210_239:                                                                   jmp   n60_statement_end_α
.Lx210_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_cmp_d@PLT
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        test             eax, eax;                            jne   .Lx210_240
                        add              rsp, 16;                             jmp   n58_coerce_numeric_β
.Lx210_240:                                                                   jmp   n60_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n60_statement_end_α:    mov              r11, 61
                        mov              r10, 12
                        add              rsp, 96;                             jmp   n61_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1010/003: facto(4)=24 via alias' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n61_statement_begin_α:  mov              r11, 62
                        mov              r10, 13;                             jmp   n62_lit_string_α
n61_statement_begin_β:  mov              r11, 62;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n62_lit_string_α:       sub              rsp, 16
                        mov              r11, 63
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 36
                        mov              rax, qword ptr [rip + .Lx215_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n63_assign_α
.Lx215_0:               .quad            .Lx215_0_s
.Lx215_0_s:             .string          "FAIL 1010/003: facto(4)=24 via alias"
#-----------------------------------------------------------------------------------------------------------------------
n63_assign_α:           mov              r11, 64
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx216_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n64_statement_end_α
.Lx216_0:               .quad            .Lx216_0_s
.Lx216_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n64_statement_end_α:    mov              r11, 65
                        mov              r10, 13
                        add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e003  <stmt 14, line 20: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n65_statement_begin_α:  mov              r11, 66
                        mov              r10, 14;                             jmp   n66_statement_end_α
n65_statement_begin_β:  mov              r11, 66;                             jmp   n67_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n66_statement_end_α:    mov              r11, 67
                        mov              r10, 14;                             jmp   n67_statement_begin_α
#=======================================================================================================================
#         DEFINE('fact2(n)', .fact2_entry)               :(fact2_end)
#-----------------------------------------------------------------------------------------------------------------------
n67_statement_begin_α:  mov              r11, 68
                        mov              r10, 15;                             jmp   n68_define_α
n67_statement_begin_β:  mov              r11, 68;                             jmp   n90_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n68_define_α:           mov              r11, 69
                        mov              rdi, qword ptr [rip + .Lx226_0]
                        mov              rsi, qword ptr [rip + .Lx226_1]
                        mov              edx, 1
                        mov              ecx, 1
                        mov              r8d, 0
                        lea              r9, [rip + n70_statement_begin_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n69_statement_end_α
n68_define_β:           mov              r11, 69;                             jmp   n67_statement_begin_β
.Lx226_0:               .quad            .Lx226_0_s
.Lx226_0_s:             .string          "fact2"
.Lx226_1:               .quad            .Lx226_1_s
.Lx226_1_s:             .string          "n"
#-----------------------------------------------------------------------------------------------------------------------
n69_statement_end_α:    mov              r11, 70
                        mov              r10, 15;                             jmp   n90_statement_begin_α
#=======================================================================================================================
# fact2_entry  <stmt 16, line 24: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n70_statement_begin_α:  mov              r11, 71
                        mov              r10, 16;                             jmp   n71_statement_end_α
n70_statement_begin_β:  mov              r11, 71;                             jmp   n72_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n71_statement_end_α:    mov              r11, 72
                        mov              r10, 16;                             jmp   n72_statement_begin_α
#=======================================================================================================================
#         fact2 = EQ(n, 1) 1                             :s(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n72_statement_begin_α:  mov              r11, 73
                        mov              r10, 17;                             jmp   n73_var_α
n72_statement_begin_β:  mov              r11, 73;                             jmp   n81_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n73_var_α:              sub              rsp, 16
                        mov              r11, 74
                        mov              rax, qword ptr [r9 + 16]             # n
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n74_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n74_lit_integer_α:      sub              rsp, 16
                        mov              r11, 75
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx237_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n75_coerce_numeric_α
n74_lit_integer_β:      mov              r11, 75
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n72_statement_begin_β
.Lx237_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n75_coerce_numeric_α:   sub              rsp, 16
                        mov              r11, 76
                        mov              eax, dword ptr [rsp + 32]            # var
                        cmp              al, 5;                               je    .Lx239_1
                        cmp              al, 3;                               jne   .Lx239_0
                        mov              eax, dword ptr [rsp + 16]            # lit_integer
                        cmp              al, 3;                               jne   .Lx239_0
.Lx239_1:               mov              rax, qword ptr [rsp + 32]            # var
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # var
                        mov              qword ptr [rsp + 8], rax;            jmp   n76_coerce_numeric_α
.Lx239_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]                      # lit_integer
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 101
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n76_coerce_numeric_α
n75_coerce_numeric_β:   mov              r11, 76
                        add              rsp, 16;                             jmp   n74_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n76_coerce_numeric_α:   sub              rsp, 16
                        mov              r11, 77
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              al, 5;                               je    .Lx241_1
                        cmp              al, 3;                               jne   .Lx241_0
                        mov              eax, dword ptr [rsp + 48]            # var
                        cmp              al, 3;                               jne   .Lx241_0
.Lx241_1:               mov              rax, qword ptr [rsp + 32]            # lit_integer
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n77_cmp_test_α
.Lx241_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 48]                      # var
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 102
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n77_cmp_test_α
n76_coerce_numeric_β:   mov              r11, 77
                        add              rsp, 16;                             jmp   n75_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n77_cmp_test_α:         sub              rsp, 16
                        mov              r11, 78
                        mov              eax, dword ptr [rsp + 32]            # coerce_numeric
                        mov              ecx, dword ptr [rsp + 16]
                        mov              edx, eax
                        and              edx, ecx
                        cmp              dl, 3;                               jne   .Lx243_0
                        mov              rax, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 24]
                        cmp              rax, rdx;                            je    .Lx243_239
                        add              rsp, 16;                             jmp   n76_coerce_numeric_β
.Lx243_239:                                                                   jmp   n78_lit_integer_α
.Lx243_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_cmp_d@PLT
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        test             eax, eax;                            je    .Lx243_240
                        add              rsp, 16;                             jmp   n76_coerce_numeric_β
.Lx243_240:                                                                   jmp   n78_lit_integer_α
n77_cmp_test_β:         mov              r11, 78
                        add              rsp, 16;                             jmp   n76_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n78_lit_integer_α:      sub              rsp, 16
                        mov              r11, 79
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx244_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n79_assign_α
.Lx244_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n79_assign_α:           mov              r11, 80
                        mov              rax, qword ptr [rsp + 0]             # lit_integer
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 32], rax             # fact2
                        mov              qword ptr [r9 + 40], rdx;            jmp   n80_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n80_statement_end_α:    mov              r11, 81
                        mov              r10, 17
                        add              rsp, 96;                             jmp   RETURN
#=======================================================================================================================
#         fact2 = n * fact2(n - 1)                       :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n81_statement_begin_α:  mov              r11, 82
                        mov              r10, 18;                             jmp   n82_var_α
n81_statement_begin_β:  mov              r11, 82;                             jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n82_var_α:              sub              rsp, 16
                        mov              r11, 83
                        mov              rax, qword ptr [r9 + 16]             # n
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n83_var_α
#-----------------------------------------------------------------------------------------------------------------------
n83_var_α:              sub              rsp, 16
                        mov              r11, 84
                        mov              rax, qword ptr [r9 + 16]             # n
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n84_lit_integer_α
n83_var_β:              mov              r11, 84
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n81_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n84_lit_integer_α:      sub              rsp, 16
                        mov              r11, 85
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx252_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n85_binop_α
n84_lit_integer_β:      mov              r11, 85
                        add              rsp, 16;                             jmp   n83_var_β
.Lx252_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n85_binop_α:            sub              rsp, 16
                        mov              r11, 86
                        mov              ecx, dword ptr [rsp + 32]            # var
                        mov              rax, qword ptr [rsp + 40]
                        cmp              cl, 3;                               jne   .Lx253_2
                        sub              rax, 1
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              qword ptr [rsp + 8], rax;            jmp   .Lx253_7
.Lx253_2:               mov              eax, ecx
                        mov              edx, ecx
                        and              edx, 1;                              jz    .Lx253_0
                        mov              rsi, qword ptr [rsp + 40]            # var
                        mov              rdi, 1
                        cmp              al, 5;                               je    .Lx253_3
                        cvtsi2sd         xmm0, rsi;                           jmp   .Lx253_4
.Lx253_3:               movq             xmm0, rsi
.Lx253_4:               cvtsi2sd         xmm1, rdi
                        subsd            xmm0, xmm1
                        movq             rax, xmm0
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              qword ptr [rsp + 8], rax
.Lx253_7:                                                                     jmp   n86_call_α
.Lx253_0:               mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_sub@PLT
                        cmp              al, 104;                             jne   .Lx253_240
                        add              rsp, 16;                             jmp   n84_lit_integer_β
.Lx253_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n86_call_α
n85_binop_β:            mov              r11, 86
                        add              rsp, 16;                             jmp   n84_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n86_call_α:             sub              rsp, 16
                        mov              r11, 87
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 16]            # binop
                        mov              rdx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_arg_stage@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdi, qword ptr [rip + .Lx255_0]
                        mov              esi, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_call_open@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        test             rax, rax;                            je    .Lx255_1
                        mov              rdi, qword ptr [rip + .Lx255_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        lea              rcx, [rip + .Lx255_4]                # s111 floater pair (ZD twin NON-SLIM fallback): THE arm GVA-off actually reaches — MONITOR_BIN forces n_gva_m3=0, the slim tail at ~:403 that s110 patched refuses, and the site falls through to rt_proc_call_open here with flat rcx/rdx wires and NO pair.  Push omega then gamma = [rsp+0]=gamma [rsp+8]=omega; the fnrbp2 floater consumes 16 so L(3)/L(4) arrive at today's depth.  SCRIP_SLIM_PAIR=0 restores prior bytes.
                        push             rcx
                        lea              rcx, [rip + .Lx255_3]
                        push             rcx
                        lea              rcx, [rip + .Lx255_3]
                        lea              rdx, [rip + .Lx255_4];               jmp   rax
.Lx255_3:               mov              rdi, qword ptr [rip + .Lx255_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_call_epilogue_named_γ@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx255_2
.Lx255_4:               mov              rdi, qword ptr [rip + .Lx255_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_call_epilogue_named_ω@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx255_2
.Lx255_1:               call             rt_faildescr@PLT
.Lx255_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx255_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx255_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx255_240
                        add              rsp, 16;                             jmp   n85_binop_β
.Lx255_240:                                                                   jmp   n87_binop_α
n86_call_β:             mov              r11, 87;                             jmp   n85_binop_β
.Lx255_0:               .quad            .Lx255_0_s
.Lx255_0_s:             .string          "fact2"
#-----------------------------------------------------------------------------------------------------------------------
n87_binop_α:            sub              rsp, 16
                        mov              r11, 88
                        mov              eax, dword ptr [rsp + 80]            # var
                        mov              ecx, dword ptr [rsp + 16]            # call
                        mov              edx, eax
                        and              edx, ecx
                        cmp              dl, 3;                               jne   .Lx256_2
                        mov              rax, qword ptr [rsp + 88]            # var
                        mov              rdx, qword ptr [rsp + 24]            # call
                        imul             rax, rdx
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              qword ptr [rsp + 8], rax;            jmp   .Lx256_7
.Lx256_2:               and              edx, 1;                              jz    .Lx256_0
                        mov              rsi, qword ptr [rsp + 88]            # var
                        mov              rdi, qword ptr [rsp + 24]            # call
                        cmp              al, 5;                               je    .Lx256_3
                        cvtsi2sd         xmm0, rsi;                           jmp   .Lx256_4
.Lx256_3:               movq             xmm0, rsi
.Lx256_4:               cmp              cl, 5;                               je    .Lx256_5
                        cvtsi2sd         xmm1, rdi;                           jmp   .Lx256_6
.Lx256_5:               movq             xmm1, rdi
.Lx256_6:               mulsd            xmm0, xmm1
                        movq             rax, xmm0
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              qword ptr [rsp + 8], rax
.Lx256_7:                                                                     jmp   n88_assign_α
.Lx256_0:               mov              rdi, qword ptr [rsp + 80]            # var
                        mov              rsi, qword ptr [rsp + 88]
                        mov              rdx, qword ptr [rsp + 16]            # call
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_mul@PLT
                        cmp              al, 104;                             jne   .Lx256_240
                        add              rsp, 32;                             jmp   n85_binop_β
.Lx256_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n88_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n88_assign_α:           mov              r11, 89
                        mov              rax, qword ptr [rsp + 0]             # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 32], rax             # fact2
                        mov              qword ptr [r9 + 40], rdx;            jmp   n89_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n89_statement_end_α:    mov              r11, 90
                        mov              r10, 18
                        add              rsp, 96;                             jmp   RETURN
#=======================================================================================================================
# fact2_end  <stmt 19, line 27: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n90_statement_begin_α:  mov              r11, 91
                        mov              r10, 19;                             jmp   n91_statement_end_α
n90_statement_begin_β:  mov              r11, 91;                             jmp   n92_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n91_statement_end_α:    mov              r11, 92
                        mov              r10, 19;                             jmp   n92_statement_begin_α
#=======================================================================================================================
#         NE(fact2(6), 720)          :f(e004)
#-----------------------------------------------------------------------------------------------------------------------
n92_statement_begin_α:  mov              r11, 93
                        mov              r10, 20;                             jmp   n93_lit_integer_α
n92_statement_begin_β:  mov              r11, 93;                             jmp   n104_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n93_lit_integer_α:      sub              rsp, 16
                        mov              r11, 94
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx266_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n94_call_α
.Lx266_0:               .quad            6
#-----------------------------------------------------------------------------------------------------------------------
n94_call_α:             sub              rsp, 16
                        mov              r11, 95
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 16]            # lit_integer
                        mov              rdx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_arg_stage@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rdi, qword ptr [rip + .Lx268_0]
                        mov              esi, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_call_open@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        test             rax, rax;                            je    .Lx268_1
                        mov              rdi, qword ptr [rip + .Lx268_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        lea              rcx, [rip + .Lx268_4]                # s111 floater pair (ZD twin NON-SLIM fallback): THE arm GVA-off actually reaches — MONITOR_BIN forces n_gva_m3=0, the slim tail at ~:403 that s110 patched refuses, and the site falls through to rt_proc_call_open here with flat rcx/rdx wires and NO pair.  Push omega then gamma = [rsp+0]=gamma [rsp+8]=omega; the fnrbp2 floater consumes 16 so L(3)/L(4) arrive at today's depth.  SCRIP_SLIM_PAIR=0 restores prior bytes.
                        push             rcx
                        lea              rcx, [rip + .Lx268_3]
                        push             rcx
                        lea              rcx, [rip + .Lx268_3]
                        lea              rdx, [rip + .Lx268_4];               jmp   rax
.Lx268_3:               mov              rdi, qword ptr [rip + .Lx268_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_call_epilogue_named_γ@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx268_2
.Lx268_4:               mov              rdi, qword ptr [rip + .Lx268_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_proc_call_epilogue_named_ω@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   .Lx268_2
.Lx268_1:               call             rt_faildescr@PLT
.Lx268_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx268_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx268_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx268_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n92_statement_begin_β
.Lx268_240:                                                                   jmp   n95_lit_integer_α
n94_call_β:             mov              r11, 95;                             jmp   n92_statement_begin_β
.Lx268_0:               .quad            .Lx268_0_s
.Lx268_0_s:             .string          "fact2"
#-----------------------------------------------------------------------------------------------------------------------
n95_lit_integer_α:      sub              rsp, 16
                        mov              r11, 96
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx269_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n96_coerce_numeric_α
n95_lit_integer_β:      mov              r11, 96
                        add              rsp, 16
                        add              rsp, 32;                             jmp   n92_statement_begin_β
.Lx269_0:               .quad            720
#-----------------------------------------------------------------------------------------------------------------------
n96_coerce_numeric_α:   sub              rsp, 16
                        mov              r11, 97
                        mov              eax, dword ptr [rsp + 32]            # call
                        cmp              al, 5;                               je    .Lx271_1
                        cmp              al, 3;                               jne   .Lx271_0
                        mov              eax, dword ptr [rsp + 16]            # lit_integer
                        cmp              al, 3;                               jne   .Lx271_0
.Lx271_1:               mov              rax, qword ptr [rsp + 32]            # call
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # call
                        mov              qword ptr [rsp + 8], rax;            jmp   n97_coerce_numeric_α
.Lx271_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]                      # lit_integer
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 149
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n97_coerce_numeric_α
n96_coerce_numeric_β:   mov              r11, 97
                        add              rsp, 16;                             jmp   n95_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n97_coerce_numeric_α:   sub              rsp, 16
                        mov              r11, 98
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              al, 5;                               je    .Lx273_1
                        cmp              al, 3;                               jne   .Lx273_0
                        mov              eax, dword ptr [rsp + 48]            # call
                        cmp              al, 3;                               jne   .Lx273_0
.Lx273_1:               mov              rax, qword ptr [rsp + 32]            # lit_integer
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n98_cmp_test_α
.Lx273_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 48]                      # call
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 150
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n98_cmp_test_α
n97_coerce_numeric_β:   mov              r11, 98
                        add              rsp, 16;                             jmp   n96_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n98_cmp_test_α:         sub              rsp, 16
                        mov              r11, 99
                        mov              eax, dword ptr [rsp + 32]            # coerce_numeric
                        mov              ecx, dword ptr [rsp + 16]
                        mov              edx, eax
                        and              edx, ecx
                        cmp              dl, 3;                               jne   .Lx275_0
                        mov              rax, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 24]
                        cmp              rax, rdx;                            jne   .Lx275_239
                        add              rsp, 16;                             jmp   n97_coerce_numeric_β
.Lx275_239:                                                                   jmp   n99_statement_end_α
.Lx275_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             rt_cmp_d@PLT
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64]
                        test             eax, eax;                            jne   .Lx275_240
                        add              rsp, 16;                             jmp   n97_coerce_numeric_β
.Lx275_240:                                                                   jmp   n99_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n99_statement_end_α:    mov              r11, 100
                        mov              r10, 20
                        add              rsp, 96;                             jmp   n100_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1010/004: fact2(6)=720 alt entry' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n100_statement_begin_α: mov              r11, 101
                        mov              r10, 21;                             jmp   n101_lit_string_α
n100_statement_begin_β: mov              r11, 101;                            jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n101_lit_string_α:      sub              rsp, 16
                        mov              r11, 102
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 37
                        mov              rax, qword ptr [rip + .Lx280_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n102_assign_α
.Lx280_0:               .quad            .Lx280_0_s
.Lx280_0_s:             .string          "FAIL 1010/004: fact2(6)=720 alt entry"
#-----------------------------------------------------------------------------------------------------------------------
n102_assign_α:          mov              r11, 103
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx281_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n103_statement_end_α
.Lx281_0:               .quad            .Lx281_0_s
.Lx281_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n103_statement_end_α:   mov              r11, 104
                        mov              r10, 21
                        add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e004  <stmt 22, line 30: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n104_statement_begin_α: mov              r11, 105
                        mov              r10, 22;                             jmp   n105_statement_end_α
n104_statement_begin_β: mov              r11, 105;                            jmp   n106_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n105_statement_end_α:   mov              r11, 106
                        mov              r10, 22;                             jmp   n106_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'PASS 1010_func_recursion (4/4)'
#-----------------------------------------------------------------------------------------------------------------------
n106_statement_begin_α: mov              r11, 107
                        mov              r10, 23;                             jmp   n107_lit_string_α
n106_statement_begin_β: mov              r11, 107;                            jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n107_lit_string_α:      sub              rsp, 16
                        mov              r11, 108
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 30
                        mov              rax, qword ptr [rip + .Lx290_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n108_assign_α
.Lx290_0:               .quad            .Lx290_0_s
.Lx290_0_s:             .string          "PASS 1010_func_recursion (4/4)"
#-----------------------------------------------------------------------------------------------------------------------
n108_assign_α:          mov              r11, 109
                        mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx291_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        mov              qword ptr [rip + rtccb+56], r10
                        mov              qword ptr [rip + rtccb+64], r11
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              r10, qword ptr [rip + rtccb+56]
                        mov              r11, qword ptr [rip + rtccb+64];     jmp   n109_statement_end_α
.Lx291_0:               .quad            .Lx291_0_s
.Lx291_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n109_statement_end_α:   mov              r11, 110
                        mov              r10, 23
                        add              rsp, 16;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
RETURN:                 mov              r11, 111
                        pop              rcx
                        add              rsp, 8;                              jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                              jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
                        xor              edi, edi
                        call             exit@PLT
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              edi, 1
                        call             exit@PLT
module_init:
                        sub              rsp, 8
                        .section         .rodata
.Lseala2:               .string          "fact"
                        .section         .text
                        .intel_syntax    noprefix
                        .weak            fact_α
                        lea              rdi, [rip + .Lseala2]
                        mov              rsi, qword ptr [rip + fact_α@GOTPCREL]
                        call             rt_proc_seal_alpha@PLT
                        .section         .rodata
.Lseala3:               .string          "facto"
                        .section         .text
                        .intel_syntax    noprefix
                        .weak            facto_α
                        lea              rdi, [rip + .Lseala3]
                        mov              rsi, qword ptr [rip + facto_α@GOTPCREL]
                        call             rt_proc_seal_alpha@PLT
                        .section         .rodata
.Lseala4:               .string          "fact2"
                        .section         .text
                        .intel_syntax    noprefix
                        .weak            fact2_α
                        lea              rdi, [rip + .Lseala4]
                        mov              rsi, qword ptr [rip + fact2_α@GOTPCREL]
                        call             rt_proc_seal_alpha@PLT
                        add              rsp, 8
                        ret
                        .section         .note.GNU-stack,"",@progbits
