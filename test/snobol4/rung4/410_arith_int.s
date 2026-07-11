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
 mov qword ptr [r12 + 1392], rax
 pop rsi
main_α_body:
# IR_LIT_INTEGER
 xchain0_n0_α:
 mov qword ptr [r12 + 128], 6
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 136], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n4_α
.Lx1_0:
 .quad 3
# IR_LIT_INTEGER
 xchain0_n1_α:
 mov qword ptr [r12 + 144], 6
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [r12 + 152], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n4_α
.Lx2_0:
 .quad 2
 xchain0_n2_α:
# IR_BINOP_ARITH
.Lx3_1:
 mov rax, 3
 mov rcx, 2
 add rax, rcx
 mov qword ptr [r12 + 112], 6
 mov qword ptr [r12 + 120], rax
 jmp xchain0_n3_α
.Lx3_0:
 mov rdi, qword ptr [r12 + 128]
 mov rsi, qword ptr [r12 + 136]
 mov rdx, qword ptr [r12 + 144]
 mov rcx, qword ptr [r12 + 152]
 mov r8d, 0
 lea r9, [r12 + 112]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx3_3
.Lx3_2:
 mov rdi, qword ptr [r12 + 128]
 mov rsi, qword ptr [r12 + 136]
 mov rdx, qword ptr [r12 + 144]
 mov rcx, qword ptr [r12 + 152]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n4_α
 mov qword ptr [r12 + 112], rax
 mov qword ptr [r12 + 120], rdx
.Lx3_3:
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n4_α
# IR_LIT_INTEGER
 xchain0_n3_α:
 mov qword ptr [r12 + 160], 6
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [r12 + 168], rax
 jmp xchain0_n5_α
 xchain0_n3_β:
 jmp xchain0_n4_α
.Lx4_0:
 .quad 5
# IR_LIT_INTEGER
 xchain0_n4_α:
 mov qword ptr [r12 + 272], 6
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [r12 + 280], rax
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp xchain0_n11_α
.Lx5_0:
 .quad 3
 xchain0_n5_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+112] -> [zr+80]
 mov rax, qword ptr [r12 + 112]
 mov qword ptr [r12 + 80], rax
 mov rax, qword ptr [r12 + 120]
 mov qword ptr [r12 + 88], rax
# marshal arg1 = producer-box slot [zr+160] -> [zr+96]
 mov rax, qword ptr [r12 + 160]
 mov qword ptr [r12 + 96], rax
 mov rax, qword ptr [r12 + 168]
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
 mov qword ptr [r12 + 288], 6
 mov rax, qword ptr [rip + .Lx7_0]
 mov qword ptr [r12 + 296], rax
 jmp xchain0_n8_α
 xchain0_n6_β:
 jmp xchain0_n11_α
.Lx7_0:
 .quad 2
# IR_LIT_STRING
 xchain0_n7_α:
 mov qword ptr [r12 + 192], 1
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [r12 + 200], rax
 jmp xchain0_n9_α
 xchain0_n7_β:
 jmp main_γ
.Lx8_0:
 .quad .Lx8_0_s
.Lx8_0_s:
 .string "FAIL 410/001: 3+2"
 xchain0_n8_α:
# IR_BINOP_ARITH
.Lx9_1:
 mov rax, 3
 mov rcx, 2
 sub rax, rcx
 mov qword ptr [r12 + 256], 6
 mov qword ptr [r12 + 264], rax
 jmp xchain0_n10_α
.Lx9_0:
 mov rdi, qword ptr [r12 + 272]
 mov rsi, qword ptr [r12 + 280]
 mov rdx, qword ptr [r12 + 288]
 mov rcx, qword ptr [r12 + 296]
 mov r8d, 1
 lea r9, [r12 + 256]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx9_3
.Lx9_2:
 mov rdi, qword ptr [r12 + 272]
 mov rsi, qword ptr [r12 + 280]
 mov rdx, qword ptr [r12 + 288]
 mov rcx, qword ptr [r12 + 296]
 mov r8d, 1
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n11_α
 mov qword ptr [r12 + 256], rax
 mov qword ptr [r12 + 264], rdx
.Lx9_3:
 jmp xchain0_n10_α
 xchain0_n8_β:
 jmp xchain0_n11_α
