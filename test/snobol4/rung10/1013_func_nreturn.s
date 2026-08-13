                        .intel_syntax    noprefix
                        .text
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__ref_a_α:
proc_LBL__ref_a_α_body:
#=======================================================================================================================
# ref_a   ref_a = .a                                     :(NRETURN)
#-----------------------------------------------------------------------------------------------------------------------
n0_statement_begin_α:                                               jmp   n1_lit_string_α
n0_statement_begin_β:                                               jmp   NRETURN
#-----------------------------------------------------------------------------------------------------------------------
n1_lit_string_α:        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 1
                        mov              rax, qword ptr [rip + .Lx7_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n2_call_α
.Lx7_0:                 .quad            .Lx7_0_s
.Lx7_0_s:               .string          "a"
#-----------------------------------------------------------------------------------------------------------------------
n2_call_α:              sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd9:              .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd9]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        add              rsp, 16
                        cmp              eax, 104;                  jne   .Lx8_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n0_statement_begin_β
.Lx8_240:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n3_assign_α
n2_call_β:              add              rsp, 16
                        add              rsp, 16;                   jmp   n0_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n3_assign_α:            mov              rax, qword ptr [rsp + 0]                       # call
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 0], rax                        # ref_a
                        mov              qword ptr [r9 + 8], rdx;   jmp   n4_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n4_statement_end_α:     add              rsp, 32;                   jmp   NRETURN
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__ref_a_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__ref_a_β:
                                                                    jmp   proc_LBL__ref_a_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__ref_a_γ:
                                                                    jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__ref_a_ω:
                                                                    jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_ref_a_α
proc_ref_a_α:
proc_ref_a_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n13_save_restore_α:                                                 jmp   n14_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n14_goto_deferred_α:    lea              rax, [rip + proc_LBL__ref_a_α]; jmp   rax
                                                                    jmp   n15_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n15_save_restore_α:
ref_a_alpha:            mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 0
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 0;                    jbe   .Lx21_2
                        mov              eax, 0
.Lx21_2:                sub              rsp, 48
                        add              rsp, rax
                        mov              rax, qword ptr [r9 + 0]                        # ref_a
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 0], 0
                        mov              qword ptr [r9 + 8], 0
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        mov              qword ptr [rsp + 32], rcx
                        lea              r10, [rip + ref_a_gamma]
                        lea              r11, [rip + ref_a_omega]
                        lea              rax, [rip + proc_LBL__ref_a_α]; jmp   rax
ref_a_gamma:            mov              rdi, qword ptr [r9 + 0]
                        mov              rsi, qword ptr [r9 + 8]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 0;                    jbe   .Lx21_75
                        mov              eax, 0
.Lx21_75:               lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 8], rax
                        mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 80
                        cmp              rcx, 0;                    jbe   .Lx21_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx21_3:                mov              rcx, qword ptr [r8 + 16]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi;                  jmp   rcx
ref_a_omega:            mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 0;                    jbe   .Lx21_145
                        mov              eax, 0
.Lx21_145:              lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 8], rax
                        mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 80
                        cmp              rcx, 0;                    jbe   .Lx21_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx21_4:                mov              rcx, qword ptr [r8 + 24]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx;                  jmp   rcx
                                                                    jmp   proc_ref_a_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_ref_a_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_ref_a_β:
                                                                    jmp   proc_ref_a_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_ref_a_γ:
                                                                    jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_ref_a_ω:
                                                                    jmp   r11
proc_startup:
                        sub              rsp, 8
                        .section         .rodata
