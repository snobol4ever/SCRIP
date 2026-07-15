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
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
  lea rdi, [rip + __gva_names]
  lea rsi, [rip + __gva]
  mov edx, 3
  call gva_register@PLT
  mov rbx, rax
  xor esi, esi
  call main_α
  xor eax, eax
  add rsp, 24
  ret
main_α:
#=======================================================================================================================
    .global main_α
    .global main_β
    .global main_γ
    .global main_ω
  sub rsp, 65544
  mov rdi, rsp
  mov ecx, 65544
  xor eax, eax
  rep stosb
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  mov qword ptr [rsp + 2920], rsp
main_α_body:
# IR_LIT_INTEGER
 xchain0_n0_α:
 mov qword ptr [rsp + 160], 6
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [rsp + 168], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n3_α
.Lx1_0:
 .quad 3
 xchain0_n1_α:
# BOX CALL array(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+160] -> [zr+128]
 mov rax, qword ptr [rsp + 160]
 mov qword ptr [rsp + 128], rax
 mov rax, qword ptr [rsp + 168]
 mov qword ptr [rsp + 136], rax
  .section .rodata
  .Lbynamefn2: .string "array"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn2]
 lea rsi, [rsp + 128]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rsp + 112], rax
 mov qword ptr [rsp + 120], rdx
 cmp eax, 99
 je xchain0_n3_α
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n3_α
# IR_ASSIGN gva
 xchain0_n2_α:
 mov rax, qword ptr [rsp + 112]
 mov rdx, qword ptr [rsp + 120]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [rsp + 96], rax
 mov qword ptr [rsp + 104], rdx
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n3_α
# IR_VAR
 xchain0_n3_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [rsp + 240], rax
 mov qword ptr [rsp + 248], rdx
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp xchain0_n5_α
# IR_LIT_INTEGER
 xchain0_n4_α:
 mov qword ptr [rsp + 272], 6
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [rsp + 280], rax
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp xchain0_n5_α
.Lx5_0:
 .quad 1
# IR_VAR
 xchain0_n5_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [rsp + 400], rax
 mov qword ptr [rsp + 408], rdx
 jmp xchain0_n7_α
 xchain0_n5_β:
 jmp xchain0_n8_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n6_α:
 mov rdi, qword ptr [rsp + 240]
 mov rsi, qword ptr [rsp + 248]
 mov rdx, qword ptr [rsp + 272]
 mov rcx, qword ptr [rsp + 280]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n5_α
 mov qword ptr [rsp + 304], rax
 mov qword ptr [rsp + 312], rdx
 jmp xchain0_n9_α
 xchain0_n6_β:
 jmp xchain0_n5_α
# IR_LIT_INTEGER
 xchain0_n7_α:
 mov qword ptr [rsp + 432], 6
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [rsp + 440], rax
 jmp xchain0_n10_α
 xchain0_n7_β:
 jmp xchain0_n8_α
.Lx8_0:
 .quad 2
# IR_VAR
 xchain0_n8_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [rsp + 608], rax
 mov qword ptr [rsp + 616], rdx
 jmp xchain0_n11_α
 xchain0_n8_β:
 jmp xchain0_n12_α
# IR_DEREF variable -> value
 xchain0_n9_α:
 mov rdi, qword ptr [rsp + 304]
 mov rsi, qword ptr [rsp + 312]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n5_α
 mov qword ptr [rsp + 336], rax
 mov qword ptr [rsp + 344], rdx
 jmp xchain0_n13_α
 xchain0_n9_β:
 jmp xchain0_n5_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n10_α:
 mov rdi, qword ptr [rsp + 400]
 mov rsi, qword ptr [rsp + 408]
 mov rdx, qword ptr [rsp + 432]
 mov rcx, qword ptr [rsp + 440]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n8_α
 mov qword ptr [rsp + 464], rax
 mov qword ptr [rsp + 472], rdx
 jmp xchain0_n14_α
 xchain0_n10_β:
 jmp xchain0_n8_α
# IR_LIT_INTEGER
 xchain0_n11_α:
 mov qword ptr [rsp + 640], 6
 mov rax, qword ptr [rip + .Lx12_0]
 mov qword ptr [rsp + 648], rax
 jmp xchain0_n15_α
 xchain0_n11_β:
 jmp xchain0_n12_α
.Lx12_0:
 .quad 2
# IR_VAR
 xchain0_n12_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [rsp + 800], rax
 mov qword ptr [rsp + 808], rdx
 jmp xchain0_n16_α
 xchain0_n12_β:
 jmp xchain0_n17_α
 xchain0_n13_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+336] -> [zr+208]
 mov rax, qword ptr [rsp + 336]
 mov qword ptr [rsp + 208], rax
 mov rax, qword ptr [rsp + 344]
 mov qword ptr [rsp + 216], rax
  .section .rodata
  .Lbynamefn14: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn14]
 lea rsi, [rsp + 208]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rsp + 192], rax
 mov qword ptr [rsp + 200], rdx
 cmp eax, 99
 je xchain0_n5_α
 jmp xchain0_n18_α
 xchain0_n13_β:
 jmp xchain0_n5_α
