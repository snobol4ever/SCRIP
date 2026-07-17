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
  mov qword ptr [rsp + 632], rsp
  mov r12, qword ptr [1879048192]
main_α_body:
# IR_LIT_INTEGER
 xchain0_n0_α:
 mov qword ptr [rsp + 192], 6
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [rsp + 200], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n3_α
.Lx1_0:
 .quad 5
# IR_UNOP
 xchain0_n1_α:
 mov rdi, qword ptr [rsp + 192]
 mov rsi, qword ptr [rsp + 200]
 call rt_num_neg@PLT
 mov qword ptr [rsp + 160], rax
 mov qword ptr [rsp + 168], rdx
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n3_α
# IR_LIT_INTEGER
 xchain0_n2_α:
 mov qword ptr [rsp + 256], 6
 mov rax, qword ptr [rip + .Lx3_0]
 mov qword ptr [rsp + 264], rax
 jmp xchain0_n4_α
 xchain0_n2_β:
 jmp xchain0_n3_α
.Lx3_0:
 .quad 0
# IR_LIT_STRING
 xchain0_n3_α:
 mov qword ptr [rsp + 464], 1
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [rsp + 472], rax
 jmp xchain0_n5_α
 xchain0_n3_β:
 jmp xchain0_n8_α
.Lx4_0:
 .quad .Lx4_0_s
.Lx4_0_s:
 .string "4"
# IR_LIT_INTEGER
 xchain0_n4_α:
 mov qword ptr [rsp + 288], 6
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [rsp + 296], rax
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp xchain0_n3_α
.Lx5_0:
 .quad 5
# IR_UNOP
 xchain0_n5_α:
 mov rdi, qword ptr [rsp + 464]
 mov rsi, qword ptr [rsp + 472]
 call rt_num_pos@PLT
 mov qword ptr [rsp + 432], rax
 mov qword ptr [rsp + 440], rdx
 jmp xchain0_n7_α
 xchain0_n5_β:
 jmp xchain0_n8_α
 xchain0_n6_α:
# IR_BINOP_ARITH
.Lx7_1:
 mov rax, 0
 mov rcx, 5
 sub rax, rcx
 mov qword ptr [rsp + 224], 6
 mov qword ptr [rsp + 232], rax
 jmp xchain0_n9_α
.Lx7_0:
 mov rdi, qword ptr [rsp + 256]
 mov rsi, qword ptr [rsp + 264]
 mov rdx, qword ptr [rsp + 288]
 mov rcx, qword ptr [rsp + 296]
 mov r8d, 1
 lea r9, [rsp + 224]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx7_3
.Lx7_2:
 mov rdi, qword ptr [rsp + 256]
 mov rsi, qword ptr [rsp + 264]
 mov rdx, qword ptr [rsp + 288]
 mov rcx, qword ptr [rsp + 296]
 mov r8d, 1
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n3_α
 mov qword ptr [rsp + 224], rax
 mov qword ptr [rsp + 232], rdx
.Lx7_3:
 jmp xchain0_n9_α
 xchain0_n6_β:
 jmp xchain0_n3_α
# IR_LIT_INTEGER
 xchain0_n7_α:
 mov qword ptr [rsp + 496], 6
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [rsp + 504], rax
 jmp xchain0_n10_α
 xchain0_n7_β:
 jmp xchain0_n8_α
.Lx8_0:
 .quad 4
# IR_LIT_STRING
 xchain0_n8_α:
 mov qword ptr [rsp + 592], 1
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [rsp + 600], rax
 jmp xchain0_n11_α
 xchain0_n8_β:
 jmp main_γ
.Lx9_0:
 .quad .Lx9_0_s
.Lx9_0_s:
 .string "PASS 411_arith_unary (2/2)"
 xchain0_n9_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+160] -> [zr+112]
 mov rax, qword ptr [rsp + 160]
 mov qword ptr [rsp + 112], rax
 mov rax, qword ptr [rsp + 168]
 mov qword ptr [rsp + 120], rax
# marshal arg1 = producer-box slot [zr+224] -> [zr+128]
 mov rax, qword ptr [rsp + 224]
 mov qword ptr [rsp + 128], rax
 mov rax, qword ptr [rsp + 232]
 mov qword ptr [rsp + 136], rax
  .section .rodata
  .Lbynamefn10: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn10]
 lea rsi, [rsp + 112]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 96], rax
 mov qword ptr [rsp + 104], rdx
 cmp eax, 99
 je xchain0_n3_α
 jmp xchain0_n12_α
 xchain0_n9_β:
 jmp xchain0_n3_α
 xchain0_n10_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+432] -> [zr+384]
 mov rax, qword ptr [rsp + 432]
 mov qword ptr [rsp + 384], rax
 mov rax, qword ptr [rsp + 440]
 mov qword ptr [rsp + 392], rax
# marshal arg1 = producer-box slot [zr+496] -> [zr+400]
 mov rax, qword ptr [rsp + 496]
 mov qword ptr [rsp + 400], rax
 mov rax, qword ptr [rsp + 504]
 mov qword ptr [rsp + 408], rax
  .section .rodata
  .Lbynamefn11: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn11]
 lea rsi, [rsp + 384]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 368], rax
 mov qword ptr [rsp + 376], rdx
 cmp eax, 99
 je xchain0_n8_α
 jmp xchain0_n13_α
 xchain0_n10_β:
 jmp xchain0_n8_α
# IR_ASSIGN gva
 xchain0_n11_α:
 mov rax, qword ptr [rsp + 592]
 mov rdx, qword ptr [rsp + 600]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rsp + 576], rax
 mov qword ptr [rsp + 584], rdx
 jmp main_γ
 xchain0_n11_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n12_α:
 mov qword ptr [rsp + 336], 1
 mov rax, qword ptr [rip + .Lx13_0]
 mov qword ptr [rsp + 344], rax
 jmp xchain0_n14_α
 xchain0_n12_β:
 jmp main_γ
.Lx13_0:
 .quad .Lx13_0_s
.Lx13_0_s:
 .string "FAIL 411/001: unary minus"
# IR_LIT_STRING
 xchain0_n13_α:
 mov qword ptr [rsp + 544], 1
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [rsp + 552], rax
 jmp xchain0_n15_α
 xchain0_n13_β:
 jmp main_γ
.Lx14_0:
 .quad .Lx14_0_s
.Lx14_0_s:
 .string "FAIL 411/002: unary plus string->int"
# IR_ASSIGN gva
 xchain0_n14_α:
 mov rax, qword ptr [rsp + 336]
 mov rdx, qword ptr [rsp + 344]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rsp + 320], rax
 mov qword ptr [rsp + 328], rdx
 jmp main_γ
 xchain0_n14_β:
 jmp main_γ
# IR_ASSIGN gva
 xchain0_n15_α:
 mov rax, qword ptr [rsp + 544]
 mov rdx, qword ptr [rsp + 552]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rsp + 528], rax
 mov qword ptr [rsp + 536], rdx
 jmp main_γ
 xchain0_n15_β:
 jmp main_γ
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rsp + 632]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rsp + 632]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
add rsp, 65544
ret
