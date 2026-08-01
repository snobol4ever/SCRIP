                        .intel_syntax    noprefix
                        .text
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_LBL__fib_α
proc_LBL__fib_α:
proc_LBL__fib_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n0_goto_α:
                                                                                        jmp   n1_var_α
n0_goto_β:
                                                                                        jmp   proc_LBL__fib_ω
#=======================================================================================================================
# fib     LE(n, 1)                                                   :S(base)
#-----------------------------------------------------------------------------------------------------------------------
n1_var_α:
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 80], rax
                        mov              qword ptr [rsp + 88], rdx
                                                                                        jmp   n2_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n2_lit_integer_α:
                        mov              qword ptr [rsp + 64], 6
                        mov              rax, qword ptr [rip + .Lx21_0]
                        mov              qword ptr [rsp + 72], rax
                                                                                        jmp   n3_coerce_numeric_α
.Lx21_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n3_coerce_numeric_α:
                        mov              eax, dword ptr [rsp + 80]
                        cmp              eax, 7
                                                                                        je    .Lx23_1
                        cmp              eax, 6
                                                                                        jne   .Lx23_0
                        mov              eax, dword ptr [rsp + 64]
                        cmp              eax, 6
                                                                                        jne   .Lx23_0
.Lx23_1:
                        mov              rax, qword ptr [rsp + 80]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [rsp + 88]
                        mov              qword ptr [rsp + 56], rax
                                                                                        jmp   n4_coerce_numeric_α
.Lx23_0:
                        lea              rdi, [rsp + 80]
                        lea              rsi, [rsp + 64]
                        lea              rdx, [rsp + 48]
                        mov              rcx, 118
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n4_coerce_numeric_α
#-----------------------------------------------------------------------------------------------------------------------
n4_coerce_numeric_α:
                        mov              eax, dword ptr [rsp + 64]
                        cmp              eax, 7
                                                                                        je    .Lx25_1
                        cmp              eax, 6
                                                                                        jne   .Lx25_0
                        mov              eax, dword ptr [rsp + 80]
                        cmp              eax, 6
                                                                                        jne   .Lx25_0
.Lx25_1:
                        mov              rax, qword ptr [rsp + 64]
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 72]
                        mov              qword ptr [rsp + 40], rax
                                                                                        jmp   n5_cmp_test_α
.Lx25_0:
                        lea              rdi, [rsp + 64]
                        lea              rsi, [rsp + 80]
                        lea              rdx, [rsp + 32]
                        mov              rcx, 119
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n5_cmp_test_α
#-----------------------------------------------------------------------------------------------------------------------
n5_cmp_test_α:
                        lea              rdi, [rsp + 48]
                        lea              rsi, [rsp + 32]
                        call             rt_cmp_d@PLT
                        test             eax, eax
                                                                                        jg    n8_var_α
                                                                                        jmp   n6_var_α
#=======================================================================================================================
# base    fib = n                                                    :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n6_var_α:
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 304], rax
                        mov              qword ptr [rsp + 312], rdx
                                                                                        jmp   n7_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n7_assign_α:
                        mov              rax, qword ptr [rsp + 304]
                        mov              rdx, qword ptr [rsp + 312]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n18_save_restore_α
#=======================================================================================================================
#         fib = fib(n - 1) + fib(n - 2)                             :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n8_var_α:
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 176], rax
                        mov              qword ptr [rsp + 184], rdx
                                                                                        jmp   n9_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n9_lit_integer_α:
                        mov              qword ptr [rsp + 192], 6
                        mov              rax, qword ptr [rip + .Lx31_0]
                        mov              qword ptr [rsp + 200], rax
                                                                                        jmp   n10_binop_α
.Lx31_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n10_binop_α:
                        mov              eax, dword ptr [rsp + 176]
                        cmp              eax, 6
                                                                                        jne   .Lx32_0
                        mov              rax, qword ptr [rsp + 184]
                        mov              rcx, 1
                        sub              rax, rcx
                        mov              qword ptr [rsp + 160], 6
                        mov              qword ptr [rsp + 168], rax
                                                                                        jmp   n11_call_α
