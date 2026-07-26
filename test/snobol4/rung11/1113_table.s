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
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
  mov edi, 4
  call rt_gva_island@PLT
  mov rsi, rax
  lea rdi, [rip + __gva_names]
  mov edx, 4
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
  mov qword ptr [rsp + 2936], rsp
  mov r12, qword ptr [1879048192]
  mov [rsp + 65536], rbp
  mov rbp, rsp
main_α_body:
#         t = table(10)
# IR_LIT_INTEGER
 xchain0_n0_α:
 mov qword ptr [rbp + 160], 6
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [rbp + 168], rax
 jmp xchain0_n1_α
.Lx1_0:
 .quad 10
 xchain0_n1_α:
# BOX IR_CALL table(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+160] -> [zr+128]
 mov rax, qword ptr [rbp + 160]
 mov qword ptr [rbp + 128], rax
 mov rax, qword ptr [rbp + 168]
 mov qword ptr [rbp + 136], rax
  .section .rodata
  .Lrkfn3: .string "table"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn3]
 lea rsi, [rbp + 128]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 112], rax
 mov qword ptr [rbp + 120], rdx
 cmp eax, 99
 je xchain0_n3_α
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n3_α
# IR_ASSIGN gva
 xchain0_n2_α:
 mov rax, qword ptr [rbp + 112]
 mov rdx, qword ptr [rbp + 120]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 96], rax
 mov qword ptr [rbp + 104], rdx
 jmp xchain0_n3_α
#         t = table(10)
#         differ(t<'cat'>)           :f(e001)
# IR_VAR
 xchain0_n3_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 mov qword ptr [rbp + 240], rax
 mov qword ptr [rbp + 248], rdx
 jmp xchain0_n4_α
# IR_LIT_STRING
 xchain0_n4_α:
 mov qword ptr [rbp + 272], 1
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [rbp + 280], rax
 jmp xchain0_n6_α
.Lx6_0:
 .quad .Lx6_0_s
.Lx6_0_s:
 .string "cat"
#         t<'cat'> = 'dog'
# IR_VAR
 xchain0_n5_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 mov qword ptr [rbp + 400], rax
 mov qword ptr [rbp + 408], rdx
 jmp xchain0_n7_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n6_α:
 mov rdi, qword ptr [rbp + 240]
 mov rsi, qword ptr [rbp + 248]
 mov rdx, qword ptr [rbp + 272]
 mov rcx, qword ptr [rbp + 280]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n5_α
 mov qword ptr [rbp + 304], rax
 mov qword ptr [rbp + 312], rdx
 jmp xchain0_n9_α
# IR_LIT_STRING
 xchain0_n7_α:
 mov qword ptr [rbp + 432], 1
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [rbp + 440], rax
 jmp xchain0_n10_α
.Lx9_0:
 .quad .Lx9_0_s
.Lx9_0_s:
 .string "cat"
#         differ(t<'cat'>, 'dog')                   :f(e002)
# IR_VAR
 xchain0_n8_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 mov qword ptr [rbp + 608], rax
 mov qword ptr [rbp + 616], rdx
 jmp xchain0_n11_α
# IR_DEREF variable -> value
 xchain0_n9_α:
 mov rdi, qword ptr [rbp + 304]
 mov rsi, qword ptr [rbp + 312]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n5_α
 mov qword ptr [rbp + 336], rax
 mov qword ptr [rbp + 344], rdx
 jmp xchain0_n13_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n10_α:
 mov rdi, qword ptr [rbp + 400]
 mov rsi, qword ptr [rbp + 408]
 mov rdx, qword ptr [rbp + 432]
 mov rcx, qword ptr [rbp + 440]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n8_α
 mov qword ptr [rbp + 464], rax
 mov qword ptr [rbp + 472], rdx
 jmp xchain0_n14_α
# IR_LIT_STRING
 xchain0_n11_α:
 mov qword ptr [rbp + 640], 1
 mov rax, qword ptr [rip + .Lx13_0]
 mov qword ptr [rbp + 648], rax
 jmp xchain0_n15_α
.Lx13_0:
 .quad .Lx13_0_s
.Lx13_0_s:
 .string "cat"
#         t<7> = 45
# IR_VAR
 xchain0_n12_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 mov qword ptr [rbp + 800], rax
 mov qword ptr [rbp + 808], rdx
 jmp xchain0_n16_α
 xchain0_n13_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+336] -> [zr+208]
 mov rax, qword ptr [rbp + 336]
 mov qword ptr [rbp + 208], rax
 mov rax, qword ptr [rbp + 344]
 mov qword ptr [rbp + 216], rax
  .section .rodata
  .Lbynamefn14: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn14]
 lea rsi, [rbp + 208]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 192], rax
 mov qword ptr [rbp + 200], rdx
 cmp eax, 99
 je xchain0_n5_α
 jmp xchain0_n18_α
 xchain0_n13_β:
 jmp xchain0_n5_α
