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
  push rbp
  mov rbp, rsp
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
 mov qword ptr [r12 + 848], rax
 pop rsi
main_α_body:
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [r12 + 176], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 184], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n5_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "axxbyyy"
# IR_LIT_STRING
 xchain0_n1_α:
 mov qword ptr [r12 + 192], 1
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [r12 + 200], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n5_α
.Lx2_0:
 .quad .Lx2_0_s
.Lx2_0_s:
 .string "xy"
# IR_LIT_STRING
 xchain0_n2_α:
 mov qword ptr [r12 + 208], 1
 mov rax, qword ptr [rip + .Lx3_0]
 mov qword ptr [r12 + 216], rax
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n5_α
.Lx3_0:
 .quad .Lx3_0_s
.Lx3_0_s:
 .string "01"
 xchain0_n3_α:
# BOX IR_CALL replace(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+176] -> [zr+128]
 mov rax, qword ptr [r12 + 176]
 mov qword ptr [r12 + 128], rax
 mov rax, qword ptr [r12 + 184]
 mov qword ptr [r12 + 136], rax
# marshal arg1 = producer-box slot [zr+192] -> [zr+144]
 mov rax, qword ptr [r12 + 192]
 mov qword ptr [r12 + 144], rax
 mov rax, qword ptr [r12 + 200]
 mov qword ptr [r12 + 152], rax
# marshal arg2 = producer-box slot [zr+208] -> [zr+160]
 mov rax, qword ptr [r12 + 208]
 mov qword ptr [r12 + 160], rax
 mov rax, qword ptr [r12 + 216]
 mov qword ptr [r12 + 168], rax
  .section .rodata
  .Lbynamefn5: .string "replace"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn5]
 lea rsi, [r12 + 128]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [r12 + 112], rax
 mov qword ptr [r12 + 120], rdx
 cmp eax, 99
 je xchain0_n5_α
 jmp xchain0_n4_α
xchain0_n3_β:
 jmp xchain0_n5_α
# IR_LIT_STRING
 xchain0_n4_α:
 mov qword ptr [r12 + 224], 1
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [r12 + 232], rax
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp xchain0_n5_α
.Lx6_0:
 .quad .Lx6_0_s
.Lx6_0_s:
 .string "a00b111"
# IR_KEYWORD_SNOBOL4_read
 xchain0_n5_α:
 mov rdi, qword ptr [rip + .Lx7_0]
 call rt_keyword_read_snobol4@PLT
 mov qword ptr [r12 + 352], rax
 mov qword ptr [r12 + 360], rdx
 jmp xchain0_n7_α
 xchain0_n5_β:
 jmp xchain0_n13_α
.Lx7_0:
 .quad .Lx7_0_s
.Lx7_0_s:
 .string "alphabet"
 xchain0_n6_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+112] -> [zr+80]
 mov rax, qword ptr [r12 + 112]
 mov qword ptr [r12 + 80], rax
 mov rax, qword ptr [r12 + 120]
 mov qword ptr [r12 + 88], rax
# marshal arg1 = producer-box slot [zr+224] -> [zr+96]
 mov rax, qword ptr [r12 + 224]
 mov qword ptr [r12 + 96], rax
 mov rax, qword ptr [r12 + 232]
 mov qword ptr [r12 + 104], rax
  .section .rodata
  .Lbynamefn9: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn9]
 lea rsi, [r12 + 80]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 64], rax
 mov qword ptr [r12 + 72], rdx
 cmp eax, 99
 je xchain0_n5_α
 jmp xchain0_n8_α
xchain0_n6_β:
 jmp xchain0_n5_α
# IR_LIT_STRING
 xchain0_n7_α:
 mov qword ptr [r12 + 368], 1
 mov rax, qword ptr [rip + .Lx10_0]
 mov qword ptr [r12 + 376], rax
 jmp xchain0_n9_α
 xchain0_n7_β:
 jmp xchain0_n13_α
.Lx10_0:
 .quad .Lx10_0_s
.Lx10_0_s:
 .string "xy"
