  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "X"
  .align 8
__gva_names:
  .quad .Lgvan0
  .section .bss
  .align 16
__gva: .space 16, 0
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
  mov edx, 1
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
  mov qword ptr [r12 + 920], rsp
 push rsi
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, [rsp + 8]
 mov qword ptr [r12 + 912], rax
 pop rsi
main_α_body:
# IR_LIT_INTEGER
 xchain0_n0_α:
 mov qword ptr [r12 + 480], 6
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 488], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n1_α
.Lx1_0:
 .quad 1
 xchain0_n1_α:
# BOX IR_CALL SNO$STMT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+480] -> [zr+448]
 mov rax, qword ptr [r12 + 480]
 mov qword ptr [r12 + 448], rax
 mov rax, qword ptr [r12 + 488]
 mov qword ptr [r12 + 456], rax
  .section .rodata
  .Lrkfn3: .string "SNO$STMT"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn3]
 lea rsi, [r12 + 448]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 432], rax
 mov qword ptr [r12 + 440], rdx
 cmp eax, 99
 je xchain0_n2_α
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n2_α
# IR_LIT_INTEGER
 xchain0_n2_α:
 mov qword ptr [r12 + 112], 6
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [r12 + 120], rax
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n4_α
.Lx4_0:
 .quad 1
# IR_ASSIGN gva
 xchain0_n3_α:
 mov rax, qword ptr [r12 + 112]
 mov rdx, qword ptr [r12 + 120]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp xchain0_n4_α
# IR_LIT_INTEGER
 xchain0_n4_α:
 mov qword ptr [r12 + 560], 6
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [r12 + 568], rax
 jmp xchain0_n5_α
 xchain0_n4_β:
 jmp xchain0_n5_α
.Lx6_0:
 .quad 2
 xchain0_n5_α:
# BOX IR_CALL SNO$STMT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+560] -> [zr+528]
 mov rax, qword ptr [r12 + 560]
 mov qword ptr [r12 + 528], rax
 mov rax, qword ptr [r12 + 568]
 mov qword ptr [r12 + 536], rax
  .section .rodata
  .Lrkfn8: .string "SNO$STMT"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn8]
 lea rsi, [r12 + 528]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 512], rax
 mov qword ptr [r12 + 520], rdx
 cmp eax, 99
 je xchain0_n6_α
 jmp xchain0_n6_α
 xchain0_n5_β:
 jmp xchain0_n6_α
# IR_LIT_INTEGER
 xchain0_n6_α:
 mov qword ptr [r12 + 160], 6
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [r12 + 168], rax
 jmp xchain0_n7_α
 xchain0_n6_β:
 jmp xchain0_n8_α
.Lx9_0:
 .quad 2
# IR_ASSIGN gva
 xchain0_n7_α:
 mov rax, qword ptr [r12 + 160]
 mov rdx, qword ptr [r12 + 168]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 144], rax
 mov qword ptr [r12 + 152], rdx
 jmp xchain0_n8_α
 xchain0_n7_β:
 jmp xchain0_n8_α
# IR_LIT_INTEGER
 xchain0_n8_α:
 mov qword ptr [r12 + 640], 6
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [r12 + 648], rax
 jmp xchain0_n9_α
 xchain0_n8_β:
 jmp xchain0_n9_α
.Lx11_0:
 .quad 3
 xchain0_n9_α:
# BOX IR_CALL SNO$STMT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+640] -> [zr+608]
 mov rax, qword ptr [r12 + 640]
 mov qword ptr [r12 + 608], rax
 mov rax, qword ptr [r12 + 648]
 mov qword ptr [r12 + 616], rax
  .section .rodata
  .Lrkfn13: .string "SNO$STMT"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn13]
 lea rsi, [r12 + 608]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 592], rax
 mov qword ptr [r12 + 600], rdx
 cmp eax, 99
 je xchain0_n10_α
 jmp xchain0_n10_α
 xchain0_n9_β:
 jmp xchain0_n10_α
