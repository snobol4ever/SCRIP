  .intel_syntax noprefix
  .text
  .globl proc_PAT$0_α
proc_PAT$0_α:
#=======================================================================================================================
    .global proc_PAT$0_α
    .global proc_PAT$0_β
    .global proc_PAT$0_γ
    .global proc_PAT$0_ω
  sub rsp, 448
  mov [rsp + 424], rcx
  mov [rsp + 432], rdx
  mov [rsp + 440], rbp
  mov rbp, rsp
  mov qword ptr [rsp], 0
  mov qword ptr [rsp + 8], 0
  mov qword ptr [rsp + 112], 0
  mov qword ptr [rsp + 120], 0
  mov qword ptr [rsp + 224], 0
  mov qword ptr [rsp + 232], 0
  mov qword ptr [rsp + 336], 0
  mov qword ptr [rsp + 344], 0
  mov qword ptr [rsp + 384], 0
  mov qword ptr [rsp + 392], 0
  mov qword ptr [rsp + 400], 0
  mov qword ptr [rsp + 408], rsp
mov qword ptr [rbp + 416], r8
mov dword ptr [rbp + 408], r14d
proc_PAT$0_attempt:
proc_PAT$0_α_body:
lea rax, [rip + xchain0_n0_β]
mov qword ptr [rbp + 384], rax
# IR_MATCH_SEQ_NARY (ZB-FC-3b: zero cell, LIFO-structural)
 xchain0_n0_α:
 jmp xchain0_n1_α
xchain0_n0_as:
 jmp proc_PAT$0_γ
 xchain0_n0_β:
 jmp xchain0_n10_β
xchain0_n0_af:
 jmp proc_PAT$0_ω
# IR_LIT_INTEGER
 xchain0_n1_α:
 mov qword ptr [rbp + 48], 6
 mov rax, qword ptr [rip + .Lx3_0]
 mov qword ptr [rbp + 56], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp proc_PAT$0_ω
.Lx3_0:
 .quad 0
# IR_MATCH_POS
 xchain0_n2_α:
 mov rax, 0
 cmp r14d, eax
 jne proc_PAT$0_ω
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp proc_PAT$0_ω
# IR_MATCH_CAPTURE_SAVE fc cell
 xchain0_n3_α:
 sub rsp, 16
 mov dword ptr [rsp + 0], r14d
 jmp xchain0_n11_α
 xchain0_n3_β:
 add rsp, 16
 jmp proc_PAT$0_ω
