  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "H"
  .Lgvan1: .string "V"
  .Lgvan2: .string "HC"
  .Lgvan3: .string "CROSS"
  .Lgvan4: .string "NH"
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
  push rbp
  mov rbp, rsp
  push rdi
  push rsi
  call core_lib_init@PLT
  lea rdi, [rip + __gva_names]
  lea rsi, [rip + __gva]
  mov edx, 9
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
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 960], rax
 pop rsi
main_α_body:
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [r12 + 112], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 120], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n3_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "TRIM"
# IR_LIT_INTEGER
 xchain0_n1_α:
 mov qword ptr [r12 + 128], 6
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [r12 + 136], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n3_α
.Lx2_0:
 .quad 1
 xchain0_n2_α:
# BOX IR_CALL SNO$KWSET(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+112] -> [zr+80]
 mov rax, qword ptr [r12 + 112]
 mov qword ptr [r12 + 80], rax
 mov rax, qword ptr [r12 + 120]
 mov qword ptr [r12 + 88], rax
# marshal arg1 = producer-box slot [zr+128] -> [zr+96]
 mov rax, qword ptr [r12 + 128]
 mov qword ptr [r12 + 96], rax
 mov rax, qword ptr [r12 + 136]
 mov qword ptr [r12 + 104], rax
  .section .rodata
  .Lrkfn4: .string "SNO$KWSET"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn4]
 lea rsi, [r12 + 80]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 64], rax
 mov qword ptr [r12 + 72], rdx
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
 mov qword ptr [r12 + 160], rax
 mov qword ptr [r12 + 168], rdx
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp main_γ
.Lx5_0:
 .quad .Lx5_0_s
.Lx5_0_s:
 .string "INPUT"
# IR_ASSIGN gva
 xchain0_n4_α:
 mov rax, qword ptr [r12 + 160]
 mov rdx, qword ptr [r12 + 168]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 144], rax
 mov qword ptr [r12 + 152], rdx
 jmp xchain0_n5_α
 xchain0_n4_β:
 jmp main_γ
# IR_VAR
 xchain0_n5_α:
 mov rdi, qword ptr [rip + .Lx7_0]
 call NV_GET_fn@PLT
 cmp eax, 99
 je main_γ
 mov qword ptr [r12 + 192], rax
 mov qword ptr [r12 + 200], rdx
 jmp xchain0_n6_α
 xchain0_n5_β:
 jmp main_γ
.Lx7_0:
 .quad .Lx7_0_s
.Lx7_0_s:
 .string "INPUT"
# IR_ASSIGN gva
 xchain0_n6_α:
 mov rax, qword ptr [r12 + 192]
 mov rdx, qword ptr [r12 + 200]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 176], rax
 mov qword ptr [r12 + 184], rdx
 jmp xchain0_n7_α
 xchain0_n6_β:
 jmp main_γ
# IR_VAR
 xchain0_n7_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 224], rax
 mov qword ptr [r12 + 232], rdx
 jmp xchain0_n8_α
 xchain0_n7_β:
 jmp xchain0_n9_α
# IR_ASSIGN gva
 xchain0_n8_α:
 mov rax, qword ptr [r12 + 224]
 mov rdx, qword ptr [r12 + 232]
 mov qword ptr [rbx + 32], rax
 mov qword ptr [rbx + 40], rdx
 mov qword ptr [r12 + 208], rax
 mov qword ptr [r12 + 216], rdx
 jmp xchain0_n9_α
 xchain0_n8_β:
 jmp xchain0_n9_α
# IR_VAR
 xchain0_n9_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 336], rax
 mov qword ptr [r12 + 344], rdx
 jmp xchain0_n10_α
 xchain0_n9_β:
 jmp xchain0_n3_α
# IR_VAR
 xchain0_n10_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 320], rax
 mov qword ptr [r12 + 328], rdx
 jmp xchain0_n11_α
 xchain0_n10_β:
 jmp xchain0_n3_α
# IR_COERCE_STRING
 xchain0_n11_α:
 lea rdi, [r12 + 320]
 lea rsi, [r12 + 304]
 mov rdx, 3866683
 call rt_coerce_str_d@PLT
 jmp xchain0_n12_α
 xchain0_n11_β:
 jmp xchain0_n3_α
