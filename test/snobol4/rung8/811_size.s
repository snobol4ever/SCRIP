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
  mov qword ptr [rsp + 824], rsp
  mov r12, qword ptr [1879048192]
  mov rbp, rsp
main_α_body:
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [rsp + 208], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [rsp + 216], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n3_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "abc"
 xchain0_n1_α:
# BOX CALL size(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+208] -> [zr+176]
 mov rax, qword ptr [rsp + 208]
 mov qword ptr [rsp + 176], rax
 mov rax, qword ptr [rsp + 216]
 mov qword ptr [rsp + 184], rax
  .section .rodata
  .Lbynamefn2: .string "size"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn2]
 lea rsi, [rsp + 176]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rsp + 160], rax
 mov qword ptr [rsp + 168], rdx
 cmp eax, 99
 je xchain0_n3_α
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n3_α
# IR_LIT_INTEGER
 xchain0_n2_α:
 mov qword ptr [rsp + 240], 6
 mov rax, qword ptr [rip + .Lx3_0]
 mov qword ptr [rsp + 248], rax
 jmp xchain0_n4_α
 xchain0_n2_β:
 jmp xchain0_n3_α
.Lx3_0:
 .quad 3
# IR_LIT_INTEGER
 xchain0_n3_α:
 mov qword ptr [rsp + 432], 6
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [rsp + 440], rax
 jmp xchain0_n5_α
 xchain0_n3_β:
 jmp xchain0_n8_α
.Lx4_0:
 .quad 12
 xchain0_n4_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+160] -> [zr+112]
 mov rax, qword ptr [rsp + 160]
 mov qword ptr [rsp + 112], rax
 mov rax, qword ptr [rsp + 168]
 mov qword ptr [rsp + 120], rax
# marshal arg1 = producer-box slot [zr+240] -> [zr+128]
 mov rax, qword ptr [rsp + 240]
 mov qword ptr [rsp + 128], rax
 mov rax, qword ptr [rsp + 248]
 mov qword ptr [rsp + 136], rax
  .section .rodata
  .Lbynamefn5: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn5]
 lea rsi, [rsp + 112]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 96], rax
 mov qword ptr [rsp + 104], rdx
 cmp eax, 99
 je xchain0_n3_α
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp xchain0_n3_α
 xchain0_n5_α:
# BOX CALL size(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+432] -> [zr+400]
 mov rax, qword ptr [rsp + 432]
 mov qword ptr [rsp + 400], rax
 mov rax, qword ptr [rsp + 440]
 mov qword ptr [rsp + 408], rax
  .section .rodata
  .Lbynamefn6: .string "size"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn6]
 lea rsi, [rsp + 400]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rsp + 384], rax
 mov qword ptr [rsp + 392], rdx
 cmp eax, 99
 je xchain0_n8_α
 jmp xchain0_n7_α
 xchain0_n5_β:
 jmp xchain0_n8_α
# IR_LIT_STRING
 xchain0_n6_α:
 mov qword ptr [rsp + 288], 1
 mov rax, qword ptr [rip + .Lx7_0]
 mov qword ptr [rsp + 296], rax
 jmp xchain0_n9_α
 xchain0_n6_β:
 jmp main_γ
.Lx7_0:
 .quad .Lx7_0_s
.Lx7_0_s:
 .string "FAIL 811/001: size of 3-char string"
# IR_LIT_INTEGER
 xchain0_n7_α:
 mov qword ptr [rsp + 464], 6
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [rsp + 472], rax
 jmp xchain0_n10_α
 xchain0_n7_β:
 jmp xchain0_n8_α
.Lx8_0:
 .quad 2
# IR_LIT_STRING
 xchain0_n8_α:
 mov qword ptr [rsp + 656], 1
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [rsp + 664], rax
 jmp xchain0_n11_α
 xchain0_n8_β:
 jmp xchain0_n14_α
.Lx9_0:
 .quad .Lx9_0_s
.Lx9_0_s:
 .string ""
# IR_ASSIGN gva
 xchain0_n9_α:
 mov rax, qword ptr [rsp + 288]
 mov rdx, qword ptr [rsp + 296]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rsp + 272], rax
 mov qword ptr [rsp + 280], rdx
 jmp main_γ
 xchain0_n9_β:
 jmp main_γ
 xchain0_n10_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+384] -> [zr+336]
 mov rax, qword ptr [rsp + 384]
 mov qword ptr [rsp + 336], rax
 mov rax, qword ptr [rsp + 392]
 mov qword ptr [rsp + 344], rax