# IR_MATCH_CAPTURE_COND (pend-park inline pend)
 xchain0_n4_α:
 mov eax, dword ptr [rsp + 16]
 lea rcx, [rip + .S0]
 mov qword ptr [r12 + 0], rcx
 mov esi, eax
 mov qword ptr [r12 + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [r12 + 16], rdx
 add r12, 24
 jmp xchain0_n5_α
 xchain0_n4_β:
 sub r12, 24
 jmp xchain0_n11_β
# IR_MATCH_SEQ_NARY (ZB-FC-3b: zero cell, LIFO-structural)
 xchain0_n5_α:
 jmp xchain0_n12_α
xchain0_n5_as:
 jmp xchain0_n6_α
 xchain0_n5_β:
 jmp xchain0_n13_β
xchain0_n5_af:
 jmp xchain0_n4_β
# IR_MATCH_CAPTURE_SAVE fc cell
 xchain0_n6_α:
 sub rsp, 16
 mov dword ptr [rsp + 0], r14d
 jmp xchain0_n14_α
 xchain0_n6_β:
 add rsp, 16
 jmp xchain0_n5_β
# IR_MATCH_CAPTURE_COND (pend-park inline pend)
 xchain0_n7_α:
 mov eax, dword ptr [rsp + 16]
 lea rcx, [rip + .S1]
 mov qword ptr [r12 + 0], rcx
 mov esi, eax
 mov qword ptr [r12 + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [r12 + 16], rdx
 add r12, 24
 jmp xchain0_n8_α
 xchain0_n7_β:
 sub r12, 24
 jmp xchain0_n14_β
# IR_MATCH_SEQ_NARY (ZB-FC-3b: zero cell, LIFO-structural)
 xchain0_n8_α:
 jmp xchain0_n15_α
xchain0_n8_as:
 jmp xchain0_n9_α
 xchain0_n8_β:
 jmp xchain0_n16_β
xchain0_n8_af:
 jmp xchain0_n7_β
# IR_MATCH_CAPTURE_SAVE fc cell
 xchain0_n9_α:
 sub rsp, 16
 mov dword ptr [rsp + 0], r14d
 jmp xchain0_n17_α
 xchain0_n9_β:
 add rsp, 16
 jmp xchain0_n8_β
# IR_MATCH_CAPTURE_COND (pend-park inline pend)
 xchain0_n10_α:
 mov eax, dword ptr [rsp + 16]
 lea rcx, [rip + .S2]
 mov qword ptr [r12 + 0], rcx
 mov esi, eax
 mov qword ptr [r12 + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [r12 + 16], rdx
 add r12, 24
 jmp proc_PAT$0_γ
 xchain0_n10_β:
 sub r12, 24
 jmp xchain0_n17_β
# IR_MATCH_BREAK
 xchain0_n11_α:
 sub rsp, 16
 movsxd rcx, r14d
.Lx22_0:
 cmp ecx, r15d
 jl .Lx22_237
 add rsp, 16
 add rsp, 16
 jmp proc_PAT$0_ω
.Lx22_237:
 movzx esi, byte ptr [r13+rcx]
 cmp esi, 32
 je .Lx22_1
 add ecx, 1
 cmp ecx, r15d
 jl .Lx22_238
 add rsp, 16
 add rsp, 16
 jmp proc_PAT$0_ω
.Lx22_238:
 movzx esi, byte ptr [r13+rcx]
 cmp esi, 32
 je .Lx22_1
 add ecx, 1
 cmp ecx, r15d
 jl .Lx22_239
 add rsp, 16
 add rsp, 16
 jmp proc_PAT$0_ω
.Lx22_239:
 movzx esi, byte ptr [r13+rcx]
 cmp esi, 32
 je .Lx22_1
 add ecx, 1
 cmp ecx, r15d
 jl .Lx22_240
 add rsp, 16
 add rsp, 16
 jmp proc_PAT$0_ω
.Lx22_240:
 movzx esi, byte ptr [r13+rcx]
 cmp esi, 32
 je .Lx22_1
 add ecx, 1
 jmp .Lx22_0
.Lx22_1:
 mov dword ptr [rsp + 0], r14d
 mov r14d, ecx
 jmp xchain0_n4_α
 xchain0_n11_β:
 mov r14d, dword ptr [rsp + 0]
 add rsp, 16
 add rsp, 16
 jmp proc_PAT$0_ω
# IR_MATCH_LIT
 xchain0_n12_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain0_n4_β
 movsxd rcx, r14d
 movzx eax, byte ptr [r13+rcx]
 cmp eax, 32
 jne xchain0_n4_β
 add r14d, 1
 jmp xchain0_n13_α
 xchain0_n12_β:
 sub r14d, 1
 jmp xchain0_n4_β
# IR_MATCH_SPAN
 xchain0_n13_α:
 sub rsp, 16
 movsxd rcx, r14d
.Lx26_0:
 cmp ecx, r15d
 jge .Lx26_1
 movzx esi, byte ptr [r13+rcx]
 cmp esi, 32
 je .Lx26_10
 jmp .Lx26_1
.Lx26_10:
 add ecx, 1
 cmp ecx, r15d
 jge .Lx26_1
 movzx esi, byte ptr [r13+rcx]
 cmp esi, 32
 je .Lx26_11
 jmp .Lx26_1
.Lx26_11:
 add ecx, 1
 cmp ecx, r15d
 jge .Lx26_1
 movzx esi, byte ptr [r13+rcx]
 cmp esi, 32
 je .Lx26_12
 jmp .Lx26_1
.Lx26_12:
 add ecx, 1
 cmp ecx, r15d
 jge .Lx26_1
 movzx esi, byte ptr [r13+rcx]
 cmp esi, 32
 je .Lx26_13
 jmp .Lx26_1
.Lx26_13:
 add ecx, 1
 jmp .Lx26_0
.Lx26_1:
 cmp ecx, r14d
 jg .Lx26_240
 add rsp, 16
 jmp xchain0_n12_β
.Lx26_240:
 mov dword ptr [rsp + 4], r14d
 mov r14d, ecx
 jmp xchain0_n6_α
 xchain0_n13_β:
 xchain0_n13_β:
 mov r14d, dword ptr [rsp + 4]
 add rsp, 16
 jmp xchain0_n12_β
# IR_MATCH_ARB
 xchain0_n14_α:
 sub rsp, 16
 mov dword ptr [rsp + 0], 0
 mov eax, r14d
 mov dword ptr [rsp + 4], eax
 jmp xchain0_n7_α
 xchain0_n14_β:
 add dword ptr [rsp + 0], 1
 mov eax, dword ptr [rsp + 4]
 add eax, dword ptr [rsp + 0]
 cmp eax, r15d
 jg .Lx28_0
 mov r14d, eax
 jmp xchain0_n7_α
.Lx28_0:
 mov r14d, dword ptr [rsp + 4]
 add rsp, 16
 add rsp, 16
 jmp xchain0_n5_β
# IR_MATCH_LIT
 xchain0_n15_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain0_n7_β
 movsxd rcx, r14d
 movzx eax, byte ptr [r13+rcx]
 cmp eax, 32
 jne xchain0_n7_β
 add r14d, 1
 jmp xchain0_n16_α
 xchain0_n15_β:
 sub r14d, 1
 jmp xchain0_n7_β
# IR_MATCH_SPAN
 xchain0_n16_α:
 sub rsp, 16
 movsxd rcx, r14d
.Lx32_0:
 cmp ecx, r15d
 jge .Lx32_1
 movzx esi, byte ptr [r13+rcx]
 cmp esi, 32
 je .Lx32_10
 cmp esi, 58
 je .Lx32_10
 jmp .Lx32_1
.Lx32_10:
 add ecx, 1
 cmp ecx, r15d
 jge .Lx32_1
 movzx esi, byte ptr [r13+rcx]
 cmp esi, 32
 je .Lx32_11
 cmp esi, 58
 je .Lx32_11
 jmp .Lx32_1
.Lx32_11:
 add ecx, 1
 cmp ecx, r15d
 jge .Lx32_1
 movzx esi, byte ptr [r13+rcx]
 cmp esi, 32
 je .Lx32_12
 cmp esi, 58
 je .Lx32_12
 jmp .Lx32_1
.Lx32_12:
 add ecx, 1
 cmp ecx, r15d
 jge .Lx32_1
 movzx esi, byte ptr [r13+rcx]
 cmp esi, 32
 je .Lx32_13
 cmp esi, 58
 je .Lx32_13
 jmp .Lx32_1
.Lx32_13:
 add ecx, 1
 jmp .Lx32_0
.Lx32_1:
 cmp ecx, r14d
 jg .Lx32_240
 add rsp, 16
 jmp xchain0_n15_β
.Lx32_240:
 mov dword ptr [rsp + 4], r14d
 mov r14d, ecx
 jmp xchain0_n9_α
 xchain0_n16_β:
 xchain0_n16_β:
 mov r14d, dword ptr [rsp + 4]
 add rsp, 16
 jmp xchain0_n15_β
# IR_MATCH_REM
 xchain0_n17_α:
 sub rsp, 16
 mov dword ptr [rsp + 0], r14d
 mov r14d, r15d
 jmp xchain0_n10_α
 xchain0_n17_β:
 mov r14d, dword ptr [rsp + 0]
 add rsp, 16
 add rsp, 16
 jmp xchain0_n8_β
proc_PAT$0_scanhit:
cmp qword ptr [rbp + 416], 1
jne 7f
mov ecx, dword ptr [rbp + 408]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$0_γ
proc_PAT$0_scanfail:
cmp qword ptr [rbp + 416], 1
jne 8f
mov eax, dword ptr [rbp + 408]
inc eax
cmp eax, r15d
jg 8f
lea rcx, [rip + g_anchor]
cmp qword ptr [rcx], 0
jne 8f
mov dword ptr [rbp + 408], eax
mov r14d, eax
mov rsp, rbp
jmp proc_PAT$0_attempt
8:
jmp proc_PAT$0_ω
proc_PAT$0_res:
add rsp, 8
pop rbp
proc_PAT$0_β:
jmp qword ptr [rbp + 384]
proc_PAT$0_γ:
push rbp
lea rax, [rip + proc_PAT$0_res]
push rax
mov rax, [rbp + 424]
mov rbp, [rbp + 440]
jmp rax
proc_PAT$0_ω:
mov rax, [rbp + 432]
lea rsp, [rbp + 448]
mov rbp, [rbp + 440]
jmp rax
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
  mov esi, 416
  call rt_proc_set_frame_bytes@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 1
  call rt_proc_set_zstatic@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 1
  call rt_proc_set_jmpentry@PLT
  add rsp, 8
  ret
  .section .rodata
  .Lgvan0: .string "PAT"
  .Lgvan1: .string "WHEN"
  .Lgvan2: .string "WHO"
  .Lgvan3: .string "WHAT"
  .Lgvan4: .string "LINE"
  .align 8
__gva_names:
  .quad .Lgvan0
  .quad .Lgvan1
  .quad .Lgvan2
  .quad .Lgvan3
  .quad .Lgvan4
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
  call proc_startup
  mov edi, 5
  call rt_gva_island@PLT
  mov rsi, rax
  lea rdi, [rip + __gva_names]
  mov edx, 5
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
  mov qword ptr [rsp + 696], rsp
  mov r12, qword ptr [1879048192]
  mov [rsp + 65536], rbp
  mov rbp, rsp
main_α_body:
#       PAT      =  POS(0) BREAK(' ') . WHEN (' ' SPAN(' '))
# IR_LIT_STRING
 xchain34_n0_α:
 mov qword ptr [rbp + 160], 1
 mov rax, qword ptr [rip + .Lx35_0]
 mov qword ptr [rbp + 168], rax
 jmp xchain34_n1_α
.Lx35_0:
 .quad .Lx35_0_s
.Lx35_0_s:
 .string "PAT$0"
 xchain34_n1_α:
# BOX IR_CALL SNO$MKPAT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+160] -> [zr+128]
 mov rax, qword ptr [rbp + 160]
 mov qword ptr [rbp + 128], rax
 mov rax, qword ptr [rbp + 168]
 mov qword ptr [rbp + 136], rax
  .section .rodata
  .Lrkfn37: .string "SNO$MKPAT"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn37]
 lea rsi, [rbp + 128]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 112], rax
 mov qword ptr [rbp + 120], rdx
 cmp eax, 99
 je xchain34_n3_α
 jmp xchain34_n2_α
 xchain34_n1_β:
 jmp xchain34_n3_α
# IR_ASSIGN gva
 xchain34_n2_α:
 mov rax, qword ptr [rbp + 112]
 mov rdx, qword ptr [rbp + 120]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 96], rax
 mov qword ptr [rbp + 104], rdx
 jmp xchain34_n3_α
