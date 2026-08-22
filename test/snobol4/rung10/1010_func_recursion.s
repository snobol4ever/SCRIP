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
n0_statement_begin_α:                                                         jmp   n1_statement_end_α
n0_statement_begin_β:                                                         jmp   n2_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n1_statement_end_α:                                                           jmp   n2_statement_begin_α
#=======================================================================================================================
#         DEFINE('fact(n)')                              :(fact_end)
#-----------------------------------------------------------------------------------------------------------------------
n2_statement_begin_α:                                                         jmp   n3_define_α
n2_statement_begin_β:                                                         jmp   n23_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n3_define_α:            mov              rdi, qword ptr [rip + .Lx117_0]
                        mov              rsi, qword ptr [rip + .Lx117_1]
                        mov              edx, 1
                        mov              ecx, 1
                        mov              r8d, 0
                        lea              r9, [rip + n5_statement_begin_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n4_statement_end_α
n3_define_β:                                                                  jmp   n2_statement_begin_β
.Lx117_0:               .quad            .Lx117_0_s
.Lx117_0_s:             .string          "fact"
.Lx117_1:               .quad            .Lx117_1_s
.Lx117_1_s:             .string          "n"
#-----------------------------------------------------------------------------------------------------------------------
n4_statement_end_α:                                                           jmp   n23_statement_begin_α
#=======================================================================================================================
# fact    fact = EQ(n, 1) 1                              :s(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n5_statement_begin_α:                                                         jmp   n6_var_α
n5_statement_begin_β:                                                         jmp   n14_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n6_var_α:               sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]             # n
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n7_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n7_lit_integer_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx124_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n8_coerce_numeric_α
n7_lit_integer_β:       add              rsp, 16
                        add              rsp, 16;                             jmp   n5_statement_begin_β
.Lx124_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n8_coerce_numeric_α:    sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]            # var
                        cmp              al, 5;                               je    .Lx126_1
                        cmp              al, 3;                               jne   .Lx126_0
                        mov              eax, dword ptr [rsp + 16]            # lit_integer
                        cmp              al, 3;                               jne   .Lx126_0
.Lx126_1:               mov              rax, qword ptr [rsp + 32]            # var
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # var
                        mov              qword ptr [rsp + 8], rax;            jmp   n9_coerce_numeric_α
.Lx126_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]                      # lit_integer
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 101
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n9_coerce_numeric_α
n8_coerce_numeric_β:    add              rsp, 16;                             jmp   n7_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n9_coerce_numeric_α:    sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              al, 5;                               je    .Lx128_1
                        cmp              al, 3;                               jne   .Lx128_0
                        mov              eax, dword ptr [rsp + 48]            # var
                        cmp              al, 3;                               jne   .Lx128_0
.Lx128_1:               mov              rax, qword ptr [rsp + 32]            # lit_integer
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n10_cmp_test_α
.Lx128_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 48]                      # var
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 102
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n10_cmp_test_α
n9_coerce_numeric_β:    add              rsp, 16;                             jmp   n8_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n10_cmp_test_α:         sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]            # coerce_numeric
                        mov              ecx, dword ptr [rsp + 16]
                        mov              edx, eax
                        and              edx, ecx
                        cmp              dl, 3;                               jne   .Lx130_0
                        mov              rax, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 24]
                        cmp              rax, rdx;                            je    .Lx130_239
                        add              rsp, 16;                             jmp   n9_coerce_numeric_β
.Lx130_239:                                                                   jmp   n11_lit_integer_α
.Lx130_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_cmp_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        test             eax, eax;                            je    .Lx130_240
                        add              rsp, 16;                             jmp   n9_coerce_numeric_β
.Lx130_240:                                                                   jmp   n11_lit_integer_α
n10_cmp_test_β:         add              rsp, 16;                             jmp   n9_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n11_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx131_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n12_assign_α
.Lx131_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n12_assign_α:           mov              rax, qword ptr [rsp + 0]             # lit_integer
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax              # fact
                        mov              qword ptr [r9 + 8], rdx;             jmp   n13_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n13_statement_end_α:    add              rsp, 96;                             jmp   RETURN
#=======================================================================================================================
#         fact = n * fact(n - 1)                         :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n14_statement_begin_α:                                                        jmp   n15_var_α
n14_statement_begin_β:                                                        jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n15_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]             # n
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n16_var_α
#-----------------------------------------------------------------------------------------------------------------------
n16_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]             # n
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n17_lit_integer_α
n16_var_β:              add              rsp, 16
                        add              rsp, 16;                             jmp   n14_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n17_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx139_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n18_binop_α
n17_lit_integer_β:      add              rsp, 16;                             jmp   n16_var_β
.Lx139_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n18_binop_α:            sub              rsp, 16
                        mov              ecx, dword ptr [rsp + 32]            # var
                        mov              rax, qword ptr [rsp + 40]
                        cmp              cl, 3;                               jne   .Lx140_2
                        sub              rax, 1
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              qword ptr [rsp + 8], rax;            jmp   .Lx140_7
.Lx140_2:               mov              eax, ecx
                        mov              edx, ecx
                        and              edx, 1;                              jz    .Lx140_0
                        mov              rsi, qword ptr [rsp + 40]            # var
                        mov              rdi, 1
                        cmp              al, 5;                               je    .Lx140_3
                        cvtsi2sd         xmm0, rsi;                           jmp   .Lx140_4
