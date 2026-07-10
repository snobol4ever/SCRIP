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
  call core_lib_init@PLT
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
 mov qword ptr [r12 + 1792], rax
 pop rsi
main_α_body:
# IR_LIT_INTEGER
 xchain0_n0_α:
 mov qword ptr [r12 + 112], 6
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 120], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n3_α
.Lx1_0:
 .quad 3
 xchain0_n1_α:
# BOX IR_CALL array(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+112] -> [zr+96]
 mov rax, qword ptr [r12 + 112]
 mov qword ptr [r12 + 96], rax
 mov rax, qword ptr [r12 + 120]
 mov qword ptr [r12 + 104], rax
  .section .rodata
  .Lbynamefn3: .string "array"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn3]
 lea rsi, [r12 + 96]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 80], rax
 mov qword ptr [r12 + 88], rdx
 cmp eax, 99
 je xchain0_n3_α
 jmp xchain0_n2_α
xchain0_n1_β:
 jmp xchain0_n3_α
# IR_ASSIGN gva
 xchain0_n2_α:
 mov rax, qword ptr [r12 + 80]
 mov rdx, qword ptr [r12 + 88]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 64], rax
 mov qword ptr [r12 + 72], rdx
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n3_α
# IR_VAR
 xchain0_n3_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 160], rax
 mov qword ptr [r12 + 168], rdx
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp xchain0_n5_α
# IR_LIT_INTEGER
 xchain0_n4_α:
 mov qword ptr [r12 + 176], 6
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [r12 + 184], rax
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp xchain0_n5_α
.Lx6_0:
 .quad 1
# IR_VAR
 xchain0_n5_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 256], rax
 mov qword ptr [r12 + 264], rdx
 jmp xchain0_n7_α
 xchain0_n5_β:
 jmp xchain0_n8_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n6_α:
 mov rdi, qword ptr [r12 + 160]
 mov rsi, qword ptr [r12 + 168]
 mov rdx, qword ptr [r12 + 176]
 mov rcx, qword ptr [r12 + 184]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n5_α
 mov qword ptr [r12 + 192], rax
 mov qword ptr [r12 + 200], rdx
 jmp xchain0_n9_α
 xchain0_n6_β:
 jmp xchain0_n5_α
# IR_LIT_INTEGER
 xchain0_n7_α:
 mov qword ptr [r12 + 272], 6
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [r12 + 280], rax
 jmp xchain0_n10_α
 xchain0_n7_β:
 jmp xchain0_n8_α
.Lx9_0:
 .quad 2
# IR_VAR
 xchain0_n8_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 384], rax
 mov qword ptr [r12 + 392], rdx
 jmp xchain0_n11_α
 xchain0_n8_β:
 jmp xchain0_n12_α
# IR_DEREF variable -> value
 xchain0_n9_α:
 mov rdi, qword ptr [r12 + 192]
 mov rsi, qword ptr [r12 + 200]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n5_α
 mov qword ptr [r12 + 208], rax
 mov qword ptr [r12 + 216], rdx
 jmp xchain0_n13_α
 xchain0_n9_β:
 jmp xchain0_n5_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n10_α:
 mov rdi, qword ptr [r12 + 256]
 mov rsi, qword ptr [r12 + 264]
 mov rdx, qword ptr [r12 + 272]
 mov rcx, qword ptr [r12 + 280]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n8_α
 mov qword ptr [r12 + 288], rax
 mov qword ptr [r12 + 296], rdx
 jmp xchain0_n14_α
 xchain0_n10_β:
 jmp xchain0_n8_α
# IR_LIT_INTEGER
 xchain0_n11_α:
 mov qword ptr [r12 + 400], 6
 mov rax, qword ptr [rip + .Lx13_0]
 mov qword ptr [r12 + 408], rax
 jmp xchain0_n15_α
 xchain0_n11_β:
 jmp xchain0_n12_α
.Lx13_0:
 .quad 2
# IR_VAR
 xchain0_n12_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 496], rax
 mov qword ptr [r12 + 504], rdx
 jmp xchain0_n16_α
 xchain0_n12_β:
 jmp xchain0_n17_α
 xchain0_n13_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+208] -> [zr+144]
 mov rax, qword ptr [r12 + 208]
 mov qword ptr [r12 + 144], rax
 mov rax, qword ptr [r12 + 216]
 mov qword ptr [r12 + 152], rax
  .section .rodata
  .Lbynamefn16: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn16]
 lea rsi, [r12 + 144]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 128], rax
 mov qword ptr [r12 + 136], rdx
 cmp eax, 99
 je xchain0_n5_α
 jmp xchain0_n18_α
xchain0_n13_β:
 jmp xchain0_n5_α
