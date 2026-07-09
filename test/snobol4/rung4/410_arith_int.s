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
 mov qword ptr [r12 + 1344], rax
 pop rsi
main_α_body:
# IR_LIT_INTEGER
 xchain0_n0_α:
 mov qword ptr [r12 + 80], 6
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 88], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n4_α
.Lx1_0:
 .quad 3
# IR_LIT_INTEGER
 xchain0_n1_α:
 mov qword ptr [r12 + 96], 6
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [r12 + 104], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n4_α
.Lx2_0:
 .quad 2
 xchain0_n2_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 80]
 cmp eax, 100
 je .Lx3_0
 mov eax, dword ptr [r12 + 96]
 cmp eax, 100
 je .Lx3_0
 mov eax, dword ptr [r12 + 80]
 cmp eax, 6
 jne .Lx3_2
 mov eax, dword ptr [r12 + 96]
 cmp eax, 6
 jne .Lx3_2
.Lx3_1:
 mov rax, qword ptr [r12 + 88]
 mov rcx, qword ptr [r12 + 104]
 add rax, rcx
 mov qword ptr [r12 + 64], 6
 mov qword ptr [r12 + 72], rax
 jmp xchain0_n3_α
.Lx3_0:
 mov rdi, qword ptr [r12 + 80]
 mov rsi, qword ptr [r12 + 88]
 mov rdx, qword ptr [r12 + 96]
 mov rcx, qword ptr [r12 + 104]
 mov r8d, 0
 lea r9, [r12 + 64]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx3_3
.Lx3_2:
 mov rdi, qword ptr [r12 + 80]
 mov rsi, qword ptr [r12 + 88]
 mov rdx, qword ptr [r12 + 96]
 mov rcx, qword ptr [r12 + 104]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n4_α
 mov qword ptr [r12 + 64], rax
 mov qword ptr [r12 + 72], rdx
.Lx3_3:
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n4_α
# IR_LIT_INTEGER
 xchain0_n3_α:
 mov qword ptr [r12 + 112], 6
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [r12 + 120], rax
 jmp xchain0_n5_α
 xchain0_n3_β:
 jmp xchain0_n4_α
.Lx4_0:
 .quad 5
# IR_LIT_INTEGER
 xchain0_n4_α:
 mov qword ptr [r12 + 224], 6
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [r12 + 232], rax
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp xchain0_n11_α
.Lx5_0:
 .quad 3
 xchain0_n5_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+64] -> [r12+32]
 mov rax, qword ptr [r12 + 64]
 mov qword ptr [r12 + 32], rax
 mov rax, qword ptr [r12 + 72]
 mov qword ptr [r12 + 40], rax
# marshal arg1 = producer-box slot [r12+112] -> [r12+48]
 mov rax, qword ptr [r12 + 112]
 mov qword ptr [r12 + 48], rax
 mov rax, qword ptr [r12 + 120]
 mov qword ptr [r12 + 56], rax
  .section .rodata
  .Lbynamefn7: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn7]
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
 mov qword ptr [r12 + 240], 6
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [r12 + 248], rax
 jmp xchain0_n8_α
 xchain0_n6_β:
 jmp xchain0_n11_α
.Lx8_0:
 .quad 2
# IR_LIT_STRING
 xchain0_n7_α:
 mov qword ptr [r12 + 144], 1
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [r12 + 152], rax
 jmp xchain0_n9_α
 xchain0_n7_β:
 jmp main_γ
.Lx9_0:
 .quad .Lx9_0_s
.Lx9_0_s:
 .string "FAIL 410/001: 3+2"
 xchain0_n8_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 224]
 cmp eax, 100
 je .Lx10_0
 mov eax, dword ptr [r12 + 240]
 cmp eax, 100
 je .Lx10_0
 mov eax, dword ptr [r12 + 224]
 cmp eax, 6
 jne .Lx10_2
 mov eax, dword ptr [r12 + 240]
 cmp eax, 6
 jne .Lx10_2
.Lx10_1:
 mov rax, qword ptr [r12 + 232]
 mov rcx, qword ptr [r12 + 248]
 sub rax, rcx
 mov qword ptr [r12 + 208], 6
 mov qword ptr [r12 + 216], rax
 jmp xchain0_n10_α
.Lx10_0:
 mov rdi, qword ptr [r12 + 224]
 mov rsi, qword ptr [r12 + 232]
 mov rdx, qword ptr [r12 + 240]
 mov rcx, qword ptr [r12 + 248]
 mov r8d, 1
 lea r9, [r12 + 208]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx10_3
