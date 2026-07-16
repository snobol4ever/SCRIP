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
  mov qword ptr [rsp + 1240], rsp
main_α_body:
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [rsp + 224], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [rsp + 232], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n4_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "abc"
# IR_LIT_INTEGER
 xchain0_n1_α:
 mov qword ptr [rsp + 256], 6
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [rsp + 264], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n4_α
.Lx2_0:
 .quad 2
 xchain0_n2_α:
# BOX CALL dupl(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+224] -> [zr+176]
 mov rax, qword ptr [rsp + 224]
 mov qword ptr [rsp + 176], rax
 mov rax, qword ptr [rsp + 232]
 mov qword ptr [rsp + 184], rax
# marshal arg1 = producer-box slot [zr+256] -> [zr+192]
 mov rax, qword ptr [rsp + 256]
 mov qword ptr [rsp + 192], rax
 mov rax, qword ptr [rsp + 264]
 mov qword ptr [rsp + 200], rax
  .section .rodata
  .Lbynamefn3: .string "dupl"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn3]
 lea rsi, [rsp + 176]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 160], rax
 mov qword ptr [rsp + 168], rdx
 cmp eax, 99
 je xchain0_n4_α
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n4_α
# IR_LIT_STRING
 xchain0_n3_α:
 mov qword ptr [rsp + 288], 1
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [rsp + 296], rax
 jmp xchain0_n5_α
 xchain0_n3_β:
 jmp xchain0_n4_α
.Lx4_0:
 .quad .Lx4_0_s
.Lx4_0_s:
 .string "abcabc"
# IR_LIT_STRING
 xchain0_n4_α:
 mov qword ptr [rsp + 496], 1
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [rsp + 504], rax
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp xchain0_n11_α
.Lx5_0:
 .quad .Lx5_0_s
.Lx5_0_s:
 .string ""
 xchain0_n5_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+160] -> [zr+112]
 mov rax, qword ptr [rsp + 160]
 mov qword ptr [rsp + 112], rax
 mov rax, qword ptr [rsp + 168]
 mov qword ptr [rsp + 120], rax
# marshal arg1 = producer-box slot [zr+288] -> [zr+128]
 mov rax, qword ptr [rsp + 288]
 mov qword ptr [rsp + 128], rax
 mov rax, qword ptr [rsp + 296]
 mov qword ptr [rsp + 136], rax
  .section .rodata
  .Lbynamefn6: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn6]
 lea rsi, [rsp + 112]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 96], rax
 mov qword ptr [rsp + 104], rdx
 cmp eax, 99
 je xchain0_n4_α
 jmp xchain0_n7_α
 xchain0_n5_β:
 jmp xchain0_n4_α
# IR_LIT_INTEGER
 xchain0_n6_α:
 mov qword ptr [rsp + 528], 6
 mov rax, qword ptr [rip + .Lx7_0]
 mov qword ptr [rsp + 536], rax
 jmp xchain0_n8_α
 xchain0_n6_β:
 jmp xchain0_n11_α
.Lx7_0:
 .quad 10
# IR_LIT_STRING
 xchain0_n7_α:
 mov qword ptr [rsp + 336], 1
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [rsp + 344], rax
 jmp xchain0_n9_α
 xchain0_n7_β:
 jmp main_γ
.Lx8_0:
 .quad .Lx8_0_s
.Lx8_0_s:
 .string "FAIL 812/001: dupl string x2"
 xchain0_n8_α:
# BOX CALL dupl(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+496] -> [zr+448]
 mov rax, qword ptr [rsp + 496]
 mov qword ptr [rsp + 448], rax
 mov rax, qword ptr [rsp + 504]
 mov qword ptr [rsp + 456], rax
# marshal arg1 = producer-box slot [zr+528] -> [zr+464]
 mov rax, qword ptr [rsp + 528]
 mov qword ptr [rsp + 464], rax
 mov rax, qword ptr [rsp + 536]
 mov qword ptr [rsp + 472], rax
  .section .rodata
  .Lbynamefn9: .string "dupl"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn9]
 lea rsi, [rsp + 448]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 432], rax
 mov qword ptr [rsp + 440], rdx
 cmp eax, 99
 je xchain0_n11_α
 jmp xchain0_n10_α
 xchain0_n8_β:
 jmp xchain0_n11_α
# IR_ASSIGN gva
 xchain0_n9_α:
 mov rax, qword ptr [rsp + 336]
 mov rdx, qword ptr [rsp + 344]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rsp + 320], rax
 mov qword ptr [rsp + 328], rdx
 jmp main_γ
 xchain0_n9_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n10_α:
 mov qword ptr [rsp + 560], 1
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [rsp + 568], rax
 jmp xchain0_n12_α
 xchain0_n10_β:
 jmp xchain0_n11_α
.Lx11_0:
 .quad .Lx11_0_s
