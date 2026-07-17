  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "a"
  .Lgvan1: .string "output"
  .Lgvan2: .string "b"
  .Lgvan3: .string "d"
  .Lgvan4: .string "lfunc"
  .Lgvan5: .string "x"
  .Lgvan6: .string "checklocal"
  .align 8
__gva_names:
  .quad .Lgvan0
  .quad .Lgvan1
  .quad .Lgvan2
  .quad .Lgvan3
  .quad .Lgvan4
  .quad .Lgvan5
  .quad .Lgvan6
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
  mov edi, 7
  call rt_gva_island@PLT
  mov rsi, rax
  lea rdi, [rip + __gva_names]
  mov edx, 7
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
  mov qword ptr [rsp + 2184], rsp
  mov r12, qword ptr [1879048192]
  mov rbp, rsp
main_α_body:
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [rsp + 144], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [rsp + 152], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n2_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "lfunc(a,b,c)d,e,f"
 xchain0_n1_α:
# BOX CALL define(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+144] -> [zr+112]
 mov rax, qword ptr [rsp + 144]
 mov qword ptr [rsp + 112], rax
 mov rax, qword ptr [rsp + 152]
 mov qword ptr [rsp + 120], rax
  .section .rodata
  .Lbynamefn2: .string "define"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn2]
 lea rsi, [rsp + 112]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rsp + 96], rax
 mov qword ptr [rsp + 104], rdx
 cmp eax, 99
 je xchain0_n2_α
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n2_α
# IR_LIT_STRING
 xchain0_n2_α:
 mov qword ptr [rsp + 992], 1
 mov rax, qword ptr [rip + .Lx3_0]
 mov qword ptr [rsp + 1000], rax
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n4_α
.Lx3_0:
 .quad .Lx3_0_s
.Lx3_0_s:
 .string "global_a"
# IR_ASSIGN gva
 xchain0_n3_α:
 mov rax, qword ptr [rsp + 992]
 mov rdx, qword ptr [rsp + 1000]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rsp + 976], rax
 mov qword ptr [rsp + 984], rdx
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp xchain0_n4_α
# IR_LIT_STRING
 xchain0_n4_α:
 mov qword ptr [rsp + 1040], 1
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [rsp + 1048], rax
 jmp xchain0_n5_α
 xchain0_n4_β:
 jmp xchain0_n6_α
.Lx5_0:
 .quad .Lx5_0_s
.Lx5_0_s:
 .string "global_d"
# IR_ASSIGN gva
 xchain0_n5_α:
 mov rax, qword ptr [rsp + 1040]
 mov rdx, qword ptr [rsp + 1048]
 mov qword ptr [1879052336], rax
 mov qword ptr [1879052344], rdx
 mov qword ptr [rsp + 1024], rax
 mov qword ptr [rsp + 1032], rdx
 jmp xchain0_n6_α
 xchain0_n5_β:
 jmp xchain0_n6_α
# IR_LIT_STRING
 xchain0_n6_α:
 mov qword ptr [rsp + 1216], 1
 mov rax, qword ptr [rip + .Lx7_0]
 mov qword ptr [rsp + 1224], rax
 jmp xchain0_n7_α
 xchain0_n6_β:
 jmp xchain0_n11_α
.Lx7_0:
 .quad .Lx7_0_s
.Lx7_0_s:
 .string "p"
# IR_LIT_STRING
 xchain0_n7_α:
 mov qword ptr [rsp + 1248], 1
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [rsp + 1256], rax
 jmp xchain0_n8_α
 xchain0_n7_β:
 jmp xchain0_n11_α
.Lx8_0:
 .quad .Lx8_0_s
.Lx8_0_s:
 .string "q"
# IR_LIT_STRING
 xchain0_n8_α:
 mov qword ptr [rsp + 1280], 1
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [rsp + 1288], rax
 jmp xchain0_n9_α
 xchain0_n8_β:
 jmp xchain0_n11_α