# IR_LIT_STRING
 xchain0_n14_α:
 mov qword ptr [rbp + 496], 1
 mov rax, qword ptr [rip + .Lx16_0]
 mov qword ptr [rbp + 504], rax
 jmp xchain0_n19_α
.Lx16_0:
 .quad .Lx16_0_s
.Lx16_0_s:
 .string "dog"
# IR_SUBSCRIPT x[i] variable
 xchain0_n15_α:
 mov rdi, qword ptr [rbp + 608]
 mov rsi, qword ptr [rbp + 616]
 mov rdx, qword ptr [rbp + 640]
 mov rcx, qword ptr [rbp + 648]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n12_α
 mov qword ptr [rbp + 672], rax
 mov qword ptr [rbp + 680], rdx
 jmp xchain0_n20_α
# IR_LIT_INTEGER
 xchain0_n16_α:
 mov qword ptr [rbp + 832], 6
 mov rax, qword ptr [rip + .Lx18_0]
 mov qword ptr [rbp + 840], rax
 jmp xchain0_n21_α
.Lx18_0:
 .quad 7
#         differ(t<7>, 45)                   :f(e003)
# IR_VAR
 xchain0_n17_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 mov qword ptr [rbp + 1008], rax
 mov qword ptr [rbp + 1016], rdx
 jmp xchain0_n22_α
#         output = 'FAIL 1113/001: absent key is null'   :(end)
# IR_LIT_STRING
 xchain0_n18_α:
 mov qword ptr [rbp + 368], 1
 mov rax, qword ptr [rip + .Lx20_0]
 mov qword ptr [rbp + 376], rax
 jmp xchain0_n24_α
.Lx20_0:
 .quad .Lx20_0_s
.Lx20_0_s:
 .string "FAIL 1113/001: absent key is null"
# IR_ASSIGN_VAR
 xchain0_n19_α:
 mov rdi, qword ptr [rbp + 464]
 mov rsi, qword ptr [rbp + 472]
 mov rdx, qword ptr [rbp + 496]
 mov rcx, qword ptr [rbp + 504]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n8_α
 mov qword ptr [rbp + 528], rax
 mov qword ptr [rbp + 536], rdx
 jmp xchain0_n8_α
# IR_DEREF variable -> value
 xchain0_n20_α:
 mov rdi, qword ptr [rbp + 672]
 mov rsi, qword ptr [rbp + 680]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n12_α
 mov qword ptr [rbp + 704], rax
 mov qword ptr [rbp + 712], rdx
 jmp xchain0_n25_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n21_α:
 mov rdi, qword ptr [rbp + 800]
 mov rsi, qword ptr [rbp + 808]
 mov rdx, qword ptr [rbp + 832]
 mov rcx, qword ptr [rbp + 840]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n17_α
 mov qword ptr [rbp + 864], rax
 mov qword ptr [rbp + 872], rdx
 jmp xchain0_n26_α
# IR_LIT_INTEGER
 xchain0_n22_α:
 mov qword ptr [rbp + 1040], 6
 mov rax, qword ptr [rip + .Lx24_0]
 mov qword ptr [rbp + 1048], rax
 jmp xchain0_n27_α
.Lx24_0:
 .quad 7
#         differ(t<'cat'>, 'dog')                   :f(e004)
# IR_VAR
 xchain0_n23_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 mov qword ptr [rbp + 1264], rax
 mov qword ptr [rbp + 1272], rdx
 jmp xchain0_n28_α
# IR_ASSIGN gva
 xchain0_n24_α:
 mov rax, qword ptr [rbp + 368]
 mov rdx, qword ptr [rbp + 376]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rbp + 352], rax
 mov qword ptr [rbp + 360], rdx
 jmp main_γ
# IR_LIT_STRING
 xchain0_n25_α:
 mov qword ptr [rbp + 720], 1
 mov rax, qword ptr [rip + .Lx27_0]
 mov qword ptr [rbp + 728], rax
 jmp xchain0_n30_α
.Lx27_0:
 .quad .Lx27_0_s
.Lx27_0_s:
 .string "dog"
# IR_LIT_INTEGER
 xchain0_n26_α:
 mov qword ptr [rbp + 896], 6
 mov rax, qword ptr [rip + .Lx28_0]
 mov qword ptr [rbp + 904], rax
 jmp xchain0_n31_α