# IR_KEYWORD_SNOBOL4_read
 xchain0_n10_α:
 mov rdi, qword ptr [rip + .Lx14_0]
 call rt_keyword_read_snobol4@PLT
 mov qword ptr [r12 + 320], rax
 mov qword ptr [r12 + 328], rdx
 jmp xchain0_n11_α
 xchain0_n10_β:
 jmp xchain0_n14_α
.Lx14_0:
 .quad .Lx14_0_s
.Lx14_0_s:
 .string "STNO"
# IR_LIT_INTEGER
 xchain0_n11_α:
 mov qword ptr [r12 + 288], 6
 mov rax, qword ptr [rip + .Lx15_0]
 mov qword ptr [r12 + 296], rax
 jmp xchain0_n12_α
 xchain0_n11_β:
 jmp xchain0_n14_α
.Lx15_0:
 .quad 1
# IR_COERCE_NUMERIC
 xchain0_n12_α:
 mov eax, dword ptr [r12 + 320]
 cmp eax, 7
 je .Lx17_1
 cmp eax, 6
 jne .Lx17_0
 mov eax, dword ptr [r12 + 288]
 cmp eax, 6
 jne .Lx17_0
.Lx17_1:
 mov rax, qword ptr [r12 + 320]
 mov qword ptr [r12 + 256], rax
 mov rax, qword ptr [r12 + 328]
 mov qword ptr [r12 + 264], rax
 jmp .Lx17_2
.Lx17_0:
 lea rdi, [r12 + 320]
 lea rsi, [r12 + 288]
 lea rdx, [r12 + 256]
 mov rcx, 111
 call rt_coerce_num2_d@PLT
.Lx17_2:
 jmp xchain0_n13_α
 xchain0_n12_β:
 jmp xchain0_n14_α
# IR_COERCE_NUMERIC
 xchain0_n13_α:
 mov eax, dword ptr [r12 + 288]
 cmp eax, 7
 je .Lx19_1
 cmp eax, 6
 jne .Lx19_0
 mov eax, dword ptr [r12 + 320]
 cmp eax, 6
 jne .Lx19_0
.Lx19_1:
 mov rax, qword ptr [r12 + 288]
 mov qword ptr [r12 + 224], rax
 mov rax, qword ptr [r12 + 296]
 mov qword ptr [r12 + 232], rax
 jmp .Lx19_2
.Lx19_0:
 lea rdi, [r12 + 288]
 lea rsi, [r12 + 320]
 lea rdx, [r12 + 224]
 mov rcx, 112
 call rt_coerce_num2_d@PLT
.Lx19_2:
 jmp xchain0_n15_α
 xchain0_n13_β:
 jmp xchain0_n14_α
# IR_LIT_INTEGER
 xchain0_n14_α:
 mov qword ptr [r12 + 880], 6
 mov rax, qword ptr [rip + .Lx20_0]
 mov qword ptr [r12 + 888], rax
 jmp xchain0_n16_α
 xchain0_n14_β:
 jmp xchain0_n16_α
.Lx20_0:
 .quad 6
# IR_CMP_TEST
 xchain0_n15_α:
 lea rdi, [r12 + 256]
 lea rsi, [r12 + 224]
 call rt_cmp_d@PLT
 test eax, eax
 jle xchain0_n14_α
 mov qword ptr [r12 + 192], 0
 mov qword ptr [r12 + 200], 0
 jmp xchain0_n17_α
 xchain0_n15_β:
 jmp xchain0_n14_α
 xchain0_n16_α:
# BOX IR_CALL SNO$STMT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+880] -> [zr+848]
 mov rax, qword ptr [r12 + 880]
 mov qword ptr [r12 + 848], rax
 mov rax, qword ptr [r12 + 888]
 mov qword ptr [r12 + 856], rax
  .section .rodata
  .Lrkfn24: .string "SNO$STMT"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn24]
 lea rsi, [r12 + 848]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 832], rax
 mov qword ptr [r12 + 840], rdx
 cmp eax, 99
 je xchain0_n18_α
 jmp xchain0_n18_α
 xchain0_n16_β:
 jmp xchain0_n18_α
