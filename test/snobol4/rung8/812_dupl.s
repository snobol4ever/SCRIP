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
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 752], rax
 pop rsi
main_α_body:
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [r12 + 112], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 120], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n4_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "abc"
# IR_LIT_INTEGER
 xchain0_n1_α:
 mov qword ptr [r12 + 128], 6
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [r12 + 136], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n4_α
.Lx2_0:
 .quad 2
 xchain0_n2_α:
# BOX IR_CALL dupl(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+112] -> [r12+80]
 mov rax, qword ptr [r12 + 112]
 mov qword ptr [r12 + 80], rax
 mov rax, qword ptr [r12 + 120]
 mov qword ptr [r12 + 88], rax
# marshal arg1 = producer-box slot [r12+128] -> [r12+96]
 mov rax, qword ptr [r12 + 128]
 mov qword ptr [r12 + 96], rax
 mov rax, qword ptr [r12 + 136]
 mov qword ptr [r12 + 104], rax
  .section .rodata
  .Lbynamefn4: .string "dupl"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn4]
 lea rsi, [r12 + 80]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 64], rax
 mov qword ptr [r12 + 72], rdx
 cmp eax, 99
 je xchain0_n4_α
 jmp xchain0_n3_α
xchain0_n2_β:
 jmp xchain0_n4_α
# IR_LIT_STRING
 xchain0_n3_α:
 mov qword ptr [r12 + 144], 1
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [r12 + 152], rax
 jmp xchain0_n5_α
 xchain0_n3_β:
 jmp xchain0_n4_α
.Lx5_0:
 .quad .Lx5_0_s
.Lx5_0_s:
 .string "abcabc"
# IR_LIT_STRING
 xchain0_n4_α:
 mov qword ptr [r12 + 288], 1
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [r12 + 296], rax
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp xchain0_n11_α
.Lx6_0:
 .quad .Lx6_0_s
.Lx6_0_s:
 .string ""
 xchain0_n5_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+64] -> [r12+32]
 mov rax, qword ptr [r12 + 64]
 mov qword ptr [r12 + 32], rax
 mov rax, qword ptr [r12 + 72]
 mov qword ptr [r12 + 40], rax
# marshal arg1 = producer-box slot [r12+144] -> [r12+48]
 mov rax, qword ptr [r12 + 144]
 mov qword ptr [r12 + 48], rax
 mov rax, qword ptr [r12 + 152]
 mov qword ptr [r12 + 56], rax
  .section .rodata
  .Lbynamefn8: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn8]
 lea rsi, [r12 + 32]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 cmp eax, 99
 je xchain0_n4_α
 jmp xchain0_n7_α
xchain0_n5_β:
 jmp xchain0_n4_α
# IR_LIT_INTEGER
 xchain0_n6_α:
 mov qword ptr [r12 + 304], 6
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [r12 + 312], rax
 jmp xchain0_n8_α
 xchain0_n6_β:
 jmp xchain0_n11_α
.Lx9_0:
 .quad 10
# IR_LIT_STRING
 xchain0_n7_α:
 mov qword ptr [r12 + 176], 1
 mov rax, qword ptr [rip + .Lx10_0]
 mov qword ptr [r12 + 184], rax
 jmp xchain0_n9_α
 xchain0_n7_β:
 jmp main_γ
.Lx10_0:
 .quad .Lx10_0_s
.Lx10_0_s:
 .string "FAIL 812/001: dupl string x2"
 xchain0_n8_α:
# BOX IR_CALL dupl(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+288] -> [r12+256]
 mov rax, qword ptr [r12 + 288]
 mov qword ptr [r12 + 256], rax
 mov rax, qword ptr [r12 + 296]
 mov qword ptr [r12 + 264], rax
# marshal arg1 = producer-box slot [r12+304] -> [r12+272]
 mov rax, qword ptr [r12 + 304]
 mov qword ptr [r12 + 272], rax
 mov rax, qword ptr [r12 + 312]
 mov qword ptr [r12 + 280], rax
  .section .rodata
  .Lbynamefn12: .string "dupl"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn12]
 lea rsi, [r12 + 256]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 240], rax
 mov qword ptr [r12 + 248], rdx
 cmp eax, 99
 je xchain0_n11_α
 jmp xchain0_n10_α
xchain0_n8_β:
 jmp xchain0_n11_α
# IR_ASSIGN gva
 xchain0_n9_α:
 mov rax, qword ptr [r12 + 176]
 mov rdx, qword ptr [r12 + 184]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 160], rax
 mov qword ptr [r12 + 168], rdx
 jmp main_γ
 xchain0_n9_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n10_α:
 mov qword ptr [r12 + 320], 1
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [r12 + 328], rax
 jmp xchain0_n12_α
 xchain0_n10_β:
 jmp xchain0_n11_α
