  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "a"
  .Lgvan1: .string "output"
  .Lgvan2: .string "b"
  .align 8
__gva_names:
  .quad .Lgvan0
  .quad .Lgvan1
  .quad .Lgvan2
  .section .bss
  .align 16
__gva: .space 48, 0
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
  mov edx, 3
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
 .quad 3
xchain0_n1_α:
bb2_α:
# BOX IR_CALL array(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+64] -> [r12+48]
 mov rax, qword ptr [r12 + 64]
 mov qword ptr [r12 + 48], rax
 mov rax, qword ptr [r12 + 72]
 mov qword ptr [r12 + 56], rax
  .section .rodata
  .Lbynamefn3: .string "array"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn3]
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
 jmp xchain0_n7_α
xchain0_n4_α:
# IR_LIT_INTEGER
bb5_α:
 mov qword ptr [r12 + 128], 6
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [r12 + 136], rax
 jmp xchain0_n5_α
 xchain0_n4_β:
 jmp xchain0_n7_α
.Lx6_0:
 .quad 1
xchain0_n5_α:
# IR_SUBSCRIPT x[i] variable
bb6_α:
 mov rdi, qword ptr [r12 + 112]
 mov rsi, qword ptr [r12 + 120]
 mov rdx, qword ptr [r12 + 128]
 mov rcx, qword ptr [r12 + 136]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n7_α
 mov qword ptr [r12 + 144], rax
 mov qword ptr [r12 + 152], rdx
 jmp xchain0_n6_α
 xchain0_n5_β:
 jmp xchain0_n7_α
xchain0_n6_α:
# IR_DEREF variable -> value
bb7_α:
 mov rdi, qword ptr [r12 + 144]
 mov rsi, qword ptr [r12 + 152]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n7_α
 mov qword ptr [r12 + 160], rax
 mov qword ptr [r12 + 168], rdx
 jmp xchain0_n8_α
 xchain0_n6_β:
 jmp xchain0_n7_α
xchain0_n7_α:
# IR_VAR gva
bb8_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 208], rax
 mov qword ptr [r12 + 216], rdx
 jmp xchain0_n9_α
 xchain0_n7_β:
 jmp xchain0_n14_α
xchain0_n8_α:
bb9_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+160] -> [r12+96]
 mov rax, qword ptr [r12 + 160]
 mov qword ptr [r12 + 96], rax
 mov rax, qword ptr [r12 + 168]
 mov qword ptr [r12 + 104], rax
  .section .rodata
  .Lbynamefn11: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn11]
 lea rsi, [r12 + 96]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 80], rax
 mov qword ptr [r12 + 88], rdx
 cmp eax, 99
 je xchain0_n7_α
 jmp xchain0_n10_α
xchain0_n8_β:
 jmp xchain0_n7_α
xchain0_n9_α:
# IR_LIT_INTEGER
bb10_α:
 mov qword ptr [r12 + 224], 6
 mov rax, qword ptr [rip + .Lx12_0]
 mov qword ptr [r12 + 232], rax
 jmp xchain0_n11_α
 xchain0_n9_β:
 jmp xchain0_n14_α
.Lx12_0:
 .quad 2
xchain0_n10_α:
# IR_LIT_STRING
bb11_α:
 mov qword ptr [r12 + 192], 1
 mov rax, qword ptr [rip + .Lx13_0]
 mov qword ptr [r12 + 200], rax
 jmp xchain0_n12_α
 xchain0_n10_β:
 jmp main_γ
.Lx13_0:
 .quad .Lx13_0_s
.Lx13_0_s:
 .string "FAIL 1110/001: array element init null"
xchain0_n11_α:
# IR_SUBSCRIPT x[i] variable
bb12_α:
 mov rdi, qword ptr [r12 + 208]
 mov rsi, qword ptr [r12 + 216]
 mov rdx, qword ptr [r12 + 224]
 mov rcx, qword ptr [r12 + 232]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n14_α
 mov qword ptr [r12 + 240], rax
 mov qword ptr [r12 + 248], rdx
 jmp xchain0_n13_α
 xchain0_n11_β:
 jmp xchain0_n14_α
xchain0_n12_α:
# IR_ASSIGN gva
bb13_α:
 mov rax, qword ptr [r12 + 192]
 mov rdx, qword ptr [r12 + 200]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 176], rax
 mov qword ptr [r12 + 184], rdx
 jmp main_γ
 xchain0_n12_β:
 jmp main_γ
xchain0_n13_α:
# IR_LIT_REAL
bb14_α:
 mov qword ptr [r12 + 256], 7
 mov rax, qword ptr [rip + .Lx16_0]
 mov qword ptr [r12 + 264], rax
 jmp xchain0_n15_α
 xchain0_n13_β:
 jmp xchain0_n14_α
.Lx16_0:
 .quad 4616752568008179712
