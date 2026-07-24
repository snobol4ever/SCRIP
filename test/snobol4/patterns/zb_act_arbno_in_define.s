  .intel_syntax noprefix
  .text
  .globl proc_MATCHIT_α
proc_MATCHIT_α:
#=======================================================================================================================
    .global proc_MATCHIT_α
    .global proc_MATCHIT_β
    .global proc_MATCHIT_γ
    .global proc_MATCHIT_ω
  sub rsp, 544
  mov [rsp + 520], rcx
  mov [rsp + 528], rdx
  mov [rsp + 536], rbp
  mov rbp, rsp
  mov rdi, rsp
  mov ecx, 512
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 504], rsp
proc_MATCHIT_α_body:
# IR_VAR
 xchain0_n0_α:
 mov rax, qword ptr [1879052304]
 mov rdx, qword ptr [1879052312]
 mov qword ptr [rbp + 368], rax
 mov qword ptr [rbp + 376], rdx
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n2_α
# IR_MATCH_HEAD
 xchain0_n1_α:
 mov qword ptr [rbp + 152], rbp
 mov rdi, qword ptr [rbp + 368]
 mov rsi, qword ptr [rbp + 376]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov qword ptr [rbp + 144], r12
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
 mov r12, qword ptr [rbp + 144]
 mov rbp, qword ptr [rbp + 152]
 jmp xchain0_n2_α
# IR_LIT_STRING
 xchain0_n2_α:
 mov qword ptr [rbp + 416], 1
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [rbp + 424], rax
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
 mov rax, qword ptr [rbp + 416]
 mov rdx, qword ptr [rbp + 424]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 400], rax
 mov qword ptr [rbp + 408], rdx
 jmp proc_MATCHIT_γ
 xchain0_n4_β:
 jmp proc_MATCHIT_γ
# IR_MATCH_RELEASE (R12-EXIT-1 tail: bracket read off the TOP ELEMENT, then the one-mov unwind collapses every element, every suspended cell, and HEAD's cell together)
 xchain0_n5_α:
 mov rax, qword ptr [rsp + 208]
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rax
 mov rsp, qword ptr [rsp + 216]
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
 mov r12, qword ptr [rbp + 144]
 mov rbp, qword ptr [rbp + 152]
 jmp xchain0_n12_α
# IR_LIT_INTEGER
 xchain0_n6_α:
 mov qword ptr [rbp + 192], 6
 mov rax, qword ptr [rip + .Lx10_0]
 mov qword ptr [rbp + 200], rax
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
 jmp xchain0_n3_af