.Lx10_2:
 mov rdi, qword ptr [r12 + 224]
 mov rsi, qword ptr [r12 + 232]
 mov rdx, qword ptr [r12 + 240]
 mov rcx, qword ptr [r12 + 248]
 mov r8d, 1
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n11_α
 mov qword ptr [r12 + 208], rax
 mov qword ptr [r12 + 216], rdx
.Lx10_3:
 jmp xchain0_n10_α
 xchain0_n8_β:
 jmp xchain0_n11_α
# IR_ASSIGN gva
 xchain0_n9_α:
 mov rax, qword ptr [r12 + 144]
 mov rdx, qword ptr [r12 + 152]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 128], rax
 mov qword ptr [r12 + 136], rdx
 jmp main_γ
 xchain0_n9_β:
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n10_α:
 mov qword ptr [r12 + 256], 6
 mov rax, qword ptr [rip + .Lx12_0]
 mov qword ptr [r12 + 264], rax
 jmp xchain0_n12_α
 xchain0_n10_β:
 jmp xchain0_n11_α
.Lx12_0:
 .quad 1
# IR_LIT_INTEGER
 xchain0_n11_α:
 mov qword ptr [r12 + 368], 6
 mov rax, qword ptr [rip + .Lx13_0]
 mov qword ptr [r12 + 376], rax
 jmp xchain0_n13_α
 xchain0_n11_β:
 jmp xchain0_n18_α
.Lx13_0:
 .quad 3
 xchain0_n12_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+208] -> [r12+176]
 mov rax, qword ptr [r12 + 208]
 mov qword ptr [r12 + 176], rax
 mov rax, qword ptr [r12 + 216]
 mov qword ptr [r12 + 184], rax
# marshal arg1 = producer-box slot [r12+256] -> [r12+192]
 mov rax, qword ptr [r12 + 256]
 mov qword ptr [r12 + 192], rax
 mov rax, qword ptr [r12 + 264]
 mov qword ptr [r12 + 200], rax
  .section .rodata
  .Lbynamefn15: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn15]
 lea rsi, [r12 + 176]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 160], rax
 mov qword ptr [r12 + 168], rdx
 cmp eax, 99
 je xchain0_n11_α
 jmp xchain0_n14_α
xchain0_n12_β:
 jmp xchain0_n11_α
# IR_LIT_INTEGER
 xchain0_n13_α:
 mov qword ptr [r12 + 384], 6
 mov rax, qword ptr [rip + .Lx16_0]
 mov qword ptr [r12 + 392], rax
 jmp xchain0_n15_α
 xchain0_n13_β:
 jmp xchain0_n18_α
.Lx16_0:
 .quad 2
# IR_LIT_STRING
 xchain0_n14_α:
 mov qword ptr [r12 + 288], 1
 mov rax, qword ptr [rip + .Lx17_0]
 mov qword ptr [r12 + 296], rax
 jmp xchain0_n16_α
 xchain0_n14_β:
 jmp main_γ
.Lx17_0:
 .quad .Lx17_0_s
.Lx17_0_s:
 .string "FAIL 410/002: 3-2"
 xchain0_n15_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 368]
 cmp eax, 100
 je .Lx18_0
 mov eax, dword ptr [r12 + 384]
 cmp eax, 100
 je .Lx18_0
 mov eax, dword ptr [r12 + 368]
 cmp eax, 6
 jne .Lx18_2
 mov eax, dword ptr [r12 + 384]
 cmp eax, 6
 jne .Lx18_2
.Lx18_1:
 mov rax, qword ptr [r12 + 376]
 mov rcx, qword ptr [r12 + 392]
 imul rax, rcx
 mov qword ptr [r12 + 352], 6
 mov qword ptr [r12 + 360], rax
 jmp xchain0_n17_α
.Lx18_0:
 mov rdi, qword ptr [r12 + 368]
 mov rsi, qword ptr [r12 + 376]
 mov rdx, qword ptr [r12 + 384]
 mov rcx, qword ptr [r12 + 392]
 mov r8d, 2
 lea r9, [r12 + 352]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx18_3
.Lx18_2:
 mov rdi, qword ptr [r12 + 368]
 mov rsi, qword ptr [r12 + 376]
 mov rdx, qword ptr [r12 + 384]
 mov rcx, qword ptr [r12 + 392]
 mov r8d, 2
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n18_α
 mov qword ptr [r12 + 352], rax
 mov qword ptr [r12 + 360], rdx