# IR_ASSIGN gva
 xchain0_n9_α:
 mov rax, qword ptr [r12 + 192]
 mov rdx, qword ptr [r12 + 200]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 176], rax
 mov qword ptr [r12 + 184], rdx
 jmp main_γ
 xchain0_n9_β:
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n10_α:
 mov qword ptr [r12 + 304], 6
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [r12 + 312], rax
 jmp xchain0_n12_α
 xchain0_n10_β:
 jmp xchain0_n11_α
.Lx11_0:
 .quad 1
# IR_LIT_INTEGER
 xchain0_n11_α:
 mov qword ptr [r12 + 416], 6
 mov rax, qword ptr [rip + .Lx12_0]
 mov qword ptr [r12 + 424], rax
 jmp xchain0_n13_α
 xchain0_n11_β:
 jmp xchain0_n18_α
.Lx12_0:
 .quad 3
 xchain0_n12_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+256] -> [zr+224]
 mov rax, qword ptr [r12 + 256]
 mov qword ptr [r12 + 224], rax
 mov rax, qword ptr [r12 + 264]
 mov qword ptr [r12 + 232], rax
# marshal arg1 = producer-box slot [zr+304] -> [zr+240]
 mov rax, qword ptr [r12 + 304]
 mov qword ptr [r12 + 240], rax
 mov rax, qword ptr [r12 + 312]
 mov qword ptr [r12 + 248], rax
  .section .rodata
  .Lbynamefn13: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn13]
 lea rsi, [r12 + 224]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 208], rax
 mov qword ptr [r12 + 216], rdx
 cmp eax, 99
 je xchain0_n11_α
 jmp xchain0_n14_α
 xchain0_n12_β:
 jmp xchain0_n11_α
# IR_LIT_INTEGER
 xchain0_n13_α:
 mov qword ptr [r12 + 432], 6
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [r12 + 440], rax
 jmp xchain0_n15_α
 xchain0_n13_β:
 jmp xchain0_n18_α
.Lx14_0:
 .quad 2
# IR_LIT_STRING
 xchain0_n14_α:
 mov qword ptr [r12 + 336], 1
 mov rax, qword ptr [rip + .Lx15_0]
 mov qword ptr [r12 + 344], rax
 jmp xchain0_n16_α
 xchain0_n14_β:
 jmp main_γ
.Lx15_0:
 .quad .Lx15_0_s
.Lx15_0_s:
 .string "FAIL 410/002: 3-2"
 xchain0_n15_α:
# IR_BINOP_ARITH
.Lx16_1:
 mov rax, 3
 mov rcx, 2
 imul rax, rcx
 mov qword ptr [r12 + 400], 6
 mov qword ptr [r12 + 408], rax
 jmp xchain0_n17_α
.Lx16_0:
 mov rdi, qword ptr [r12 + 416]
 mov rsi, qword ptr [r12 + 424]
 mov rdx, qword ptr [r12 + 432]
 mov rcx, qword ptr [r12 + 440]
 mov r8d, 2
 lea r9, [r12 + 400]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx16_3
.Lx16_2:
 mov rdi, qword ptr [r12 + 416]
 mov rsi, qword ptr [r12 + 424]
 mov rdx, qword ptr [r12 + 432]
 mov rcx, qword ptr [r12 + 440]
 mov r8d, 2
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n18_α
 mov qword ptr [r12 + 400], rax
 mov qword ptr [r12 + 408], rdx
.Lx16_3:
 jmp xchain0_n17_α
 xchain0_n15_β:
 jmp xchain0_n18_α
# IR_ASSIGN gva
 xchain0_n16_α:
 mov rax, qword ptr [r12 + 336]
 mov rdx, qword ptr [r12 + 344]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 320], rax
 mov qword ptr [r12 + 328], rdx
 jmp main_γ
 xchain0_n16_β:
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n17_α:
 mov qword ptr [r12 + 448], 6
 mov rax, qword ptr [rip + .Lx18_0]
 mov qword ptr [r12 + 456], rax
 jmp xchain0_n19_α
 xchain0_n17_β:
 jmp xchain0_n18_α
.Lx18_0:
 .quad 6
