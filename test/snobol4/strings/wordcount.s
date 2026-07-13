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
  push rbp
  mov rbp, rsp
  sub rsp, 8
  cmp esi, 0
  jne proc_PAT$0_β
 push rsi
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, [rsp + 8]
 mov qword ptr [r12 + 128], rax
 pop rsi
proc_PAT$0_α_body:
lea rax, [rip + xchain0_n0_β]
mov qword ptr [r12 + 112], rax
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
 je xchain0_n2_α
 jmp proc_PAT$0_γ
 xchain0_n0_β:
 mov dword ptr [r12 + 36], 2
xchain0_n0_af:
 mov eax, dword ptr [r12 + 36]
 sub eax, 1
 mov dword ptr [r12 + 36], eax
 cmp eax, 0
 je xchain0_n1_β
 cmp eax, 1
 je xchain0_n2_β
 jmp proc_PAT$0_ω
# IR_MATCH_BREAK
 xchain0_n1_α:
 mov dword ptr [r12 + 64], 0
.Lx4_0:
 mov eax, r14d
 add eax, dword ptr [r12 + 64]
 cmp eax, r15d
 jge xchain0_n0_af
 movsxd rcx, eax
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S0]
 call strchr@PLT
 test rax, rax
 jnz .Lx4_1
 add dword ptr [r12 + 64], 1
 jmp .Lx4_0
.Lx4_1:
 mov eax, r14d
 add eax, dword ptr [r12 + 64]
 mov r14d, eax
 jmp xchain0_n0_as
 xchain0_n1_β:
 mov eax, r14d
 sub eax, dword ptr [r12 + 64]
 mov r14d, eax
 jmp xchain0_n0_af
# IR_MATCH_SPAN
 xchain0_n2_α:
 mov dword ptr [r12 + 96], 0
.Lx6_0:
 mov eax, r14d
 add eax, dword ptr [r12 + 96]
 cmp eax, r15d
 jge .Lx6_1
 movsxd rcx, eax
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S0]
 call strchr@PLT
 test rax, rax
 je .Lx6_1
 add dword ptr [r12 + 96], 1
 jmp .Lx6_0
.Lx6_1:
 mov eax, dword ptr [r12 + 96]
 test eax, eax
 jle xchain0_n0_af
 mov edx, r14d
 mov dword ptr [r12 + 100], edx
 add edx, eax
 mov r14d, edx
 jmp xchain0_n0_as
 xchain0_n2_β:
 xchain0_n2_β:
 mov r14d, dword ptr [r12 + 100]
 jmp xchain0_n0_af
proc_PAT$0_β:
jmp qword ptr [r12 + 112]
proc_PAT$0_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [r12 + 128]
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
proc_PAT$0_ω:
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
  mov esi, 144
  call rt_proc_set_frame_bytes@PLT
  pop rbp
  ret
  .section .rodata
  .Lgvan0: .string "NUMERALS"
  .Lgvan1: .string "WORD"
  .Lgvan2: .string "WPAT"
  .Lgvan3: .string "LINE"
  .Lgvan4: .string "N"
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
  push rbp
  mov rbp, rsp
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
 mov qword ptr [r12 + 1072], rax
 pop rsi
main_α_body:
# IR_LIT_STRING
 xchain7_n0_α:
 mov qword ptr [r12 + 160], 1
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [r12 + 168], rax
 jmp xchain7_n1_α
 xchain7_n0_β:
 jmp xchain7_n3_α
.Lx8_0:
 .quad .Lx8_0_s
.Lx8_0_s:
 .string "TRIM"
# IR_LIT_INTEGER
 xchain7_n1_α:
 mov qword ptr [r12 + 192], 6
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [r12 + 200], rax
 jmp xchain7_n2_α
 xchain7_n1_β:
 jmp xchain7_n3_α
.Lx9_0:
 .quad 1
 xchain7_n2_α:
