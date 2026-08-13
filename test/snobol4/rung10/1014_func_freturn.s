                        .intel_syntax    noprefix
                        .text
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__always_fail_α:
proc_LBL__always_fail_α_body:
#=======================================================================================================================
# always_fail                                            :(FRETURN)
#-----------------------------------------------------------------------------------------------------------------------
n0_statement_begin_α:                                               jmp   n1_statement_end_α
n0_statement_begin_β:                                               jmp   FRETURN
#-----------------------------------------------------------------------------------------------------------------------
n1_statement_end_α:                                                 jmp   FRETURN
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__always_fail_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__always_fail_β:
                                                                    jmp   proc_LBL__always_fail_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__always_fail_γ:
                                                                    jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__always_fail_ω:
                                                                    jmp   r11
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_always_fail_α
proc_always_fail_α:
proc_always_fail_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n6_save_restore_α:                                                  jmp   n7_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n7_goto_deferred_α:     lea              rax, [rip + proc_LBL__always_fail_α]; jmp   rax
                                                                    jmp   n8_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n8_save_restore_α:
always_fail_alpha:      mov              rcx, qword ptr [rsp + 0]
                        mov              r8, rsp
                        sub              r8, 0
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 0;                    jbe   .Lx14_2
                        mov              eax, 0
.Lx14_2:                sub              rsp, 48
                        add              rsp, rax
                        mov              rax, qword ptr [r9 + 0]                        # always_fail
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [r9 + 8]
                        mov              qword ptr [rsp + 8], rax
                        mov              qword ptr [r9 + 0], 0
                        mov              qword ptr [r9 + 8], 0
                        mov              qword ptr [rsp + 16], r10
                        mov              qword ptr [rsp + 24], r11
                        mov              qword ptr [rsp + 32], rcx
                        lea              r10, [rip + always_fail_gamma]
                        lea              r11, [rip + always_fail_omega]
                        lea              rax, [rip + proc_LBL__always_fail_α]; jmp   rax
always_fail_gamma:      mov              rdi, qword ptr [r9 + 0]
                        mov              rsi, qword ptr [r9 + 8]
                        mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 0;                    jbe   .Lx14_75
                        mov              eax, 0
.Lx14_75:               lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 8], rax
                        mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 80
                        cmp              rcx, 0;                    jbe   .Lx14_3
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx14_3:                mov              rcx, qword ptr [r8 + 16]
                        add              rsp, rax
                        mov              rax, rdi
                        mov              rdx, rsi;                  jmp   rcx
always_fail_omega:      mov              rcx, qword ptr [rsp + 32]
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        cmp              rcx, 0;                    jbe   .Lx14_145
                        mov              eax, 0
.Lx14_145:              lea              r8, [rsp + 48]
                        sub              r8, rax
                        mov              rax, qword ptr [rsp + 0]
                        mov              qword ptr [r9 + 0], rax
                        mov              rax, qword ptr [rsp + 8]
                        mov              qword ptr [r9 + 8], rax
                        mov              r10, qword ptr [rsp + 16]
                        mov              r11, qword ptr [rsp + 24]
                        mov              eax, 80
                        cmp              rcx, 0;                    jbe   .Lx14_4
                        mov              rax, rcx
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, rax
                        add              rax, 80
.Lx14_4:                mov              rcx, qword ptr [r8 + 24]
                        add              rsp, rax
                        mov              eax, 104
                        xor              edx, edx;                  jmp   rcx
                                                                    jmp   proc_always_fail_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_always_fail_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_always_fail_β:
                                                                    jmp   proc_always_fail_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_always_fail_γ:
                                                                    jmp   r10
#-----------------------------------------------------------------------------------------------------------------------
proc_always_fail_ω:
                                                                    jmp   r11
proc_startup:
                        sub              rsp, 8
                        .section         .rodata