# IR_MATCH_HEAD
 xchain0_n12_α:
 call rt_zls_mark@PLT
 mov qword ptr [r12 + 248], rax
 mov qword ptr [r12 + 256], rsp
 mov rdi, qword ptr [r12 + 336]
 mov rsi, qword ptr [r12 + 344]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov dword ptr [r12 + 240], 0
.Lx16_0:
 mov r14d, dword ptr [r12 + 240]
 jmp xchain0_n13_α
 xchain0_n12_β:
 add dword ptr [r12 + 240], 1
 mov eax, dword ptr [r12 + 240]
 cmp eax, r15d
 jg .Lx16_1
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne .Lx16_1
 jmp .Lx16_0
.Lx16_1:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 248]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
 mov rsp, qword ptr [r12 + 256]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_end_fail@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n3_α
# IR_MATCH_ATP
 xchain0_n13_α:
 mov esi, r14d
 lea rdi, [rip + .S0]
 sub rsp, 8
 call rt_at_cursor@PLT
 add rsp, 8
 jmp xchain0_n14_α
 xchain0_n13_β:
 jmp xchain0_n12_β
# IR_MATCH_CAPTURE_SAVE push
 xchain0_n14_α:
 lea rdi, [r12 + 288]
 mov esi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_cap_push@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n15_α
 xchain0_n14_β:
 lea rdi, [r12 + 288]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_cap_pop@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n13_β
# IR_MATCH_ANY
 xchain0_n15_α:
 mov eax, r14d
 cmp eax, r15d
 jge xchain0_n14_β
 movsxd rcx, r14d
 movzx esi, byte ptr [r13+rcx]
 mov rdi, qword ptr [r12 + 312]
 sub rsp, 8
 call strchr@PLT
 add rsp, 8
 test rax, rax
 je xchain0_n14_β
 add r14d, 1
 jmp xchain0_n16_α
 xchain0_n15_β:
 sub r14d, 1
 jmp xchain0_n14_β
# IR_MATCH_CAPTURE_COND
 xchain0_n16_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 lea rdi, [r12 + 288]
 call rt_cap_top@PLT
 lea rdi, [rip + .S2]
 mov esi, eax
 mov edx, r14d
 mov ecx, 0
 call rt_cap_open@PLT
 test rax, rax
 je .Lx22_1
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_cap_finish@PLT
.Lx22_1:
 mov rsp, rbp
 pop rbp
 jmp xchain0_n17_α
 xchain0_n16_β:
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_pop@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n15_β
# IR_MATCH_RELEASE
 xchain0_n17_α:
 mov qword ptr [r12 + 264], r14
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 248]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
 mov rsp, qword ptr [r12 + 256]
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rsp, rbp
 pop rbp
 push r14
 push r15
 push r13
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_end_ok_open@PLT
.Lx24_1:
 test rax, rax
 je .Lx24_2
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
 jmp .Lx24_1
.Lx24_2:
 call rt_dcap_end_ok_close@PLT
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 jmp xchain0_n18_α
# IR_LIT_STRING
 xchain0_n18_α:
 mov qword ptr [r12 + 272], 1
 mov rax, qword ptr [rip + .Lx25_0]
 mov qword ptr [r12 + 280], rax
 jmp xchain0_n19_α
 xchain0_n18_β:
 jmp xchain0_n3_α
.Lx25_0:
 .quad .Lx25_0_s
.Lx25_0_s:
 .string "*"
# IR_MATCH_REPLACE
 xchain0_n19_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [rip + .Lx27_0]
 mov rsi, qword ptr [r12 + 336]
 mov rdx, qword ptr [r12 + 344]
 mov ecx, dword ptr [r12 + 240]
 mov r8, qword ptr [r12 + 264]
 lea r9, [r12 + 272]
 call rt_match_replace@PLT
 mov rsp, rbp
 pop rbp
 jmp .Lx27_1
.Lx27_0:
 .quad .Lx27_0_s
.Lx27_0_s:
 .string "HC"
.Lx27_1:
 jmp xchain0_n20_α