# marshal arg1 = producer-box slot [zr+464] -> [zr+352]
 mov rax, qword ptr [rsp + 464]
 mov qword ptr [rsp + 352], rax
 mov rax, qword ptr [rsp + 472]
 mov qword ptr [rsp + 360], rax
  .section .rodata
  .Lbynamefn11: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn11]
 lea rsi, [rsp + 336]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 320], rax
 mov qword ptr [rsp + 328], rdx
 cmp eax, 99
 je xchain0_n8_α
 jmp xchain0_n12_α
 xchain0_n10_β:
 jmp xchain0_n8_α
 xchain0_n11_α:
# BOX CALL size(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+656] -> [zr+624]
 mov rax, qword ptr [rsp + 656]
 mov qword ptr [rsp + 624], rax
 mov rax, qword ptr [rsp + 664]
 mov qword ptr [rsp + 632], rax
  .section .rodata
  .Lbynamefn12: .string "size"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn12]
 lea rsi, [rsp + 624]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rsp + 608], rax
 mov qword ptr [rsp + 616], rdx
 cmp eax, 99
 je xchain0_n14_α
 jmp xchain0_n13_α
 xchain0_n11_β:
 jmp xchain0_n14_α
# IR_LIT_STRING
 xchain0_n12_α:
 mov qword ptr [rsp + 512], 1
 mov rax, qword ptr [rip + .Lx13_0]
 mov qword ptr [rsp + 520], rax
 jmp xchain0_n15_α
 xchain0_n12_β:
 jmp main_γ
.Lx13_0:
 .quad .Lx13_0_s
.Lx13_0_s:
 .string "FAIL 811/002: size of integer 12 = 2 digits"
# IR_LIT_INTEGER
 xchain0_n13_α:
 mov qword ptr [rsp + 688], 6
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [rsp + 696], rax
 jmp xchain0_n16_α
 xchain0_n13_β:
 jmp xchain0_n14_α
.Lx14_0:
 .quad 0
# IR_LIT_STRING
 xchain0_n14_α:
 mov qword ptr [rsp + 784], 1
 mov rax, qword ptr [rip + .Lx15_0]
 mov qword ptr [rsp + 792], rax
 jmp xchain0_n17_α
 xchain0_n14_β:
 jmp main_γ
.Lx15_0:
 .quad .Lx15_0_s
.Lx15_0_s:
 .string "PASS 811_size (3/3)"
# IR_ASSIGN gva
 xchain0_n15_α:
 mov rax, qword ptr [rsp + 512]
 mov rdx, qword ptr [rsp + 520]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rsp + 496], rax
 mov qword ptr [rsp + 504], rdx
 jmp main_γ
 xchain0_n15_β:
 jmp main_γ
 xchain0_n16_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+608] -> [zr+560]
 mov rax, qword ptr [rsp + 608]
 mov qword ptr [rsp + 560], rax
 mov rax, qword ptr [rsp + 616]
 mov qword ptr [rsp + 568], rax
# marshal arg1 = producer-box slot [zr+688] -> [zr+576]
 mov rax, qword ptr [rsp + 688]
 mov qword ptr [rsp + 576], rax
 mov rax, qword ptr [rsp + 696]
 mov qword ptr [rsp + 584], rax
  .section .rodata
  .Lbynamefn17: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn17]
 lea rsi, [rsp + 560]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 544], rax
 mov qword ptr [rsp + 552], rdx
 cmp eax, 99
 je xchain0_n14_α
 jmp xchain0_n18_α
 xchain0_n16_β:
 jmp xchain0_n14_α
# IR_ASSIGN gva
 xchain0_n17_α:
 mov rax, qword ptr [rsp + 784]
 mov rdx, qword ptr [rsp + 792]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rsp + 768], rax
 mov qword ptr [rsp + 776], rdx
 jmp main_γ
 xchain0_n17_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n18_α:
 mov qword ptr [rsp + 736], 1
 mov rax, qword ptr [rip + .Lx19_0]
 mov qword ptr [rsp + 744], rax
 jmp xchain0_n19_α
 xchain0_n18_β:
 jmp main_γ
.Lx19_0:
 .quad .Lx19_0_s
.Lx19_0_s:
 .string "FAIL 811/003: size of null = 0"
# IR_ASSIGN gva
 xchain0_n19_α:
 mov rax, qword ptr [rsp + 736]
 mov rdx, qword ptr [rsp + 744]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rsp + 720], rax
 mov qword ptr [rsp + 728], rdx
 jmp main_γ
 xchain0_n19_β:
 jmp main_γ
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rsp + 824]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rsp + 824]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
add rsp, 65544
ret