.Lx9_0:
 .quad .Lx9_0_s
.Lx9_0_s:
 .string "r"
 xchain0_n9_α:
# BOX CALL lfunc(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1216] -> [zr+1152]
 mov rax, qword ptr [rsp + 1216]
 mov qword ptr [rsp + 1152], rax
 mov rax, qword ptr [rsp + 1224]
 mov qword ptr [rsp + 1160], rax
# marshal arg1 = producer-box slot [zr+1248] -> [zr+1168]
 mov rax, qword ptr [rsp + 1248]
 mov qword ptr [rsp + 1168], rax
 mov rax, qword ptr [rsp + 1256]
 mov qword ptr [rsp + 1176], rax
# marshal arg2 = producer-box slot [zr+1280] -> [zr+1184]
 mov rax, qword ptr [rsp + 1280]
 mov qword ptr [rsp + 1184], rax
 mov rax, qword ptr [rsp + 1288]
 mov qword ptr [rsp + 1192], rax
  .section .rodata
  .Lbynamefn10: .string "lfunc"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn10]
 lea rsi, [rsp + 1152]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [rsp + 1136], rax
 mov qword ptr [rsp + 1144], rdx
 cmp eax, 99
 je xchain0_n11_α
 jmp xchain0_n10_α
 xchain0_n9_β:
 jmp xchain0_n11_α
# IR_LIT_STRING
 xchain0_n10_α:
 mov qword ptr [rsp + 1312], 1
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [rsp + 1320], rax
 jmp xchain0_n12_α
 xchain0_n10_β:
 jmp xchain0_n11_α
.Lx11_0:
 .quad .Lx11_0_s
.Lx11_0_s:
 .string "aabbdd"
# IR_VAR
 xchain0_n11_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 mov qword ptr [rsp + 1456], rax
 mov qword ptr [rsp + 1464], rdx
 jmp xchain0_n13_α
 xchain0_n11_β:
 jmp xchain0_n14_α
 xchain0_n12_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1136] -> [zr+1088]
 mov rax, qword ptr [rsp + 1136]
 mov qword ptr [rsp + 1088], rax
 mov rax, qword ptr [rsp + 1144]
 mov qword ptr [rsp + 1096], rax
# marshal arg1 = producer-box slot [zr+1312] -> [zr+1104]
 mov rax, qword ptr [rsp + 1312]
 mov qword ptr [rsp + 1104], rax
 mov rax, qword ptr [rsp + 1320]
 mov qword ptr [rsp + 1112], rax
  .section .rodata
  .Lbynamefn13: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn13]
 lea rsi, [rsp + 1088]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 1072], rax
 mov qword ptr [rsp + 1080], rdx
 cmp eax, 99
 je xchain0_n11_α
 jmp xchain0_n15_α
 xchain0_n12_β:
 jmp xchain0_n11_α
# IR_LIT_STRING
 xchain0_n13_α:
 mov qword ptr [rsp + 1488], 1
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [rsp + 1496], rax
 jmp xchain0_n16_α
 xchain0_n13_β:
 jmp xchain0_n14_α
.Lx14_0:
 .quad .Lx14_0_s
.Lx14_0_s:
 .string "global_a"
# IR_LIT_STRING
 xchain0_n14_α:
 mov qword ptr [rsp + 1536], 1
 mov rax, qword ptr [rip + .Lx15_0]
 mov qword ptr [rsp + 1544], rax
 jmp xchain0_n17_α
 xchain0_n14_β:
 jmp main_γ
.Lx15_0:
 .quad .Lx15_0_s
.Lx15_0_s:
 .string "FAIL 1012/005: global a not clobbered"
# IR_LIT_STRING
 xchain0_n15_α:
 mov qword ptr [rsp + 1360], 1
 mov rax, qword ptr [rip + .Lx16_0]
 mov qword ptr [rsp + 1368], rax
 jmp xchain0_n18_α
 xchain0_n15_β:
 jmp main_γ