# BOX IR_CALL SNO$KWSET(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+160] -> [zr+112]
 mov rax, qword ptr [r12 + 160]
 mov qword ptr [r12 + 112], rax
 mov rax, qword ptr [r12 + 168]
 mov qword ptr [r12 + 120], rax
# marshal arg1 = producer-box slot [zr+192] -> [zr+128]
 mov rax, qword ptr [r12 + 192]
 mov qword ptr [r12 + 128], rax
 mov rax, qword ptr [r12 + 200]
 mov qword ptr [r12 + 136], rax
  .section .rodata
  .Lrkfn11: .string "SNO$KWSET"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn11]
 lea rsi, [r12 + 112]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 cmp eax, 99
 je xchain7_n3_α
 jmp xchain7_n3_α
 xchain7_n2_β:
 jmp xchain7_n3_α
# IR_LIT_STRING
 xchain7_n3_α:
 mov qword ptr [r12 + 240], 1
 mov rax, qword ptr [rip + .Lx12_0]
 mov qword ptr [r12 + 248], rax
 jmp xchain7_n4_α
 xchain7_n3_β:
 jmp xchain7_n5_α
.Lx12_0:
 .quad .Lx12_0_s
.Lx12_0_s:
 .string "0123456789"
# IR_ASSIGN gva
 xchain7_n4_α:
 mov rax, qword ptr [r12 + 240]
 mov rdx, qword ptr [r12 + 248]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 224], rax
 mov qword ptr [r12 + 232], rdx
 jmp xchain7_n5_α
 xchain7_n4_β:
 jmp xchain7_n5_α
# IR_LIT_STRING
 xchain7_n5_α:
 mov qword ptr [r12 + 384], 1
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [r12 + 392], rax
 jmp xchain7_n6_α
 xchain7_n5_β:
 jmp xchain7_n8_α
.Lx14_0:
 .quad .Lx14_0_s
.Lx14_0_s:
 .string "'-"
# IR_VAR
 xchain7_n6_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 416], rax
 mov qword ptr [r12 + 424], rdx
 jmp xchain7_n7_α
 xchain7_n6_β:
 jmp xchain7_n8_α
 xchain7_n7_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 384]
 mov rsi, qword ptr [r12 + 392]
 mov rdx, qword ptr [r12 + 416]
 mov rcx, qword ptr [r12 + 424]
 call str_concat_d@PLT
 mov qword ptr [r12 + 352], rax
 mov qword ptr [r12 + 360], rdx
 jmp xchain7_n9_α
 xchain7_n7_β:
 jmp xchain7_n8_α
# IR_LIT_STRING
 xchain7_n8_α:
 mov qword ptr [r12 + 544], 1
 mov rax, qword ptr [rip + .Lx17_0]
 mov qword ptr [r12 + 552], rax
 jmp xchain7_n10_α
 xchain7_n8_β:
 jmp xchain7_n13_α
.Lx17_0:
 .quad .Lx17_0_s
.Lx17_0_s:
 .string "PAT$0"
# IR_KEYWORD_SNOBOL4_read
 xchain7_n9_α:
 mov rdi, qword ptr [rip + .Lx18_0]
 call rt_keyword_read_snobol4@PLT
 mov qword ptr [r12 + 448], rax
 mov qword ptr [r12 + 456], rdx
 jmp xchain7_n11_α
 xchain7_n9_β:
 jmp xchain7_n8_α
.Lx18_0:
 .quad .Lx18_0_s
.Lx18_0_s:
 .string "UCASE"
 xchain7_n10_α:
# BOX IR_CALL SNO$MKPAT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+544] -> [zr+512]
 mov rax, qword ptr [r12 + 544]
 mov qword ptr [r12 + 512], rax
 mov rax, qword ptr [r12 + 552]
 mov qword ptr [r12 + 520], rax
  .section .rodata
  .Lrkfn20: .string "SNO$MKPAT"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn20]
 lea rsi, [r12 + 512]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 496], rax
 mov qword ptr [r12 + 504], rdx
 cmp eax, 99
 je xchain7_n13_α
 jmp xchain7_n12_α
 xchain7_n10_β:
 jmp xchain7_n13_α
 xchain7_n11_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 352]
 mov rsi, qword ptr [r12 + 360]
 mov rdx, qword ptr [r12 + 448]
 mov rcx, qword ptr [r12 + 456]
 call str_concat_d@PLT
 mov qword ptr [r12 + 320], rax
 mov qword ptr [r12 + 328], rdx
 jmp xchain7_n14_α
 xchain7_n11_β:
 jmp xchain7_n8_α
