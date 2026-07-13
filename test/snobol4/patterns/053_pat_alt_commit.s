  .intel_syntax noprefix
  .text
  .globl proc_PAT$0_α
proc_PAT$0_α:
#=======================================================================================================================
    .global proc_PAT$0_α
    .global proc_PAT$0_β
    .global proc_PAT$0_γ
    .global proc_PAT$0_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  mov qword ptr [r12 + 120], rsp
  cmp esi, 0
  jne proc_PAT$0_β
 push rsi
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, [rsp + 8]
 mov qword ptr [r12 + 112], rax
 pop rsi
proc_PAT$0_α_body:
lea rax, [rip + xchain0_n0_β]
mov qword ptr [r12 + 96], rax
# IR_MATCH_ALT_NARY
 xchain0_n0_α:
 mov dword ptr [r12 + 32], r14d
 mov dword ptr [r12 + 36], 0
 jmp xchain0_n1_α
xchain0_n0_as:
 jmp proc_PAT$0_γ
 xchain0_n0_β:
 mov eax, dword ptr [r12 + 36]
 cmp eax, 0
 je xchain0_n1_β
 cmp eax, 1
 je xchain0_n2_β
 jmp xchain0_n3_β
xchain0_n0_af:
 add dword ptr [r12 + 36], 1
 mov r14d, dword ptr [r12 + 32]
 mov eax, dword ptr [r12 + 36]
 cmp eax, 1
 je xchain0_n2_α
 cmp eax, 2
 je xchain0_n3_α
 jmp proc_PAT$0_ω
# IR_MATCH_LIT
 xchain0_n1_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain0_n0_af
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S0]
 mov edx, 1
 call memcmp@PLT
 test eax, eax
 jne xchain0_n0_af
 add r14d, 1
 jmp xchain0_n0_as
 xchain0_n1_β:
 sub r14d, 1
 jmp xchain0_n0_af
# IR_MATCH_LIT
 xchain0_n2_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain0_n0_af
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S1]
 mov edx, 1
 call memcmp@PLT
 test eax, eax
 jne xchain0_n0_af
 add r14d, 1
 jmp xchain0_n0_as
 xchain0_n2_β:
 sub r14d, 1
 jmp xchain0_n0_af
# IR_MATCH_LIT
 xchain0_n3_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain0_n0_af
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S2]
 mov edx, 1
 call memcmp@PLT
 test eax, eax
 jne xchain0_n0_af
 add r14d, 1
 jmp xchain0_n0_as
 xchain0_n3_β:
 sub r14d, 1
 jmp xchain0_n0_af
proc_PAT$0_β:
jmp qword ptr [r12 + 96]
proc_PAT$0_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [r12 + 120]
pop r12
ret
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [r12 + 112]
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
proc_PAT$0_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, qword ptr [r12 + 120]
pop r12
ret
proc_startup:
  sub rsp, 8
  .section .rodata
  .Lstartup_pname0: .string "PAT$0"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname0]
  lea rsi, [rip + proc_PAT$0_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 128
  call rt_proc_set_frame_bytes@PLT
  add rsp, 8
  ret
  .section .rodata
  .Lgvan0: .string "P"
  .Lgvan1: .string "X"
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
  sub rsp, 8
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
  add rsp, 24
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
  mov qword ptr [r12 + 552], rsp
 push rsi
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, [rsp + 8]
 mov qword ptr [r12 + 544], rax
 pop rsi
main_α_body:
# IR_LIT_STRING
 xchain9_n0_α:
 mov qword ptr [r12 + 160], 1
 mov rax, qword ptr [rip + .Lx10_0]
 mov qword ptr [r12 + 168], rax
 jmp xchain9_n1_α
 xchain9_n0_β:
 jmp xchain9_n3_α
.Lx10_0:
 .quad .Lx10_0_s
.Lx10_0_s:
 .string "PAT$0"
 xchain9_n1_α:
# BOX IR_CALL SNO$MKPAT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+160] -> [zr+128]
 mov rax, qword ptr [r12 + 160]
 mov qword ptr [r12 + 128], rax
 mov rax, qword ptr [r12 + 168]
 mov qword ptr [r12 + 136], rax
  .section .rodata
  .Lrkfn12: .string "SNO$MKPAT"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn12]
 lea rsi, [r12 + 128]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 112], rax
 mov qword ptr [r12 + 120], rdx
 cmp eax, 99
 je xchain9_n3_α
 jmp xchain9_n2_α
 xchain9_n1_β:
 jmp xchain9_n3_α
