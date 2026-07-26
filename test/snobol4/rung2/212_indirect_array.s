                        .intel_syntax    noprefix
                        .text
                        .section         .rodata
.Lgvan0:                .string          "a"
.Lgvan1:                .string          "output"
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
                        sub              rsp, 648
                        mov              rdi, rsp
                        mov              ecx, 648
                        xor              eax, eax
                        rep stosb
                        mov              r12, qword ptr [1879048192]
                        mov              [rsp + 640], rbp
                        mov              rbp, rsp
main_α_body:
#=======================================================================================================================
#         a = array(3)
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_INTEGER
xchain0_n0_α:
                        mov              qword ptr [rbp + 160], 6
                        mov              rax, qword ptr [rip + .Lx1_0]
                        mov              qword ptr [rbp + 168], rax
                                                                                        jmp   xchain0_n1_α
.Lx1_0:
                        .quad            3
#-----------------------------------------------------------------------------------------------------------------------
xchain0_n1_α:
# BOX CALL array(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+160] -> [zr+128]
                        mov              rax, qword ptr [rbp + 160]
                        mov              qword ptr [rbp + 128], rax
                        mov              rax, qword ptr [rbp + 168]
                        mov              qword ptr [rbp + 136], rax
                        .section         .rodata
.Lbynamefn2:            .string          "array"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn2]
                        lea              rsi, [rbp + 128]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 112], rax
                        mov              qword ptr [rbp + 120], rdx
                        cmp              eax, 99
                                                                                        je    xchain0_n3_α
                                                                                        jmp   xchain0_n2_α
xchain0_n1_β:
                                                                                        jmp   xchain0_n3_α
#-----------------------------------------------------------------------------------------------------------------------
# IR_ASSIGN gva
xchain0_n2_α:
                        mov              rax, qword ptr [rbp + 112]
                        mov              rdx, qword ptr [rbp + 120]
                        mov              qword ptr [1879052288], rax
                        mov              qword ptr [1879052296], rdx
                        mov              qword ptr [rbp + 96], rax
                        mov              qword ptr [rbp + 104], rdx
                                                                                        jmp   xchain0_n3_α
#=======================================================================================================================
#         a<2> = 'x'
#-----------------------------------------------------------------------------------------------------------------------
# IR_VAR
xchain0_n3_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rbp + 192], rax
                        mov              qword ptr [rbp + 200], rdx
                                                                                        jmp   xchain0_n4_α
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_INTEGER
xchain0_n4_α:
                        mov              qword ptr [rbp + 224], 6
                        mov              rax, qword ptr [rip + .Lx5_0]
                        mov              qword ptr [rbp + 232], rax
                                                                                        jmp   xchain0_n6_α
.Lx5_0:
                        .quad            2
#=======================================================================================================================
#         a<2> = 'x'
#         differ($.a<2>, 'x')                   :f(e001)
#-----------------------------------------------------------------------------------------------------------------------
# IR_VAR
xchain0_n5_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rbp + 416], rax
                        mov              qword ptr [rbp + 424], rdx
                                                                                        jmp   xchain0_n7_α
#-----------------------------------------------------------------------------------------------------------------------
# IR_SUBSCRIPT x[i] variable
xchain0_n6_α:
                        mov              rdi, qword ptr [rbp + 192]
                        mov              rsi, qword ptr [rbp + 200]
                        mov              rdx, qword ptr [rbp + 224]
                        mov              rcx, qword ptr [rbp + 232]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    xchain0_n5_α
                        mov              qword ptr [rbp + 256], rax
                        mov              qword ptr [rbp + 264], rdx
                                                                                        jmp   xchain0_n9_α
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_INTEGER
xchain0_n7_α:
                        mov              qword ptr [rbp + 448], 6
                        mov              rax, qword ptr [rip + .Lx8_0]
                        mov              qword ptr [rbp + 456], rax
                                                                                        jmp   xchain0_n10_α
.Lx8_0:
                        .quad            2
#=======================================================================================================================
#         output = 'PASS 212_indirect_array (1/1)'
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n8_α:
                        mov              qword ptr [rbp + 608], 1
                        mov              rax, qword ptr [rip + .Lx9_0]
                        mov              qword ptr [rbp + 616], rax
                                                                                        jmp   xchain0_n11_α
.Lx9_0:
                        .quad            .Lx9_0_s
.Lx9_0_s:
                        .string          "PASS 212_indirect_array (1/1)"
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n9_α:
                        mov              qword ptr [rbp + 288], 1
                        mov              rax, qword ptr [rip + .Lx10_0]
                        mov              qword ptr [rbp + 296], rax
                                                                                        jmp   xchain0_n12_α
.Lx10_0:
                        .quad            .Lx10_0_s
.Lx10_0_s:
                        .string          "x"
