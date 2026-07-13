  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "fact"
  .Lgvan1: .string "n"
  .Lgvan2: .string "output"
  .Lgvan3: .string "facto"
  .Lgvan4: .string "fact2_entry"
  .Lgvan5: .string "fact2"
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
  mov qword ptr [r12 + 2216], rsp
 push rsi
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, [rsp + 8]
 mov qword ptr [r12 + 2208], rax
 pop rsi
main_α_body:
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [r12 + 144], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 152], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n2_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "fact(n)"
 xchain0_n1_α:
# BOX CALL define(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+144] -> [zr+112]
 mov rax, qword ptr [r12 + 144]
 mov qword ptr [r12 + 112], rax
 mov rax, qword ptr [r12 + 152]
 mov qword ptr [r12 + 120], rax
  .section .rodata
  .Lbynamefn2: .string "define"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn2]
 lea rsi, [r12 + 112]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 cmp eax, 99
 je xchain0_n2_α
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n2_α
# IR_LIT_INTEGER
 xchain0_n2_α:
 mov qword ptr [r12 + 720], 6
 mov rax, qword ptr [rip + .Lx3_0]
 mov qword ptr [r12 + 728], rax
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n5_α
.Lx3_0:
 .quad 5
 xchain0_n3_α:
# BOX CALL fact(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+720] -> [zr+688]
 mov rax, qword ptr [r12 + 720]
 mov qword ptr [r12 + 688], rax
 mov rax, qword ptr [r12 + 728]
 mov qword ptr [r12 + 696], rax
  .section .rodata
  .Lbynamefn4: .string "fact"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn4]
 lea rsi, [r12 + 688]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 672], rax
 mov qword ptr [r12 + 680], rdx
 cmp eax, 99
 je xchain0_n5_α
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp xchain0_n5_α
# IR_LIT_INTEGER
 xchain0_n4_α:
 mov qword ptr [r12 + 752], 6
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [r12 + 760], rax
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp xchain0_n5_α
.Lx5_0:
 .quad 120
# IR_LIT_STRING
 xchain0_n5_α:
 mov qword ptr [r12 + 992], 1
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [r12 + 1000], rax
 jmp xchain0_n7_α
 xchain0_n5_β:
 jmp xchain0_n10_α
.Lx6_0:
 .quad .Lx6_0_s
.Lx6_0_s:
 .string "facto"
 xchain0_n6_α:
# BOX CALL ne(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+672] -> [zr+624]
 mov rax, qword ptr [r12 + 672]
 mov qword ptr [r12 + 624], rax
 mov rax, qword ptr [r12 + 680]
 mov qword ptr [r12 + 632], rax
# marshal arg1 = producer-box slot [zr+752] -> [zr+640]
 mov rax, qword ptr [r12 + 752]
 mov qword ptr [r12 + 640], rax
 mov rax, qword ptr [r12 + 760]
 mov qword ptr [r12 + 648], rax
  .section .rodata
  .Lbynamefn7: .string "ne"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn7]
 lea rsi, [r12 + 624]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 608], rax
 mov qword ptr [r12 + 616], rdx
 cmp eax, 99
 je xchain0_n5_α
 jmp xchain0_n8_α
 xchain0_n6_β:
 jmp xchain0_n5_α
 xchain0_n7_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+992] -> [zr+960]
 mov rax, qword ptr [r12 + 992]
 mov qword ptr [r12 + 960], rax
 mov rax, qword ptr [r12 + 1000]
 mov qword ptr [r12 + 968], rax
  .section .rodata
  .Lrkfn9: .string "SNO$NAME"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn9]
 lea rsi, [r12 + 960]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 944], rax
 mov qword ptr [r12 + 952], rdx
 cmp eax, 99
 je xchain0_n10_α
 jmp xchain0_n9_α
 xchain0_n7_β:
 jmp xchain0_n10_α
# IR_LIT_STRING
 xchain0_n8_α:
 mov qword ptr [r12 + 800], 1
 mov rax, qword ptr [rip + .Lx10_0]
 mov qword ptr [r12 + 808], rax
 jmp xchain0_n11_α
 xchain0_n8_β:
 jmp main_γ
.Lx10_0:
 .quad .Lx10_0_s
.Lx10_0_s:
 .string "FAIL 1010/001: fact(5)=120"
