  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "c"
  .Lgvan1: .string "output"
  .Lgvan2: .string "d"
  .align 8
__gva_names:
  .quad .Lgvan0
  .quad .Lgvan1
  .quad .Lgvan2
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
  mov edi, 3
  call rt_gva_island@PLT
  mov rsi, rax
  lea rdi, [rip + __gva_names]
  mov edx, 3
  call gva_register@PLT
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
  mov qword ptr [rsp + 2168], rsp
main_α_body:
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [rsp + 160], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [rsp + 168], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n3_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "2,2"
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
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rsp + 96], rax
 mov qword ptr [rsp + 104], rdx
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n3_α
# IR_VAR
 xchain0_n3_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 mov qword ptr [rsp + 192], rax
 mov qword ptr [rsp + 200], rdx
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp xchain0_n5_α
# IR_LIT_INTEGER
 xchain0_n4_α:
 mov qword ptr [rsp + 224], 6
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [rsp + 232], rax
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp xchain0_n5_α
.Lx5_0:
 .quad 1
# IR_VAR
 xchain0_n5_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 mov qword ptr [rsp + 464], rax
 mov qword ptr [rsp + 472], rdx
 jmp xchain0_n7_α
 xchain0_n5_β:
 jmp xchain0_n8_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n6_α:
 mov rdi, qword ptr [rsp + 192]
 mov rsi, qword ptr [rsp + 200]
 mov rdx, qword ptr [rsp + 224]
 mov rcx, qword ptr [rsp + 232]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n5_α
 mov qword ptr [rsp + 256], rax
 mov qword ptr [rsp + 264], rdx
 jmp xchain0_n9_α
 xchain0_n6_β:
 jmp xchain0_n5_α
# IR_LIT_INTEGER
 xchain0_n7_α:
 mov qword ptr [rsp + 496], 6
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [rsp + 504], rax
 jmp xchain0_n10_α
 xchain0_n7_β:
 jmp xchain0_n8_α
.Lx8_0:
 .quad 1
# IR_VAR
 xchain0_n8_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 mov qword ptr [rsp + 832], rax
 mov qword ptr [rsp + 840], rdx
 jmp xchain0_n11_α
 xchain0_n8_β:
 jmp xchain0_n12_α
# IR_LIT_INTEGER
 xchain0_n9_α:
 mov qword ptr [rsp + 288], 6
 mov rax, qword ptr [rip + .Lx10_0]
 mov qword ptr [rsp + 296], rax
 jmp xchain0_n13_α
 xchain0_n9_β:
 jmp xchain0_n5_α
.Lx10_0:
 .quad 2
# IR_SUBSCRIPT x[i] variable
 xchain0_n10_α:
 mov rdi, qword ptr [rsp + 464]
 mov rsi, qword ptr [rsp + 472]
 mov rdx, qword ptr [rsp + 496]
 mov rcx, qword ptr [rsp + 504]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n8_α
 mov qword ptr [rsp + 528], rax
 mov qword ptr [rsp + 536], rdx
 jmp xchain0_n14_α
 xchain0_n10_β:
 jmp xchain0_n8_α
 xchain0_n11_α:
# BOX CALL prototype(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+832] -> [zr+800]
 mov rax, qword ptr [rsp + 832]
 mov qword ptr [rsp + 800], rax
 mov rax, qword ptr [rsp + 840]
 mov qword ptr [rsp + 808], rax
  .section .rodata
  .Lbynamefn12: .string "prototype"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn12]
 lea rsi, [rsp + 800]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rsp + 784], rax
 mov qword ptr [rsp + 792], rdx
 cmp eax, 99
 je xchain0_n12_α
 jmp xchain0_n15_α
 xchain0_n11_β:
 jmp xchain0_n12_α
# IR_LIT_STRING
 xchain0_n12_α:
 mov qword ptr [rsp + 1008], 1
 mov rax, qword ptr [rip + .Lx13_0]
 mov qword ptr [rsp + 1016], rax
 jmp xchain0_n16_α
 xchain0_n12_β:
 jmp xchain0_n21_α
