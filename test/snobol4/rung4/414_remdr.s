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
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  mov qword ptr [rsp + 696], rsp
main_α_body:
# IR_LIT_INTEGER
 xchain0_n0_α:
 mov qword ptr [rsp + 224], 6
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [rsp + 232], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n4_α
.Lx1_0:
 .quad 10
# IR_LIT_INTEGER
 xchain0_n1_α:
 mov qword ptr [rsp + 256], 6
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [rsp + 264], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n4_α
.Lx2_0:
 .quad 3
 xchain0_n2_α:
# BOX CALL remdr(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
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
  .Lbynamefn3: .string "remdr"
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
# IR_LIT_INTEGER
 xchain0_n3_α:
 mov qword ptr [rsp + 288], 6
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [rsp + 296], rax
 jmp xchain0_n5_α
 xchain0_n3_β:
 jmp xchain0_n4_α
.Lx4_0:
 .quad 1
# IR_LIT_INTEGER
 xchain0_n4_α:
 mov qword ptr [rsp + 496], 6
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [rsp + 504], rax
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp xchain0_n11_α
.Lx5_0:
 .quad 11
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
 .string "FAIL 414/001: remdr(10,3)"
 xchain0_n8_α:
# BOX CALL remdr(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
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
  .Lbynamefn9: .string "remdr"
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
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [rsp + 320], rax
 mov qword ptr [rsp + 328], rdx
 jmp main_γ
 xchain0_n9_β:
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n10_α:
 mov qword ptr [rsp + 560], 6
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [rsp + 568], rax
 jmp xchain0_n12_α
 xchain0_n10_β:
 jmp xchain0_n11_α
.Lx11_0:
 .quad 1
# IR_LIT_STRING
 xchain0_n11_α:
 mov qword ptr [rsp + 656], 1
 mov rax, qword ptr [rip + .Lx12_0]
 mov qword ptr [rsp + 664], rax
 jmp xchain0_n13_α
 xchain0_n11_β:
 jmp main_γ
.Lx12_0:
 .quad .Lx12_0_s
.Lx12_0_s:
 .string "PASS 414_remdr (2/2)"
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
# IR_ASSIGN gva
 xchain0_n13_α:
 mov rax, qword ptr [rsp + 656]
 mov rdx, qword ptr [rsp + 664]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [rsp + 640], rax
 mov qword ptr [rsp + 648], rdx
 jmp main_γ
 xchain0_n13_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n14_α:
 mov qword ptr [rsp + 608], 1
 mov rax, qword ptr [rip + .Lx15_0]
 mov qword ptr [rsp + 616], rax
 jmp xchain0_n15_α
 xchain0_n14_β:
 jmp main_γ
.Lx15_0:
 .quad .Lx15_0_s
.Lx15_0_s:
 .string "FAIL 414/002: remdr(11,10)"
# IR_ASSIGN gva
 xchain0_n15_α:
 mov rax, qword ptr [rsp + 608]
 mov rdx, qword ptr [rsp + 616]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [rsp + 592], rax
 mov qword ptr [rsp + 600], rdx
 jmp main_γ
 xchain0_n15_β:
 jmp main_γ
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rsp + 696]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rsp + 696]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
add rsp, 65544
ret