# IR_LIT_REAL
 xchain0_n14_α:
 mov qword ptr [rsp + 496], 7
 mov rax, qword ptr [rip + .Lx15_0]
 mov qword ptr [rsp + 504], rax
 jmp xchain0_n19_α
 xchain0_n14_β:
 jmp xchain0_n8_α
.Lx15_0:
 .quad 4616752568008179712
# IR_SUBSCRIPT x[i] variable
 xchain0_n15_α:
 mov rdi, qword ptr [rsp + 608]
 mov rsi, qword ptr [rsp + 616]
 mov rdx, qword ptr [rsp + 640]
 mov rcx, qword ptr [rsp + 648]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n12_α
 mov qword ptr [rsp + 672], rax
 mov qword ptr [rsp + 680], rdx
 jmp xchain0_n20_α
 xchain0_n15_β:
 jmp xchain0_n12_α
# IR_LIT_INTEGER
 xchain0_n16_α:
 mov qword ptr [rsp + 832], 6
 mov rax, qword ptr [rip + .Lx17_0]
 mov qword ptr [rsp + 840], rax
 jmp xchain0_n21_α
 xchain0_n16_β:
 jmp xchain0_n17_α
.Lx17_0:
 .quad 4
# IR_VAR
 xchain0_n17_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [rsp + 960], rax
 mov qword ptr [rsp + 968], rdx
 jmp xchain0_n22_α
 xchain0_n17_β:
 jmp xchain0_n23_α
# IR_LIT_STRING
 xchain0_n18_α:
 mov qword ptr [rsp + 368], 1
 mov rax, qword ptr [rip + .Lx19_0]
 mov qword ptr [rsp + 376], rax
 jmp xchain0_n24_α
 xchain0_n18_β:
 jmp main_γ
.Lx19_0:
 .quad .Lx19_0_s
.Lx19_0_s:
 .string "FAIL 1110/001: array element init null"
# IR_ASSIGN_VAR
 xchain0_n19_α:
 mov rdi, qword ptr [rsp + 464]
 mov rsi, qword ptr [rsp + 472]
 mov rdx, qword ptr [rsp + 496]
 mov rcx, qword ptr [rsp + 504]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n8_α
 mov qword ptr [rsp + 528], rax
 mov qword ptr [rsp + 536], rdx
 jmp xchain0_n8_α
 xchain0_n19_β:
 jmp xchain0_n8_α
# IR_DEREF variable -> value
 xchain0_n20_α:
 mov rdi, qword ptr [rsp + 672]
 mov rsi, qword ptr [rsp + 680]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n12_α
 mov qword ptr [rsp + 704], rax
 mov qword ptr [rsp + 712], rdx
 jmp xchain0_n25_α
 xchain0_n20_β:
 jmp xchain0_n12_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n21_α:
 mov rdi, qword ptr [rsp + 800]
 mov rsi, qword ptr [rsp + 808]
 mov rdx, qword ptr [rsp + 832]
 mov rcx, qword ptr [rsp + 840]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n17_α
 mov qword ptr [rsp + 864], rax
 mov qword ptr [rsp + 872], rdx
 jmp xchain0_n26_α
 xchain0_n21_β:
 jmp xchain0_n17_α
# IR_LIT_INTEGER
 xchain0_n22_α:
 mov qword ptr [rsp + 992], 6
 mov rax, qword ptr [rip + .Lx23_0]
 mov qword ptr [rsp + 1000], rax
 jmp xchain0_n27_α
 xchain0_n22_β:
 jmp xchain0_n23_α
.Lx23_0:
 .quad 0
# IR_VAR
 xchain0_n23_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [rsp + 1232], rax
 mov qword ptr [rsp + 1240], rdx
 jmp xchain0_n28_α
 xchain0_n23_β:
 jmp xchain0_n29_α
# IR_ASSIGN gva
 xchain0_n24_α:
 mov rax, qword ptr [rsp + 368]
 mov rdx, qword ptr [rsp + 376]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [rsp + 352], rax
 mov qword ptr [rsp + 360], rdx
 jmp main_γ
 xchain0_n24_β:
 jmp main_γ
# IR_LIT_REAL
 xchain0_n25_α:
 mov qword ptr [rsp + 720], 7
 mov rax, qword ptr [rip + .Lx26_0]
 mov qword ptr [rsp + 728], rax
 jmp xchain0_n30_α
 xchain0_n25_β:
 jmp xchain0_n12_α
.Lx26_0:
 .quad 4616752568008179712
