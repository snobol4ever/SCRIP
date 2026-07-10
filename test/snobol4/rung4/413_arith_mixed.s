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
 mov qword ptr [r12 + 384], rax
 pop rsi
main_α_body:
# IR_LIT_INTEGER
 xchain0_n0_α:
 mov qword ptr [r12 + 128], 6
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 136], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n4_α
.Lx1_0:
 .quad 1
# IR_LIT_REAL
 xchain0_n1_α:
 mov qword ptr [r12 + 144], 7
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [r12 + 152], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n4_α
.Lx2_0:
 .quad 4611686018427387904
 xchain0_n2_α:
# IR_BINOP_ARITH_REAL
 mov rdi, qword ptr [r12 + 128]
 mov rsi, qword ptr [r12 + 136]
 mov rdx, qword ptr [r12 + 144]
 mov rcx, qword ptr [r12 + 152]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n4_α
 mov qword ptr [r12 + 112], rax
 mov qword ptr [r12 + 120], rdx
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n4_α
# IR_LIT_REAL
 xchain0_n3_α:
 mov qword ptr [r12 + 160], 7
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [r12 + 168], rax
 jmp xchain0_n5_α
 xchain0_n3_β:
 jmp xchain0_n4_α
.Lx4_0:
 .quad 4613937818241073152
# IR_LIT_REAL
 xchain0_n4_α:
 mov qword ptr [r12 + 272], 7
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [r12 + 280], rax
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp xchain0_n11_α
.Lx5_0:
 .quad 4613937818241073152
 xchain0_n5_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+112] -> [zr+80]
 mov rax, qword ptr [r12 + 112]
 mov qword ptr [r12 + 80], rax
 mov rax, qword ptr [r12 + 120]
 mov qword ptr [r12 + 88], rax
# marshal arg1 = producer-box slot [zr+160] -> [zr+96]
 mov rax, qword ptr [r12 + 160]
 mov qword ptr [r12 + 96], rax
 mov rax, qword ptr [r12 + 168]
 mov qword ptr [r12 + 104], rax
  .section .rodata
  .Lbynamefn7: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn7]
 lea rsi, [r12 + 80]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 64], rax
 mov qword ptr [r12 + 72], rdx
 cmp eax, 99
 je xchain0_n4_α
 jmp xchain0_n7_α
xchain0_n5_β:
 jmp xchain0_n4_α
# IR_LIT_INTEGER
 xchain0_n6_α:
 mov qword ptr [r12 + 288], 6
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [r12 + 296], rax
 jmp xchain0_n8_α
 xchain0_n6_β:
 jmp xchain0_n11_α
.Lx8_0:
 .quad 2
# IR_LIT_STRING
 xchain0_n7_α:
 mov qword ptr [r12 + 192], 1
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [r12 + 200], rax
 jmp xchain0_n9_α
 xchain0_n7_β:
 jmp main_γ
.Lx9_0:
 .quad .Lx9_0_s
.Lx9_0_s:
 .string "FAIL 413/001: int+real promotes to real"
 xchain0_n8_α:
# IR_BINOP_ARITH_REAL
 mov rdi, qword ptr [r12 + 272]
 mov rsi, qword ptr [r12 + 280]
 mov rdx, qword ptr [r12 + 288]
 mov rcx, qword ptr [r12 + 296]
 mov r8d, 3
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n11_α
 mov qword ptr [r12 + 256], rax
 mov qword ptr [r12 + 264], rdx
 jmp xchain0_n10_α
 xchain0_n8_β:
 jmp xchain0_n11_α
# IR_ASSIGN gva
 xchain0_n9_α:
 mov rax, qword ptr [r12 + 192]
 mov rdx, qword ptr [r12 + 200]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 176], rax
 mov qword ptr [r12 + 184], rdx
 jmp main_γ
 xchain0_n9_β:
 jmp main_γ
# IR_LIT_REAL
 xchain0_n10_α:
 mov qword ptr [r12 + 304], 7
 mov rax, qword ptr [rip + .Lx12_0]
 mov qword ptr [r12 + 312], rax
 jmp xchain0_n12_α
 xchain0_n10_β:
 jmp xchain0_n11_α
.Lx12_0:
 .quad 4609434218613702656
# IR_LIT_STRING
 xchain0_n11_α:
 mov qword ptr [r12 + 368], 1
 mov rax, qword ptr [rip + .Lx13_0]
 mov qword ptr [r12 + 376], rax
 jmp xchain0_n13_α
 xchain0_n11_β:
 jmp main_γ
.Lx13_0:
 .quad .Lx13_0_s
.Lx13_0_s:
 .string "PASS 413_arith_mixed (2/2)"
 xchain0_n12_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+256] -> [zr+224]
 mov rax, qword ptr [r12 + 256]
 mov qword ptr [r12 + 224], rax
 mov rax, qword ptr [r12 + 264]
 mov qword ptr [r12 + 232], rax
# marshal arg1 = producer-box slot [zr+304] -> [zr+240]
 mov rax, qword ptr [r12 + 304]
 mov qword ptr [r12 + 240], rax
 mov rax, qword ptr [r12 + 312]
 mov qword ptr [r12 + 248], rax
  .section .rodata
  .Lbynamefn15: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn15]
 lea rsi, [r12 + 224]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 208], rax
 mov qword ptr [r12 + 216], rdx
 cmp eax, 99
 je xchain0_n11_α
 jmp xchain0_n14_α
xchain0_n12_β:
 jmp xchain0_n11_α
# IR_ASSIGN gva
 xchain0_n13_α:
 mov rax, qword ptr [r12 + 368]
 mov rdx, qword ptr [r12 + 376]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 352], rax
 mov qword ptr [r12 + 360], rdx
 jmp main_γ
 xchain0_n13_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n14_α:
 mov qword ptr [r12 + 336], 1
 mov rax, qword ptr [rip + .Lx17_0]
 mov qword ptr [r12 + 344], rax
 jmp xchain0_n15_α
 xchain0_n14_β:
 jmp main_γ
.Lx17_0:
 .quad .Lx17_0_s
.Lx17_0_s:
 .string "FAIL 413/002: real/int promotes to real"
# IR_ASSIGN gva
 xchain0_n15_α:
 mov rax, qword ptr [r12 + 336]
 mov rdx, qword ptr [r12 + 344]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 320], rax
 mov qword ptr [r12 + 328], rdx
 jmp main_γ
 xchain0_n15_β:
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
 mov rdi, qword ptr [r12 + 384]
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
