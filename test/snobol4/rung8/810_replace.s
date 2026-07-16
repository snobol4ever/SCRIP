  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "output"
  .Lgvan1: .string "a"
  .align 8
__gva_names:
  .quad .Lgvan0
  .quad .Lgvan1
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
  mov edi, 2
  call rt_gva_island@PLT
  mov rsi, rax
  lea rdi, [rip + __gva_names]
  mov edx, 2
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
  mov qword ptr [rsp + 1272], rsp
main_α_body:
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [rsp + 240], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [rsp + 248], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n5_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "axxbyyy"
# IR_LIT_STRING
 xchain0_n1_α:
 mov qword ptr [rsp + 272], 1
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [rsp + 280], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n5_α
.Lx2_0:
 .quad .Lx2_0_s
.Lx2_0_s:
 .string "xy"
# IR_LIT_STRING
 xchain0_n2_α:
 mov qword ptr [rsp + 304], 1
 mov rax, qword ptr [rip + .Lx3_0]
 mov qword ptr [rsp + 312], rax
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n5_α
.Lx3_0:
 .quad .Lx3_0_s
.Lx3_0_s:
 .string "01"
 xchain0_n3_α:
# BOX CALL replace(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+240] -> [zr+176]
 mov rax, qword ptr [rsp + 240]
 mov qword ptr [rsp + 176], rax
 mov rax, qword ptr [rsp + 248]
 mov qword ptr [rsp + 184], rax
# marshal arg1 = producer-box slot [zr+272] -> [zr+192]
 mov rax, qword ptr [rsp + 272]
 mov qword ptr [rsp + 192], rax
 mov rax, qword ptr [rsp + 280]
 mov qword ptr [rsp + 200], rax
# marshal arg2 = producer-box slot [zr+304] -> [zr+208]
 mov rax, qword ptr [rsp + 304]
 mov qword ptr [rsp + 208], rax
 mov rax, qword ptr [rsp + 312]
 mov qword ptr [rsp + 216], rax
  .section .rodata
  .Lbynamefn4: .string "replace"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn4]
 lea rsi, [rsp + 176]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [rsp + 160], rax
 mov qword ptr [rsp + 168], rdx
 cmp eax, 99
 je xchain0_n5_α
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp xchain0_n5_α
# IR_LIT_STRING
 xchain0_n4_α:
 mov qword ptr [rsp + 336], 1
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [rsp + 344], rax
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp xchain0_n5_α
.Lx5_0:
 .quad .Lx5_0_s
.Lx5_0_s:
 .string "a00b111"
# IR_KEYWORD_SNOBOL4_read
 xchain0_n5_α:
 mov rdi, qword ptr [rip + .Lx6_0]
 call rt_keyword_read_snobol4@PLT
 mov qword ptr [rsp + 512], rax
 mov qword ptr [rsp + 520], rdx
 jmp xchain0_n7_α
 xchain0_n5_β:
 jmp xchain0_n13_α
.Lx6_0:
 .quad .Lx6_0_s
.Lx6_0_s:
 .string "alphabet"
 xchain0_n6_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+160] -> [zr+112]
 mov rax, qword ptr [rsp + 160]
 mov qword ptr [rsp + 112], rax
 mov rax, qword ptr [rsp + 168]
 mov qword ptr [rsp + 120], rax
# marshal arg1 = producer-box slot [zr+336] -> [zr+128]
 mov rax, qword ptr [rsp + 336]
 mov qword ptr [rsp + 128], rax
 mov rax, qword ptr [rsp + 344]
 mov qword ptr [rsp + 136], rax
  .section .rodata
  .Lbynamefn7: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn7]
 lea rsi, [rsp + 112]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 96], rax
 mov qword ptr [rsp + 104], rdx
 cmp eax, 99
 je xchain0_n5_α
 jmp xchain0_n8_α
 xchain0_n6_β:
 jmp xchain0_n5_α
# IR_LIT_STRING
 xchain0_n7_α:
 mov qword ptr [rsp + 528], 1
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [rsp + 536], rax
 jmp xchain0_n9_α
 xchain0_n7_β:
 jmp xchain0_n13_α