# LOOP  LINE     =  INPUT                            :F(END)
# IR_VAR
 xchain34_n3_α:
 mov rdi, qword ptr [rip + .Lx39_0]
 call NV_GET_fn@PLT
 cmp eax, 99
 je main_γ
 mov qword ptr [rbp + 208], rax
 mov qword ptr [rbp + 216], rdx
 jmp xchain34_n4_α
.Lx39_0:
 .quad .Lx39_0_s
.Lx39_0_s:
 .string "INPUT"
# IR_ASSIGN gva
 xchain34_n4_α:
 mov rax, qword ptr [rbp + 208]
 mov rdx, qword ptr [rbp + 216]
 mov qword ptr [1879052352], rax
 mov qword ptr [1879052360], rdx
 mov qword ptr [rbp + 192], rax
 mov qword ptr [rbp + 200], rdx
 jmp xchain34_n5_α
#       LINE     ?  PAT                              :F(LOOP)
# IR_VAR
 xchain34_n5_α:
 mov rax, qword ptr [1879052352]
 mov rdx, qword ptr [1879052360]
 mov qword ptr [rbp + 352], rax
 mov qword ptr [rbp + 360], rdx
 jmp xchain34_n6_α
# IR_MATCH_HEAD
 xchain34_n6_α:
 mov qword ptr [rbp + 296], rbp
 mov rdi, qword ptr [rbp + 352]
 mov rsi, qword ptr [rbp + 360]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov qword ptr [rbp + 288], r12
 mov qword ptr [rbp + 272], rsp
 lea rcx, [rip + g_patstk_sp]
 mov rax, qword ptr [rcx + 0]
 mov qword ptr [rbp + 264], rax
 mov dword ptr [rbp + 256], 0
