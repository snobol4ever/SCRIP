  .intel_syntax noprefix
  .text
  .globl proc_PAT$0_α
proc_PAT$0_α:
#=======================================================================================================================
    .global proc_PAT$0_α
    .global proc_PAT$0_β
    .global proc_PAT$0_γ
    .global proc_PAT$0_ω
  sub rsp, 464
  mov [rsp + 440], rcx
  mov [rsp + 448], rdx
  mov [rsp + 456], rbp
  mov rbp, rsp
  mov qword ptr [rsp], 0
  mov qword ptr [rsp + 8], 0
  mov rdi, rsp
  add rdi, 112
  mov ecx, 320
  xor eax, eax
  rep stosb
  mov qword ptr [rsp + 424], rsp
mov qword ptr [rbp + 432], r8
mov dword ptr [rbp + 424], r14d
proc_PAT$0_attempt:
proc_PAT$0_α_body:
lea rax, [rip + xchain0_n0_β]
mov qword ptr [rbp + 400], rax
# IR_MATCH_SEQ_NARY (ZB-FC-3b: zero cell, LIFO-structural)
 xchain0_n0_α:
 jmp xchain0_n1_α
xchain0_n0_as:
 jmp proc_PAT$0_γ
 xchain0_n0_β:
 jmp xchain0_n13_β
xchain0_n0_af:
 jmp proc_PAT$0_ω
# IR_LIT_INTEGER
 xchain0_n1_α:
 mov qword ptr [rbp + 48], 6
 mov rax, qword ptr [rip + .Lx3_0]
 mov qword ptr [rbp + 56], rax
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
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n0_af
# IR_MATCH_CAPTURE_SAVE fc cell
 xchain0_n3_α:
 sub rsp, 16
 mov dword ptr [rsp + 0], r14d
 jmp xchain0_n14_α
 xchain0_n3_β:
 add rsp, 16
 jmp xchain0_n0_af
# IR_MATCH_CAPTURE_COND (pend-park inline pend)
 xchain0_n4_α:
 mov eax, dword ptr [rsp + 0]
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
 jmp xchain0_n14_β
# IR_LIT_INTEGER
 xchain0_n5_α:
 mov qword ptr [rbp + 160], 6
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [rbp + 168], rax
 jmp xchain0_n6_α
 xchain0_n5_β:
 jmp xchain0_n4_β
.Lx9_0:
 .quad 6
# IR_MATCH_TAB
 xchain0_n6_α:
 sub rsp, 16
 mov dword ptr [rsp + 0], r14d
 mov rax, 6
 cmp r14d, eax
 jle .Lx10_239
 add rsp, 16
 jmp xchain0_n4_β
.Lx10_239:
 cmp r15d, eax
 jge .Lx10_240
 add rsp, 16
 jmp xchain0_n4_β
.Lx10_240:
 mov r14d, eax
 jmp xchain0_n7_α
 xchain0_n6_β:
 mov r14d, dword ptr [rsp + 0]
 add rsp, 16
 jmp xchain0_n4_β
# IR_MATCH_CAPTURE_SAVE fc cell
 xchain0_n7_α:
 sub rsp, 16
 mov dword ptr [rsp + 0], r14d
 jmp xchain0_n15_α
 xchain0_n7_β:
 add rsp, 16
 jmp xchain0_n6_β