.Lx140_3:               movq             xmm0, rsi
.Lx140_4:               cvtsi2sd         xmm1, rdi
                        subsd            xmm0, xmm1
                        movq             rax, xmm0
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              qword ptr [rsp + 8], rax
.Lx140_7:                                                                     jmp   n19_call_α
.Lx140_0:               mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_sub@PLT
                        cmp              al, 104;                             jne   .Lx140_240
                        add              rsp, 16;                             jmp   n17_lit_integer_β
.Lx140_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n19_call_α
n18_binop_β:            add              rsp, 16;                             jmp   n17_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n19_call_α:             sub              rsp, 16
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 16]            # binop
                        mov              rdx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_arg_stage@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdi, qword ptr [rip + .Lx142_0]
                        mov              esi, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_proc_call_open@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        test             rax, rax;                            je    .Lx142_1
                        mov              rdi, qword ptr [rip + .Lx142_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_proc_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        lea              rcx, [rip + .Lx142_4]                # s111 floater pair (ZD twin NON-SLIM fallback): THE arm GVA-off actually reaches — MONITOR_BIN forces n_gva_m3=0, the slim tail at ~:403 that s110 patched refuses, and the site falls through to rt_proc_call_open here with flat rcx/rdx wires and NO pair.  Push omega then gamma = [rsp+0]=gamma [rsp+8]=omega; the fnrbp2 floater consumes 16 so L(3)/L(4) arrive at today's depth.  SCRIP_SLIM_PAIR=0 restores prior bytes.
                        push             rcx
                        lea              rcx, [rip + .Lx142_3]
                        push             rcx
                        lea              rcx, [rip + .Lx142_3]
                        lea              rdx, [rip + .Lx142_4];               jmp   rax
.Lx142_3:               mov              rdi, qword ptr [rip + .Lx142_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_proc_call_epilogue_named_γ@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   .Lx142_2
.Lx142_4:               mov              rdi, qword ptr [rip + .Lx142_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_proc_call_epilogue_named_ω@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   .Lx142_2
.Lx142_1:               call             rt_faildescr@PLT
.Lx142_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx142_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx142_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx142_240
                        add              rsp, 16;                             jmp   n18_binop_β
.Lx142_240:                                                                   jmp   n20_binop_α
n19_call_β:                                                                   jmp   n18_binop_β
.Lx142_0:               .quad            .Lx142_0_s
.Lx142_0_s:             .string          "fact"
#-----------------------------------------------------------------------------------------------------------------------
n20_binop_α:            sub              rsp, 16
                        mov              eax, dword ptr [rsp + 80]            # var
                        mov              ecx, dword ptr [rsp + 16]            # call
                        mov              edx, eax
                        and              edx, ecx
                        cmp              dl, 3;                               jne   .Lx143_2
                        mov              rax, qword ptr [rsp + 88]            # var
                        mov              rdx, qword ptr [rsp + 24]            # call
                        imul             rax, rdx
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              qword ptr [rsp + 8], rax;            jmp   .Lx143_7
.Lx143_2:               and              edx, 1;                              jz    .Lx143_0
                        mov              rsi, qword ptr [rsp + 88]            # var
                        mov              rdi, qword ptr [rsp + 24]            # call
                        cmp              al, 5;                               je    .Lx143_3
                        cvtsi2sd         xmm0, rsi;                           jmp   .Lx143_4
.Lx143_3:               movq             xmm0, rsi
.Lx143_4:               cmp              cl, 5;                               je    .Lx143_5
                        cvtsi2sd         xmm1, rdi;                           jmp   .Lx143_6
.Lx143_5:               movq             xmm1, rdi
.Lx143_6:               mulsd            xmm0, xmm1
                        movq             rax, xmm0
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              qword ptr [rsp + 8], rax
.Lx143_7:                                                                     jmp   n21_assign_α
.Lx143_0:               mov              rdi, qword ptr [rsp + 80]            # var
                        mov              rsi, qword ptr [rsp + 88]
                        mov              rdx, qword ptr [rsp + 16]            # call
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_mul@PLT
                        cmp              al, 104;                             jne   .Lx143_240
                        add              rsp, 32;                             jmp   n18_binop_β
.Lx143_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n21_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n21_assign_α:           mov              rax, qword ptr [rsp + 0]             # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax              # fact
                        mov              qword ptr [r9 + 8], rdx;             jmp   n22_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n22_statement_end_α:    add              rsp, 96;                             jmp   RETURN
#=======================================================================================================================
# fact_end  <stmt 5, line 8: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n23_statement_begin_α:                                                        jmp   n24_statement_end_α
n23_statement_begin_β:                                                        jmp   n25_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n24_statement_end_α:                                                          jmp   n25_statement_begin_α
#=======================================================================================================================
#         NE(fact(5), 120)           :f(e001)
#-----------------------------------------------------------------------------------------------------------------------
n25_statement_begin_α:                                                        jmp   n26_lit_integer_α
n25_statement_begin_β:                                                        jmp   n37_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n26_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx153_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n27_call_α
.Lx153_0:               .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n27_call_α:             sub              rsp, 16
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 16]            # lit_integer
                        mov              rdx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_arg_stage@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdi, qword ptr [rip + .Lx155_0]
                        mov              esi, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_proc_call_open@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        test             rax, rax;                            je    .Lx155_1
                        mov              rdi, qword ptr [rip + .Lx155_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_proc_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        lea              rcx, [rip + .Lx155_4]                # s111 floater pair (ZD twin NON-SLIM fallback): THE arm GVA-off actually reaches — MONITOR_BIN forces n_gva_m3=0, the slim tail at ~:403 that s110 patched refuses, and the site falls through to rt_proc_call_open here with flat rcx/rdx wires and NO pair.  Push omega then gamma = [rsp+0]=gamma [rsp+8]=omega; the fnrbp2 floater consumes 16 so L(3)/L(4) arrive at today's depth.  SCRIP_SLIM_PAIR=0 restores prior bytes.
                        push             rcx
                        lea              rcx, [rip + .Lx155_3]
                        push             rcx
                        lea              rcx, [rip + .Lx155_3]
                        lea              rdx, [rip + .Lx155_4];               jmp   rax
.Lx155_3:               mov              rdi, qword ptr [rip + .Lx155_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_proc_call_epilogue_named_γ@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   .Lx155_2
.Lx155_4:               mov              rdi, qword ptr [rip + .Lx155_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_proc_call_epilogue_named_ω@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   .Lx155_2
.Lx155_1:               call             rt_faildescr@PLT
.Lx155_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx155_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx155_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx155_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n25_statement_begin_β
.Lx155_240:                                                                   jmp   n28_lit_integer_α
n27_call_β:                                                                   jmp   n25_statement_begin_β
.Lx155_0:               .quad            .Lx155_0_s
.Lx155_0_s:             .string          "fact"
#-----------------------------------------------------------------------------------------------------------------------
n28_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx156_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n29_coerce_numeric_α
n28_lit_integer_β:      add              rsp, 16
                        add              rsp, 32;                             jmp   n25_statement_begin_β
.Lx156_0:               .quad            120
#-----------------------------------------------------------------------------------------------------------------------
n29_coerce_numeric_α:   sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]            # call
                        cmp              al, 5;                               je    .Lx158_1
                        cmp              al, 3;                               jne   .Lx158_0
                        mov              eax, dword ptr [rsp + 16]            # lit_integer
                        cmp              al, 3;                               jne   .Lx158_0
.Lx158_1:               mov              rax, qword ptr [rsp + 32]            # call
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # call
                        mov              qword ptr [rsp + 8], rax;            jmp   n30_coerce_numeric_α
.Lx158_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]                      # lit_integer
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 149
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n30_coerce_numeric_α
n29_coerce_numeric_β:   add              rsp, 16;                             jmp   n28_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n30_coerce_numeric_α:   sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              al, 5;                               je    .Lx160_1
                        cmp              al, 3;                               jne   .Lx160_0
                        mov              eax, dword ptr [rsp + 48]            # call
                        cmp              al, 3;                               jne   .Lx160_0
.Lx160_1:               mov              rax, qword ptr [rsp + 32]            # lit_integer
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n31_cmp_test_α
.Lx160_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 48]                      # call
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 150
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n31_cmp_test_α
n30_coerce_numeric_β:   add              rsp, 16;                             jmp   n29_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n31_cmp_test_α:         sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]            # coerce_numeric
                        mov              ecx, dword ptr [rsp + 16]
                        mov              edx, eax
                        and              edx, ecx
                        cmp              dl, 3;                               jne   .Lx162_0
                        mov              rax, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 24]
                        cmp              rax, rdx;                            jne   .Lx162_239
                        add              rsp, 16;                             jmp   n30_coerce_numeric_β
