                        .intel_syntax    noprefix
                        .text
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_LBL__bump_α
proc_LBL__bump_α:
                        .global          proc_LBL__bump_α
                        .global          proc_LBL__bump_β
                        .global          proc_LBL__bump_γ
                        .global          proc_LBL__bump_ω
                        sub              rsp, 400
                        mov              [rsp + 376], rcx
                        mov              [rsp + 384], rdx
                        mov              qword ptr [rsp], 0
                        mov              qword ptr [rsp + 8], 0
proc_LBL__bump_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n0_goto_α:
                                                                                        jmp   n1_var_α
n0_goto_β:
                                                                                        jmp   proc_LBL__bump_ω
#=======================================================================================================================
# bump    bump = v + 1                                               :(RETURN)
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
                                                                                        jmp   n4_binop_α
.Lx8_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n3_op14_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n4_binop_α:
                        mov              eax, dword ptr [rsp + 16]
                        cmp              eax, 6
                                                                                        jne   .Lx11_0
                        mov              eax, dword ptr [rsp + 0]
                        cmp              eax, 6
                                                                                        jne   .Lx11_0
                        mov              rax, qword ptr [rsp + 24]
                        mov              rcx, qword ptr [rsp + 8]
                        add              rax, rcx
                        add              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n5_assign_α
.Lx11_0:
                        mov              rdi, qword ptr [rsp + 16]
                        mov              rsi, qword ptr [rsp + 24]
                        mov              rdx, qword ptr [rsp + 0]
                        mov              rcx, qword ptr [rsp + 8]
                        call             rt_add@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx11_240
                        add              rsp, 32
                                                                                        jmp   n3_op14_α
.Lx11_240:
                        add              rsp, 16
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n5_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n5_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n3_op14_α
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__bump_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__bump_β:
                                                                                        jmp   proc_LBL__bump_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__bump_γ:
                        mov              rdi, [rsp]
                        mov              rsi, [rsp + 8]
                        mov              rax, [rsp + 376]
                        add              rsp, 400
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_LBL__bump_ω:
                        mov              rax, [rsp + 384]
                        add              rsp, 400
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
                        .globl           proc_bump_α
proc_bump_α:
                        .global          proc_bump_α
                        .global          proc_bump_β
                        .global          proc_bump_γ
                        .global          proc_bump_ω
                        sub              rsp, 400
                        mov              [rsp + 376], rcx
                        mov              [rsp + 384], rdx
                        mov              rdi, rsp
                        mov              ecx, 368
                        xor              eax, eax
                        rep stosb
proc_bump_α_body:
#-----------------------------------------------------------------------------------------------------------------------
n13_op14_α:
                        mov              rdi, qword ptr [rsp + 376]
                        mov              rsi, qword ptr [rsp + 384]
                        lea              rdx, [rsp + 400]
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
                        .string          "bump"
.Lx18_1:
                                                                                        jmp   proc_bump_γ
#-----------------------------------------------------------------------------------------------------------------------
proc_bump_res:
                        add              rsp, 8
                        pop              rsp
#-----------------------------------------------------------------------------------------------------------------------
proc_bump_β:
                                                                                        jmp   proc_bump_ω
#-----------------------------------------------------------------------------------------------------------------------
proc_bump_γ:
                        mov              rdi, [rsp]
                        mov              rsi, [rsp + 8]
                        mov              rax, [rsp + 376]
                        add              rsp, 400
                                                                                        jmp   rax
#-----------------------------------------------------------------------------------------------------------------------
proc_bump_ω:
                        mov              rax, [rsp + 384]
                        add              rsp, 400
                                                                                        jmp   rax
proc_startup:
                        sub              rsp, 8
                        .section         .rodata
.Lstartup_pname0:       .string          "LBL__bump"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lstartup_pname0]
                        lea              rsi, [rip + proc_LBL__bump_α]
                        call             rt_proc_set_fn@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 0
                        call             rt_proc_set_nparams@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 368
                        call             rt_proc_set_frame_bytes@PLT
                        lea              rdi, [rip + .Lstartup_pname0]
                        mov              esi, 1
                        call             rt_proc_set_jmpentry@PLT
                        .section         .rodata
.Lstartup_pname1:       .string          "bump"
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
                        lea              rsi, [rip + proc_bump_α]
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
.Lgvan0:                .string          "bump"
.Lgvan1:                .string          "v"
.Lgvan2:                .string          "S"
.Lgvan3:                .string          "J"
                        .align           8