.Lx13_0:
 .quad .Lx13_0_s
.Lx13_0_s:
 .string "-1:1,2"
# IR_SUBSCRIPT x[i] variable
 xchain0_n13_α:
 mov rdi, qword ptr [rsp + 256]
 mov rsi, qword ptr [rsp + 264]
 mov rdx, qword ptr [rsp + 288]
 mov rcx, qword ptr [rsp + 296]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n5_α
 mov qword ptr [rsp + 320], rax
 mov qword ptr [rsp + 328], rdx
 jmp xchain0_n17_α
 xchain0_n13_β:
 jmp xchain0_n5_α
# IR_LIT_INTEGER
 xchain0_n14_α:
 mov qword ptr [rsp + 560], 6
 mov rax, qword ptr [rip + .Lx15_0]
 mov qword ptr [rsp + 568], rax
 jmp xchain0_n18_α
 xchain0_n14_β:
 jmp xchain0_n8_α
.Lx15_0:
 .quad 2
# IR_LIT_STRING
 xchain0_n15_α:
 mov qword ptr [rsp + 864], 1
 mov rax, qword ptr [rip + .Lx16_0]
 mov qword ptr [rsp + 872], rax
 jmp xchain0_n19_α
 xchain0_n15_β:
 jmp xchain0_n12_α
.Lx16_0:
 .quad .Lx16_0_s
.Lx16_0_s:
 .string "2,2"
 xchain0_n16_α:
# BOX CALL array(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1008] -> [zr+976]
 mov rax, qword ptr [rsp + 1008]
 mov qword ptr [rsp + 976], rax
 mov rax, qword ptr [rsp + 1016]
 mov qword ptr [rsp + 984], rax
  .section .rodata
  .Lbynamefn17: .string "array"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn17]
 lea rsi, [rsp + 976]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rsp + 960], rax
 mov qword ptr [rsp + 968], rdx
 cmp eax, 99
 je xchain0_n21_α
 jmp xchain0_n20_α
 xchain0_n16_β:
 jmp xchain0_n21_α
# IR_LIT_STRING
 xchain0_n17_α:
 mov qword ptr [rsp + 352], 1
 mov rax, qword ptr [rip + .Lx18_0]
 mov qword ptr [rsp + 360], rax
 jmp xchain0_n22_α
 xchain0_n17_β:
 jmp xchain0_n5_α
.Lx18_0:
 .quad .Lx18_0_s
.Lx18_0_s:
 .string "*"
# IR_SUBSCRIPT x[i] variable
 xchain0_n18_α:
 mov rdi, qword ptr [rsp + 528]
 mov rsi, qword ptr [rsp + 536]
 mov rdx, qword ptr [rsp + 560]
 mov rcx, qword ptr [rsp + 568]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n8_α
 mov qword ptr [rsp + 592], rax
 mov qword ptr [rsp + 600], rdx
 jmp xchain0_n23_α
 xchain0_n18_β:
 jmp xchain0_n8_α
 xchain0_n19_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+784] -> [zr+736]
 mov rax, qword ptr [rsp + 784]
 mov qword ptr [rsp + 736], rax
 mov rax, qword ptr [rsp + 792]
 mov qword ptr [rsp + 744], rax
# marshal arg1 = producer-box slot [zr+864] -> [zr+752]
 mov rax, qword ptr [rsp + 864]
 mov qword ptr [rsp + 752], rax
 mov rax, qword ptr [rsp + 872]
 mov qword ptr [rsp + 760], rax
  .section .rodata
  .Lbynamefn20: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn20]
 lea rsi, [rsp + 736]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 720], rax
 mov qword ptr [rsp + 728], rdx
 cmp eax, 99
 je xchain0_n12_α
 jmp xchain0_n24_α
 xchain0_n19_β:
 jmp xchain0_n12_α
