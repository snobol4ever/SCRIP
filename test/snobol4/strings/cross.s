  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "H"
  .Lgvan1: .string "V"
  .Lgvan2: .string "HC"
  .Lgvan3: .string "NH"
  .Lgvan4: .string "CROSS"
  .Lgvan5: .string "VC"
  .Lgvan6: .string "NV"
  .Lgvan7: .string "PRINTV"
  .Lgvan8: .string "C"
  .align 8
__gva_names:
  .quad .Lgvan0
  .quad .Lgvan1
  .quad .Lgvan2
  .quad .Lgvan3
  .quad .Lgvan4
  .quad .Lgvan5
  .quad .Lgvan6
  .quad .Lgvan7
  .quad .Lgvan8
  .section .bss
  .align 16
__gva: .space 144, 0
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
  lea rdi, [rip + __gva_names]
  lea rsi, [rip + __gva]
  mov edx, 9
  call gva_register@PLT
  mov rbx, rax
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
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  mov qword ptr [rsp + 1960], rsp
main_α_body:
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [rsp + 160], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [rsp + 168], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n3_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "TRIM"
# IR_LIT_INTEGER
 xchain0_n1_α:
 mov qword ptr [rsp + 192], 6
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [rsp + 200], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n3_α
.Lx2_0:
 .quad 1
 xchain0_n2_α:
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
  .Lrkfn4: .string "SNO$KWSET"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn4]
 lea rsi, [rsp + 112]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 96], rax
 mov qword ptr [rsp + 104], rdx
 cmp eax, 99
 je xchain0_n3_α
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n3_α
# IR_VAR
 xchain0_n3_α:
 mov rdi, qword ptr [rip + .Lx5_0]
 call NV_GET_fn@PLT
 cmp eax, 99
 je main_γ
 mov qword ptr [rsp + 240], rax
 mov qword ptr [rsp + 248], rdx
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp main_γ
.Lx5_0:
 .quad .Lx5_0_s
.Lx5_0_s:
 .string "INPUT"
# IR_ASSIGN gva
 xchain0_n4_α:
 mov rax, qword ptr [rsp + 240]
 mov rdx, qword ptr [rsp + 248]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [rsp + 224], rax
 mov qword ptr [rsp + 232], rdx
 jmp xchain0_n5_α
 xchain0_n4_β:
 jmp main_γ
# IR_VAR
 xchain0_n5_α:
 mov rdi, qword ptr [rip + .Lx7_0]
 call NV_GET_fn@PLT
 cmp eax, 99
 je main_γ
 mov qword ptr [rsp + 288], rax
 mov qword ptr [rsp + 296], rdx
 jmp xchain0_n6_α
 xchain0_n5_β:
 jmp main_γ
.Lx7_0:
 .quad .Lx7_0_s
.Lx7_0_s:
 .string "INPUT"
# IR_ASSIGN gva
 xchain0_n6_α:
 mov rax, qword ptr [rsp + 288]
 mov rdx, qword ptr [rsp + 296]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [rsp + 272], rax
 mov qword ptr [rsp + 280], rdx
 jmp xchain0_n7_α
 xchain0_n6_β:
 jmp main_γ
# IR_VAR
 xchain0_n7_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [rsp + 336], rax
 mov qword ptr [rsp + 344], rdx
 jmp xchain0_n8_α
 xchain0_n7_β:
 jmp xchain0_n9_α
# IR_ASSIGN gva
 xchain0_n8_α:
 mov rax, qword ptr [rsp + 336]
 mov rdx, qword ptr [rsp + 344]
 mov qword ptr [rbx + 32], rax
 mov qword ptr [rbx + 40], rdx
 mov qword ptr [rsp + 320], rax
 mov qword ptr [rsp + 328], rdx
 jmp xchain0_n9_α
 xchain0_n8_β:
 jmp xchain0_n9_α
# IR_VAR
 xchain0_n9_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [rsp + 656], rax
 mov qword ptr [rsp + 664], rdx
 jmp xchain0_n10_α
 xchain0_n9_β:
 jmp xchain0_n3_α
# IR_VAR
 xchain0_n10_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [rsp + 624], rax
 mov qword ptr [rsp + 632], rdx
 jmp xchain0_n11_α
 xchain0_n10_β:
 jmp xchain0_n3_α
# IR_COERCE_STRING
 xchain0_n11_α:
 lea rdi, [rsp + 624]
 lea rsi, [rsp + 592]
 mov rdx, 3866683
 call rt_coerce_str_d@PLT
 jmp xchain0_n12_α
 xchain0_n11_β:
 jmp xchain0_n3_α
