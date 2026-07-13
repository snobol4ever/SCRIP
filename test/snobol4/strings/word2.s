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
  mov qword ptr [r12 + 488], rsp
  cmp esi, 0
  jne proc_PAT$0_β
 push rsi
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, [rsp + 8]
 mov qword ptr [r12 + 480], rax
 pop rsi
proc_PAT$0_α_body:
lea rax, [rip + xchain0_n0_β]
mov qword ptr [r12 + 464], rax
# IR_MATCH_SEQ_NARY
 xchain0_n0_α:
 mov dword ptr [r12 + 32], r14d
 mov dword ptr [r12 + 36], 0
 jmp xchain0_n1_α
xchain0_n0_as:
 mov eax, dword ptr [r12 + 36]
 add eax, 1
 mov dword ptr [r12 + 36], eax
 cmp eax, 1
 je xchain0_n3_α
 cmp eax, 2
 je xchain0_n5_α
 cmp eax, 3
 je xchain0_n7_α
 cmp eax, 4
 je xchain0_n9_α
 cmp eax, 5
 je xchain0_n10_α
 cmp eax, 6
 je xchain0_n12_α
 jmp proc_PAT$0_γ
 xchain0_n0_β:
 mov dword ptr [r12 + 36], 7
xchain0_n0_af:
 mov eax, dword ptr [r12 + 36]
 sub eax, 1
 mov dword ptr [r12 + 36], eax
 cmp eax, 0
 je xchain0_n2_β
 cmp eax, 1
 je xchain0_n4_β
 cmp eax, 2
 je xchain0_n6_β
 cmp eax, 3
 je xchain0_n8_β
 cmp eax, 4
 je xchain0_n9_β
 cmp eax, 5
 je xchain0_n11_β
 cmp eax, 6
 je xchain0_n13_β
 jmp proc_PAT$0_ω
# IR_LIT_INTEGER
 xchain0_n1_α:
 mov qword ptr [r12 + 64], 6
 mov rax, qword ptr [rip + .Lx3_0]
 mov qword ptr [r12 + 72], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n0_af
.Lx3_0:
 .quad 0
# IR_MATCH_POS
 xchain0_n2_α:
 mov rax, 0
 cmp r14d, eax
 jne xchain0_n0_af
 jmp xchain0_n0_as
 xchain0_n2_β:
 jmp xchain0_n0_af
# IR_MATCH_CAPTURE_SAVE push
 xchain0_n3_α:
 lea rdi, [r12 + 128]
 mov esi, r14d
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_cap_push@PLT
 mov rsp, [rsp + 8]
 jmp xchain0_n14_α
 xchain0_n3_β:
 lea rdi, [r12 + 128]
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_cap_pop@PLT
 mov rsp, [rsp + 8]
 jmp xchain0_n0_af