# IR_ASSIGN gva
 xchain0_n20_α:
 mov rax, qword ptr [rsp + 960]
 mov rdx, qword ptr [rsp + 968]
 mov qword ptr [1879052320], rax
 mov qword ptr [1879052328], rdx
 mov qword ptr [rsp + 944], rax
 mov qword ptr [rsp + 952], rdx
 jmp xchain0_n21_α
 xchain0_n20_β:
 jmp xchain0_n21_α
# IR_VAR
 xchain0_n21_α:
 mov rax, qword ptr [1879052320]
 mov rdx, qword ptr [1879052328]
 mov qword ptr [rsp + 1040], rax
 mov qword ptr [rsp + 1048], rdx
 jmp xchain0_n25_α
 xchain0_n21_β:
 jmp xchain0_n26_α
# IR_ASSIGN_VAR
 xchain0_n22_α:
 mov rdi, qword ptr [rsp + 320]
 mov rsi, qword ptr [rsp + 328]
 mov rdx, qword ptr [rsp + 352]
 mov rcx, qword ptr [rsp + 360]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n5_α
 mov qword ptr [rsp + 384], rax
 mov qword ptr [rsp + 392], rdx
 jmp xchain0_n5_α
 xchain0_n22_β:
 jmp xchain0_n5_α
# IR_DEREF variable -> value
 xchain0_n23_α:
 mov rdi, qword ptr [rsp + 592]
 mov rsi, qword ptr [rsp + 600]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n8_α
 mov qword ptr [rsp + 624], rax
 mov qword ptr [rsp + 632], rdx
 jmp xchain0_n27_α
 xchain0_n23_β:
 jmp xchain0_n8_α
# IR_LIT_STRING
 xchain0_n24_α:
 mov qword ptr [rsp + 912], 1
 mov rax, qword ptr [rip + .Lx25_0]
 mov qword ptr [rsp + 920], rax
 jmp xchain0_n28_α
 xchain0_n24_β:
 jmp main_γ
.Lx25_0:
 .quad .Lx25_0_s
.Lx25_0_s:
 .string "FAIL 1112/002: prototype of 2D array"
# IR_LIT_INTEGER
 xchain0_n25_α:
 mov qword ptr [rsp + 1104], 6
 mov rax, qword ptr [rip + .Lx26_0]
 mov qword ptr [rsp + 1112], rax
 jmp xchain0_n29_α
 xchain0_n25_β:
 jmp xchain0_n26_α
.Lx26_0:
 .quad 1
# IR_VAR
 xchain0_n26_α:
 mov rax, qword ptr [1879052320]
 mov rdx, qword ptr [1879052328]
 mov qword ptr [rsp + 1344], rax
 mov qword ptr [rsp + 1352], rdx
 jmp xchain0_n30_α
 xchain0_n26_β:
 jmp xchain0_n31_α
# IR_LIT_STRING
 xchain0_n27_α:
 mov qword ptr [rsp + 640], 1
 mov rax, qword ptr [rip + .Lx28_0]
 mov qword ptr [rsp + 648], rax
 jmp xchain0_n32_α
 xchain0_n27_β:
 jmp xchain0_n8_α
.Lx28_0:
 .quad .Lx28_0_s
.Lx28_0_s:
 .string "*"
# IR_ASSIGN gva
 xchain0_n28_α:
 mov rax, qword ptr [rsp + 912]
 mov rdx, qword ptr [rsp + 920]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rsp + 896], rax
 mov qword ptr [rsp + 904], rdx
 jmp main_γ
 xchain0_n28_β:
 jmp main_γ
# IR_UNOP
 xchain0_n29_α:
 mov rdi, qword ptr [rsp + 1104]
 mov rsi, qword ptr [rsp + 1112]
 call rt_num_neg@PLT
 mov qword ptr [rsp + 1072], rax
 mov qword ptr [rsp + 1080], rdx
 jmp xchain0_n33_α
 xchain0_n29_β:
 jmp xchain0_n26_α
