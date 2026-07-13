  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "output"
  .Lgvan1: .string "a"
  .align 8
__gva_names:
  .quad .Lgvan0
  .quad .Lgvan1
  .section .bss
  .align 16
__gva: .space 32, 0
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
  mov edx, 2
  call gva_register@PLT
  mov rbx, rax
  call rt_frame@PLT
  mov rdi, rax
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
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  mov qword ptr [r12 + 1272], rsp
 push rsi
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, [rsp + 8]
 mov qword ptr [r12 + 1264], rax
 pop rsi
main_α_body:
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [r12 + 240], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 248], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n5_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "axxbyyy"
# IR_LIT_STRING
 xchain0_n1_α:
 mov qword ptr [r12 + 272], 1
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [r12 + 280], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n5_α
.Lx2_0:
 .quad .Lx2_0_s
.Lx2_0_s:
 .string "xy"
# IR_LIT_STRING
 xchain0_n2_α:
 mov qword ptr [r12 + 304], 1
 mov rax, qword ptr [rip + .Lx3_0]
 mov qword ptr [r12 + 312], rax
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
 mov rax, qword ptr [r12 + 240]
 mov qword ptr [r12 + 176], rax
 mov rax, qword ptr [r12 + 248]
 mov qword ptr [r12 + 184], rax
# marshal arg1 = producer-box slot [zr+272] -> [zr+192]
 mov rax, qword ptr [r12 + 272]
 mov qword ptr [r12 + 192], rax
 mov rax, qword ptr [r12 + 280]
 mov qword ptr [r12 + 200], rax
# marshal arg2 = producer-box slot [zr+304] -> [zr+208]
 mov rax, qword ptr [r12 + 304]
 mov qword ptr [r12 + 208], rax
 mov rax, qword ptr [r12 + 312]
 mov qword ptr [r12 + 216], rax
  .section .rodata
  .Lbynamefn4: .string "replace"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn4]
 lea rsi, [r12 + 176]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [r12 + 160], rax
 mov qword ptr [r12 + 168], rdx
 cmp eax, 99
 je xchain0_n5_α
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp xchain0_n5_α
# IR_LIT_STRING
 xchain0_n4_α:
 mov qword ptr [r12 + 336], 1
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [r12 + 344], rax
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
 mov qword ptr [r12 + 512], rax
 mov qword ptr [r12 + 520], rdx
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
 mov rax, qword ptr [r12 + 160]
 mov qword ptr [r12 + 112], rax
 mov rax, qword ptr [r12 + 168]
 mov qword ptr [r12 + 120], rax
# marshal arg1 = producer-box slot [zr+336] -> [zr+128]
 mov rax, qword ptr [r12 + 336]
 mov qword ptr [r12 + 128], rax
 mov rax, qword ptr [r12 + 344]
 mov qword ptr [r12 + 136], rax
  .section .rodata
  .Lbynamefn7: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn7]
 lea rsi, [r12 + 112]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 cmp eax, 99
 je xchain0_n5_α
 jmp xchain0_n8_α
 xchain0_n6_β:
 jmp xchain0_n5_α
# IR_LIT_STRING
 xchain0_n7_α:
 mov qword ptr [r12 + 528], 1
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [r12 + 536], rax
 jmp xchain0_n9_α
 xchain0_n7_β:
 jmp xchain0_n13_α
.Lx8_0:
 .quad .Lx8_0_s
.Lx8_0_s:
 .string "xy"
# IR_LIT_STRING
 xchain0_n8_α:
 mov qword ptr [r12 + 384], 1
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [r12 + 392], rax
 jmp xchain0_n10_α
 xchain0_n8_β:
 jmp main_γ
.Lx9_0:
 .quad .Lx9_0_s
.Lx9_0_s:
 .string "FAIL 810/001: xy->01 mapping"
# IR_LIT_STRING
 xchain0_n9_α:
 mov qword ptr [r12 + 560], 1
 mov rax, qword ptr [rip + .Lx10_0]
 mov qword ptr [r12 + 568], rax
 jmp xchain0_n11_α
 xchain0_n9_β:
 jmp xchain0_n13_α
.Lx10_0:
 .quad .Lx10_0_s
.Lx10_0_s:
 .string "ab"
# IR_ASSIGN gva
 xchain0_n10_α:
 mov rax, qword ptr [r12 + 384]
 mov rdx, qword ptr [r12 + 392]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 368], rax
 mov qword ptr [r12 + 376], rdx
 jmp main_γ
 xchain0_n10_β:
 jmp main_γ
 xchain0_n11_α:
# BOX CALL replace(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+512] -> [zr+448]
 mov rax, qword ptr [r12 + 512]
 mov qword ptr [r12 + 448], rax
 mov rax, qword ptr [r12 + 520]
 mov qword ptr [r12 + 456], rax