# IR_MATCH_CAPTURE_COND (pend-park inline pend)
 xchain0_n9_α:
 mov eax, dword ptr [rsp + 224]
 lea rcx, [rip + .S0]
 mov qword ptr [r12 + 0], rcx
 mov esi, eax
 mov qword ptr [r12 + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [r12 + 16], rdx
 add r12, 24
 jmp xchain0_n10_α
 xchain0_n9_β:
 sub r12, 24
 jmp xchain0_n13_β
# IR_LIT_INTEGER
 xchain0_n10_α:
 mov qword ptr [rbp + 336], 6
 mov rax, qword ptr [rip + .Lx16_0]
 mov qword ptr [rbp + 344], rax
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
 mov qword ptr [rbp + 464], rax
 mov qword ptr [rbp + 472], rdx
 jmp xchain0_n14_α
 xchain0_n12_β:
 jmp proc_MATCHIT_γ
# IR_MATCH_ARBNO_TAIL (R12-EXIT-1 carry-the-tail rsp elements)
 xchain0_n13_α:
 sub rsp, 240
 mov eax, 0
 mov qword ptr [rsp + 0], rax
 mov qword ptr [rsp + 8], rax
 mov qword ptr [rsp + 16], rax
 mov qword ptr [rsp + 24], rax
 mov qword ptr [rsp + 32], rax
 mov qword ptr [rsp + 40], rax
 mov qword ptr [rsp + 48], rax
 mov qword ptr [rsp + 56], rax
 mov qword ptr [rsp + 64], rax
 mov qword ptr [rsp + 72], rax
 mov qword ptr [rsp + 80], rax
 mov qword ptr [rsp + 88], rax
 mov qword ptr [rsp + 96], rax
 mov qword ptr [rsp + 104], rax
 mov qword ptr [rsp + 112], rax
 mov qword ptr [rsp + 120], rax
 mov qword ptr [rsp + 128], rax
 mov qword ptr [rsp + 136], rax
 mov qword ptr [rsp + 144], rax
 mov qword ptr [rsp + 152], rax
 mov qword ptr [rsp + 160], rax
 mov qword ptr [rsp + 168], rax
 mov qword ptr [rsp + 176], rax
 mov qword ptr [rsp + 184], rax
 mov qword ptr [rsp + 232], rax
 mov dword ptr [rsp + 192], r14d
 mov dword ptr [rsp + 196], r14d
 mov dword ptr [rsp + 200], 1
 mov rax, qword ptr [rsp + 264]
 mov qword ptr [rsp + 208], rax
 mov rax, qword ptr [rsp + 272]
 mov qword ptr [rsp + 216], rax
 mov rax, qword ptr [rsp + 240]
 mov qword ptr [rsp + 224], rax
 jmp xchain0_n9_α
 xchain0_n13_β:
 mov r14d, dword ptr [rsp + 196]
 mov rax, qword ptr [rsp + 208]
 mov rcx, qword ptr [rsp + 216]
 sub rsp, 240
 mov edx, 0
 mov qword ptr [rsp + 0], rdx
 mov qword ptr [rsp + 8], rdx
 mov qword ptr [rsp + 16], rdx
 mov qword ptr [rsp + 24], rdx
 mov qword ptr [rsp + 32], rdx
 mov qword ptr [rsp + 40], rdx
 mov qword ptr [rsp + 48], rdx
 mov qword ptr [rsp + 56], rdx
 mov qword ptr [rsp + 64], rdx
 mov qword ptr [rsp + 72], rdx
 mov qword ptr [rsp + 80], rdx
 mov qword ptr [rsp + 88], rdx
 mov qword ptr [rsp + 96], rdx
 mov qword ptr [rsp + 104], rdx
 mov qword ptr [rsp + 112], rdx
 mov qword ptr [rsp + 120], rdx
 mov qword ptr [rsp + 128], rdx
 mov qword ptr [rsp + 136], rdx
 mov qword ptr [rsp + 144], rdx
 mov qword ptr [rsp + 152], rdx
 mov qword ptr [rsp + 160], rdx
 mov qword ptr [rsp + 168], rdx
 mov qword ptr [rsp + 176], rdx
 mov qword ptr [rsp + 184], rdx
 mov dword ptr [rsp + 192], r14d
 mov dword ptr [rsp + 196], r14d
 mov dword ptr [rsp + 200], 0
 mov qword ptr [rsp + 208], rax
 mov qword ptr [rsp + 216], rcx
 mov qword ptr [rsp + 232], rdx
 mov rax, qword ptr [rsp + 464]
 mov qword ptr [rsp + 224], rax
 jmp xchain0_n15_α
xchain0_n13_as:
 mov eax, dword ptr [rsp + 192]
 cmp r14d, eax
 je xchain0_n15_β
 mov dword ptr [rsp + 196], r14d
 jmp xchain0_n9_α
xchain0_n13_af:
 mov eax, dword ptr [rsp + 200]
 test eax, eax
 jnz .Lx20_2
 add rsp, 240
 jmp xchain0_n15_β
.Lx20_2:
 mov r14d, dword ptr [rsp + 192]
 add rsp, 240
 jmp xchain0_n8_β
# IR_ASSIGN gva
 xchain0_n14_α:
 mov rax, qword ptr [rbp + 464]
 mov rdx, qword ptr [rbp + 472]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 448], rax
 mov qword ptr [rbp + 456], rdx
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
 movzx eax, byte ptr [r13+rcx]
 cmp eax, 97
 jne xchain0_n13_af
 add r14d, 1
 jmp xchain0_n13_as
 xchain0_n15_β:
 sub r14d, 1
 jmp xchain0_n13_af
