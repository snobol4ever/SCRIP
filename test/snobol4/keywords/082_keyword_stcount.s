  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "X"
  .align 8
__gva_names:
  .quad .Lgvan0
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
  mov edi, 1
  call rt_gva_island@PLT
  mov rsi, rax
  lea rdi, [rip + __gva_names]
  mov edx, 1
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
  mov qword ptr [rsp + 920], rsp
  mov r12, qword ptr [1879048192]
  mov rbp, rsp
main_α_body:
# IR_LIT_INTEGER
 xchain0_n0_α:
 mov qword ptr [rsp + 480], 6
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [rsp + 488], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n1_α
.Lx1_0:
 .quad 1
 xchain0_n1_α:
# BOX IR_CALL SNO$STMT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+480] -> [zr+448]
 mov rax, qword ptr [rsp + 480]
 mov qword ptr [rsp + 448], rax
 mov rax, qword ptr [rsp + 488]
 mov qword ptr [rsp + 456], rax
  .section .rodata
  .Lrkfn3: .string "SNO$STMT"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn3]
 lea rsi, [rsp + 448]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rsp + 432], rax
 mov qword ptr [rsp + 440], rdx
 cmp eax, 99
 je xchain0_n2_α
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n2_α
# IR_LIT_INTEGER
 xchain0_n2_α:
 mov qword ptr [rsp + 112], 6
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [rsp + 120], rax
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n4_α
.Lx4_0:
 .quad 1
# IR_ASSIGN gva
 xchain0_n3_α:
 mov rax, qword ptr [rsp + 112]
 mov rdx, qword ptr [rsp + 120]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rsp + 96], rax
 mov qword ptr [rsp + 104], rdx
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp xchain0_n4_α
# IR_LIT_INTEGER
 xchain0_n4_α:
 mov qword ptr [rsp + 560], 6
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [rsp + 568], rax
 jmp xchain0_n5_α
 xchain0_n4_β:
 jmp xchain0_n5_α
.Lx6_0:
 .quad 2
 xchain0_n5_α:
# BOX IR_CALL SNO$STMT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+560] -> [zr+528]
 mov rax, qword ptr [rsp + 560]
 mov qword ptr [rsp + 528], rax
 mov rax, qword ptr [rsp + 568]
 mov qword ptr [rsp + 536], rax
  .section .rodata
  .Lrkfn8: .string "SNO$STMT"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn8]
 lea rsi, [rsp + 528]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rsp + 512], rax
 mov qword ptr [rsp + 520], rdx
 cmp eax, 99
 je xchain0_n6_α
 jmp xchain0_n6_α
 xchain0_n5_β:
 jmp xchain0_n6_α
# IR_LIT_INTEGER
 xchain0_n6_α:
 mov qword ptr [rsp + 160], 6
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [rsp + 168], rax
 jmp xchain0_n7_α
 xchain0_n6_β:
 jmp xchain0_n8_α
.Lx9_0:
 .quad 2
# IR_ASSIGN gva
 xchain0_n7_α:
 mov rax, qword ptr [rsp + 160]
 mov rdx, qword ptr [rsp + 168]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rsp + 144], rax
 mov qword ptr [rsp + 152], rdx
 jmp xchain0_n8_α
 xchain0_n7_β:
 jmp xchain0_n8_α
# IR_LIT_INTEGER
 xchain0_n8_α:
 mov qword ptr [rsp + 640], 6
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [rsp + 648], rax
 jmp xchain0_n9_α
 xchain0_n8_β:
 jmp xchain0_n9_α
.Lx11_0:
 .quad 3
 xchain0_n9_α:
# BOX IR_CALL SNO$STMT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+640] -> [zr+608]
 mov rax, qword ptr [rsp + 640]
 mov qword ptr [rsp + 608], rax
 mov rax, qword ptr [rsp + 648]
 mov qword ptr [rsp + 616], rax
  .section .rodata
  .Lrkfn13: .string "SNO$STMT"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn13]
 lea rsi, [rsp + 608]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rsp + 592], rax
 mov qword ptr [rsp + 600], rdx
 cmp eax, 99
 je xchain0_n10_α
 jmp xchain0_n10_α
 xchain0_n9_β:
 jmp xchain0_n10_α