# IR_LIT_INTEGER
 xchain0_n30_α:
 mov qword ptr [rsp + 1408], 6
 mov rax, qword ptr [rip + .Lx31_0]
 mov qword ptr [rsp + 1416], rax
 jmp xchain0_n34_α
 xchain0_n30_β:
 jmp xchain0_n31_α
.Lx31_0:
 .quad 1
# IR_VAR
 xchain0_n31_α:
 mov rax, qword ptr [1879052320]
 mov rdx, qword ptr [1879052328]
 mov qword ptr [rsp + 1632], rax
 mov qword ptr [rsp + 1640], rdx
 jmp xchain0_n35_α
 xchain0_n31_β:
 jmp xchain0_n36_α
 xchain0_n32_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+624] -> [zr+416]
 mov rax, qword ptr [rsp + 624]
 mov qword ptr [rsp + 416], rax
 mov rax, qword ptr [rsp + 632]
 mov qword ptr [rsp + 424], rax
# marshal arg1 = producer-box slot [zr+640] -> [zr+432]
 mov rax, qword ptr [rsp + 640]
 mov qword ptr [rsp + 432], rax
 mov rax, qword ptr [rsp + 648]
 mov qword ptr [rsp + 440], rax
  .section .rodata
  .Lbynamefn33: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn33]
 lea rsi, [rsp + 416]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 400], rax
 mov qword ptr [rsp + 408], rdx
 cmp eax, 99
 je xchain0_n8_α
 jmp xchain0_n37_α
 xchain0_n32_β:
 jmp xchain0_n8_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n33_α:
 mov rdi, qword ptr [rsp + 1040]
 mov rsi, qword ptr [rsp + 1048]
 mov rdx, qword ptr [rsp + 1072]
 mov rcx, qword ptr [rsp + 1080]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n26_α
 mov qword ptr [rsp + 1136], rax
 mov qword ptr [rsp + 1144], rdx
 jmp xchain0_n38_α
 xchain0_n33_β:
 jmp xchain0_n26_α
# IR_UNOP
 xchain0_n34_α:
 mov rdi, qword ptr [rsp + 1408]
 mov rsi, qword ptr [rsp + 1416]
 call rt_num_neg@PLT
 mov qword ptr [rsp + 1376], rax
 mov qword ptr [rsp + 1384], rdx
 jmp xchain0_n39_α
 xchain0_n34_β:
 jmp xchain0_n31_α
# IR_LIT_INTEGER
 xchain0_n35_α:
 mov qword ptr [rsp + 1696], 6
 mov rax, qword ptr [rip + .Lx36_0]
 mov qword ptr [rsp + 1704], rax
 jmp xchain0_n40_α
 xchain0_n35_β:
 jmp xchain0_n36_α
.Lx36_0:
 .quad 2
# IR_VAR
 xchain0_n36_α:
 mov rax, qword ptr [1879052320]
 mov rdx, qword ptr [1879052328]
 mov qword ptr [rsp + 1888], rax
 mov qword ptr [rsp + 1896], rdx
 jmp xchain0_n41_α
 xchain0_n36_β:
 jmp xchain0_n42_α
# IR_LIT_STRING
 xchain0_n37_α:
 mov qword ptr [rsp + 688], 1
 mov rax, qword ptr [rip + .Lx38_0]
 mov qword ptr [rsp + 696], rax
 jmp xchain0_n43_α
 xchain0_n37_β:
 jmp main_γ
.Lx38_0:
 .quad .Lx38_0_s
.Lx38_0_s:
 .string "FAIL 1112/001: 2D array assign/read"
# IR_LIT_INTEGER
 xchain0_n38_α:
 mov qword ptr [rsp + 1168], 6
 mov rax, qword ptr [rip + .Lx39_0]
 mov qword ptr [rsp + 1176], rax
 jmp xchain0_n44_α
 xchain0_n38_β:
 jmp xchain0_n26_α
.Lx39_0:
 .quad 1