# IR_DEREF variable -> value
 xchain0_n26_α:
 mov rdi, qword ptr [rsp + 864]
 mov rsi, qword ptr [rsp + 872]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n17_α
 mov qword ptr [rsp + 896], rax
 mov qword ptr [rsp + 904], rdx
 jmp xchain0_n31_α
 xchain0_n26_β:
 jmp xchain0_n17_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n27_α:
 mov rdi, qword ptr [rsp + 960]
 mov rsi, qword ptr [rsp + 968]
 mov rdx, qword ptr [rsp + 992]
 mov rcx, qword ptr [rsp + 1000]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n23_α
 mov qword ptr [rsp + 1024], rax
 mov qword ptr [rsp + 1032], rdx
 jmp xchain0_n32_α
 xchain0_n27_β:
 jmp xchain0_n23_α
 xchain0_n28_α:
# BOX CALL prototype(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1232] -> [zr+1200]
 mov rax, qword ptr [rsp + 1232]
 mov qword ptr [rsp + 1200], rax
 mov rax, qword ptr [rsp + 1240]
 mov qword ptr [rsp + 1208], rax
  .section .rodata
  .Lbynamefn29: .string "prototype"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn29]
 lea rsi, [rsp + 1200]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rsp + 1184], rax
 mov qword ptr [rsp + 1192], rdx
 cmp eax, 99
 je xchain0_n29_α
 jmp xchain0_n33_α
 xchain0_n28_β:
 jmp xchain0_n29_α
# IR_LIT_STRING
 xchain0_n29_α:
 mov qword ptr [rsp + 1408], 1
 mov rax, qword ptr [rip + .Lx30_0]
 mov qword ptr [rsp + 1416], rax
 jmp xchain0_n34_α
 xchain0_n29_β:
 jmp xchain0_n40_α
.Lx30_0:
 .quad .Lx30_0_s
.Lx30_0_s:
 .string "3"
 xchain0_n30_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+704] -> [zr+560]
 mov rax, qword ptr [rsp + 704]
 mov qword ptr [rsp + 560], rax
 mov rax, qword ptr [rsp + 712]
 mov qword ptr [rsp + 568], rax
# marshal arg1 = producer-box slot [zr+720] -> [zr+576]
 mov rax, qword ptr [rsp + 720]
 mov qword ptr [rsp + 576], rax
 mov rax, qword ptr [rsp + 728]
 mov qword ptr [rsp + 584], rax
  .section .rodata
  .Lbynamefn31: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn31]
 lea rsi, [rsp + 560]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 544], rax
 mov qword ptr [rsp + 552], rdx
 cmp eax, 99
 je xchain0_n12_α
 jmp xchain0_n35_α
 xchain0_n30_β:
 jmp xchain0_n12_α
# IR_LIT_STRING
 xchain0_n31_α:
 mov qword ptr [rsp + 928], 1
 mov rax, qword ptr [rip + .Lx32_0]
 mov qword ptr [rsp + 936], rax
 jmp xchain0_n36_α
 xchain0_n31_β:
 jmp main_γ
.Lx32_0:
 .quad .Lx32_0_s
.Lx32_0_s:
 .string "FAIL 1110/003: OOB high should fail"
# IR_DEREF variable -> value
 xchain0_n32_α:
 mov rdi, qword ptr [rsp + 1024]
 mov rsi, qword ptr [rsp + 1032]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n23_α
 mov qword ptr [rsp + 1056], rax
 mov qword ptr [rsp + 1064], rdx
 jmp xchain0_n37_α
 xchain0_n32_β:
 jmp xchain0_n23_α
# IR_LIT_STRING
 xchain0_n33_α:
 mov qword ptr [rsp + 1264], 1
 mov rax, qword ptr [rip + .Lx34_0]
 mov qword ptr [rsp + 1272], rax
 jmp xchain0_n38_α
 xchain0_n33_β:
 jmp xchain0_n29_α
.Lx34_0:
 .quad .Lx34_0_s
.Lx34_0_s:
 .string "3"
 xchain0_n34_α:
# BOX CALL array(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1408] -> [zr+1376]
 mov rax, qword ptr [rsp + 1408]
 mov qword ptr [rsp + 1376], rax
 mov rax, qword ptr [rsp + 1416]
 mov qword ptr [rsp + 1384], rax
  .section .rodata
  .Lbynamefn35: .string "array"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn35]
 lea rsi, [rsp + 1376]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rsp + 1360], rax
 mov qword ptr [rsp + 1368], rdx
 cmp eax, 99
 je xchain0_n40_α
 jmp xchain0_n39_α
 xchain0_n34_β:
 jmp xchain0_n40_α
# IR_LIT_STRING
 xchain0_n35_α:
 mov qword ptr [rsp + 768], 1
 mov rax, qword ptr [rip + .Lx36_0]
 mov qword ptr [rsp + 776], rax
 jmp xchain0_n41_α
 xchain0_n35_β:
 jmp main_γ