.Lx32_0:
                        mov              rdi, qword ptr [rsp + 176]
                        mov              rsi, qword ptr [rsp + 184]
                        mov              rdx, qword ptr [rsp + 192]
                        mov              rcx, qword ptr [rsp + 200]
                        call             rt_sub@PLT
                        cmp              eax, 99
                                                                                        je    n18_save_restore_α
                        mov              qword ptr [rsp + 160], rax
                        mov              qword ptr [rsp + 168], rdx
                                                                                        jmp   n11_call_α
#-----------------------------------------------------------------------------------------------------------------------
n11_call_α:
                        sub              rsp, 32
                        mov              rax, qword ptr [1879052304]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052288]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx34_0]
                        mov              esi, 1
                        mov              edx, 1
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx34_5
                        mov              rax, qword ptr [rsp + 192]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 200]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx34_6]
                        lea              rdx, [rip + .Lx34_7]
                                                                                        jmp   rax
.Lx34_6:
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
                                                                                        jmp   .Lx34_2
.Lx34_7:
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
                                                                                        jmp   .Lx34_2
.Lx34_5:
                        add              rsp, 32
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx34_20
                        mov              rax, qword ptr [rsp + 160]
                        mov              rdx, qword ptr [rsp + 168]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx34_21
.Lx34_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 160]
                        mov              rdx, qword ptr [rsp + 168]
                        call             rt_arg_stage@PLT
.Lx34_21:
                        mov              rdi, qword ptr [rip + .Lx34_0]
                        mov              esi, 1
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx34_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx34_3]
                        lea              rdx, [rip + .Lx34_4]
                                                                                        jmp   rax
.Lx34_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx34_2
.Lx34_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx34_2
.Lx34_1:
                        call             rt_faildescr@PLT
.Lx34_2:
                        mov              qword ptr [rsp + 112], rax
                        mov              qword ptr [rsp + 120], rdx
                        cmp              eax, 99
                                                                                        je    n18_save_restore_α
                                                                                        jmp   n12_var_α
n11_call_β:
                                                                                        jmp   n18_save_restore_α
.Lx34_0:
                        .quad            .Lx34_0_s
.Lx34_0_s:
                        .string          "fib"
#-----------------------------------------------------------------------------------------------------------------------
n12_var_α:
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 272], rax
                        mov              qword ptr [rsp + 280], rdx
                                                                                        jmp   n13_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n13_lit_integer_α:
                        mov              qword ptr [rsp + 288], 6
                        mov              rax, qword ptr [rip + .Lx36_0]
                        mov              qword ptr [rsp + 296], rax
                                                                                        jmp   n14_binop_α
.Lx36_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n14_binop_α:
                        mov              eax, dword ptr [rsp + 272]
                        cmp              eax, 6
                                                                                        jne   .Lx37_0
                        mov              rax, qword ptr [rsp + 280]
                        mov              rcx, 2
                        sub              rax, rcx
                        mov              qword ptr [rsp + 256], 6
                        mov              qword ptr [rsp + 264], rax
                                                                                        jmp   n15_call_α
.Lx37_0:
                        mov              rdi, qword ptr [rsp + 272]
                        mov              rsi, qword ptr [rsp + 280]
                        mov              rdx, qword ptr [rsp + 288]
                        mov              rcx, qword ptr [rsp + 296]
                        call             rt_sub@PLT
                        cmp              eax, 99
                                                                                        je    n18_save_restore_α
                        mov              qword ptr [rsp + 256], rax
                        mov              qword ptr [rsp + 264], rdx
                                                                                        jmp   n15_call_α
#-----------------------------------------------------------------------------------------------------------------------
n15_call_α:
                        sub              rsp, 32
                        mov              rax, qword ptr [1879052304]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052288]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx39_0]
                        mov              esi, 1
                        mov              edx, 1
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx39_5
                        mov              rax, qword ptr [rsp + 288]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 296]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx39_6]
                        lea              rdx, [rip + .Lx39_7]
                                                                                        jmp   rax
.Lx39_6:
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
                                                                                        jmp   .Lx39_2
.Lx39_7:
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
                                                                                        jmp   .Lx39_2
.Lx39_5:
                        add              rsp, 32
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx39_20
                        mov              rax, qword ptr [rsp + 256]
                        mov              rdx, qword ptr [rsp + 264]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx39_21
