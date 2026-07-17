  .intel_syntax noprefix
  .text
  .globl proc_PAT$0_α
proc_PAT$0_α:
#=======================================================================================================================
    .global proc_PAT$0_α
    .global proc_PAT$0_β
    .global proc_PAT$0_γ
    .global proc_PAT$0_ω
  sub rsp, 176
  mov [rsp+8], rcx
  mov [rsp+16], rdx
  mov [rsp+24], rbp
  lea rbp, [rsp+32]
  mov rdi, rbp
  mov ecx, 144
  xor eax, eax
  rep stosb
  mov qword ptr [rbp + 136], rsp
 push rsi
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, [rsp + 8]
 mov qword ptr [rbp + 128], rax
 pop rsi
proc_PAT$0_α_body:
lea rax, [rip + xchain0_n0_β]
mov qword ptr [rbp + 112], rax
# IR_MATCH_SEQ_NARY (ZB-FC-3b: zero cell, LIFO-structural)
 xchain0_n0_α:
 jmp xchain0_n1_α
xchain0_n0_as:
 jmp proc_PAT$0_γ
 xchain0_n0_β:
 jmp xchain0_n2_β
xchain0_n0_af:
 jmp proc_PAT$0_ω
# IR_MATCH_BREAK
 xchain0_n1_α:
 sub rsp, 16
 mov dword ptr [rsp + 0], 0
.Lx4_0:
 mov eax, r14d
 add eax, dword ptr [rsp + 0]
 cmp eax, r15d
 jl .Lx4_240
 add rsp, 16
 jmp xchain0_n0_af
.Lx4_240:
 movsxd rcx, eax
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S0]
 call strchr@PLT
 test rax, rax
 jnz .Lx4_1
 add dword ptr [rsp + 0], 1
 jmp .Lx4_0
.Lx4_1:
 mov eax, r14d
 add eax, dword ptr [rsp + 0]
 mov r14d, eax
 jmp xchain0_n2_α
 xchain0_n1_β:
 mov eax, r14d
 sub eax, dword ptr [rsp + 0]
 mov r14d, eax
 add rsp, 16
 jmp xchain0_n0_af
# IR_MATCH_SPAN
 xchain0_n2_α:
 sub rsp, 16
 mov dword ptr [rsp + 0], 0
.Lx6_0:
 mov eax, r14d
 add eax, dword ptr [rsp + 0]
 cmp eax, r15d
 jge .Lx6_1
 movsxd rcx, eax
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S0]
 call strchr@PLT
 test rax, rax
 je .Lx6_1
 add dword ptr [rsp + 0], 1
 jmp .Lx6_0
.Lx6_1:
 mov eax, dword ptr [rsp + 0]
 test eax, eax
 jg .Lx6_240
 add rsp, 16
 jmp xchain0_n1_β
.Lx6_240:
 mov edx, r14d
 mov dword ptr [rsp + 4], edx
 add edx, eax
 mov r14d, edx
 jmp xchain0_n0_as
 xchain0_n2_β:
 xchain0_n2_β:
 mov r14d, dword ptr [rsp + 4]
 add rsp, 16
 jmp xchain0_n1_β
proc_PAT$0_res:
add rsp, 8
pop rbp
proc_PAT$0_β:
jmp qword ptr [rbp + 112]
proc_PAT$0_γ:
push rbp
lea rax, [rip + proc_PAT$0_res]
push rax
mov rax, [rbp-24]
mov rbp, [rbp-8]
jmp rax
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [rbp + 128]
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
proc_PAT$0_ω:
mov rax, [rbp-16]
lea rsp, [rbp + 144]
mov rbp, [rbp-8]
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
  mov esi, 144
  call rt_proc_set_frame_bytes@PLT
  add rsp, 8
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
  mov qword ptr [rsp + 1096], rsp
main_α_body:
# IR_LIT_STRING
 xchain7_n0_α:
 mov qword ptr [rsp + 160], 1
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [rsp + 168], rax
 jmp xchain7_n1_α
 xchain7_n0_β:
 jmp xchain7_n3_α
.Lx8_0:
 .quad .Lx8_0_s
.Lx8_0_s:
 .string "TRIM"