.Lx18_3:
 jmp xchain0_n17_α
 xchain0_n15_β:
 jmp xchain0_n18_α
# IR_ASSIGN gva
 xchain0_n16_α:
 mov rax, qword ptr [r12 + 288]
 mov rdx, qword ptr [r12 + 296]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 272], rax
 mov qword ptr [r12 + 280], rdx
 jmp main_γ
 xchain0_n16_β:
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n17_α:
 mov qword ptr [r12 + 400], 6
 mov rax, qword ptr [rip + .Lx20_0]
 mov qword ptr [r12 + 408], rax
 jmp xchain0_n19_α
 xchain0_n17_β:
 jmp xchain0_n18_α
.Lx20_0:
 .quad 6
# IR_LIT_INTEGER
 xchain0_n18_α:
 mov qword ptr [r12 + 512], 6
 mov rax, qword ptr [rip + .Lx21_0]
 mov qword ptr [r12 + 520], rax
 jmp xchain0_n20_α
 xchain0_n18_β:
 jmp xchain0_n25_α
.Lx21_0:
 .quad 5
 xchain0_n19_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+352] -> [r12+320]
 mov rax, qword ptr [r12 + 352]
 mov qword ptr [r12 + 320], rax
 mov rax, qword ptr [r12 + 360]
 mov qword ptr [r12 + 328], rax
# marshal arg1 = producer-box slot [r12+400] -> [r12+336]
 mov rax, qword ptr [r12 + 400]
 mov qword ptr [r12 + 336], rax
 mov rax, qword ptr [r12 + 408]
 mov qword ptr [r12 + 344], rax
  .section .rodata
  .Lbynamefn23: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn23]
 lea rsi, [r12 + 320]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 304], rax
 mov qword ptr [r12 + 312], rdx
 cmp eax, 99
 je xchain0_n18_α
 jmp xchain0_n21_α
xchain0_n19_β:
 jmp xchain0_n18_α
# IR_LIT_INTEGER
 xchain0_n20_α:
 mov qword ptr [r12 + 528], 6
 mov rax, qword ptr [rip + .Lx24_0]
 mov qword ptr [r12 + 536], rax
 jmp xchain0_n22_α
 xchain0_n20_β:
 jmp xchain0_n25_α
.Lx24_0:
 .quad 2
# IR_LIT_STRING
 xchain0_n21_α:
 mov qword ptr [r12 + 432], 1
 mov rax, qword ptr [rip + .Lx25_0]
 mov qword ptr [r12 + 440], rax
 jmp xchain0_n23_α
 xchain0_n21_β:
 jmp main_γ
.Lx25_0:
 .quad .Lx25_0_s
.Lx25_0_s:
 .string "FAIL 410/003: 3*2"
 xchain0_n22_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 512]
 cmp eax, 100
 je .Lx26_0
 mov eax, dword ptr [r12 + 528]
 cmp eax, 100
 je .Lx26_0
 mov eax, dword ptr [r12 + 512]
 cmp eax, 6
 jne .Lx26_2
 mov eax, dword ptr [r12 + 528]
 cmp eax, 6
 jne .Lx26_2
.Lx26_1:
 mov rax, qword ptr [r12 + 520]
 mov rcx, qword ptr [r12 + 536]
 cqo
 idiv rcx
 mov qword ptr [r12 + 496], 6
 mov qword ptr [r12 + 504], rax
 jmp xchain0_n24_α
.Lx26_0:
 mov rdi, qword ptr [r12 + 512]
 mov rsi, qword ptr [r12 + 520]
 mov rdx, qword ptr [r12 + 528]
 mov rcx, qword ptr [r12 + 536]
 mov r8d, 3
 lea r9, [r12 + 496]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx26_3
.Lx26_2:
 mov rdi, qword ptr [r12 + 512]
 mov rsi, qword ptr [r12 + 520]
 mov rdx, qword ptr [r12 + 528]
 mov rcx, qword ptr [r12 + 536]
 mov r8d, 3
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n25_α
 mov qword ptr [r12 + 496], rax
 mov qword ptr [r12 + 504], rdx
.Lx26_3:
 jmp xchain0_n24_α
 xchain0_n22_β:
 jmp xchain0_n25_α