.Lx14_0:
 .quad .Lx14_0_s
.Lx14_0_s:
 .string ""
# IR_LIT_STRING
 xchain0_n11_α:
 mov qword ptr [r12 + 464], 1
 mov rax, qword ptr [rip + .Lx15_0]
 mov qword ptr [r12 + 472], rax
 jmp xchain0_n13_α
 xchain0_n11_β:
 jmp xchain0_n18_α
.Lx15_0:
 .quad .Lx15_0_s
.Lx15_0_s:
 .string "abcdefg"
 xchain0_n12_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+240] -> [r12+208]
 mov rax, qword ptr [r12 + 240]
 mov qword ptr [r12 + 208], rax
 mov rax, qword ptr [r12 + 248]
 mov qword ptr [r12 + 216], rax
# marshal arg1 = producer-box slot [r12+320] -> [r12+224]
 mov rax, qword ptr [r12 + 320]
 mov qword ptr [r12 + 224], rax
 mov rax, qword ptr [r12 + 328]
 mov qword ptr [r12 + 232], rax
  .section .rodata
  .Lbynamefn17: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn17]
 lea rsi, [r12 + 208]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 192], rax
 mov qword ptr [r12 + 200], rdx
 cmp eax, 99
 je xchain0_n11_α
 jmp xchain0_n14_α
xchain0_n12_β:
 jmp xchain0_n11_α
# IR_LIT_INTEGER
 xchain0_n13_α:
 mov qword ptr [r12 + 480], 6
 mov rax, qword ptr [rip + .Lx18_0]
 mov qword ptr [r12 + 488], rax
 jmp xchain0_n15_α
 xchain0_n13_β:
 jmp xchain0_n18_α
.Lx18_0:
 .quad 0
# IR_LIT_STRING
 xchain0_n14_α:
 mov qword ptr [r12 + 352], 1
 mov rax, qword ptr [rip + .Lx19_0]
 mov qword ptr [r12 + 360], rax
 jmp xchain0_n16_α
 xchain0_n14_β:
 jmp main_γ
.Lx19_0:
 .quad .Lx19_0_s
.Lx19_0_s:
 .string "FAIL 812/002: dupl null is null"
 xchain0_n15_α:
# BOX IR_CALL dupl(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+464] -> [r12+432]
 mov rax, qword ptr [r12 + 464]
 mov qword ptr [r12 + 432], rax
 mov rax, qword ptr [r12 + 472]
 mov qword ptr [r12 + 440], rax
# marshal arg1 = producer-box slot [r12+480] -> [r12+448]
 mov rax, qword ptr [r12 + 480]
 mov qword ptr [r12 + 448], rax
 mov rax, qword ptr [r12 + 488]
 mov qword ptr [r12 + 456], rax
  .section .rodata
  .Lbynamefn21: .string "dupl"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn21]
 lea rsi, [r12 + 432]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 416], rax
 mov qword ptr [r12 + 424], rdx
 cmp eax, 99
 je xchain0_n18_α
 jmp xchain0_n17_α
xchain0_n15_β:
 jmp xchain0_n18_α
# IR_ASSIGN gva
 xchain0_n16_α:
 mov rax, qword ptr [r12 + 352]
 mov rdx, qword ptr [r12 + 360]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 336], rax
 mov qword ptr [r12 + 344], rdx
 jmp main_γ
 xchain0_n16_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n17_α:
 mov qword ptr [r12 + 496], 1
 mov rax, qword ptr [rip + .Lx23_0]
 mov qword ptr [r12 + 504], rax
 jmp xchain0_n19_α
 xchain0_n17_β:
 jmp xchain0_n18_α
.Lx23_0:
 .quad .Lx23_0_s
.Lx23_0_s:
 .string ""
# IR_LIT_INTEGER
 xchain0_n18_α:
 mov qword ptr [r12 + 640], 6
 mov rax, qword ptr [rip + .Lx24_0]
 mov qword ptr [r12 + 648], rax
 jmp xchain0_n20_α
 xchain0_n18_β:
 jmp xchain0_n25_α
.Lx24_0:
 .quad 1
 xchain0_n19_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+416] -> [r12+384]
 mov rax, qword ptr [r12 + 416]
 mov qword ptr [r12 + 384], rax
 mov rax, qword ptr [r12 + 424]
 mov qword ptr [r12 + 392], rax
# marshal arg1 = producer-box slot [r12+496] -> [r12+400]
 mov rax, qword ptr [r12 + 496]
 mov qword ptr [r12 + 400], rax
 mov rax, qword ptr [r12 + 504]
 mov qword ptr [r12 + 408], rax
  .section .rodata
  .Lbynamefn26: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn26]
 lea rsi, [r12 + 384]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 368], rax
 mov qword ptr [r12 + 376], rdx
 cmp eax, 99
 je xchain0_n18_α
 jmp xchain0_n21_α