.Lx28_0:
 .quad 45
# IR_SUBSCRIPT x[i] variable
 xchain0_n27_α:
 mov rdi, qword ptr [rbp + 1008]
 mov rsi, qword ptr [rbp + 1016]
 mov rdx, qword ptr [rbp + 1040]
 mov rcx, qword ptr [rbp + 1048]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n23_α
 mov qword ptr [rbp + 1072], rax
 mov qword ptr [rbp + 1080], rdx
 jmp xchain0_n32_α
# IR_LIT_STRING
 xchain0_n28_α:
 mov qword ptr [rbp + 1296], 1
 mov rax, qword ptr [rip + .Lx30_0]
 mov qword ptr [rbp + 1304], rax
 jmp xchain0_n33_α
.Lx30_0:
 .quad .Lx30_0_s
.Lx30_0_s:
 .string "cat"
#         ta = convert(t, 'array')
# IR_VAR
 xchain0_n29_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 mov qword ptr [rbp + 1536], rax
 mov qword ptr [rbp + 1544], rdx
 jmp xchain0_n34_α
 xchain0_n30_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+704] -> [zr+560]
 mov rax, qword ptr [rbp + 704]
 mov qword ptr [rbp + 560], rax
 mov rax, qword ptr [rbp + 712]
 mov qword ptr [rbp + 568], rax
# marshal arg1 = producer-box slot [zr+720] -> [zr+576]
 mov rax, qword ptr [rbp + 720]
 mov qword ptr [rbp + 576], rax
 mov rax, qword ptr [rbp + 728]
 mov qword ptr [rbp + 584], rax
  .section .rodata
  .Lbynamefn31: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn31]
 lea rsi, [rbp + 560]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 544], rax
 mov qword ptr [rbp + 552], rdx
 cmp eax, 99
 je xchain0_n12_α
 jmp xchain0_n36_α
 xchain0_n30_β:
 jmp xchain0_n12_α
# IR_ASSIGN_VAR
 xchain0_n31_α:
 mov rdi, qword ptr [rbp + 864]
 mov rsi, qword ptr [rbp + 872]
 mov rdx, qword ptr [rbp + 896]
 mov rcx, qword ptr [rbp + 904]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n17_α
 mov qword ptr [rbp + 928], rax
 mov qword ptr [rbp + 936], rdx
 jmp xchain0_n17_α
# IR_DEREF variable -> value
 xchain0_n32_α:
 mov rdi, qword ptr [rbp + 1072]
 mov rsi, qword ptr [rbp + 1080]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n23_α
 mov qword ptr [rbp + 1104], rax
 mov qword ptr [rbp + 1112], rdx
 jmp xchain0_n37_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n33_α:
 mov rdi, qword ptr [rbp + 1264]
 mov rsi, qword ptr [rbp + 1272]
 mov rdx, qword ptr [rbp + 1296]
 mov rcx, qword ptr [rbp + 1304]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n29_α
 mov qword ptr [rbp + 1328], rax
 mov qword ptr [rbp + 1336], rdx
 jmp xchain0_n38_α
# IR_LIT_STRING
 xchain0_n34_α:
 mov qword ptr [rbp + 1568], 1
 mov rax, qword ptr [rip + .Lx36_0]
 mov qword ptr [rbp + 1576], rax
 jmp xchain0_n39_α
.Lx36_0:
 .quad .Lx36_0_s
.Lx36_0_s:
 .string "array"
#         differ(prototype(ta), '2,2')                   :f(e005)
# IR_VAR
 xchain0_n35_α:
 mov rax, qword ptr [1879052320]
 mov rdx, qword ptr [1879052328]
 mov qword ptr [rbp + 1712], rax
 mov qword ptr [rbp + 1720], rdx
 jmp xchain0_n40_α
#         output = 'FAIL 1113/002: string key assign/read' :(end)
# IR_LIT_STRING
 xchain0_n36_α:
 mov qword ptr [rbp + 768], 1
 mov rax, qword ptr [rip + .Lx38_0]
 mov qword ptr [rbp + 776], rax
 jmp xchain0_n42_α
.Lx38_0:
 .quad .Lx38_0_s
.Lx38_0_s:
 .string "FAIL 1113/002: string key assign/read"
# IR_LIT_INTEGER
 xchain0_n37_α:
 mov qword ptr [rbp + 1120], 6
 mov rax, qword ptr [rip + .Lx39_0]
 mov qword ptr [rbp + 1128], rax
 jmp xchain0_n43_α
.Lx39_0:
 .quad 45