.Lx36_0:
 .quad .Lx36_0_s
.Lx36_0_s:
 .string "FAIL 1110/002: array assign/read"
# IR_ASSIGN gva
 xchain0_n36_α:
 mov rax, qword ptr [rsp + 928]
 mov rdx, qword ptr [rsp + 936]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [rsp + 912], rax
 mov qword ptr [rsp + 920], rdx
 jmp main_γ
 xchain0_n36_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n37_α:
 mov qword ptr [rsp + 1088], 1
 mov rax, qword ptr [rip + .Lx38_0]
 mov qword ptr [rsp + 1096], rax
 jmp xchain0_n42_α
 xchain0_n37_β:
 jmp main_γ
.Lx38_0:
 .quad .Lx38_0_s
.Lx38_0_s:
 .string "FAIL 1110/004: OOB zero should fail"
 xchain0_n38_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1184] -> [zr+1136]
 mov rax, qword ptr [rsp + 1184]
 mov qword ptr [rsp + 1136], rax
 mov rax, qword ptr [rsp + 1192]
 mov qword ptr [rsp + 1144], rax
# marshal arg1 = producer-box slot [zr+1264] -> [zr+1152]
 mov rax, qword ptr [rsp + 1264]
 mov qword ptr [rsp + 1152], rax
 mov rax, qword ptr [rsp + 1272]
 mov qword ptr [rsp + 1160], rax
  .section .rodata
  .Lbynamefn39: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn39]
 lea rsi, [rsp + 1136]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 1120], rax
 mov qword ptr [rsp + 1128], rdx
 cmp eax, 99
 je xchain0_n29_α
 jmp xchain0_n43_α
 xchain0_n38_β:
 jmp xchain0_n29_α
# IR_ASSIGN gva
 xchain0_n39_α:
 mov rax, qword ptr [rsp + 1360]
 mov rdx, qword ptr [rsp + 1368]
 mov qword ptr [rbx + 32], rax
 mov qword ptr [rbx + 40], rdx
 mov qword ptr [rsp + 1344], rax
 mov qword ptr [rsp + 1352], rdx
 jmp xchain0_n40_α
 xchain0_n39_β:
 jmp xchain0_n40_α
# IR_VAR
 xchain0_n40_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [rsp + 1440], rax
 mov qword ptr [rsp + 1448], rdx
 jmp xchain0_n44_α
 xchain0_n40_β:
 jmp xchain0_n45_α
# IR_ASSIGN gva
 xchain0_n41_α:
 mov rax, qword ptr [rsp + 768]
 mov rdx, qword ptr [rsp + 776]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [rsp + 752], rax
 mov qword ptr [rsp + 760], rdx
 jmp main_γ
 xchain0_n41_β:
 jmp main_γ
# IR_ASSIGN gva
 xchain0_n42_α:
 mov rax, qword ptr [rsp + 1088]
 mov rdx, qword ptr [rsp + 1096]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [rsp + 1072], rax
 mov qword ptr [rsp + 1080], rdx
 jmp main_γ
 xchain0_n42_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n43_α:
 mov qword ptr [rsp + 1312], 1
 mov rax, qword ptr [rip + .Lx44_0]
 mov qword ptr [rsp + 1320], rax
 jmp xchain0_n46_α
 xchain0_n43_β:
 jmp main_γ
.Lx44_0:
 .quad .Lx44_0_s
.Lx44_0_s:
 .string "FAIL 1110/005: prototype(array(3))=3"
# IR_LIT_INTEGER
 xchain0_n44_α:
 mov qword ptr [rsp + 1472], 6
 mov rax, qword ptr [rip + .Lx45_0]
 mov qword ptr [rsp + 1480], rax
 jmp xchain0_n47_α
 xchain0_n44_β:
 jmp xchain0_n45_α
.Lx45_0:
 .quad 2
# IR_VAR
 xchain0_n45_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [rsp + 1648], rax
 mov qword ptr [rsp + 1656], rdx
 jmp xchain0_n48_α
 xchain0_n45_β:
 jmp xchain0_n49_α
# IR_ASSIGN gva
 xchain0_n46_α:
 mov rax, qword ptr [rsp + 1312]
 mov rdx, qword ptr [rsp + 1320]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [rsp + 1296], rax
 mov qword ptr [rsp + 1304], rdx
 jmp main_γ
 xchain0_n46_β:
 jmp main_γ
# IR_SUBSCRIPT x[i] variable
 xchain0_n47_α:
 mov rdi, qword ptr [rsp + 1440]
 mov rsi, qword ptr [rsp + 1448]
 mov rdx, qword ptr [rsp + 1472]
 mov rcx, qword ptr [rsp + 1480]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n45_α
 mov qword ptr [rsp + 1504], rax
 mov qword ptr [rsp + 1512], rdx
 jmp xchain0_n50_α
 xchain0_n47_β:
 jmp xchain0_n45_α
