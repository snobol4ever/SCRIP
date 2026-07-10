  .intel_syntax noprefix
  .text
  .globl proc_roman_α
proc_roman_α:
#=======================================================================================================================
    .global proc_roman_α
    .global proc_roman_β
    .global proc_roman_γ
    .global proc_roman_ω
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
 mov qword ptr [r12 + 3248], rax
 pop rsi
proc_roman_α_body:
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [r12 + 80], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 88], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n2_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string ""
# IR_ASSIGN gva
 xchain0_n1_α:
 mov rax, qword ptr [r12 + 80]
 mov rdx, qword ptr [r12 + 88]
 mov qword ptr [rbx + 32], rax
 mov qword ptr [rbx + 40], rdx
 mov qword ptr [r12 + 64], rax
 mov qword ptr [r12 + 72], rdx
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n2_α
# IR_LIT_INTEGER
 xchain0_n2_α:
 mov qword ptr [r12 + 144], 6
 mov rax, qword ptr [rip + .Lx3_0]
 mov qword ptr [r12 + 152], rax
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n5_α
.Lx3_0:
 .quad 13
 xchain0_n3_α:
# BOX IR_CALL ARRAY(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+144] -> [zr+128]
 mov rax, qword ptr [r12 + 144]
 mov qword ptr [r12 + 128], rax
 mov rax, qword ptr [r12 + 152]
 mov qword ptr [r12 + 136], rax
  .section .rodata
  .Lrkfn5: .string "ARRAY"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn5]
 lea rsi, [r12 + 128]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 112], rax
 mov qword ptr [r12 + 120], rdx
 cmp eax, 99
 je xchain0_n5_α
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp xchain0_n5_α
# IR_ASSIGN gva
 xchain0_n4_α:
 mov rax, qword ptr [r12 + 112]
 mov rdx, qword ptr [r12 + 120]
 mov qword ptr [rbx + 48], rax
 mov qword ptr [rbx + 56], rdx
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 jmp xchain0_n5_α
 xchain0_n4_β:
 jmp xchain0_n5_α
# IR_VAR
 xchain0_n5_α:
 mov rax, qword ptr [rbx + 48]
 mov rdx, qword ptr [rbx + 56]
 mov qword ptr [r12 + 160], rax
 mov qword ptr [r12 + 168], rdx
 jmp xchain0_n6_α
 xchain0_n5_β:
 jmp xchain0_n7_α
# IR_LIT_INTEGER
 xchain0_n6_α:
 mov qword ptr [r12 + 176], 6
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [r12 + 184], rax
 jmp xchain0_n8_α
 xchain0_n6_β:
 jmp xchain0_n7_α
.Lx8_0:
 .quad 1
# IR_VAR
 xchain0_n7_α:
 mov rax, qword ptr [rbx + 48]
 mov rdx, qword ptr [rbx + 56]
 mov qword ptr [r12 + 240], rax
 mov qword ptr [r12 + 248], rdx
 jmp xchain0_n9_α
 xchain0_n7_β:
 jmp xchain0_n10_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n8_α:
 mov rdi, qword ptr [r12 + 160]
 mov rsi, qword ptr [r12 + 168]
 mov rdx, qword ptr [r12 + 176]
 mov rcx, qword ptr [r12 + 184]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n7_α
 mov qword ptr [r12 + 192], rax
 mov qword ptr [r12 + 200], rdx
 jmp xchain0_n11_α
 xchain0_n8_β:
 jmp xchain0_n7_α
# IR_LIT_INTEGER
 xchain0_n9_α:
 mov qword ptr [r12 + 256], 6
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [r12 + 264], rax
 jmp xchain0_n12_α
 xchain0_n9_β:
 jmp xchain0_n10_α
.Lx11_0:
 .quad 2
# IR_VAR
 xchain0_n10_α:
 mov rax, qword ptr [rbx + 48]
 mov rdx, qword ptr [rbx + 56]
 mov qword ptr [r12 + 320], rax
 mov qword ptr [r12 + 328], rdx
 jmp xchain0_n13_α
 xchain0_n10_β:
 jmp xchain0_n14_α
# IR_LIT_INTEGER
 xchain0_n11_α:
 mov qword ptr [r12 + 208], 6
 mov rax, qword ptr [rip + .Lx13_0]
 mov qword ptr [r12 + 216], rax
 jmp xchain0_n15_α
 xchain0_n11_β:
 jmp xchain0_n7_α
.Lx13_0:
 .quad 1000
# IR_SUBSCRIPT x[i] variable
 xchain0_n12_α:
 mov rdi, qword ptr [r12 + 240]
 mov rsi, qword ptr [r12 + 248]
 mov rdx, qword ptr [r12 + 256]
 mov rcx, qword ptr [r12 + 264]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n10_α
 mov qword ptr [r12 + 272], rax
 mov qword ptr [r12 + 280], rdx
 jmp xchain0_n16_α
 xchain0_n12_β:
 jmp xchain0_n10_α
# IR_LIT_INTEGER
 xchain0_n13_α:
 mov qword ptr [r12 + 336], 6
 mov rax, qword ptr [rip + .Lx15_0]
 mov qword ptr [r12 + 344], rax
 jmp xchain0_n17_α
 xchain0_n13_β:
 jmp xchain0_n14_α
.Lx15_0:
 .quad 3
# IR_VAR
 xchain0_n14_α:
 mov rax, qword ptr [rbx + 48]
 mov rdx, qword ptr [rbx + 56]
 mov qword ptr [r12 + 400], rax
 mov qword ptr [r12 + 408], rdx
 jmp xchain0_n18_α
 xchain0_n14_β:
 jmp xchain0_n19_α
# IR_ASSIGN_VAR
 xchain0_n15_α:
 mov rdi, qword ptr [r12 + 192]
 mov rsi, qword ptr [r12 + 200]
 mov rdx, qword ptr [r12 + 208]
 mov rcx, qword ptr [r12 + 216]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n7_α
 mov qword ptr [r12 + 224], rax
 mov qword ptr [r12 + 232], rdx
 jmp xchain0_n7_α
 xchain0_n15_β:
 jmp xchain0_n7_α
# IR_LIT_INTEGER
 xchain0_n16_α:
 mov qword ptr [r12 + 288], 6
 mov rax, qword ptr [rip + .Lx18_0]
 mov qword ptr [r12 + 296], rax
 jmp xchain0_n20_α
 xchain0_n16_β:
 jmp xchain0_n10_α
.Lx18_0:
 .quad 900
# IR_SUBSCRIPT x[i] variable
 xchain0_n17_α:
 mov rdi, qword ptr [r12 + 320]
 mov rsi, qword ptr [r12 + 328]
 mov rdx, qword ptr [r12 + 336]
 mov rcx, qword ptr [r12 + 344]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n14_α
 mov qword ptr [r12 + 352], rax
 mov qword ptr [r12 + 360], rdx
 jmp xchain0_n21_α
 xchain0_n17_β:
 jmp xchain0_n14_α
# IR_LIT_INTEGER
 xchain0_n18_α:
 mov qword ptr [r12 + 416], 6
 mov rax, qword ptr [rip + .Lx20_0]
 mov qword ptr [r12 + 424], rax
 jmp xchain0_n22_α
 xchain0_n18_β:
 jmp xchain0_n19_α
.Lx20_0:
 .quad 4
# IR_VAR
 xchain0_n19_α:
 mov rax, qword ptr [rbx + 48]
 mov rdx, qword ptr [rbx + 56]
 mov qword ptr [r12 + 480], rax
 mov qword ptr [r12 + 488], rdx
 jmp xchain0_n23_α
 xchain0_n19_β:
 jmp xchain0_n24_α
# IR_ASSIGN_VAR
 xchain0_n20_α:
 mov rdi, qword ptr [r12 + 272]
 mov rsi, qword ptr [r12 + 280]
 mov rdx, qword ptr [r12 + 288]
 mov rcx, qword ptr [r12 + 296]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n10_α
 mov qword ptr [r12 + 304], rax
 mov qword ptr [r12 + 312], rdx
 jmp xchain0_n10_α
 xchain0_n20_β:
 jmp xchain0_n10_α
# IR_LIT_INTEGER
 xchain0_n21_α:
 mov qword ptr [r12 + 368], 6
 mov rax, qword ptr [rip + .Lx23_0]
 mov qword ptr [r12 + 376], rax
 jmp xchain0_n25_α
 xchain0_n21_β:
 jmp xchain0_n14_α
.Lx23_0:
 .quad 500
# IR_SUBSCRIPT x[i] variable
 xchain0_n22_α:
 mov rdi, qword ptr [r12 + 400]
 mov rsi, qword ptr [r12 + 408]
 mov rdx, qword ptr [r12 + 416]
 mov rcx, qword ptr [r12 + 424]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n19_α
 mov qword ptr [r12 + 432], rax
 mov qword ptr [r12 + 440], rdx
 jmp xchain0_n26_α
 xchain0_n22_β:
 jmp xchain0_n19_α
# IR_LIT_INTEGER
 xchain0_n23_α:
 mov qword ptr [r12 + 496], 6
 mov rax, qword ptr [rip + .Lx25_0]
 mov qword ptr [r12 + 504], rax
 jmp xchain0_n27_α
 xchain0_n23_β:
 jmp xchain0_n24_α
.Lx25_0:
 .quad 5
# IR_VAR
 xchain0_n24_α:
 mov rax, qword ptr [rbx + 48]
 mov rdx, qword ptr [rbx + 56]
 mov qword ptr [r12 + 560], rax
 mov qword ptr [r12 + 568], rdx
 jmp xchain0_n28_α
 xchain0_n24_β:
 jmp xchain0_n29_α
# IR_ASSIGN_VAR
 xchain0_n25_α:
 mov rdi, qword ptr [r12 + 352]
 mov rsi, qword ptr [r12 + 360]
 mov rdx, qword ptr [r12 + 368]
 mov rcx, qword ptr [r12 + 376]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n14_α
 mov qword ptr [r12 + 384], rax
 mov qword ptr [r12 + 392], rdx
 jmp xchain0_n14_α
 xchain0_n25_β:
 jmp xchain0_n14_α
# IR_LIT_INTEGER
 xchain0_n26_α:
 mov qword ptr [r12 + 448], 6
 mov rax, qword ptr [rip + .Lx28_0]
 mov qword ptr [r12 + 456], rax
 jmp xchain0_n30_α
 xchain0_n26_β:
 jmp xchain0_n19_α
