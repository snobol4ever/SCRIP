                        .intel_syntax    noprefix
                        .text
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_LBL__bumpit_α
proc_LBL__bumpit_α:
                        .global          proc_LBL__bumpit_α
                        .global          proc_LBL__bumpit_β
                        .global          proc_LBL__bumpit_γ
                        .global          proc_LBL__bumpit_ω
                        sub              rsp, 192
                        mov              [rsp + 168], rcx
                        mov              [rsp + 176], rdx
                        mov              qword ptr [rsp], 0
                        mov              qword ptr [rsp + 8], 0
                        mov              qword ptr [rsp + 144], 0
                        mov              qword ptr [rsp + 152], 0
proc_LBL__bumpit_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n0_goto_α:
                                                                                        jmp   n1_var_α
n0_goto_β:
                                                                                        jmp   proc_LBL__bumpit_ω
#=======================================================================================================================
# bumpit  bumpit = v + 1                                             :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n1_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n2_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n2_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx8_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n3_binop_α
.Lx8_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n3_binop_α:
                        mov              eax, dword ptr [rsp + 16]
                        cmp              eax, 6
                                                                                        jne   .Lx9_0
                        mov              eax, dword ptr [rsp + 0]
                        cmp              eax, 6
                                                                                        jne   .Lx9_0
                        mov              rax, qword ptr [rsp + 24]
                        mov              rcx, qword ptr [rsp + 8]
                        add              rax, rcx
                        add              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n4_assign_α
.Lx9_0:
                        mov              rdi, qword ptr [rsp + 16]
                        mov              rsi, qword ptr [rsp + 24]
                        mov              rdx, qword ptr [rsp + 0]
                        mov              rcx, qword ptr [rsp + 8]
                        call             rt_add@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx9_240
                        add              rsp, 32
                                                                                        jmp   n5_save_restore_α
.Lx9_240:
                        add              rsp, 16
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n4_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n4_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n5_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n5_save_restore_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__bumpit_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__bumpit_β:
                                                                                        jmp   proc_LBL__bumpit_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__bumpit_γ:
                        mov              rdi, [rsp]
                        mov              rsi, [rsp + 8]
                        mov              rax, [rsp + 168]
                        add              rsp, 192
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__bumpit_ω:
                        mov              rax, [rsp + 176]
                        add              rsp, 192
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_bumpit_α
proc_bumpit_α:
                        .global          proc_bumpit_α
                        .global          proc_bumpit_β
                        .global          proc_bumpit_γ
                        .global          proc_bumpit_ω
                        sub              rsp, 64
                        mov              [rsp + 40], rcx
                        mov              [rsp + 48], rdx
                        mov              rdi, rsp
                        mov              ecx, 32
                        xor              eax, eax
                        rep stosb
proc_bumpit_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n13_save_restore_α:
                        mov              rdi, qword ptr [rsp + 40]
                        mov              rsi, qword ptr [rsp + 48]
                        lea              rdx, [rsp + 64]
                        mov              rcx, rbp
                        call             rt_flat_wire_adopt@PLT
                                                                                        jmp   n14_goto_deferred_α
#-----------------------------------------------------------------------------------------------------------------------
n14_goto_deferred_α:
                        mov              rdi, qword ptr [rip + .Lx18_0]
                        call             rt_goto_transfer@PLT
                                                                                        jmp   .Lx18_1
.Lx18_0:
                        .quad            .Lx18_0_s
.Lx18_0_s:
                        .string          "bumpit"
.Lx18_1:
                                                                                        jmp   proc_bumpit_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_bumpit_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_bumpit_β:
                                                                                        jmp   proc_bumpit_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_bumpit_γ:
                        mov              rdi, [rsp]
                        mov              rsi, [rsp + 8]
                        mov              rax, [rsp + 40]
                        add              rsp, 64
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_bumpit_ω:
                        mov              rax, [rsp + 48]
                        add              rsp, 64
                                                                                        jmp   rax
proc_startup:
                        sub              rsp, 8
                        .section         .rodata
