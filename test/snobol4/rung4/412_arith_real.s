  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "output"
  .align 8
__gva_names:
  .quad .Lgvan0
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
  mov edi, 1
  call rt_gva_island@PLT
  mov rsi, rax
  lea rdi, [rip + __gva_names]
  mov edx, 1
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
  mov qword ptr [rsp + 1304], rsp
  mov r12, qword ptr [1879048192]
  mov [rsp + 65536], rbp
  mov rbp, rsp
main_α_body:
# IR_LIT_REAL
 xchain0_n0_α:
 mov qword ptr [rbp + 160], 7
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [rbp + 168], rax
 jmp xchain0_n1_α
.Lx1_0:
 .quad 4617315517961601024
# IR_LIT_REAL
 xchain0_n1_α:
 mov qword ptr [rbp + 192], 7
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [rbp + 200], rax
 jmp xchain0_n2_α
.Lx2_0:
 .quad 4617315517961601024
 xchain0_n2_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+160] -> [zr+112]
 mov rax, qword ptr [rbp + 160]
 mov qword ptr [rbp + 112], rax
 mov rax, qword ptr [rbp + 168]
 mov qword ptr [rbp + 120], rax
# marshal arg1 = producer-box slot [zr+192] -> [zr+128]
 mov rax, qword ptr [rbp + 192]
 mov qword ptr [rbp + 128], rax
 mov rax, qword ptr [rbp + 200]
 mov qword ptr [rbp + 136], rax
  .section .rodata
  .Lbynamefn3: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn3]
 lea rsi, [rbp + 112]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 96], rax
 mov qword ptr [rbp + 104], rdx
 cmp eax, 99
 je xchain0_n4_α
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n4_α
#         output = 'FAIL 412/001: 2.0+3.0'              :(end)
# IR_LIT_STRING
 xchain0_n3_α:
 mov qword ptr [rbp + 240], 1
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [rbp + 248], rax
 jmp xchain0_n5_α
.Lx4_0:
 .quad .Lx4_0_s
.Lx4_0_s:
 .string "FAIL 412/001: 2.0+3.0"
# IR_LIT_REAL
 xchain0_n4_α:
 mov qword ptr [rbp + 336], 7
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [rbp + 344], rax
 jmp xchain0_n6_α
.Lx5_0:
 .quad 4611686018427387904
# IR_ASSIGN gva
 xchain0_n5_α:
 mov rax, qword ptr [rbp + 240]
 mov rdx, qword ptr [rbp + 248]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 224], rax
 mov qword ptr [rbp + 232], rdx
 jmp main_γ
# IR_LIT_REAL
 xchain0_n6_α:
 mov qword ptr [rbp + 368], 7
 mov rax, qword ptr [rip + .Lx7_0]
 mov qword ptr [rbp + 376], rax
 jmp xchain0_n7_α
.Lx7_0:
 .quad 4611686018427387904
 xchain0_n7_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+336] -> [zr+288]
 mov rax, qword ptr [rbp + 336]
 mov qword ptr [rbp + 288], rax
 mov rax, qword ptr [rbp + 344]
 mov qword ptr [rbp + 296], rax
# marshal arg1 = producer-box slot [zr+368] -> [zr+304]
 mov rax, qword ptr [rbp + 368]
 mov qword ptr [rbp + 304], rax
 mov rax, qword ptr [rbp + 376]
 mov qword ptr [rbp + 312], rax
  .section .rodata
  .Lbynamefn8: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn8]
 lea rsi, [rbp + 288]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 272], rax
 mov qword ptr [rbp + 280], rdx
 cmp eax, 99
 je xchain0_n9_α
 jmp xchain0_n8_α
 xchain0_n7_β:
 jmp xchain0_n9_α
#         output = 'FAIL 412/002: 3.0-1.0'              :(end)
# IR_LIT_STRING
 xchain0_n8_α:
 mov qword ptr [rbp + 416], 1
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [rbp + 424], rax
 jmp xchain0_n10_α
.Lx9_0:
 .quad .Lx9_0_s
.Lx9_0_s:
 .string "FAIL 412/002: 3.0-1.0"
# IR_LIT_REAL
 xchain0_n9_α:
 mov qword ptr [rbp + 512], 7
 mov rax, qword ptr [rip + .Lx10_0]
 mov qword ptr [rbp + 520], rax
 jmp xchain0_n11_α
.Lx10_0:
 .quad 4618441417868443648
# IR_ASSIGN gva
 xchain0_n10_α:
 mov rax, qword ptr [rbp + 416]
 mov rdx, qword ptr [rbp + 424]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 400], rax
 mov qword ptr [rbp + 408], rdx
 jmp main_γ
# IR_LIT_REAL
 xchain0_n11_α:
 mov qword ptr [rbp + 544], 7
 mov rax, qword ptr [rip + .Lx12_0]
 mov qword ptr [rbp + 552], rax
 jmp xchain0_n12_α