.Lx28_0:
 .quad 400
# IR_SUBSCRIPT x[i] variable
 xchain0_n27_α:
 mov rdi, qword ptr [r12 + 480]
 mov rsi, qword ptr [r12 + 488]
 mov rdx, qword ptr [r12 + 496]
 mov rcx, qword ptr [r12 + 504]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n24_α
 mov qword ptr [r12 + 512], rax
 mov qword ptr [r12 + 520], rdx
 jmp xchain0_n31_α
 xchain0_n27_β:
 jmp xchain0_n24_α
# IR_LIT_INTEGER
 xchain0_n28_α:
 mov qword ptr [r12 + 576], 6
 mov rax, qword ptr [rip + .Lx30_0]
 mov qword ptr [r12 + 584], rax
 jmp xchain0_n32_α
 xchain0_n28_β:
 jmp xchain0_n29_α
.Lx30_0:
 .quad 6
# IR_VAR
 xchain0_n29_α:
 mov rax, qword ptr [rbx + 48]
 mov rdx, qword ptr [rbx + 56]
 mov qword ptr [r12 + 640], rax
 mov qword ptr [r12 + 648], rdx
 jmp xchain0_n33_α
 xchain0_n29_β:
 jmp xchain0_n34_α
# IR_ASSIGN_VAR
 xchain0_n30_α:
 mov rdi, qword ptr [r12 + 432]
 mov rsi, qword ptr [r12 + 440]
 mov rdx, qword ptr [r12 + 448]
 mov rcx, qword ptr [r12 + 456]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n19_α
 mov qword ptr [r12 + 464], rax
 mov qword ptr [r12 + 472], rdx
 jmp xchain0_n19_α
 xchain0_n30_β:
 jmp xchain0_n19_α
# IR_LIT_INTEGER
 xchain0_n31_α:
 mov qword ptr [r12 + 528], 6
 mov rax, qword ptr [rip + .Lx33_0]
 mov qword ptr [r12 + 536], rax
 jmp xchain0_n35_α
 xchain0_n31_β:
 jmp xchain0_n24_α
.Lx33_0:
 .quad 100
# IR_SUBSCRIPT x[i] variable
 xchain0_n32_α:
 mov rdi, qword ptr [r12 + 560]
 mov rsi, qword ptr [r12 + 568]
 mov rdx, qword ptr [r12 + 576]
 mov rcx, qword ptr [r12 + 584]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n29_α
 mov qword ptr [r12 + 592], rax
 mov qword ptr [r12 + 600], rdx
 jmp xchain0_n36_α
 xchain0_n32_β:
 jmp xchain0_n29_α
# IR_LIT_INTEGER
 xchain0_n33_α:
 mov qword ptr [r12 + 656], 6
 mov rax, qword ptr [rip + .Lx35_0]
 mov qword ptr [r12 + 664], rax
 jmp xchain0_n37_α
 xchain0_n33_β:
 jmp xchain0_n34_α
.Lx35_0:
 .quad 7
# IR_VAR
 xchain0_n34_α:
 mov rax, qword ptr [rbx + 48]
 mov rdx, qword ptr [rbx + 56]
 mov qword ptr [r12 + 720], rax
 mov qword ptr [r12 + 728], rdx
 jmp xchain0_n38_α
 xchain0_n34_β:
 jmp xchain0_n39_α
# IR_ASSIGN_VAR
 xchain0_n35_α:
 mov rdi, qword ptr [r12 + 512]
 mov rsi, qword ptr [r12 + 520]
 mov rdx, qword ptr [r12 + 528]
 mov rcx, qword ptr [r12 + 536]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n24_α
 mov qword ptr [r12 + 544], rax
 mov qword ptr [r12 + 552], rdx
 jmp xchain0_n24_α
 xchain0_n35_β:
 jmp xchain0_n24_α
# IR_LIT_INTEGER
 xchain0_n36_α:
 mov qword ptr [r12 + 608], 6
 mov rax, qword ptr [rip + .Lx38_0]
 mov qword ptr [r12 + 616], rax
 jmp xchain0_n40_α
 xchain0_n36_β:
 jmp xchain0_n29_α
.Lx38_0:
 .quad 90
# IR_SUBSCRIPT x[i] variable
 xchain0_n37_α:
 mov rdi, qword ptr [r12 + 640]
 mov rsi, qword ptr [r12 + 648]
 mov rdx, qword ptr [r12 + 656]
 mov rcx, qword ptr [r12 + 664]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n34_α
 mov qword ptr [r12 + 672], rax
 mov qword ptr [r12 + 680], rdx
 jmp xchain0_n41_α
 xchain0_n37_β:
 jmp xchain0_n34_α
# IR_LIT_INTEGER
 xchain0_n38_α:
 mov qword ptr [r12 + 736], 6
 mov rax, qword ptr [rip + .Lx40_0]
 mov qword ptr [r12 + 744], rax
 jmp xchain0_n42_α
 xchain0_n38_β:
 jmp xchain0_n39_α
.Lx40_0:
 .quad 8
# IR_VAR
 xchain0_n39_α:
 mov rax, qword ptr [rbx + 48]
 mov rdx, qword ptr [rbx + 56]
 mov qword ptr [r12 + 800], rax
 mov qword ptr [r12 + 808], rdx
 jmp xchain0_n43_α
 xchain0_n39_β:
 jmp xchain0_n44_α
# IR_ASSIGN_VAR
 xchain0_n40_α:
 mov rdi, qword ptr [r12 + 592]
 mov rsi, qword ptr [r12 + 600]
 mov rdx, qword ptr [r12 + 608]
 mov rcx, qword ptr [r12 + 616]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n29_α
 mov qword ptr [r12 + 624], rax
 mov qword ptr [r12 + 632], rdx
 jmp xchain0_n29_α
 xchain0_n40_β:
 jmp xchain0_n29_α
# IR_LIT_INTEGER
 xchain0_n41_α:
 mov qword ptr [r12 + 688], 6
 mov rax, qword ptr [rip + .Lx43_0]
 mov qword ptr [r12 + 696], rax
 jmp xchain0_n45_α
 xchain0_n41_β:
 jmp xchain0_n34_α
.Lx43_0:
 .quad 50
# IR_SUBSCRIPT x[i] variable
 xchain0_n42_α:
 mov rdi, qword ptr [r12 + 720]
 mov rsi, qword ptr [r12 + 728]
 mov rdx, qword ptr [r12 + 736]
 mov rcx, qword ptr [r12 + 744]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n39_α
 mov qword ptr [r12 + 752], rax
 mov qword ptr [r12 + 760], rdx
 jmp xchain0_n46_α
 xchain0_n42_β:
 jmp xchain0_n39_α
# IR_LIT_INTEGER
 xchain0_n43_α:
 mov qword ptr [r12 + 816], 6
 mov rax, qword ptr [rip + .Lx45_0]
 mov qword ptr [r12 + 824], rax
 jmp xchain0_n47_α
 xchain0_n43_β:
 jmp xchain0_n44_α
.Lx45_0:
 .quad 9
# IR_VAR
 xchain0_n44_α:
 mov rax, qword ptr [rbx + 48]
 mov rdx, qword ptr [rbx + 56]
 mov qword ptr [r12 + 880], rax
 mov qword ptr [r12 + 888], rdx
 jmp xchain0_n48_α
 xchain0_n44_β:
 jmp xchain0_n49_α
# IR_ASSIGN_VAR
 xchain0_n45_α:
 mov rdi, qword ptr [r12 + 672]
 mov rsi, qword ptr [r12 + 680]
 mov rdx, qword ptr [r12 + 688]
 mov rcx, qword ptr [r12 + 696]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n34_α
 mov qword ptr [r12 + 704], rax
 mov qword ptr [r12 + 712], rdx
 jmp xchain0_n34_α
 xchain0_n45_β:
 jmp xchain0_n34_α
# IR_LIT_INTEGER
 xchain0_n46_α:
 mov qword ptr [r12 + 768], 6
 mov rax, qword ptr [rip + .Lx48_0]
 mov qword ptr [r12 + 776], rax
 jmp xchain0_n50_α
 xchain0_n46_β:
 jmp xchain0_n39_α
.Lx48_0:
 .quad 40
# IR_SUBSCRIPT x[i] variable
 xchain0_n47_α:
 mov rdi, qword ptr [r12 + 800]
 mov rsi, qword ptr [r12 + 808]
 mov rdx, qword ptr [r12 + 816]
 mov rcx, qword ptr [r12 + 824]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n44_α
 mov qword ptr [r12 + 832], rax
 mov qword ptr [r12 + 840], rdx
 jmp xchain0_n51_α
 xchain0_n47_β:
 jmp xchain0_n44_α
# IR_LIT_INTEGER
 xchain0_n48_α:
 mov qword ptr [r12 + 896], 6
 mov rax, qword ptr [rip + .Lx50_0]
 mov qword ptr [r12 + 904], rax
 jmp xchain0_n52_α
 xchain0_n48_β:
 jmp xchain0_n49_α
.Lx50_0:
 .quad 10
# IR_VAR
 xchain0_n49_α:
 mov rax, qword ptr [rbx + 48]
 mov rdx, qword ptr [rbx + 56]
 mov qword ptr [r12 + 960], rax
 mov qword ptr [r12 + 968], rdx
 jmp xchain0_n53_α
 xchain0_n49_β:
 jmp xchain0_n54_α
# IR_ASSIGN_VAR
 xchain0_n50_α:
 mov rdi, qword ptr [r12 + 752]
 mov rsi, qword ptr [r12 + 760]
 mov rdx, qword ptr [r12 + 768]
 mov rcx, qword ptr [r12 + 776]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n39_α
 mov qword ptr [r12 + 784], rax
 mov qword ptr [r12 + 792], rdx
 jmp xchain0_n39_α
 xchain0_n50_β:
 jmp xchain0_n39_α
# IR_LIT_INTEGER
 xchain0_n51_α:
 mov qword ptr [r12 + 848], 6
 mov rax, qword ptr [rip + .Lx53_0]
 mov qword ptr [r12 + 856], rax
 jmp xchain0_n55_α
 xchain0_n51_β:
 jmp xchain0_n44_α
.Lx53_0:
 .quad 10