# IR_ASSIGN gva
 xchain7_n12_α:
 mov rax, qword ptr [r12 + 496]
 mov rdx, qword ptr [r12 + 504]
 mov qword ptr [rbx + 32], rax
 mov qword ptr [rbx + 40], rdx
 mov qword ptr [r12 + 480], rax
 mov qword ptr [r12 + 488], rdx
 jmp xchain7_n13_α
 xchain7_n12_β:
 jmp xchain7_n13_α
# IR_VAR
 xchain7_n13_α:
 mov rdi, qword ptr [rip + .Lx23_0]
 call NV_GET_fn@PLT
 cmp eax, 99
 je xchain7_n16_α
 mov qword ptr [r12 + 592], rax
 mov qword ptr [r12 + 600], rdx
 jmp xchain7_n15_α
 xchain7_n13_β:
 jmp xchain7_n16_α
.Lx23_0:
 .quad .Lx23_0_s
.Lx23_0_s:
 .string "INPUT"
# IR_KEYWORD_SNOBOL4_read
 xchain7_n14_α:
 mov rdi, qword ptr [rip + .Lx24_0]
 call rt_keyword_read_snobol4@PLT
 mov qword ptr [r12 + 464], rax
 mov qword ptr [r12 + 472], rdx
 jmp xchain7_n17_α
 xchain7_n14_β:
 jmp xchain7_n8_α
.Lx24_0:
 .quad .Lx24_0_s
.Lx24_0_s:
 .string "LCASE"
# IR_ASSIGN gva
 xchain7_n15_α:
 mov rax, qword ptr [r12 + 592]
 mov rdx, qword ptr [r12 + 600]
 mov qword ptr [rbx + 48], rax
 mov qword ptr [rbx + 56], rdx
 mov qword ptr [r12 + 576], rax
 mov qword ptr [r12 + 584], rdx
 jmp xchain7_n18_α
 xchain7_n15_β:
 jmp xchain7_n16_α
# IR_VAR
 xchain7_n16_α:
 mov rax, qword ptr [rbx + 64]
 mov rdx, qword ptr [rbx + 72]
 mov qword ptr [r12 + 1008], rax
 mov qword ptr [r12 + 1016], rdx
 jmp xchain7_n19_α
 xchain7_n16_β:
 jmp main_γ
 xchain7_n17_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 320]
 mov rsi, qword ptr [r12 + 328]
 mov rdx, qword ptr [r12 + 464]
 mov rcx, qword ptr [r12 + 472]
 call str_concat_d@PLT
 mov qword ptr [r12 + 288], rax
 mov qword ptr [r12 + 296], rdx
 jmp xchain7_n20_α
 xchain7_n17_β:
 jmp xchain7_n8_α
# IR_VAR
 xchain7_n18_α:
 mov rax, qword ptr [rbx + 48]
 mov rdx, qword ptr [rbx + 56]
 mov qword ptr [r12 + 784], rax
 mov qword ptr [r12 + 792], rdx
 jmp xchain7_n21_α
 xchain7_n18_β:
 jmp xchain7_n13_α
# IR_UNOP
 xchain7_n19_α:
 mov rdi, qword ptr [r12 + 1008]
 mov rsi, qword ptr [r12 + 1016]
 call rt_num_pos@PLT
 mov qword ptr [r12 + 976], rax
 mov qword ptr [r12 + 984], rdx
 jmp xchain7_n22_α
 xchain7_n19_β:
 jmp main_γ