xchain0_n14_α:
# IR_VAR gva
bb15_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 336], rax
 mov qword ptr [r12 + 344], rdx
 jmp xchain0_n16_α
 xchain0_n14_β:
 jmp xchain0_n19_α
xchain0_n15_α:
# IR_ASSIGN_VAR write through variable
bb16_α:
 mov rdi, qword ptr [r12 + 240]
 mov rsi, qword ptr [r12 + 248]
 mov rdx, qword ptr [r12 + 256]
 mov rcx, qword ptr [r12 + 264]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n14_α
 mov qword ptr [r12 + 272], rax
 mov qword ptr [r12 + 280], rdx
 jmp xchain0_n14_α
 xchain0_n15_β:
 jmp xchain0_n14_α
xchain0_n16_α:
# IR_LIT_INTEGER
bb17_α:
 mov qword ptr [r12 + 352], 6
 mov rax, qword ptr [rip + .Lx19_0]
 mov qword ptr [r12 + 360], rax
 jmp xchain0_n17_α
 xchain0_n16_β:
 jmp xchain0_n19_α
.Lx19_0:
 .quad 2
xchain0_n17_α:
# IR_SUBSCRIPT x[i] variable
bb18_α:
 mov rdi, qword ptr [r12 + 336]
 mov rsi, qword ptr [r12 + 344]
 mov rdx, qword ptr [r12 + 352]
 mov rcx, qword ptr [r12 + 360]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n19_α
 mov qword ptr [r12 + 368], rax
 mov qword ptr [r12 + 376], rdx
 jmp xchain0_n18_α
 xchain0_n17_β:
 jmp xchain0_n19_α
xchain0_n18_α:
# IR_DEREF variable -> value
bb19_α:
 mov rdi, qword ptr [r12 + 368]
 mov rsi, qword ptr [r12 + 376]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n19_α
 mov qword ptr [r12 + 384], rax
 mov qword ptr [r12 + 392], rdx
 jmp xchain0_n20_α
 xchain0_n18_β:
 jmp xchain0_n19_α
xchain0_n19_α:
# IR_VAR gva
bb20_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 448], rax
 mov qword ptr [r12 + 456], rdx
 jmp xchain0_n21_α
 xchain0_n19_β:
 jmp xchain0_n26_α
xchain0_n20_α:
# IR_LIT_REAL
bb21_α:
 mov qword ptr [r12 + 400], 7
 mov rax, qword ptr [rip + .Lx23_0]
 mov qword ptr [r12 + 408], rax
 jmp xchain0_n22_α
 xchain0_n20_β:
 jmp xchain0_n19_α
.Lx23_0:
 .quad 4616752568008179712
xchain0_n21_α:
# IR_LIT_INTEGER
bb22_α:
 mov qword ptr [r12 + 464], 6
 mov rax, qword ptr [rip + .Lx24_0]
 mov qword ptr [r12 + 472], rax
 jmp xchain0_n23_α
 xchain0_n21_β:
 jmp xchain0_n26_α
.Lx24_0:
 .quad 4
xchain0_n22_α:
bb23_α:
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
  .Lbynamefn26: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn26]
 lea rsi, [r12 + 304]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 288], rax
 mov qword ptr [r12 + 296], rdx
 cmp eax, 99
 je xchain0_n19_α
 jmp xchain0_n24_α
xchain0_n22_β:
 jmp xchain0_n19_α
xchain0_n23_α:
# IR_SUBSCRIPT x[i] variable
bb24_α:
 mov rdi, qword ptr [r12 + 448]
 mov rsi, qword ptr [r12 + 456]
 mov rdx, qword ptr [r12 + 464]
 mov rcx, qword ptr [r12 + 472]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n26_α
 mov qword ptr [r12 + 480], rax
 mov qword ptr [r12 + 488], rdx
 jmp xchain0_n25_α
 xchain0_n23_β:
 jmp xchain0_n26_α
xchain0_n24_α:
# IR_LIT_STRING
bb25_α:
 mov qword ptr [r12 + 432], 1
 mov rax, qword ptr [rip + .Lx28_0]
 mov qword ptr [r12 + 440], rax
 jmp xchain0_n27_α
 xchain0_n24_β:
 jmp main_γ
.Lx28_0:
 .quad .Lx28_0_s
.Lx28_0_s:
 .string "FAIL 1110/002: array assign/read"
xchain0_n25_α:
# IR_DEREF variable -> value
bb26_α:
 mov rdi, qword ptr [r12 + 480]
 mov rsi, qword ptr [r12 + 488]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n26_α
 mov qword ptr [r12 + 496], rax
 mov qword ptr [r12 + 504], rdx
 jmp xchain0_n28_α
 xchain0_n25_β:
 jmp xchain0_n26_α