# IR_MATCH_CAPTURE_COND (pend-park inline pend)
 xchain0_n8_α:
 mov eax, dword ptr [rsp + 16]
 lea rcx, [rip + .S1]
 mov qword ptr [r12 + 0], rcx
 mov esi, eax
 mov qword ptr [r12 + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [r12 + 16], rdx
 add r12, 24
 jmp xchain0_n9_α
 xchain0_n8_β:
 sub r12, 24
 jmp xchain0_n15_β
# IR_MATCH_LIT
 xchain0_n9_α:
 mov eax, r14d
 add eax, 2
 cmp eax, r15d
 jg xchain0_n8_β
 movsxd rcx, r14d
 movzx eax, byte ptr [r13+rcx]
 cmp eax, 32
 jne xchain0_n8_β
 movzx eax, byte ptr [r13+rcx+1]
 cmp eax, 58
 jne xchain0_n8_β
 add r14d, 2
 jmp xchain0_n10_α
 xchain0_n9_β:
 sub r14d, 2
 jmp xchain0_n8_β
# IR_LIT_INTEGER
 xchain0_n10_α:
 mov qword ptr [rbp + 288], 6
 mov rax, qword ptr [rip + .Lx17_0]
 mov qword ptr [rbp + 296], rax
 jmp xchain0_n11_α
 xchain0_n10_β:
 jmp xchain0_n9_β
.Lx17_0:
 .quad 24
# IR_MATCH_TAB
 xchain0_n11_α:
 sub rsp, 16
 mov dword ptr [rsp + 0], r14d
 mov rax, 24
 cmp r14d, eax
 jle .Lx18_239
 add rsp, 16
 jmp xchain0_n9_β
.Lx18_239:
 cmp r15d, eax
 jge .Lx18_240
 add rsp, 16
 jmp xchain0_n9_β
.Lx18_240:
 mov r14d, eax
 jmp xchain0_n12_α
 xchain0_n11_β:
 mov r14d, dword ptr [rsp + 0]
 add rsp, 16
 jmp xchain0_n9_β
# IR_MATCH_CAPTURE_SAVE fc cell
 xchain0_n12_α:
 sub rsp, 16
 mov dword ptr [rsp + 0], r14d
 jmp xchain0_n16_α
 xchain0_n12_β:
 add rsp, 16
 jmp xchain0_n11_β
# IR_MATCH_CAPTURE_COND (pend-park inline pend)
 xchain0_n13_α:
 mov eax, dword ptr [rsp + 16]
 lea rcx, [rip + .S2]
 mov qword ptr [r12 + 0], rcx
 mov esi, eax
 mov qword ptr [r12 + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [r12 + 16], rdx
 add r12, 24
 jmp xchain0_n0_as
 xchain0_n13_β:
 sub r12, 24
 jmp xchain0_n16_β
# IR_MATCH_LEN
 xchain0_n14_α:
 mov eax, r14d
 add eax, 4
 cmp eax, r15d
 jle .Lx23_240
 add rsp, 16
 jmp xchain0_n0_af
.Lx23_240:
 add r14d, 4
 jmp xchain0_n4_α
 xchain0_n14_β:
 sub r14d, 4
 add rsp, 16
 jmp xchain0_n0_af
# IR_MATCH_ARB
 xchain0_n15_α:
 sub rsp, 16
 mov dword ptr [rsp + 0], 0
 mov eax, r14d
 mov dword ptr [rsp + 4], eax
 jmp xchain0_n8_α
 xchain0_n15_β:
 add dword ptr [rsp + 0], 1
 mov eax, dword ptr [rsp + 4]
 add eax, dword ptr [rsp + 0]
 cmp eax, r15d
 jg .Lx25_0
 mov r14d, eax
 jmp xchain0_n8_α
.Lx25_0:
 mov r14d, dword ptr [rsp + 4]
 add rsp, 16
 add rsp, 16
 jmp xchain0_n6_β
# IR_MATCH_REM
 xchain0_n16_α:
 sub rsp, 16
 mov dword ptr [rsp + 0], r14d
 mov r14d, r15d
 jmp xchain0_n13_α
 xchain0_n16_β:
 mov r14d, dword ptr [rsp + 0]
 add rsp, 16
 add rsp, 16
 jmp xchain0_n11_β
proc_PAT$0_scanhit:
cmp qword ptr [rbp + 432], 1
jne 7f
mov ecx, dword ptr [rbp + 424]
lea rdx, [rip + g_scan_hit_start]
mov dword ptr [rdx], ecx
7:
jmp proc_PAT$0_γ
proc_PAT$0_scanfail:
cmp qword ptr [rbp + 432], 1
jne 8f
mov eax, dword ptr [rbp + 424]
inc eax
cmp eax, r15d
jg 8f
lea rcx, [rip + g_anchor]
cmp qword ptr [rcx], 0
jne 8f
mov dword ptr [rbp + 424], eax
mov r14d, eax
mov rsp, rbp
jmp proc_PAT$0_attempt
8:
jmp proc_PAT$0_ω
proc_PAT$0_res:
add rsp, 8
pop rbp
proc_PAT$0_β:
jmp qword ptr [rbp + 400]
proc_PAT$0_γ:
push rbp
lea rax, [rip + proc_PAT$0_res]
push rax
mov rax, [rbp + 440]
mov rbp, [rbp + 456]
jmp rax
proc_PAT$0_ω:
mov rax, [rbp + 448]
lea rsp, [rbp + 464]
mov rbp, [rbp + 456]
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
  mov esi, 432
  call rt_proc_set_frame_bytes@PLT
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
# IR_LIT_STRING
 xchain27_n0_α:
 mov qword ptr [rbp + 160], 1
 mov rax, qword ptr [rip + .Lx28_0]
 mov qword ptr [rbp + 168], rax
 jmp xchain27_n1_α
.Lx28_0:
 .quad .Lx28_0_s
.Lx28_0_s:
 .string "PAT$0"
 xchain27_n1_α:
# BOX IR_CALL SNO$MKPAT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+160] -> [zr+128]
 mov rax, qword ptr [rbp + 160]
 mov qword ptr [rbp + 128], rax
 mov rax, qword ptr [rbp + 168]
 mov qword ptr [rbp + 136], rax
  .section .rodata
  .Lrkfn30: .string "SNO$MKPAT"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn30]
 lea rsi, [rbp + 128]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rbp + 112], rax
 mov qword ptr [rbp + 120], rdx
 cmp eax, 99
 je xchain27_n3_α
 jmp xchain27_n2_α
 xchain27_n1_β:
 jmp xchain27_n3_α
