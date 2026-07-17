  .intel_syntax noprefix
  .text
  .globl proc_MATCHIT_α
proc_MATCHIT_α:
#=======================================================================================================================
    .global proc_MATCHIT_α
    .global proc_MATCHIT_β
    .global proc_MATCHIT_γ
    .global proc_MATCHIT_ω
  sub rsp, 672
  mov [rsp + 648], rcx
  mov [rsp + 656], rdx
  mov rdi, rsp
  mov ecx, 640
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 632], rsp
proc_MATCHIT_α_body:
# IR_VAR
 xchain0_n0_α:
 mov rax, qword ptr [1879052304]
 mov rdx, qword ptr [1879052312]
 mov qword ptr [rsp + 400], rax
 mov qword ptr [rsp + 408], rdx
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n2_α
# IR_MATCH_HEAD
 xchain0_n1_α:
 mov qword ptr [rsp + 152], rbp
 mov rdi, qword ptr [rsp + 400]
 mov rsi, qword ptr [rsp + 408]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov qword ptr [rsp + 144], r12
 mov rax, rsp
 sub rsp, 32
 mov qword ptr [rsp + 16], rax
 lea rcx, [rip + g_patstk_sp]
 mov rax, qword ptr [rcx + 0]
 mov qword ptr [rsp + 8], rax
 mov dword ptr [rsp + 0], 0
.Lx3_0:
 mov r14d, dword ptr [rsp + 0]
 jmp xchain0_n3_α
 xchain0_n1_β:
 add dword ptr [rsp + 0], 1
 mov eax, dword ptr [rsp + 0]
 cmp eax, r15d
 jg .Lx3_1
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne .Lx3_1
 jmp .Lx3_0
.Lx3_1:
 mov rax, qword ptr [rsp + 8]
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rax
 mov rsp, qword ptr [rsp + 16]
 mov r12, qword ptr [rsp + 144]
 mov rbp, qword ptr [rsp + 152]
 jmp xchain0_n2_α
# IR_LIT_STRING
 xchain0_n2_α:
 mov qword ptr [rsp + 448], 1
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [rsp + 456], rax
 jmp xchain0_n4_α
 xchain0_n2_β:
 jmp proc_MATCHIT_γ
.Lx4_0:
 .quad .Lx4_0_s
.Lx4_0_s:
 .string "fail"
# IR_MATCH_SEQ_NARY (ZB-FC-3b: zero cell, LIFO-structural)
 xchain0_n3_α:
 jmp xchain0_n6_α
xchain0_n3_as:
 jmp xchain0_n5_α
 xchain0_n3_β:
 jmp xchain0_n11_β
xchain0_n3_af:
 jmp xchain0_n1_β
# IR_ASSIGN gva
 xchain0_n4_α:
 mov rax, qword ptr [rsp + 448]
 mov rdx, qword ptr [rsp + 456]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rsp + 432], rax
 mov qword ptr [rsp + 440], rdx
 jmp proc_MATCHIT_γ
 xchain0_n4_β:
 jmp proc_MATCHIT_γ
# IR_MATCH_RELEASE (R12-EXIT-1 tail: bracket read off the TOP ELEMENT, then the one-mov unwind collapses every element, every suspended cell, and HEAD's cell together)
 xchain0_n5_α:
 mov rax, qword ptr [rsp + 64]
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rax
 mov rsp, qword ptr [rsp + 72]
 push r14
 push r15
 push r13
 sub rsp, 8
 mov rdi, qword ptr [rsp + 176]
 mov rsi, r12
 mov rdx, r13
 call rt_dcap_end_ok_open@PLT
.Lx9_1:
 test rax, rax
 je .Lx9_2
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx9_3]
 lea rdx, [rip + .Lx9_4]
 jmp rax
.Lx9_3:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx9_1
.Lx9_4:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx9_1
.Lx9_2:
 call rt_dcap_end_ok_close@PLT
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov r12, qword ptr [rsp + 144]
 mov rbp, qword ptr [rsp + 152]
 jmp xchain0_n12_α
# IR_LIT_INTEGER
 xchain0_n6_α:
 mov qword ptr [rsp + 240], 6
 mov rax, qword ptr [rip + .Lx10_0]
 mov qword ptr [rsp + 248], rax
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
 jmp xchain0_n8_α
 xchain0_n7_β:
 jmp xchain0_n3_af
# IR_MATCH_CAPTURE_SAVE fc cell
 xchain0_n8_α:
 sub rsp, 16
 mov dword ptr [rsp + 0], r14d
 jmp xchain0_n13_α
 xchain0_n8_β:
 add rsp, 16
 jmp xchain0_n7_β