# IR_LIT_INTEGER
 xchain7_n1_α:
 mov qword ptr [rsp + 192], 6
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [rsp + 200], rax
 jmp xchain7_n2_α
 xchain7_n1_β:
 jmp xchain7_n3_α
.Lx9_0:
 .quad 1
 xchain7_n2_α:
# BOX IR_CALL SNO$KWSET(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+160] -> [zr+112]
 mov rax, qword ptr [rsp + 160]
 mov qword ptr [rsp + 112], rax
 mov rax, qword ptr [rsp + 168]
 mov qword ptr [rsp + 120], rax
# marshal arg1 = producer-box slot [zr+192] -> [zr+128]
 mov rax, qword ptr [rsp + 192]
 mov qword ptr [rsp + 128], rax
 mov rax, qword ptr [rsp + 200]
 mov qword ptr [rsp + 136], rax
  .section .rodata
  .Lrkfn11: .string "SNO$KWSET"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn11]
 lea rsi, [rsp + 112]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 96], rax
 mov qword ptr [rsp + 104], rdx
 cmp eax, 99
 je xchain7_n3_α
 jmp xchain7_n3_α
 xchain7_n2_β:
 jmp xchain7_n3_α
# IR_LIT_STRING
 xchain7_n3_α:
 mov qword ptr [rsp + 240], 1
 mov rax, qword ptr [rip + .Lx12_0]
 mov qword ptr [rsp + 248], rax
 jmp xchain7_n4_α
 xchain7_n3_β:
 jmp xchain7_n5_α
.Lx12_0:
 .quad .Lx12_0_s
.Lx12_0_s:
 .string "0123456789"
# IR_ASSIGN gva
 xchain7_n4_α:
 mov rax, qword ptr [rsp + 240]
 mov rdx, qword ptr [rsp + 248]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rsp + 224], rax
 mov qword ptr [rsp + 232], rdx
 jmp xchain7_n5_α
 xchain7_n4_β:
 jmp xchain7_n5_α
# IR_LIT_STRING
 xchain7_n5_α:
 mov qword ptr [rsp + 384], 1
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [rsp + 392], rax
 jmp xchain7_n6_α
 xchain7_n5_β:
 jmp xchain7_n8_α
.Lx14_0:
 .quad .Lx14_0_s
.Lx14_0_s:
 .string "'-"
# IR_VAR
 xchain7_n6_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 mov qword ptr [rsp + 416], rax
 mov qword ptr [rsp + 424], rdx
 jmp xchain7_n7_α
 xchain7_n6_β:
 jmp xchain7_n8_α
 xchain7_n7_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rsp + 384]
 mov rsi, qword ptr [rsp + 392]
 mov rdx, qword ptr [rsp + 416]
 mov rcx, qword ptr [rsp + 424]
 call str_concat_d@PLT
 mov qword ptr [rsp + 352], rax
 mov qword ptr [rsp + 360], rdx
 jmp xchain7_n9_α
 xchain7_n7_β:
 jmp xchain7_n8_α
# IR_LIT_STRING
 xchain7_n8_α:
 mov qword ptr [rsp + 544], 1
 mov rax, qword ptr [rip + .Lx17_0]
 mov qword ptr [rsp + 552], rax
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
 mov qword ptr [rsp + 448], rax
 mov qword ptr [rsp + 456], rdx
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
 mov rax, qword ptr [rsp + 544]
 mov qword ptr [rsp + 512], rax
 mov rax, qword ptr [rsp + 552]
 mov qword ptr [rsp + 520], rax
  .section .rodata
  .Lrkfn20: .string "SNO$MKPAT"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn20]
 lea rsi, [rsp + 512]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rsp + 496], rax
 mov qword ptr [rsp + 504], rdx
 cmp eax, 99
 je xchain7_n13_α
 jmp xchain7_n12_α
 xchain7_n10_β:
 jmp xchain7_n13_α
 xchain7_n11_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rsp + 352]
 mov rsi, qword ptr [rsp + 360]
 mov rdx, qword ptr [rsp + 448]
 mov rcx, qword ptr [rsp + 456]
 call str_concat_d@PLT
 mov qword ptr [rsp + 320], rax
 mov qword ptr [rsp + 328], rdx
 jmp xchain7_n14_α
 xchain7_n11_β:
 jmp xchain7_n8_α
