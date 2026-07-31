                        .intel_syntax    noprefix
                        .text
                        .section         .rodata
.Lgvan0:                .string          "qq"
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
                        sub              rsp, 392
                        mov              rdi, rsp
                        mov              ecx, 392
                        xor              eax, eax
                        rep stosb
main_α_body:
#=======================================================================================================================
#         $'qq' = 'x'
#-----------------------------------------------------------------------------------------------------------------------
n0_lit_string_α:
                        mov              qword ptr [rsp + 48], 1
                        mov              dword ptr [rsp + 52], 2
                        mov              rax, qword ptr [rip + .Lx20_0]
                        mov              qword ptr [rsp + 56], rax
                                                                                        jmp   n4_call_α
.Lx20_0:
                        .quad            .Lx20_0_s
.Lx20_0_s:
                        .string          "qq"
#-----------------------------------------------------------------------------------------------------------------------
n1_goto_α:
                                                                                        jmp   n5_lit_string_α
n1_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n2_goto_α:
                                                                                        jmp   n6_lit_string_α
n2_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n3_goto_α:
                                                                                        jmp   main_γ
n3_goto_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
n4_call_α:
                        mov              rax, qword ptr [rsp + 48]
                        mov              qword ptr [rsp + 16], rax
                        mov              rax, qword ptr [rsp + 56]
                        mov              qword ptr [rsp + 24], rax
                        .section         .rodata
.Lrkfn25:               .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn25]
                        lea              rsi, [rsp + 16]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 0], rax
                        mov              qword ptr [rsp + 8], rdx
                        cmp              eax, 99
                                                                                        je    n8_var_α
                                                                                        jmp   n7_lit_string_α
n4_call_β:
                                                                                        jmp   n8_var_α
#=======================================================================================================================
#         differ($'_no_such_var_')                   :f(e002)
#-----------------------------------------------------------------------------------------------------------------------
n5_lit_string_α:
                        mov              qword ptr [rsp + 336], 1
                        mov              dword ptr [rsp + 340], 13
                        mov              rax, qword ptr [rip + .Lx26_0]
                        mov              qword ptr [rsp + 344], rax
                                                                                        jmp   n9_call_α
.Lx26_0:
                        .quad            .Lx26_0_s
.Lx26_0_s:
                        .string          "_no_such_var_"
#=======================================================================================================================
#         output = 'PASS 211_indirect_assign (2/2)'
#-----------------------------------------------------------------------------------------------------------------------
n6_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 30
                        mov              rax, qword ptr [rip + .Lx27_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n10_assign_α
.Lx27_0:
                        .quad            .Lx27_0_s
.Lx27_0_s:
                        .string          "PASS 211_indirect_assign (2/2)"
#-----------------------------------------------------------------------------------------------------------------------
n7_lit_string_α:
                        mov              qword ptr [rsp + 64], 1
                        mov              dword ptr [rsp + 68], 1
                        mov              rax, qword ptr [rip + .Lx28_0]
                        mov              qword ptr [rsp + 72], rax
                                                                                        jmp   n11_assign_var_α
.Lx28_0:
                        .quad            .Lx28_0_s
.Lx28_0_s:
                        .string          "x"
#=======================================================================================================================
#         differ(qq, 'x')                   :f(e001)
#-----------------------------------------------------------------------------------------------------------------------
n8_var_α:
                        mov              rax, qword ptr [1879052288]
                        mov              rdx, qword ptr [1879052296]
                        mov              qword ptr [rsp + 160], rax
                        mov              qword ptr [rsp + 168], rdx
                                                                                        jmp   n12_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n9_call_α:
                        mov              rax, qword ptr [rsp + 336]
                        mov              qword ptr [rsp + 304], rax
                        mov              rax, qword ptr [rsp + 344]
                        mov              qword ptr [rsp + 312], rax
                        .section         .rodata
.Lrkfn31:               .string          "SNO$NAME"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lrkfn31]
                        lea              rsi, [rsp + 304]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 288], rax
                        mov              qword ptr [rsp + 296], rdx
                        cmp              eax, 99
                                                                                        je    n6_lit_string_α
                                                                                        jmp   n13_deref_α
n9_call_β:
                                                                                        jmp   n6_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n10_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n11_assign_var_α:
                        mov              rdi, qword ptr [rsp + 0]
                        mov              rsi, qword ptr [rsp + 8]
                        mov              rdx, qword ptr [rsp + 64]
                        mov              rcx, qword ptr [rsp + 72]
                        call             rt_assign_var@PLT
                        cmp              eax, 99
                                                                                        je    n8_var_α
                        mov              qword ptr [rsp + 80], rax
                        mov              qword ptr [rsp + 88], rdx
                                                                                        jmp   n8_var_α