# IR_LIT_INTEGER
 xchain0_n18_α:
 mov qword ptr [r12 + 560], 6
 mov rax, qword ptr [rip + .Lx19_0]
 mov qword ptr [r12 + 568], rax
 jmp xchain0_n20_α
 xchain0_n18_β:
 jmp xchain0_n25_α
.Lx19_0:
 .quad 5
 xchain0_n19_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+400] -> [zr+368]
 mov rax, qword ptr [r12 + 400]
 mov qword ptr [r12 + 368], rax
 mov rax, qword ptr [r12 + 408]
 mov qword ptr [r12 + 376], rax
# marshal arg1 = producer-box slot [zr+448] -> [zr+384]
 mov rax, qword ptr [r12 + 448]
 mov qword ptr [r12 + 384], rax
 mov rax, qword ptr [r12 + 456]
 mov qword ptr [r12 + 392], rax
  .section .rodata
  .Lbynamefn20: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn20]
 lea rsi, [r12 + 368]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 352], rax
 mov qword ptr [r12 + 360], rdx
 cmp eax, 99
 je xchain0_n18_α
 jmp xchain0_n21_α
 xchain0_n19_β:
 jmp xchain0_n18_α
# IR_LIT_INTEGER
 xchain0_n20_α:
 mov qword ptr [r12 + 576], 6
 mov rax, qword ptr [rip + .Lx21_0]
 mov qword ptr [r12 + 584], rax
 jmp xchain0_n22_α
 xchain0_n20_β:
 jmp xchain0_n25_α
.Lx21_0:
 .quad 2
# IR_LIT_STRING
 xchain0_n21_α:
 mov qword ptr [r12 + 480], 1
 mov rax, qword ptr [rip + .Lx22_0]
 mov qword ptr [r12 + 488], rax
 jmp xchain0_n23_α
 xchain0_n21_β:
 jmp main_γ
.Lx22_0:
 .quad .Lx22_0_s
.Lx22_0_s:
 .string "FAIL 410/003: 3*2"
 xchain0_n22_α:
# IR_BINOP_ARITH
.Lx23_1:
 mov rax, 5
 mov rcx, 2
 cqo
 idiv rcx
 mov qword ptr [r12 + 544], 6
 mov qword ptr [r12 + 552], rax
 jmp xchain0_n24_α
.Lx23_0:
 mov rdi, qword ptr [r12 + 560]
 mov rsi, qword ptr [r12 + 568]
 mov rdx, qword ptr [r12 + 576]
 mov rcx, qword ptr [r12 + 584]
 mov r8d, 3
 lea r9, [r12 + 544]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx23_3
.Lx23_2:
 mov rdi, qword ptr [r12 + 560]
 mov rsi, qword ptr [r12 + 568]
 mov rdx, qword ptr [r12 + 576]
 mov rcx, qword ptr [r12 + 584]
 mov r8d, 3
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n25_α
 mov qword ptr [r12 + 544], rax
 mov qword ptr [r12 + 552], rdx
.Lx23_3:
 jmp xchain0_n24_α
 xchain0_n22_β:
 jmp xchain0_n25_α
# IR_ASSIGN gva
 xchain0_n23_α:
 mov rax, qword ptr [r12 + 480]
 mov rdx, qword ptr [r12 + 488]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 464], rax
 mov qword ptr [r12 + 472], rdx
 jmp main_γ
 xchain0_n23_β:
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n24_α:
 mov qword ptr [r12 + 592], 6
 mov rax, qword ptr [rip + .Lx25_0]
 mov qword ptr [r12 + 600], rax
 jmp xchain0_n26_α
 xchain0_n24_β:
 jmp xchain0_n25_α
.Lx25_0:
 .quad 2
# IR_LIT_INTEGER
 xchain0_n25_α:
 mov qword ptr [r12 + 704], 6
 mov rax, qword ptr [rip + .Lx26_0]
 mov qword ptr [r12 + 712], rax
 jmp xchain0_n27_α
 xchain0_n25_β:
 jmp xchain0_n32_α
.Lx26_0:
 .quad 2
 xchain0_n26_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+544] -> [zr+512]
 mov rax, qword ptr [r12 + 544]
 mov qword ptr [r12 + 512], rax
 mov rax, qword ptr [r12 + 552]
 mov qword ptr [r12 + 520], rax