# IR_SUBSCRIPT x[i] variable
 xchain0_n52_α:
 mov rdi, qword ptr [r12 + 880]
 mov rsi, qword ptr [r12 + 888]
 mov rdx, qword ptr [r12 + 896]
 mov rcx, qword ptr [r12 + 904]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n49_α
 mov qword ptr [r12 + 912], rax
 mov qword ptr [r12 + 920], rdx
 jmp xchain0_n56_α
 xchain0_n52_β:
 jmp xchain0_n49_α
# IR_LIT_INTEGER
 xchain0_n53_α:
 mov qword ptr [r12 + 976], 6
 mov rax, qword ptr [rip + .Lx55_0]
 mov qword ptr [r12 + 984], rax
 jmp xchain0_n57_α
 xchain0_n53_β:
 jmp xchain0_n54_α
.Lx55_0:
 .quad 11
# IR_VAR
 xchain0_n54_α:
 mov rax, qword ptr [rbx + 48]
 mov rdx, qword ptr [rbx + 56]
 mov qword ptr [r12 + 1040], rax
 mov qword ptr [r12 + 1048], rdx
 jmp xchain0_n58_α
 xchain0_n54_β:
 jmp xchain0_n59_α
# IR_ASSIGN_VAR
 xchain0_n55_α:
 mov rdi, qword ptr [r12 + 832]
 mov rsi, qword ptr [r12 + 840]
 mov rdx, qword ptr [r12 + 848]
 mov rcx, qword ptr [r12 + 856]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n44_α
 mov qword ptr [r12 + 864], rax
 mov qword ptr [r12 + 872], rdx
 jmp xchain0_n44_α
 xchain0_n55_β:
 jmp xchain0_n44_α
# IR_LIT_INTEGER
 xchain0_n56_α:
 mov qword ptr [r12 + 928], 6
 mov rax, qword ptr [rip + .Lx58_0]
 mov qword ptr [r12 + 936], rax
 jmp xchain0_n60_α
 xchain0_n56_β:
 jmp xchain0_n49_α
.Lx58_0:
 .quad 9
# IR_SUBSCRIPT x[i] variable
 xchain0_n57_α:
 mov rdi, qword ptr [r12 + 960]
 mov rsi, qword ptr [r12 + 968]
 mov rdx, qword ptr [r12 + 976]
 mov rcx, qword ptr [r12 + 984]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n54_α
 mov qword ptr [r12 + 992], rax
 mov qword ptr [r12 + 1000], rdx
 jmp xchain0_n61_α
 xchain0_n57_β:
 jmp xchain0_n54_α
# IR_LIT_INTEGER
 xchain0_n58_α:
 mov qword ptr [r12 + 1056], 6
 mov rax, qword ptr [rip + .Lx60_0]
 mov qword ptr [r12 + 1064], rax
 jmp xchain0_n62_α
 xchain0_n58_β:
 jmp xchain0_n59_α
.Lx60_0:
 .quad 12
# IR_VAR
 xchain0_n59_α:
 mov rax, qword ptr [rbx + 48]
 mov rdx, qword ptr [rbx + 56]
 mov qword ptr [r12 + 1120], rax
 mov qword ptr [r12 + 1128], rdx
 jmp xchain0_n63_α
 xchain0_n59_β:
 jmp xchain0_n64_α
# IR_ASSIGN_VAR
 xchain0_n60_α:
 mov rdi, qword ptr [r12 + 912]
 mov rsi, qword ptr [r12 + 920]
 mov rdx, qword ptr [r12 + 928]
 mov rcx, qword ptr [r12 + 936]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n49_α
 mov qword ptr [r12 + 944], rax
 mov qword ptr [r12 + 952], rdx
 jmp xchain0_n49_α
 xchain0_n60_β:
 jmp xchain0_n49_α
# IR_LIT_INTEGER
 xchain0_n61_α:
 mov qword ptr [r12 + 1008], 6
 mov rax, qword ptr [rip + .Lx63_0]
 mov qword ptr [r12 + 1016], rax
 jmp xchain0_n65_α
 xchain0_n61_β:
 jmp xchain0_n54_α
.Lx63_0:
 .quad 5
# IR_SUBSCRIPT x[i] variable
 xchain0_n62_α:
 mov rdi, qword ptr [r12 + 1040]
 mov rsi, qword ptr [r12 + 1048]
 mov rdx, qword ptr [r12 + 1056]
 mov rcx, qword ptr [r12 + 1064]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n59_α
 mov qword ptr [r12 + 1072], rax
 mov qword ptr [r12 + 1080], rdx
 jmp xchain0_n66_α
 xchain0_n62_β:
 jmp xchain0_n59_α
# IR_LIT_INTEGER
 xchain0_n63_α:
 mov qword ptr [r12 + 1136], 6
 mov rax, qword ptr [rip + .Lx65_0]
 mov qword ptr [r12 + 1144], rax
 jmp xchain0_n67_α
 xchain0_n63_β:
 jmp xchain0_n64_α
.Lx65_0:
 .quad 13
# IR_LIT_INTEGER
 xchain0_n64_α:
 mov qword ptr [r12 + 1248], 6
 mov rax, qword ptr [rip + .Lx66_0]
 mov qword ptr [r12 + 1256], rax
 jmp xchain0_n68_α
 xchain0_n64_β:
 jmp xchain0_n72_α
.Lx66_0:
 .quad 13
# IR_ASSIGN_VAR
 xchain0_n65_α:
 mov rdi, qword ptr [r12 + 992]
 mov rsi, qword ptr [r12 + 1000]
 mov rdx, qword ptr [r12 + 1008]
 mov rcx, qword ptr [r12 + 1016]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n54_α
 mov qword ptr [r12 + 1024], rax
 mov qword ptr [r12 + 1032], rdx
 jmp xchain0_n54_α
 xchain0_n65_β:
 jmp xchain0_n54_α
# IR_LIT_INTEGER
 xchain0_n66_α:
 mov qword ptr [r12 + 1088], 6
 mov rax, qword ptr [rip + .Lx68_0]
 mov qword ptr [r12 + 1096], rax
 jmp xchain0_n69_α
 xchain0_n66_β:
 jmp xchain0_n59_α
.Lx68_0:
 .quad 4
# IR_SUBSCRIPT x[i] variable
 xchain0_n67_α:
 mov rdi, qword ptr [r12 + 1120]
 mov rsi, qword ptr [r12 + 1128]
 mov rdx, qword ptr [r12 + 1136]
 mov rcx, qword ptr [r12 + 1144]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n64_α
 mov qword ptr [r12 + 1152], rax
 mov qword ptr [r12 + 1160], rdx
 jmp xchain0_n70_α
 xchain0_n67_β:
 jmp xchain0_n64_α
 xchain0_n68_α:
# BOX IR_CALL ARRAY(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+1248] -> [zr+1232]
 mov rax, qword ptr [r12 + 1248]
 mov qword ptr [r12 + 1232], rax
 mov rax, qword ptr [r12 + 1256]
 mov qword ptr [r12 + 1240], rax
  .section .rodata
  .Lrkfn71: .string "ARRAY"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn71]
 lea rsi, [r12 + 1232]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1216], rax
 mov qword ptr [r12 + 1224], rdx
 cmp eax, 99
 je xchain0_n72_α
 jmp xchain0_n71_α
 xchain0_n68_β:
 jmp xchain0_n72_α
# IR_ASSIGN_VAR
 xchain0_n69_α:
 mov rdi, qword ptr [r12 + 1072]
 mov rsi, qword ptr [r12 + 1080]
 mov rdx, qword ptr [r12 + 1088]
 mov rcx, qword ptr [r12 + 1096]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n59_α
 mov qword ptr [r12 + 1104], rax
 mov qword ptr [r12 + 1112], rdx
 jmp xchain0_n59_α
 xchain0_n69_β:
 jmp xchain0_n59_α
# IR_LIT_INTEGER
 xchain0_n70_α:
 mov qword ptr [r12 + 1168], 6
 mov rax, qword ptr [rip + .Lx73_0]
 mov qword ptr [r12 + 1176], rax
 jmp xchain0_n73_α
 xchain0_n70_β:
 jmp xchain0_n64_α
.Lx73_0:
 .quad 1
# IR_ASSIGN gva
 xchain0_n71_α:
 mov rax, qword ptr [r12 + 1216]
 mov rdx, qword ptr [r12 + 1224]
 mov qword ptr [rbx + 64], rax
 mov qword ptr [rbx + 72], rdx
 mov qword ptr [r12 + 1200], rax
 mov qword ptr [r12 + 1208], rdx
 jmp xchain0_n72_α
 xchain0_n71_β:
 jmp xchain0_n72_α
# IR_VAR
 xchain0_n72_α:
 mov rax, qword ptr [rbx + 64]
 mov rdx, qword ptr [rbx + 72]
 mov qword ptr [r12 + 1264], rax
 mov qword ptr [r12 + 1272], rdx
 jmp xchain0_n74_α
 xchain0_n72_β:
 jmp xchain0_n75_α
# IR_ASSIGN_VAR
 xchain0_n73_α:
 mov rdi, qword ptr [r12 + 1152]
 mov rsi, qword ptr [r12 + 1160]
 mov rdx, qword ptr [r12 + 1168]
 mov rcx, qword ptr [r12 + 1176]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n64_α
 mov qword ptr [r12 + 1184], rax
 mov qword ptr [r12 + 1192], rdx
 jmp xchain0_n64_α
 xchain0_n73_β:
 jmp xchain0_n64_α
# IR_LIT_INTEGER
 xchain0_n74_α:
 mov qword ptr [r12 + 1280], 6
 mov rax, qword ptr [rip + .Lx77_0]
 mov qword ptr [r12 + 1288], rax
 jmp xchain0_n76_α
 xchain0_n74_β:
 jmp xchain0_n75_α
.Lx77_0:
 .quad 1
# IR_VAR
 xchain0_n75_α:
 mov rax, qword ptr [rbx + 64]
 mov rdx, qword ptr [rbx + 72]
 mov qword ptr [r12 + 1344], rax
 mov qword ptr [r12 + 1352], rdx
 jmp xchain0_n77_α
 xchain0_n75_β:
 jmp xchain0_n78_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n76_α:
 mov rdi, qword ptr [r12 + 1264]
 mov rsi, qword ptr [r12 + 1272]
 mov rdx, qword ptr [r12 + 1280]
 mov rcx, qword ptr [r12 + 1288]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n75_α
 mov qword ptr [r12 + 1296], rax
 mov qword ptr [r12 + 1304], rdx
 jmp xchain0_n79_α
 xchain0_n76_β:
 jmp xchain0_n75_α