# IR_ASSIGN gva
 xchain27_n2_α:
 mov rax, qword ptr [rbp + 112]
 mov rdx, qword ptr [rbp + 120]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rbp + 96], rax
 mov qword ptr [rbp + 104], rdx
 jmp xchain27_n3_α
# IR_VAR
 xchain27_n3_α:
 mov rdi, qword ptr [rip + .Lx32_0]
 call NV_GET_fn@PLT
 cmp eax, 99
 je main_γ
 mov qword ptr [rbp + 208], rax
 mov qword ptr [rbp + 216], rdx
 jmp xchain27_n4_α
.Lx32_0:
 .quad .Lx32_0_s
.Lx32_0_s:
 .string "INPUT"
# IR_ASSIGN gva
 xchain27_n4_α:
 mov rax, qword ptr [rbp + 208]
 mov rdx, qword ptr [rbp + 216]
 mov qword ptr [1879052352], rax
 mov qword ptr [1879052360], rdx
 mov qword ptr [rbp + 192], rax
 mov qword ptr [rbp + 200], rdx
 jmp xchain27_n5_α
# IR_VAR
 xchain27_n5_α:
 mov rax, qword ptr [1879052352]
 mov rdx, qword ptr [1879052360]
 mov qword ptr [rbp + 352], rax
 mov qword ptr [rbp + 360], rdx
 jmp xchain27_n6_α
# IR_MATCH_HEAD
 xchain27_n6_α:
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
.Lx36_0:
 mov r14d, dword ptr [rbp + 256]
 jmp xchain27_n7_α
 xchain27_n6_β:
 add dword ptr [rbp + 256], 1
 mov eax, dword ptr [rbp + 256]
 cmp eax, r15d
 jg .Lx36_1
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne .Lx36_1
 jmp .Lx36_0
.Lx36_1:
 mov rax, qword ptr [rbp + 264]
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rax
 mov rsp, qword ptr [rbp + 272]
 mov r12, qword ptr [rbp + 288]
 mov rbp, qword ptr [rbp + 296]
 jmp xchain27_n3_α
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain27_n7_α:
 lea rsi, [rip + g_sno_defer_cells+0]
 mov rax, qword ptr [rsi + 0]
 test rax, rax
 jne .Lx37_11
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 cmp eax, 3
 jne .Lx37_9
 mov rax, qword ptr [rdx + 0]
 test rax, rax
 jne .Lx37_10
 mov rdi, rdx
 call dtp_fn_of@PLT
 jmp .Lx37_10
.Lx37_9:
 xor eax, eax
.Lx37_10:
 lea rsi, [rip + g_sno_defer_cells+0]
 mov qword ptr [rsi + 0], rax
.Lx37_11:
 test rax, rax
 jz .Lx37_0
 mov r8d, 1
 lea rcx, [rip + .Lx37_4]
 lea rdx, [rip + .Lx37_5]
 jmp rax
.Lx37_4:
 lea rcx, [rip + g_scan_hit_start]
 mov rax, qword ptr [rcx]
 mov dword ptr [rbp + 256], eax
 jmp xchain27_n8_α
.Lx37_5:
 jmp xchain27_n6_β
.Lx37_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S3]
 xor esi, esi
 call rt_defer_open@PLT