# marshal arg1 = producer-box slot [zr+592] -> [zr+528]
 mov rax, qword ptr [r12 + 592]
 mov qword ptr [r12 + 528], rax
 mov rax, qword ptr [r12 + 600]
 mov qword ptr [r12 + 536], rax
  .section .rodata
  .Lbynamefn27: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn27]
 lea rsi, [r12 + 512]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 496], rax
 mov qword ptr [r12 + 504], rdx
 cmp eax, 99
 je xchain0_n25_α
 jmp xchain0_n28_α
 xchain0_n26_β:
 jmp xchain0_n25_α
# IR_LIT_INTEGER
 xchain0_n27_α:
 mov qword ptr [r12 + 720], 6
 mov rax, qword ptr [rip + .Lx28_0]
 mov qword ptr [r12 + 728], rax
 jmp xchain0_n29_α
 xchain0_n27_β:
 jmp xchain0_n32_α
.Lx28_0:
 .quad 3
# IR_LIT_STRING
 xchain0_n28_α:
 mov qword ptr [r12 + 624], 1
 mov rax, qword ptr [rip + .Lx29_0]
 mov qword ptr [r12 + 632], rax
 jmp xchain0_n30_α
 xchain0_n28_β:
 jmp main_γ
.Lx29_0:
 .quad .Lx29_0_s
.Lx29_0_s:
 .string "FAIL 410/004: 5/2 integer division"
 xchain0_n29_α:
# IR_BINOP_ARITH_REAL
 mov rdi, qword ptr [r12 + 704]
 mov rsi, qword ptr [r12 + 712]
 mov rdx, qword ptr [r12 + 720]
 mov rcx, qword ptr [r12 + 728]
 mov r8d, 18
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n32_α
 mov qword ptr [r12 + 688], rax
 mov qword ptr [r12 + 696], rdx
 jmp xchain0_n31_α
 xchain0_n29_β:
 jmp xchain0_n32_α
# IR_ASSIGN gva
 xchain0_n30_α:
 mov rax, qword ptr [r12 + 624]
 mov rdx, qword ptr [r12 + 632]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 608], rax
 mov qword ptr [r12 + 616], rdx
 jmp main_γ
 xchain0_n30_β:
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n31_α:
 mov qword ptr [r12 + 736], 6
 mov rax, qword ptr [rip + .Lx32_0]
 mov qword ptr [r12 + 744], rax
 jmp xchain0_n33_α
 xchain0_n31_β:
 jmp xchain0_n32_α
.Lx32_0:
 .quad 8
# IR_LIT_STRING
 xchain0_n32_α:
 mov qword ptr [r12 + 848], 1
 mov rax, qword ptr [rip + .Lx33_0]
 mov qword ptr [r12 + 856], rax
 jmp xchain0_n34_α
 xchain0_n32_β:
 jmp xchain0_n39_α
.Lx33_0:
 .quad .Lx33_0_s
.Lx33_0_s:
 .string "3"
 xchain0_n33_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+688] -> [zr+656]
 mov rax, qword ptr [r12 + 688]
 mov qword ptr [r12 + 656], rax
 mov rax, qword ptr [r12 + 696]
 mov qword ptr [r12 + 664], rax
# marshal arg1 = producer-box slot [zr+736] -> [zr+672]
 mov rax, qword ptr [r12 + 736]
 mov qword ptr [r12 + 672], rax
 mov rax, qword ptr [r12 + 744]
 mov qword ptr [r12 + 680], rax
  .section .rodata
  .Lbynamefn34: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn34]
 lea rsi, [r12 + 656]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 640], rax
 mov qword ptr [r12 + 648], rdx
 cmp eax, 99
 je xchain0_n32_α
 jmp xchain0_n35_α
 xchain0_n33_β:
 jmp xchain0_n32_α
# IR_LIT_INTEGER
 xchain0_n34_α:
 mov qword ptr [r12 + 864], 6
 mov rax, qword ptr [rip + .Lx35_0]
 mov qword ptr [r12 + 872], rax
 jmp xchain0_n36_α
 xchain0_n34_β:
 jmp xchain0_n39_α