# IR_ASSIGN gva
 xchain0_n23_α:
 mov rax, qword ptr [r12 + 432]
 mov rdx, qword ptr [r12 + 440]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 416], rax
 mov qword ptr [r12 + 424], rdx
 jmp main_γ
 xchain0_n23_β:
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n24_α:
 mov qword ptr [r12 + 544], 6
 mov rax, qword ptr [rip + .Lx28_0]
 mov qword ptr [r12 + 552], rax
 jmp xchain0_n26_α
 xchain0_n24_β:
 jmp xchain0_n25_α
.Lx28_0:
 .quad 2
# IR_LIT_INTEGER
 xchain0_n25_α:
 mov qword ptr [r12 + 656], 6
 mov rax, qword ptr [rip + .Lx29_0]
 mov qword ptr [r12 + 664], rax
 jmp xchain0_n27_α
 xchain0_n25_β:
 jmp xchain0_n32_α
.Lx29_0:
 .quad 2
 xchain0_n26_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+496] -> [r12+464]
 mov rax, qword ptr [r12 + 496]
 mov qword ptr [r12 + 464], rax
 mov rax, qword ptr [r12 + 504]
 mov qword ptr [r12 + 472], rax
# marshal arg1 = producer-box slot [r12+544] -> [r12+480]
 mov rax, qword ptr [r12 + 544]
 mov qword ptr [r12 + 480], rax
 mov rax, qword ptr [r12 + 552]
 mov qword ptr [r12 + 488], rax
  .section .rodata
  .Lbynamefn31: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn31]
 lea rsi, [r12 + 464]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 448], rax
 mov qword ptr [r12 + 456], rdx
 cmp eax, 99
 je xchain0_n25_α
 jmp xchain0_n28_α
xchain0_n26_β:
 jmp xchain0_n25_α
# IR_LIT_INTEGER
 xchain0_n27_α:
 mov qword ptr [r12 + 672], 6
 mov rax, qword ptr [rip + .Lx32_0]
 mov qword ptr [r12 + 680], rax
 jmp xchain0_n29_α
 xchain0_n27_β:
 jmp xchain0_n32_α
.Lx32_0:
 .quad 3
# IR_LIT_STRING
 xchain0_n28_α:
 mov qword ptr [r12 + 576], 1
 mov rax, qword ptr [rip + .Lx33_0]
 mov qword ptr [r12 + 584], rax
 jmp xchain0_n30_α
 xchain0_n28_β:
 jmp main_γ
.Lx33_0:
 .quad .Lx33_0_s
.Lx33_0_s:
 .string "FAIL 410/004: 5/2 integer division"
 xchain0_n29_α:
# IR_BINOP_ARITH_REAL
 mov rdi, qword ptr [r12 + 656]
 mov rsi, qword ptr [r12 + 664]
 mov rdx, qword ptr [r12 + 672]
 mov rcx, qword ptr [r12 + 680]
 mov r8d, 18
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n32_α
 mov qword ptr [r12 + 640], rax
 mov qword ptr [r12 + 648], rdx
 jmp xchain0_n31_α
 xchain0_n29_β:
 jmp xchain0_n32_α
# IR_ASSIGN gva
 xchain0_n30_α:
 mov rax, qword ptr [r12 + 576]
 mov rdx, qword ptr [r12 + 584]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 560], rax
 mov qword ptr [r12 + 568], rdx
 jmp main_γ
 xchain0_n30_β:
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n31_α:
 mov qword ptr [r12 + 688], 6
 mov rax, qword ptr [rip + .Lx36_0]
 mov qword ptr [r12 + 696], rax
 jmp xchain0_n33_α
 xchain0_n31_β:
 jmp xchain0_n32_α
.Lx36_0:
 .quad 8
# IR_LIT_STRING
 xchain0_n32_α:
 mov qword ptr [r12 + 800], 1
 mov rax, qword ptr [rip + .Lx37_0]
 mov qword ptr [r12 + 808], rax
 jmp xchain0_n34_α
 xchain0_n32_β:
 jmp xchain0_n39_α
.Lx37_0:
 .quad .Lx37_0_s
.Lx37_0_s:
 .string "3"
 xchain0_n33_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+640] -> [r12+608]
 mov rax, qword ptr [r12 + 640]
 mov qword ptr [r12 + 608], rax
 mov rax, qword ptr [r12 + 648]
 mov qword ptr [r12 + 616], rax
# marshal arg1 = producer-box slot [r12+688] -> [r12+624]
 mov rax, qword ptr [r12 + 688]
 mov qword ptr [r12 + 624], rax
 mov rax, qword ptr [r12 + 696]
 mov qword ptr [r12 + 632], rax
  .section .rodata
  .Lbynamefn39: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn39]
 lea rsi, [r12 + 608]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 592], rax
 mov qword ptr [r12 + 600], rdx
 cmp eax, 99
 je xchain0_n32_α
 jmp xchain0_n35_α
