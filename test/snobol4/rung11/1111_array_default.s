  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "b"
  .Lgvan1: .string "output"
  .align 8
__gva_names:
  .quad .Lgvan0
  .quad .Lgvan1
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
  mov edi, 2
  call rt_gva_island@PLT
  mov rsi, rax
  lea rdi, [rip + __gva_names]
  mov edx, 2
  call gva_register@PLT
  xor esi, esi
  call main_α
  xor eax, eax
  add rsp, 24
  ret
main_α:
#=======================================================================================================================
    .global main_α
    .global main_β
    .global main_γ
    .global main_ω
  sub rsp, 65544
  mov rdi, rsp
  mov ecx, 65544
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 808], rsp
  mov r12, qword ptr [1879048192]
main_α_body:
# IR_LIT_INTEGER
 xchain0_n0_α:
 mov qword ptr [rsp + 176], 6
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [rsp + 184], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n4_α
.Lx1_0:
 .quad 3
# IR_LIT_INTEGER
 xchain0_n1_α:
 mov qword ptr [rsp + 208], 6
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [rsp + 216], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n4_α
.Lx2_0:
 .quad 10
 xchain0_n2_α:
# BOX CALL array(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+176] -> [zr+128]
 mov rax, qword ptr [rsp + 176]
 mov qword ptr [rsp + 128], rax
 mov rax, qword ptr [rsp + 184]
 mov qword ptr [rsp + 136], rax
# marshal arg1 = producer-box slot [zr+208] -> [zr+144]
 mov rax, qword ptr [rsp + 208]
 mov qword ptr [rsp + 144], rax
 mov rax, qword ptr [rsp + 216]
 mov qword ptr [rsp + 152], rax
  .section .rodata
  .Lbynamefn3: .string "array"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn3]
 lea rsi, [rsp + 128]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 112], rax
 mov qword ptr [rsp + 120], rdx
 cmp eax, 99
 je xchain0_n4_α
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n4_α
# IR_ASSIGN gva
 xchain0_n3_α:
 mov rax, qword ptr [rsp + 112]
 mov rdx, qword ptr [rsp + 120]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rsp + 96], rax
 mov qword ptr [rsp + 104], rdx
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp xchain0_n4_α
# IR_VAR
 xchain0_n4_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 mov qword ptr [rsp + 304], rax
 mov qword ptr [rsp + 312], rdx
 jmp xchain0_n5_α
 xchain0_n4_β:
 jmp xchain0_n6_α
# IR_LIT_INTEGER
 xchain0_n5_α:
 mov qword ptr [rsp + 336], 6
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [rsp + 344], rax
 jmp xchain0_n7_α
 xchain0_n5_β:
 jmp xchain0_n6_α
.Lx6_0:
 .quad 2
# IR_VAR
 xchain0_n6_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 mov qword ptr [rsp + 560], rax
 mov qword ptr [rsp + 568], rdx
 jmp xchain0_n8_α
 xchain0_n6_β:
 jmp xchain0_n9_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n7_α:
 mov rdi, qword ptr [rsp + 304]
 mov rsi, qword ptr [rsp + 312]
 mov rdx, qword ptr [rsp + 336]
 mov rcx, qword ptr [rsp + 344]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n6_α
 mov qword ptr [rsp + 368], rax
 mov qword ptr [rsp + 376], rdx
 jmp xchain0_n10_α
 xchain0_n7_β:
 jmp xchain0_n6_α
# IR_LIT_INTEGER
 xchain0_n8_α:
 mov qword ptr [rsp + 592], 6
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [rsp + 600], rax
 jmp xchain0_n11_α
 xchain0_n8_β:
 jmp xchain0_n9_α
.Lx9_0:
 .quad 1
# IR_LIT_STRING
 xchain0_n9_α:
 mov qword ptr [rsp + 768], 1
 mov rax, qword ptr [rip + .Lx10_0]
 mov qword ptr [rsp + 776], rax
 jmp xchain0_n12_α
 xchain0_n9_β:
 jmp main_γ
.Lx10_0:
 .quad .Lx10_0_s
.Lx10_0_s:
 .string "PASS 1111_array_default (2/2)"
# IR_DEREF variable -> value
 xchain0_n10_α:
 mov rdi, qword ptr [rsp + 368]
 mov rsi, qword ptr [rsp + 376]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n6_α
 mov qword ptr [rsp + 400], rax
 mov qword ptr [rsp + 408], rdx
 jmp xchain0_n13_α
 xchain0_n10_β:
 jmp xchain0_n6_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n11_α:
 mov rdi, qword ptr [rsp + 560]
 mov rsi, qword ptr [rsp + 568]
 mov rdx, qword ptr [rsp + 592]
 mov rcx, qword ptr [rsp + 600]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n9_α
 mov qword ptr [rsp + 624], rax
 mov qword ptr [rsp + 632], rdx
 jmp xchain0_n14_α
 xchain0_n11_β:
 jmp xchain0_n9_α