# IR_MATCH_HEAD
 xchain0_n12_α:
 mov rdi, qword ptr [rsp + 656]
 mov rsi, qword ptr [rsp + 664]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov qword ptr [rsp + 424], rbp
 lea rcx, [rip + g_dcap_top]
 mov rbp, qword ptr [rcx + 0]
 mov qword ptr [rsp + 416], rbp
 mov rax, rsp
 sub rsp, 32
 mov qword ptr [rsp + 16], rax
 lea rcx, [rip + g_patstk_sp]
 mov rax, qword ptr [rcx + 0]
 mov qword ptr [rsp + 8], rax
 mov dword ptr [rsp + 0], 0
.Lx16_0:
 mov r14d, dword ptr [rsp + 0]
 jmp xchain0_n13_α
 xchain0_n12_β:
 add dword ptr [rsp + 0], 1
 mov eax, dword ptr [rsp + 0]
 cmp eax, r15d
 jg .Lx16_1
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne .Lx16_1
 jmp .Lx16_0
.Lx16_1:
 mov rax, qword ptr [rsp + 8]
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rax
 mov rsp, qword ptr [rsp + 16]
 lea rcx, [rip + g_dcap_top]
 mov rax, qword ptr [rsp + 416]
 mov qword ptr [rcx + 0], rax
 mov rbp, qword ptr [rsp + 424]
 jmp xchain0_n3_α
# IR_MATCH_SEQ_NARY (ZB-FC-3b: zero cell, LIFO-structural)
 xchain0_n13_α:
 jmp xchain0_n15_α
xchain0_n13_as:
 jmp xchain0_n14_α
 xchain0_n13_β:
 jmp xchain0_n17_β
xchain0_n13_af:
 jmp xchain0_n12_β
# IR_MATCH_RELEASE
 xchain0_n14_α:
 mov rax, qword ptr [rsp + 24]
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rax
 mov eax, dword ptr [rsp + 16]
 mov dword ptr [rsp + 432], eax
 mov qword ptr [rsp + 456], r14
 mov rsp, qword ptr [rsp + 32]
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rcx, [rip + g_dcap_top]
 mov qword ptr [rcx + 0], rbp
 mov rdi, qword ptr [rsp + 448]
 mov rsi, rbp
 mov rdx, r13
 call rt_dcap_end_ok_open@PLT
.Lx20_1:
 test rax, rax
 je .Lx20_2
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx20_3]
 lea rdx, [rip + .Lx20_4]
 jmp rax
.Lx20_3:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx20_1
.Lx20_4:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx20_1
.Lx20_2:
 call rt_dcap_end_ok_close@PLT
 add rsp, 8
 pop r13
 pop r15
 pop r14
 lea rcx, [rip + g_dcap_top]
 mov rax, qword ptr [rsp + 416]
 mov qword ptr [rcx + 0], rax
 mov rbp, qword ptr [rsp + 424]
 jmp xchain0_n18_α
# IR_MATCH_ATP
 xchain0_n15_α:
 mov esi, r14d
 lea rdi, [rip + .S0]
 call rt_at_cursor@PLT
 jmp xchain0_n16_α
 xchain0_n15_β:
 jmp xchain0_n13_af
# IR_MATCH_CAPTURE_SAVE fc cell
 xchain0_n16_α:
 sub rsp, 16
 mov dword ptr [rsp + 0], r14d
 jmp xchain0_n19_α
 xchain0_n16_β:
 add rsp, 16
 jmp xchain0_n15_β