.Lstartup_pname0:       .string          "LBL__ref_a"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname0]
                        lea              rsi, [rip + proc_LBL__ref_a_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 608
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        add              rsp, 8
                        ret
                        .section         .rodata
.Lgvan0:                .string          "ref_a"
.Lgvan1:                .string          "a"
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
                        mov              r12, qword ptr [0x70000000]
                        call             rtcc_load_all@PLT
                        xor              esi, esi
                                                                    jmp   main_α
#-----------------------------------------------------------------------------------------------------------------------
main_α:
main_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n22_statement_begin_α:                                              jmp   n23_statement_end_α
n22_statement_begin_β:                                              jmp   n24_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n23_statement_end_α:                                                jmp   n24_statement_begin_α
#=======================================================================================================================
#         DEFINE('ref_a()')                              :(ref_a_end)
#-----------------------------------------------------------------------------------------------------------------------
n24_statement_begin_α:                                              jmp   n25_func_activate_α
n24_statement_begin_β:                                              jmp   n27_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n25_func_activate_α:    mov              rdi, qword ptr [rip + .Lx89_0]
                        mov              rsi, qword ptr [rip + .Lx89_1]
                        mov              edx, 0
                        mov              ecx, 0
                        mov              r8d, 0
                        lea              r9, [rip + proc_ref_a_α]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_define_site@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n26_statement_end_α
n25_func_activate_β:                                                jmp   n24_statement_begin_β
.Lx89_0:                .quad            .Lx89_0_s
.Lx89_0_s:              .string          "ref_a"
.Lx89_1:                .quad            .Lx89_1_s
.Lx89_1_s:              .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n26_statement_end_α:                                                jmp   n27_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n27_statement_begin_α:                                              jmp   n28_statement_end_α
n27_statement_begin_β:                                              jmp   n29_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n28_statement_end_α:                                                jmp   n29_statement_begin_α
#=======================================================================================================================
#         a = 27
#-----------------------------------------------------------------------------------------------------------------------
n29_statement_begin_α:                                              jmp   n30_lit_integer_α
n29_statement_begin_β:                                              jmp   n33_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n30_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx98_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n31_assign_α
.Lx98_0:                .quad            27
#-----------------------------------------------------------------------------------------------------------------------
n31_assign_α:           mov              rax, qword ptr [rsp + 0]                       # lit_integer
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 16], rax                       # a
                        mov              qword ptr [r9 + 24], rdx;  jmp   n32_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n32_statement_end_α:    add              rsp, 16;                   jmp   n33_statement_begin_α
#=======================================================================================================================
#         a = 27
#-----------------------------------------------------------------------------------------------------------------------
n33_statement_begin_α:                                              jmp   n34_statement_end_α
n33_statement_begin_β:                                              jmp   n35_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n34_statement_end_α:                                                jmp   n35_statement_begin_α
#=======================================================================================================================
#         DIFFER(ref_a(), 27)                            :f(e001)
#-----------------------------------------------------------------------------------------------------------------------
n35_statement_begin_α:                                              jmp   n36_call_α
n35_statement_begin_β:                                              jmp   n44_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n36_call_α:             sub              rsp, 16
                        sub              rsp, 32
                        mov              eax, 0
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx109_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + ref_a_alpha];  jmp   rax
.Lx109_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx109_240
                        add              rsp, 16;                   jmp   n35_statement_begin_β
.Lx109_240:                                                         jmp   n37_lit_integer_α
n36_call_β:                                                         jmp   n35_statement_begin_β
.Lx109_0:               .quad            .Lx109_0_s
.Lx109_0_s:             .string          "ref_a"
#-----------------------------------------------------------------------------------------------------------------------
n37_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx110_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n38_call_α
n37_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                   jmp   n35_statement_begin_β
.Lx110_0:               .quad            27
#-----------------------------------------------------------------------------------------------------------------------
n38_call_α:             sub              rsp, 16
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
.Lrkfnzd112:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd112]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        add              rsp, 32
                        cmp              eax, 104;                  jne   .Lx111_240
                        add              rsp, 16;                   jmp   n37_lit_integer_β
.Lx111_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n39_statement_end_α
n38_call_β:             add              rsp, 16;                   jmp   n37_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n39_statement_end_α:    add              rsp, 48;                   jmp   n40_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1013/001: nreturn read gives value' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n40_statement_begin_α:                                              jmp   n41_lit_string_α
n40_statement_begin_β:                                              jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n41_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 39
                        mov              rax, qword ptr [rip + .Lx117_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n42_assign_α
.Lx117_0:               .quad            .Lx117_0_s
.Lx117_0_s:             .string          "FAIL 1013/001: nreturn read gives value"
#-----------------------------------------------------------------------------------------------------------------------
n42_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx118_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n43_statement_end_α
.Lx118_0:               .quad            .Lx118_0_s
.Lx118_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n43_statement_end_α:    add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n44_statement_begin_α:                                              jmp   n45_statement_end_α
n44_statement_begin_β:                                              jmp   n46_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n45_statement_end_α:                                                jmp   n46_statement_begin_α
#=======================================================================================================================
#         ref_a() = 26                                   :s(e002)
#-----------------------------------------------------------------------------------------------------------------------
n46_statement_begin_α:                                              jmp   n47_lit_string_α
n46_statement_begin_β:                                              jmp   n70_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n47_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx127_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n48_call_α
.Lx127_0:               .quad            .Lx127_0_s
.Lx127_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n48_call_α:             sub              rsp, 16
                        sub              rsp, 16
                        mov              r8, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], r8
                        mov              r8, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], r8
                        .section         .rodata