.Lx162_239:                                                                   jmp   n32_statement_end_α
.Lx162_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_cmp_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        test             eax, eax;                            jne   .Lx162_240
                        add              rsp, 16;                             jmp   n30_coerce_numeric_β
.Lx162_240:                                                                   jmp   n32_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n32_statement_end_α:    add              rsp, 96;                             jmp   n33_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1010/001: fact(5)=120'          :(END)
#-----------------------------------------------------------------------------------------------------------------------
n33_statement_begin_α:                                                        jmp   n34_lit_string_α
n33_statement_begin_β:                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n34_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 26
                        mov              rax, qword ptr [rip + .Lx167_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n35_assign_α
.Lx167_0:               .quad            .Lx167_0_s
.Lx167_0_s:             .string          "FAIL 1010/001: fact(5)=120"
#-----------------------------------------------------------------------------------------------------------------------
n35_assign_α:           mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx168_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n36_statement_end_α
.Lx168_0:               .quad            .Lx168_0_s
.Lx168_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n36_statement_end_α:    add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e001  <stmt 8, line 12: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n37_statement_begin_α:                                                        jmp   n38_statement_end_α
n37_statement_begin_β:                                                        jmp   n39_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n38_statement_end_α:                                                          jmp   n39_statement_begin_α
#=======================================================================================================================
#         DIFFER(OPSYN(.facto, 'fact'))                   :f(e002)
#-----------------------------------------------------------------------------------------------------------------------
n39_statement_begin_α:                                                        jmp   n40_lit_string_α
n39_statement_begin_β:                                                        jmp   n50_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n40_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 5
                        mov              rax, qword ptr [rip + .Lx177_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n41_call_α
.Lx177_0:               .quad            .Lx177_0_s
.Lx177_0_s:             .string          "facto"
#-----------------------------------------------------------------------------------------------------------------------
n41_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd179:            .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd179]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx178_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n39_statement_begin_β
.Lx178_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n42_lit_string_α
n41_call_β:             add              rsp, 16
                        add              rsp, 16;                             jmp   n39_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n42_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 4
                        mov              rax, qword ptr [rip + .Lx180_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n43_call_α
n42_lit_string_β:       add              rsp, 16
                        add              rsp, 32;                             jmp   n39_statement_begin_β
.Lx180_0:               .quad            .Lx180_0_s
.Lx180_0_s:             .string          "fact"
#-----------------------------------------------------------------------------------------------------------------------
n43_call_α:             sub              rsp, 16
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
.Lrkfnzd182:            .string          "OPSYN"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd182]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 32
                        cmp              al, 104;                             jne   .Lx181_240
                        add              rsp, 16;                             jmp   n42_lit_string_β
.Lx181_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n44_call_α
n43_call_β:             add              rsp, 16;                             jmp   n42_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n44_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd184:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd184]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_call_arr@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        add              rsp, 16
                        cmp              al, 104;                             jne   .Lx183_240
                        add              rsp, 32;                             jmp   n42_lit_string_β