# IR_LIT_INTEGER
 xchain0_n77_α:
 mov qword ptr [r12 + 1360], 6
 mov rax, qword ptr [rip + .Lx80_0]
 mov qword ptr [r12 + 1368], rax
 jmp xchain0_n80_α
 xchain0_n77_β:
 jmp xchain0_n78_α
.Lx80_0:
 .quad 2
# IR_VAR
 xchain0_n78_α:
 mov rax, qword ptr [rbx + 64]
 mov rdx, qword ptr [rbx + 72]
 mov qword ptr [r12 + 1424], rax
 mov qword ptr [r12 + 1432], rdx
 jmp xchain0_n81_α
 xchain0_n78_β:
 jmp xchain0_n82_α
# IR_LIT_STRING
 xchain0_n79_α:
 mov qword ptr [r12 + 1312], 1
 mov rax, qword ptr [rip + .Lx82_0]
 mov qword ptr [r12 + 1320], rax
 jmp xchain0_n83_α
 xchain0_n79_β:
 jmp xchain0_n75_α
.Lx82_0:
 .quad .Lx82_0_s
.Lx82_0_s:
 .string "M"
# IR_SUBSCRIPT x[i] variable
 xchain0_n80_α:
 mov rdi, qword ptr [r12 + 1344]
 mov rsi, qword ptr [r12 + 1352]
 mov rdx, qword ptr [r12 + 1360]
 mov rcx, qword ptr [r12 + 1368]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n78_α
 mov qword ptr [r12 + 1376], rax
 mov qword ptr [r12 + 1384], rdx
 jmp xchain0_n84_α
 xchain0_n80_β:
 jmp xchain0_n78_α
# IR_LIT_INTEGER
 xchain0_n81_α:
 mov qword ptr [r12 + 1440], 6
 mov rax, qword ptr [rip + .Lx84_0]
 mov qword ptr [r12 + 1448], rax
 jmp xchain0_n85_α
 xchain0_n81_β:
 jmp xchain0_n82_α
.Lx84_0:
 .quad 3
# IR_VAR
 xchain0_n82_α:
 mov rax, qword ptr [rbx + 64]
 mov rdx, qword ptr [rbx + 72]
 mov qword ptr [r12 + 1504], rax
 mov qword ptr [r12 + 1512], rdx
 jmp xchain0_n86_α
 xchain0_n82_β:
 jmp xchain0_n87_α
# IR_ASSIGN_VAR
 xchain0_n83_α:
 mov rdi, qword ptr [r12 + 1296]
 mov rsi, qword ptr [r12 + 1304]
 mov rdx, qword ptr [r12 + 1312]
 mov rcx, qword ptr [r12 + 1320]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n75_α
 mov qword ptr [r12 + 1328], rax
 mov qword ptr [r12 + 1336], rdx
 jmp xchain0_n75_α
 xchain0_n83_β:
 jmp xchain0_n75_α
# IR_LIT_STRING
 xchain0_n84_α:
 mov qword ptr [r12 + 1392], 1
 mov rax, qword ptr [rip + .Lx87_0]
 mov qword ptr [r12 + 1400], rax
 jmp xchain0_n88_α
 xchain0_n84_β:
 jmp xchain0_n78_α
.Lx87_0:
 .quad .Lx87_0_s
.Lx87_0_s:
 .string "CM"
# IR_SUBSCRIPT x[i] variable
 xchain0_n85_α:
 mov rdi, qword ptr [r12 + 1424]
 mov rsi, qword ptr [r12 + 1432]
 mov rdx, qword ptr [r12 + 1440]
 mov rcx, qword ptr [r12 + 1448]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n82_α
 mov qword ptr [r12 + 1456], rax
 mov qword ptr [r12 + 1464], rdx
 jmp xchain0_n89_α
 xchain0_n85_β:
 jmp xchain0_n82_α
# IR_LIT_INTEGER
 xchain0_n86_α:
 mov qword ptr [r12 + 1520], 6
 mov rax, qword ptr [rip + .Lx89_0]
 mov qword ptr [r12 + 1528], rax
 jmp xchain0_n90_α
 xchain0_n86_β:
 jmp xchain0_n87_α
.Lx89_0:
 .quad 4
# IR_VAR
 xchain0_n87_α:
 mov rax, qword ptr [rbx + 64]
 mov rdx, qword ptr [rbx + 72]
 mov qword ptr [r12 + 1584], rax
 mov qword ptr [r12 + 1592], rdx
 jmp xchain0_n91_α
 xchain0_n87_β:
 jmp xchain0_n92_α
# IR_ASSIGN_VAR
 xchain0_n88_α:
 mov rdi, qword ptr [r12 + 1376]
 mov rsi, qword ptr [r12 + 1384]
 mov rdx, qword ptr [r12 + 1392]
 mov rcx, qword ptr [r12 + 1400]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n78_α
 mov qword ptr [r12 + 1408], rax
 mov qword ptr [r12 + 1416], rdx
 jmp xchain0_n78_α
 xchain0_n88_β:
 jmp xchain0_n78_α
# IR_LIT_STRING
 xchain0_n89_α:
 mov qword ptr [r12 + 1472], 1
 mov rax, qword ptr [rip + .Lx92_0]
 mov qword ptr [r12 + 1480], rax
 jmp xchain0_n93_α
 xchain0_n89_β:
 jmp xchain0_n82_α
.Lx92_0:
 .quad .Lx92_0_s
.Lx92_0_s:
 .string "D"
# IR_SUBSCRIPT x[i] variable
 xchain0_n90_α:
 mov rdi, qword ptr [r12 + 1504]
 mov rsi, qword ptr [r12 + 1512]
 mov rdx, qword ptr [r12 + 1520]
 mov rcx, qword ptr [r12 + 1528]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n87_α
 mov qword ptr [r12 + 1536], rax
 mov qword ptr [r12 + 1544], rdx
 jmp xchain0_n94_α
 xchain0_n90_β:
 jmp xchain0_n87_α
# IR_LIT_INTEGER
 xchain0_n91_α:
 mov qword ptr [r12 + 1600], 6
 mov rax, qword ptr [rip + .Lx94_0]
 mov qword ptr [r12 + 1608], rax
 jmp xchain0_n95_α
 xchain0_n91_β:
 jmp xchain0_n92_α
.Lx94_0:
 .quad 5
# IR_VAR
 xchain0_n92_α:
 mov rax, qword ptr [rbx + 64]
 mov rdx, qword ptr [rbx + 72]
 mov qword ptr [r12 + 1664], rax
 mov qword ptr [r12 + 1672], rdx
 jmp xchain0_n96_α
 xchain0_n92_β:
 jmp xchain0_n97_α
# IR_ASSIGN_VAR
 xchain0_n93_α:
 mov rdi, qword ptr [r12 + 1456]
 mov rsi, qword ptr [r12 + 1464]
 mov rdx, qword ptr [r12 + 1472]
 mov rcx, qword ptr [r12 + 1480]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n82_α
 mov qword ptr [r12 + 1488], rax
 mov qword ptr [r12 + 1496], rdx
 jmp xchain0_n82_α
 xchain0_n93_β:
 jmp xchain0_n82_α
# IR_LIT_STRING
 xchain0_n94_α:
 mov qword ptr [r12 + 1552], 1
 mov rax, qword ptr [rip + .Lx97_0]
 mov qword ptr [r12 + 1560], rax
 jmp xchain0_n98_α
 xchain0_n94_β:
 jmp xchain0_n87_α
.Lx97_0:
 .quad .Lx97_0_s
.Lx97_0_s:
 .string "CD"
# IR_SUBSCRIPT x[i] variable
 xchain0_n95_α:
 mov rdi, qword ptr [r12 + 1584]
 mov rsi, qword ptr [r12 + 1592]
 mov rdx, qword ptr [r12 + 1600]
 mov rcx, qword ptr [r12 + 1608]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n92_α
 mov qword ptr [r12 + 1616], rax
 mov qword ptr [r12 + 1624], rdx
 jmp xchain0_n99_α
 xchain0_n95_β:
 jmp xchain0_n92_α
# IR_LIT_INTEGER
 xchain0_n96_α:
 mov qword ptr [r12 + 1680], 6
 mov rax, qword ptr [rip + .Lx99_0]
 mov qword ptr [r12 + 1688], rax
 jmp xchain0_n100_α
 xchain0_n96_β:
 jmp xchain0_n97_α
.Lx99_0:
 .quad 6
# IR_VAR
 xchain0_n97_α:
 mov rax, qword ptr [rbx + 64]
 mov rdx, qword ptr [rbx + 72]
 mov qword ptr [r12 + 1744], rax
 mov qword ptr [r12 + 1752], rdx
 jmp xchain0_n101_α
 xchain0_n97_β:
 jmp xchain0_n102_α
# IR_ASSIGN_VAR
 xchain0_n98_α:
 mov rdi, qword ptr [r12 + 1536]
 mov rsi, qword ptr [r12 + 1544]
 mov rdx, qword ptr [r12 + 1552]
 mov rcx, qword ptr [r12 + 1560]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n87_α
 mov qword ptr [r12 + 1568], rax
 mov qword ptr [r12 + 1576], rdx
 jmp xchain0_n87_α
 xchain0_n98_β:
 jmp xchain0_n87_α
# IR_LIT_STRING
 xchain0_n99_α:
 mov qword ptr [r12 + 1632], 1
 mov rax, qword ptr [rip + .Lx102_0]
 mov qword ptr [r12 + 1640], rax
 jmp xchain0_n103_α
 xchain0_n99_β:
 jmp xchain0_n92_α
.Lx102_0:
 .quad .Lx102_0_s
.Lx102_0_s:
 .string "C"
# IR_SUBSCRIPT x[i] variable
 xchain0_n100_α:
 mov rdi, qword ptr [r12 + 1664]
 mov rsi, qword ptr [r12 + 1672]
 mov rdx, qword ptr [r12 + 1680]
 mov rcx, qword ptr [r12 + 1688]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n97_α
 mov qword ptr [r12 + 1696], rax
 mov qword ptr [r12 + 1704], rdx
 jmp xchain0_n104_α
 xchain0_n100_β:
 jmp xchain0_n97_α