# IR_LIT_STRING
 xchain0_n9_α:
 mov qword ptr [r12 + 1024], 1
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [r12 + 1032], rax
 jmp xchain0_n12_α
 xchain0_n9_β:
 jmp xchain0_n10_α
.Lx11_0:
 .quad .Lx11_0_s
.Lx11_0_s:
 .string "fact"
# IR_LIT_INTEGER
 xchain0_n10_α:
 mov qword ptr [r12 + 1216], 6
 mov rax, qword ptr [rip + .Lx12_0]
 mov qword ptr [r12 + 1224], rax
 jmp xchain0_n13_α
 xchain0_n10_β:
 jmp xchain0_n16_α
.Lx12_0:
 .quad 4
# IR_ASSIGN gva
 xchain0_n11_α:
 mov rax, qword ptr [r12 + 800]
 mov rdx, qword ptr [r12 + 808]
 mov qword ptr [rbx + 32], rax
 mov qword ptr [rbx + 40], rdx
 mov qword ptr [r12 + 784], rax
 mov qword ptr [r12 + 792], rdx
 jmp main_γ
 xchain0_n11_β:
 jmp main_γ
 xchain0_n12_α:
# BOX CALL opsyn(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+944] -> [zr+896]
 mov rax, qword ptr [r12 + 944]
 mov qword ptr [r12 + 896], rax
 mov rax, qword ptr [r12 + 952]
 mov qword ptr [r12 + 904], rax
# marshal arg1 = producer-box slot [zr+1024] -> [zr+912]
 mov rax, qword ptr [r12 + 1024]
 mov qword ptr [r12 + 912], rax
 mov rax, qword ptr [r12 + 1032]
 mov qword ptr [r12 + 920], rax
  .section .rodata
  .Lbynamefn13: .string "opsyn"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn13]
 lea rsi, [r12 + 896]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 880], rax
 mov qword ptr [r12 + 888], rdx
 cmp eax, 99
 je xchain0_n10_α
 jmp xchain0_n14_α
 xchain0_n12_β:
 jmp xchain0_n10_α
 xchain0_n13_α:
# BOX CALL facto(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1216] -> [zr+1184]
 mov rax, qword ptr [r12 + 1216]
 mov qword ptr [r12 + 1184], rax
 mov rax, qword ptr [r12 + 1224]
 mov qword ptr [r12 + 1192], rax
  .section .rodata
  .Lbynamefn14: .string "facto"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn14]
 lea rsi, [r12 + 1184]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1168], rax
 mov qword ptr [r12 + 1176], rdx
 cmp eax, 99
 je xchain0_n16_α
 jmp xchain0_n15_α
 xchain0_n13_β:
 jmp xchain0_n16_α
 xchain0_n14_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+880] -> [zr+848]
 mov rax, qword ptr [r12 + 880]
 mov qword ptr [r12 + 848], rax
 mov rax, qword ptr [r12 + 888]
 mov qword ptr [r12 + 856], rax
  .section .rodata
  .Lbynamefn15: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn15]
 lea rsi, [r12 + 848]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 832], rax
 mov qword ptr [r12 + 840], rdx
 cmp eax, 99
 je xchain0_n10_α
 jmp xchain0_n17_α
 xchain0_n14_β:
 jmp xchain0_n10_α
# IR_LIT_INTEGER
 xchain0_n15_α:
 mov qword ptr [r12 + 1248], 6
 mov rax, qword ptr [rip + .Lx17_0]
 mov qword ptr [r12 + 1256], rax
 jmp xchain0_n18_α
 xchain0_n15_β:
 jmp xchain0_n16_α
.Lx17_0:
 .quad 24
# IR_LIT_STRING
 xchain0_n16_α:
 mov qword ptr [r12 + 1392], 1
 mov rax, qword ptr [rip + .Lx18_0]
 mov qword ptr [r12 + 1400], rax
 jmp xchain0_n19_α
 xchain0_n16_β:
 jmp xchain0_n25_α
.Lx18_0:
 .quad .Lx18_0_s
.Lx18_0_s:
 .string "fact2(n)"
# IR_LIT_STRING
 xchain0_n17_α:
 mov qword ptr [r12 + 1072], 1
 mov rax, qword ptr [rip + .Lx19_0]
 mov qword ptr [r12 + 1080], rax
 jmp xchain0_n20_α
 xchain0_n17_β:
 jmp main_γ