# IR_ASSIGN gva
 xchain7_n20_α:
 mov rax, qword ptr [r12 + 288]
 mov rdx, qword ptr [r12 + 296]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 272], rax
 mov qword ptr [r12 + 280], rdx
 jmp xchain7_n8_α
 xchain7_n20_β:
 jmp xchain7_n8_α
# IR_MATCH_HEAD
 xchain7_n21_α:
 call rt_zls_mark@PLT
 mov qword ptr [r12 + 648], rax
 mov qword ptr [r12 + 656], rsp
 mov rdi, qword ptr [r12 + 784]
 mov rsi, qword ptr [r12 + 792]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov dword ptr [r12 + 640], 0
.Lx32_0:
 mov r14d, dword ptr [r12 + 640]
 jmp xchain7_n23_α
 xchain7_n21_β:
 add dword ptr [r12 + 640], 1
 mov eax, dword ptr [r12 + 640]
 cmp eax, r15d
 jg .Lx32_1
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne .Lx32_1
 jmp .Lx32_0
.Lx32_1:
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [r12 + 648]
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
 mov rsp, qword ptr [r12 + 656]
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_dcap_end_fail@PLT
 mov rsp, [rsp + 8]
 jmp xchain7_n13_α
# IR_LIT_STRING
 xchain7_n22_α:
 mov qword ptr [r12 + 1040], 1
 mov rax, qword ptr [rip + .Lx33_0]
 mov qword ptr [r12 + 1048], rax
 jmp xchain7_n24_α
 xchain7_n22_β:
 jmp main_γ
.Lx33_0:
 .quad .Lx33_0_s
.Lx33_0_s:
 .string " words"
# IR_MATCH_DEFER inlined frozen head (FZ-5b)
 xchain7_n23_α:
 lea rax, [rip + proc_PAT$0_α]
 xor ecx, ecx
 mov qword ptr [r12 + 736], rcx
 test rax, rax
 jz .Lx34_0
 mov qword ptr [r12 + 736], rax
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, [rsp + 8]
 mov qword ptr [r12 + 744], rax
 mov rcx, qword ptr [r12 + 736]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx34_1
 mov rdi, qword ptr [r12 + 744]
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, [rsp + 8]
 xor eax, eax
 mov qword ptr [r12 + 736], rax
 jmp xchain7_n21_β
.Lx34_1:
 jmp xchain7_n25_α
.Lx34_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S1]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_open@PLT
.Lx34_2:
 test rax, rax
 je .Lx34_3
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx34_2
.Lx34_3:
 mov rsp, rbp
 pop rbp
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
 js xchain7_n21_β
 mov r14d, eax
 jmp xchain7_n25_α
 xchain7_n23_β:
 mov rcx, qword ptr [r12 + 736]
 test rcx, rcx
 jz xchain7_n21_β
 mov rdi, qword ptr [r12 + 744]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx34_1
 mov rdi, qword ptr [r12 + 744]
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, [rsp + 8]
 xor eax, eax
 mov qword ptr [r12 + 736], rax
 jmp xchain7_n21_β
 xchain7_n24_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 976]
 mov rsi, qword ptr [r12 + 984]
 mov rdx, qword ptr [r12 + 1040]
 mov rcx, qword ptr [r12 + 1048]
 call str_concat_d@PLT
 mov qword ptr [r12 + 944], rax
 mov qword ptr [r12 + 952], rdx
 jmp xchain7_n26_α
 xchain7_n24_β:
 jmp main_γ
# IR_MATCH_RELEASE
 xchain7_n25_α:
 mov qword ptr [r12 + 664], r14
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [r12 + 648]
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
 mov rsp, qword ptr [r12 + 656]
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
.Lx37_1:
 test rax, rax
 je .Lx37_2
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
 jmp .Lx37_1