# IR_ASSIGN gva
 xchain7_n12_α:
 mov rax, qword ptr [rsp + 496]
 mov rdx, qword ptr [rsp + 504]
 mov qword ptr [1879052320], rax
 mov qword ptr [1879052328], rdx
 mov qword ptr [rsp + 480], rax
 mov qword ptr [rsp + 488], rdx
 jmp xchain7_n13_α
 xchain7_n12_β:
 jmp xchain7_n13_α
# IR_VAR
 xchain7_n13_α:
 mov rdi, qword ptr [rip + .Lx23_0]
 call NV_GET_fn@PLT
 cmp eax, 99
 je xchain7_n16_α
 mov qword ptr [rsp + 592], rax
 mov qword ptr [rsp + 600], rdx
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
 mov qword ptr [rsp + 464], rax
 mov qword ptr [rsp + 472], rdx
 jmp xchain7_n17_α
 xchain7_n14_β:
 jmp xchain7_n8_α
.Lx24_0:
 .quad .Lx24_0_s
.Lx24_0_s:
 .string "LCASE"
# IR_ASSIGN gva
 xchain7_n15_α:
 mov rax, qword ptr [rsp + 592]
 mov rdx, qword ptr [rsp + 600]
 mov qword ptr [1879052336], rax
 mov qword ptr [1879052344], rdx
 mov qword ptr [rsp + 576], rax
 mov qword ptr [rsp + 584], rdx
 jmp xchain7_n18_α
 xchain7_n15_β:
 jmp xchain7_n16_α
# IR_VAR
 xchain7_n16_α:
 mov rax, qword ptr [1879052352]
 mov rdx, qword ptr [1879052360]
 mov qword ptr [rsp + 1024], rax
 mov qword ptr [rsp + 1032], rdx
 jmp xchain7_n19_α
 xchain7_n16_β:
 jmp main_γ
 xchain7_n17_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rsp + 320]
 mov rsi, qword ptr [rsp + 328]
 mov rdx, qword ptr [rsp + 464]
 mov rcx, qword ptr [rsp + 472]
 call str_concat_d@PLT
 mov qword ptr [rsp + 288], rax
 mov qword ptr [rsp + 296], rdx
 jmp xchain7_n20_α
 xchain7_n17_β:
 jmp xchain7_n8_α
# IR_VAR
 xchain7_n18_α:
 mov rax, qword ptr [1879052336]
 mov rdx, qword ptr [1879052344]
 mov qword ptr [rsp + 800], rax
 mov qword ptr [rsp + 808], rdx
 jmp xchain7_n21_α
 xchain7_n18_β:
 jmp xchain7_n13_α
# IR_UNOP
 xchain7_n19_α:
 mov rdi, qword ptr [rsp + 1024]
 mov rsi, qword ptr [rsp + 1032]
 call rt_num_pos@PLT
 mov qword ptr [rsp + 992], rax
 mov qword ptr [rsp + 1000], rdx
 jmp xchain7_n22_α
 xchain7_n19_β:
 jmp main_γ
# IR_ASSIGN gva
 xchain7_n20_α:
 mov rax, qword ptr [rsp + 288]
 mov rdx, qword ptr [rsp + 296]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rsp + 272], rax
 mov qword ptr [rsp + 280], rdx
 jmp xchain7_n8_α
 xchain7_n20_β:
 jmp xchain7_n8_α
# IR_MATCH_HEAD
 xchain7_n21_α:
 mov qword ptr [rsp + 680], rbp
 mov rbp, rsp
 mov rdi, qword ptr [rbp + 800]
 mov rsi, qword ptr [rbp + 808]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov rax, qword ptr [1879048192]
 mov qword ptr [rbp + 672], rax
 mov qword ptr [rbp + 656], rsp
 lea rcx, [rip + g_patstk_sp]
 mov rax, qword ptr [rcx + 0]
 mov qword ptr [rbp + 648], rax
 mov dword ptr [rbp + 640], 0
.Lx32_0:
 mov r14d, dword ptr [rbp + 640]
 jmp xchain7_n23_α
 xchain7_n21_β:
 add dword ptr [rbp + 640], 1
 mov eax, dword ptr [rbp + 640]
 cmp eax, r15d
 jg .Lx32_1
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne .Lx32_1
 jmp .Lx32_0