xchain0_n19_β:
 jmp xchain0_n18_α
# IR_LIT_INTEGER
 xchain0_n20_α:
 mov qword ptr [r12 + 656], 6
 mov rax, qword ptr [rip + .Lx27_0]
 mov qword ptr [r12 + 664], rax
 jmp xchain0_n22_α
 xchain0_n20_β:
 jmp xchain0_n25_α
.Lx27_0:
 .quad 10
# IR_LIT_STRING
 xchain0_n21_α:
 mov qword ptr [r12 + 528], 1
 mov rax, qword ptr [rip + .Lx28_0]
 mov qword ptr [r12 + 536], rax
 jmp xchain0_n23_α
 xchain0_n21_β:
 jmp main_γ
.Lx28_0:
 .quad .Lx28_0_s
.Lx28_0_s:
 .string "FAIL 812/003: dupl x0 is null"
 xchain0_n22_α:
# BOX IR_CALL dupl(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+640] -> [r12+608]
 mov rax, qword ptr [r12 + 640]
 mov qword ptr [r12 + 608], rax
 mov rax, qword ptr [r12 + 648]
 mov qword ptr [r12 + 616], rax
# marshal arg1 = producer-box slot [r12+656] -> [r12+624]
 mov rax, qword ptr [r12 + 656]
 mov qword ptr [r12 + 624], rax
 mov rax, qword ptr [r12 + 664]
 mov qword ptr [r12 + 632], rax
  .section .rodata
  .Lbynamefn30: .string "dupl"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn30]
 lea rsi, [r12 + 608]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 592], rax
 mov qword ptr [r12 + 600], rdx
 cmp eax, 99
 je xchain0_n25_α
 jmp xchain0_n24_α
xchain0_n22_β:
 jmp xchain0_n25_α
# IR_ASSIGN gva
 xchain0_n23_α:
 mov rax, qword ptr [r12 + 528]
 mov rdx, qword ptr [r12 + 536]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 512], rax
 mov qword ptr [r12 + 520], rdx
 jmp main_γ
 xchain0_n23_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n24_α:
 mov qword ptr [r12 + 672], 1
 mov rax, qword ptr [rip + .Lx32_0]
 mov qword ptr [r12 + 680], rax
 jmp xchain0_n26_α
 xchain0_n24_β:
 jmp xchain0_n25_α
.Lx32_0:
 .quad .Lx32_0_s
.Lx32_0_s:
 .string "1111111111"
# IR_LIT_STRING
 xchain0_n25_α:
 mov qword ptr [r12 + 736], 1
 mov rax, qword ptr [rip + .Lx33_0]
 mov qword ptr [r12 + 744], rax
 jmp xchain0_n27_α
 xchain0_n25_β:
 jmp main_γ
.Lx33_0:
 .quad .Lx33_0_s
.Lx33_0_s:
 .string "PASS 812_dupl (4/4)"
 xchain0_n26_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+592] -> [r12+560]
 mov rax, qword ptr [r12 + 592]
 mov qword ptr [r12 + 560], rax
 mov rax, qword ptr [r12 + 600]
 mov qword ptr [r12 + 568], rax
# marshal arg1 = producer-box slot [r12+672] -> [r12+576]
 mov rax, qword ptr [r12 + 672]
 mov qword ptr [r12 + 576], rax
 mov rax, qword ptr [r12 + 680]
 mov qword ptr [r12 + 584], rax
  .section .rodata
  .Lbynamefn35: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn35]
 lea rsi, [r12 + 560]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 544], rax
 mov qword ptr [r12 + 552], rdx
 cmp eax, 99
 je xchain0_n25_α
 jmp xchain0_n28_α
xchain0_n26_β:
 jmp xchain0_n25_α
# IR_ASSIGN gva
 xchain0_n27_α:
 mov rax, qword ptr [r12 + 736]
 mov rdx, qword ptr [r12 + 744]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 720], rax
 mov qword ptr [r12 + 728], rdx
 jmp main_γ
 xchain0_n27_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n28_α:
 mov qword ptr [r12 + 704], 1
 mov rax, qword ptr [rip + .Lx37_0]
 mov qword ptr [r12 + 712], rax
 jmp xchain0_n29_α
 xchain0_n28_β:
 jmp main_γ
.Lx37_0:
 .quad .Lx37_0_s
.Lx37_0_s:
 .string "FAIL 812/004: dupl integer coerce"
# IR_ASSIGN gva
 xchain0_n29_α:
 mov rax, qword ptr [r12 + 704]
 mov rdx, qword ptr [r12 + 712]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 688], rax
 mov qword ptr [r12 + 696], rdx
 jmp main_γ
 xchain0_n29_β:
 jmp main_γ
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 752]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
main_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
pop r12
ret