#-----------------------------------------------------------------------------------------------------------------------
# IR_SUBSCRIPT x[i] variable
xchain0_n10_α:
                        mov              rdi, qword ptr [rbp + 416]
                        mov              rsi, qword ptr [rbp + 424]
                        mov              rdx, qword ptr [rbp + 448]
                        mov              rcx, qword ptr [rbp + 456]
                        call             rt_subscript_var@PLT
                        cmp              eax, 99
                                                                                        je    xchain0_n8_α
                        mov              qword ptr [rbp + 480], rax
                        mov              qword ptr [rbp + 488], rdx
                                                                                        jmp   xchain0_n13_α
#-----------------------------------------------------------------------------------------------------------------------
# IR_ASSIGN gva
xchain0_n11_α:
                        mov              rax, qword ptr [rbp + 608]
                        mov              rdx, qword ptr [rbp + 616]
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                        mov              qword ptr [rbp + 592], rax
                        mov              qword ptr [rbp + 600], rdx
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
# IR_ASSIGN_VAR
xchain0_n12_α:
                        mov              rdi, qword ptr [rbp + 256]
                        mov              rsi, qword ptr [rbp + 264]
                        mov              rdx, qword ptr [rbp + 288]
                        mov              rcx, qword ptr [rbp + 296]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    xchain0_n5_α
                        mov              qword ptr [rbp + 320], rax
                        mov              qword ptr [rbp + 328], rdx
                                                                                        jmp   xchain0_n5_α
#-----------------------------------------------------------------------------------------------------------------------
# IR_DEREF variable -> value
xchain0_n13_α:
                        mov              rdi, qword ptr [rbp + 480]
                        mov              rsi, qword ptr [rbp + 488]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        je    xchain0_n8_α
                        mov              qword ptr [rbp + 400], rax
                        mov              qword ptr [rbp + 408], rdx
                                                                                        jmp   xchain0_n14_α
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n14_α:
                        mov              qword ptr [rbp + 512], 1
                        mov              rax, qword ptr [rip + .Lx15_0]
                        mov              qword ptr [rbp + 520], rax
                                                                                        jmp   xchain0_n15_α
.Lx15_0:
                        .quad            .Lx15_0_s
.Lx15_0_s:
                        .string          "x"
#-----------------------------------------------------------------------------------------------------------------------
xchain0_n15_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+400] -> [zr+352]
                        mov              rax, qword ptr [rbp + 400]
                        mov              qword ptr [rbp + 352], rax
                        mov              rax, qword ptr [rbp + 408]
                        mov              qword ptr [rbp + 360], rax
# marshal arg1 = producer-box slot [zr+512] -> [zr+368]
                        mov              rax, qword ptr [rbp + 512]
                        mov              qword ptr [rbp + 368], rax
                        mov              rax, qword ptr [rbp + 520]
                        mov              qword ptr [rbp + 376], rax
                        .section         .rodata
.Lbynamefn16:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn16]
                        lea              rsi, [rbp + 352]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rbp + 336], rax
                        mov              qword ptr [rbp + 344], rdx
                        cmp              eax, 99
                                                                                        je    xchain0_n8_α
                                                                                        jmp   xchain0_n16_α
xchain0_n15_β:
                                                                                        jmp   xchain0_n8_α
#=======================================================================================================================
#         output = 'FAIL 212/001: $.var<index> indirect array' :(end)
#-----------------------------------------------------------------------------------------------------------------------
# IR_LIT_STRING
xchain0_n16_α:
                        mov              qword ptr [rbp + 560], 1
                        mov              rax, qword ptr [rip + .Lx17_0]
                        mov              qword ptr [rbp + 568], rax
                                                                                        jmp   xchain0_n17_α
.Lx17_0:
                        .quad            .Lx17_0_s
.Lx17_0_s:
                        .string          "FAIL 212/001: $.var<index> indirect array"
#-----------------------------------------------------------------------------------------------------------------------
# IR_ASSIGN gva
xchain0_n17_α:
                        mov              rax, qword ptr [rbp + 560]
                        mov              rdx, qword ptr [rbp + 568]
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                        mov              qword ptr [rbp + 544], rax
                        mov              qword ptr [rbp + 552], rdx
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
                        mov              eax, 1
                        xor              edx, edx
                        mov              rsp, rbp
                        mov              rbp, [rsp + 640]
                        add              rsp, 648
                        ret
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              rsp, rbp
                        mov              dword ptr [rsp+0], 99
                        mov              dword ptr [rsp+4], 0
                        mov              qword ptr [rsp+8], 0
                        mov              eax, 99
                        xor              edx, edx
                        mov              rbp, [rsp + 640]
                        add              rsp, 648
                        ret
                        .section         .note.GNU-stack,"",@progbits