# IR_MATCH_CAPTURE_COND (rbp-dcap inline pend)
 xchain0_n17_α:
 mov eax, dword ptr [rsp + 0]
 lea rcx, [rip + .S1]
 mov qword ptr [rbp + 0], rcx
 mov esi, eax
 mov qword ptr [rbp + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [rbp + 16], rdx
 add rbp, 24
 jmp xchain0_n13_as
 xchain0_n17_β:
 sub rbp, 24
 jmp xchain0_n19_β
# IR_LIT_STRING
 xchain0_n18_α:
 mov qword ptr [rsp + 448], 1
 mov rax, qword ptr [rip + .Lx26_0]
 mov qword ptr [rsp + 456], rax
 jmp xchain0_n20_α
 xchain0_n18_β:
 jmp xchain0_n3_α
.Lx26_0:
 .quad .Lx26_0_s
.Lx26_0_s:
 .string "*"
# IR_MATCH_ANY
 xchain0_n19_α:
 mov eax, r14d
 cmp eax, r15d
 jge xchain0_n16_β
 movsxd rcx, r14d
 movzx esi, byte ptr [r13+rcx]
 mov rdi, qword ptr [rsp + 648]
 call strchr@PLT
 test rax, rax
 je xchain0_n16_β
 add r14d, 1
 jmp xchain0_n17_α
 xchain0_n19_β:
 sub r14d, 1
 jmp xchain0_n16_β
# IR_MATCH_REPLACE
 xchain0_n20_α:
 mov rdi, qword ptr [rip + .Lx29_0]
 mov rsi, qword ptr [rsp + 656]
 mov rdx, qword ptr [rsp + 664]
 mov ecx, dword ptr [rsp + 384]
 mov r8, qword ptr [rsp + 408]
 lea r9, [rsp + 448]
 call rt_match_replace@PLT
 jmp .Lx29_1
.Lx29_0:
 .quad .Lx29_0_s
.Lx29_0_s:
 .string "HC"
.Lx29_1:
 jmp xchain0_n21_α
# IR_VAR
 xchain0_n21_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [rsp + 704], rax
 mov qword ptr [rsp + 712], rdx
 jmp xchain0_n22_α
 xchain0_n21_β:
 jmp xchain0_n23_α
# IR_ASSIGN gva
 xchain0_n22_α:
 mov rax, qword ptr [rsp + 704]
 mov rdx, qword ptr [rsp + 712]
 mov qword ptr [rbx + 80], rax
 mov qword ptr [rbx + 88], rdx
 mov qword ptr [rsp + 688], rax
 mov qword ptr [rsp + 696], rdx
 jmp xchain0_n23_α
 xchain0_n22_β:
 jmp xchain0_n23_α
# IR_VAR
 xchain0_n23_α:
 mov rax, qword ptr [rbx + 80]
 mov rdx, qword ptr [rbx + 88]
 mov qword ptr [rsp + 928], rax
 mov qword ptr [rsp + 936], rdx
 jmp xchain0_n24_α
 xchain0_n23_β:
 jmp xchain0_n9_α
# IR_MATCH_HEAD
 xchain0_n24_α:
 mov r12, rsp
 mov rdi, qword ptr [r12 + 928]
 mov rsi, qword ptr [r12 + 936]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov qword ptr [r12 + 792], rbp
 lea rcx, [rip + g_dcap_top]
 mov rbp, qword ptr [rcx + 0]
 mov qword ptr [r12 + 784], rbp
 mov qword ptr [r12 + 768], rsp
 lea rcx, [rip + g_patstk_sp]
 mov rax, qword ptr [rcx + 0]
 mov qword ptr [r12 + 760], rax
 mov dword ptr [r12 + 752], 0
.Lx34_0:
 mov r14d, dword ptr [r12 + 752]
 jmp xchain0_n25_α
 xchain0_n24_β:
 add dword ptr [r12 + 752], 1
 mov eax, dword ptr [r12 + 752]
 cmp eax, r15d
 jg .Lx34_1
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne .Lx34_1
 jmp .Lx34_0
.Lx34_1:
 mov rax, qword ptr [r12 + 760]
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rax
 mov rsp, qword ptr [r12 + 768]
 lea rcx, [rip + g_dcap_top]
 mov rax, qword ptr [r12 + 784]
 mov qword ptr [rcx + 0], rax
 mov rbp, qword ptr [r12 + 792]
 jmp xchain0_n9_α
# IR_MATCH_SEQ_NARY
 xchain0_n25_α:
 mov dword ptr [r12 + 864], r14d
 mov dword ptr [r12 + 868], 0
 jmp xchain0_n27_α
xchain0_n25_as:
 mov eax, dword ptr [r12 + 868]
 add eax, 1
 mov dword ptr [r12 + 868], eax
 cmp eax, 1
 je xchain0_n28_α
 jmp xchain0_n26_α
 xchain0_n25_β:
 mov dword ptr [r12 + 868], 2
xchain0_n25_af:
 mov eax, dword ptr [r12 + 868]
 sub eax, 1
 mov dword ptr [r12 + 868], eax
 cmp eax, 0
 je xchain0_n27_β
 cmp eax, 1
 je xchain0_n28_β
 jmp xchain0_n24_β
# IR_MATCH_RELEASE
 xchain0_n26_α:
 mov rax, qword ptr [r12 + 760]
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rax
 mov qword ptr [r12 + 776], r14
 mov rsp, qword ptr [r12 + 768]
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rcx, [rip + g_dcap_top]
 mov qword ptr [rcx + 0], rbp
 mov rdi, qword ptr [rsp + 816]
 mov rsi, rbp
 mov rdx, r13
 call rt_dcap_end_ok_open@PLT
.Lx38_1:
 test rax, rax
 je .Lx38_2
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx38_3]
 lea rdx, [rip + .Lx38_4]
 jmp rax
