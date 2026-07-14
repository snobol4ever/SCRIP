  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "b"
  .Lgvan1: .string "output"
  .align 8
__gva_names:
  .quad .Lgvan0
  .quad .Lgvan1
  .section .bss
  .align 16
__gva: .space 32, 0
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
  lea rdi, [rip + __gva_names]
  lea rsi, [rip + __gva]
  mov edx, 2
  call gva_register@PLT
  mov rbx, rax
  sub rsp, 65536
  mov rdi, rsp
  mov ecx, 8192
  xor eax, eax
  rep stosq
  mov rdi, rsp
  xor esi, esi
  call main_α
  xor eax, eax
  add rsp, 65536
  add rsp, 24
  ret
main_α:
#=======================================================================================================================
    .global main_α
    .global main_β
    .global main_γ
    .global main_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  mov qword ptr [r12 + 808], rsp
 push rsi
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, [rsp + 8]
 mov qword ptr [r12 + 800], rax
 pop rsi
main_α_body:
# IR_LIT_INTEGER
 xchain0_n0_α:
 mov qword ptr [r12 + 176], 6
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 184], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n4_α
.Lx1_0:
 .quad 3
# IR_LIT_INTEGER
 xchain0_n1_α:
 mov qword ptr [r12 + 208], 6
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [r12 + 216], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n4_α
.Lx2_0:
 .quad 10
 xchain0_n2_α:
# BOX CALL array(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+176] -> [zr+128]
 mov rax, qword ptr [r12 + 176]
 mov qword ptr [r12 + 128], rax
 mov rax, qword ptr [r12 + 184]
 mov qword ptr [r12 + 136], rax
# marshal arg1 = producer-box slot [zr+208] -> [zr+144]
 mov rax, qword ptr [r12 + 208]
 mov qword ptr [r12 + 144], rax
 mov rax, qword ptr [r12 + 216]
 mov qword ptr [r12 + 152], rax
  .section .rodata
  .Lbynamefn3: .string "array"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn3]
 lea rsi, [r12 + 128]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 112], rax
 mov qword ptr [r12 + 120], rdx
 cmp eax, 99
 je xchain0_n4_α
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n4_α
# IR_ASSIGN gva
 xchain0_n3_α:
 mov rax, qword ptr [r12 + 112]
 mov rdx, qword ptr [r12 + 120]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp xchain0_n4_α
# IR_VAR
 xchain0_n4_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 304], rax
 mov qword ptr [r12 + 312], rdx
 jmp xchain0_n5_α
 xchain0_n4_β:
 jmp xchain0_n6_α
# IR_LIT_INTEGER
 xchain0_n5_α:
 mov qword ptr [r12 + 336], 6
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [r12 + 344], rax
 jmp xchain0_n7_α
 xchain0_n5_β:
 jmp xchain0_n6_α
.Lx6_0:
 .quad 2
# IR_VAR
 xchain0_n6_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 560], rax
 mov qword ptr [r12 + 568], rdx
 jmp xchain0_n8_α
 xchain0_n6_β:
 jmp xchain0_n9_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n7_α:
 mov rdi, qword ptr [r12 + 304]
 mov rsi, qword ptr [r12 + 312]
 mov rdx, qword ptr [r12 + 336]
 mov rcx, qword ptr [r12 + 344]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n6_α
 mov qword ptr [r12 + 368], rax
 mov qword ptr [r12 + 376], rdx
 jmp xchain0_n10_α
 xchain0_n7_β:
 jmp xchain0_n6_α
# IR_LIT_INTEGER
 xchain0_n8_α:
 mov qword ptr [r12 + 592], 6
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [r12 + 600], rax
 jmp xchain0_n11_α
 xchain0_n8_β:
 jmp xchain0_n9_α
.Lx9_0:
 .quad 1
# IR_LIT_STRING
 xchain0_n9_α:
 mov qword ptr [r12 + 768], 1
 mov rax, qword ptr [rip + .Lx10_0]
 mov qword ptr [r12 + 776], rax
 jmp xchain0_n12_α
 xchain0_n9_β:
 jmp main_γ
.Lx10_0:
 .quad .Lx10_0_s
.Lx10_0_s:
 .string "PASS 1111_array_default (2/2)"
# IR_DEREF variable -> value
 xchain0_n10_α:
 mov rdi, qword ptr [r12 + 368]
 mov rsi, qword ptr [r12 + 376]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n6_α
 mov qword ptr [r12 + 400], rax
 mov qword ptr [r12 + 408], rdx
 jmp xchain0_n13_α
 xchain0_n10_β:
 jmp xchain0_n6_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n11_α:
 mov rdi, qword ptr [r12 + 560]
 mov rsi, qword ptr [r12 + 568]
 mov rdx, qword ptr [r12 + 592]
 mov rcx, qword ptr [r12 + 600]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n9_α
 mov qword ptr [r12 + 624], rax
 mov qword ptr [r12 + 632], rdx
 jmp xchain0_n14_α
 xchain0_n11_β:
 jmp xchain0_n9_α