# IR_LIT_REAL
 xchain0_n14_α:
 mov qword ptr [r12 + 304], 7
 mov rax, qword ptr [rip + .Lx17_0]
 mov qword ptr [r12 + 312], rax
 jmp xchain0_n19_α
 xchain0_n14_β:
 jmp xchain0_n8_α
.Lx17_0:
 .quad 4616752568008179712
# IR_SUBSCRIPT x[i] variable
 xchain0_n15_α:
 mov rdi, qword ptr [r12 + 384]
 mov rsi, qword ptr [r12 + 392]
 mov rdx, qword ptr [r12 + 400]
 mov rcx, qword ptr [r12 + 408]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n12_α
 mov qword ptr [r12 + 416], rax
 mov qword ptr [r12 + 424], rdx
 jmp xchain0_n20_α
 xchain0_n15_β:
 jmp xchain0_n12_α
# IR_LIT_INTEGER
 xchain0_n16_α:
 mov qword ptr [r12 + 512], 6
 mov rax, qword ptr [rip + .Lx19_0]
 mov qword ptr [r12 + 520], rax
 jmp xchain0_n21_α
 xchain0_n16_β:
 jmp xchain0_n17_α
.Lx19_0:
 .quad 4
# IR_VAR
 xchain0_n17_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 592], rax
 mov qword ptr [r12 + 600], rdx
 jmp xchain0_n22_α
 xchain0_n17_β:
 jmp xchain0_n23_α
# IR_LIT_STRING
 xchain0_n18_α:
 mov qword ptr [r12 + 240], 1
 mov rax, qword ptr [rip + .Lx21_0]
 mov qword ptr [r12 + 248], rax
 jmp xchain0_n24_α
 xchain0_n18_β:
 jmp main_γ
.Lx21_0:
 .quad .Lx21_0_s
.Lx21_0_s:
 .string "FAIL 1110/001: array element init null"
# IR_ASSIGN_VAR
 xchain0_n19_α:
 mov rdi, qword ptr [r12 + 288]
 mov rsi, qword ptr [r12 + 296]
 mov rdx, qword ptr [r12 + 304]
 mov rcx, qword ptr [r12 + 312]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n8_α
 mov qword ptr [r12 + 320], rax
 mov qword ptr [r12 + 328], rdx
 jmp xchain0_n8_α
 xchain0_n19_β:
 jmp xchain0_n8_α
# IR_DEREF variable -> value
 xchain0_n20_α:
 mov rdi, qword ptr [r12 + 416]
 mov rsi, qword ptr [r12 + 424]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n12_α
 mov qword ptr [r12 + 432], rax
 mov qword ptr [r12 + 440], rdx
 jmp xchain0_n25_α
 xchain0_n20_β:
 jmp xchain0_n12_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n21_α:
 mov rdi, qword ptr [r12 + 496]
 mov rsi, qword ptr [r12 + 504]
 mov rdx, qword ptr [r12 + 512]
 mov rcx, qword ptr [r12 + 520]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n17_α
 mov qword ptr [r12 + 528], rax
 mov qword ptr [r12 + 536], rdx
 jmp xchain0_n26_α
 xchain0_n21_β:
 jmp xchain0_n17_α
# IR_LIT_INTEGER
 xchain0_n22_α:
 mov qword ptr [r12 + 608], 6
 mov rax, qword ptr [rip + .Lx25_0]
 mov qword ptr [r12 + 616], rax
 jmp xchain0_n27_α
 xchain0_n22_β:
 jmp xchain0_n23_α
.Lx25_0:
 .quad 0
# IR_VAR
 xchain0_n23_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 768], rax
 mov qword ptr [r12 + 776], rdx
 jmp xchain0_n28_α
 xchain0_n23_β:
 jmp xchain0_n29_α
# IR_ASSIGN gva
 xchain0_n24_α:
 mov rax, qword ptr [r12 + 240]
 mov rdx, qword ptr [r12 + 248]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 224], rax
 mov qword ptr [r12 + 232], rdx
 jmp main_γ
 xchain0_n24_β:
 jmp main_γ
# IR_LIT_REAL
 xchain0_n25_α:
 mov qword ptr [r12 + 448], 7
 mov rax, qword ptr [rip + .Lx28_0]
 mov qword ptr [r12 + 456], rax
 jmp xchain0_n30_α
 xchain0_n25_β:
 jmp xchain0_n12_α
.Lx28_0:
 .quad 4616752568008179712