.Lstartup_pname0:       .string          "LBL__bumpit"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname0]
                        lea              rsi, [rip + proc_LBL__bumpit_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 144
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname1:       .string          "bumpit"
.Lstartup_pp1_0:        .string          "v"
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
                        lea              rsi, [rip + proc_bumpit_α]
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
.Lgvan0:                .string          "bumpit"
.Lgvan1:                .string          "v"
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
                        sub              rsp, 152
                        mov              rdi, rsp
                        mov              ecx, 152
                        xor              eax, eax
                        rep stosb
main_α_body:
#=======================================================================================================================
#         DEFINE('bumpit(v)', .bumpit)                                :(bumpend)
#         OUTPUT = bumpit(41)
#-----------------------------------------------------------------------------------------------------------------------
n19_lit_integer_α:
                        mov              qword ptr [rsp + 128], 6
                        mov              rax, qword ptr [rip + .Lx29_0]
                        mov              qword ptr [rsp + 136], rax
                                                                                        jmp   n20_call_α
.Lx29_0:
                        .quad            41
#-----------------------------------------------------------------------------------------------------------------------
n20_call_α:
                        sub              rsp, 32
                        mov              rax, qword ptr [1879052304]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052288]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx31_0]
                        mov              esi, 1
                        mov              edx, 1
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx31_5
                        mov              rax, qword ptr [rsp + 160]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 168]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx31_6]
                        lea              rdx, [rip + .Lx31_7]
                                                                                        jmp   rax
.Lx31_6:
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
                                                                                        jmp   .Lx31_2
.Lx31_7:
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
                                                                                        jmp   .Lx31_2
.Lx31_5:
                        add              rsp, 32
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx31_20
                        mov              rax, qword ptr [rsp + 128]
                        mov              rdx, qword ptr [rsp + 136]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx31_21
.Lx31_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 128]
                        mov              rdx, qword ptr [rsp + 136]
                        call             rt_arg_stage@PLT
.Lx31_21:
                        mov              rdi, qword ptr [rip + .Lx31_0]
                        mov              esi, 1
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx31_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx31_3]
                        lea              rdx, [rip + .Lx31_4]
                                                                                        jmp   rax
.Lx31_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx31_2
.Lx31_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx31_2
.Lx31_1:
                        call             rt_faildescr@PLT
.Lx31_2:
                        mov              qword ptr [rsp + 80], rax
                        mov              qword ptr [rsp + 88], rdx
                        cmp              eax, 99
                                                                                        je    main_γ
                                                                                        jmp   n21_assign_α
n20_call_β:
                                                                                        jmp   main_γ
.Lx31_0:
                        .quad            .Lx31_0_s
.Lx31_0_s:
                        .string          "bumpit"
#-----------------------------------------------------------------------------------------------------------------------
n21_assign_α:
                        mov              rsi, qword ptr [rsp + 80]
                        mov              rdx, qword ptr [rsp + 88]
                        mov              rdi, qword ptr [rip + .Lx32_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   main_γ
.Lx32_0:
                        .quad            .Lx32_0_s
.Lx32_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n22_goto_α:
                                                                                        jmp   n23_var_α
n22_goto_β:
                                                                                        jmp   main_ω
#=======================================================================================================================
# bumpit  bumpit = v + 1                                             :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n23_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n24_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n24_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx35_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n25_binop_α
.Lx35_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n25_binop_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 32]
                        mov              rsi, qword ptr [rsp + 40]
                        mov              rdx, qword ptr [rsp + 16]
                        mov              rcx, qword ptr [rsp + 24]
                        call             rt_add@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx36_240
                        add              rsp, 16
                                                                                        jmp   n27_save_restore_α
.Lx36_240:
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n26_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n26_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n27_save_restore_α
#-----------------------------------------------------------------------------------------------------------------------
n27_save_restore_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n28_goto_α:
                                                                                        jmp   n19_lit_integer_α
n28_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
                        mov              eax, 1
                        xor              edx, edx
                        add              rsp, 152
                        ret
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              eax, 99
                        xor              edx, edx
                        add              rsp, 152
                        ret
                        .section         .note.GNU-stack,"",@progbits