.Lx35_0:
 .quad 2
# IR_LIT_STRING
 xchain0_n35_α:
 mov qword ptr [r12 + 768], 1
 mov rax, qword ptr [rip + .Lx36_0]
 mov qword ptr [r12 + 776], rax
 jmp xchain0_n37_α
 xchain0_n35_β:
 jmp main_γ
.Lx36_0:
 .quad .Lx36_0_s
.Lx36_0_s:
 .string "FAIL 410/005: 2**3"
 xchain0_n36_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 848]
 cmp eax, 100
 je .Lx37_0
 mov eax, dword ptr [r12 + 848]
 cmp eax, 6
 jne .Lx37_2
.Lx37_1:
 mov rax, qword ptr [r12 + 856]
 mov rcx, 2
 add rax, rcx
 mov qword ptr [r12 + 832], 6
 mov qword ptr [r12 + 840], rax
 jmp xchain0_n38_α
.Lx37_0:
 mov rdi, qword ptr [r12 + 848]
 mov rsi, qword ptr [r12 + 856]
 mov rdx, qword ptr [r12 + 864]
 mov rcx, qword ptr [r12 + 872]
 mov r8d, 0
 lea r9, [r12 + 832]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx37_3
.Lx37_2:
 mov rdi, qword ptr [r12 + 848]
 mov rsi, qword ptr [r12 + 856]
 mov rdx, qword ptr [r12 + 864]
 mov rcx, qword ptr [r12 + 872]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n39_α
 mov qword ptr [r12 + 832], rax
 mov qword ptr [r12 + 840], rdx
.Lx37_3:
 jmp xchain0_n38_α
 xchain0_n36_β:
 jmp xchain0_n39_α
# IR_ASSIGN gva
 xchain0_n37_α:
 mov rax, qword ptr [r12 + 768]
 mov rdx, qword ptr [r12 + 776]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 752], rax
 mov qword ptr [r12 + 760], rdx
 jmp main_γ
 xchain0_n37_β:
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n38_α:
 mov qword ptr [r12 + 880], 6
 mov rax, qword ptr [rip + .Lx39_0]
 mov qword ptr [r12 + 888], rax
 jmp xchain0_n40_α
 xchain0_n38_β:
 jmp xchain0_n39_α
.Lx39_0:
 .quad 5
# IR_LIT_INTEGER
 xchain0_n39_α:
 mov qword ptr [r12 + 992], 6
 mov rax, qword ptr [rip + .Lx40_0]
 mov qword ptr [r12 + 1000], rax
 jmp xchain0_n41_α
 xchain0_n39_β:
 jmp xchain0_n46_α
.Lx40_0:
 .quad 3
 xchain0_n40_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+832] -> [zr+800]
 mov rax, qword ptr [r12 + 832]
 mov qword ptr [r12 + 800], rax
 mov rax, qword ptr [r12 + 840]
 mov qword ptr [r12 + 808], rax
# marshal arg1 = producer-box slot [zr+880] -> [zr+816]
 mov rax, qword ptr [r12 + 880]
 mov qword ptr [r12 + 816], rax
 mov rax, qword ptr [r12 + 888]
 mov qword ptr [r12 + 824], rax
  .section .rodata
  .Lbynamefn41: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn41]
 lea rsi, [r12 + 800]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 784], rax
 mov qword ptr [r12 + 792], rdx
 cmp eax, 99
 je xchain0_n39_α
 jmp xchain0_n42_α
 xchain0_n40_β:
 jmp xchain0_n39_α
# IR_LIT_STRING
 xchain0_n41_α:
 mov qword ptr [r12 + 1008], 1
 mov rax, qword ptr [rip + .Lx42_0]
 mov qword ptr [r12 + 1016], rax
 jmp xchain0_n43_α
 xchain0_n41_β:
 jmp xchain0_n46_α
.Lx42_0:
 .quad .Lx42_0_s
.Lx42_0_s:
 .string "-2"
# IR_LIT_STRING
 xchain0_n42_α:
 mov qword ptr [r12 + 912], 1
 mov rax, qword ptr [rip + .Lx43_0]
 mov qword ptr [r12 + 920], rax
 jmp xchain0_n44_α
 xchain0_n42_β:
 jmp main_γ