.Lx38_3:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx38_1
.Lx38_4:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx38_1
.Lx38_2:
 call rt_dcap_end_ok_close@PLT
 add rsp, 8
 pop r13
 pop r15
 pop r14
 lea rcx, [rip + g_dcap_top]
 mov rax, qword ptr [r12 + 784]
 mov qword ptr [rcx + 0], rax
 mov rbp, qword ptr [r12 + 792]
 jmp xchain0_n29_α
# IR_MATCH_ATP
 xchain0_n27_α:
 mov esi, r14d
 lea rdi, [rip + .S3]
 call rt_at_cursor@PLT
 jmp xchain0_n25_as
 xchain0_n27_β:
 jmp xchain0_n25_af
# IR_MATCH_DEFER (ZS-2 jmp-entry)
 xchain0_n28_α:
 lea rdi, [rip + .S1]
 xor esi, esi
 call rt_defer_get_pat_fn@PLT
 test rax, rax
 jz .Lx40_0
 lea rcx, [rip + g_pat_main_rsp]
 mov qword ptr [rcx + 0], rsp
 lea rcx, [rip + g_patstk_sp]
 mov rsp, qword ptr [rcx + 0]
 lea rcx, [rip + .Lx40_4]
 lea rdx, [rip + .Lx40_5]
 jmp rax
.Lx40_4:
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rsp
 lea rcx, [rip + g_pat_main_rsp]
 mov rsp, qword ptr [rcx + 0]
 jmp xchain0_n25_as
.Lx40_5:
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rsp
 lea rcx, [rip + g_pat_main_rsp]
 mov rsp, qword ptr [rcx + 0]
 jmp xchain0_n25_af
.Lx40_0:
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rcx, [rip + g_dcap_top]
 mov qword ptr [rcx + 0], rbp
 lea rdi, [rip + .S1]
 xor esi, esi
 call rt_defer_open@PLT
.Lx40_2:
 test rax, rax
 je .Lx40_3
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx40_7]
 lea rdx, [rip + .Lx40_8]
 jmp rax
.Lx40_7:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx40_2
.Lx40_8:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_defer_step@PLT
 jmp .Lx40_2
.Lx40_3:
 add rsp, 8
 pop r13
 pop r15
 pop r14
 mov edi, r14d
 call rt_defer_close@PLT
 test eax, eax
 js xchain0_n25_af
 mov r14d, eax
 lea rax, [rip + .Lx40_6]
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
 jmp xchain0_n25_as
.Lx40_6:
 add rsp, 16
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rsp
 lea rcx, [rip + g_pat_main_rsp]
 mov rsp, qword ptr [rcx + 0]
 jmp xchain0_n25_af
 xchain0_n28_β:
 lea rcx, [rip + g_pat_main_rsp]
 mov qword ptr [rcx + 0], rsp
 lea rcx, [rip + g_patstk_sp]
 mov rsp, qword ptr [rcx + 0]
 jmp qword ptr [rsp]
# IR_LIT_STRING
 xchain0_n29_α:
 mov qword ptr [rsp + 816], 1
 mov rax, qword ptr [rip + .Lx41_0]
 mov qword ptr [rsp + 824], rax
 jmp xchain0_n30_α
 xchain0_n29_β:
 jmp xchain0_n9_α
.Lx41_0:
 .quad .Lx41_0_s
.Lx41_0_s:
 .string "#"
# IR_MATCH_REPLACE
 xchain0_n30_α:
 mov rdi, qword ptr [rip + .Lx43_0]
 mov rsi, qword ptr [r12 + 928]
 mov rdx, qword ptr [r12 + 936]
 mov ecx, dword ptr [r12 + 752]
 mov r8, qword ptr [r12 + 776]
 lea r9, [r12 + 816]
 call rt_match_replace@PLT
 jmp .Lx43_1
.Lx43_0:
 .quad .Lx43_0_s
.Lx43_0_s:
 .string "VC"
.Lx43_1:
 jmp xchain0_n31_α
# IR_LIT_STRING
 xchain0_n31_α:
 mov qword ptr [rsp + 976], 1
 mov rax, qword ptr [rip + .Lx44_0]
 mov qword ptr [rsp + 984], rax
 jmp xchain0_n32_α
 xchain0_n31_β:
 jmp xchain0_n33_α
.Lx44_0:
 .quad .Lx44_0_s
.Lx44_0_s:
 .string ""
