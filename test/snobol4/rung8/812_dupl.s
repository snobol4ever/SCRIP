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
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 800], rax
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
# IR_LIT_INTEGER
 xchain0_n1_α:
 mov qword ptr [r12 + 176], 6
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [r12 + 184], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n4_α
.Lx2_0:
 .quad 2
 xchain0_n2_α:
# BOX IR_CALL dupl(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+160] -> [zr+128]
 mov rax, qword ptr [r12 + 160]
 mov qword ptr [r12 + 128], rax
 mov rax, qword ptr [r12 + 168]
 mov qword ptr [r12 + 136], rax
# marshal arg1 = producer-box slot [zr+176] -> [zr+144]
 mov rax, qword ptr [r12 + 176]
 mov qword ptr [r12 + 144], rax
 mov rax, qword ptr [r12 + 184]
 mov qword ptr [r12 + 152], rax
  .section .rodata
  .Lbynamefn3: .string "dupl"
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
# IR_LIT_STRING
 xchain0_n3_α:
 mov qword ptr [r12 + 192], 1
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [r12 + 200], rax
 jmp xchain0_n5_α
 xchain0_n3_β:
 jmp xchain0_n4_α
.Lx4_0:
 .quad .Lx4_0_s
.Lx4_0_s:
 .string "abcabc"
# IR_LIT_STRING
 xchain0_n4_α:
 mov qword ptr [r12 + 336], 1
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [r12 + 344], rax
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp xchain0_n11_α
.Lx5_0:
 .quad .Lx5_0_s
.Lx5_0_s:
 .string ""
 xchain0_n5_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+112] -> [zr+80]
 mov rax, qword ptr [r12 + 112]
 mov qword ptr [r12 + 80], rax
 mov rax, qword ptr [r12 + 120]
 mov qword ptr [r12 + 88], rax
# marshal arg1 = producer-box slot [zr+192] -> [zr+96]
 mov rax, qword ptr [r12 + 192]
 mov qword ptr [r12 + 96], rax
 mov rax, qword ptr [r12 + 200]
 mov qword ptr [r12 + 104], rax
  .section .rodata
  .Lbynamefn6: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn6]
 lea rsi, [r12 + 80]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 64], rax
 mov qword ptr [r12 + 72], rdx
 cmp eax, 99
 je xchain0_n4_α
 jmp xchain0_n7_α
 xchain0_n5_β:
 jmp xchain0_n4_α
# IR_LIT_INTEGER
 xchain0_n6_α:
 mov qword ptr [r12 + 352], 6
 mov rax, qword ptr [rip + .Lx7_0]
 mov qword ptr [r12 + 360], rax
 jmp xchain0_n8_α
 xchain0_n6_β:
 jmp xchain0_n11_α
.Lx7_0:
 .quad 10
# IR_LIT_STRING
 xchain0_n7_α:
 mov qword ptr [r12 + 224], 1
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [r12 + 232], rax
 jmp xchain0_n9_α
 xchain0_n7_β:
 jmp main_γ
.Lx8_0:
 .quad .Lx8_0_s
.Lx8_0_s:
 .string "FAIL 812/001: dupl string x2"
 xchain0_n8_α:
# BOX IR_CALL dupl(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+336] -> [zr+304]
 mov rax, qword ptr [r12 + 336]
 mov qword ptr [r12 + 304], rax
 mov rax, qword ptr [r12 + 344]
 mov qword ptr [r12 + 312], rax
# marshal arg1 = producer-box slot [zr+352] -> [zr+320]
 mov rax, qword ptr [r12 + 352]
 mov qword ptr [r12 + 320], rax
 mov rax, qword ptr [r12 + 360]
 mov qword ptr [r12 + 328], rax
  .section .rodata
  .Lbynamefn9: .string "dupl"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn9]
 lea rsi, [r12 + 304]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 288], rax
 mov qword ptr [r12 + 296], rdx
 cmp eax, 99
 je xchain0_n11_α
 jmp xchain0_n10_α
 xchain0_n8_β:
 jmp xchain0_n11_α
# IR_ASSIGN gva
 xchain0_n9_α:
 mov rax, qword ptr [r12 + 224]
 mov rdx, qword ptr [r12 + 232]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 208], rax
 mov qword ptr [r12 + 216], rdx
 jmp main_γ
 xchain0_n9_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n10_α:
 mov qword ptr [r12 + 368], 1
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [r12 + 376], rax
 jmp xchain0_n12_α
 xchain0_n10_β:
 jmp xchain0_n11_α
.Lx11_0:
 .quad .Lx11_0_s
.Lx11_0_s:
 .string ""