# IR_LIT_INTEGER
 xchain0_n101_α:
 mov qword ptr [r12 + 1760], 6
 mov rax, qword ptr [rip + .Lx104_0]
 mov qword ptr [r12 + 1768], rax
 jmp xchain0_n105_α
 xchain0_n101_β:
 jmp xchain0_n102_α
.Lx104_0:
 .quad 7
# IR_VAR
 xchain0_n102_α:
 mov rax, qword ptr [rbx + 64]
 mov rdx, qword ptr [rbx + 72]
 mov qword ptr [r12 + 1824], rax
 mov qword ptr [r12 + 1832], rdx
 jmp xchain0_n106_α
 xchain0_n102_β:
 jmp xchain0_n107_α
# IR_ASSIGN_VAR
 xchain0_n103_α:
 mov rdi, qword ptr [r12 + 1616]
 mov rsi, qword ptr [r12 + 1624]
 mov rdx, qword ptr [r12 + 1632]
 mov rcx, qword ptr [r12 + 1640]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n92_α
 mov qword ptr [r12 + 1648], rax
 mov qword ptr [r12 + 1656], rdx
 jmp xchain0_n92_α
 xchain0_n103_β:
 jmp xchain0_n92_α
# IR_LIT_STRING
 xchain0_n104_α:
 mov qword ptr [r12 + 1712], 1
 mov rax, qword ptr [rip + .Lx107_0]
 mov qword ptr [r12 + 1720], rax
 jmp xchain0_n108_α
 xchain0_n104_β:
 jmp xchain0_n97_α
.Lx107_0:
 .quad .Lx107_0_s
.Lx107_0_s:
 .string "XC"
# IR_SUBSCRIPT x[i] variable
 xchain0_n105_α:
 mov rdi, qword ptr [r12 + 1744]
 mov rsi, qword ptr [r12 + 1752]
 mov rdx, qword ptr [r12 + 1760]
 mov rcx, qword ptr [r12 + 1768]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n102_α
 mov qword ptr [r12 + 1776], rax
 mov qword ptr [r12 + 1784], rdx
 jmp xchain0_n109_α
 xchain0_n105_β:
 jmp xchain0_n102_α
# IR_LIT_INTEGER
 xchain0_n106_α:
 mov qword ptr [r12 + 1840], 6
 mov rax, qword ptr [rip + .Lx109_0]
 mov qword ptr [r12 + 1848], rax
 jmp xchain0_n110_α
 xchain0_n106_β:
 jmp xchain0_n107_α
.Lx109_0:
 .quad 8
# IR_VAR
 xchain0_n107_α:
 mov rax, qword ptr [rbx + 64]
 mov rdx, qword ptr [rbx + 72]
 mov qword ptr [r12 + 1904], rax
 mov qword ptr [r12 + 1912], rdx
 jmp xchain0_n111_α
 xchain0_n107_β:
 jmp xchain0_n112_α
# IR_ASSIGN_VAR
 xchain0_n108_α:
 mov rdi, qword ptr [r12 + 1696]
 mov rsi, qword ptr [r12 + 1704]
 mov rdx, qword ptr [r12 + 1712]
 mov rcx, qword ptr [r12 + 1720]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n97_α
 mov qword ptr [r12 + 1728], rax
 mov qword ptr [r12 + 1736], rdx
 jmp xchain0_n97_α
 xchain0_n108_β:
 jmp xchain0_n97_α
# IR_LIT_STRING
 xchain0_n109_α:
 mov qword ptr [r12 + 1792], 1
 mov rax, qword ptr [rip + .Lx112_0]
 mov qword ptr [r12 + 1800], rax
 jmp xchain0_n113_α
 xchain0_n109_β:
 jmp xchain0_n102_α
.Lx112_0:
 .quad .Lx112_0_s
.Lx112_0_s:
 .string "L"
# IR_SUBSCRIPT x[i] variable
 xchain0_n110_α:
 mov rdi, qword ptr [r12 + 1824]
 mov rsi, qword ptr [r12 + 1832]
 mov rdx, qword ptr [r12 + 1840]
 mov rcx, qword ptr [r12 + 1848]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n107_α
 mov qword ptr [r12 + 1856], rax
 mov qword ptr [r12 + 1864], rdx
 jmp xchain0_n114_α
 xchain0_n110_β:
 jmp xchain0_n107_α
# IR_LIT_INTEGER
 xchain0_n111_α:
 mov qword ptr [r12 + 1920], 6
 mov rax, qword ptr [rip + .Lx114_0]
 mov qword ptr [r12 + 1928], rax
 jmp xchain0_n115_α
 xchain0_n111_β:
 jmp xchain0_n112_α
.Lx114_0:
 .quad 9
# IR_VAR
 xchain0_n112_α:
 mov rax, qword ptr [rbx + 64]
 mov rdx, qword ptr [rbx + 72]
 mov qword ptr [r12 + 1984], rax
 mov qword ptr [r12 + 1992], rdx
 jmp xchain0_n116_α
 xchain0_n112_β:
 jmp xchain0_n117_α
# IR_ASSIGN_VAR
 xchain0_n113_α:
 mov rdi, qword ptr [r12 + 1776]
 mov rsi, qword ptr [r12 + 1784]
 mov rdx, qword ptr [r12 + 1792]
 mov rcx, qword ptr [r12 + 1800]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n102_α
 mov qword ptr [r12 + 1808], rax
 mov qword ptr [r12 + 1816], rdx
 jmp xchain0_n102_α
 xchain0_n113_β:
 jmp xchain0_n102_α
# IR_LIT_STRING
 xchain0_n114_α:
 mov qword ptr [r12 + 1872], 1
 mov rax, qword ptr [rip + .Lx117_0]
 mov qword ptr [r12 + 1880], rax
 jmp xchain0_n118_α
 xchain0_n114_β:
 jmp xchain0_n107_α
.Lx117_0:
 .quad .Lx117_0_s
.Lx117_0_s:
 .string "XL"
# IR_SUBSCRIPT x[i] variable
 xchain0_n115_α:
 mov rdi, qword ptr [r12 + 1904]
 mov rsi, qword ptr [r12 + 1912]
 mov rdx, qword ptr [r12 + 1920]
 mov rcx, qword ptr [r12 + 1928]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n112_α
 mov qword ptr [r12 + 1936], rax
 mov qword ptr [r12 + 1944], rdx
 jmp xchain0_n119_α
 xchain0_n115_β:
 jmp xchain0_n112_α
# IR_LIT_INTEGER
 xchain0_n116_α:
 mov qword ptr [r12 + 2000], 6
 mov rax, qword ptr [rip + .Lx119_0]
 mov qword ptr [r12 + 2008], rax
 jmp xchain0_n120_α
 xchain0_n116_β:
 jmp xchain0_n117_α
.Lx119_0:
 .quad 10
# IR_VAR
 xchain0_n117_α:
 mov rax, qword ptr [rbx + 64]
 mov rdx, qword ptr [rbx + 72]
 mov qword ptr [r12 + 2064], rax
 mov qword ptr [r12 + 2072], rdx
 jmp xchain0_n121_α
 xchain0_n117_β:
 jmp xchain0_n122_α
# IR_ASSIGN_VAR
 xchain0_n118_α:
 mov rdi, qword ptr [r12 + 1856]
 mov rsi, qword ptr [r12 + 1864]
 mov rdx, qword ptr [r12 + 1872]
 mov rcx, qword ptr [r12 + 1880]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n107_α
 mov qword ptr [r12 + 1888], rax
 mov qword ptr [r12 + 1896], rdx
 jmp xchain0_n107_α
 xchain0_n118_β:
 jmp xchain0_n107_α
# IR_LIT_STRING
 xchain0_n119_α:
 mov qword ptr [r12 + 1952], 1
 mov rax, qword ptr [rip + .Lx122_0]
 mov qword ptr [r12 + 1960], rax
 jmp xchain0_n123_α
 xchain0_n119_β:
 jmp xchain0_n112_α
.Lx122_0:
 .quad .Lx122_0_s
.Lx122_0_s:
 .string "X"
# IR_SUBSCRIPT x[i] variable
 xchain0_n120_α:
 mov rdi, qword ptr [r12 + 1984]
 mov rsi, qword ptr [r12 + 1992]
 mov rdx, qword ptr [r12 + 2000]
 mov rcx, qword ptr [r12 + 2008]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n117_α
 mov qword ptr [r12 + 2016], rax
 mov qword ptr [r12 + 2024], rdx
 jmp xchain0_n124_α
 xchain0_n120_β:
 jmp xchain0_n117_α
# IR_LIT_INTEGER
 xchain0_n121_α:
 mov qword ptr [r12 + 2080], 6
 mov rax, qword ptr [rip + .Lx124_0]
 mov qword ptr [r12 + 2088], rax
 jmp xchain0_n125_α
 xchain0_n121_β:
 jmp xchain0_n122_α
.Lx124_0:
 .quad 11
# IR_VAR
 xchain0_n122_α:
 mov rax, qword ptr [rbx + 64]
 mov rdx, qword ptr [rbx + 72]
 mov qword ptr [r12 + 2144], rax
 mov qword ptr [r12 + 2152], rdx
 jmp xchain0_n126_α
 xchain0_n122_β:
 jmp xchain0_n127_α
# IR_ASSIGN_VAR
 xchain0_n123_α:
 mov rdi, qword ptr [r12 + 1936]
 mov rsi, qword ptr [r12 + 1944]
 mov rdx, qword ptr [r12 + 1952]
 mov rcx, qword ptr [r12 + 1960]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n112_α
 mov qword ptr [r12 + 1968], rax
 mov qword ptr [r12 + 1976], rdx
 jmp xchain0_n112_α
 xchain0_n123_β:
 jmp xchain0_n112_α
# IR_LIT_STRING
 xchain0_n124_α:
 mov qword ptr [r12 + 2032], 1
 mov rax, qword ptr [rip + .Lx127_0]
 mov qword ptr [r12 + 2040], rax
 jmp xchain0_n128_α
 xchain0_n124_β:
 jmp xchain0_n117_α
.Lx127_0:
 .quad .Lx127_0_s
.Lx127_0_s:
 .string "IX"