# IR_ASSIGN gva
 xchain9_n2_α:
 mov rax, qword ptr [r12 + 112]
 mov rdx, qword ptr [r12 + 120]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 jmp xchain9_n3_α
 xchain9_n2_β:
 jmp xchain9_n3_α
# IR_LIT_STRING
 xchain9_n3_α:
 mov qword ptr [r12 + 208], 1
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [r12 + 216], rax
 jmp xchain9_n4_α
 xchain9_n3_β:
 jmp xchain9_n5_α
.Lx14_0:
 .quad .Lx14_0_s
.Lx14_0_s:
 .string "b"
# IR_ASSIGN gva
 xchain9_n4_α:
 mov rax, qword ptr [r12 + 208]
 mov rdx, qword ptr [r12 + 216]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 192], rax
 mov qword ptr [r12 + 200], rdx
 jmp xchain9_n5_α
 xchain9_n4_β:
 jmp xchain9_n5_α
# IR_VAR
 xchain9_n5_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 416], rax
 mov qword ptr [r12 + 424], rdx
 jmp xchain9_n6_α
 xchain9_n5_β:
 jmp xchain9_n7_α
# IR_MATCH_HEAD
 xchain9_n6_α:
 call rt_zls_mark@PLT
 mov qword ptr [r12 + 264], rax
 mov qword ptr [r12 + 272], rsp
 mov rdi, qword ptr [r12 + 416]
 mov rsi, qword ptr [r12 + 424]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov qword ptr [r12 + 296], rbp
 lea rcx, [rip + g_dcap_top]
 mov rbp, qword ptr [rcx + 0]
 mov qword ptr [r12 + 288], rbp
 mov dword ptr [r12 + 256], 0
.Lx18_0:
 mov r14d, dword ptr [r12 + 256]
 jmp xchain9_n8_α
 xchain9_n6_β:
 add dword ptr [r12 + 256], 1
 mov eax, dword ptr [r12 + 256]
 cmp eax, r15d
 jg .Lx18_1
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne .Lx18_1
 jmp .Lx18_0
.Lx18_1:
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [r12 + 264]
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
 mov rsp, qword ptr [r12 + 272]
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rsp, [rsp + 8]
 lea rcx, [rip + g_dcap_top]
 mov rax, qword ptr [r12 + 288]
 mov qword ptr [rcx + 0], rax
 mov rbp, qword ptr [r12 + 296]
 jmp xchain9_n7_α
# IR_LIT_STRING
 xchain9_n7_α:
 mov qword ptr [r12 + 512], 1
 mov rax, qword ptr [rip + .Lx19_0]
 mov qword ptr [r12 + 520], rax
 jmp xchain9_n9_α
 xchain9_n7_β:
 jmp main_γ
.Lx19_0:
 .quad .Lx19_0_s
.Lx19_0_s:
 .string "no match"
# IR_MATCH_CAPTURE_SAVE push
 xchain9_n8_α:
 lea rdi, [r12 + 336]
 mov esi, r14d
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_cap_push@PLT
 mov rsp, [rsp + 8]
 jmp xchain9_n10_α
 xchain9_n8_β:
 lea rdi, [r12 + 336]
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_cap_pop@PLT
 mov rsp, [rsp + 8]
 jmp xchain9_n6_β
# IR_ASSIGN global
 xchain9_n9_α:
 mov rsi, qword ptr [r12 + 512]
 mov rdx, qword ptr [r12 + 520]
 mov rdi, qword ptr [rip + .Lx22_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 496], rax
 mov qword ptr [r12 + 504], rdx
 jmp main_γ
 xchain9_n9_β:
 jmp main_γ
.Lx22_0:
 .quad .Lx22_0_s
.Lx22_0_s:
 .string "OUTPUT"
# IR_MATCH_DEFER inlined frozen head (FZ-5b)
 xchain9_n10_α:
 lea rax, [rip + proc_PAT$0_α]
 xor ecx, ecx
 mov qword ptr [r12 + 368], rcx
 test rax, rax
 jz .Lx23_0
 mov qword ptr [r12 + 368], rax
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, [rsp + 8]
 mov qword ptr [r12 + 376], rax
 mov rcx, qword ptr [r12 + 368]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx23_1
 mov rdi, qword ptr [r12 + 376]
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, [rsp + 8]
 xor eax, eax
 mov qword ptr [r12 + 368], rax
 jmp xchain9_n8_β
