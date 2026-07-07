  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "t"
  .Lgvan1: .string "output"
  .Lgvan2: .string "ta"
  .Lgvan3: .string "ata"
  .align 8
__gva_names:
  .quad .Lgvan0
  .quad .Lgvan1
  .quad .Lgvan2
  .quad .Lgvan3
  .section .bss
  .align 16
__gva: .space 64, 0
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
  mov edx, 4
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
 mov qword ptr [r12 + 1776], rax
 pop rsi
main_α_body:
xchain0_n0_α:
# IR_LIT_INTEGER
bb1_α:
 mov qword ptr [r12 + 64], 6
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 72], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n3_α
.Lx1_0:
 .quad 10
xchain0_n1_α:
bb2_α:
# BOX IR_CALL table(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [r12+64] -> [r12+48]
 mov rax, qword ptr [r12 + 64]
 mov qword ptr [r12 + 48], rax
 mov rax, qword ptr [r12 + 72]
 mov qword ptr [r12 + 56], rax
  .section .rodata
  .Lrkfn3: .string "table"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn3]
 lea rsi, [r12 + 48]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je xchain0_n3_α
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n3_α
xchain0_n2_α:
# IR_ASSIGN gva
bb3_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n3_α
xchain0_n3_α:
# IR_VAR gva
bb4_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 112], rax
 mov qword ptr [r12 + 120], rdx
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp xchain0_n5_α
xchain0_n4_α:
# IR_LIT_STRING
bb5_α:
 mov qword ptr [r12 + 128], 1
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [r12 + 136], rax
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp xchain0_n5_α
.Lx6_0:
 .quad .Lx6_0_s
.Lx6_0_s:
 .string "cat"
xchain0_n5_α:
# IR_VAR gva
bb6_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 208], rax
 mov qword ptr [r12 + 216], rdx
 jmp xchain0_n7_α
 xchain0_n5_β:
 jmp xchain0_n8_α
xchain0_n6_α:
# IR_SUBSCRIPT x[i] variable
bb7_α:
 mov rdi, qword ptr [r12 + 112]
 mov rsi, qword ptr [r12 + 120]
 mov rdx, qword ptr [r12 + 128]
 mov rcx, qword ptr [r12 + 136]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n5_α
 mov qword ptr [r12 + 144], rax
 mov qword ptr [r12 + 152], rdx
 jmp xchain0_n9_α
 xchain0_n6_β:
 jmp xchain0_n5_α
xchain0_n7_α:
# IR_LIT_STRING
bb8_α:
 mov qword ptr [r12 + 224], 1
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [r12 + 232], rax
 jmp xchain0_n10_α
 xchain0_n7_β:
 jmp xchain0_n8_α
.Lx9_0:
 .quad .Lx9_0_s
.Lx9_0_s:
 .string "cat"
xchain0_n8_α:
# IR_VAR gva
bb9_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 336], rax
 mov qword ptr [r12 + 344], rdx
 jmp xchain0_n11_α
 xchain0_n8_β:
 jmp xchain0_n12_α
xchain0_n9_α:
# IR_DEREF variable -> value
bb10_α:
 mov rdi, qword ptr [r12 + 144]
 mov rsi, qword ptr [r12 + 152]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n5_α
 mov qword ptr [r12 + 160], rax
 mov qword ptr [r12 + 168], rdx
 jmp xchain0_n13_α
 xchain0_n9_β:
 jmp xchain0_n5_α
xchain0_n10_α:
# IR_SUBSCRIPT x[i] variable
bb11_α:
 mov rdi, qword ptr [r12 + 208]
 mov rsi, qword ptr [r12 + 216]
 mov rdx, qword ptr [r12 + 224]
 mov rcx, qword ptr [r12 + 232]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n8_α
 mov qword ptr [r12 + 240], rax
 mov qword ptr [r12 + 248], rdx
 jmp xchain0_n14_α
 xchain0_n10_β:
 jmp xchain0_n8_α
xchain0_n11_α:
# IR_LIT_STRING
bb12_α:
 mov qword ptr [r12 + 352], 1
 mov rax, qword ptr [rip + .Lx13_0]
 mov qword ptr [r12 + 360], rax
 jmp xchain0_n15_α
 xchain0_n11_β:
 jmp xchain0_n12_α
.Lx13_0:
 .quad .Lx13_0_s
.Lx13_0_s:
 .string "cat"
xchain0_n12_α:
# IR_VAR gva
bb13_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 448], rax
 mov qword ptr [r12 + 456], rdx
 jmp xchain0_n16_α
 xchain0_n12_β:
 jmp xchain0_n17_α
xchain0_n13_α:
bb14_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+160] -> [r12+96]
 mov rax, qword ptr [r12 + 160]
 mov qword ptr [r12 + 96], rax
 mov rax, qword ptr [r12 + 168]
 mov qword ptr [r12 + 104], rax
  .section .rodata
  .Lbynamefn16: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn16]
 lea rsi, [r12 + 96]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 80], rax
 mov qword ptr [r12 + 88], rdx
 cmp eax, 99
 je xchain0_n5_α
 jmp xchain0_n18_α