.Lx183_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n45_statement_end_α
n44_call_β:             add              rsp, 32;                             jmp   n42_lit_string_β
#-----------------------------------------------------------------------------------------------------------------------
n45_statement_end_α:    add              rsp, 80;                             jmp   n46_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1010/002: opsyn alias'          :(END)
#-----------------------------------------------------------------------------------------------------------------------
n46_statement_begin_α:                                                        jmp   n47_lit_string_α
n46_statement_begin_β:                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n47_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 26
                        mov              rax, qword ptr [rip + .Lx189_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n48_assign_α
.Lx189_0:               .quad            .Lx189_0_s
.Lx189_0_s:             .string          "FAIL 1010/002: opsyn alias"
#-----------------------------------------------------------------------------------------------------------------------
n48_assign_α:           mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx190_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n49_statement_end_α
.Lx190_0:               .quad            .Lx190_0_s
.Lx190_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n49_statement_end_α:    add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e002  <stmt 11, line 17: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n50_statement_begin_α:                                                        jmp   n51_statement_end_α
n50_statement_begin_β:                                                        jmp   n52_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n51_statement_end_α:                                                          jmp   n52_statement_begin_α
#=======================================================================================================================
#         NE(facto(4), 24)           :f(e003)
#-----------------------------------------------------------------------------------------------------------------------
n52_statement_begin_α:                                                        jmp   n53_lit_integer_α
n52_statement_begin_β:                                                        jmp   n64_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n53_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx199_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n54_call_α
.Lx199_0:               .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n54_call_α:             sub              rsp, 16
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 16]            # lit_integer
                        mov              rdx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_arg_stage@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdi, qword ptr [rip + .Lx201_0]
                        mov              esi, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_proc_call_open@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        test             rax, rax;                            je    .Lx201_1
                        mov              rdi, qword ptr [rip + .Lx201_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_proc_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        lea              rcx, [rip + .Lx201_4]                # s111 floater pair (ZD twin NON-SLIM fallback): THE arm GVA-off actually reaches — MONITOR_BIN forces n_gva_m3=0, the slim tail at ~:403 that s110 patched refuses, and the site falls through to rt_proc_call_open here with flat rcx/rdx wires and NO pair.  Push omega then gamma = [rsp+0]=gamma [rsp+8]=omega; the fnrbp2 floater consumes 16 so L(3)/L(4) arrive at today's depth.  SCRIP_SLIM_PAIR=0 restores prior bytes.
                        push             rcx
                        lea              rcx, [rip + .Lx201_3]
                        push             rcx
                        lea              rcx, [rip + .Lx201_3]
                        lea              rdx, [rip + .Lx201_4];               jmp   rax
.Lx201_3:               mov              rdi, qword ptr [rip + .Lx201_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_proc_call_epilogue_named_γ@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   .Lx201_2
.Lx201_4:               mov              rdi, qword ptr [rip + .Lx201_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_proc_call_epilogue_named_ω@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   .Lx201_2
.Lx201_1:               call             rt_faildescr@PLT
.Lx201_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx201_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx201_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx201_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n52_statement_begin_β
.Lx201_240:                                                                   jmp   n55_lit_integer_α
n54_call_β:                                                                   jmp   n52_statement_begin_β
.Lx201_0:               .quad            .Lx201_0_s
.Lx201_0_s:             .string          "facto"
#-----------------------------------------------------------------------------------------------------------------------
n55_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx202_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n56_coerce_numeric_α
n55_lit_integer_β:      add              rsp, 16
                        add              rsp, 32;                             jmp   n52_statement_begin_β
.Lx202_0:               .quad            24
#-----------------------------------------------------------------------------------------------------------------------
n56_coerce_numeric_α:   sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]            # call
                        cmp              al, 5;                               je    .Lx204_1
                        cmp              al, 3;                               jne   .Lx204_0
                        mov              eax, dword ptr [rsp + 16]            # lit_integer
                        cmp              al, 3;                               jne   .Lx204_0
.Lx204_1:               mov              rax, qword ptr [rsp + 32]            # call
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # call
                        mov              qword ptr [rsp + 8], rax;            jmp   n57_coerce_numeric_α
.Lx204_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]                      # lit_integer
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 149
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n57_coerce_numeric_α
n56_coerce_numeric_β:   add              rsp, 16;                             jmp   n55_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n57_coerce_numeric_α:   sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              al, 5;                               je    .Lx206_1
                        cmp              al, 3;                               jne   .Lx206_0
                        mov              eax, dword ptr [rsp + 48]            # call
                        cmp              al, 3;                               jne   .Lx206_0
.Lx206_1:               mov              rax, qword ptr [rsp + 32]            # lit_integer
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n58_cmp_test_α
.Lx206_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 48]                      # call
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 150
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n58_cmp_test_α
n57_coerce_numeric_β:   add              rsp, 16;                             jmp   n56_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n58_cmp_test_α:         sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]            # coerce_numeric
                        mov              ecx, dword ptr [rsp + 16]
                        mov              edx, eax
                        and              edx, ecx
                        cmp              dl, 3;                               jne   .Lx208_0
                        mov              rax, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 24]
                        cmp              rax, rdx;                            jne   .Lx208_239
                        add              rsp, 16;                             jmp   n57_coerce_numeric_β