# IR_MATCH_CAPTURE_COND (rbp-dcap inline pend)
 xchain0_n4_α:
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 lea rdi, [r12 + 128]
 call rt_cap_top@PLT
 mov rsp, [rsp + 8]
 lea rcx, [rip + .S0]
 mov qword ptr [rbp + 0], rcx
 mov esi, eax
 mov qword ptr [rbp + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [rbp + 16], rdx
 add rbp, 24
 jmp xchain0_n0_as
 xchain0_n4_β:
 sub rbp, 24
 jmp xchain0_n14_β
# IR_LIT_INTEGER
 xchain0_n5_α:
 mov qword ptr [r12 + 192], 6
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [r12 + 200], rax
 jmp xchain0_n6_α
 xchain0_n5_β:
 jmp xchain0_n0_af
.Lx9_0:
 .quad 6
# IR_MATCH_TAB
 xchain0_n6_α:
 mov dword ptr [r12 + 176], r14d
 mov rax, 6
 cmp r14d, eax
 jg xchain0_n0_af
 cmp r15d, eax
 jl xchain0_n0_af
 mov r14d, eax
 jmp xchain0_n0_as
 xchain0_n6_β:
 mov r14d, dword ptr [r12 + 176]
 jmp xchain0_n0_af
# IR_MATCH_CAPTURE_SAVE push
 xchain0_n7_α:
 lea rdi, [r12 + 256]
 mov esi, r14d
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_cap_push@PLT
 mov rsp, [rsp + 8]
 jmp xchain0_n15_α
 xchain0_n7_β:
 lea rdi, [r12 + 256]
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_cap_pop@PLT
 mov rsp, [rsp + 8]
 jmp xchain0_n0_af
# IR_MATCH_CAPTURE_COND (rbp-dcap inline pend)
 xchain0_n8_α:
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 lea rdi, [r12 + 256]
 call rt_cap_top@PLT
 mov rsp, [rsp + 8]
 lea rcx, [rip + .S1]
 mov qword ptr [rbp + 0], rcx
 mov esi, eax
 mov qword ptr [rbp + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [rbp + 16], rdx
 add rbp, 24
 jmp xchain0_n0_as
 xchain0_n8_β:
 sub rbp, 24
 jmp xchain0_n15_β
# IR_MATCH_LIT
 xchain0_n9_α:
 mov eax, r14d
 add eax, 2
 cmp eax, r15d
 jg xchain0_n0_af
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S2]
 mov edx, 2
 call memcmp@PLT
 test eax, eax
 jne xchain0_n0_af
 add r14d, 2
 jmp xchain0_n0_as
 xchain0_n9_β:
 sub r14d, 2
 jmp xchain0_n0_af
# IR_LIT_INTEGER
 xchain0_n10_α:
 mov qword ptr [r12 + 352], 6
 mov rax, qword ptr [rip + .Lx17_0]
 mov qword ptr [r12 + 360], rax
 jmp xchain0_n11_α
 xchain0_n10_β:
 jmp xchain0_n0_af
.Lx17_0:
 .quad 24
# IR_MATCH_TAB
 xchain0_n11_α:
 mov dword ptr [r12 + 336], r14d
 mov rax, 24
 cmp r14d, eax
 jg xchain0_n0_af
 cmp r15d, eax
 jl xchain0_n0_af
 mov r14d, eax
 jmp xchain0_n0_as
 xchain0_n11_β:
 mov r14d, dword ptr [r12 + 336]
 jmp xchain0_n0_af
# IR_MATCH_CAPTURE_SAVE push
 xchain0_n12_α:
 lea rdi, [r12 + 416]
 mov esi, r14d
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_cap_push@PLT
 mov rsp, [rsp + 8]
 jmp xchain0_n16_α
 xchain0_n12_β:
 lea rdi, [r12 + 416]
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_cap_pop@PLT
 mov rsp, [rsp + 8]
 jmp xchain0_n0_af
# IR_MATCH_CAPTURE_COND (rbp-dcap inline pend)
 xchain0_n13_α:
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 lea rdi, [r12 + 416]
 call rt_cap_top@PLT
 mov rsp, [rsp + 8]
 lea rcx, [rip + .S3]
 mov qword ptr [rbp + 0], rcx
 mov esi, eax
 mov qword ptr [rbp + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [rbp + 16], rdx
 add rbp, 24
 jmp xchain0_n0_as
 xchain0_n13_β:
 sub rbp, 24
 jmp xchain0_n16_β
# IR_MATCH_LEN
 xchain0_n14_α:
 mov eax, r14d
 add eax, 4
 cmp eax, r15d
 jg xchain0_n3_β
 add r14d, 4
 jmp xchain0_n4_α
 xchain0_n14_β:
 sub r14d, 4
 jmp xchain0_n3_β
# IR_MATCH_ARB
 xchain0_n15_α:
 sub rsp, 16
 mov rcx, qword ptr [r12 + 296]
 mov qword ptr [rsp + 0], rcx
 mov qword ptr [r12 + 296], rsp
 mov dword ptr [r12 + 288], 0
 mov eax, r14d
 mov dword ptr [r12 + 292], eax
 jmp xchain0_n8_α
 xchain0_n15_β:
 add dword ptr [r12 + 288], 1
 mov eax, dword ptr [r12 + 292]
 add eax, dword ptr [r12 + 288]
 cmp eax, r15d
 jg .Lx25_0
 mov r14d, eax
 jmp xchain0_n8_α
.Lx25_0:
 mov r14d, dword ptr [r12 + 292]
 mov rax, qword ptr [r12 + 296]
 mov rcx, qword ptr [rax + 0]
 mov qword ptr [r12 + 296], rcx
 lea rsp, [rax + 16]
 jmp xchain0_n7_β
# IR_MATCH_REM
 xchain0_n16_α:
 mov dword ptr [r12 + 448], r14d
 mov r14d, r15d
 jmp xchain0_n13_α
 xchain0_n16_β:
 mov r14d, dword ptr [r12 + 448]
 jmp xchain0_n12_β
proc_PAT$0_β:
jmp qword ptr [r12 + 464]
proc_PAT$0_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [r12 + 488]
pop r12
ret
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [r12 + 480]
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
proc_PAT$0_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, qword ptr [r12 + 488]
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
  mov esi, 496
  call rt_proc_set_frame_bytes@PLT
  add rsp, 8
  ret
  .section .rodata
  .Lgvan0: .string "PAT"
  .Lgvan1: .string "LINE"
  .Lgvan2: .string "WHO"
  .Lgvan3: .string "WHAT"
  .Lgvan4: .string "WHEN"
  .align 8
__gva_names:
  .quad .Lgvan0
  .quad .Lgvan1
  .quad .Lgvan2
  .quad .Lgvan3
  .quad .Lgvan4
  .section .bss
  .align 16
__gva: .space 80, 0
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
  mov edx, 5
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
  mov qword ptr [r12 + 712], rsp
 push rsi
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, [rsp + 8]
 mov qword ptr [r12 + 704], rax
 pop rsi
main_α_body:
# IR_LIT_STRING
 xchain27_n0_α:
 mov qword ptr [r12 + 160], 1
 mov rax, qword ptr [rip + .Lx28_0]
 mov qword ptr [r12 + 168], rax
 jmp xchain27_n1_α
 xchain27_n0_β:
 jmp xchain27_n3_α
.Lx28_0:
 .quad .Lx28_0_s
.Lx28_0_s:
 .string "PAT$0"
 xchain27_n1_α:
# BOX IR_CALL SNO$MKPAT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+160] -> [zr+128]
 mov rax, qword ptr [r12 + 160]
 mov qword ptr [r12 + 128], rax
 mov rax, qword ptr [r12 + 168]
 mov qword ptr [r12 + 136], rax
  .section .rodata
  .Lrkfn30: .string "SNO$MKPAT"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn30]
 lea rsi, [r12 + 128]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 112], rax
 mov qword ptr [r12 + 120], rdx
 cmp eax, 99
 je xchain27_n3_α
 jmp xchain27_n2_α
 xchain27_n1_β:
 jmp xchain27_n3_α