.Lx43_0:
 mov r14d, dword ptr [rbp + 256]
 jmp xchain34_n7_α
 xchain34_n6_β:
 add dword ptr [rbp + 256], 1
 mov eax, dword ptr [rbp + 256]
 cmp eax, r15d
 jg .Lx43_1
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne .Lx43_1
 jmp .Lx43_0
.Lx43_1:
 mov rax, qword ptr [rbp + 264]
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rax
 mov rsp, qword ptr [rbp + 272]
 mov r12, qword ptr [rbp + 288]
 mov rbp, qword ptr [rbp + 296]
 jmp xchain34_n3_α
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain34_n7_α:
 lea rsi, [rip + g_sno_defer_cells+0]
 mov rax, qword ptr [rsi + 0]
 test rax, rax
 jne .Lx44_11
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 cmp eax, 3
 jne .Lx44_9
 mov rax, qword ptr [rdx + 0]
 test rax, rax
 jne .Lx44_10
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx44_10
.Lx44_9:
 xor eax, eax
.Lx44_10:
 lea rsi, [rip + g_sno_defer_cells+0]
 mov qword ptr [rsi + 0], rax
.Lx44_11:
 test rax, rax
 jz .Lx44_0
 mov r8d, 1
 lea rcx, [rip + .Lx44_4]
 lea rdx, [rip + .Lx44_5]
 jmp rax