xchain0_n13_β:
 jmp xchain0_n5_α
xchain0_n14_α:
# IR_LIT_STRING
bb15_α:
 mov qword ptr [r12 + 256], 1
 mov rax, qword ptr [rip + .Lx17_0]
 mov qword ptr [r12 + 264], rax
 jmp xchain0_n19_α
 xchain0_n14_β:
 jmp xchain0_n8_α
.Lx17_0:
 .quad .Lx17_0_s
.Lx17_0_s:
 .string "dog"
xchain0_n15_α:
# IR_SUBSCRIPT x[i] variable
bb16_α:
 mov rdi, qword ptr [r12 + 336]
 mov rsi, qword ptr [r12 + 344]
 mov rdx, qword ptr [r12 + 352]
 mov rcx, qword ptr [r12 + 360]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n12_α
 mov qword ptr [r12 + 368], rax
 mov qword ptr [r12 + 376], rdx
 jmp xchain0_n20_α
 xchain0_n15_β:
 jmp xchain0_n12_α
xchain0_n16_α:
# IR_LIT_INTEGER
bb17_α:
 mov qword ptr [r12 + 464], 6
 mov rax, qword ptr [rip + .Lx19_0]
 mov qword ptr [r12 + 472], rax
 jmp xchain0_n21_α
 xchain0_n16_β:
 jmp xchain0_n17_α
.Lx19_0:
 .quad 7
xchain0_n17_α:
# IR_VAR gva
bb18_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 576], rax
 mov qword ptr [r12 + 584], rdx
 jmp xchain0_n22_α
 xchain0_n17_β:
 jmp xchain0_n23_α
xchain0_n18_α:
# IR_LIT_STRING
bb19_α:
 mov qword ptr [r12 + 192], 1
 mov rax, qword ptr [rip + .Lx21_0]
 mov qword ptr [r12 + 200], rax
 jmp xchain0_n24_α
 xchain0_n18_β:
 jmp main_γ
.Lx21_0:
 .quad .Lx21_0_s
.Lx21_0_s:
 .string "FAIL 1113/001: absent key is null"
xchain0_n19_α:
# IR_ASSIGN_VAR write through variable
bb20_α:
 mov rdi, qword ptr [r12 + 240]
 mov rsi, qword ptr [r12 + 248]
 mov rdx, qword ptr [r12 + 256]
 mov rcx, qword ptr [r12 + 264]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n8_α
 mov qword ptr [r12 + 272], rax
 mov qword ptr [r12 + 280], rdx
 jmp xchain0_n8_α
 xchain0_n19_β:
 jmp xchain0_n8_α
xchain0_n20_α:
# IR_DEREF variable -> value
bb21_α:
 mov rdi, qword ptr [r12 + 368]
 mov rsi, qword ptr [r12 + 376]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n12_α
 mov qword ptr [r12 + 384], rax
 mov qword ptr [r12 + 392], rdx
 jmp xchain0_n25_α
 xchain0_n20_β:
 jmp xchain0_n12_α
xchain0_n21_α:
# IR_SUBSCRIPT x[i] variable
bb22_α:
 mov rdi, qword ptr [r12 + 448]
 mov rsi, qword ptr [r12 + 456]
 mov rdx, qword ptr [r12 + 464]
 mov rcx, qword ptr [r12 + 472]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n17_α
 mov qword ptr [r12 + 480], rax
 mov qword ptr [r12 + 488], rdx
 jmp xchain0_n26_α
 xchain0_n21_β:
 jmp xchain0_n17_α
xchain0_n22_α:
# IR_LIT_INTEGER
bb23_α:
 mov qword ptr [r12 + 592], 6
 mov rax, qword ptr [rip + .Lx25_0]
 mov qword ptr [r12 + 600], rax
 jmp xchain0_n27_α
 xchain0_n22_β:
 jmp xchain0_n23_α
.Lx25_0:
 .quad 7
xchain0_n23_α:
# IR_VAR gva
bb24_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 736], rax
 mov qword ptr [r12 + 744], rdx
 jmp xchain0_n28_α
 xchain0_n23_β:
 jmp xchain0_n29_α
xchain0_n24_α:
# IR_ASSIGN gva
bb25_α:
 mov rax, qword ptr [r12 + 192]
 mov rdx, qword ptr [r12 + 200]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 176], rax
 mov qword ptr [r12 + 184], rdx
 jmp main_γ
 xchain0_n24_β:
 jmp main_γ
xchain0_n25_α:
# IR_LIT_STRING
bb26_α:
 mov qword ptr [r12 + 400], 1
 mov rax, qword ptr [rip + .Lx28_0]
 mov qword ptr [r12 + 408], rax
 jmp xchain0_n30_α
 xchain0_n25_β:
 jmp xchain0_n12_α
.Lx28_0:
 .quad .Lx28_0_s
.Lx28_0_s:
 .string "dog"