__gva_names:
                        .quad            .Lgvan0
                        .quad            .Lgvan1
                        .quad            .Lgvan2
                        .quad            .Lgvan3
                        .section         .text
                        .intel_syntax    noprefix
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
                        call             proc_startup
                        mov              edi, 4
                        call             rt_gva_island@PLT
                        mov              rsi, rax
                        lea              rdi, [rip + __gva_names]
                        mov              edx, 4
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
                        sub              rsp, 376
                        mov              rdi, rsp
                        mov              ecx, 376
                        xor              eax, eax
                        rep stosb
main_α_body:
#=======================================================================================================================
#         DEFINE('bump(v)')                                           :(bumpend)
#         S = ''
#-----------------------------------------------------------------------------------------------------------------------
n19_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 0
                        mov              rax, qword ptr [rip + .Lx49_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n23_assign_α
.Lx49_0:
                        .quad            .Lx49_0_s
.Lx49_0_s:
                        .string          ""
#-----------------------------------------------------------------------------------------------------------------------
n20_goto_α:
                                                                                        jmp   n24_var_α
n20_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n21_goto_α:
                                                                                        jmp   n19_lit_string_α
n21_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n22_goto_α:
                                                                                        jmp   n25_var_α
n22_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n23_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052320], rax
                        mov              qword ptr [1879052328], rdx
                                                                                        jmp   n26_lit_integer_α
#=======================================================================================================================
# bump    bump = v + 1                                               :(RETURN)
#-----------------------------------------------------------------------------------------------------------------------
n24_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052304]
                        mov              rdx, qword ptr [1879052312]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n27_lit_integer_α
#=======================================================================================================================
# LOOP    S = S bump(2 * J)
#-----------------------------------------------------------------------------------------------------------------------
n25_var_α:
                        mov              rax, qword ptr [1879052320]
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rsp + 128], rax
                        mov              qword ptr [rsp + 136], rdx
                                                                                        jmp   n29_lit_integer_α
#=======================================================================================================================
#         J = 0
#-----------------------------------------------------------------------------------------------------------------------
n26_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx56_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n31_assign_α
.Lx56_0:
                        .quad            0
#-----------------------------------------------------------------------------------------------------------------------
n27_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx57_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n32_binop_α
.Lx57_0:
                        .quad            1
#-----------------------------------------------------------------------------------------------------------------------
n28_op14_α:
                        call             rt_flat_ret_snap@PLT
                        mov              rcx, qword ptr [rax + 0]
                        mov              rbp, qword ptr [rax + 24]
                        mov              rsp, qword ptr [rax + 16]
                                                                                        jmp   rcx
#-----------------------------------------------------------------------------------------------------------------------
n29_lit_integer_α:
                        mov              qword ptr [rsp + 208], 6
                        mov              rax, qword ptr [rip + .Lx60_0]
                        mov              qword ptr [rsp + 216], rax
                                                                                        jmp   n33_var_α
.Lx60_0:
                        .quad            2
#=======================================================================================================================
#         J = J + 1
#-----------------------------------------------------------------------------------------------------------------------
n30_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n34_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n31_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052336], rax
                        mov              qword ptr [1879052344], rdx
                                                                                        jmp   n25_var_α
#-----------------------------------------------------------------------------------------------------------------------
n32_binop_α:
                        mov              eax, dword ptr [rsp + 16]
                        cmp              eax, 6
                                                                                        jne   .Lx63_0
                        mov              eax, dword ptr [rsp + 0]
                        cmp              eax, 6
                                                                                        jne   .Lx63_0
                        mov              rax, qword ptr [rsp + 24]
                        mov              rcx, qword ptr [rsp + 8]
                        add              rax, rcx
                        add              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n36_assign_α
.Lx63_0:
                        mov              rdi, qword ptr [rsp + 16]
                        mov              rsi, qword ptr [rsp + 24]
                        mov              rdx, qword ptr [rsp + 0]
                        mov              rcx, qword ptr [rsp + 8]
                        call             rt_add@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx63_240
                        add              rsp, 32
                                                                                        jmp   n28_op14_α
.Lx63_240:
                        add              rsp, 16
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n36_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n33_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 224], rax
                        mov              qword ptr [rsp + 232], rdx
                                                                                        jmp   n37_binop_α