.Lx43_0:
 .quad .Lx43_0_s
.Lx43_0_s:
 .string "FAIL 410/006: string+int coerce"
 xchain0_n43_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 1008]
 cmp eax, 100
 je .Lx44_0
 mov eax, dword ptr [r12 + 1008]
 cmp eax, 6
 jne .Lx44_2
.Lx44_1:
 mov rax, 3
 mov rcx, qword ptr [r12 + 1016]
 add rax, rcx
 mov qword ptr [r12 + 976], 6
 mov qword ptr [r12 + 984], rax
 jmp xchain0_n45_α
.Lx44_0:
 mov rdi, qword ptr [r12 + 992]
 mov rsi, qword ptr [r12 + 1000]
 mov rdx, qword ptr [r12 + 1008]
 mov rcx, qword ptr [r12 + 1016]
 mov r8d, 0
 lea r9, [r12 + 976]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx44_3
.Lx44_2:
 mov rdi, qword ptr [r12 + 992]
 mov rsi, qword ptr [r12 + 1000]
 mov rdx, qword ptr [r12 + 1008]
 mov rcx, qword ptr [r12 + 1016]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n46_α
 mov qword ptr [r12 + 976], rax
 mov qword ptr [r12 + 984], rdx
.Lx44_3:
 jmp xchain0_n45_α
 xchain0_n43_β:
 jmp xchain0_n46_α
# IR_ASSIGN gva
 xchain0_n44_α:
 mov rax, qword ptr [r12 + 912]
 mov rdx, qword ptr [r12 + 920]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 896], rax
 mov qword ptr [r12 + 904], rdx
 jmp main_γ
 xchain0_n44_β:
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n45_α:
 mov qword ptr [r12 + 1024], 6
 mov rax, qword ptr [rip + .Lx46_0]
 mov qword ptr [r12 + 1032], rax
 jmp xchain0_n47_α
 xchain0_n45_β:
 jmp xchain0_n46_α
.Lx46_0:
 .quad 1
# IR_LIT_STRING
 xchain0_n46_α:
 mov qword ptr [r12 + 1136], 1
 mov rax, qword ptr [rip + .Lx47_0]
 mov qword ptr [r12 + 1144], rax
 jmp xchain0_n48_α
 xchain0_n46_β:
 jmp xchain0_n53_α
.Lx47_0:
 .quad .Lx47_0_s
.Lx47_0_s:
 .string "1"
 xchain0_n47_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+976] -> [zr+944]
 mov rax, qword ptr [r12 + 976]
 mov qword ptr [r12 + 944], rax
 mov rax, qword ptr [r12 + 984]
 mov qword ptr [r12 + 952], rax
# marshal arg1 = producer-box slot [zr+1024] -> [zr+960]
 mov rax, qword ptr [r12 + 1024]
 mov qword ptr [r12 + 960], rax
 mov rax, qword ptr [r12 + 1032]
 mov qword ptr [r12 + 968], rax
  .section .rodata
  .Lbynamefn48: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn48]
 lea rsi, [r12 + 944]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 928], rax
 mov qword ptr [r12 + 936], rdx
 cmp eax, 99
 je xchain0_n46_α
 jmp xchain0_n49_α
 xchain0_n47_β:
 jmp xchain0_n46_α
# IR_LIT_STRING
 xchain0_n48_α:
 mov qword ptr [r12 + 1152], 1
 mov rax, qword ptr [rip + .Lx49_0]
 mov qword ptr [r12 + 1160], rax
 jmp xchain0_n50_α
 xchain0_n48_β:
 jmp xchain0_n53_α
.Lx49_0:
 .quad .Lx49_0_s
.Lx49_0_s:
 .string "0"
# IR_LIT_STRING
 xchain0_n49_α:
 mov qword ptr [r12 + 1056], 1
 mov rax, qword ptr [rip + .Lx50_0]
 mov qword ptr [r12 + 1064], rax
 jmp xchain0_n51_α
 xchain0_n49_β:
 jmp main_γ
.Lx50_0:
 .quad .Lx50_0_s