.Lrkfnzd129:            .string          "SNO$WANTNM"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd129]
                        lea              rsi, [rsp + 0]
                        mov              edx, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        add              rsp, 16
                        cmp              eax, 104;                  jne   .Lx128_240
                        add              rsp, 16
                        add              rsp, 16;                   jmp   n46_statement_begin_β
.Lx128_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n49_call_α
n48_call_β:             add              rsp, 16
                        add              rsp, 16;                   jmp   n46_statement_begin_β
#-----------------------------------------------------------------------------------------------------------------------
n49_call_α:             sub              rsp, 16
                        sub              rsp, 32
                        mov              eax, 0
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx131_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + ref_a_alpha];  jmp   rax
.Lx131_2:               mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx131_240
                        add              rsp, 16
                        add              rsp, 32;                   jmp   n46_statement_begin_β
.Lx131_240:                                                         jmp   n50_lit_integer_α
n49_call_β:                                                         jmp   n46_statement_begin_β
.Lx131_0:               .quad            .Lx131_0_s
.Lx131_0_s:             .string          "ref_a"
#-----------------------------------------------------------------------------------------------------------------------
n50_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx132_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n51_assign_var_α
n50_lit_integer_β:      add              rsp, 16
                        add              rsp, 48;                   jmp   n46_statement_begin_β
.Lx132_0:               .quad            26
#-----------------------------------------------------------------------------------------------------------------------
n51_assign_var_α:       sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]                      # call
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]                      # lit_integer
                        mov              rcx, qword ptr [rsp + 24]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_assign_var@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        cmp              eax, 104;                  jne   .Lx133_240
                        add              rsp, 16;                   jmp   n50_lit_integer_β
.Lx133_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n52_statement_end_α
#-----------------------------------------------------------------------------------------------------------------------
n52_statement_end_α:                                                jmp   n53_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n53_statement_begin_α:                                              jmp   n54_statement_end_α
n53_statement_begin_β:  add              rsp, 80;                   jmp   n55_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n54_statement_end_α:    add              rsp, 80;                   jmp   n55_statement_begin_α
#=======================================================================================================================
#         DIFFER(a, 26)                                  :f(e003)
#-----------------------------------------------------------------------------------------------------------------------
n55_statement_begin_α:                                              jmp   n56_var_α
n55_statement_begin_β:                                              jmp   n64_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n56_var_α:              sub              rsp, 16
                        mov              rax, qword ptr [r9 + 16]                       # a
                        mov              rdx, qword ptr [r9 + 24]
                        mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n57_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n57_lit_integer_α:      sub              rsp, 16
                        mov              qword ptr [rsp + 0], 3                         # result
                        mov              rax, qword ptr [rip + .Lx143_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n58_call_α
n57_lit_integer_β:      add              rsp, 16
                        add              rsp, 16;                   jmp   n55_statement_begin_β
.Lx143_0:               .quad            26
#-----------------------------------------------------------------------------------------------------------------------
n58_call_α:             sub              rsp, 16
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
.Lrkfnzd145:            .string          "DIFFER"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfnzd145]
                        lea              rsi, [rsp + 0]
                        mov              edx, 2
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]
                        add              rsp, 32
                        cmp              eax, 104;                  jne   .Lx144_240
                        add              rsp, 16;                   jmp   n57_lit_integer_β