xchain0_n33_β:
 jmp xchain0_n32_α
# IR_LIT_INTEGER
 xchain0_n34_α:
 mov qword ptr [r12 + 816], 6
 mov rax, qword ptr [rip + .Lx40_0]
 mov qword ptr [r12 + 824], rax
 jmp xchain0_n36_α
 xchain0_n34_β:
 jmp xchain0_n39_α
.Lx40_0:
 .quad 2
# IR_LIT_STRING
 xchain0_n35_α:
 mov qword ptr [r12 + 720], 1
 mov rax, qword ptr [rip + .Lx41_0]
 mov qword ptr [r12 + 728], rax
 jmp xchain0_n37_α
 xchain0_n35_β:
 jmp main_γ
.Lx41_0:
 .quad .Lx41_0_s
.Lx41_0_s:
 .string "FAIL 410/005: 2**3"
 xchain0_n36_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 800]
 cmp eax, 100
 je .Lx42_0
 mov eax, dword ptr [r12 + 816]
 cmp eax, 100
 je .Lx42_0
 mov eax, dword ptr [r12 + 800]
 cmp eax, 6
 jne .Lx42_2
 mov eax, dword ptr [r12 + 816]
 cmp eax, 6
 jne .Lx42_2
.Lx42_1:
 mov rax, qword ptr [r12 + 808]
 mov rcx, qword ptr [r12 + 824]
 add rax, rcx
 mov qword ptr [r12 + 784], 6
 mov qword ptr [r12 + 792], rax
 jmp xchain0_n38_α
.Lx42_0:
 mov rdi, qword ptr [r12 + 800]
 mov rsi, qword ptr [r12 + 808]
 mov rdx, qword ptr [r12 + 816]
 mov rcx, qword ptr [r12 + 824]
 mov r8d, 0
 lea r9, [r12 + 784]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx42_3
.Lx42_2:
 mov rdi, qword ptr [r12 + 800]
 mov rsi, qword ptr [r12 + 808]
 mov rdx, qword ptr [r12 + 816]
 mov rcx, qword ptr [r12 + 824]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n39_α
 mov qword ptr [r12 + 784], rax
 mov qword ptr [r12 + 792], rdx
.Lx42_3:
 jmp xchain0_n38_α
 xchain0_n36_β:
 jmp xchain0_n39_α
# IR_ASSIGN gva
 xchain0_n37_α:
 mov rax, qword ptr [r12 + 720]
 mov rdx, qword ptr [r12 + 728]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 704], rax
 mov qword ptr [r12 + 712], rdx
 jmp main_γ
 xchain0_n37_β:
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n38_α:
 mov qword ptr [r12 + 832], 6
 mov rax, qword ptr [rip + .Lx44_0]
 mov qword ptr [r12 + 840], rax
 jmp xchain0_n40_α
 xchain0_n38_β:
 jmp xchain0_n39_α
.Lx44_0:
 .quad 5
# IR_LIT_INTEGER
 xchain0_n39_α:
 mov qword ptr [r12 + 944], 6
 mov rax, qword ptr [rip + .Lx45_0]
 mov qword ptr [r12 + 952], rax
 jmp xchain0_n41_α
 xchain0_n39_β:
 jmp xchain0_n46_α
.Lx45_0:
 .quad 3
 xchain0_n40_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+784] -> [r12+752]
 mov rax, qword ptr [r12 + 784]
 mov qword ptr [r12 + 752], rax
 mov rax, qword ptr [r12 + 792]
 mov qword ptr [r12 + 760], rax
# marshal arg1 = producer-box slot [r12+832] -> [r12+768]
 mov rax, qword ptr [r12 + 832]
 mov qword ptr [r12 + 768], rax
 mov rax, qword ptr [r12 + 840]
 mov qword ptr [r12 + 776], rax
  .section .rodata
  .Lbynamefn47: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn47]
 lea rsi, [r12 + 752]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 736], rax
 mov qword ptr [r12 + 744], rdx
 cmp eax, 99
 je xchain0_n39_α
 jmp xchain0_n42_α
xchain0_n40_β:
 jmp xchain0_n39_α
# IR_LIT_STRING
 xchain0_n41_α:
 mov qword ptr [r12 + 960], 1
 mov rax, qword ptr [rip + .Lx48_0]
 mov qword ptr [r12 + 968], rax
 jmp xchain0_n43_α
 xchain0_n41_β:
 jmp xchain0_n46_α
