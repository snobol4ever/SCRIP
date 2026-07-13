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
  mov qword ptr [r12 + 520], rsp
  cmp esi, 0
  jne proc_PAT$0_β
 push rsi
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, [rsp + 8]
 mov qword ptr [r12 + 512], rax
 pop rsi
proc_PAT$0_α_body:
lea rax, [rip + xchain0_n0_β]
mov qword ptr [r12 + 496], rax
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
 je xchain0_n6_α
 cmp eax, 4
 je xchain0_n8_α
 cmp eax, 5
 je xchain0_n9_α
 jmp proc_PAT$0_γ
 xchain0_n0_β:
 mov dword ptr [r12 + 36], 6
xchain0_n0_af:
 mov eax, dword ptr [r12 + 36]
 sub eax, 1
 mov dword ptr [r12 + 36], eax
 cmp eax, 0
 je xchain0_n2_β
 cmp eax, 1
 je xchain0_n4_β
 cmp eax, 2
 je xchain0_n5_β
 cmp eax, 3
 je xchain0_n7_β
 cmp eax, 4
 je xchain0_n8_β
 cmp eax, 5
 je xchain0_n10_β
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
 jmp xchain0_n11_α
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
 jmp xchain0_n11_β
# IR_MATCH_SEQ_NARY
 xchain0_n5_α:
 mov dword ptr [r12 + 192], r14d
 mov dword ptr [r12 + 196], 0
 jmp xchain0_n12_α
xchain0_n5_as:
 mov eax, dword ptr [r12 + 196]
 add eax, 1
 mov dword ptr [r12 + 196], eax
 cmp eax, 1
 je xchain0_n13_α
 jmp xchain0_n0_as
 xchain0_n5_β:
 mov dword ptr [r12 + 196], 2
xchain0_n5_af:
 mov eax, dword ptr [r12 + 196]
 sub eax, 1
 mov dword ptr [r12 + 196], eax
 cmp eax, 0
 je xchain0_n12_β
 cmp eax, 1
 je xchain0_n13_β
 jmp xchain0_n0_af
# IR_MATCH_CAPTURE_SAVE push
 xchain0_n6_α:
 lea rdi, [r12 + 288]
 mov esi, r14d
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_cap_push@PLT
 mov rsp, [rsp + 8]
 jmp xchain0_n14_α
 xchain0_n6_β:
 lea rdi, [r12 + 288]
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_cap_pop@PLT
 mov rsp, [rsp + 8]
 jmp xchain0_n0_af
# IR_MATCH_CAPTURE_COND (rbp-dcap inline pend)
 xchain0_n7_α:
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 lea rdi, [r12 + 288]
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
 xchain0_n7_β:
 sub rbp, 24
 jmp xchain0_n14_β
# IR_MATCH_SEQ_NARY
 xchain0_n8_α:
 mov dword ptr [r12 + 352], r14d
 mov dword ptr [r12 + 356], 0
 jmp xchain0_n15_α
xchain0_n8_as:
 mov eax, dword ptr [r12 + 356]
 add eax, 1
 mov dword ptr [r12 + 356], eax
 cmp eax, 1
 je xchain0_n16_α
 jmp xchain0_n0_as
 xchain0_n8_β:
 mov dword ptr [r12 + 356], 2
xchain0_n8_af:
 mov eax, dword ptr [r12 + 356]
 sub eax, 1
 mov dword ptr [r12 + 356], eax
 cmp eax, 0
 je xchain0_n15_β
 cmp eax, 1
 je xchain0_n16_β
 jmp xchain0_n0_af
# IR_MATCH_CAPTURE_SAVE push
 xchain0_n9_α:
 lea rdi, [r12 + 448]
 mov esi, r14d
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_cap_push@PLT
 mov rsp, [rsp + 8]
 jmp xchain0_n17_α
 xchain0_n9_β:
 lea rdi, [r12 + 448]
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_cap_pop@PLT
 mov rsp, [rsp + 8]
 jmp xchain0_n0_af
# IR_MATCH_CAPTURE_COND (rbp-dcap inline pend)
 xchain0_n10_α:
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 lea rdi, [r12 + 448]
 call rt_cap_top@PLT
 mov rsp, [rsp + 8]
 lea rcx, [rip + .S2]
 mov qword ptr [rbp + 0], rcx
 mov esi, eax
 mov qword ptr [rbp + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [rbp + 16], rdx
 add rbp, 24
 jmp xchain0_n0_as
 xchain0_n10_β:
 sub rbp, 24
 jmp xchain0_n17_β
# IR_MATCH_BREAK
 xchain0_n11_α:
 mov dword ptr [r12 + 160], 0
.Lx22_0:
 mov eax, r14d
 add eax, dword ptr [r12 + 160]
 cmp eax, r15d
 jge xchain0_n3_β
 movsxd rcx, eax
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S3]
 call strchr@PLT
 test rax, rax
 jnz .Lx22_1
 add dword ptr [r12 + 160], 1
 jmp .Lx22_0