.Lstartup_pname0:       .string          "LBL__always_fail"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname0]
                        lea              rsi, [rip + proc_LBL__always_fail_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 0
                        call             rt_proc_set_nformals@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 112
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        add              rsp, 8
                        ret
                        .section         .rodata
.Lgvan0:                .string          "always_fail"
                        .align           8
__gva_names:
                        .quad            .Lgvan0
                        .section         .text
                        .intel_syntax    noprefix
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        call             proc_startup
                        mov              edi, 1
                        call             rt_gva_island@PLT
                        mov              rsi, rax
                        lea              rdi, [rip + __gva_names]
                        mov              edx, 1
                        call             gva_register@PLT
                        mov              r12, qword ptr [0x70000000]
                        call             rtcc_load_all@PLT
                        xor              esi, esi
                                                                    jmp   main_α
#-----------------------------------------------------------------------------------------------------------------------
main_α:
main_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n15_statement_begin_α:                                              jmp   n16_statement_end_α
n15_statement_begin_β:                                              jmp   n17_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n16_statement_end_α:                                                jmp   n17_statement_begin_α
#=======================================================================================================================
#         DEFINE('always_fail()')                        :(af_end)
#-----------------------------------------------------------------------------------------------------------------------
n17_statement_begin_α:                                              jmp   n18_func_activate_α
n17_statement_begin_β:                                              jmp   n20_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n18_func_activate_α:    mov              rdi, qword ptr [rip + .Lx45_0]
                        mov              rsi, qword ptr [rip + .Lx45_1]
                        mov              edx, 0
                        mov              ecx, 0
                        mov              r8d, 0
                        lea              r9, [rip + proc_always_fail_α]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             rt_define_site@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n19_statement_end_α
n18_func_activate_β:                                                jmp   n17_statement_begin_β
.Lx45_0:                .quad            .Lx45_0_s
.Lx45_0_s:              .string          "always_fail"
.Lx45_1:                .quad            .Lx45_1_s
.Lx45_1_s:              .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n19_statement_end_α:                                                jmp   n20_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n20_statement_begin_α:                                              jmp   n21_statement_end_α
n20_statement_begin_β:                                              jmp   n22_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n21_statement_end_α:                                                jmp   n22_statement_begin_α
#=======================================================================================================================
#         always_fail()                                  :f(e001)
#-----------------------------------------------------------------------------------------------------------------------
n22_statement_begin_α:                                              jmp   n23_call_α
n22_statement_begin_β:                                              jmp   n29_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n23_call_α:             sub              rsp, 16
                        sub              rsp, 32
                        mov              eax, 0
                        mov              qword ptr [rsp + 0], rax
                        lea              rax, [rip + .Lx55_2]
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rax
                        lea              rax, [rip + always_fail_alpha]; jmp   rax
.Lx55_2:                mov              qword ptr [rsp + 0], rax                       # result
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 104;                  jne   .Lx55_240
                        add              rsp, 16;                   jmp   n22_statement_begin_β
.Lx55_240:                                                          jmp   n24_statement_end_α
n23_call_β:                                                         jmp   n22_statement_begin_β
.Lx55_0:                .quad            .Lx55_0_s
.Lx55_0_s:              .string          "always_fail"
#-----------------------------------------------------------------------------------------------------------------------
n24_statement_end_α:    add              rsp, 16;                   jmp   n25_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'FAIL 1014/001: freturn should cause statement failure' :(END)
#-----------------------------------------------------------------------------------------------------------------------
n25_statement_begin_α:                                              jmp   n26_lit_string_α
n25_statement_begin_β:                                              jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n26_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 53
                        mov              rax, qword ptr [rip + .Lx60_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n27_assign_α
.Lx60_0:                .quad            .Lx60_0_s
.Lx60_0_s:              .string          "FAIL 1014/001: freturn should cause statement failure"
#-----------------------------------------------------------------------------------------------------------------------
n27_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx61_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n28_statement_end_α
.Lx61_0:                .quad            .Lx61_0_s
.Lx61_0_s:              .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n28_statement_end_α:    add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n29_statement_begin_α:                                              jmp   n30_statement_end_α
n29_statement_begin_β:                                              jmp   n31_statement_begin_α
#-----------------------------------------------------------------------------------------------------------------------
n30_statement_end_α:                                                jmp   n31_statement_begin_α
#=======================================================================================================================
#         OUTPUT = 'PASS 1014_func_freturn (1/1)'
#-----------------------------------------------------------------------------------------------------------------------
n31_statement_begin_α:                                              jmp   n32_lit_string_α
n31_statement_begin_β:                                              jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n32_lit_string_α:       sub              rsp, 16
                        mov              qword ptr [rsp + 0], 2                         # result
                        mov              dword ptr [rsp + 4], 28
                        mov              rax, qword ptr [rip + .Lx70_0]
                        mov              qword ptr [rsp + 8], rax;  jmp   n33_assign_α
.Lx70_0:                .quad            .Lx70_0_s
.Lx70_0_s:              .string          "PASS 1014_func_freturn (1/1)"
#-----------------------------------------------------------------------------------------------------------------------
n33_assign_α:           mov              rsi, qword ptr [rsp + 0]                       # lit_string
                        mov              rdx, qword ptr [rsp + 8]
                        mov              rdi, qword ptr [rip + .Lx71_0]
                        mov              rax, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              qword ptr [rax + 40], r8
                        mov              qword ptr [rax + 56], r10
                        mov              qword ptr [rax + 64], r11
                        call             NV_SET_fn@PLT
                        mov              r11, qword ptr [rip + g_rtcc_block@GOTPCREL]
                        mov              r8,   qword ptr [r11 + 40]
                        mov              r9,   qword ptr [r11 + 48]
                        mov              r10,  qword ptr [r11 + 56]
                        mov              r11,  qword ptr [r11 + 64]; jmp   n34_statement_end_α
.Lx71_0:                .quad            .Lx71_0_s
.Lx71_0_s:              .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n34_statement_end_α:    add              rsp, 16;                   jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n35_goto_α:                                                         jmp   n20_statement_begin_α
n35_goto_β:                                                         jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n36_goto_α:                                                         jmp   n29_statement_begin_α
n36_goto_β:                                                         jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
FRETURN:                lea              rdi, [rip + .S0]
                        call             rt_bomb@PLT
                        ud2
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
.S0:                    .string          "BOMB-FRETURN: descent complete, coming-out frozen (s58 RSP-only) \342\200\224 UNKNOWN STACK DEPTH: the rsp-resident record cannot be found from here without a frame anchor"
                        .text
                        .section         .note.GNU-stack,"",@progbits