# IR_ASSIGN gva
 xchain0_n12_α:
 mov rax, qword ptr [r12 + 768]
 mov rdx, qword ptr [r12 + 776]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 752], rax
 mov qword ptr [r12 + 760], rdx
 jmp main_γ
 xchain0_n12_β:
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n13_α:
 mov qword ptr [r12 + 416], 6
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [r12 + 424], rax
 jmp xchain0_n15_α
 xchain0_n13_β:
 jmp xchain0_n6_α
.Lx14_0:
 .quad 10
# IR_DEREF variable -> value
 xchain0_n14_α:
 mov rdi, qword ptr [r12 + 624]
 mov rsi, qword ptr [r12 + 632]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n9_α
 mov qword ptr [r12 + 656], rax
 mov qword ptr [r12 + 664], rdx
 jmp xchain0_n16_α
 xchain0_n14_β:
 jmp xchain0_n9_α
 xchain0_n15_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+400] -> [zr+256]
 mov rax, qword ptr [r12 + 400]
 mov qword ptr [r12 + 256], rax
 mov rax, qword ptr [r12 + 408]
 mov qword ptr [r12 + 264], rax
# marshal arg1 = producer-box slot [zr+416] -> [zr+272]
 mov rax, qword ptr [r12 + 416]
 mov qword ptr [r12 + 272], rax
 mov rax, qword ptr [r12 + 424]
 mov qword ptr [r12 + 280], rax
  .section .rodata
  .Lbynamefn16: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn16]
 lea rsi, [r12 + 256]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 240], rax
 mov qword ptr [r12 + 248], rdx
 cmp eax, 99
 je xchain0_n6_α
 jmp xchain0_n17_α
 xchain0_n15_β:
 jmp xchain0_n6_α
# IR_LIT_INTEGER
 xchain0_n16_α:
 mov qword ptr [r12 + 672], 6
 mov rax, qword ptr [rip + .Lx17_0]
 mov qword ptr [r12 + 680], rax
 jmp xchain0_n18_α
 xchain0_n16_β:
 jmp xchain0_n9_α
.Lx17_0:
 .quad 10
# IR_LIT_STRING
 xchain0_n17_α:
 mov qword ptr [r12 + 464], 1
 mov rax, qword ptr [rip + .Lx18_0]
 mov qword ptr [r12 + 472], rax
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
 mov rax, qword ptr [r12 + 656]
 mov qword ptr [r12 + 512], rax
 mov rax, qword ptr [r12 + 664]
 mov qword ptr [r12 + 520], rax
# marshal arg1 = producer-box slot [zr+672] -> [zr+528]
 mov rax, qword ptr [r12 + 672]
 mov qword ptr [r12 + 528], rax
 mov rax, qword ptr [r12 + 680]
 mov qword ptr [r12 + 536], rax
  .section .rodata
  .Lbynamefn19: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn19]
 lea rsi, [r12 + 512]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 496], rax
 mov qword ptr [r12 + 504], rdx
 cmp eax, 99
 je xchain0_n9_α
 jmp xchain0_n20_α
 xchain0_n18_β:
 jmp xchain0_n9_α
# IR_ASSIGN gva
 xchain0_n19_α:
 mov rax, qword ptr [r12 + 464]
 mov rdx, qword ptr [r12 + 472]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 448], rax
 mov qword ptr [r12 + 456], rdx
 jmp main_γ
 xchain0_n19_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n20_α:
 mov qword ptr [r12 + 720], 1
 mov rax, qword ptr [rip + .Lx21_0]
 mov qword ptr [r12 + 728], rax
 jmp xchain0_n21_α
 xchain0_n20_β:
 jmp main_γ
.Lx21_0:
 .quad .Lx21_0_s
.Lx21_0_s:
 .string "FAIL 1111/002: default fills all slots"
# IR_ASSIGN gva
 xchain0_n21_α:
 mov rax, qword ptr [r12 + 720]
 mov rdx, qword ptr [r12 + 728]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 704], rax
 mov qword ptr [r12 + 712], rdx
 jmp main_γ
 xchain0_n21_β:
 jmp main_γ
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [r12 + 808]
pop r12
ret
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [r12 + 800]
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
main_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, qword ptr [r12 + 808]
pop r12
ret
