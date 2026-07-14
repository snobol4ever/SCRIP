  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "output"
  .align 8
__gva_names:
  .quad .Lgvan0
  .section .bss
  .align 16
__gva: .space 16, 0
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
  mov edx, 1
  call gva_register@PLT
  mov rbx, rax
  sub rsp, 65536
  mov rdi, rsp
  mov ecx, 8192
  xor eax, eax
  rep stosq
  mov rdi, rsp
  xor esi, esi
  call main_α
  xor eax, eax
  add rsp, 65536
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
  mov qword ptr [r12 + 1048], rsp
 push rsi
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, [rsp + 8]
 mov qword ptr [r12 + 1040], rax
 pop rsi
main_α_body:
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [r12 + 208], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 216], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n3_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "hello"
 xchain0_n1_α:
# BOX CALL datatype(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+208] -> [zr+176]
 mov rax, qword ptr [r12 + 208]
 mov qword ptr [r12 + 176], rax
 mov rax, qword ptr [r12 + 216]
 mov qword ptr [r12 + 184], rax
  .section .rodata
  .Lbynamefn2: .string "datatype"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn2]
 lea rsi, [r12 + 176]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 160], rax
 mov qword ptr [r12 + 168], rdx
 cmp eax, 99
 je xchain0_n3_α
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n3_α
# IR_LIT_STRING
 xchain0_n2_α:
 mov qword ptr [r12 + 240], 1
 mov rax, qword ptr [rip + .Lx3_0]
 mov qword ptr [r12 + 248], rax
 jmp xchain0_n4_α
 xchain0_n2_β:
 jmp xchain0_n3_α
.Lx3_0:
 .quad .Lx3_0_s
.Lx3_0_s:
 .string "string"
# IR_LIT_INTEGER
 xchain0_n3_α:
 mov qword ptr [r12 + 432], 6
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [r12 + 440], rax
 jmp xchain0_n5_α
 xchain0_n3_β:
 jmp xchain0_n8_α
.Lx4_0:
 .quad 12
 xchain0_n4_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+160] -> [zr+112]
 mov rax, qword ptr [r12 + 160]
 mov qword ptr [r12 + 112], rax
 mov rax, qword ptr [r12 + 168]
 mov qword ptr [r12 + 120], rax
# marshal arg1 = producer-box slot [zr+240] -> [zr+128]
 mov rax, qword ptr [r12 + 240]
 mov qword ptr [r12 + 128], rax
 mov rax, qword ptr [r12 + 248]
 mov qword ptr [r12 + 136], rax
  .section .rodata
  .Lbynamefn5: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn5]
 lea rsi, [r12 + 112]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 cmp eax, 99
 je xchain0_n3_α
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp xchain0_n3_α
 xchain0_n5_α:
# BOX CALL datatype(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+432] -> [zr+400]
 mov rax, qword ptr [r12 + 432]
 mov qword ptr [r12 + 400], rax
 mov rax, qword ptr [r12 + 440]
 mov qword ptr [r12 + 408], rax
  .section .rodata
  .Lbynamefn6: .string "datatype"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn6]
 lea rsi, [r12 + 400]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 384], rax
 mov qword ptr [r12 + 392], rdx
 cmp eax, 99
 je xchain0_n8_α
 jmp xchain0_n7_α
 xchain0_n5_β:
 jmp xchain0_n8_α
# IR_LIT_STRING
 xchain0_n6_α:
 mov qword ptr [r12 + 288], 1
 mov rax, qword ptr [rip + .Lx7_0]
 mov qword ptr [r12 + 296], rax
 jmp xchain0_n9_α
 xchain0_n6_β:
 jmp main_γ
.Lx7_0:
 .quad .Lx7_0_s
.Lx7_0_s:
 .string "FAIL 911/001: string literal datatype"
# IR_LIT_STRING
 xchain0_n7_α:
 mov qword ptr [r12 + 464], 1
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [r12 + 472], rax
 jmp xchain0_n10_α
 xchain0_n7_β:
 jmp xchain0_n8_α
.Lx8_0:
 .quad .Lx8_0_s
.Lx8_0_s:
 .string "integer"
# IR_LIT_REAL
 xchain0_n8_α:
 mov qword ptr [r12 + 656], 7
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [r12 + 664], rax
 jmp xchain0_n11_α
 xchain0_n8_β:
 jmp xchain0_n14_α