# IR_DEREF variable -> value
 xchain0_n26_α:
 mov rdi, qword ptr [r12 + 528]
 mov rsi, qword ptr [r12 + 536]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n17_α
 mov qword ptr [r12 + 544], rax
 mov qword ptr [r12 + 552], rdx
 jmp xchain0_n31_α
 xchain0_n26_β:
 jmp xchain0_n17_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n27_α:
 mov rdi, qword ptr [r12 + 592]
 mov rsi, qword ptr [r12 + 600]
 mov rdx, qword ptr [r12 + 608]
 mov rcx, qword ptr [r12 + 616]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n23_α
 mov qword ptr [r12 + 624], rax
 mov qword ptr [r12 + 632], rdx
 jmp xchain0_n32_α
 xchain0_n27_β:
 jmp xchain0_n23_α
 xchain0_n28_α:
# BOX IR_CALL prototype(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+768] -> [zr+752]
 mov rax, qword ptr [r12 + 768]
 mov qword ptr [r12 + 752], rax
 mov rax, qword ptr [r12 + 776]
 mov qword ptr [r12 + 760], rax
  .section .rodata
  .Lbynamefn32: .string "prototype"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn32]
 lea rsi, [r12 + 752]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 736], rax
 mov qword ptr [r12 + 744], rdx
 cmp eax, 99
 je xchain0_n29_α
 jmp xchain0_n33_α
xchain0_n28_β:
 jmp xchain0_n29_α
# IR_LIT_STRING
 xchain0_n29_α:
 mov qword ptr [r12 + 880], 1
 mov rax, qword ptr [rip + .Lx33_0]
 mov qword ptr [r12 + 888], rax
 jmp xchain0_n34_α
 xchain0_n29_β:
 jmp xchain0_n40_α
.Lx33_0:
 .quad .Lx33_0_s
.Lx33_0_s:
 .string "3"
 xchain0_n30_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+432] -> [zr+352]
 mov rax, qword ptr [r12 + 432]
 mov qword ptr [r12 + 352], rax
 mov rax, qword ptr [r12 + 440]
 mov qword ptr [r12 + 360], rax
# marshal arg1 = producer-box slot [zr+448] -> [zr+368]
 mov rax, qword ptr [r12 + 448]
 mov qword ptr [r12 + 368], rax
 mov rax, qword ptr [r12 + 456]
 mov qword ptr [r12 + 376], rax
  .section .rodata
  .Lbynamefn35: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn35]
 lea rsi, [r12 + 352]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 336], rax
 mov qword ptr [r12 + 344], rdx
 cmp eax, 99
 je xchain0_n12_α
 jmp xchain0_n35_α
xchain0_n30_β:
 jmp xchain0_n12_α
# IR_LIT_STRING
 xchain0_n31_α:
 mov qword ptr [r12 + 576], 1
 mov rax, qword ptr [rip + .Lx36_0]
 mov qword ptr [r12 + 584], rax
 jmp xchain0_n36_α
 xchain0_n31_β:
 jmp main_γ
.Lx36_0:
 .quad .Lx36_0_s
.Lx36_0_s:
 .string "FAIL 1110/003: OOB high should fail"
# IR_DEREF variable -> value
 xchain0_n32_α:
 mov rdi, qword ptr [r12 + 624]
 mov rsi, qword ptr [r12 + 632]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n23_α
 mov qword ptr [r12 + 640], rax
 mov qword ptr [r12 + 648], rdx
 jmp xchain0_n37_α
 xchain0_n32_β:
 jmp xchain0_n23_α
# IR_LIT_STRING
 xchain0_n33_α:
 mov qword ptr [r12 + 784], 1
 mov rax, qword ptr [rip + .Lx38_0]
 mov qword ptr [r12 + 792], rax
 jmp xchain0_n38_α
 xchain0_n33_β:
 jmp xchain0_n29_α
.Lx38_0:
 .quad .Lx38_0_s
.Lx38_0_s:
 .string "3"
 xchain0_n34_α:
# BOX IR_CALL array(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+880] -> [zr+864]
 mov rax, qword ptr [r12 + 880]
 mov qword ptr [r12 + 864], rax
 mov rax, qword ptr [r12 + 888]
 mov qword ptr [r12 + 872], rax
  .section .rodata
  .Lbynamefn40: .string "array"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn40]
 lea rsi, [r12 + 864]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 848], rax
 mov qword ptr [r12 + 856], rdx
 cmp eax, 99
 je xchain0_n40_α
 jmp xchain0_n39_α
xchain0_n34_β:
 jmp xchain0_n40_α
# IR_LIT_STRING
 xchain0_n35_α:
 mov qword ptr [r12 + 480], 1
 mov rax, qword ptr [rip + .Lx41_0]
 mov qword ptr [r12 + 488], rax
 jmp xchain0_n41_α
 xchain0_n35_β:
 jmp main_γ
.Lx41_0:
 .quad .Lx41_0_s
.Lx41_0_s:
 .string "FAIL 1110/002: array assign/read"