xchain0_n26_α:
# IR_VAR gva
bb27_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 544], rax
 mov qword ptr [r12 + 552], rdx
 jmp xchain0_n29_α
 xchain0_n26_β:
 jmp xchain0_n33_α
xchain0_n27_α:
# IR_ASSIGN gva
bb28_α:
 mov rax, qword ptr [r12 + 432]
 mov rdx, qword ptr [r12 + 440]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 416], rax
 mov qword ptr [r12 + 424], rdx
 jmp main_γ
 xchain0_n27_β:
 jmp main_γ
xchain0_n28_α:
# IR_LIT_STRING
bb29_α:
 mov qword ptr [r12 + 528], 1
 mov rax, qword ptr [rip + .Lx32_0]
 mov qword ptr [r12 + 536], rax
 jmp xchain0_n30_α
 xchain0_n28_β:
 jmp main_γ
.Lx32_0:
 .quad .Lx32_0_s
.Lx32_0_s:
 .string "FAIL 1110/003: OOB high should fail"
xchain0_n29_α:
# IR_LIT_INTEGER
bb30_α:
 mov qword ptr [r12 + 560], 6
 mov rax, qword ptr [rip + .Lx33_0]
 mov qword ptr [r12 + 568], rax
 jmp xchain0_n31_α
 xchain0_n29_β:
 jmp xchain0_n33_α
.Lx33_0:
 .quad 0
xchain0_n30_α:
# IR_ASSIGN gva
bb31_α:
 mov rax, qword ptr [r12 + 528]
 mov rdx, qword ptr [r12 + 536]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 512], rax
 mov qword ptr [r12 + 520], rdx
 jmp main_γ
 xchain0_n30_β:
 jmp main_γ
xchain0_n31_α:
# IR_SUBSCRIPT x[i] variable
bb32_α:
 mov rdi, qword ptr [r12 + 544]
 mov rsi, qword ptr [r12 + 552]
 mov rdx, qword ptr [r12 + 560]
 mov rcx, qword ptr [r12 + 568]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n33_α
 mov qword ptr [r12 + 576], rax
 mov qword ptr [r12 + 584], rdx
 jmp xchain0_n32_α
 xchain0_n31_β:
 jmp xchain0_n33_α
xchain0_n32_α:
# IR_DEREF variable -> value
bb33_α:
 mov rdi, qword ptr [r12 + 576]
 mov rsi, qword ptr [r12 + 584]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n33_α
 mov qword ptr [r12 + 592], rax
 mov qword ptr [r12 + 600], rdx
 jmp xchain0_n34_α
 xchain0_n32_β:
 jmp xchain0_n33_α
xchain0_n33_α:
# IR_VAR gva
bb34_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 720], rax
 mov qword ptr [r12 + 728], rdx
 jmp xchain0_n35_α
 xchain0_n33_β:
 jmp xchain0_n38_α
xchain0_n34_α:
# IR_LIT_STRING
bb35_α:
 mov qword ptr [r12 + 624], 1
 mov rax, qword ptr [rip + .Lx38_0]
 mov qword ptr [r12 + 632], rax
 jmp xchain0_n36_α
 xchain0_n34_β:
 jmp main_γ
.Lx38_0:
 .quad .Lx38_0_s
.Lx38_0_s:
 .string "FAIL 1110/004: OOB zero should fail"
xchain0_n35_α:
bb36_α:
# BOX IR_CALL prototype(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+720] -> [r12+704]
 mov rax, qword ptr [r12 + 720]
 mov qword ptr [r12 + 704], rax
 mov rax, qword ptr [r12 + 728]
 mov qword ptr [r12 + 712], rax
  .section .rodata
  .Lbynamefn40: .string "prototype"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn40]
 lea rsi, [r12 + 704]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 688], rax
 mov qword ptr [r12 + 696], rdx
 cmp eax, 99
 je xchain0_n38_α
 jmp xchain0_n37_α
xchain0_n35_β:
 jmp xchain0_n38_α
xchain0_n36_α:
# IR_ASSIGN gva
bb37_α:
 mov rax, qword ptr [r12 + 624]
 mov rdx, qword ptr [r12 + 632]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 608], rax
 mov qword ptr [r12 + 616], rdx
 jmp main_γ
 xchain0_n36_β:
 jmp main_γ
xchain0_n37_α:
# IR_LIT_STRING
bb38_α:
 mov qword ptr [r12 + 736], 1
 mov rax, qword ptr [rip + .Lx42_0]
 mov qword ptr [r12 + 744], rax
 jmp xchain0_n39_α
 xchain0_n37_β:
 jmp xchain0_n38_α
.Lx42_0:
 .quad .Lx42_0_s
.Lx42_0_s:
 .string "3"
xchain0_n38_α:
# IR_LIT_STRING
bb39_α:
 mov qword ptr [r12 + 832], 1
 mov rax, qword ptr [rip + .Lx43_0]
 mov qword ptr [r12 + 840], rax
 jmp xchain0_n40_α
 xchain0_n38_β:
 jmp xchain0_n43_α