#-----------------------------------------------------------------------------------------------------------------------
n12_lit_string_α:
                        mov              qword ptr [rsp + 176], 1
                        mov              dword ptr [rsp + 180], 1
                        mov              rax, qword ptr [rip + .Lx34_0]
                        mov              qword ptr [rsp + 184], rax
                                                                                        jmp   n14_call_α
.Lx34_0:
                        .quad            .Lx34_0_s
.Lx34_0_s:
                        .string          "x"
#-----------------------------------------------------------------------------------------------------------------------
n13_deref_α:
                        sub              rsp, 16
                        mov              rdi, qword ptr [rsp + 304]
                        mov              rsi, qword ptr [rsp + 312]
                        call             rt_deref@PLT
                        cmp              eax, 99
                                                                                        jne   .Lx35_240
                        add              rsp, 16
                                                                                        jmp   n6_lit_string_α
.Lx35_240:
                        mov              qword ptr [rsp + 288], rax
                        mov              qword ptr [rsp + 296], rdx
                        add              rsp, 16
                                                                                        jmp   n15_call_α
#-----------------------------------------------------------------------------------------------------------------------
n14_call_α:
                        mov              rax, qword ptr [rsp + 160]
                        mov              qword ptr [rsp + 112], rax
                        mov              rax, qword ptr [rsp + 168]
                        mov              qword ptr [rsp + 120], rax
                        mov              rax, qword ptr [rsp + 176]
                        mov              qword ptr [rsp + 128], rax
                        mov              rax, qword ptr [rsp + 184]
                        mov              qword ptr [rsp + 136], rax
                        .section         .rodata
.Lbynamefn15:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn15]
                        lea              rsi, [rsp + 112]
                        mov              edx, 2
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 96], rax
                        mov              qword ptr [rsp + 104], rdx
                        cmp              eax, 99
                                                                                        je    n5_lit_string_α
                                                                                        jmp   n16_lit_string_α
n14_call_β:
                                                                                        jmp   n5_lit_string_α
#-----------------------------------------------------------------------------------------------------------------------
n15_call_α:
                        mov              rax, qword ptr [rsp + 272]
                        mov              qword ptr [rsp + 240], rax
                        mov              rax, qword ptr [rsp + 280]
                        mov              qword ptr [rsp + 248], rax
                        .section         .rodata
.Lbynamefn16:           .string          "differ"
                        .section         .text
                        .intel_syntax    noprefix
                        lea              rdi, [rip + .Lbynamefn16]
                        lea              rsi, [rsp + 240]
                        mov              edx, 1
                        call             rt_call_arr@PLT
                        mov              qword ptr [rsp + 224], rax
                        mov              qword ptr [rsp + 232], rdx
                        cmp              eax, 99
                                                                                        je    n6_lit_string_α
                                                                                        jmp   n17_lit_string_α
n15_call_β:
                                                                                        jmp   n6_lit_string_α
#=======================================================================================================================
#         output = 'FAIL 211/001: indirect assign sets named var' :(end)
#-----------------------------------------------------------------------------------------------------------------------
n16_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 44
                        mov              rax, qword ptr [rip + .Lx38_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n18_assign_α
.Lx38_0:
                        .quad            .Lx38_0_s
.Lx38_0_s:
                        .string          "FAIL 211/001: indirect assign sets named var"
#=======================================================================================================================
#         output = 'FAIL 211/002: undefined indirect is null'     :(end)
#-----------------------------------------------------------------------------------------------------------------------
n17_lit_string_α:
                        sub              rsp, 16
                        mov              qword ptr [rsp + 0], 1
                        mov              dword ptr [rsp + 4], 40
                        mov              rax, qword ptr [rip + .Lx39_0]
                        mov              qword ptr [rsp + 8], rax
                                                                                        jmp   n19_assign_α
.Lx39_0:
                        .quad            .Lx39_0_s
.Lx39_0_s:
                        .string          "FAIL 211/002: undefined indirect is null"
#-----------------------------------------------------------------------------------------------------------------------
n18_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
n19_assign_α:
                        mov              rax, qword ptr [rsp + 0]
                        mov              rdx, qword ptr [rsp + 8]
                        mov              qword ptr [1879052304], rax
                        mov              qword ptr [1879052312], rdx
                        add              rsp, 16
                                                                                        jmp   main_γ
#-----------------------------------------------------------------------------------------------------------------------
main_β:
                                                                                        jmp   main_ω
#-----------------------------------------------------------------------------------------------------------------------
main_γ:
                        mov              eax, 1
                        xor              edx, edx
                        add              rsp, 392
                        ret
#-----------------------------------------------------------------------------------------------------------------------
main_ω:
                        mov              eax, 99
                        xor              edx, edx
                        add              rsp, 392
                        ret
                        .section         .note.GNU-stack,"",@progbits