# IR_ASSIGN global
 xchain0_n32_α:
 mov rsi, qword ptr [rsp + 976]
 mov rdx, qword ptr [rsp + 984]
 mov rdi, qword ptr [rip + .Lx45_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 960], rax
 mov qword ptr [rsp + 968], rdx
 jmp xchain0_n33_α
 xchain0_n32_β:
 jmp xchain0_n33_α
.Lx45_0:
 .quad .Lx45_0_s
.Lx45_0_s:
 .string "OUTPUT"
# IR_VAR
 xchain0_n33_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [rsp + 1024], rax
 mov qword ptr [rsp + 1032], rdx
 jmp xchain0_n34_α
 xchain0_n33_β:
 jmp xchain0_n35_α
# IR_ASSIGN gva
 xchain0_n34_α:
 mov rax, qword ptr [rsp + 1024]
 mov rdx, qword ptr [rsp + 1032]
 mov qword ptr [rbx + 112], rax
 mov qword ptr [rbx + 120], rdx
 mov qword ptr [rsp + 1008], rax
 mov qword ptr [rsp + 1016], rdx
 jmp xchain0_n35_α
 xchain0_n34_β:
 jmp xchain0_n35_α
# IR_VAR
 xchain0_n35_α:
 mov rax, qword ptr [rbx + 112]
 mov rdx, qword ptr [rbx + 120]
 mov qword ptr [rsp + 1296], rax
 mov qword ptr [rsp + 1304], rdx
 jmp xchain0_n36_α
 xchain0_n35_β:
 jmp xchain0_n37_α
# IR_VAR
 xchain0_n36_α:
 mov rax, qword ptr [rbx + 96]
 mov rdx, qword ptr [rbx + 104]
 mov qword ptr [rsp + 1264], rax
 mov qword ptr [rsp + 1272], rdx
 jmp xchain0_n38_α
 xchain0_n36_β:
 jmp xchain0_n37_α
# IR_VAR
 xchain0_n37_α:
 mov rax, qword ptr [rbx + 112]
 mov rdx, qword ptr [rbx + 120]
 mov qword ptr [rsp + 1504], rax
 mov qword ptr [rsp + 1512], rdx
 jmp xchain0_n39_α
 xchain0_n37_β:
 jmp xchain0_n23_α
# IR_COERCE_INTEGER
 xchain0_n38_α:
 lea rdi, [rsp + 1264]
 lea rsi, [rsp + 1232]
 mov rdx, 10682530
 call rt_coerce_int_d@PLT
 jmp xchain0_n40_α
 xchain0_n38_β:
 jmp xchain0_n37_α
# IR_MATCH_HEAD
 xchain0_n39_α:
 mov rdi, qword ptr [rsp + 1504]
 mov rsi, qword ptr [rsp + 1512]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov qword ptr [rsp + 1384], rbp
 lea rcx, [rip + g_dcap_top]
 mov rbp, qword ptr [rcx + 0]
 mov qword ptr [rsp + 1376], rbp
 mov rax, rsp
 sub rsp, 32
 mov qword ptr [rsp + 16], rax
 lea rcx, [rip + g_patstk_sp]
 mov rax, qword ptr [rcx + 0]
 mov qword ptr [rsp + 8], rax
 mov dword ptr [rsp + 0], 0
.Lx54_0:
 mov r14d, dword ptr [rsp + 0]
 jmp xchain0_n41_α
 xchain0_n39_β:
 add dword ptr [rsp + 0], 1
 mov eax, dword ptr [rsp + 0]
 cmp eax, r15d
 jg .Lx54_1
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne .Lx54_1
 jmp .Lx54_0
.Lx54_1:
 mov rax, qword ptr [rsp + 8]
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rax
 mov rsp, qword ptr [rsp + 16]
 lea rcx, [rip + g_dcap_top]
 mov rax, qword ptr [rsp + 1376]
 mov qword ptr [rcx + 0], rax
 mov rbp, qword ptr [rsp + 1384]
 jmp xchain0_n23_α
# IR_MATCH_HEAD
 xchain0_n40_α:
 mov rdi, qword ptr [rsp + 1296]
 mov rsi, qword ptr [rsp + 1304]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov qword ptr [rsp + 1112], rbp
 lea rcx, [rip + g_dcap_top]
 mov rbp, qword ptr [rcx + 0]
 mov qword ptr [rsp + 1104], rbp
 mov rax, rsp
 sub rsp, 32
 mov qword ptr [rsp + 16], rax
 lea rcx, [rip + g_patstk_sp]
 mov rax, qword ptr [rcx + 0]
 mov qword ptr [rsp + 8], rax
 mov dword ptr [rsp + 0], 0