.Lx39_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 256]
                        mov              rdx, qword ptr [rsp + 264]
                        call             rt_arg_stage@PLT
.Lx39_21:
                        mov              rdi, qword ptr [rip + .Lx39_0]
                        mov              esi, 1
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx39_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx39_3]
                        lea              rdx, [rip + .Lx39_4]
                                                                                        jmp   rax
.Lx39_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx39_2
.Lx39_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx39_2
.Lx39_1:
                        call             rt_faildescr@PLT
.Lx39_2:
                        mov              qword ptr [rsp + 208], rax
                        mov              qword ptr [rsp + 216], rdx
                        cmp              eax, 99
                                                                                        je    n18_save_restore_α
                                                                                        jmp   n16_binop_α
n15_call_β:
                                                                                        jmp   n18_save_restore_α
.Lx39_0:
                        .quad            .Lx39_0_s
.Lx39_0_s:
                        .string          "fib"
#-----------------------------------------------------------------------------------------------------------------------
n16_binop_α:
                        mov              eax, dword ptr [rsp + 112]
                        cmp              eax, 6
                                                                                        jne   .Lx40_0
                        mov              eax, dword ptr [rsp + 208]
                        cmp              eax, 6
                                                                                        jne   .Lx40_0
                        mov              rax, qword ptr [rsp + 120]
                        mov              rcx, qword ptr [rsp + 216]
                        add              rax, rcx
                        mov              qword ptr [rsp + 96], 6
                        mov              qword ptr [rsp + 104], rax
                                                                                        jmp   n17_assign_α
.Lx40_0:
                        mov              rdi, qword ptr [rsp + 112]
                        mov              rsi, qword ptr [rsp + 120]
                        mov              rdx, qword ptr [rsp + 208]
                        mov              rcx, qword ptr [rsp + 216]
                        call             rt_add@PLT
                        cmp              eax, 99
                                                                                        je    n18_save_restore_α
                        mov              qword ptr [rsp + 96], rax
                        mov              qword ptr [rsp + 104], rdx
                                                                                        jmp   n17_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n17_assign_α:
                        mov              rax, qword ptr [rsp + 96]
                        mov              rdx, qword ptr [rsp + 104]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n18_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n18_save_restore_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__fib_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__fib_β:
                                                                                        jmp   proc_LBL__fib_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__fib_γ:
                        mov              rsp, rbp
                        pop              rbp
                        xor              edi, edi
                        call             exit@PLT
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__fib_ω:
                        mov              rsp, rbp
                        pop              rbp
                        mov              edi, 1
                        call             exit@PLT
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_fib_α
proc_fib_α:
proc_fib_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n44_save_restore_α:
                        mov              rdi, rcx
                        mov              rsi, rdx
                        lea              rdx, [rsp + 0]
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
                        .string          "fib"
.Lx49_1:
                                                                                        jmp   proc_fib_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_fib_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_fib_β:
                                                                                        jmp   proc_fib_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_fib_γ:
                        mov              rsp, rbp
                        pop              rbp
                        xor              edi, edi
                        call             exit@PLT
#-----------------------------------------------------------------------------------------------------------------------
proc_fib_ω:
                        mov              rsp, rbp
                        pop              rbp
                        mov              edi, 1
                        call             exit@PLT
proc_startup:
                        sub              rsp, 8
                        .section         .rodata
.Lstartup_pname0:       .string          "LBL__fib"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname0]
                        lea              rsi, [rip + proc_LBL__fib_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 576
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname1:       .string          "fib"
.Lstartup_pp1_0:        .string          "n"
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
                        lea              rsi, [rip + proc_fib_α]
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
.Lgvan0:                .string          "fib"
.Lgvan1:                .string          "n"
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
                                                                                        jmp   main_α
#-----------------------------------------------------------------------------------------------------------------------
main_α:
main_α_body:
                        push             rbp
                        mov              rbp, rsp
                        sub              rsp, 8
#=======================================================================================================================
#         DEFINE('fib(n)')                                            :(fib_end)
#         OUTPUT = fib(0)
#-----------------------------------------------------------------------------------------------------------------------
n50_lit_integer_α:
                        sub              rsp, 368
                        mov              qword ptr [rsp + 352], 6
                        mov              rax, qword ptr [rip + .Lx83_0]
                        mov              qword ptr [rsp + 360], rax
                                                                                        jmp   n51_call_α
.Lx83_0:
                        .quad            0
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
                        mov              rdi, qword ptr [rip + .Lx85_0]
                        mov              esi, 1
                        mov              edx, 1
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx85_5
                        mov              rax, qword ptr [rsp + 400]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 408]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx85_6]
                        lea              rdx, [rip + .Lx85_7]
                                                                                        jmp   rax