.Lx8_0:
 .quad .Lx8_0_s
.Lx8_0_s:
 .string "xy"
# IR_LIT_STRING
 xchain0_n8_α:
 mov qword ptr [rsp + 384], 1
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [rsp + 392], rax
 jmp xchain0_n10_α
 xchain0_n8_β:
 jmp main_γ
.Lx9_0:
 .quad .Lx9_0_s
.Lx9_0_s:
 .string "FAIL 810/001: xy->01 mapping"
# IR_LIT_STRING
 xchain0_n9_α:
 mov qword ptr [rsp + 560], 1
 mov rax, qword ptr [rip + .Lx10_0]
 mov qword ptr [rsp + 568], rax
 jmp xchain0_n11_α
 xchain0_n9_β:
 jmp xchain0_n13_α
.Lx10_0:
 .quad .Lx10_0_s
.Lx10_0_s:
 .string "ab"
# IR_ASSIGN gva
 xchain0_n10_α:
 mov rax, qword ptr [rsp + 384]
 mov rdx, qword ptr [rsp + 392]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rsp + 368], rax
 mov qword ptr [rsp + 376], rdx
 jmp main_γ
 xchain0_n10_β:
 jmp main_γ
 xchain0_n11_α:
# BOX CALL replace(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+512] -> [zr+448]
 mov rax, qword ptr [rsp + 512]
 mov qword ptr [rsp + 448], rax
 mov rax, qword ptr [rsp + 520]
 mov qword ptr [rsp + 456], rax
# marshal arg1 = producer-box slot [zr+528] -> [zr+464]
 mov rax, qword ptr [rsp + 528]
 mov qword ptr [rsp + 464], rax
 mov rax, qword ptr [rsp + 536]
 mov qword ptr [rsp + 472], rax
# marshal arg2 = producer-box slot [zr+560] -> [zr+480]
 mov rax, qword ptr [rsp + 560]
 mov qword ptr [rsp + 480], rax
 mov rax, qword ptr [rsp + 568]
 mov qword ptr [rsp + 488], rax
  .section .rodata
  .Lbynamefn12: .string "replace"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn12]
 lea rsi, [rsp + 448]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [rsp + 432], rax
 mov qword ptr [rsp + 440], rdx
 cmp eax, 99
 je xchain0_n13_α
 jmp xchain0_n12_α
 xchain0_n11_β:
 jmp xchain0_n13_α
# IR_ASSIGN gva
 xchain0_n12_α:
 mov rax, qword ptr [rsp + 432]
 mov rdx, qword ptr [rsp + 440]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rsp + 416], rax
 mov qword ptr [rsp + 424], rdx
 jmp xchain0_n13_α
 xchain0_n12_β:
 jmp xchain0_n13_α
# IR_LIT_STRING
 xchain0_n13_α:
 mov qword ptr [rsp + 736], 1
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [rsp + 744], rax
 jmp xchain0_n14_α
 xchain0_n13_β:
 jmp xchain0_n17_α
.Lx14_0:
 .quad .Lx14_0_s
.Lx14_0_s:
 .string "axy"
# IR_KEYWORD_SNOBOL4_read
 xchain0_n14_α:
 mov rdi, qword ptr [rip + .Lx15_0]
 call rt_keyword_read_snobol4@PLT
 mov qword ptr [rsp + 768], rax
 mov qword ptr [rsp + 776], rdx
 jmp xchain0_n15_α
 xchain0_n14_β:
 jmp xchain0_n17_α
.Lx15_0:
 .quad .Lx15_0_s
.Lx15_0_s:
 .string "alphabet"
# IR_VAR
 xchain0_n15_α:
 mov rax, qword ptr [1879052304]
 mov rdx, qword ptr [1879052312]
 mov qword ptr [rsp + 784], rax
 mov qword ptr [rsp + 792], rdx
 jmp xchain0_n16_α
 xchain0_n15_β:
 jmp xchain0_n17_α
 xchain0_n16_α:
# BOX CALL replace(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+736] -> [zr+672]
 mov rax, qword ptr [rsp + 736]
 mov qword ptr [rsp + 672], rax
 mov rax, qword ptr [rsp + 744]
 mov qword ptr [rsp + 680], rax