.Lx48_0:
 .quad .Lx48_0_s
.Lx48_0_s:
 .string "-2"
# IR_LIT_STRING
 xchain0_n42_α:
 mov qword ptr [r12 + 864], 1
 mov rax, qword ptr [rip + .Lx49_0]
 mov qword ptr [r12 + 872], rax
 jmp xchain0_n44_α
 xchain0_n42_β:
 jmp main_γ
.Lx49_0:
 .quad .Lx49_0_s
.Lx49_0_s:
 .string "FAIL 410/006: string+int coerce"
 xchain0_n43_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 944]
 cmp eax, 100
 je .Lx50_0
 mov eax, dword ptr [r12 + 960]
 cmp eax, 100
 je .Lx50_0
 mov eax, dword ptr [r12 + 944]
 cmp eax, 6
 jne .Lx50_2
 mov eax, dword ptr [r12 + 960]
 cmp eax, 6
 jne .Lx50_2
.Lx50_1:
 mov rax, qword ptr [r12 + 952]
 mov rcx, qword ptr [r12 + 968]
 add rax, rcx
 mov qword ptr [r12 + 928], 6
 mov qword ptr [r12 + 936], rax
 jmp xchain0_n45_α
.Lx50_0:
 mov rdi, qword ptr [r12 + 944]
 mov rsi, qword ptr [r12 + 952]
 mov rdx, qword ptr [r12 + 960]
 mov rcx, qword ptr [r12 + 968]
 mov r8d, 0
 lea r9, [r12 + 928]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx50_3
.Lx50_2:
 mov rdi, qword ptr [r12 + 944]
 mov rsi, qword ptr [r12 + 952]
 mov rdx, qword ptr [r12 + 960]
 mov rcx, qword ptr [r12 + 968]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n46_α
 mov qword ptr [r12 + 928], rax
 mov qword ptr [r12 + 936], rdx
.Lx50_3:
 jmp xchain0_n45_α
 xchain0_n43_β:
 jmp xchain0_n46_α
# IR_ASSIGN gva
 xchain0_n44_α:
 mov rax, qword ptr [r12 + 864]
 mov rdx, qword ptr [r12 + 872]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 848], rax
 mov qword ptr [r12 + 856], rdx
 jmp main_γ
 xchain0_n44_β:
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n45_α:
 mov qword ptr [r12 + 976], 6
 mov rax, qword ptr [rip + .Lx52_0]
 mov qword ptr [r12 + 984], rax
 jmp xchain0_n47_α
 xchain0_n45_β:
 jmp xchain0_n46_α
.Lx52_0:
 .quad 1
# IR_LIT_STRING
 xchain0_n46_α:
 mov qword ptr [r12 + 1088], 1
 mov rax, qword ptr [rip + .Lx53_0]
 mov qword ptr [r12 + 1096], rax
 jmp xchain0_n48_α
 xchain0_n46_β:
 jmp xchain0_n53_α
.Lx53_0:
 .quad .Lx53_0_s
.Lx53_0_s:
 .string "1"
 xchain0_n47_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+928] -> [r12+896]
 mov rax, qword ptr [r12 + 928]
 mov qword ptr [r12 + 896], rax
 mov rax, qword ptr [r12 + 936]
 mov qword ptr [r12 + 904], rax
# marshal arg1 = producer-box slot [r12+976] -> [r12+912]
 mov rax, qword ptr [r12 + 976]
 mov qword ptr [r12 + 912], rax
 mov rax, qword ptr [r12 + 984]
 mov qword ptr [r12 + 920], rax
  .section .rodata
  .Lbynamefn55: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn55]
 lea rsi, [r12 + 896]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 880], rax
 mov qword ptr [r12 + 888], rdx
 cmp eax, 99
 je xchain0_n46_α
 jmp xchain0_n49_α
xchain0_n47_β:
 jmp xchain0_n46_α
# IR_LIT_STRING
 xchain0_n48_α:
 mov qword ptr [r12 + 1104], 1
 mov rax, qword ptr [rip + .Lx56_0]
 mov qword ptr [r12 + 1112], rax
 jmp xchain0_n50_α
 xchain0_n48_β:
 jmp xchain0_n53_α
.Lx56_0:
 .quad .Lx56_0_s
.Lx56_0_s:
 .string "0"