.Lx85_6:
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
                                                                                        jmp   .Lx85_2
.Lx85_7:
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
                                                                                        jmp   .Lx85_2
.Lx85_5:
                        add              rsp, 32
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx85_20
                        mov              rax, qword ptr [rsp + 352]
                        mov              rdx, qword ptr [rsp + 360]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx85_21
.Lx85_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 352]
                        mov              rdx, qword ptr [rsp + 360]
                        call             rt_arg_stage@PLT
.Lx85_21:
                        mov              rdi, qword ptr [rip + .Lx85_0]
                        mov              esi, 1
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx85_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx85_3]
                        lea              rdx, [rip + .Lx85_4]
                                                                                        jmp   rax
.Lx85_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx85_2
.Lx85_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx85_2
.Lx85_1:
                        call             rt_faildescr@PLT
.Lx85_2:
                        mov              qword ptr [rsp + 304], rax
                        mov              qword ptr [rsp + 312], rdx
                        cmp              eax, 99
                                                                                        jne   .Lx85_240
                        add              rsp, 368
                                                                                        jmp   n53_lit_integer_α
.Lx85_240:
                                                                                        jmp   n52_assign_α
n51_call_β:
                                                                                        jmp   n53_lit_integer_α
.Lx85_0:
                        .quad            .Lx85_0_s
.Lx85_0_s:
                        .string          "fib"
#-----------------------------------------------------------------------------------------------------------------------
n52_assign_α:
                        mov              rsi, qword ptr [rsp + 304]
                        mov              rdx, qword ptr [rsp + 312]
                        mov              rdi, qword ptr [rip + .Lx86_0]
                        call             NV_SET_fn@PLT
                        add              rsp, 368
                                                                                        jmp   n53_lit_integer_α
.Lx86_0:
                        .quad            .Lx86_0_s
.Lx86_0_s:
                        .string          "OUTPUT"
#=======================================================================================================================
#         OUTPUT = fib(1)
#-----------------------------------------------------------------------------------------------------------------------
n53_lit_integer_α:
                        sub              rsp, 432
                        mov              qword ptr [rsp + 416], 6
                        mov              rax, qword ptr [rip + .Lx87_0]
                        mov              qword ptr [rsp + 424], rax
                                                                                        jmp   n54_call_α
.Lx87_0:
                        .quad            1
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
                        mov              rdi, qword ptr [rip + .Lx89_0]
                        mov              esi, 1
                        mov              edx, 1
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx89_5
                        mov              rax, qword ptr [rsp + 464]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 472]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx89_6]
                        lea              rdx, [rip + .Lx89_7]
                                                                                        jmp   rax
.Lx89_6:
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
                                                                                        jmp   .Lx89_2
.Lx89_7:
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
                                                                                        jmp   .Lx89_2
.Lx89_5:
                        add              rsp, 32
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx89_20
                        mov              rax, qword ptr [rsp + 416]
                        mov              rdx, qword ptr [rsp + 424]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx89_21
.Lx89_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 416]
                        mov              rdx, qword ptr [rsp + 424]
                        call             rt_arg_stage@PLT
.Lx89_21:
                        mov              rdi, qword ptr [rip + .Lx89_0]
                        mov              esi, 1
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx89_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx89_3]
                        lea              rdx, [rip + .Lx89_4]
                                                                                        jmp   rax
.Lx89_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx89_2
.Lx89_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx89_2
.Lx89_1:
                        call             rt_faildescr@PLT