# marshal arg1 = producer-box slot [zr+528] -> [zr+464]
 mov rax, qword ptr [r12 + 528]
 mov qword ptr [r12 + 464], rax
 mov rax, qword ptr [r12 + 536]
 mov qword ptr [r12 + 472], rax
# marshal arg2 = producer-box slot [zr+560] -> [zr+480]
 mov rax, qword ptr [r12 + 560]
 mov qword ptr [r12 + 480], rax
 mov rax, qword ptr [r12 + 568]
 mov qword ptr [r12 + 488], rax
  .section .rodata
  .Lbynamefn12: .string "replace"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn12]
 lea rsi, [r12 + 448]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [r12 + 432], rax
 mov qword ptr [r12 + 440], rdx
 cmp eax, 99
 je xchain0_n13_α
 jmp xchain0_n12_α
 xchain0_n11_β:
 jmp xchain0_n13_α
# IR_ASSIGN gva
 xchain0_n12_α:
 mov rax, qword ptr [r12 + 432]
 mov rdx, qword ptr [r12 + 440]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 416], rax
 mov qword ptr [r12 + 424], rdx
 jmp xchain0_n13_α
 xchain0_n12_β:
 jmp xchain0_n13_α
# IR_LIT_STRING
 xchain0_n13_α:
 mov qword ptr [r12 + 736], 1
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [r12 + 744], rax
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
 mov qword ptr [r12 + 768], rax
 mov qword ptr [r12 + 776], rdx
 jmp xchain0_n15_α
 xchain0_n14_β:
 jmp xchain0_n17_α
.Lx15_0:
 .quad .Lx15_0_s
.Lx15_0_s:
 .string "alphabet"
# IR_VAR
 xchain0_n15_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 784], rax
 mov qword ptr [r12 + 792], rdx
 jmp xchain0_n16_α
 xchain0_n15_β:
 jmp xchain0_n17_α
 xchain0_n16_α:
# BOX CALL replace(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+736] -> [zr+672]
 mov rax, qword ptr [r12 + 736]
 mov qword ptr [r12 + 672], rax
 mov rax, qword ptr [r12 + 744]
 mov qword ptr [r12 + 680], rax
# marshal arg1 = producer-box slot [zr+768] -> [zr+688]
 mov rax, qword ptr [r12 + 768]
 mov qword ptr [r12 + 688], rax
 mov rax, qword ptr [r12 + 776]
 mov qword ptr [r12 + 696], rax
# marshal arg2 = producer-box slot [zr+784] -> [zr+704]
 mov rax, qword ptr [r12 + 784]
 mov qword ptr [r12 + 704], rax
 mov rax, qword ptr [r12 + 792]
 mov qword ptr [r12 + 712], rax
  .section .rodata
  .Lbynamefn17: .string "replace"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn17]
 lea rsi, [r12 + 672]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [r12 + 656], rax
 mov qword ptr [r12 + 664], rdx
 cmp eax, 99
 je xchain0_n17_α
 jmp xchain0_n18_α
 xchain0_n16_β:
 jmp xchain0_n17_α
# IR_LIT_STRING
 xchain0_n17_α:
 mov qword ptr [r12 + 1040], 1
 mov rax, qword ptr [rip + .Lx18_0]
 mov qword ptr [r12 + 1048], rax
 jmp xchain0_n19_α
 xchain0_n17_β:
 jmp xchain0_n26_α
.Lx18_0:
 .quad .Lx18_0_s
.Lx18_0_s:
 .string "hello"
# IR_LIT_STRING
 xchain0_n18_α:
 mov qword ptr [r12 + 816], 1
 mov rax, qword ptr [rip + .Lx19_0]
 mov qword ptr [r12 + 824], rax
 jmp xchain0_n20_α
 xchain0_n18_β:
 jmp xchain0_n17_α
.Lx19_0:
 .quad .Lx19_0_s
.Lx19_0_s:
 .string "aab"
# IR_LIT_STRING
 xchain0_n19_α:
 mov qword ptr [r12 + 1072], 1
 mov rax, qword ptr [rip + .Lx20_0]
 mov qword ptr [r12 + 1080], rax
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
 mov rax, qword ptr [r12 + 656]
 mov qword ptr [r12 + 608], rax
 mov rax, qword ptr [r12 + 664]
 mov qword ptr [r12 + 616], rax
# marshal arg1 = producer-box slot [zr+816] -> [zr+624]
 mov rax, qword ptr [r12 + 816]
 mov qword ptr [r12 + 624], rax
 mov rax, qword ptr [r12 + 824]
 mov qword ptr [r12 + 632], rax
  .section .rodata
  .Lbynamefn21: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn21]
 lea rsi, [r12 + 608]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 592], rax
 mov qword ptr [r12 + 600], rdx
 cmp eax, 99
 je xchain0_n17_α
 jmp xchain0_n22_α
 xchain0_n20_β:
 jmp xchain0_n17_α