# IR_VAR
 xchain0_n20_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 368], rax
 mov qword ptr [r12 + 376], rdx
 jmp xchain0_n21_α
 xchain0_n20_β:
 jmp xchain0_n22_α
# IR_ASSIGN gva
 xchain0_n21_α:
 mov rax, qword ptr [r12 + 368]
 mov rdx, qword ptr [r12 + 376]
 mov qword ptr [rbx + 80], rax
 mov qword ptr [rbx + 88], rdx
 mov qword ptr [r12 + 352], rax
 mov qword ptr [r12 + 360], rdx
 jmp xchain0_n22_α
 xchain0_n21_β:
 jmp xchain0_n22_α
# IR_VAR
 xchain0_n22_α:
 mov rax, qword ptr [rbx + 80]
 mov rdx, qword ptr [rbx + 88]
 mov qword ptr [r12 + 448], rax
 mov qword ptr [r12 + 456], rdx
 jmp xchain0_n23_α
 xchain0_n22_β:
 jmp xchain0_n9_α
# IR_MATCH_HEAD
 xchain0_n23_α:
 call rt_zls_mark@PLT
 mov qword ptr [r12 + 392], rax
 mov qword ptr [r12 + 400], rsp
 mov rdi, qword ptr [r12 + 448]
 mov rsi, qword ptr [r12 + 456]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov dword ptr [r12 + 384], 0
.Lx32_0:
 mov r14d, dword ptr [r12 + 384]
 jmp xchain0_n24_α
 xchain0_n23_β:
 add dword ptr [r12 + 384], 1
 mov eax, dword ptr [r12 + 384]
 cmp eax, r15d
 jg .Lx32_1
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne .Lx32_1
 jmp .Lx32_0
.Lx32_1:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 392]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
 mov rsp, qword ptr [r12 + 400]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_end_fail@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n9_α
# IR_MATCH_ATP
 xchain0_n24_α:
 mov esi, r14d
 lea rdi, [rip + .S3]
 sub rsp, 8
 call rt_at_cursor@PLT
 add rsp, 8
 jmp xchain0_n25_α
 xchain0_n24_β:
 jmp xchain0_n23_β
# IR_MATCH_DEFER
 xchain0_n25_α:
 lea rdi, [rip + .S2]
 xor esi, esi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_get_pat_fn@PLT
 mov rsp, rbp
 pop rbp
 xor ecx, ecx
 mov qword ptr [r12 + 432], rcx
 test rax, rax
 jz .Lx34_0
 mov qword ptr [r12 + 432], rax
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, 65536
 call rt_zls_alloc@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 440], rax
 mov rcx, qword ptr [r12 + 432]
 mov rdi, rax
 xor esi, esi
 call rcx
 cmp eax, 1
 je .Lx34_1
 mov rdi, qword ptr [r12 + 440]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 432], rax
 jmp xchain0_n24_β
.Lx34_1:
 jmp xchain0_n26_α
.Lx34_0:
 push r14
 push r15
 push r13
 lea rdi, [rip + .S2]
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
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_defer_close@PLT
 mov rsp, rbp
 pop rbp
 test eax, eax
 js xchain0_n24_β
 mov r14d, eax
 jmp xchain0_n26_α
 xchain0_n25_β:
 mov rcx, qword ptr [r12 + 432]
 test rcx, rcx
 jz xchain0_n24_β
 mov rdi, qword ptr [r12 + 440]
 mov esi, 1
 call rcx
 cmp eax, 1
 je .Lx34_1
 mov rdi, qword ptr [r12 + 440]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_release@PLT
 mov rsp, rbp
 pop rbp
 xor eax, eax
 mov qword ptr [r12 + 432], rax
 jmp xchain0_n24_β
# IR_MATCH_RELEASE
 xchain0_n26_α:
 mov qword ptr [r12 + 408], r14
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 392]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
 mov rsp, qword ptr [r12 + 400]
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rsp, rbp
 pop rbp
 push r14
 push r15
 push r13
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_end_ok_open@PLT
.Lx36_1:
 test rax, rax
 je .Lx36_2
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
 jmp .Lx36_1