.Lx11_0_s:
 .string ""
# IR_LIT_STRING
 xchain0_n11_α:
 mov qword ptr [rsp + 768], 1
 mov rax, qword ptr [rip + .Lx12_0]
 mov qword ptr [rsp + 776], rax
 jmp xchain0_n13_α
 xchain0_n11_β:
 jmp xchain0_n18_α
.Lx12_0:
 .quad .Lx12_0_s
.Lx12_0_s:
 .string "abcdefg"
 xchain0_n12_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+432] -> [zr+384]
 mov rax, qword ptr [rsp + 432]
 mov qword ptr [rsp + 384], rax
 mov rax, qword ptr [rsp + 440]
 mov qword ptr [rsp + 392], rax
# marshal arg1 = producer-box slot [zr+560] -> [zr+400]
 mov rax, qword ptr [rsp + 560]
 mov qword ptr [rsp + 400], rax
 mov rax, qword ptr [rsp + 568]
 mov qword ptr [rsp + 408], rax
  .section .rodata
  .Lbynamefn13: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn13]
 lea rsi, [rsp + 384]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 368], rax
 mov qword ptr [rsp + 376], rdx
 cmp eax, 99
 je xchain0_n11_α
 jmp xchain0_n14_α
 xchain0_n12_β:
 jmp xchain0_n11_α
# IR_LIT_INTEGER
 xchain0_n13_α:
 mov qword ptr [rsp + 800], 6
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [rsp + 808], rax
 jmp xchain0_n15_α
 xchain0_n13_β:
 jmp xchain0_n18_α
.Lx14_0:
 .quad 0
# IR_LIT_STRING
 xchain0_n14_α:
 mov qword ptr [rsp + 608], 1
 mov rax, qword ptr [rip + .Lx15_0]
 mov qword ptr [rsp + 616], rax
 jmp xchain0_n16_α
 xchain0_n14_β:
 jmp main_γ
.Lx15_0:
 .quad .Lx15_0_s
.Lx15_0_s:
 .string "FAIL 812/002: dupl null is null"
 xchain0_n15_α:
# BOX CALL dupl(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+768] -> [zr+720]
 mov rax, qword ptr [rsp + 768]
 mov qword ptr [rsp + 720], rax
 mov rax, qword ptr [rsp + 776]
 mov qword ptr [rsp + 728], rax
# marshal arg1 = producer-box slot [zr+800] -> [zr+736]
 mov rax, qword ptr [rsp + 800]
 mov qword ptr [rsp + 736], rax
 mov rax, qword ptr [rsp + 808]
 mov qword ptr [rsp + 744], rax
  .section .rodata
  .Lbynamefn16: .string "dupl"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn16]
 lea rsi, [rsp + 720]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 704], rax
 mov qword ptr [rsp + 712], rdx
 cmp eax, 99
 je xchain0_n18_α
 jmp xchain0_n17_α
 xchain0_n15_β:
 jmp xchain0_n18_α
# IR_ASSIGN gva
 xchain0_n16_α:
 mov rax, qword ptr [rsp + 608]
 mov rdx, qword ptr [rsp + 616]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rsp + 592], rax
 mov qword ptr [rsp + 600], rdx
 jmp main_γ
 xchain0_n16_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n17_α:
 mov qword ptr [rsp + 832], 1
 mov rax, qword ptr [rip + .Lx18_0]
 mov qword ptr [rsp + 840], rax
 jmp xchain0_n19_α
 xchain0_n17_β:
 jmp xchain0_n18_α
.Lx18_0:
 .quad .Lx18_0_s
.Lx18_0_s:
 .string ""
# IR_LIT_INTEGER
 xchain0_n18_α:
 mov qword ptr [rsp + 1040], 6
 mov rax, qword ptr [rip + .Lx19_0]
 mov qword ptr [rsp + 1048], rax
 jmp xchain0_n20_α
 xchain0_n18_β:
 jmp xchain0_n25_α
.Lx19_0:
 .quad 1
 xchain0_n19_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+704] -> [zr+656]
 mov rax, qword ptr [rsp + 704]
 mov qword ptr [rsp + 656], rax
 mov rax, qword ptr [rsp + 712]
 mov qword ptr [rsp + 664], rax
# marshal arg1 = producer-box slot [zr+832] -> [zr+672]
 mov rax, qword ptr [rsp + 832]
 mov qword ptr [rsp + 672], rax
 mov rax, qword ptr [rsp + 840]
 mov qword ptr [rsp + 680], rax
  .section .rodata
  .Lbynamefn20: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn20]
 lea rsi, [rsp + 656]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 640], rax
 mov qword ptr [rsp + 648], rdx
 cmp eax, 99
 je xchain0_n18_α
 jmp xchain0_n21_α
 xchain0_n19_β:
 jmp xchain0_n18_α