.Lx208_239:                                                                   jmp   n59_statement_end_α
.Lx208_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_cmp_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        test             eax, eax;                            jne   .Lx208_240
                        add              rsp, 16;                             jmp   n57_coerce_numeric_β
.Lx208_240:                                                                   jmp   n59_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n59_statement_end_α:    add              rsp, 96;                             jmp   n60_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1010/003: facto(4)=24 via alias' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n60_statement_begin_α:                                                        jmp   n61_lit_string_α
n60_statement_begin_β:                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n61_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 36
                        mov              rax, qword ptr [rip + .Lx213_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n62_assign_α
.Lx213_0:               .quad            .Lx213_0_s
.Lx213_0_s:             .string          "FAIL 1010/003: facto(4)=24 via alias"
#-----------------------------------------------------------------------------------------------------------------------
n62_assign_α:           mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx214_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n63_statement_end_α
.Lx214_0:               .quad            .Lx214_0_s
.Lx214_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n63_statement_end_α:    add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e003  <stmt 14, line 20: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n64_statement_begin_α:                                                        jmp   n65_statement_end_α
n64_statement_begin_β:                                                        jmp   n66_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n65_statement_end_α:                                                          jmp   n66_statement_begin_α
#=======================================================================================================================
#         DEFINE('fact2(n)', .fact2_entry)               :(fact2_end)
#-----------------------------------------------------------------------------------------------------------------------
n66_statement_begin_α:                                                        jmp   n67_define_α
n66_statement_begin_β:                                                        jmp   n89_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n67_define_α:           mov              rdi, qword ptr [rip + .Lx224_0]
                        mov              rsi, qword ptr [rip + .Lx224_1]
                        mov              edx, 1
                        mov              ecx, 1
                        mov              r8d, 0
                        lea              r9, [rip + n69_statement_begin_α]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_define_site@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n68_statement_end_α
n67_define_β:                                                                 jmp   n66_statement_begin_β
.Lx224_0:               .quad            .Lx224_0_s
.Lx224_0_s:             .string          "fact2"
.Lx224_1:               .quad            .Lx224_1_s
.Lx224_1_s:             .string          "n"
#-----------------------------------------------------------------------------------------------------------------------
n68_statement_end_α:                                                          jmp   n89_statement_begin_α
#=======================================================================================================================
# fact2_entry  <stmt 16, line 24: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n69_statement_begin_α:                                                        jmp   n70_statement_end_α
n69_statement_begin_β:                                                        jmp   n71_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n70_statement_end_α:                                                          jmp   n71_statement_begin_α
#=======================================================================================================================
#         fact2 = EQ(n, 1) 1                             :s(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n71_statement_begin_α:                                                        jmp   n72_var_α
n71_statement_begin_β:                                                        jmp   n80_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n72_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]             # n
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n73_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n73_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx235_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n74_coerce_numeric_α
n73_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                             jmp   n71_statement_begin_β
.Lx235_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n74_coerce_numeric_α:   sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]            # var
                        cmp              al, 5;                               je    .Lx237_1
                        cmp              al, 3;                               jne   .Lx237_0
                        mov              eax, dword ptr [rsp + 16]            # lit_integer
                        cmp              al, 3;                               jne   .Lx237_0
.Lx237_1:               mov              rax, qword ptr [rsp + 32]            # var
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # var
                        mov              qword ptr [rsp + 8], rax;            jmp   n75_coerce_numeric_α
.Lx237_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]                      # lit_integer
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 101
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n75_coerce_numeric_α
n74_coerce_numeric_β:   add              rsp, 16;                             jmp   n73_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n75_coerce_numeric_α:   sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              al, 5;                               je    .Lx239_1
                        cmp              al, 3;                               jne   .Lx239_0
                        mov              eax, dword ptr [rsp + 48]            # var
                        cmp              al, 3;                               jne   .Lx239_0