.Lx9_0:
 .quad 4608668606677049672
# IR_ASSIGN gva
 xchain0_n9_α:
 mov rax, qword ptr [r12 + 288]
 mov rdx, qword ptr [r12 + 296]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 272], rax
 mov qword ptr [r12 + 280], rdx
 jmp main_γ
 xchain0_n9_β:
 jmp main_γ
 xchain0_n10_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+384] -> [zr+336]
 mov rax, qword ptr [r12 + 384]
 mov qword ptr [r12 + 336], rax
 mov rax, qword ptr [r12 + 392]
 mov qword ptr [r12 + 344], rax
# marshal arg1 = producer-box slot [zr+464] -> [zr+352]
 mov rax, qword ptr [r12 + 464]
 mov qword ptr [r12 + 352], rax
 mov rax, qword ptr [r12 + 472]
 mov qword ptr [r12 + 360], rax
  .section .rodata
  .Lbynamefn11: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn11]
 lea rsi, [r12 + 336]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 320], rax
 mov qword ptr [r12 + 328], rdx
 cmp eax, 99
 je xchain0_n8_α
 jmp xchain0_n12_α
 xchain0_n10_β:
 jmp xchain0_n8_α
 xchain0_n11_α:
# BOX CALL datatype(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+656] -> [zr+624]
 mov rax, qword ptr [r12 + 656]
 mov qword ptr [r12 + 624], rax
 mov rax, qword ptr [r12 + 664]
 mov qword ptr [r12 + 632], rax
  .section .rodata
  .Lbynamefn12: .string "datatype"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn12]
 lea rsi, [r12 + 624]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 608], rax
 mov qword ptr [r12 + 616], rdx
 cmp eax, 99
 je xchain0_n14_α
 jmp xchain0_n13_α
 xchain0_n11_β:
 jmp xchain0_n14_α
# IR_LIT_STRING
 xchain0_n12_α:
 mov qword ptr [r12 + 512], 1
 mov rax, qword ptr [rip + .Lx13_0]
 mov qword ptr [r12 + 520], rax
 jmp xchain0_n15_α
 xchain0_n12_β:
 jmp main_γ
.Lx13_0:
 .quad .Lx13_0_s
.Lx13_0_s:
 .string "FAIL 911/002: integer datatype"
# IR_LIT_STRING
 xchain0_n13_α:
 mov qword ptr [r12 + 688], 1
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [r12 + 696], rax
 jmp xchain0_n16_α
 xchain0_n13_β:
 jmp xchain0_n14_α
.Lx14_0:
 .quad .Lx14_0_s
.Lx14_0_s:
 .string "real"
# IR_LIT_STRING
 xchain0_n14_α:
 mov qword ptr [r12 + 880], 1
 mov rax, qword ptr [rip + .Lx15_0]
 mov qword ptr [r12 + 888], rax
 jmp xchain0_n17_α
 xchain0_n14_β:
 jmp xchain0_n20_α
.Lx15_0:
 .quad .Lx15_0_s
.Lx15_0_s:
 .string ""
# IR_ASSIGN gva
 xchain0_n15_α:
 mov rax, qword ptr [r12 + 512]
 mov rdx, qword ptr [r12 + 520]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 496], rax
 mov qword ptr [r12 + 504], rdx
 jmp main_γ
 xchain0_n15_β:
 jmp main_γ
 xchain0_n16_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+608] -> [zr+560]
 mov rax, qword ptr [r12 + 608]
 mov qword ptr [r12 + 560], rax
 mov rax, qword ptr [r12 + 616]
 mov qword ptr [r12 + 568], rax
# marshal arg1 = producer-box slot [zr+688] -> [zr+576]
 mov rax, qword ptr [r12 + 688]
 mov qword ptr [r12 + 576], rax
 mov rax, qword ptr [r12 + 696]
 mov qword ptr [r12 + 584], rax
  .section .rodata
  .Lbynamefn17: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn17]
 lea rsi, [r12 + 560]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 544], rax
 mov qword ptr [r12 + 552], rdx
 cmp eax, 99
 je xchain0_n14_α
 jmp xchain0_n18_α
 xchain0_n16_β:
 jmp xchain0_n14_α
 xchain0_n17_α:
# BOX CALL datatype(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+880] -> [zr+848]
 mov rax, qword ptr [r12 + 880]
 mov qword ptr [r12 + 848], rax
 mov rax, qword ptr [r12 + 888]
 mov qword ptr [r12 + 856], rax
  .section .rodata
  .Lbynamefn18: .string "datatype"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn18]
 lea rsi, [r12 + 848]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 832], rax
 mov qword ptr [r12 + 840], rdx
 cmp eax, 99
 je xchain0_n20_α
 jmp xchain0_n19_α
 xchain0_n17_β:
 jmp xchain0_n20_α
# IR_LIT_STRING
 xchain0_n18_α:
 mov qword ptr [r12 + 736], 1
 mov rax, qword ptr [rip + .Lx19_0]
 mov qword ptr [r12 + 744], rax
 jmp xchain0_n21_α
 xchain0_n18_β:
 jmp main_γ
.Lx19_0:
 .quad .Lx19_0_s
.Lx19_0_s:
 .string "FAIL 911/003: real datatype"
# IR_LIT_STRING
 xchain0_n19_α:
 mov qword ptr [r12 + 912], 1
 mov rax, qword ptr [rip + .Lx20_0]
 mov qword ptr [r12 + 920], rax
 jmp xchain0_n22_α
 xchain0_n19_β:
 jmp xchain0_n20_α
.Lx20_0:
 .quad .Lx20_0_s
.Lx20_0_s:
 .string "string"
# IR_LIT_STRING
 xchain0_n20_α:
 mov qword ptr [r12 + 1008], 1
 mov rax, qword ptr [rip + .Lx21_0]
 mov qword ptr [r12 + 1016], rax
 jmp xchain0_n23_α
 xchain0_n20_β:
 jmp main_γ
.Lx21_0:
 .quad .Lx21_0_s
.Lx21_0_s:
 .string "PASS 911_datatype (4/4)"
# IR_ASSIGN gva
 xchain0_n21_α:
 mov rax, qword ptr [r12 + 736]
 mov rdx, qword ptr [r12 + 744]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 720], rax
 mov qword ptr [r12 + 728], rdx
 jmp main_γ
 xchain0_n21_β:
 jmp main_γ
 xchain0_n22_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+832] -> [zr+784]
 mov rax, qword ptr [r12 + 832]
 mov qword ptr [r12 + 784], rax
 mov rax, qword ptr [r12 + 840]
 mov qword ptr [r12 + 792], rax
# marshal arg1 = producer-box slot [zr+912] -> [zr+800]
 mov rax, qword ptr [r12 + 912]
 mov qword ptr [r12 + 800], rax
 mov rax, qword ptr [r12 + 920]
 mov qword ptr [r12 + 808], rax
  .section .rodata
  .Lbynamefn23: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn23]
 lea rsi, [r12 + 784]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 768], rax
 mov qword ptr [r12 + 776], rdx
 cmp eax, 99
 je xchain0_n20_α
 jmp xchain0_n24_α
 xchain0_n22_β:
 jmp xchain0_n20_α
# IR_ASSIGN gva
 xchain0_n23_α:
 mov rax, qword ptr [r12 + 1008]
 mov rdx, qword ptr [r12 + 1016]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 992], rax
 mov qword ptr [r12 + 1000], rdx
 jmp main_γ
 xchain0_n23_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n24_α:
 mov qword ptr [r12 + 960], 1
 mov rax, qword ptr [rip + .Lx25_0]
 mov qword ptr [r12 + 968], rax
 jmp xchain0_n25_α
 xchain0_n24_β:
 jmp main_γ
.Lx25_0:
 .quad .Lx25_0_s
.Lx25_0_s:
 .string "FAIL 911/004: null is string"
# IR_ASSIGN gva
 xchain0_n25_α:
 mov rax, qword ptr [r12 + 960]
 mov rdx, qword ptr [r12 + 968]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 944], rax
 mov qword ptr [r12 + 952], rdx
 jmp main_γ
 xchain0_n25_β:
 jmp main_γ
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [r12 + 1048]
pop r12
ret
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [r12 + 1040]
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
main_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, qword ptr [r12 + 1048]
pop r12
ret