# IR_LIT_INTEGER
 xchain0_n48_α:
 mov qword ptr [rsp + 1680], 6
 mov rax, qword ptr [rip + .Lx49_0]
 mov qword ptr [rsp + 1688], rax
 jmp xchain0_n51_α
 xchain0_n48_β:
 jmp xchain0_n49_α
.Lx49_0:
 .quad 2
# IR_VAR
 xchain0_n49_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [rsp + 1952], rax
 mov qword ptr [rsp + 1960], rdx
 jmp xchain0_n52_α
 xchain0_n49_β:
 jmp xchain0_n53_α
# IR_LIT_STRING
 xchain0_n50_α:
 mov qword ptr [rsp + 1536], 1
 mov rax, qword ptr [rip + .Lx51_0]
 mov qword ptr [rsp + 1544], rax
 jmp xchain0_n54_α
 xchain0_n50_β:
 jmp xchain0_n45_α
.Lx51_0:
 .quad .Lx51_0_s
.Lx51_0_s:
 .string "x"
# IR_SUBSCRIPT x[i] variable
 xchain0_n51_α:
 mov rdi, qword ptr [rsp + 1648]
 mov rsi, qword ptr [rsp + 1656]
 mov rdx, qword ptr [rsp + 1680]
 mov rcx, qword ptr [rsp + 1688]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n49_α
 mov qword ptr [rsp + 1712], rax
 mov qword ptr [rsp + 1720], rdx
 jmp xchain0_n55_α
 xchain0_n51_β:
 jmp xchain0_n49_α
 xchain0_n52_α:
# BOX CALL prototype(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1952] -> [zr+1920]
 mov rax, qword ptr [rsp + 1952]
 mov qword ptr [rsp + 1920], rax
 mov rax, qword ptr [rsp + 1960]
 mov qword ptr [rsp + 1928], rax
  .section .rodata
  .Lbynamefn53: .string "prototype"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn53]
 lea rsi, [rsp + 1920]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rsp + 1904], rax
 mov qword ptr [rsp + 1912], rdx
 cmp eax, 99
 je xchain0_n53_α
 jmp xchain0_n56_α
 xchain0_n52_β:
 jmp xchain0_n53_α
# IR_VAR
 xchain0_n53_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [rsp + 2064], rax
 mov qword ptr [rsp + 2072], rdx
 jmp xchain0_n57_α
 xchain0_n53_β:
 jmp xchain0_n58_α
# IR_ASSIGN_VAR
 xchain0_n54_α:
 mov rdi, qword ptr [rsp + 1504]
 mov rsi, qword ptr [rsp + 1512]
 mov rdx, qword ptr [rsp + 1536]
 mov rcx, qword ptr [rsp + 1544]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n45_α
 mov qword ptr [rsp + 1568], rax
 mov qword ptr [rsp + 1576], rdx
 jmp xchain0_n45_α
 xchain0_n54_β:
 jmp xchain0_n45_α
# IR_DEREF variable -> value
 xchain0_n55_α:
 mov rdi, qword ptr [rsp + 1712]
 mov rsi, qword ptr [rsp + 1720]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n49_α
 mov qword ptr [rsp + 1744], rax
 mov qword ptr [rsp + 1752], rdx
 jmp xchain0_n59_α
 xchain0_n55_β:
 jmp xchain0_n49_α
# IR_LIT_STRING
 xchain0_n56_α:
 mov qword ptr [rsp + 1984], 1
 mov rax, qword ptr [rip + .Lx57_0]
 mov qword ptr [rsp + 1992], rax
 jmp xchain0_n60_α
 xchain0_n56_β:
 jmp xchain0_n53_α
.Lx57_0:
 .quad .Lx57_0_s
.Lx57_0_s:
 .string "3"
# IR_LIT_INTEGER
 xchain0_n57_α:
 mov qword ptr [rsp + 2096], 6
 mov rax, qword ptr [rip + .Lx58_0]
 mov qword ptr [rsp + 2104], rax
 jmp xchain0_n61_α
 xchain0_n57_β:
 jmp xchain0_n58_α
.Lx58_0:
 .quad 1
# IR_VAR
 xchain0_n58_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [rsp + 2272], rax
 mov qword ptr [rsp + 2280], rdx
 jmp xchain0_n62_α
 xchain0_n58_β:
 jmp xchain0_n63_α
# IR_LIT_STRING
 xchain0_n59_α:
 mov qword ptr [rsp + 1760], 1
 mov rax, qword ptr [rip + .Lx60_0]
 mov qword ptr [rsp + 1768], rax
 jmp xchain0_n64_α
 xchain0_n59_β:
 jmp xchain0_n49_α