.Lx12_0:
 .quad 4618441417868443648
 xchain0_n12_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+512] -> [zr+464]
 mov rax, qword ptr [rbp + 512]
 mov qword ptr [rbp + 464], rax
 mov rax, qword ptr [rbp + 520]
 mov qword ptr [rbp + 472], rax
# marshal arg1 = producer-box slot [zr+544] -> [zr+480]
 mov rax, qword ptr [rbp + 544]
 mov qword ptr [rbp + 480], rax
 mov rax, qword ptr [rbp + 552]
 mov qword ptr [rbp + 488], rax
  .section .rodata
  .Lbynamefn13: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn13]
 lea rsi, [rbp + 464]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 448], rax
 mov qword ptr [rbp + 456], rdx
 cmp eax, 99
 je xchain0_n14_α
 jmp xchain0_n13_α
 xchain0_n12_β:
 jmp xchain0_n14_α
#         output = 'FAIL 412/003: 3.0*2.0'              :(end)
# IR_LIT_STRING
 xchain0_n13_α:
 mov qword ptr [rbp + 592], 1
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [rbp + 600], rax
 jmp xchain0_n15_α
.Lx14_0:
 .quad .Lx14_0_s
.Lx14_0_s:
 .string "FAIL 412/003: 3.0*2.0"
# IR_LIT_REAL
 xchain0_n14_α:
 mov qword ptr [rbp + 688], 7
 mov rax, qword ptr [rip + .Lx15_0]
 mov qword ptr [rbp + 696], rax
 jmp xchain0_n16_α
.Lx15_0:
 .quad 4609434218613702656
# IR_ASSIGN gva
 xchain0_n15_α:
 mov rax, qword ptr [rbp + 592]
 mov rdx, qword ptr [rbp + 600]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 576], rax
 mov qword ptr [rbp + 584], rdx
 jmp main_γ
# IR_LIT_REAL
 xchain0_n16_α:
 mov qword ptr [rbp + 720], 7
 mov rax, qword ptr [rip + .Lx17_0]
 mov qword ptr [rbp + 728], rax
 jmp xchain0_n17_α
.Lx17_0:
 .quad 4609434218613702656
 xchain0_n17_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+688] -> [zr+640]
 mov rax, qword ptr [rbp + 688]
 mov qword ptr [rbp + 640], rax
 mov rax, qword ptr [rbp + 696]
 mov qword ptr [rbp + 648], rax
# marshal arg1 = producer-box slot [zr+720] -> [zr+656]
 mov rax, qword ptr [rbp + 720]
 mov qword ptr [rbp + 656], rax
 mov rax, qword ptr [rbp + 728]
 mov qword ptr [rbp + 664], rax
  .section .rodata
  .Lbynamefn18: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn18]
 lea rsi, [rbp + 640]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 624], rax
 mov qword ptr [rbp + 632], rdx
 cmp eax, 99
 je xchain0_n19_α
 jmp xchain0_n18_α
 xchain0_n17_β:
 jmp xchain0_n19_α
#         output = 'FAIL 412/004: 3.0/2.0'              :(end)
# IR_LIT_STRING
 xchain0_n18_α:
 mov qword ptr [rbp + 768], 1
 mov rax, qword ptr [rip + .Lx19_0]
 mov qword ptr [rbp + 776], rax
 jmp xchain0_n20_α
.Lx19_0:
 .quad .Lx19_0_s
.Lx19_0_s:
 .string "FAIL 412/004: 3.0/2.0"
#         differ(3.0 ** 3, 27.0)                   :f(e005)
# IR_LIT_REAL
 xchain0_n19_α:
 mov qword ptr [rbp + 896], 7
 mov rax, qword ptr [rip + .Lx20_0]
 mov qword ptr [rbp + 904], rax
 jmp xchain0_n21_α
.Lx20_0:
 .quad 4613937818241073152
# IR_ASSIGN gva
 xchain0_n20_α:
 mov rax, qword ptr [rbp + 768]
 mov rdx, qword ptr [rbp + 776]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 752], rax
 mov qword ptr [rbp + 760], rdx
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n21_α:
 mov qword ptr [rbp + 928], 6
 mov rax, qword ptr [rip + .Lx22_0]
 mov qword ptr [rbp + 936], rax
 jmp xchain0_n22_α
.Lx22_0:
 .quad 3
 xchain0_n22_α:
# IR_BINOP_ARITH_REAL
 mov rdi, qword ptr [rbp + 896]
 mov rsi, qword ptr [rbp + 904]
 mov rdx, qword ptr [rbp + 928]
 mov rcx, qword ptr [rbp + 936]
 mov r8d, 18
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n24_α
 mov qword ptr [rbp + 864], rax
 mov qword ptr [rbp + 872], rdx
 jmp xchain0_n23_α
# IR_LIT_REAL
 xchain0_n23_α:
 mov qword ptr [rbp + 960], 7
 mov rax, qword ptr [rip + .Lx24_0]
 mov qword ptr [rbp + 968], rax
 jmp xchain0_n25_α
.Lx24_0:
 .quad 4628293042053316608