.Lx43_0:
 .quad .Lx43_0_s
.Lx43_0_s:
 .string "3"
xchain0_n39_α:
bb40_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+688] -> [r12+656]
 mov rax, qword ptr [r12 + 688]
 mov qword ptr [r12 + 656], rax
 mov rax, qword ptr [r12 + 696]
 mov qword ptr [r12 + 664], rax
# marshal arg1 = producer-box slot [r12+736] -> [r12+672]
 mov rax, qword ptr [r12 + 736]
 mov qword ptr [r12 + 672], rax
 mov rax, qword ptr [r12 + 744]
 mov qword ptr [r12 + 680], rax
  .section .rodata
  .Lbynamefn45: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn45]
 lea rsi, [r12 + 656]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 640], rax
 mov qword ptr [r12 + 648], rdx
 cmp eax, 99
 je xchain0_n38_α
 jmp xchain0_n41_α
xchain0_n39_β:
 jmp xchain0_n38_α
xchain0_n40_α:
bb41_α:
# BOX IR_CALL array(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+832] -> [r12+816]
 mov rax, qword ptr [r12 + 832]
 mov qword ptr [r12 + 816], rax
 mov rax, qword ptr [r12 + 840]
 mov qword ptr [r12 + 824], rax
  .section .rodata
  .Lbynamefn47: .string "array"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn47]
 lea rsi, [r12 + 816]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 800], rax
 mov qword ptr [r12 + 808], rdx
 cmp eax, 99
 je xchain0_n43_α
 jmp xchain0_n42_α
xchain0_n40_β:
 jmp xchain0_n43_α
xchain0_n41_α:
# IR_LIT_STRING
bb42_α:
 mov qword ptr [r12 + 768], 1
 mov rax, qword ptr [rip + .Lx48_0]
 mov qword ptr [r12 + 776], rax
 jmp xchain0_n44_α
 xchain0_n41_β:
 jmp main_γ
.Lx48_0:
 .quad .Lx48_0_s
.Lx48_0_s:
 .string "FAIL 1110/005: prototype(array(3))=3"
xchain0_n42_α:
# IR_ASSIGN gva
bb43_α:
 mov rax, qword ptr [r12 + 800]
 mov rdx, qword ptr [r12 + 808]
 mov qword ptr [rbx + 32], rax
 mov qword ptr [rbx + 40], rdx
 mov qword ptr [r12 + 784], rax
 mov qword ptr [r12 + 792], rdx
 jmp xchain0_n43_α
 xchain0_n42_β:
 jmp xchain0_n43_α
xchain0_n43_α:
# IR_VAR gva
bb44_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 848], rax
 mov qword ptr [r12 + 856], rdx
 jmp xchain0_n45_α
 xchain0_n43_β:
 jmp xchain0_n48_α
xchain0_n44_α:
# IR_ASSIGN gva
bb45_α:
 mov rax, qword ptr [r12 + 768]
 mov rdx, qword ptr [r12 + 776]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 752], rax
 mov qword ptr [r12 + 760], rdx
 jmp main_γ
 xchain0_n44_β:
 jmp main_γ
xchain0_n45_α:
# IR_LIT_INTEGER
bb46_α:
 mov qword ptr [r12 + 864], 6
 mov rax, qword ptr [rip + .Lx52_0]
 mov qword ptr [r12 + 872], rax
 jmp xchain0_n46_α
 xchain0_n45_β:
 jmp xchain0_n48_α
.Lx52_0:
 .quad 2
xchain0_n46_α:
# IR_SUBSCRIPT x[i] variable
bb47_α:
 mov rdi, qword ptr [r12 + 848]
 mov rsi, qword ptr [r12 + 856]
 mov rdx, qword ptr [r12 + 864]
 mov rcx, qword ptr [r12 + 872]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n48_α
 mov qword ptr [r12 + 880], rax
 mov qword ptr [r12 + 888], rdx
 jmp xchain0_n47_α
 xchain0_n46_β:
 jmp xchain0_n48_α
xchain0_n47_α:
# IR_LIT_STRING
bb48_α:
 mov qword ptr [r12 + 896], 1
 mov rax, qword ptr [rip + .Lx54_0]
 mov qword ptr [r12 + 904], rax
 jmp xchain0_n49_α
 xchain0_n47_β:
 jmp xchain0_n48_α
.Lx54_0:
 .quad .Lx54_0_s
.Lx54_0_s:
 .string "x"
xchain0_n48_α:
# IR_VAR gva
bb49_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 976], rax
 mov qword ptr [r12 + 984], rdx
 jmp xchain0_n50_α
 xchain0_n48_β:
 jmp xchain0_n53_α