# IR_SUBSCRIPT x[i] variable
 xchain0_n39_α:
 mov rdi, qword ptr [rsp + 1344]
 mov rsi, qword ptr [rsp + 1352]
 mov rdx, qword ptr [rsp + 1376]
 mov rcx, qword ptr [rsp + 1384]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n31_α
 mov qword ptr [rsp + 1440], rax
 mov qword ptr [rsp + 1448], rdx
 jmp xchain0_n45_α
 xchain0_n39_β:
 jmp xchain0_n31_α
# IR_UNOP
 xchain0_n40_α:
 mov rdi, qword ptr [rsp + 1696]
 mov rsi, qword ptr [rsp + 1704]
 call rt_num_neg@PLT
 mov qword ptr [rsp + 1664], rax
 mov qword ptr [rsp + 1672], rdx
 jmp xchain0_n46_α
 xchain0_n40_β:
 jmp xchain0_n36_α
# IR_LIT_INTEGER
 xchain0_n41_α:
 mov qword ptr [rsp + 1920], 6
 mov rax, qword ptr [rip + .Lx42_0]
 mov qword ptr [rsp + 1928], rax
 jmp xchain0_n47_α
 xchain0_n41_β:
 jmp xchain0_n42_α
.Lx42_0:
 .quad 2
# IR_LIT_STRING
 xchain0_n42_α:
 mov qword ptr [rsp + 2128], 1
 mov rax, qword ptr [rip + .Lx43_0]
 mov qword ptr [rsp + 2136], rax
 jmp xchain0_n48_α
 xchain0_n42_β:
 jmp main_γ
.Lx43_0:
 .quad .Lx43_0_s
.Lx43_0_s:
 .string "PASS 1112_array_multi (5/5)"
# IR_ASSIGN gva
 xchain0_n43_α:
 mov rax, qword ptr [rsp + 688]
 mov rdx, qword ptr [rsp + 696]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rsp + 672], rax
 mov qword ptr [rsp + 680], rdx
 jmp main_γ
 xchain0_n43_β:
 jmp main_γ
# IR_SUBSCRIPT x[i] variable
 xchain0_n44_α:
 mov rdi, qword ptr [rsp + 1136]
 mov rsi, qword ptr [rsp + 1144]
 mov rdx, qword ptr [rsp + 1168]
 mov rcx, qword ptr [rsp + 1176]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n26_α
 mov qword ptr [rsp + 1200], rax
 mov qword ptr [rsp + 1208], rdx
 jmp xchain0_n49_α
 xchain0_n44_β:
 jmp xchain0_n26_α
# IR_LIT_INTEGER
 xchain0_n45_α:
 mov qword ptr [rsp + 1472], 6
 mov rax, qword ptr [rip + .Lx46_0]
 mov qword ptr [rsp + 1480], rax
 jmp xchain0_n50_α
 xchain0_n45_β:
 jmp xchain0_n31_α
.Lx46_0:
 .quad 1
# IR_SUBSCRIPT x[i] variable
 xchain0_n46_α:
 mov rdi, qword ptr [rsp + 1632]
 mov rsi, qword ptr [rsp + 1640]
 mov rdx, qword ptr [rsp + 1664]
 mov rcx, qword ptr [rsp + 1672]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n36_α
 mov qword ptr [rsp + 1728], rax
 mov qword ptr [rsp + 1736], rdx
 jmp xchain0_n51_α
 xchain0_n46_β:
 jmp xchain0_n36_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n47_α:
 mov rdi, qword ptr [rsp + 1888]
 mov rsi, qword ptr [rsp + 1896]
 mov rdx, qword ptr [rsp + 1920]
 mov rcx, qword ptr [rsp + 1928]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n42_α
 mov qword ptr [rsp + 1952], rax
 mov qword ptr [rsp + 1960], rdx
 jmp xchain0_n52_α
 xchain0_n47_β:
 jmp xchain0_n42_α