# IR_LIT_STRING
 xchain0_n49_α:
 mov qword ptr [r12 + 1008], 1
 mov rax, qword ptr [rip + .Lx57_0]
 mov qword ptr [r12 + 1016], rax
 jmp xchain0_n51_α
 xchain0_n49_β:
 jmp main_γ
.Lx57_0:
 .quad .Lx57_0_s
.Lx57_0_s:
 .string "FAIL 410/007: int+neg-string"
 xchain0_n50_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 1088]
 cmp eax, 100
 je .Lx58_0
 mov eax, dword ptr [r12 + 1104]
 cmp eax, 100
 je .Lx58_0
 mov eax, dword ptr [r12 + 1088]
 cmp eax, 6
 jne .Lx58_2
 mov eax, dword ptr [r12 + 1104]
 cmp eax, 6
 jne .Lx58_2
.Lx58_1:
 mov rax, qword ptr [r12 + 1096]
 mov rcx, qword ptr [r12 + 1112]
 add rax, rcx
 mov qword ptr [r12 + 1072], 6
 mov qword ptr [r12 + 1080], rax
 jmp xchain0_n52_α
.Lx58_0:
 mov rdi, qword ptr [r12 + 1088]
 mov rsi, qword ptr [r12 + 1096]
 mov rdx, qword ptr [r12 + 1104]
 mov rcx, qword ptr [r12 + 1112]
 mov r8d, 0
 lea r9, [r12 + 1072]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx58_3
.Lx58_2:
 mov rdi, qword ptr [r12 + 1088]
 mov rsi, qword ptr [r12 + 1096]
 mov rdx, qword ptr [r12 + 1104]
 mov rcx, qword ptr [r12 + 1112]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n53_α
 mov qword ptr [r12 + 1072], rax
 mov qword ptr [r12 + 1080], rdx
.Lx58_3:
 jmp xchain0_n52_α
 xchain0_n50_β:
 jmp xchain0_n53_α
# IR_ASSIGN gva
 xchain0_n51_α:
 mov rax, qword ptr [r12 + 1008]
 mov rdx, qword ptr [r12 + 1016]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 992], rax
 mov qword ptr [r12 + 1000], rdx
 jmp main_γ
 xchain0_n51_β:
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n52_α:
 mov qword ptr [r12 + 1120], 6
 mov rax, qword ptr [rip + .Lx60_0]
 mov qword ptr [r12 + 1128], rax
 jmp xchain0_n54_α
 xchain0_n52_β:
 jmp xchain0_n53_α
.Lx60_0:
 .quad 1
# IR_LIT_INTEGER
 xchain0_n53_α:
 mov qword ptr [r12 + 1232], 6
 mov rax, qword ptr [rip + .Lx61_0]
 mov qword ptr [r12 + 1240], rax
 jmp xchain0_n55_α
 xchain0_n53_β:
 jmp xchain0_n60_α
.Lx61_0:
 .quad 5
 xchain0_n54_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+1072] -> [r12+1040]
 mov rax, qword ptr [r12 + 1072]
 mov qword ptr [r12 + 1040], rax
 mov rax, qword ptr [r12 + 1080]
 mov qword ptr [r12 + 1048], rax
# marshal arg1 = producer-box slot [r12+1120] -> [r12+1056]
 mov rax, qword ptr [r12 + 1120]
 mov qword ptr [r12 + 1056], rax
 mov rax, qword ptr [r12 + 1128]
 mov qword ptr [r12 + 1064], rax
  .section .rodata
  .Lbynamefn63: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn63]
 lea rsi, [r12 + 1040]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1024], rax
 mov qword ptr [r12 + 1032], rdx
 cmp eax, 99
 je xchain0_n53_α
 jmp xchain0_n56_α
xchain0_n54_β:
 jmp xchain0_n53_α
# IR_LIT_STRING
 xchain0_n55_α:
 mov qword ptr [r12 + 1248], 1
 mov rax, qword ptr [rip + .Lx64_0]
 mov qword ptr [r12 + 1256], rax
 jmp xchain0_n57_α
 xchain0_n55_β:
 jmp xchain0_n60_α
.Lx64_0:
 .quad .Lx64_0_s
.Lx64_0_s:
 .string ""
# IR_LIT_STRING
 xchain0_n56_α:
 mov qword ptr [r12 + 1152], 1
 mov rax, qword ptr [rip + .Lx65_0]
 mov qword ptr [r12 + 1160], rax
 jmp xchain0_n58_α
 xchain0_n56_β:
 jmp main_γ
.Lx65_0:
 .quad .Lx65_0_s