# IR_ASSIGN gva
 xchain0_n36_α:
 mov rax, qword ptr [r12 + 576]
 mov rdx, qword ptr [r12 + 584]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 560], rax
 mov qword ptr [r12 + 568], rdx
 jmp main_γ
 xchain0_n36_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n37_α:
 mov qword ptr [r12 + 672], 1
 mov rax, qword ptr [rip + .Lx43_0]
 mov qword ptr [r12 + 680], rax
 jmp xchain0_n42_α
 xchain0_n37_β:
 jmp main_γ
.Lx43_0:
 .quad .Lx43_0_s
.Lx43_0_s:
 .string "FAIL 1110/004: OOB zero should fail"
 xchain0_n38_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+736] -> [zr+704]
 mov rax, qword ptr [r12 + 736]
 mov qword ptr [r12 + 704], rax
 mov rax, qword ptr [r12 + 744]
 mov qword ptr [r12 + 712], rax
# marshal arg1 = producer-box slot [zr+784] -> [zr+720]
 mov rax, qword ptr [r12 + 784]
 mov qword ptr [r12 + 720], rax
 mov rax, qword ptr [r12 + 792]
 mov qword ptr [r12 + 728], rax
  .section .rodata
  .Lbynamefn45: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn45]
 lea rsi, [r12 + 704]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 688], rax
 mov qword ptr [r12 + 696], rdx
 cmp eax, 99
 je xchain0_n29_α
 jmp xchain0_n43_α
xchain0_n38_β:
 jmp xchain0_n29_α
# IR_ASSIGN gva
 xchain0_n39_α:
 mov rax, qword ptr [r12 + 848]
 mov rdx, qword ptr [r12 + 856]
 mov qword ptr [rbx + 32], rax
 mov qword ptr [rbx + 40], rdx
 mov qword ptr [r12 + 832], rax
 mov qword ptr [r12 + 840], rdx
 jmp xchain0_n40_α
 xchain0_n39_β:
 jmp xchain0_n40_α
# IR_VAR
 xchain0_n40_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 896], rax
 mov qword ptr [r12 + 904], rdx
 jmp xchain0_n44_α
 xchain0_n40_β:
 jmp xchain0_n45_α
# IR_ASSIGN gva
 xchain0_n41_α:
 mov rax, qword ptr [r12 + 480]
 mov rdx, qword ptr [r12 + 488]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 464], rax
 mov qword ptr [r12 + 472], rdx
 jmp main_γ
 xchain0_n41_β:
 jmp main_γ
# IR_ASSIGN gva
 xchain0_n42_α:
 mov rax, qword ptr [r12 + 672]
 mov rdx, qword ptr [r12 + 680]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 656], rax
 mov qword ptr [r12 + 664], rdx
 jmp main_γ
 xchain0_n42_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n43_α:
 mov qword ptr [r12 + 816], 1
 mov rax, qword ptr [rip + .Lx50_0]
 mov qword ptr [r12 + 824], rax
 jmp xchain0_n46_α
 xchain0_n43_β:
 jmp main_γ
.Lx50_0:
 .quad .Lx50_0_s
.Lx50_0_s:
 .string "FAIL 1110/005: prototype(array(3))=3"
# IR_LIT_INTEGER
 xchain0_n44_α:
 mov qword ptr [r12 + 912], 6
 mov rax, qword ptr [rip + .Lx51_0]
 mov qword ptr [r12 + 920], rax
 jmp xchain0_n47_α
 xchain0_n44_β:
 jmp xchain0_n45_α
.Lx51_0:
 .quad 2
# IR_VAR
 xchain0_n45_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 1024], rax
 mov qword ptr [r12 + 1032], rdx
 jmp xchain0_n48_α
 xchain0_n45_β:
 jmp xchain0_n49_α
# IR_ASSIGN gva
 xchain0_n46_α:
 mov rax, qword ptr [r12 + 816]
 mov rdx, qword ptr [r12 + 824]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 800], rax
 mov qword ptr [r12 + 808], rdx
 jmp main_γ
 xchain0_n46_β:
 jmp main_γ
# IR_SUBSCRIPT x[i] variable
 xchain0_n47_α:
 mov rdi, qword ptr [r12 + 896]
 mov rsi, qword ptr [r12 + 904]
 mov rdx, qword ptr [r12 + 912]
 mov rcx, qword ptr [r12 + 920]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n45_α
 mov qword ptr [r12 + 928], rax
 mov qword ptr [r12 + 936], rdx
 jmp xchain0_n50_α
 xchain0_n47_β:
 jmp xchain0_n45_α