.Lx89_2:
                        mov              qword ptr [rsp + 368], rax
                        mov              qword ptr [rsp + 376], rdx
                        cmp              eax, 99
                                                                                        jne   .Lx89_240
                        add              rsp, 432
                                                                                        jmp   n56_lit_integer_α
.Lx89_240:
                                                                                        jmp   n55_assign_α
n54_call_β:
                                                                                        jmp   n56_lit_integer_α
.Lx89_0:
                        .quad            .Lx89_0_s
.Lx89_0_s:
                        .string          "fib"
#-----------------------------------------------------------------------------------------------------------------------
n55_assign_α:
                        mov              rsi, qword ptr [rsp + 368]
                        mov              rdx, qword ptr [rsp + 376]
                        mov              rdi, qword ptr [rip + .Lx90_0]
                        call             NV_SET_fn@PLT
                        add              rsp, 432
                                                                                        jmp   n56_lit_integer_α
.Lx90_0:
                        .quad            .Lx90_0_s
.Lx90_0_s:
                        .string          "OUTPUT"
#=======================================================================================================================
#         OUTPUT = fib(6)
#-----------------------------------------------------------------------------------------------------------------------
n56_lit_integer_α:
                        sub              rsp, 496
                        mov              qword ptr [rsp + 480], 6
                        mov              rax, qword ptr [rip + .Lx91_0]
                        mov              qword ptr [rsp + 488], rax
                                                                                        jmp   n57_call_α
.Lx91_0:
                        .quad            6
#-----------------------------------------------------------------------------------------------------------------------
n57_call_α:
                        sub              rsp, 32
                        mov              rax, qword ptr [1879052304]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052288]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx93_0]
                        mov              esi, 1
                        mov              edx, 1
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx93_5
                        mov              rax, qword ptr [rsp + 528]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 536]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx93_6]
                        lea              rdx, [rip + .Lx93_7]
                                                                                        jmp   rax
.Lx93_6:
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
                                                                                        jmp   .Lx93_2
.Lx93_7:
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
                                                                                        jmp   .Lx93_2
.Lx93_5:
                        add              rsp, 32
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx93_20
                        mov              rax, qword ptr [rsp + 480]
                        mov              rdx, qword ptr [rsp + 488]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx93_21
.Lx93_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 480]
                        mov              rdx, qword ptr [rsp + 488]
                        call             rt_arg_stage@PLT
.Lx93_21:
                        mov              rdi, qword ptr [rip + .Lx93_0]
                        mov              esi, 1
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx93_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx93_3]
                        lea              rdx, [rip + .Lx93_4]
                                                                                        jmp   rax
.Lx93_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx93_2
.Lx93_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx93_2
.Lx93_1:
                        call             rt_faildescr@PLT
.Lx93_2:
                        mov              qword ptr [rsp + 432], rax
                        mov              qword ptr [rsp + 440], rdx
                        cmp              eax, 99
                                                                                        jne   .Lx93_240
                        add              rsp, 496
                                                                                        jmp   n59_lit_integer_α
.Lx93_240:
                                                                                        jmp   n58_assign_α
n57_call_β:
                                                                                        jmp   n59_lit_integer_α
.Lx93_0:
                        .quad            .Lx93_0_s
.Lx93_0_s:
                        .string          "fib"
#-----------------------------------------------------------------------------------------------------------------------
n58_assign_α:
                        mov              rsi, qword ptr [rsp + 432]
                        mov              rdx, qword ptr [rsp + 440]
                        mov              rdi, qword ptr [rip + .Lx94_0]
                        call             NV_SET_fn@PLT
                        add              rsp, 496
                                                                                        jmp   n59_lit_integer_α
.Lx94_0:
                        .quad            .Lx94_0_s
.Lx94_0_s:
                        .string          "OUTPUT"
#=======================================================================================================================
#         OUTPUT = fib(10)
#-----------------------------------------------------------------------------------------------------------------------
n59_lit_integer_α:
                        sub              rsp, 560
                        mov              qword ptr [rsp + 544], 6
                        mov              rax, qword ptr [rip + .Lx95_0]
                        mov              qword ptr [rsp + 552], rax
                                                                                        jmp   n60_call_α
.Lx95_0:
                        .quad            10