xchain0_n49_α:
# IR_ASSIGN_VAR write through variable
bb50_α:
 mov rdi, qword ptr [r12 + 880]
 mov rsi, qword ptr [r12 + 888]
 mov rdx, qword ptr [r12 + 896]
 mov rcx, qword ptr [r12 + 904]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n48_α
 mov qword ptr [r12 + 912], rax
 mov qword ptr [r12 + 920], rdx
 jmp xchain0_n48_α
 xchain0_n49_β:
 jmp xchain0_n48_α
xchain0_n50_α:
# IR_LIT_INTEGER
bb51_α:
 mov qword ptr [r12 + 992], 6
 mov rax, qword ptr [rip + .Lx57_0]
 mov qword ptr [r12 + 1000], rax
 jmp xchain0_n51_α
 xchain0_n50_β:
 jmp xchain0_n53_α
.Lx57_0:
 .quad 2
xchain0_n51_α:
# IR_SUBSCRIPT x[i] variable
bb52_α:
 mov rdi, qword ptr [r12 + 976]
 mov rsi, qword ptr [r12 + 984]
 mov rdx, qword ptr [r12 + 992]
 mov rcx, qword ptr [r12 + 1000]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n53_α
 mov qword ptr [r12 + 1008], rax
 mov qword ptr [r12 + 1016], rdx
 jmp xchain0_n52_α
 xchain0_n51_β:
 jmp xchain0_n53_α
xchain0_n52_α:
# IR_DEREF variable -> value
bb53_α:
 mov rdi, qword ptr [r12 + 1008]
 mov rsi, qword ptr [r12 + 1016]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n53_α
 mov qword ptr [r12 + 1024], rax
 mov qword ptr [r12 + 1032], rdx
 jmp xchain0_n54_α
 xchain0_n52_β:
 jmp xchain0_n53_α
xchain0_n53_α:
# IR_VAR gva
bb54_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 1168], rax
 mov qword ptr [r12 + 1176], rdx
 jmp xchain0_n55_α
 xchain0_n53_β:
 jmp xchain0_n58_α
xchain0_n54_α:
# IR_LIT_STRING
bb55_α:
 mov qword ptr [r12 + 1040], 1
 mov rax, qword ptr [rip + .Lx61_0]
 mov qword ptr [r12 + 1048], rax
 jmp xchain0_n56_α
 xchain0_n54_β:
 jmp xchain0_n53_α
.Lx61_0:
 .quad .Lx61_0_s
.Lx61_0_s:
 .string "x"
xchain0_n55_α:
bb56_α:
# BOX IR_CALL prototype(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+1168] -> [r12+1152]
 mov rax, qword ptr [r12 + 1168]
 mov qword ptr [r12 + 1152], rax
 mov rax, qword ptr [r12 + 1176]
 mov qword ptr [r12 + 1160], rax
  .section .rodata
  .Lbynamefn63: .string "prototype"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn63]
 lea rsi, [r12 + 1152]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1136], rax
 mov qword ptr [r12 + 1144], rdx
 cmp eax, 99
 je xchain0_n58_α
 jmp xchain0_n57_α
xchain0_n55_β:
 jmp xchain0_n58_α
xchain0_n56_α:
bb57_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+1024] -> [r12+944]
 mov rax, qword ptr [r12 + 1024]
 mov qword ptr [r12 + 944], rax
 mov rax, qword ptr [r12 + 1032]
 mov qword ptr [r12 + 952], rax
# marshal arg1 = producer-box slot [r12+1040] -> [r12+960]
 mov rax, qword ptr [r12 + 1040]
 mov qword ptr [r12 + 960], rax
 mov rax, qword ptr [r12 + 1048]
 mov qword ptr [r12 + 968], rax
  .section .rodata
  .Lbynamefn65: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn65]
 lea rsi, [r12 + 944]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 928], rax
 mov qword ptr [r12 + 936], rdx
 cmp eax, 99
 je xchain0_n53_α
 jmp xchain0_n59_α
xchain0_n56_β:
 jmp xchain0_n53_α
xchain0_n57_α:
# IR_LIT_STRING
bb58_α:
 mov qword ptr [r12 + 1184], 1
 mov rax, qword ptr [rip + .Lx66_0]
 mov qword ptr [r12 + 1192], rax
 jmp xchain0_n60_α
 xchain0_n57_β:
 jmp xchain0_n58_α
.Lx66_0:
 .quad .Lx66_0_s
.Lx66_0_s:
 .string "3"
xchain0_n58_α:
# IR_VAR gva
bb59_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 1232], rax
 mov qword ptr [r12 + 1240], rdx
 jmp xchain0_n61_α
 xchain0_n58_β:
 jmp xchain0_n67_α