# IR_LIT_INTEGER
 xchain0_n48_α:
 mov qword ptr [r12 + 1040], 6
 mov rax, qword ptr [rip + .Lx55_0]
 mov qword ptr [r12 + 1048], rax
 jmp xchain0_n51_α
 xchain0_n48_β:
 jmp xchain0_n49_α
.Lx55_0:
 .quad 2
# IR_VAR
 xchain0_n49_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 1216], rax
 mov qword ptr [r12 + 1224], rdx
 jmp xchain0_n52_α
 xchain0_n49_β:
 jmp xchain0_n53_α
# IR_LIT_STRING
 xchain0_n50_α:
 mov qword ptr [r12 + 944], 1
 mov rax, qword ptr [rip + .Lx57_0]
 mov qword ptr [r12 + 952], rax
 jmp xchain0_n54_α
 xchain0_n50_β:
 jmp xchain0_n45_α
.Lx57_0:
 .quad .Lx57_0_s
.Lx57_0_s:
 .string "x"
# IR_SUBSCRIPT x[i] variable
 xchain0_n51_α:
 mov rdi, qword ptr [r12 + 1024]
 mov rsi, qword ptr [r12 + 1032]
 mov rdx, qword ptr [r12 + 1040]
 mov rcx, qword ptr [r12 + 1048]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n49_α
 mov qword ptr [r12 + 1056], rax
 mov qword ptr [r12 + 1064], rdx
 jmp xchain0_n55_α
 xchain0_n51_β:
 jmp xchain0_n49_α
 xchain0_n52_α:
# BOX IR_CALL prototype(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1216] -> [zr+1200]
 mov rax, qword ptr [r12 + 1216]
 mov qword ptr [r12 + 1200], rax
 mov rax, qword ptr [r12 + 1224]
 mov qword ptr [r12 + 1208], rax
  .section .rodata
  .Lbynamefn60: .string "prototype"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn60]
 lea rsi, [r12 + 1200]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1184], rax
 mov qword ptr [r12 + 1192], rdx
 cmp eax, 99
 je xchain0_n53_α
 jmp xchain0_n56_α
xchain0_n52_β:
 jmp xchain0_n53_α
# IR_VAR
 xchain0_n53_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 1280], rax
 mov qword ptr [r12 + 1288], rdx
 jmp xchain0_n57_α
 xchain0_n53_β:
 jmp xchain0_n58_α
# IR_ASSIGN_VAR
 xchain0_n54_α:
 mov rdi, qword ptr [r12 + 928]
 mov rsi, qword ptr [r12 + 936]
 mov rdx, qword ptr [r12 + 944]
 mov rcx, qword ptr [r12 + 952]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n45_α
 mov qword ptr [r12 + 960], rax
 mov qword ptr [r12 + 968], rdx
 jmp xchain0_n45_α
 xchain0_n54_β:
 jmp xchain0_n45_α
# IR_DEREF variable -> value
 xchain0_n55_α:
 mov rdi, qword ptr [r12 + 1056]
 mov rsi, qword ptr [r12 + 1064]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n49_α
 mov qword ptr [r12 + 1072], rax
 mov qword ptr [r12 + 1080], rdx
 jmp xchain0_n59_α
 xchain0_n55_β:
 jmp xchain0_n49_α
# IR_LIT_STRING
 xchain0_n56_α:
 mov qword ptr [r12 + 1232], 1
 mov rax, qword ptr [rip + .Lx64_0]
 mov qword ptr [r12 + 1240], rax
 jmp xchain0_n60_α
 xchain0_n56_β:
 jmp xchain0_n53_α
.Lx64_0:
 .quad .Lx64_0_s
.Lx64_0_s:
 .string "3"
# IR_LIT_INTEGER
 xchain0_n57_α:
 mov qword ptr [r12 + 1296], 6
 mov rax, qword ptr [rip + .Lx65_0]
 mov qword ptr [r12 + 1304], rax
 jmp xchain0_n61_α
 xchain0_n57_β:
 jmp xchain0_n58_α
.Lx65_0:
 .quad 1
# IR_VAR
 xchain0_n58_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 1408], rax
 mov qword ptr [r12 + 1416], rdx
 jmp xchain0_n62_α
 xchain0_n58_β:
 jmp xchain0_n63_α
# IR_LIT_STRING
 xchain0_n59_α:
 mov qword ptr [r12 + 1088], 1
 mov rax, qword ptr [rip + .Lx67_0]
 mov qword ptr [r12 + 1096], rax
 jmp xchain0_n64_α
 xchain0_n59_β:
 jmp xchain0_n49_α
.Lx67_0:
 .quad .Lx67_0_s