xchain0_n26_α:
# IR_LIT_INTEGER
bb27_α:
 mov qword ptr [r12 + 496], 6
 mov rax, qword ptr [rip + .Lx29_0]
 mov qword ptr [r12 + 504], rax
 jmp xchain0_n31_α
 xchain0_n26_β:
 jmp xchain0_n17_α
.Lx29_0:
 .quad 45
xchain0_n27_α:
# IR_SUBSCRIPT x[i] variable
bb28_α:
 mov rdi, qword ptr [r12 + 576]
 mov rsi, qword ptr [r12 + 584]
 mov rdx, qword ptr [r12 + 592]
 mov rcx, qword ptr [r12 + 600]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n23_α
 mov qword ptr [r12 + 608], rax
 mov qword ptr [r12 + 616], rdx
 jmp xchain0_n32_α
 xchain0_n27_β:
 jmp xchain0_n23_α
xchain0_n28_α:
# IR_LIT_STRING
bb29_α:
 mov qword ptr [r12 + 752], 1
 mov rax, qword ptr [rip + .Lx31_0]
 mov qword ptr [r12 + 760], rax
 jmp xchain0_n33_α
 xchain0_n28_β:
 jmp xchain0_n29_α
.Lx31_0:
 .quad .Lx31_0_s
.Lx31_0_s:
 .string "cat"
xchain0_n29_α:
# IR_VAR gva
bb30_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 912], rax
 mov qword ptr [r12 + 920], rdx
 jmp xchain0_n34_α
 xchain0_n29_β:
 jmp xchain0_n35_α
xchain0_n30_α:
bb31_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+384] -> [r12+304]
 mov rax, qword ptr [r12 + 384]
 mov qword ptr [r12 + 304], rax
 mov rax, qword ptr [r12 + 392]
 mov qword ptr [r12 + 312], rax
# marshal arg1 = producer-box slot [r12+400] -> [r12+320]
 mov rax, qword ptr [r12 + 400]
 mov qword ptr [r12 + 320], rax
 mov rax, qword ptr [r12 + 408]
 mov qword ptr [r12 + 328], rax
  .section .rodata
  .Lbynamefn34: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn34]
 lea rsi, [r12 + 304]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 288], rax
 mov qword ptr [r12 + 296], rdx
 cmp eax, 99
 je xchain0_n12_α
 jmp xchain0_n36_α
xchain0_n30_β:
 jmp xchain0_n12_α
xchain0_n31_α:
# IR_ASSIGN_VAR write through variable
bb32_α:
 mov rdi, qword ptr [r12 + 480]
 mov rsi, qword ptr [r12 + 488]
 mov rdx, qword ptr [r12 + 496]
 mov rcx, qword ptr [r12 + 504]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n17_α
 mov qword ptr [r12 + 512], rax
 mov qword ptr [r12 + 520], rdx
 jmp xchain0_n17_α
 xchain0_n31_β:
 jmp xchain0_n17_α
xchain0_n32_α:
# IR_DEREF variable -> value
bb33_α:
 mov rdi, qword ptr [r12 + 608]
 mov rsi, qword ptr [r12 + 616]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n23_α
 mov qword ptr [r12 + 624], rax
 mov qword ptr [r12 + 632], rdx
 jmp xchain0_n37_α
 xchain0_n32_β:
 jmp xchain0_n23_α
xchain0_n33_α:
# IR_SUBSCRIPT x[i] variable
bb34_α:
 mov rdi, qword ptr [r12 + 736]
 mov rsi, qword ptr [r12 + 744]
 mov rdx, qword ptr [r12 + 752]
 mov rcx, qword ptr [r12 + 760]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n29_α
 mov qword ptr [r12 + 768], rax
 mov qword ptr [r12 + 776], rdx
 jmp xchain0_n38_α
 xchain0_n33_β:
 jmp xchain0_n29_α
xchain0_n34_α:
# IR_LIT_STRING
bb35_α:
 mov qword ptr [r12 + 928], 1
 mov rax, qword ptr [rip + .Lx38_0]
 mov qword ptr [r12 + 936], rax
 jmp xchain0_n39_α
 xchain0_n34_β:
 jmp xchain0_n35_α
.Lx38_0:
 .quad .Lx38_0_s
.Lx38_0_s:
 .string "array"
xchain0_n35_α:
# IR_VAR gva
bb36_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 1024], rax
 mov qword ptr [r12 + 1032], rdx
 jmp xchain0_n40_α
 xchain0_n35_β:
 jmp xchain0_n41_α
xchain0_n36_α:
# IR_LIT_STRING
bb37_α:
 mov qword ptr [r12 + 432], 1
 mov rax, qword ptr [rip + .Lx40_0]
 mov qword ptr [r12 + 440], rax
 jmp xchain0_n42_α
 xchain0_n36_β:
 jmp main_γ
.Lx40_0:
 .quad .Lx40_0_s
.Lx40_0_s:
 .string "FAIL 1113/002: string key assign/read"
xchain0_n37_α:
# IR_LIT_INTEGER
bb38_α:
 mov qword ptr [r12 + 640], 6
 mov rax, qword ptr [rip + .Lx41_0]
 mov qword ptr [r12 + 648], rax
 jmp xchain0_n43_α
 xchain0_n37_β:
 jmp xchain0_n23_α