#-----------------------------------------------------------------------------------------------------------------------
n34_lit_integer_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              rax, qword ptr [rip + .Lx65_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n38_binop_α
.Lx65_0:
                        .quad            1
#=======================================================================================================================
#         LT(J, 5)                                                    :S(LOOP)
#-----------------------------------------------------------------------------------------------------------------------
n35_var_α:
                        mov              rax, qword ptr [1879052336]
                        mov              rdx, qword ptr [1879052344]
                        mov              qword ptr [rsp + 336], rax
                        mov              qword ptr [rsp + 344], rdx
                                                                                        jmp   n39_lit_integer_α
#-----------------------------------------------------------------------------------------------------------------------
n36_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                                                                                        jmp   n28_op14_α
#-----------------------------------------------------------------------------------------------------------------------
n37_binop_α:
                        mov              eax, dword ptr [rsp + 224]
                        cmp              eax, 6
                                                                                        jne   .Lx68_0
                        mov              rax, 2
                        mov              rcx, qword ptr [rsp + 232]
                        imul             rax, rcx
                        mov              qword ptr [rsp + 192], 6
                        mov              qword ptr [rsp + 200], rax
                                                                                        jmp   n41_call_α
.Lx68_0:
                        mov              rdi, qword ptr [rsp + 208]
                        mov              rsi, qword ptr [rsp + 216]
                        mov              rdx, qword ptr [rsp + 224]
                        mov              rcx, qword ptr [rsp + 232]
                        call             rt_mul@PLT
                        cmp              eax, 99
                                                                                        je    n30_var_α
                        mov              qword ptr [rsp + 192], rax
                        mov              qword ptr [rsp + 200], rdx
                                                                                        jmp   n41_call_α
#-----------------------------------------------------------------------------------------------------------------------
n38_binop_α:
                        mov              eax, dword ptr [rsp + 16]
                        cmp              eax, 6
                                                                                        jne   .Lx69_0
                        mov              eax, dword ptr [rsp + 0]
                        cmp              eax, 6
                                                                                        jne   .Lx69_0
                        mov              rax, qword ptr [rsp + 24]
                        mov              rcx, qword ptr [rsp + 8]
                        add              rax, rcx
                        add              rsp, 16
                        mov              qword ptr [rsp + 0], 6
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n42_assign_α
.Lx69_0:
                        mov              rdi, qword ptr [rsp + 16]
                        mov              rsi, qword ptr [rsp + 24]
                        mov              rdx, qword ptr [rsp + 0]
                        mov              rcx, qword ptr [rsp + 8]
                        call             rt_add@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx69_240
                        add              rsp, 32
                                                                                        jmp   n35_var_α
.Lx69_240:
                        add              rsp, 16
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n42_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n39_lit_integer_α:
                        mov              qword ptr [rsp + 320], 6
                        mov              rax, qword ptr [rip + .Lx70_0]
                        mov              qword ptr [rsp + 328], rax
                                                                                        jmp   n43_op75_α
.Lx70_0:
                        .quad            5
#=======================================================================================================================
#         OUTPUT = S
#-----------------------------------------------------------------------------------------------------------------------
n40_var_α:
                        sub              rsp, 16
                        mov              rax, qword ptr [1879052320]
                        mov              rdx, qword ptr [1879052328]
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                                                                                        jmp   n44_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n41_call_α:
                        sub              rsp, 32
                        mov              rax, qword ptr [1879052304]
                        mov              qword ptr [rsp + 0], rax
                        mov              rax, qword ptr [1879052312]
                        mov              qword ptr [rsp + 8], rax
                        mov              rax, qword ptr [1879052288]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [1879052296]
                        mov              qword ptr [rsp + 24], rax
                        mov              rdi, qword ptr [rip + .Lx73_0]
                        mov              esi, 1
                        mov              edx, 1
                        call             rt_proc_call_open_slim@PLT
                        test             rax, rax
                                                                                        je    .Lx73_5
                        mov              rax, qword ptr [rsp + 224]
                        mov              qword ptr [1879052304], rax
                        mov              rax, qword ptr [rsp + 232]
                        mov              qword ptr [1879052312], rax
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx73_6]
                        lea              rdx, [rip + .Lx73_7]
                                                                                        jmp   rax
.Lx73_6:
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
                                                                                        jmp   .Lx73_2
.Lx73_7:
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
                                                                                        jmp   .Lx73_2
.Lx73_5:
                        add              rsp, 32
                        lea              r11, [rip + g_gc_pending]
                        mov              eax, dword ptr [r11 + 0]
                        test             eax, eax
                                                                                        jne   .Lx73_20
                        mov              rax, qword ptr [rsp + 192]
                        mov              rdx, qword ptr [rsp + 200]
                        lea              r10, [rip + g_call_args]
                        mov              qword ptr [r10 + 0], rax
                        mov              qword ptr [r10 + 8], rdx
                                                                                        jmp   .Lx73_21
.Lx73_20:
                        mov              edi, 0
                        mov              rsi, qword ptr [rsp + 192]
                        mov              rdx, qword ptr [rsp + 200]
                        call             rt_arg_stage@PLT
.Lx73_21:
                        mov              rdi, qword ptr [rip + .Lx73_0]
                        mov              esi, 1
                        call             rt_proc_call_open@PLT
                        test             rax, rax
                                                                                        je    .Lx73_1
                        call             rt_proc_open_fn@PLT
                        lea              rcx, [rip + .Lx73_3]
                        lea              rdx, [rip + .Lx73_4]
                                                                                        jmp   rax