xchain0_n59_α:
# IR_LIT_STRING
bb60_α:
 mov qword ptr [r12 + 1072], 1
 mov rax, qword ptr [rip + .Lx68_0]
 mov qword ptr [r12 + 1080], rax
 jmp xchain0_n62_α
 xchain0_n59_β:
 jmp main_γ
.Lx68_0:
 .quad .Lx68_0_s
.Lx68_0_s:
 .string "FAIL 1110/006: array from string dim"
xchain0_n60_α:
bb61_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+1136] -> [r12+1104]
 mov rax, qword ptr [r12 + 1136]
 mov qword ptr [r12 + 1104], rax
 mov rax, qword ptr [r12 + 1144]
 mov qword ptr [r12 + 1112], rax
# marshal arg1 = producer-box slot [r12+1184] -> [r12+1120]
 mov rax, qword ptr [r12 + 1184]
 mov qword ptr [r12 + 1120], rax
 mov rax, qword ptr [r12 + 1192]
 mov qword ptr [r12 + 1128], rax
  .section .rodata
  .Lbynamefn70: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn70]
 lea rsi, [r12 + 1104]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1088], rax
 mov qword ptr [r12 + 1096], rdx
 cmp eax, 99
 je xchain0_n58_α
 jmp xchain0_n63_α
xchain0_n60_β:
 jmp xchain0_n58_α
xchain0_n61_α:
# IR_LIT_INTEGER
bb62_α:
 mov qword ptr [r12 + 1248], 6
 mov rax, qword ptr [rip + .Lx71_0]
 mov qword ptr [r12 + 1256], rax
 jmp xchain0_n64_α
 xchain0_n61_β:
 jmp xchain0_n67_α
.Lx71_0:
 .quad 1
xchain0_n62_α:
# IR_ASSIGN gva
bb63_α:
 mov rax, qword ptr [r12 + 1072]
 mov rdx, qword ptr [r12 + 1080]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 1056], rax
 mov qword ptr [r12 + 1064], rdx
 jmp main_γ
 xchain0_n62_β:
 jmp main_γ
xchain0_n63_α:
# IR_LIT_STRING
bb64_α:
 mov qword ptr [r12 + 1216], 1
 mov rax, qword ptr [rip + .Lx73_0]
 mov qword ptr [r12 + 1224], rax
 jmp xchain0_n65_α
 xchain0_n63_β:
 jmp main_γ
.Lx73_0:
 .quad .Lx73_0_s
.Lx73_0_s:
 .string "FAIL 1110/007: prototype string-dim array"
xchain0_n64_α:
# IR_SUBSCRIPT x[i] variable
bb65_α:
 mov rdi, qword ptr [r12 + 1232]
 mov rsi, qword ptr [r12 + 1240]
 mov rdx, qword ptr [r12 + 1248]
 mov rcx, qword ptr [r12 + 1256]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n67_α
 mov qword ptr [r12 + 1264], rax
 mov qword ptr [r12 + 1272], rdx
 jmp xchain0_n66_α
 xchain0_n64_β:
 jmp xchain0_n67_α
xchain0_n65_α:
# IR_ASSIGN gva
bb66_α:
 mov rax, qword ptr [r12 + 1216]
 mov rdx, qword ptr [r12 + 1224]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 1200], rax
 mov qword ptr [r12 + 1208], rdx
 jmp main_γ
 xchain0_n65_β:
 jmp main_γ
xchain0_n66_α:
# IR_LIT_REAL
bb67_α:
 mov qword ptr [r12 + 1280], 7
 mov rax, qword ptr [rip + .Lx76_0]
 mov qword ptr [r12 + 1288], rax
 jmp xchain0_n68_α
 xchain0_n66_β:
 jmp xchain0_n67_α
.Lx76_0:
 .quad 4614253070214989087
xchain0_n67_α:
# IR_VAR gva
bb68_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 1360], rax
 mov qword ptr [r12 + 1368], rdx
 jmp xchain0_n69_α
 xchain0_n67_β:
 jmp xchain0_n72_α
xchain0_n68_α:
# IR_ASSIGN_VAR write through variable
bb69_α:
 mov rdi, qword ptr [r12 + 1264]
 mov rsi, qword ptr [r12 + 1272]
 mov rdx, qword ptr [r12 + 1280]
 mov rcx, qword ptr [r12 + 1288]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n67_α
 mov qword ptr [r12 + 1296], rax
 mov qword ptr [r12 + 1304], rdx
 jmp xchain0_n67_α
 xchain0_n68_β:
 jmp xchain0_n67_α
xchain0_n69_α:
# IR_LIT_INTEGER
bb70_α:
 mov qword ptr [r12 + 1376], 6
 mov rax, qword ptr [rip + .Lx79_0]
 mov qword ptr [r12 + 1384], rax
 jmp xchain0_n70_α
 xchain0_n69_β:
 jmp xchain0_n72_α
.Lx79_0:
 .quad 1