.Lx239_1:               mov              rax, qword ptr [rsp + 32]            # lit_integer
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n76_cmp_test_α
.Lx239_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 48]                      # var
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 102
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n76_cmp_test_α
n75_coerce_numeric_β:   add              rsp, 16;                             jmp   n74_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n76_cmp_test_α:         sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]            # coerce_numeric
                        mov              ecx, dword ptr [rsp + 16]
                        mov              edx, eax
                        and              edx, ecx
                        cmp              dl, 3;                               jne   .Lx241_0
                        mov              rax, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 24]
                        cmp              rax, rdx;                            je    .Lx241_239
                        add              rsp, 16;                             jmp   n75_coerce_numeric_β
.Lx241_239:                                                                   jmp   n77_lit_integer_α
.Lx241_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_cmp_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        test             eax, eax;                            je    .Lx241_240
                        add              rsp, 16;                             jmp   n75_coerce_numeric_β
.Lx241_240:                                                                   jmp   n77_lit_integer_α
n76_cmp_test_β:         add              rsp, 16;                             jmp   n75_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n77_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx242_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n78_assign_α
.Lx242_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n78_assign_α:           mov              rax, qword ptr [rsp + 0]             # lit_integer
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 32], rax             # fact2
                        mov              qword ptr [r9 + 40], rdx;            jmp   n79_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n79_statement_end_α:    add              rsp, 96;                             jmp   RETURN
#=======================================================================================================================
#         fact2 = n * fact2(n - 1)                       :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n80_statement_begin_α:                                                        jmp   n81_var_α
n80_statement_begin_β:                                                        jmp   RETURN
#-----------------------------------------------------------------------------------------------------------------------
n81_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]             # n
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n82_var_α
#-----------------------------------------------------------------------------------------------------------------------
n82_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]             # n
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx;            jmp   n83_lit_integer_α
n82_var_β:              add              rsp, 16
                        add              rsp, 16;                             jmp   n80_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n83_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx250_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n84_binop_α
n83_lit_integer_β:      add              rsp, 16;                             jmp   n82_var_β
.Lx250_0:               .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n84_binop_α:            sub              rsp, 16
                        mov              ecx, dword ptr [rsp + 32]            # var
                        mov              rax, qword ptr [rsp + 40]
                        cmp              cl, 3;                               jne   .Lx251_2
                        sub              rax, 1
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              qword ptr [rsp + 8], rax;            jmp   .Lx251_7
.Lx251_2:               mov              eax, ecx
                        mov              edx, ecx
                        and              edx, 1;                              jz    .Lx251_0
                        mov              rsi, qword ptr [rsp + 40]            # var
                        mov              rdi, 1
                        cmp              al, 5;                               je    .Lx251_3
                        cvtsi2sd         xmm0, rsi;                           jmp   .Lx251_4
.Lx251_3:               movq             xmm0, rsi
.Lx251_4:               cvtsi2sd         xmm1, rdi
                        subsd            xmm0, xmm1
                        movq             rax, xmm0
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              qword ptr [rsp + 8], rax
.Lx251_7:                                                                     jmp   n85_call_α
.Lx251_0:               mov              rdi, qword ptr [rsp + 32]            # var
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]            # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_sub@PLT
                        cmp              al, 104;                             jne   .Lx251_240
                        add              rsp, 16;                             jmp   n83_lit_integer_β