.Lx19_0:
 .quad .Lx19_0_s
.Lx19_0_s:
 .string "FAIL 1010/002: opsyn alias"
 xchain0_n18_α:
# BOX CALL ne(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1168] -> [zr+1120]
 mov rax, qword ptr [r12 + 1168]
 mov qword ptr [r12 + 1120], rax
 mov rax, qword ptr [r12 + 1176]
 mov qword ptr [r12 + 1128], rax
# marshal arg1 = producer-box slot [zr+1248] -> [zr+1136]
 mov rax, qword ptr [r12 + 1248]
 mov qword ptr [r12 + 1136], rax
 mov rax, qword ptr [r12 + 1256]
 mov qword ptr [r12 + 1144], rax
  .section .rodata
  .Lbynamefn19: .string "ne"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn19]
 lea rsi, [r12 + 1120]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1104], rax
 mov qword ptr [r12 + 1112], rdx
 cmp eax, 99
 je xchain0_n16_α
 jmp xchain0_n21_α
 xchain0_n18_β:
 jmp xchain0_n16_α
# IR_LIT_STRING
 xchain0_n19_α:
 mov qword ptr [r12 + 1472], 1
 mov rax, qword ptr [rip + .Lx21_0]
 mov qword ptr [r12 + 1480], rax
 jmp xchain0_n22_α
 xchain0_n19_β:
 jmp xchain0_n25_α
.Lx21_0:
 .quad .Lx21_0_s
.Lx21_0_s:
 .string "fact2_entry"
# IR_ASSIGN gva
 xchain0_n20_α:
 mov rax, qword ptr [r12 + 1072]
 mov rdx, qword ptr [r12 + 1080]
 mov qword ptr [rbx + 32], rax
 mov qword ptr [rbx + 40], rdx
 mov qword ptr [r12 + 1056], rax
 mov qword ptr [r12 + 1064], rdx
 jmp main_γ
 xchain0_n20_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n21_α:
 mov qword ptr [r12 + 1296], 1
 mov rax, qword ptr [rip + .Lx23_0]
 mov qword ptr [r12 + 1304], rax
 jmp xchain0_n23_α
 xchain0_n21_β:
 jmp main_γ
.Lx23_0:
 .quad .Lx23_0_s
.Lx23_0_s:
 .string "FAIL 1010/003: facto(4)=24 via alias"
 xchain0_n22_α:
# BOX IR_CALL SNO$NAME(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+1472] -> [zr+1440]
 mov rax, qword ptr [r12 + 1472]
 mov qword ptr [r12 + 1440], rax
 mov rax, qword ptr [r12 + 1480]
 mov qword ptr [r12 + 1448], rax
  .section .rodata
  .Lrkfn25: .string "SNO$NAME"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn25]
 lea rsi, [r12 + 1440]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1424], rax
 mov qword ptr [r12 + 1432], rdx
 cmp eax, 99
 je xchain0_n25_α
 jmp xchain0_n24_α
 xchain0_n22_β:
 jmp xchain0_n25_α
# IR_ASSIGN gva
 xchain0_n23_α:
 mov rax, qword ptr [r12 + 1296]
 mov rdx, qword ptr [r12 + 1304]
 mov qword ptr [rbx + 32], rax
 mov qword ptr [rbx + 40], rdx
 mov qword ptr [r12 + 1280], rax
 mov qword ptr [r12 + 1288], rdx
 jmp main_γ
 xchain0_n23_β:
 jmp main_γ
 xchain0_n24_α:
# BOX CALL define(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1392] -> [zr+1344]
 mov rax, qword ptr [r12 + 1392]
 mov qword ptr [r12 + 1344], rax
 mov rax, qword ptr [r12 + 1400]
 mov qword ptr [r12 + 1352], rax
# marshal arg1 = producer-box slot [zr+1424] -> [zr+1360]
 mov rax, qword ptr [r12 + 1424]
 mov qword ptr [r12 + 1360], rax
 mov rax, qword ptr [r12 + 1432]
 mov qword ptr [r12 + 1368], rax
  .section .rodata
  .Lbynamefn25: .string "define"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn25]
 lea rsi, [r12 + 1344]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1328], rax
 mov qword ptr [r12 + 1336], rdx
 cmp eax, 99
 je xchain0_n25_α
 jmp xchain0_n25_α
 xchain0_n24_β:
 jmp xchain0_n25_α