.Lx16_0:
 .quad .Lx16_0_s
.Lx16_0_s:
 .string "FAIL 1012/004: lfunc return value"
 xchain0_n16_α:
# BOX CALL ident(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1456] -> [zr+1408]
 mov rax, qword ptr [rsp + 1456]
 mov qword ptr [rsp + 1408], rax
 mov rax, qword ptr [rsp + 1464]
 mov qword ptr [rsp + 1416], rax
# marshal arg1 = producer-box slot [zr+1488] -> [zr+1424]
 mov rax, qword ptr [rsp + 1488]
 mov qword ptr [rsp + 1424], rax
 mov rax, qword ptr [rsp + 1496]
 mov qword ptr [rsp + 1432], rax
  .section .rodata
  .Lbynamefn17: .string "ident"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn17]
 lea rsi, [rsp + 1408]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 1392], rax
 mov qword ptr [rsp + 1400], rdx
 cmp eax, 99
 je xchain0_n14_α
 jmp xchain0_n19_α
 xchain0_n16_β:
 jmp xchain0_n14_α
# IR_ASSIGN gva
 xchain0_n17_α:
 mov rax, qword ptr [rsp + 1536]
 mov rdx, qword ptr [rsp + 1544]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rsp + 1520], rax
 mov qword ptr [rsp + 1528], rdx
 jmp main_γ
 xchain0_n17_β:
 jmp main_γ
# IR_ASSIGN gva
 xchain0_n18_α:
 mov rax, qword ptr [rsp + 1360]
 mov rdx, qword ptr [rsp + 1368]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rsp + 1344], rax
 mov qword ptr [rsp + 1352], rdx
 jmp main_γ
 xchain0_n18_β:
 jmp main_γ
# IR_VAR
 xchain0_n19_α:
 mov rax, qword ptr [1879052336]
 mov rdx, qword ptr [1879052344]
 mov qword ptr [rsp + 1632], rax
 mov qword ptr [rsp + 1640], rdx
 jmp xchain0_n20_α
 xchain0_n19_β:
 jmp xchain0_n21_α
# IR_LIT_STRING
 xchain0_n20_α:
 mov qword ptr [rsp + 1664], 1
 mov rax, qword ptr [rip + .Lx21_0]
 mov qword ptr [rsp + 1672], rax
 jmp xchain0_n22_α
 xchain0_n20_β:
 jmp xchain0_n21_α
.Lx21_0:
 .quad .Lx21_0_s
.Lx21_0_s:
 .string "global_d"
# IR_LIT_STRING
 xchain0_n21_α:
 mov qword ptr [rsp + 1712], 1
 mov rax, qword ptr [rip + .Lx22_0]
 mov qword ptr [rsp + 1720], rax
 jmp xchain0_n23_α
 xchain0_n21_β:
 jmp main_γ
.Lx22_0:
 .quad .Lx22_0_s
.Lx22_0_s:
 .string "FAIL 1012/006: global d not clobbered"
 xchain0_n22_α:
# BOX CALL ident(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1632] -> [zr+1584]
 mov rax, qword ptr [rsp + 1632]
 mov qword ptr [rsp + 1584], rax
 mov rax, qword ptr [rsp + 1640]
 mov qword ptr [rsp + 1592], rax
# marshal arg1 = producer-box slot [zr+1664] -> [zr+1600]
 mov rax, qword ptr [rsp + 1664]
 mov qword ptr [rsp + 1600], rax
 mov rax, qword ptr [rsp + 1672]
 mov qword ptr [rsp + 1608], rax
  .section .rodata
  .Lbynamefn23: .string "ident"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn23]
 lea rsi, [rsp + 1584]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 1568], rax
 mov qword ptr [rsp + 1576], rdx
 cmp eax, 99
 je xchain0_n21_α
 jmp xchain0_n24_α
 xchain0_n22_β:
 jmp xchain0_n21_α