# IR_ASSIGN gva
 xchain27_n2_α:
 mov rax, qword ptr [r12 + 112]
 mov rdx, qword ptr [r12 + 120]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 jmp xchain27_n3_α
 xchain27_n2_β:
 jmp xchain27_n3_α
# IR_VAR
 xchain27_n3_α:
 mov rdi, qword ptr [rip + .Lx32_0]
 call NV_GET_fn@PLT
 cmp eax, 99
 je main_γ
 mov qword ptr [r12 + 208], rax
 mov qword ptr [r12 + 216], rdx
 jmp xchain27_n4_α
 xchain27_n3_β:
 jmp main_γ
.Lx32_0:
 .quad .Lx32_0_s
.Lx32_0_s:
 .string "INPUT"
# IR_ASSIGN gva
 xchain27_n4_α:
 mov rax, qword ptr [r12 + 208]
 mov rdx, qword ptr [r12 + 216]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 192], rax
 mov qword ptr [r12 + 200], rdx
 jmp xchain27_n5_α
 xchain27_n4_β:
 jmp main_γ
# IR_VAR
 xchain27_n5_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 368], rax
 mov qword ptr [r12 + 376], rdx
 jmp xchain27_n6_α
 xchain27_n5_β:
 jmp xchain27_n3_α
