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
  mov qword ptr [rsp + 472], rsp
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
 .string "banana"
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
 mov qword ptr [rsp + 336], rax
 mov qword ptr [rsp + 344], rdx
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n4_α
# IR_MATCH_HEAD
 xchain0_n3_α:
 mov rdi, qword ptr [rsp + 336]
 mov rsi, qword ptr [rsp + 344]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov qword ptr [rsp + 200], rbp
 lea rcx, [rip + g_dcap_top]
 mov rbp, qword ptr [rcx + 0]
 mov qword ptr [rsp + 192], rbp
 mov qword ptr [rsp + 176], rsp
 lea rcx, [rip + g_patstk_sp]
 mov rax, qword ptr [rcx + 0]
 mov qword ptr [rsp + 168], rax
 mov dword ptr [rsp + 160], 0
.Lx5_0:
 mov r14d, dword ptr [rsp + 160]
 jmp xchain0_n5_α
 xchain0_n3_β:
 add dword ptr [rsp + 160], 1
 mov eax, dword ptr [rsp + 160]
 cmp eax, r15d
 jg .Lx5_1
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne .Lx5_1
 jmp .Lx5_0
.Lx5_1:
 mov rax, qword ptr [rsp + 168]
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rax
 mov rsp, qword ptr [rsp + 176]
 lea rcx, [rip + g_dcap_top]
 mov rax, qword ptr [rsp + 192]
 mov qword ptr [rcx + 0], rax
 mov rbp, qword ptr [rsp + 200]
 jmp xchain0_n4_α
# IR_LIT_STRING
 xchain0_n4_α:
 mov qword ptr [rsp + 384], 1
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [rsp + 392], rax
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp main_γ
.Lx6_0:
 .quad .Lx6_0_s
.Lx6_0_s:
 .string "no match"
# IR_MATCH_CAPTURE_SAVE push
 xchain0_n5_α:
 lea rdi, [rsp + 240]
 mov esi, r14d
 call rt_cap_push@PLT
 jmp xchain0_n7_α
 xchain0_n5_β:
 lea rdi, [rsp + 240]
 call rt_cap_pop@PLT
 jmp xchain0_n3_β
# IR_ASSIGN global
 xchain0_n6_α:
 mov rsi, qword ptr [rsp + 384]
 mov rdx, qword ptr [rsp + 392]
 mov rdi, qword ptr [rip + .Lx9_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 368], rax
 mov qword ptr [rsp + 376], rdx
 jmp main_γ
 xchain0_n6_β:
 jmp main_γ
.Lx9_0:
 .quad .Lx9_0_s
.Lx9_0_s:
 .string "OUTPUT"
# IR_MATCH_ALT_NARY
 xchain0_n7_α:
 sub rsp, 16
 mov dword ptr [rsp + 0], r14d
 mov dword ptr [rsp + 4], 0
 jmp xchain0_n9_α
xchain0_n7_s0:
 jmp xchain0_n7_as
xchain0_n7_s1:
 jmp xchain0_n7_as
xchain0_n7_s2:
 jmp xchain0_n7_as
xchain0_n7_as:
 jmp xchain0_n8_α
 xchain0_n7_β:
 mov eax, dword ptr [rsp + 4]
 cmp eax, 0
 je xchain0_n9_β
 cmp eax, 1
 je xchain0_n10_β
 jmp xchain0_n11_β
xchain0_n7_af:
 add dword ptr [rsp + 4], 1
 mov r14d, dword ptr [rsp + 0]
 mov eax, dword ptr [rsp + 4]
 cmp eax, 1
 je xchain0_n10_α
 cmp eax, 2
 je xchain0_n11_α
 add rsp, 16
 jmp xchain0_n5_β
# IR_MATCH_CAPTURE_COND (rbp-dcap inline pend)
 xchain0_n8_α:
 lea rdi, [rsp + 240]
 call rt_cap_top@PLT
 lea rcx, [rip + .S0]
 mov qword ptr [rbp + 0], rcx
 mov esi, eax
 mov qword ptr [rbp + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [rbp + 16], rdx
 add rbp, 24
 jmp xchain0_n12_α
 xchain0_n8_β:
 sub rbp, 24
 jmp xchain0_n7_β
# IR_MATCH_LIT
 xchain0_n9_α:
 mov eax, r14d
 add eax, 5
 cmp eax, r15d
 jg xchain0_n7_af
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S1]
 mov edx, 5
 call memcmp@PLT
 test eax, eax
 jne xchain0_n7_af
 add r14d, 5
 jmp xchain0_n7_s0
 xchain0_n9_β:
 sub r14d, 5
 jmp xchain0_n7_af
# IR_MATCH_LIT
 xchain0_n10_α:
 mov eax, r14d
 add eax, 6
 cmp eax, r15d
 jg xchain0_n7_af
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S2]
 mov edx, 6
 call memcmp@PLT
 test eax, eax
 jne xchain0_n7_af
 add r14d, 6
 jmp xchain0_n7_s1
 xchain0_n10_β:
 sub r14d, 6
 jmp xchain0_n7_af
# IR_MATCH_LIT
 xchain0_n11_α:
 mov eax, r14d
 add eax, 6
 cmp eax, r15d
 jg xchain0_n7_af
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S3]
 mov edx, 6
 call memcmp@PLT
 test eax, eax
 jne xchain0_n7_af
 add r14d, 6
 jmp xchain0_n7_s2
 xchain0_n11_β:
 sub r14d, 6
 jmp xchain0_n7_af
# IR_MATCH_RELEASE
 xchain0_n12_α:
 mov rax, qword ptr [rsp + 168]
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rax
 mov rsp, qword ptr [rsp + 176]
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
.Lx21_1:
 test rax, rax
 je .Lx21_2
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx21_3]
 lea rdx, [rip + .Lx21_4]
 jmp rax
.Lx21_3:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx21_1
.Lx21_4:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx21_1
.Lx21_2:
 call rt_dcap_end_ok_close@PLT
 add rsp, 8
 pop r13
 pop r15
 pop r14
 lea rcx, [rip + g_dcap_top]
 mov rax, qword ptr [rsp + 192]
 mov qword ptr [rcx + 0], rax
 mov rbp, qword ptr [rsp + 200]
 jmp xchain0_n13_α
# IR_VAR
 xchain0_n13_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [rsp + 432], rax
 mov qword ptr [rsp + 440], rdx
 jmp xchain0_n14_α
 xchain0_n13_β:
 jmp main_γ
# IR_ASSIGN global
 xchain0_n14_α:
 mov rsi, qword ptr [rsp + 432]
 mov rdx, qword ptr [rsp + 440]
 mov rdi, qword ptr [rip + .Lx23_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 416], rax
 mov qword ptr [rsp + 424], rdx
 jmp main_γ
 xchain0_n14_β:
 jmp main_γ
.Lx23_0:
 .quad .Lx23_0_s
.Lx23_0_s:
 .string "OUTPUT"
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rsp + 472]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rsp + 472]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
add rsp, 65544
ret
.section .rodata
.S0: .string "V"
.S1: .string "apple"
.S2: .string "banana"
.S3: .string "cherry"
.text