.Lx36_2:
 call rt_dcap_end_ok_close@PLT
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 jmp xchain0_n27_α
# IR_LIT_STRING
 xchain0_n27_α:
 mov qword ptr [r12 + 416], 1
 mov rax, qword ptr [rip + .Lx37_0]
 mov qword ptr [r12 + 424], rax
 jmp xchain0_n28_α
 xchain0_n27_β:
 jmp xchain0_n9_α
.Lx37_0:
 .quad .Lx37_0_s
.Lx37_0_s:
 .string "#"
# IR_MATCH_REPLACE
 xchain0_n28_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [rip + .Lx39_0]
 mov rsi, qword ptr [r12 + 448]
 mov rdx, qword ptr [r12 + 456]
 mov ecx, dword ptr [r12 + 384]
 mov r8, qword ptr [r12 + 408]
 lea r9, [r12 + 416]
 call rt_match_replace@PLT
 mov rsp, rbp
 pop rbp
 jmp .Lx39_1
.Lx39_0:
 .quad .Lx39_0_s
.Lx39_0_s:
 .string "VC"
.Lx39_1:
 jmp xchain0_n29_α
# IR_LIT_STRING
 xchain0_n29_α:
 mov qword ptr [r12 + 480], 1
 mov rax, qword ptr [rip + .Lx40_0]
 mov qword ptr [r12 + 488], rax
 jmp xchain0_n30_α
 xchain0_n29_β:
 jmp xchain0_n31_α
.Lx40_0:
 .quad .Lx40_0_s
.Lx40_0_s:
 .string ""
# IR_ASSIGN global
 xchain0_n30_α:
 mov rsi, qword ptr [r12 + 480]
 mov rdx, qword ptr [r12 + 488]
 mov rdi, qword ptr [rip + .Lx41_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 464], rax
 mov qword ptr [r12 + 472], rdx
 jmp xchain0_n31_α
 xchain0_n30_β:
 jmp xchain0_n31_α
.Lx41_0:
 .quad .Lx41_0_s
.Lx41_0_s:
 .string "OUTPUT"
# IR_VAR
 xchain0_n31_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 512], rax
 mov qword ptr [r12 + 520], rdx
 jmp xchain0_n32_α
 xchain0_n31_β:
 jmp xchain0_n33_α
# IR_ASSIGN gva
 xchain0_n32_α:
 mov rax, qword ptr [r12 + 512]
 mov rdx, qword ptr [r12 + 520]
 mov qword ptr [rbx + 112], rax
 mov qword ptr [rbx + 120], rdx
 mov qword ptr [r12 + 496], rax
 mov qword ptr [r12 + 504], rdx
 jmp xchain0_n33_α
 xchain0_n32_β:
 jmp xchain0_n33_α
# IR_VAR
 xchain0_n33_α:
 mov rax, qword ptr [rbx + 112]
 mov rdx, qword ptr [rbx + 120]
 mov qword ptr [r12 + 608], rax
 mov qword ptr [r12 + 616], rdx
 jmp xchain0_n34_α
 xchain0_n33_β:
 jmp xchain0_n35_α
# IR_VAR
 xchain0_n34_α:
 mov rax, qword ptr [rbx + 96]
 mov rdx, qword ptr [rbx + 104]
 mov qword ptr [r12 + 592], rax
 mov qword ptr [r12 + 600], rdx
 jmp xchain0_n36_α
 xchain0_n34_β:
 jmp xchain0_n35_α
# IR_VAR
 xchain0_n35_α:
 mov rax, qword ptr [rbx + 112]
 mov rdx, qword ptr [rbx + 120]
 mov qword ptr [r12 + 688], rax
 mov qword ptr [r12 + 696], rdx
 jmp xchain0_n37_α
 xchain0_n35_β:
 jmp xchain0_n22_α
# IR_COERCE_INTEGER
 xchain0_n36_α:
 lea rdi, [r12 + 592]
 lea rsi, [r12 + 576]
 mov rdx, 10682530
 call rt_coerce_int_d@PLT
 jmp xchain0_n38_α
 xchain0_n36_β:
 jmp xchain0_n35_α
# IR_MATCH_HEAD
 xchain0_n37_α:
 call rt_zls_mark@PLT
 mov qword ptr [r12 + 632], rax
 mov qword ptr [r12 + 640], rsp
 mov rdi, qword ptr [r12 + 688]
 mov rsi, qword ptr [r12 + 696]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov dword ptr [r12 + 624], 0