.Lx32_1:
 mov rax, qword ptr [rbp + 648]
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rax
 mov rsp, qword ptr [rbp + 656]
 mov rax, qword ptr [rbp + 672]
 mov qword ptr [1879048192], rax
 mov rbp, qword ptr [rbp + 680]
 jmp xchain7_n13_α
# IR_LIT_STRING
 xchain7_n22_α:
 mov qword ptr [rsp + 1056], 1
 mov rax, qword ptr [rip + .Lx33_0]
 mov qword ptr [rsp + 1064], rax
 jmp xchain7_n24_α
 xchain7_n22_β:
 jmp main_γ
.Lx33_0:
 .quad .Lx33_0_s
.Lx33_0_s:
 .string " words"
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain7_n23_α:
 lea rdi, [rip + .S1]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx34_0
 lea rcx, [rip + g_pat_main_rsp]
 mov qword ptr [rcx + 0], rsp
 lea rcx, [rip + g_patstk_sp]
 mov rsp, qword ptr [rcx + 0]
 lea rcx, [rip + .Lx34_4]
 lea rdx, [rip + .Lx34_5]
 jmp rax
.Lx34_4:
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rsp
 lea rcx, [rip + g_pat_main_rsp]
 mov rsp, qword ptr [rcx + 0]
 jmp xchain7_n25_α
.Lx34_5:
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rsp
 lea rcx, [rip + g_pat_main_rsp]
 mov rsp, qword ptr [rcx + 0]
 jmp xchain7_n21_β
.Lx34_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rdi, [rip + .S1]
 xor esi, esi
 call rt_defer_open@PLT
.Lx34_2:
 test rax, rax
 je .Lx34_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx34_7]
 lea rdx, [rip + .Lx34_8]
 jmp rax
.Lx34_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx34_2
.Lx34_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx34_2
.Lx34_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain7_n21_β
 mov r14d, eax
 lea rax, [rip + .Lx34_6]
 lea rcx, [rip + g_pat_main_rsp]
 mov qword ptr [rcx + 0], rsp
 lea rcx, [rip + g_patstk_sp]
 mov rsp, qword ptr [rcx + 0]
 sub rsp, 8
 push rax
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rsp
 lea rcx, [rip + g_pat_main_rsp]
 mov rsp, qword ptr [rcx + 0]
 jmp xchain7_n25_α
.Lx34_6:
 add rsp, 16
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rsp
 lea rcx, [rip + g_pat_main_rsp]
 mov rsp, qword ptr [rcx + 0]
 jmp xchain7_n21_β
 xchain7_n23_β:
 lea rcx, [rip + g_pat_main_rsp]
 mov qword ptr [rcx + 0], rsp
 lea rcx, [rip + g_patstk_sp]
 mov rsp, qword ptr [rcx + 0]
 jmp qword ptr [rsp]
 xchain7_n24_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rsp + 992]
 mov rsi, qword ptr [rsp + 1000]
 mov rdx, qword ptr [rsp + 1056]
 mov rcx, qword ptr [rsp + 1064]
 call str_concat_d@PLT
 mov qword ptr [rsp + 960], rax
 mov qword ptr [rsp + 968], rdx
 jmp xchain7_n26_α
 xchain7_n24_β:
 jmp main_γ
# IR_MATCH_RELEASE
 xchain7_n25_α:
 mov rax, qword ptr [rbp + 648]
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rax
 mov qword ptr [rbp + 664], r14
 mov rsp, qword ptr [rbp + 656]
 push r14
 push r15
 push r13
 sub rsp, 8
 mov rdi, qword ptr [rsp + 704]
 mov rsi, qword ptr [1879048192]
 mov rdx, r13
 call rt_dcap_end_ok_open@PLT
.Lx37_1:
 test rax, rax
 je .Lx37_2
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx37_3]
 lea rdx, [rip + .Lx37_4]
 jmp rax
.Lx37_3:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx37_1
.Lx37_4:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx37_1
.Lx37_2:
 call rt_dcap_end_ok_close@PLT
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov rax, qword ptr [rbp + 672]
 mov qword ptr [1879048192], rax
 jmp xchain7_n27_α