.Lx60_0:
 .quad .Lx60_0_s
.Lx60_0_s:
 .string "x"
 xchain0_n60_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1904] -> [zr+1856]
 mov rax, qword ptr [rsp + 1904]
 mov qword ptr [rsp + 1856], rax
 mov rax, qword ptr [rsp + 1912]
 mov qword ptr [rsp + 1864], rax
# marshal arg1 = producer-box slot [zr+1984] -> [zr+1872]
 mov rax, qword ptr [rsp + 1984]
 mov qword ptr [rsp + 1872], rax
 mov rax, qword ptr [rsp + 1992]
 mov qword ptr [rsp + 1880], rax
  .section .rodata
  .Lbynamefn61: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn61]
 lea rsi, [rsp + 1856]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 1840], rax
 mov qword ptr [rsp + 1848], rdx
 cmp eax, 99
 je xchain0_n53_α
 jmp xchain0_n65_α
 xchain0_n60_β:
 jmp xchain0_n53_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n61_α:
 mov rdi, qword ptr [rsp + 2064]
 mov rsi, qword ptr [rsp + 2072]
 mov rdx, qword ptr [rsp + 2096]
 mov rcx, qword ptr [rsp + 2104]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n58_α
 mov qword ptr [rsp + 2128], rax
 mov qword ptr [rsp + 2136], rdx
 jmp xchain0_n66_α
 xchain0_n61_β:
 jmp xchain0_n58_α
# IR_LIT_INTEGER
 xchain0_n62_α:
 mov qword ptr [rsp + 2304], 6
 mov rax, qword ptr [rip + .Lx63_0]
 mov qword ptr [rsp + 2312], rax
 jmp xchain0_n67_α
 xchain0_n62_β:
 jmp xchain0_n63_α
.Lx63_0:
 .quad 1
# IR_VAR
 xchain0_n63_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [rsp + 2464], rax
 mov qword ptr [rsp + 2472], rdx
 jmp xchain0_n68_α
 xchain0_n63_β:
 jmp xchain0_n69_α
 xchain0_n64_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1744] -> [zr+1600]
 mov rax, qword ptr [rsp + 1744]
 mov qword ptr [rsp + 1600], rax
 mov rax, qword ptr [rsp + 1752]
 mov qword ptr [rsp + 1608], rax
# marshal arg1 = producer-box slot [zr+1760] -> [zr+1616]
 mov rax, qword ptr [rsp + 1760]
 mov qword ptr [rsp + 1616], rax
 mov rax, qword ptr [rsp + 1768]
 mov qword ptr [rsp + 1624], rax
  .section .rodata
  .Lbynamefn65: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn65]
 lea rsi, [rsp + 1600]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 1584], rax
 mov qword ptr [rsp + 1592], rdx
 cmp eax, 99
 je xchain0_n49_α
 jmp xchain0_n70_α
 xchain0_n64_β:
 jmp xchain0_n49_α
# IR_LIT_STRING
 xchain0_n65_α:
 mov qword ptr [rsp + 2032], 1
 mov rax, qword ptr [rip + .Lx66_0]
 mov qword ptr [rsp + 2040], rax
 jmp xchain0_n71_α
 xchain0_n65_β:
 jmp main_γ
.Lx66_0:
 .quad .Lx66_0_s
.Lx66_0_s:
 .string "FAIL 1110/007: prototype string-dim array"
# IR_LIT_REAL
 xchain0_n66_α:
 mov qword ptr [rsp + 2160], 7
 mov rax, qword ptr [rip + .Lx67_0]
 mov qword ptr [rsp + 2168], rax
 jmp xchain0_n72_α
 xchain0_n66_β:
 jmp xchain0_n58_α
.Lx67_0:
 .quad 4614253070214989087
# IR_SUBSCRIPT x[i] variable
 xchain0_n67_α:
 mov rdi, qword ptr [rsp + 2272]
 mov rsi, qword ptr [rsp + 2280]
 mov rdx, qword ptr [rsp + 2304]
 mov rcx, qword ptr [rsp + 2312]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n63_α
 mov qword ptr [rsp + 2336], rax
 mov qword ptr [rsp + 2344], rdx
 jmp xchain0_n73_α
 xchain0_n67_β:
 jmp xchain0_n63_α
# IR_LIT_INTEGER
 xchain0_n68_α:
 mov qword ptr [rsp + 2496], 6
 mov rax, qword ptr [rip + .Lx69_0]
 mov qword ptr [rsp + 2504], rax
 jmp xchain0_n74_α
 xchain0_n68_β:
 jmp xchain0_n69_α
.Lx69_0:
 .quad 3
# IR_VAR
 xchain0_n69_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [rsp + 2672], rax
 mov qword ptr [rsp + 2680], rdx
 jmp xchain0_n75_α
 xchain0_n69_β:
 jmp xchain0_n76_α