# IR_MATCH_HEAD
 xchain27_n6_α:
 call rt_zls_mark@PLT
 mov qword ptr [r12 + 264], rax
 mov qword ptr [r12 + 272], rsp
 mov rdi, qword ptr [r12 + 368]
 mov rsi, qword ptr [r12 + 376]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov qword ptr [r12 + 296], rbp
 lea rcx, [rip + g_dcap_top]
 mov rbp, qword ptr [rcx + 0]
 mov qword ptr [r12 + 288], rbp
 mov dword ptr [r12 + 256], 0
.Lx36_0:
 mov r14d, dword ptr [r12 + 256]
 jmp xchain27_n7_α
 xchain27_n6_β:
 add dword ptr [r12 + 256], 1
 mov eax, dword ptr [r12 + 256]
 cmp eax, r15d
 jg .Lx36_1
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne .Lx36_1
 jmp .Lx36_0
.Lx36_1:
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
 jmp xchain27_n3_α
# IR_MATCH_DEFER inlined frozen head (FZ-5b)
 xchain27_n7_α:
 lea rax, [rip + proc_PAT$0_α]
 xor ecx, ecx
 mov qword ptr [r12 + 320], rcx
 test rax, rax
 jz .Lx37_0
 mov qword ptr [r12 + 320], rax
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, [rsp + 8]
 mov qword ptr [r12 + 328], rax
 mov rcx, qword ptr [r12 + 320]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx37_1
 mov rdi, qword ptr [r12 + 328]
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, [rsp + 8]
 xor eax, eax
 mov qword ptr [r12 + 320], rax
 jmp xchain27_n6_β
.Lx37_1:
 jmp xchain27_n8_α
.Lx37_0:
 push r14
 push r15
 push r13
 lea rcx, [rip + g_dcap_top]
 mov qword ptr [rcx + 0], rbp
 lea rdi, [rip + .S4]
 xor esi, esi
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_defer_open@PLT
.Lx37_2:
 test rax, rax
 je .Lx37_3
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
 jmp .Lx37_2
.Lx37_3:
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
 js xchain27_n6_β
 mov r14d, eax
 jmp xchain27_n8_α
 xchain27_n7_β:
 mov rcx, qword ptr [r12 + 320]
 test rcx, rcx
 jz xchain27_n6_β
 mov rdi, qword ptr [r12 + 328]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx37_1
 mov rdi, qword ptr [r12 + 328]
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, [rsp + 8]
 xor eax, eax
 mov qword ptr [r12 + 320], rax
 jmp xchain27_n6_β
# IR_MATCH_RELEASE
 xchain27_n8_α:
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
.Lx39_1:
 test rax, rax
 je .Lx39_2
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
 jmp .Lx39_1
.Lx39_2:
 call rt_dcap_end_ok_close@PLT
 mov rsp, [rsp + 8]
 pop r13
 pop r15
 pop r14
 lea rcx, [rip + g_dcap_top]
 mov rax, qword ptr [r12 + 288]
 mov qword ptr [rcx + 0], rax
 mov rbp, qword ptr [r12 + 296]
 jmp xchain27_n9_α
# IR_VAR
 xchain27_n9_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 544], rax
 mov qword ptr [r12 + 552], rdx
 jmp xchain27_n10_α
 xchain27_n9_β:
 jmp xchain27_n3_α