#         differ(-1.0, 0.0 - 1.0)                   :f(e006)
# IR_LIT_REAL
 xchain0_n24_α:
 mov qword ptr [rbp + 1136], 7
 mov rax, qword ptr [rip + .Lx25_0]
 mov qword ptr [rbp + 1144], rax
 jmp xchain0_n26_α
.Lx25_0:
 .quad 4607182418800017408
 xchain0_n25_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+864] -> [zr+816]
 mov rax, qword ptr [rbp + 864]
 mov qword ptr [rbp + 816], rax
 mov rax, qword ptr [rbp + 872]
 mov qword ptr [rbp + 824], rax
# marshal arg1 = producer-box slot [zr+960] -> [zr+832]
 mov rax, qword ptr [rbp + 960]
 mov qword ptr [rbp + 832], rax
 mov rax, qword ptr [rbp + 968]
 mov qword ptr [rbp + 840], rax
  .section .rodata
  .Lbynamefn26: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn26]
 lea rsi, [rbp + 816]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 800], rax
 mov qword ptr [rbp + 808], rdx
 cmp eax, 99
 je xchain0_n24_α
 jmp xchain0_n27_α
 xchain0_n25_β:
 jmp xchain0_n24_α
# IR_UNOP
 xchain0_n26_α:
 mov rdi, qword ptr [rbp + 1136]
 mov rsi, qword ptr [rbp + 1144]
 call rt_num_neg@PLT
 mov qword ptr [rbp + 1104], rax
 mov qword ptr [rbp + 1112], rdx
 jmp xchain0_n28_α
#         output = 'FAIL 412/005: 3.0**3'               :(end)
# IR_LIT_STRING
 xchain0_n27_α:
 mov qword ptr [rbp + 1008], 1
 mov rax, qword ptr [rip + .Lx28_0]
 mov qword ptr [rbp + 1016], rax
 jmp xchain0_n30_α
.Lx28_0:
 .quad .Lx28_0_s
.Lx28_0_s:
 .string "FAIL 412/005: 3.0**3"
# IR_LIT_REAL
 xchain0_n28_α:
 mov qword ptr [rbp + 1168], 7
 mov rax, qword ptr [rip + .Lx29_0]
 mov qword ptr [rbp + 1176], rax
 jmp xchain0_n31_α
.Lx29_0:
 .quad 13830554455654793216
#         output = 'PASS 412_arith_real (6/6)'
# IR_LIT_STRING
 xchain0_n29_α:
 mov qword ptr [rbp + 1264], 1
 mov rax, qword ptr [rip + .Lx30_0]
 mov qword ptr [rbp + 1272], rax
 jmp xchain0_n32_α
.Lx30_0:
 .quad .Lx30_0_s
.Lx30_0_s:
 .string "PASS 412_arith_real (6/6)"
# IR_ASSIGN gva
 xchain0_n30_α:
 mov rax, qword ptr [rbp + 1008]
 mov rdx, qword ptr [rbp + 1016]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 992], rax
 mov qword ptr [rbp + 1000], rdx
 jmp main_γ
 xchain0_n31_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1104] -> [zr+1056]
 mov rax, qword ptr [rbp + 1104]
 mov qword ptr [rbp + 1056], rax
 mov rax, qword ptr [rbp + 1112]
 mov qword ptr [rbp + 1064], rax
# marshal arg1 = producer-box slot [zr+1168] -> [zr+1072]
 mov rax, qword ptr [rbp + 1168]
 mov qword ptr [rbp + 1072], rax
 mov rax, qword ptr [rbp + 1176]
 mov qword ptr [rbp + 1080], rax
  .section .rodata
  .Lbynamefn32: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn32]
 lea rsi, [rbp + 1056]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 1040], rax
 mov qword ptr [rbp + 1048], rdx
 cmp eax, 99
 je xchain0_n29_α
 jmp xchain0_n33_α
 xchain0_n31_β:
 jmp xchain0_n29_α
# IR_ASSIGN gva
 xchain0_n32_α:
 mov rax, qword ptr [rbp + 1264]
 mov rdx, qword ptr [rbp + 1272]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 1248], rax
 mov qword ptr [rbp + 1256], rdx
 jmp main_γ
#         output = 'FAIL 412/006: unary minus on real'   :(end)
# IR_LIT_STRING
 xchain0_n33_α:
 mov qword ptr [rbp + 1216], 1
 mov rax, qword ptr [rip + .Lx34_0]
 mov qword ptr [rbp + 1224], rax
 jmp xchain0_n34_α
.Lx34_0:
 .quad .Lx34_0_s
.Lx34_0_s:
 .string "FAIL 412/006: unary minus on real"
# IR_ASSIGN gva
 xchain0_n34_α:
 mov rax, qword ptr [rbp + 1216]
 mov rdx, qword ptr [rbp + 1224]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 1200], rax
 mov qword ptr [rbp + 1208], rdx
 jmp main_γ
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rbp + 1304]
mov rbp, [rsp + 65536]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rbp + 1304]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
mov rbp, [rsp + 65536]
add rsp, 65544
ret