# IR_LIT_STRING
 xchain0_n70_α:
 mov qword ptr [rsp + 1808], 1
 mov rax, qword ptr [rip + .Lx71_0]
 mov qword ptr [rsp + 1816], rax
 jmp xchain0_n77_α
 xchain0_n70_β:
 jmp main_γ
.Lx71_0:
 .quad .Lx71_0_s
.Lx71_0_s:
 .string "FAIL 1110/006: array from string dim"
# IR_ASSIGN gva
 xchain0_n71_α:
 mov rax, qword ptr [rsp + 2032]
 mov rdx, qword ptr [rsp + 2040]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [rsp + 2016], rax
 mov qword ptr [rsp + 2024], rdx
 jmp main_γ
 xchain0_n71_β:
 jmp main_γ
# IR_ASSIGN_VAR
 xchain0_n72_α:
 mov rdi, qword ptr [rsp + 2128]
 mov rsi, qword ptr [rsp + 2136]
 mov rdx, qword ptr [rsp + 2160]
 mov rcx, qword ptr [rsp + 2168]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n58_α
 mov qword ptr [rsp + 2192], rax
 mov qword ptr [rsp + 2200], rdx
 jmp xchain0_n58_α
 xchain0_n72_β:
 jmp xchain0_n58_α
# IR_DEREF variable -> value
 xchain0_n73_α:
 mov rdi, qword ptr [rsp + 2336]
 mov rsi, qword ptr [rsp + 2344]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n63_α
 mov qword ptr [rsp + 2368], rax
 mov qword ptr [rsp + 2376], rdx
 jmp xchain0_n78_α
 xchain0_n73_β:
 jmp xchain0_n63_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n74_α:
 mov rdi, qword ptr [rsp + 2464]
 mov rsi, qword ptr [rsp + 2472]
 mov rdx, qword ptr [rsp + 2496]
 mov rcx, qword ptr [rsp + 2504]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n69_α
 mov qword ptr [rsp + 2528], rax
 mov qword ptr [rsp + 2536], rdx
 jmp xchain0_n79_α
 xchain0_n74_β:
 jmp xchain0_n69_α
# IR_LIT_INTEGER
 xchain0_n75_α:
 mov qword ptr [rsp + 2704], 6
 mov rax, qword ptr [rip + .Lx76_0]
 mov qword ptr [rsp + 2712], rax
 jmp xchain0_n80_α
 xchain0_n75_β:
 jmp xchain0_n76_α
.Lx76_0:
 .quad 3
# IR_LIT_STRING
 xchain0_n76_α:
 mov qword ptr [rsp + 2880], 1
 mov rax, qword ptr [rip + .Lx77_0]
 mov qword ptr [rsp + 2888], rax
 jmp xchain0_n81_α
 xchain0_n76_β:
 jmp main_γ
.Lx77_0:
 .quad .Lx77_0_s
.Lx77_0_s:
 .string "PASS 1110_array_1d (9/9)"
# IR_ASSIGN gva
 xchain0_n77_α:
 mov rax, qword ptr [rsp + 1808]
 mov rdx, qword ptr [rsp + 1816]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [rsp + 1792], rax
 mov qword ptr [rsp + 1800], rdx
 jmp main_γ
 xchain0_n77_β:
 jmp main_γ
# IR_LIT_REAL
 xchain0_n78_α:
 mov qword ptr [rsp + 2384], 7
 mov rax, qword ptr [rip + .Lx79_0]
 mov qword ptr [rsp + 2392], rax
 jmp xchain0_n82_α
 xchain0_n78_β:
 jmp xchain0_n63_α
.Lx79_0:
 .quad 4614253070214989087
# IR_LIT_STRING
 xchain0_n79_α:
 mov qword ptr [rsp + 2560], 1
 mov rax, qword ptr [rip + .Lx80_0]
 mov qword ptr [rsp + 2568], rax
 jmp xchain0_n83_α
 xchain0_n79_β:
 jmp xchain0_n69_α
.Lx80_0:
 .quad .Lx80_0_s
.Lx80_0_s:
 .string "z"
# IR_SUBSCRIPT x[i] variable
 xchain0_n80_α:
 mov rdi, qword ptr [rsp + 2672]
 mov rsi, qword ptr [rsp + 2680]
 mov rdx, qword ptr [rsp + 2704]
 mov rcx, qword ptr [rsp + 2712]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n76_α
 mov qword ptr [rsp + 2736], rax
 mov qword ptr [rsp + 2744], rdx
 jmp xchain0_n84_α
 xchain0_n80_β:
 jmp xchain0_n76_α
