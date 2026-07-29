                        .intel_syntax    noprefix
                        .text
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_roman$2F2_α
proc_roman$2F2_α:
                        .global          proc_roman$2F2_α
                        .global          proc_roman$2F2_β
                        .global          proc_roman$2F2_γ
                        .global          proc_roman$2F2_ω
                        sub              rsp, 8832
                        mov              [rsp + 8808], rcx
                        mov              [rsp + 8816], rdx
                        mov              [rsp + 8824], rbp
                        mov              rbp, rsp
                        mov              rdi, rsp
                        mov              esi, 8736
                        mov              edx, 8800
                        call             rt_jmp_frame_lexprep2@PLT
proc_roman$2F2_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n0_op11_α:
                        lea              r10, [rip + g_plw_cellws_on]
                        mov              eax, dword ptr [r10 + 0]
                        test             eax, eax
                                                                                        jne   .Lx327_100
                        lea              r10, [rip + g_zeta_mode]
                        mov              eax, dword ptr [r10 + 0]
                        cmp              eax, 2
                                                                                        je    .Lx327_100
                        lea              r10, [rip + g_pl_trail]
                        mov              eax, dword ptr [r10 + 32]
                        movsxd           rdx, eax
                        mov              eax, 6
                                                                                        jmp   .Lx327_101
.Lx327_100:
                        lea              rdi, [rbp + 96]
                        mov              esi, 0
                        call             rt_pl_dop_trail_mark@PLT
.Lx327_101:
                        mov              qword ptr [rbp + 80], rax
                        mov              qword ptr [rbp + 88], rdx
                        cmp              eax, 99
                                                                                        je    proc_roman$2F2_ω
                                                                                        jmp   n1_var_ref_α
n0_op11_β:
                                                                                        jmp   proc_roman$2F2_ω
#-----------------------------------------------------------------------------------------------------------------------
n1_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 16]
                        mov              qword ptr [rbp + 8688], rax
                        mov              qword ptr [rbp + 8696], rdx
                                                                                        jmp   n2_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n2_lit_integer_α:
                        mov              qword ptr [rbp + 8704], 6
                        mov              rax, qword ptr [rip + .Lx330_0]
                        mov              qword ptr [rbp + 8712], rax
                                                                                        jmp   n3_lit_integer_α
.Lx330_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n3_lit_integer_α:
                        mov              qword ptr [rbp + 8720], 6
                        mov              rax, qword ptr [rip + .Lx331_0]
                        mov              qword ptr [rbp + 8728], rax
                                                                                        jmp   n4_op11_α
.Lx331_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n4_op11_α:
                        mov              rax, qword ptr [rbp + 8688]
                        mov              qword ptr [rbp + 8640], rax
                        mov              rax, qword ptr [rbp + 8696]
                        mov              qword ptr [rbp + 8648], rax
                        mov              rax, qword ptr [rbp + 8704]
                        mov              qword ptr [rbp + 8656], rax
                        mov              rax, qword ptr [rbp + 8712]
                        mov              qword ptr [rbp + 8664], rax
                        mov              rax, qword ptr [rbp + 8720]
                        mov              qword ptr [rbp + 8672], rax
                        mov              rax, qword ptr [rbp + 8728]
                        mov              qword ptr [rbp + 8680], rax
                        lea              rdi, [rbp + 8640]
                        lea              r8, [rbp + 8640]
.Lx332_110:
                        mov              eax, dword ptr [r8 + 0]
                        cmp              eax, 9
                                                                                        jne   .Lx332_113
                        mov              esi, dword ptr [r8 + 4]
                        mov              rax, qword ptr [r8 + 8]
                        test             rax, rax
                                                                                        je    .Lx332_111
                        cmp              esi, 1
                                                                                        jne   .Lx332_112
                        mov              r8, rax
                                                                                        jmp   .Lx332_110
.Lx332_112:
                        cmp              esi, 2
                                                                                        jne   .Lx332_111
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx332_111
                        mov              r8, rax
                                                                                        jmp   .Lx332_110
.Lx332_113:
                        cmp              eax, 13
                                                                                        jne   .Lx332_111
                        mov              rax, qword ptr [r8 + 8]
                        test             rax, rax
                                                                                        je    .Lx332_111
                        cmp              rax, r8
                                                                                        je    .Lx332_111
                        mov              r8, rax
                                                                                        jmp   .Lx332_110
.Lx332_111:
                        mov              eax, dword ptr [r8 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx332_114
                        cmp              eax, 99
                                                                                        je    .Lx332_114
                        cmp              eax, 13
                                                                                        jne   .Lx332_118
                        mov              rax, qword ptr [r8 + 8]
                        cmp              rax, r8
                                                                                        je    .Lx332_114
                                                                                        jmp   .Lx332_118
.Lx332_118:
                        mov              eax, dword ptr [r8 + 0]
                        cmp              eax, 14
                                                                                        je    .Lx332_115
                        cmp              eax, 6
                                                                                        jne   .Lx332_114
                        mov              esi, dword ptr [r8 + 4]
                        cmp              esi, 0
                                                                                        jne   .Lx332_114
                        movabs           rdx, 0
                        mov              rsi, qword ptr [r8 + 8]
                        cmp              rsi, rdx
                                                                                        jne   .Lx332_115
                                                                                        jmp   .Lx332_114
.Lx332_114:
                        mov              eax, 6
                        mov              edx, 1
                                                                                        jmp   .Lx332_117
.Lx332_115:
                        mov              eax, 99
                        mov              edx, 0
                                                                                        jmp   .Lx332_117
.Lx332_116:
                        mov              esi, 3
                        call             rt_pl_dop_ix_g@PLT
.Lx332_117:
                        mov              qword ptr [rbp + 8624], rax
                        mov              qword ptr [rbp + 8632], rdx
                        cmp              eax, 99
                                                                                        je    n6_var_ref_α
                                                                                        jmp   n5_var_ref_α
n4_op11_β:
                                                                                        jmp   n6_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n5_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 16]
                        mov              qword ptr [rbp + 8592], rax
                        mov              qword ptr [rbp + 8600], rdx
                                                                                        jmp   n7_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n6_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 16]
                        mov              qword ptr [rbp + 8400], rax
                        mov              qword ptr [rbp + 8408], rdx
                                                                                        jmp   n8_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n7_lit_integer_α:
                        mov              qword ptr [rbp + 8608], 6
                        mov              rax, qword ptr [rip + .Lx337_0]
                        mov              qword ptr [rbp + 8616], rax
                                                                                        jmp   n9_op11_α
.Lx337_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n8_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8784]
                        mov              qword ptr [rbp + 8416], rax
                        mov              qword ptr [rbp + 8424], rdx
                                                                                        jmp   n10_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n9_op11_α:
                        mov              rax, qword ptr [rbp + 8592]
                        mov              qword ptr [rbp + 8560], rax
                        mov              rax, qword ptr [rbp + 8600]
                        mov              qword ptr [rbp + 8568], rax
                        lea              rdi, [rbp + 8560]
                        movabs           rsi, 0
                        call             rt_pl_dop_unify_ci@PLT
                        mov              qword ptr [rbp + 8544], rax
                        mov              qword ptr [rbp + 8552], rdx
                        cmp              eax, 99
                                                                                        je    n12_op11_α
                                                                                        jmp   n11_var_ref_α
n9_op11_β:
                                                                                        jmp   n12_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n10_op11_α:
                        mov              rax, qword ptr [rbp + 8400]
                        mov              qword ptr [rbp + 8368], rax
                        mov              rax, qword ptr [rbp + 8408]
                        mov              qword ptr [rbp + 8376], rax
                        mov              rax, qword ptr [rbp + 8416]
                        mov              qword ptr [rbp + 8384], rax
                        mov              rax, qword ptr [rbp + 8424]
                        mov              qword ptr [rbp + 8392], rax
                        lea              rdi, [rbp + 8368]
                        lea              r8, [rbp + 8368]
.Lx341_40:
                        mov              eax, dword ptr [r8 + 0]
                        cmp              eax, 9
                                                                                        jne   .Lx341_56
                        mov              esi, dword ptr [r8 + 4]
                        mov              rax, qword ptr [r8 + 8]
                        test             rax, rax
                                                                                        je    .Lx341_41
                        cmp              esi, 1
                                                                                        jne   .Lx341_55
                        mov              r8, rax
                                                                                        jmp   .Lx341_40
.Lx341_55:
                        cmp              esi, 2
                                                                                        jne   .Lx341_41
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx341_41
                        mov              r8, rax
                                                                                        jmp   .Lx341_40
.Lx341_56:
                        cmp              eax, 13
                                                                                        jne   .Lx341_41
                        mov              rax, qword ptr [r8 + 8]
                        test             rax, rax
                                                                                        je    .Lx341_41
                        cmp              rax, r8
                                                                                        je    .Lx341_41
                        mov              r8, rax
                                                                                        jmp   .Lx341_40
.Lx341_41:
                        lea              r9, [rbp + 8384]
.Lx341_42:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 9
                                                                                        jne   .Lx341_58
                        mov              esi, dword ptr [r9 + 4]
                        mov              rax, qword ptr [r9 + 8]
                        test             rax, rax
                                                                                        je    .Lx341_43
                        cmp              esi, 1
                                                                                        jne   .Lx341_57
                        mov              r9, rax
                                                                                        jmp   .Lx341_42
.Lx341_57:
                        cmp              esi, 2
                                                                                        jne   .Lx341_43
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx341_43
                        mov              r9, rax
                                                                                        jmp   .Lx341_42
.Lx341_58:
                        cmp              eax, 13
                                                                                        jne   .Lx341_43
                        mov              rax, qword ptr [r9 + 8]
                        test             rax, rax
                                                                                        je    .Lx341_43
                        cmp              rax, r9
                                                                                        je    .Lx341_43
                        mov              r9, rax
                                                                                        jmp   .Lx341_42
.Lx341_43:
                        cmp              r8, r9
                                                                                        je    .Lx341_51
                        mov              eax, dword ptr [r8 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx341_44
                        cmp              eax, 99
                                                                                        je    .Lx341_44
                        cmp              eax, 13
                                                                                        jne   .Lx341_45
                        mov              rax, qword ptr [r8 + 8]
                        cmp              rax, r8
                                                                                        je    .Lx341_44
                                                                                        jmp   .Lx341_45
.Lx341_44:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx341_53
                        cmp              eax, 99
                                                                                        je    .Lx341_53
                        cmp              eax, 13
                                                                                        jne   .Lx341_46
                        mov              rax, qword ptr [r9 + 8]
                        cmp              rax, r9
                                                                                        je    .Lx341_53
                                                                                        jmp   .Lx341_46
.Lx341_46:
                        lea              r10, [rip + g_pl_trail]
                        mov              r11, qword ptr [r10 + 0]
                        test             r11, r11
                                                                                        je    .Lx341_53
                        mov              eax, dword ptr [r10 + 32]
                        mov              esi, 24
                        imul             rsi, rax
                        mov              rax, qword ptr [r10 + 24]
                        sub              rax, 24
                        cmp              rsi, rax
                                                                                        ja    .Lx341_53
                        add              r11, rsi
                        mov              qword ptr [r11 + 0], r8
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r11 + 8], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r11 + 16], rax
                        mov              eax, dword ptr [r10 + 32]
                        add              eax, 1
                        mov              dword ptr [r10 + 32], eax
                        mov              rax, qword ptr [r9 + 0]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [r8 + 8], rax
                                                                                        jmp   .Lx341_51
.Lx341_45:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx341_47
                        cmp              eax, 99
                                                                                        je    .Lx341_47
                        cmp              eax, 13
                                                                                        jne   .Lx341_48
                        mov              rax, qword ptr [r9 + 8]
                        cmp              rax, r9
                                                                                        je    .Lx341_47
                                                                                        jmp   .Lx341_48
.Lx341_47:
                        lea              r10, [rip + g_pl_trail]
                        mov              r11, qword ptr [r10 + 0]
                        test             r11, r11
                                                                                        je    .Lx341_53
                        mov              eax, dword ptr [r10 + 32]
                        mov              esi, 24
                        imul             rsi, rax
                        mov              rax, qword ptr [r10 + 24]
                        sub              rax, 24
                        cmp              rsi, rax
                                                                                        ja    .Lx341_53
                        add              r11, rsi
                        mov              qword ptr [r11 + 0], r9
                        mov              rax, qword ptr [r9 + 0]
                        mov              qword ptr [r11 + 8], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [r11 + 16], rax
                        mov              eax, dword ptr [r10 + 32]
                        add              eax, 1
                        mov              dword ptr [r10 + 32], eax
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 8], rax
                                                                                        jmp   .Lx341_51
.Lx341_48:
                        mov              ecx, dword ptr [r8 + 0]
                        mov              edx, dword ptr [r9 + 0]
                        cmp              ecx, 14
                                                                                        jne   .Lx341_49
                        cmp              edx, 14
                                                                                        je    .Lx341_53
                                                                                        jmp   .Lx341_52
.Lx341_49:
                        cmp              edx, 14
                                                                                        je    .Lx341_52
                        cmp              ecx, 7
                                                                                        je    .Lx341_53
                        cmp              edx, 7
                                                                                        je    .Lx341_53
                        cmp              ecx, 6
                                                                                        jne   .Lx341_50
                        cmp              edx, 6
                                                                                        jne   .Lx341_50
                        mov              rax, qword ptr [r8 + 0]
                        cmp              rax, 6
                                                                                        jne   .Lx341_53
                        mov              rax, qword ptr [r9 + 0]
                        cmp              rax, 6
                                                                                        jne   .Lx341_53
                        mov              rax, qword ptr [r8 + 8]
                        mov              rsi, qword ptr [r9 + 8]
                        cmp              rax, rsi
                                                                                        je    .Lx341_51
                                                                                        jmp   .Lx341_52
.Lx341_50:
                        mov              rax, qword ptr [r8 + 0]
                        mov              rsi, qword ptr [r9 + 0]
                        cmp              rax, rsi
                                                                                        jne   .Lx341_53
                        mov              rax, qword ptr [r8 + 8]
                        mov              rsi, qword ptr [r9 + 8]
                        cmp              rax, rsi
                                                                                        jne   .Lx341_53
.Lx341_51:
                        mov              rax, qword ptr [r8 + 0]
                        mov              rdx, qword ptr [r8 + 8]
                                                                                        jmp   .Lx341_54
.Lx341_52:
                        mov              eax, 99
                        mov              edx, 0
                                                                                        jmp   .Lx341_54
.Lx341_53:
                        mov              esi, 2
                        call             rt_pl_dop_unify@PLT
.Lx341_54:
                        mov              qword ptr [rbp + 8352], rax
                        mov              qword ptr [rbp + 8360], rdx
                        cmp              eax, 99
                                                                                        je    n14_op11_α
                                                                                        jmp   n13_var_ref_α
n10_op11_β:
                                                                                        jmp   n14_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n11_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 32]
                        mov              qword ptr [rbp + 8512], rax
                        mov              qword ptr [rbp + 8520], rdx
                                                                                        jmp   n15_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n12_op11_α:
                        mov              rax, qword ptr [rbp + 80]
                        mov              qword ptr [rbp + 8448], rax
                        mov              rax, qword ptr [rbp + 88]
                        mov              qword ptr [rbp + 8456], rax
                        lea              rdi, [rbp + 8448]
                        mov              esi, 1
                        call             rt_pl_dop_unwind_nothrow@PLT
                        mov              qword ptr [rbp + 8432], rax
                        mov              qword ptr [rbp + 8440], rdx
                        cmp              eax, 99
                                                                                        je    proc_roman$2F2_ω
                                                                                        jmp   n6_var_ref_α
n12_op11_β:
                                                                                        jmp   proc_roman$2F2_ω
#-----------------------------------------------------------------------------------------------------------------------
n13_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 32]
                        mov              qword ptr [rbp + 8320], rax
                        mov              qword ptr [rbp + 8328], rdx
                                                                                        jmp   n16_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n14_op11_α:
                        mov              rax, qword ptr [rbp + 80]
                        mov              qword ptr [rbp + 7808], rax
                        mov              rax, qword ptr [rbp + 88]
                        mov              qword ptr [rbp + 7816], rax
                        lea              rdi, [rbp + 7808]
                        mov              esi, 1
                        call             rt_pl_dop_unwind_nothrow@PLT
                        mov              qword ptr [rbp + 7792], rax
                        mov              qword ptr [rbp + 7800], rdx
                        cmp              eax, 99
                                                                                        je    proc_roman$2F2_ω
                                                                                        jmp   n17_var_ref_α
n14_op11_β:
                                                                                        jmp   proc_roman$2F2_ω
#-----------------------------------------------------------------------------------------------------------------------
n15_lit_string_α:
                        mov              qword ptr [rbp + 8528], 1
                        mov              rax, qword ptr [rip + .Lx348_0]
                        mov              qword ptr [rbp + 8536], rax
                                                                                        jmp   n18_op11_α
.Lx348_0:
                        .quad            .Lx348_0_s
.Lx348_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n16_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8752]
                        mov              qword ptr [rbp + 8336], rax
                        mov              qword ptr [rbp + 8344], rdx
                                                                                        jmp   n19_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n17_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 16]
                        mov              qword ptr [rbp + 7760], rax
                        mov              qword ptr [rbp + 7768], rdx
                                                                                        jmp   n20_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n18_op11_α:
                        mov              rax, qword ptr [rbp + 8512]
                        mov              qword ptr [rbp + 8480], rax
                        mov              rax, qword ptr [rbp + 8520]
                        mov              qword ptr [rbp + 8488], rax
                        lea              rdi, [rbp + 8480]
                        mov              rsi, qword ptr [rip + .Lx353_2]
                                                                                        jmp   .Lx353_3
.Lx353_2:
                        .quad            .Lx353_2_s
.Lx353_2_s:
                        .string          ""
.Lx353_3:
                        call             rt_pl_dop_unify_cs@PLT
                        mov              qword ptr [rbp + 8464], rax
                        mov              qword ptr [rbp + 8472], rdx
                        cmp              eax, 99
                                                                                        je    n12_op11_α
                                                                                        jmp   n21_op19_α
n18_op11_β:
                                                                                        jmp   n12_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n19_op11_α:
                        mov              rax, qword ptr [rbp + 8320]
                        mov              qword ptr [rbp + 8288], rax
                        mov              rax, qword ptr [rbp + 8328]
                        mov              qword ptr [rbp + 8296], rax
                        mov              rax, qword ptr [rbp + 8336]
                        mov              qword ptr [rbp + 8304], rax
                        mov              rax, qword ptr [rbp + 8344]
                        mov              qword ptr [rbp + 8312], rax
                        lea              rdi, [rbp + 8288]
                        lea              r8, [rbp + 8288]
.Lx354_40:
                        mov              eax, dword ptr [r8 + 0]
                        cmp              eax, 9
                                                                                        jne   .Lx354_56
                        mov              esi, dword ptr [r8 + 4]
                        mov              rax, qword ptr [r8 + 8]
                        test             rax, rax
                                                                                        je    .Lx354_41
                        cmp              esi, 1
                                                                                        jne   .Lx354_55
                        mov              r8, rax
                                                                                        jmp   .Lx354_40
.Lx354_55:
                        cmp              esi, 2
                                                                                        jne   .Lx354_41
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx354_41
                        mov              r8, rax
                                                                                        jmp   .Lx354_40
.Lx354_56:
                        cmp              eax, 13
                                                                                        jne   .Lx354_41
                        mov              rax, qword ptr [r8 + 8]
                        test             rax, rax
                                                                                        je    .Lx354_41
                        cmp              rax, r8
                                                                                        je    .Lx354_41
                        mov              r8, rax
                                                                                        jmp   .Lx354_40
.Lx354_41:
                        lea              r9, [rbp + 8304]
.Lx354_42:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 9
                                                                                        jne   .Lx354_58
                        mov              esi, dword ptr [r9 + 4]
                        mov              rax, qword ptr [r9 + 8]
                        test             rax, rax
                                                                                        je    .Lx354_43
                        cmp              esi, 1
                                                                                        jne   .Lx354_57
                        mov              r9, rax
                                                                                        jmp   .Lx354_42
.Lx354_57:
                        cmp              esi, 2
                                                                                        jne   .Lx354_43
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx354_43
                        mov              r9, rax
                                                                                        jmp   .Lx354_42
.Lx354_58:
                        cmp              eax, 13
                                                                                        jne   .Lx354_43
                        mov              rax, qword ptr [r9 + 8]
                        test             rax, rax
                                                                                        je    .Lx354_43
                        cmp              rax, r9
                                                                                        je    .Lx354_43
                        mov              r9, rax
                                                                                        jmp   .Lx354_42
.Lx354_43:
                        cmp              r8, r9
                                                                                        je    .Lx354_51
                        mov              eax, dword ptr [r8 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx354_44
                        cmp              eax, 99
                                                                                        je    .Lx354_44
                        cmp              eax, 13
                                                                                        jne   .Lx354_45
                        mov              rax, qword ptr [r8 + 8]
                        cmp              rax, r8
                                                                                        je    .Lx354_44
                                                                                        jmp   .Lx354_45
.Lx354_44:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx354_53
                        cmp              eax, 99
                                                                                        je    .Lx354_53
                        cmp              eax, 13
                                                                                        jne   .Lx354_46
                        mov              rax, qword ptr [r9 + 8]
                        cmp              rax, r9
                                                                                        je    .Lx354_53
                                                                                        jmp   .Lx354_46
.Lx354_46:
                        lea              r10, [rip + g_pl_trail]
                        mov              r11, qword ptr [r10 + 0]
                        test             r11, r11
                                                                                        je    .Lx354_53
                        mov              eax, dword ptr [r10 + 32]
                        mov              esi, 24
                        imul             rsi, rax
                        mov              rax, qword ptr [r10 + 24]
                        sub              rax, 24
                        cmp              rsi, rax
                                                                                        ja    .Lx354_53
                        add              r11, rsi
                        mov              qword ptr [r11 + 0], r8
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r11 + 8], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r11 + 16], rax
                        mov              eax, dword ptr [r10 + 32]
                        add              eax, 1
                        mov              dword ptr [r10 + 32], eax
                        mov              rax, qword ptr [r9 + 0]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [r8 + 8], rax
                                                                                        jmp   .Lx354_51
.Lx354_45:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx354_47
                        cmp              eax, 99
                                                                                        je    .Lx354_47
                        cmp              eax, 13
                                                                                        jne   .Lx354_48
                        mov              rax, qword ptr [r9 + 8]
                        cmp              rax, r9
                                                                                        je    .Lx354_47
                                                                                        jmp   .Lx354_48
.Lx354_47:
                        lea              r10, [rip + g_pl_trail]
                        mov              r11, qword ptr [r10 + 0]
                        test             r11, r11
                                                                                        je    .Lx354_53
                        mov              eax, dword ptr [r10 + 32]
                        mov              esi, 24
                        imul             rsi, rax
                        mov              rax, qword ptr [r10 + 24]
                        sub              rax, 24
                        cmp              rsi, rax
                                                                                        ja    .Lx354_53
                        add              r11, rsi
                        mov              qword ptr [r11 + 0], r9
                        mov              rax, qword ptr [r9 + 0]
                        mov              qword ptr [r11 + 8], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [r11 + 16], rax
                        mov              eax, dword ptr [r10 + 32]
                        add              eax, 1
                        mov              dword ptr [r10 + 32], eax
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 8], rax
                                                                                        jmp   .Lx354_51
.Lx354_48:
                        mov              ecx, dword ptr [r8 + 0]
                        mov              edx, dword ptr [r9 + 0]
                        cmp              ecx, 14
                                                                                        jne   .Lx354_49
                        cmp              edx, 14
                                                                                        je    .Lx354_53
                                                                                        jmp   .Lx354_52
.Lx354_49:
                        cmp              edx, 14
                                                                                        je    .Lx354_52
                        cmp              ecx, 7
                                                                                        je    .Lx354_53
                        cmp              edx, 7
                                                                                        je    .Lx354_53
                        cmp              ecx, 6
                                                                                        jne   .Lx354_50
                        cmp              edx, 6
                                                                                        jne   .Lx354_50
                        mov              rax, qword ptr [r8 + 0]
                        cmp              rax, 6
                                                                                        jne   .Lx354_53
                        mov              rax, qword ptr [r9 + 0]
                        cmp              rax, 6
                                                                                        jne   .Lx354_53
                        mov              rax, qword ptr [r8 + 8]
                        mov              rsi, qword ptr [r9 + 8]
                        cmp              rax, rsi
                                                                                        je    .Lx354_51
                                                                                        jmp   .Lx354_52
.Lx354_50:
                        mov              rax, qword ptr [r8 + 0]
                        mov              rsi, qword ptr [r9 + 0]
                        cmp              rax, rsi
                                                                                        jne   .Lx354_53
                        mov              rax, qword ptr [r8 + 8]
                        mov              rsi, qword ptr [r9 + 8]
                        cmp              rax, rsi
                                                                                        jne   .Lx354_53
.Lx354_51:
                        mov              rax, qword ptr [r8 + 0]
                        mov              rdx, qword ptr [r8 + 8]
                                                                                        jmp   .Lx354_54
.Lx354_52:
                        mov              eax, 99
                        mov              edx, 0
                                                                                        jmp   .Lx354_54
.Lx354_53:
                        mov              esi, 2
                        call             rt_pl_dop_unify@PLT
.Lx354_54:
                        mov              qword ptr [rbp + 8272], rax
                        mov              qword ptr [rbp + 8280], rdx
                        cmp              eax, 99
                                                                                        je    n14_op11_α
                                                                                        jmp   n22_var_α
n19_op11_β:
                                                                                        jmp   n14_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n20_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8784]
                        mov              qword ptr [rbp + 7776], rax
                        mov              qword ptr [rbp + 7784], rdx
                                                                                        jmp   n23_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n21_op19_α:
                                                                                        jmp   n24_move_label_α
#-----------------------------------------------------------------------------------------------------------------------
n22_var_α:
                        mov              rax, qword ptr [rbp + 8784]
                        mov              qword ptr [rbp + 8240], rax
                        mov              rax, qword ptr [rbp + 8792]
                        mov              qword ptr [rbp + 8248], rax
                                                                                        jmp   n25_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n23_op11_α:
                        mov              rax, qword ptr [rbp + 7760]
                        mov              qword ptr [rbp + 7728], rax
                        mov              rax, qword ptr [rbp + 7768]
                        mov              qword ptr [rbp + 7736], rax
                        mov              rax, qword ptr [rbp + 7776]
                        mov              qword ptr [rbp + 7744], rax
                        mov              rax, qword ptr [rbp + 7784]
                        mov              qword ptr [rbp + 7752], rax
                        lea              rdi, [rbp + 7728]
                        lea              r8, [rbp + 7728]
.Lx360_40:
                        mov              eax, dword ptr [r8 + 0]
                        cmp              eax, 9
                                                                                        jne   .Lx360_56
                        mov              esi, dword ptr [r8 + 4]
                        mov              rax, qword ptr [r8 + 8]
                        test             rax, rax
                                                                                        je    .Lx360_41
                        cmp              esi, 1
                                                                                        jne   .Lx360_55
                        mov              r8, rax
                                                                                        jmp   .Lx360_40
.Lx360_55:
                        cmp              esi, 2
                                                                                        jne   .Lx360_41
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx360_41
                        mov              r8, rax
                                                                                        jmp   .Lx360_40
.Lx360_56:
                        cmp              eax, 13
                                                                                        jne   .Lx360_41
                        mov              rax, qword ptr [r8 + 8]
                        test             rax, rax
                                                                                        je    .Lx360_41
                        cmp              rax, r8
                                                                                        je    .Lx360_41
                        mov              r8, rax
                                                                                        jmp   .Lx360_40
.Lx360_41:
                        lea              r9, [rbp + 7744]
.Lx360_42:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 9
                                                                                        jne   .Lx360_58
                        mov              esi, dword ptr [r9 + 4]
                        mov              rax, qword ptr [r9 + 8]
                        test             rax, rax
                                                                                        je    .Lx360_43
                        cmp              esi, 1
                                                                                        jne   .Lx360_57
                        mov              r9, rax
                                                                                        jmp   .Lx360_42
.Lx360_57:
                        cmp              esi, 2
                                                                                        jne   .Lx360_43
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx360_43
                        mov              r9, rax
                                                                                        jmp   .Lx360_42
.Lx360_58:
                        cmp              eax, 13
                                                                                        jne   .Lx360_43
                        mov              rax, qword ptr [r9 + 8]
                        test             rax, rax
                                                                                        je    .Lx360_43
                        cmp              rax, r9
                                                                                        je    .Lx360_43
                        mov              r9, rax
                                                                                        jmp   .Lx360_42
.Lx360_43:
                        cmp              r8, r9
                                                                                        je    .Lx360_51
                        mov              eax, dword ptr [r8 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx360_44
                        cmp              eax, 99
                                                                                        je    .Lx360_44
                        cmp              eax, 13
                                                                                        jne   .Lx360_45
                        mov              rax, qword ptr [r8 + 8]
                        cmp              rax, r8
                                                                                        je    .Lx360_44
                                                                                        jmp   .Lx360_45
.Lx360_44:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx360_53
                        cmp              eax, 99
                                                                                        je    .Lx360_53
                        cmp              eax, 13
                                                                                        jne   .Lx360_46
                        mov              rax, qword ptr [r9 + 8]
                        cmp              rax, r9
                                                                                        je    .Lx360_53
                                                                                        jmp   .Lx360_46
.Lx360_46:
                        lea              r10, [rip + g_pl_trail]
                        mov              r11, qword ptr [r10 + 0]
                        test             r11, r11
                                                                                        je    .Lx360_53
                        mov              eax, dword ptr [r10 + 32]
                        mov              esi, 24
                        imul             rsi, rax
                        mov              rax, qword ptr [r10 + 24]
                        sub              rax, 24
                        cmp              rsi, rax
                                                                                        ja    .Lx360_53
                        add              r11, rsi
                        mov              qword ptr [r11 + 0], r8
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r11 + 8], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r11 + 16], rax
                        mov              eax, dword ptr [r10 + 32]
                        add              eax, 1
                        mov              dword ptr [r10 + 32], eax
                        mov              rax, qword ptr [r9 + 0]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [r8 + 8], rax
                                                                                        jmp   .Lx360_51
.Lx360_45:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx360_47
                        cmp              eax, 99
                                                                                        je    .Lx360_47
                        cmp              eax, 13
                                                                                        jne   .Lx360_48
                        mov              rax, qword ptr [r9 + 8]
                        cmp              rax, r9
                                                                                        je    .Lx360_47
                                                                                        jmp   .Lx360_48
.Lx360_47:
                        lea              r10, [rip + g_pl_trail]
                        mov              r11, qword ptr [r10 + 0]
                        test             r11, r11
                                                                                        je    .Lx360_53
                        mov              eax, dword ptr [r10 + 32]
                        mov              esi, 24
                        imul             rsi, rax
                        mov              rax, qword ptr [r10 + 24]
                        sub              rax, 24
                        cmp              rsi, rax
                                                                                        ja    .Lx360_53
                        add              r11, rsi
                        mov              qword ptr [r11 + 0], r9
                        mov              rax, qword ptr [r9 + 0]
                        mov              qword ptr [r11 + 8], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [r11 + 16], rax
                        mov              eax, dword ptr [r10 + 32]
                        add              eax, 1
                        mov              dword ptr [r10 + 32], eax
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 8], rax
                                                                                        jmp   .Lx360_51
.Lx360_48:
                        mov              ecx, dword ptr [r8 + 0]
                        mov              edx, dword ptr [r9 + 0]
                        cmp              ecx, 14
                                                                                        jne   .Lx360_49
                        cmp              edx, 14
                                                                                        je    .Lx360_53
                                                                                        jmp   .Lx360_52
.Lx360_49:
                        cmp              edx, 14
                                                                                        je    .Lx360_52
                        cmp              ecx, 7
                                                                                        je    .Lx360_53
                        cmp              edx, 7
                                                                                        je    .Lx360_53
                        cmp              ecx, 6
                                                                                        jne   .Lx360_50
                        cmp              edx, 6
                                                                                        jne   .Lx360_50
                        mov              rax, qword ptr [r8 + 0]
                        cmp              rax, 6
                                                                                        jne   .Lx360_53
                        mov              rax, qword ptr [r9 + 0]
                        cmp              rax, 6
                                                                                        jne   .Lx360_53
                        mov              rax, qword ptr [r8 + 8]
                        mov              rsi, qword ptr [r9 + 8]
                        cmp              rax, rsi
                                                                                        je    .Lx360_51
                                                                                        jmp   .Lx360_52
.Lx360_50:
                        mov              rax, qword ptr [r8 + 0]
                        mov              rsi, qword ptr [r9 + 0]
                        cmp              rax, rsi
                                                                                        jne   .Lx360_53
                        mov              rax, qword ptr [r8 + 8]
                        mov              rsi, qword ptr [r9 + 8]
                        cmp              rax, rsi
                                                                                        jne   .Lx360_53
.Lx360_51:
                        mov              rax, qword ptr [r8 + 0]
                        mov              rdx, qword ptr [r8 + 8]
                                                                                        jmp   .Lx360_54
.Lx360_52:
                        mov              eax, 99
                        mov              edx, 0
                                                                                        jmp   .Lx360_54
.Lx360_53:
                        mov              esi, 2
                        call             rt_pl_dop_unify@PLT
.Lx360_54:
                        mov              qword ptr [rbp + 7712], rax
                        mov              qword ptr [rbp + 7720], rdx
                        cmp              eax, 99
                                                                                        je    n27_op11_α
                                                                                        jmp   n26_var_ref_α
n23_op11_β:
                                                                                        jmp   n27_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n24_move_label_α:
                        lea              rax, [rip + n12_op11_α]
                        mov              qword ptr [rbp + 64], rax
                                                                                        jmp   proc_roman$2F2_γ
#-----------------------------------------------------------------------------------------------------------------------
n25_lit_integer_α:
                        mov              qword ptr [rbp + 8256], 6
                        mov              rax, qword ptr [rip + .Lx363_0]
                        mov              qword ptr [rbp + 8264], rax
                                                                                        jmp   n29_op11_α
.Lx363_0:
                        .quad            1000
#-----------------------------------------------------------------------------------------------------------------------
n26_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 32]
                        mov              qword ptr [rbp + 7680], rax
                        mov              qword ptr [rbp + 7688], rdx
                                                                                        jmp   n30_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n27_op11_α:
                        mov              rax, qword ptr [rbp + 80]
                        mov              qword ptr [rbp + 7168], rax
                        mov              rax, qword ptr [rbp + 88]
                        mov              qword ptr [rbp + 7176], rax
                        lea              rdi, [rbp + 7168]
                        mov              esi, 1
                        call             rt_pl_dop_unwind_nothrow@PLT
                        mov              qword ptr [rbp + 7152], rax
                        mov              qword ptr [rbp + 7160], rdx
                        cmp              eax, 99
                                                                                        je    proc_roman$2F2_ω
                                                                                        jmp   n31_var_ref_α
n27_op11_β:
                                                                                        jmp   proc_roman$2F2_ω
#-----------------------------------------------------------------------------------------------------------------------
n28_disjunction_α:
                                                                                        jmp   qword ptr [rbp + 64]
n28_disjunction_β:
                                                                                        jmp   proc_roman$2F2_ω
#-----------------------------------------------------------------------------------------------------------------------
n29_op11_α:
                        mov              rax, qword ptr [rbp + 8240]
                        mov              qword ptr [rbp + 8208], rax
                        mov              rax, qword ptr [rbp + 8248]
                        mov              qword ptr [rbp + 8216], rax
                        mov              rax, qword ptr [rbp + 8256]
                        mov              qword ptr [rbp + 8224], rax
                        mov              rax, qword ptr [rbp + 8264]
                        mov              qword ptr [rbp + 8232], rax
                        lea              rdi, [rbp + 8208]
                        mov              esi, 2
                        call             rt_pl_dop_cmp_ge@PLT
                        mov              qword ptr [rbp + 8192], rax
                        mov              qword ptr [rbp + 8200], rdx
                        cmp              eax, 99
                                                                                        je    n14_op11_α
                                                                                        jmp   n32_op19_α
n29_op11_β:
                                                                                        jmp   n14_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n30_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8752]
                        mov              qword ptr [rbp + 7696], rax
                        mov              qword ptr [rbp + 7704], rdx
                                                                                        jmp   n33_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n31_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 16]
                        mov              qword ptr [rbp + 7120], rax
                        mov              qword ptr [rbp + 7128], rdx
                                                                                        jmp   n34_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n32_op19_α:
                                                                                        jmp   n35_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n33_op11_α:
                        mov              rax, qword ptr [rbp + 7680]
                        mov              qword ptr [rbp + 7648], rax
                        mov              rax, qword ptr [rbp + 7688]
                        mov              qword ptr [rbp + 7656], rax
                        mov              rax, qword ptr [rbp + 7696]
                        mov              qword ptr [rbp + 7664], rax
                        mov              rax, qword ptr [rbp + 7704]
                        mov              qword ptr [rbp + 7672], rax
                        lea              rdi, [rbp + 7648]
                        lea              r8, [rbp + 7648]
.Lx375_40:
                        mov              eax, dword ptr [r8 + 0]
                        cmp              eax, 9
                                                                                        jne   .Lx375_56
                        mov              esi, dword ptr [r8 + 4]
                        mov              rax, qword ptr [r8 + 8]
                        test             rax, rax
                                                                                        je    .Lx375_41
                        cmp              esi, 1
                                                                                        jne   .Lx375_55
                        mov              r8, rax
                                                                                        jmp   .Lx375_40
.Lx375_55:
                        cmp              esi, 2
                                                                                        jne   .Lx375_41
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx375_41
                        mov              r8, rax
                                                                                        jmp   .Lx375_40
.Lx375_56:
                        cmp              eax, 13
                                                                                        jne   .Lx375_41
                        mov              rax, qword ptr [r8 + 8]
                        test             rax, rax
                                                                                        je    .Lx375_41
                        cmp              rax, r8
                                                                                        je    .Lx375_41
                        mov              r8, rax
                                                                                        jmp   .Lx375_40
.Lx375_41:
                        lea              r9, [rbp + 7664]
.Lx375_42:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 9
                                                                                        jne   .Lx375_58
                        mov              esi, dword ptr [r9 + 4]
                        mov              rax, qword ptr [r9 + 8]
                        test             rax, rax
                                                                                        je    .Lx375_43
                        cmp              esi, 1
                                                                                        jne   .Lx375_57
                        mov              r9, rax
                                                                                        jmp   .Lx375_42
.Lx375_57:
                        cmp              esi, 2
                                                                                        jne   .Lx375_43
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx375_43
                        mov              r9, rax
                                                                                        jmp   .Lx375_42
.Lx375_58:
                        cmp              eax, 13
                                                                                        jne   .Lx375_43
                        mov              rax, qword ptr [r9 + 8]
                        test             rax, rax
                                                                                        je    .Lx375_43
                        cmp              rax, r9
                                                                                        je    .Lx375_43
                        mov              r9, rax
                                                                                        jmp   .Lx375_42
.Lx375_43:
                        cmp              r8, r9
                                                                                        je    .Lx375_51
                        mov              eax, dword ptr [r8 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx375_44
                        cmp              eax, 99
                                                                                        je    .Lx375_44
                        cmp              eax, 13
                                                                                        jne   .Lx375_45
                        mov              rax, qword ptr [r8 + 8]
                        cmp              rax, r8
                                                                                        je    .Lx375_44
                                                                                        jmp   .Lx375_45
.Lx375_44:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx375_53
                        cmp              eax, 99
                                                                                        je    .Lx375_53
                        cmp              eax, 13
                                                                                        jne   .Lx375_46
                        mov              rax, qword ptr [r9 + 8]
                        cmp              rax, r9
                                                                                        je    .Lx375_53
                                                                                        jmp   .Lx375_46
.Lx375_46:
                        lea              r10, [rip + g_pl_trail]
                        mov              r11, qword ptr [r10 + 0]
                        test             r11, r11
                                                                                        je    .Lx375_53
                        mov              eax, dword ptr [r10 + 32]
                        mov              esi, 24
                        imul             rsi, rax
                        mov              rax, qword ptr [r10 + 24]
                        sub              rax, 24
                        cmp              rsi, rax
                                                                                        ja    .Lx375_53
                        add              r11, rsi
                        mov              qword ptr [r11 + 0], r8
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r11 + 8], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r11 + 16], rax
                        mov              eax, dword ptr [r10 + 32]
                        add              eax, 1
                        mov              dword ptr [r10 + 32], eax
                        mov              rax, qword ptr [r9 + 0]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [r8 + 8], rax
                                                                                        jmp   .Lx375_51
.Lx375_45:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx375_47
                        cmp              eax, 99
                                                                                        je    .Lx375_47
                        cmp              eax, 13
                                                                                        jne   .Lx375_48
                        mov              rax, qword ptr [r9 + 8]
                        cmp              rax, r9
                                                                                        je    .Lx375_47
                                                                                        jmp   .Lx375_48
.Lx375_47:
                        lea              r10, [rip + g_pl_trail]
                        mov              r11, qword ptr [r10 + 0]
                        test             r11, r11
                                                                                        je    .Lx375_53
                        mov              eax, dword ptr [r10 + 32]
                        mov              esi, 24
                        imul             rsi, rax
                        mov              rax, qword ptr [r10 + 24]
                        sub              rax, 24
                        cmp              rsi, rax
                                                                                        ja    .Lx375_53
                        add              r11, rsi
                        mov              qword ptr [r11 + 0], r9
                        mov              rax, qword ptr [r9 + 0]
                        mov              qword ptr [r11 + 8], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [r11 + 16], rax
                        mov              eax, dword ptr [r10 + 32]
                        add              eax, 1
                        mov              dword ptr [r10 + 32], eax
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 8], rax
                                                                                        jmp   .Lx375_51
.Lx375_48:
                        mov              ecx, dword ptr [r8 + 0]
                        mov              edx, dword ptr [r9 + 0]
                        cmp              ecx, 14
                                                                                        jne   .Lx375_49
                        cmp              edx, 14
                                                                                        je    .Lx375_53
                                                                                        jmp   .Lx375_52
.Lx375_49:
                        cmp              edx, 14
                                                                                        je    .Lx375_52
                        cmp              ecx, 7
                                                                                        je    .Lx375_53
                        cmp              edx, 7
                                                                                        je    .Lx375_53
                        cmp              ecx, 6
                                                                                        jne   .Lx375_50
                        cmp              edx, 6
                                                                                        jne   .Lx375_50
                        mov              rax, qword ptr [r8 + 0]
                        cmp              rax, 6
                                                                                        jne   .Lx375_53
                        mov              rax, qword ptr [r9 + 0]
                        cmp              rax, 6
                                                                                        jne   .Lx375_53
                        mov              rax, qword ptr [r8 + 8]
                        mov              rsi, qword ptr [r9 + 8]
                        cmp              rax, rsi
                                                                                        je    .Lx375_51
                                                                                        jmp   .Lx375_52
.Lx375_50:
                        mov              rax, qword ptr [r8 + 0]
                        mov              rsi, qword ptr [r9 + 0]
                        cmp              rax, rsi
                                                                                        jne   .Lx375_53
                        mov              rax, qword ptr [r8 + 8]
                        mov              rsi, qword ptr [r9 + 8]
                        cmp              rax, rsi
                                                                                        jne   .Lx375_53
.Lx375_51:
                        mov              rax, qword ptr [r8 + 0]
                        mov              rdx, qword ptr [r8 + 8]
                                                                                        jmp   .Lx375_54
.Lx375_52:
                        mov              eax, 99
                        mov              edx, 0
                                                                                        jmp   .Lx375_54
.Lx375_53:
                        mov              esi, 2
                        call             rt_pl_dop_unify@PLT
.Lx375_54:
                        mov              qword ptr [rbp + 7632], rax
                        mov              qword ptr [rbp + 7640], rdx
                        cmp              eax, 99
                                                                                        je    n27_op11_α
                                                                                        jmp   n36_var_α
n33_op11_β:
                                                                                        jmp   n27_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n34_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8784]
                        mov              qword ptr [rbp + 7136], rax
                        mov              qword ptr [rbp + 7144], rdx
                                                                                        jmp   n37_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n35_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8768]
                        mov              qword ptr [rbp + 8080], rax
                        mov              qword ptr [rbp + 8088], rdx
                                                                                        jmp   n38_var_α
#-----------------------------------------------------------------------------------------------------------------------
n36_var_α:
                        mov              rax, qword ptr [rbp + 8784]
                        mov              qword ptr [rbp + 7600], rax
                        mov              rax, qword ptr [rbp + 8792]
                        mov              qword ptr [rbp + 7608], rax
                                                                                        jmp   n39_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n37_op11_α:
                        mov              rax, qword ptr [rbp + 7120]
                        mov              qword ptr [rbp + 7088], rax
                        mov              rax, qword ptr [rbp + 7128]
                        mov              qword ptr [rbp + 7096], rax
                        mov              rax, qword ptr [rbp + 7136]
                        mov              qword ptr [rbp + 7104], rax
                        mov              rax, qword ptr [rbp + 7144]
                        mov              qword ptr [rbp + 7112], rax
                        lea              rdi, [rbp + 7088]
                        lea              r8, [rbp + 7088]
.Lx382_40:
                        mov              eax, dword ptr [r8 + 0]
                        cmp              eax, 9
                                                                                        jne   .Lx382_56
                        mov              esi, dword ptr [r8 + 4]
                        mov              rax, qword ptr [r8 + 8]
                        test             rax, rax
                                                                                        je    .Lx382_41
                        cmp              esi, 1
                                                                                        jne   .Lx382_55
                        mov              r8, rax
                                                                                        jmp   .Lx382_40
.Lx382_55:
                        cmp              esi, 2
                                                                                        jne   .Lx382_41
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx382_41
                        mov              r8, rax
                                                                                        jmp   .Lx382_40
.Lx382_56:
                        cmp              eax, 13
                                                                                        jne   .Lx382_41
                        mov              rax, qword ptr [r8 + 8]
                        test             rax, rax
                                                                                        je    .Lx382_41
                        cmp              rax, r8
                                                                                        je    .Lx382_41
                        mov              r8, rax
                                                                                        jmp   .Lx382_40
.Lx382_41:
                        lea              r9, [rbp + 7104]
.Lx382_42:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 9
                                                                                        jne   .Lx382_58
                        mov              esi, dword ptr [r9 + 4]
                        mov              rax, qword ptr [r9 + 8]
                        test             rax, rax
                                                                                        je    .Lx382_43
                        cmp              esi, 1
                                                                                        jne   .Lx382_57
                        mov              r9, rax
                                                                                        jmp   .Lx382_42
.Lx382_57:
                        cmp              esi, 2
                                                                                        jne   .Lx382_43
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx382_43
                        mov              r9, rax
                                                                                        jmp   .Lx382_42
.Lx382_58:
                        cmp              eax, 13
                                                                                        jne   .Lx382_43
                        mov              rax, qword ptr [r9 + 8]
                        test             rax, rax
                                                                                        je    .Lx382_43
                        cmp              rax, r9
                                                                                        je    .Lx382_43
                        mov              r9, rax
                                                                                        jmp   .Lx382_42
.Lx382_43:
                        cmp              r8, r9
                                                                                        je    .Lx382_51
                        mov              eax, dword ptr [r8 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx382_44
                        cmp              eax, 99
                                                                                        je    .Lx382_44
                        cmp              eax, 13
                                                                                        jne   .Lx382_45
                        mov              rax, qword ptr [r8 + 8]
                        cmp              rax, r8
                                                                                        je    .Lx382_44
                                                                                        jmp   .Lx382_45
.Lx382_44:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx382_53
                        cmp              eax, 99
                                                                                        je    .Lx382_53
                        cmp              eax, 13
                                                                                        jne   .Lx382_46
                        mov              rax, qword ptr [r9 + 8]
                        cmp              rax, r9
                                                                                        je    .Lx382_53
                                                                                        jmp   .Lx382_46
.Lx382_46:
                        lea              r10, [rip + g_pl_trail]
                        mov              r11, qword ptr [r10 + 0]
                        test             r11, r11
                                                                                        je    .Lx382_53
                        mov              eax, dword ptr [r10 + 32]
                        mov              esi, 24
                        imul             rsi, rax
                        mov              rax, qword ptr [r10 + 24]
                        sub              rax, 24
                        cmp              rsi, rax
                                                                                        ja    .Lx382_53
                        add              r11, rsi
                        mov              qword ptr [r11 + 0], r8
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r11 + 8], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r11 + 16], rax
                        mov              eax, dword ptr [r10 + 32]
                        add              eax, 1
                        mov              dword ptr [r10 + 32], eax
                        mov              rax, qword ptr [r9 + 0]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [r8 + 8], rax
                                                                                        jmp   .Lx382_51
.Lx382_45:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx382_47
                        cmp              eax, 99
                                                                                        je    .Lx382_47
                        cmp              eax, 13
                                                                                        jne   .Lx382_48
                        mov              rax, qword ptr [r9 + 8]
                        cmp              rax, r9
                                                                                        je    .Lx382_47
                                                                                        jmp   .Lx382_48
.Lx382_47:
                        lea              r10, [rip + g_pl_trail]
                        mov              r11, qword ptr [r10 + 0]
                        test             r11, r11
                                                                                        je    .Lx382_53
                        mov              eax, dword ptr [r10 + 32]
                        mov              esi, 24
                        imul             rsi, rax
                        mov              rax, qword ptr [r10 + 24]
                        sub              rax, 24
                        cmp              rsi, rax
                                                                                        ja    .Lx382_53
                        add              r11, rsi
                        mov              qword ptr [r11 + 0], r9
                        mov              rax, qword ptr [r9 + 0]
                        mov              qword ptr [r11 + 8], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [r11 + 16], rax
                        mov              eax, dword ptr [r10 + 32]
                        add              eax, 1
                        mov              dword ptr [r10 + 32], eax
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 8], rax
                                                                                        jmp   .Lx382_51
.Lx382_48:
                        mov              ecx, dword ptr [r8 + 0]
                        mov              edx, dword ptr [r9 + 0]
                        cmp              ecx, 14
                                                                                        jne   .Lx382_49
                        cmp              edx, 14
                                                                                        je    .Lx382_53
                                                                                        jmp   .Lx382_52
.Lx382_49:
                        cmp              edx, 14
                                                                                        je    .Lx382_52
                        cmp              ecx, 7
                                                                                        je    .Lx382_53
                        cmp              edx, 7
                                                                                        je    .Lx382_53
                        cmp              ecx, 6
                                                                                        jne   .Lx382_50
                        cmp              edx, 6
                                                                                        jne   .Lx382_50
                        mov              rax, qword ptr [r8 + 0]
                        cmp              rax, 6
                                                                                        jne   .Lx382_53
                        mov              rax, qword ptr [r9 + 0]
                        cmp              rax, 6
                                                                                        jne   .Lx382_53
                        mov              rax, qword ptr [r8 + 8]
                        mov              rsi, qword ptr [r9 + 8]
                        cmp              rax, rsi
                                                                                        je    .Lx382_51
                                                                                        jmp   .Lx382_52
.Lx382_50:
                        mov              rax, qword ptr [r8 + 0]
                        mov              rsi, qword ptr [r9 + 0]
                        cmp              rax, rsi
                                                                                        jne   .Lx382_53
                        mov              rax, qword ptr [r8 + 8]
                        mov              rsi, qword ptr [r9 + 8]
                        cmp              rax, rsi
                                                                                        jne   .Lx382_53
.Lx382_51:
                        mov              rax, qword ptr [r8 + 0]
                        mov              rdx, qword ptr [r8 + 8]
                                                                                        jmp   .Lx382_54
.Lx382_52:
                        mov              eax, 99
                        mov              edx, 0
                                                                                        jmp   .Lx382_54
.Lx382_53:
                        mov              esi, 2
                        call             rt_pl_dop_unify@PLT
.Lx382_54:
                        mov              qword ptr [rbp + 7072], rax
                        mov              qword ptr [rbp + 7080], rdx
                        cmp              eax, 99
                                                                                        je    n41_op11_α
                                                                                        jmp   n40_var_ref_α
n37_op11_β:
                                                                                        jmp   n41_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n38_var_α:
                        mov              rax, qword ptr [rbp + 8784]
                        mov              qword ptr [rbp + 8160], rax
                        mov              rax, qword ptr [rbp + 8792]
                        mov              qword ptr [rbp + 8168], rax
                                                                                        jmp   n42_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n39_lit_integer_α:
                        mov              qword ptr [rbp + 7616], 6
                        mov              rax, qword ptr [rip + .Lx385_0]
                        mov              qword ptr [rbp + 7624], rax
                                                                                        jmp   n43_op11_α
.Lx385_0:
                        .quad            900
#-----------------------------------------------------------------------------------------------------------------------
n40_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 32]
                        mov              qword ptr [rbp + 7040], rax
                        mov              qword ptr [rbp + 7048], rdx
                                                                                        jmp   n44_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n41_op11_α:
                        mov              rax, qword ptr [rbp + 80]
                        mov              qword ptr [rbp + 6528], rax
                        mov              rax, qword ptr [rbp + 88]
                        mov              qword ptr [rbp + 6536], rax
                        lea              rdi, [rbp + 6528]
                        mov              esi, 1
                        call             rt_pl_dop_unwind_nothrow@PLT
                        mov              qword ptr [rbp + 6512], rax
                        mov              qword ptr [rbp + 6520], rdx
                        cmp              eax, 99
                                                                                        je    proc_roman$2F2_ω
                                                                                        jmp   n45_var_ref_α
n41_op11_β:
                                                                                        jmp   proc_roman$2F2_ω
#-----------------------------------------------------------------------------------------------------------------------
n42_lit_integer_α:
                        mov              qword ptr [rbp + 8176], 6
                        mov              rax, qword ptr [rip + .Lx389_0]
                        mov              qword ptr [rbp + 8184], rax
                                                                                        jmp   n46_op11_α
.Lx389_0:
                        .quad            1000
#-----------------------------------------------------------------------------------------------------------------------
n43_op11_α:
                        mov              rax, qword ptr [rbp + 7600]
                        mov              qword ptr [rbp + 7568], rax
                        mov              rax, qword ptr [rbp + 7608]
                        mov              qword ptr [rbp + 7576], rax
                        mov              rax, qword ptr [rbp + 7616]
                        mov              qword ptr [rbp + 7584], rax
                        mov              rax, qword ptr [rbp + 7624]
                        mov              qword ptr [rbp + 7592], rax
                        lea              rdi, [rbp + 7568]
                        mov              esi, 2
                        call             rt_pl_dop_cmp_ge@PLT
                        mov              qword ptr [rbp + 7552], rax
                        mov              qword ptr [rbp + 7560], rdx
                        cmp              eax, 99
                                                                                        je    n27_op11_α
                                                                                        jmp   n47_op19_α
n43_op11_β:
                                                                                        jmp   n27_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n44_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8752]
                        mov              qword ptr [rbp + 7056], rax
                        mov              qword ptr [rbp + 7064], rdx
                                                                                        jmp   n48_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n45_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 16]
                        mov              qword ptr [rbp + 6480], rax
                        mov              qword ptr [rbp + 6488], rdx
                                                                                        jmp   n49_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n46_op11_α:
                        mov              rax, qword ptr [rbp + 8160]
                        mov              qword ptr [rbp + 8112], rax
                        mov              rax, qword ptr [rbp + 8168]
                        mov              qword ptr [rbp + 8120], rax
                        mov              rax, qword ptr [rbp + 8176]
                        mov              qword ptr [rbp + 8128], rax
                        mov              rax, qword ptr [rbp + 8184]
                        mov              qword ptr [rbp + 8136], rax
                        lea              rdi, [rbp + 8112]
                        mov              esi, 2
                        call             rt_pl_dop_ax_sub@PLT
                        mov              qword ptr [rbp + 8096], rax
                        mov              qword ptr [rbp + 8104], rdx
                        cmp              eax, 99
                                                                                        je    n51_op11_α
                                                                                        jmp   n50_op11_α
n46_op11_β:
                                                                                        jmp   n51_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n47_op19_α:
                                                                                        jmp   n52_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n48_op11_α:
                        mov              rax, qword ptr [rbp + 7040]
                        mov              qword ptr [rbp + 7008], rax
                        mov              rax, qword ptr [rbp + 7048]
                        mov              qword ptr [rbp + 7016], rax
                        mov              rax, qword ptr [rbp + 7056]
                        mov              qword ptr [rbp + 7024], rax
                        mov              rax, qword ptr [rbp + 7064]
                        mov              qword ptr [rbp + 7032], rax
                        lea              rdi, [rbp + 7008]
                        lea              r8, [rbp + 7008]
.Lx397_40:
                        mov              eax, dword ptr [r8 + 0]
                        cmp              eax, 9
                                                                                        jne   .Lx397_56
                        mov              esi, dword ptr [r8 + 4]
                        mov              rax, qword ptr [r8 + 8]
                        test             rax, rax
                                                                                        je    .Lx397_41
                        cmp              esi, 1
                                                                                        jne   .Lx397_55
                        mov              r8, rax
                                                                                        jmp   .Lx397_40
.Lx397_55:
                        cmp              esi, 2
                                                                                        jne   .Lx397_41
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx397_41
                        mov              r8, rax
                                                                                        jmp   .Lx397_40
.Lx397_56:
                        cmp              eax, 13
                                                                                        jne   .Lx397_41
                        mov              rax, qword ptr [r8 + 8]
                        test             rax, rax
                                                                                        je    .Lx397_41
                        cmp              rax, r8
                                                                                        je    .Lx397_41
                        mov              r8, rax
                                                                                        jmp   .Lx397_40
.Lx397_41:
                        lea              r9, [rbp + 7024]
.Lx397_42:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 9
                                                                                        jne   .Lx397_58
                        mov              esi, dword ptr [r9 + 4]
                        mov              rax, qword ptr [r9 + 8]
                        test             rax, rax
                                                                                        je    .Lx397_43
                        cmp              esi, 1
                                                                                        jne   .Lx397_57
                        mov              r9, rax
                                                                                        jmp   .Lx397_42
.Lx397_57:
                        cmp              esi, 2
                                                                                        jne   .Lx397_43
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx397_43
                        mov              r9, rax
                                                                                        jmp   .Lx397_42
.Lx397_58:
                        cmp              eax, 13
                                                                                        jne   .Lx397_43
                        mov              rax, qword ptr [r9 + 8]
                        test             rax, rax
                                                                                        je    .Lx397_43
                        cmp              rax, r9
                                                                                        je    .Lx397_43
                        mov              r9, rax
                                                                                        jmp   .Lx397_42
.Lx397_43:
                        cmp              r8, r9
                                                                                        je    .Lx397_51
                        mov              eax, dword ptr [r8 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx397_44
                        cmp              eax, 99
                                                                                        je    .Lx397_44
                        cmp              eax, 13
                                                                                        jne   .Lx397_45
                        mov              rax, qword ptr [r8 + 8]
                        cmp              rax, r8
                                                                                        je    .Lx397_44
                                                                                        jmp   .Lx397_45
.Lx397_44:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx397_53
                        cmp              eax, 99
                                                                                        je    .Lx397_53
                        cmp              eax, 13
                                                                                        jne   .Lx397_46
                        mov              rax, qword ptr [r9 + 8]
                        cmp              rax, r9
                                                                                        je    .Lx397_53
                                                                                        jmp   .Lx397_46
.Lx397_46:
                        lea              r10, [rip + g_pl_trail]
                        mov              r11, qword ptr [r10 + 0]
                        test             r11, r11
                                                                                        je    .Lx397_53
                        mov              eax, dword ptr [r10 + 32]
                        mov              esi, 24
                        imul             rsi, rax
                        mov              rax, qword ptr [r10 + 24]
                        sub              rax, 24
                        cmp              rsi, rax
                                                                                        ja    .Lx397_53
                        add              r11, rsi
                        mov              qword ptr [r11 + 0], r8
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r11 + 8], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r11 + 16], rax
                        mov              eax, dword ptr [r10 + 32]
                        add              eax, 1
                        mov              dword ptr [r10 + 32], eax
                        mov              rax, qword ptr [r9 + 0]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [r8 + 8], rax
                                                                                        jmp   .Lx397_51
.Lx397_45:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx397_47
                        cmp              eax, 99
                                                                                        je    .Lx397_47
                        cmp              eax, 13
                                                                                        jne   .Lx397_48
                        mov              rax, qword ptr [r9 + 8]
                        cmp              rax, r9
                                                                                        je    .Lx397_47
                                                                                        jmp   .Lx397_48
.Lx397_47:
                        lea              r10, [rip + g_pl_trail]
                        mov              r11, qword ptr [r10 + 0]
                        test             r11, r11
                                                                                        je    .Lx397_53
                        mov              eax, dword ptr [r10 + 32]
                        mov              esi, 24
                        imul             rsi, rax
                        mov              rax, qword ptr [r10 + 24]
                        sub              rax, 24
                        cmp              rsi, rax
                                                                                        ja    .Lx397_53
                        add              r11, rsi
                        mov              qword ptr [r11 + 0], r9
                        mov              rax, qword ptr [r9 + 0]
                        mov              qword ptr [r11 + 8], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [r11 + 16], rax
                        mov              eax, dword ptr [r10 + 32]
                        add              eax, 1
                        mov              dword ptr [r10 + 32], eax
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 8], rax
                                                                                        jmp   .Lx397_51
.Lx397_48:
                        mov              ecx, dword ptr [r8 + 0]
                        mov              edx, dword ptr [r9 + 0]
                        cmp              ecx, 14
                                                                                        jne   .Lx397_49
                        cmp              edx, 14
                                                                                        je    .Lx397_53
                                                                                        jmp   .Lx397_52
.Lx397_49:
                        cmp              edx, 14
                                                                                        je    .Lx397_52
                        cmp              ecx, 7
                                                                                        je    .Lx397_53
                        cmp              edx, 7
                                                                                        je    .Lx397_53
                        cmp              ecx, 6
                                                                                        jne   .Lx397_50
                        cmp              edx, 6
                                                                                        jne   .Lx397_50
                        mov              rax, qword ptr [r8 + 0]
                        cmp              rax, 6
                                                                                        jne   .Lx397_53
                        mov              rax, qword ptr [r9 + 0]
                        cmp              rax, 6
                                                                                        jne   .Lx397_53
                        mov              rax, qword ptr [r8 + 8]
                        mov              rsi, qword ptr [r9 + 8]
                        cmp              rax, rsi
                                                                                        je    .Lx397_51
                                                                                        jmp   .Lx397_52
.Lx397_50:
                        mov              rax, qword ptr [r8 + 0]
                        mov              rsi, qword ptr [r9 + 0]
                        cmp              rax, rsi
                                                                                        jne   .Lx397_53
                        mov              rax, qword ptr [r8 + 8]
                        mov              rsi, qword ptr [r9 + 8]
                        cmp              rax, rsi
                                                                                        jne   .Lx397_53
.Lx397_51:
                        mov              rax, qword ptr [r8 + 0]
                        mov              rdx, qword ptr [r8 + 8]
                                                                                        jmp   .Lx397_54
.Lx397_52:
                        mov              eax, 99
                        mov              edx, 0
                                                                                        jmp   .Lx397_54
.Lx397_53:
                        mov              esi, 2
                        call             rt_pl_dop_unify@PLT
.Lx397_54:
                        mov              qword ptr [rbp + 6992], rax
                        mov              qword ptr [rbp + 7000], rdx
                        cmp              eax, 99
                                                                                        je    n41_op11_α
                                                                                        jmp   n53_var_α
n48_op11_β:
                                                                                        jmp   n41_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n49_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8784]
                        mov              qword ptr [rbp + 6496], rax
                        mov              qword ptr [rbp + 6504], rdx
                                                                                        jmp   n54_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n50_op11_α:
                        mov              rax, qword ptr [rbp + 8080]
                        mov              qword ptr [rbp + 8048], rax
                        mov              rax, qword ptr [rbp + 8088]
                        mov              qword ptr [rbp + 8056], rax
                        mov              rax, qword ptr [rbp + 8096]
                        mov              qword ptr [rbp + 8064], rax
                        mov              rax, qword ptr [rbp + 8104]
                        mov              qword ptr [rbp + 8072], rax
                        lea              rdi, [rbp + 8048]
                        mov              esi, 2
                        call             rt_pl_dop_is_v@PLT
                        mov              qword ptr [rbp + 8032], rax
                        mov              qword ptr [rbp + 8040], rdx
                        cmp              eax, 99
                                                                                        je    n51_op11_α
                                                                                        jmp   n55_var_ref_α
n50_op11_β:
                                                                                        jmp   n51_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n51_op11_α:
                        mov              rax, qword ptr [rbp + 80]
                        mov              qword ptr [rbp + 128], rax
                        mov              rax, qword ptr [rbp + 88]
                        mov              qword ptr [rbp + 136], rax
                        lea              rdi, [rbp + 128]
                        mov              esi, 1
                        call             rt_pl_dop_trail_unwind@PLT
                        mov              qword ptr [rbp + 112], rax
                        mov              qword ptr [rbp + 120], rdx
                        cmp              eax, 99
                                                                                        je    proc_roman$2F2_ω
                                                                                        jmp   proc_roman$2F2_ω
n51_op11_β:
                                                                                        jmp   proc_roman$2F2_ω
#-----------------------------------------------------------------------------------------------------------------------
n52_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8768]
                        mov              qword ptr [rbp + 7440], rax
                        mov              qword ptr [rbp + 7448], rdx
                                                                                        jmp   n56_var_α
#-----------------------------------------------------------------------------------------------------------------------
n53_var_α:
                        mov              rax, qword ptr [rbp + 8784]
                        mov              qword ptr [rbp + 6960], rax
                        mov              rax, qword ptr [rbp + 8792]
                        mov              qword ptr [rbp + 6968], rax
                                                                                        jmp   n57_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n54_op11_α:
                        mov              rax, qword ptr [rbp + 6480]
                        mov              qword ptr [rbp + 6448], rax
                        mov              rax, qword ptr [rbp + 6488]
                        mov              qword ptr [rbp + 6456], rax
                        mov              rax, qword ptr [rbp + 6496]
                        mov              qword ptr [rbp + 6464], rax
                        mov              rax, qword ptr [rbp + 6504]
                        mov              qword ptr [rbp + 6472], rax
                        lea              rdi, [rbp + 6448]
                        lea              r8, [rbp + 6448]
.Lx406_40:
                        mov              eax, dword ptr [r8 + 0]
                        cmp              eax, 9
                                                                                        jne   .Lx406_56
                        mov              esi, dword ptr [r8 + 4]
                        mov              rax, qword ptr [r8 + 8]
                        test             rax, rax
                                                                                        je    .Lx406_41
                        cmp              esi, 1
                                                                                        jne   .Lx406_55
                        mov              r8, rax
                                                                                        jmp   .Lx406_40
.Lx406_55:
                        cmp              esi, 2
                                                                                        jne   .Lx406_41
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx406_41
                        mov              r8, rax
                                                                                        jmp   .Lx406_40
.Lx406_56:
                        cmp              eax, 13
                                                                                        jne   .Lx406_41
                        mov              rax, qword ptr [r8 + 8]
                        test             rax, rax
                                                                                        je    .Lx406_41
                        cmp              rax, r8
                                                                                        je    .Lx406_41
                        mov              r8, rax
                                                                                        jmp   .Lx406_40
.Lx406_41:
                        lea              r9, [rbp + 6464]
.Lx406_42:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 9
                                                                                        jne   .Lx406_58
                        mov              esi, dword ptr [r9 + 4]
                        mov              rax, qword ptr [r9 + 8]
                        test             rax, rax
                                                                                        je    .Lx406_43
                        cmp              esi, 1
                                                                                        jne   .Lx406_57
                        mov              r9, rax
                                                                                        jmp   .Lx406_42
.Lx406_57:
                        cmp              esi, 2
                                                                                        jne   .Lx406_43
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx406_43
                        mov              r9, rax
                                                                                        jmp   .Lx406_42
.Lx406_58:
                        cmp              eax, 13
                                                                                        jne   .Lx406_43
                        mov              rax, qword ptr [r9 + 8]
                        test             rax, rax
                                                                                        je    .Lx406_43
                        cmp              rax, r9
                                                                                        je    .Lx406_43
                        mov              r9, rax
                                                                                        jmp   .Lx406_42
.Lx406_43:
                        cmp              r8, r9
                                                                                        je    .Lx406_51
                        mov              eax, dword ptr [r8 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx406_44
                        cmp              eax, 99
                                                                                        je    .Lx406_44
                        cmp              eax, 13
                                                                                        jne   .Lx406_45
                        mov              rax, qword ptr [r8 + 8]
                        cmp              rax, r8
                                                                                        je    .Lx406_44
                                                                                        jmp   .Lx406_45
.Lx406_44:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx406_53
                        cmp              eax, 99
                                                                                        je    .Lx406_53
                        cmp              eax, 13
                                                                                        jne   .Lx406_46
                        mov              rax, qword ptr [r9 + 8]
                        cmp              rax, r9
                                                                                        je    .Lx406_53
                                                                                        jmp   .Lx406_46
.Lx406_46:
                        lea              r10, [rip + g_pl_trail]
                        mov              r11, qword ptr [r10 + 0]
                        test             r11, r11
                                                                                        je    .Lx406_53
                        mov              eax, dword ptr [r10 + 32]
                        mov              esi, 24
                        imul             rsi, rax
                        mov              rax, qword ptr [r10 + 24]
                        sub              rax, 24
                        cmp              rsi, rax
                                                                                        ja    .Lx406_53
                        add              r11, rsi
                        mov              qword ptr [r11 + 0], r8
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r11 + 8], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r11 + 16], rax
                        mov              eax, dword ptr [r10 + 32]
                        add              eax, 1
                        mov              dword ptr [r10 + 32], eax
                        mov              rax, qword ptr [r9 + 0]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [r8 + 8], rax
                                                                                        jmp   .Lx406_51
.Lx406_45:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx406_47
                        cmp              eax, 99
                                                                                        je    .Lx406_47
                        cmp              eax, 13
                                                                                        jne   .Lx406_48
                        mov              rax, qword ptr [r9 + 8]
                        cmp              rax, r9
                                                                                        je    .Lx406_47
                                                                                        jmp   .Lx406_48
.Lx406_47:
                        lea              r10, [rip + g_pl_trail]
                        mov              r11, qword ptr [r10 + 0]
                        test             r11, r11
                                                                                        je    .Lx406_53
                        mov              eax, dword ptr [r10 + 32]
                        mov              esi, 24
                        imul             rsi, rax
                        mov              rax, qword ptr [r10 + 24]
                        sub              rax, 24
                        cmp              rsi, rax
                                                                                        ja    .Lx406_53
                        add              r11, rsi
                        mov              qword ptr [r11 + 0], r9
                        mov              rax, qword ptr [r9 + 0]
                        mov              qword ptr [r11 + 8], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [r11 + 16], rax
                        mov              eax, dword ptr [r10 + 32]
                        add              eax, 1
                        mov              dword ptr [r10 + 32], eax
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 8], rax
                                                                                        jmp   .Lx406_51
.Lx406_48:
                        mov              ecx, dword ptr [r8 + 0]
                        mov              edx, dword ptr [r9 + 0]
                        cmp              ecx, 14
                                                                                        jne   .Lx406_49
                        cmp              edx, 14
                                                                                        je    .Lx406_53
                                                                                        jmp   .Lx406_52
.Lx406_49:
                        cmp              edx, 14
                                                                                        je    .Lx406_52
                        cmp              ecx, 7
                                                                                        je    .Lx406_53
                        cmp              edx, 7
                                                                                        je    .Lx406_53
                        cmp              ecx, 6
                                                                                        jne   .Lx406_50
                        cmp              edx, 6
                                                                                        jne   .Lx406_50
                        mov              rax, qword ptr [r8 + 0]
                        cmp              rax, 6
                                                                                        jne   .Lx406_53
                        mov              rax, qword ptr [r9 + 0]
                        cmp              rax, 6
                                                                                        jne   .Lx406_53
                        mov              rax, qword ptr [r8 + 8]
                        mov              rsi, qword ptr [r9 + 8]
                        cmp              rax, rsi
                                                                                        je    .Lx406_51
                                                                                        jmp   .Lx406_52
.Lx406_50:
                        mov              rax, qword ptr [r8 + 0]
                        mov              rsi, qword ptr [r9 + 0]
                        cmp              rax, rsi
                                                                                        jne   .Lx406_53
                        mov              rax, qword ptr [r8 + 8]
                        mov              rsi, qword ptr [r9 + 8]
                        cmp              rax, rsi
                                                                                        jne   .Lx406_53
.Lx406_51:
                        mov              rax, qword ptr [r8 + 0]
                        mov              rdx, qword ptr [r8 + 8]
                                                                                        jmp   .Lx406_54
.Lx406_52:
                        mov              eax, 99
                        mov              edx, 0
                                                                                        jmp   .Lx406_54
.Lx406_53:
                        mov              esi, 2
                        call             rt_pl_dop_unify@PLT
.Lx406_54:
                        mov              qword ptr [rbp + 6432], rax
                        mov              qword ptr [rbp + 6440], rdx
                        cmp              eax, 99
                                                                                        je    n59_op11_α
                                                                                        jmp   n58_var_ref_α
n54_op11_β:
                                                                                        jmp   n59_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n55_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8768]
                        mov              qword ptr [rbp + 8000], rax
                        mov              qword ptr [rbp + 8008], rdx
                                                                                        jmp   n60_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n56_var_α:
                        mov              rax, qword ptr [rbp + 8784]
                        mov              qword ptr [rbp + 7520], rax
                        mov              rax, qword ptr [rbp + 8792]
                        mov              qword ptr [rbp + 7528], rax
                                                                                        jmp   n61_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n57_lit_integer_α:
                        mov              qword ptr [rbp + 6976], 6
                        mov              rax, qword ptr [rip + .Lx411_0]
                        mov              qword ptr [rbp + 6984], rax
                                                                                        jmp   n62_op11_α
.Lx411_0:
                        .quad            500
#-----------------------------------------------------------------------------------------------------------------------
n58_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 32]
                        mov              qword ptr [rbp + 6400], rax
                        mov              qword ptr [rbp + 6408], rdx
                                                                                        jmp   n63_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n59_op11_α:
                        mov              rax, qword ptr [rbp + 80]
                        mov              qword ptr [rbp + 5888], rax
                        mov              rax, qword ptr [rbp + 88]
                        mov              qword ptr [rbp + 5896], rax
                        lea              rdi, [rbp + 5888]
                        mov              esi, 1
                        call             rt_pl_dop_unwind_nothrow@PLT
                        mov              qword ptr [rbp + 5872], rax
                        mov              qword ptr [rbp + 5880], rdx
                        cmp              eax, 99
                                                                                        je    proc_roman$2F2_ω
                                                                                        jmp   n64_var_ref_α
n59_op11_β:
                                                                                        jmp   proc_roman$2F2_ω
#-----------------------------------------------------------------------------------------------------------------------
n60_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8736]
                        mov              qword ptr [rbp + 8016], rax
                        mov              qword ptr [rbp + 8024], rdx
                                                                                        jmp   n65_call_proc_staged_α
#-----------------------------------------------------------------------------------------------------------------------
n61_lit_integer_α:
                        mov              qword ptr [rbp + 7536], 6
                        mov              rax, qword ptr [rip + .Lx417_0]
                        mov              qword ptr [rbp + 7544], rax
                                                                                        jmp   n66_op11_α
.Lx417_0:
                        .quad            900
#-----------------------------------------------------------------------------------------------------------------------
n62_op11_α:
                        mov              rax, qword ptr [rbp + 6960]
                        mov              qword ptr [rbp + 6928], rax
                        mov              rax, qword ptr [rbp + 6968]
                        mov              qword ptr [rbp + 6936], rax
                        mov              rax, qword ptr [rbp + 6976]
                        mov              qword ptr [rbp + 6944], rax
                        mov              rax, qword ptr [rbp + 6984]
                        mov              qword ptr [rbp + 6952], rax
                        lea              rdi, [rbp + 6928]
                        mov              esi, 2
                        call             rt_pl_dop_cmp_ge@PLT
                        mov              qword ptr [rbp + 6912], rax
                        mov              qword ptr [rbp + 6920], rdx
                        cmp              eax, 99
                                                                                        je    n41_op11_α
                                                                                        jmp   n67_op19_α
n62_op11_β:
                                                                                        jmp   n41_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n63_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8752]
                        mov              qword ptr [rbp + 6416], rax
                        mov              qword ptr [rbp + 6424], rdx
                                                                                        jmp   n68_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n64_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 16]
                        mov              qword ptr [rbp + 5840], rax
                        mov              qword ptr [rbp + 5848], rdx
                                                                                        jmp   n69_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n65_call_proc_staged_α:
                        lea              rsi, [rbp + 8000]
                        lea              rdx, [rbp + 8016]
                        call             proc_roman$2F2_dcα
                                                                                        jmp   .Lx424_2
.Lx424_2:
                        mov              qword ptr [rbp + 7936], rax
                        mov              qword ptr [rbp + 7944], rdx
                        cmp              eax, 99
                                                                                        je    n51_op11_α
                                                                                        jmp   n70_lit_string_α
n65_call_proc_staged_β:
                                                                                        jmp   n51_op11_α
.Lx424_0:
                        .quad            .Lx424_0_s
.Lx424_0_s:
                        .string          "roman/2"
#-----------------------------------------------------------------------------------------------------------------------
n66_op11_α:
                        mov              rax, qword ptr [rbp + 7520]
                        mov              qword ptr [rbp + 7472], rax
                        mov              rax, qword ptr [rbp + 7528]
                        mov              qword ptr [rbp + 7480], rax
                        mov              rax, qword ptr [rbp + 7536]
                        mov              qword ptr [rbp + 7488], rax
                        mov              rax, qword ptr [rbp + 7544]
                        mov              qword ptr [rbp + 7496], rax
                        lea              rdi, [rbp + 7472]
                        mov              esi, 2
                        call             rt_pl_dop_ax_sub@PLT
                        mov              qword ptr [rbp + 7456], rax
                        mov              qword ptr [rbp + 7464], rdx
                        cmp              eax, 99
                                                                                        je    n51_op11_α
                                                                                        jmp   n71_op11_α
n66_op11_β:
                                                                                        jmp   n51_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n67_op19_α:
                                                                                        jmp   n72_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n68_op11_α:
                        mov              rax, qword ptr [rbp + 6400]
                        mov              qword ptr [rbp + 6368], rax
                        mov              rax, qword ptr [rbp + 6408]
                        mov              qword ptr [rbp + 6376], rax
                        mov              rax, qword ptr [rbp + 6416]
                        mov              qword ptr [rbp + 6384], rax
                        mov              rax, qword ptr [rbp + 6424]
                        mov              qword ptr [rbp + 6392], rax
                        lea              rdi, [rbp + 6368]
                        lea              r8, [rbp + 6368]
.Lx427_40:
                        mov              eax, dword ptr [r8 + 0]
                        cmp              eax, 9
                                                                                        jne   .Lx427_56
                        mov              esi, dword ptr [r8 + 4]
                        mov              rax, qword ptr [r8 + 8]
                        test             rax, rax
                                                                                        je    .Lx427_41
                        cmp              esi, 1
                                                                                        jne   .Lx427_55
                        mov              r8, rax
                                                                                        jmp   .Lx427_40
.Lx427_55:
                        cmp              esi, 2
                                                                                        jne   .Lx427_41
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx427_41
                        mov              r8, rax
                                                                                        jmp   .Lx427_40
.Lx427_56:
                        cmp              eax, 13
                                                                                        jne   .Lx427_41
                        mov              rax, qword ptr [r8 + 8]
                        test             rax, rax
                                                                                        je    .Lx427_41
                        cmp              rax, r8
                                                                                        je    .Lx427_41
                        mov              r8, rax
                                                                                        jmp   .Lx427_40
.Lx427_41:
                        lea              r9, [rbp + 6384]
.Lx427_42:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 9
                                                                                        jne   .Lx427_58
                        mov              esi, dword ptr [r9 + 4]
                        mov              rax, qword ptr [r9 + 8]
                        test             rax, rax
                                                                                        je    .Lx427_43
                        cmp              esi, 1
                                                                                        jne   .Lx427_57
                        mov              r9, rax
                                                                                        jmp   .Lx427_42
.Lx427_57:
                        cmp              esi, 2
                                                                                        jne   .Lx427_43
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx427_43
                        mov              r9, rax
                                                                                        jmp   .Lx427_42
.Lx427_58:
                        cmp              eax, 13
                                                                                        jne   .Lx427_43
                        mov              rax, qword ptr [r9 + 8]
                        test             rax, rax
                                                                                        je    .Lx427_43
                        cmp              rax, r9
                                                                                        je    .Lx427_43
                        mov              r9, rax
                                                                                        jmp   .Lx427_42
.Lx427_43:
                        cmp              r8, r9
                                                                                        je    .Lx427_51
                        mov              eax, dword ptr [r8 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx427_44
                        cmp              eax, 99
                                                                                        je    .Lx427_44
                        cmp              eax, 13
                                                                                        jne   .Lx427_45
                        mov              rax, qword ptr [r8 + 8]
                        cmp              rax, r8
                                                                                        je    .Lx427_44
                                                                                        jmp   .Lx427_45
.Lx427_44:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx427_53
                        cmp              eax, 99
                                                                                        je    .Lx427_53
                        cmp              eax, 13
                                                                                        jne   .Lx427_46
                        mov              rax, qword ptr [r9 + 8]
                        cmp              rax, r9
                                                                                        je    .Lx427_53
                                                                                        jmp   .Lx427_46
.Lx427_46:
                        lea              r10, [rip + g_pl_trail]
                        mov              r11, qword ptr [r10 + 0]
                        test             r11, r11
                                                                                        je    .Lx427_53
                        mov              eax, dword ptr [r10 + 32]
                        mov              esi, 24
                        imul             rsi, rax
                        mov              rax, qword ptr [r10 + 24]
                        sub              rax, 24
                        cmp              rsi, rax
                                                                                        ja    .Lx427_53
                        add              r11, rsi
                        mov              qword ptr [r11 + 0], r8
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r11 + 8], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r11 + 16], rax
                        mov              eax, dword ptr [r10 + 32]
                        add              eax, 1
                        mov              dword ptr [r10 + 32], eax
                        mov              rax, qword ptr [r9 + 0]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [r8 + 8], rax
                                                                                        jmp   .Lx427_51
.Lx427_45:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx427_47
                        cmp              eax, 99
                                                                                        je    .Lx427_47
                        cmp              eax, 13
                                                                                        jne   .Lx427_48
                        mov              rax, qword ptr [r9 + 8]
                        cmp              rax, r9
                                                                                        je    .Lx427_47
                                                                                        jmp   .Lx427_48
.Lx427_47:
                        lea              r10, [rip + g_pl_trail]
                        mov              r11, qword ptr [r10 + 0]
                        test             r11, r11
                                                                                        je    .Lx427_53
                        mov              eax, dword ptr [r10 + 32]
                        mov              esi, 24
                        imul             rsi, rax
                        mov              rax, qword ptr [r10 + 24]
                        sub              rax, 24
                        cmp              rsi, rax
                                                                                        ja    .Lx427_53
                        add              r11, rsi
                        mov              qword ptr [r11 + 0], r9
                        mov              rax, qword ptr [r9 + 0]
                        mov              qword ptr [r11 + 8], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [r11 + 16], rax
                        mov              eax, dword ptr [r10 + 32]
                        add              eax, 1
                        mov              dword ptr [r10 + 32], eax
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 8], rax
                                                                                        jmp   .Lx427_51
.Lx427_48:
                        mov              ecx, dword ptr [r8 + 0]
                        mov              edx, dword ptr [r9 + 0]
                        cmp              ecx, 14
                                                                                        jne   .Lx427_49
                        cmp              edx, 14
                                                                                        je    .Lx427_53
                                                                                        jmp   .Lx427_52
.Lx427_49:
                        cmp              edx, 14
                                                                                        je    .Lx427_52
                        cmp              ecx, 7
                                                                                        je    .Lx427_53
                        cmp              edx, 7
                                                                                        je    .Lx427_53
                        cmp              ecx, 6
                                                                                        jne   .Lx427_50
                        cmp              edx, 6
                                                                                        jne   .Lx427_50
                        mov              rax, qword ptr [r8 + 0]
                        cmp              rax, 6
                                                                                        jne   .Lx427_53
                        mov              rax, qword ptr [r9 + 0]
                        cmp              rax, 6
                                                                                        jne   .Lx427_53
                        mov              rax, qword ptr [r8 + 8]
                        mov              rsi, qword ptr [r9 + 8]
                        cmp              rax, rsi
                                                                                        je    .Lx427_51
                                                                                        jmp   .Lx427_52
.Lx427_50:
                        mov              rax, qword ptr [r8 + 0]
                        mov              rsi, qword ptr [r9 + 0]
                        cmp              rax, rsi
                                                                                        jne   .Lx427_53
                        mov              rax, qword ptr [r8 + 8]
                        mov              rsi, qword ptr [r9 + 8]
                        cmp              rax, rsi
                                                                                        jne   .Lx427_53
.Lx427_51:
                        mov              rax, qword ptr [r8 + 0]
                        mov              rdx, qword ptr [r8 + 8]
                                                                                        jmp   .Lx427_54
.Lx427_52:
                        mov              eax, 99
                        mov              edx, 0
                                                                                        jmp   .Lx427_54
.Lx427_53:
                        mov              esi, 2
                        call             rt_pl_dop_unify@PLT
.Lx427_54:
                        mov              qword ptr [rbp + 6352], rax
                        mov              qword ptr [rbp + 6360], rdx
                        cmp              eax, 99
                                                                                        je    n59_op11_α
                                                                                        jmp   n73_var_α
n68_op11_β:
                                                                                        jmp   n59_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n69_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8784]
                        mov              qword ptr [rbp + 5856], rax
                        mov              qword ptr [rbp + 5864], rdx
                                                                                        jmp   n74_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n70_lit_string_α:
                        mov              qword ptr [rbp + 7888], 1
                        mov              rax, qword ptr [rip + .Lx430_0]
                        mov              qword ptr [rbp + 7896], rax
                                                                                        jmp   n75_var_ref_α
.Lx430_0:
                        .quad            .Lx430_0_s
.Lx430_0_s:
                        .string          "M"
#-----------------------------------------------------------------------------------------------------------------------
n71_op11_α:
                        mov              rax, qword ptr [rbp + 7440]
                        mov              qword ptr [rbp + 7408], rax
                        mov              rax, qword ptr [rbp + 7448]
                        mov              qword ptr [rbp + 7416], rax
                        mov              rax, qword ptr [rbp + 7456]
                        mov              qword ptr [rbp + 7424], rax
                        mov              rax, qword ptr [rbp + 7464]
                        mov              qword ptr [rbp + 7432], rax
                        lea              rdi, [rbp + 7408]
                        mov              esi, 2
                        call             rt_pl_dop_is_v@PLT
                        mov              qword ptr [rbp + 7392], rax
                        mov              qword ptr [rbp + 7400], rdx
                        cmp              eax, 99
                                                                                        je    n51_op11_α
                                                                                        jmp   n76_var_ref_α
n71_op11_β:
                                                                                        jmp   n51_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n72_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8768]
                        mov              qword ptr [rbp + 6800], rax
                        mov              qword ptr [rbp + 6808], rdx
                                                                                        jmp   n77_var_α
#-----------------------------------------------------------------------------------------------------------------------
n73_var_α:
                        mov              rax, qword ptr [rbp + 8784]
                        mov              qword ptr [rbp + 6320], rax
                        mov              rax, qword ptr [rbp + 8792]
                        mov              qword ptr [rbp + 6328], rax
                                                                                        jmp   n78_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n74_op11_α:
                        mov              rax, qword ptr [rbp + 5840]
                        mov              qword ptr [rbp + 5808], rax
                        mov              rax, qword ptr [rbp + 5848]
                        mov              qword ptr [rbp + 5816], rax
                        mov              rax, qword ptr [rbp + 5856]
                        mov              qword ptr [rbp + 5824], rax
                        mov              rax, qword ptr [rbp + 5864]
                        mov              qword ptr [rbp + 5832], rax
                        lea              rdi, [rbp + 5808]
                        lea              r8, [rbp + 5808]
.Lx436_40:
                        mov              eax, dword ptr [r8 + 0]
                        cmp              eax, 9
                                                                                        jne   .Lx436_56
                        mov              esi, dword ptr [r8 + 4]
                        mov              rax, qword ptr [r8 + 8]
                        test             rax, rax
                                                                                        je    .Lx436_41
                        cmp              esi, 1
                                                                                        jne   .Lx436_55
                        mov              r8, rax
                                                                                        jmp   .Lx436_40
.Lx436_55:
                        cmp              esi, 2
                                                                                        jne   .Lx436_41
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx436_41
                        mov              r8, rax
                                                                                        jmp   .Lx436_40
.Lx436_56:
                        cmp              eax, 13
                                                                                        jne   .Lx436_41
                        mov              rax, qword ptr [r8 + 8]
                        test             rax, rax
                                                                                        je    .Lx436_41
                        cmp              rax, r8
                                                                                        je    .Lx436_41
                        mov              r8, rax
                                                                                        jmp   .Lx436_40
.Lx436_41:
                        lea              r9, [rbp + 5824]
.Lx436_42:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 9
                                                                                        jne   .Lx436_58
                        mov              esi, dword ptr [r9 + 4]
                        mov              rax, qword ptr [r9 + 8]
                        test             rax, rax
                                                                                        je    .Lx436_43
                        cmp              esi, 1
                                                                                        jne   .Lx436_57
                        mov              r9, rax
                                                                                        jmp   .Lx436_42
.Lx436_57:
                        cmp              esi, 2
                                                                                        jne   .Lx436_43
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx436_43
                        mov              r9, rax
                                                                                        jmp   .Lx436_42
.Lx436_58:
                        cmp              eax, 13
                                                                                        jne   .Lx436_43
                        mov              rax, qword ptr [r9 + 8]
                        test             rax, rax
                                                                                        je    .Lx436_43
                        cmp              rax, r9
                                                                                        je    .Lx436_43
                        mov              r9, rax
                                                                                        jmp   .Lx436_42
.Lx436_43:
                        cmp              r8, r9
                                                                                        je    .Lx436_51
                        mov              eax, dword ptr [r8 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx436_44
                        cmp              eax, 99
                                                                                        je    .Lx436_44
                        cmp              eax, 13
                                                                                        jne   .Lx436_45
                        mov              rax, qword ptr [r8 + 8]
                        cmp              rax, r8
                                                                                        je    .Lx436_44
                                                                                        jmp   .Lx436_45
.Lx436_44:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx436_53
                        cmp              eax, 99
                                                                                        je    .Lx436_53
                        cmp              eax, 13
                                                                                        jne   .Lx436_46
                        mov              rax, qword ptr [r9 + 8]
                        cmp              rax, r9
                                                                                        je    .Lx436_53
                                                                                        jmp   .Lx436_46
.Lx436_46:
                        lea              r10, [rip + g_pl_trail]
                        mov              r11, qword ptr [r10 + 0]
                        test             r11, r11
                                                                                        je    .Lx436_53
                        mov              eax, dword ptr [r10 + 32]
                        mov              esi, 24
                        imul             rsi, rax
                        mov              rax, qword ptr [r10 + 24]
                        sub              rax, 24
                        cmp              rsi, rax
                                                                                        ja    .Lx436_53
                        add              r11, rsi
                        mov              qword ptr [r11 + 0], r8
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r11 + 8], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r11 + 16], rax
                        mov              eax, dword ptr [r10 + 32]
                        add              eax, 1
                        mov              dword ptr [r10 + 32], eax
                        mov              rax, qword ptr [r9 + 0]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [r8 + 8], rax
                                                                                        jmp   .Lx436_51
.Lx436_45:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx436_47
                        cmp              eax, 99
                                                                                        je    .Lx436_47
                        cmp              eax, 13
                                                                                        jne   .Lx436_48
                        mov              rax, qword ptr [r9 + 8]
                        cmp              rax, r9
                                                                                        je    .Lx436_47
                                                                                        jmp   .Lx436_48
.Lx436_47:
                        lea              r10, [rip + g_pl_trail]
                        mov              r11, qword ptr [r10 + 0]
                        test             r11, r11
                                                                                        je    .Lx436_53
                        mov              eax, dword ptr [r10 + 32]
                        mov              esi, 24
                        imul             rsi, rax
                        mov              rax, qword ptr [r10 + 24]
                        sub              rax, 24
                        cmp              rsi, rax
                                                                                        ja    .Lx436_53
                        add              r11, rsi
                        mov              qword ptr [r11 + 0], r9
                        mov              rax, qword ptr [r9 + 0]
                        mov              qword ptr [r11 + 8], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [r11 + 16], rax
                        mov              eax, dword ptr [r10 + 32]
                        add              eax, 1
                        mov              dword ptr [r10 + 32], eax
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 8], rax
                                                                                        jmp   .Lx436_51
.Lx436_48:
                        mov              ecx, dword ptr [r8 + 0]
                        mov              edx, dword ptr [r9 + 0]
                        cmp              ecx, 14
                                                                                        jne   .Lx436_49
                        cmp              edx, 14
                                                                                        je    .Lx436_53
                                                                                        jmp   .Lx436_52
.Lx436_49:
                        cmp              edx, 14
                                                                                        je    .Lx436_52
                        cmp              ecx, 7
                                                                                        je    .Lx436_53
                        cmp              edx, 7
                                                                                        je    .Lx436_53
                        cmp              ecx, 6
                                                                                        jne   .Lx436_50
                        cmp              edx, 6
                                                                                        jne   .Lx436_50
                        mov              rax, qword ptr [r8 + 0]
                        cmp              rax, 6
                                                                                        jne   .Lx436_53
                        mov              rax, qword ptr [r9 + 0]
                        cmp              rax, 6
                                                                                        jne   .Lx436_53
                        mov              rax, qword ptr [r8 + 8]
                        mov              rsi, qword ptr [r9 + 8]
                        cmp              rax, rsi
                                                                                        je    .Lx436_51
                                                                                        jmp   .Lx436_52
.Lx436_50:
                        mov              rax, qword ptr [r8 + 0]
                        mov              rsi, qword ptr [r9 + 0]
                        cmp              rax, rsi
                                                                                        jne   .Lx436_53
                        mov              rax, qword ptr [r8 + 8]
                        mov              rsi, qword ptr [r9 + 8]
                        cmp              rax, rsi
                                                                                        jne   .Lx436_53
.Lx436_51:
                        mov              rax, qword ptr [r8 + 0]
                        mov              rdx, qword ptr [r8 + 8]
                                                                                        jmp   .Lx436_54
.Lx436_52:
                        mov              eax, 99
                        mov              edx, 0
                                                                                        jmp   .Lx436_54
.Lx436_53:
                        mov              esi, 2
                        call             rt_pl_dop_unify@PLT
.Lx436_54:
                        mov              qword ptr [rbp + 5792], rax
                        mov              qword ptr [rbp + 5800], rdx
                        cmp              eax, 99
                                                                                        je    n80_op11_α
                                                                                        jmp   n79_var_ref_α
n74_op11_β:
                                                                                        jmp   n80_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n75_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8736]
                        mov              qword ptr [rbp + 7904], rax
                        mov              qword ptr [rbp + 7912], rdx
                                                                                        jmp   n81_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n76_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8768]
                        mov              qword ptr [rbp + 7360], rax
                        mov              qword ptr [rbp + 7368], rdx
                                                                                        jmp   n82_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n77_var_α:
                        mov              rax, qword ptr [rbp + 8784]
                        mov              qword ptr [rbp + 6880], rax
                        mov              rax, qword ptr [rbp + 8792]
                        mov              qword ptr [rbp + 6888], rax
                                                                                        jmp   n83_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n78_lit_integer_α:
                        mov              qword ptr [rbp + 6336], 6
                        mov              rax, qword ptr [rip + .Lx443_0]
                        mov              qword ptr [rbp + 6344], rax
                                                                                        jmp   n84_op11_α
.Lx443_0:
                        .quad            400
#-----------------------------------------------------------------------------------------------------------------------
n79_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 32]
                        mov              qword ptr [rbp + 5760], rax
                        mov              qword ptr [rbp + 5768], rdx
                                                                                        jmp   n85_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n80_op11_α:
                        mov              rax, qword ptr [rbp + 80]
                        mov              qword ptr [rbp + 5248], rax
                        mov              rax, qword ptr [rbp + 88]
                        mov              qword ptr [rbp + 5256], rax
                        lea              rdi, [rbp + 5248]
                        mov              esi, 1
                        call             rt_pl_dop_unwind_nothrow@PLT
                        mov              qword ptr [rbp + 5232], rax
                        mov              qword ptr [rbp + 5240], rdx
                        cmp              eax, 99
                                                                                        je    proc_roman$2F2_ω
                                                                                        jmp   n86_var_ref_α
n80_op11_β:
                                                                                        jmp   proc_roman$2F2_ω
#-----------------------------------------------------------------------------------------------------------------------
n81_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8752]
                        mov              qword ptr [rbp + 7920], rax
                        mov              qword ptr [rbp + 7928], rdx
                                                                                        jmp   n87_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n82_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8736]
                        mov              qword ptr [rbp + 7376], rax
                        mov              qword ptr [rbp + 7384], rdx
                                                                                        jmp   n88_call_proc_staged_α
#-----------------------------------------------------------------------------------------------------------------------
n83_lit_integer_α:
                        mov              qword ptr [rbp + 6896], 6
                        mov              rax, qword ptr [rip + .Lx451_0]
                        mov              qword ptr [rbp + 6904], rax
                                                                                        jmp   n89_op11_α
.Lx451_0:
                        .quad            500
#-----------------------------------------------------------------------------------------------------------------------
n84_op11_α:
                        mov              rax, qword ptr [rbp + 6320]
                        mov              qword ptr [rbp + 6288], rax
                        mov              rax, qword ptr [rbp + 6328]
                        mov              qword ptr [rbp + 6296], rax
                        mov              rax, qword ptr [rbp + 6336]
                        mov              qword ptr [rbp + 6304], rax
                        mov              rax, qword ptr [rbp + 6344]
                        mov              qword ptr [rbp + 6312], rax
                        lea              rdi, [rbp + 6288]
                        mov              esi, 2
                        call             rt_pl_dop_cmp_ge@PLT
                        mov              qword ptr [rbp + 6272], rax
                        mov              qword ptr [rbp + 6280], rdx
                        cmp              eax, 99
                                                                                        je    n59_op11_α
                                                                                        jmp   n90_op19_α
n84_op11_β:
                                                                                        jmp   n59_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n85_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8752]
                        mov              qword ptr [rbp + 5776], rax
                        mov              qword ptr [rbp + 5784], rdx
                                                                                        jmp   n91_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n86_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 16]
                        mov              qword ptr [rbp + 5200], rax
                        mov              qword ptr [rbp + 5208], rdx
                                                                                        jmp   n92_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n87_op11_α:
                        mov              rax, qword ptr [rbp + 7888]
                        mov              qword ptr [rbp + 7840], rax
                        mov              rax, qword ptr [rbp + 7896]
                        mov              qword ptr [rbp + 7848], rax
                        mov              rax, qword ptr [rbp + 7904]
                        mov              qword ptr [rbp + 7856], rax
                        mov              rax, qword ptr [rbp + 7912]
                        mov              qword ptr [rbp + 7864], rax
                        mov              rax, qword ptr [rbp + 7920]
                        mov              qword ptr [rbp + 7872], rax
                        mov              rax, qword ptr [rbp + 7928]
                        mov              qword ptr [rbp + 7880], rax
                        .section         .rodata
.Lrkfn458:              .string          "$atom_concat"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn458]
                        lea              rsi, [rbp + 7840]
                        mov              edx, 3
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 7824], rax
                        mov              qword ptr [rbp + 7832], rdx
                        cmp              eax, 99
                                                                                        je    n65_call_proc_staged_β
                                                                                        jmp   n93_move_label_α
n87_op11_β:
                                                                                        jmp   n65_call_proc_staged_β
#-----------------------------------------------------------------------------------------------------------------------
n88_call_proc_staged_α:
                        lea              rsi, [rbp + 7360]
                        lea              rdx, [rbp + 7376]
                        call             proc_roman$2F2_dcα
                                                                                        jmp   .Lx460_2
.Lx460_2:
                        mov              qword ptr [rbp + 7296], rax
                        mov              qword ptr [rbp + 7304], rdx
                        cmp              eax, 99
                                                                                        je    n51_op11_α
                                                                                        jmp   n94_lit_string_α
n88_call_proc_staged_β:
                                                                                        jmp   n51_op11_α
.Lx460_0:
                        .quad            .Lx460_0_s
.Lx460_0_s:
                        .string          "roman/2"
#-----------------------------------------------------------------------------------------------------------------------
n89_op11_α:
                        mov              rax, qword ptr [rbp + 6880]
                        mov              qword ptr [rbp + 6832], rax
                        mov              rax, qword ptr [rbp + 6888]
                        mov              qword ptr [rbp + 6840], rax
                        mov              rax, qword ptr [rbp + 6896]
                        mov              qword ptr [rbp + 6848], rax
                        mov              rax, qword ptr [rbp + 6904]
                        mov              qword ptr [rbp + 6856], rax
                        lea              rdi, [rbp + 6832]
                        mov              esi, 2
                        call             rt_pl_dop_ax_sub@PLT
                        mov              qword ptr [rbp + 6816], rax
                        mov              qword ptr [rbp + 6824], rdx
                        cmp              eax, 99
                                                                                        je    n51_op11_α
                                                                                        jmp   n95_op11_α
n89_op11_β:
                                                                                        jmp   n51_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n90_op19_α:
                                                                                        jmp   n96_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n91_op11_α:
                        mov              rax, qword ptr [rbp + 5760]
                        mov              qword ptr [rbp + 5728], rax
                        mov              rax, qword ptr [rbp + 5768]
                        mov              qword ptr [rbp + 5736], rax
                        mov              rax, qword ptr [rbp + 5776]
                        mov              qword ptr [rbp + 5744], rax
                        mov              rax, qword ptr [rbp + 5784]
                        mov              qword ptr [rbp + 5752], rax
                        lea              rdi, [rbp + 5728]
                        lea              r8, [rbp + 5728]
.Lx463_40:
                        mov              eax, dword ptr [r8 + 0]
                        cmp              eax, 9
                                                                                        jne   .Lx463_56
                        mov              esi, dword ptr [r8 + 4]
                        mov              rax, qword ptr [r8 + 8]
                        test             rax, rax
                                                                                        je    .Lx463_41
                        cmp              esi, 1
                                                                                        jne   .Lx463_55
                        mov              r8, rax
                                                                                        jmp   .Lx463_40
.Lx463_55:
                        cmp              esi, 2
                                                                                        jne   .Lx463_41
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx463_41
                        mov              r8, rax
                                                                                        jmp   .Lx463_40
.Lx463_56:
                        cmp              eax, 13
                                                                                        jne   .Lx463_41
                        mov              rax, qword ptr [r8 + 8]
                        test             rax, rax
                                                                                        je    .Lx463_41
                        cmp              rax, r8
                                                                                        je    .Lx463_41
                        mov              r8, rax
                                                                                        jmp   .Lx463_40
.Lx463_41:
                        lea              r9, [rbp + 5744]
.Lx463_42:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 9
                                                                                        jne   .Lx463_58
                        mov              esi, dword ptr [r9 + 4]
                        mov              rax, qword ptr [r9 + 8]
                        test             rax, rax
                                                                                        je    .Lx463_43
                        cmp              esi, 1
                                                                                        jne   .Lx463_57
                        mov              r9, rax
                                                                                        jmp   .Lx463_42
.Lx463_57:
                        cmp              esi, 2
                                                                                        jne   .Lx463_43
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx463_43
                        mov              r9, rax
                                                                                        jmp   .Lx463_42
.Lx463_58:
                        cmp              eax, 13
                                                                                        jne   .Lx463_43
                        mov              rax, qword ptr [r9 + 8]
                        test             rax, rax
                                                                                        je    .Lx463_43
                        cmp              rax, r9
                                                                                        je    .Lx463_43
                        mov              r9, rax
                                                                                        jmp   .Lx463_42
.Lx463_43:
                        cmp              r8, r9
                                                                                        je    .Lx463_51
                        mov              eax, dword ptr [r8 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx463_44
                        cmp              eax, 99
                                                                                        je    .Lx463_44
                        cmp              eax, 13
                                                                                        jne   .Lx463_45
                        mov              rax, qword ptr [r8 + 8]
                        cmp              rax, r8
                                                                                        je    .Lx463_44
                                                                                        jmp   .Lx463_45
.Lx463_44:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx463_53
                        cmp              eax, 99
                                                                                        je    .Lx463_53
                        cmp              eax, 13
                                                                                        jne   .Lx463_46
                        mov              rax, qword ptr [r9 + 8]
                        cmp              rax, r9
                                                                                        je    .Lx463_53
                                                                                        jmp   .Lx463_46
.Lx463_46:
                        lea              r10, [rip + g_pl_trail]
                        mov              r11, qword ptr [r10 + 0]
                        test             r11, r11
                                                                                        je    .Lx463_53
                        mov              eax, dword ptr [r10 + 32]
                        mov              esi, 24
                        imul             rsi, rax
                        mov              rax, qword ptr [r10 + 24]
                        sub              rax, 24
                        cmp              rsi, rax
                                                                                        ja    .Lx463_53
                        add              r11, rsi
                        mov              qword ptr [r11 + 0], r8
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r11 + 8], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r11 + 16], rax
                        mov              eax, dword ptr [r10 + 32]
                        add              eax, 1
                        mov              dword ptr [r10 + 32], eax
                        mov              rax, qword ptr [r9 + 0]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [r8 + 8], rax
                                                                                        jmp   .Lx463_51
.Lx463_45:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx463_47
                        cmp              eax, 99
                                                                                        je    .Lx463_47
                        cmp              eax, 13
                                                                                        jne   .Lx463_48
                        mov              rax, qword ptr [r9 + 8]
                        cmp              rax, r9
                                                                                        je    .Lx463_47
                                                                                        jmp   .Lx463_48
.Lx463_47:
                        lea              r10, [rip + g_pl_trail]
                        mov              r11, qword ptr [r10 + 0]
                        test             r11, r11
                                                                                        je    .Lx463_53
                        mov              eax, dword ptr [r10 + 32]
                        mov              esi, 24
                        imul             rsi, rax
                        mov              rax, qword ptr [r10 + 24]
                        sub              rax, 24
                        cmp              rsi, rax
                                                                                        ja    .Lx463_53
                        add              r11, rsi
                        mov              qword ptr [r11 + 0], r9
                        mov              rax, qword ptr [r9 + 0]
                        mov              qword ptr [r11 + 8], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [r11 + 16], rax
                        mov              eax, dword ptr [r10 + 32]
                        add              eax, 1
                        mov              dword ptr [r10 + 32], eax
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 8], rax
                                                                                        jmp   .Lx463_51
.Lx463_48:
                        mov              ecx, dword ptr [r8 + 0]
                        mov              edx, dword ptr [r9 + 0]
                        cmp              ecx, 14
                                                                                        jne   .Lx463_49
                        cmp              edx, 14
                                                                                        je    .Lx463_53
                                                                                        jmp   .Lx463_52
.Lx463_49:
                        cmp              edx, 14
                                                                                        je    .Lx463_52
                        cmp              ecx, 7
                                                                                        je    .Lx463_53
                        cmp              edx, 7
                                                                                        je    .Lx463_53
                        cmp              ecx, 6
                                                                                        jne   .Lx463_50
                        cmp              edx, 6
                                                                                        jne   .Lx463_50
                        mov              rax, qword ptr [r8 + 0]
                        cmp              rax, 6
                                                                                        jne   .Lx463_53
                        mov              rax, qword ptr [r9 + 0]
                        cmp              rax, 6
                                                                                        jne   .Lx463_53
                        mov              rax, qword ptr [r8 + 8]
                        mov              rsi, qword ptr [r9 + 8]
                        cmp              rax, rsi
                                                                                        je    .Lx463_51
                                                                                        jmp   .Lx463_52
.Lx463_50:
                        mov              rax, qword ptr [r8 + 0]
                        mov              rsi, qword ptr [r9 + 0]
                        cmp              rax, rsi
                                                                                        jne   .Lx463_53
                        mov              rax, qword ptr [r8 + 8]
                        mov              rsi, qword ptr [r9 + 8]
                        cmp              rax, rsi
                                                                                        jne   .Lx463_53
.Lx463_51:
                        mov              rax, qword ptr [r8 + 0]
                        mov              rdx, qword ptr [r8 + 8]
                                                                                        jmp   .Lx463_54
.Lx463_52:
                        mov              eax, 99
                        mov              edx, 0
                                                                                        jmp   .Lx463_54
.Lx463_53:
                        mov              esi, 2
                        call             rt_pl_dop_unify@PLT
.Lx463_54:
                        mov              qword ptr [rbp + 5712], rax
                        mov              qword ptr [rbp + 5720], rdx
                        cmp              eax, 99
                                                                                        je    n80_op11_α
                                                                                        jmp   n97_var_α
n91_op11_β:
                                                                                        jmp   n80_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n92_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8784]
                        mov              qword ptr [rbp + 5216], rax
                        mov              qword ptr [rbp + 5224], rdx
                                                                                        jmp   n98_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n93_move_label_α:
                        lea              rax, [rip + n65_call_proc_staged_β]
                        mov              qword ptr [rbp + 64], rax
                                                                                        jmp   proc_roman$2F2_γ
#-----------------------------------------------------------------------------------------------------------------------
n94_lit_string_α:
                        mov              qword ptr [rbp + 7248], 1
                        mov              rax, qword ptr [rip + .Lx468_0]
                        mov              qword ptr [rbp + 7256], rax
                                                                                        jmp   n99_var_ref_α
.Lx468_0:
                        .quad            .Lx468_0_s
.Lx468_0_s:
                        .string          "CM"
#-----------------------------------------------------------------------------------------------------------------------
n95_op11_α:
                        mov              rax, qword ptr [rbp + 6800]
                        mov              qword ptr [rbp + 6768], rax
                        mov              rax, qword ptr [rbp + 6808]
                        mov              qword ptr [rbp + 6776], rax
                        mov              rax, qword ptr [rbp + 6816]
                        mov              qword ptr [rbp + 6784], rax
                        mov              rax, qword ptr [rbp + 6824]
                        mov              qword ptr [rbp + 6792], rax
                        lea              rdi, [rbp + 6768]
                        mov              esi, 2
                        call             rt_pl_dop_is_v@PLT
                        mov              qword ptr [rbp + 6752], rax
                        mov              qword ptr [rbp + 6760], rdx
                        cmp              eax, 99
                                                                                        je    n51_op11_α
                                                                                        jmp   n100_var_ref_α
n95_op11_β:
                                                                                        jmp   n51_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n96_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8768]
                        mov              qword ptr [rbp + 6160], rax
                        mov              qword ptr [rbp + 6168], rdx
                                                                                        jmp   n101_var_α
#-----------------------------------------------------------------------------------------------------------------------
n97_var_α:
                        mov              rax, qword ptr [rbp + 8784]
                        mov              qword ptr [rbp + 5680], rax
                        mov              rax, qword ptr [rbp + 8792]
                        mov              qword ptr [rbp + 5688], rax
                                                                                        jmp   n102_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n98_op11_α:
                        mov              rax, qword ptr [rbp + 5200]
                        mov              qword ptr [rbp + 5168], rax
                        mov              rax, qword ptr [rbp + 5208]
                        mov              qword ptr [rbp + 5176], rax
                        mov              rax, qword ptr [rbp + 5216]
                        mov              qword ptr [rbp + 5184], rax
                        mov              rax, qword ptr [rbp + 5224]
                        mov              qword ptr [rbp + 5192], rax
                        lea              rdi, [rbp + 5168]
                        lea              r8, [rbp + 5168]
.Lx474_40:
                        mov              eax, dword ptr [r8 + 0]
                        cmp              eax, 9
                                                                                        jne   .Lx474_56
                        mov              esi, dword ptr [r8 + 4]
                        mov              rax, qword ptr [r8 + 8]
                        test             rax, rax
                                                                                        je    .Lx474_41
                        cmp              esi, 1
                                                                                        jne   .Lx474_55
                        mov              r8, rax
                                                                                        jmp   .Lx474_40
.Lx474_55:
                        cmp              esi, 2
                                                                                        jne   .Lx474_41
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx474_41
                        mov              r8, rax
                                                                                        jmp   .Lx474_40
.Lx474_56:
                        cmp              eax, 13
                                                                                        jne   .Lx474_41
                        mov              rax, qword ptr [r8 + 8]
                        test             rax, rax
                                                                                        je    .Lx474_41
                        cmp              rax, r8
                                                                                        je    .Lx474_41
                        mov              r8, rax
                                                                                        jmp   .Lx474_40
.Lx474_41:
                        lea              r9, [rbp + 5184]
.Lx474_42:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 9
                                                                                        jne   .Lx474_58
                        mov              esi, dword ptr [r9 + 4]
                        mov              rax, qword ptr [r9 + 8]
                        test             rax, rax
                                                                                        je    .Lx474_43
                        cmp              esi, 1
                                                                                        jne   .Lx474_57
                        mov              r9, rax
                                                                                        jmp   .Lx474_42
.Lx474_57:
                        cmp              esi, 2
                                                                                        jne   .Lx474_43
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx474_43
                        mov              r9, rax
                                                                                        jmp   .Lx474_42
.Lx474_58:
                        cmp              eax, 13
                                                                                        jne   .Lx474_43
                        mov              rax, qword ptr [r9 + 8]
                        test             rax, rax
                                                                                        je    .Lx474_43
                        cmp              rax, r9
                                                                                        je    .Lx474_43
                        mov              r9, rax
                                                                                        jmp   .Lx474_42
.Lx474_43:
                        cmp              r8, r9
                                                                                        je    .Lx474_51
                        mov              eax, dword ptr [r8 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx474_44
                        cmp              eax, 99
                                                                                        je    .Lx474_44
                        cmp              eax, 13
                                                                                        jne   .Lx474_45
                        mov              rax, qword ptr [r8 + 8]
                        cmp              rax, r8
                                                                                        je    .Lx474_44
                                                                                        jmp   .Lx474_45
.Lx474_44:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx474_53
                        cmp              eax, 99
                                                                                        je    .Lx474_53
                        cmp              eax, 13
                                                                                        jne   .Lx474_46
                        mov              rax, qword ptr [r9 + 8]
                        cmp              rax, r9
                                                                                        je    .Lx474_53
                                                                                        jmp   .Lx474_46
.Lx474_46:
                        lea              r10, [rip + g_pl_trail]
                        mov              r11, qword ptr [r10 + 0]
                        test             r11, r11
                                                                                        je    .Lx474_53
                        mov              eax, dword ptr [r10 + 32]
                        mov              esi, 24
                        imul             rsi, rax
                        mov              rax, qword ptr [r10 + 24]
                        sub              rax, 24
                        cmp              rsi, rax
                                                                                        ja    .Lx474_53
                        add              r11, rsi
                        mov              qword ptr [r11 + 0], r8
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r11 + 8], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r11 + 16], rax
                        mov              eax, dword ptr [r10 + 32]
                        add              eax, 1
                        mov              dword ptr [r10 + 32], eax
                        mov              rax, qword ptr [r9 + 0]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [r8 + 8], rax
                                                                                        jmp   .Lx474_51
.Lx474_45:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx474_47
                        cmp              eax, 99
                                                                                        je    .Lx474_47
                        cmp              eax, 13
                                                                                        jne   .Lx474_48
                        mov              rax, qword ptr [r9 + 8]
                        cmp              rax, r9
                                                                                        je    .Lx474_47
                                                                                        jmp   .Lx474_48
.Lx474_47:
                        lea              r10, [rip + g_pl_trail]
                        mov              r11, qword ptr [r10 + 0]
                        test             r11, r11
                                                                                        je    .Lx474_53
                        mov              eax, dword ptr [r10 + 32]
                        mov              esi, 24
                        imul             rsi, rax
                        mov              rax, qword ptr [r10 + 24]
                        sub              rax, 24
                        cmp              rsi, rax
                                                                                        ja    .Lx474_53
                        add              r11, rsi
                        mov              qword ptr [r11 + 0], r9
                        mov              rax, qword ptr [r9 + 0]
                        mov              qword ptr [r11 + 8], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [r11 + 16], rax
                        mov              eax, dword ptr [r10 + 32]
                        add              eax, 1
                        mov              dword ptr [r10 + 32], eax
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 8], rax
                                                                                        jmp   .Lx474_51
.Lx474_48:
                        mov              ecx, dword ptr [r8 + 0]
                        mov              edx, dword ptr [r9 + 0]
                        cmp              ecx, 14
                                                                                        jne   .Lx474_49
                        cmp              edx, 14
                                                                                        je    .Lx474_53
                                                                                        jmp   .Lx474_52
.Lx474_49:
                        cmp              edx, 14
                                                                                        je    .Lx474_52
                        cmp              ecx, 7
                                                                                        je    .Lx474_53
                        cmp              edx, 7
                                                                                        je    .Lx474_53
                        cmp              ecx, 6
                                                                                        jne   .Lx474_50
                        cmp              edx, 6
                                                                                        jne   .Lx474_50
                        mov              rax, qword ptr [r8 + 0]
                        cmp              rax, 6
                                                                                        jne   .Lx474_53
                        mov              rax, qword ptr [r9 + 0]
                        cmp              rax, 6
                                                                                        jne   .Lx474_53
                        mov              rax, qword ptr [r8 + 8]
                        mov              rsi, qword ptr [r9 + 8]
                        cmp              rax, rsi
                                                                                        je    .Lx474_51
                                                                                        jmp   .Lx474_52
.Lx474_50:
                        mov              rax, qword ptr [r8 + 0]
                        mov              rsi, qword ptr [r9 + 0]
                        cmp              rax, rsi
                                                                                        jne   .Lx474_53
                        mov              rax, qword ptr [r8 + 8]
                        mov              rsi, qword ptr [r9 + 8]
                        cmp              rax, rsi
                                                                                        jne   .Lx474_53
.Lx474_51:
                        mov              rax, qword ptr [r8 + 0]
                        mov              rdx, qword ptr [r8 + 8]
                                                                                        jmp   .Lx474_54
.Lx474_52:
                        mov              eax, 99
                        mov              edx, 0
                                                                                        jmp   .Lx474_54
.Lx474_53:
                        mov              esi, 2
                        call             rt_pl_dop_unify@PLT
.Lx474_54:
                        mov              qword ptr [rbp + 5152], rax
                        mov              qword ptr [rbp + 5160], rdx
                        cmp              eax, 99
                                                                                        je    n104_op11_α
                                                                                        jmp   n103_var_ref_α
n98_op11_β:
                                                                                        jmp   n104_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n99_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8736]
                        mov              qword ptr [rbp + 7264], rax
                        mov              qword ptr [rbp + 7272], rdx
                                                                                        jmp   n105_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n100_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8768]
                        mov              qword ptr [rbp + 6720], rax
                        mov              qword ptr [rbp + 6728], rdx
                                                                                        jmp   n106_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n101_var_α:
                        mov              rax, qword ptr [rbp + 8784]
                        mov              qword ptr [rbp + 6240], rax
                        mov              rax, qword ptr [rbp + 8792]
                        mov              qword ptr [rbp + 6248], rax
                                                                                        jmp   n107_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n102_lit_integer_α:
                        mov              qword ptr [rbp + 5696], 6
                        mov              rax, qword ptr [rip + .Lx481_0]
                        mov              qword ptr [rbp + 5704], rax
                                                                                        jmp   n108_op11_α
.Lx481_0:
                        .quad            100
#-----------------------------------------------------------------------------------------------------------------------
n103_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 32]
                        mov              qword ptr [rbp + 5120], rax
                        mov              qword ptr [rbp + 5128], rdx
                                                                                        jmp   n109_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n104_op11_α:
                        mov              rax, qword ptr [rbp + 80]
                        mov              qword ptr [rbp + 4608], rax
                        mov              rax, qword ptr [rbp + 88]
                        mov              qword ptr [rbp + 4616], rax
                        lea              rdi, [rbp + 4608]
                        mov              esi, 1
                        call             rt_pl_dop_unwind_nothrow@PLT
                        mov              qword ptr [rbp + 4592], rax
                        mov              qword ptr [rbp + 4600], rdx
                        cmp              eax, 99
                                                                                        je    proc_roman$2F2_ω
                                                                                        jmp   n110_var_ref_α
n104_op11_β:
                                                                                        jmp   proc_roman$2F2_ω
#-----------------------------------------------------------------------------------------------------------------------
n105_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8752]
                        mov              qword ptr [rbp + 7280], rax
                        mov              qword ptr [rbp + 7288], rdx
                                                                                        jmp   n111_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n106_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8736]
                        mov              qword ptr [rbp + 6736], rax
                        mov              qword ptr [rbp + 6744], rdx
                                                                                        jmp   n112_call_proc_staged_α
#-----------------------------------------------------------------------------------------------------------------------
n107_lit_integer_α:
                        mov              qword ptr [rbp + 6256], 6
                        mov              rax, qword ptr [rip + .Lx489_0]
                        mov              qword ptr [rbp + 6264], rax
                                                                                        jmp   n113_op11_α
.Lx489_0:
                        .quad            400
#-----------------------------------------------------------------------------------------------------------------------
n108_op11_α:
                        mov              rax, qword ptr [rbp + 5680]
                        mov              qword ptr [rbp + 5648], rax
                        mov              rax, qword ptr [rbp + 5688]
                        mov              qword ptr [rbp + 5656], rax
                        mov              rax, qword ptr [rbp + 5696]
                        mov              qword ptr [rbp + 5664], rax
                        mov              rax, qword ptr [rbp + 5704]
                        mov              qword ptr [rbp + 5672], rax
                        lea              rdi, [rbp + 5648]
                        mov              esi, 2
                        call             rt_pl_dop_cmp_ge@PLT
                        mov              qword ptr [rbp + 5632], rax
                        mov              qword ptr [rbp + 5640], rdx
                        cmp              eax, 99
                                                                                        je    n80_op11_α
                                                                                        jmp   n114_op19_α
n108_op11_β:
                                                                                        jmp   n80_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n109_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8752]
                        mov              qword ptr [rbp + 5136], rax
                        mov              qword ptr [rbp + 5144], rdx
                                                                                        jmp   n115_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n110_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 16]
                        mov              qword ptr [rbp + 4560], rax
                        mov              qword ptr [rbp + 4568], rdx
                                                                                        jmp   n116_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n111_op11_α:
                        mov              rax, qword ptr [rbp + 7248]
                        mov              qword ptr [rbp + 7200], rax
                        mov              rax, qword ptr [rbp + 7256]
                        mov              qword ptr [rbp + 7208], rax
                        mov              rax, qword ptr [rbp + 7264]
                        mov              qword ptr [rbp + 7216], rax
                        mov              rax, qword ptr [rbp + 7272]
                        mov              qword ptr [rbp + 7224], rax
                        mov              rax, qword ptr [rbp + 7280]
                        mov              qword ptr [rbp + 7232], rax
                        mov              rax, qword ptr [rbp + 7288]
                        mov              qword ptr [rbp + 7240], rax
                        .section         .rodata
.Lrkfn496:              .string          "$atom_concat"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn496]
                        lea              rsi, [rbp + 7200]
                        mov              edx, 3
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 7184], rax
                        mov              qword ptr [rbp + 7192], rdx
                        cmp              eax, 99
                                                                                        je    n88_call_proc_staged_β
                                                                                        jmp   n117_move_label_α
n111_op11_β:
                                                                                        jmp   n88_call_proc_staged_β
#-----------------------------------------------------------------------------------------------------------------------
n112_call_proc_staged_α:
                        lea              rsi, [rbp + 6720]
                        lea              rdx, [rbp + 6736]
                        call             proc_roman$2F2_dcα
                                                                                        jmp   .Lx498_2
.Lx498_2:
                        mov              qword ptr [rbp + 6656], rax
                        mov              qword ptr [rbp + 6664], rdx
                        cmp              eax, 99
                                                                                        je    n51_op11_α
                                                                                        jmp   n118_lit_string_α
n112_call_proc_staged_β:
                                                                                        jmp   n51_op11_α
.Lx498_0:
                        .quad            .Lx498_0_s
.Lx498_0_s:
                        .string          "roman/2"
#-----------------------------------------------------------------------------------------------------------------------
n113_op11_α:
                        mov              rax, qword ptr [rbp + 6240]
                        mov              qword ptr [rbp + 6192], rax
                        mov              rax, qword ptr [rbp + 6248]
                        mov              qword ptr [rbp + 6200], rax
                        mov              rax, qword ptr [rbp + 6256]
                        mov              qword ptr [rbp + 6208], rax
                        mov              rax, qword ptr [rbp + 6264]
                        mov              qword ptr [rbp + 6216], rax
                        lea              rdi, [rbp + 6192]
                        mov              esi, 2
                        call             rt_pl_dop_ax_sub@PLT
                        mov              qword ptr [rbp + 6176], rax
                        mov              qword ptr [rbp + 6184], rdx
                        cmp              eax, 99
                                                                                        je    n51_op11_α
                                                                                        jmp   n119_op11_α
n113_op11_β:
                                                                                        jmp   n51_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n114_op19_α:
                                                                                        jmp   n120_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n115_op11_α:
                        mov              rax, qword ptr [rbp + 5120]
                        mov              qword ptr [rbp + 5088], rax
                        mov              rax, qword ptr [rbp + 5128]
                        mov              qword ptr [rbp + 5096], rax
                        mov              rax, qword ptr [rbp + 5136]
                        mov              qword ptr [rbp + 5104], rax
                        mov              rax, qword ptr [rbp + 5144]
                        mov              qword ptr [rbp + 5112], rax
                        lea              rdi, [rbp + 5088]
                        lea              r8, [rbp + 5088]
.Lx501_40:
                        mov              eax, dword ptr [r8 + 0]
                        cmp              eax, 9
                                                                                        jne   .Lx501_56
                        mov              esi, dword ptr [r8 + 4]
                        mov              rax, qword ptr [r8 + 8]
                        test             rax, rax
                                                                                        je    .Lx501_41
                        cmp              esi, 1
                                                                                        jne   .Lx501_55
                        mov              r8, rax
                                                                                        jmp   .Lx501_40
.Lx501_55:
                        cmp              esi, 2
                                                                                        jne   .Lx501_41
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx501_41
                        mov              r8, rax
                                                                                        jmp   .Lx501_40
.Lx501_56:
                        cmp              eax, 13
                                                                                        jne   .Lx501_41
                        mov              rax, qword ptr [r8 + 8]
                        test             rax, rax
                                                                                        je    .Lx501_41
                        cmp              rax, r8
                                                                                        je    .Lx501_41
                        mov              r8, rax
                                                                                        jmp   .Lx501_40
.Lx501_41:
                        lea              r9, [rbp + 5104]
.Lx501_42:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 9
                                                                                        jne   .Lx501_58
                        mov              esi, dword ptr [r9 + 4]
                        mov              rax, qword ptr [r9 + 8]
                        test             rax, rax
                                                                                        je    .Lx501_43
                        cmp              esi, 1
                                                                                        jne   .Lx501_57
                        mov              r9, rax
                                                                                        jmp   .Lx501_42
.Lx501_57:
                        cmp              esi, 2
                                                                                        jne   .Lx501_43
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx501_43
                        mov              r9, rax
                                                                                        jmp   .Lx501_42
.Lx501_58:
                        cmp              eax, 13
                                                                                        jne   .Lx501_43
                        mov              rax, qword ptr [r9 + 8]
                        test             rax, rax
                                                                                        je    .Lx501_43
                        cmp              rax, r9
                                                                                        je    .Lx501_43
                        mov              r9, rax
                                                                                        jmp   .Lx501_42
.Lx501_43:
                        cmp              r8, r9
                                                                                        je    .Lx501_51
                        mov              eax, dword ptr [r8 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx501_44
                        cmp              eax, 99
                                                                                        je    .Lx501_44
                        cmp              eax, 13
                                                                                        jne   .Lx501_45
                        mov              rax, qword ptr [r8 + 8]
                        cmp              rax, r8
                                                                                        je    .Lx501_44
                                                                                        jmp   .Lx501_45
.Lx501_44:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx501_53
                        cmp              eax, 99
                                                                                        je    .Lx501_53
                        cmp              eax, 13
                                                                                        jne   .Lx501_46
                        mov              rax, qword ptr [r9 + 8]
                        cmp              rax, r9
                                                                                        je    .Lx501_53
                                                                                        jmp   .Lx501_46
.Lx501_46:
                        lea              r10, [rip + g_pl_trail]
                        mov              r11, qword ptr [r10 + 0]
                        test             r11, r11
                                                                                        je    .Lx501_53
                        mov              eax, dword ptr [r10 + 32]
                        mov              esi, 24
                        imul             rsi, rax
                        mov              rax, qword ptr [r10 + 24]
                        sub              rax, 24
                        cmp              rsi, rax
                                                                                        ja    .Lx501_53
                        add              r11, rsi
                        mov              qword ptr [r11 + 0], r8
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r11 + 8], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r11 + 16], rax
                        mov              eax, dword ptr [r10 + 32]
                        add              eax, 1
                        mov              dword ptr [r10 + 32], eax
                        mov              rax, qword ptr [r9 + 0]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [r8 + 8], rax
                                                                                        jmp   .Lx501_51
.Lx501_45:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx501_47
                        cmp              eax, 99
                                                                                        je    .Lx501_47
                        cmp              eax, 13
                                                                                        jne   .Lx501_48
                        mov              rax, qword ptr [r9 + 8]
                        cmp              rax, r9
                                                                                        je    .Lx501_47
                                                                                        jmp   .Lx501_48
.Lx501_47:
                        lea              r10, [rip + g_pl_trail]
                        mov              r11, qword ptr [r10 + 0]
                        test             r11, r11
                                                                                        je    .Lx501_53
                        mov              eax, dword ptr [r10 + 32]
                        mov              esi, 24
                        imul             rsi, rax
                        mov              rax, qword ptr [r10 + 24]
                        sub              rax, 24
                        cmp              rsi, rax
                                                                                        ja    .Lx501_53
                        add              r11, rsi
                        mov              qword ptr [r11 + 0], r9
                        mov              rax, qword ptr [r9 + 0]
                        mov              qword ptr [r11 + 8], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [r11 + 16], rax
                        mov              eax, dword ptr [r10 + 32]
                        add              eax, 1
                        mov              dword ptr [r10 + 32], eax
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 8], rax
                                                                                        jmp   .Lx501_51
.Lx501_48:
                        mov              ecx, dword ptr [r8 + 0]
                        mov              edx, dword ptr [r9 + 0]
                        cmp              ecx, 14
                                                                                        jne   .Lx501_49
                        cmp              edx, 14
                                                                                        je    .Lx501_53
                                                                                        jmp   .Lx501_52
.Lx501_49:
                        cmp              edx, 14
                                                                                        je    .Lx501_52
                        cmp              ecx, 7
                                                                                        je    .Lx501_53
                        cmp              edx, 7
                                                                                        je    .Lx501_53
                        cmp              ecx, 6
                                                                                        jne   .Lx501_50
                        cmp              edx, 6
                                                                                        jne   .Lx501_50
                        mov              rax, qword ptr [r8 + 0]
                        cmp              rax, 6
                                                                                        jne   .Lx501_53
                        mov              rax, qword ptr [r9 + 0]
                        cmp              rax, 6
                                                                                        jne   .Lx501_53
                        mov              rax, qword ptr [r8 + 8]
                        mov              rsi, qword ptr [r9 + 8]
                        cmp              rax, rsi
                                                                                        je    .Lx501_51
                                                                                        jmp   .Lx501_52
.Lx501_50:
                        mov              rax, qword ptr [r8 + 0]
                        mov              rsi, qword ptr [r9 + 0]
                        cmp              rax, rsi
                                                                                        jne   .Lx501_53
                        mov              rax, qword ptr [r8 + 8]
                        mov              rsi, qword ptr [r9 + 8]
                        cmp              rax, rsi
                                                                                        jne   .Lx501_53
.Lx501_51:
                        mov              rax, qword ptr [r8 + 0]
                        mov              rdx, qword ptr [r8 + 8]
                                                                                        jmp   .Lx501_54
.Lx501_52:
                        mov              eax, 99
                        mov              edx, 0
                                                                                        jmp   .Lx501_54
.Lx501_53:
                        mov              esi, 2
                        call             rt_pl_dop_unify@PLT
.Lx501_54:
                        mov              qword ptr [rbp + 5072], rax
                        mov              qword ptr [rbp + 5080], rdx
                        cmp              eax, 99
                                                                                        je    n104_op11_α
                                                                                        jmp   n121_var_α
n115_op11_β:
                                                                                        jmp   n104_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n116_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8784]
                        mov              qword ptr [rbp + 4576], rax
                        mov              qword ptr [rbp + 4584], rdx
                                                                                        jmp   n122_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n117_move_label_α:
                        lea              rax, [rip + n88_call_proc_staged_β]
                        mov              qword ptr [rbp + 64], rax
                                                                                        jmp   proc_roman$2F2_γ
#-----------------------------------------------------------------------------------------------------------------------
n118_lit_string_α:
                        mov              qword ptr [rbp + 6608], 1
                        mov              rax, qword ptr [rip + .Lx506_0]
                        mov              qword ptr [rbp + 6616], rax
                                                                                        jmp   n123_var_ref_α
.Lx506_0:
                        .quad            .Lx506_0_s
.Lx506_0_s:
                        .string          "D"
#-----------------------------------------------------------------------------------------------------------------------
n119_op11_α:
                        mov              rax, qword ptr [rbp + 6160]
                        mov              qword ptr [rbp + 6128], rax
                        mov              rax, qword ptr [rbp + 6168]
                        mov              qword ptr [rbp + 6136], rax
                        mov              rax, qword ptr [rbp + 6176]
                        mov              qword ptr [rbp + 6144], rax
                        mov              rax, qword ptr [rbp + 6184]
                        mov              qword ptr [rbp + 6152], rax
                        lea              rdi, [rbp + 6128]
                        mov              esi, 2
                        call             rt_pl_dop_is_v@PLT
                        mov              qword ptr [rbp + 6112], rax
                        mov              qword ptr [rbp + 6120], rdx
                        cmp              eax, 99
                                                                                        je    n51_op11_α
                                                                                        jmp   n124_var_ref_α
n119_op11_β:
                                                                                        jmp   n51_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n120_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8768]
                        mov              qword ptr [rbp + 5520], rax
                        mov              qword ptr [rbp + 5528], rdx
                                                                                        jmp   n125_var_α
#-----------------------------------------------------------------------------------------------------------------------
n121_var_α:
                        mov              rax, qword ptr [rbp + 8784]
                        mov              qword ptr [rbp + 5040], rax
                        mov              rax, qword ptr [rbp + 8792]
                        mov              qword ptr [rbp + 5048], rax
                                                                                        jmp   n126_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n122_op11_α:
                        mov              rax, qword ptr [rbp + 4560]
                        mov              qword ptr [rbp + 4528], rax
                        mov              rax, qword ptr [rbp + 4568]
                        mov              qword ptr [rbp + 4536], rax
                        mov              rax, qword ptr [rbp + 4576]
                        mov              qword ptr [rbp + 4544], rax
                        mov              rax, qword ptr [rbp + 4584]
                        mov              qword ptr [rbp + 4552], rax
                        lea              rdi, [rbp + 4528]
                        lea              r8, [rbp + 4528]
.Lx512_40:
                        mov              eax, dword ptr [r8 + 0]
                        cmp              eax, 9
                                                                                        jne   .Lx512_56
                        mov              esi, dword ptr [r8 + 4]
                        mov              rax, qword ptr [r8 + 8]
                        test             rax, rax
                                                                                        je    .Lx512_41
                        cmp              esi, 1
                                                                                        jne   .Lx512_55
                        mov              r8, rax
                                                                                        jmp   .Lx512_40
.Lx512_55:
                        cmp              esi, 2
                                                                                        jne   .Lx512_41
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx512_41
                        mov              r8, rax
                                                                                        jmp   .Lx512_40
.Lx512_56:
                        cmp              eax, 13
                                                                                        jne   .Lx512_41
                        mov              rax, qword ptr [r8 + 8]
                        test             rax, rax
                                                                                        je    .Lx512_41
                        cmp              rax, r8
                                                                                        je    .Lx512_41
                        mov              r8, rax
                                                                                        jmp   .Lx512_40
.Lx512_41:
                        lea              r9, [rbp + 4544]
.Lx512_42:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 9
                                                                                        jne   .Lx512_58
                        mov              esi, dword ptr [r9 + 4]
                        mov              rax, qword ptr [r9 + 8]
                        test             rax, rax
                                                                                        je    .Lx512_43
                        cmp              esi, 1
                                                                                        jne   .Lx512_57
                        mov              r9, rax
                                                                                        jmp   .Lx512_42
.Lx512_57:
                        cmp              esi, 2
                                                                                        jne   .Lx512_43
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx512_43
                        mov              r9, rax
                                                                                        jmp   .Lx512_42
.Lx512_58:
                        cmp              eax, 13
                                                                                        jne   .Lx512_43
                        mov              rax, qword ptr [r9 + 8]
                        test             rax, rax
                                                                                        je    .Lx512_43
                        cmp              rax, r9
                                                                                        je    .Lx512_43
                        mov              r9, rax
                                                                                        jmp   .Lx512_42
.Lx512_43:
                        cmp              r8, r9
                                                                                        je    .Lx512_51
                        mov              eax, dword ptr [r8 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx512_44
                        cmp              eax, 99
                                                                                        je    .Lx512_44
                        cmp              eax, 13
                                                                                        jne   .Lx512_45
                        mov              rax, qword ptr [r8 + 8]
                        cmp              rax, r8
                                                                                        je    .Lx512_44
                                                                                        jmp   .Lx512_45
.Lx512_44:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx512_53
                        cmp              eax, 99
                                                                                        je    .Lx512_53
                        cmp              eax, 13
                                                                                        jne   .Lx512_46
                        mov              rax, qword ptr [r9 + 8]
                        cmp              rax, r9
                                                                                        je    .Lx512_53
                                                                                        jmp   .Lx512_46
.Lx512_46:
                        lea              r10, [rip + g_pl_trail]
                        mov              r11, qword ptr [r10 + 0]
                        test             r11, r11
                                                                                        je    .Lx512_53
                        mov              eax, dword ptr [r10 + 32]
                        mov              esi, 24
                        imul             rsi, rax
                        mov              rax, qword ptr [r10 + 24]
                        sub              rax, 24
                        cmp              rsi, rax
                                                                                        ja    .Lx512_53
                        add              r11, rsi
                        mov              qword ptr [r11 + 0], r8
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r11 + 8], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r11 + 16], rax
                        mov              eax, dword ptr [r10 + 32]
                        add              eax, 1
                        mov              dword ptr [r10 + 32], eax
                        mov              rax, qword ptr [r9 + 0]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [r8 + 8], rax
                                                                                        jmp   .Lx512_51
.Lx512_45:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx512_47
                        cmp              eax, 99
                                                                                        je    .Lx512_47
                        cmp              eax, 13
                                                                                        jne   .Lx512_48
                        mov              rax, qword ptr [r9 + 8]
                        cmp              rax, r9
                                                                                        je    .Lx512_47
                                                                                        jmp   .Lx512_48
.Lx512_47:
                        lea              r10, [rip + g_pl_trail]
                        mov              r11, qword ptr [r10 + 0]
                        test             r11, r11
                                                                                        je    .Lx512_53
                        mov              eax, dword ptr [r10 + 32]
                        mov              esi, 24
                        imul             rsi, rax
                        mov              rax, qword ptr [r10 + 24]
                        sub              rax, 24
                        cmp              rsi, rax
                                                                                        ja    .Lx512_53
                        add              r11, rsi
                        mov              qword ptr [r11 + 0], r9
                        mov              rax, qword ptr [r9 + 0]
                        mov              qword ptr [r11 + 8], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [r11 + 16], rax
                        mov              eax, dword ptr [r10 + 32]
                        add              eax, 1
                        mov              dword ptr [r10 + 32], eax
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 8], rax
                                                                                        jmp   .Lx512_51
.Lx512_48:
                        mov              ecx, dword ptr [r8 + 0]
                        mov              edx, dword ptr [r9 + 0]
                        cmp              ecx, 14
                                                                                        jne   .Lx512_49
                        cmp              edx, 14
                                                                                        je    .Lx512_53
                                                                                        jmp   .Lx512_52
.Lx512_49:
                        cmp              edx, 14
                                                                                        je    .Lx512_52
                        cmp              ecx, 7
                                                                                        je    .Lx512_53
                        cmp              edx, 7
                                                                                        je    .Lx512_53
                        cmp              ecx, 6
                                                                                        jne   .Lx512_50
                        cmp              edx, 6
                                                                                        jne   .Lx512_50
                        mov              rax, qword ptr [r8 + 0]
                        cmp              rax, 6
                                                                                        jne   .Lx512_53
                        mov              rax, qword ptr [r9 + 0]
                        cmp              rax, 6
                                                                                        jne   .Lx512_53
                        mov              rax, qword ptr [r8 + 8]
                        mov              rsi, qword ptr [r9 + 8]
                        cmp              rax, rsi
                                                                                        je    .Lx512_51
                                                                                        jmp   .Lx512_52
.Lx512_50:
                        mov              rax, qword ptr [r8 + 0]
                        mov              rsi, qword ptr [r9 + 0]
                        cmp              rax, rsi
                                                                                        jne   .Lx512_53
                        mov              rax, qword ptr [r8 + 8]
                        mov              rsi, qword ptr [r9 + 8]
                        cmp              rax, rsi
                                                                                        jne   .Lx512_53
.Lx512_51:
                        mov              rax, qword ptr [r8 + 0]
                        mov              rdx, qword ptr [r8 + 8]
                                                                                        jmp   .Lx512_54
.Lx512_52:
                        mov              eax, 99
                        mov              edx, 0
                                                                                        jmp   .Lx512_54
.Lx512_53:
                        mov              esi, 2
                        call             rt_pl_dop_unify@PLT
.Lx512_54:
                        mov              qword ptr [rbp + 4512], rax
                        mov              qword ptr [rbp + 4520], rdx
                        cmp              eax, 99
                                                                                        je    n128_op11_α
                                                                                        jmp   n127_var_ref_α
n122_op11_β:
                                                                                        jmp   n128_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n123_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8736]
                        mov              qword ptr [rbp + 6624], rax
                        mov              qword ptr [rbp + 6632], rdx
                                                                                        jmp   n129_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n124_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8768]
                        mov              qword ptr [rbp + 6080], rax
                        mov              qword ptr [rbp + 6088], rdx
                                                                                        jmp   n130_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n125_var_α:
                        mov              rax, qword ptr [rbp + 8784]
                        mov              qword ptr [rbp + 5600], rax
                        mov              rax, qword ptr [rbp + 8792]
                        mov              qword ptr [rbp + 5608], rax
                                                                                        jmp   n131_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n126_lit_integer_α:
                        mov              qword ptr [rbp + 5056], 6
                        mov              rax, qword ptr [rip + .Lx519_0]
                        mov              qword ptr [rbp + 5064], rax
                                                                                        jmp   n132_op11_α
.Lx519_0:
                        .quad            90
#-----------------------------------------------------------------------------------------------------------------------
n127_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 32]
                        mov              qword ptr [rbp + 4480], rax
                        mov              qword ptr [rbp + 4488], rdx
                                                                                        jmp   n133_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n128_op11_α:
                        mov              rax, qword ptr [rbp + 80]
                        mov              qword ptr [rbp + 3968], rax
                        mov              rax, qword ptr [rbp + 88]
                        mov              qword ptr [rbp + 3976], rax
                        lea              rdi, [rbp + 3968]
                        mov              esi, 1
                        call             rt_pl_dop_unwind_nothrow@PLT
                        mov              qword ptr [rbp + 3952], rax
                        mov              qword ptr [rbp + 3960], rdx
                        cmp              eax, 99
                                                                                        je    proc_roman$2F2_ω
                                                                                        jmp   n134_var_ref_α
n128_op11_β:
                                                                                        jmp   proc_roman$2F2_ω
#-----------------------------------------------------------------------------------------------------------------------
n129_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8752]
                        mov              qword ptr [rbp + 6640], rax
                        mov              qword ptr [rbp + 6648], rdx
                                                                                        jmp   n135_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n130_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8736]
                        mov              qword ptr [rbp + 6096], rax
                        mov              qword ptr [rbp + 6104], rdx
                                                                                        jmp   n136_call_proc_staged_α
#-----------------------------------------------------------------------------------------------------------------------
n131_lit_integer_α:
                        mov              qword ptr [rbp + 5616], 6
                        mov              rax, qword ptr [rip + .Lx527_0]
                        mov              qword ptr [rbp + 5624], rax
                                                                                        jmp   n137_op11_α
.Lx527_0:
                        .quad            100
#-----------------------------------------------------------------------------------------------------------------------
n132_op11_α:
                        mov              rax, qword ptr [rbp + 5040]
                        mov              qword ptr [rbp + 5008], rax
                        mov              rax, qword ptr [rbp + 5048]
                        mov              qword ptr [rbp + 5016], rax
                        mov              rax, qword ptr [rbp + 5056]
                        mov              qword ptr [rbp + 5024], rax
                        mov              rax, qword ptr [rbp + 5064]
                        mov              qword ptr [rbp + 5032], rax
                        lea              rdi, [rbp + 5008]
                        mov              esi, 2
                        call             rt_pl_dop_cmp_ge@PLT
                        mov              qword ptr [rbp + 4992], rax
                        mov              qword ptr [rbp + 5000], rdx
                        cmp              eax, 99
                                                                                        je    n104_op11_α
                                                                                        jmp   n138_op19_α
n132_op11_β:
                                                                                        jmp   n104_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n133_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8752]
                        mov              qword ptr [rbp + 4496], rax
                        mov              qword ptr [rbp + 4504], rdx
                                                                                        jmp   n139_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n134_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 16]
                        mov              qword ptr [rbp + 3920], rax
                        mov              qword ptr [rbp + 3928], rdx
                                                                                        jmp   n140_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n135_op11_α:
                        mov              rax, qword ptr [rbp + 6608]
                        mov              qword ptr [rbp + 6560], rax
                        mov              rax, qword ptr [rbp + 6616]
                        mov              qword ptr [rbp + 6568], rax
                        mov              rax, qword ptr [rbp + 6624]
                        mov              qword ptr [rbp + 6576], rax
                        mov              rax, qword ptr [rbp + 6632]
                        mov              qword ptr [rbp + 6584], rax
                        mov              rax, qword ptr [rbp + 6640]
                        mov              qword ptr [rbp + 6592], rax
                        mov              rax, qword ptr [rbp + 6648]
                        mov              qword ptr [rbp + 6600], rax
                        .section         .rodata
.Lrkfn534:              .string          "$atom_concat"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn534]
                        lea              rsi, [rbp + 6560]
                        mov              edx, 3
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 6544], rax
                        mov              qword ptr [rbp + 6552], rdx
                        cmp              eax, 99
                                                                                        je    n112_call_proc_staged_β
                                                                                        jmp   n141_move_label_α
n135_op11_β:
                                                                                        jmp   n112_call_proc_staged_β
#-----------------------------------------------------------------------------------------------------------------------
n136_call_proc_staged_α:
                        lea              rsi, [rbp + 6080]
                        lea              rdx, [rbp + 6096]
                        call             proc_roman$2F2_dcα
                                                                                        jmp   .Lx536_2
.Lx536_2:
                        mov              qword ptr [rbp + 6016], rax
                        mov              qword ptr [rbp + 6024], rdx
                        cmp              eax, 99
                                                                                        je    n51_op11_α
                                                                                        jmp   n142_lit_string_α
n136_call_proc_staged_β:
                                                                                        jmp   n51_op11_α
.Lx536_0:
                        .quad            .Lx536_0_s
.Lx536_0_s:
                        .string          "roman/2"
#-----------------------------------------------------------------------------------------------------------------------
n137_op11_α:
                        mov              rax, qword ptr [rbp + 5600]
                        mov              qword ptr [rbp + 5552], rax
                        mov              rax, qword ptr [rbp + 5608]
                        mov              qword ptr [rbp + 5560], rax
                        mov              rax, qword ptr [rbp + 5616]
                        mov              qword ptr [rbp + 5568], rax
                        mov              rax, qword ptr [rbp + 5624]
                        mov              qword ptr [rbp + 5576], rax
                        lea              rdi, [rbp + 5552]
                        mov              esi, 2
                        call             rt_pl_dop_ax_sub@PLT
                        mov              qword ptr [rbp + 5536], rax
                        mov              qword ptr [rbp + 5544], rdx
                        cmp              eax, 99
                                                                                        je    n51_op11_α
                                                                                        jmp   n143_op11_α
n137_op11_β:
                                                                                        jmp   n51_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n138_op19_α:
                                                                                        jmp   n144_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n139_op11_α:
                        mov              rax, qword ptr [rbp + 4480]
                        mov              qword ptr [rbp + 4448], rax
                        mov              rax, qword ptr [rbp + 4488]
                        mov              qword ptr [rbp + 4456], rax
                        mov              rax, qword ptr [rbp + 4496]
                        mov              qword ptr [rbp + 4464], rax
                        mov              rax, qword ptr [rbp + 4504]
                        mov              qword ptr [rbp + 4472], rax
                        lea              rdi, [rbp + 4448]
                        lea              r8, [rbp + 4448]
.Lx539_40:
                        mov              eax, dword ptr [r8 + 0]
                        cmp              eax, 9
                                                                                        jne   .Lx539_56
                        mov              esi, dword ptr [r8 + 4]
                        mov              rax, qword ptr [r8 + 8]
                        test             rax, rax
                                                                                        je    .Lx539_41
                        cmp              esi, 1
                                                                                        jne   .Lx539_55
                        mov              r8, rax
                                                                                        jmp   .Lx539_40
.Lx539_55:
                        cmp              esi, 2
                                                                                        jne   .Lx539_41
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx539_41
                        mov              r8, rax
                                                                                        jmp   .Lx539_40
.Lx539_56:
                        cmp              eax, 13
                                                                                        jne   .Lx539_41
                        mov              rax, qword ptr [r8 + 8]
                        test             rax, rax
                                                                                        je    .Lx539_41
                        cmp              rax, r8
                                                                                        je    .Lx539_41
                        mov              r8, rax
                                                                                        jmp   .Lx539_40
.Lx539_41:
                        lea              r9, [rbp + 4464]
.Lx539_42:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 9
                                                                                        jne   .Lx539_58
                        mov              esi, dword ptr [r9 + 4]
                        mov              rax, qword ptr [r9 + 8]
                        test             rax, rax
                                                                                        je    .Lx539_43
                        cmp              esi, 1
                                                                                        jne   .Lx539_57
                        mov              r9, rax
                                                                                        jmp   .Lx539_42
.Lx539_57:
                        cmp              esi, 2
                                                                                        jne   .Lx539_43
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx539_43
                        mov              r9, rax
                                                                                        jmp   .Lx539_42
.Lx539_58:
                        cmp              eax, 13
                                                                                        jne   .Lx539_43
                        mov              rax, qword ptr [r9 + 8]
                        test             rax, rax
                                                                                        je    .Lx539_43
                        cmp              rax, r9
                                                                                        je    .Lx539_43
                        mov              r9, rax
                                                                                        jmp   .Lx539_42
.Lx539_43:
                        cmp              r8, r9
                                                                                        je    .Lx539_51
                        mov              eax, dword ptr [r8 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx539_44
                        cmp              eax, 99
                                                                                        je    .Lx539_44
                        cmp              eax, 13
                                                                                        jne   .Lx539_45
                        mov              rax, qword ptr [r8 + 8]
                        cmp              rax, r8
                                                                                        je    .Lx539_44
                                                                                        jmp   .Lx539_45
.Lx539_44:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx539_53
                        cmp              eax, 99
                                                                                        je    .Lx539_53
                        cmp              eax, 13
                                                                                        jne   .Lx539_46
                        mov              rax, qword ptr [r9 + 8]
                        cmp              rax, r9
                                                                                        je    .Lx539_53
                                                                                        jmp   .Lx539_46
.Lx539_46:
                        lea              r10, [rip + g_pl_trail]
                        mov              r11, qword ptr [r10 + 0]
                        test             r11, r11
                                                                                        je    .Lx539_53
                        mov              eax, dword ptr [r10 + 32]
                        mov              esi, 24
                        imul             rsi, rax
                        mov              rax, qword ptr [r10 + 24]
                        sub              rax, 24
                        cmp              rsi, rax
                                                                                        ja    .Lx539_53
                        add              r11, rsi
                        mov              qword ptr [r11 + 0], r8
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r11 + 8], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r11 + 16], rax
                        mov              eax, dword ptr [r10 + 32]
                        add              eax, 1
                        mov              dword ptr [r10 + 32], eax
                        mov              rax, qword ptr [r9 + 0]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [r8 + 8], rax
                                                                                        jmp   .Lx539_51
.Lx539_45:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx539_47
                        cmp              eax, 99
                                                                                        je    .Lx539_47
                        cmp              eax, 13
                                                                                        jne   .Lx539_48
                        mov              rax, qword ptr [r9 + 8]
                        cmp              rax, r9
                                                                                        je    .Lx539_47
                                                                                        jmp   .Lx539_48
.Lx539_47:
                        lea              r10, [rip + g_pl_trail]
                        mov              r11, qword ptr [r10 + 0]
                        test             r11, r11
                                                                                        je    .Lx539_53
                        mov              eax, dword ptr [r10 + 32]
                        mov              esi, 24
                        imul             rsi, rax
                        mov              rax, qword ptr [r10 + 24]
                        sub              rax, 24
                        cmp              rsi, rax
                                                                                        ja    .Lx539_53
                        add              r11, rsi
                        mov              qword ptr [r11 + 0], r9
                        mov              rax, qword ptr [r9 + 0]
                        mov              qword ptr [r11 + 8], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [r11 + 16], rax
                        mov              eax, dword ptr [r10 + 32]
                        add              eax, 1
                        mov              dword ptr [r10 + 32], eax
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 8], rax
                                                                                        jmp   .Lx539_51
.Lx539_48:
                        mov              ecx, dword ptr [r8 + 0]
                        mov              edx, dword ptr [r9 + 0]
                        cmp              ecx, 14
                                                                                        jne   .Lx539_49
                        cmp              edx, 14
                                                                                        je    .Lx539_53
                                                                                        jmp   .Lx539_52
.Lx539_49:
                        cmp              edx, 14
                                                                                        je    .Lx539_52
                        cmp              ecx, 7
                                                                                        je    .Lx539_53
                        cmp              edx, 7
                                                                                        je    .Lx539_53
                        cmp              ecx, 6
                                                                                        jne   .Lx539_50
                        cmp              edx, 6
                                                                                        jne   .Lx539_50
                        mov              rax, qword ptr [r8 + 0]
                        cmp              rax, 6
                                                                                        jne   .Lx539_53
                        mov              rax, qword ptr [r9 + 0]
                        cmp              rax, 6
                                                                                        jne   .Lx539_53
                        mov              rax, qword ptr [r8 + 8]
                        mov              rsi, qword ptr [r9 + 8]
                        cmp              rax, rsi
                                                                                        je    .Lx539_51
                                                                                        jmp   .Lx539_52
.Lx539_50:
                        mov              rax, qword ptr [r8 + 0]
                        mov              rsi, qword ptr [r9 + 0]
                        cmp              rax, rsi
                                                                                        jne   .Lx539_53
                        mov              rax, qword ptr [r8 + 8]
                        mov              rsi, qword ptr [r9 + 8]
                        cmp              rax, rsi
                                                                                        jne   .Lx539_53
.Lx539_51:
                        mov              rax, qword ptr [r8 + 0]
                        mov              rdx, qword ptr [r8 + 8]
                                                                                        jmp   .Lx539_54
.Lx539_52:
                        mov              eax, 99
                        mov              edx, 0
                                                                                        jmp   .Lx539_54
.Lx539_53:
                        mov              esi, 2
                        call             rt_pl_dop_unify@PLT
.Lx539_54:
                        mov              qword ptr [rbp + 4432], rax
                        mov              qword ptr [rbp + 4440], rdx
                        cmp              eax, 99
                                                                                        je    n128_op11_α
                                                                                        jmp   n145_var_α
n139_op11_β:
                                                                                        jmp   n128_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n140_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8784]
                        mov              qword ptr [rbp + 3936], rax
                        mov              qword ptr [rbp + 3944], rdx
                                                                                        jmp   n146_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n141_move_label_α:
                        lea              rax, [rip + n112_call_proc_staged_β]
                        mov              qword ptr [rbp + 64], rax
                                                                                        jmp   proc_roman$2F2_γ
#-----------------------------------------------------------------------------------------------------------------------
n142_lit_string_α:
                        mov              qword ptr [rbp + 5968], 1
                        mov              rax, qword ptr [rip + .Lx544_0]
                        mov              qword ptr [rbp + 5976], rax
                                                                                        jmp   n147_var_ref_α
.Lx544_0:
                        .quad            .Lx544_0_s
.Lx544_0_s:
                        .string          "CD"
#-----------------------------------------------------------------------------------------------------------------------
n143_op11_α:
                        mov              rax, qword ptr [rbp + 5520]
                        mov              qword ptr [rbp + 5488], rax
                        mov              rax, qword ptr [rbp + 5528]
                        mov              qword ptr [rbp + 5496], rax
                        mov              rax, qword ptr [rbp + 5536]
                        mov              qword ptr [rbp + 5504], rax
                        mov              rax, qword ptr [rbp + 5544]
                        mov              qword ptr [rbp + 5512], rax
                        lea              rdi, [rbp + 5488]
                        mov              esi, 2
                        call             rt_pl_dop_is_v@PLT
                        mov              qword ptr [rbp + 5472], rax
                        mov              qword ptr [rbp + 5480], rdx
                        cmp              eax, 99
                                                                                        je    n51_op11_α
                                                                                        jmp   n148_var_ref_α
n143_op11_β:
                                                                                        jmp   n51_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n144_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8768]
                        mov              qword ptr [rbp + 4880], rax
                        mov              qword ptr [rbp + 4888], rdx
                                                                                        jmp   n149_var_α
#-----------------------------------------------------------------------------------------------------------------------
n145_var_α:
                        mov              rax, qword ptr [rbp + 8784]
                        mov              qword ptr [rbp + 4400], rax
                        mov              rax, qword ptr [rbp + 8792]
                        mov              qword ptr [rbp + 4408], rax
                                                                                        jmp   n150_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n146_op11_α:
                        mov              rax, qword ptr [rbp + 3920]
                        mov              qword ptr [rbp + 3888], rax
                        mov              rax, qword ptr [rbp + 3928]
                        mov              qword ptr [rbp + 3896], rax
                        mov              rax, qword ptr [rbp + 3936]
                        mov              qword ptr [rbp + 3904], rax
                        mov              rax, qword ptr [rbp + 3944]
                        mov              qword ptr [rbp + 3912], rax
                        lea              rdi, [rbp + 3888]
                        lea              r8, [rbp + 3888]
.Lx550_40:
                        mov              eax, dword ptr [r8 + 0]
                        cmp              eax, 9
                                                                                        jne   .Lx550_56
                        mov              esi, dword ptr [r8 + 4]
                        mov              rax, qword ptr [r8 + 8]
                        test             rax, rax
                                                                                        je    .Lx550_41
                        cmp              esi, 1
                                                                                        jne   .Lx550_55
                        mov              r8, rax
                                                                                        jmp   .Lx550_40
.Lx550_55:
                        cmp              esi, 2
                                                                                        jne   .Lx550_41
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx550_41
                        mov              r8, rax
                                                                                        jmp   .Lx550_40
.Lx550_56:
                        cmp              eax, 13
                                                                                        jne   .Lx550_41
                        mov              rax, qword ptr [r8 + 8]
                        test             rax, rax
                                                                                        je    .Lx550_41
                        cmp              rax, r8
                                                                                        je    .Lx550_41
                        mov              r8, rax
                                                                                        jmp   .Lx550_40
.Lx550_41:
                        lea              r9, [rbp + 3904]
.Lx550_42:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 9
                                                                                        jne   .Lx550_58
                        mov              esi, dword ptr [r9 + 4]
                        mov              rax, qword ptr [r9 + 8]
                        test             rax, rax
                                                                                        je    .Lx550_43
                        cmp              esi, 1
                                                                                        jne   .Lx550_57
                        mov              r9, rax
                                                                                        jmp   .Lx550_42
.Lx550_57:
                        cmp              esi, 2
                                                                                        jne   .Lx550_43
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx550_43
                        mov              r9, rax
                                                                                        jmp   .Lx550_42
.Lx550_58:
                        cmp              eax, 13
                                                                                        jne   .Lx550_43
                        mov              rax, qword ptr [r9 + 8]
                        test             rax, rax
                                                                                        je    .Lx550_43
                        cmp              rax, r9
                                                                                        je    .Lx550_43
                        mov              r9, rax
                                                                                        jmp   .Lx550_42
.Lx550_43:
                        cmp              r8, r9
                                                                                        je    .Lx550_51
                        mov              eax, dword ptr [r8 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx550_44
                        cmp              eax, 99
                                                                                        je    .Lx550_44
                        cmp              eax, 13
                                                                                        jne   .Lx550_45
                        mov              rax, qword ptr [r8 + 8]
                        cmp              rax, r8
                                                                                        je    .Lx550_44
                                                                                        jmp   .Lx550_45
.Lx550_44:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx550_53
                        cmp              eax, 99
                                                                                        je    .Lx550_53
                        cmp              eax, 13
                                                                                        jne   .Lx550_46
                        mov              rax, qword ptr [r9 + 8]
                        cmp              rax, r9
                                                                                        je    .Lx550_53
                                                                                        jmp   .Lx550_46
.Lx550_46:
                        lea              r10, [rip + g_pl_trail]
                        mov              r11, qword ptr [r10 + 0]
                        test             r11, r11
                                                                                        je    .Lx550_53
                        mov              eax, dword ptr [r10 + 32]
                        mov              esi, 24
                        imul             rsi, rax
                        mov              rax, qword ptr [r10 + 24]
                        sub              rax, 24
                        cmp              rsi, rax
                                                                                        ja    .Lx550_53
                        add              r11, rsi
                        mov              qword ptr [r11 + 0], r8
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r11 + 8], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r11 + 16], rax
                        mov              eax, dword ptr [r10 + 32]
                        add              eax, 1
                        mov              dword ptr [r10 + 32], eax
                        mov              rax, qword ptr [r9 + 0]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [r8 + 8], rax
                                                                                        jmp   .Lx550_51
.Lx550_45:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx550_47
                        cmp              eax, 99
                                                                                        je    .Lx550_47
                        cmp              eax, 13
                                                                                        jne   .Lx550_48
                        mov              rax, qword ptr [r9 + 8]
                        cmp              rax, r9
                                                                                        je    .Lx550_47
                                                                                        jmp   .Lx550_48
.Lx550_47:
                        lea              r10, [rip + g_pl_trail]
                        mov              r11, qword ptr [r10 + 0]
                        test             r11, r11
                                                                                        je    .Lx550_53
                        mov              eax, dword ptr [r10 + 32]
                        mov              esi, 24
                        imul             rsi, rax
                        mov              rax, qword ptr [r10 + 24]
                        sub              rax, 24
                        cmp              rsi, rax
                                                                                        ja    .Lx550_53
                        add              r11, rsi
                        mov              qword ptr [r11 + 0], r9
                        mov              rax, qword ptr [r9 + 0]
                        mov              qword ptr [r11 + 8], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [r11 + 16], rax
                        mov              eax, dword ptr [r10 + 32]
                        add              eax, 1
                        mov              dword ptr [r10 + 32], eax
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 8], rax
                                                                                        jmp   .Lx550_51
.Lx550_48:
                        mov              ecx, dword ptr [r8 + 0]
                        mov              edx, dword ptr [r9 + 0]
                        cmp              ecx, 14
                                                                                        jne   .Lx550_49
                        cmp              edx, 14
                                                                                        je    .Lx550_53
                                                                                        jmp   .Lx550_52
.Lx550_49:
                        cmp              edx, 14
                                                                                        je    .Lx550_52
                        cmp              ecx, 7
                                                                                        je    .Lx550_53
                        cmp              edx, 7
                                                                                        je    .Lx550_53
                        cmp              ecx, 6
                                                                                        jne   .Lx550_50
                        cmp              edx, 6
                                                                                        jne   .Lx550_50
                        mov              rax, qword ptr [r8 + 0]
                        cmp              rax, 6
                                                                                        jne   .Lx550_53
                        mov              rax, qword ptr [r9 + 0]
                        cmp              rax, 6
                                                                                        jne   .Lx550_53
                        mov              rax, qword ptr [r8 + 8]
                        mov              rsi, qword ptr [r9 + 8]
                        cmp              rax, rsi
                                                                                        je    .Lx550_51
                                                                                        jmp   .Lx550_52
.Lx550_50:
                        mov              rax, qword ptr [r8 + 0]
                        mov              rsi, qword ptr [r9 + 0]
                        cmp              rax, rsi
                                                                                        jne   .Lx550_53
                        mov              rax, qword ptr [r8 + 8]
                        mov              rsi, qword ptr [r9 + 8]
                        cmp              rax, rsi
                                                                                        jne   .Lx550_53
.Lx550_51:
                        mov              rax, qword ptr [r8 + 0]
                        mov              rdx, qword ptr [r8 + 8]
                                                                                        jmp   .Lx550_54
.Lx550_52:
                        mov              eax, 99
                        mov              edx, 0
                                                                                        jmp   .Lx550_54
.Lx550_53:
                        mov              esi, 2
                        call             rt_pl_dop_unify@PLT
.Lx550_54:
                        mov              qword ptr [rbp + 3872], rax
                        mov              qword ptr [rbp + 3880], rdx
                        cmp              eax, 99
                                                                                        je    n152_op11_α
                                                                                        jmp   n151_var_ref_α
n146_op11_β:
                                                                                        jmp   n152_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n147_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8736]
                        mov              qword ptr [rbp + 5984], rax
                        mov              qword ptr [rbp + 5992], rdx
                                                                                        jmp   n153_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n148_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8768]
                        mov              qword ptr [rbp + 5440], rax
                        mov              qword ptr [rbp + 5448], rdx
                                                                                        jmp   n154_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n149_var_α:
                        mov              rax, qword ptr [rbp + 8784]
                        mov              qword ptr [rbp + 4960], rax
                        mov              rax, qword ptr [rbp + 8792]
                        mov              qword ptr [rbp + 4968], rax
                                                                                        jmp   n155_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n150_lit_integer_α:
                        mov              qword ptr [rbp + 4416], 6
                        mov              rax, qword ptr [rip + .Lx557_0]
                        mov              qword ptr [rbp + 4424], rax
                                                                                        jmp   n156_op11_α
.Lx557_0:
                        .quad            50
#-----------------------------------------------------------------------------------------------------------------------
n151_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 32]
                        mov              qword ptr [rbp + 3840], rax
                        mov              qword ptr [rbp + 3848], rdx
                                                                                        jmp   n157_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n152_op11_α:
                        mov              rax, qword ptr [rbp + 80]
                        mov              qword ptr [rbp + 3328], rax
                        mov              rax, qword ptr [rbp + 88]
                        mov              qword ptr [rbp + 3336], rax
                        lea              rdi, [rbp + 3328]
                        mov              esi, 1
                        call             rt_pl_dop_unwind_nothrow@PLT
                        mov              qword ptr [rbp + 3312], rax
                        mov              qword ptr [rbp + 3320], rdx
                        cmp              eax, 99
                                                                                        je    proc_roman$2F2_ω
                                                                                        jmp   n158_var_ref_α
n152_op11_β:
                                                                                        jmp   proc_roman$2F2_ω
#-----------------------------------------------------------------------------------------------------------------------
n153_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8752]
                        mov              qword ptr [rbp + 6000], rax
                        mov              qword ptr [rbp + 6008], rdx
                                                                                        jmp   n159_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n154_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8736]
                        mov              qword ptr [rbp + 5456], rax
                        mov              qword ptr [rbp + 5464], rdx
                                                                                        jmp   n160_call_proc_staged_α
#-----------------------------------------------------------------------------------------------------------------------
n155_lit_integer_α:
                        mov              qword ptr [rbp + 4976], 6
                        mov              rax, qword ptr [rip + .Lx565_0]
                        mov              qword ptr [rbp + 4984], rax
                                                                                        jmp   n161_op11_α
.Lx565_0:
                        .quad            90
#-----------------------------------------------------------------------------------------------------------------------
n156_op11_α:
                        mov              rax, qword ptr [rbp + 4400]
                        mov              qword ptr [rbp + 4368], rax
                        mov              rax, qword ptr [rbp + 4408]
                        mov              qword ptr [rbp + 4376], rax
                        mov              rax, qword ptr [rbp + 4416]
                        mov              qword ptr [rbp + 4384], rax
                        mov              rax, qword ptr [rbp + 4424]
                        mov              qword ptr [rbp + 4392], rax
                        lea              rdi, [rbp + 4368]
                        mov              esi, 2
                        call             rt_pl_dop_cmp_ge@PLT
                        mov              qword ptr [rbp + 4352], rax
                        mov              qword ptr [rbp + 4360], rdx
                        cmp              eax, 99
                                                                                        je    n128_op11_α
                                                                                        jmp   n162_op19_α
n156_op11_β:
                                                                                        jmp   n128_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n157_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8752]
                        mov              qword ptr [rbp + 3856], rax
                        mov              qword ptr [rbp + 3864], rdx
                                                                                        jmp   n163_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n158_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 16]
                        mov              qword ptr [rbp + 3280], rax
                        mov              qword ptr [rbp + 3288], rdx
                                                                                        jmp   n164_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n159_op11_α:
                        mov              rax, qword ptr [rbp + 5968]
                        mov              qword ptr [rbp + 5920], rax
                        mov              rax, qword ptr [rbp + 5976]
                        mov              qword ptr [rbp + 5928], rax
                        mov              rax, qword ptr [rbp + 5984]
                        mov              qword ptr [rbp + 5936], rax
                        mov              rax, qword ptr [rbp + 5992]
                        mov              qword ptr [rbp + 5944], rax
                        mov              rax, qword ptr [rbp + 6000]
                        mov              qword ptr [rbp + 5952], rax
                        mov              rax, qword ptr [rbp + 6008]
                        mov              qword ptr [rbp + 5960], rax
                        .section         .rodata
.Lrkfn572:              .string          "$atom_concat"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn572]
                        lea              rsi, [rbp + 5920]
                        mov              edx, 3
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 5904], rax
                        mov              qword ptr [rbp + 5912], rdx
                        cmp              eax, 99
                                                                                        je    n136_call_proc_staged_β
                                                                                        jmp   n165_move_label_α
n159_op11_β:
                                                                                        jmp   n136_call_proc_staged_β
#-----------------------------------------------------------------------------------------------------------------------
n160_call_proc_staged_α:
                        lea              rsi, [rbp + 5440]
                        lea              rdx, [rbp + 5456]
                        call             proc_roman$2F2_dcα
                                                                                        jmp   .Lx574_2
.Lx574_2:
                        mov              qword ptr [rbp + 5376], rax
                        mov              qword ptr [rbp + 5384], rdx
                        cmp              eax, 99
                                                                                        je    n51_op11_α
                                                                                        jmp   n166_lit_string_α
n160_call_proc_staged_β:
                                                                                        jmp   n51_op11_α
.Lx574_0:
                        .quad            .Lx574_0_s
.Lx574_0_s:
                        .string          "roman/2"
#-----------------------------------------------------------------------------------------------------------------------
n161_op11_α:
                        mov              rax, qword ptr [rbp + 4960]
                        mov              qword ptr [rbp + 4912], rax
                        mov              rax, qword ptr [rbp + 4968]
                        mov              qword ptr [rbp + 4920], rax
                        mov              rax, qword ptr [rbp + 4976]
                        mov              qword ptr [rbp + 4928], rax
                        mov              rax, qword ptr [rbp + 4984]
                        mov              qword ptr [rbp + 4936], rax
                        lea              rdi, [rbp + 4912]
                        mov              esi, 2
                        call             rt_pl_dop_ax_sub@PLT
                        mov              qword ptr [rbp + 4896], rax
                        mov              qword ptr [rbp + 4904], rdx
                        cmp              eax, 99
                                                                                        je    n51_op11_α
                                                                                        jmp   n167_op11_α
n161_op11_β:
                                                                                        jmp   n51_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n162_op19_α:
                                                                                        jmp   n168_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n163_op11_α:
                        mov              rax, qword ptr [rbp + 3840]
                        mov              qword ptr [rbp + 3808], rax
                        mov              rax, qword ptr [rbp + 3848]
                        mov              qword ptr [rbp + 3816], rax
                        mov              rax, qword ptr [rbp + 3856]
                        mov              qword ptr [rbp + 3824], rax
                        mov              rax, qword ptr [rbp + 3864]
                        mov              qword ptr [rbp + 3832], rax
                        lea              rdi, [rbp + 3808]
                        lea              r8, [rbp + 3808]
.Lx577_40:
                        mov              eax, dword ptr [r8 + 0]
                        cmp              eax, 9
                                                                                        jne   .Lx577_56
                        mov              esi, dword ptr [r8 + 4]
                        mov              rax, qword ptr [r8 + 8]
                        test             rax, rax
                                                                                        je    .Lx577_41
                        cmp              esi, 1
                                                                                        jne   .Lx577_55
                        mov              r8, rax
                                                                                        jmp   .Lx577_40
.Lx577_55:
                        cmp              esi, 2
                                                                                        jne   .Lx577_41
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx577_41
                        mov              r8, rax
                                                                                        jmp   .Lx577_40
.Lx577_56:
                        cmp              eax, 13
                                                                                        jne   .Lx577_41
                        mov              rax, qword ptr [r8 + 8]
                        test             rax, rax
                                                                                        je    .Lx577_41
                        cmp              rax, r8
                                                                                        je    .Lx577_41
                        mov              r8, rax
                                                                                        jmp   .Lx577_40
.Lx577_41:
                        lea              r9, [rbp + 3824]
.Lx577_42:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 9
                                                                                        jne   .Lx577_58
                        mov              esi, dword ptr [r9 + 4]
                        mov              rax, qword ptr [r9 + 8]
                        test             rax, rax
                                                                                        je    .Lx577_43
                        cmp              esi, 1
                                                                                        jne   .Lx577_57
                        mov              r9, rax
                                                                                        jmp   .Lx577_42
.Lx577_57:
                        cmp              esi, 2
                                                                                        jne   .Lx577_43
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx577_43
                        mov              r9, rax
                                                                                        jmp   .Lx577_42
.Lx577_58:
                        cmp              eax, 13
                                                                                        jne   .Lx577_43
                        mov              rax, qword ptr [r9 + 8]
                        test             rax, rax
                                                                                        je    .Lx577_43
                        cmp              rax, r9
                                                                                        je    .Lx577_43
                        mov              r9, rax
                                                                                        jmp   .Lx577_42
.Lx577_43:
                        cmp              r8, r9
                                                                                        je    .Lx577_51
                        mov              eax, dword ptr [r8 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx577_44
                        cmp              eax, 99
                                                                                        je    .Lx577_44
                        cmp              eax, 13
                                                                                        jne   .Lx577_45
                        mov              rax, qword ptr [r8 + 8]
                        cmp              rax, r8
                                                                                        je    .Lx577_44
                                                                                        jmp   .Lx577_45
.Lx577_44:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx577_53
                        cmp              eax, 99
                                                                                        je    .Lx577_53
                        cmp              eax, 13
                                                                                        jne   .Lx577_46
                        mov              rax, qword ptr [r9 + 8]
                        cmp              rax, r9
                                                                                        je    .Lx577_53
                                                                                        jmp   .Lx577_46
.Lx577_46:
                        lea              r10, [rip + g_pl_trail]
                        mov              r11, qword ptr [r10 + 0]
                        test             r11, r11
                                                                                        je    .Lx577_53
                        mov              eax, dword ptr [r10 + 32]
                        mov              esi, 24
                        imul             rsi, rax
                        mov              rax, qword ptr [r10 + 24]
                        sub              rax, 24
                        cmp              rsi, rax
                                                                                        ja    .Lx577_53
                        add              r11, rsi
                        mov              qword ptr [r11 + 0], r8
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r11 + 8], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r11 + 16], rax
                        mov              eax, dword ptr [r10 + 32]
                        add              eax, 1
                        mov              dword ptr [r10 + 32], eax
                        mov              rax, qword ptr [r9 + 0]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [r8 + 8], rax
                                                                                        jmp   .Lx577_51
.Lx577_45:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx577_47
                        cmp              eax, 99
                                                                                        je    .Lx577_47
                        cmp              eax, 13
                                                                                        jne   .Lx577_48
                        mov              rax, qword ptr [r9 + 8]
                        cmp              rax, r9
                                                                                        je    .Lx577_47
                                                                                        jmp   .Lx577_48
.Lx577_47:
                        lea              r10, [rip + g_pl_trail]
                        mov              r11, qword ptr [r10 + 0]
                        test             r11, r11
                                                                                        je    .Lx577_53
                        mov              eax, dword ptr [r10 + 32]
                        mov              esi, 24
                        imul             rsi, rax
                        mov              rax, qword ptr [r10 + 24]
                        sub              rax, 24
                        cmp              rsi, rax
                                                                                        ja    .Lx577_53
                        add              r11, rsi
                        mov              qword ptr [r11 + 0], r9
                        mov              rax, qword ptr [r9 + 0]
                        mov              qword ptr [r11 + 8], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [r11 + 16], rax
                        mov              eax, dword ptr [r10 + 32]
                        add              eax, 1
                        mov              dword ptr [r10 + 32], eax
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 8], rax
                                                                                        jmp   .Lx577_51
.Lx577_48:
                        mov              ecx, dword ptr [r8 + 0]
                        mov              edx, dword ptr [r9 + 0]
                        cmp              ecx, 14
                                                                                        jne   .Lx577_49
                        cmp              edx, 14
                                                                                        je    .Lx577_53
                                                                                        jmp   .Lx577_52
.Lx577_49:
                        cmp              edx, 14
                                                                                        je    .Lx577_52
                        cmp              ecx, 7
                                                                                        je    .Lx577_53
                        cmp              edx, 7
                                                                                        je    .Lx577_53
                        cmp              ecx, 6
                                                                                        jne   .Lx577_50
                        cmp              edx, 6
                                                                                        jne   .Lx577_50
                        mov              rax, qword ptr [r8 + 0]
                        cmp              rax, 6
                                                                                        jne   .Lx577_53
                        mov              rax, qword ptr [r9 + 0]
                        cmp              rax, 6
                                                                                        jne   .Lx577_53
                        mov              rax, qword ptr [r8 + 8]
                        mov              rsi, qword ptr [r9 + 8]
                        cmp              rax, rsi
                                                                                        je    .Lx577_51
                                                                                        jmp   .Lx577_52
.Lx577_50:
                        mov              rax, qword ptr [r8 + 0]
                        mov              rsi, qword ptr [r9 + 0]
                        cmp              rax, rsi
                                                                                        jne   .Lx577_53
                        mov              rax, qword ptr [r8 + 8]
                        mov              rsi, qword ptr [r9 + 8]
                        cmp              rax, rsi
                                                                                        jne   .Lx577_53
.Lx577_51:
                        mov              rax, qword ptr [r8 + 0]
                        mov              rdx, qword ptr [r8 + 8]
                                                                                        jmp   .Lx577_54
.Lx577_52:
                        mov              eax, 99
                        mov              edx, 0
                                                                                        jmp   .Lx577_54
.Lx577_53:
                        mov              esi, 2
                        call             rt_pl_dop_unify@PLT
.Lx577_54:
                        mov              qword ptr [rbp + 3792], rax
                        mov              qword ptr [rbp + 3800], rdx
                        cmp              eax, 99
                                                                                        je    n152_op11_α
                                                                                        jmp   n169_var_α
n163_op11_β:
                                                                                        jmp   n152_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n164_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8784]
                        mov              qword ptr [rbp + 3296], rax
                        mov              qword ptr [rbp + 3304], rdx
                                                                                        jmp   n170_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n165_move_label_α:
                        lea              rax, [rip + n136_call_proc_staged_β]
                        mov              qword ptr [rbp + 64], rax
                                                                                        jmp   proc_roman$2F2_γ
#-----------------------------------------------------------------------------------------------------------------------
n166_lit_string_α:
                        mov              qword ptr [rbp + 5328], 1
                        mov              rax, qword ptr [rip + .Lx582_0]
                        mov              qword ptr [rbp + 5336], rax
                                                                                        jmp   n171_var_ref_α
.Lx582_0:
                        .quad            .Lx582_0_s
.Lx582_0_s:
                        .string          "C"
#-----------------------------------------------------------------------------------------------------------------------
n167_op11_α:
                        mov              rax, qword ptr [rbp + 4880]
                        mov              qword ptr [rbp + 4848], rax
                        mov              rax, qword ptr [rbp + 4888]
                        mov              qword ptr [rbp + 4856], rax
                        mov              rax, qword ptr [rbp + 4896]
                        mov              qword ptr [rbp + 4864], rax
                        mov              rax, qword ptr [rbp + 4904]
                        mov              qword ptr [rbp + 4872], rax
                        lea              rdi, [rbp + 4848]
                        mov              esi, 2
                        call             rt_pl_dop_is_v@PLT
                        mov              qword ptr [rbp + 4832], rax
                        mov              qword ptr [rbp + 4840], rdx
                        cmp              eax, 99
                                                                                        je    n51_op11_α
                                                                                        jmp   n172_var_ref_α
n167_op11_β:
                                                                                        jmp   n51_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n168_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8768]
                        mov              qword ptr [rbp + 4240], rax
                        mov              qword ptr [rbp + 4248], rdx
                                                                                        jmp   n173_var_α
#-----------------------------------------------------------------------------------------------------------------------
n169_var_α:
                        mov              rax, qword ptr [rbp + 8784]
                        mov              qword ptr [rbp + 3760], rax
                        mov              rax, qword ptr [rbp + 8792]
                        mov              qword ptr [rbp + 3768], rax
                                                                                        jmp   n174_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n170_op11_α:
                        mov              rax, qword ptr [rbp + 3280]
                        mov              qword ptr [rbp + 3248], rax
                        mov              rax, qword ptr [rbp + 3288]
                        mov              qword ptr [rbp + 3256], rax
                        mov              rax, qword ptr [rbp + 3296]
                        mov              qword ptr [rbp + 3264], rax
                        mov              rax, qword ptr [rbp + 3304]
                        mov              qword ptr [rbp + 3272], rax
                        lea              rdi, [rbp + 3248]
                        lea              r8, [rbp + 3248]
.Lx588_40:
                        mov              eax, dword ptr [r8 + 0]
                        cmp              eax, 9
                                                                                        jne   .Lx588_56
                        mov              esi, dword ptr [r8 + 4]
                        mov              rax, qword ptr [r8 + 8]
                        test             rax, rax
                                                                                        je    .Lx588_41
                        cmp              esi, 1
                                                                                        jne   .Lx588_55
                        mov              r8, rax
                                                                                        jmp   .Lx588_40
.Lx588_55:
                        cmp              esi, 2
                                                                                        jne   .Lx588_41
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx588_41
                        mov              r8, rax
                                                                                        jmp   .Lx588_40
.Lx588_56:
                        cmp              eax, 13
                                                                                        jne   .Lx588_41
                        mov              rax, qword ptr [r8 + 8]
                        test             rax, rax
                                                                                        je    .Lx588_41
                        cmp              rax, r8
                                                                                        je    .Lx588_41
                        mov              r8, rax
                                                                                        jmp   .Lx588_40
.Lx588_41:
                        lea              r9, [rbp + 3264]
.Lx588_42:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 9
                                                                                        jne   .Lx588_58
                        mov              esi, dword ptr [r9 + 4]
                        mov              rax, qword ptr [r9 + 8]
                        test             rax, rax
                                                                                        je    .Lx588_43
                        cmp              esi, 1
                                                                                        jne   .Lx588_57
                        mov              r9, rax
                                                                                        jmp   .Lx588_42
.Lx588_57:
                        cmp              esi, 2
                                                                                        jne   .Lx588_43
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx588_43
                        mov              r9, rax
                                                                                        jmp   .Lx588_42
.Lx588_58:
                        cmp              eax, 13
                                                                                        jne   .Lx588_43
                        mov              rax, qword ptr [r9 + 8]
                        test             rax, rax
                                                                                        je    .Lx588_43
                        cmp              rax, r9
                                                                                        je    .Lx588_43
                        mov              r9, rax
                                                                                        jmp   .Lx588_42
.Lx588_43:
                        cmp              r8, r9
                                                                                        je    .Lx588_51
                        mov              eax, dword ptr [r8 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx588_44
                        cmp              eax, 99
                                                                                        je    .Lx588_44
                        cmp              eax, 13
                                                                                        jne   .Lx588_45
                        mov              rax, qword ptr [r8 + 8]
                        cmp              rax, r8
                                                                                        je    .Lx588_44
                                                                                        jmp   .Lx588_45
.Lx588_44:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx588_53
                        cmp              eax, 99
                                                                                        je    .Lx588_53
                        cmp              eax, 13
                                                                                        jne   .Lx588_46
                        mov              rax, qword ptr [r9 + 8]
                        cmp              rax, r9
                                                                                        je    .Lx588_53
                                                                                        jmp   .Lx588_46
.Lx588_46:
                        lea              r10, [rip + g_pl_trail]
                        mov              r11, qword ptr [r10 + 0]
                        test             r11, r11
                                                                                        je    .Lx588_53
                        mov              eax, dword ptr [r10 + 32]
                        mov              esi, 24
                        imul             rsi, rax
                        mov              rax, qword ptr [r10 + 24]
                        sub              rax, 24
                        cmp              rsi, rax
                                                                                        ja    .Lx588_53
                        add              r11, rsi
                        mov              qword ptr [r11 + 0], r8
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r11 + 8], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r11 + 16], rax
                        mov              eax, dword ptr [r10 + 32]
                        add              eax, 1
                        mov              dword ptr [r10 + 32], eax
                        mov              rax, qword ptr [r9 + 0]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [r8 + 8], rax
                                                                                        jmp   .Lx588_51
.Lx588_45:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx588_47
                        cmp              eax, 99
                                                                                        je    .Lx588_47
                        cmp              eax, 13
                                                                                        jne   .Lx588_48
                        mov              rax, qword ptr [r9 + 8]
                        cmp              rax, r9
                                                                                        je    .Lx588_47
                                                                                        jmp   .Lx588_48
.Lx588_47:
                        lea              r10, [rip + g_pl_trail]
                        mov              r11, qword ptr [r10 + 0]
                        test             r11, r11
                                                                                        je    .Lx588_53
                        mov              eax, dword ptr [r10 + 32]
                        mov              esi, 24
                        imul             rsi, rax
                        mov              rax, qword ptr [r10 + 24]
                        sub              rax, 24
                        cmp              rsi, rax
                                                                                        ja    .Lx588_53
                        add              r11, rsi
                        mov              qword ptr [r11 + 0], r9
                        mov              rax, qword ptr [r9 + 0]
                        mov              qword ptr [r11 + 8], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [r11 + 16], rax
                        mov              eax, dword ptr [r10 + 32]
                        add              eax, 1
                        mov              dword ptr [r10 + 32], eax
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 8], rax
                                                                                        jmp   .Lx588_51
.Lx588_48:
                        mov              ecx, dword ptr [r8 + 0]
                        mov              edx, dword ptr [r9 + 0]
                        cmp              ecx, 14
                                                                                        jne   .Lx588_49
                        cmp              edx, 14
                                                                                        je    .Lx588_53
                                                                                        jmp   .Lx588_52
.Lx588_49:
                        cmp              edx, 14
                                                                                        je    .Lx588_52
                        cmp              ecx, 7
                                                                                        je    .Lx588_53
                        cmp              edx, 7
                                                                                        je    .Lx588_53
                        cmp              ecx, 6
                                                                                        jne   .Lx588_50
                        cmp              edx, 6
                                                                                        jne   .Lx588_50
                        mov              rax, qword ptr [r8 + 0]
                        cmp              rax, 6
                                                                                        jne   .Lx588_53
                        mov              rax, qword ptr [r9 + 0]
                        cmp              rax, 6
                                                                                        jne   .Lx588_53
                        mov              rax, qword ptr [r8 + 8]
                        mov              rsi, qword ptr [r9 + 8]
                        cmp              rax, rsi
                                                                                        je    .Lx588_51
                                                                                        jmp   .Lx588_52
.Lx588_50:
                        mov              rax, qword ptr [r8 + 0]
                        mov              rsi, qword ptr [r9 + 0]
                        cmp              rax, rsi
                                                                                        jne   .Lx588_53
                        mov              rax, qword ptr [r8 + 8]
                        mov              rsi, qword ptr [r9 + 8]
                        cmp              rax, rsi
                                                                                        jne   .Lx588_53
.Lx588_51:
                        mov              rax, qword ptr [r8 + 0]
                        mov              rdx, qword ptr [r8 + 8]
                                                                                        jmp   .Lx588_54
.Lx588_52:
                        mov              eax, 99
                        mov              edx, 0
                                                                                        jmp   .Lx588_54
.Lx588_53:
                        mov              esi, 2
                        call             rt_pl_dop_unify@PLT
.Lx588_54:
                        mov              qword ptr [rbp + 3232], rax
                        mov              qword ptr [rbp + 3240], rdx
                        cmp              eax, 99
                                                                                        je    n176_op11_α
                                                                                        jmp   n175_var_ref_α
n170_op11_β:
                                                                                        jmp   n176_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n171_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8736]
                        mov              qword ptr [rbp + 5344], rax
                        mov              qword ptr [rbp + 5352], rdx
                                                                                        jmp   n177_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n172_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8768]
                        mov              qword ptr [rbp + 4800], rax
                        mov              qword ptr [rbp + 4808], rdx
                                                                                        jmp   n178_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n173_var_α:
                        mov              rax, qword ptr [rbp + 8784]
                        mov              qword ptr [rbp + 4320], rax
                        mov              rax, qword ptr [rbp + 8792]
                        mov              qword ptr [rbp + 4328], rax
                                                                                        jmp   n179_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n174_lit_integer_α:
                        mov              qword ptr [rbp + 3776], 6
                        mov              rax, qword ptr [rip + .Lx595_0]
                        mov              qword ptr [rbp + 3784], rax
                                                                                        jmp   n180_op11_α
.Lx595_0:
                        .quad            40
#-----------------------------------------------------------------------------------------------------------------------
n175_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 32]
                        mov              qword ptr [rbp + 3200], rax
                        mov              qword ptr [rbp + 3208], rdx
                                                                                        jmp   n181_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n176_op11_α:
                        mov              rax, qword ptr [rbp + 80]
                        mov              qword ptr [rbp + 2688], rax
                        mov              rax, qword ptr [rbp + 88]
                        mov              qword ptr [rbp + 2696], rax
                        lea              rdi, [rbp + 2688]
                        mov              esi, 1
                        call             rt_pl_dop_unwind_nothrow@PLT
                        mov              qword ptr [rbp + 2672], rax
                        mov              qword ptr [rbp + 2680], rdx
                        cmp              eax, 99
                                                                                        je    proc_roman$2F2_ω
                                                                                        jmp   n182_var_ref_α
n176_op11_β:
                                                                                        jmp   proc_roman$2F2_ω
#-----------------------------------------------------------------------------------------------------------------------
n177_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8752]
                        mov              qword ptr [rbp + 5360], rax
                        mov              qword ptr [rbp + 5368], rdx
                                                                                        jmp   n183_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n178_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8736]
                        mov              qword ptr [rbp + 4816], rax
                        mov              qword ptr [rbp + 4824], rdx
                                                                                        jmp   n184_call_proc_staged_α
#-----------------------------------------------------------------------------------------------------------------------
n179_lit_integer_α:
                        mov              qword ptr [rbp + 4336], 6
                        mov              rax, qword ptr [rip + .Lx603_0]
                        mov              qword ptr [rbp + 4344], rax
                                                                                        jmp   n185_op11_α
.Lx603_0:
                        .quad            50
#-----------------------------------------------------------------------------------------------------------------------
n180_op11_α:
                        mov              rax, qword ptr [rbp + 3760]
                        mov              qword ptr [rbp + 3728], rax
                        mov              rax, qword ptr [rbp + 3768]
                        mov              qword ptr [rbp + 3736], rax
                        mov              rax, qword ptr [rbp + 3776]
                        mov              qword ptr [rbp + 3744], rax
                        mov              rax, qword ptr [rbp + 3784]
                        mov              qword ptr [rbp + 3752], rax
                        lea              rdi, [rbp + 3728]
                        mov              esi, 2
                        call             rt_pl_dop_cmp_ge@PLT
                        mov              qword ptr [rbp + 3712], rax
                        mov              qword ptr [rbp + 3720], rdx
                        cmp              eax, 99
                                                                                        je    n152_op11_α
                                                                                        jmp   n186_op19_α
n180_op11_β:
                                                                                        jmp   n152_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n181_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8752]
                        mov              qword ptr [rbp + 3216], rax
                        mov              qword ptr [rbp + 3224], rdx
                                                                                        jmp   n187_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n182_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 16]
                        mov              qword ptr [rbp + 2640], rax
                        mov              qword ptr [rbp + 2648], rdx
                                                                                        jmp   n188_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n183_op11_α:
                        mov              rax, qword ptr [rbp + 5328]
                        mov              qword ptr [rbp + 5280], rax
                        mov              rax, qword ptr [rbp + 5336]
                        mov              qword ptr [rbp + 5288], rax
                        mov              rax, qword ptr [rbp + 5344]
                        mov              qword ptr [rbp + 5296], rax
                        mov              rax, qword ptr [rbp + 5352]
                        mov              qword ptr [rbp + 5304], rax
                        mov              rax, qword ptr [rbp + 5360]
                        mov              qword ptr [rbp + 5312], rax
                        mov              rax, qword ptr [rbp + 5368]
                        mov              qword ptr [rbp + 5320], rax
                        .section         .rodata
.Lrkfn610:              .string          "$atom_concat"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn610]
                        lea              rsi, [rbp + 5280]
                        mov              edx, 3
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 5264], rax
                        mov              qword ptr [rbp + 5272], rdx
                        cmp              eax, 99
                                                                                        je    n160_call_proc_staged_β
                                                                                        jmp   n189_move_label_α
n183_op11_β:
                                                                                        jmp   n160_call_proc_staged_β
#-----------------------------------------------------------------------------------------------------------------------
n184_call_proc_staged_α:
                        lea              rsi, [rbp + 4800]
                        lea              rdx, [rbp + 4816]
                        call             proc_roman$2F2_dcα
                                                                                        jmp   .Lx612_2
.Lx612_2:
                        mov              qword ptr [rbp + 4736], rax
                        mov              qword ptr [rbp + 4744], rdx
                        cmp              eax, 99
                                                                                        je    n51_op11_α
                                                                                        jmp   n190_lit_string_α
n184_call_proc_staged_β:
                                                                                        jmp   n51_op11_α
.Lx612_0:
                        .quad            .Lx612_0_s
.Lx612_0_s:
                        .string          "roman/2"
#-----------------------------------------------------------------------------------------------------------------------
n185_op11_α:
                        mov              rax, qword ptr [rbp + 4320]
                        mov              qword ptr [rbp + 4272], rax
                        mov              rax, qword ptr [rbp + 4328]
                        mov              qword ptr [rbp + 4280], rax
                        mov              rax, qword ptr [rbp + 4336]
                        mov              qword ptr [rbp + 4288], rax
                        mov              rax, qword ptr [rbp + 4344]
                        mov              qword ptr [rbp + 4296], rax
                        lea              rdi, [rbp + 4272]
                        mov              esi, 2
                        call             rt_pl_dop_ax_sub@PLT
                        mov              qword ptr [rbp + 4256], rax
                        mov              qword ptr [rbp + 4264], rdx
                        cmp              eax, 99
                                                                                        je    n51_op11_α
                                                                                        jmp   n191_op11_α
n185_op11_β:
                                                                                        jmp   n51_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n186_op19_α:
                                                                                        jmp   n192_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n187_op11_α:
                        mov              rax, qword ptr [rbp + 3200]
                        mov              qword ptr [rbp + 3168], rax
                        mov              rax, qword ptr [rbp + 3208]
                        mov              qword ptr [rbp + 3176], rax
                        mov              rax, qword ptr [rbp + 3216]
                        mov              qword ptr [rbp + 3184], rax
                        mov              rax, qword ptr [rbp + 3224]
                        mov              qword ptr [rbp + 3192], rax
                        lea              rdi, [rbp + 3168]
                        lea              r8, [rbp + 3168]
.Lx615_40:
                        mov              eax, dword ptr [r8 + 0]
                        cmp              eax, 9
                                                                                        jne   .Lx615_56
                        mov              esi, dword ptr [r8 + 4]
                        mov              rax, qword ptr [r8 + 8]
                        test             rax, rax
                                                                                        je    .Lx615_41
                        cmp              esi, 1
                                                                                        jne   .Lx615_55
                        mov              r8, rax
                                                                                        jmp   .Lx615_40
.Lx615_55:
                        cmp              esi, 2
                                                                                        jne   .Lx615_41
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx615_41
                        mov              r8, rax
                                                                                        jmp   .Lx615_40
.Lx615_56:
                        cmp              eax, 13
                                                                                        jne   .Lx615_41
                        mov              rax, qword ptr [r8 + 8]
                        test             rax, rax
                                                                                        je    .Lx615_41
                        cmp              rax, r8
                                                                                        je    .Lx615_41
                        mov              r8, rax
                                                                                        jmp   .Lx615_40
.Lx615_41:
                        lea              r9, [rbp + 3184]
.Lx615_42:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 9
                                                                                        jne   .Lx615_58
                        mov              esi, dword ptr [r9 + 4]
                        mov              rax, qword ptr [r9 + 8]
                        test             rax, rax
                                                                                        je    .Lx615_43
                        cmp              esi, 1
                                                                                        jne   .Lx615_57
                        mov              r9, rax
                                                                                        jmp   .Lx615_42
.Lx615_57:
                        cmp              esi, 2
                                                                                        jne   .Lx615_43
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx615_43
                        mov              r9, rax
                                                                                        jmp   .Lx615_42
.Lx615_58:
                        cmp              eax, 13
                                                                                        jne   .Lx615_43
                        mov              rax, qword ptr [r9 + 8]
                        test             rax, rax
                                                                                        je    .Lx615_43
                        cmp              rax, r9
                                                                                        je    .Lx615_43
                        mov              r9, rax
                                                                                        jmp   .Lx615_42
.Lx615_43:
                        cmp              r8, r9
                                                                                        je    .Lx615_51
                        mov              eax, dword ptr [r8 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx615_44
                        cmp              eax, 99
                                                                                        je    .Lx615_44
                        cmp              eax, 13
                                                                                        jne   .Lx615_45
                        mov              rax, qword ptr [r8 + 8]
                        cmp              rax, r8
                                                                                        je    .Lx615_44
                                                                                        jmp   .Lx615_45
.Lx615_44:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx615_53
                        cmp              eax, 99
                                                                                        je    .Lx615_53
                        cmp              eax, 13
                                                                                        jne   .Lx615_46
                        mov              rax, qword ptr [r9 + 8]
                        cmp              rax, r9
                                                                                        je    .Lx615_53
                                                                                        jmp   .Lx615_46
.Lx615_46:
                        lea              r10, [rip + g_pl_trail]
                        mov              r11, qword ptr [r10 + 0]
                        test             r11, r11
                                                                                        je    .Lx615_53
                        mov              eax, dword ptr [r10 + 32]
                        mov              esi, 24
                        imul             rsi, rax
                        mov              rax, qword ptr [r10 + 24]
                        sub              rax, 24
                        cmp              rsi, rax
                                                                                        ja    .Lx615_53
                        add              r11, rsi
                        mov              qword ptr [r11 + 0], r8
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r11 + 8], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r11 + 16], rax
                        mov              eax, dword ptr [r10 + 32]
                        add              eax, 1
                        mov              dword ptr [r10 + 32], eax
                        mov              rax, qword ptr [r9 + 0]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [r8 + 8], rax
                                                                                        jmp   .Lx615_51
.Lx615_45:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx615_47
                        cmp              eax, 99
                                                                                        je    .Lx615_47
                        cmp              eax, 13
                                                                                        jne   .Lx615_48
                        mov              rax, qword ptr [r9 + 8]
                        cmp              rax, r9
                                                                                        je    .Lx615_47
                                                                                        jmp   .Lx615_48
.Lx615_47:
                        lea              r10, [rip + g_pl_trail]
                        mov              r11, qword ptr [r10 + 0]
                        test             r11, r11
                                                                                        je    .Lx615_53
                        mov              eax, dword ptr [r10 + 32]
                        mov              esi, 24
                        imul             rsi, rax
                        mov              rax, qword ptr [r10 + 24]
                        sub              rax, 24
                        cmp              rsi, rax
                                                                                        ja    .Lx615_53
                        add              r11, rsi
                        mov              qword ptr [r11 + 0], r9
                        mov              rax, qword ptr [r9 + 0]
                        mov              qword ptr [r11 + 8], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [r11 + 16], rax
                        mov              eax, dword ptr [r10 + 32]
                        add              eax, 1
                        mov              dword ptr [r10 + 32], eax
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 8], rax
                                                                                        jmp   .Lx615_51
.Lx615_48:
                        mov              ecx, dword ptr [r8 + 0]
                        mov              edx, dword ptr [r9 + 0]
                        cmp              ecx, 14
                                                                                        jne   .Lx615_49
                        cmp              edx, 14
                                                                                        je    .Lx615_53
                                                                                        jmp   .Lx615_52
.Lx615_49:
                        cmp              edx, 14
                                                                                        je    .Lx615_52
                        cmp              ecx, 7
                                                                                        je    .Lx615_53
                        cmp              edx, 7
                                                                                        je    .Lx615_53
                        cmp              ecx, 6
                                                                                        jne   .Lx615_50
                        cmp              edx, 6
                                                                                        jne   .Lx615_50
                        mov              rax, qword ptr [r8 + 0]
                        cmp              rax, 6
                                                                                        jne   .Lx615_53
                        mov              rax, qword ptr [r9 + 0]
                        cmp              rax, 6
                                                                                        jne   .Lx615_53
                        mov              rax, qword ptr [r8 + 8]
                        mov              rsi, qword ptr [r9 + 8]
                        cmp              rax, rsi
                                                                                        je    .Lx615_51
                                                                                        jmp   .Lx615_52
.Lx615_50:
                        mov              rax, qword ptr [r8 + 0]
                        mov              rsi, qword ptr [r9 + 0]
                        cmp              rax, rsi
                                                                                        jne   .Lx615_53
                        mov              rax, qword ptr [r8 + 8]
                        mov              rsi, qword ptr [r9 + 8]
                        cmp              rax, rsi
                                                                                        jne   .Lx615_53
.Lx615_51:
                        mov              rax, qword ptr [r8 + 0]
                        mov              rdx, qword ptr [r8 + 8]
                                                                                        jmp   .Lx615_54
.Lx615_52:
                        mov              eax, 99
                        mov              edx, 0
                                                                                        jmp   .Lx615_54
.Lx615_53:
                        mov              esi, 2
                        call             rt_pl_dop_unify@PLT
.Lx615_54:
                        mov              qword ptr [rbp + 3152], rax
                        mov              qword ptr [rbp + 3160], rdx
                        cmp              eax, 99
                                                                                        je    n176_op11_α
                                                                                        jmp   n193_var_α
n187_op11_β:
                                                                                        jmp   n176_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n188_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8784]
                        mov              qword ptr [rbp + 2656], rax
                        mov              qword ptr [rbp + 2664], rdx
                                                                                        jmp   n194_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n189_move_label_α:
                        lea              rax, [rip + n160_call_proc_staged_β]
                        mov              qword ptr [rbp + 64], rax
                                                                                        jmp   proc_roman$2F2_γ
#-----------------------------------------------------------------------------------------------------------------------
n190_lit_string_α:
                        mov              qword ptr [rbp + 4688], 1
                        mov              rax, qword ptr [rip + .Lx620_0]
                        mov              qword ptr [rbp + 4696], rax
                                                                                        jmp   n195_var_ref_α
.Lx620_0:
                        .quad            .Lx620_0_s
.Lx620_0_s:
                        .string          "XC"
#-----------------------------------------------------------------------------------------------------------------------
n191_op11_α:
                        mov              rax, qword ptr [rbp + 4240]
                        mov              qword ptr [rbp + 4208], rax
                        mov              rax, qword ptr [rbp + 4248]
                        mov              qword ptr [rbp + 4216], rax
                        mov              rax, qword ptr [rbp + 4256]
                        mov              qword ptr [rbp + 4224], rax
                        mov              rax, qword ptr [rbp + 4264]
                        mov              qword ptr [rbp + 4232], rax
                        lea              rdi, [rbp + 4208]
                        mov              esi, 2
                        call             rt_pl_dop_is_v@PLT
                        mov              qword ptr [rbp + 4192], rax
                        mov              qword ptr [rbp + 4200], rdx
                        cmp              eax, 99
                                                                                        je    n51_op11_α
                                                                                        jmp   n196_var_ref_α
n191_op11_β:
                                                                                        jmp   n51_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n192_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8768]
                        mov              qword ptr [rbp + 3600], rax
                        mov              qword ptr [rbp + 3608], rdx
                                                                                        jmp   n197_var_α
#-----------------------------------------------------------------------------------------------------------------------
n193_var_α:
                        mov              rax, qword ptr [rbp + 8784]
                        mov              qword ptr [rbp + 3120], rax
                        mov              rax, qword ptr [rbp + 8792]
                        mov              qword ptr [rbp + 3128], rax
                                                                                        jmp   n198_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n194_op11_α:
                        mov              rax, qword ptr [rbp + 2640]
                        mov              qword ptr [rbp + 2608], rax
                        mov              rax, qword ptr [rbp + 2648]
                        mov              qword ptr [rbp + 2616], rax
                        mov              rax, qword ptr [rbp + 2656]
                        mov              qword ptr [rbp + 2624], rax
                        mov              rax, qword ptr [rbp + 2664]
                        mov              qword ptr [rbp + 2632], rax
                        lea              rdi, [rbp + 2608]
                        lea              r8, [rbp + 2608]
.Lx626_40:
                        mov              eax, dword ptr [r8 + 0]
                        cmp              eax, 9
                                                                                        jne   .Lx626_56
                        mov              esi, dword ptr [r8 + 4]
                        mov              rax, qword ptr [r8 + 8]
                        test             rax, rax
                                                                                        je    .Lx626_41
                        cmp              esi, 1
                                                                                        jne   .Lx626_55
                        mov              r8, rax
                                                                                        jmp   .Lx626_40
.Lx626_55:
                        cmp              esi, 2
                                                                                        jne   .Lx626_41
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx626_41
                        mov              r8, rax
                                                                                        jmp   .Lx626_40
.Lx626_56:
                        cmp              eax, 13
                                                                                        jne   .Lx626_41
                        mov              rax, qword ptr [r8 + 8]
                        test             rax, rax
                                                                                        je    .Lx626_41
                        cmp              rax, r8
                                                                                        je    .Lx626_41
                        mov              r8, rax
                                                                                        jmp   .Lx626_40
.Lx626_41:
                        lea              r9, [rbp + 2624]
.Lx626_42:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 9
                                                                                        jne   .Lx626_58
                        mov              esi, dword ptr [r9 + 4]
                        mov              rax, qword ptr [r9 + 8]
                        test             rax, rax
                                                                                        je    .Lx626_43
                        cmp              esi, 1
                                                                                        jne   .Lx626_57
                        mov              r9, rax
                                                                                        jmp   .Lx626_42
.Lx626_57:
                        cmp              esi, 2
                                                                                        jne   .Lx626_43
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx626_43
                        mov              r9, rax
                                                                                        jmp   .Lx626_42
.Lx626_58:
                        cmp              eax, 13
                                                                                        jne   .Lx626_43
                        mov              rax, qword ptr [r9 + 8]
                        test             rax, rax
                                                                                        je    .Lx626_43
                        cmp              rax, r9
                                                                                        je    .Lx626_43
                        mov              r9, rax
                                                                                        jmp   .Lx626_42
.Lx626_43:
                        cmp              r8, r9
                                                                                        je    .Lx626_51
                        mov              eax, dword ptr [r8 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx626_44
                        cmp              eax, 99
                                                                                        je    .Lx626_44
                        cmp              eax, 13
                                                                                        jne   .Lx626_45
                        mov              rax, qword ptr [r8 + 8]
                        cmp              rax, r8
                                                                                        je    .Lx626_44
                                                                                        jmp   .Lx626_45
.Lx626_44:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx626_53
                        cmp              eax, 99
                                                                                        je    .Lx626_53
                        cmp              eax, 13
                                                                                        jne   .Lx626_46
                        mov              rax, qword ptr [r9 + 8]
                        cmp              rax, r9
                                                                                        je    .Lx626_53
                                                                                        jmp   .Lx626_46
.Lx626_46:
                        lea              r10, [rip + g_pl_trail]
                        mov              r11, qword ptr [r10 + 0]
                        test             r11, r11
                                                                                        je    .Lx626_53
                        mov              eax, dword ptr [r10 + 32]
                        mov              esi, 24
                        imul             rsi, rax
                        mov              rax, qword ptr [r10 + 24]
                        sub              rax, 24
                        cmp              rsi, rax
                                                                                        ja    .Lx626_53
                        add              r11, rsi
                        mov              qword ptr [r11 + 0], r8
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r11 + 8], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r11 + 16], rax
                        mov              eax, dword ptr [r10 + 32]
                        add              eax, 1
                        mov              dword ptr [r10 + 32], eax
                        mov              rax, qword ptr [r9 + 0]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [r8 + 8], rax
                                                                                        jmp   .Lx626_51
.Lx626_45:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx626_47
                        cmp              eax, 99
                                                                                        je    .Lx626_47
                        cmp              eax, 13
                                                                                        jne   .Lx626_48
                        mov              rax, qword ptr [r9 + 8]
                        cmp              rax, r9
                                                                                        je    .Lx626_47
                                                                                        jmp   .Lx626_48
.Lx626_47:
                        lea              r10, [rip + g_pl_trail]
                        mov              r11, qword ptr [r10 + 0]
                        test             r11, r11
                                                                                        je    .Lx626_53
                        mov              eax, dword ptr [r10 + 32]
                        mov              esi, 24
                        imul             rsi, rax
                        mov              rax, qword ptr [r10 + 24]
                        sub              rax, 24
                        cmp              rsi, rax
                                                                                        ja    .Lx626_53
                        add              r11, rsi
                        mov              qword ptr [r11 + 0], r9
                        mov              rax, qword ptr [r9 + 0]
                        mov              qword ptr [r11 + 8], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [r11 + 16], rax
                        mov              eax, dword ptr [r10 + 32]
                        add              eax, 1
                        mov              dword ptr [r10 + 32], eax
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 8], rax
                                                                                        jmp   .Lx626_51
.Lx626_48:
                        mov              ecx, dword ptr [r8 + 0]
                        mov              edx, dword ptr [r9 + 0]
                        cmp              ecx, 14
                                                                                        jne   .Lx626_49
                        cmp              edx, 14
                                                                                        je    .Lx626_53
                                                                                        jmp   .Lx626_52
.Lx626_49:
                        cmp              edx, 14
                                                                                        je    .Lx626_52
                        cmp              ecx, 7
                                                                                        je    .Lx626_53
                        cmp              edx, 7
                                                                                        je    .Lx626_53
                        cmp              ecx, 6
                                                                                        jne   .Lx626_50
                        cmp              edx, 6
                                                                                        jne   .Lx626_50
                        mov              rax, qword ptr [r8 + 0]
                        cmp              rax, 6
                                                                                        jne   .Lx626_53
                        mov              rax, qword ptr [r9 + 0]
                        cmp              rax, 6
                                                                                        jne   .Lx626_53
                        mov              rax, qword ptr [r8 + 8]
                        mov              rsi, qword ptr [r9 + 8]
                        cmp              rax, rsi
                                                                                        je    .Lx626_51
                                                                                        jmp   .Lx626_52
.Lx626_50:
                        mov              rax, qword ptr [r8 + 0]
                        mov              rsi, qword ptr [r9 + 0]
                        cmp              rax, rsi
                                                                                        jne   .Lx626_53
                        mov              rax, qword ptr [r8 + 8]
                        mov              rsi, qword ptr [r9 + 8]
                        cmp              rax, rsi
                                                                                        jne   .Lx626_53
.Lx626_51:
                        mov              rax, qword ptr [r8 + 0]
                        mov              rdx, qword ptr [r8 + 8]
                                                                                        jmp   .Lx626_54
.Lx626_52:
                        mov              eax, 99
                        mov              edx, 0
                                                                                        jmp   .Lx626_54
.Lx626_53:
                        mov              esi, 2
                        call             rt_pl_dop_unify@PLT
.Lx626_54:
                        mov              qword ptr [rbp + 2592], rax
                        mov              qword ptr [rbp + 2600], rdx
                        cmp              eax, 99
                                                                                        je    n200_op11_α
                                                                                        jmp   n199_var_ref_α
n194_op11_β:
                                                                                        jmp   n200_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n195_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8736]
                        mov              qword ptr [rbp + 4704], rax
                        mov              qword ptr [rbp + 4712], rdx
                                                                                        jmp   n201_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n196_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8768]
                        mov              qword ptr [rbp + 4160], rax
                        mov              qword ptr [rbp + 4168], rdx
                                                                                        jmp   n202_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n197_var_α:
                        mov              rax, qword ptr [rbp + 8784]
                        mov              qword ptr [rbp + 3680], rax
                        mov              rax, qword ptr [rbp + 8792]
                        mov              qword ptr [rbp + 3688], rax
                                                                                        jmp   n203_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n198_lit_integer_α:
                        mov              qword ptr [rbp + 3136], 6
                        mov              rax, qword ptr [rip + .Lx633_0]
                        mov              qword ptr [rbp + 3144], rax
                                                                                        jmp   n204_op11_α
.Lx633_0:
                        .quad            10
#-----------------------------------------------------------------------------------------------------------------------
n199_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 32]
                        mov              qword ptr [rbp + 2560], rax
                        mov              qword ptr [rbp + 2568], rdx
                                                                                        jmp   n205_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n200_op11_α:
                        mov              rax, qword ptr [rbp + 80]
                        mov              qword ptr [rbp + 2048], rax
                        mov              rax, qword ptr [rbp + 88]
                        mov              qword ptr [rbp + 2056], rax
                        lea              rdi, [rbp + 2048]
                        mov              esi, 1
                        call             rt_pl_dop_unwind_nothrow@PLT
                        mov              qword ptr [rbp + 2032], rax
                        mov              qword ptr [rbp + 2040], rdx
                        cmp              eax, 99
                                                                                        je    proc_roman$2F2_ω
                                                                                        jmp   n206_var_ref_α
n200_op11_β:
                                                                                        jmp   proc_roman$2F2_ω
#-----------------------------------------------------------------------------------------------------------------------
n201_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8752]
                        mov              qword ptr [rbp + 4720], rax
                        mov              qword ptr [rbp + 4728], rdx
                                                                                        jmp   n207_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n202_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8736]
                        mov              qword ptr [rbp + 4176], rax
                        mov              qword ptr [rbp + 4184], rdx
                                                                                        jmp   n208_call_proc_staged_α
#-----------------------------------------------------------------------------------------------------------------------
n203_lit_integer_α:
                        mov              qword ptr [rbp + 3696], 6
                        mov              rax, qword ptr [rip + .Lx641_0]
                        mov              qword ptr [rbp + 3704], rax
                                                                                        jmp   n209_op11_α
.Lx641_0:
                        .quad            40
#-----------------------------------------------------------------------------------------------------------------------
n204_op11_α:
                        mov              rax, qword ptr [rbp + 3120]
                        mov              qword ptr [rbp + 3088], rax
                        mov              rax, qword ptr [rbp + 3128]
                        mov              qword ptr [rbp + 3096], rax
                        mov              rax, qword ptr [rbp + 3136]
                        mov              qword ptr [rbp + 3104], rax
                        mov              rax, qword ptr [rbp + 3144]
                        mov              qword ptr [rbp + 3112], rax
                        lea              rdi, [rbp + 3088]
                        mov              esi, 2
                        call             rt_pl_dop_cmp_ge@PLT
                        mov              qword ptr [rbp + 3072], rax
                        mov              qword ptr [rbp + 3080], rdx
                        cmp              eax, 99
                                                                                        je    n176_op11_α
                                                                                        jmp   n210_op19_α
n204_op11_β:
                                                                                        jmp   n176_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n205_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8752]
                        mov              qword ptr [rbp + 2576], rax
                        mov              qword ptr [rbp + 2584], rdx
                                                                                        jmp   n211_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n206_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 16]
                        mov              qword ptr [rbp + 2000], rax
                        mov              qword ptr [rbp + 2008], rdx
                                                                                        jmp   n212_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n207_op11_α:
                        mov              rax, qword ptr [rbp + 4688]
                        mov              qword ptr [rbp + 4640], rax
                        mov              rax, qword ptr [rbp + 4696]
                        mov              qword ptr [rbp + 4648], rax
                        mov              rax, qword ptr [rbp + 4704]
                        mov              qword ptr [rbp + 4656], rax
                        mov              rax, qword ptr [rbp + 4712]
                        mov              qword ptr [rbp + 4664], rax
                        mov              rax, qword ptr [rbp + 4720]
                        mov              qword ptr [rbp + 4672], rax
                        mov              rax, qword ptr [rbp + 4728]
                        mov              qword ptr [rbp + 4680], rax
                        .section         .rodata
.Lrkfn648:              .string          "$atom_concat"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn648]
                        lea              rsi, [rbp + 4640]
                        mov              edx, 3
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 4624], rax
                        mov              qword ptr [rbp + 4632], rdx
                        cmp              eax, 99
                                                                                        je    n184_call_proc_staged_β
                                                                                        jmp   n213_move_label_α
n207_op11_β:
                                                                                        jmp   n184_call_proc_staged_β
#-----------------------------------------------------------------------------------------------------------------------
n208_call_proc_staged_α:
                        lea              rsi, [rbp + 4160]
                        lea              rdx, [rbp + 4176]
                        call             proc_roman$2F2_dcα
                                                                                        jmp   .Lx650_2
.Lx650_2:
                        mov              qword ptr [rbp + 4096], rax
                        mov              qword ptr [rbp + 4104], rdx
                        cmp              eax, 99
                                                                                        je    n51_op11_α
                                                                                        jmp   n214_lit_string_α
n208_call_proc_staged_β:
                                                                                        jmp   n51_op11_α
.Lx650_0:
                        .quad            .Lx650_0_s
.Lx650_0_s:
                        .string          "roman/2"
#-----------------------------------------------------------------------------------------------------------------------
n209_op11_α:
                        mov              rax, qword ptr [rbp + 3680]
                        mov              qword ptr [rbp + 3632], rax
                        mov              rax, qword ptr [rbp + 3688]
                        mov              qword ptr [rbp + 3640], rax
                        mov              rax, qword ptr [rbp + 3696]
                        mov              qword ptr [rbp + 3648], rax
                        mov              rax, qword ptr [rbp + 3704]
                        mov              qword ptr [rbp + 3656], rax
                        lea              rdi, [rbp + 3632]
                        mov              esi, 2
                        call             rt_pl_dop_ax_sub@PLT
                        mov              qword ptr [rbp + 3616], rax
                        mov              qword ptr [rbp + 3624], rdx
                        cmp              eax, 99
                                                                                        je    n51_op11_α
                                                                                        jmp   n215_op11_α
n209_op11_β:
                                                                                        jmp   n51_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n210_op19_α:
                                                                                        jmp   n216_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n211_op11_α:
                        mov              rax, qword ptr [rbp + 2560]
                        mov              qword ptr [rbp + 2528], rax
                        mov              rax, qword ptr [rbp + 2568]
                        mov              qword ptr [rbp + 2536], rax
                        mov              rax, qword ptr [rbp + 2576]
                        mov              qword ptr [rbp + 2544], rax
                        mov              rax, qword ptr [rbp + 2584]
                        mov              qword ptr [rbp + 2552], rax
                        lea              rdi, [rbp + 2528]
                        lea              r8, [rbp + 2528]
.Lx653_40:
                        mov              eax, dword ptr [r8 + 0]
                        cmp              eax, 9
                                                                                        jne   .Lx653_56
                        mov              esi, dword ptr [r8 + 4]
                        mov              rax, qword ptr [r8 + 8]
                        test             rax, rax
                                                                                        je    .Lx653_41
                        cmp              esi, 1
                                                                                        jne   .Lx653_55
                        mov              r8, rax
                                                                                        jmp   .Lx653_40
.Lx653_55:
                        cmp              esi, 2
                                                                                        jne   .Lx653_41
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx653_41
                        mov              r8, rax
                                                                                        jmp   .Lx653_40
.Lx653_56:
                        cmp              eax, 13
                                                                                        jne   .Lx653_41
                        mov              rax, qword ptr [r8 + 8]
                        test             rax, rax
                                                                                        je    .Lx653_41
                        cmp              rax, r8
                                                                                        je    .Lx653_41
                        mov              r8, rax
                                                                                        jmp   .Lx653_40
.Lx653_41:
                        lea              r9, [rbp + 2544]
.Lx653_42:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 9
                                                                                        jne   .Lx653_58
                        mov              esi, dword ptr [r9 + 4]
                        mov              rax, qword ptr [r9 + 8]
                        test             rax, rax
                                                                                        je    .Lx653_43
                        cmp              esi, 1
                                                                                        jne   .Lx653_57
                        mov              r9, rax
                                                                                        jmp   .Lx653_42
.Lx653_57:
                        cmp              esi, 2
                                                                                        jne   .Lx653_43
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx653_43
                        mov              r9, rax
                                                                                        jmp   .Lx653_42
.Lx653_58:
                        cmp              eax, 13
                                                                                        jne   .Lx653_43
                        mov              rax, qword ptr [r9 + 8]
                        test             rax, rax
                                                                                        je    .Lx653_43
                        cmp              rax, r9
                                                                                        je    .Lx653_43
                        mov              r9, rax
                                                                                        jmp   .Lx653_42
.Lx653_43:
                        cmp              r8, r9
                                                                                        je    .Lx653_51
                        mov              eax, dword ptr [r8 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx653_44
                        cmp              eax, 99
                                                                                        je    .Lx653_44
                        cmp              eax, 13
                                                                                        jne   .Lx653_45
                        mov              rax, qword ptr [r8 + 8]
                        cmp              rax, r8
                                                                                        je    .Lx653_44
                                                                                        jmp   .Lx653_45
.Lx653_44:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx653_53
                        cmp              eax, 99
                                                                                        je    .Lx653_53
                        cmp              eax, 13
                                                                                        jne   .Lx653_46
                        mov              rax, qword ptr [r9 + 8]
                        cmp              rax, r9
                                                                                        je    .Lx653_53
                                                                                        jmp   .Lx653_46
.Lx653_46:
                        lea              r10, [rip + g_pl_trail]
                        mov              r11, qword ptr [r10 + 0]
                        test             r11, r11
                                                                                        je    .Lx653_53
                        mov              eax, dword ptr [r10 + 32]
                        mov              esi, 24
                        imul             rsi, rax
                        mov              rax, qword ptr [r10 + 24]
                        sub              rax, 24
                        cmp              rsi, rax
                                                                                        ja    .Lx653_53
                        add              r11, rsi
                        mov              qword ptr [r11 + 0], r8
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r11 + 8], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r11 + 16], rax
                        mov              eax, dword ptr [r10 + 32]
                        add              eax, 1
                        mov              dword ptr [r10 + 32], eax
                        mov              rax, qword ptr [r9 + 0]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [r8 + 8], rax
                                                                                        jmp   .Lx653_51
.Lx653_45:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx653_47
                        cmp              eax, 99
                                                                                        je    .Lx653_47
                        cmp              eax, 13
                                                                                        jne   .Lx653_48
                        mov              rax, qword ptr [r9 + 8]
                        cmp              rax, r9
                                                                                        je    .Lx653_47
                                                                                        jmp   .Lx653_48
.Lx653_47:
                        lea              r10, [rip + g_pl_trail]
                        mov              r11, qword ptr [r10 + 0]
                        test             r11, r11
                                                                                        je    .Lx653_53
                        mov              eax, dword ptr [r10 + 32]
                        mov              esi, 24
                        imul             rsi, rax
                        mov              rax, qword ptr [r10 + 24]
                        sub              rax, 24
                        cmp              rsi, rax
                                                                                        ja    .Lx653_53
                        add              r11, rsi
                        mov              qword ptr [r11 + 0], r9
                        mov              rax, qword ptr [r9 + 0]
                        mov              qword ptr [r11 + 8], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [r11 + 16], rax
                        mov              eax, dword ptr [r10 + 32]
                        add              eax, 1
                        mov              dword ptr [r10 + 32], eax
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 8], rax
                                                                                        jmp   .Lx653_51
.Lx653_48:
                        mov              ecx, dword ptr [r8 + 0]
                        mov              edx, dword ptr [r9 + 0]
                        cmp              ecx, 14
                                                                                        jne   .Lx653_49
                        cmp              edx, 14
                                                                                        je    .Lx653_53
                                                                                        jmp   .Lx653_52
.Lx653_49:
                        cmp              edx, 14
                                                                                        je    .Lx653_52
                        cmp              ecx, 7
                                                                                        je    .Lx653_53
                        cmp              edx, 7
                                                                                        je    .Lx653_53
                        cmp              ecx, 6
                                                                                        jne   .Lx653_50
                        cmp              edx, 6
                                                                                        jne   .Lx653_50
                        mov              rax, qword ptr [r8 + 0]
                        cmp              rax, 6
                                                                                        jne   .Lx653_53
                        mov              rax, qword ptr [r9 + 0]
                        cmp              rax, 6
                                                                                        jne   .Lx653_53
                        mov              rax, qword ptr [r8 + 8]
                        mov              rsi, qword ptr [r9 + 8]
                        cmp              rax, rsi
                                                                                        je    .Lx653_51
                                                                                        jmp   .Lx653_52
.Lx653_50:
                        mov              rax, qword ptr [r8 + 0]
                        mov              rsi, qword ptr [r9 + 0]
                        cmp              rax, rsi
                                                                                        jne   .Lx653_53
                        mov              rax, qword ptr [r8 + 8]
                        mov              rsi, qword ptr [r9 + 8]
                        cmp              rax, rsi
                                                                                        jne   .Lx653_53
.Lx653_51:
                        mov              rax, qword ptr [r8 + 0]
                        mov              rdx, qword ptr [r8 + 8]
                                                                                        jmp   .Lx653_54
.Lx653_52:
                        mov              eax, 99
                        mov              edx, 0
                                                                                        jmp   .Lx653_54
.Lx653_53:
                        mov              esi, 2
                        call             rt_pl_dop_unify@PLT
.Lx653_54:
                        mov              qword ptr [rbp + 2512], rax
                        mov              qword ptr [rbp + 2520], rdx
                        cmp              eax, 99
                                                                                        je    n200_op11_α
                                                                                        jmp   n217_var_α
n211_op11_β:
                                                                                        jmp   n200_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n212_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8784]
                        mov              qword ptr [rbp + 2016], rax
                        mov              qword ptr [rbp + 2024], rdx
                                                                                        jmp   n218_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n213_move_label_α:
                        lea              rax, [rip + n184_call_proc_staged_β]
                        mov              qword ptr [rbp + 64], rax
                                                                                        jmp   proc_roman$2F2_γ
#-----------------------------------------------------------------------------------------------------------------------
n214_lit_string_α:
                        mov              qword ptr [rbp + 4048], 1
                        mov              rax, qword ptr [rip + .Lx658_0]
                        mov              qword ptr [rbp + 4056], rax
                                                                                        jmp   n219_var_ref_α
.Lx658_0:
                        .quad            .Lx658_0_s
.Lx658_0_s:
                        .string          "L"
#-----------------------------------------------------------------------------------------------------------------------
n215_op11_α:
                        mov              rax, qword ptr [rbp + 3600]
                        mov              qword ptr [rbp + 3568], rax
                        mov              rax, qword ptr [rbp + 3608]
                        mov              qword ptr [rbp + 3576], rax
                        mov              rax, qword ptr [rbp + 3616]
                        mov              qword ptr [rbp + 3584], rax
                        mov              rax, qword ptr [rbp + 3624]
                        mov              qword ptr [rbp + 3592], rax
                        lea              rdi, [rbp + 3568]
                        mov              esi, 2
                        call             rt_pl_dop_is_v@PLT
                        mov              qword ptr [rbp + 3552], rax
                        mov              qword ptr [rbp + 3560], rdx
                        cmp              eax, 99
                                                                                        je    n51_op11_α
                                                                                        jmp   n220_var_ref_α
n215_op11_β:
                                                                                        jmp   n51_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n216_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8768]
                        mov              qword ptr [rbp + 2960], rax
                        mov              qword ptr [rbp + 2968], rdx
                                                                                        jmp   n221_var_α
#-----------------------------------------------------------------------------------------------------------------------
n217_var_α:
                        mov              rax, qword ptr [rbp + 8784]
                        mov              qword ptr [rbp + 2480], rax
                        mov              rax, qword ptr [rbp + 8792]
                        mov              qword ptr [rbp + 2488], rax
                                                                                        jmp   n222_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n218_op11_α:
                        mov              rax, qword ptr [rbp + 2000]
                        mov              qword ptr [rbp + 1968], rax
                        mov              rax, qword ptr [rbp + 2008]
                        mov              qword ptr [rbp + 1976], rax
                        mov              rax, qword ptr [rbp + 2016]
                        mov              qword ptr [rbp + 1984], rax
                        mov              rax, qword ptr [rbp + 2024]
                        mov              qword ptr [rbp + 1992], rax
                        lea              rdi, [rbp + 1968]
                        lea              r8, [rbp + 1968]
.Lx664_40:
                        mov              eax, dword ptr [r8 + 0]
                        cmp              eax, 9
                                                                                        jne   .Lx664_56
                        mov              esi, dword ptr [r8 + 4]
                        mov              rax, qword ptr [r8 + 8]
                        test             rax, rax
                                                                                        je    .Lx664_41
                        cmp              esi, 1
                                                                                        jne   .Lx664_55
                        mov              r8, rax
                                                                                        jmp   .Lx664_40
.Lx664_55:
                        cmp              esi, 2
                                                                                        jne   .Lx664_41
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx664_41
                        mov              r8, rax
                                                                                        jmp   .Lx664_40
.Lx664_56:
                        cmp              eax, 13
                                                                                        jne   .Lx664_41
                        mov              rax, qword ptr [r8 + 8]
                        test             rax, rax
                                                                                        je    .Lx664_41
                        cmp              rax, r8
                                                                                        je    .Lx664_41
                        mov              r8, rax
                                                                                        jmp   .Lx664_40
.Lx664_41:
                        lea              r9, [rbp + 1984]
.Lx664_42:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 9
                                                                                        jne   .Lx664_58
                        mov              esi, dword ptr [r9 + 4]
                        mov              rax, qword ptr [r9 + 8]
                        test             rax, rax
                                                                                        je    .Lx664_43
                        cmp              esi, 1
                                                                                        jne   .Lx664_57
                        mov              r9, rax
                                                                                        jmp   .Lx664_42
.Lx664_57:
                        cmp              esi, 2
                                                                                        jne   .Lx664_43
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx664_43
                        mov              r9, rax
                                                                                        jmp   .Lx664_42
.Lx664_58:
                        cmp              eax, 13
                                                                                        jne   .Lx664_43
                        mov              rax, qword ptr [r9 + 8]
                        test             rax, rax
                                                                                        je    .Lx664_43
                        cmp              rax, r9
                                                                                        je    .Lx664_43
                        mov              r9, rax
                                                                                        jmp   .Lx664_42
.Lx664_43:
                        cmp              r8, r9
                                                                                        je    .Lx664_51
                        mov              eax, dword ptr [r8 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx664_44
                        cmp              eax, 99
                                                                                        je    .Lx664_44
                        cmp              eax, 13
                                                                                        jne   .Lx664_45
                        mov              rax, qword ptr [r8 + 8]
                        cmp              rax, r8
                                                                                        je    .Lx664_44
                                                                                        jmp   .Lx664_45
.Lx664_44:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx664_53
                        cmp              eax, 99
                                                                                        je    .Lx664_53
                        cmp              eax, 13
                                                                                        jne   .Lx664_46
                        mov              rax, qword ptr [r9 + 8]
                        cmp              rax, r9
                                                                                        je    .Lx664_53
                                                                                        jmp   .Lx664_46
.Lx664_46:
                        lea              r10, [rip + g_pl_trail]
                        mov              r11, qword ptr [r10 + 0]
                        test             r11, r11
                                                                                        je    .Lx664_53
                        mov              eax, dword ptr [r10 + 32]
                        mov              esi, 24
                        imul             rsi, rax
                        mov              rax, qword ptr [r10 + 24]
                        sub              rax, 24
                        cmp              rsi, rax
                                                                                        ja    .Lx664_53
                        add              r11, rsi
                        mov              qword ptr [r11 + 0], r8
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r11 + 8], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r11 + 16], rax
                        mov              eax, dword ptr [r10 + 32]
                        add              eax, 1
                        mov              dword ptr [r10 + 32], eax
                        mov              rax, qword ptr [r9 + 0]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [r8 + 8], rax
                                                                                        jmp   .Lx664_51
.Lx664_45:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx664_47
                        cmp              eax, 99
                                                                                        je    .Lx664_47
                        cmp              eax, 13
                                                                                        jne   .Lx664_48
                        mov              rax, qword ptr [r9 + 8]
                        cmp              rax, r9
                                                                                        je    .Lx664_47
                                                                                        jmp   .Lx664_48
.Lx664_47:
                        lea              r10, [rip + g_pl_trail]
                        mov              r11, qword ptr [r10 + 0]
                        test             r11, r11
                                                                                        je    .Lx664_53
                        mov              eax, dword ptr [r10 + 32]
                        mov              esi, 24
                        imul             rsi, rax
                        mov              rax, qword ptr [r10 + 24]
                        sub              rax, 24
                        cmp              rsi, rax
                                                                                        ja    .Lx664_53
                        add              r11, rsi
                        mov              qword ptr [r11 + 0], r9
                        mov              rax, qword ptr [r9 + 0]
                        mov              qword ptr [r11 + 8], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [r11 + 16], rax
                        mov              eax, dword ptr [r10 + 32]
                        add              eax, 1
                        mov              dword ptr [r10 + 32], eax
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 8], rax
                                                                                        jmp   .Lx664_51
.Lx664_48:
                        mov              ecx, dword ptr [r8 + 0]
                        mov              edx, dword ptr [r9 + 0]
                        cmp              ecx, 14
                                                                                        jne   .Lx664_49
                        cmp              edx, 14
                                                                                        je    .Lx664_53
                                                                                        jmp   .Lx664_52
.Lx664_49:
                        cmp              edx, 14
                                                                                        je    .Lx664_52
                        cmp              ecx, 7
                                                                                        je    .Lx664_53
                        cmp              edx, 7
                                                                                        je    .Lx664_53
                        cmp              ecx, 6
                                                                                        jne   .Lx664_50
                        cmp              edx, 6
                                                                                        jne   .Lx664_50
                        mov              rax, qword ptr [r8 + 0]
                        cmp              rax, 6
                                                                                        jne   .Lx664_53
                        mov              rax, qword ptr [r9 + 0]
                        cmp              rax, 6
                                                                                        jne   .Lx664_53
                        mov              rax, qword ptr [r8 + 8]
                        mov              rsi, qword ptr [r9 + 8]
                        cmp              rax, rsi
                                                                                        je    .Lx664_51
                                                                                        jmp   .Lx664_52
.Lx664_50:
                        mov              rax, qword ptr [r8 + 0]
                        mov              rsi, qword ptr [r9 + 0]
                        cmp              rax, rsi
                                                                                        jne   .Lx664_53
                        mov              rax, qword ptr [r8 + 8]
                        mov              rsi, qword ptr [r9 + 8]
                        cmp              rax, rsi
                                                                                        jne   .Lx664_53
.Lx664_51:
                        mov              rax, qword ptr [r8 + 0]
                        mov              rdx, qword ptr [r8 + 8]
                                                                                        jmp   .Lx664_54
.Lx664_52:
                        mov              eax, 99
                        mov              edx, 0
                                                                                        jmp   .Lx664_54
.Lx664_53:
                        mov              esi, 2
                        call             rt_pl_dop_unify@PLT
.Lx664_54:
                        mov              qword ptr [rbp + 1952], rax
                        mov              qword ptr [rbp + 1960], rdx
                        cmp              eax, 99
                                                                                        je    n224_op11_α
                                                                                        jmp   n223_var_ref_α
n218_op11_β:
                                                                                        jmp   n224_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n219_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8736]
                        mov              qword ptr [rbp + 4064], rax
                        mov              qword ptr [rbp + 4072], rdx
                                                                                        jmp   n225_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n220_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8768]
                        mov              qword ptr [rbp + 3520], rax
                        mov              qword ptr [rbp + 3528], rdx
                                                                                        jmp   n226_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n221_var_α:
                        mov              rax, qword ptr [rbp + 8784]
                        mov              qword ptr [rbp + 3040], rax
                        mov              rax, qword ptr [rbp + 8792]
                        mov              qword ptr [rbp + 3048], rax
                                                                                        jmp   n227_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n222_lit_integer_α:
                        mov              qword ptr [rbp + 2496], 6
                        mov              rax, qword ptr [rip + .Lx671_0]
                        mov              qword ptr [rbp + 2504], rax
                                                                                        jmp   n228_op11_α
.Lx671_0:
                        .quad            9
#-----------------------------------------------------------------------------------------------------------------------
n223_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 32]
                        mov              qword ptr [rbp + 1920], rax
                        mov              qword ptr [rbp + 1928], rdx
                                                                                        jmp   n229_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n224_op11_α:
                        mov              rax, qword ptr [rbp + 80]
                        mov              qword ptr [rbp + 1408], rax
                        mov              rax, qword ptr [rbp + 88]
                        mov              qword ptr [rbp + 1416], rax
                        lea              rdi, [rbp + 1408]
                        mov              esi, 1
                        call             rt_pl_dop_unwind_nothrow@PLT
                        mov              qword ptr [rbp + 1392], rax
                        mov              qword ptr [rbp + 1400], rdx
                        cmp              eax, 99
                                                                                        je    proc_roman$2F2_ω
                                                                                        jmp   n230_var_ref_α
n224_op11_β:
                                                                                        jmp   proc_roman$2F2_ω
#-----------------------------------------------------------------------------------------------------------------------
n225_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8752]
                        mov              qword ptr [rbp + 4080], rax
                        mov              qword ptr [rbp + 4088], rdx
                                                                                        jmp   n231_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n226_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8736]
                        mov              qword ptr [rbp + 3536], rax
                        mov              qword ptr [rbp + 3544], rdx
                                                                                        jmp   n232_call_proc_staged_α
#-----------------------------------------------------------------------------------------------------------------------
n227_lit_integer_α:
                        mov              qword ptr [rbp + 3056], 6
                        mov              rax, qword ptr [rip + .Lx679_0]
                        mov              qword ptr [rbp + 3064], rax
                                                                                        jmp   n233_op11_α
.Lx679_0:
                        .quad            10
#-----------------------------------------------------------------------------------------------------------------------
n228_op11_α:
                        mov              rax, qword ptr [rbp + 2480]
                        mov              qword ptr [rbp + 2448], rax
                        mov              rax, qword ptr [rbp + 2488]
                        mov              qword ptr [rbp + 2456], rax
                        mov              rax, qword ptr [rbp + 2496]
                        mov              qword ptr [rbp + 2464], rax
                        mov              rax, qword ptr [rbp + 2504]
                        mov              qword ptr [rbp + 2472], rax
                        lea              rdi, [rbp + 2448]
                        mov              esi, 2
                        call             rt_pl_dop_cmp_ge@PLT
                        mov              qword ptr [rbp + 2432], rax
                        mov              qword ptr [rbp + 2440], rdx
                        cmp              eax, 99
                                                                                        je    n200_op11_α
                                                                                        jmp   n234_op19_α
n228_op11_β:
                                                                                        jmp   n200_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n229_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8752]
                        mov              qword ptr [rbp + 1936], rax
                        mov              qword ptr [rbp + 1944], rdx
                                                                                        jmp   n235_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n230_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 16]
                        mov              qword ptr [rbp + 1360], rax
                        mov              qword ptr [rbp + 1368], rdx
                                                                                        jmp   n236_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n231_op11_α:
                        mov              rax, qword ptr [rbp + 4048]
                        mov              qword ptr [rbp + 4000], rax
                        mov              rax, qword ptr [rbp + 4056]
                        mov              qword ptr [rbp + 4008], rax
                        mov              rax, qword ptr [rbp + 4064]
                        mov              qword ptr [rbp + 4016], rax
                        mov              rax, qword ptr [rbp + 4072]
                        mov              qword ptr [rbp + 4024], rax
                        mov              rax, qword ptr [rbp + 4080]
                        mov              qword ptr [rbp + 4032], rax
                        mov              rax, qword ptr [rbp + 4088]
                        mov              qword ptr [rbp + 4040], rax
                        .section         .rodata
.Lrkfn686:              .string          "$atom_concat"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn686]
                        lea              rsi, [rbp + 4000]
                        mov              edx, 3
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 3984], rax
                        mov              qword ptr [rbp + 3992], rdx
                        cmp              eax, 99
                                                                                        je    n208_call_proc_staged_β
                                                                                        jmp   n237_move_label_α
n231_op11_β:
                                                                                        jmp   n208_call_proc_staged_β
#-----------------------------------------------------------------------------------------------------------------------
n232_call_proc_staged_α:
                        lea              rsi, [rbp + 3520]
                        lea              rdx, [rbp + 3536]
                        call             proc_roman$2F2_dcα
                                                                                        jmp   .Lx688_2
.Lx688_2:
                        mov              qword ptr [rbp + 3456], rax
                        mov              qword ptr [rbp + 3464], rdx
                        cmp              eax, 99
                                                                                        je    n51_op11_α
                                                                                        jmp   n238_lit_string_α
n232_call_proc_staged_β:
                                                                                        jmp   n51_op11_α
.Lx688_0:
                        .quad            .Lx688_0_s
.Lx688_0_s:
                        .string          "roman/2"
#-----------------------------------------------------------------------------------------------------------------------
n233_op11_α:
                        mov              rax, qword ptr [rbp + 3040]
                        mov              qword ptr [rbp + 2992], rax
                        mov              rax, qword ptr [rbp + 3048]
                        mov              qword ptr [rbp + 3000], rax
                        mov              rax, qword ptr [rbp + 3056]
                        mov              qword ptr [rbp + 3008], rax
                        mov              rax, qword ptr [rbp + 3064]
                        mov              qword ptr [rbp + 3016], rax
                        lea              rdi, [rbp + 2992]
                        mov              esi, 2
                        call             rt_pl_dop_ax_sub@PLT
                        mov              qword ptr [rbp + 2976], rax
                        mov              qword ptr [rbp + 2984], rdx
                        cmp              eax, 99
                                                                                        je    n51_op11_α
                                                                                        jmp   n239_op11_α
n233_op11_β:
                                                                                        jmp   n51_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n234_op19_α:
                                                                                        jmp   n240_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n235_op11_α:
                        mov              rax, qword ptr [rbp + 1920]
                        mov              qword ptr [rbp + 1888], rax
                        mov              rax, qword ptr [rbp + 1928]
                        mov              qword ptr [rbp + 1896], rax
                        mov              rax, qword ptr [rbp + 1936]
                        mov              qword ptr [rbp + 1904], rax
                        mov              rax, qword ptr [rbp + 1944]
                        mov              qword ptr [rbp + 1912], rax
                        lea              rdi, [rbp + 1888]
                        lea              r8, [rbp + 1888]
.Lx691_40:
                        mov              eax, dword ptr [r8 + 0]
                        cmp              eax, 9
                                                                                        jne   .Lx691_56
                        mov              esi, dword ptr [r8 + 4]
                        mov              rax, qword ptr [r8 + 8]
                        test             rax, rax
                                                                                        je    .Lx691_41
                        cmp              esi, 1
                                                                                        jne   .Lx691_55
                        mov              r8, rax
                                                                                        jmp   .Lx691_40
.Lx691_55:
                        cmp              esi, 2
                                                                                        jne   .Lx691_41
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx691_41
                        mov              r8, rax
                                                                                        jmp   .Lx691_40
.Lx691_56:
                        cmp              eax, 13
                                                                                        jne   .Lx691_41
                        mov              rax, qword ptr [r8 + 8]
                        test             rax, rax
                                                                                        je    .Lx691_41
                        cmp              rax, r8
                                                                                        je    .Lx691_41
                        mov              r8, rax
                                                                                        jmp   .Lx691_40
.Lx691_41:
                        lea              r9, [rbp + 1904]
.Lx691_42:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 9
                                                                                        jne   .Lx691_58
                        mov              esi, dword ptr [r9 + 4]
                        mov              rax, qword ptr [r9 + 8]
                        test             rax, rax
                                                                                        je    .Lx691_43
                        cmp              esi, 1
                                                                                        jne   .Lx691_57
                        mov              r9, rax
                                                                                        jmp   .Lx691_42
.Lx691_57:
                        cmp              esi, 2
                                                                                        jne   .Lx691_43
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx691_43
                        mov              r9, rax
                                                                                        jmp   .Lx691_42
.Lx691_58:
                        cmp              eax, 13
                                                                                        jne   .Lx691_43
                        mov              rax, qword ptr [r9 + 8]
                        test             rax, rax
                                                                                        je    .Lx691_43
                        cmp              rax, r9
                                                                                        je    .Lx691_43
                        mov              r9, rax
                                                                                        jmp   .Lx691_42
.Lx691_43:
                        cmp              r8, r9
                                                                                        je    .Lx691_51
                        mov              eax, dword ptr [r8 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx691_44
                        cmp              eax, 99
                                                                                        je    .Lx691_44
                        cmp              eax, 13
                                                                                        jne   .Lx691_45
                        mov              rax, qword ptr [r8 + 8]
                        cmp              rax, r8
                                                                                        je    .Lx691_44
                                                                                        jmp   .Lx691_45
.Lx691_44:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx691_53
                        cmp              eax, 99
                                                                                        je    .Lx691_53
                        cmp              eax, 13
                                                                                        jne   .Lx691_46
                        mov              rax, qword ptr [r9 + 8]
                        cmp              rax, r9
                                                                                        je    .Lx691_53
                                                                                        jmp   .Lx691_46
.Lx691_46:
                        lea              r10, [rip + g_pl_trail]
                        mov              r11, qword ptr [r10 + 0]
                        test             r11, r11
                                                                                        je    .Lx691_53
                        mov              eax, dword ptr [r10 + 32]
                        mov              esi, 24
                        imul             rsi, rax
                        mov              rax, qword ptr [r10 + 24]
                        sub              rax, 24
                        cmp              rsi, rax
                                                                                        ja    .Lx691_53
                        add              r11, rsi
                        mov              qword ptr [r11 + 0], r8
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r11 + 8], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r11 + 16], rax
                        mov              eax, dword ptr [r10 + 32]
                        add              eax, 1
                        mov              dword ptr [r10 + 32], eax
                        mov              rax, qword ptr [r9 + 0]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [r8 + 8], rax
                                                                                        jmp   .Lx691_51
.Lx691_45:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx691_47
                        cmp              eax, 99
                                                                                        je    .Lx691_47
                        cmp              eax, 13
                                                                                        jne   .Lx691_48
                        mov              rax, qword ptr [r9 + 8]
                        cmp              rax, r9
                                                                                        je    .Lx691_47
                                                                                        jmp   .Lx691_48
.Lx691_47:
                        lea              r10, [rip + g_pl_trail]
                        mov              r11, qword ptr [r10 + 0]
                        test             r11, r11
                                                                                        je    .Lx691_53
                        mov              eax, dword ptr [r10 + 32]
                        mov              esi, 24
                        imul             rsi, rax
                        mov              rax, qword ptr [r10 + 24]
                        sub              rax, 24
                        cmp              rsi, rax
                                                                                        ja    .Lx691_53
                        add              r11, rsi
                        mov              qword ptr [r11 + 0], r9
                        mov              rax, qword ptr [r9 + 0]
                        mov              qword ptr [r11 + 8], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [r11 + 16], rax
                        mov              eax, dword ptr [r10 + 32]
                        add              eax, 1
                        mov              dword ptr [r10 + 32], eax
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 8], rax
                                                                                        jmp   .Lx691_51
.Lx691_48:
                        mov              ecx, dword ptr [r8 + 0]
                        mov              edx, dword ptr [r9 + 0]
                        cmp              ecx, 14
                                                                                        jne   .Lx691_49
                        cmp              edx, 14
                                                                                        je    .Lx691_53
                                                                                        jmp   .Lx691_52
.Lx691_49:
                        cmp              edx, 14
                                                                                        je    .Lx691_52
                        cmp              ecx, 7
                                                                                        je    .Lx691_53
                        cmp              edx, 7
                                                                                        je    .Lx691_53
                        cmp              ecx, 6
                                                                                        jne   .Lx691_50
                        cmp              edx, 6
                                                                                        jne   .Lx691_50
                        mov              rax, qword ptr [r8 + 0]
                        cmp              rax, 6
                                                                                        jne   .Lx691_53
                        mov              rax, qword ptr [r9 + 0]
                        cmp              rax, 6
                                                                                        jne   .Lx691_53
                        mov              rax, qword ptr [r8 + 8]
                        mov              rsi, qword ptr [r9 + 8]
                        cmp              rax, rsi
                                                                                        je    .Lx691_51
                                                                                        jmp   .Lx691_52
.Lx691_50:
                        mov              rax, qword ptr [r8 + 0]
                        mov              rsi, qword ptr [r9 + 0]
                        cmp              rax, rsi
                                                                                        jne   .Lx691_53
                        mov              rax, qword ptr [r8 + 8]
                        mov              rsi, qword ptr [r9 + 8]
                        cmp              rax, rsi
                                                                                        jne   .Lx691_53
.Lx691_51:
                        mov              rax, qword ptr [r8 + 0]
                        mov              rdx, qword ptr [r8 + 8]
                                                                                        jmp   .Lx691_54
.Lx691_52:
                        mov              eax, 99
                        mov              edx, 0
                                                                                        jmp   .Lx691_54
.Lx691_53:
                        mov              esi, 2
                        call             rt_pl_dop_unify@PLT
.Lx691_54:
                        mov              qword ptr [rbp + 1872], rax
                        mov              qword ptr [rbp + 1880], rdx
                        cmp              eax, 99
                                                                                        je    n224_op11_α
                                                                                        jmp   n241_var_α
n235_op11_β:
                                                                                        jmp   n224_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n236_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8784]
                        mov              qword ptr [rbp + 1376], rax
                        mov              qword ptr [rbp + 1384], rdx
                                                                                        jmp   n242_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n237_move_label_α:
                        lea              rax, [rip + n208_call_proc_staged_β]
                        mov              qword ptr [rbp + 64], rax
                                                                                        jmp   proc_roman$2F2_γ
#-----------------------------------------------------------------------------------------------------------------------
n238_lit_string_α:
                        mov              qword ptr [rbp + 3408], 1
                        mov              rax, qword ptr [rip + .Lx696_0]
                        mov              qword ptr [rbp + 3416], rax
                                                                                        jmp   n243_var_ref_α
.Lx696_0:
                        .quad            .Lx696_0_s
.Lx696_0_s:
                        .string          "XL"
#-----------------------------------------------------------------------------------------------------------------------
n239_op11_α:
                        mov              rax, qword ptr [rbp + 2960]
                        mov              qword ptr [rbp + 2928], rax
                        mov              rax, qword ptr [rbp + 2968]
                        mov              qword ptr [rbp + 2936], rax
                        mov              rax, qword ptr [rbp + 2976]
                        mov              qword ptr [rbp + 2944], rax
                        mov              rax, qword ptr [rbp + 2984]
                        mov              qword ptr [rbp + 2952], rax
                        lea              rdi, [rbp + 2928]
                        mov              esi, 2
                        call             rt_pl_dop_is_v@PLT
                        mov              qword ptr [rbp + 2912], rax
                        mov              qword ptr [rbp + 2920], rdx
                        cmp              eax, 99
                                                                                        je    n51_op11_α
                                                                                        jmp   n244_var_ref_α
n239_op11_β:
                                                                                        jmp   n51_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n240_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8768]
                        mov              qword ptr [rbp + 2320], rax
                        mov              qword ptr [rbp + 2328], rdx
                                                                                        jmp   n245_var_α
#-----------------------------------------------------------------------------------------------------------------------
n241_var_α:
                        mov              rax, qword ptr [rbp + 8784]
                        mov              qword ptr [rbp + 1840], rax
                        mov              rax, qword ptr [rbp + 8792]
                        mov              qword ptr [rbp + 1848], rax
                                                                                        jmp   n246_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n242_op11_α:
                        mov              rax, qword ptr [rbp + 1360]
                        mov              qword ptr [rbp + 1328], rax
                        mov              rax, qword ptr [rbp + 1368]
                        mov              qword ptr [rbp + 1336], rax
                        mov              rax, qword ptr [rbp + 1376]
                        mov              qword ptr [rbp + 1344], rax
                        mov              rax, qword ptr [rbp + 1384]
                        mov              qword ptr [rbp + 1352], rax
                        lea              rdi, [rbp + 1328]
                        lea              r8, [rbp + 1328]
.Lx702_40:
                        mov              eax, dword ptr [r8 + 0]
                        cmp              eax, 9
                                                                                        jne   .Lx702_56
                        mov              esi, dword ptr [r8 + 4]
                        mov              rax, qword ptr [r8 + 8]
                        test             rax, rax
                                                                                        je    .Lx702_41
                        cmp              esi, 1
                                                                                        jne   .Lx702_55
                        mov              r8, rax
                                                                                        jmp   .Lx702_40
.Lx702_55:
                        cmp              esi, 2
                                                                                        jne   .Lx702_41
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx702_41
                        mov              r8, rax
                                                                                        jmp   .Lx702_40
.Lx702_56:
                        cmp              eax, 13
                                                                                        jne   .Lx702_41
                        mov              rax, qword ptr [r8 + 8]
                        test             rax, rax
                                                                                        je    .Lx702_41
                        cmp              rax, r8
                                                                                        je    .Lx702_41
                        mov              r8, rax
                                                                                        jmp   .Lx702_40
.Lx702_41:
                        lea              r9, [rbp + 1344]
.Lx702_42:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 9
                                                                                        jne   .Lx702_58
                        mov              esi, dword ptr [r9 + 4]
                        mov              rax, qword ptr [r9 + 8]
                        test             rax, rax
                                                                                        je    .Lx702_43
                        cmp              esi, 1
                                                                                        jne   .Lx702_57
                        mov              r9, rax
                                                                                        jmp   .Lx702_42
.Lx702_57:
                        cmp              esi, 2
                                                                                        jne   .Lx702_43
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx702_43
                        mov              r9, rax
                                                                                        jmp   .Lx702_42
.Lx702_58:
                        cmp              eax, 13
                                                                                        jne   .Lx702_43
                        mov              rax, qword ptr [r9 + 8]
                        test             rax, rax
                                                                                        je    .Lx702_43
                        cmp              rax, r9
                                                                                        je    .Lx702_43
                        mov              r9, rax
                                                                                        jmp   .Lx702_42
.Lx702_43:
                        cmp              r8, r9
                                                                                        je    .Lx702_51
                        mov              eax, dword ptr [r8 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx702_44
                        cmp              eax, 99
                                                                                        je    .Lx702_44
                        cmp              eax, 13
                                                                                        jne   .Lx702_45
                        mov              rax, qword ptr [r8 + 8]
                        cmp              rax, r8
                                                                                        je    .Lx702_44
                                                                                        jmp   .Lx702_45
.Lx702_44:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx702_53
                        cmp              eax, 99
                                                                                        je    .Lx702_53
                        cmp              eax, 13
                                                                                        jne   .Lx702_46
                        mov              rax, qword ptr [r9 + 8]
                        cmp              rax, r9
                                                                                        je    .Lx702_53
                                                                                        jmp   .Lx702_46
.Lx702_46:
                        lea              r10, [rip + g_pl_trail]
                        mov              r11, qword ptr [r10 + 0]
                        test             r11, r11
                                                                                        je    .Lx702_53
                        mov              eax, dword ptr [r10 + 32]
                        mov              esi, 24
                        imul             rsi, rax
                        mov              rax, qword ptr [r10 + 24]
                        sub              rax, 24
                        cmp              rsi, rax
                                                                                        ja    .Lx702_53
                        add              r11, rsi
                        mov              qword ptr [r11 + 0], r8
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r11 + 8], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r11 + 16], rax
                        mov              eax, dword ptr [r10 + 32]
                        add              eax, 1
                        mov              dword ptr [r10 + 32], eax
                        mov              rax, qword ptr [r9 + 0]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [r8 + 8], rax
                                                                                        jmp   .Lx702_51
.Lx702_45:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx702_47
                        cmp              eax, 99
                                                                                        je    .Lx702_47
                        cmp              eax, 13
                                                                                        jne   .Lx702_48
                        mov              rax, qword ptr [r9 + 8]
                        cmp              rax, r9
                                                                                        je    .Lx702_47
                                                                                        jmp   .Lx702_48
.Lx702_47:
                        lea              r10, [rip + g_pl_trail]
                        mov              r11, qword ptr [r10 + 0]
                        test             r11, r11
                                                                                        je    .Lx702_53
                        mov              eax, dword ptr [r10 + 32]
                        mov              esi, 24
                        imul             rsi, rax
                        mov              rax, qword ptr [r10 + 24]
                        sub              rax, 24
                        cmp              rsi, rax
                                                                                        ja    .Lx702_53
                        add              r11, rsi
                        mov              qword ptr [r11 + 0], r9
                        mov              rax, qword ptr [r9 + 0]
                        mov              qword ptr [r11 + 8], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [r11 + 16], rax
                        mov              eax, dword ptr [r10 + 32]
                        add              eax, 1
                        mov              dword ptr [r10 + 32], eax
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 8], rax
                                                                                        jmp   .Lx702_51
.Lx702_48:
                        mov              ecx, dword ptr [r8 + 0]
                        mov              edx, dword ptr [r9 + 0]
                        cmp              ecx, 14
                                                                                        jne   .Lx702_49
                        cmp              edx, 14
                                                                                        je    .Lx702_53
                                                                                        jmp   .Lx702_52
.Lx702_49:
                        cmp              edx, 14
                                                                                        je    .Lx702_52
                        cmp              ecx, 7
                                                                                        je    .Lx702_53
                        cmp              edx, 7
                                                                                        je    .Lx702_53
                        cmp              ecx, 6
                                                                                        jne   .Lx702_50
                        cmp              edx, 6
                                                                                        jne   .Lx702_50
                        mov              rax, qword ptr [r8 + 0]
                        cmp              rax, 6
                                                                                        jne   .Lx702_53
                        mov              rax, qword ptr [r9 + 0]
                        cmp              rax, 6
                                                                                        jne   .Lx702_53
                        mov              rax, qword ptr [r8 + 8]
                        mov              rsi, qword ptr [r9 + 8]
                        cmp              rax, rsi
                                                                                        je    .Lx702_51
                                                                                        jmp   .Lx702_52
.Lx702_50:
                        mov              rax, qword ptr [r8 + 0]
                        mov              rsi, qword ptr [r9 + 0]
                        cmp              rax, rsi
                                                                                        jne   .Lx702_53
                        mov              rax, qword ptr [r8 + 8]
                        mov              rsi, qword ptr [r9 + 8]
                        cmp              rax, rsi
                                                                                        jne   .Lx702_53
.Lx702_51:
                        mov              rax, qword ptr [r8 + 0]
                        mov              rdx, qword ptr [r8 + 8]
                                                                                        jmp   .Lx702_54
.Lx702_52:
                        mov              eax, 99
                        mov              edx, 0
                                                                                        jmp   .Lx702_54
.Lx702_53:
                        mov              esi, 2
                        call             rt_pl_dop_unify@PLT
.Lx702_54:
                        mov              qword ptr [rbp + 1312], rax
                        mov              qword ptr [rbp + 1320], rdx
                        cmp              eax, 99
                                                                                        je    n248_op11_α
                                                                                        jmp   n247_var_ref_α
n242_op11_β:
                                                                                        jmp   n248_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n243_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8736]
                        mov              qword ptr [rbp + 3424], rax
                        mov              qword ptr [rbp + 3432], rdx
                                                                                        jmp   n249_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n244_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8768]
                        mov              qword ptr [rbp + 2880], rax
                        mov              qword ptr [rbp + 2888], rdx
                                                                                        jmp   n250_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n245_var_α:
                        mov              rax, qword ptr [rbp + 8784]
                        mov              qword ptr [rbp + 2400], rax
                        mov              rax, qword ptr [rbp + 8792]
                        mov              qword ptr [rbp + 2408], rax
                                                                                        jmp   n251_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n246_lit_integer_α:
                        mov              qword ptr [rbp + 1856], 6
                        mov              rax, qword ptr [rip + .Lx709_0]
                        mov              qword ptr [rbp + 1864], rax
                                                                                        jmp   n252_op11_α
.Lx709_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n247_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 32]
                        mov              qword ptr [rbp + 1280], rax
                        mov              qword ptr [rbp + 1288], rdx
                                                                                        jmp   n253_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n248_op11_α:
                        mov              rax, qword ptr [rbp + 80]
                        mov              qword ptr [rbp + 768], rax
                        mov              rax, qword ptr [rbp + 88]
                        mov              qword ptr [rbp + 776], rax
                        lea              rdi, [rbp + 768]
                        mov              esi, 1
                        call             rt_pl_dop_unwind_nothrow@PLT
                        mov              qword ptr [rbp + 752], rax
                        mov              qword ptr [rbp + 760], rdx
                        cmp              eax, 99
                                                                                        je    proc_roman$2F2_ω
                                                                                        jmp   n254_var_ref_α
n248_op11_β:
                                                                                        jmp   proc_roman$2F2_ω
#-----------------------------------------------------------------------------------------------------------------------
n249_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8752]
                        mov              qword ptr [rbp + 3440], rax
                        mov              qword ptr [rbp + 3448], rdx
                                                                                        jmp   n255_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n250_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8736]
                        mov              qword ptr [rbp + 2896], rax
                        mov              qword ptr [rbp + 2904], rdx
                                                                                        jmp   n256_call_proc_staged_α
#-----------------------------------------------------------------------------------------------------------------------
n251_lit_integer_α:
                        mov              qword ptr [rbp + 2416], 6
                        mov              rax, qword ptr [rip + .Lx717_0]
                        mov              qword ptr [rbp + 2424], rax
                                                                                        jmp   n257_op11_α
.Lx717_0:
                        .quad            9
#-----------------------------------------------------------------------------------------------------------------------
n252_op11_α:
                        mov              rax, qword ptr [rbp + 1840]
                        mov              qword ptr [rbp + 1808], rax
                        mov              rax, qword ptr [rbp + 1848]
                        mov              qword ptr [rbp + 1816], rax
                        mov              rax, qword ptr [rbp + 1856]
                        mov              qword ptr [rbp + 1824], rax
                        mov              rax, qword ptr [rbp + 1864]
                        mov              qword ptr [rbp + 1832], rax
                        lea              rdi, [rbp + 1808]
                        mov              esi, 2
                        call             rt_pl_dop_cmp_ge@PLT
                        mov              qword ptr [rbp + 1792], rax
                        mov              qword ptr [rbp + 1800], rdx
                        cmp              eax, 99
                                                                                        je    n224_op11_α
                                                                                        jmp   n258_op19_α
n252_op11_β:
                                                                                        jmp   n224_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n253_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8752]
                        mov              qword ptr [rbp + 1296], rax
                        mov              qword ptr [rbp + 1304], rdx
                                                                                        jmp   n259_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n254_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 16]
                        mov              qword ptr [rbp + 720], rax
                        mov              qword ptr [rbp + 728], rdx
                                                                                        jmp   n260_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n255_op11_α:
                        mov              rax, qword ptr [rbp + 3408]
                        mov              qword ptr [rbp + 3360], rax
                        mov              rax, qword ptr [rbp + 3416]
                        mov              qword ptr [rbp + 3368], rax
                        mov              rax, qword ptr [rbp + 3424]
                        mov              qword ptr [rbp + 3376], rax
                        mov              rax, qword ptr [rbp + 3432]
                        mov              qword ptr [rbp + 3384], rax
                        mov              rax, qword ptr [rbp + 3440]
                        mov              qword ptr [rbp + 3392], rax
                        mov              rax, qword ptr [rbp + 3448]
                        mov              qword ptr [rbp + 3400], rax
                        .section         .rodata
.Lrkfn724:              .string          "$atom_concat"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn724]
                        lea              rsi, [rbp + 3360]
                        mov              edx, 3
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 3344], rax
                        mov              qword ptr [rbp + 3352], rdx
                        cmp              eax, 99
                                                                                        je    n232_call_proc_staged_β
                                                                                        jmp   n261_move_label_α
n255_op11_β:
                                                                                        jmp   n232_call_proc_staged_β
#-----------------------------------------------------------------------------------------------------------------------
n256_call_proc_staged_α:
                        lea              rsi, [rbp + 2880]
                        lea              rdx, [rbp + 2896]
                        call             proc_roman$2F2_dcα
                                                                                        jmp   .Lx726_2
.Lx726_2:
                        mov              qword ptr [rbp + 2816], rax
                        mov              qword ptr [rbp + 2824], rdx
                        cmp              eax, 99
                                                                                        je    n51_op11_α
                                                                                        jmp   n262_lit_string_α
n256_call_proc_staged_β:
                                                                                        jmp   n51_op11_α
.Lx726_0:
                        .quad            .Lx726_0_s
.Lx726_0_s:
                        .string          "roman/2"
#-----------------------------------------------------------------------------------------------------------------------
n257_op11_α:
                        mov              rax, qword ptr [rbp + 2400]
                        mov              qword ptr [rbp + 2352], rax
                        mov              rax, qword ptr [rbp + 2408]
                        mov              qword ptr [rbp + 2360], rax
                        mov              rax, qword ptr [rbp + 2416]
                        mov              qword ptr [rbp + 2368], rax
                        mov              rax, qword ptr [rbp + 2424]
                        mov              qword ptr [rbp + 2376], rax
                        lea              rdi, [rbp + 2352]
                        mov              esi, 2
                        call             rt_pl_dop_ax_sub@PLT
                        mov              qword ptr [rbp + 2336], rax
                        mov              qword ptr [rbp + 2344], rdx
                        cmp              eax, 99
                                                                                        je    n51_op11_α
                                                                                        jmp   n263_op11_α
n257_op11_β:
                                                                                        jmp   n51_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n258_op19_α:
                                                                                        jmp   n264_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n259_op11_α:
                        mov              rax, qword ptr [rbp + 1280]
                        mov              qword ptr [rbp + 1248], rax
                        mov              rax, qword ptr [rbp + 1288]
                        mov              qword ptr [rbp + 1256], rax
                        mov              rax, qword ptr [rbp + 1296]
                        mov              qword ptr [rbp + 1264], rax
                        mov              rax, qword ptr [rbp + 1304]
                        mov              qword ptr [rbp + 1272], rax
                        lea              rdi, [rbp + 1248]
                        lea              r8, [rbp + 1248]
.Lx729_40:
                        mov              eax, dword ptr [r8 + 0]
                        cmp              eax, 9
                                                                                        jne   .Lx729_56
                        mov              esi, dword ptr [r8 + 4]
                        mov              rax, qword ptr [r8 + 8]
                        test             rax, rax
                                                                                        je    .Lx729_41
                        cmp              esi, 1
                                                                                        jne   .Lx729_55
                        mov              r8, rax
                                                                                        jmp   .Lx729_40
.Lx729_55:
                        cmp              esi, 2
                                                                                        jne   .Lx729_41
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx729_41
                        mov              r8, rax
                                                                                        jmp   .Lx729_40
.Lx729_56:
                        cmp              eax, 13
                                                                                        jne   .Lx729_41
                        mov              rax, qword ptr [r8 + 8]
                        test             rax, rax
                                                                                        je    .Lx729_41
                        cmp              rax, r8
                                                                                        je    .Lx729_41
                        mov              r8, rax
                                                                                        jmp   .Lx729_40
.Lx729_41:
                        lea              r9, [rbp + 1264]
.Lx729_42:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 9
                                                                                        jne   .Lx729_58
                        mov              esi, dword ptr [r9 + 4]
                        mov              rax, qword ptr [r9 + 8]
                        test             rax, rax
                                                                                        je    .Lx729_43
                        cmp              esi, 1
                                                                                        jne   .Lx729_57
                        mov              r9, rax
                                                                                        jmp   .Lx729_42
.Lx729_57:
                        cmp              esi, 2
                                                                                        jne   .Lx729_43
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx729_43
                        mov              r9, rax
                                                                                        jmp   .Lx729_42
.Lx729_58:
                        cmp              eax, 13
                                                                                        jne   .Lx729_43
                        mov              rax, qword ptr [r9 + 8]
                        test             rax, rax
                                                                                        je    .Lx729_43
                        cmp              rax, r9
                                                                                        je    .Lx729_43
                        mov              r9, rax
                                                                                        jmp   .Lx729_42
.Lx729_43:
                        cmp              r8, r9
                                                                                        je    .Lx729_51
                        mov              eax, dword ptr [r8 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx729_44
                        cmp              eax, 99
                                                                                        je    .Lx729_44
                        cmp              eax, 13
                                                                                        jne   .Lx729_45
                        mov              rax, qword ptr [r8 + 8]
                        cmp              rax, r8
                                                                                        je    .Lx729_44
                                                                                        jmp   .Lx729_45
.Lx729_44:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx729_53
                        cmp              eax, 99
                                                                                        je    .Lx729_53
                        cmp              eax, 13
                                                                                        jne   .Lx729_46
                        mov              rax, qword ptr [r9 + 8]
                        cmp              rax, r9
                                                                                        je    .Lx729_53
                                                                                        jmp   .Lx729_46
.Lx729_46:
                        lea              r10, [rip + g_pl_trail]
                        mov              r11, qword ptr [r10 + 0]
                        test             r11, r11
                                                                                        je    .Lx729_53
                        mov              eax, dword ptr [r10 + 32]
                        mov              esi, 24
                        imul             rsi, rax
                        mov              rax, qword ptr [r10 + 24]
                        sub              rax, 24
                        cmp              rsi, rax
                                                                                        ja    .Lx729_53
                        add              r11, rsi
                        mov              qword ptr [r11 + 0], r8
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r11 + 8], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r11 + 16], rax
                        mov              eax, dword ptr [r10 + 32]
                        add              eax, 1
                        mov              dword ptr [r10 + 32], eax
                        mov              rax, qword ptr [r9 + 0]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [r8 + 8], rax
                                                                                        jmp   .Lx729_51
.Lx729_45:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx729_47
                        cmp              eax, 99
                                                                                        je    .Lx729_47
                        cmp              eax, 13
                                                                                        jne   .Lx729_48
                        mov              rax, qword ptr [r9 + 8]
                        cmp              rax, r9
                                                                                        je    .Lx729_47
                                                                                        jmp   .Lx729_48
.Lx729_47:
                        lea              r10, [rip + g_pl_trail]
                        mov              r11, qword ptr [r10 + 0]
                        test             r11, r11
                                                                                        je    .Lx729_53
                        mov              eax, dword ptr [r10 + 32]
                        mov              esi, 24
                        imul             rsi, rax
                        mov              rax, qword ptr [r10 + 24]
                        sub              rax, 24
                        cmp              rsi, rax
                                                                                        ja    .Lx729_53
                        add              r11, rsi
                        mov              qword ptr [r11 + 0], r9
                        mov              rax, qword ptr [r9 + 0]
                        mov              qword ptr [r11 + 8], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [r11 + 16], rax
                        mov              eax, dword ptr [r10 + 32]
                        add              eax, 1
                        mov              dword ptr [r10 + 32], eax
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 8], rax
                                                                                        jmp   .Lx729_51
.Lx729_48:
                        mov              ecx, dword ptr [r8 + 0]
                        mov              edx, dword ptr [r9 + 0]
                        cmp              ecx, 14
                                                                                        jne   .Lx729_49
                        cmp              edx, 14
                                                                                        je    .Lx729_53
                                                                                        jmp   .Lx729_52
.Lx729_49:
                        cmp              edx, 14
                                                                                        je    .Lx729_52
                        cmp              ecx, 7
                                                                                        je    .Lx729_53
                        cmp              edx, 7
                                                                                        je    .Lx729_53
                        cmp              ecx, 6
                                                                                        jne   .Lx729_50
                        cmp              edx, 6
                                                                                        jne   .Lx729_50
                        mov              rax, qword ptr [r8 + 0]
                        cmp              rax, 6
                                                                                        jne   .Lx729_53
                        mov              rax, qword ptr [r9 + 0]
                        cmp              rax, 6
                                                                                        jne   .Lx729_53
                        mov              rax, qword ptr [r8 + 8]
                        mov              rsi, qword ptr [r9 + 8]
                        cmp              rax, rsi
                                                                                        je    .Lx729_51
                                                                                        jmp   .Lx729_52
.Lx729_50:
                        mov              rax, qword ptr [r8 + 0]
                        mov              rsi, qword ptr [r9 + 0]
                        cmp              rax, rsi
                                                                                        jne   .Lx729_53
                        mov              rax, qword ptr [r8 + 8]
                        mov              rsi, qword ptr [r9 + 8]
                        cmp              rax, rsi
                                                                                        jne   .Lx729_53
.Lx729_51:
                        mov              rax, qword ptr [r8 + 0]
                        mov              rdx, qword ptr [r8 + 8]
                                                                                        jmp   .Lx729_54
.Lx729_52:
                        mov              eax, 99
                        mov              edx, 0
                                                                                        jmp   .Lx729_54
.Lx729_53:
                        mov              esi, 2
                        call             rt_pl_dop_unify@PLT
.Lx729_54:
                        mov              qword ptr [rbp + 1232], rax
                        mov              qword ptr [rbp + 1240], rdx
                        cmp              eax, 99
                                                                                        je    n248_op11_α
                                                                                        jmp   n265_var_α
n259_op11_β:
                                                                                        jmp   n248_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n260_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8784]
                        mov              qword ptr [rbp + 736], rax
                        mov              qword ptr [rbp + 744], rdx
                                                                                        jmp   n266_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n261_move_label_α:
                        lea              rax, [rip + n232_call_proc_staged_β]
                        mov              qword ptr [rbp + 64], rax
                                                                                        jmp   proc_roman$2F2_γ
#-----------------------------------------------------------------------------------------------------------------------
n262_lit_string_α:
                        mov              qword ptr [rbp + 2768], 1
                        mov              rax, qword ptr [rip + .Lx734_0]
                        mov              qword ptr [rbp + 2776], rax
                                                                                        jmp   n267_var_ref_α
.Lx734_0:
                        .quad            .Lx734_0_s
.Lx734_0_s:
                        .string          "X"
#-----------------------------------------------------------------------------------------------------------------------
n263_op11_α:
                        mov              rax, qword ptr [rbp + 2320]
                        mov              qword ptr [rbp + 2288], rax
                        mov              rax, qword ptr [rbp + 2328]
                        mov              qword ptr [rbp + 2296], rax
                        mov              rax, qword ptr [rbp + 2336]
                        mov              qword ptr [rbp + 2304], rax
                        mov              rax, qword ptr [rbp + 2344]
                        mov              qword ptr [rbp + 2312], rax
                        lea              rdi, [rbp + 2288]
                        mov              esi, 2
                        call             rt_pl_dop_is_v@PLT
                        mov              qword ptr [rbp + 2272], rax
                        mov              qword ptr [rbp + 2280], rdx
                        cmp              eax, 99
                                                                                        je    n51_op11_α
                                                                                        jmp   n268_var_ref_α
n263_op11_β:
                                                                                        jmp   n51_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n264_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8768]
                        mov              qword ptr [rbp + 1680], rax
                        mov              qword ptr [rbp + 1688], rdx
                                                                                        jmp   n269_var_α
#-----------------------------------------------------------------------------------------------------------------------
n265_var_α:
                        mov              rax, qword ptr [rbp + 8784]
                        mov              qword ptr [rbp + 1200], rax
                        mov              rax, qword ptr [rbp + 8792]
                        mov              qword ptr [rbp + 1208], rax
                                                                                        jmp   n270_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n266_op11_α:
                        mov              rax, qword ptr [rbp + 720]
                        mov              qword ptr [rbp + 688], rax
                        mov              rax, qword ptr [rbp + 728]
                        mov              qword ptr [rbp + 696], rax
                        mov              rax, qword ptr [rbp + 736]
                        mov              qword ptr [rbp + 704], rax
                        mov              rax, qword ptr [rbp + 744]
                        mov              qword ptr [rbp + 712], rax
                        lea              rdi, [rbp + 688]
                        lea              r8, [rbp + 688]
.Lx740_40:
                        mov              eax, dword ptr [r8 + 0]
                        cmp              eax, 9
                                                                                        jne   .Lx740_56
                        mov              esi, dword ptr [r8 + 4]
                        mov              rax, qword ptr [r8 + 8]
                        test             rax, rax
                                                                                        je    .Lx740_41
                        cmp              esi, 1
                                                                                        jne   .Lx740_55
                        mov              r8, rax
                                                                                        jmp   .Lx740_40
.Lx740_55:
                        cmp              esi, 2
                                                                                        jne   .Lx740_41
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx740_41
                        mov              r8, rax
                                                                                        jmp   .Lx740_40
.Lx740_56:
                        cmp              eax, 13
                                                                                        jne   .Lx740_41
                        mov              rax, qword ptr [r8 + 8]
                        test             rax, rax
                                                                                        je    .Lx740_41
                        cmp              rax, r8
                                                                                        je    .Lx740_41
                        mov              r8, rax
                                                                                        jmp   .Lx740_40
.Lx740_41:
                        lea              r9, [rbp + 704]
.Lx740_42:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 9
                                                                                        jne   .Lx740_58
                        mov              esi, dword ptr [r9 + 4]
                        mov              rax, qword ptr [r9 + 8]
                        test             rax, rax
                                                                                        je    .Lx740_43
                        cmp              esi, 1
                                                                                        jne   .Lx740_57
                        mov              r9, rax
                                                                                        jmp   .Lx740_42
.Lx740_57:
                        cmp              esi, 2
                                                                                        jne   .Lx740_43
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx740_43
                        mov              r9, rax
                                                                                        jmp   .Lx740_42
.Lx740_58:
                        cmp              eax, 13
                                                                                        jne   .Lx740_43
                        mov              rax, qword ptr [r9 + 8]
                        test             rax, rax
                                                                                        je    .Lx740_43
                        cmp              rax, r9
                                                                                        je    .Lx740_43
                        mov              r9, rax
                                                                                        jmp   .Lx740_42
.Lx740_43:
                        cmp              r8, r9
                                                                                        je    .Lx740_51
                        mov              eax, dword ptr [r8 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx740_44
                        cmp              eax, 99
                                                                                        je    .Lx740_44
                        cmp              eax, 13
                                                                                        jne   .Lx740_45
                        mov              rax, qword ptr [r8 + 8]
                        cmp              rax, r8
                                                                                        je    .Lx740_44
                                                                                        jmp   .Lx740_45
.Lx740_44:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx740_53
                        cmp              eax, 99
                                                                                        je    .Lx740_53
                        cmp              eax, 13
                                                                                        jne   .Lx740_46
                        mov              rax, qword ptr [r9 + 8]
                        cmp              rax, r9
                                                                                        je    .Lx740_53
                                                                                        jmp   .Lx740_46
.Lx740_46:
                        lea              r10, [rip + g_pl_trail]
                        mov              r11, qword ptr [r10 + 0]
                        test             r11, r11
                                                                                        je    .Lx740_53
                        mov              eax, dword ptr [r10 + 32]
                        mov              esi, 24
                        imul             rsi, rax
                        mov              rax, qword ptr [r10 + 24]
                        sub              rax, 24
                        cmp              rsi, rax
                                                                                        ja    .Lx740_53
                        add              r11, rsi
                        mov              qword ptr [r11 + 0], r8
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r11 + 8], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r11 + 16], rax
                        mov              eax, dword ptr [r10 + 32]
                        add              eax, 1
                        mov              dword ptr [r10 + 32], eax
                        mov              rax, qword ptr [r9 + 0]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [r8 + 8], rax
                                                                                        jmp   .Lx740_51
.Lx740_45:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx740_47
                        cmp              eax, 99
                                                                                        je    .Lx740_47
                        cmp              eax, 13
                                                                                        jne   .Lx740_48
                        mov              rax, qword ptr [r9 + 8]
                        cmp              rax, r9
                                                                                        je    .Lx740_47
                                                                                        jmp   .Lx740_48
.Lx740_47:
                        lea              r10, [rip + g_pl_trail]
                        mov              r11, qword ptr [r10 + 0]
                        test             r11, r11
                                                                                        je    .Lx740_53
                        mov              eax, dword ptr [r10 + 32]
                        mov              esi, 24
                        imul             rsi, rax
                        mov              rax, qword ptr [r10 + 24]
                        sub              rax, 24
                        cmp              rsi, rax
                                                                                        ja    .Lx740_53
                        add              r11, rsi
                        mov              qword ptr [r11 + 0], r9
                        mov              rax, qword ptr [r9 + 0]
                        mov              qword ptr [r11 + 8], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [r11 + 16], rax
                        mov              eax, dword ptr [r10 + 32]
                        add              eax, 1
                        mov              dword ptr [r10 + 32], eax
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 8], rax
                                                                                        jmp   .Lx740_51
.Lx740_48:
                        mov              ecx, dword ptr [r8 + 0]
                        mov              edx, dword ptr [r9 + 0]
                        cmp              ecx, 14
                                                                                        jne   .Lx740_49
                        cmp              edx, 14
                                                                                        je    .Lx740_53
                                                                                        jmp   .Lx740_52
.Lx740_49:
                        cmp              edx, 14
                                                                                        je    .Lx740_52
                        cmp              ecx, 7
                                                                                        je    .Lx740_53
                        cmp              edx, 7
                                                                                        je    .Lx740_53
                        cmp              ecx, 6
                                                                                        jne   .Lx740_50
                        cmp              edx, 6
                                                                                        jne   .Lx740_50
                        mov              rax, qword ptr [r8 + 0]
                        cmp              rax, 6
                                                                                        jne   .Lx740_53
                        mov              rax, qword ptr [r9 + 0]
                        cmp              rax, 6
                                                                                        jne   .Lx740_53
                        mov              rax, qword ptr [r8 + 8]
                        mov              rsi, qword ptr [r9 + 8]
                        cmp              rax, rsi
                                                                                        je    .Lx740_51
                                                                                        jmp   .Lx740_52
.Lx740_50:
                        mov              rax, qword ptr [r8 + 0]
                        mov              rsi, qword ptr [r9 + 0]
                        cmp              rax, rsi
                                                                                        jne   .Lx740_53
                        mov              rax, qword ptr [r8 + 8]
                        mov              rsi, qword ptr [r9 + 8]
                        cmp              rax, rsi
                                                                                        jne   .Lx740_53
.Lx740_51:
                        mov              rax, qword ptr [r8 + 0]
                        mov              rdx, qword ptr [r8 + 8]
                                                                                        jmp   .Lx740_54
.Lx740_52:
                        mov              eax, 99
                        mov              edx, 0
                                                                                        jmp   .Lx740_54
.Lx740_53:
                        mov              esi, 2
                        call             rt_pl_dop_unify@PLT
.Lx740_54:
                        mov              qword ptr [rbp + 672], rax
                        mov              qword ptr [rbp + 680], rdx
                        cmp              eax, 99
                                                                                        je    n51_op11_α
                                                                                        jmp   n271_var_ref_α
n266_op11_β:
                                                                                        jmp   n51_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n267_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8736]
                        mov              qword ptr [rbp + 2784], rax
                        mov              qword ptr [rbp + 2792], rdx
                                                                                        jmp   n272_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n268_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8768]
                        mov              qword ptr [rbp + 2240], rax
                        mov              qword ptr [rbp + 2248], rdx
                                                                                        jmp   n273_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n269_var_α:
                        mov              rax, qword ptr [rbp + 8784]
                        mov              qword ptr [rbp + 1760], rax
                        mov              rax, qword ptr [rbp + 8792]
                        mov              qword ptr [rbp + 1768], rax
                                                                                        jmp   n274_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n270_lit_integer_α:
                        mov              qword ptr [rbp + 1216], 6
                        mov              rax, qword ptr [rip + .Lx747_0]
                        mov              qword ptr [rbp + 1224], rax
                                                                                        jmp   n275_op11_α
.Lx747_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n271_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 32]
                        mov              qword ptr [rbp + 640], rax
                        mov              qword ptr [rbp + 648], rdx
                                                                                        jmp   n276_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n272_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8752]
                        mov              qword ptr [rbp + 2800], rax
                        mov              qword ptr [rbp + 2808], rdx
                                                                                        jmp   n277_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n273_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8736]
                        mov              qword ptr [rbp + 2256], rax
                        mov              qword ptr [rbp + 2264], rdx
                                                                                        jmp   n278_call_proc_staged_α
#-----------------------------------------------------------------------------------------------------------------------
n274_lit_integer_α:
                        mov              qword ptr [rbp + 1776], 6
                        mov              rax, qword ptr [rip + .Lx754_0]
                        mov              qword ptr [rbp + 1784], rax
                                                                                        jmp   n279_op11_α
.Lx754_0:
                        .quad            5
#-----------------------------------------------------------------------------------------------------------------------
n275_op11_α:
                        mov              rax, qword ptr [rbp + 1200]
                        mov              qword ptr [rbp + 1168], rax
                        mov              rax, qword ptr [rbp + 1208]
                        mov              qword ptr [rbp + 1176], rax
                        mov              rax, qword ptr [rbp + 1216]
                        mov              qword ptr [rbp + 1184], rax
                        mov              rax, qword ptr [rbp + 1224]
                        mov              qword ptr [rbp + 1192], rax
                        lea              rdi, [rbp + 1168]
                        mov              esi, 2
                        call             rt_pl_dop_cmp_ge@PLT
                        mov              qword ptr [rbp + 1152], rax
                        mov              qword ptr [rbp + 1160], rdx
                        cmp              eax, 99
                                                                                        je    n248_op11_α
                                                                                        jmp   n280_op19_α
n275_op11_β:
                                                                                        jmp   n248_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n276_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8752]
                        mov              qword ptr [rbp + 656], rax
                        mov              qword ptr [rbp + 664], rdx
                                                                                        jmp   n281_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n277_op11_α:
                        mov              rax, qword ptr [rbp + 2768]
                        mov              qword ptr [rbp + 2720], rax
                        mov              rax, qword ptr [rbp + 2776]
                        mov              qword ptr [rbp + 2728], rax
                        mov              rax, qword ptr [rbp + 2784]
                        mov              qword ptr [rbp + 2736], rax
                        mov              rax, qword ptr [rbp + 2792]
                        mov              qword ptr [rbp + 2744], rax
                        mov              rax, qword ptr [rbp + 2800]
                        mov              qword ptr [rbp + 2752], rax
                        mov              rax, qword ptr [rbp + 2808]
                        mov              qword ptr [rbp + 2760], rax
                        .section         .rodata
.Lrkfn759:              .string          "$atom_concat"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn759]
                        lea              rsi, [rbp + 2720]
                        mov              edx, 3
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 2704], rax
                        mov              qword ptr [rbp + 2712], rdx
                        cmp              eax, 99
                                                                                        je    n256_call_proc_staged_β
                                                                                        jmp   n282_move_label_α
n277_op11_β:
                                                                                        jmp   n256_call_proc_staged_β
#-----------------------------------------------------------------------------------------------------------------------
n278_call_proc_staged_α:
                        lea              rsi, [rbp + 2240]
                        lea              rdx, [rbp + 2256]
                        call             proc_roman$2F2_dcα
                                                                                        jmp   .Lx761_2
.Lx761_2:
                        mov              qword ptr [rbp + 2176], rax
                        mov              qword ptr [rbp + 2184], rdx
                        cmp              eax, 99
                                                                                        je    n51_op11_α
                                                                                        jmp   n283_lit_string_α
n278_call_proc_staged_β:
                                                                                        jmp   n51_op11_α
.Lx761_0:
                        .quad            .Lx761_0_s
.Lx761_0_s:
                        .string          "roman/2"
#-----------------------------------------------------------------------------------------------------------------------
n279_op11_α:
                        mov              rax, qword ptr [rbp + 1760]
                        mov              qword ptr [rbp + 1712], rax
                        mov              rax, qword ptr [rbp + 1768]
                        mov              qword ptr [rbp + 1720], rax
                        mov              rax, qword ptr [rbp + 1776]
                        mov              qword ptr [rbp + 1728], rax
                        mov              rax, qword ptr [rbp + 1784]
                        mov              qword ptr [rbp + 1736], rax
                        lea              rdi, [rbp + 1712]
                        mov              esi, 2
                        call             rt_pl_dop_ax_sub@PLT
                        mov              qword ptr [rbp + 1696], rax
                        mov              qword ptr [rbp + 1704], rdx
                        cmp              eax, 99
                                                                                        je    n51_op11_α
                                                                                        jmp   n284_op11_α
n279_op11_β:
                                                                                        jmp   n51_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n280_op19_α:
                                                                                        jmp   n285_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n281_op11_α:
                        mov              rax, qword ptr [rbp + 640]
                        mov              qword ptr [rbp + 608], rax
                        mov              rax, qword ptr [rbp + 648]
                        mov              qword ptr [rbp + 616], rax
                        mov              rax, qword ptr [rbp + 656]
                        mov              qword ptr [rbp + 624], rax
                        mov              rax, qword ptr [rbp + 664]
                        mov              qword ptr [rbp + 632], rax
                        lea              rdi, [rbp + 608]
                        lea              r8, [rbp + 608]
.Lx764_40:
                        mov              eax, dword ptr [r8 + 0]
                        cmp              eax, 9
                                                                                        jne   .Lx764_56
                        mov              esi, dword ptr [r8 + 4]
                        mov              rax, qword ptr [r8 + 8]
                        test             rax, rax
                                                                                        je    .Lx764_41
                        cmp              esi, 1
                                                                                        jne   .Lx764_55
                        mov              r8, rax
                                                                                        jmp   .Lx764_40
.Lx764_55:
                        cmp              esi, 2
                                                                                        jne   .Lx764_41
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx764_41
                        mov              r8, rax
                                                                                        jmp   .Lx764_40
.Lx764_56:
                        cmp              eax, 13
                                                                                        jne   .Lx764_41
                        mov              rax, qword ptr [r8 + 8]
                        test             rax, rax
                                                                                        je    .Lx764_41
                        cmp              rax, r8
                                                                                        je    .Lx764_41
                        mov              r8, rax
                                                                                        jmp   .Lx764_40
.Lx764_41:
                        lea              r9, [rbp + 624]
.Lx764_42:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 9
                                                                                        jne   .Lx764_58
                        mov              esi, dword ptr [r9 + 4]
                        mov              rax, qword ptr [r9 + 8]
                        test             rax, rax
                                                                                        je    .Lx764_43
                        cmp              esi, 1
                                                                                        jne   .Lx764_57
                        mov              r9, rax
                                                                                        jmp   .Lx764_42
.Lx764_57:
                        cmp              esi, 2
                                                                                        jne   .Lx764_43
                        mov              rax, qword ptr [rax + 0]
                        test             rax, rax
                                                                                        je    .Lx764_43
                        mov              r9, rax
                                                                                        jmp   .Lx764_42
.Lx764_58:
                        cmp              eax, 13
                                                                                        jne   .Lx764_43
                        mov              rax, qword ptr [r9 + 8]
                        test             rax, rax
                                                                                        je    .Lx764_43
                        cmp              rax, r9
                                                                                        je    .Lx764_43
                        mov              r9, rax
                                                                                        jmp   .Lx764_42
.Lx764_43:
                        cmp              r8, r9
                                                                                        je    .Lx764_51
                        mov              eax, dword ptr [r8 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx764_44
                        cmp              eax, 99
                                                                                        je    .Lx764_44
                        cmp              eax, 13
                                                                                        jne   .Lx764_45
                        mov              rax, qword ptr [r8 + 8]
                        cmp              rax, r8
                                                                                        je    .Lx764_44
                                                                                        jmp   .Lx764_45
.Lx764_44:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx764_53
                        cmp              eax, 99
                                                                                        je    .Lx764_53
                        cmp              eax, 13
                                                                                        jne   .Lx764_46
                        mov              rax, qword ptr [r9 + 8]
                        cmp              rax, r9
                                                                                        je    .Lx764_53
                                                                                        jmp   .Lx764_46
.Lx764_46:
                        lea              r10, [rip + g_pl_trail]
                        mov              r11, qword ptr [r10 + 0]
                        test             r11, r11
                                                                                        je    .Lx764_53
                        mov              eax, dword ptr [r10 + 32]
                        mov              esi, 24
                        imul             rsi, rax
                        mov              rax, qword ptr [r10 + 24]
                        sub              rax, 24
                        cmp              rsi, rax
                                                                                        ja    .Lx764_53
                        add              r11, rsi
                        mov              qword ptr [r11 + 0], r8
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r11 + 8], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r11 + 16], rax
                        mov              eax, dword ptr [r10 + 32]
                        add              eax, 1
                        mov              dword ptr [r10 + 32], eax
                        mov              rax, qword ptr [r9 + 0]
                        mov              qword ptr [r8 + 0], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [r8 + 8], rax
                                                                                        jmp   .Lx764_51
.Lx764_45:
                        mov              eax, dword ptr [r9 + 0]
                        cmp              eax, 0
                                                                                        je    .Lx764_47
                        cmp              eax, 99
                                                                                        je    .Lx764_47
                        cmp              eax, 13
                                                                                        jne   .Lx764_48
                        mov              rax, qword ptr [r9 + 8]
                        cmp              rax, r9
                                                                                        je    .Lx764_47
                                                                                        jmp   .Lx764_48
.Lx764_47:
                        lea              r10, [rip + g_pl_trail]
                        mov              r11, qword ptr [r10 + 0]
                        test             r11, r11
                                                                                        je    .Lx764_53
                        mov              eax, dword ptr [r10 + 32]
                        mov              esi, 24
                        imul             rsi, rax
                        mov              rax, qword ptr [r10 + 24]
                        sub              rax, 24
                        cmp              rsi, rax
                                                                                        ja    .Lx764_53
                        add              r11, rsi
                        mov              qword ptr [r11 + 0], r9
                        mov              rax, qword ptr [r9 + 0]
                        mov              qword ptr [r11 + 8], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [r11 + 16], rax
                        mov              eax, dword ptr [r10 + 32]
                        add              eax, 1
                        mov              dword ptr [r10 + 32], eax
                        mov              rax, qword ptr [r8 + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [r8 + 8]
                        mov              qword ptr [r9 + 8], rax
                                                                                        jmp   .Lx764_51
.Lx764_48:
                        mov              ecx, dword ptr [r8 + 0]
                        mov              edx, dword ptr [r9 + 0]
                        cmp              ecx, 14
                                                                                        jne   .Lx764_49
                        cmp              edx, 14
                                                                                        je    .Lx764_53
                                                                                        jmp   .Lx764_52
.Lx764_49:
                        cmp              edx, 14
                                                                                        je    .Lx764_52
                        cmp              ecx, 7
                                                                                        je    .Lx764_53
                        cmp              edx, 7
                                                                                        je    .Lx764_53
                        cmp              ecx, 6
                                                                                        jne   .Lx764_50
                        cmp              edx, 6
                                                                                        jne   .Lx764_50
                        mov              rax, qword ptr [r8 + 0]
                        cmp              rax, 6
                                                                                        jne   .Lx764_53
                        mov              rax, qword ptr [r9 + 0]
                        cmp              rax, 6
                                                                                        jne   .Lx764_53
                        mov              rax, qword ptr [r8 + 8]
                        mov              rsi, qword ptr [r9 + 8]
                        cmp              rax, rsi
                                                                                        je    .Lx764_51
                                                                                        jmp   .Lx764_52
.Lx764_50:
                        mov              rax, qword ptr [r8 + 0]
                        mov              rsi, qword ptr [r9 + 0]
                        cmp              rax, rsi
                                                                                        jne   .Lx764_53
                        mov              rax, qword ptr [r8 + 8]
                        mov              rsi, qword ptr [r9 + 8]
                        cmp              rax, rsi
                                                                                        jne   .Lx764_53
.Lx764_51:
                        mov              rax, qword ptr [r8 + 0]
                        mov              rdx, qword ptr [r8 + 8]
                                                                                        jmp   .Lx764_54
.Lx764_52:
                        mov              eax, 99
                        mov              edx, 0
                                                                                        jmp   .Lx764_54
.Lx764_53:
                        mov              esi, 2
                        call             rt_pl_dop_unify@PLT
.Lx764_54:
                        mov              qword ptr [rbp + 592], rax
                        mov              qword ptr [rbp + 600], rdx
                        cmp              eax, 99
                                                                                        je    n51_op11_α
                                                                                        jmp   n286_var_α
n281_op11_β:
                                                                                        jmp   n51_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n282_move_label_α:
                        lea              rax, [rip + n256_call_proc_staged_β]
                        mov              qword ptr [rbp + 64], rax
                                                                                        jmp   proc_roman$2F2_γ
#-----------------------------------------------------------------------------------------------------------------------
n283_lit_string_α:
                        mov              qword ptr [rbp + 2128], 1
                        mov              rax, qword ptr [rip + .Lx767_0]
                        mov              qword ptr [rbp + 2136], rax
                                                                                        jmp   n287_var_ref_α
.Lx767_0:
                        .quad            .Lx767_0_s
.Lx767_0_s:
                        .string          "IX"
#-----------------------------------------------------------------------------------------------------------------------
n284_op11_α:
                        mov              rax, qword ptr [rbp + 1680]
                        mov              qword ptr [rbp + 1648], rax
                        mov              rax, qword ptr [rbp + 1688]
                        mov              qword ptr [rbp + 1656], rax
                        mov              rax, qword ptr [rbp + 1696]
                        mov              qword ptr [rbp + 1664], rax
                        mov              rax, qword ptr [rbp + 1704]
                        mov              qword ptr [rbp + 1672], rax
                        lea              rdi, [rbp + 1648]
                        mov              esi, 2
                        call             rt_pl_dop_is_v@PLT
                        mov              qword ptr [rbp + 1632], rax
                        mov              qword ptr [rbp + 1640], rdx
                        cmp              eax, 99
                                                                                        je    n51_op11_α
                                                                                        jmp   n288_var_ref_α
n284_op11_β:
                                                                                        jmp   n51_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n285_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8768]
                        mov              qword ptr [rbp + 1040], rax
                        mov              qword ptr [rbp + 1048], rdx
                                                                                        jmp   n289_var_α
#-----------------------------------------------------------------------------------------------------------------------
n286_var_α:
                        mov              rax, qword ptr [rbp + 8784]
                        mov              qword ptr [rbp + 560], rax
                        mov              rax, qword ptr [rbp + 8792]
                        mov              qword ptr [rbp + 568], rax
                                                                                        jmp   n290_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n287_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8736]
                        mov              qword ptr [rbp + 2144], rax
                        mov              qword ptr [rbp + 2152], rdx
                                                                                        jmp   n291_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n288_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8768]
                        mov              qword ptr [rbp + 1600], rax
                        mov              qword ptr [rbp + 1608], rdx
                                                                                        jmp   n292_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n289_var_α:
                        mov              rax, qword ptr [rbp + 8784]
                        mov              qword ptr [rbp + 1120], rax
                        mov              rax, qword ptr [rbp + 8792]
                        mov              qword ptr [rbp + 1128], rax
                                                                                        jmp   n293_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n290_lit_integer_α:
                        mov              qword ptr [rbp + 576], 6
                        mov              rax, qword ptr [rip + .Lx779_0]
                        mov              qword ptr [rbp + 584], rax
                                                                                        jmp   n294_op11_α
.Lx779_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n291_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8752]
                        mov              qword ptr [rbp + 2160], rax
                        mov              qword ptr [rbp + 2168], rdx
                                                                                        jmp   n295_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n292_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8736]
                        mov              qword ptr [rbp + 1616], rax
                        mov              qword ptr [rbp + 1624], rdx
                                                                                        jmp   n296_call_proc_staged_α
#-----------------------------------------------------------------------------------------------------------------------
n293_lit_integer_α:
                        mov              qword ptr [rbp + 1136], 6
                        mov              rax, qword ptr [rip + .Lx784_0]
                        mov              qword ptr [rbp + 1144], rax
                                                                                        jmp   n297_op11_α
.Lx784_0:
                        .quad            4
#-----------------------------------------------------------------------------------------------------------------------
n294_op11_α:
                        mov              rax, qword ptr [rbp + 560]
                        mov              qword ptr [rbp + 528], rax
                        mov              rax, qword ptr [rbp + 568]
                        mov              qword ptr [rbp + 536], rax
                        mov              rax, qword ptr [rbp + 576]
                        mov              qword ptr [rbp + 544], rax
                        mov              rax, qword ptr [rbp + 584]
                        mov              qword ptr [rbp + 552], rax
                        lea              rdi, [rbp + 528]
                        mov              esi, 2
                        call             rt_pl_dop_cmp_ge@PLT
                        mov              qword ptr [rbp + 512], rax
                        mov              qword ptr [rbp + 520], rdx
                        cmp              eax, 99
                                                                                        je    n51_op11_α
                                                                                        jmp   n298_op19_α
n294_op11_β:
                                                                                        jmp   n51_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n295_op11_α:
                        mov              rax, qword ptr [rbp + 2128]
                        mov              qword ptr [rbp + 2080], rax
                        mov              rax, qword ptr [rbp + 2136]
                        mov              qword ptr [rbp + 2088], rax
                        mov              rax, qword ptr [rbp + 2144]
                        mov              qword ptr [rbp + 2096], rax
                        mov              rax, qword ptr [rbp + 2152]
                        mov              qword ptr [rbp + 2104], rax
                        mov              rax, qword ptr [rbp + 2160]
                        mov              qword ptr [rbp + 2112], rax
                        mov              rax, qword ptr [rbp + 2168]
                        mov              qword ptr [rbp + 2120], rax
                        .section         .rodata
.Lrkfn787:              .string          "$atom_concat"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn787]
                        lea              rsi, [rbp + 2080]
                        mov              edx, 3
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 2064], rax
                        mov              qword ptr [rbp + 2072], rdx
                        cmp              eax, 99
                                                                                        je    n278_call_proc_staged_β
                                                                                        jmp   n299_move_label_α
n295_op11_β:
                                                                                        jmp   n278_call_proc_staged_β
#-----------------------------------------------------------------------------------------------------------------------
n296_call_proc_staged_α:
                        lea              rsi, [rbp + 1600]
                        lea              rdx, [rbp + 1616]
                        call             proc_roman$2F2_dcα
                                                                                        jmp   .Lx789_2
.Lx789_2:
                        mov              qword ptr [rbp + 1536], rax
                        mov              qword ptr [rbp + 1544], rdx
                        cmp              eax, 99
                                                                                        je    n51_op11_α
                                                                                        jmp   n300_lit_string_α
n296_call_proc_staged_β:
                                                                                        jmp   n51_op11_α
.Lx789_0:
                        .quad            .Lx789_0_s
.Lx789_0_s:
                        .string          "roman/2"
#-----------------------------------------------------------------------------------------------------------------------
n297_op11_α:
                        mov              rax, qword ptr [rbp + 1120]
                        mov              qword ptr [rbp + 1072], rax
                        mov              rax, qword ptr [rbp + 1128]
                        mov              qword ptr [rbp + 1080], rax
                        mov              rax, qword ptr [rbp + 1136]
                        mov              qword ptr [rbp + 1088], rax
                        mov              rax, qword ptr [rbp + 1144]
                        mov              qword ptr [rbp + 1096], rax
                        lea              rdi, [rbp + 1072]
                        mov              esi, 2
                        call             rt_pl_dop_ax_sub@PLT
                        mov              qword ptr [rbp + 1056], rax
                        mov              qword ptr [rbp + 1064], rdx
                        cmp              eax, 99
                                                                                        je    n51_op11_α
                                                                                        jmp   n301_op11_α
n297_op11_β:
                                                                                        jmp   n51_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n298_op19_α:
                                                                                        jmp   n302_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n299_move_label_α:
                        lea              rax, [rip + n278_call_proc_staged_β]
                        mov              qword ptr [rbp + 64], rax
                                                                                        jmp   proc_roman$2F2_γ
#-----------------------------------------------------------------------------------------------------------------------
n300_lit_string_α:
                        mov              qword ptr [rbp + 1488], 1
                        mov              rax, qword ptr [rip + .Lx794_0]
                        mov              qword ptr [rbp + 1496], rax
                                                                                        jmp   n303_var_ref_α
.Lx794_0:
                        .quad            .Lx794_0_s
.Lx794_0_s:
                        .string          "V"
#-----------------------------------------------------------------------------------------------------------------------
n301_op11_α:
                        mov              rax, qword ptr [rbp + 1040]
                        mov              qword ptr [rbp + 1008], rax
                        mov              rax, qword ptr [rbp + 1048]
                        mov              qword ptr [rbp + 1016], rax
                        mov              rax, qword ptr [rbp + 1056]
                        mov              qword ptr [rbp + 1024], rax
                        mov              rax, qword ptr [rbp + 1064]
                        mov              qword ptr [rbp + 1032], rax
                        lea              rdi, [rbp + 1008]
                        mov              esi, 2
                        call             rt_pl_dop_is_v@PLT
                        mov              qword ptr [rbp + 992], rax
                        mov              qword ptr [rbp + 1000], rdx
                        cmp              eax, 99
                                                                                        je    n51_op11_α
                                                                                        jmp   n304_var_ref_α
n301_op11_β:
                                                                                        jmp   n51_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n302_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8768]
                        mov              qword ptr [rbp + 400], rax
                        mov              qword ptr [rbp + 408], rdx
                                                                                        jmp   n305_var_α
#-----------------------------------------------------------------------------------------------------------------------
n303_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8736]
                        mov              qword ptr [rbp + 1504], rax
                        mov              qword ptr [rbp + 1512], rdx
                                                                                        jmp   n306_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n304_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8768]
                        mov              qword ptr [rbp + 960], rax
                        mov              qword ptr [rbp + 968], rdx
                                                                                        jmp   n307_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n305_var_α:
                        mov              rax, qword ptr [rbp + 8784]
                        mov              qword ptr [rbp + 480], rax
                        mov              rax, qword ptr [rbp + 8792]
                        mov              qword ptr [rbp + 488], rax
                                                                                        jmp   n308_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n306_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8752]
                        mov              qword ptr [rbp + 1520], rax
                        mov              qword ptr [rbp + 1528], rdx
                                                                                        jmp   n309_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n307_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8736]
                        mov              qword ptr [rbp + 976], rax
                        mov              qword ptr [rbp + 984], rdx
                                                                                        jmp   n310_call_proc_staged_α
#-----------------------------------------------------------------------------------------------------------------------
n308_lit_integer_α:
                        mov              qword ptr [rbp + 496], 6
                        mov              rax, qword ptr [rip + .Lx808_0]
                        mov              qword ptr [rbp + 504], rax
                                                                                        jmp   n311_op11_α
.Lx808_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n309_op11_α:
                        mov              rax, qword ptr [rbp + 1488]
                        mov              qword ptr [rbp + 1440], rax
                        mov              rax, qword ptr [rbp + 1496]
                        mov              qword ptr [rbp + 1448], rax
                        mov              rax, qword ptr [rbp + 1504]
                        mov              qword ptr [rbp + 1456], rax
                        mov              rax, qword ptr [rbp + 1512]
                        mov              qword ptr [rbp + 1464], rax
                        mov              rax, qword ptr [rbp + 1520]
                        mov              qword ptr [rbp + 1472], rax
                        mov              rax, qword ptr [rbp + 1528]
                        mov              qword ptr [rbp + 1480], rax
                        .section         .rodata
.Lrkfn810:              .string          "$atom_concat"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn810]
                        lea              rsi, [rbp + 1440]
                        mov              edx, 3
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 1424], rax
                        mov              qword ptr [rbp + 1432], rdx
                        cmp              eax, 99
                                                                                        je    n296_call_proc_staged_β
                                                                                        jmp   n312_move_label_α
n309_op11_β:
                                                                                        jmp   n296_call_proc_staged_β
#-----------------------------------------------------------------------------------------------------------------------
n310_call_proc_staged_α:
                        lea              rsi, [rbp + 960]
                        lea              rdx, [rbp + 976]
                        call             proc_roman$2F2_dcα
                                                                                        jmp   .Lx812_2
.Lx812_2:
                        mov              qword ptr [rbp + 896], rax
                        mov              qword ptr [rbp + 904], rdx
                        cmp              eax, 99
                                                                                        je    n51_op11_α
                                                                                        jmp   n313_lit_string_α
n310_call_proc_staged_β:
                                                                                        jmp   n51_op11_α
.Lx812_0:
                        .quad            .Lx812_0_s
.Lx812_0_s:
                        .string          "roman/2"
#-----------------------------------------------------------------------------------------------------------------------
n311_op11_α:
                        mov              rax, qword ptr [rbp + 480]
                        mov              qword ptr [rbp + 432], rax
                        mov              rax, qword ptr [rbp + 488]
                        mov              qword ptr [rbp + 440], rax
                        mov              rax, qword ptr [rbp + 496]
                        mov              qword ptr [rbp + 448], rax
                        mov              rax, qword ptr [rbp + 504]
                        mov              qword ptr [rbp + 456], rax
                        lea              rdi, [rbp + 432]
                        mov              esi, 2
                        call             rt_pl_dop_ax_sub@PLT
                        mov              qword ptr [rbp + 416], rax
                        mov              qword ptr [rbp + 424], rdx
                        cmp              eax, 99
                                                                                        je    n51_op11_α
                                                                                        jmp   n314_op11_α
n311_op11_β:
                                                                                        jmp   n51_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n312_move_label_α:
                        lea              rax, [rip + n296_call_proc_staged_β]
                        mov              qword ptr [rbp + 64], rax
                                                                                        jmp   proc_roman$2F2_γ
#-----------------------------------------------------------------------------------------------------------------------
n313_lit_string_α:
                        mov              qword ptr [rbp + 848], 1
                        mov              rax, qword ptr [rip + .Lx816_0]
                        mov              qword ptr [rbp + 856], rax
                                                                                        jmp   n315_var_ref_α
.Lx816_0:
                        .quad            .Lx816_0_s
.Lx816_0_s:
                        .string          "IV"
#-----------------------------------------------------------------------------------------------------------------------
n314_op11_α:
                        mov              rax, qword ptr [rbp + 400]
                        mov              qword ptr [rbp + 368], rax
                        mov              rax, qword ptr [rbp + 408]
                        mov              qword ptr [rbp + 376], rax
                        mov              rax, qword ptr [rbp + 416]
                        mov              qword ptr [rbp + 384], rax
                        mov              rax, qword ptr [rbp + 424]
                        mov              qword ptr [rbp + 392], rax
                        lea              rdi, [rbp + 368]
                        mov              esi, 2
                        call             rt_pl_dop_is_v@PLT
                        mov              qword ptr [rbp + 352], rax
                        mov              qword ptr [rbp + 360], rdx
                        cmp              eax, 99
                                                                                        je    n51_op11_α
                                                                                        jmp   n316_var_ref_α
n314_op11_β:
                                                                                        jmp   n51_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n315_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8736]
                        mov              qword ptr [rbp + 864], rax
                        mov              qword ptr [rbp + 872], rdx
                                                                                        jmp   n317_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n316_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8768]
                        mov              qword ptr [rbp + 320], rax
                        mov              qword ptr [rbp + 328], rdx
                                                                                        jmp   n318_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n317_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8752]
                        mov              qword ptr [rbp + 880], rax
                        mov              qword ptr [rbp + 888], rdx
                                                                                        jmp   n319_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n318_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8736]
                        mov              qword ptr [rbp + 336], rax
                        mov              qword ptr [rbp + 344], rdx
                                                                                        jmp   n320_call_proc_staged_α
#-----------------------------------------------------------------------------------------------------------------------
n319_op11_α:
                        mov              rax, qword ptr [rbp + 848]
                        mov              qword ptr [rbp + 800], rax
                        mov              rax, qword ptr [rbp + 856]
                        mov              qword ptr [rbp + 808], rax
                        mov              rax, qword ptr [rbp + 864]
                        mov              qword ptr [rbp + 816], rax
                        mov              rax, qword ptr [rbp + 872]
                        mov              qword ptr [rbp + 824], rax
                        mov              rax, qword ptr [rbp + 880]
                        mov              qword ptr [rbp + 832], rax
                        mov              rax, qword ptr [rbp + 888]
                        mov              qword ptr [rbp + 840], rax
                        .section         .rodata
.Lrkfn827:              .string          "$atom_concat"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn827]
                        lea              rsi, [rbp + 800]
                        mov              edx, 3
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 784], rax
                        mov              qword ptr [rbp + 792], rdx
                        cmp              eax, 99
                                                                                        je    n310_call_proc_staged_β
                                                                                        jmp   n321_move_label_α
n319_op11_β:
                                                                                        jmp   n310_call_proc_staged_β
#-----------------------------------------------------------------------------------------------------------------------
n320_call_proc_staged_α:
                        lea              rsi, [rbp + 320]
                        lea              rdx, [rbp + 336]
                        call             proc_roman$2F2_dcα
                                                                                        jmp   .Lx829_2
.Lx829_2:
                        mov              qword ptr [rbp + 256], rax
                        mov              qword ptr [rbp + 264], rdx
                        cmp              eax, 99
                                                                                        je    n51_op11_α
                                                                                        jmp   n322_lit_string_α
n320_call_proc_staged_β:
                                                                                        jmp   n51_op11_α
.Lx829_0:
                        .quad            .Lx829_0_s
.Lx829_0_s:
                        .string          "roman/2"
#-----------------------------------------------------------------------------------------------------------------------
n321_move_label_α:
                        lea              rax, [rip + n310_call_proc_staged_β]
                        mov              qword ptr [rbp + 64], rax
                                                                                        jmp   proc_roman$2F2_γ
#-----------------------------------------------------------------------------------------------------------------------
n322_lit_string_α:
                        mov              qword ptr [rbp + 208], 1
                        mov              rax, qword ptr [rip + .Lx832_0]
                        mov              qword ptr [rbp + 216], rax
                                                                                        jmp   n323_var_ref_α
.Lx832_0:
                        .quad            .Lx832_0_s
.Lx832_0_s:
                        .string          "I"
#-----------------------------------------------------------------------------------------------------------------------
n323_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8736]
                        mov              qword ptr [rbp + 224], rax
                        mov              qword ptr [rbp + 232], rdx
                                                                                        jmp   n324_var_ref_α
#-----------------------------------------------------------------------------------------------------------------------
n324_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 8752]
                        mov              qword ptr [rbp + 240], rax
                        mov              qword ptr [rbp + 248], rdx
                                                                                        jmp   n325_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n325_op11_α:
                        mov              rax, qword ptr [rbp + 208]
                        mov              qword ptr [rbp + 160], rax
                        mov              rax, qword ptr [rbp + 216]
                        mov              qword ptr [rbp + 168], rax
                        mov              rax, qword ptr [rbp + 224]
                        mov              qword ptr [rbp + 176], rax
                        mov              rax, qword ptr [rbp + 232]
                        mov              qword ptr [rbp + 184], rax
                        mov              rax, qword ptr [rbp + 240]
                        mov              qword ptr [rbp + 192], rax
                        mov              rax, qword ptr [rbp + 248]
                        mov              qword ptr [rbp + 200], rax
                        .section         .rodata
.Lrkfn838:              .string          "$atom_concat"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn838]
                        lea              rsi, [rbp + 160]
                        mov              edx, 3
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 144], rax
                        mov              qword ptr [rbp + 152], rdx
                        cmp              eax, 99
                                                                                        je    n320_call_proc_staged_β
                                                                                        jmp   n326_move_label_α
n325_op11_β:
                                                                                        jmp   n320_call_proc_staged_β
#-----------------------------------------------------------------------------------------------------------------------
n326_move_label_α:
                        lea              rax, [rip + n320_call_proc_staged_β]
                        mov              qword ptr [rbp + 64], rax
                                                                                        jmp   proc_roman$2F2_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_roman$2F2_res:
                        add              rsp, 8
                        pop              rbp
#-----------------------------------------------------------------------------------------------------------------------
proc_roman$2F2_β:
                                                                                        jmp   n28_disjunction_α
#-----------------------------------------------------------------------------------------------------------------------
proc_roman$2F2_γ:
                        mov              rdi, [rbp]
                        mov              rsi, [rbp + 8]
                        mov              rax, [rbp + 8808]
                        lea              rsp, [rbp + 8832]
                        mov              rbp, [rbp + 8824]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_roman$2F2_ω:
                        mov              rax, [rbp + 8816]
                        lea              rsp, [rbp + 8832]
                        mov              rbp, [rbp + 8824]
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_roman$2F2_dcα:
                        pop              r11
                        sub              rsp, 8848
                        mov              qword ptr [rsp + 8824], rbp
                        mov              rbp, rsp
                        mov              qword ptr [rbp + 8800], r11
                        lea              rax, [rip + .Lx841_2]
                        mov              qword ptr [rbp + 8808], rax
                        lea              rax, [rip + .Lx841_3]
                        mov              qword ptr [rbp + 8816], rax
                        mov              qword ptr [rbp + 16], rsi
                        mov              qword ptr [rbp + 24], rdx
                        mov              rdi, rbp
                        mov              esi, 8736
                        mov              edx, 8800
                        mov              ecx, 2
                        mov              r8d, 2
                        mov              r9d, 0
                        call             rt_pl_dc_prep@PLT
                                                                                        jmp   proc_roman$2F2_α_body
.Lx841_2:
                        mov              rdx, qword ptr [rsp + 0]
                        mov              rcx, rsp
                        add              rcx, -8832
                        mov              r11, qword ptr [rsp + -32]
                        mov              rbp, qword ptr [rsp + -8]
                        add              rsp, 16
                        push             r11
                                                                                        jmp   rt_pl_dc_leave_γ@PLT
.Lx841_3:
                        mov              rdi, qword ptr [rsp + 0]
                        mov              rsi, rsp
                        add              rsi, -8832
                        mov              r11, qword ptr [rsp + -32]
                        mov              rbp, qword ptr [rsp + -8]
                        add              rsp, 16
                        push             r11
                                                                                        jmp   rt_pl_dc_leave_ω@PLT
proc_startup:
                        sub              rsp, 8
                        .section         .rodata
.Lstartup_pname0:       .string          "roman/2"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname0]
                        lea              rsi, [rip + proc_roman$2F2_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 2
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 8800
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        lea              rsi, [rip + proc_roman$2F2_dcα]
                        call             rt_proc_set_dcfn@PLT
                        add              rsp, 8
                        ret
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        call             proc_startup
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
                        sub              rsp, 728
                        mov              rdi, rsp
                        mov              ecx, 728
                        xor              eax, eax
                        rep stosb
                        mov              [rsp + 720], rbp
                        mov              rbp, rsp
main_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n842_op11_α:
                        lea              r10, [rip + g_plw_cellws_on]
                        mov              eax, dword ptr [r10 + 0]
                        test             eax, eax
                                                                                        jne   .Lx867_100
                        lea              r10, [rip + g_zeta_mode]
                        mov              eax, dword ptr [r10 + 0]
                        cmp              eax, 2
                                                                                        je    .Lx867_100
                        lea              r10, [rip + g_pl_trail]
                        mov              eax, dword ptr [r10 + 32]
                        movsxd           rdx, eax
                        mov              eax, 6
                                                                                        jmp   .Lx867_101
.Lx867_100:
                        lea              rdi, [rbp + 48]
                        mov              esi, 0
                        call             rt_pl_dop_trail_mark@PLT
.Lx867_101:
                        mov              qword ptr [rbp + 32], rax
                        mov              qword ptr [rbp + 40], rdx
                        cmp              eax, 99
                                                                                        je    main_ω
                                                                                        jmp   n843_lit_integer_α
n842_op11_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n843_lit_integer_α:
                        mov              qword ptr [rbp + 640], 6
                        mov              rax, qword ptr [rip + .Lx868_0]
                        mov              qword ptr [rbp + 648], rax
                                                                                        jmp   n844_var_ref_α
.Lx868_0:
                        .quad            1776
#-----------------------------------------------------------------------------------------------------------------------
n844_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 704]
                        mov              qword ptr [rbp + 656], rax
                        mov              qword ptr [rbp + 664], rdx
                                                                                        jmp   n845_call_proc_staged_α
#-----------------------------------------------------------------------------------------------------------------------
n845_call_proc_staged_α:
                        lea              rsi, [rbp + 640]
                        lea              rdx, [rbp + 656]
                        call             proc_roman$2F2_dcα
                                                                                        jmp   .Lx872_2
.Lx872_2:
                        mov              qword ptr [rbp + 576], rax
                        mov              qword ptr [rbp + 584], rdx
                        cmp              eax, 99
                                                                                        je    n847_op11_α
                                                                                        jmp   n846_var_α
n845_call_proc_staged_β:
                                                                                        jmp   n847_op11_α
.Lx872_0:
                        .quad            .Lx872_0_s
.Lx872_0_s:
                        .string          "roman/2"
#-----------------------------------------------------------------------------------------------------------------------
n846_var_α:
                        mov              rax, qword ptr [rbp + 704]
                        mov              qword ptr [rbp + 560], rax
                        mov              rax, qword ptr [rbp + 712]
                        mov              qword ptr [rbp + 568], rax
                                                                                        jmp   n848_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n847_op11_α:
                        mov              rax, qword ptr [rbp + 32]
                        mov              qword ptr [rbp + 80], rax
                        mov              rax, qword ptr [rbp + 40]
                        mov              qword ptr [rbp + 88], rax
                        lea              rdi, [rbp + 80]
                        mov              esi, 1
                        call             rt_pl_dop_trail_unwind@PLT
                        mov              qword ptr [rbp + 64], rax
                        mov              qword ptr [rbp + 72], rdx
                        cmp              eax, 99
                                                                                        je    main_ω
                                                                                        jmp   main_ω
n847_op11_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n848_op11_α:
                        mov              rax, qword ptr [rbp + 560]
                        mov              qword ptr [rbp + 544], rax
                        mov              rax, qword ptr [rbp + 568]
                        mov              qword ptr [rbp + 552], rax
                        .section         .rodata
.Lrkfn877:              .string          "$write"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn877]
                        lea              rsi, [rbp + 544]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 528], rax
                        mov              qword ptr [rbp + 536], rdx
                        cmp              eax, 99
                                                                                        je    n845_call_proc_staged_β
                                                                                        jmp   n849_lit_string_α
n848_op11_β:
                                                                                        jmp   n845_call_proc_staged_β
#-----------------------------------------------------------------------------------------------------------------------
n849_lit_string_α:
                        mov              qword ptr [rbp + 512], 1
                        mov              rax, qword ptr [rip + .Lx878_0]
                        mov              qword ptr [rbp + 520], rax
                                                                                        jmp   n850_op11_α
.Lx878_0:
                        .quad            .Lx878_0_s
.Lx878_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n850_op11_α:
                        mov              rax, qword ptr [rbp + 512]
                        mov              qword ptr [rbp + 496], rax
                        mov              rax, qword ptr [rbp + 520]
                        mov              qword ptr [rbp + 504], rax
                        .section         .rodata
.Lrkfn880:              .string          "$nl0"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn880]
                        lea              rsi, [rbp + 496]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 480], rax
                        mov              qword ptr [rbp + 488], rdx
                        cmp              eax, 99
                                                                                        je    n845_call_proc_staged_β
                                                                                        jmp   n851_lit_integer_α
n850_op11_β:
                                                                                        jmp   n845_call_proc_staged_β
#-----------------------------------------------------------------------------------------------------------------------
n851_lit_integer_α:
                        mov              qword ptr [rbp + 448], 6
                        mov              rax, qword ptr [rip + .Lx881_0]
                        mov              qword ptr [rbp + 456], rax
                                                                                        jmp   n852_var_ref_α
.Lx881_0:
                        .quad            42
#-----------------------------------------------------------------------------------------------------------------------
n852_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 688]
                        mov              qword ptr [rbp + 464], rax
                        mov              qword ptr [rbp + 472], rdx
                                                                                        jmp   n853_call_proc_staged_α
#-----------------------------------------------------------------------------------------------------------------------
n853_call_proc_staged_α:
                        lea              rsi, [rbp + 448]
                        lea              rdx, [rbp + 464]
                        call             proc_roman$2F2_dcα
                                                                                        jmp   .Lx885_2
.Lx885_2:
                        mov              qword ptr [rbp + 384], rax
                        mov              qword ptr [rbp + 392], rdx
                        cmp              eax, 99
                                                                                        je    n845_call_proc_staged_β
                                                                                        jmp   n854_var_α
n853_call_proc_staged_β:
                                                                                        jmp   n845_call_proc_staged_β
.Lx885_0:
                        .quad            .Lx885_0_s
.Lx885_0_s:
                        .string          "roman/2"
#-----------------------------------------------------------------------------------------------------------------------
n854_var_α:
                        mov              rax, qword ptr [rbp + 688]
                        mov              qword ptr [rbp + 368], rax
                        mov              rax, qword ptr [rbp + 696]
                        mov              qword ptr [rbp + 376], rax
                                                                                        jmp   n855_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n855_op11_α:
                        mov              rax, qword ptr [rbp + 368]
                        mov              qword ptr [rbp + 352], rax
                        mov              rax, qword ptr [rbp + 376]
                        mov              qword ptr [rbp + 360], rax
                        .section         .rodata
.Lrkfn889:              .string          "$write"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn889]
                        lea              rsi, [rbp + 352]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 336], rax
                        mov              qword ptr [rbp + 344], rdx
                        cmp              eax, 99
                                                                                        je    n853_call_proc_staged_β
                                                                                        jmp   n856_lit_string_α
n855_op11_β:
                                                                                        jmp   n853_call_proc_staged_β
#-----------------------------------------------------------------------------------------------------------------------
n856_lit_string_α:
                        mov              qword ptr [rbp + 320], 1
                        mov              rax, qword ptr [rip + .Lx890_0]
                        mov              qword ptr [rbp + 328], rax
                                                                                        jmp   n857_op11_α
.Lx890_0:
                        .quad            .Lx890_0_s
.Lx890_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n857_op11_α:
                        mov              rax, qword ptr [rbp + 320]
                        mov              qword ptr [rbp + 304], rax
                        mov              rax, qword ptr [rbp + 328]
                        mov              qword ptr [rbp + 312], rax
                        .section         .rodata
.Lrkfn892:              .string          "$nl0"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn892]
                        lea              rsi, [rbp + 304]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 288], rax
                        mov              qword ptr [rbp + 296], rdx
                        cmp              eax, 99
                                                                                        je    n853_call_proc_staged_β
                                                                                        jmp   n858_lit_integer_α
n857_op11_β:
                                                                                        jmp   n853_call_proc_staged_β
#-----------------------------------------------------------------------------------------------------------------------
n858_lit_integer_α:
                        mov              qword ptr [rbp + 256], 6
                        mov              rax, qword ptr [rip + .Lx893_0]
                        mov              qword ptr [rbp + 264], rax
                                                                                        jmp   n859_var_ref_α
.Lx893_0:
                        .quad            9
#-----------------------------------------------------------------------------------------------------------------------
n859_var_ref_α:
                        mov              rax, 4294967305
                        lea              rdx, [rbp + 672]
                        mov              qword ptr [rbp + 272], rax
                        mov              qword ptr [rbp + 280], rdx
                                                                                        jmp   n860_call_proc_staged_α
#-----------------------------------------------------------------------------------------------------------------------
n860_call_proc_staged_α:
                        lea              rsi, [rbp + 256]
                        lea              rdx, [rbp + 272]
                        call             proc_roman$2F2_dcα
                                                                                        jmp   .Lx897_2
.Lx897_2:
                        mov              qword ptr [rbp + 192], rax
                        mov              qword ptr [rbp + 200], rdx
                        cmp              eax, 99
                                                                                        je    n853_call_proc_staged_β
                                                                                        jmp   n861_var_α
n860_call_proc_staged_β:
                                                                                        jmp   n853_call_proc_staged_β
.Lx897_0:
                        .quad            .Lx897_0_s
.Lx897_0_s:
                        .string          "roman/2"
#-----------------------------------------------------------------------------------------------------------------------
n861_var_α:
                        mov              rax, qword ptr [rbp + 672]
                        mov              qword ptr [rbp + 176], rax
                        mov              rax, qword ptr [rbp + 680]
                        mov              qword ptr [rbp + 184], rax
                                                                                        jmp   n862_op11_α
#-----------------------------------------------------------------------------------------------------------------------
n862_op11_α:
                        mov              rax, qword ptr [rbp + 176]
                        mov              qword ptr [rbp + 160], rax
                        mov              rax, qword ptr [rbp + 184]
                        mov              qword ptr [rbp + 168], rax
                        .section         .rodata
.Lrkfn901:              .string          "$write"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn901]
                        lea              rsi, [rbp + 160]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 144], rax
                        mov              qword ptr [rbp + 152], rdx
                        cmp              eax, 99
                                                                                        je    n860_call_proc_staged_β
                                                                                        jmp   n863_lit_string_α
n862_op11_β:
                                                                                        jmp   n860_call_proc_staged_β
#-----------------------------------------------------------------------------------------------------------------------
n863_lit_string_α:
                        mov              qword ptr [rbp + 128], 1
                        mov              rax, qword ptr [rip + .Lx902_0]
                        mov              qword ptr [rbp + 136], rax
                                                                                        jmp   n864_op11_α
.Lx902_0:
                        .quad            .Lx902_0_s
.Lx902_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n864_op11_α:
                        mov              rax, qword ptr [rbp + 128]
                        mov              qword ptr [rbp + 112], rax
                        mov              rax, qword ptr [rbp + 136]
                        mov              qword ptr [rbp + 120], rax
                        .section         .rodata
.Lrkfn904:              .string          "$nl0"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn904]
                        lea              rsi, [rbp + 112]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 96], rax
                        mov              qword ptr [rbp + 104], rdx
                        cmp              eax, 99
                                                                                        je    n860_call_proc_staged_β
                                                                                        jmp   n865_move_label_α
n864_op11_β:
                                                                                        jmp   n860_call_proc_staged_β
#-----------------------------------------------------------------------------------------------------------------------
n865_move_label_α:
                        lea              rax, [rip + n860_call_proc_staged_β]
                        mov              qword ptr [rbp + 16], rax
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n866_disjunction_α:
                                                                                        jmp   qword ptr [rbp + 16]
n866_disjunction_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                                        jmp   n866_disjunction_α
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
                        mov              eax, 1
                        xor              edx, edx
                        mov              rsp, rbp
                        mov              rbp, [rsp + 720]
                        add              rsp, 728
                        ret
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              rsp, rbp
                        mov              eax, 99
                        xor              edx, edx
                        mov              rbp, [rsp + 720]
                        add              rsp, 728
                        ret
                        .section         .note.GNU-stack,"",@progbits
