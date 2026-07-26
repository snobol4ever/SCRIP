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
  mov r12, qword ptr [1879048192]
  mov [rsp + 65536], rbp
  mov rbp, rsp
main_α_body:
#         differ(convert('12', 'integer'), 12)                   :f(e001)
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [rbp + 224], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [rbp + 232], rax
 jmp xchain0_n1_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "12"
# IR_LIT_STRING
 xchain0_n1_α:
 mov qword ptr [rbp + 256], 1
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [rbp + 264], rax
 jmp xchain0_n2_α
.Lx2_0:
 .quad .Lx2_0_s
.Lx2_0_s:
 .string "integer"
 xchain0_n2_α:
# BOX CALL convert(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+224] -> [zr+176]
 mov rax, qword ptr [rbp + 224]
 mov qword ptr [rbp + 176], rax
 mov rax, qword ptr [rbp + 232]
 mov qword ptr [rbp + 184], rax
# marshal arg1 = producer-box slot [zr+256] -> [zr+192]
 mov rax, qword ptr [rbp + 256]
 mov qword ptr [rbp + 192], rax
 mov rax, qword ptr [rbp + 264]
 mov qword ptr [rbp + 200], rax
  .section .rodata
  .Lbynamefn3: .string "convert"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn3]
 lea rsi, [rbp + 176]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 160], rax
 mov qword ptr [rbp + 168], rdx
 cmp eax, 99
 je xchain0_n4_α
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n4_α
# IR_LIT_INTEGER
 xchain0_n3_α:
 mov qword ptr [rbp + 288], 6
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [rbp + 296], rax
 jmp xchain0_n5_α
.Lx4_0:
 .quad 12
#         differ(convert(2.5, 'integer'), 2)                   :f(e002)
# IR_LIT_REAL
 xchain0_n4_α:
 mov qword ptr [rbp + 496], 7
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [rbp + 504], rax
 jmp xchain0_n6_α
.Lx5_0:
 .quad 4612811918334230528
 xchain0_n5_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+160] -> [zr+112]
 mov rax, qword ptr [rbp + 160]
 mov qword ptr [rbp + 112], rax
 mov rax, qword ptr [rbp + 168]
 mov qword ptr [rbp + 120], rax
# marshal arg1 = producer-box slot [zr+288] -> [zr+128]
 mov rax, qword ptr [rbp + 288]
 mov qword ptr [rbp + 128], rax
 mov rax, qword ptr [rbp + 296]
 mov qword ptr [rbp + 136], rax
  .section .rodata
  .Lbynamefn6: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn6]
 lea rsi, [rbp + 112]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 96], rax
 mov qword ptr [rbp + 104], rdx
 cmp eax, 99
 je xchain0_n4_α
 jmp xchain0_n7_α
 xchain0_n5_β:
 jmp xchain0_n4_α
# IR_LIT_STRING
 xchain0_n6_α:
 mov qword ptr [rbp + 528], 1
 mov rax, qword ptr [rip + .Lx7_0]
 mov qword ptr [rbp + 536], rax
 jmp xchain0_n8_α
.Lx7_0:
 .quad .Lx7_0_s
.Lx7_0_s:
 .string "integer"
#         output = 'FAIL 910/001: string->integer'        :(end)
# IR_LIT_STRING
 xchain0_n7_α:
 mov qword ptr [rbp + 336], 1
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [rbp + 344], rax
 jmp xchain0_n9_α
.Lx8_0:
 .quad .Lx8_0_s
.Lx8_0_s:
 .string "FAIL 910/001: string->integer"
 xchain0_n8_α:
# BOX CALL convert(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+496] -> [zr+448]
 mov rax, qword ptr [rbp + 496]
 mov qword ptr [rbp + 448], rax
 mov rax, qword ptr [rbp + 504]
 mov qword ptr [rbp + 456], rax
# marshal arg1 = producer-box slot [zr+528] -> [zr+464]
 mov rax, qword ptr [rbp + 528]
 mov qword ptr [rbp + 464], rax
 mov rax, qword ptr [rbp + 536]
 mov qword ptr [rbp + 472], rax
  .section .rodata
  .Lbynamefn9: .string "convert"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn9]
 lea rsi, [rbp + 448]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 432], rax
 mov qword ptr [rbp + 440], rdx
 cmp eax, 99
 je xchain0_n11_α
 jmp xchain0_n10_α
 xchain0_n8_β:
 jmp xchain0_n11_α
# IR_ASSIGN gva
 xchain0_n9_α:
 mov rax, qword ptr [rbp + 336]
 mov rdx, qword ptr [rbp + 344]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 320], rax
 mov qword ptr [rbp + 328], rdx
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n10_α:
 mov qword ptr [rbp + 560], 6
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [rbp + 568], rax
 jmp xchain0_n12_α