# marshal arg1 = producer-box slot [zr+768] -> [zr+688]
 mov rax, qword ptr [rsp + 768]
 mov qword ptr [rsp + 688], rax
 mov rax, qword ptr [rsp + 776]
 mov qword ptr [rsp + 696], rax
# marshal arg2 = producer-box slot [zr+784] -> [zr+704]
 mov rax, qword ptr [rsp + 784]
 mov qword ptr [rsp + 704], rax
 mov rax, qword ptr [rsp + 792]
 mov qword ptr [rsp + 712], rax
  .section .rodata
  .Lbynamefn17: .string "replace"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn17]
 lea rsi, [rsp + 672]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [rsp + 656], rax
 mov qword ptr [rsp + 664], rdx
 cmp eax, 99
 je xchain0_n17_α
 jmp xchain0_n18_α
 xchain0_n16_β:
 jmp xchain0_n17_α
# IR_LIT_STRING
 xchain0_n17_α:
 mov qword ptr [rsp + 1040], 1
 mov rax, qword ptr [rip + .Lx18_0]
 mov qword ptr [rsp + 1048], rax
 jmp xchain0_n19_α
 xchain0_n17_β:
 jmp xchain0_n26_α
.Lx18_0:
 .quad .Lx18_0_s
.Lx18_0_s:
 .string "hello"
# IR_LIT_STRING
 xchain0_n18_α:
 mov qword ptr [rsp + 816], 1
 mov rax, qword ptr [rip + .Lx19_0]
 mov qword ptr [rsp + 824], rax
 jmp xchain0_n20_α
 xchain0_n18_β:
 jmp xchain0_n17_α
.Lx19_0:
 .quad .Lx19_0_s
.Lx19_0_s:
 .string "aab"
# IR_LIT_STRING
 xchain0_n19_α:
 mov qword ptr [rsp + 1072], 1
 mov rax, qword ptr [rip + .Lx20_0]
 mov qword ptr [rsp + 1080], rax
 jmp xchain0_n21_α
 xchain0_n19_β:
 jmp xchain0_n26_α
.Lx20_0:
 .quad .Lx20_0_s
.Lx20_0_s:
 .string "aeiou"
 xchain0_n20_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+656] -> [zr+608]
 mov rax, qword ptr [rsp + 656]
 mov qword ptr [rsp + 608], rax
 mov rax, qword ptr [rsp + 664]
 mov qword ptr [rsp + 616], rax
# marshal arg1 = producer-box slot [zr+816] -> [zr+624]
 mov rax, qword ptr [rsp + 816]
 mov qword ptr [rsp + 624], rax
 mov rax, qword ptr [rsp + 824]
 mov qword ptr [rsp + 632], rax
  .section .rodata
  .Lbynamefn21: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn21]
 lea rsi, [rsp + 608]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 592], rax
 mov qword ptr [rsp + 600], rdx
 cmp eax, 99
 je xchain0_n17_α
 jmp xchain0_n22_α
 xchain0_n20_β:
 jmp xchain0_n17_α
# IR_LIT_STRING
 xchain0_n21_α:
 mov qword ptr [rsp + 1104], 1
 mov rax, qword ptr [rip + .Lx22_0]
 mov qword ptr [rsp + 1112], rax
 jmp xchain0_n23_α
 xchain0_n21_β:
 jmp xchain0_n26_α
.Lx22_0:
 .quad .Lx22_0_s
.Lx22_0_s:
 .string "aeiou"
# IR_LIT_STRING
 xchain0_n22_α:
 mov qword ptr [rsp + 864], 1
 mov rax, qword ptr [rip + .Lx23_0]
 mov qword ptr [rsp + 872], rax
 jmp xchain0_n24_α
 xchain0_n22_β:
 jmp main_γ
.Lx23_0:
 .quad .Lx23_0_s
.Lx23_0_s:
 .string "FAIL 810/002: alphabet translation"
 xchain0_n23_α:
# BOX CALL replace(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1040] -> [zr+976]
 mov rax, qword ptr [rsp + 1040]
 mov qword ptr [rsp + 976], rax
 mov rax, qword ptr [rsp + 1048]
 mov qword ptr [rsp + 984], rax
# marshal arg1 = producer-box slot [zr+1072] -> [zr+992]
 mov rax, qword ptr [rsp + 1072]
 mov qword ptr [rsp + 992], rax
 mov rax, qword ptr [rsp + 1080]
 mov qword ptr [rsp + 1000], rax
# marshal arg2 = producer-box slot [zr+1104] -> [zr+1008]
 mov rax, qword ptr [rsp + 1104]
 mov qword ptr [rsp + 1008], rax
 mov rax, qword ptr [rsp + 1112]
 mov qword ptr [rsp + 1016], rax
  .section .rodata
  .Lbynamefn24: .string "replace"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn24]
 lea rsi, [rsp + 976]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [rsp + 960], rax
 mov qword ptr [rsp + 968], rdx
 cmp eax, 99
 je xchain0_n26_α
 jmp xchain0_n25_α
 xchain0_n23_β:
 jmp xchain0_n26_α
# IR_ASSIGN gva
 xchain0_n24_α:
 mov rax, qword ptr [rsp + 864]
 mov rdx, qword ptr [rsp + 872]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rsp + 848], rax
 mov qword ptr [rsp + 856], rdx
 jmp main_γ
 xchain0_n24_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n25_α:
 mov qword ptr [rsp + 1136], 1
 mov rax, qword ptr [rip + .Lx26_0]
 mov qword ptr [rsp + 1144], rax
 jmp xchain0_n27_α
 xchain0_n25_β:
 jmp xchain0_n26_α
.Lx26_0:
 .quad .Lx26_0_s
.Lx26_0_s:
 .string "hello"
# IR_LIT_STRING
 xchain0_n26_α:
 mov qword ptr [rsp + 1232], 1
 mov rax, qword ptr [rip + .Lx27_0]
 mov qword ptr [rsp + 1240], rax
 jmp xchain0_n28_α
 xchain0_n26_β:
 jmp main_γ
.Lx27_0:
 .quad .Lx27_0_s
.Lx27_0_s:
 .string "PASS 810_replace (3/3)"
 xchain0_n27_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+960] -> [zr+912]
 mov rax, qword ptr [rsp + 960]
 mov qword ptr [rsp + 912], rax
 mov rax, qword ptr [rsp + 968]
 mov qword ptr [rsp + 920], rax
# marshal arg1 = producer-box slot [zr+1136] -> [zr+928]
 mov rax, qword ptr [rsp + 1136]
 mov qword ptr [rsp + 928], rax
 mov rax, qword ptr [rsp + 1144]
 mov qword ptr [rsp + 936], rax
  .section .rodata
  .Lbynamefn28: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn28]
 lea rsi, [rsp + 912]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 896], rax
 mov qword ptr [rsp + 904], rdx
 cmp eax, 99
 je xchain0_n26_α
 jmp xchain0_n29_α
 xchain0_n27_β:
 jmp xchain0_n26_α
# IR_ASSIGN gva
 xchain0_n28_α:
 mov rax, qword ptr [rsp + 1232]
 mov rdx, qword ptr [rsp + 1240]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rsp + 1216], rax
 mov qword ptr [rsp + 1224], rdx
 jmp main_γ
 xchain0_n28_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n29_α:
 mov qword ptr [rsp + 1184], 1
 mov rax, qword ptr [rip + .Lx30_0]
 mov qword ptr [rsp + 1192], rax
 jmp xchain0_n30_α
 xchain0_n29_β:
 jmp main_γ
.Lx30_0:
 .quad .Lx30_0_s
.Lx30_0_s:
 .string "FAIL 810/003: identity replace"
# IR_ASSIGN gva
 xchain0_n30_α:
 mov rax, qword ptr [rsp + 1184]
 mov rdx, qword ptr [rsp + 1192]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rsp + 1168], rax
 mov qword ptr [rsp + 1176], rdx
 jmp main_γ
 xchain0_n30_β:
 jmp main_γ
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rsp + 1272]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rsp + 1272]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
add rsp, 65544
ret