# IR_LIT_STRING
 xchain27_n10_α:
 mov qword ptr [r12 + 576], 1
 mov rax, qword ptr [rip + .Lx41_0]
 mov qword ptr [r12 + 584], rax
 jmp xchain27_n11_α
 xchain27_n10_β:
 jmp xchain27_n3_α
.Lx41_0:
 .quad .Lx41_0_s
.Lx41_0_s:
 .string " invented the "
 xchain27_n11_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 544]
 mov rsi, qword ptr [r12 + 552]
 mov rdx, qword ptr [r12 + 576]
 mov rcx, qword ptr [r12 + 584]
 call str_concat_d@PLT
 mov qword ptr [r12 + 512], rax
 mov qword ptr [r12 + 520], rdx
 jmp xchain27_n12_α
 xchain27_n11_β:
 jmp xchain27_n3_α
# IR_VAR
 xchain27_n12_α:
 mov rax, qword ptr [rbx + 48]
 mov rdx, qword ptr [rbx + 56]
 mov qword ptr [r12 + 608], rax
 mov qword ptr [r12 + 616], rdx
 jmp xchain27_n13_α
 xchain27_n12_β:
 jmp xchain27_n3_α
 xchain27_n13_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 512]
 mov rsi, qword ptr [r12 + 520]
 mov rdx, qword ptr [r12 + 608]
 mov rcx, qword ptr [r12 + 616]
 call str_concat_d@PLT
 mov qword ptr [r12 + 480], rax
 mov qword ptr [r12 + 488], rdx
 jmp xchain27_n14_α
 xchain27_n13_β:
 jmp xchain27_n3_α
# IR_LIT_STRING
 xchain27_n14_α:
 mov qword ptr [r12 + 640], 1
 mov rax, qword ptr [rip + .Lx45_0]
 mov qword ptr [r12 + 648], rax
 jmp xchain27_n15_α
 xchain27_n14_β:
 jmp xchain27_n3_α
.Lx45_0:
 .quad .Lx45_0_s
.Lx45_0_s:
 .string " in "
 xchain27_n15_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 480]
 mov rsi, qword ptr [r12 + 488]
 mov rdx, qword ptr [r12 + 640]
 mov rcx, qword ptr [r12 + 648]
 call str_concat_d@PLT
 mov qword ptr [r12 + 448], rax
 mov qword ptr [r12 + 456], rdx
 jmp xchain27_n16_α
 xchain27_n15_β:
 jmp xchain27_n3_α
# IR_VAR
 xchain27_n16_α:
 mov rax, qword ptr [rbx + 64]
 mov rdx, qword ptr [rbx + 72]
 mov qword ptr [r12 + 672], rax
 mov qword ptr [r12 + 680], rdx
 jmp xchain27_n17_α
 xchain27_n16_β:
 jmp xchain27_n3_α
 xchain27_n17_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 448]
 mov rsi, qword ptr [r12 + 456]
 mov rdx, qword ptr [r12 + 672]
 mov rcx, qword ptr [r12 + 680]
 call str_concat_d@PLT
 mov qword ptr [r12 + 416], rax
 mov qword ptr [r12 + 424], rdx
 jmp xchain27_n18_α
 xchain27_n17_β:
 jmp xchain27_n3_α
# IR_ASSIGN global
 xchain27_n18_α:
 mov rsi, qword ptr [r12 + 416]
 mov rdx, qword ptr [r12 + 424]
 mov rdi, qword ptr [rip + .Lx49_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 400], rax
 mov qword ptr [r12 + 408], rdx
 jmp xchain27_n3_α
 xchain27_n18_β:
 jmp xchain27_n3_α
.Lx49_0:
 .quad .Lx49_0_s
.Lx49_0_s:
 .string "OUTPUT"
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [r12 + 712]
pop r12
ret
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [r12 + 704]
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
main_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, qword ptr [r12 + 712]
pop r12
ret
.section .rodata
.S0: .string "WHEN"
.S1: .string "WHO"
.S2: .string " :"
.S3: .string "WHAT"
.S4: .string "PAT"
.text
