  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "X"
  .Lgvan1: .string "V"
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
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
  lea rdi, [rip + __gva_names]
  lea rsi, [rip + __gva]
  mov edx, 2
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
  mov qword ptr [rsp + 424], rsp
main_α_body:
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [rsp + 112], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [rsp + 120], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n2_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "12345abc"
# IR_ASSIGN gva
 xchain0_n1_α:
 mov rax, qword ptr [rsp + 112]
 mov rdx, qword ptr [rsp + 120]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [rsp + 96], rax
 mov qword ptr [rsp + 104], rdx
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n2_α
# IR_VAR
 xchain0_n2_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [rsp + 288], rax
 mov qword ptr [rsp + 296], rdx
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n4_α
# IR_MATCH_HEAD
 xchain0_n3_α:
 mov rdi, qword ptr [rsp + 288]
 mov rsi, qword ptr [rsp + 296]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov qword ptr [rsp + 200], rbp
 lea rcx, [rip + g_dcap_top]
 mov rbp, qword ptr [rcx + 0]
 mov qword ptr [rsp + 192], rbp
 mov rax, rsp
 sub rsp, 32
 mov qword ptr [rsp + 16], rax
 lea rcx, [rip + g_patstk_sp]
 mov rax, qword ptr [rcx + 0]
 mov qword ptr [rsp + 8], rax
 mov dword ptr [rsp + 0], 0
.Lx5_0:
 mov r14d, dword ptr [rsp + 0]
 jmp xchain0_n5_α
 xchain0_n3_β:
 add dword ptr [rsp + 0], 1
 mov eax, dword ptr [rsp + 0]
 cmp eax, r15d
 jg .Lx5_1
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne .Lx5_1
 jmp .Lx5_0
.Lx5_1:
 mov rax, qword ptr [rsp + 8]
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rax
 mov rsp, qword ptr [rsp + 16]
 lea rcx, [rip + g_dcap_top]
 mov rax, qword ptr [rsp + 192]
 mov qword ptr [rcx + 0], rax
 mov rbp, qword ptr [rsp + 200]
 jmp xchain0_n4_α
# IR_LIT_STRING
 xchain0_n4_α:
 mov qword ptr [rsp + 336], 1
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [rsp + 344], rax
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp main_γ
.Lx6_0:
 .quad .Lx6_0_s
.Lx6_0_s:
 .string "no digits"
# IR_MATCH_CAPTURE_SAVE fc cell
 xchain0_n5_α:
 sub rsp, 16
 mov dword ptr [rsp + 0], r14d
 jmp xchain0_n7_α
 xchain0_n5_β:
 add rsp, 16
 jmp xchain0_n3_β
# IR_ASSIGN global
 xchain0_n6_α:
 mov rsi, qword ptr [rsp + 336]
 mov rdx, qword ptr [rsp + 344]
 mov rdi, qword ptr [rip + .Lx9_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 320], rax
 mov qword ptr [rsp + 328], rdx
 jmp main_γ
 xchain0_n6_β:
 jmp main_γ
.Lx9_0:
 .quad .Lx9_0_s
.Lx9_0_s:
 .string "OUTPUT"
# IR_MATCH_SPAN
 xchain0_n7_α:
 sub rsp, 16
 mov dword ptr [rsp + 0], 0
.Lx11_0:
 mov eax, r14d
 add eax, dword ptr [rsp + 0]
 cmp eax, r15d
 jge .Lx11_1
 movsxd rcx, eax
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S0]
 call strchr@PLT
 test rax, rax
 je .Lx11_1
 add dword ptr [rsp + 0], 1
 jmp .Lx11_0
.Lx11_1:
 mov eax, dword ptr [rsp + 0]
 test eax, eax
 jg .Lx11_240
 add rsp, 16
 jmp xchain0_n5_β
.Lx11_240:
 mov edx, r14d
 mov dword ptr [rsp + 4], edx
 add edx, eax
 mov r14d, edx
 jmp xchain0_n8_α
 xchain0_n7_β:
 xchain0_n7_β:
 mov r14d, dword ptr [rsp + 4]
 add rsp, 16
 jmp xchain0_n5_β
# IR_MATCH_CAPTURE_COND (rbp-dcap inline pend)
 xchain0_n8_α:
 mov eax, dword ptr [rsp + 16]
 lea rcx, [rip + .S1]
 mov qword ptr [rbp + 0], rcx
 mov esi, eax
 mov qword ptr [rbp + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [rbp + 16], rdx
 add rbp, 24
 jmp xchain0_n9_α
 xchain0_n8_β:
 sub rbp, 24
 jmp xchain0_n7_β
# IR_MATCH_RELEASE
 xchain0_n9_α:
 mov rax, qword ptr [rsp + 40]
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rax
 mov rsp, qword ptr [rsp + 48]
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rcx, [rip + g_dcap_top]
 mov qword ptr [rcx + 0], rbp
 mov rdi, qword ptr [rsp + 224]
 mov rsi, rbp
 mov rdx, r13
 call rt_dcap_end_ok_open@PLT
.Lx15_1:
 test rax, rax
 je .Lx15_2
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx15_3]
 lea rdx, [rip + .Lx15_4]
 jmp rax
.Lx15_3:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx15_1
.Lx15_4:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx15_1
.Lx15_2:
 call rt_dcap_end_ok_close@PLT
 add rsp, 8
 pop r13
 pop r15
 pop r14
 lea rcx, [rip + g_dcap_top]
 mov rax, qword ptr [rsp + 192]
 mov qword ptr [rcx + 0], rax
 mov rbp, qword ptr [rsp + 200]
 jmp xchain0_n10_α
# IR_VAR
 xchain0_n10_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [rsp + 384], rax
 mov qword ptr [rsp + 392], rdx
 jmp xchain0_n11_α
 xchain0_n10_β:
 jmp main_γ
# IR_ASSIGN global
 xchain0_n11_α:
 mov rsi, qword ptr [rsp + 384]
 mov rdx, qword ptr [rsp + 392]
 mov rdi, qword ptr [rip + .Lx17_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 368], rax
 mov qword ptr [rsp + 376], rdx
 jmp main_γ
 xchain0_n11_β:
 jmp main_γ
.Lx17_0:
 .quad .Lx17_0_s
.Lx17_0_s:
 .string "OUTPUT"
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rsp + 424]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rsp + 424]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
add rsp, 65544
ret
.section .rodata
.S0: .string "0123456789"
.S1: .string "V"
.text
