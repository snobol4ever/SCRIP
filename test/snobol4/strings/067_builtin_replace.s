                        .intel_syntax    noprefix
                        .text
                        .globl           main
main:
                        sub              rsp, 8
                        push             rdi
                        push             rsi
                        call             core_lib_init@PLT
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
#         OUTPUT = REPLACE('hello', 'aeiou', 'AEIOU')
#-----------------------------------------------------------------------------------------------------------------------
n0_lit_string_α:
                        mov              qword ptr [rsp + 96], 1
                        mov              dword ptr [rsp + 100], 5
                        mov              rax, qword ptr [rip + .Lx5_0]
                        mov              qword ptr [rsp + 104], rax
                                                                                        jmp   n1_lit_string_α
.Lx5_0:
                        .quad            .Lx5_0_s
.Lx5_0_s:
                        .string          "hello"
#-----------------------------------------------------------------------------------------------------------------------
n1_lit_string_α:
                        mov              qword ptr [rsp + 112], 1
                        mov              dword ptr [rsp + 116], 5
                        mov              rax, qword ptr [rip + .Lx6_0]
                        mov              qword ptr [rsp + 120], rax
                                                                                        jmp   n2_lit_string_α
.Lx6_0:
                        .quad            .Lx6_0_s
.Lx6_0_s:
                        .string          "aeiou"
#-----------------------------------------------------------------------------------------------------------------------
n2_lit_string_α:
                        mov              qword ptr [rsp + 128], 1
                        mov              dword ptr [rsp + 132], 5
                        mov              rax, qword ptr [rip + .Lx7_0]
                        mov              qword ptr [rsp + 136], rax
                                                                                        jmp   n3_call_α
.Lx7_0:
                        .quad            .Lx7_0_s
.Lx7_0_s:
                        .string          "AEIOU"
#-----------------------------------------------------------------------------------------------------------------------
n3_call_α:
                        mov              rax, qword ptr [rsp + 96]
                        mov              qword ptr [rsp + 32], rax
                        mov              rax, qword ptr [rsp + 104]
                        mov              qword ptr [rsp + 40], rax
                        mov              rax, qword ptr [rsp + 112]
                        mov              qword ptr [rsp + 48], rax
                        mov              rax, qword ptr [rsp + 120]
                        mov              qword ptr [rsp + 56], rax
                        mov              rax, qword ptr [rsp + 128]
                        mov              qword ptr [rsp + 64], rax
                        mov              rax, qword ptr [rsp + 136]
                        mov              qword ptr [rsp + 72], rax
                        .section         .rodata
.Lrkfn9:                .string          "REPLACE"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn9]
                        lea              rsi, [rsp + 32]
                        mov              edx, 3
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 16], rax
                        mov              qword ptr [rsp + 24], rdx
                        cmp              eax, 99
                                                                                        je    main_γ
                                                                                        jmp   n4_assign_α
n3_call_β:
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n4_assign_α:
                        mov              rsi, qword ptr [rsp + 16]
                        mov              rdx, qword ptr [rsp + 24]
                        mov              rdi, qword ptr [rip + .Lx10_0]
                        call             NV_SET_fn@PLT
                                                                                        jmp   main_γ
.Lx10_0:
                        .quad            .Lx10_0_s
.Lx10_0_s:
                        .string          "OUTPUT"
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