.Lx41_0:
 .quad 45
xchain0_n38_α:
# IR_DEREF variable -> value
bb39_α:
 mov rdi, qword ptr [r12 + 768]
 mov rsi, qword ptr [r12 + 776]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n29_α
 mov qword ptr [r12 + 784], rax
 mov qword ptr [r12 + 792], rdx
 jmp xchain0_n44_α
 xchain0_n38_β:
 jmp xchain0_n29_α
xchain0_n39_α:
bb40_α:
# BOX IR_CALL convert(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+912] -> [r12+880]
 mov rax, qword ptr [r12 + 912]
 mov qword ptr [r12 + 880], rax
 mov rax, qword ptr [r12 + 920]
 mov qword ptr [r12 + 888], rax
# marshal arg1 = producer-box slot [r12+928] -> [r12+896]
 mov rax, qword ptr [r12 + 928]
 mov qword ptr [r12 + 896], rax
 mov rax, qword ptr [r12 + 936]
 mov qword ptr [r12 + 904], rax
  .section .rodata
  .Lbynamefn44: .string "convert"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn44]
 lea rsi, [r12 + 880]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 864], rax
 mov qword ptr [r12 + 872], rdx
 cmp eax, 99
 je xchain0_n35_α
 jmp xchain0_n45_α
xchain0_n39_β:
 jmp xchain0_n35_α
xchain0_n40_α:
bb41_α:
# BOX IR_CALL prototype(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+1024] -> [r12+1008]
 mov rax, qword ptr [r12 + 1024]
 mov qword ptr [r12 + 1008], rax
 mov rax, qword ptr [r12 + 1032]
 mov qword ptr [r12 + 1016], rax
  .section .rodata
  .Lbynamefn46: .string "prototype"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn46]
 lea rsi, [r12 + 1008]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 992], rax
 mov qword ptr [r12 + 1000], rdx
 cmp eax, 99
 je xchain0_n41_α
 jmp xchain0_n46_α
xchain0_n40_β:
 jmp xchain0_n41_α
xchain0_n41_α:
# IR_VAR gva
bb42_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 1152], rax
 mov qword ptr [r12 + 1160], rdx
 jmp xchain0_n47_α
 xchain0_n41_β:
 jmp xchain0_n48_α
xchain0_n42_α:
# IR_ASSIGN gva
bb43_α:
 mov rax, qword ptr [r12 + 432]
 mov rdx, qword ptr [r12 + 440]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 416], rax
 mov qword ptr [r12 + 424], rdx
 jmp main_γ
 xchain0_n42_β:
 jmp main_γ
xchain0_n43_α:
bb44_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+624] -> [r12+544]
 mov rax, qword ptr [r12 + 624]
 mov qword ptr [r12 + 544], rax
 mov rax, qword ptr [r12 + 632]
 mov qword ptr [r12 + 552], rax
# marshal arg1 = producer-box slot [r12+640] -> [r12+560]
 mov rax, qword ptr [r12 + 640]
 mov qword ptr [r12 + 560], rax
 mov rax, qword ptr [r12 + 648]
 mov qword ptr [r12 + 568], rax
  .section .rodata
  .Lbynamefn50: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn50]
 lea rsi, [r12 + 544]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 528], rax
 mov qword ptr [r12 + 536], rdx
 cmp eax, 99
 je xchain0_n23_α
 jmp xchain0_n49_α
xchain0_n43_β:
 jmp xchain0_n23_α
xchain0_n44_α:
# IR_LIT_STRING
bb45_α:
 mov qword ptr [r12 + 800], 1
 mov rax, qword ptr [rip + .Lx51_0]
 mov qword ptr [r12 + 808], rax
 jmp xchain0_n50_α
 xchain0_n44_β:
 jmp xchain0_n29_α
.Lx51_0:
 .quad .Lx51_0_s
.Lx51_0_s:
 .string "dog"
xchain0_n45_α:
# IR_ASSIGN gva
bb46_α:
 mov rax, qword ptr [r12 + 864]
 mov rdx, qword ptr [r12 + 872]
 mov qword ptr [rbx + 32], rax
 mov qword ptr [rbx + 40], rdx
 mov qword ptr [r12 + 848], rax
 mov qword ptr [r12 + 856], rdx
 jmp xchain0_n35_α
 xchain0_n45_β:
 jmp xchain0_n35_α
xchain0_n46_α:
# IR_LIT_STRING
bb47_α:
 mov qword ptr [r12 + 1040], 1
 mov rax, qword ptr [rip + .Lx53_0]
 mov qword ptr [r12 + 1048], rax
 jmp xchain0_n51_α
 xchain0_n46_β:
 jmp xchain0_n41_α
.Lx53_0:
 .quad .Lx53_0_s
.Lx53_0_s:
 .string "2,2"