# IR_ASSIGN gva
 xchain0_n48_α:
 mov rax, qword ptr [rsp + 2128]
 mov rdx, qword ptr [rsp + 2136]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rsp + 2112], rax
 mov qword ptr [rsp + 2120], rdx
 jmp main_γ
 xchain0_n48_β:
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n49_α:
 mov qword ptr [rsp + 1232], 6
 mov rax, qword ptr [rip + .Lx50_0]
 mov qword ptr [rsp + 1240], rax
 jmp xchain0_n53_α
 xchain0_n49_β:
 jmp xchain0_n26_α
.Lx50_0:
 .quad 0
# IR_SUBSCRIPT x[i] variable
 xchain0_n50_α:
 mov rdi, qword ptr [rsp + 1440]
 mov rsi, qword ptr [rsp + 1448]
 mov rdx, qword ptr [rsp + 1472]
 mov rcx, qword ptr [rsp + 1480]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n31_α
 mov qword ptr [rsp + 1504], rax
 mov qword ptr [rsp + 1512], rdx
 jmp xchain0_n54_α
 xchain0_n50_β:
 jmp xchain0_n31_α
# IR_LIT_INTEGER
 xchain0_n51_α:
 mov qword ptr [rsp + 1760], 6
 mov rax, qword ptr [rip + .Lx52_0]
 mov qword ptr [rsp + 1768], rax
 jmp xchain0_n55_α
 xchain0_n51_β:
 jmp xchain0_n36_α
.Lx52_0:
 .quad 1
# IR_LIT_INTEGER
 xchain0_n52_α:
 mov qword ptr [rsp + 1984], 6
 mov rax, qword ptr [rip + .Lx53_0]
 mov qword ptr [rsp + 1992], rax
 jmp xchain0_n56_α
 xchain0_n52_β:
 jmp xchain0_n42_α
.Lx53_0:
 .quad 1
# IR_ASSIGN_VAR
 xchain0_n53_α:
 mov rdi, qword ptr [rsp + 1200]
 mov rsi, qword ptr [rsp + 1208]
 mov rdx, qword ptr [rsp + 1232]
 mov rcx, qword ptr [rsp + 1240]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n26_α
 mov qword ptr [rsp + 1264], rax
 mov qword ptr [rsp + 1272], rdx
 jmp xchain0_n26_α
 xchain0_n53_β:
 jmp xchain0_n26_α
# IR_DEREF variable -> value
 xchain0_n54_α:
 mov rdi, qword ptr [rsp + 1504]
 mov rsi, qword ptr [rsp + 1512]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n31_α
 mov qword ptr [rsp + 1536], rax
 mov qword ptr [rsp + 1544], rdx
 jmp xchain0_n57_α
 xchain0_n54_β:
 jmp xchain0_n31_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n55_α:
 mov rdi, qword ptr [rsp + 1728]
 mov rsi, qword ptr [rsp + 1736]
 mov rdx, qword ptr [rsp + 1760]
 mov rcx, qword ptr [rsp + 1768]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n36_α
 mov qword ptr [rsp + 1792], rax
 mov qword ptr [rsp + 1800], rdx
 jmp xchain0_n58_α
 xchain0_n55_β:
 jmp xchain0_n36_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n56_α:
 mov rdi, qword ptr [rsp + 1952]
 mov rsi, qword ptr [rsp + 1960]
 mov rdx, qword ptr [rsp + 1984]
 mov rcx, qword ptr [rsp + 1992]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n42_α
 mov qword ptr [rsp + 2016], rax
 mov qword ptr [rsp + 2024], rdx
 jmp xchain0_n59_α
 xchain0_n56_β:
 jmp xchain0_n42_α
# IR_LIT_INTEGER
 xchain0_n57_α:
 mov qword ptr [rsp + 1552], 6
 mov rax, qword ptr [rip + .Lx58_0]
 mov qword ptr [rsp + 1560], rax
 jmp xchain0_n60_α
 xchain0_n57_β:
 jmp xchain0_n31_α
.Lx58_0:
 .quad 0