# IR_LIT_STRING
 xchain0_n8_α:
 mov qword ptr [r12 + 256], 1
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [r12 + 264], rax
 jmp xchain0_n10_α
 xchain0_n8_β:
 jmp main_γ
.Lx11_0:
 .quad .Lx11_0_s
.Lx11_0_s:
 .string "FAIL 810/001: xy->01 mapping"
# IR_LIT_STRING
 xchain0_n9_α:
 mov qword ptr [r12 + 384], 1
 mov rax, qword ptr [rip + .Lx12_0]
 mov qword ptr [r12 + 392], rax
 jmp xchain0_n11_α
 xchain0_n9_β:
 jmp xchain0_n13_α
.Lx12_0:
 .quad .Lx12_0_s
.Lx12_0_s:
 .string "ab"
# IR_ASSIGN gva
 xchain0_n10_α:
 mov rax, qword ptr [r12 + 256]
 mov rdx, qword ptr [r12 + 264]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 240], rax
 mov qword ptr [r12 + 248], rdx
 jmp main_γ
 xchain0_n10_β:
 jmp main_γ
 xchain0_n11_α:
# BOX IR_CALL replace(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+352] -> [zr+304]
 mov rax, qword ptr [r12 + 352]
 mov qword ptr [r12 + 304], rax
 mov rax, qword ptr [r12 + 360]
 mov qword ptr [r12 + 312], rax
# marshal arg1 = producer-box slot [zr+368] -> [zr+320]
 mov rax, qword ptr [r12 + 368]
 mov qword ptr [r12 + 320], rax
 mov rax, qword ptr [r12 + 376]
 mov qword ptr [r12 + 328], rax
# marshal arg2 = producer-box slot [zr+384] -> [zr+336]
 mov rax, qword ptr [r12 + 384]
 mov qword ptr [r12 + 336], rax
 mov rax, qword ptr [r12 + 392]
 mov qword ptr [r12 + 344], rax
  .section .rodata
  .Lbynamefn15: .string "replace"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn15]
 lea rsi, [r12 + 304]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [r12 + 288], rax
 mov qword ptr [r12 + 296], rdx
 cmp eax, 99
 je xchain0_n13_α
 jmp xchain0_n12_α
xchain0_n11_β:
 jmp xchain0_n13_α
# IR_ASSIGN gva
 xchain0_n12_α:
 mov rax, qword ptr [r12 + 288]
 mov rdx, qword ptr [r12 + 296]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 272], rax
 mov qword ptr [r12 + 280], rdx
 jmp xchain0_n13_α
 xchain0_n12_β:
 jmp xchain0_n13_α
# IR_LIT_STRING
 xchain0_n13_α:
 mov qword ptr [r12 + 512], 1
 mov rax, qword ptr [rip + .Lx17_0]
 mov qword ptr [r12 + 520], rax
 jmp xchain0_n14_α
 xchain0_n13_β:
 jmp xchain0_n17_α
.Lx17_0:
 .quad .Lx17_0_s
.Lx17_0_s:
 .string "axy"
# IR_KEYWORD_SNOBOL4_read
 xchain0_n14_α:
 mov rdi, qword ptr [rip + .Lx18_0]
 call rt_keyword_read_snobol4@PLT
 mov qword ptr [r12 + 528], rax
 mov qword ptr [r12 + 536], rdx
 jmp xchain0_n15_α
 xchain0_n14_β:
 jmp xchain0_n17_α
.Lx18_0:
 .quad .Lx18_0_s
.Lx18_0_s:
 .string "alphabet"
# IR_VAR
 xchain0_n15_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 544], rax
 mov qword ptr [r12 + 552], rdx
 jmp xchain0_n16_α
 xchain0_n15_β:
 jmp xchain0_n17_α
 xchain0_n16_α:
# BOX IR_CALL replace(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+512] -> [zr+464]
 mov rax, qword ptr [r12 + 512]
 mov qword ptr [r12 + 464], rax
 mov rax, qword ptr [r12 + 520]
 mov qword ptr [r12 + 472], rax