#-----------------------------------------------------------------------------------------------------------------------
n60_call_α:
                        sub              rsp, 32
                        mov              rax, qword ptr [1879052304]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052288]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx97_0]
                        mov              esi, 1
                        mov              edx, 1
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx97_5
                        mov              rax, qword ptr [rsp + 592]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 600]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx97_6]
                        lea              rdx, [rip + .Lx97_7]
                                                                                        jmp   rax
.Lx97_6:
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
                                                                                        jmp   .Lx97_2
.Lx97_7:
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
                                                                                        jmp   .Lx97_2
.Lx97_5:
                        add              rsp, 32
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx97_20
                        mov              rax, qword ptr [rsp + 544]
                        mov              rdx, qword ptr [rsp + 552]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx97_21
.Lx97_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 544]
                        mov              rdx, qword ptr [rsp + 552]
                        call             rt_arg_stage@PLT
.Lx97_21:
                        mov              rdi, qword ptr [rip + .Lx97_0]
                        mov              esi, 1
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx97_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx97_3]
                        lea              rdx, [rip + .Lx97_4]
                                                                                        jmp   rax
.Lx97_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx97_2
.Lx97_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx97_2
.Lx97_1:
                        call             rt_faildescr@PLT
.Lx97_2:
                        mov              qword ptr [rsp + 496], rax
                        mov              qword ptr [rsp + 504], rdx
                        cmp              eax, 99
                                                                                        jne   .Lx97_240
                        add              rsp, 560
                                                                                        jmp   main_γ
.Lx97_240:
                                                                                        jmp   n61_assign_α
n60_call_β:
                                                                                        jmp   main_γ
.Lx97_0:
                        .quad            .Lx97_0_s
.Lx97_0_s:
                        .string          "fib"
#-----------------------------------------------------------------------------------------------------------------------
n61_assign_α:
                        mov              rsi, qword ptr [rsp + 496]
                        mov              rdx, qword ptr [rsp + 504]
                        mov              rdi, qword ptr [rip + .Lx98_0]
                        call             NV_SET_fn@PLT
                        add              rsp, 560
                                                                                        jmp   main_γ
.Lx98_0:
                        .quad            .Lx98_0_s
.Lx98_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n62_goto_α:
                                                                                        jmp   n63_var_α
n62_goto_β:
                                                                                        jmp   main_ω
#=======================================================================================================================
# fib     LE(n, 1)                                                   :S(base)
#-----------------------------------------------------------------------------------------------------------------------
n63_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n64_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n64_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx101_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n65_coerce_numeric_α
.Lx101_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n65_coerce_numeric_α:
                        sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]
                        cmp              eax, 7
                                                                                        je    .Lx103_1
                        cmp              eax, 6
                                                                                        jne   .Lx103_0
                        mov              eax, dword ptr [rsp + 16]
                        cmp              eax, 6
                                                                                        jne   .Lx103_0
.Lx103_1:
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n66_coerce_numeric_α
.Lx103_0:
                        lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]
                        lea              rdx, [rsp + 0]
                        mov              rcx, 118
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n66_coerce_numeric_α
#-----------------------------------------------------------------------------------------------------------------------
n66_coerce_numeric_α:
                        sub              rsp, 16
                        mov              eax, dword ptr [rsp + 32]
                        cmp              eax, 7
                                                                                        je    .Lx105_1
                        cmp              eax, 6
                                                                                        jne   .Lx105_0
                        mov              eax, dword ptr [rsp + 48]
                        cmp              eax, 6
                                                                                        jne   .Lx105_0
.Lx105_1:
                        mov              rax, qword ptr [rsp + 32]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [rsp + 40]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n67_cmp_test_α
.Lx105_0:
                        lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 48]
                        lea              rdx, [rsp + 0]
                        mov              rcx, 119
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n67_cmp_test_α
#-----------------------------------------------------------------------------------------------------------------------
n67_cmp_test_α:
                        sub              rsp, 16
                        lea              rdi, [rsp + 32]
                        lea              rsi, [rsp + 16]
                        call             rt_cmp_d@PLT
                        test             eax, eax
                                                                                        jle   .Lx107_240
                        add              rsp, 16
                        add              rsp, 64
                                                                                        jmp   n70_var_α
