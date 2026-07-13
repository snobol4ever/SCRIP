  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "output"
  .align 8
__gva_names:
  .quad .Lgvan0
  .section .bss
  .align 16
__gva: .space 16, 0
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  push rbp
  mov rbp, rsp
  push rdi
  push rsi
  call core_lib_init@PLT
  lea rdi, [rip + __gva_names]
  lea rsi, [rip + __gva]
  mov edx, 1
  call gva_register@PLT
  mov rbx, rax
  call rt_frame@PLT
  mov rdi, rax
  xor esi, esi
  call main_α
  xor eax, eax
  mov rsp, rbp
  pop rbp
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
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, [rsp + 8]
 mov qword ptr [r12 + 816], rax
 pop rsi
main_α_body:
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [r12 + 208], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 216], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n3_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "abc"
 xchain0_n1_α:
# BOX CALL size(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+208] -> [zr+176]
 mov rax, qword ptr [r12 + 208]
 mov qword ptr [r12 + 176], rax
 mov rax, qword ptr [r12 + 216]
 mov qword ptr [r12 + 184], rax
  .section .rodata
  .Lbynamefn2: .string "size"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn2]
 lea rsi, [r12 + 176]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 160], rax
 mov qword ptr [r12 + 168], rdx
 cmp eax, 99
 je xchain0_n3_α
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n3_α
# IR_LIT_INTEGER
 xchain0_n2_α:
 mov qword ptr [r12 + 240], 6
 mov rax, qword ptr [rip + .Lx3_0]
 mov qword ptr [r12 + 248], rax
 jmp xchain0_n4_α
 xchain0_n2_β:
 jmp xchain0_n3_α
.Lx3_0:
 .quad 3
# IR_LIT_INTEGER
 xchain0_n3_α:
 mov qword ptr [r12 + 432], 6
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [r12 + 440], rax
 jmp xchain0_n5_α
 xchain0_n3_β:
 jmp xchain0_n8_α
.Lx4_0:
 .quad 12
 xchain0_n4_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+160] -> [zr+112]
 mov rax, qword ptr [r12 + 160]
 mov qword ptr [r12 + 112], rax
 mov rax, qword ptr [r12 + 168]
 mov qword ptr [r12 + 120], rax
# marshal arg1 = producer-box slot [zr+240] -> [zr+128]
 mov rax, qword ptr [r12 + 240]
 mov qword ptr [r12 + 128], rax
 mov rax, qword ptr [r12 + 248]
 mov qword ptr [r12 + 136], rax
  .section .rodata
  .Lbynamefn5: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn5]
 lea rsi, [r12 + 112]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 cmp eax, 99
 je xchain0_n3_α
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp xchain0_n3_α
 xchain0_n5_α:
# BOX CALL size(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+432] -> [zr+400]
 mov rax, qword ptr [r12 + 432]
 mov qword ptr [r12 + 400], rax
 mov rax, qword ptr [r12 + 440]
 mov qword ptr [r12 + 408], rax
  .section .rodata
  .Lbynamefn6: .string "size"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn6]
 lea rsi, [r12 + 400]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 384], rax
 mov qword ptr [r12 + 392], rdx
 cmp eax, 99
 je xchain0_n8_α
 jmp xchain0_n7_α
 xchain0_n5_β:
 jmp xchain0_n8_α
# IR_LIT_STRING
 xchain0_n6_α:
 mov qword ptr [r12 + 288], 1
 mov rax, qword ptr [rip + .Lx7_0]
 mov qword ptr [r12 + 296], rax
 jmp xchain0_n9_α
 xchain0_n6_β:
 jmp main_γ
.Lx7_0:
 .quad .Lx7_0_s
.Lx7_0_s:
 .string "FAIL 811/001: size of 3-char string"
# IR_LIT_INTEGER
 xchain0_n7_α:
 mov qword ptr [r12 + 464], 6
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [r12 + 472], rax
 jmp xchain0_n10_α
 xchain0_n7_β:
 jmp xchain0_n8_α
.Lx8_0:
 .quad 2
# IR_LIT_STRING
 xchain0_n8_α:
 mov qword ptr [r12 + 656], 1
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [r12 + 664], rax
 jmp xchain0_n11_α
 xchain0_n8_β:
 jmp xchain0_n14_α
.Lx9_0:
 .quad .Lx9_0_s
.Lx9_0_s:
 .string ""
# IR_ASSIGN gva
 xchain0_n9_α:
 mov rax, qword ptr [r12 + 288]
 mov rdx, qword ptr [r12 + 296]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 272], rax
 mov qword ptr [r12 + 280], rdx
 jmp main_γ
 xchain0_n9_β:
 jmp main_γ
 xchain0_n10_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+384] -> [zr+336]
 mov rax, qword ptr [r12 + 384]
 mov qword ptr [r12 + 336], rax
 mov rax, qword ptr [r12 + 392]
 mov qword ptr [r12 + 344], rax