# IR_SUBSCRIPT x[i] variable
 xchain0_n125_α:
 mov rdi, qword ptr [r12 + 2064]
 mov rsi, qword ptr [r12 + 2072]
 mov rdx, qword ptr [r12 + 2080]
 mov rcx, qword ptr [r12 + 2088]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n122_α
 mov qword ptr [r12 + 2096], rax
 mov qword ptr [r12 + 2104], rdx
 jmp xchain0_n129_α
 xchain0_n125_β:
 jmp xchain0_n122_α
# IR_LIT_INTEGER
 xchain0_n126_α:
 mov qword ptr [r12 + 2160], 6
 mov rax, qword ptr [rip + .Lx129_0]
 mov qword ptr [r12 + 2168], rax
 jmp xchain0_n130_α
 xchain0_n126_β:
 jmp xchain0_n127_α
.Lx129_0:
 .quad 12
# IR_VAR
 xchain0_n127_α:
 mov rax, qword ptr [rbx + 64]
 mov rdx, qword ptr [rbx + 72]
 mov qword ptr [r12 + 2224], rax
 mov qword ptr [r12 + 2232], rdx
 jmp xchain0_n131_α
 xchain0_n127_β:
 jmp xchain0_n132_α
# IR_ASSIGN_VAR
 xchain0_n128_α:
 mov rdi, qword ptr [r12 + 2016]
 mov rsi, qword ptr [r12 + 2024]
 mov rdx, qword ptr [r12 + 2032]
 mov rcx, qword ptr [r12 + 2040]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n117_α
 mov qword ptr [r12 + 2048], rax
 mov qword ptr [r12 + 2056], rdx
 jmp xchain0_n117_α
 xchain0_n128_β:
 jmp xchain0_n117_α
# IR_LIT_STRING
 xchain0_n129_α:
 mov qword ptr [r12 + 2112], 1
 mov rax, qword ptr [rip + .Lx132_0]
 mov qword ptr [r12 + 2120], rax
 jmp xchain0_n133_α
 xchain0_n129_β:
 jmp xchain0_n122_α
.Lx132_0:
 .quad .Lx132_0_s
.Lx132_0_s:
 .string "V"
# IR_SUBSCRIPT x[i] variable
 xchain0_n130_α:
 mov rdi, qword ptr [r12 + 2144]
 mov rsi, qword ptr [r12 + 2152]
 mov rdx, qword ptr [r12 + 2160]
 mov rcx, qword ptr [r12 + 2168]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n127_α
 mov qword ptr [r12 + 2176], rax
 mov qword ptr [r12 + 2184], rdx
 jmp xchain0_n134_α
 xchain0_n130_β:
 jmp xchain0_n127_α
# IR_LIT_INTEGER
 xchain0_n131_α:
 mov qword ptr [r12 + 2240], 6
 mov rax, qword ptr [rip + .Lx134_0]
 mov qword ptr [r12 + 2248], rax
 jmp xchain0_n135_α
 xchain0_n131_β:
 jmp xchain0_n132_α
.Lx134_0:
 .quad 13
# IR_LIT_INTEGER
 xchain0_n132_α:
 mov qword ptr [r12 + 2320], 6
 mov rax, qword ptr [rip + .Lx135_0]
 mov qword ptr [r12 + 2328], rax
 jmp xchain0_n136_α
 xchain0_n132_β:
 jmp xchain0_n139_α
.Lx135_0:
 .quad 1
# IR_ASSIGN_VAR
 xchain0_n133_α:
 mov rdi, qword ptr [r12 + 2096]
 mov rsi, qword ptr [r12 + 2104]
 mov rdx, qword ptr [r12 + 2112]
 mov rcx, qword ptr [r12 + 2120]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n122_α
 mov qword ptr [r12 + 2128], rax
 mov qword ptr [r12 + 2136], rdx
 jmp xchain0_n122_α
 xchain0_n133_β:
 jmp xchain0_n122_α
# IR_LIT_STRING
 xchain0_n134_α:
 mov qword ptr [r12 + 2192], 1
 mov rax, qword ptr [rip + .Lx137_0]
 mov qword ptr [r12 + 2200], rax
 jmp xchain0_n137_α
 xchain0_n134_β:
 jmp xchain0_n127_α
.Lx137_0:
 .quad .Lx137_0_s
.Lx137_0_s:
 .string "IV"
# IR_SUBSCRIPT x[i] variable
 xchain0_n135_α:
 mov rdi, qword ptr [r12 + 2224]
 mov rsi, qword ptr [r12 + 2232]
 mov rdx, qword ptr [r12 + 2240]
 mov rcx, qword ptr [r12 + 2248]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n132_α
 mov qword ptr [r12 + 2256], rax
 mov qword ptr [r12 + 2264], rdx
 jmp xchain0_n138_α
 xchain0_n135_β:
 jmp xchain0_n132_α
# IR_ASSIGN gva
 xchain0_n136_α:
 mov rax, qword ptr [r12 + 2320]
 mov rdx, qword ptr [r12 + 2328]
 mov qword ptr [rbx + 80], rax
 mov qword ptr [rbx + 88], rdx
 mov qword ptr [r12 + 2304], rax
 mov qword ptr [r12 + 2312], rdx
 jmp xchain0_n139_α
 xchain0_n136_β:
 jmp xchain0_n139_α
# IR_ASSIGN_VAR
 xchain0_n137_α:
 mov rdi, qword ptr [r12 + 2176]
 mov rsi, qword ptr [r12 + 2184]
 mov rdx, qword ptr [r12 + 2192]
 mov rcx, qword ptr [r12 + 2200]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n127_α
 mov qword ptr [r12 + 2208], rax
 mov qword ptr [r12 + 2216], rdx
 jmp xchain0_n127_α
 xchain0_n137_β:
 jmp xchain0_n127_α
# IR_LIT_STRING
 xchain0_n138_α:
 mov qword ptr [r12 + 2272], 1
 mov rax, qword ptr [rip + .Lx141_0]
 mov qword ptr [r12 + 2280], rax
 jmp xchain0_n140_α
 xchain0_n138_β:
 jmp xchain0_n132_α
.Lx141_0:
 .quad .Lx141_0_s
.Lx141_0_s:
 .string "I"
# IR_VAR
 xchain0_n139_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 2384], rax
 mov qword ptr [r12 + 2392], rdx
 jmp xchain0_n141_α
 xchain0_n139_β:
 jmp xchain0_n142_α
# IR_ASSIGN_VAR
 xchain0_n140_α:
 mov rdi, qword ptr [r12 + 2256]
 mov rsi, qword ptr [r12 + 2264]
 mov rdx, qword ptr [r12 + 2272]
 mov rcx, qword ptr [r12 + 2280]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n132_α
 mov qword ptr [r12 + 2288], rax
 mov qword ptr [r12 + 2296], rdx
 jmp xchain0_n132_α
 xchain0_n140_β:
 jmp xchain0_n132_α
# IR_LIT_INTEGER
 xchain0_n141_α:
 mov qword ptr [r12 + 2400], 6
 mov rax, qword ptr [rip + .Lx144_0]
 mov qword ptr [r12 + 2408], rax
 jmp xchain0_n143_α
 xchain0_n141_β:
 jmp xchain0_n142_α
.Lx144_0:
 .quad 0
# IR_VAR
 xchain0_n142_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 2848], rax
 mov qword ptr [r12 + 2856], rdx
 jmp xchain0_n144_α
 xchain0_n142_β:
 jmp proc_roman_γ
 xchain0_n143_α:
# BOX IR_CALL GT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+2384] -> [zr+2352]
 mov rax, qword ptr [r12 + 2384]
 mov qword ptr [r12 + 2352], rax
 mov rax, qword ptr [r12 + 2392]
 mov qword ptr [r12 + 2360], rax
# marshal arg1 = producer-box slot [zr+2400] -> [zr+2368]
 mov rax, qword ptr [r12 + 2400]
 mov qword ptr [r12 + 2368], rax
 mov rax, qword ptr [r12 + 2408]
 mov qword ptr [r12 + 2376], rax
  .section .rodata
  .Lrkfn147: .string "GT"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn147]
 lea rsi, [r12 + 2352]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 2336], rax
 mov qword ptr [r12 + 2344], rdx
 cmp eax, 99
 je xchain0_n142_α
 jmp xchain0_n145_α
 xchain0_n143_β:
 jmp xchain0_n142_α
# IR_ASSIGN gva
 xchain0_n144_α:
 mov rax, qword ptr [r12 + 2848]
 mov rdx, qword ptr [r12 + 2856]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 2832], rax
 mov qword ptr [r12 + 2840], rdx
 jmp proc_roman_γ
 xchain0_n144_β:
 jmp proc_roman_γ
# IR_VAR
 xchain0_n145_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 2464], rax
 mov qword ptr [r12 + 2472], rdx
 jmp xchain0_n146_α
 xchain0_n145_β:
 jmp xchain0_n147_α
# IR_VAR
 xchain0_n146_α:
 mov rax, qword ptr [rbx + 48]
 mov rdx, qword ptr [rbx + 56]
 mov qword ptr [r12 + 2480], rax
 mov qword ptr [r12 + 2488], rdx
 jmp xchain0_n148_α
 xchain0_n146_β:
 jmp xchain0_n147_α
# IR_VAR
 xchain0_n147_α:
 mov rax, qword ptr [rbx + 80]
 mov rdx, qword ptr [rbx + 88]
 mov qword ptr [r12 + 2800], rax
 mov qword ptr [r12 + 2808], rdx
 jmp xchain0_n149_α
 xchain0_n147_β:
 jmp xchain0_n139_α
# IR_VAR
 xchain0_n148_α:
 mov rax, qword ptr [rbx + 80]
 mov rdx, qword ptr [rbx + 88]
 mov qword ptr [r12 + 2496], rax
 mov qword ptr [r12 + 2504], rdx
 jmp xchain0_n150_α
 xchain0_n148_β:
 jmp xchain0_n147_α
# IR_LIT_INTEGER
 xchain0_n149_α:
 mov qword ptr [r12 + 2816], 6
 mov rax, qword ptr [rip + .Lx153_0]
 mov qword ptr [r12 + 2824], rax
 jmp xchain0_n151_α
 xchain0_n149_β:
 jmp xchain0_n139_α
.Lx153_0:
 .quad 1