# IR_LIT_STRING
 xchain0_n11_α:
 mov qword ptr [r12 + 512], 1
 mov rax, qword ptr [rip + .Lx12_0]
 mov qword ptr [r12 + 520], rax
 jmp xchain0_n13_α
 xchain0_n11_β:
 jmp xchain0_n18_α
.Lx12_0:
 .quad .Lx12_0_s
.Lx12_0_s:
 .string "abcdefg"
 xchain0_n12_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+288] -> [zr+256]
 mov rax, qword ptr [r12 + 288]
 mov qword ptr [r12 + 256], rax
 mov rax, qword ptr [r12 + 296]
 mov qword ptr [r12 + 264], rax
# marshal arg1 = producer-box slot [zr+368] -> [zr+272]
 mov rax, qword ptr [r12 + 368]
 mov qword ptr [r12 + 272], rax
 mov rax, qword ptr [r12 + 376]
 mov qword ptr [r12 + 280], rax
  .section .rodata
  .Lbynamefn13: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn13]
 lea rsi, [r12 + 256]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 240], rax
 mov qword ptr [r12 + 248], rdx
 cmp eax, 99
 je xchain0_n11_α
 jmp xchain0_n14_α
 xchain0_n12_β:
 jmp xchain0_n11_α
# IR_LIT_INTEGER
 xchain0_n13_α:
 mov qword ptr [r12 + 528], 6
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [r12 + 536], rax
 jmp xchain0_n15_α
 xchain0_n13_β:
 jmp xchain0_n18_α
.Lx14_0:
 .quad 0
# IR_LIT_STRING
 xchain0_n14_α:
 mov qword ptr [r12 + 400], 1
 mov rax, qword ptr [rip + .Lx15_0]
 mov qword ptr [r12 + 408], rax
 jmp xchain0_n16_α
 xchain0_n14_β:
 jmp main_γ
.Lx15_0:
 .quad .Lx15_0_s
.Lx15_0_s:
 .string "FAIL 812/002: dupl null is null"
 xchain0_n15_α:
# BOX IR_CALL dupl(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+512] -> [zr+480]
 mov rax, qword ptr [r12 + 512]
 mov qword ptr [r12 + 480], rax
 mov rax, qword ptr [r12 + 520]
 mov qword ptr [r12 + 488], rax
# marshal arg1 = producer-box slot [zr+528] -> [zr+496]
 mov rax, qword ptr [r12 + 528]
 mov qword ptr [r12 + 496], rax
 mov rax, qword ptr [r12 + 536]
 mov qword ptr [r12 + 504], rax
  .section .rodata
  .Lbynamefn16: .string "dupl"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn16]
 lea rsi, [r12 + 480]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 464], rax
 mov qword ptr [r12 + 472], rdx
 cmp eax, 99
 je xchain0_n18_α
 jmp xchain0_n17_α
 xchain0_n15_β:
 jmp xchain0_n18_α
# IR_ASSIGN gva
 xchain0_n16_α:
 mov rax, qword ptr [r12 + 400]
 mov rdx, qword ptr [r12 + 408]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 384], rax
 mov qword ptr [r12 + 392], rdx
 jmp main_γ
 xchain0_n16_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n17_α:
 mov qword ptr [r12 + 544], 1
 mov rax, qword ptr [rip + .Lx18_0]
 mov qword ptr [r12 + 552], rax
 jmp xchain0_n19_α
 xchain0_n17_β:
 jmp xchain0_n18_α
.Lx18_0:
 .quad .Lx18_0_s
.Lx18_0_s:
 .string ""
# IR_LIT_INTEGER
 xchain0_n18_α:
 mov qword ptr [r12 + 688], 6
 mov rax, qword ptr [rip + .Lx19_0]
 mov qword ptr [r12 + 696], rax
 jmp xchain0_n20_α
 xchain0_n18_β:
 jmp xchain0_n25_α
.Lx19_0:
 .quad 1
 xchain0_n19_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+464] -> [zr+432]
 mov rax, qword ptr [r12 + 464]
 mov qword ptr [r12 + 432], rax
 mov rax, qword ptr [r12 + 472]
 mov qword ptr [r12 + 440], rax
# marshal arg1 = producer-box slot [zr+544] -> [zr+448]
 mov rax, qword ptr [r12 + 544]
 mov qword ptr [r12 + 448], rax
 mov rax, qword ptr [r12 + 552]
 mov qword ptr [r12 + 456], rax
  .section .rodata
  .Lbynamefn20: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn20]
 lea rsi, [r12 + 432]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 416], rax
 mov qword ptr [r12 + 424], rdx
 cmp eax, 99
 je xchain0_n18_α
 jmp xchain0_n21_α
 xchain0_n19_β:
 jmp xchain0_n18_α