# marshal arg1 = producer-box slot [zr+464] -> [zr+352]
 mov rax, qword ptr [r12 + 464]
 mov qword ptr [r12 + 352], rax
 mov rax, qword ptr [r12 + 472]
 mov qword ptr [r12 + 360], rax
  .section .rodata
  .Lbynamefn11: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn11]
 lea rsi, [r12 + 336]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 320], rax
 mov qword ptr [r12 + 328], rdx
 cmp eax, 99
 je xchain0_n8_α
 jmp xchain0_n12_α
 xchain0_n10_β:
 jmp xchain0_n8_α
 xchain0_n11_α:
# BOX CALL size(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+656] -> [zr+624]
 mov rax, qword ptr [r12 + 656]
 mov qword ptr [r12 + 624], rax
 mov rax, qword ptr [r12 + 664]
 mov qword ptr [r12 + 632], rax
  .section .rodata
  .Lbynamefn12: .string "size"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn12]
 lea rsi, [r12 + 624]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 608], rax
 mov qword ptr [r12 + 616], rdx
 cmp eax, 99
 je xchain0_n14_α
 jmp xchain0_n13_α
 xchain0_n11_β:
 jmp xchain0_n14_α
# IR_LIT_STRING
 xchain0_n12_α:
 mov qword ptr [r12 + 512], 1
 mov rax, qword ptr [rip + .Lx13_0]
 mov qword ptr [r12 + 520], rax
 jmp xchain0_n15_α
 xchain0_n12_β:
 jmp main_γ
.Lx13_0:
 .quad .Lx13_0_s
.Lx13_0_s:
 .string "FAIL 811/002: size of integer 12 = 2 digits"
# IR_LIT_INTEGER
 xchain0_n13_α:
 mov qword ptr [r12 + 688], 6
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [r12 + 696], rax
 jmp xchain0_n16_α
 xchain0_n13_β:
 jmp xchain0_n14_α
.Lx14_0:
 .quad 0
# IR_LIT_STRING
 xchain0_n14_α:
 mov qword ptr [r12 + 784], 1
 mov rax, qword ptr [rip + .Lx15_0]
 mov qword ptr [r12 + 792], rax
 jmp xchain0_n17_α
 xchain0_n14_β:
 jmp main_γ
.Lx15_0:
 .quad .Lx15_0_s
.Lx15_0_s:
 .string "PASS 811_size (3/3)"
# IR_ASSIGN gva
 xchain0_n15_α:
 mov rax, qword ptr [r12 + 512]
 mov rdx, qword ptr [r12 + 520]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 496], rax
 mov qword ptr [r12 + 504], rdx
 jmp main_γ
 xchain0_n15_β:
 jmp main_γ
 xchain0_n16_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+608] -> [zr+560]
 mov rax, qword ptr [r12 + 608]
 mov qword ptr [r12 + 560], rax
 mov rax, qword ptr [r12 + 616]
 mov qword ptr [r12 + 568], rax
# marshal arg1 = producer-box slot [zr+688] -> [zr+576]
 mov rax, qword ptr [r12 + 688]
 mov qword ptr [r12 + 576], rax
 mov rax, qword ptr [r12 + 696]
 mov qword ptr [r12 + 584], rax
  .section .rodata
  .Lbynamefn17: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn17]
 lea rsi, [r12 + 560]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 544], rax
 mov qword ptr [r12 + 552], rdx
 cmp eax, 99
 je xchain0_n14_α
 jmp xchain0_n18_α
 xchain0_n16_β:
 jmp xchain0_n14_α
# IR_ASSIGN gva
 xchain0_n17_α:
 mov rax, qword ptr [r12 + 784]
 mov rdx, qword ptr [r12 + 792]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 768], rax
 mov qword ptr [r12 + 776], rdx
 jmp main_γ
 xchain0_n17_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n18_α:
 mov qword ptr [r12 + 736], 1
 mov rax, qword ptr [rip + .Lx19_0]
 mov qword ptr [r12 + 744], rax
 jmp xchain0_n19_α
 xchain0_n18_β:
 jmp main_γ
.Lx19_0:
 .quad .Lx19_0_s
.Lx19_0_s:
 .string "FAIL 811/003: size of null = 0"
# IR_ASSIGN gva
 xchain0_n19_α:
 mov rax, qword ptr [r12 + 736]
 mov rdx, qword ptr [r12 + 744]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 720], rax
 mov qword ptr [r12 + 728], rdx
 jmp main_γ
 xchain0_n19_β:
 jmp main_γ
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [r12 + 816]
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
main_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