.Lx107_240:
                        add              rsp, 80
                                                                                        jmp   n68_var_α
#=======================================================================================================================
# base    fib = n                                                    :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n68_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n69_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n69_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n80_save_restore_α
#=======================================================================================================================
#         fib = fib(n - 1) + fib(n - 2)                             :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n70_var_α:
                        sub              rsp, 288
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 160], rax
                        mov              qword ptr [rsp + 168], rdx
                                                                                        jmp   n71_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n71_lit_integer_α:
                        mov              qword ptr [rsp + 176], 6
                        mov              rax, qword ptr [rip + .Lx111_0]
                        mov              qword ptr [rsp + 184], rax
                                                                                        jmp   n72_binop_α
.Lx111_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n72_binop_α:
                        mov              eax, dword ptr [rsp + 160]
                        cmp              eax, 6
                                                                                        jne   .Lx112_0
                        mov              rax, qword ptr [rsp + 168]
                        mov              rcx, 1
                        sub              rax, rcx
                        mov              qword ptr [rsp + 144], 6
                        mov              qword ptr [rsp + 152], rax
                                                                                        jmp   n73_call_α
.Lx112_0:
                        mov              rdi, qword ptr [rsp + 160]
                        mov              rsi, qword ptr [rsp + 168]
                        mov              rdx, qword ptr [rsp + 176]
                        mov              rcx, qword ptr [rsp + 184]
                        call             rt_sub@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx112_240
                        add              rsp, 288
                                                                                        jmp   n80_save_restore_α
.Lx112_240:
                        mov              qword ptr [rsp + 144], rax
                        mov              qword ptr [rsp + 152], rdx
                                                                                        jmp   n73_call_α
#-----------------------------------------------------------------------------------------------------------------------
n73_call_α:
                        sub              rsp, 32
                        mov              rax, qword ptr [1879052304]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052288]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx114_0]
                        mov              esi, 1
                        mov              edx, 1
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx114_5
                        mov              rax, qword ptr [rsp + 176]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 184]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx114_6]
                        lea              rdx, [rip + .Lx114_7]
                                                                                        jmp   rax
.Lx114_6:
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
                                                                                        jmp   .Lx114_2
.Lx114_7:
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
                                                                                        jmp   .Lx114_2
.Lx114_5:
                        add              rsp, 32
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx114_20
                        mov              rax, qword ptr [rsp + 144]
                        mov              rdx, qword ptr [rsp + 152]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx114_21
.Lx114_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 144]
                        mov              rdx, qword ptr [rsp + 152]
                        call             rt_arg_stage@PLT
.Lx114_21:
                        mov              rdi, qword ptr [rip + .Lx114_0]
                        mov              esi, 1
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx114_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx114_3]
                        lea              rdx, [rip + .Lx114_4]
                                                                                        jmp   rax
.Lx114_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx114_2
.Lx114_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx114_2
.Lx114_1:
                        call             rt_faildescr@PLT
.Lx114_2:
                        mov              qword ptr [rsp + 96], rax
                        mov              qword ptr [rsp + 104], rdx
                        cmp              eax, 99
                                                                                        jne   .Lx114_240
                        add              rsp, 288
                                                                                        jmp   n80_save_restore_α
.Lx114_240:
                                                                                        jmp   n74_var_α
n73_call_β:
                                                                                        jmp   n80_save_restore_α
.Lx114_0:
                        .quad            .Lx114_0_s
.Lx114_0_s:
                        .string          "fib"
#-----------------------------------------------------------------------------------------------------------------------
n74_var_α:
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 256], rax
                        mov              qword ptr [rsp + 264], rdx
                                                                                        jmp   n75_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n75_lit_integer_α:
                        mov              qword ptr [rsp + 272], 6
                        mov              rax, qword ptr [rip + .Lx116_0]
                        mov              qword ptr [rsp + 280], rax
                                                                                        jmp   n76_binop_α
.Lx116_0:
                        .quad            2