xchain0_n47_α:
# IR_LIT_STRING
bb48_α:
 mov qword ptr [r12 + 1168], 1
 mov rax, qword ptr [rip + .Lx54_0]
 mov qword ptr [r12 + 1176], rax
 jmp xchain0_n52_α
 xchain0_n47_β:
 jmp xchain0_n48_α
.Lx54_0:
 .quad .Lx54_0_s
.Lx54_0_s:
 .string "table"
xchain0_n48_α:
# IR_VAR gva
bb49_α:
 mov rax, qword ptr [rbx + 48]
 mov rdx, qword ptr [rbx + 56]
 mov qword ptr [r12 + 1232], rax
 mov qword ptr [r12 + 1240], rdx
 jmp xchain0_n53_α
 xchain0_n48_β:
 jmp xchain0_n54_α
xchain0_n49_α:
# IR_LIT_STRING
bb50_α:
 mov qword ptr [r12 + 672], 1
 mov rax, qword ptr [rip + .Lx56_0]
 mov qword ptr [r12 + 680], rax
 jmp xchain0_n55_α
 xchain0_n49_β:
 jmp main_γ
.Lx56_0:
 .quad .Lx56_0_s
.Lx56_0_s:
 .string "FAIL 1113/003: integer key assign/read"
xchain0_n50_α:
bb51_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+784] -> [r12+704]
 mov rax, qword ptr [r12 + 784]
 mov qword ptr [r12 + 704], rax
 mov rax, qword ptr [r12 + 792]
 mov qword ptr [r12 + 712], rax
# marshal arg1 = producer-box slot [r12+800] -> [r12+720]
 mov rax, qword ptr [r12 + 800]
 mov qword ptr [r12 + 720], rax
 mov rax, qword ptr [r12 + 808]
 mov qword ptr [r12 + 728], rax
  .section .rodata
  .Lbynamefn58: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn58]
 lea rsi, [r12 + 704]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 688], rax
 mov qword ptr [r12 + 696], rdx
 cmp eax, 99
 je xchain0_n29_α
 jmp xchain0_n56_α
xchain0_n50_β:
 jmp xchain0_n29_α
xchain0_n51_α:
bb52_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+992] -> [r12+960]
 mov rax, qword ptr [r12 + 992]
 mov qword ptr [r12 + 960], rax
 mov rax, qword ptr [r12 + 1000]
 mov qword ptr [r12 + 968], rax
# marshal arg1 = producer-box slot [r12+1040] -> [r12+976]
 mov rax, qword ptr [r12 + 1040]
 mov qword ptr [r12 + 976], rax
 mov rax, qword ptr [r12 + 1048]
 mov qword ptr [r12 + 984], rax
  .section .rodata
  .Lbynamefn60: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn60]
 lea rsi, [r12 + 960]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 944], rax
 mov qword ptr [r12 + 952], rdx
 cmp eax, 99
 je xchain0_n41_α
 jmp xchain0_n57_α
xchain0_n51_β:
 jmp xchain0_n41_α
xchain0_n52_α:
bb53_α:
# BOX IR_CALL convert(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+1152] -> [r12+1120]
 mov rax, qword ptr [r12 + 1152]
 mov qword ptr [r12 + 1120], rax
 mov rax, qword ptr [r12 + 1160]
 mov qword ptr [r12 + 1128], rax
# marshal arg1 = producer-box slot [r12+1168] -> [r12+1136]
 mov rax, qword ptr [r12 + 1168]
 mov qword ptr [r12 + 1136], rax
 mov rax, qword ptr [r12 + 1176]
 mov qword ptr [r12 + 1144], rax
  .section .rodata
  .Lbynamefn62: .string "convert"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn62]
 lea rsi, [r12 + 1120]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1104], rax
 mov qword ptr [r12 + 1112], rdx
 cmp eax, 99
 je xchain0_n48_α
 jmp xchain0_n58_α
xchain0_n52_β:
 jmp xchain0_n48_α
xchain0_n53_α:
# IR_LIT_INTEGER
bb54_α:
 mov qword ptr [r12 + 1248], 6
 mov rax, qword ptr [rip + .Lx63_0]
 mov qword ptr [r12 + 1256], rax
 jmp xchain0_n59_α
 xchain0_n53_β:
 jmp xchain0_n54_α
.Lx63_0:
 .quad 7
xchain0_n54_α:
# IR_VAR gva
bb55_α:
 mov rax, qword ptr [rbx + 48]
 mov rdx, qword ptr [rbx + 56]
 mov qword ptr [r12 + 1392], rax
 mov qword ptr [r12 + 1400], rdx
 jmp xchain0_n60_α
 xchain0_n54_β:
 jmp xchain0_n61_α
xchain0_n55_α:
# IR_ASSIGN gva
bb56_α:
 mov rax, qword ptr [r12 + 672]
 mov rdx, qword ptr [r12 + 680]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 656], rax
 mov qword ptr [r12 + 664], rdx
 jmp main_γ
 xchain0_n55_β:
 jmp main_γ
xchain0_n56_α:
# IR_LIT_STRING
bb57_α:
 mov qword ptr [r12 + 832], 1
 mov rax, qword ptr [rip + .Lx66_0]
 mov qword ptr [r12 + 840], rax
 jmp xchain0_n62_α
 xchain0_n56_β:
 jmp main_γ