# IR_DEREF variable -> value
 xchain0_n38_α:
 mov rdi, qword ptr [rbp + 1328]
 mov rsi, qword ptr [rbp + 1336]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n29_α
 mov qword ptr [rbp + 1360], rax
 mov qword ptr [rbp + 1368], rdx
 jmp xchain0_n44_α
 xchain0_n39_α:
# BOX CALL convert(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1536] -> [zr+1488]
 mov rax, qword ptr [rbp + 1536]
 mov qword ptr [rbp + 1488], rax
 mov rax, qword ptr [rbp + 1544]
 mov qword ptr [rbp + 1496], rax
# marshal arg1 = producer-box slot [zr+1568] -> [zr+1504]
 mov rax, qword ptr [rbp + 1568]
 mov qword ptr [rbp + 1504], rax
 mov rax, qword ptr [rbp + 1576]
 mov qword ptr [rbp + 1512], rax
  .section .rodata
  .Lbynamefn40: .string "convert"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn40]
 lea rsi, [rbp + 1488]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 1472], rax
 mov qword ptr [rbp + 1480], rdx
 cmp eax, 99
 je xchain0_n35_α
 jmp xchain0_n45_α
 xchain0_n39_β:
 jmp xchain0_n35_α
 xchain0_n40_α:
# BOX CALL prototype(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1712] -> [zr+1680]
 mov rax, qword ptr [rbp + 1712]
 mov qword ptr [rbp + 1680], rax
 mov rax, qword ptr [rbp + 1720]
 mov qword ptr [rbp + 1688], rax
  .section .rodata
  .Lbynamefn41: .string "prototype"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn41]
 lea rsi, [rbp + 1680]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 1664], rax
 mov qword ptr [rbp + 1672], rdx
 cmp eax, 99
 je xchain0_n41_α
 jmp xchain0_n46_α
 xchain0_n40_β:
 jmp xchain0_n41_α
#         ata = convert(ta, 'table')
# IR_VAR
 xchain0_n41_α:
 mov rax, qword ptr [1879052320]
 mov rdx, qword ptr [1879052328]
 mov qword ptr [rbp + 1904], rax
 mov qword ptr [rbp + 1912], rdx
 jmp xchain0_n47_α
# IR_ASSIGN gva
 xchain0_n42_α:
 mov rax, qword ptr [rbp + 768]
 mov rdx, qword ptr [rbp + 776]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rbp + 752], rax
 mov qword ptr [rbp + 760], rdx
 jmp main_γ
 xchain0_n43_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1104] -> [zr+960]
 mov rax, qword ptr [rbp + 1104]
 mov qword ptr [rbp + 960], rax
 mov rax, qword ptr [rbp + 1112]
 mov qword ptr [rbp + 968], rax
# marshal arg1 = producer-box slot [zr+1120] -> [zr+976]
 mov rax, qword ptr [rbp + 1120]
 mov qword ptr [rbp + 976], rax
 mov rax, qword ptr [rbp + 1128]
 mov qword ptr [rbp + 984], rax
  .section .rodata
  .Lbynamefn44: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn44]
 lea rsi, [rbp + 960]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 944], rax
 mov qword ptr [rbp + 952], rdx
 cmp eax, 99
 je xchain0_n23_α
 jmp xchain0_n49_α
 xchain0_n43_β:
 jmp xchain0_n23_α
# IR_LIT_STRING
 xchain0_n44_α:
 mov qword ptr [rbp + 1376], 1
 mov rax, qword ptr [rip + .Lx46_0]
 mov qword ptr [rbp + 1384], rax
 jmp xchain0_n50_α
.Lx46_0:
 .quad .Lx46_0_s
.Lx46_0_s:
 .string "dog"
# IR_ASSIGN gva
 xchain0_n45_α:
 mov rax, qword ptr [rbp + 1472]
 mov rdx, qword ptr [rbp + 1480]
 mov qword ptr [1879052320], rax
 mov qword ptr [1879052328], rdx
 mov qword ptr [rbp + 1456], rax
 mov qword ptr [rbp + 1464], rdx
 jmp xchain0_n35_α
# IR_LIT_STRING
 xchain0_n46_α:
 mov qword ptr [rbp + 1744], 1
 mov rax, qword ptr [rip + .Lx48_0]
 mov qword ptr [rbp + 1752], rax
 jmp xchain0_n51_α
.Lx48_0:
 .quad .Lx48_0_s
.Lx48_0_s:
 .string "2,2"
# IR_LIT_STRING
 xchain0_n47_α:
 mov qword ptr [rbp + 1936], 1
 mov rax, qword ptr [rip + .Lx49_0]
 mov qword ptr [rbp + 1944], rax
 jmp xchain0_n52_α