# IR_LIT_INTEGER
 xchain0_n20_α:
 mov qword ptr [r12 + 704], 6
 mov rax, qword ptr [rip + .Lx21_0]
 mov qword ptr [r12 + 712], rax
 jmp xchain0_n22_α
 xchain0_n20_β:
 jmp xchain0_n25_α
.Lx21_0:
 .quad 10
# IR_LIT_STRING
 xchain0_n21_α:
 mov qword ptr [r12 + 576], 1
 mov rax, qword ptr [rip + .Lx22_0]
 mov qword ptr [r12 + 584], rax
 jmp xchain0_n23_α
 xchain0_n21_β:
 jmp main_γ
.Lx22_0:
 .quad .Lx22_0_s
.Lx22_0_s:
 .string "FAIL 812/003: dupl x0 is null"
 xchain0_n22_α:
# BOX IR_CALL dupl(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+688] -> [zr+656]
 mov rax, qword ptr [r12 + 688]
 mov qword ptr [r12 + 656], rax
 mov rax, qword ptr [r12 + 696]
 mov qword ptr [r12 + 664], rax
# marshal arg1 = producer-box slot [zr+704] -> [zr+672]
 mov rax, qword ptr [r12 + 704]
 mov qword ptr [r12 + 672], rax
 mov rax, qword ptr [r12 + 712]
 mov qword ptr [r12 + 680], rax
  .section .rodata
  .Lbynamefn23: .string "dupl"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn23]
 lea rsi, [r12 + 656]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 640], rax
 mov qword ptr [r12 + 648], rdx
 cmp eax, 99
 je xchain0_n25_α
 jmp xchain0_n24_α
 xchain0_n22_β:
 jmp xchain0_n25_α
# IR_ASSIGN gva
 xchain0_n23_α:
 mov rax, qword ptr [r12 + 576]
 mov rdx, qword ptr [r12 + 584]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 560], rax
 mov qword ptr [r12 + 568], rdx
 jmp main_γ
 xchain0_n23_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n24_α:
 mov qword ptr [r12 + 720], 1
 mov rax, qword ptr [rip + .Lx25_0]
 mov qword ptr [r12 + 728], rax
 jmp xchain0_n26_α
 xchain0_n24_β:
 jmp xchain0_n25_α
.Lx25_0:
 .quad .Lx25_0_s
.Lx25_0_s:
 .string "1111111111"
# IR_LIT_STRING
 xchain0_n25_α:
 mov qword ptr [r12 + 784], 1
 mov rax, qword ptr [rip + .Lx26_0]
 mov qword ptr [r12 + 792], rax
 jmp xchain0_n27_α
 xchain0_n25_β:
 jmp main_γ
.Lx26_0:
 .quad .Lx26_0_s
.Lx26_0_s:
 .string "PASS 812_dupl (4/4)"
 xchain0_n26_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+640] -> [zr+608]
 mov rax, qword ptr [r12 + 640]
 mov qword ptr [r12 + 608], rax
 mov rax, qword ptr [r12 + 648]
 mov qword ptr [r12 + 616], rax
# marshal arg1 = producer-box slot [zr+720] -> [zr+624]
 mov rax, qword ptr [r12 + 720]
 mov qword ptr [r12 + 624], rax
 mov rax, qword ptr [r12 + 728]
 mov qword ptr [r12 + 632], rax
  .section .rodata
  .Lbynamefn27: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn27]
 lea rsi, [r12 + 608]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 592], rax
 mov qword ptr [r12 + 600], rdx
 cmp eax, 99
 je xchain0_n25_α
 jmp xchain0_n28_α
 xchain0_n26_β:
 jmp xchain0_n25_α
# IR_ASSIGN gva
 xchain0_n27_α:
 mov rax, qword ptr [r12 + 784]
 mov rdx, qword ptr [r12 + 792]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 768], rax
 mov qword ptr [r12 + 776], rdx
 jmp main_γ
 xchain0_n27_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n28_α:
 mov qword ptr [r12 + 752], 1
 mov rax, qword ptr [rip + .Lx29_0]
 mov qword ptr [r12 + 760], rax
 jmp xchain0_n29_α
 xchain0_n28_β:
 jmp main_γ
.Lx29_0:
 .quad .Lx29_0_s
.Lx29_0_s:
 .string "FAIL 812/004: dupl integer coerce"
# IR_ASSIGN gva
 xchain0_n29_α:
 mov rax, qword ptr [r12 + 752]
 mov rdx, qword ptr [r12 + 760]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 736], rax
 mov qword ptr [r12 + 744], rdx
 jmp main_γ
 xchain0_n29_β:
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
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 800]
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
mov rsp, rbp
pop rbp
pop r12
ret