# IR_LIT_INTEGER
 xchain0_n17_α:
 mov qword ptr [r12 + 720], 6
 mov rax, qword ptr [rip + .Lx25_0]
 mov qword ptr [r12 + 728], rax
 jmp xchain0_n19_α
 xchain0_n17_β:
 jmp xchain0_n19_α
.Lx25_0:
 .quad 4
# IR_LIT_STRING
 xchain0_n18_α:
 mov qword ptr [r12 + 400], 1
 mov rax, qword ptr [rip + .Lx26_0]
 mov qword ptr [r12 + 408], rax
 jmp xchain0_n20_α
 xchain0_n18_β:
 jmp main_γ
.Lx26_0:
 .quad .Lx26_0_s
.Lx26_0_s:
 .string "wrong"
 xchain0_n19_α:
# BOX IR_CALL SNO$STMT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+720] -> [zr+688]
 mov rax, qword ptr [r12 + 720]
 mov qword ptr [r12 + 688], rax
 mov rax, qword ptr [r12 + 728]
 mov qword ptr [r12 + 696], rax
  .section .rodata
  .Lrkfn28: .string "SNO$STMT"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn28]
 lea rsi, [r12 + 688]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 672], rax
 mov qword ptr [r12 + 680], rdx
 cmp eax, 99
 je xchain0_n21_α
 jmp xchain0_n21_α
 xchain0_n19_β:
 jmp xchain0_n21_α
# IR_ASSIGN global
 xchain0_n20_α:
 mov rsi, qword ptr [r12 + 400]
 mov rdx, qword ptr [r12 + 408]
 mov rdi, qword ptr [rip + .Lx29_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 384], rax
 mov qword ptr [r12 + 392], rdx
 jmp main_γ
 xchain0_n20_β:
 jmp main_γ
.Lx29_0:
 .quad .Lx29_0_s
.Lx29_0_s:
 .string "OUTPUT"
# IR_LIT_STRING
 xchain0_n21_α:
 mov qword ptr [r12 + 352], 1
 mov rax, qword ptr [rip + .Lx30_0]
 mov qword ptr [r12 + 360], rax
 jmp xchain0_n22_α
 xchain0_n21_β:
 jmp xchain0_n23_α
.Lx30_0:
 .quad .Lx30_0_s
.Lx30_0_s:
 .string "stno ok"
# IR_ASSIGN global
 xchain0_n22_α:
 mov rsi, qword ptr [r12 + 352]
 mov rdx, qword ptr [r12 + 360]
 mov rdi, qword ptr [rip + .Lx31_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 336], rax
 mov qword ptr [r12 + 344], rdx
 jmp xchain0_n23_α
 xchain0_n22_β:
 jmp xchain0_n23_α
.Lx31_0:
 .quad .Lx31_0_s
.Lx31_0_s:
 .string "OUTPUT"
# IR_LIT_INTEGER
 xchain0_n23_α:
 mov qword ptr [r12 + 800], 6
 mov rax, qword ptr [rip + .Lx32_0]
 mov qword ptr [r12 + 808], rax
 jmp xchain0_n24_α
 xchain0_n23_β:
 jmp xchain0_n24_α
.Lx32_0:
 .quad 5
 xchain0_n24_α:
# BOX IR_CALL SNO$STMT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+800] -> [zr+768]
 mov rax, qword ptr [r12 + 800]
 mov qword ptr [r12 + 768], rax
 mov rax, qword ptr [r12 + 808]
 mov qword ptr [r12 + 776], rax
  .section .rodata
  .Lrkfn34: .string "SNO$STMT"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn34]
 lea rsi, [r12 + 768]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 752], rax
 mov qword ptr [r12 + 760], rdx
 cmp eax, 99
 je main_γ
 jmp main_γ
 xchain0_n24_β:
 jmp main_γ
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [r12 + 920]
pop r12
ret
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [r12 + 912]
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
main_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, qword ptr [r12 + 920]
pop r12
ret