.Lx22_1:
 mov eax, r14d
 add eax, dword ptr [r12 + 160]
 mov r14d, eax
 jmp xchain0_n4_α
 xchain0_n11_β:
 mov eax, r14d
 sub eax, dword ptr [r12 + 160]
 mov r14d, eax
 jmp xchain0_n3_β
# IR_MATCH_LIT
 xchain0_n12_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain0_n5_af
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S3]
 mov edx, 1
 call memcmp@PLT
 test eax, eax
 jne xchain0_n5_af
 add r14d, 1
 jmp xchain0_n5_as
 xchain0_n12_β:
 sub r14d, 1
 jmp xchain0_n5_af
# IR_MATCH_SPAN
 xchain0_n13_α:
 mov dword ptr [r12 + 240], 0
.Lx26_0:
 mov eax, r14d
 add eax, dword ptr [r12 + 240]
 cmp eax, r15d
 jge .Lx26_1
 movsxd rcx, eax
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S3]
 call strchr@PLT
 test rax, rax
 je .Lx26_1
 add dword ptr [r12 + 240], 1
 jmp .Lx26_0
.Lx26_1:
 mov eax, dword ptr [r12 + 240]
 test eax, eax
 jle xchain0_n5_af
 mov edx, r14d
 mov dword ptr [r12 + 244], edx
 add edx, eax
 mov r14d, edx
 jmp xchain0_n5_as
 xchain0_n13_β:
 xchain0_n13_β:
 mov r14d, dword ptr [r12 + 244]
 jmp xchain0_n5_af
# IR_MATCH_ARB
 xchain0_n14_α:
 sub rsp, 16
 mov rcx, qword ptr [r12 + 328]
 mov qword ptr [rsp + 0], rcx
 mov qword ptr [r12 + 328], rsp
 mov dword ptr [r12 + 320], 0
 mov eax, r14d
 mov dword ptr [r12 + 324], eax
 jmp xchain0_n7_α
 xchain0_n14_β:
 add dword ptr [r12 + 320], 1
 mov eax, dword ptr [r12 + 324]
 add eax, dword ptr [r12 + 320]
 cmp eax, r15d
 jg .Lx28_0
 mov r14d, eax
 jmp xchain0_n7_α
.Lx28_0:
 mov r14d, dword ptr [r12 + 324]
 mov rax, qword ptr [r12 + 328]
 mov rcx, qword ptr [rax + 0]
 mov qword ptr [r12 + 328], rcx
 lea rsp, [rax + 16]
 jmp xchain0_n6_β
# IR_MATCH_LIT
 xchain0_n15_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain0_n8_af
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S3]
 mov edx, 1
 call memcmp@PLT
 test eax, eax
 jne xchain0_n8_af
 add r14d, 1
 jmp xchain0_n8_as
 xchain0_n15_β:
 sub r14d, 1
 jmp xchain0_n8_af
# IR_MATCH_SPAN
 xchain0_n16_α:
 mov dword ptr [r12 + 400], 0
.Lx32_0:
 mov eax, r14d
 add eax, dword ptr [r12 + 400]
 cmp eax, r15d
 jge .Lx32_1
 movsxd rcx, eax
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S4]
 call strchr@PLT
 test rax, rax
 je .Lx32_1
 add dword ptr [r12 + 400], 1
 jmp .Lx32_0
.Lx32_1:
 mov eax, dword ptr [r12 + 400]
 test eax, eax
 jle xchain0_n8_af
 mov edx, r14d
 mov dword ptr [r12 + 404], edx
 add edx, eax
 mov r14d, edx
 jmp xchain0_n8_as
 xchain0_n16_β:
 xchain0_n16_β:
 mov r14d, dword ptr [r12 + 404]
 jmp xchain0_n8_af
# IR_MATCH_REM
 xchain0_n17_α:
 mov dword ptr [r12 + 480], r14d
 mov r14d, r15d
 jmp xchain0_n10_α
 xchain0_n17_β:
 mov r14d, dword ptr [r12 + 480]
 jmp xchain0_n9_β