# IR_LIT_INTEGER
 xchain0_n25_α:
 mov qword ptr [r12 + 2048], 6
 mov rax, qword ptr [rip + .Lx28_0]
 mov qword ptr [r12 + 2056], rax
 jmp xchain0_n26_α
 xchain0_n25_β:
 jmp xchain0_n28_α
.Lx28_0:
 .quad 6
 xchain0_n26_α:
# BOX CALL fact2(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+2048] -> [zr+2016]
 mov rax, qword ptr [r12 + 2048]
 mov qword ptr [r12 + 2016], rax
 mov rax, qword ptr [r12 + 2056]
 mov qword ptr [r12 + 2024], rax
  .section .rodata
  .Lbynamefn27: .string "fact2"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn27]
 lea rsi, [r12 + 2016]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 2000], rax
 mov qword ptr [r12 + 2008], rdx
 cmp eax, 99
 je xchain0_n28_α
 jmp xchain0_n27_α
 xchain0_n26_β:
 jmp xchain0_n28_α
# IR_LIT_INTEGER
 xchain0_n27_α:
 mov qword ptr [r12 + 2080], 6
 mov rax, qword ptr [rip + .Lx30_0]
 mov qword ptr [r12 + 2088], rax
 jmp xchain0_n29_α
 xchain0_n27_β:
 jmp xchain0_n28_α
.Lx30_0:
 .quad 720
# IR_LIT_STRING
 xchain0_n28_α:
 mov qword ptr [r12 + 2176], 1
 mov rax, qword ptr [rip + .Lx31_0]
 mov qword ptr [r12 + 2184], rax
 jmp xchain0_n30_α
 xchain0_n28_β:
 jmp main_γ
.Lx31_0:
 .quad .Lx31_0_s
.Lx31_0_s:
 .string "PASS 1010_func_recursion (4/4)"
 xchain0_n29_α:
# BOX CALL ne(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+2000] -> [zr+1952]
 mov rax, qword ptr [r12 + 2000]
 mov qword ptr [r12 + 1952], rax
 mov rax, qword ptr [r12 + 2008]
 mov qword ptr [r12 + 1960], rax
# marshal arg1 = producer-box slot [zr+2080] -> [zr+1968]
 mov rax, qword ptr [r12 + 2080]
 mov qword ptr [r12 + 1968], rax
 mov rax, qword ptr [r12 + 2088]
 mov qword ptr [r12 + 1976], rax
  .section .rodata
  .Lbynamefn30: .string "ne"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn30]
 lea rsi, [r12 + 1952]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 1936], rax
 mov qword ptr [r12 + 1944], rdx
 cmp eax, 99
 je xchain0_n28_α
 jmp xchain0_n31_α
 xchain0_n29_β:
 jmp xchain0_n28_α
# IR_ASSIGN gva
 xchain0_n30_α:
 mov rax, qword ptr [r12 + 2176]
 mov rdx, qword ptr [r12 + 2184]
 mov qword ptr [rbx + 32], rax
 mov qword ptr [rbx + 40], rdx
 mov qword ptr [r12 + 2160], rax
 mov qword ptr [r12 + 2168], rdx
 jmp main_γ
 xchain0_n30_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n31_α:
 mov qword ptr [r12 + 2128], 1
 mov rax, qword ptr [rip + .Lx34_0]
 mov qword ptr [r12 + 2136], rax
 jmp xchain0_n32_α
 xchain0_n31_β:
 jmp main_γ
.Lx34_0:
 .quad .Lx34_0_s
.Lx34_0_s:
 .string "FAIL 1010/004: fact2(6)=720 alt entry"
# IR_ASSIGN gva
 xchain0_n32_α:
 mov rax, qword ptr [r12 + 2128]
 mov rdx, qword ptr [r12 + 2136]
 mov qword ptr [rbx + 32], rax
 mov qword ptr [rbx + 40], rdx
 mov qword ptr [r12 + 2112], rax
 mov qword ptr [r12 + 2120], rdx
 jmp main_γ
 xchain0_n32_β:
 jmp main_γ
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [r12 + 2216]
pop r12
ret
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [r12 + 2208]
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
main_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, qword ptr [r12 + 2216]
pop r12
ret