# IR_LIT_STRING
 xchain0_n21_α:
 mov qword ptr [r12 + 1104], 1
 mov rax, qword ptr [rip + .Lx22_0]
 mov qword ptr [r12 + 1112], rax
 jmp xchain0_n23_α
 xchain0_n21_β:
 jmp xchain0_n26_α
.Lx22_0:
 .quad .Lx22_0_s
.Lx22_0_s:
 .string "aeiou"
# IR_LIT_STRING
 xchain0_n22_α:
 mov qword ptr [r12 + 864], 1
 mov rax, qword ptr [rip + .Lx23_0]
 mov qword ptr [r12 + 872], rax
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
 mov rax, qword ptr [r12 + 1040]
 mov qword ptr [r12 + 976], rax
 mov rax, qword ptr [r12 + 1048]
 mov qword ptr [r12 + 984], rax
# marshal arg1 = producer-box slot [zr+1072] -> [zr+992]
 mov rax, qword ptr [r12 + 1072]
 mov qword ptr [r12 + 992], rax
 mov rax, qword ptr [r12 + 1080]
 mov qword ptr [r12 + 1000], rax
# marshal arg2 = producer-box slot [zr+1104] -> [zr+1008]
 mov rax, qword ptr [r12 + 1104]
 mov qword ptr [r12 + 1008], rax
 mov rax, qword ptr [r12 + 1112]
 mov qword ptr [r12 + 1016], rax
  .section .rodata
  .Lbynamefn24: .string "replace"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn24]
 lea rsi, [r12 + 976]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [r12 + 960], rax
 mov qword ptr [r12 + 968], rdx
 cmp eax, 99
 je xchain0_n26_α
 jmp xchain0_n25_α
 xchain0_n23_β:
 jmp xchain0_n26_α
# IR_ASSIGN gva
 xchain0_n24_α:
 mov rax, qword ptr [r12 + 864]
 mov rdx, qword ptr [r12 + 872]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 848], rax
 mov qword ptr [r12 + 856], rdx
 jmp main_γ
 xchain0_n24_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n25_α:
 mov qword ptr [r12 + 1136], 1
 mov rax, qword ptr [rip + .Lx26_0]
 mov qword ptr [r12 + 1144], rax
 jmp xchain0_n27_α
 xchain0_n25_β:
 jmp xchain0_n26_α
.Lx26_0:
 .quad .Lx26_0_s
.Lx26_0_s:
 .string "hello"
# IR_LIT_STRING
 xchain0_n26_α:
 mov qword ptr [r12 + 1232], 1
 mov rax, qword ptr [rip + .Lx27_0]
 mov qword ptr [r12 + 1240], rax
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
 mov rax, qword ptr [r12 + 960]
 mov qword ptr [r12 + 912], rax
 mov rax, qword ptr [r12 + 968]
 mov qword ptr [r12 + 920], rax
# marshal arg1 = producer-box slot [zr+1136] -> [zr+928]
 mov rax, qword ptr [r12 + 1136]
 mov qword ptr [r12 + 928], rax
 mov rax, qword ptr [r12 + 1144]
 mov qword ptr [r12 + 936], rax
  .section .rodata
  .Lbynamefn28: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn28]
 lea rsi, [r12 + 912]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 896], rax
 mov qword ptr [r12 + 904], rdx
 cmp eax, 99
 je xchain0_n26_α
 jmp xchain0_n29_α
 xchain0_n27_β:
 jmp xchain0_n26_α
# IR_ASSIGN gva
 xchain0_n28_α:
 mov rax, qword ptr [r12 + 1232]
 mov rdx, qword ptr [r12 + 1240]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 1216], rax
 mov qword ptr [r12 + 1224], rdx
 jmp main_γ
 xchain0_n28_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n29_α:
 mov qword ptr [r12 + 1184], 1
 mov rax, qword ptr [rip + .Lx30_0]
 mov qword ptr [r12 + 1192], rax
 jmp xchain0_n30_α
 xchain0_n29_β:
 jmp main_γ
.Lx30_0:
 .quad .Lx30_0_s
.Lx30_0_s:
 .string "FAIL 810/003: identity replace"
# IR_ASSIGN gva
 xchain0_n30_α:
 mov rax, qword ptr [r12 + 1184]
 mov rdx, qword ptr [r12 + 1192]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 1168], rax
 mov qword ptr [r12 + 1176], rdx
 jmp main_γ
 xchain0_n30_β:
 jmp main_γ
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [r12 + 1272]
pop r12
ret
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [r12 + 1264]
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
main_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, qword ptr [r12 + 1272]
pop r12
ret