#-----------------------------------------------------------------------------------------------------------------------
n76_binop_α:
                        mov              eax, dword ptr [rsp + 256]
                        cmp              eax, 6
                                                                                        jne   .Lx117_0
                        mov              rax, qword ptr [rsp + 264]
                        mov              rcx, 2
                        sub              rax, rcx
                        mov              qword ptr [rsp + 240], 6
                        mov              qword ptr [rsp + 248], rax
                                                                                        jmp   n77_call_α
.Lx117_0:
                        mov              rdi, qword ptr [rsp + 256]
                        mov              rsi, qword ptr [rsp + 264]
                        mov              rdx, qword ptr [rsp + 272]
                        mov              rcx, qword ptr [rsp + 280]
                        call             rt_sub@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx117_240
                        add              rsp, 288
                                                                                        jmp   n80_save_restore_α
.Lx117_240:
                        mov              qword ptr [rsp + 240], rax
                        mov              qword ptr [rsp + 248], rdx
                                                                                        jmp   n77_call_α
#-----------------------------------------------------------------------------------------------------------------------
n77_call_α:
                        sub              rsp, 32
                        mov              rax, qword ptr [1879052304]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052288]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx119_0]
                        mov              esi, 1
                        mov              edx, 1
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx119_5
                        mov              rax, qword ptr [rsp + 272]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 280]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx119_6]
                        lea              rdx, [rip + .Lx119_7]
                                                                                        jmp   rax
.Lx119_6:
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
                                                                                        jmp   .Lx119_2
.Lx119_7:
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
                                                                                        jmp   .Lx119_2
.Lx119_5:
                        add              rsp, 32
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx119_20
                        mov              rax, qword ptr [rsp + 240]
                        mov              rdx, qword ptr [rsp + 248]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx119_21
.Lx119_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 240]
                        mov              rdx, qword ptr [rsp + 248]
                        call             rt_arg_stage@PLT
.Lx119_21:
                        mov              rdi, qword ptr [rip + .Lx119_0]
                        mov              esi, 1
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx119_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx119_3]
                        lea              rdx, [rip + .Lx119_4]
                                                                                        jmp   rax
.Lx119_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx119_2
.Lx119_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx119_2
.Lx119_1:
                        call             rt_faildescr@PLT
.Lx119_2:
                        mov              qword ptr [rsp + 192], rax
                        mov              qword ptr [rsp + 200], rdx
                        cmp              eax, 99
                                                                                        jne   .Lx119_240
                        add              rsp, 288
                                                                                        jmp   n80_save_restore_α
.Lx119_240:
                                                                                        jmp   n78_binop_α
n77_call_β:
                                                                                        jmp   n80_save_restore_α
.Lx119_0:
                        .quad            .Lx119_0_s
.Lx119_0_s:
                        .string          "fib"
#-----------------------------------------------------------------------------------------------------------------------
n78_binop_α:
                        mov              eax, dword ptr [rsp + 96]
                        cmp              eax, 6
                                                                                        jne   .Lx120_0
                        mov              eax, dword ptr [rsp + 192]
                        cmp              eax, 6
                                                                                        jne   .Lx120_0
                        mov              rax, qword ptr [rsp + 104]
                        mov              rcx, qword ptr [rsp + 200]
                        add              rax, rcx
                        mov              qword ptr [rsp + 80], 6
                        mov              qword ptr [rsp + 88], rax
                                                                                        jmp   n79_assign_α
.Lx120_0:
                        mov              rdi, qword ptr [rsp + 96]
                        mov              rsi, qword ptr [rsp + 104]
                        mov              rdx, qword ptr [rsp + 192]
                        mov              rcx, qword ptr [rsp + 200]
                        call             rt_add@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx120_240
                        add              rsp, 288
                                                                                        jmp   n80_save_restore_α
.Lx120_240:
                        mov              qword ptr [rsp + 80], rax
                        mov              qword ptr [rsp + 88], rdx
                                                                                        jmp   n79_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n79_assign_α:
                        mov              rax, qword ptr [rsp + 80]
                        mov              rdx, qword ptr [rsp + 88]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        add              rsp, 288
                                                                                        jmp   n80_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n80_save_restore_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n81_goto_α:
                                                                                        jmp   n68_var_α
n81_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n82_goto_α:
                                                                                        jmp   n50_lit_integer_α
n82_goto_β:
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
                        .section         .note.GNU-stack,"",@progbits