.Lx50_0:
 mov r14d, dword ptr [r12 + 624]
 jmp xchain0_n39_α
 xchain0_n37_β:
 add dword ptr [r12 + 624], 1
 mov eax, dword ptr [r12 + 624]
 cmp eax, r15d
 jg .Lx50_1
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne .Lx50_1
 jmp .Lx50_0
.Lx50_1:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 632]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
 mov rsp, qword ptr [r12 + 640]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_end_fail@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n22_α
# IR_MATCH_HEAD
 xchain0_n38_α:
 call rt_zls_mark@PLT
 mov qword ptr [r12 + 536], rax
 mov qword ptr [r12 + 544], rsp
 mov rdi, qword ptr [r12 + 608]
 mov rsi, qword ptr [r12 + 616]
 call rt_match_enter@PLT
 mov r13, rax
 mov r15, rdx
 mov dword ptr [r12 + 528], 0
.Lx52_0:
 mov r14d, dword ptr [r12 + 528]
 jmp xchain0_n40_α
 xchain0_n38_β:
 add dword ptr [r12 + 528], 1
 mov eax, dword ptr [r12 + 528]
 cmp eax, r15d
 jg .Lx52_1
 lea rcx, [rip + g_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne .Lx52_1
 jmp .Lx52_0
.Lx52_1:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 536]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
 mov rsp, qword ptr [r12 + 544]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_end_fail@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n35_α
# IR_MATCH_CAPTURE_SAVE push
 xchain0_n39_α:
 lea rdi, [r12 + 672]
 mov esi, r14d
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_cap_push@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n41_α
 xchain0_n39_β:
 lea rdi, [r12 + 672]
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_cap_pop@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n37_β
# IR_MATCH_POS
 xchain0_n40_α:
 mov rax, qword ptr [r12 + 584]
 cmp r14d, eax
 jne xchain0_n38_β
 jmp xchain0_n42_α
 xchain0_n40_β:
 jmp xchain0_n38_β
# IR_MATCH_LEN
 xchain0_n41_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain0_n39_β
 add r14d, 1
 jmp xchain0_n43_α
 xchain0_n41_β:
 sub r14d, 1
 jmp xchain0_n39_β
# IR_MATCH_LEN
 xchain0_n42_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xchain0_n40_β
 add r14d, 1
 jmp xchain0_n44_α
 xchain0_n42_β:
 sub r14d, 1
 jmp xchain0_n40_β
# IR_MATCH_CAPTURE_COND
 xchain0_n43_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 lea rdi, [r12 + 672]
 call rt_cap_top@PLT
 lea rdi, [rip + .S4]
 mov esi, eax
 mov edx, r14d
 mov ecx, 0
 call rt_cap_open@PLT
 test rax, rax
 je .Lx59_1
 sub rsp, rax
 mov rdi, rsp
 mov rsi, rax
 call rt_frame_prep@PLT
 mov rdi, rsp
 xor esi, esi
 call rax
 mov rdi, rax
 mov rsi, rdx
 call rt_cap_finish@PLT
.Lx59_1:
 mov rsp, rbp
 pop rbp
 jmp xchain0_n45_α
 xchain0_n43_β:
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_pop@PLT
 mov rsp, rbp
 pop rbp
 jmp xchain0_n41_β
# IR_MATCH_RELEASE
 xchain0_n44_α:
 mov qword ptr [r12 + 552], r14
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 536]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
 mov rsp, qword ptr [r12 + 544]
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rsp, rbp
 pop rbp
 push r14
 push r15
 push r13
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_end_ok_open@PLT
.Lx61_1:
 test rax, rax
 je .Lx61_2
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
 jmp .Lx61_1
.Lx61_2:
 call rt_dcap_end_ok_close@PLT
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 jmp xchain0_n46_α
# IR_MATCH_RELEASE
 xchain0_n45_α:
 mov qword ptr [r12 + 648], r14
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 632]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
 mov rsp, qword ptr [r12 + 640]
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rsp, rbp
 pop rbp
 push r14
 push r15
 push r13
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_dcap_end_ok_open@PLT
.Lx63_1:
 test rax, rax
 je .Lx63_2
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
 jmp .Lx63_1