.Lx65_0_s:
 .string "FAIL 410/008: string+string coerce"
 xchain0_n57_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 1232]
 cmp eax, 100
 je .Lx66_0
 mov eax, dword ptr [r12 + 1248]
 cmp eax, 100
 je .Lx66_0
 mov eax, dword ptr [r12 + 1232]
 cmp eax, 6
 jne .Lx66_2
 mov eax, dword ptr [r12 + 1248]
 cmp eax, 6
 jne .Lx66_2
.Lx66_1:
 mov rax, qword ptr [r12 + 1240]
 mov rcx, qword ptr [r12 + 1256]
 add rax, rcx
 mov qword ptr [r12 + 1216], 6
 mov qword ptr [r12 + 1224], rax
 jmp xchain0_n59_α
.Lx66_0:
 mov rdi, qword ptr [r12 + 1232]
 mov rsi, qword ptr [r12 + 1240]
 mov rdx, qword ptr [r12 + 1248]
 mov rcx, qword ptr [r12 + 1256]
 mov r8d, 0
 lea r9, [r12 + 1216]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx66_3
.Lx66_2:
 mov rdi, qword ptr [r12 + 1232]
 mov rsi, qword ptr [r12 + 1240]
 mov rdx, qword ptr [r12 + 1248]
 mov rcx, qword ptr [r12 + 1256]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n60_α
 mov qword ptr [r12 + 1216], rax
 mov qword ptr [r12 + 1224], rdx
.Lx66_3:
 jmp xchain0_n59_α
 xchain0_n57_β:
 jmp xchain0_n60_α
# IR_ASSIGN gva
 xchain0_n58_α:
 mov rax, qword ptr [r12 + 1152]
 mov rdx, qword ptr [r12 + 1160]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 1136], rax
 mov qword ptr [r12 + 1144], rdx
 jmp main_γ
 xchain0_n58_β:
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n59_α:
 mov qword ptr [r12 + 1264], 6
 mov rax, qword ptr [rip + .Lx68_0]
 mov qword ptr [r12 + 1272], rax
 jmp xchain0_n61_α
 xchain0_n59_β:
 jmp xchain0_n60_α
.Lx68_0:
 .quad 5
# IR_LIT_STRING
 xchain0_n60_α:
 mov qword ptr [r12 + 1328], 1
 mov rax, qword ptr [rip + .Lx69_0]
 mov qword ptr [r12 + 1336], rax
 jmp xchain0_n62_α
 xchain0_n60_β:
 jmp main_γ
.Lx69_0:
 .quad .Lx69_0_s
.Lx69_0_s:
 .string "PASS 410_arith_int (9/9)"
 xchain0_n61_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+1216] -> [r12+1184]
 mov rax, qword ptr [r12 + 1216]
 mov qword ptr [r12 + 1184], rax
 mov rax, qword ptr [r12 + 1224]
 mov qword ptr [r12 + 1192], rax
# marshal arg1 = producer-box slot [r12+1264] -> [r12+1200]
 mov rax, qword ptr [r12 + 1264]
 mov qword ptr [r12 + 1200], rax
 mov rax, qword ptr [r12 + 1272]
 mov qword ptr [r12 + 1208], rax
  .section .rodata
  .Lbynamefn71: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn71]
 lea rsi, [r12 + 1184]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1168], rax
 mov qword ptr [r12 + 1176], rdx
 cmp eax, 99
 je xchain0_n60_α
 jmp xchain0_n63_α
xchain0_n61_β:
 jmp xchain0_n60_α
# IR_ASSIGN gva
 xchain0_n62_α:
 mov rax, qword ptr [r12 + 1328]
 mov rdx, qword ptr [r12 + 1336]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 1312], rax
 mov qword ptr [r12 + 1320], rdx
 jmp main_γ
 xchain0_n62_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n63_α:
 mov qword ptr [r12 + 1296], 1
 mov rax, qword ptr [rip + .Lx73_0]
 mov qword ptr [r12 + 1304], rax
 jmp xchain0_n64_α
 xchain0_n63_β:
 jmp main_γ
.Lx73_0:
 .quad .Lx73_0_s
.Lx73_0_s:
 .string "FAIL 410/009: null addend is zero"
# IR_ASSIGN gva
 xchain0_n64_α:
 mov rax, qword ptr [r12 + 1296]
 mov rdx, qword ptr [r12 + 1304]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 1280], rax
 mov qword ptr [r12 + 1288], rdx
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
 mov rdi, qword ptr [r12 + 1344]
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