.Lx251_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n85_call_α
n84_binop_β:            add              rsp, 16;                             jmp   n83_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n85_call_α:             sub              rsp, 16
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 16]            # binop
                        mov              rdx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_arg_stage@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdi, qword ptr [rip + .Lx253_0]
                        mov              esi, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_proc_call_open@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        test             rax, rax;                            je    .Lx253_1
                        mov              rdi, qword ptr [rip + .Lx253_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_proc_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        lea              rcx, [rip + .Lx253_4]                # s111 floater pair (ZD twin NON-SLIM fallback): THE arm GVA-off actually reaches — MONITOR_BIN forces n_gva_m3=0, the slim tail at ~:403 that s110 patched refuses, and the site falls through to rt_proc_call_open here with flat rcx/rdx wires and NO pair.  Push omega then gamma = [rsp+0]=gamma [rsp+8]=omega; the fnrbp2 floater consumes 16 so L(3)/L(4) arrive at today's depth.  SCRIP_SLIM_PAIR=0 restores prior bytes.
                        push             rcx
                        lea              rcx, [rip + .Lx253_3]
                        push             rcx
                        lea              rcx, [rip + .Lx253_3]
                        lea              rdx, [rip + .Lx253_4];               jmp   rax
.Lx253_3:               mov              rdi, qword ptr [rip + .Lx253_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_proc_call_epilogue_named_γ@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   .Lx253_2
.Lx253_4:               mov              rdi, qword ptr [rip + .Lx253_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_proc_call_epilogue_named_ω@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   .Lx253_2
.Lx253_1:               call             rt_faildescr@PLT
.Lx253_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx253_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx253_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx253_240
                        add              rsp, 16;                             jmp   n84_binop_β
.Lx253_240:                                                                   jmp   n86_binop_α
n85_call_β:                                                                   jmp   n84_binop_β
.Lx253_0:               .quad            .Lx253_0_s
.Lx253_0_s:             .string          "fact2"
#-----------------------------------------------------------------------------------------------------------------------
n86_binop_α:            sub              rsp, 16
                        mov              eax, dword ptr [rsp + 80]            # var
                        mov              ecx, dword ptr [rsp + 16]            # call
                        mov              edx, eax
                        and              edx, ecx
                        cmp              dl, 3;                               jne   .Lx254_2
                        mov              rax, qword ptr [rsp + 88]            # var
                        mov              rdx, qword ptr [rsp + 24]            # call
                        imul             rax, rdx
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              qword ptr [rsp + 8], rax;            jmp   .Lx254_7
.Lx254_2:               and              edx, 1;                              jz    .Lx254_0
                        mov              rsi, qword ptr [rsp + 88]            # var
                        mov              rdi, qword ptr [rsp + 24]            # call
                        cmp              al, 5;                               je    .Lx254_3
                        cvtsi2sd         xmm0, rsi;                           jmp   .Lx254_4
.Lx254_3:               movq             xmm0, rsi
.Lx254_4:               cmp              cl, 5;                               je    .Lx254_5
                        cvtsi2sd         xmm1, rdi;                           jmp   .Lx254_6
.Lx254_5:               movq             xmm1, rdi
.Lx254_6:               mulsd            xmm0, xmm1
                        movq             rax, xmm0
                        mov              qword ptr [rsp + 0], 5               # result
                        mov              qword ptr [rsp + 8], rax
.Lx254_7:                                                                     jmp   n87_assign_α
.Lx254_0:               mov              rdi, qword ptr [rsp + 80]            # var
                        mov              rsi, qword ptr [rsp + 88]
                        mov              rdx, qword ptr [rsp + 16]            # call
                        mov              rcx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_mul@PLT
                        cmp              al, 104;                             jne   .Lx254_240
                        add              rsp, 32;                             jmp   n84_binop_β
.Lx254_240:             mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n87_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n87_assign_α:           mov              rax, qword ptr [rsp + 0]             # binop
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 32], rax             # fact2
                        mov              qword ptr [r9 + 40], rdx;            jmp   n88_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n88_statement_end_α:    add              rsp, 96;                             jmp   RETURN
#=======================================================================================================================
# fact2_end  <stmt 19, line 27: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n89_statement_begin_α:                                                        jmp   n90_statement_end_α
n89_statement_begin_β:                                                        jmp   n91_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n90_statement_end_α:                                                          jmp   n91_statement_begin_α
#=======================================================================================================================
#         NE(fact2(6), 720)          :f(e004)
#-----------------------------------------------------------------------------------------------------------------------
n91_statement_begin_α:                                                        jmp   n92_lit_integer_α
n91_statement_begin_β:                                                        jmp   n103_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n92_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx264_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n93_call_α
.Lx264_0:               .quad            6
#-----------------------------------------------------------------------------------------------------------------------
n93_call_α:             sub              rsp, 16
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 16]            # lit_integer
                        mov              rdx, qword ptr [rsp + 24]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_arg_stage@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rdi, qword ptr [rip + .Lx266_0]
                        mov              esi, 1
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_proc_call_open@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        test             rax, rax;                            je    .Lx266_1
                        mov              rdi, qword ptr [rip + .Lx266_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_proc_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        lea              rcx, [rip + .Lx266_4]                # s111 floater pair (ZD twin NON-SLIM fallback): THE arm GVA-off actually reaches — MONITOR_BIN forces n_gva_m3=0, the slim tail at ~:403 that s110 patched refuses, and the site falls through to rt_proc_call_open here with flat rcx/rdx wires and NO pair.  Push omega then gamma = [rsp+0]=gamma [rsp+8]=omega; the fnrbp2 floater consumes 16 so L(3)/L(4) arrive at today's depth.  SCRIP_SLIM_PAIR=0 restores prior bytes.
                        push             rcx
                        lea              rcx, [rip + .Lx266_3]
                        push             rcx
                        lea              rcx, [rip + .Lx266_3]
                        lea              rdx, [rip + .Lx266_4];               jmp   rax
.Lx266_3:               mov              rdi, qword ptr [rip + .Lx266_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_proc_call_epilogue_named_γ@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   .Lx266_2
.Lx266_4:               mov              rdi, qword ptr [rip + .Lx266_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_proc_call_epilogue_named_ω@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   .Lx266_2
.Lx266_1:               call             rt_faildescr@PLT
.Lx266_2:               mov              rcx, qword ptr [rip + rt_g_ret_by_name@GOTPCREL] # NRETURN by-name consult (live wn, consumed)
                        mov              ecx, dword ptr [rcx + 0]
                        cmp              ecx, 0;                              je    .Lx266_29
                        mov              rdi, rax
                        mov              rsi, rdx
                        mov              edx, 0
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_nret_fix_tiny@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
.Lx266_29:              mov              qword ptr [rsp + 0], rax             # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              al, 104;                             jne   .Lx266_240
                        add              rsp, 16
                        add              rsp, 16;                             jmp   n91_statement_begin_β
.Lx266_240:                                                                   jmp   n94_lit_integer_α
n93_call_β:                                                                   jmp   n91_statement_begin_β
.Lx266_0:               .quad            .Lx266_0_s
.Lx266_0_s:             .string          "fact2"
#-----------------------------------------------------------------------------------------------------------------------
n94_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3               # result
                        mov              rax, qword ptr [rip + .Lx267_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n95_coerce_numeric_α
n94_lit_integer_β:      add              rsp, 16
                        add              rsp, 32;                             jmp   n91_statement_begin_β
.Lx267_0:               .quad            720
#-----------------------------------------------------------------------------------------------------------------------
n95_coerce_numeric_α:   sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]            # call
                        cmp              al, 5;                               je    .Lx269_1
                        cmp              al, 3;                               jne   .Lx269_0
                        mov              eax, dword ptr [rsp + 16]            # lit_integer
                        cmp              al, 3;                               jne   .Lx269_0
.Lx269_1:               mov              rax, qword ptr [rsp + 32]            # call
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # call
                        mov              qword ptr [rsp + 8], rax;            jmp   n96_coerce_numeric_α
.Lx269_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]                      # lit_integer
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 149
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n96_coerce_numeric_α
n95_coerce_numeric_β:   add              rsp, 16;                             jmp   n94_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n96_coerce_numeric_α:   sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]            # lit_integer
                        cmp              al, 5;                               je    .Lx271_1
                        cmp              al, 3;                               jne   .Lx271_0
                        mov              eax, dword ptr [rsp + 48]            # call
                        cmp              al, 3;                               jne   .Lx271_0
.Lx271_1:               mov              rax, qword ptr [rsp + 32]            # lit_integer
                        mov              qword ptr [rsp + 0], rax             # result
                        mov              rax, qword ptr [rsp + 40]            # lit_integer
                        mov              qword ptr [rsp + 8], rax;            jmp   n97_cmp_test_α
.Lx271_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 48]                      # call
                        lea              rdx, [rsp + 0]                       # result
                        mov              rcx, 150
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_coerce_num2_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n97_cmp_test_α
n96_coerce_numeric_β:   add              rsp, 16;                             jmp   n95_coerce_numeric_β
#-----------------------------------------------------------------------------------------------------------------------
n97_cmp_test_α:         sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]            # coerce_numeric
                        mov              ecx, dword ptr [rsp + 16]
                        mov              edx, eax
                        and              edx, ecx
                        cmp              dl, 3;                               jne   .Lx273_0
                        mov              rax, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 24]
                        cmp              rax, rdx;                            jne   .Lx273_239
                        add              rsp, 16;                             jmp   n96_coerce_numeric_β
