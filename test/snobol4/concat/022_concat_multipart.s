  .intel_syntax noprefix
  .text
  .globl main
main:
  push rbp
  mov rbp, rsp
  push rdi
  push rsi
  call core_lib_init@PLT
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
  mov qword ptr [r12 + 344], rsp
 push rsi
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, [rsp + 8]
 mov qword ptr [r12 + 336], rax
 pop rsi
main_α_body:
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [r12 + 208], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 216], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp main_γ
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "a"
# IR_LIT_STRING
 xchain0_n1_α:
 mov qword ptr [r12 + 240], 1
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [r12 + 248], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp main_γ
.Lx2_0:
 .quad .Lx2_0_s
.Lx2_0_s:
 .string "b"
 xchain0_n2_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 208]
 mov rsi, qword ptr [r12 + 216]
 mov rdx, qword ptr [r12 + 240]
 mov rcx, qword ptr [r12 + 248]
 call str_concat_d@PLT
 mov qword ptr [r12 + 176], rax
 mov qword ptr [r12 + 184], rdx
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n3_α:
 mov qword ptr [r12 + 272], 1
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [r12 + 280], rax
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp main_γ
.Lx4_0:
 .quad .Lx4_0_s
.Lx4_0_s:
 .string "c"
 xchain0_n4_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 176]
 mov rsi, qword ptr [r12 + 184]
 mov rdx, qword ptr [r12 + 272]
 mov rcx, qword ptr [r12 + 280]
 call str_concat_d@PLT
 mov qword ptr [r12 + 144], rax
 mov qword ptr [r12 + 152], rdx
 jmp xchain0_n5_α
 xchain0_n4_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n5_α:
 mov qword ptr [r12 + 304], 1
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [r12 + 312], rax
 jmp xchain0_n6_α
 xchain0_n5_β:
 jmp main_γ
.Lx6_0:
 .quad .Lx6_0_s
.Lx6_0_s:
 .string "d"
 xchain0_n6_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 144]
 mov rsi, qword ptr [r12 + 152]
 mov rdx, qword ptr [r12 + 304]
 mov rcx, qword ptr [r12 + 312]
 call str_concat_d@PLT
 mov qword ptr [r12 + 112], rax
 mov qword ptr [r12 + 120], rdx
 jmp xchain0_n7_α
 xchain0_n6_β:
 jmp main_γ
# IR_ASSIGN global
 xchain0_n7_α:
 mov rsi, qword ptr [r12 + 112]
 mov rdx, qword ptr [r12 + 120]
 mov rdi, qword ptr [rip + .Lx8_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 jmp main_γ
 xchain0_n7_β:
 jmp main_γ
.Lx8_0:
 .quad .Lx8_0_s
.Lx8_0_s:
 .string "OUTPUT"
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [r12 + 344]
pop r12
ret
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [r12 + 336]
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
main_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, qword ptr [r12 + 344]
pop r12
ret