# IR_ASSIGN gva
 xchain0_n12_α:
 mov rax, qword ptr [rsp + 768]
 mov rdx, qword ptr [rsp + 776]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rsp + 752], rax
 mov qword ptr [rsp + 760], rdx
 jmp main_γ
 xchain0_n12_β:
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n13_α:
 mov qword ptr [rsp + 416], 6
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [rsp + 424], rax
 jmp xchain0_n15_α
 xchain0_n13_β:
 jmp xchain0_n6_α
.Lx14_0:
 .quad 10
# IR_DEREF variable -> value
 xchain0_n14_α:
 mov rdi, qword ptr [rsp + 624]
 mov rsi, qword ptr [rsp + 632]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n9_α
 mov qword ptr [rsp + 656], rax
 mov qword ptr [rsp + 664], rdx
 jmp xchain0_n16_α
 xchain0_n14_β:
 jmp xchain0_n9_α
 xchain0_n15_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+400] -> [zr+256]
 mov rax, qword ptr [rsp + 400]
 mov qword ptr [rsp + 256], rax
 mov rax, qword ptr [rsp + 408]
 mov qword ptr [rsp + 264], rax
# marshal arg1 = producer-box slot [zr+416] -> [zr+272]
 mov rax, qword ptr [rsp + 416]
 mov qword ptr [rsp + 272], rax
 mov rax, qword ptr [rsp + 424]
 mov qword ptr [rsp + 280], rax
  .section .rodata
  .Lbynamefn16: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn16]
 lea rsi, [rsp + 256]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 240], rax
 mov qword ptr [rsp + 248], rdx
 cmp eax, 99
 je xchain0_n6_α
 jmp xchain0_n17_α
 xchain0_n15_β:
 jmp xchain0_n6_α
# IR_LIT_INTEGER
 xchain0_n16_α:
 mov qword ptr [rsp + 672], 6
 mov rax, qword ptr [rip + .Lx17_0]
 mov qword ptr [rsp + 680], rax
 jmp xchain0_n18_α
 xchain0_n16_β:
 jmp xchain0_n9_α
.Lx17_0:
 .quad 10
# IR_LIT_STRING
 xchain0_n17_α:
 mov qword ptr [rsp + 464], 1
 mov rax, qword ptr [rip + .Lx18_0]
 mov qword ptr [rsp + 472], rax
 jmp xchain0_n19_α
 xchain0_n17_β:
 jmp main_γ
.Lx18_0:
 .quad .Lx18_0_s
.Lx18_0_s:
 .string "FAIL 1111/001: array default value"
 xchain0_n18_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+656] -> [zr+512]
 mov rax, qword ptr [rsp + 656]
 mov qword ptr [rsp + 512], rax
 mov rax, qword ptr [rsp + 664]
 mov qword ptr [rsp + 520], rax
# marshal arg1 = producer-box slot [zr+672] -> [zr+528]
 mov rax, qword ptr [rsp + 672]
 mov qword ptr [rsp + 528], rax
 mov rax, qword ptr [rsp + 680]
 mov qword ptr [rsp + 536], rax
  .section .rodata
  .Lbynamefn19: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn19]
 lea rsi, [rsp + 512]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 496], rax
 mov qword ptr [rsp + 504], rdx
 cmp eax, 99
 je xchain0_n9_α
 jmp xchain0_n20_α
 xchain0_n18_β:
 jmp xchain0_n9_α
# IR_ASSIGN gva
 xchain0_n19_α:
 mov rax, qword ptr [rsp + 464]
 mov rdx, qword ptr [rsp + 472]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rsp + 448], rax
 mov qword ptr [rsp + 456], rdx
 jmp main_γ
 xchain0_n19_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n20_α:
 mov qword ptr [rsp + 720], 1
 mov rax, qword ptr [rip + .Lx21_0]
 mov qword ptr [rsp + 728], rax
 jmp xchain0_n21_α
 xchain0_n20_β:
 jmp main_γ
.Lx21_0:
 .quad .Lx21_0_s
.Lx21_0_s:
 .string "FAIL 1111/002: default fills all slots"
# IR_ASSIGN gva
 xchain0_n21_α:
 mov rax, qword ptr [rsp + 720]
 mov rdx, qword ptr [rsp + 728]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rsp + 704], rax
 mov qword ptr [rsp + 712], rdx
 jmp main_γ
 xchain0_n21_β:
 jmp main_γ
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rsp + 808]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rsp + 808]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
add rsp, 65544
ret