proc_PAT$0_β:
jmp qword ptr [r12 + 496]
proc_PAT$0_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [r12 + 520]
pop r12
ret
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [r12 + 512]
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
proc_PAT$0_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, qword ptr [r12 + 520]
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
  mov esi, 528
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
 xchain34_n0_α:
 mov qword ptr [r12 + 160], 1
 mov rax, qword ptr [rip + .Lx35_0]
 mov qword ptr [r12 + 168], rax
 jmp xchain34_n1_α
 xchain34_n0_β:
 jmp xchain34_n3_α
.Lx35_0:
 .quad .Lx35_0_s
.Lx35_0_s:
 .string "PAT$0"
 xchain34_n1_α:
# BOX IR_CALL SNO$MKPAT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+160] -> [zr+128]
 mov rax, qword ptr [r12 + 160]
 mov qword ptr [r12 + 128], rax
 mov rax, qword ptr [r12 + 168]
 mov qword ptr [r12 + 136], rax
  .section .rodata
  .Lrkfn37: .string "SNO$MKPAT"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn37]
 lea rsi, [r12 + 128]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 112], rax
 mov qword ptr [r12 + 120], rdx
 cmp eax, 99
 je xchain34_n3_α
 jmp xchain34_n2_α
 xchain34_n1_β:
 jmp xchain34_n3_α
# IR_ASSIGN gva
 xchain34_n2_α:
 mov rax, qword ptr [r12 + 112]
 mov rdx, qword ptr [r12 + 120]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 jmp xchain34_n3_α
 xchain34_n2_β:
 jmp xchain34_n3_α
# IR_VAR
 xchain34_n3_α:
 mov rdi, qword ptr [rip + .Lx39_0]
 call NV_GET_fn@PLT
 cmp eax, 99
 je main_γ
 mov qword ptr [r12 + 208], rax
 mov qword ptr [r12 + 216], rdx
 jmp xchain34_n4_α
 xchain34_n3_β:
 jmp main_γ
.Lx39_0:
 .quad .Lx39_0_s
.Lx39_0_s:
 .string "INPUT"
# IR_ASSIGN gva
 xchain34_n4_α:
 mov rax, qword ptr [r12 + 208]
 mov rdx, qword ptr [r12 + 216]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 192], rax
 mov qword ptr [r12 + 200], rdx
 jmp xchain34_n5_α
 xchain34_n4_β:
 jmp main_γ
# IR_VAR
 xchain34_n5_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 368], rax
 mov qword ptr [r12 + 376], rdx
 jmp xchain34_n6_α
 xchain34_n5_β:
 jmp xchain34_n3_α
# IR_MATCH_HEAD
 xchain34_n6_α:
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
.Lx43_0:
 mov r14d, dword ptr [r12 + 256]
 jmp xchain34_n7_α
 xchain34_n6_β:
 add dword ptr [r12 + 256], 1
 mov eax, dword ptr [r12 + 256]
 cmp eax, r15d
 jg .Lx43_1
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne .Lx43_1
 jmp .Lx43_0
.Lx43_1:
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
 jmp xchain34_n3_α
# IR_MATCH_DEFER inlined frozen head (FZ-5b)
 xchain34_n7_α:
 lea rax, [rip + proc_PAT$0_α]
 xor ecx, ecx
 mov qword ptr [r12 + 320], rcx
 test rax, rax
 jz .Lx44_0
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
 je .Lx44_1
 mov rdi, qword ptr [r12 + 328]
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, [rsp + 8]
 xor eax, eax
 mov qword ptr [r12 + 320], rax
 jmp xchain34_n6_β
.Lx44_1:
 jmp xchain34_n8_α
.Lx44_0:
 push r14
 push r15
 push r13
 lea rcx, [rip + g_dcap_top]
 mov qword ptr [rcx + 0], rbp
 lea rdi, [rip + .S5]
 xor esi, esi
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_defer_open@PLT
.Lx44_2:
 test rax, rax
 je .Lx44_3
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
 jmp .Lx44_2
.Lx44_3:
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
 js xchain34_n6_β
 mov r14d, eax
 jmp xchain34_n8_α
 xchain34_n7_β:
 mov rcx, qword ptr [r12 + 320]
 test rcx, rcx
 jz xchain34_n6_β
 mov rdi, qword ptr [r12 + 328]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx44_1
 mov rdi, qword ptr [r12 + 328]
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, [rsp + 8]
 xor eax, eax
 mov qword ptr [r12 + 320], rax
 jmp xchain34_n6_β