.Lx63_2:
 call rt_dcap_end_ok_close@PLT
 mov rsp, rbp
 pop rbp
 pop r13
 pop r15
 pop r14
 jmp xchain0_n47_α
# IR_LIT_STRING
 xchain0_n46_α:
 mov qword ptr [r12 + 560], 1
 mov rax, qword ptr [rip + .Lx64_0]
 mov qword ptr [r12 + 568], rax
 jmp xchain0_n48_α
 xchain0_n46_β:
 jmp xchain0_n35_α
.Lx64_0:
 .quad .Lx64_0_s
.Lx64_0_s:
 .string "#"
# IR_LIT_STRING
 xchain0_n47_α:
 mov qword ptr [r12 + 656], 1
 mov rax, qword ptr [rip + .Lx65_0]
 mov qword ptr [r12 + 664], rax
 jmp xchain0_n49_α
 xchain0_n47_β:
 jmp xchain0_n22_α
.Lx65_0:
 .quad .Lx65_0_s
.Lx65_0_s:
 .string ""
# IR_MATCH_REPLACE
 xchain0_n48_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [rip + .Lx67_0]
 mov rsi, qword ptr [r12 + 608]
 mov rdx, qword ptr [r12 + 616]
 mov ecx, dword ptr [r12 + 528]
 mov r8, qword ptr [r12 + 552]
 lea r9, [r12 + 560]
 call rt_match_replace@PLT
 mov rsp, rbp
 pop rbp
 jmp .Lx67_1
.Lx67_0:
 .quad .Lx67_0_s
.Lx67_0_s:
 .string "PRINTV"
.Lx67_1:
 jmp xchain0_n35_α
# IR_MATCH_REPLACE
 xchain0_n49_α:
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [rip + .Lx69_0]
 mov rsi, qword ptr [r12 + 688]
 mov rdx, qword ptr [r12 + 696]
 mov ecx, dword ptr [r12 + 624]
 mov r8, qword ptr [r12 + 648]
 lea r9, [r12 + 656]
 call rt_match_replace@PLT
 mov rsp, rbp
 pop rbp
 jmp .Lx69_1
.Lx69_0:
 .quad .Lx69_0_s
.Lx69_0_s:
 .string "PRINTV"
.Lx69_1:
 jmp xchain0_n50_α
# IR_VAR
 xchain0_n50_α:
 mov rax, qword ptr [rbx + 128]
 mov rdx, qword ptr [rbx + 136]
 mov qword ptr [r12 + 800], rax
 mov qword ptr [r12 + 808], rdx
 jmp xchain0_n51_α
 xchain0_n50_β:
 jmp xchain0_n52_α
# IR_LIT_STRING
 xchain0_n51_α:
 mov qword ptr [r12 + 816], 1
 mov rax, qword ptr [rip + .Lx71_0]
 mov qword ptr [r12 + 824], rax
 jmp xchain0_n53_α
 xchain0_n51_β:
 jmp xchain0_n52_α
.Lx71_0:
 .quad .Lx71_0_s
.Lx71_0_s:
 .string "#"
# IR_VAR
 xchain0_n52_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 944], rax
 mov qword ptr [r12 + 952], rdx
 jmp xchain0_n54_α
 xchain0_n52_β:
 jmp xchain0_n35_α
 xchain0_n53_α:
# BOX IR_CALL DIFFER(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+800] -> [zr+768]
 mov rax, qword ptr [r12 + 800]
 mov qword ptr [r12 + 768], rax
 mov rax, qword ptr [r12 + 808]
 mov qword ptr [r12 + 776], rax
# marshal arg1 = producer-box slot [zr+816] -> [zr+784]
 mov rax, qword ptr [r12 + 816]
 mov qword ptr [r12 + 784], rax
 mov rax, qword ptr [r12 + 824]
 mov qword ptr [r12 + 792], rax
  .section .rodata
  .Lrkfn74: .string "DIFFER"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn74]
 lea rsi, [r12 + 768]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 752], rax
 mov qword ptr [r12 + 760], rdx
 cmp eax, 99
 je xchain0_n52_α
 jmp xchain0_n55_α
 xchain0_n53_β:
 jmp xchain0_n52_α
