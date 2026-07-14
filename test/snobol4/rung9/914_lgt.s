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
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
  lea rdi, [rip + __gva_names]
  lea rsi, [rip + __gva]
  mov edx, 1
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
  mov qword ptr [r12 + 1032], rsp
 push rsi
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, [rsp + 8]
 mov qword ptr [r12 + 1024], rax
 pop rsi
main_α_body:
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [r12 + 160], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 168], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n4_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "abc"
# IR_LIT_STRING
 xchain0_n1_α:
 mov qword ptr [r12 + 192], 1
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [r12 + 200], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n4_α
.Lx2_0:
 .quad .Lx2_0_s
.Lx2_0_s:
 .string "xyz"
 xchain0_n2_α:
# BOX CALL lgt(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+160] -> [zr+112]
 mov rax, qword ptr [r12 + 160]
 mov qword ptr [r12 + 112], rax
 mov rax, qword ptr [r12 + 168]
 mov qword ptr [r12 + 120], rax
# marshal arg1 = producer-box slot [zr+192] -> [zr+128]
 mov rax, qword ptr [r12 + 192]
 mov qword ptr [r12 + 128], rax
 mov rax, qword ptr [r12 + 200]
 mov qword ptr [r12 + 136], rax
  .section .rodata
  .Lbynamefn3: .string "lgt"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn3]
 lea rsi, [r12 + 112]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 cmp eax, 99
 je xchain0_n4_α
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n4_α
# IR_LIT_STRING
 xchain0_n3_α:
 mov qword ptr [r12 + 240], 1
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [r12 + 248], rax
 jmp xchain0_n5_α
 xchain0_n3_β:
 jmp main_γ
.Lx4_0:
 .quad .Lx4_0_s
.Lx4_0_s:
 .string "FAIL 914/001: lgt(abc,xyz) should fail"
# IR_LIT_STRING
 xchain0_n4_α:
 mov qword ptr [r12 + 336], 1
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [r12 + 344], rax
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp xchain0_n9_α
.Lx5_0:
 .quad .Lx5_0_s
.Lx5_0_s:
 .string "abc"
# IR_ASSIGN gva
 xchain0_n5_α:
 mov rax, qword ptr [r12 + 240]
 mov rdx, qword ptr [r12 + 248]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 224], rax
 mov qword ptr [r12 + 232], rdx
 jmp main_γ
 xchain0_n5_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n6_α:
 mov qword ptr [r12 + 368], 1
 mov rax, qword ptr [rip + .Lx7_0]
 mov qword ptr [r12 + 376], rax
 jmp xchain0_n7_α
 xchain0_n6_β:
 jmp xchain0_n9_α
.Lx7_0:
 .quad .Lx7_0_s
.Lx7_0_s:
 .string "abc"
 xchain0_n7_α:
# BOX CALL lgt(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+336] -> [zr+288]
 mov rax, qword ptr [r12 + 336]
 mov qword ptr [r12 + 288], rax
 mov rax, qword ptr [r12 + 344]
 mov qword ptr [r12 + 296], rax
# marshal arg1 = producer-box slot [zr+368] -> [zr+304]
 mov rax, qword ptr [r12 + 368]
 mov qword ptr [r12 + 304], rax
 mov rax, qword ptr [r12 + 376]
 mov qword ptr [r12 + 312], rax
  .section .rodata
  .Lbynamefn8: .string "lgt"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn8]
 lea rsi, [r12 + 288]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 272], rax
 mov qword ptr [r12 + 280], rdx
 cmp eax, 99
 je xchain0_n9_α
 jmp xchain0_n8_α
 xchain0_n7_β:
 jmp xchain0_n9_α
# IR_LIT_STRING
 xchain0_n8_α:
 mov qword ptr [r12 + 416], 1
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [r12 + 424], rax
 jmp xchain0_n10_α
 xchain0_n8_β:
 jmp main_γ
.Lx9_0:
 .quad .Lx9_0_s
.Lx9_0_s:
 .string "FAIL 914/002: lgt(abc,abc) should fail"