.Lx49_0:
 .quad .Lx49_0_s
.Lx49_0_s:
 .string "table"
#         differ(ata<7>, 45)                   :f(e006)
# IR_VAR
 xchain0_n48_α:
 mov rax, qword ptr [1879052336]
 mov rdx, qword ptr [1879052344]
 mov qword ptr [rbp + 2032], rax
 mov qword ptr [rbp + 2040], rdx
 jmp xchain0_n53_α
#         output = 'FAIL 1113/003: integer key assign/read' :(end)
# IR_LIT_STRING
 xchain0_n49_α:
 mov qword ptr [rbp + 1168], 1
 mov rax, qword ptr [rip + .Lx51_0]
 mov qword ptr [rbp + 1176], rax
 jmp xchain0_n55_α
.Lx51_0:
 .quad .Lx51_0_s
.Lx51_0_s:
 .string "FAIL 1113/003: integer key assign/read"
 xchain0_n50_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1360] -> [zr+1216]
 mov rax, qword ptr [rbp + 1360]
 mov qword ptr [rbp + 1216], rax
 mov rax, qword ptr [rbp + 1368]
 mov qword ptr [rbp + 1224], rax
# marshal arg1 = producer-box slot [zr+1376] -> [zr+1232]
 mov rax, qword ptr [rbp + 1376]
 mov qword ptr [rbp + 1232], rax
 mov rax, qword ptr [rbp + 1384]
 mov qword ptr [rbp + 1240], rax
  .section .rodata
  .Lbynamefn51: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn51]
 lea rsi, [rbp + 1216]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 1200], rax
 mov qword ptr [rbp + 1208], rdx
 cmp eax, 99
 je xchain0_n29_α
 jmp xchain0_n56_α
 xchain0_n50_β:
 jmp xchain0_n29_α
 xchain0_n51_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1664] -> [zr+1616]
 mov rax, qword ptr [rbp + 1664]
 mov qword ptr [rbp + 1616], rax
 mov rax, qword ptr [rbp + 1672]
 mov qword ptr [rbp + 1624], rax
# marshal arg1 = producer-box slot [zr+1744] -> [zr+1632]
 mov rax, qword ptr [rbp + 1744]
 mov qword ptr [rbp + 1632], rax
 mov rax, qword ptr [rbp + 1752]
 mov qword ptr [rbp + 1640], rax
  .section .rodata
  .Lbynamefn52: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn52]
 lea rsi, [rbp + 1616]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 1600], rax
 mov qword ptr [rbp + 1608], rdx
 cmp eax, 99
 je xchain0_n41_α
 jmp xchain0_n57_α
 xchain0_n51_β:
 jmp xchain0_n41_α
 xchain0_n52_α:
# BOX CALL convert(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1904] -> [zr+1856]
 mov rax, qword ptr [rbp + 1904]
 mov qword ptr [rbp + 1856], rax
 mov rax, qword ptr [rbp + 1912]
 mov qword ptr [rbp + 1864], rax
# marshal arg1 = producer-box slot [zr+1936] -> [zr+1872]
 mov rax, qword ptr [rbp + 1936]
 mov qword ptr [rbp + 1872], rax
 mov rax, qword ptr [rbp + 1944]
 mov qword ptr [rbp + 1880], rax
  .section .rodata
  .Lbynamefn53: .string "convert"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn53]
 lea rsi, [rbp + 1856]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 1840], rax
 mov qword ptr [rbp + 1848], rdx
 cmp eax, 99
 je xchain0_n48_α
 jmp xchain0_n58_α
 xchain0_n52_β:
 jmp xchain0_n48_α
# IR_LIT_INTEGER
 xchain0_n53_α:
 mov qword ptr [rbp + 2064], 6
 mov rax, qword ptr [rip + .Lx55_0]
 mov qword ptr [rbp + 2072], rax
 jmp xchain0_n59_α
.Lx55_0:
 .quad 7
#         differ(ata<'cat'>, 'dog')                   :f(e007)
# IR_VAR
 xchain0_n54_α:
 mov rax, qword ptr [1879052336]
 mov rdx, qword ptr [1879052344]
 mov qword ptr [rbp + 2288], rax
 mov qword ptr [rbp + 2296], rdx
 jmp xchain0_n60_α
# IR_ASSIGN gva
 xchain0_n55_α:
 mov rax, qword ptr [rbp + 1168]
 mov rdx, qword ptr [rbp + 1176]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rbp + 1152], rax
 mov qword ptr [rbp + 1160], rdx
 jmp main_γ