.Lx66_0:
 .quad .Lx66_0_s
.Lx66_0_s:
 .string "FAIL 1113/004: string key survives int key add"
xchain0_n57_α:
# IR_LIT_STRING
bb58_α:
 mov qword ptr [r12 + 1072], 1
 mov rax, qword ptr [rip + .Lx67_0]
 mov qword ptr [r12 + 1080], rax
 jmp xchain0_n63_α
 xchain0_n57_β:
 jmp main_γ
.Lx67_0:
 .quad .Lx67_0_s
.Lx67_0_s:
 .string "FAIL 1113/005: table->array prototype 2,2"
xchain0_n58_α:
# IR_ASSIGN gva
bb59_α:
 mov rax, qword ptr [r12 + 1104]
 mov rdx, qword ptr [r12 + 1112]
 mov qword ptr [rbx + 48], rax
 mov qword ptr [rbx + 56], rdx
 mov qword ptr [r12 + 1088], rax
 mov qword ptr [r12 + 1096], rdx
 jmp xchain0_n48_α
 xchain0_n58_β:
 jmp xchain0_n48_α
xchain0_n59_α:
# IR_SUBSCRIPT x[i] variable
bb60_α:
 mov rdi, qword ptr [r12 + 1232]
 mov rsi, qword ptr [r12 + 1240]
 mov rdx, qword ptr [r12 + 1248]
 mov rcx, qword ptr [r12 + 1256]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n54_α
 mov qword ptr [r12 + 1264], rax
 mov qword ptr [r12 + 1272], rdx
 jmp xchain0_n64_α
 xchain0_n59_β:
 jmp xchain0_n54_α
xchain0_n60_α:
# IR_LIT_STRING
bb61_α:
 mov qword ptr [r12 + 1408], 1
 mov rax, qword ptr [rip + .Lx70_0]
 mov qword ptr [r12 + 1416], rax
 jmp xchain0_n65_α
 xchain0_n60_β:
 jmp xchain0_n61_α
.Lx70_0:
 .quad .Lx70_0_s
.Lx70_0_s:
 .string "cat"
xchain0_n61_α:
# IR_VAR gva
bb62_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 1504], rax
 mov qword ptr [r12 + 1512], rdx
 jmp xchain0_n66_α
 xchain0_n61_β:
 jmp xchain0_n67_α
xchain0_n62_α:
# IR_ASSIGN gva
bb63_α:
 mov rax, qword ptr [r12 + 832]
 mov rdx, qword ptr [r12 + 840]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 816], rax
 mov qword ptr [r12 + 824], rdx
 jmp main_γ
 xchain0_n62_β:
 jmp main_γ
xchain0_n63_α:
# IR_ASSIGN gva
bb64_α:
 mov rax, qword ptr [r12 + 1072]
 mov rdx, qword ptr [r12 + 1080]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 1056], rax
 mov qword ptr [r12 + 1064], rdx
 jmp main_γ
 xchain0_n63_β:
 jmp main_γ
xchain0_n64_α:
# IR_DEREF variable -> value
bb65_α:
 mov rdi, qword ptr [r12 + 1264]
 mov rsi, qword ptr [r12 + 1272]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n54_α
 mov qword ptr [r12 + 1280], rax
 mov qword ptr [r12 + 1288], rdx
 jmp xchain0_n68_α
 xchain0_n64_β:
 jmp xchain0_n54_α
xchain0_n65_α:
# IR_SUBSCRIPT x[i] variable
bb66_α:
 mov rdi, qword ptr [r12 + 1392]
 mov rsi, qword ptr [r12 + 1400]
 mov rdx, qword ptr [r12 + 1408]
 mov rcx, qword ptr [r12 + 1416]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n61_α
 mov qword ptr [r12 + 1424], rax
 mov qword ptr [r12 + 1432], rdx
 jmp xchain0_n69_α
 xchain0_n65_β:
 jmp xchain0_n61_α
xchain0_n66_α:
# IR_LIT_STRING
bb67_α:
 mov qword ptr [r12 + 1520], 1
 mov rax, qword ptr [rip + .Lx76_0]
 mov qword ptr [r12 + 1528], rax
 jmp xchain0_n70_α
 xchain0_n66_β:
 jmp xchain0_n67_α
.Lx76_0:
 .quad .Lx76_0_s
.Lx76_0_s:
 .string "cat"
xchain0_n67_α:
# IR_VAR gva
bb68_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 1632], rax
 mov qword ptr [r12 + 1640], rdx
 jmp xchain0_n71_α
 xchain0_n67_β:
 jmp xchain0_n72_α
xchain0_n68_α:
# IR_LIT_INTEGER
bb69_α:
 mov qword ptr [r12 + 1296], 6
 mov rax, qword ptr [rip + .Lx78_0]
 mov qword ptr [r12 + 1304], rax
 jmp xchain0_n73_α
 xchain0_n68_β:
 jmp xchain0_n54_α