# IR_ASSIGN global
 xchain0_n54_α:
 mov rsi, qword ptr [r12 + 944]
 mov rdx, qword ptr [r12 + 952]
 mov rdi, qword ptr [rip + .Lx75_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 928], rax
 mov qword ptr [r12 + 936], rdx
 jmp xchain0_n35_α
 xchain0_n54_β:
 jmp xchain0_n35_α
.Lx75_0:
 .quad .Lx75_0_s
.Lx75_0_s:
 .string "OUTPUT"
# IR_LIT_STRING
 xchain0_n55_α:
 mov qword ptr [r12 + 880], 1
 mov rax, qword ptr [rip + .Lx76_0]
 mov qword ptr [r12 + 888], rax
 jmp xchain0_n56_α
 xchain0_n55_β:
 jmp xchain0_n52_α
.Lx76_0:
 .quad .Lx76_0_s
.Lx76_0_s:
 .string " "
# IR_VAR
 xchain0_n56_α:
 mov rax, qword ptr [rbx + 64]
 mov rdx, qword ptr [rbx + 72]
 mov qword ptr [r12 + 896], rax
 mov qword ptr [r12 + 904], rdx
 jmp xchain0_n57_α
 xchain0_n56_β:
 jmp xchain0_n52_α
 xchain0_n57_α:
# BOX IR_CALL DUPL(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+880] -> [zr+848]
 mov rax, qword ptr [r12 + 880]
 mov qword ptr [r12 + 848], rax
 mov rax, qword ptr [r12 + 888]
 mov qword ptr [r12 + 856], rax
# marshal arg1 = producer-box slot [zr+896] -> [zr+864]
 mov rax, qword ptr [r12 + 896]
 mov qword ptr [r12 + 864], rax
 mov rax, qword ptr [r12 + 904]
 mov qword ptr [r12 + 872], rax
  .section .rodata
  .Lrkfn79: .string "DUPL"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn79]
 lea rsi, [r12 + 848]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 832], rax
 mov qword ptr [r12 + 840], rdx
 cmp eax, 99
 je xchain0_n52_α
 jmp xchain0_n58_α
 xchain0_n57_β:
 jmp xchain0_n52_α
 xchain0_n58_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 752]
 mov rsi, qword ptr [r12 + 760]
 mov rdx, qword ptr [r12 + 832]
 mov rcx, qword ptr [r12 + 840]
 call str_concat_d@PLT
 mov qword ptr [r12 + 736], rax
 mov qword ptr [r12 + 744], rdx
 jmp xchain0_n59_α
 xchain0_n58_β:
 jmp xchain0_n52_α
# IR_VAR
 xchain0_n59_α:
 mov rax, qword ptr [rbx + 128]
 mov rdx, qword ptr [rbx + 136]
 mov qword ptr [r12 + 912], rax
 mov qword ptr [r12 + 920], rdx
 jmp xchain0_n60_α
 xchain0_n59_β:
 jmp xchain0_n52_α
 xchain0_n60_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 736]
 mov rsi, qword ptr [r12 + 744]
 mov rdx, qword ptr [r12 + 912]
 mov rcx, qword ptr [r12 + 920]
 call str_concat_d@PLT
 mov qword ptr [r12 + 720], rax
 mov qword ptr [r12 + 728], rdx
 jmp xchain0_n61_α
 xchain0_n60_β:
 jmp xchain0_n52_α
# IR_ASSIGN global
 xchain0_n61_α:
 mov rsi, qword ptr [r12 + 720]
 mov rdx, qword ptr [r12 + 728]
 mov rdi, qword ptr [rip + .Lx83_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 704], rax
 mov qword ptr [r12 + 712], rdx
 jmp xchain0_n35_α
 xchain0_n61_β:
 jmp xchain0_n52_α
.Lx83_0:
 .quad .Lx83_0_s
.Lx83_0_s:
 .string "OUTPUT"
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 960]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
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
.S0: .string "NH"
.S1: .string ""
.S2: .string "CROSS"
.S3: .string "NV"
.S4: .string "C"
.text