# IR_LIT_STRING
 xchain0_n9_α:
 mov qword ptr [r12 + 512], 1
 mov rax, qword ptr [rip + .Lx10_0]
 mov qword ptr [r12 + 520], rax
 jmp xchain0_n11_α
 xchain0_n9_β:
 jmp xchain0_n14_α
.Lx10_0:
 .quad .Lx10_0_s
.Lx10_0_s:
 .string "xyz"
# IR_ASSIGN gva
 xchain0_n10_α:
 mov rax, qword ptr [r12 + 416]
 mov rdx, qword ptr [r12 + 424]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 400], rax
 mov qword ptr [r12 + 408], rdx
 jmp main_γ
 xchain0_n10_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n11_α:
 mov qword ptr [r12 + 544], 1
 mov rax, qword ptr [rip + .Lx12_0]
 mov qword ptr [r12 + 552], rax
 jmp xchain0_n12_α
 xchain0_n11_β:
 jmp xchain0_n14_α
.Lx12_0:
 .quad .Lx12_0_s
.Lx12_0_s:
 .string "abc"
 xchain0_n12_α:
# BOX CALL lgt(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+512] -> [zr+464]
 mov rax, qword ptr [r12 + 512]
 mov qword ptr [r12 + 464], rax
 mov rax, qword ptr [r12 + 520]
 mov qword ptr [r12 + 472], rax
# marshal arg1 = producer-box slot [zr+544] -> [zr+480]
 mov rax, qword ptr [r12 + 544]
 mov qword ptr [r12 + 480], rax
 mov rax, qword ptr [r12 + 552]
 mov qword ptr [r12 + 488], rax
  .section .rodata
  .Lbynamefn13: .string "lgt"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn13]
 lea rsi, [r12 + 464]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 448], rax
 mov qword ptr [r12 + 456], rdx
 cmp eax, 99
 je xchain0_n14_α
 jmp xchain0_n13_α
 xchain0_n12_β:
 jmp xchain0_n14_α
# IR_LIT_STRING
 xchain0_n13_α:
 mov qword ptr [r12 + 688], 1
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [r12 + 696], rax
 jmp xchain0_n15_α
 xchain0_n13_β:
 jmp xchain0_n19_α
.Lx14_0:
 .quad .Lx14_0_s
.Lx14_0_s:
 .string ""
# IR_LIT_STRING
 xchain0_n14_α:
 mov qword ptr [r12 + 592], 1
 mov rax, qword ptr [rip + .Lx15_0]
 mov qword ptr [r12 + 600], rax
 jmp xchain0_n16_α
 xchain0_n14_β:
 jmp main_γ
.Lx15_0:
 .quad .Lx15_0_s
.Lx15_0_s:
 .string "FAIL 914/003: lgt(xyz,abc) should succeed"
# IR_LIT_STRING
 xchain0_n15_α:
 mov qword ptr [r12 + 720], 1
 mov rax, qword ptr [rip + .Lx16_0]
 mov qword ptr [r12 + 728], rax
 jmp xchain0_n17_α
 xchain0_n15_β:
 jmp xchain0_n19_α
.Lx16_0:
 .quad .Lx16_0_s
.Lx16_0_s:
 .string "abc"
# IR_ASSIGN gva
 xchain0_n16_α:
 mov rax, qword ptr [r12 + 592]
 mov rdx, qword ptr [r12 + 600]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 576], rax
 mov qword ptr [r12 + 584], rdx
 jmp main_γ
 xchain0_n16_β:
 jmp main_γ
 xchain0_n17_α:
# BOX CALL lgt(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+688] -> [zr+640]
 mov rax, qword ptr [r12 + 688]
 mov qword ptr [r12 + 640], rax
 mov rax, qword ptr [r12 + 696]
 mov qword ptr [r12 + 648], rax
# marshal arg1 = producer-box slot [zr+720] -> [zr+656]
 mov rax, qword ptr [r12 + 720]
 mov qword ptr [r12 + 656], rax
 mov rax, qword ptr [r12 + 728]
 mov qword ptr [r12 + 664], rax
  .section .rodata
  .Lbynamefn18: .string "lgt"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn18]
 lea rsi, [r12 + 640]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 624], rax
 mov qword ptr [r12 + 632], rdx
 cmp eax, 99
 je xchain0_n19_α
 jmp xchain0_n18_α
 xchain0_n17_β:
 jmp xchain0_n19_α