# IR_SUBSCRIPT x[i] variable
 xchain0_n150_α:
 mov rdi, qword ptr [r12 + 2480]
 mov rsi, qword ptr [r12 + 2488]
 mov rdx, qword ptr [r12 + 2496]
 mov rcx, qword ptr [r12 + 2504]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n147_α
 mov qword ptr [r12 + 2512], rax
 mov qword ptr [r12 + 2520], rdx
 jmp xchain0_n152_α
 xchain0_n150_β:
 jmp xchain0_n147_α
 xchain0_n151_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 2800]
 cmp eax, 100
 je .Lx155_0
 mov eax, dword ptr [r12 + 2816]
 cmp eax, 100
 je .Lx155_0
 mov eax, dword ptr [r12 + 2800]
 cmp eax, 6
 jne .Lx155_2
 mov eax, dword ptr [r12 + 2816]
 cmp eax, 6
 jne .Lx155_2
.Lx155_1:
 mov rax, qword ptr [r12 + 2808]
 mov rcx, qword ptr [r12 + 2824]
 add rax, rcx
 mov qword ptr [r12 + 2784], 6
 mov qword ptr [r12 + 2792], rax
 jmp xchain0_n153_α
.Lx155_0:
 mov rdi, qword ptr [r12 + 2800]
 mov rsi, qword ptr [r12 + 2808]
 mov rdx, qword ptr [r12 + 2816]
 mov rcx, qword ptr [r12 + 2824]
 mov r8d, 0
 lea r9, [r12 + 2784]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx155_3
.Lx155_2:
 mov rdi, qword ptr [r12 + 2800]
 mov rsi, qword ptr [r12 + 2808]
 mov rdx, qword ptr [r12 + 2816]
 mov rcx, qword ptr [r12 + 2824]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n139_α
 mov qword ptr [r12 + 2784], rax
 mov qword ptr [r12 + 2792], rdx
.Lx155_3:
 jmp xchain0_n153_α
 xchain0_n151_β:
 jmp xchain0_n139_α
# IR_DEREF variable -> value
 xchain0_n152_α:
 mov rdi, qword ptr [r12 + 2512]
 mov rsi, qword ptr [r12 + 2520]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n147_α
 mov qword ptr [r12 + 2528], rax
 mov qword ptr [r12 + 2536], rdx
 jmp xchain0_n154_α
 xchain0_n152_β:
 jmp xchain0_n147_α
# IR_ASSIGN gva
 xchain0_n153_α:
 mov rax, qword ptr [r12 + 2784]
 mov rdx, qword ptr [r12 + 2792]
 mov qword ptr [rbx + 80], rax
 mov qword ptr [rbx + 88], rdx
 mov qword ptr [r12 + 2768], rax
 mov qword ptr [r12 + 2776], rdx
 jmp xchain0_n139_α
 xchain0_n153_β:
 jmp xchain0_n139_α
 xchain0_n154_α:
# BOX IR_CALL GE(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+2464] -> [zr+2432]
 mov rax, qword ptr [r12 + 2464]
 mov qword ptr [r12 + 2432], rax
 mov rax, qword ptr [r12 + 2472]
 mov qword ptr [r12 + 2440], rax
# marshal arg1 = producer-box slot [zr+2528] -> [zr+2448]
 mov rax, qword ptr [r12 + 2528]
 mov qword ptr [r12 + 2448], rax
 mov rax, qword ptr [r12 + 2536]
 mov qword ptr [r12 + 2456], rax
  .section .rodata
  .Lrkfn159: .string "GE"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn159]
 lea rsi, [r12 + 2432]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 2416], rax
 mov qword ptr [r12 + 2424], rdx
 cmp eax, 99
 je xchain0_n147_α
 jmp xchain0_n155_α
 xchain0_n154_β:
 jmp xchain0_n147_α
# IR_VAR
 xchain0_n155_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 2576], rax
 mov qword ptr [r12 + 2584], rdx
 jmp xchain0_n156_α
 xchain0_n155_β:
 jmp xchain0_n157_α
# IR_VAR
 xchain0_n156_α:
 mov rax, qword ptr [rbx + 64]
 mov rdx, qword ptr [rbx + 72]
 mov qword ptr [r12 + 2592], rax
 mov qword ptr [r12 + 2600], rdx
 jmp xchain0_n158_α
 xchain0_n156_β:
 jmp xchain0_n157_α
# IR_VAR
 xchain0_n157_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 2688], rax
 mov qword ptr [r12 + 2696], rdx
 jmp xchain0_n159_α
 xchain0_n157_β:
 jmp xchain0_n139_α
# IR_VAR
 xchain0_n158_α:
 mov rax, qword ptr [rbx + 80]
 mov rdx, qword ptr [rbx + 88]
 mov qword ptr [r12 + 2608], rax
 mov qword ptr [r12 + 2616], rdx
 jmp xchain0_n160_α
 xchain0_n158_β:
 jmp xchain0_n157_α
# IR_VAR
 xchain0_n159_α:
 mov rax, qword ptr [rbx + 48]
 mov rdx, qword ptr [rbx + 56]
 mov qword ptr [r12 + 2704], rax
 mov qword ptr [r12 + 2712], rdx
 jmp xchain0_n161_α
 xchain0_n159_β:
 jmp xchain0_n139_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n160_α:
 mov rdi, qword ptr [r12 + 2592]
 mov rsi, qword ptr [r12 + 2600]
 mov rdx, qword ptr [r12 + 2608]
 mov rcx, qword ptr [r12 + 2616]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n157_α
 mov qword ptr [r12 + 2624], rax
 mov qword ptr [r12 + 2632], rdx
 jmp xchain0_n162_α
 xchain0_n160_β:
 jmp xchain0_n157_α
# IR_VAR
 xchain0_n161_α:
 mov rax, qword ptr [rbx + 80]
 mov rdx, qword ptr [rbx + 88]
 mov qword ptr [r12 + 2720], rax
 mov qword ptr [r12 + 2728], rdx
 jmp xchain0_n163_α
 xchain0_n161_β:
 jmp xchain0_n139_α
# IR_DEREF variable -> value
 xchain0_n162_α:
 mov rdi, qword ptr [r12 + 2624]
 mov rsi, qword ptr [r12 + 2632]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n157_α
 mov qword ptr [r12 + 2640], rax
 mov qword ptr [r12 + 2648], rdx
 jmp xchain0_n164_α
 xchain0_n162_β:
 jmp xchain0_n157_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n163_α:
 mov rdi, qword ptr [r12 + 2704]
 mov rsi, qword ptr [r12 + 2712]
 mov rdx, qword ptr [r12 + 2720]
 mov rcx, qword ptr [r12 + 2728]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n139_α
 mov qword ptr [r12 + 2736], rax
 mov qword ptr [r12 + 2744], rdx
 jmp xchain0_n165_α
 xchain0_n163_β:
 jmp xchain0_n139_α
 xchain0_n164_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 2576]
 mov rsi, qword ptr [r12 + 2584]
 mov rdx, qword ptr [r12 + 2640]
 mov rcx, qword ptr [r12 + 2648]
 call str_concat_d@PLT
 mov qword ptr [r12 + 2560], rax
 mov qword ptr [r12 + 2568], rdx
 jmp xchain0_n166_α
 xchain0_n164_β:
 jmp xchain0_n157_α
# IR_DEREF variable -> value
 xchain0_n165_α:
 mov rdi, qword ptr [r12 + 2736]
 mov rsi, qword ptr [r12 + 2744]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n139_α
 mov qword ptr [r12 + 2752], rax
 mov qword ptr [r12 + 2760], rdx
 jmp xchain0_n167_α
 xchain0_n165_β:
 jmp xchain0_n139_α
# IR_ASSIGN gva
 xchain0_n166_α:
 mov rax, qword ptr [r12 + 2560]
 mov rdx, qword ptr [r12 + 2568]
 mov qword ptr [rbx + 32], rax
 mov qword ptr [rbx + 40], rdx
 mov qword ptr [r12 + 2544], rax
 mov qword ptr [r12 + 2552], rdx
 jmp xchain0_n157_α
 xchain0_n166_β:
 jmp xchain0_n157_α
 xchain0_n167_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 2688]
 cmp eax, 100
 je .Lx172_0
 mov eax, dword ptr [r12 + 2752]
 cmp eax, 100
 je .Lx172_0
 mov eax, dword ptr [r12 + 2688]
 cmp eax, 6
 jne .Lx172_2
 mov eax, dword ptr [r12 + 2752]
 cmp eax, 6
 jne .Lx172_2
.Lx172_1:
 mov rax, qword ptr [r12 + 2696]
 mov rcx, qword ptr [r12 + 2760]
 sub rax, rcx
 mov qword ptr [r12 + 2672], 6
 mov qword ptr [r12 + 2680], rax
 jmp xchain0_n168_α
.Lx172_0:
 mov rdi, qword ptr [r12 + 2688]
 mov rsi, qword ptr [r12 + 2696]
 mov rdx, qword ptr [r12 + 2752]
 mov rcx, qword ptr [r12 + 2760]
 mov r8d, 1
 lea r9, [r12 + 2672]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx172_3
.Lx172_2:
 mov rdi, qword ptr [r12 + 2688]
 mov rsi, qword ptr [r12 + 2696]
 mov rdx, qword ptr [r12 + 2752]
 mov rcx, qword ptr [r12 + 2760]
 mov r8d, 1
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n139_α
 mov qword ptr [r12 + 2672], rax
 mov qword ptr [r12 + 2680], rdx
.Lx172_3:
 jmp xchain0_n168_α
 xchain0_n167_β:
 jmp xchain0_n139_α
# IR_ASSIGN gva
 xchain0_n168_α:
 mov rax, qword ptr [r12 + 2672]
 mov rdx, qword ptr [r12 + 2680]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 2656], rax
 mov qword ptr [r12 + 2664], rdx
 jmp xchain0_n139_α
 xchain0_n168_β:
 jmp xchain0_n139_α