# IR_KEYWORD_SNOBOL4_read
 xchain0_n10_α:
 mov rdi, qword ptr [rip + .Lx14_0]
 call rt_keyword_read_snobol4@PLT
 mov qword ptr [rsp + 320], rax
 mov qword ptr [rsp + 328], rdx
 jmp xchain0_n11_α
 xchain0_n10_β:
 jmp xchain0_n14_α
.Lx14_0:
 .quad .Lx14_0_s
.Lx14_0_s:
 .string "STNO"
# IR_LIT_INTEGER
 xchain0_n11_α:
 mov qword ptr [rsp + 288], 6
 mov rax, qword ptr [rip + .Lx15_0]
 mov qword ptr [rsp + 296], rax
 jmp xchain0_n12_α
 xchain0_n11_β:
 jmp xchain0_n14_α
.Lx15_0:
 .quad 1
# IR_COERCE_NUMERIC
 xchain0_n12_α:
 mov eax, dword ptr [rsp + 320]
 cmp eax, 7
 je .Lx17_1
 cmp eax, 6
 jne .Lx17_0
 mov eax, dword ptr [rsp + 288]
 cmp eax, 6
 jne .Lx17_0
.Lx17_1:
 mov rax, qword ptr [rsp + 320]
 mov qword ptr [rsp + 256], rax
 mov rax, qword ptr [rsp + 328]
 mov qword ptr [rsp + 264], rax
 jmp .Lx17_2
.Lx17_0:
 lea rdi, [rsp + 320]
 lea rsi, [rsp + 288]
 lea rdx, [rsp + 256]
 mov rcx, 111
 call rt_coerce_num2_d@PLT
.Lx17_2:
 jmp xchain0_n13_α
 xchain0_n12_β:
 jmp xchain0_n14_α
# IR_COERCE_NUMERIC
 xchain0_n13_α:
 mov eax, dword ptr [rsp + 288]
 cmp eax, 7
 je .Lx19_1
 cmp eax, 6
 jne .Lx19_0
 mov eax, dword ptr [rsp + 320]
 cmp eax, 6
 jne .Lx19_0
.Lx19_1:
 mov rax, qword ptr [rsp + 288]
 mov qword ptr [rsp + 224], rax
 mov rax, qword ptr [rsp + 296]
 mov qword ptr [rsp + 232], rax
 jmp .Lx19_2
.Lx19_0:
 lea rdi, [rsp + 288]
 lea rsi, [rsp + 320]
 lea rdx, [rsp + 224]
 mov rcx, 112
 call rt_coerce_num2_d@PLT
.Lx19_2:
 jmp xchain0_n15_α
 xchain0_n13_β:
 jmp xchain0_n14_α
# IR_LIT_INTEGER
 xchain0_n14_α:
 mov qword ptr [rsp + 880], 6
 mov rax, qword ptr [rip + .Lx20_0]
 mov qword ptr [rsp + 888], rax
 jmp xchain0_n16_α
 xchain0_n14_β:
 jmp xchain0_n16_α
.Lx20_0:
 .quad 6
# IR_CMP_TEST
 xchain0_n15_α:
 lea rdi, [rsp + 256]
 lea rsi, [rsp + 224]
 call rt_cmp_d@PLT
 test eax, eax
 jle xchain0_n14_α
 mov qword ptr [rsp + 192], 0
 mov qword ptr [rsp + 200], 0
 jmp xchain0_n17_α
 xchain0_n15_β:
 jmp xchain0_n14_α
 xchain0_n16_α:
# BOX IR_CALL SNO$STMT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+880] -> [zr+848]
 mov rax, qword ptr [rsp + 880]
 mov qword ptr [rsp + 848], rax
 mov rax, qword ptr [rsp + 888]
 mov qword ptr [rsp + 856], rax
  .section .rodata
  .Lrkfn24: .string "SNO$STMT"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn24]
 lea rsi, [rsp + 848]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rsp + 832], rax
 mov qword ptr [rsp + 840], rdx
 cmp eax, 99
 je xchain0_n18_α
 jmp xchain0_n18_α
 xchain0_n16_β:
 jmp xchain0_n18_α
# IR_LIT_INTEGER
 xchain0_n17_α:
 mov qword ptr [rsp + 720], 6
 mov rax, qword ptr [rip + .Lx25_0]
 mov qword ptr [rsp + 728], rax
 jmp xchain0_n19_α
 xchain0_n17_β:
 jmp xchain0_n19_α
.Lx25_0:
 .quad 4
# IR_LIT_STRING
 xchain0_n18_α:
 mov qword ptr [rsp + 400], 1
 mov rax, qword ptr [rip + .Lx26_0]
 mov qword ptr [rsp + 408], rax
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
 mov rax, qword ptr [rsp + 720]
 mov qword ptr [rsp + 688], rax
 mov rax, qword ptr [rsp + 728]
 mov qword ptr [rsp + 696], rax
  .section .rodata
  .Lrkfn28: .string "SNO$STMT"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn28]
 lea rsi, [rsp + 688]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rsp + 672], rax
 mov qword ptr [rsp + 680], rdx
 cmp eax, 99
 je xchain0_n21_α
 jmp xchain0_n21_α
 xchain0_n19_β:
 jmp xchain0_n21_α
# IR_ASSIGN global
 xchain0_n20_α:
 mov rsi, qword ptr [rsp + 400]
 mov rdx, qword ptr [rsp + 408]
 mov rdi, qword ptr [rip + .Lx29_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 384], rax
 mov qword ptr [rsp + 392], rdx
 jmp main_γ
 xchain0_n20_β:
 jmp main_γ
.Lx29_0:
 .quad .Lx29_0_s
.Lx29_0_s:
 .string "OUTPUT"
# IR_LIT_STRING
 xchain0_n21_α:
 mov qword ptr [rsp + 352], 1
 mov rax, qword ptr [rip + .Lx30_0]
 mov qword ptr [rsp + 360], rax
 jmp xchain0_n22_α
 xchain0_n21_β:
 jmp xchain0_n23_α
.Lx30_0:
 .quad .Lx30_0_s
.Lx30_0_s:
 .string "stno ok"
# IR_ASSIGN global
 xchain0_n22_α:
 mov rsi, qword ptr [rsp + 352]
 mov rdx, qword ptr [rsp + 360]
 mov rdi, qword ptr [rip + .Lx31_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 336], rax
 mov qword ptr [rsp + 344], rdx
 jmp xchain0_n23_α
 xchain0_n22_β:
 jmp xchain0_n23_α
.Lx31_0:
 .quad .Lx31_0_s
.Lx31_0_s:
 .string "OUTPUT"
# IR_LIT_INTEGER
 xchain0_n23_α:
 mov qword ptr [rsp + 800], 6
 mov rax, qword ptr [rip + .Lx32_0]
 mov qword ptr [rsp + 808], rax
 jmp xchain0_n24_α
 xchain0_n23_β:
 jmp xchain0_n24_α
.Lx32_0:
 .quad 5
 xchain0_n24_α:
# BOX IR_CALL SNO$STMT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+800] -> [zr+768]
 mov rax, qword ptr [rsp + 800]
 mov qword ptr [rsp + 768], rax
 mov rax, qword ptr [rsp + 808]
 mov qword ptr [rsp + 776], rax
  .section .rodata
  .Lrkfn34: .string "SNO$STMT"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn34]
 lea rsi, [rsp + 768]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rsp + 752], rax
 mov qword ptr [rsp + 760], rdx
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
mov rsp, qword ptr [rsp + 920]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rsp + 920]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
add rsp, 65544
ret