.Lx50_0_s:
 .string "FAIL 410/007: int+neg-string"
 xchain0_n50_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 1136]
 cmp eax, 100
 je .Lx51_0
 mov eax, dword ptr [r12 + 1152]
 cmp eax, 100
 je .Lx51_0
 mov eax, dword ptr [r12 + 1136]
 cmp eax, 6
 jne .Lx51_2
 mov eax, dword ptr [r12 + 1152]
 cmp eax, 6
 jne .Lx51_2
.Lx51_1:
 mov rax, qword ptr [r12 + 1144]
 mov rcx, qword ptr [r12 + 1160]
 add rax, rcx
 mov qword ptr [r12 + 1120], 6
 mov qword ptr [r12 + 1128], rax
 jmp xchain0_n52_α
.Lx51_0:
 mov rdi, qword ptr [r12 + 1136]
 mov rsi, qword ptr [r12 + 1144]
 mov rdx, qword ptr [r12 + 1152]
 mov rcx, qword ptr [r12 + 1160]
 mov r8d, 0
 lea r9, [r12 + 1120]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx51_3
.Lx51_2:
 mov rdi, qword ptr [r12 + 1136]
 mov rsi, qword ptr [r12 + 1144]
 mov rdx, qword ptr [r12 + 1152]
 mov rcx, qword ptr [r12 + 1160]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n53_α
 mov qword ptr [r12 + 1120], rax
 mov qword ptr [r12 + 1128], rdx
.Lx51_3:
 jmp xchain0_n52_α
 xchain0_n50_β:
 jmp xchain0_n53_α
# IR_ASSIGN gva
 xchain0_n51_α:
 mov rax, qword ptr [r12 + 1056]
 mov rdx, qword ptr [r12 + 1064]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 1040], rax
 mov qword ptr [r12 + 1048], rdx
 jmp main_γ
 xchain0_n51_β:
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n52_α:
 mov qword ptr [r12 + 1168], 6
 mov rax, qword ptr [rip + .Lx53_0]
 mov qword ptr [r12 + 1176], rax
 jmp xchain0_n54_α
 xchain0_n52_β:
 jmp xchain0_n53_α
.Lx53_0:
 .quad 1
# IR_LIT_INTEGER
 xchain0_n53_α:
 mov qword ptr [r12 + 1280], 6
 mov rax, qword ptr [rip + .Lx54_0]
 mov qword ptr [r12 + 1288], rax
 jmp xchain0_n55_α
 xchain0_n53_β:
 jmp xchain0_n60_α
.Lx54_0:
 .quad 5
 xchain0_n54_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1120] -> [zr+1088]
 mov rax, qword ptr [r12 + 1120]
 mov qword ptr [r12 + 1088], rax
 mov rax, qword ptr [r12 + 1128]
 mov qword ptr [r12 + 1096], rax
# marshal arg1 = producer-box slot [zr+1168] -> [zr+1104]
 mov rax, qword ptr [r12 + 1168]
 mov qword ptr [r12 + 1104], rax
 mov rax, qword ptr [r12 + 1176]
 mov qword ptr [r12 + 1112], rax
  .section .rodata
  .Lbynamefn55: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn55]
 lea rsi, [r12 + 1088]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1072], rax
 mov qword ptr [r12 + 1080], rdx
 cmp eax, 99
 je xchain0_n53_α
 jmp xchain0_n56_α
 xchain0_n54_β:
 jmp xchain0_n53_α
# IR_LIT_STRING
 xchain0_n55_α:
 mov qword ptr [r12 + 1296], 1
 mov rax, qword ptr [rip + .Lx56_0]
 mov qword ptr [r12 + 1304], rax
 jmp xchain0_n57_α
 xchain0_n55_β:
 jmp xchain0_n60_α
.Lx56_0:
 .quad .Lx56_0_s
.Lx56_0_s:
 .string ""
# IR_LIT_STRING
 xchain0_n56_α:
 mov qword ptr [r12 + 1200], 1
 mov rax, qword ptr [rip + .Lx57_0]
 mov qword ptr [r12 + 1208], rax
 jmp xchain0_n58_α
 xchain0_n56_β:
 jmp main_γ
.Lx57_0:
 .quad .Lx57_0_s