# IR_MATCH_RELEASE
 xchain34_n8_α:
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
.Lx46_1:
 test rax, rax
 je .Lx46_2
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
 jmp .Lx46_1
.Lx46_2:
 call rt_dcap_end_ok_close@PLT
 mov rsp, [rsp + 8]
 pop r13
 pop r15
 pop r14
 lea rcx, [rip + g_dcap_top]
 mov rax, qword ptr [r12 + 288]
 mov qword ptr [rcx + 0], rax
 mov rbp, qword ptr [r12 + 296]
 jmp xchain34_n9_α
# IR_VAR
 xchain34_n9_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 544], rax
 mov qword ptr [r12 + 552], rdx
 jmp xchain34_n10_α
 xchain34_n9_β:
 jmp xchain34_n3_α
# IR_LIT_STRING
 xchain34_n10_α:
 mov qword ptr [r12 + 576], 1
 mov rax, qword ptr [rip + .Lx48_0]
 mov qword ptr [r12 + 584], rax
 jmp xchain34_n11_α
 xchain34_n10_β:
 jmp xchain34_n3_α
.Lx48_0:
 .quad .Lx48_0_s
.Lx48_0_s:
 .string " invented the "
 xchain34_n11_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 544]
 mov rsi, qword ptr [r12 + 552]
 mov rdx, qword ptr [r12 + 576]
 mov rcx, qword ptr [r12 + 584]
 call str_concat_d@PLT
 mov qword ptr [r12 + 512], rax
 mov qword ptr [r12 + 520], rdx
 jmp xchain34_n12_α
 xchain34_n11_β:
 jmp xchain34_n3_α
# IR_VAR
 xchain34_n12_α:
 mov rax, qword ptr [rbx + 48]
 mov rdx, qword ptr [rbx + 56]
 mov qword ptr [r12 + 608], rax
 mov qword ptr [r12 + 616], rdx
 jmp xchain34_n13_α
 xchain34_n12_β:
 jmp xchain34_n3_α
 xchain34_n13_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 512]
 mov rsi, qword ptr [r12 + 520]
 mov rdx, qword ptr [r12 + 608]
 mov rcx, qword ptr [r12 + 616]
 call str_concat_d@PLT
 mov qword ptr [r12 + 480], rax
 mov qword ptr [r12 + 488], rdx
 jmp xchain34_n14_α
 xchain34_n13_β:
 jmp xchain34_n3_α
# IR_LIT_STRING
 xchain34_n14_α:
 mov qword ptr [r12 + 640], 1
 mov rax, qword ptr [rip + .Lx52_0]
 mov qword ptr [r12 + 648], rax
 jmp xchain34_n15_α
 xchain34_n14_β:
 jmp xchain34_n3_α
.Lx52_0:
 .quad .Lx52_0_s
.Lx52_0_s:
 .string " in "
 xchain34_n15_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 480]
 mov rsi, qword ptr [r12 + 488]
 mov rdx, qword ptr [r12 + 640]
 mov rcx, qword ptr [r12 + 648]
 call str_concat_d@PLT
 mov qword ptr [r12 + 448], rax
 mov qword ptr [r12 + 456], rdx
 jmp xchain34_n16_α
 xchain34_n15_β:
 jmp xchain34_n3_α
# IR_VAR
 xchain34_n16_α:
 mov rax, qword ptr [rbx + 64]
 mov rdx, qword ptr [rbx + 72]
 mov qword ptr [r12 + 672], rax
 mov qword ptr [r12 + 680], rdx
 jmp xchain34_n17_α
 xchain34_n16_β:
 jmp xchain34_n3_α
 xchain34_n17_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 448]
 mov rsi, qword ptr [r12 + 456]
 mov rdx, qword ptr [r12 + 672]
 mov rcx, qword ptr [r12 + 680]
 call str_concat_d@PLT
 mov qword ptr [r12 + 416], rax
 mov qword ptr [r12 + 424], rdx
 jmp xchain34_n18_α
 xchain34_n17_β:
 jmp xchain34_n3_α
# IR_ASSIGN global
 xchain34_n18_α:
 mov rsi, qword ptr [r12 + 416]
 mov rdx, qword ptr [r12 + 424]
 mov rdi, qword ptr [rip + .Lx56_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 400], rax
 mov qword ptr [r12 + 408], rdx
 jmp xchain34_n3_α
 xchain34_n18_β:
 jmp xchain34_n3_α
.Lx56_0:
 .quad .Lx56_0_s
.Lx56_0_s:
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
.S2: .string "WHAT"
.S3: .string " "
.S4: .string " :"
.S5: .string "PAT"
.text