# IR_DEREF variable -> value
 xchain0_n58_α:
 mov rdi, qword ptr [rsp + 1792]
 mov rsi, qword ptr [rsp + 1800]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n36_α
 mov qword ptr [rsp + 1824], rax
 mov qword ptr [rsp + 1832], rdx
 jmp xchain0_n61_α
 xchain0_n58_β:
 jmp xchain0_n36_α
# IR_DEREF variable -> value
 xchain0_n59_α:
 mov rdi, qword ptr [rsp + 2016]
 mov rsi, qword ptr [rsp + 2024]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n42_α
 mov qword ptr [rsp + 2048], rax
 mov qword ptr [rsp + 2056], rdx
 jmp xchain0_n62_α
 xchain0_n59_β:
 jmp xchain0_n42_α
 xchain0_n60_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1536] -> [zr+1296]
 mov rax, qword ptr [rsp + 1536]
 mov qword ptr [rsp + 1296], rax
 mov rax, qword ptr [rsp + 1544]
 mov qword ptr [rsp + 1304], rax
# marshal arg1 = producer-box slot [zr+1552] -> [zr+1312]
 mov rax, qword ptr [rsp + 1552]
 mov qword ptr [rsp + 1312], rax
 mov rax, qword ptr [rsp + 1560]
 mov qword ptr [rsp + 1320], rax
  .section .rodata
  .Lbynamefn61: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn61]
 lea rsi, [rsp + 1296]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 1280], rax
 mov qword ptr [rsp + 1288], rdx
 cmp eax, 99
 je xchain0_n31_α
 jmp xchain0_n63_α
 xchain0_n60_β:
 jmp xchain0_n31_α
# IR_LIT_STRING
 xchain0_n61_α:
 mov qword ptr [rsp + 1856], 1
 mov rax, qword ptr [rip + .Lx62_0]
 mov qword ptr [rsp + 1864], rax
 jmp xchain0_n64_α
 xchain0_n61_β:
 jmp main_γ
.Lx62_0:
 .quad .Lx62_0_s
.Lx62_0_s:
 .string "FAIL 1112/004: below lower bound fails"
# IR_LIT_STRING
 xchain0_n62_α:
 mov qword ptr [rsp + 2080], 1
 mov rax, qword ptr [rip + .Lx63_0]
 mov qword ptr [rsp + 2088], rax
 jmp xchain0_n65_α
 xchain0_n62_β:
 jmp main_γ
.Lx63_0:
 .quad .Lx63_0_s
.Lx63_0_s:
 .string "FAIL 1112/005: above upper bound fails"
# IR_LIT_STRING
 xchain0_n63_α:
 mov qword ptr [rsp + 1600], 1
 mov rax, qword ptr [rip + .Lx64_0]
 mov qword ptr [rsp + 1608], rax
 jmp xchain0_n66_α
 xchain0_n63_β:
 jmp main_γ
.Lx64_0:
 .quad .Lx64_0_s
.Lx64_0_s:
 .string "FAIL 1112/003: custom lower bound assign/read"
# IR_ASSIGN gva
 xchain0_n64_α:
 mov rax, qword ptr [rsp + 1856]
 mov rdx, qword ptr [rsp + 1864]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rsp + 1840], rax
 mov qword ptr [rsp + 1848], rdx
 jmp main_γ
 xchain0_n64_β:
 jmp main_γ
# IR_ASSIGN gva
 xchain0_n65_α:
 mov rax, qword ptr [rsp + 2080]
 mov rdx, qword ptr [rsp + 2088]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rsp + 2064], rax
 mov qword ptr [rsp + 2072], rdx
 jmp main_γ
 xchain0_n65_β:
 jmp main_γ
# IR_ASSIGN gva
 xchain0_n66_α:
 mov rax, qword ptr [rsp + 1600]
 mov rdx, qword ptr [rsp + 1608]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rsp + 1584], rax
 mov qword ptr [rsp + 1592], rdx
 jmp main_γ
 xchain0_n66_β:
 jmp main_γ
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rsp + 2168]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rsp + 2168]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
add rsp, 65544
ret