# IR_LIT_STRING
 xchain0_n18_α:
 mov qword ptr [r12 + 768], 1
 mov rax, qword ptr [rip + .Lx19_0]
 mov qword ptr [r12 + 776], rax
 jmp xchain0_n20_α
 xchain0_n18_β:
 jmp main_γ
.Lx19_0:
 .quad .Lx19_0_s
.Lx19_0_s:
 .string "FAIL 914/004: lgt(null,abc) should fail"
# IR_LIT_STRING
 xchain0_n19_α:
 mov qword ptr [r12 + 864], 1
 mov rax, qword ptr [rip + .Lx20_0]
 mov qword ptr [r12 + 872], rax
 jmp xchain0_n21_α
 xchain0_n19_β:
 jmp xchain0_n24_α
.Lx20_0:
 .quad .Lx20_0_s
.Lx20_0_s:
 .string "abc"
# IR_ASSIGN gva
 xchain0_n20_α:
 mov rax, qword ptr [r12 + 768]
 mov rdx, qword ptr [r12 + 776]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 752], rax
 mov qword ptr [r12 + 760], rdx
 jmp main_γ
 xchain0_n20_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n21_α:
 mov qword ptr [r12 + 896], 1
 mov rax, qword ptr [rip + .Lx22_0]
 mov qword ptr [r12 + 904], rax
 jmp xchain0_n22_α
 xchain0_n21_β:
 jmp xchain0_n24_α
.Lx22_0:
 .quad .Lx22_0_s
.Lx22_0_s:
 .string ""
 xchain0_n22_α:
# BOX CALL lgt(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+864] -> [zr+816]
 mov rax, qword ptr [r12 + 864]
 mov qword ptr [r12 + 816], rax
 mov rax, qword ptr [r12 + 872]
 mov qword ptr [r12 + 824], rax
# marshal arg1 = producer-box slot [zr+896] -> [zr+832]
 mov rax, qword ptr [r12 + 896]
 mov qword ptr [r12 + 832], rax
 mov rax, qword ptr [r12 + 904]
 mov qword ptr [r12 + 840], rax
  .section .rodata
  .Lbynamefn23: .string "lgt"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn23]
 lea rsi, [r12 + 816]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 800], rax
 mov qword ptr [r12 + 808], rdx
 cmp eax, 99
 je xchain0_n24_α
 jmp xchain0_n23_α
 xchain0_n22_β:
 jmp xchain0_n24_α
# IR_LIT_STRING
 xchain0_n23_α:
 mov qword ptr [r12 + 992], 1
 mov rax, qword ptr [rip + .Lx24_0]
 mov qword ptr [r12 + 1000], rax
 jmp xchain0_n25_α
 xchain0_n23_β:
 jmp main_γ
.Lx24_0:
 .quad .Lx24_0_s
.Lx24_0_s:
 .string "PASS 914_lgt (5/5)"
# IR_LIT_STRING
 xchain0_n24_α:
 mov qword ptr [r12 + 944], 1
 mov rax, qword ptr [rip + .Lx25_0]
 mov qword ptr [r12 + 952], rax
 jmp xchain0_n26_α
 xchain0_n24_β:
 jmp main_γ
.Lx25_0:
 .quad .Lx25_0_s
.Lx25_0_s:
 .string "FAIL 914/005: lgt(abc,null) should succeed"
# IR_ASSIGN gva
 xchain0_n25_α:
 mov rax, qword ptr [r12 + 992]
 mov rdx, qword ptr [r12 + 1000]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 976], rax
 mov qword ptr [r12 + 984], rdx
 jmp main_γ
 xchain0_n25_β:
 jmp main_γ
# IR_ASSIGN gva
 xchain0_n26_α:
 mov rax, qword ptr [r12 + 944]
 mov rdx, qword ptr [r12 + 952]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 928], rax
 mov qword ptr [r12 + 936], rdx
 jmp main_γ
 xchain0_n26_β:
 jmp main_γ
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [r12 + 1032]
pop r12
ret
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [r12 + 1024]
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
main_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, qword ptr [r12 + 1032]
pop r12
ret