xchain0_n70_α:
# IR_SUBSCRIPT x[i] variable
bb71_α:
 mov rdi, qword ptr [r12 + 1360]
 mov rsi, qword ptr [r12 + 1368]
 mov rdx, qword ptr [r12 + 1376]
 mov rcx, qword ptr [r12 + 1384]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n72_α
 mov qword ptr [r12 + 1392], rax
 mov qword ptr [r12 + 1400], rdx
 jmp xchain0_n71_α
 xchain0_n70_β:
 jmp xchain0_n72_α
xchain0_n71_α:
# IR_DEREF variable -> value
bb72_α:
 mov rdi, qword ptr [r12 + 1392]
 mov rsi, qword ptr [r12 + 1400]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n72_α
 mov qword ptr [r12 + 1408], rax
 mov qword ptr [r12 + 1416], rdx
 jmp xchain0_n73_α
 xchain0_n71_β:
 jmp xchain0_n72_α
xchain0_n72_α:
# IR_VAR gva
bb73_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 1472], rax
 mov qword ptr [r12 + 1480], rdx
 jmp xchain0_n74_α
 xchain0_n72_β:
 jmp xchain0_n79_α
xchain0_n73_α:
# IR_LIT_REAL
bb74_α:
 mov qword ptr [r12 + 1424], 7
 mov rax, qword ptr [rip + .Lx83_0]
 mov qword ptr [r12 + 1432], rax
 jmp xchain0_n75_α
 xchain0_n73_β:
 jmp xchain0_n72_α
.Lx83_0:
 .quad 4614253070214989087
xchain0_n74_α:
# IR_LIT_INTEGER
bb75_α:
 mov qword ptr [r12 + 1488], 6
 mov rax, qword ptr [rip + .Lx84_0]
 mov qword ptr [r12 + 1496], rax
 jmp xchain0_n76_α
 xchain0_n74_β:
 jmp xchain0_n79_α
.Lx84_0:
 .quad 3
xchain0_n75_α:
bb76_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+1408] -> [r12+1328]
 mov rax, qword ptr [r12 + 1408]
 mov qword ptr [r12 + 1328], rax
 mov rax, qword ptr [r12 + 1416]
 mov qword ptr [r12 + 1336], rax
# marshal arg1 = producer-box slot [r12+1424] -> [r12+1344]
 mov rax, qword ptr [r12 + 1424]
 mov qword ptr [r12 + 1344], rax
 mov rax, qword ptr [r12 + 1432]
 mov qword ptr [r12 + 1352], rax
  .section .rodata
  .Lbynamefn86: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn86]
 lea rsi, [r12 + 1328]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1312], rax
 mov qword ptr [r12 + 1320], rdx
 cmp eax, 99
 je xchain0_n72_α
 jmp xchain0_n77_α
xchain0_n75_β:
 jmp xchain0_n72_α
xchain0_n76_α:
# IR_SUBSCRIPT x[i] variable
bb77_α:
 mov rdi, qword ptr [r12 + 1472]
 mov rsi, qword ptr [r12 + 1480]
 mov rdx, qword ptr [r12 + 1488]
 mov rcx, qword ptr [r12 + 1496]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n79_α
 mov qword ptr [r12 + 1504], rax
 mov qword ptr [r12 + 1512], rdx
 jmp xchain0_n78_α
 xchain0_n76_β:
 jmp xchain0_n79_α
xchain0_n77_α:
# IR_LIT_STRING
bb78_α:
 mov qword ptr [r12 + 1456], 1
 mov rax, qword ptr [rip + .Lx88_0]
 mov qword ptr [r12 + 1464], rax
 jmp xchain0_n80_α
 xchain0_n77_β:
 jmp main_γ
.Lx88_0:
 .quad .Lx88_0_s
.Lx88_0_s:
 .string "FAIL 1110/008: array stores real"
xchain0_n78_α:
# IR_LIT_STRING
bb79_α:
 mov qword ptr [r12 + 1520], 1
 mov rax, qword ptr [rip + .Lx89_0]
 mov qword ptr [r12 + 1528], rax
 jmp xchain0_n81_α
 xchain0_n78_β:
 jmp xchain0_n79_α
.Lx89_0:
 .quad .Lx89_0_s
.Lx89_0_s:
 .string "z"
xchain0_n79_α:
# IR_VAR gva
bb80_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 1600], rax
 mov qword ptr [r12 + 1608], rdx
 jmp xchain0_n82_α
 xchain0_n79_β:
 jmp xchain0_n85_α
xchain0_n80_α:
# IR_ASSIGN gva
bb81_α:
 mov rax, qword ptr [r12 + 1456]
 mov rdx, qword ptr [r12 + 1464]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 1440], rax
 mov qword ptr [r12 + 1448], rdx
 jmp main_γ
 xchain0_n80_β:
 jmp main_γ