.Lx67_0_s:
 .string "x"
 xchain0_n60_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1184] -> [zr+1152]
 mov rax, qword ptr [r12 + 1184]
 mov qword ptr [r12 + 1152], rax
 mov rax, qword ptr [r12 + 1192]
 mov qword ptr [r12 + 1160], rax
# marshal arg1 = producer-box slot [zr+1232] -> [zr+1168]
 mov rax, qword ptr [r12 + 1232]
 mov qword ptr [r12 + 1168], rax
 mov rax, qword ptr [r12 + 1240]
 mov qword ptr [r12 + 1176], rax
  .section .rodata
  .Lbynamefn69: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn69]
 lea rsi, [r12 + 1152]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1136], rax
 mov qword ptr [r12 + 1144], rdx
 cmp eax, 99
 je xchain0_n53_α
 jmp xchain0_n65_α
xchain0_n60_β:
 jmp xchain0_n53_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n61_α:
 mov rdi, qword ptr [r12 + 1280]
 mov rsi, qword ptr [r12 + 1288]
 mov rdx, qword ptr [r12 + 1296]
 mov rcx, qword ptr [r12 + 1304]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n58_α
 mov qword ptr [r12 + 1312], rax
 mov qword ptr [r12 + 1320], rdx
 jmp xchain0_n66_α
 xchain0_n61_β:
 jmp xchain0_n58_α
# IR_LIT_INTEGER
 xchain0_n62_α:
 mov qword ptr [r12 + 1424], 6
 mov rax, qword ptr [rip + .Lx71_0]
 mov qword ptr [r12 + 1432], rax
 jmp xchain0_n67_α
 xchain0_n62_β:
 jmp xchain0_n63_α
.Lx71_0:
 .quad 1
# IR_VAR
 xchain0_n63_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 1520], rax
 mov qword ptr [r12 + 1528], rdx
 jmp xchain0_n68_α
 xchain0_n63_β:
 jmp xchain0_n69_α
 xchain0_n64_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1072] -> [zr+992]
 mov rax, qword ptr [r12 + 1072]
 mov qword ptr [r12 + 992], rax
 mov rax, qword ptr [r12 + 1080]
 mov qword ptr [r12 + 1000], rax
# marshal arg1 = producer-box slot [zr+1088] -> [zr+1008]
 mov rax, qword ptr [r12 + 1088]
 mov qword ptr [r12 + 1008], rax
 mov rax, qword ptr [r12 + 1096]
 mov qword ptr [r12 + 1016], rax
  .section .rodata
  .Lbynamefn74: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn74]
 lea rsi, [r12 + 992]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 976], rax
 mov qword ptr [r12 + 984], rdx
 cmp eax, 99
 je xchain0_n49_α
 jmp xchain0_n70_α
xchain0_n64_β:
 jmp xchain0_n49_α
# IR_LIT_STRING
 xchain0_n65_α:
 mov qword ptr [r12 + 1264], 1
 mov rax, qword ptr [rip + .Lx75_0]
 mov qword ptr [r12 + 1272], rax
 jmp xchain0_n71_α
 xchain0_n65_β:
 jmp main_γ
.Lx75_0:
 .quad .Lx75_0_s
.Lx75_0_s:
 .string "FAIL 1110/007: prototype string-dim array"
# IR_LIT_REAL
 xchain0_n66_α:
 mov qword ptr [r12 + 1328], 7
 mov rax, qword ptr [rip + .Lx76_0]
 mov qword ptr [r12 + 1336], rax
 jmp xchain0_n72_α
 xchain0_n66_β:
 jmp xchain0_n58_α
.Lx76_0:
 .quad 4614253070214989087
# IR_SUBSCRIPT x[i] variable
 xchain0_n67_α:
 mov rdi, qword ptr [r12 + 1408]
 mov rsi, qword ptr [r12 + 1416]
 mov rdx, qword ptr [r12 + 1424]
 mov rcx, qword ptr [r12 + 1432]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n63_α
 mov qword ptr [r12 + 1440], rax
 mov qword ptr [r12 + 1448], rdx
 jmp xchain0_n73_α
 xchain0_n67_β:
 jmp xchain0_n63_α
# IR_LIT_INTEGER
 xchain0_n68_α:
 mov qword ptr [r12 + 1536], 6
 mov rax, qword ptr [rip + .Lx78_0]
 mov qword ptr [r12 + 1544], rax
 jmp xchain0_n74_α
 xchain0_n68_β:
 jmp xchain0_n69_α
.Lx78_0:
 .quad 3
# IR_VAR
 xchain0_n69_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 1648], rax
 mov qword ptr [r12 + 1656], rdx
 jmp xchain0_n75_α
 xchain0_n69_β:
 jmp xchain0_n76_α