.Lx23_1:
 jmp xchain9_n11_α
.Lx23_0:
 push r14
 push r15
 push r13
 lea rcx, [rip + g_dcap_top]
 mov qword ptr [rcx + 0], rbp
 lea rdi, [rip + .S3]
 xor esi, esi
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_defer_open@PLT
.Lx23_2:
 test rax, rax
 je .Lx23_3
 mov rcx, rsp
 sub rsp, rax
 sub rsp, 16
 and rsp, -16
 mov qword ptr [rsp + 0], rcx
 mov rdi, rsp
 add rdi, 16
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 add rdi, 16
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 mov rsp, qword ptr [rsp + 0]
 call rt_defer_step@PLT
 jmp .Lx23_2
.Lx23_3:
 mov rsp, [rsp + 8]
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, [rsp + 8]
 test eax, eax
 js xchain9_n8_β
 mov r14d, eax
 jmp xchain9_n11_α
 xchain9_n10_β:
 mov rcx, qword ptr [r12 + 368]
 test rcx, rcx
 jz xchain9_n8_β
 mov rdi, qword ptr [r12 + 376]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx23_1
 mov rdi, qword ptr [r12 + 376]
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, [rsp + 8]
 xor eax, eax
 mov qword ptr [r12 + 368], rax
 jmp xchain9_n8_β
# IR_MATCH_CAPTURE_COND (rbp-dcap inline pend)
 xchain9_n11_α:
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 lea rdi, [r12 + 336]
 call rt_cap_top@PLT
 mov rsp, [rsp + 8]
 lea rcx, [rip + .S4]
 mov qword ptr [rbp + 0], rcx
 mov esi, eax
 mov qword ptr [rbp + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [rbp + 16], rdx
 add rbp, 24
 jmp xchain9_n12_α
 xchain9_n11_β:
 sub rbp, 24
 jmp xchain9_n10_β
# IR_MATCH_RELEASE
 xchain9_n12_α:
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [r12 + 264]
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
 mov rsp, qword ptr [r12 + 272]
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rsp, [rsp + 8]
 push r14
 push r15
 push r13
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 lea rcx, [rip + g_dcap_top]
 mov qword ptr [rcx + 0], rbp
 mov rdi, qword ptr [r12 + 288]
 mov rsi, rbp
 mov rdx, r13
 call rt_dcap_end_ok_open@PLT
.Lx27_1:
 test rax, rax
 je .Lx27_2
 mov rcx, rsp
 sub rsp, rax
 sub rsp, 16
 and rsp, -16
 mov qword ptr [rsp + 0], rcx
 mov rdi, rsp
 add rdi, 16
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 add rdi, 16
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 mov rsp, qword ptr [rsp + 0]
 call rt_dcap_step@PLT
 jmp .Lx27_1
.Lx27_2:
 call rt_dcap_end_ok_close@PLT
 mov rsp, [rsp + 8]
 pop r13
 pop r15
 pop r14
 lea rcx, [rip + g_dcap_top]
 mov rax, qword ptr [r12 + 288]
 mov qword ptr [rcx + 0], rax
 mov rbp, qword ptr [r12 + 296]
 jmp xchain9_n13_α
# IR_VAR
 xchain9_n13_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 464], rax
 mov qword ptr [r12 + 472], rdx
 jmp xchain9_n14_α
 xchain9_n13_β:
 jmp main_γ
# IR_ASSIGN global
 xchain9_n14_α:
 mov rsi, qword ptr [r12 + 464]
 mov rdx, qword ptr [r12 + 472]
 mov rdi, qword ptr [rip + .Lx29_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 448], rax
 mov qword ptr [r12 + 456], rdx
 jmp main_γ
 xchain9_n14_β:
 jmp main_γ
.Lx29_0:
 .quad .Lx29_0_s
.Lx29_0_s:
 .string "OUTPUT"
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [r12 + 552]
pop r12
ret
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [r12 + 544]
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
main_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, qword ptr [r12 + 552]
pop r12
ret
.section .rodata
.S0: .string "a"
.S1: .string "b"
.S2: .string "c"
.S3: .string "P"
.S4: .string "V"
.text