.Lx73_3:
                        call             rt_proc_call_epilogue_γ@PLT
                                                                                        jmp   .Lx73_2
.Lx73_4:
                        call             rt_proc_call_epilogue_ω@PLT
                                                                                        jmp   .Lx73_2
.Lx73_1:
                        call             rt_faildescr@PLT
.Lx73_2:
                        mov              qword ptr [rsp + 144], rax
                        mov              qword ptr [rsp + 152], rdx
                        cmp              eax, 99
                                                                                        je    n30_var_α
                                                                                        jmp   n45_binop_α
n41_call_β:
                                                                                        jmp   n30_var_α
.Lx73_0:
                        .quad            .Lx73_0_s
.Lx73_0_s:
                        .string          "bump"
#-----------------------------------------------------------------------------------------------------------------------
n42_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              qword ptr [1879052336], rax
                        mov              qword ptr [1879052344], rdx
                                                                                        jmp   n35_var_α
#-----------------------------------------------------------------------------------------------------------------------
n43_op75_α:
                        mov              eax, dword ptr [rsp + 336]
                        cmp              eax, 7
                                                                                        je    .Lx76_1
                        cmp              eax, 6
                                                                                        jne   .Lx76_0
                        mov              eax, dword ptr [rsp + 320]
                        cmp              eax, 6
                                                                                        jne   .Lx76_0
.Lx76_1:
                        mov              rax, qword ptr [rsp + 336]
                        mov              qword ptr [rsp + 304], rax
                        mov              rax, qword ptr [rsp + 344]
                        mov              qword ptr [rsp + 312], rax
                                                                                        jmp   n46_op75_α
.Lx76_0:
                        lea              rdi, [rsp + 336]
                        lea              rsi, [rsp + 320]
                        lea              rdx, [rsp + 304]
                        mov              rcx, 147
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n46_op75_α
#-----------------------------------------------------------------------------------------------------------------------
n44_assign_α:
                        mov              rsi, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        add              rsp, 16
                        mov              rdi, qword ptr [rip + .Lx77_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   main_γ
.Lx77_0:
                        .quad            .Lx77_0_s
.Lx77_0_s:
                        .string          "OUTPUT"
#-----------------------------------------------------------------------------------------------------------------------
n45_binop_α:
                        mov              rdi, qword ptr [rsp + 128]
                        mov              rsi, qword ptr [rsp + 136]
                        mov              rdx, qword ptr [rsp + 144]
                        mov              rcx, qword ptr [rsp + 152]
                        call             str_concat_d@PLT
                        mov              qword ptr [rsp + 112], rax
                        mov              qword ptr [rsp + 120], rdx
                                                                                        jmp   n47_assign_α
#-----------------------------------------------------------------------------------------------------------------------
n46_op75_α:
                        mov              eax, dword ptr [rsp + 320]
                        cmp              eax, 7
                                                                                        je    .Lx80_1
                        cmp              eax, 6
                                                                                        jne   .Lx80_0
                        mov              eax, dword ptr [rsp + 336]
                        cmp              eax, 6
                                                                                        jne   .Lx80_0
.Lx80_1:
                        mov              rax, qword ptr [rsp + 320]
                        mov              qword ptr [rsp + 288], rax
                        mov              rax, qword ptr [rsp + 328]
                        mov              qword ptr [rsp + 296], rax
                                                                                        jmp   n48_op77_α
.Lx80_0:
                        lea              rdi, [rsp + 320]
                        lea              rsi, [rsp + 336]
                        lea              rdx, [rsp + 288]
                        mov              rcx, 148
                        call             rt_coerce_num2_d@PLT
                                                                                        jmp   n48_op77_α
#-----------------------------------------------------------------------------------------------------------------------
n47_assign_α:
                        mov              rax, qword ptr [rsp + 112]
                        mov              rdx, qword ptr [rsp + 120]
                        mov              qword ptr [1879052320], rax
                        mov              qword ptr [1879052328], rdx
                                                                                        jmp   n30_var_α
#-----------------------------------------------------------------------------------------------------------------------
n48_op77_α:
                        lea              rdi, [rsp + 304]
                        lea              rsi, [rsp + 288]
                        call             rt_cmp_d@PLT
                        test             eax, eax
                                                                                        jns   n40_var_α
                                                                                        jmp   n25_var_α
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
                        mov              eax, 1
                        xor              edx, edx
                        add              rsp, 376
                        ret
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              eax, 99
                        xor              edx, edx
                        add              rsp, 376
                        ret
                        .section         .note.GNU-stack,"",@progbits