.Lx144_240:             mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx;  jmp   n59_statement_end_α
n58_call_β:             add              rsp, 16;                   jmp   n57_lit_integer_β
#-----------------------------------------------------------------------------------------------------------------------
n59_statement_end_α:    add              rsp, 48;                   jmp   n60_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1013/003: a updated via nreturn'   :(END)
#-----------------------------------------------------------------------------------------------------------------------
n60_statement_begin_α:                                              jmp   n61_lit_string_α
n60_statement_begin_β:                                              jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n61_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 36
                        mov              rax, qword ptr [rip + .Lx150_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n62_assign_α
.Lx150_0:               .quad            .Lx150_0_s
.Lx150_0_s:             .string          "FAIL 1013/003: a updated via nreturn"
#-----------------------------------------------------------------------------------------------------------------------
n62_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx151_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n63_statement_end_α
.Lx151_0:               .quad            .Lx151_0_s
.Lx151_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n63_statement_end_α:    add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n64_statement_begin_α:                                              jmp   n65_statement_end_α
n64_statement_begin_β:                                              jmp   n66_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n65_statement_end_α:                                                jmp   n66_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'PASS 1013_func_nreturn (3/3)'
#-----------------------------------------------------------------------------------------------------------------------
n66_statement_begin_α:                                              jmp   n67_lit_string_α
n66_statement_begin_β:                                              jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n67_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 28
                        mov              rax, qword ptr [rip + .Lx160_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n68_assign_α
.Lx160_0:               .quad            .Lx160_0_s
.Lx160_0_s:             .string          "PASS 1013_func_nreturn (3/3)"
#-----------------------------------------------------------------------------------------------------------------------
n68_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx161_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n69_statement_end_α
.Lx161_0:               .quad            .Lx161_0_s
.Lx161_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n69_statement_end_α:    add              rsp, 16;                   jmp   main_γ
#=======================================================================================================================
#         OUTPUT = 'FAIL 1013/002: nreturn lvalue assign failed' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n70_statement_begin_α:                                              jmp   n71_lit_string_α
n70_statement_begin_β:                                              jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n71_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 43
                        mov              rax, qword ptr [rip + .Lx166_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n72_assign_α
.Lx166_0:               .quad            .Lx166_0_s
.Lx166_0_s:             .string          "FAIL 1013/002: nreturn lvalue assign failed"
#-----------------------------------------------------------------------------------------------------------------------
n72_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx167_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n73_statement_end_α
.Lx167_0:               .quad            .Lx167_0_s
.Lx167_0_s:             .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n73_statement_end_α:    add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n74_goto_α:                                                         jmp   n27_statement_begin_α
n74_goto_β:                                                         jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n75_goto_α:                                                         jmp   n44_statement_begin_α
n75_goto_β:                                                         jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n76_goto_α:                                                         jmp   n53_statement_begin_α
n76_goto_β:                                                         jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n77_goto_α:                                                         jmp   n64_statement_begin_α
n77_goto_β:                                                         jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
RETURN:                 lea              rdi, [rip + .S0]
                        call             rt_bomb@PLT
                        ud2
#-----------------------------------------------------------------------------------------------------------------------
FRETURN:                lea              rdi, [rip + .S1]
                        call             rt_bomb@PLT
                        ud2
#-----------------------------------------------------------------------------------------------------------------------
NRETURN:                mov              qword ptr [rsp + 32], 2                        # result
                        mov              dword ptr [rsp + 36], 0
                        mov              rax, qword ptr [rip + .Lx178_0]
                        mov              qword ptr [rsp + 40], rax; jmp   n81_call_α
.Lx178_0:               .quad            .Lx178_0_s
.Lx178_0_s:             .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n81_call_α:             mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 72], rax
                        .section         .rodata
.Lrkfn180:              .string          "SNO$NRET"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn180]
                        lea              rsi, [rsp + 64]
                        mov              edx, 1
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 48], rax
                        mov              qword ptr [rsp + 56], rdx
                        cmp              eax, 104;                  je    FRETURN
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   RETURN
n81_call_β:                                                         jmp   FRETURN
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
                        .section         .rodata
.S0:                    .string          "BOMB-RETURN: descent complete, coming-out frozen (s58 RSP-only) \342\200\224 UNKNOWN STACK DEPTH: the rsp-resident record cannot be found from here without a frame anchor"
.S1:                    .string          "BOMB-FRETURN: descent complete, coming-out frozen (s58 RSP-only) \342\200\224 UNKNOWN STACK DEPTH: the rsp-resident record cannot be found from here without a frame anchor"
                        .text
                        .section         .note.GNU-stack,"",@progbits