# IR_ASSIGN gva
 xchain0_n81_α:
 mov rax, qword ptr [rsp + 2880]
 mov rdx, qword ptr [rsp + 2888]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [rsp + 2864], rax
 mov qword ptr [rsp + 2872], rdx
 jmp main_γ
 xchain0_n81_β:
 jmp main_γ
 xchain0_n82_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+2368] -> [zr+2224]
 mov rax, qword ptr [rsp + 2368]
 mov qword ptr [rsp + 2224], rax
 mov rax, qword ptr [rsp + 2376]
 mov qword ptr [rsp + 2232], rax
# marshal arg1 = producer-box slot [zr+2384] -> [zr+2240]
 mov rax, qword ptr [rsp + 2384]
 mov qword ptr [rsp + 2240], rax
 mov rax, qword ptr [rsp + 2392]
 mov qword ptr [rsp + 2248], rax
  .section .rodata
  .Lbynamefn83: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn83]
 lea rsi, [rsp + 2224]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 2208], rax
 mov qword ptr [rsp + 2216], rdx
 cmp eax, 99
 je xchain0_n63_α
 jmp xchain0_n85_α
 xchain0_n82_β:
 jmp xchain0_n63_α
# IR_ASSIGN_VAR
 xchain0_n83_α:
 mov rdi, qword ptr [rsp + 2528]
 mov rsi, qword ptr [rsp + 2536]
 mov rdx, qword ptr [rsp + 2560]
 mov rcx, qword ptr [rsp + 2568]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n69_α
 mov qword ptr [rsp + 2592], rax
 mov qword ptr [rsp + 2600], rdx
 jmp xchain0_n69_α
 xchain0_n83_β:
 jmp xchain0_n69_α
# IR_DEREF variable -> value
 xchain0_n84_α:
 mov rdi, qword ptr [rsp + 2736]
 mov rsi, qword ptr [rsp + 2744]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n76_α
 mov qword ptr [rsp + 2768], rax
 mov qword ptr [rsp + 2776], rdx
 jmp xchain0_n86_α
 xchain0_n84_β:
 jmp xchain0_n76_α
# IR_LIT_STRING
 xchain0_n85_α:
 mov qword ptr [rsp + 2432], 1
 mov rax, qword ptr [rip + .Lx86_0]
 mov qword ptr [rsp + 2440], rax
 jmp xchain0_n87_α
 xchain0_n85_β:
 jmp main_γ
.Lx86_0:
 .quad .Lx86_0_s
.Lx86_0_s:
 .string "FAIL 1110/008: array stores real"
# IR_LIT_STRING
 xchain0_n86_α:
 mov qword ptr [rsp + 2784], 1
 mov rax, qword ptr [rip + .Lx87_0]
 mov qword ptr [rsp + 2792], rax
 jmp xchain0_n88_α
 xchain0_n86_β:
 jmp xchain0_n76_α
.Lx87_0:
 .quad .Lx87_0_s
.Lx87_0_s:
 .string "z"
# IR_ASSIGN gva
 xchain0_n87_α:
 mov rax, qword ptr [rsp + 2432]
 mov rdx, qword ptr [rsp + 2440]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [rsp + 2416], rax
 mov qword ptr [rsp + 2424], rdx
 jmp main_γ
 xchain0_n87_β:
 jmp main_γ
 xchain0_n88_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+2768] -> [zr+2624]
 mov rax, qword ptr [rsp + 2768]
 mov qword ptr [rsp + 2624], rax
 mov rax, qword ptr [rsp + 2776]
 mov qword ptr [rsp + 2632], rax
# marshal arg1 = producer-box slot [zr+2784] -> [zr+2640]
 mov rax, qword ptr [rsp + 2784]
 mov qword ptr [rsp + 2640], rax
 mov rax, qword ptr [rsp + 2792]
 mov qword ptr [rsp + 2648], rax
  .section .rodata
  .Lbynamefn89: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn89]
 lea rsi, [rsp + 2624]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 2608], rax
 mov qword ptr [rsp + 2616], rdx
 cmp eax, 99
 je xchain0_n76_α
 jmp xchain0_n89_α
 xchain0_n88_β:
 jmp xchain0_n76_α
# IR_LIT_STRING
 xchain0_n89_α:
 mov qword ptr [rsp + 2832], 1
 mov rax, qword ptr [rip + .Lx90_0]
 mov qword ptr [rsp + 2840], rax
 jmp xchain0_n90_α
 xchain0_n89_β:
 jmp main_γ
.Lx90_0:
 .quad .Lx90_0_s
.Lx90_0_s:
 .string "FAIL 1110/009: array stores string"
# IR_ASSIGN gva
 xchain0_n90_α:
 mov rax, qword ptr [rsp + 2832]
 mov rdx, qword ptr [rsp + 2840]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [rsp + 2816], rax
 mov qword ptr [rsp + 2824], rdx
 jmp main_γ
 xchain0_n90_β:
 jmp main_γ
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rsp + 2920]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rsp + 2920]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
add rsp, 65544
ret