.Lx56_0:
 mov r14d, dword ptr [rsp + 0]
 jmp xchain0_n42_α
 xchain0_n40_β:
 add dword ptr [rsp + 0], 1
 mov eax, dword ptr [rsp + 0]
 cmp eax, r15d
 jg .Lx56_1
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne .Lx56_1
 jmp .Lx56_0
.Lx56_1:
 mov rax, qword ptr [rsp + 8]
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rax
 mov rsp, qword ptr [rsp + 16]
 lea rcx, [rip + g_dcap_top]
 mov rax, qword ptr [rsp + 1104]
 mov qword ptr [rcx + 0], rax
 mov rbp, qword ptr [rsp + 1112]
 jmp xchain0_n37_α
# IR_MATCH_CAPTURE_SAVE fc cell
 xchain0_n41_α:
 sub rsp, 16
 mov dword ptr [rsp + 0], r14d
 jmp xchain0_n43_α
 xchain0_n41_β:
 add rsp, 16
 jmp xchain0_n39_β
# IR_MATCH_SEQ_NARY (ZB-FC-3b: zero cell, LIFO-structural)
 xchain0_n42_α:
 jmp xchain0_n45_α
xchain0_n42_as:
 jmp xchain0_n44_α
 xchain0_n42_β:
 jmp xchain0_n46_β
xchain0_n42_af:
 jmp xchain0_n40_β
# IR_MATCH_LEN
 xchain0_n43_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain0_n41_β
 add r14d, 1
 jmp xchain0_n47_α
 xchain0_n43_β:
 sub r14d, 1
 jmp xchain0_n41_β
# IR_MATCH_RELEASE
 xchain0_n44_α:
 mov rax, qword ptr [rsp + 8]
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rax
 mov eax, dword ptr [rsp + 0]
 mov dword ptr [rsp + 1104], eax
 mov qword ptr [rsp + 1128], r14
 mov rsp, qword ptr [rsp + 16]
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rcx, [rip + g_dcap_top]
 mov qword ptr [rcx + 0], rbp
 mov rdi, qword ptr [rsp + 1136]
 mov rsi, rbp
 mov rdx, r13
 call rt_dcap_end_ok_open@PLT
.Lx63_1:
 test rax, rax
 je .Lx63_2
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx63_3]
 lea rdx, [rip + .Lx63_4]
 jmp rax
.Lx63_3:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx63_1
.Lx63_4:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx63_1
.Lx63_2:
 call rt_dcap_end_ok_close@PLT
 add rsp, 8
 pop r13
 pop r15
 pop r14
 lea rcx, [rip + g_dcap_top]
 mov rax, qword ptr [rsp + 1104]
 mov qword ptr [rcx + 0], rax
 mov rbp, qword ptr [rsp + 1112]
 jmp xchain0_n48_α
# IR_MATCH_POS
 xchain0_n45_α:
 mov rax, qword ptr [rsp + 1272]
 cmp r14d, eax
 jne xchain0_n42_af
 jmp xchain0_n46_α
 xchain0_n45_β:
 jmp xchain0_n42_af
# IR_MATCH_LEN
 xchain0_n46_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain0_n45_β
 add r14d, 1
 jmp xchain0_n42_as
 xchain0_n46_β:
 sub r14d, 1
 jmp xchain0_n45_β
# IR_MATCH_CAPTURE_COND (rbp-dcap inline pend)
 xchain0_n47_α:
 mov eax, dword ptr [rsp + 0]
 lea rcx, [rip + .S4]
 mov qword ptr [rbp + 0], rcx
 mov esi, eax
 mov qword ptr [rbp + 8], rsi
 mov edx, r14d
 sub edx, eax
 mov qword ptr [rbp + 16], rdx
 add rbp, 24
 jmp xchain0_n49_α
 xchain0_n47_β:
 sub rbp, 24
 jmp xchain0_n43_β
# IR_LIT_STRING
 xchain0_n48_α:
 mov qword ptr [rsp + 1136], 1
 mov rax, qword ptr [rip + .Lx68_0]
 mov qword ptr [rsp + 1144], rax
 jmp xchain0_n50_α
 xchain0_n48_β:
 jmp xchain0_n37_α
.Lx68_0:
 .quad .Lx68_0_s
.Lx68_0_s:
 .string "#"