#         output = 'FAIL 1113/004: string key survives int key add' :(end)
# IR_LIT_STRING
 xchain0_n56_α:
 mov qword ptr [rbp + 1424], 1
 mov rax, qword ptr [rip + .Lx58_0]
 mov qword ptr [rbp + 1432], rax
 jmp xchain0_n62_α
.Lx58_0:
 .quad .Lx58_0_s
.Lx58_0_s:
 .string "FAIL 1113/004: string key survives int key add"
#         output = 'FAIL 1113/005: table->array prototype 2,2' :(end)
# IR_LIT_STRING
 xchain0_n57_α:
 mov qword ptr [rbp + 1792], 1
 mov rax, qword ptr [rip + .Lx59_0]
 mov qword ptr [rbp + 1800], rax
 jmp xchain0_n63_α
.Lx59_0:
 .quad .Lx59_0_s
.Lx59_0_s:
 .string "FAIL 1113/005: table->array prototype 2,2"
# IR_ASSIGN gva
 xchain0_n58_α:
 mov rax, qword ptr [rbp + 1840]
 mov rdx, qword ptr [rbp + 1848]
 mov qword ptr [1879052336], rax
 mov qword ptr [1879052344], rdx
 mov qword ptr [rbp + 1824], rax
 mov qword ptr [rbp + 1832], rdx
 jmp xchain0_n48_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n59_α:
 mov rdi, qword ptr [rbp + 2032]
 mov rsi, qword ptr [rbp + 2040]
 mov rdx, qword ptr [rbp + 2064]
 mov rcx, qword ptr [rbp + 2072]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n54_α
 mov qword ptr [rbp + 2096], rax
 mov qword ptr [rbp + 2104], rdx
 jmp xchain0_n64_α
# IR_LIT_STRING
 xchain0_n60_α:
 mov qword ptr [rbp + 2320], 1
 mov rax, qword ptr [rip + .Lx62_0]
 mov qword ptr [rbp + 2328], rax
 jmp xchain0_n65_α
.Lx62_0:
 .quad .Lx62_0_s
.Lx62_0_s:
 .string "cat"
#         t['cat'] = 'fish'
# IR_VAR
 xchain0_n61_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 mov qword ptr [rbp + 2480], rax
 mov qword ptr [rbp + 2488], rdx
 jmp xchain0_n66_α
# IR_ASSIGN gva
 xchain0_n62_α:
 mov rax, qword ptr [rbp + 1424]
 mov rdx, qword ptr [rbp + 1432]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rbp + 1408], rax
 mov qword ptr [rbp + 1416], rdx
 jmp main_γ
# IR_ASSIGN gva
 xchain0_n63_α:
 mov rax, qword ptr [rbp + 1792]
 mov rdx, qword ptr [rbp + 1800]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rbp + 1776], rax
 mov qword ptr [rbp + 1784], rdx
 jmp main_γ
# IR_DEREF variable -> value
 xchain0_n64_α:
 mov rdi, qword ptr [rbp + 2096]
 mov rsi, qword ptr [rbp + 2104]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n54_α
 mov qword ptr [rbp + 2128], rax
 mov qword ptr [rbp + 2136], rdx
 jmp xchain0_n68_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n65_α:
 mov rdi, qword ptr [rbp + 2288]
 mov rsi, qword ptr [rbp + 2296]
 mov rdx, qword ptr [rbp + 2320]
 mov rcx, qword ptr [rbp + 2328]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n61_α
 mov qword ptr [rbp + 2352], rax
 mov qword ptr [rbp + 2360], rdx
 jmp xchain0_n69_α
# IR_LIT_STRING
 xchain0_n66_α:
 mov qword ptr [rbp + 2512], 1
 mov rax, qword ptr [rip + .Lx68_0]
 mov qword ptr [rbp + 2520], rax
 jmp xchain0_n70_α
.Lx68_0:
 .quad .Lx68_0_s
.Lx68_0_s:
 .string "cat"
#         differ(t<'cat'>, 'fish')                   :f(e008)
# IR_VAR
 xchain0_n67_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 mov qword ptr [rbp + 2688], rax
 mov qword ptr [rbp + 2696], rdx
 jmp xchain0_n71_α
# IR_LIT_INTEGER
 xchain0_n68_α:
 mov qword ptr [rbp + 2144], 6
 mov rax, qword ptr [rip + .Lx70_0]
 mov qword ptr [rbp + 2152], rax
 jmp xchain0_n73_α
.Lx70_0:
 .quad 45
# IR_DEREF variable -> value
 xchain0_n69_α:
 mov rdi, qword ptr [rbp + 2352]
 mov rsi, qword ptr [rbp + 2360]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n61_α
 mov qword ptr [rbp + 2384], rax
 mov qword ptr [rbp + 2392], rdx
 jmp xchain0_n74_α