# IR_LIT_STRING
 xchain0_n70_α:
 mov qword ptr [r12 + 1120], 1
 mov rax, qword ptr [rip + .Lx80_0]
 mov qword ptr [r12 + 1128], rax
 jmp xchain0_n77_α
 xchain0_n70_β:
 jmp main_γ
.Lx80_0:
 .quad .Lx80_0_s
.Lx80_0_s:
 .string "FAIL 1110/006: array from string dim"
# IR_ASSIGN gva
 xchain0_n71_α:
 mov rax, qword ptr [r12 + 1264]
 mov rdx, qword ptr [r12 + 1272]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 1248], rax
 mov qword ptr [r12 + 1256], rdx
 jmp main_γ
 xchain0_n71_β:
 jmp main_γ
# IR_ASSIGN_VAR
 xchain0_n72_α:
 mov rdi, qword ptr [r12 + 1312]
 mov rsi, qword ptr [r12 + 1320]
 mov rdx, qword ptr [r12 + 1328]
 mov rcx, qword ptr [r12 + 1336]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n58_α
 mov qword ptr [r12 + 1344], rax
 mov qword ptr [r12 + 1352], rdx
 jmp xchain0_n58_α
 xchain0_n72_β:
 jmp xchain0_n58_α
# IR_DEREF variable -> value
 xchain0_n73_α:
 mov rdi, qword ptr [r12 + 1440]
 mov rsi, qword ptr [r12 + 1448]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n63_α
 mov qword ptr [r12 + 1456], rax
 mov qword ptr [r12 + 1464], rdx
 jmp xchain0_n78_α
 xchain0_n73_β:
 jmp xchain0_n63_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n74_α:
 mov rdi, qword ptr [r12 + 1520]
 mov rsi, qword ptr [r12 + 1528]
 mov rdx, qword ptr [r12 + 1536]
 mov rcx, qword ptr [r12 + 1544]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n69_α
 mov qword ptr [r12 + 1552], rax
 mov qword ptr [r12 + 1560], rdx
 jmp xchain0_n79_α
 xchain0_n74_β:
 jmp xchain0_n69_α
# IR_LIT_INTEGER
 xchain0_n75_α:
 mov qword ptr [r12 + 1664], 6
 mov rax, qword ptr [rip + .Lx85_0]
 mov qword ptr [r12 + 1672], rax
 jmp xchain0_n80_α
 xchain0_n75_β:
 jmp xchain0_n76_α
.Lx85_0:
 .quad 3
# IR_LIT_STRING
 xchain0_n76_α:
 mov qword ptr [r12 + 1776], 1
 mov rax, qword ptr [rip + .Lx86_0]
 mov qword ptr [r12 + 1784], rax
 jmp xchain0_n81_α
 xchain0_n76_β:
 jmp main_γ
.Lx86_0:
 .quad .Lx86_0_s
.Lx86_0_s:
 .string "PASS 1110_array_1d (9/9)"
# IR_ASSIGN gva
 xchain0_n77_α:
 mov rax, qword ptr [r12 + 1120]
 mov rdx, qword ptr [r12 + 1128]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 1104], rax
 mov qword ptr [r12 + 1112], rdx
 jmp main_γ
 xchain0_n77_β:
 jmp main_γ
# IR_LIT_REAL
 xchain0_n78_α:
 mov qword ptr [r12 + 1472], 7
 mov rax, qword ptr [rip + .Lx88_0]
 mov qword ptr [r12 + 1480], rax
 jmp xchain0_n82_α
 xchain0_n78_β:
 jmp xchain0_n63_α
.Lx88_0:
 .quad 4614253070214989087
# IR_LIT_STRING
 xchain0_n79_α:
 mov qword ptr [r12 + 1568], 1
 mov rax, qword ptr [rip + .Lx89_0]
 mov qword ptr [r12 + 1576], rax
 jmp xchain0_n83_α
 xchain0_n79_β:
 jmp xchain0_n69_α
.Lx89_0:
 .quad .Lx89_0_s
.Lx89_0_s:
 .string "z"
# IR_SUBSCRIPT x[i] variable
 xchain0_n80_α:
 mov rdi, qword ptr [r12 + 1648]
 mov rsi, qword ptr [r12 + 1656]
 mov rdx, qword ptr [r12 + 1664]
 mov rcx, qword ptr [r12 + 1672]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n76_α
 mov qword ptr [r12 + 1680], rax
 mov qword ptr [r12 + 1688], rdx
 jmp xchain0_n84_α
 xchain0_n80_β:
 jmp xchain0_n76_α