.Lx44_4:
 lea rcx, [rip + g_scan_hit_start]
 mov rax, qword ptr [rcx]
 mov dword ptr [rbp + 256], eax
 jmp xchain34_n8_α
.Lx44_5:
 jmp xchain34_n6_β
.Lx44_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S3]
 xor esi, esi
 call rt_defer_open@PLT
.Lx44_2:
 test rax, rax
 je .Lx44_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx44_7]
 lea rdx, [rip + .Lx44_8]
 jmp rax
.Lx44_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx44_2
.Lx44_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx44_2
.Lx44_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain34_n6_β
 mov r14d, eax
 lea rax, [rip + .Lx44_6]
 sub rsp, 8
 push rax
 jmp xchain34_n8_α
.Lx44_6:
 add rsp, 16
 jmp xchain34_n6_β
 xchain34_n7_β:
 jmp qword ptr [rsp]
# IR_MATCH_RELEASE
 xchain34_n8_α:
 mov rax, qword ptr [rbp + 264]
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rax
 mov rsp, qword ptr [rbp + 272]
 push r14
 push r15
 push r13
 sub rsp, 8
 mov rdi, qword ptr [rsp + 320]
 mov rsi, r12
 mov rdx, r13
 call rt_dcap_end_ok_open@PLT
.Lx46_1:
 test rax, rax
 je .Lx46_2
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx46_3]
 lea rdx, [rip + .Lx46_4]
 jmp rax
