  .intel_syntax noprefix
  .text
  .globl proc_MATCHIT_α
proc_MATCHIT_α:
#=======================================================================================================================
    .global proc_MATCHIT_α
    .global proc_MATCHIT_β
    .global proc_MATCHIT_γ
    .global proc_MATCHIT_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, [rsp + 8]
 mov qword ptr [r12 + 608], rax
 pop rsi
proc_MATCHIT_α_body:
# IR_VAR
 xchain0_n0_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 384], rax
 mov qword ptr [r12 + 392], rdx
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n2_α
# IR_MATCH_HEAD
 xchain0_n1_α:
 call rt_zls_mark@PLT
 mov qword ptr [r12 + 120], rax
 mov qword ptr [r12 + 128], rsp
 mov rdi, qword ptr [r12 + 384]
 mov rsi, qword ptr [r12 + 392]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov dword ptr [r12 + 112], 0
.Lx3_0:
 mov r14d, dword ptr [r12 + 112]
 jmp xchain0_n3_α
 xchain0_n1_β:
 add dword ptr [r12 + 112], 1
 mov eax, dword ptr [r12 + 112]
 cmp eax, r15d
 jg .Lx3_1
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne .Lx3_1
 jmp .Lx3_0
.Lx3_1:
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [r12 + 120]
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
 mov rsp, qword ptr [r12 + 128]
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_dcap_end_fail@PLT
 mov rsp, [rsp + 8]
 jmp xchain0_n2_α
# IR_LIT_STRING
 xchain0_n2_α:
 mov qword ptr [r12 + 432], 1
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [r12 + 440], rax
 jmp xchain0_n4_α
 xchain0_n2_β:
 jmp proc_MATCHIT_γ
.Lx4_0:
 .quad .Lx4_0_s
.Lx4_0_s:
 .string "fail"
# IR_MATCH_SEQ_NARY
 xchain0_n3_α:
 mov dword ptr [r12 + 160], r14d
 mov dword ptr [r12 + 164], 0
 jmp xchain0_n6_α
xchain0_n3_as:
 mov eax, dword ptr [r12 + 164]
 add eax, 1
 mov dword ptr [r12 + 164], eax
 cmp eax, 1
 je xchain0_n8_α
 cmp eax, 2
 je xchain0_n10_α
 jmp xchain0_n5_α
 xchain0_n3_β:
 mov dword ptr [r12 + 164], 3
xchain0_n3_af:
 mov eax, dword ptr [r12 + 164]
 sub eax, 1
 mov dword ptr [r12 + 164], eax
 cmp eax, 0
 je xchain0_n7_β
 cmp eax, 1
 je xchain0_n9_β
 cmp eax, 2
 je xchain0_n11_β
 jmp xchain0_n1_β
# IR_ASSIGN gva
 xchain0_n4_α:
 mov rax, qword ptr [r12 + 432]
 mov rdx, qword ptr [r12 + 440]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 416], rax
 mov qword ptr [r12 + 424], rdx
 jmp proc_MATCHIT_γ
 xchain0_n4_β:
 jmp proc_MATCHIT_γ
# IR_MATCH_RELEASE
 xchain0_n5_α:
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [r12 + 120]
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
 mov rsp, qword ptr [r12 + 128]
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rsp, [rsp + 8]
 push r14
 push r15
 push r13
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_end_ok_open@PLT
.Lx9_1:
 test rax, rax
 je .Lx9_2
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx9_1
.Lx9_2:
 call rt_dcap_end_ok_close@PLT
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 jmp xchain0_n12_α
# IR_LIT_INTEGER
 xchain0_n6_α:
 mov qword ptr [r12 + 192], 6
 mov rax, qword ptr [rip + .Lx10_0]
 mov qword ptr [r12 + 200], rax
 jmp xchain0_n7_α
 xchain0_n6_β:
 jmp xchain0_n3_af
.Lx10_0:
 .quad 0