# IR_MATCH_CAPTURE_COND (pend-park inline pend)
 xchain0_n9_α:
 mov eax, dword ptr [rsp + 80]
 mov rdi, r12
 lea rcx, [rip + .S0]
 mov qword ptr [rdi + 0], rcx
 mov esi, eax
 mov qword ptr [rdi + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [rdi + 16], rdx
 add r12, 24
 jmp xchain0_n10_α
 xchain0_n9_β:
 sub r12, 24
 jmp xchain0_n13_β
# IR_LIT_INTEGER
 xchain0_n10_α:
 mov qword ptr [rsp + 32], 6
 mov rax, qword ptr [rip + .Lx16_0]
 mov qword ptr [rsp + 40], rax
 jmp xchain0_n11_α
 xchain0_n10_β:
 jmp xchain0_n9_β
.Lx16_0:
 .quad 0
# IR_MATCH_RPOS
 xchain0_n11_α:
 mov rax, 0
 mov ecx, r15d
 sub ecx, eax
 cmp r14d, ecx
 jne xchain0_n9_β
 jmp xchain0_n3_as
 xchain0_n11_β:
 jmp xchain0_n9_β
# IR_VAR
 xchain0_n12_α:
 mov rax, qword ptr [1879052320]
 mov rdx, qword ptr [1879052328]
 mov qword ptr [rsp + 496], rax
 mov qword ptr [rsp + 504], rdx
 jmp xchain0_n14_α
 xchain0_n12_β:
 jmp proc_MATCHIT_γ
# IR_MATCH_ARBNO_TAIL (R12-EXIT-1 carry-the-tail rsp elements)
 xchain0_n13_α:
 sub rsp, 96
 mov eax, 0
 mov qword ptr [rsp + 0], rax
 mov qword ptr [rsp + 8], rax
 mov qword ptr [rsp + 16], rax
 mov qword ptr [rsp + 24], rax
 mov qword ptr [rsp + 32], rax
 mov qword ptr [rsp + 40], rax
 mov qword ptr [rsp + 88], rax
 mov dword ptr [rsp + 48], r14d
 mov dword ptr [rsp + 52], r14d
 mov dword ptr [rsp + 56], 1
 mov rax, qword ptr [rsp + 120]
 mov qword ptr [rsp + 64], rax
 mov rax, qword ptr [rsp + 128]
 mov qword ptr [rsp + 72], rax
 mov rax, qword ptr [rsp + 96]
 mov qword ptr [rsp + 80], rax
 jmp xchain0_n9_α
 xchain0_n13_β:
 mov r14d, dword ptr [rsp + 52]
 mov rax, qword ptr [rsp + 64]
 mov rcx, qword ptr [rsp + 72]
 sub rsp, 96
 mov edx, 0
 mov qword ptr [rsp + 0], rdx
 mov qword ptr [rsp + 8], rdx
 mov qword ptr [rsp + 16], rdx
 mov qword ptr [rsp + 24], rdx
 mov qword ptr [rsp + 32], rdx
 mov qword ptr [rsp + 40], rdx
 mov dword ptr [rsp + 48], r14d
 mov dword ptr [rsp + 52], r14d
 mov dword ptr [rsp + 56], 0
 mov qword ptr [rsp + 64], rax
 mov qword ptr [rsp + 72], rcx
 mov qword ptr [rsp + 88], rdx
 mov rax, qword ptr [rsp + 176]
 mov qword ptr [rsp + 80], rax
 jmp xchain0_n15_α
xchain0_n13_as:
 mov eax, dword ptr [rsp + 48]
 cmp r14d, eax
 je xchain0_n15_β
 mov dword ptr [rsp + 52], r14d
 jmp xchain0_n9_α
xchain0_n13_af:
 mov eax, dword ptr [rsp + 56]
 test eax, eax
 jnz .Lx20_2
 add rsp, 96
 jmp xchain0_n15_β
.Lx20_2:
 mov r14d, dword ptr [rsp + 48]
 add rsp, 96
 jmp xchain0_n8_β
# IR_ASSIGN gva
 xchain0_n14_α:
 mov rax, qword ptr [rsp + 496]
 mov rdx, qword ptr [rsp + 504]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rsp + 480], rax
 mov qword ptr [rsp + 488], rdx
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
proc_MATCHIT_res:
add rsp, 8
pop rsp
proc_MATCHIT_β:
jmp proc_MATCHIT_ω
proc_MATCHIT_γ:
mov rdi, [rsp]
mov rsi, [rsp + 8]
mov rax, [rsp + 648]
lea rsp, [rsp + 672]
jmp rax
proc_MATCHIT_ω:
mov rax, [rsp + 656]
lea rsp, [rsp + 672]
jmp rax
proc_startup:
  sub rsp, 8
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
  mov esi, 640
  call rt_proc_set_frame_bytes@PLT
  add rsp, 8
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
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
  call proc_startup
  mov edi, 3
  call rt_gva_island@PLT
  mov rsi, rax
  lea rdi, [rip + __gva_names]
  mov edx, 3
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
# IR_LIT_STRING
 xchain24_n0_α:
 mov qword ptr [rsp + 592], 1
 mov rax, qword ptr [rip + .Lx25_0]
 mov qword ptr [rsp + 600], rax
 jmp xchain24_n1_α
 xchain24_n0_β:
 jmp main_γ
.Lx25_0:
 .quad .Lx25_0_s
.Lx25_0_s:
 .string "aaa"
 xchain24_n1_α:
 mov edi, 0
 mov rsi, qword ptr [rsp + 592]
 mov rdx, qword ptr [rsp + 600]
 call rt_arg_stage@PLT
 mov rdi, qword ptr [rip + .Lx27_0]
 mov esi, 1
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx27_1
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx27_3]
 lea rdx, [rip + .Lx27_4]
 jmp rax
.Lx27_3:
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx27_2
.Lx27_4:
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx27_2
.Lx27_1:
 call rt_faildescr@PLT
.Lx27_2:
 mov qword ptr [rsp + 544], rax
 mov qword ptr [rsp + 552], rdx
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
 mov rsi, qword ptr [rsp + 544]
 mov rdx, qword ptr [rsp + 552]
 mov rdi, qword ptr [rip + .Lx28_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 528], rax
 mov qword ptr [rsp + 536], rdx
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
.section .rodata
.S0: .string "V"
.S1: .string "a"
.text