proc_MATCHIT_res:
add rsp, 8
pop rbp
proc_MATCHIT_β:
jmp proc_MATCHIT_ω
proc_MATCHIT_γ:
mov rdi, [rbp]
mov rsi, [rbp + 8]
mov rax, [rbp + 520]
lea rsp, [rbp + 544]
mov rbp, [rbp + 536]
jmp rax
proc_MATCHIT_ω:
mov rax, [rbp + 528]
lea rsp, [rbp + 544]
mov rbp, [rbp + 536]
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
  mov esi, 512
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
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
  mov qword ptr [rsp + 600], rsp
  mov r12, qword ptr [1879048192]
  mov [rsp + 65536], rbp
  mov rbp, rsp
main_α_body:
# IR_LIT_STRING
 xchain24_n0_α:
 mov qword ptr [rbp + 560], 1
 mov rax, qword ptr [rip + .Lx25_0]
 mov qword ptr [rbp + 568], rax
 jmp xchain24_n1_α
.Lx25_0:
 .quad .Lx25_0_s
.Lx25_0_s:
 .string "aaa"
 xchain24_n1_α:
 sub rsp, 32
 mov rax, qword ptr [1879052304]
 mov qword ptr [rsp + 0], rax
 mov rax, qword ptr [1879052312]
 mov qword ptr [rsp + 8], rax
 mov rax, qword ptr [1879052288]
 mov qword ptr [rsp + 16], rax
 mov rax, qword ptr [1879052296]
 mov qword ptr [rsp + 24], rax
 mov rdi, qword ptr [rip + .Lx27_0]
 mov esi, 1
 mov edx, 1
 call rt_proc_call_open_slim@PLT
 test rax, rax
 je .Lx27_5
 mov rax, qword ptr [rbp + 560]
 mov qword ptr [1879052304], rax
 mov rax, qword ptr [rbp + 568]
 mov qword ptr [1879052312], rax
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx27_6]
 lea rdx, [rip + .Lx27_7]
 jmp rax
.Lx27_6:
 mov rdi, qword ptr [1879052288]
 mov rsi, qword ptr [1879052296]
 mov rax, qword ptr [rsp + 16]
 mov qword ptr [1879052288], rax
 mov rax, qword ptr [rsp + 24]
 mov qword ptr [1879052296], rax
 mov rax, qword ptr [rsp + 0]
 mov qword ptr [1879052304], rax
 mov rax, qword ptr [rsp + 8]
 mov qword ptr [1879052312], rax
 add rsp, 32
 call rt_proc_call_epilogue_slim_γ@PLT
 jmp .Lx27_2
.Lx27_7:
 mov rax, qword ptr [rsp + 16]
 mov qword ptr [1879052288], rax
 mov rax, qword ptr [rsp + 24]
 mov qword ptr [1879052296], rax
 mov rax, qword ptr [rsp + 0]
 mov qword ptr [1879052304], rax
 mov rax, qword ptr [rsp + 8]
 mov qword ptr [1879052312], rax
 add rsp, 32
 call rt_proc_call_epilogue_slim_ω@PLT
 jmp .Lx27_2
.Lx27_5:
 add rsp, 32
 mov edi, 0
 mov rsi, qword ptr [rbp + 560]
 mov rdx, qword ptr [rbp + 568]
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
 mov qword ptr [rbp + 512], rax
 mov qword ptr [rbp + 520], rdx
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
 mov rsi, qword ptr [rbp + 512]
 mov rdx, qword ptr [rbp + 520]
 mov rdi, qword ptr [rip + .Lx28_0]
 call NV_SET_fn@PLT
 mov qword ptr [rbp + 496], rax
 mov qword ptr [rbp + 504], rdx
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
mov rsp, qword ptr [rbp + 600]
mov rbp, [rsp + 65536]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rbp + 600]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
mov rbp, [rsp + 65536]
add rsp, 65544
ret
.section .rodata
.S0: .string "V"
.text