# IR_MATCH_POS
 xchain0_n7_α:
 mov rax, 0
 cmp r14d, eax
 jne xchain0_n3_af
 jmp xchain0_n3_as
 xchain0_n7_β:
 jmp xchain0_n3_af
# IR_MATCH_CAPTURE_SAVE push
 xchain0_n8_α:
 lea rdi, [r12 + 256]
 mov esi, r14d
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_cap_push@PLT
 mov rsp, [rsp + 8]
 jmp xchain0_n13_α
 xchain0_n8_β:
 lea rdi, [r12 + 256]
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_cap_pop@PLT
 mov rsp, [rsp + 8]
 jmp xchain0_n3_af
# IR_MATCH_CAPTURE_COND
 xchain0_n9_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 lea rdi, [r12 + 256]
 call rt_cap_top@PLT
 lea rdi, [rip + .S0]
 mov esi, eax
 mov edx, r14d
 mov ecx, 0
 call rt_cap_open@PLT
 test rax, rax
 je .Lx15_1
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_cap_finish@PLT
.Lx15_1:
 mov rsp, rbp
 pop rbp
 jmp xchain0_n3_as
 xchain0_n9_β:
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_dcap_pop@PLT
 mov rsp, [rsp + 8]
 jmp xchain0_n13_β
# IR_LIT_INTEGER
 xchain0_n10_α:
 mov qword ptr [r12 + 352], 6
 mov rax, qword ptr [rip + .Lx16_0]
 mov qword ptr [r12 + 360], rax
 jmp xchain0_n11_α
 xchain0_n10_β:
 jmp xchain0_n3_af
.Lx16_0:
 .quad 0
# IR_MATCH_RPOS
 xchain0_n11_α:
 mov rax, 0
 mov ecx, r15d
 sub ecx, eax
 cmp r14d, ecx
 jne xchain0_n3_af
 jmp xchain0_n3_as
 xchain0_n11_β:
 jmp xchain0_n3_af
# IR_VAR
 xchain0_n12_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 480], rax
 mov qword ptr [r12 + 488], rdx
 jmp xchain0_n14_α
 xchain0_n12_β:
 jmp proc_MATCHIT_γ
# IR_MATCH_ARBNO_NARY
 xchain0_n13_α:
 mov dword ptr [r12 + 288], r14d
 mov dword ptr [r12 + 292], r14d
 mov dword ptr [r12 + 296], 0
 jmp xchain0_n9_α
 xchain0_n13_β:
 mov r14d, dword ptr [r12 + 292]
 lea rdi, [r12 + 304]
 lea rsi, [r12 + 300]
 mov edx, dword ptr [r12 + 296]
 mov rcx, 32
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_zcol_push@PLT
 mov rsp, [rsp + 8]
 mov qword ptr [rax + 0], r12
 mov ecx, r14d
 mov qword ptr [rax + 8], rcx
 lea r12, [rax + -304]
 jmp xchain0_n15_α
xchain0_n13_as:
 mov eax, dword ptr [r12 + 312]
 cmp r14d, eax
 je xchain0_n15_β
 mov r12, qword ptr [r12 + 304]
 mov eax, dword ptr [r12 + 296]
 add eax, 1
 mov dword ptr [r12 + 296], eax
 mov dword ptr [r12 + 292], r14d
 jmp xchain0_n9_α
xchain0_n13_af:
 mov r12, qword ptr [r12 + 304]
 mov eax, dword ptr [r12 + 296]
 test eax, eax
 jz .Lx20_2
 sub eax, 1
 mov dword ptr [r12 + 296], eax
 mov rax, qword ptr [r12 + 304]
 mov ecx, dword ptr [r12 + 296]
 mov rdx, 32
 imul rcx, rdx
 add rax, rcx
 lea r12, [rax + -304]
 jmp xchain0_n15_β
.Lx20_2:
 mov r14d, dword ptr [r12 + 288]
 jmp xchain0_n8_β