.Lx57_0_s:
 .string "FAIL 410/008: string+string coerce"
 xchain0_n57_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 1296]
 cmp eax, 100
 je .Lx58_0
 mov eax, dword ptr [r12 + 1296]
 cmp eax, 6
 jne .Lx58_2
.Lx58_1:
 mov rax, 5
 mov rcx, qword ptr [r12 + 1304]
 add rax, rcx
 mov qword ptr [r12 + 1264], 6
 mov qword ptr [r12 + 1272], rax
 jmp xchain0_n59_α
.Lx58_0:
 mov rdi, qword ptr [r12 + 1280]
 mov rsi, qword ptr [r12 + 1288]
 mov rdx, qword ptr [r12 + 1296]
 mov rcx, qword ptr [r12 + 1304]
 mov r8d, 0
 lea r9, [r12 + 1264]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx58_3
.Lx58_2:
 mov rdi, qword ptr [r12 + 1280]
 mov rsi, qword ptr [r12 + 1288]
 mov rdx, qword ptr [r12 + 1296]
 mov rcx, qword ptr [r12 + 1304]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n60_α
 mov qword ptr [r12 + 1264], rax
 mov qword ptr [r12 + 1272], rdx
.Lx58_3:
 jmp xchain0_n59_α
 xchain0_n57_β:
 jmp xchain0_n60_α
# IR_ASSIGN gva
 xchain0_n58_α:
 mov rax, qword ptr [r12 + 1200]
 mov rdx, qword ptr [r12 + 1208]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 1184], rax
 mov qword ptr [r12 + 1192], rdx
 jmp main_γ
 xchain0_n58_β:
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n59_α:
 mov qword ptr [r12 + 1312], 6
 mov rax, qword ptr [rip + .Lx60_0]
 mov qword ptr [r12 + 1320], rax
 jmp xchain0_n61_α
 xchain0_n59_β:
 jmp xchain0_n60_α
.Lx60_0:
 .quad 5
# IR_LIT_STRING
 xchain0_n60_α:
 mov qword ptr [r12 + 1376], 1
 mov rax, qword ptr [rip + .Lx61_0]
 mov qword ptr [r12 + 1384], rax
 jmp xchain0_n62_α
 xchain0_n60_β:
 jmp main_γ
.Lx61_0:
 .quad .Lx61_0_s
.Lx61_0_s:
 .string "PASS 410_arith_int (9/9)"
 xchain0_n61_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1264] -> [zr+1232]
 mov rax, qword ptr [r12 + 1264]
 mov qword ptr [r12 + 1232], rax
 mov rax, qword ptr [r12 + 1272]
 mov qword ptr [r12 + 1240], rax
# marshal arg1 = producer-box slot [zr+1312] -> [zr+1248]
 mov rax, qword ptr [r12 + 1312]
 mov qword ptr [r12 + 1248], rax
 mov rax, qword ptr [r12 + 1320]
 mov qword ptr [r12 + 1256], rax
  .section .rodata
  .Lbynamefn62: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn62]
 lea rsi, [r12 + 1232]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1216], rax
 mov qword ptr [r12 + 1224], rdx
 cmp eax, 99
 je xchain0_n60_α
 jmp xchain0_n63_α
 xchain0_n61_β:
 jmp xchain0_n60_α
# IR_ASSIGN gva
 xchain0_n62_α:
 mov rax, qword ptr [r12 + 1376]
 mov rdx, qword ptr [r12 + 1384]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 1360], rax
 mov qword ptr [r12 + 1368], rdx
 jmp main_γ
 xchain0_n62_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n63_α:
 mov qword ptr [r12 + 1344], 1
 mov rax, qword ptr [rip + .Lx64_0]
 mov qword ptr [r12 + 1352], rax
 jmp xchain0_n64_α
 xchain0_n63_β:
 jmp main_γ
.Lx64_0:
 .quad .Lx64_0_s
.Lx64_0_s:
 .string "FAIL 410/009: null addend is zero"
# IR_ASSIGN gva
 xchain0_n64_α:
 mov rax, qword ptr [r12 + 1344]
 mov rdx, qword ptr [r12 + 1352]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 1328], rax
 mov qword ptr [r12 + 1336], rdx
 jmp main_γ
 xchain0_n64_β:
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
 mov rdi, qword ptr [r12 + 1392]
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