.Lx11_0:
 .quad 2
#         differ(convert(2, 'real'), 2.0)                   :f(e003)
# IR_LIT_INTEGER
 xchain0_n11_α:
 mov qword ptr [rbp + 768], 6
 mov rax, qword ptr [rip + .Lx12_0]
 mov qword ptr [rbp + 776], rax
 jmp xchain0_n13_α
.Lx12_0:
 .quad 2
 xchain0_n12_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+432] -> [zr+384]
 mov rax, qword ptr [rbp + 432]
 mov qword ptr [rbp + 384], rax
 mov rax, qword ptr [rbp + 440]
 mov qword ptr [rbp + 392], rax
# marshal arg1 = producer-box slot [zr+560] -> [zr+400]
 mov rax, qword ptr [rbp + 560]
 mov qword ptr [rbp + 400], rax
 mov rax, qword ptr [rbp + 568]
 mov qword ptr [rbp + 408], rax
  .section .rodata
  .Lbynamefn13: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn13]
 lea rsi, [rbp + 384]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 368], rax
 mov qword ptr [rbp + 376], rdx
 cmp eax, 99
 je xchain0_n11_α
 jmp xchain0_n14_α
 xchain0_n12_β:
 jmp xchain0_n11_α
# IR_LIT_STRING
 xchain0_n13_α:
 mov qword ptr [rbp + 800], 1
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [rbp + 808], rax
 jmp xchain0_n15_α
.Lx14_0:
 .quad .Lx14_0_s
.Lx14_0_s:
 .string "real"
#         output = 'FAIL 910/002: real->integer truncation' :(end)
# IR_LIT_STRING
 xchain0_n14_α:
 mov qword ptr [rbp + 608], 1
 mov rax, qword ptr [rip + .Lx15_0]
 mov qword ptr [rbp + 616], rax
 jmp xchain0_n16_α
.Lx15_0:
 .quad .Lx15_0_s
.Lx15_0_s:
 .string "FAIL 910/002: real->integer truncation"
 xchain0_n15_α:
# BOX CALL convert(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+768] -> [zr+720]
 mov rax, qword ptr [rbp + 768]
 mov qword ptr [rbp + 720], rax
 mov rax, qword ptr [rbp + 776]
 mov qword ptr [rbp + 728], rax
# marshal arg1 = producer-box slot [zr+800] -> [zr+736]
 mov rax, qword ptr [rbp + 800]
 mov qword ptr [rbp + 736], rax
 mov rax, qword ptr [rbp + 808]
 mov qword ptr [rbp + 744], rax
  .section .rodata
  .Lbynamefn16: .string "convert"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn16]
 lea rsi, [rbp + 720]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 704], rax
 mov qword ptr [rbp + 712], rdx
 cmp eax, 99
 je xchain0_n18_α
 jmp xchain0_n17_α
 xchain0_n15_β:
 jmp xchain0_n18_α
# IR_ASSIGN gva
 xchain0_n16_α:
 mov rax, qword ptr [rbp + 608]
 mov rdx, qword ptr [rbp + 616]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 592], rax
 mov qword ptr [rbp + 600], rdx
 jmp main_γ
# IR_LIT_REAL
 xchain0_n17_α:
 mov qword ptr [rbp + 832], 7
 mov rax, qword ptr [rip + .Lx18_0]
 mov qword ptr [rbp + 840], rax
 jmp xchain0_n19_α
.Lx18_0:
 .quad 4611686018427387904
#         differ(convert('.2', 'real'), 0.2)                   :f(e004)
# IR_LIT_STRING
 xchain0_n18_α:
 mov qword ptr [rbp + 1040], 1
 mov rax, qword ptr [rip + .Lx19_0]
 mov qword ptr [rbp + 1048], rax
 jmp xchain0_n20_α
.Lx19_0:
 .quad .Lx19_0_s
.Lx19_0_s:
 .string ".2"
 xchain0_n19_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+704] -> [zr+656]
 mov rax, qword ptr [rbp + 704]
 mov qword ptr [rbp + 656], rax
 mov rax, qword ptr [rbp + 712]
 mov qword ptr [rbp + 664], rax
# marshal arg1 = producer-box slot [zr+832] -> [zr+672]
 mov rax, qword ptr [rbp + 832]
 mov qword ptr [rbp + 672], rax
 mov rax, qword ptr [rbp + 840]
 mov qword ptr [rbp + 680], rax
  .section .rodata
  .Lbynamefn20: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn20]
 lea rsi, [rbp + 656]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 640], rax
 mov qword ptr [rbp + 648], rdx
 cmp eax, 99
 je xchain0_n18_α
 jmp xchain0_n21_α
 xchain0_n19_β:
 jmp xchain0_n18_α
