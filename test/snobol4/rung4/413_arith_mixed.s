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
  push rbp
  mov rbp, rsp
  push rdi
  push rsi
  call core_lib_init@PLT
  lea rdi, [rip + __gva_names]
  lea rsi, [rip + __gva]
  mov edx, 1
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
  mov qword ptr [r12 + 632], rsp
 push rsi
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, [rsp + 8]
 mov qword ptr [r12 + 624], rax
 pop rsi
main_α_body:
# IR_LIT_INTEGER
 xchain0_n0_α:
 mov qword ptr [r12 + 192], 6
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 200], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n4_α
.Lx1_0:
 .quad 1
# IR_LIT_REAL
 xchain0_n1_α:
 mov qword ptr [r12 + 224], 7
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [r12 + 232], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n4_α
.Lx2_0:
 .quad 4611686018427387904
 xchain0_n2_α:
# IR_BINOP_ARITH_REAL
 mov rdi, qword ptr [r12 + 192]
 mov rsi, qword ptr [r12 + 200]
 mov rdx, qword ptr [r12 + 224]
 mov rcx, qword ptr [r12 + 232]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n4_α
 mov qword ptr [r12 + 160], rax
 mov qword ptr [r12 + 168], rdx
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n4_α
# IR_LIT_REAL
 xchain0_n3_α:
 mov qword ptr [r12 + 256], 7
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [r12 + 264], rax
 jmp xchain0_n5_α
 xchain0_n3_β:
 jmp xchain0_n4_α
.Lx4_0:
 .quad 4613937818241073152
# IR_LIT_REAL
 xchain0_n4_α:
 mov qword ptr [r12 + 432], 7
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [r12 + 440], rax
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp xchain0_n11_α
.Lx5_0:
 .quad 4613937818241073152
 xchain0_n5_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+160] -> [zr+112]
 mov rax, qword ptr [r12 + 160]
 mov qword ptr [r12 + 112], rax
 mov rax, qword ptr [r12 + 168]
 mov qword ptr [r12 + 120], rax
# marshal arg1 = producer-box slot [zr+256] -> [zr+128]
 mov rax, qword ptr [r12 + 256]
 mov qword ptr [r12 + 128], rax
 mov rax, qword ptr [r12 + 264]
 mov qword ptr [r12 + 136], rax
  .section .rodata
  .Lbynamefn6: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn6]
 lea rsi, [r12 + 112]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 cmp eax, 99
 je xchain0_n4_α
 jmp xchain0_n7_α
 xchain0_n5_β:
 jmp xchain0_n4_α
# IR_LIT_INTEGER
 xchain0_n6_α:
 mov qword ptr [r12 + 464], 6
 mov rax, qword ptr [rip + .Lx7_0]
 mov qword ptr [r12 + 472], rax
 jmp xchain0_n8_α
 xchain0_n6_β:
 jmp xchain0_n11_α
.Lx7_0:
 .quad 2
# IR_LIT_STRING
 xchain0_n7_α:
 mov qword ptr [r12 + 304], 1
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [r12 + 312], rax
 jmp xchain0_n9_α
 xchain0_n7_β:
 jmp main_γ
.Lx8_0:
 .quad .Lx8_0_s
.Lx8_0_s:
 .string "FAIL 413/001: int+real promotes to real"
 xchain0_n8_α:
# IR_BINOP_ARITH_REAL
 mov rdi, qword ptr [r12 + 432]
 mov rsi, qword ptr [r12 + 440]
 mov rdx, qword ptr [r12 + 464]
 mov rcx, qword ptr [r12 + 472]
 mov r8d, 3
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n11_α
 mov qword ptr [r12 + 400], rax
 mov qword ptr [r12 + 408], rdx
 jmp xchain0_n10_α
 xchain0_n8_β:
 jmp xchain0_n11_α
# IR_ASSIGN gva
 xchain0_n9_α:
 mov rax, qword ptr [r12 + 304]
 mov rdx, qword ptr [r12 + 312]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 288], rax
 mov qword ptr [r12 + 296], rdx
 jmp main_γ
 xchain0_n9_β:
 jmp main_γ
# IR_LIT_REAL
 xchain0_n10_α:
 mov qword ptr [r12 + 496], 7
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [r12 + 504], rax
 jmp xchain0_n12_α
 xchain0_n10_β:
 jmp xchain0_n11_α
.Lx11_0:
 .quad 4609434218613702656
# IR_LIT_STRING
 xchain0_n11_α:
 mov qword ptr [r12 + 592], 1
 mov rax, qword ptr [rip + .Lx12_0]
 mov qword ptr [r12 + 600], rax
 jmp xchain0_n13_α
 xchain0_n11_β:
 jmp main_γ
.Lx12_0:
 .quad .Lx12_0_s
.Lx12_0_s:
 .string "PASS 413_arith_mixed (2/2)"
 xchain0_n12_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+400] -> [zr+352]
 mov rax, qword ptr [r12 + 400]
 mov qword ptr [r12 + 352], rax
 mov rax, qword ptr [r12 + 408]
 mov qword ptr [r12 + 360], rax
# marshal arg1 = producer-box slot [zr+496] -> [zr+368]
 mov rax, qword ptr [r12 + 496]
 mov qword ptr [r12 + 368], rax
 mov rax, qword ptr [r12 + 504]
 mov qword ptr [r12 + 376], rax
  .section .rodata
  .Lbynamefn13: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn13]
 lea rsi, [r12 + 352]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 336], rax
 mov qword ptr [r12 + 344], rdx
 cmp eax, 99
 je xchain0_n11_α
 jmp xchain0_n14_α
 xchain0_n12_β:
 jmp xchain0_n11_α
# IR_ASSIGN gva
 xchain0_n13_α:
 mov rax, qword ptr [r12 + 592]
 mov rdx, qword ptr [r12 + 600]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 576], rax
 mov qword ptr [r12 + 584], rdx
 jmp main_γ
 xchain0_n13_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n14_α:
 mov qword ptr [r12 + 544], 1
 mov rax, qword ptr [rip + .Lx15_0]
 mov qword ptr [r12 + 552], rax
 jmp xchain0_n15_α
 xchain0_n14_β:
 jmp main_γ
.Lx15_0:
 .quad .Lx15_0_s
.Lx15_0_s:
 .string "FAIL 413/002: real/int promotes to real"
# IR_ASSIGN gva
 xchain0_n15_α:
 mov rax, qword ptr [r12 + 544]
 mov rdx, qword ptr [r12 + 552]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 528], rax
 mov qword ptr [r12 + 536], rdx
 jmp main_γ
 xchain0_n15_β:
 jmp main_γ
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [r12 + 632]
pop r12
ret
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [r12 + 624]
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
main_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, qword ptr [r12 + 632]
pop r12
ret