proc_roman_β:
jmp proc_roman_ω
proc_roman_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 3248]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_roman_ω:
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
proc_startup:
  push rbp
  mov rbp, rsp
  .section .rodata
  .Lstartup_pname0: .string "roman"
  .Lstartup_pp0_0: .string "n"
  .Lstartup_pp0_1: .string "s"
  .Lstartup_pp0_2: .string "v"
  .Lstartup_pp0_3: .string "r"
  .Lstartup_pp0_4: .string "i"
  .align 8
  .Lstartup_pnames0:
  .quad .Lstartup_pp0_0
  .quad .Lstartup_pp0_1
  .quad .Lstartup_pp0_2
  .quad .Lstartup_pp0_3
  .quad .Lstartup_pp0_4
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname0]
  lea rsi, [rip + .Lstartup_pnames0]
  mov edx, 5
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname0]
  lea rsi, [rip + proc_roman_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 5
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 3264
  call rt_proc_set_frame_bytes@PLT
  pop rbp
  ret
  .section .rodata
  .Lgvan0: .string "roman"
  .Lgvan1: .string "n"
  .Lgvan2: .string "s"
  .Lgvan3: .string "v"
  .Lgvan4: .string "r"
  .Lgvan5: .string "i"
  .align 8
__gva_names:
  .quad .Lgvan0
  .quad .Lgvan1
  .quad .Lgvan2
  .quad .Lgvan3
  .quad .Lgvan4
  .quad .Lgvan5
  .section .bss
  .align 16
__gva: .space 96, 0
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  push rbp
  mov rbp, rsp
  push rdi
  push rsi
  call core_lib_init@PLT
  call proc_startup
  lea rdi, [rip + __gva_names]
  lea rsi, [rip + __gva]
  mov edx, 6
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
 mov qword ptr [r12 + 3248], rax
 pop rsi
main_α_body:
# IR_LIT_INTEGER
 xchain174_n0_α:
 mov qword ptr [r12 + 2912], 6
 mov rax, qword ptr [rip + .Lx175_0]
 mov qword ptr [r12 + 2920], rax
 jmp xchain174_n1_α
 xchain174_n0_β:
 jmp xchain174_n3_α
.Lx175_0:
 .quad 1
 xchain174_n1_α:
  .section .rodata
  .Lcall171_pname: .string "roman"
  .section .text
  .intel_syntax noprefix
 mov edi, 0
 mov rsi, qword ptr [r12 + 2912]
 mov rdx, qword ptr [r12 + 2920]
 call rt_arg_stage@PLT
   lea rdi, [rip + .Lcall171_pname]
 mov esi, 1
 call rt_call_proc_descr@PLT
 mov qword ptr [r12 + 2880], rax
 mov qword ptr [r12 + 2888], rdx
 cmp eax, 99
 je xchain174_n3_α
 jmp xchain174_n2_α
xchain174_n1_β:
 jmp xchain174_n3_α
# IR_ASSIGN global
 xchain174_n2_α:
 mov rsi, qword ptr [r12 + 2880]
 mov rdx, qword ptr [r12 + 2888]
 mov rdi, qword ptr [rip + .Lx177_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 2864], rax
 mov qword ptr [r12 + 2872], rdx
 jmp xchain174_n3_α
 xchain174_n2_β:
 jmp xchain174_n3_α
.Lx177_0:
 .quad .Lx177_0_s
.Lx177_0_s:
 .string "OUTPUT"
# IR_LIT_INTEGER
 xchain174_n3_α:
 mov qword ptr [r12 + 2976], 6
 mov rax, qword ptr [rip + .Lx178_0]
 mov qword ptr [r12 + 2984], rax
 jmp xchain174_n4_α
 xchain174_n3_β:
 jmp xchain174_n6_α
.Lx178_0:
 .quad 4
 xchain174_n4_α:
  .section .rodata
  .Lcall174_pname: .string "roman"
  .section .text
  .intel_syntax noprefix
 mov edi, 0
 mov rsi, qword ptr [r12 + 2976]
 mov rdx, qword ptr [r12 + 2984]
 call rt_arg_stage@PLT
   lea rdi, [rip + .Lcall174_pname]
 mov esi, 1
 call rt_call_proc_descr@PLT
 mov qword ptr [r12 + 2944], rax
 mov qword ptr [r12 + 2952], rdx
 cmp eax, 99
 je xchain174_n6_α
 jmp xchain174_n5_α
xchain174_n4_β:
 jmp xchain174_n6_α
# IR_ASSIGN global
 xchain174_n5_α:
 mov rsi, qword ptr [r12 + 2944]
 mov rdx, qword ptr [r12 + 2952]
 mov rdi, qword ptr [rip + .Lx180_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 2928], rax
 mov qword ptr [r12 + 2936], rdx
 jmp xchain174_n6_α
 xchain174_n5_β:
 jmp xchain174_n6_α
.Lx180_0:
 .quad .Lx180_0_s
.Lx180_0_s:
 .string "OUTPUT"
# IR_LIT_INTEGER
 xchain174_n6_α:
 mov qword ptr [r12 + 3040], 6
 mov rax, qword ptr [rip + .Lx181_0]
 mov qword ptr [r12 + 3048], rax
 jmp xchain174_n7_α
 xchain174_n6_β:
 jmp xchain174_n9_α
.Lx181_0:
 .quad 9
 xchain174_n7_α:
  .section .rodata
  .Lcall177_pname: .string "roman"
  .section .text
  .intel_syntax noprefix
 mov edi, 0
 mov rsi, qword ptr [r12 + 3040]
 mov rdx, qword ptr [r12 + 3048]
 call rt_arg_stage@PLT
   lea rdi, [rip + .Lcall177_pname]
 mov esi, 1
 call rt_call_proc_descr@PLT
 mov qword ptr [r12 + 3008], rax
 mov qword ptr [r12 + 3016], rdx
 cmp eax, 99
 je xchain174_n9_α
 jmp xchain174_n8_α
xchain174_n7_β:
 jmp xchain174_n9_α
# IR_ASSIGN global
 xchain174_n8_α:
 mov rsi, qword ptr [r12 + 3008]
 mov rdx, qword ptr [r12 + 3016]
 mov rdi, qword ptr [rip + .Lx183_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 2992], rax
 mov qword ptr [r12 + 3000], rdx
 jmp xchain174_n9_α
 xchain174_n8_β:
 jmp xchain174_n9_α
.Lx183_0:
 .quad .Lx183_0_s
.Lx183_0_s:
 .string "OUTPUT"
# IR_LIT_INTEGER
 xchain174_n9_α:
 mov qword ptr [r12 + 3104], 6
 mov rax, qword ptr [rip + .Lx184_0]
 mov qword ptr [r12 + 3112], rax
 jmp xchain174_n10_α
 xchain174_n9_β:
 jmp xchain174_n12_α
.Lx184_0:
 .quad 42
 xchain174_n10_α:
  .section .rodata
  .Lcall180_pname: .string "roman"
  .section .text
  .intel_syntax noprefix
 mov edi, 0
 mov rsi, qword ptr [r12 + 3104]
 mov rdx, qword ptr [r12 + 3112]
 call rt_arg_stage@PLT
   lea rdi, [rip + .Lcall180_pname]
 mov esi, 1
 call rt_call_proc_descr@PLT
 mov qword ptr [r12 + 3072], rax
 mov qword ptr [r12 + 3080], rdx
 cmp eax, 99
 je xchain174_n12_α
 jmp xchain174_n11_α
xchain174_n10_β:
 jmp xchain174_n12_α
# IR_ASSIGN global
 xchain174_n11_α:
 mov rsi, qword ptr [r12 + 3072]
 mov rdx, qword ptr [r12 + 3080]
 mov rdi, qword ptr [rip + .Lx186_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 3056], rax
 mov qword ptr [r12 + 3064], rdx
 jmp xchain174_n12_α
 xchain174_n11_β:
 jmp xchain174_n12_α
.Lx186_0:
 .quad .Lx186_0_s
.Lx186_0_s:
 .string "OUTPUT"
# IR_LIT_INTEGER
 xchain174_n12_α:
 mov qword ptr [r12 + 3168], 6
 mov rax, qword ptr [rip + .Lx187_0]
 mov qword ptr [r12 + 3176], rax
 jmp xchain174_n13_α
 xchain174_n12_β:
 jmp xchain174_n15_α
.Lx187_0:
 .quad 1999
 xchain174_n13_α:
  .section .rodata
  .Lcall183_pname: .string "roman"
  .section .text
  .intel_syntax noprefix
 mov edi, 0
 mov rsi, qword ptr [r12 + 3168]
 mov rdx, qword ptr [r12 + 3176]
 call rt_arg_stage@PLT
   lea rdi, [rip + .Lcall183_pname]
 mov esi, 1
 call rt_call_proc_descr@PLT
 mov qword ptr [r12 + 3136], rax
 mov qword ptr [r12 + 3144], rdx
 cmp eax, 99
 je xchain174_n15_α
 jmp xchain174_n14_α
xchain174_n13_β:
 jmp xchain174_n15_α
# IR_ASSIGN global
 xchain174_n14_α:
 mov rsi, qword ptr [r12 + 3136]
 mov rdx, qword ptr [r12 + 3144]
 mov rdi, qword ptr [rip + .Lx189_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 3120], rax
 mov qword ptr [r12 + 3128], rdx
 jmp xchain174_n15_α
 xchain174_n14_β:
 jmp xchain174_n15_α
.Lx189_0:
 .quad .Lx189_0_s
.Lx189_0_s:
 .string "OUTPUT"
# IR_LIT_INTEGER
 xchain174_n15_α:
 mov qword ptr [r12 + 3232], 6
 mov rax, qword ptr [rip + .Lx190_0]
 mov qword ptr [r12 + 3240], rax
 jmp xchain174_n16_α
 xchain174_n15_β:
 jmp main_γ
.Lx190_0:
 .quad 2024
 xchain174_n16_α:
  .section .rodata
  .Lcall186_pname: .string "roman"
  .section .text
  .intel_syntax noprefix
 mov edi, 0
 mov rsi, qword ptr [r12 + 3232]
 mov rdx, qword ptr [r12 + 3240]
 call rt_arg_stage@PLT
   lea rdi, [rip + .Lcall186_pname]
 mov esi, 1
 call rt_call_proc_descr@PLT
 mov qword ptr [r12 + 3200], rax
 mov qword ptr [r12 + 3208], rdx
 cmp eax, 99
 je main_γ
 jmp xchain174_n17_α
xchain174_n16_β:
 jmp main_γ
# IR_ASSIGN global
 xchain174_n17_α:
 mov rsi, qword ptr [r12 + 3200]
 mov rdx, qword ptr [r12 + 3208]
 mov rdi, qword ptr [rip + .Lx192_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 3184], rax
 mov qword ptr [r12 + 3192], rdx
 jmp main_γ
 xchain174_n17_β:
 jmp main_γ
.Lx192_0:
 .quad .Lx192_0_s
.Lx192_0_s:
 .string "OUTPUT"
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
 mov rdi, qword ptr [r12 + 3248]
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