.Lx46_3:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx46_1
.Lx46_4:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx46_1
.Lx46_2:
 call rt_dcap_end_ok_close@PLT
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov r12, qword ptr [rbp + 288]
 mov rbp, qword ptr [rbp + 296]
 jmp xchain34_n9_α
#       OUTPUT   =  WHO " invented the " WHAT " in " WHEN  :(LOOP)
# IR_VAR
 xchain34_n9_α:
 mov rax, qword ptr [1879052320]
 mov rdx, qword ptr [1879052328]
 mov qword ptr [rbp + 528], rax
 mov qword ptr [rbp + 536], rdx
 jmp xchain34_n10_α
# IR_LIT_STRING
 xchain34_n10_α:
 mov qword ptr [rbp + 560], 1
 mov rax, qword ptr [rip + .Lx48_0]
 mov qword ptr [rbp + 568], rax
 jmp xchain34_n11_α
.Lx48_0:
 .quad .Lx48_0_s
.Lx48_0_s:
 .string " invented the "
 xchain34_n11_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rbp + 528]
 mov rsi, qword ptr [rbp + 536]
 mov rdx, qword ptr [rbp + 560]
 mov rcx, qword ptr [rbp + 568]
 call str_concat_d@PLT
 mov qword ptr [rbp + 496], rax
 mov qword ptr [rbp + 504], rdx
 jmp xchain34_n12_α
# IR_VAR
 xchain34_n12_α:
 mov rax, qword ptr [1879052336]
 mov rdx, qword ptr [1879052344]
 mov qword ptr [rbp + 592], rax
 mov qword ptr [rbp + 600], rdx
 jmp xchain34_n13_α
 xchain34_n13_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rbp + 496]
 mov rsi, qword ptr [rbp + 504]
 mov rdx, qword ptr [rbp + 592]
 mov rcx, qword ptr [rbp + 600]
 call str_concat_d@PLT
 mov qword ptr [rbp + 464], rax
 mov qword ptr [rbp + 472], rdx
 jmp xchain34_n14_α
# IR_LIT_STRING
 xchain34_n14_α:
 mov qword ptr [rbp + 624], 1
 mov rax, qword ptr [rip + .Lx52_0]
 mov qword ptr [rbp + 632], rax
 jmp xchain34_n15_α
.Lx52_0:
 .quad .Lx52_0_s
.Lx52_0_s:
 .string " in "
 xchain34_n15_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rbp + 464]
 mov rsi, qword ptr [rbp + 472]
 mov rdx, qword ptr [rbp + 624]
 mov rcx, qword ptr [rbp + 632]
 call str_concat_d@PLT
 mov qword ptr [rbp + 432], rax
 mov qword ptr [rbp + 440], rdx
 jmp xchain34_n16_α
# IR_VAR
 xchain34_n16_α:
 mov rax, qword ptr [1879052304]
 mov rdx, qword ptr [1879052312]
 mov qword ptr [rbp + 656], rax
 mov qword ptr [rbp + 664], rdx
 jmp xchain34_n17_α
 xchain34_n17_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rbp + 432]
 mov rsi, qword ptr [rbp + 440]
 mov rdx, qword ptr [rbp + 656]
 mov rcx, qword ptr [rbp + 664]
 call str_concat_d@PLT
 mov qword ptr [rbp + 400], rax
 mov qword ptr [rbp + 408], rdx
 jmp xchain34_n18_α
# IR_ASSIGN global
 xchain34_n18_α:
 mov rsi, qword ptr [rbp + 400]
 mov rdx, qword ptr [rbp + 408]
 mov rdi, qword ptr [rip + .Lx56_0]
 call NV_SET_fn@PLT
 mov qword ptr [rbp + 384], rax
 mov qword ptr [rbp + 392], rdx
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
mov rsp, qword ptr [rbp + 696]
mov rbp, [rsp + 65536]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rbp + 696]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
mov rbp, [rsp + 65536]
add rsp, 65544
ret
.section .rodata
.S0: .string "WHEN"
.S1: .string "WHO"
.S2: .string "WHAT"
.S3: .string "PAT"
.text