# IR_ASSIGN gva
 xchain0_n81_α:
 mov rax, qword ptr [r12 + 1776]
 mov rdx, qword ptr [r12 + 1784]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 1760], rax
 mov qword ptr [r12 + 1768], rdx
 jmp main_γ
 xchain0_n81_β:
 jmp main_γ
 xchain0_n82_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1456] -> [zr+1376]
 mov rax, qword ptr [r12 + 1456]
 mov qword ptr [r12 + 1376], rax
 mov rax, qword ptr [r12 + 1464]
 mov qword ptr [r12 + 1384], rax
# marshal arg1 = producer-box slot [zr+1472] -> [zr+1392]
 mov rax, qword ptr [r12 + 1472]
 mov qword ptr [r12 + 1392], rax
 mov rax, qword ptr [r12 + 1480]
 mov qword ptr [r12 + 1400], rax
  .section .rodata
  .Lbynamefn93: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn93]
 lea rsi, [r12 + 1376]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1360], rax
 mov qword ptr [r12 + 1368], rdx
 cmp eax, 99
 je xchain0_n63_α
 jmp xchain0_n85_α
xchain0_n82_β:
 jmp xchain0_n63_α
# IR_ASSIGN_VAR
 xchain0_n83_α:
 mov rdi, qword ptr [r12 + 1552]
 mov rsi, qword ptr [r12 + 1560]
 mov rdx, qword ptr [r12 + 1568]
 mov rcx, qword ptr [r12 + 1576]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n69_α
 mov qword ptr [r12 + 1584], rax
 mov qword ptr [r12 + 1592], rdx
 jmp xchain0_n69_α
 xchain0_n83_β:
 jmp xchain0_n69_α
# IR_DEREF variable -> value
 xchain0_n84_α:
 mov rdi, qword ptr [r12 + 1680]
 mov rsi, qword ptr [r12 + 1688]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n76_α
 mov qword ptr [r12 + 1696], rax
 mov qword ptr [r12 + 1704], rdx
 jmp xchain0_n86_α
 xchain0_n84_β:
 jmp xchain0_n76_α
# IR_LIT_STRING
 xchain0_n85_α:
 mov qword ptr [r12 + 1504], 1
 mov rax, qword ptr [rip + .Lx96_0]
 mov qword ptr [r12 + 1512], rax
 jmp xchain0_n87_α
 xchain0_n85_β:
 jmp main_γ
.Lx96_0:
 .quad .Lx96_0_s
.Lx96_0_s:
 .string "FAIL 1110/008: array stores real"
# IR_LIT_STRING
 xchain0_n86_α:
 mov qword ptr [r12 + 1712], 1
 mov rax, qword ptr [rip + .Lx97_0]
 mov qword ptr [r12 + 1720], rax
 jmp xchain0_n88_α
 xchain0_n86_β:
 jmp xchain0_n76_α
.Lx97_0:
 .quad .Lx97_0_s
.Lx97_0_s:
 .string "z"
# IR_ASSIGN gva
 xchain0_n87_α:
 mov rax, qword ptr [r12 + 1504]
 mov rdx, qword ptr [r12 + 1512]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 1488], rax
 mov qword ptr [r12 + 1496], rdx
 jmp main_γ
 xchain0_n87_β:
 jmp main_γ
 xchain0_n88_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1696] -> [zr+1616]
 mov rax, qword ptr [r12 + 1696]
 mov qword ptr [r12 + 1616], rax
 mov rax, qword ptr [r12 + 1704]
 mov qword ptr [r12 + 1624], rax
# marshal arg1 = producer-box slot [zr+1712] -> [zr+1632]
 mov rax, qword ptr [r12 + 1712]
 mov qword ptr [r12 + 1632], rax
 mov rax, qword ptr [r12 + 1720]
 mov qword ptr [r12 + 1640], rax
  .section .rodata
  .Lbynamefn100: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn100]
 lea rsi, [r12 + 1616]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1600], rax
 mov qword ptr [r12 + 1608], rdx
 cmp eax, 99
 je xchain0_n76_α
 jmp xchain0_n89_α
xchain0_n88_β:
 jmp xchain0_n76_α
# IR_LIT_STRING
 xchain0_n89_α:
 mov qword ptr [r12 + 1744], 1
 mov rax, qword ptr [rip + .Lx101_0]
 mov qword ptr [r12 + 1752], rax
 jmp xchain0_n90_α
 xchain0_n89_β:
 jmp main_γ
.Lx101_0:
 .quad .Lx101_0_s
.Lx101_0_s:
 .string "FAIL 1110/009: array stores string"
# IR_ASSIGN gva
 xchain0_n90_α:
 mov rax, qword ptr [r12 + 1744]
 mov rdx, qword ptr [r12 + 1752]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 1728], rax
 mov qword ptr [r12 + 1736], rdx
 jmp main_γ
 xchain0_n90_β:
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
 mov rdi, qword ptr [r12 + 1792]
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