xchain0_n81_α:
# IR_ASSIGN_VAR write through variable
bb82_α:
 mov rdi, qword ptr [r12 + 1504]
 mov rsi, qword ptr [r12 + 1512]
 mov rdx, qword ptr [r12 + 1520]
 mov rcx, qword ptr [r12 + 1528]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n79_α
 mov qword ptr [r12 + 1536], rax
 mov qword ptr [r12 + 1544], rdx
 jmp xchain0_n79_α
 xchain0_n81_β:
 jmp xchain0_n79_α
xchain0_n82_α:
# IR_LIT_INTEGER
bb83_α:
 mov qword ptr [r12 + 1616], 6
 mov rax, qword ptr [rip + .Lx93_0]
 mov qword ptr [r12 + 1624], rax
 jmp xchain0_n83_α
 xchain0_n82_β:
 jmp xchain0_n85_α
.Lx93_0:
 .quad 3
xchain0_n83_α:
# IR_SUBSCRIPT x[i] variable
bb84_α:
 mov rdi, qword ptr [r12 + 1600]
 mov rsi, qword ptr [r12 + 1608]
 mov rdx, qword ptr [r12 + 1616]
 mov rcx, qword ptr [r12 + 1624]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n85_α
 mov qword ptr [r12 + 1632], rax
 mov qword ptr [r12 + 1640], rdx
 jmp xchain0_n84_α
 xchain0_n83_β:
 jmp xchain0_n85_α
xchain0_n84_α:
# IR_DEREF variable -> value
bb85_α:
 mov rdi, qword ptr [r12 + 1632]
 mov rsi, qword ptr [r12 + 1640]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n85_α
 mov qword ptr [r12 + 1648], rax
 mov qword ptr [r12 + 1656], rdx
 jmp xchain0_n86_α
 xchain0_n84_β:
 jmp xchain0_n85_α
xchain0_n85_α:
# IR_LIT_STRING
bb86_α:
 mov qword ptr [r12 + 1728], 1
 mov rax, qword ptr [rip + .Lx96_0]
 mov qword ptr [r12 + 1736], rax
 jmp xchain0_n87_α
 xchain0_n85_β:
 jmp main_γ
.Lx96_0:
 .quad .Lx96_0_s
.Lx96_0_s:
 .string "PASS 1110_array_1d (9/9)"
xchain0_n86_α:
# IR_LIT_STRING
bb87_α:
 mov qword ptr [r12 + 1664], 1
 mov rax, qword ptr [rip + .Lx97_0]
 mov qword ptr [r12 + 1672], rax
 jmp xchain0_n88_α
 xchain0_n86_β:
 jmp xchain0_n85_α
.Lx97_0:
 .quad .Lx97_0_s
.Lx97_0_s:
 .string "z"
xchain0_n87_α:
# IR_ASSIGN gva
bb88_α:
 mov rax, qword ptr [r12 + 1728]
 mov rdx, qword ptr [r12 + 1736]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 1712], rax
 mov qword ptr [r12 + 1720], rdx
 jmp main_γ
 xchain0_n87_β:
 jmp main_γ
xchain0_n88_α:
bb89_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+1648] -> [r12+1568]
 mov rax, qword ptr [r12 + 1648]
 mov qword ptr [r12 + 1568], rax
 mov rax, qword ptr [r12 + 1656]
 mov qword ptr [r12 + 1576], rax
# marshal arg1 = producer-box slot [r12+1664] -> [r12+1584]
 mov rax, qword ptr [r12 + 1664]
 mov qword ptr [r12 + 1584], rax
 mov rax, qword ptr [r12 + 1672]
 mov qword ptr [r12 + 1592], rax
  .section .rodata
  .Lbynamefn100: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn100]
 lea rsi, [r12 + 1568]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1552], rax
 mov qword ptr [r12 + 1560], rdx
 cmp eax, 99
 je xchain0_n85_α
 jmp xchain0_n89_α
xchain0_n88_β:
 jmp xchain0_n85_α
xchain0_n89_α:
# IR_LIT_STRING
bb90_α:
 mov qword ptr [r12 + 1696], 1
 mov rax, qword ptr [rip + .Lx101_0]
 mov qword ptr [r12 + 1704], rax
 jmp xchain0_n90_α
 xchain0_n89_β:
 jmp main_γ
.Lx101_0:
 .quad .Lx101_0_s
.Lx101_0_s:
 .string "FAIL 1110/009: array stores string"
xchain0_n90_α:
# IR_ASSIGN gva
bb91_α:
 mov rax, qword ptr [r12 + 1696]
 mov rdx, qword ptr [r12 + 1704]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 1680], rax
 mov qword ptr [r12 + 1688], rdx
 jmp main_γ
 xchain0_n90_β:
 jmp main_γ
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
pop r12
ret
main_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
pop r12
ret