.Lx273_239:                                                                   jmp   n98_statement_end_α
.Lx273_0:               lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             rt_cmp_d@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48]
                        test             eax, eax;                            jne   .Lx273_240
                        add              rsp, 16;                             jmp   n96_coerce_numeric_β
.Lx273_240:                                                                   jmp   n98_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n98_statement_end_α:    add              rsp, 96;                             jmp   n99_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1010/004: fact2(6)=720 alt entry' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n99_statement_begin_α:                                                        jmp   n100_lit_string_α
n99_statement_begin_β:                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n100_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 37
                        mov              rax, qword ptr [rip + .Lx278_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n101_assign_α
.Lx278_0:               .quad            .Lx278_0_s
.Lx278_0_s:             .string          "FAIL 1010/004: fact2(6)=720 alt entry"
#-----------------------------------------------------------------------------------------------------------------------
n101_assign_α:          mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx279_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n102_statement_end_α
.Lx279_0:               .quad            .Lx279_0_s
.Lx279_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n102_statement_end_α:   add              rsp, 16;                             jmp   main_γ
#=======================================================================================================================
# e004  <stmt 22, line 30: source not in main file (INCLUDE)>
#-----------------------------------------------------------------------------------------------------------------------
n103_statement_begin_α:                                                       jmp   n104_statement_end_α
n103_statement_begin_β:                                                       jmp   n105_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n104_statement_end_α:                                                         jmp   n105_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'PASS 1010_func_recursion (4/4)'
#-----------------------------------------------------------------------------------------------------------------------
n105_statement_begin_α:                                                       jmp   n106_lit_string_α
n105_statement_begin_β:                                                       jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n106_lit_string_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2               # result
                        mov              dword ptr [rsp + 4], 30
                        mov              rax, qword ptr [rip + .Lx288_0]
                        mov              qword ptr [rsp + 8], rax;            jmp   n107_assign_α
.Lx288_0:               .quad            .Lx288_0_s
.Lx288_0_s:             .string          "PASS 1010_func_recursion (4/4)"
#-----------------------------------------------------------------------------------------------------------------------
n107_assign_α:          mov              rsi, qword ptr [rsp + 0]             # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx289_0]
                        mov              qword ptr [rip + rtccb+40], r8
                        call             NV_SET_fn@PLT
                        mov              r8,  qword ptr [rip + rtccb+40]
                        mov              r9,  qword ptr [rip + rtccb+48];     jmp   n108_statement_end_α
.Lx289_0:               .quad            .Lx289_0_s
.Lx289_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n108_statement_end_α:   add              rsp, 16;                             jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
RETURN:                 pop              rcx
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