.Lx37_2:
 call rt_dcap_end_ok_close@PLT
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 jmp xchain7_n27_α
# IR_ASSIGN global
 xchain7_n26_α:
 mov rsi, qword ptr [r12 + 944]
 mov rdx, qword ptr [r12 + 952]
 mov rdi, qword ptr [rip + .Lx38_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 928], rax
 mov qword ptr [r12 + 936], rdx
 jmp main_γ
 xchain7_n26_β:
 jmp main_γ
.Lx38_0:
 .quad .Lx38_0_s
.Lx38_0_s:
 .string "OUTPUT"
# IR_LIT_STRING
 xchain7_n27_α:
 mov qword ptr [r12 + 688], 1
 mov rax, qword ptr [rip + .Lx39_0]
 mov qword ptr [r12 + 696], rax
 jmp xchain7_n28_α
 xchain7_n27_β:
 jmp xchain7_n13_α
.Lx39_0:
 .quad .Lx39_0_s
.Lx39_0_s:
 .string ""
# IR_MATCH_REPLACE
 xchain7_n28_α:
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [rip + .Lx41_0]
 mov rsi, qword ptr [r12 + 784]
 mov rdx, qword ptr [r12 + 792]
 mov ecx, dword ptr [r12 + 640]
 mov r8, qword ptr [r12 + 664]
 lea r9, [r12 + 688]
 call rt_match_replace@PLT
 mov rsp, [rsp + 8]
 jmp .Lx41_1
.Lx41_0:
 .quad .Lx41_0_s
.Lx41_0_s:
 .string "LINE"
.Lx41_1:
 jmp xchain7_n29_α
# IR_VAR
 xchain7_n29_α:
 mov rax, qword ptr [rbx + 64]
 mov rdx, qword ptr [rbx + 72]
 mov qword ptr [r12 + 864], rax
 mov qword ptr [r12 + 872], rdx
 jmp xchain7_n30_α
 xchain7_n29_β:
 jmp xchain7_n18_α
# IR_LIT_INTEGER
 xchain7_n30_α:
 mov qword ptr [r12 + 896], 6
 mov rax, qword ptr [rip + .Lx43_0]
 mov qword ptr [r12 + 904], rax
 jmp xchain7_n31_α
 xchain7_n30_β:
 jmp xchain7_n18_α
.Lx43_0:
 .quad 1
 xchain7_n31_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 864]
 cmp eax, 100
 je .Lx44_0
 mov eax, dword ptr [r12 + 864]
 cmp eax, 6
 jne .Lx44_2
.Lx44_1:
 mov rax, qword ptr [r12 + 872]
 mov rcx, 1
 add rax, rcx
 mov qword ptr [r12 + 832], 6
 mov qword ptr [r12 + 840], rax
 jmp xchain7_n32_α
.Lx44_0:
 mov rdi, qword ptr [r12 + 864]
 mov rsi, qword ptr [r12 + 872]
 mov rdx, qword ptr [r12 + 896]
 mov rcx, qword ptr [r12 + 904]
 mov r8d, 0
 lea r9, [r12 + 832]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx44_3
.Lx44_2:
 mov rdi, qword ptr [r12 + 864]
 mov rsi, qword ptr [r12 + 872]
 mov rdx, qword ptr [r12 + 896]
 mov rcx, qword ptr [r12 + 904]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain7_n18_α
 mov qword ptr [r12 + 832], rax
 mov qword ptr [r12 + 840], rdx
.Lx44_3:
 jmp xchain7_n32_α
 xchain7_n31_β:
 jmp xchain7_n18_α
# IR_ASSIGN gva
 xchain7_n32_α:
 mov rax, qword ptr [r12 + 832]
 mov rdx, qword ptr [r12 + 840]
 mov qword ptr [rbx + 64], rax
 mov qword ptr [rbx + 72], rdx
 mov qword ptr [r12 + 816], rax
 mov qword ptr [r12 + 824], rdx
 jmp xchain7_n18_α
 xchain7_n32_β:
 jmp xchain7_n18_α
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
 mov rdi, qword ptr [r12 + 1072]
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
.S0: .string "'-0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
.S1: .string "WPAT"
.text