# IR_MATCH_RELEASE
 xchain0_n49_α:
 mov rax, qword ptr [rsp + 24]
 lea rcx, [rip + g_patstk_sp]
 mov qword ptr [rcx + 0], rax
 mov eax, dword ptr [rsp + 16]
 mov dword ptr [rsp + 1392], eax
 mov qword ptr [rsp + 1416], r14
 mov rsp, qword ptr [rsp + 32]
 push r14
 push r15
 push r13
 sub rsp, 8
 lea rcx, [rip + g_dcap_top]
 mov qword ptr [rcx + 0], rbp
 mov rdi, qword ptr [rsp + 1408]
 mov rsi, rbp
 mov rdx, r13
 call rt_dcap_end_ok_open@PLT
.Lx70_1:
 test rax, rax
 je .Lx70_2
 call rt_proc_open_fn@PLT
 lea rcx, [rip + .Lx70_3]
 lea rdx, [rip + .Lx70_4]
 jmp rax
.Lx70_3:
 call rt_proc_call_epilogue_γ@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx70_1
.Lx70_4:
 call rt_proc_call_epilogue_ω@PLT
 mov rdi, rax
 mov rsi, rdx
 call rt_dcap_step@PLT
 jmp .Lx70_1
.Lx70_2:
 call rt_dcap_end_ok_close@PLT
 add rsp, 8
 pop r13
 pop r15
 pop r14
 lea rcx, [rip + g_dcap_top]
 mov rax, qword ptr [rsp + 1376]
 mov qword ptr [rcx + 0], rax
 mov rbp, qword ptr [rsp + 1384]
 jmp xchain0_n51_α
# IR_MATCH_REPLACE
 xchain0_n50_α:
 mov rdi, qword ptr [rip + .Lx72_0]
 mov rsi, qword ptr [rsp + 1296]
 mov rdx, qword ptr [rsp + 1304]
 mov ecx, dword ptr [rsp + 1072]
 mov r8, qword ptr [rsp + 1096]
 lea r9, [rsp + 1136]
 call rt_match_replace@PLT
 jmp .Lx72_1
.Lx72_0:
 .quad .Lx72_0_s
.Lx72_0_s:
 .string "PRINTV"
.Lx72_1:
 jmp xchain0_n37_α
# IR_LIT_STRING
 xchain0_n51_α:
 mov qword ptr [rsp + 1408], 1
 mov rax, qword ptr [rip + .Lx73_0]
 mov qword ptr [rsp + 1416], rax
 jmp xchain0_n52_α
 xchain0_n51_β:
 jmp xchain0_n23_α
.Lx73_0:
 .quad .Lx73_0_s
.Lx73_0_s:
 .string ""
# IR_MATCH_REPLACE
 xchain0_n52_α:
 mov rdi, qword ptr [rip + .Lx75_0]
 mov rsi, qword ptr [rsp + 1504]
 mov rdx, qword ptr [rsp + 1512]
 mov ecx, dword ptr [rsp + 1344]
 mov r8, qword ptr [rsp + 1368]
 lea r9, [rsp + 1408]
 call rt_match_replace@PLT
 jmp .Lx75_1
.Lx75_0:
 .quad .Lx75_0_s
.Lx75_0_s:
 .string "PRINTV"
.Lx75_1:
 jmp xchain0_n53_α
# IR_VAR
 xchain0_n53_α:
 mov rax, qword ptr [rbx + 128]
 mov rdx, qword ptr [rbx + 136]
 mov qword ptr [rsp + 1680], rax
 mov qword ptr [rsp + 1688], rdx
 jmp xchain0_n54_α
 xchain0_n53_β:
 jmp xchain0_n55_α
# IR_LIT_STRING
 xchain0_n54_α:
 mov qword ptr [rsp + 1712], 1
 mov rax, qword ptr [rip + .Lx77_0]
 mov qword ptr [rsp + 1720], rax
 jmp xchain0_n56_α
 xchain0_n54_β:
 jmp xchain0_n55_α
.Lx77_0:
 .quad .Lx77_0_s
.Lx77_0_s:
 .string "#"
# IR_VAR
 xchain0_n55_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [rsp + 1920], rax
 mov qword ptr [rsp + 1928], rdx
 jmp xchain0_n57_α
 xchain0_n55_β:
 jmp xchain0_n37_α
 xchain0_n56_α:
# BOX IR_CALL DIFFER(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+1680] -> [zr+1632]
 mov rax, qword ptr [rsp + 1680]
 mov qword ptr [rsp + 1632], rax
 mov rax, qword ptr [rsp + 1688]
 mov qword ptr [rsp + 1640], rax