# IR_ASSIGN gva
 xchain0_n23_α:
 mov rax, qword ptr [rsp + 1712]
 mov rdx, qword ptr [rsp + 1720]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rsp + 1696], rax
 mov qword ptr [rsp + 1704], rdx
 jmp main_γ
 xchain0_n23_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n24_α:
 mov qword ptr [rsp + 1792], 1
 mov rax, qword ptr [rip + .Lx25_0]
 mov qword ptr [rsp + 1800], rax
 jmp xchain0_n25_α
 xchain0_n24_β:
 jmp xchain0_n26_α
.Lx25_0:
 .quad .Lx25_0_s
.Lx25_0_s:
 .string "checklocal()x"
 xchain0_n25_α:
# BOX CALL define(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1792] -> [zr+1760]
 mov rax, qword ptr [rsp + 1792]
 mov qword ptr [rsp + 1760], rax
 mov rax, qword ptr [rsp + 1800]
 mov qword ptr [rsp + 1768], rax
  .section .rodata
  .Lbynamefn26: .string "define"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn26]
 lea rsi, [rsp + 1760]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rsp + 1744], rax
 mov qword ptr [rsp + 1752], rdx
 cmp eax, 99
 je xchain0_n26_α
 jmp xchain0_n26_α
 xchain0_n25_β:
 jmp xchain0_n26_α
 xchain0_n26_α:
# BOX CALL checklocal(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
  .section .rodata
  .Lbynamefn27: .string "checklocal"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn27]
 lea rsi, [rsp + 2064]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [rsp + 2048], rax
 mov qword ptr [rsp + 2056], rdx
 cmp eax, 99
 je xchain0_n28_α
 jmp xchain0_n27_α
 xchain0_n26_β:
 jmp xchain0_n28_α
 xchain0_n27_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+2048] -> [zr+2016]
 mov rax, qword ptr [rsp + 2048]
 mov qword ptr [rsp + 2016], rax
 mov rax, qword ptr [rsp + 2056]
 mov qword ptr [rsp + 2024], rax
  .section .rodata
  .Lbynamefn28: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn28]
 lea rsi, [rsp + 2016]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rsp + 2000], rax
 mov qword ptr [rsp + 2008], rdx
 cmp eax, 99
 je xchain0_n28_α
 jmp xchain0_n29_α
 xchain0_n27_β:
 jmp xchain0_n28_α
# IR_LIT_STRING
 xchain0_n28_α:
 mov qword ptr [rsp + 2144], 1
 mov rax, qword ptr [rip + .Lx29_0]
 mov qword ptr [rsp + 2152], rax
 jmp xchain0_n30_α
 xchain0_n28_β:
 jmp main_γ
.Lx29_0:
 .quad .Lx29_0_s
.Lx29_0_s:
 .string "PASS 1012_func_locals (7/7)"
# IR_LIT_STRING
 xchain0_n29_α:
 mov qword ptr [rsp + 2096], 1
 mov rax, qword ptr [rip + .Lx30_0]
 mov qword ptr [rsp + 2104], rax
 jmp xchain0_n31_α
 xchain0_n29_β:
 jmp main_γ
.Lx30_0:
 .quad .Lx30_0_s
.Lx30_0_s:
 .string "FAIL 1012/007: local null on fresh call"
# IR_ASSIGN gva
 xchain0_n30_α:
 mov rax, qword ptr [rsp + 2144]
 mov rdx, qword ptr [rsp + 2152]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rsp + 2128], rax
 mov qword ptr [rsp + 2136], rdx
 jmp main_γ
 xchain0_n30_β:
 jmp main_γ
# IR_ASSIGN gva
 xchain0_n31_α:
 mov rax, qword ptr [rsp + 2096]
 mov rdx, qword ptr [rsp + 2104]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rsp + 2080], rax
 mov qword ptr [rsp + 2088], rdx
 jmp main_γ
 xchain0_n31_β:
 jmp main_γ
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rsp + 2184]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rsp + 2184]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
add rsp, 65544
ret