# IR_SUBSCRIPT x[i] variable
 xchain0_n70_α:
 mov rdi, qword ptr [rbp + 2480]
 mov rsi, qword ptr [rbp + 2488]
 mov rdx, qword ptr [rbp + 2512]
 mov rcx, qword ptr [rbp + 2520]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n67_α
 mov qword ptr [rbp + 2544], rax
 mov qword ptr [rbp + 2552], rdx
 jmp xchain0_n75_α
# IR_LIT_STRING
 xchain0_n71_α:
 mov qword ptr [rbp + 2720], 1
 mov rax, qword ptr [rip + .Lx73_0]
 mov qword ptr [rbp + 2728], rax
 jmp xchain0_n76_α
.Lx73_0:
 .quad .Lx73_0_s
.Lx73_0_s:
 .string "cat"
#         output = 'PASS 1113_table (8/8)'
# IR_LIT_STRING
 xchain0_n72_α:
 mov qword ptr [rbp + 2896], 1
 mov rax, qword ptr [rip + .Lx74_0]
 mov qword ptr [rbp + 2904], rax
 jmp xchain0_n77_α
.Lx74_0:
 .quad .Lx74_0_s
.Lx74_0_s:
 .string "PASS 1113_table (8/8)"
 xchain0_n73_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+2128] -> [zr+1984]
 mov rax, qword ptr [rbp + 2128]
 mov qword ptr [rbp + 1984], rax
 mov rax, qword ptr [rbp + 2136]
 mov qword ptr [rbp + 1992], rax
# marshal arg1 = producer-box slot [zr+2144] -> [zr+2000]
 mov rax, qword ptr [rbp + 2144]
 mov qword ptr [rbp + 2000], rax
 mov rax, qword ptr [rbp + 2152]
 mov qword ptr [rbp + 2008], rax
  .section .rodata
  .Lbynamefn74: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn74]
 lea rsi, [rbp + 1984]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 1968], rax
 mov qword ptr [rbp + 1976], rdx
 cmp eax, 99
 je xchain0_n54_α
 jmp xchain0_n78_α
 xchain0_n73_β:
 jmp xchain0_n54_α
# IR_LIT_STRING
 xchain0_n74_α:
 mov qword ptr [rbp + 2400], 1
 mov rax, qword ptr [rip + .Lx76_0]
 mov qword ptr [rbp + 2408], rax
 jmp xchain0_n79_α
.Lx76_0:
 .quad .Lx76_0_s
.Lx76_0_s:
 .string "dog"
# IR_LIT_STRING
 xchain0_n75_α:
 mov qword ptr [rbp + 2576], 1
 mov rax, qword ptr [rip + .Lx77_0]
 mov qword ptr [rbp + 2584], rax
 jmp xchain0_n80_α
.Lx77_0:
 .quad .Lx77_0_s
.Lx77_0_s:
 .string "fish"
# IR_SUBSCRIPT x[i] variable
 xchain0_n76_α:
 mov rdi, qword ptr [rbp + 2688]
 mov rsi, qword ptr [rbp + 2696]
 mov rdx, qword ptr [rbp + 2720]
 mov rcx, qword ptr [rbp + 2728]
 call rt_subscript_var@PLT
 cmp eax, 99
 je xchain0_n72_α
 mov qword ptr [rbp + 2752], rax
 mov qword ptr [rbp + 2760], rdx
 jmp xchain0_n81_α
# IR_ASSIGN gva
 xchain0_n77_α:
 mov rax, qword ptr [rbp + 2896]
 mov rdx, qword ptr [rbp + 2904]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rbp + 2880], rax
 mov qword ptr [rbp + 2888], rdx
 jmp main_γ
#         output = 'FAIL 1113/006: array->table int key roundtrip' :(end)
# IR_LIT_STRING
 xchain0_n78_α:
 mov qword ptr [rbp + 2192], 1
 mov rax, qword ptr [rip + .Lx80_0]
 mov qword ptr [rbp + 2200], rax
 jmp xchain0_n82_α
.Lx80_0:
 .quad .Lx80_0_s
.Lx80_0_s:
 .string "FAIL 1113/006: array->table int key roundtrip"
 xchain0_n79_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+2384] -> [zr+2240]
 mov rax, qword ptr [rbp + 2384]
 mov qword ptr [rbp + 2240], rax
 mov rax, qword ptr [rbp + 2392]
 mov qword ptr [rbp + 2248], rax