# marshal arg1 = producer-box slot [zr+1712] -> [zr+1648]
 mov rax, qword ptr [rsp + 1712]
 mov qword ptr [rsp + 1648], rax
 mov rax, qword ptr [rsp + 1720]
 mov qword ptr [rsp + 1656], rax
  .section .rodata
  .Lrkfn80: .string "DIFFER"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn80]
 lea rsi, [rsp + 1632]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 1616], rax
 mov qword ptr [rsp + 1624], rdx
 cmp eax, 99
 je xchain0_n55_α
 jmp xchain0_n58_α
 xchain0_n56_β:
 jmp xchain0_n55_α
# IR_ASSIGN global
 xchain0_n57_α:
 mov rsi, qword ptr [rsp + 1920]
 mov rdx, qword ptr [rsp + 1928]
 mov rdi, qword ptr [rip + .Lx81_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 1904], rax
 mov qword ptr [rsp + 1912], rdx
 jmp xchain0_n37_α
 xchain0_n57_β:
 jmp xchain0_n37_α
.Lx81_0:
 .quad .Lx81_0_s
.Lx81_0_s:
 .string "OUTPUT"
# IR_LIT_STRING
 xchain0_n58_α:
 mov qword ptr [rsp + 1808], 1
 mov rax, qword ptr [rip + .Lx82_0]
 mov qword ptr [rsp + 1816], rax
 jmp xchain0_n59_α
 xchain0_n58_β:
 jmp xchain0_n55_α
.Lx82_0:
 .quad .Lx82_0_s
.Lx82_0_s:
 .string " "
# IR_VAR
 xchain0_n59_α:
 mov rax, qword ptr [rbx + 48]
 mov rdx, qword ptr [rbx + 56]
 mov qword ptr [rsp + 1840], rax
 mov qword ptr [rsp + 1848], rdx
 jmp xchain0_n60_α
 xchain0_n59_β:
 jmp xchain0_n55_α
 xchain0_n60_α:
# BOX IR_CALL DUPL(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+1808] -> [zr+1760]
 mov rax, qword ptr [rsp + 1808]
 mov qword ptr [rsp + 1760], rax
 mov rax, qword ptr [rsp + 1816]
 mov qword ptr [rsp + 1768], rax
# marshal arg1 = producer-box slot [zr+1840] -> [zr+1776]
 mov rax, qword ptr [rsp + 1840]
 mov qword ptr [rsp + 1776], rax
 mov rax, qword ptr [rsp + 1848]
 mov qword ptr [rsp + 1784], rax
  .section .rodata
  .Lrkfn85: .string "DUPL"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn85]
 lea rsi, [rsp + 1760]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 1744], rax
 mov qword ptr [rsp + 1752], rdx
 cmp eax, 99
 je xchain0_n55_α
 jmp xchain0_n61_α
 xchain0_n60_β:
 jmp xchain0_n55_α
 xchain0_n61_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rsp + 1616]
 mov rsi, qword ptr [rsp + 1624]
 mov rdx, qword ptr [rsp + 1744]
 mov rcx, qword ptr [rsp + 1752]
 call str_concat_d@PLT
 mov qword ptr [rsp + 1584], rax
 mov qword ptr [rsp + 1592], rdx
 jmp xchain0_n62_α
 xchain0_n61_β:
 jmp xchain0_n55_α
# IR_VAR
 xchain0_n62_α:
 mov rax, qword ptr [rbx + 128]
 mov rdx, qword ptr [rbx + 136]
 mov qword ptr [rsp + 1872], rax
 mov qword ptr [rsp + 1880], rdx
 jmp xchain0_n63_α
 xchain0_n62_β:
 jmp xchain0_n55_α
 xchain0_n63_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rsp + 1584]
 mov rsi, qword ptr [rsp + 1592]
 mov rdx, qword ptr [rsp + 1872]
 mov rcx, qword ptr [rsp + 1880]
 call str_concat_d@PLT
 mov qword ptr [rsp + 1552], rax
 mov qword ptr [rsp + 1560], rdx
 jmp xchain0_n64_α
 xchain0_n63_β:
 jmp xchain0_n55_α
# IR_ASSIGN global
 xchain0_n64_α:
 mov rsi, qword ptr [rsp + 1552]
 mov rdx, qword ptr [rsp + 1560]
 mov rdi, qword ptr [rip + .Lx89_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 1536], rax
 mov qword ptr [rsp + 1544], rdx
 jmp xchain0_n37_α
 xchain0_n64_β:
 jmp xchain0_n55_α
.Lx89_0:
 .quad .Lx89_0_s
.Lx89_0_s:
 .string "OUTPUT"
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rsp + 1960]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rsp + 1960]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
add rsp, 65544
ret
.section .rodata
.S0: .string "NH"
.S1: .string "CROSS"
.S2: .string ""
.S3: .string "NV"
.S4: .string "C"
.text