# IR_LIT_INTEGER
 xchain0_n20_α:
 mov qword ptr [rsp + 1072], 6
 mov rax, qword ptr [rip + .Lx21_0]
 mov qword ptr [rsp + 1080], rax
 jmp xchain0_n22_α
 xchain0_n20_β:
 jmp xchain0_n25_α
.Lx21_0:
 .quad 10
# IR_LIT_STRING
 xchain0_n21_α:
 mov qword ptr [rsp + 880], 1
 mov rax, qword ptr [rip + .Lx22_0]
 mov qword ptr [rsp + 888], rax
 jmp xchain0_n23_α
 xchain0_n21_β:
 jmp main_γ
.Lx22_0:
 .quad .Lx22_0_s
.Lx22_0_s:
 .string "FAIL 812/003: dupl x0 is null"
 xchain0_n22_α:
# BOX CALL dupl(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1040] -> [zr+992]
 mov rax, qword ptr [rsp + 1040]
 mov qword ptr [rsp + 992], rax
 mov rax, qword ptr [rsp + 1048]
 mov qword ptr [rsp + 1000], rax
# marshal arg1 = producer-box slot [zr+1072] -> [zr+1008]
 mov rax, qword ptr [rsp + 1072]
 mov qword ptr [rsp + 1008], rax
 mov rax, qword ptr [rsp + 1080]
 mov qword ptr [rsp + 1016], rax
  .section .rodata
  .Lbynamefn23: .string "dupl"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn23]
 lea rsi, [rsp + 992]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 976], rax
 mov qword ptr [rsp + 984], rdx
 cmp eax, 99
 je xchain0_n25_α
 jmp xchain0_n24_α
 xchain0_n22_β:
 jmp xchain0_n25_α
# IR_ASSIGN gva
 xchain0_n23_α:
 mov rax, qword ptr [rsp + 880]
 mov rdx, qword ptr [rsp + 888]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rsp + 864], rax
 mov qword ptr [rsp + 872], rdx
 jmp main_γ
 xchain0_n23_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n24_α:
 mov qword ptr [rsp + 1104], 1
 mov rax, qword ptr [rip + .Lx25_0]
 mov qword ptr [rsp + 1112], rax
 jmp xchain0_n26_α
 xchain0_n24_β:
 jmp xchain0_n25_α
.Lx25_0:
 .quad .Lx25_0_s
.Lx25_0_s:
 .string "1111111111"
# IR_LIT_STRING
 xchain0_n25_α:
 mov qword ptr [rsp + 1200], 1
 mov rax, qword ptr [rip + .Lx26_0]
 mov qword ptr [rsp + 1208], rax
 jmp xchain0_n27_α
 xchain0_n25_β:
 jmp main_γ
.Lx26_0:
 .quad .Lx26_0_s
.Lx26_0_s:
 .string "PASS 812_dupl (4/4)"
 xchain0_n26_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+976] -> [zr+928]
 mov rax, qword ptr [rsp + 976]
 mov qword ptr [rsp + 928], rax
 mov rax, qword ptr [rsp + 984]
 mov qword ptr [rsp + 936], rax
# marshal arg1 = producer-box slot [zr+1104] -> [zr+944]
 mov rax, qword ptr [rsp + 1104]
 mov qword ptr [rsp + 944], rax
 mov rax, qword ptr [rsp + 1112]
 mov qword ptr [rsp + 952], rax
  .section .rodata
  .Lbynamefn27: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn27]
 lea rsi, [rsp + 928]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 912], rax
 mov qword ptr [rsp + 920], rdx
 cmp eax, 99
 je xchain0_n25_α
 jmp xchain0_n28_α
 xchain0_n26_β:
 jmp xchain0_n25_α
# IR_ASSIGN gva
 xchain0_n27_α:
 mov rax, qword ptr [rsp + 1200]
 mov rdx, qword ptr [rsp + 1208]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rsp + 1184], rax
 mov qword ptr [rsp + 1192], rdx
 jmp main_γ
 xchain0_n27_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n28_α:
 mov qword ptr [rsp + 1152], 1
 mov rax, qword ptr [rip + .Lx29_0]
 mov qword ptr [rsp + 1160], rax
 jmp xchain0_n29_α
 xchain0_n28_β:
 jmp main_γ
.Lx29_0:
 .quad .Lx29_0_s
.Lx29_0_s:
 .string "FAIL 812/004: dupl integer coerce"
# IR_ASSIGN gva
 xchain0_n29_α:
 mov rax, qword ptr [rsp + 1152]
 mov rdx, qword ptr [rsp + 1160]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rsp + 1136], rax
 mov qword ptr [rsp + 1144], rdx
 jmp main_γ
 xchain0_n29_β:
 jmp main_γ
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rsp + 1240]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rsp + 1240]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
add rsp, 65544
ret