.Lx78_0:
 .quad 45
xchain0_n69_α:
# IR_DEREF variable -> value
bb70_α:
 mov rdi, qword ptr [r12 + 1424]
 mov rsi, qword ptr [r12 + 1432]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n61_α
 mov qword ptr [r12 + 1440], rax
 mov qword ptr [r12 + 1448], rdx
 jmp xchain0_n74_α
 xchain0_n69_β:
 jmp xchain0_n61_α
xchain0_n70_α:
# IR_SUBSCRIPT x[i] variable
bb71_α:
 mov rdi, qword ptr [r12 + 1504]
 mov rsi, qword ptr [r12 + 1512]
 mov rdx, qword ptr [r12 + 1520]
 mov rcx, qword ptr [r12 + 1528]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n67_α
 mov qword ptr [r12 + 1536], rax
 mov qword ptr [r12 + 1544], rdx
 jmp xchain0_n75_α
 xchain0_n70_β:
 jmp xchain0_n67_α
xchain0_n71_α:
# IR_LIT_STRING
bb72_α:
 mov qword ptr [r12 + 1648], 1
 mov rax, qword ptr [rip + .Lx81_0]
 mov qword ptr [r12 + 1656], rax
 jmp xchain0_n76_α
 xchain0_n71_β:
 jmp xchain0_n72_α
.Lx81_0:
 .quad .Lx81_0_s
.Lx81_0_s:
 .string "cat"
xchain0_n72_α:
# IR_LIT_STRING
bb73_α:
 mov qword ptr [r12 + 1760], 1
 mov rax, qword ptr [rip + .Lx82_0]
 mov qword ptr [r12 + 1768], rax
 jmp xchain0_n77_α
 xchain0_n72_β:
 jmp main_γ
.Lx82_0:
 .quad .Lx82_0_s
.Lx82_0_s:
 .string "PASS 1113_table (8/8)"
xchain0_n73_α:
bb74_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+1280] -> [r12+1200]
 mov rax, qword ptr [r12 + 1280]
 mov qword ptr [r12 + 1200], rax
 mov rax, qword ptr [r12 + 1288]
 mov qword ptr [r12 + 1208], rax
# marshal arg1 = producer-box slot [r12+1296] -> [r12+1216]
 mov rax, qword ptr [r12 + 1296]
 mov qword ptr [r12 + 1216], rax
 mov rax, qword ptr [r12 + 1304]
 mov qword ptr [r12 + 1224], rax
  .section .rodata
  .Lbynamefn84: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn84]
 lea rsi, [r12 + 1200]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1184], rax
 mov qword ptr [r12 + 1192], rdx
 cmp eax, 99
 je xchain0_n54_α
 jmp xchain0_n78_α
xchain0_n73_β:
 jmp xchain0_n54_α
xchain0_n74_α:
# IR_LIT_STRING
bb75_α:
 mov qword ptr [r12 + 1456], 1
 mov rax, qword ptr [rip + .Lx85_0]
 mov qword ptr [r12 + 1464], rax
 jmp xchain0_n79_α
 xchain0_n74_β:
 jmp xchain0_n61_α
.Lx85_0:
 .quad .Lx85_0_s
.Lx85_0_s:
 .string "dog"
xchain0_n75_α:
# IR_LIT_STRING
bb76_α:
 mov qword ptr [r12 + 1552], 1
 mov rax, qword ptr [rip + .Lx86_0]
 mov qword ptr [r12 + 1560], rax
 jmp xchain0_n80_α
 xchain0_n75_β:
 jmp xchain0_n67_α
.Lx86_0:
 .quad .Lx86_0_s
.Lx86_0_s:
 .string "fish"
xchain0_n76_α:
# IR_SUBSCRIPT x[i] variable
bb77_α:
 mov rdi, qword ptr [r12 + 1632]
 mov rsi, qword ptr [r12 + 1640]
 mov rdx, qword ptr [r12 + 1648]
 mov rcx, qword ptr [r12 + 1656]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n72_α
 mov qword ptr [r12 + 1664], rax
 mov qword ptr [r12 + 1672], rdx
 jmp xchain0_n81_α
 xchain0_n76_β:
 jmp xchain0_n72_α
xchain0_n77_α:
# IR_ASSIGN gva
bb78_α:
 mov rax, qword ptr [r12 + 1760]
 mov rdx, qword ptr [r12 + 1768]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 1744], rax
 mov qword ptr [r12 + 1752], rdx
 jmp main_γ
 xchain0_n77_β:
 jmp main_γ
xchain0_n78_α:
# IR_LIT_STRING
bb79_α:
 mov qword ptr [r12 + 1328], 1
 mov rax, qword ptr [rip + .Lx89_0]
 mov qword ptr [r12 + 1336], rax
 jmp xchain0_n82_α
 xchain0_n78_β:
 jmp main_γ
.Lx89_0:
 .quad .Lx89_0_s
.Lx89_0_s:
 .string "FAIL 1113/006: array->table int key roundtrip"