# marshal arg1 = producer-box slot [zr+528] -> [zr+480]
 mov rax, qword ptr [r12 + 528]
 mov qword ptr [r12 + 480], rax
 mov rax, qword ptr [r12 + 536]
 mov qword ptr [r12 + 488], rax
# marshal arg2 = producer-box slot [zr+544] -> [zr+496]
 mov rax, qword ptr [r12 + 544]
 mov qword ptr [r12 + 496], rax
 mov rax, qword ptr [r12 + 552]
 mov qword ptr [r12 + 504], rax
  .section .rodata
  .Lbynamefn21: .string "replace"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn21]
 lea rsi, [r12 + 464]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [r12 + 448], rax
 mov qword ptr [r12 + 456], rdx
 cmp eax, 99
 je xchain0_n17_α
 jmp xchain0_n18_α
xchain0_n16_β:
 jmp xchain0_n17_α
# IR_LIT_STRING
 xchain0_n17_α:
 mov qword ptr [r12 + 720], 1
 mov rax, qword ptr [rip + .Lx22_0]
 mov qword ptr [r12 + 728], rax
 jmp xchain0_n19_α
 xchain0_n17_β:
 jmp xchain0_n26_α
.Lx22_0:
 .quad .Lx22_0_s
.Lx22_0_s:
 .string "hello"
# IR_LIT_STRING
 xchain0_n18_α:
 mov qword ptr [r12 + 560], 1
 mov rax, qword ptr [rip + .Lx23_0]
 mov qword ptr [r12 + 568], rax
 jmp xchain0_n20_α
 xchain0_n18_β:
 jmp xchain0_n17_α
.Lx23_0:
 .quad .Lx23_0_s
.Lx23_0_s:
 .string "aab"
# IR_LIT_STRING
 xchain0_n19_α:
 mov qword ptr [r12 + 736], 1
 mov rax, qword ptr [rip + .Lx24_0]
 mov qword ptr [r12 + 744], rax
 jmp xchain0_n21_α
 xchain0_n19_β:
 jmp xchain0_n26_α
.Lx24_0:
 .quad .Lx24_0_s
.Lx24_0_s:
 .string "aeiou"
 xchain0_n20_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+448] -> [zr+416]
 mov rax, qword ptr [r12 + 448]
 mov qword ptr [r12 + 416], rax
 mov rax, qword ptr [r12 + 456]
 mov qword ptr [r12 + 424], rax
# marshal arg1 = producer-box slot [zr+560] -> [zr+432]
 mov rax, qword ptr [r12 + 560]
 mov qword ptr [r12 + 432], rax
 mov rax, qword ptr [r12 + 568]
 mov qword ptr [r12 + 440], rax
  .section .rodata
  .Lbynamefn26: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn26]
 lea rsi, [r12 + 416]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 400], rax
 mov qword ptr [r12 + 408], rdx
 cmp eax, 99
 je xchain0_n17_α
 jmp xchain0_n22_α
xchain0_n20_β:
 jmp xchain0_n17_α
# IR_LIT_STRING
 xchain0_n21_α:
 mov qword ptr [r12 + 752], 1
 mov rax, qword ptr [rip + .Lx27_0]
 mov qword ptr [r12 + 760], rax
 jmp xchain0_n23_α
 xchain0_n21_β:
 jmp xchain0_n26_α
.Lx27_0:
 .quad .Lx27_0_s
.Lx27_0_s:
 .string "aeiou"
# IR_LIT_STRING
 xchain0_n22_α:
 mov qword ptr [r12 + 592], 1
 mov rax, qword ptr [rip + .Lx28_0]
 mov qword ptr [r12 + 600], rax
 jmp xchain0_n24_α
 xchain0_n22_β:
 jmp main_γ
.Lx28_0:
 .quad .Lx28_0_s
.Lx28_0_s:
 .string "FAIL 810/002: alphabet translation"
 xchain0_n23_α:
# BOX IR_CALL replace(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+720] -> [zr+672]
 mov rax, qword ptr [r12 + 720]
 mov qword ptr [r12 + 672], rax
 mov rax, qword ptr [r12 + 728]
 mov qword ptr [r12 + 680], rax
# marshal arg1 = producer-box slot [zr+736] -> [zr+688]
 mov rax, qword ptr [r12 + 736]
 mov qword ptr [r12 + 688], rax
 mov rax, qword ptr [r12 + 744]
 mov qword ptr [r12 + 696], rax
# marshal arg2 = producer-box slot [zr+752] -> [zr+704]
 mov rax, qword ptr [r12 + 752]
 mov qword ptr [r12 + 704], rax
 mov rax, qword ptr [r12 + 760]
 mov qword ptr [r12 + 712], rax
  .section .rodata
  .Lbynamefn30: .string "replace"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn30]
 lea rsi, [r12 + 672]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [r12 + 656], rax
 mov qword ptr [r12 + 664], rdx
 cmp eax, 99
 je xchain0_n26_α
 jmp xchain0_n25_α
xchain0_n23_β:
 jmp xchain0_n26_α
# IR_ASSIGN gva
 xchain0_n24_α:
 mov rax, qword ptr [r12 + 592]
 mov rdx, qword ptr [r12 + 600]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 576], rax
 mov qword ptr [r12 + 584], rdx
 jmp main_γ
 xchain0_n24_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n25_α:
 mov qword ptr [r12 + 768], 1
 mov rax, qword ptr [rip + .Lx32_0]
 mov qword ptr [r12 + 776], rax
 jmp xchain0_n27_α
 xchain0_n25_β:
 jmp xchain0_n26_α
.Lx32_0:
 .quad .Lx32_0_s
.Lx32_0_s:
 .string "hello"
# IR_LIT_STRING
 xchain0_n26_α:
 mov qword ptr [r12 + 832], 1
 mov rax, qword ptr [rip + .Lx33_0]
 mov qword ptr [r12 + 840], rax
 jmp xchain0_n28_α
 xchain0_n26_β:
 jmp main_γ
.Lx33_0:
 .quad .Lx33_0_s
.Lx33_0_s:
 .string "PASS 810_replace (3/3)"
 xchain0_n27_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+656] -> [zr+624]
 mov rax, qword ptr [r12 + 656]
 mov qword ptr [r12 + 624], rax
 mov rax, qword ptr [r12 + 664]
 mov qword ptr [r12 + 632], rax
# marshal arg1 = producer-box slot [zr+768] -> [zr+640]
 mov rax, qword ptr [r12 + 768]
 mov qword ptr [r12 + 640], rax
 mov rax, qword ptr [r12 + 776]
 mov qword ptr [r12 + 648], rax
  .section .rodata
  .Lbynamefn35: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn35]
 lea rsi, [r12 + 624]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 608], rax
 mov qword ptr [r12 + 616], rdx
 cmp eax, 99
 je xchain0_n26_α
 jmp xchain0_n29_α
xchain0_n27_β:
 jmp xchain0_n26_α
# IR_ASSIGN gva
 xchain0_n28_α:
 mov rax, qword ptr [r12 + 832]
 mov rdx, qword ptr [r12 + 840]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 816], rax
 mov qword ptr [r12 + 824], rdx
 jmp main_γ
 xchain0_n28_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n29_α:
 mov qword ptr [r12 + 800], 1
 mov rax, qword ptr [rip + .Lx37_0]
 mov qword ptr [r12 + 808], rax
 jmp xchain0_n30_α
 xchain0_n29_β:
 jmp main_γ
.Lx37_0:
 .quad .Lx37_0_s
.Lx37_0_s:
 .string "FAIL 810/003: identity replace"
# IR_ASSIGN gva
 xchain0_n30_α:
 mov rax, qword ptr [r12 + 800]
 mov rdx, qword ptr [r12 + 808]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 784], rax
 mov qword ptr [r12 + 792], rdx
 jmp main_γ
 xchain0_n30_β:
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
 mov rdi, qword ptr [r12 + 848]
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