# IR_ASSIGN global
 xchain7_n26_α:
 mov rsi, qword ptr [rsp + 960]
 mov rdx, qword ptr [rsp + 968]
 mov rdi, qword ptr [rip + .Lx38_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 944], rax
 mov qword ptr [rsp + 952], rdx
 jmp main_γ
 xchain7_n26_β:
 jmp main_γ
.Lx38_0:
 .quad .Lx38_0_s
.Lx38_0_s:
 .string "OUTPUT"
# IR_LIT_STRING
 xchain7_n27_α:
 mov qword ptr [rsp + 704], 1
 mov rax, qword ptr [rip + .Lx39_0]
 mov qword ptr [rsp + 712], rax
 jmp xchain7_n28_α
 xchain7_n27_β:
 jmp xchain7_n13_α
.Lx39_0:
 .quad .Lx39_0_s
.Lx39_0_s:
 .string ""
# IR_MATCH_REPLACE
 xchain7_n28_α:
 mov rdi, qword ptr [rip + .Lx41_0]
 mov rsi, qword ptr [rbp + 800]
 mov rdx, qword ptr [rbp + 808]
 mov ecx, dword ptr [rbp + 640]
 mov r8, qword ptr [rbp + 664]
 lea r9, [rbp + 704]
 call rt_match_replace@PLT
 jmp .Lx41_1
.Lx41_0:
 .quad .Lx41_0_s
.Lx41_0_s:
 .string "LINE"
.Lx41_1:
 mov rbp, qword ptr [rbp + 680]
 jmp xchain7_n29_α
# IR_VAR
 xchain7_n29_α:
 mov rax, qword ptr [1879052352]
 mov rdx, qword ptr [1879052360]
 mov qword ptr [rsp + 880], rax
 mov qword ptr [rsp + 888], rdx
 jmp xchain7_n30_α
 xchain7_n29_β:
 jmp xchain7_n18_α
# IR_LIT_INTEGER
 xchain7_n30_α:
 mov qword ptr [rsp + 912], 6
 mov rax, qword ptr [rip + .Lx43_0]
 mov qword ptr [rsp + 920], rax
 jmp xchain7_n31_α
 xchain7_n30_β:
 jmp xchain7_n18_α
.Lx43_0:
 .quad 1
 xchain7_n31_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [rsp + 880]
 cmp eax, 100
 je .Lx44_0
 mov eax, dword ptr [rsp + 880]
 cmp eax, 6
 jne .Lx44_2
.Lx44_1:
 mov rax, qword ptr [rsp + 888]
 mov rcx, 1
 add rax, rcx
 mov qword ptr [rsp + 848], 6
 mov qword ptr [rsp + 856], rax
 jmp xchain7_n32_α
.Lx44_0:
 mov rdi, qword ptr [rsp + 880]
 mov rsi, qword ptr [rsp + 888]
 mov rdx, qword ptr [rsp + 912]
 mov rcx, qword ptr [rsp + 920]
 mov r8d, 0
 lea r9, [rsp + 848]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx44_3
.Lx44_2:
 mov rdi, qword ptr [rsp + 880]
 mov rsi, qword ptr [rsp + 888]
 mov rdx, qword ptr [rsp + 912]
 mov rcx, qword ptr [rsp + 920]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain7_n18_α
 mov qword ptr [rsp + 848], rax
 mov qword ptr [rsp + 856], rdx
.Lx44_3:
 jmp xchain7_n32_α
 xchain7_n31_β:
 jmp xchain7_n18_α
# IR_ASSIGN gva
 xchain7_n32_α:
 mov rax, qword ptr [rsp + 848]
 mov rdx, qword ptr [rsp + 856]
 mov qword ptr [1879052352], rax
 mov qword ptr [1879052360], rdx
 mov qword ptr [rsp + 832], rax
 mov qword ptr [rsp + 840], rdx
 jmp xchain7_n18_α
 xchain7_n32_β:
 jmp xchain7_n18_α
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rsp + 1096]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rsp + 1096]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
add rsp, 65544
ret
.section .rodata
.S0: .string "'-0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
.S1: .string "WPAT"
.text