xchain0_n79_α:
bb80_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+1440] -> [r12+1360]
 mov rax, qword ptr [r12 + 1440]
 mov qword ptr [r12 + 1360], rax
 mov rax, qword ptr [r12 + 1448]
 mov qword ptr [r12 + 1368], rax
# marshal arg1 = producer-box slot [r12+1456] -> [r12+1376]
 mov rax, qword ptr [r12 + 1456]
 mov qword ptr [r12 + 1376], rax
 mov rax, qword ptr [r12 + 1464]
 mov qword ptr [r12 + 1384], rax
  .section .rodata
  .Lbynamefn91: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn91]
 lea rsi, [r12 + 1360]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1344], rax
 mov qword ptr [r12 + 1352], rdx
 cmp eax, 99
 je xchain0_n61_α
 jmp xchain0_n83_α
xchain0_n79_β:
 jmp xchain0_n61_α
xchain0_n80_α:
# IR_ASSIGN_VAR write through variable
bb81_α:
 mov rdi, qword ptr [r12 + 1536]
 mov rsi, qword ptr [r12 + 1544]
 mov rdx, qword ptr [r12 + 1552]
 mov rcx, qword ptr [r12 + 1560]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n67_α
 mov qword ptr [r12 + 1568], rax
 mov qword ptr [r12 + 1576], rdx
 jmp xchain0_n67_α
 xchain0_n80_β:
 jmp xchain0_n67_α
xchain0_n81_α:
# IR_DEREF variable -> value
bb82_α:
 mov rdi, qword ptr [r12 + 1664]
 mov rsi, qword ptr [r12 + 1672]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n72_α
 mov qword ptr [r12 + 1680], rax
 mov qword ptr [r12 + 1688], rdx
 jmp xchain0_n84_α
 xchain0_n81_β:
 jmp xchain0_n72_α
xchain0_n82_α:
# IR_ASSIGN gva
bb83_α:
 mov rax, qword ptr [r12 + 1328]
 mov rdx, qword ptr [r12 + 1336]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 1312], rax
 mov qword ptr [r12 + 1320], rdx
 jmp main_γ
 xchain0_n82_β:
 jmp main_γ
xchain0_n83_α:
# IR_LIT_STRING
bb84_α:
 mov qword ptr [r12 + 1488], 1
 mov rax, qword ptr [rip + .Lx95_0]
 mov qword ptr [r12 + 1496], rax
 jmp xchain0_n85_α
 xchain0_n83_β:
 jmp main_γ
.Lx95_0:
 .quad .Lx95_0_s
.Lx95_0_s:
 .string "FAIL 1113/007: array->table string key roundtrip"
xchain0_n84_α:
# IR_LIT_STRING
bb85_α:
 mov qword ptr [r12 + 1696], 1
 mov rax, qword ptr [rip + .Lx96_0]
 mov qword ptr [r12 + 1704], rax
 jmp xchain0_n86_α
 xchain0_n84_β:
 jmp xchain0_n72_α
.Lx96_0:
 .quad .Lx96_0_s
.Lx96_0_s:
 .string "fish"
xchain0_n85_α:
# IR_ASSIGN gva
bb86_α:
 mov rax, qword ptr [r12 + 1488]
 mov rdx, qword ptr [r12 + 1496]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 1472], rax
 mov qword ptr [r12 + 1480], rdx
 jmp main_γ
 xchain0_n85_β:
 jmp main_γ
xchain0_n86_α:
bb87_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+1680] -> [r12+1600]
 mov rax, qword ptr [r12 + 1680]
 mov qword ptr [r12 + 1600], rax
 mov rax, qword ptr [r12 + 1688]
 mov qword ptr [r12 + 1608], rax
# marshal arg1 = producer-box slot [r12+1696] -> [r12+1616]
 mov rax, qword ptr [r12 + 1696]
 mov qword ptr [r12 + 1616], rax
 mov rax, qword ptr [r12 + 1704]
 mov qword ptr [r12 + 1624], rax
  .section .rodata
  .Lbynamefn99: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn99]
 lea rsi, [r12 + 1600]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1584], rax
 mov qword ptr [r12 + 1592], rdx
 cmp eax, 99
 je xchain0_n72_α
 jmp xchain0_n87_α
xchain0_n86_β:
 jmp xchain0_n72_α
xchain0_n87_α:
# IR_LIT_STRING
bb88_α:
 mov qword ptr [r12 + 1728], 1
 mov rax, qword ptr [rip + .Lx100_0]
 mov qword ptr [r12 + 1736], rax
 jmp xchain0_n88_α
 xchain0_n87_β:
 jmp main_γ
.Lx100_0:
 .quad .Lx100_0_s
.Lx100_0_s:
 .string "FAIL 1113/008: [] and <> syntax equivalent"
xchain0_n88_α:
# IR_ASSIGN gva
bb89_α:
 mov rax, qword ptr [r12 + 1728]
 mov rdx, qword ptr [r12 + 1736]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 1712], rax
 mov qword ptr [r12 + 1720], rdx
 jmp main_γ
 xchain0_n88_β:
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
 mov rdi, qword ptr [r12 + 1776]
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