.Lx37_2:
 test rax, rax
 je .Lx37_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx37_7]
 lea rdx, [rip + .Lx37_8]
 jmp rax
.Lx37_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx37_2
.Lx37_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx37_2
.Lx37_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain27_n6_β
 mov r14d, eax
 lea rax, [rip + .Lx37_6]
 sub rsp, 8
 push rax
 jmp xchain27_n8_α
.Lx37_6:
 add rsp, 16
 jmp xchain27_n6_β
 xchain27_n7_β:
 jmp qword ptr [rsp]
# IR_MATCH_RELEASE
 xchain27_n8_α:
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
.Lx39_1:
 test rax, rax
 je .Lx39_2
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx39_3]
 lea rdx, [rip + .Lx39_4]
 jmp rax
.Lx39_3:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx39_1
.Lx39_4:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx39_1
.Lx39_2:
 call rt_dcap_end_ok_close@PLT
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov r12, qword ptr [rbp + 288]
 mov rbp, qword ptr [rbp + 296]
 jmp xchain27_n9_α
# IR_VAR
 xchain27_n9_α:
 mov rax, qword ptr [1879052320]
 mov rdx, qword ptr [1879052328]
 mov qword ptr [rbp + 528], rax
 mov qword ptr [rbp + 536], rdx
 jmp xchain27_n10_α
# IR_LIT_STRING
 xchain27_n10_α:
 mov qword ptr [rbp + 560], 1
 mov rax, qword ptr [rip + .Lx41_0]
 mov qword ptr [rbp + 568], rax
 jmp xchain27_n11_α
.Lx41_0:
 .quad .Lx41_0_s
.Lx41_0_s:
 .string " invented the "
 xchain27_n11_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rbp + 528]
 mov rsi, qword ptr [rbp + 536]
 mov rdx, qword ptr [rbp + 560]
 mov rcx, qword ptr [rbp + 568]
 call str_concat_d@PLT
 mov qword ptr [rbp + 496], rax
 mov qword ptr [rbp + 504], rdx
 jmp xchain27_n12_α
# IR_VAR
 xchain27_n12_α:
 mov rax, qword ptr [1879052336]
 mov rdx, qword ptr [1879052344]
 mov qword ptr [rbp + 592], rax
 mov qword ptr [rbp + 600], rdx
 jmp xchain27_n13_α
 xchain27_n13_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rbp + 496]
 mov rsi, qword ptr [rbp + 504]
 mov rdx, qword ptr [rbp + 592]
 mov rcx, qword ptr [rbp + 600]
 call str_concat_d@PLT
 mov qword ptr [rbp + 464], rax
 mov qword ptr [rbp + 472], rdx
 jmp xchain27_n14_α
# IR_LIT_STRING
 xchain27_n14_α:
 mov qword ptr [rbp + 624], 1
 mov rax, qword ptr [rip + .Lx45_0]
 mov qword ptr [rbp + 632], rax
 jmp xchain27_n15_α
.Lx45_0:
 .quad .Lx45_0_s
.Lx45_0_s:
 .string " in "
 xchain27_n15_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rbp + 464]
 mov rsi, qword ptr [rbp + 472]
 mov rdx, qword ptr [rbp + 624]
 mov rcx, qword ptr [rbp + 632]
 call str_concat_d@PLT
 mov qword ptr [rbp + 432], rax
 mov qword ptr [rbp + 440], rdx
 jmp xchain27_n16_α
# IR_VAR
 xchain27_n16_α:
 mov rax, qword ptr [1879052304]
 mov rdx, qword ptr [1879052312]
 mov qword ptr [rbp + 656], rax
 mov qword ptr [rbp + 664], rdx
 jmp xchain27_n17_α
 xchain27_n17_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rbp + 432]
 mov rsi, qword ptr [rbp + 440]
 mov rdx, qword ptr [rbp + 656]
 mov rcx, qword ptr [rbp + 664]
 call str_concat_d@PLT
 mov qword ptr [rbp + 400], rax
 mov qword ptr [rbp + 408], rdx
 jmp xchain27_n18_α
# IR_ASSIGN global
 xchain27_n18_α:
 mov rsi, qword ptr [rbp + 400]
 mov rdx, qword ptr [rbp + 408]
 mov rdi, qword ptr [rip + .Lx49_0]
 call NV_SET_fn@PLT
 mov qword ptr [rbp + 384], rax
 mov qword ptr [rbp + 392], rdx
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