# marshal arg1 = producer-box slot [zr+2400] -> [zr+2256]
 mov rax, qword ptr [rbp + 2400]
 mov qword ptr [rbp + 2256], rax
 mov rax, qword ptr [rbp + 2408]
 mov qword ptr [rbp + 2264], rax
  .section .rodata
  .Lbynamefn80: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn80]
 lea rsi, [rbp + 2240]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 2224], rax
 mov qword ptr [rbp + 2232], rdx
 cmp eax, 99
 je xchain0_n61_α
 jmp xchain0_n83_α
 xchain0_n79_β:
 jmp xchain0_n61_α
# IR_ASSIGN_VAR
 xchain0_n80_α:
 mov rdi, qword ptr [rbp + 2544]
 mov rsi, qword ptr [rbp + 2552]
 mov rdx, qword ptr [rbp + 2576]
 mov rcx, qword ptr [rbp + 2584]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n67_α
 mov qword ptr [rbp + 2608], rax
 mov qword ptr [rbp + 2616], rdx
 jmp xchain0_n67_α
# IR_DEREF variable -> value
 xchain0_n81_α:
 mov rdi, qword ptr [rbp + 2752]
 mov rsi, qword ptr [rbp + 2760]
 call rt_deref@PLT
 cmp eax, 99
 je xchain0_n72_α
 mov qword ptr [rbp + 2784], rax
 mov qword ptr [rbp + 2792], rdx
 jmp xchain0_n84_α
# IR_ASSIGN gva
 xchain0_n82_α:
 mov rax, qword ptr [rbp + 2192]
 mov rdx, qword ptr [rbp + 2200]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rbp + 2176], rax
 mov qword ptr [rbp + 2184], rdx
 jmp main_γ
#         output = 'FAIL 1113/007: array->table string key roundtrip' :(end)
# IR_LIT_STRING
 xchain0_n83_α:
 mov qword ptr [rbp + 2448], 1
 mov rax, qword ptr [rip + .Lx85_0]
 mov qword ptr [rbp + 2456], rax
 jmp xchain0_n85_α
.Lx85_0:
 .quad .Lx85_0_s
.Lx85_0_s:
 .string "FAIL 1113/007: array->table string key roundtrip"
# IR_LIT_STRING
 xchain0_n84_α:
 mov qword ptr [rbp + 2800], 1
 mov rax, qword ptr [rip + .Lx86_0]
 mov qword ptr [rbp + 2808], rax
 jmp xchain0_n86_α
.Lx86_0:
 .quad .Lx86_0_s
.Lx86_0_s:
 .string "fish"
# IR_ASSIGN gva
 xchain0_n85_α:
 mov rax, qword ptr [rbp + 2448]
 mov rdx, qword ptr [rbp + 2456]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rbp + 2432], rax
 mov qword ptr [rbp + 2440], rdx
 jmp main_γ
 xchain0_n86_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+2784] -> [zr+2640]
 mov rax, qword ptr [rbp + 2784]
 mov qword ptr [rbp + 2640], rax
 mov rax, qword ptr [rbp + 2792]
 mov qword ptr [rbp + 2648], rax
# marshal arg1 = producer-box slot [zr+2800] -> [zr+2656]
 mov rax, qword ptr [rbp + 2800]
 mov qword ptr [rbp + 2656], rax
 mov rax, qword ptr [rbp + 2808]
 mov qword ptr [rbp + 2664], rax
  .section .rodata
  .Lbynamefn87: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn87]
 lea rsi, [rbp + 2640]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 2624], rax
 mov qword ptr [rbp + 2632], rdx
 cmp eax, 99
 je xchain0_n72_α
 jmp xchain0_n87_α
 xchain0_n86_β:
 jmp xchain0_n72_α
#         output = 'FAIL 1113/008: [] and <> syntax equivalent' :(end)
# IR_LIT_STRING
 xchain0_n87_α:
 mov qword ptr [rbp + 2848], 1
 mov rax, qword ptr [rip + .Lx89_0]
 mov qword ptr [rbp + 2856], rax
 jmp xchain0_n88_α
.Lx89_0:
 .quad .Lx89_0_s
.Lx89_0_s:
 .string "FAIL 1113/008: [] and <> syntax equivalent"
# IR_ASSIGN gva
 xchain0_n88_α:
 mov rax, qword ptr [rbp + 2848]
 mov rdx, qword ptr [rbp + 2856]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rbp + 2832], rax
 mov qword ptr [rbp + 2840], rdx
 jmp main_γ
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rbp + 2936]
mov rbp, [rsp + 65536]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rbp + 2936]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
mov rbp, [rsp + 65536]
add rsp, 65544
ret