# IR_LIT_STRING
 xchain0_n20_α:
 mov qword ptr [rbp + 1072], 1
 mov rax, qword ptr [rip + .Lx21_0]
 mov qword ptr [rbp + 1080], rax
 jmp xchain0_n22_α
.Lx21_0:
 .quad .Lx21_0_s
.Lx21_0_s:
 .string "real"
#         output = 'FAIL 910/003: integer->real'          :(end)
# IR_LIT_STRING
 xchain0_n21_α:
 mov qword ptr [rbp + 880], 1
 mov rax, qword ptr [rip + .Lx22_0]
 mov qword ptr [rbp + 888], rax
 jmp xchain0_n23_α
.Lx22_0:
 .quad .Lx22_0_s
.Lx22_0_s:
 .string "FAIL 910/003: integer->real"
 xchain0_n22_α:
# BOX CALL convert(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+1040] -> [zr+992]
 mov rax, qword ptr [rbp + 1040]
 mov qword ptr [rbp + 992], rax
 mov rax, qword ptr [rbp + 1048]
 mov qword ptr [rbp + 1000], rax
# marshal arg1 = producer-box slot [zr+1072] -> [zr+1008]
 mov rax, qword ptr [rbp + 1072]
 mov qword ptr [rbp + 1008], rax
 mov rax, qword ptr [rbp + 1080]
 mov qword ptr [rbp + 1016], rax
  .section .rodata
  .Lbynamefn23: .string "convert"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn23]
 lea rsi, [rbp + 992]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 976], rax
 mov qword ptr [rbp + 984], rdx
 cmp eax, 99
 je xchain0_n25_α
 jmp xchain0_n24_α
 xchain0_n22_β:
 jmp xchain0_n25_α
# IR_ASSIGN gva
 xchain0_n23_α:
 mov rax, qword ptr [rbp + 880]
 mov rdx, qword ptr [rbp + 888]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 864], rax
 mov qword ptr [rbp + 872], rdx
 jmp main_γ
# IR_LIT_REAL
 xchain0_n24_α:
 mov qword ptr [rbp + 1104], 7
 mov rax, qword ptr [rip + .Lx25_0]
 mov qword ptr [rbp + 1112], rax
 jmp xchain0_n26_α
.Lx25_0:
 .quad 4596373779694328218
#         output = 'PASS 910_convert (4/4)'
# IR_LIT_STRING
 xchain0_n25_α:
 mov qword ptr [rbp + 1200], 1
 mov rax, qword ptr [rip + .Lx26_0]
 mov qword ptr [rbp + 1208], rax
 jmp xchain0_n27_α
.Lx26_0:
 .quad .Lx26_0_s
.Lx26_0_s:
 .string "PASS 910_convert (4/4)"
 xchain0_n26_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+976] -> [zr+928]
 mov rax, qword ptr [rbp + 976]
 mov qword ptr [rbp + 928], rax
 mov rax, qword ptr [rbp + 984]
 mov qword ptr [rbp + 936], rax
# marshal arg1 = producer-box slot [zr+1104] -> [zr+944]
 mov rax, qword ptr [rbp + 1104]
 mov qword ptr [rbp + 944], rax
 mov rax, qword ptr [rbp + 1112]
 mov qword ptr [rbp + 952], rax
  .section .rodata
  .Lbynamefn27: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn27]
 lea rsi, [rbp + 928]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rbp + 912], rax
 mov qword ptr [rbp + 920], rdx
 cmp eax, 99
 je xchain0_n25_α
 jmp xchain0_n28_α
 xchain0_n26_β:
 jmp xchain0_n25_α
# IR_ASSIGN gva
 xchain0_n27_α:
 mov rax, qword ptr [rbp + 1200]
 mov rdx, qword ptr [rbp + 1208]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 1184], rax
 mov qword ptr [rbp + 1192], rdx
 jmp main_γ
#         output = 'FAIL 910/004: string->real'           :(end)
# IR_LIT_STRING
 xchain0_n28_α:
 mov qword ptr [rbp + 1152], 1
 mov rax, qword ptr [rip + .Lx29_0]
 mov qword ptr [rbp + 1160], rax
 jmp xchain0_n29_α
.Lx29_0:
 .quad .Lx29_0_s
.Lx29_0_s:
 .string "FAIL 910/004: string->real"
# IR_ASSIGN gva
 xchain0_n29_α:
 mov rax, qword ptr [rbp + 1152]
 mov rdx, qword ptr [rbp + 1160]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 1136], rax
 mov qword ptr [rbp + 1144], rdx
 jmp main_γ
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rbp + 1240]
mov rbp, [rsp + 65536]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rbp + 1240]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
mov rbp, [rsp + 65536]
add rsp, 65544
ret