# IR_ASSIGN gva
 xchain0_n14_α:
 mov rax, qword ptr [r12 + 480]
 mov rdx, qword ptr [r12 + 488]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 464], rax
 mov qword ptr [r12 + 472], rdx
 jmp proc_MATCHIT_γ
 xchain0_n14_β:
 jmp proc_MATCHIT_γ
# IR_MATCH_LIT
 xchain0_n15_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain0_n13_af
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S1]
 mov edx, 1
 call memcmp@PLT
 test eax, eax
 jne xchain0_n13_af
 add r14d, 1
 jmp xchain0_n13_as
 xchain0_n15_β:
 sub r14d, 1
 jmp xchain0_n13_af
proc_MATCHIT_β:
jmp proc_MATCHIT_ω
proc_MATCHIT_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [r12 + 608]
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
proc_MATCHIT_ω:
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
proc_startup:
  push rbp
  mov rbp, rsp
  .section .rodata
  .Lstartup_pname0: .string "MATCHIT"
  .Lstartup_pp0_0: .string "S"
  .align 8
  .Lstartup_pnames0:
  .quad .Lstartup_pp0_0
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname0]
  lea rsi, [rip + .Lstartup_pnames0]
  mov edx, 1
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname0]
  lea rsi, [rip + proc_MATCHIT_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 1
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 624
  call rt_proc_set_frame_bytes@PLT
  pop rbp
  ret
  .section .rodata
  .Lgvan0: .string "MATCHIT"
  .Lgvan1: .string "S"
  .Lgvan2: .string "V"
  .align 8
__gva_names:
  .quad .Lgvan0
  .quad .Lgvan1
  .quad .Lgvan2
  .section .bss
  .align 16
__gva: .space 48, 0
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  push rbp
  mov rbp, rsp
  push rdi
  push rsi
  call core_lib_init@PLT
  call proc_startup
  lea rdi, [rip + __gva_names]
  lea rsi, [rip + __gva]
  mov edx, 3
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
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, [rsp + 8]
 mov qword ptr [r12 + 608], rax
 pop rsi
main_α_body:
# IR_LIT_STRING
 xchain24_n0_α:
 mov qword ptr [r12 + 576], 1
 mov rax, qword ptr [rip + .Lx25_0]
 mov qword ptr [r12 + 584], rax
 jmp xchain24_n1_α
 xchain24_n0_β:
 jmp main_γ
.Lx25_0:
 .quad .Lx25_0_s
.Lx25_0_s:
 .string "aaa"
 xchain24_n1_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov edi, 0
 mov rsi, qword ptr [r12 + 576]
 mov rdx, qword ptr [r12 + 584]
 call rt_arg_stage@PLT
 mov rdi, qword ptr [rip + .Lx27_0]
 mov esi, 1
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx27_1
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_proc_call_epilogue@PLT
 jmp .Lx27_2
.Lx27_1:
 call rt_faildescr@PLT
.Lx27_2:
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 528], rax
 mov qword ptr [r12 + 536], rdx
 cmp eax, 99
 je main_γ
 jmp xchain24_n2_α
 xchain24_n1_β:
 jmp main_γ
.Lx27_0:
 .quad .Lx27_0_s
.Lx27_0_s:
 .string "MATCHIT"
# IR_ASSIGN global
 xchain24_n2_α:
 mov rsi, qword ptr [r12 + 528]
 mov rdx, qword ptr [r12 + 536]
 mov rdi, qword ptr [rip + .Lx28_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 512], rax
 mov qword ptr [r12 + 520], rdx
 jmp main_γ
 xchain24_n2_β:
 jmp main_γ
.Lx28_0:
 .quad .Lx28_0_s
.Lx28_0_s:
 .string "OUTPUT"
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [r12 + 608]
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
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
.section .rodata
.S0: .string "V"
.S1: .string "a"
.text
