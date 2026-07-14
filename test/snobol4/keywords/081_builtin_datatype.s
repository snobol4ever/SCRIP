  .intel_syntax noprefix
  .text
  .globl main
main:
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
  sub rsp, 65536
  mov rdi, rsp
  mov ecx, 8192
  xor eax, eax
  rep stosq
  mov rdi, rsp
  xor esi, esi
  call main_α
  xor eax, eax
  add rsp, 65536
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
  mov qword ptr [r12 + 728], rsp
 push rsi
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, [rsp + 8]
 mov qword ptr [r12 + 720], rax
 pop rsi
main_α_body:
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [r12 + 240], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 248], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n3_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "hello"
 xchain0_n1_α:
# BOX IR_CALL DATATYPE(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+240] -> [zr+208]
 mov rax, qword ptr [r12 + 240]
 mov qword ptr [r12 + 208], rax
 mov rax, qword ptr [r12 + 248]
 mov qword ptr [r12 + 216], rax
  .section .rodata
  .Lrkfn3: .string "DATATYPE"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn3]
 lea rsi, [r12 + 208]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 192], rax
 mov qword ptr [r12 + 200], rdx
 cmp eax, 99
 je xchain0_n3_α
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n3_α
# IR_KEYWORD_SNOBOL4_read
 xchain0_n2_α:
 mov rdi, qword ptr [rip + .Lx4_0]
 call rt_keyword_read_snobol4@PLT
 mov qword ptr [r12 + 272], rax
 mov qword ptr [r12 + 280], rdx
 jmp xchain0_n4_α
 xchain0_n2_β:
 jmp xchain0_n3_α
.Lx4_0:
 .quad .Lx4_0_s
.Lx4_0_s:
 .string "LCASE"
# IR_LIT_INTEGER
 xchain0_n3_α:
 mov qword ptr [r12 + 448], 6
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [r12 + 456], rax
 jmp xchain0_n5_α
 xchain0_n3_β:
 jmp xchain0_n8_α
.Lx5_0:
 .quad 42
# IR_KEYWORD_SNOBOL4_read
 xchain0_n4_α:
 mov rdi, qword ptr [rip + .Lx6_0]
 call rt_keyword_read_snobol4@PLT
 mov qword ptr [r12 + 288], rax
 mov qword ptr [r12 + 296], rdx
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp xchain0_n3_α
.Lx6_0:
 .quad .Lx6_0_s
.Lx6_0_s:
 .string "UCASE"
 xchain0_n5_α:
# BOX IR_CALL DATATYPE(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+448] -> [zr+416]
 mov rax, qword ptr [r12 + 448]
 mov qword ptr [r12 + 416], rax
 mov rax, qword ptr [r12 + 456]
 mov qword ptr [r12 + 424], rax
  .section .rodata
  .Lrkfn8: .string "DATATYPE"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn8]
 lea rsi, [r12 + 416]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 400], rax
 mov qword ptr [r12 + 408], rdx
 cmp eax, 99
 je xchain0_n8_α
 jmp xchain0_n7_α
 xchain0_n5_β:
 jmp xchain0_n8_α
 xchain0_n6_α:
# BOX IR_CALL REPLACE(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+192] -> [zr+128]
 mov rax, qword ptr [r12 + 192]
 mov qword ptr [r12 + 128], rax
 mov rax, qword ptr [r12 + 200]
 mov qword ptr [r12 + 136], rax
# marshal arg1 = producer-box slot [zr+272] -> [zr+144]
 mov rax, qword ptr [r12 + 272]
 mov qword ptr [r12 + 144], rax
 mov rax, qword ptr [r12 + 280]
 mov qword ptr [r12 + 152], rax
# marshal arg2 = producer-box slot [zr+288] -> [zr+160]
 mov rax, qword ptr [r12 + 288]
 mov qword ptr [r12 + 160], rax
 mov rax, qword ptr [r12 + 296]
 mov qword ptr [r12 + 168], rax
  .section .rodata
  .Lrkfn10: .string "REPLACE"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn10]
 lea rsi, [r12 + 128]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [r12 + 112], rax
 mov qword ptr [r12 + 120], rdx
 cmp eax, 99
 je xchain0_n3_α
 jmp xchain0_n9_α
 xchain0_n6_β:
 jmp xchain0_n3_α
# IR_KEYWORD_SNOBOL4_read
 xchain0_n7_α:
 mov rdi, qword ptr [rip + .Lx11_0]
 call rt_keyword_read_snobol4@PLT
 mov qword ptr [r12 + 480], rax
 mov qword ptr [r12 + 488], rdx
 jmp xchain0_n10_α
 xchain0_n7_β:
 jmp xchain0_n8_α
.Lx11_0:
 .quad .Lx11_0_s
.Lx11_0_s:
 .string "LCASE"
# IR_LIT_REAL
 xchain0_n8_α:
 mov qword ptr [r12 + 656], 7
 mov rax, qword ptr [rip + .Lx12_0]
 mov qword ptr [r12 + 664], rax
 jmp xchain0_n11_α
 xchain0_n8_β:
 jmp main_γ
.Lx12_0:
 .quad 4614253070214989087
# IR_ASSIGN global
 xchain0_n9_α:
 mov rsi, qword ptr [r12 + 112]
 mov rdx, qword ptr [r12 + 120]
 mov rdi, qword ptr [rip + .Lx13_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 jmp xchain0_n3_α
 xchain0_n9_β:
 jmp xchain0_n3_α
.Lx13_0:
 .quad .Lx13_0_s
.Lx13_0_s:
 .string "OUTPUT"
# IR_KEYWORD_SNOBOL4_read
 xchain0_n10_α:
 mov rdi, qword ptr [rip + .Lx14_0]
 call rt_keyword_read_snobol4@PLT
 mov qword ptr [r12 + 496], rax
 mov qword ptr [r12 + 504], rdx
 jmp xchain0_n12_α
 xchain0_n10_β:
 jmp xchain0_n8_α
.Lx14_0:
 .quad .Lx14_0_s
.Lx14_0_s:
 .string "UCASE"
 xchain0_n11_α:
# BOX IR_CALL DATATYPE(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+656] -> [zr+624]
 mov rax, qword ptr [r12 + 656]
 mov qword ptr [r12 + 624], rax
 mov rax, qword ptr [r12 + 664]
 mov qword ptr [r12 + 632], rax
  .section .rodata
  .Lrkfn16: .string "DATATYPE"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn16]
 lea rsi, [r12 + 624]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 608], rax
 mov qword ptr [r12 + 616], rdx
 cmp eax, 99
 je main_γ
 jmp xchain0_n13_α
 xchain0_n11_β:
 jmp main_γ
 xchain0_n12_α:
# BOX IR_CALL REPLACE(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+400] -> [zr+336]
 mov rax, qword ptr [r12 + 400]
 mov qword ptr [r12 + 336], rax
 mov rax, qword ptr [r12 + 408]
 mov qword ptr [r12 + 344], rax
# marshal arg1 = producer-box slot [zr+480] -> [zr+352]
 mov rax, qword ptr [r12 + 480]
 mov qword ptr [r12 + 352], rax
 mov rax, qword ptr [r12 + 488]
 mov qword ptr [r12 + 360], rax
# marshal arg2 = producer-box slot [zr+496] -> [zr+368]
 mov rax, qword ptr [r12 + 496]
 mov qword ptr [r12 + 368], rax
 mov rax, qword ptr [r12 + 504]
 mov qword ptr [r12 + 376], rax
  .section .rodata
  .Lrkfn18: .string "REPLACE"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn18]
 lea rsi, [r12 + 336]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [r12 + 320], rax
 mov qword ptr [r12 + 328], rdx
 cmp eax, 99
 je xchain0_n8_α
 jmp xchain0_n14_α
 xchain0_n12_β:
 jmp xchain0_n8_α
# IR_KEYWORD_SNOBOL4_read
 xchain0_n13_α:
 mov rdi, qword ptr [rip + .Lx19_0]
 call rt_keyword_read_snobol4@PLT
 mov qword ptr [r12 + 688], rax
 mov qword ptr [r12 + 696], rdx
 jmp xchain0_n15_α
 xchain0_n13_β:
 jmp main_γ
.Lx19_0:
 .quad .Lx19_0_s
.Lx19_0_s:
 .string "LCASE"
# IR_ASSIGN global
 xchain0_n14_α:
 mov rsi, qword ptr [r12 + 320]
 mov rdx, qword ptr [r12 + 328]
 mov rdi, qword ptr [rip + .Lx20_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 304], rax
 mov qword ptr [r12 + 312], rdx
 jmp xchain0_n8_α
 xchain0_n14_β:
 jmp xchain0_n8_α
.Lx20_0:
 .quad .Lx20_0_s
.Lx20_0_s:
 .string "OUTPUT"
# IR_KEYWORD_SNOBOL4_read
 xchain0_n15_α:
 mov rdi, qword ptr [rip + .Lx21_0]
 call rt_keyword_read_snobol4@PLT
 mov qword ptr [r12 + 704], rax
 mov qword ptr [r12 + 712], rdx
 jmp xchain0_n16_α
 xchain0_n15_β:
 jmp main_γ
.Lx21_0:
 .quad .Lx21_0_s
.Lx21_0_s:
 .string "UCASE"
 xchain0_n16_α:
# BOX IR_CALL REPLACE(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+608] -> [zr+544]
 mov rax, qword ptr [r12 + 608]
 mov qword ptr [r12 + 544], rax
 mov rax, qword ptr [r12 + 616]
 mov qword ptr [r12 + 552], rax
# marshal arg1 = producer-box slot [zr+688] -> [zr+560]
 mov rax, qword ptr [r12 + 688]
 mov qword ptr [r12 + 560], rax
 mov rax, qword ptr [r12 + 696]
 mov qword ptr [r12 + 568], rax
# marshal arg2 = producer-box slot [zr+704] -> [zr+576]
 mov rax, qword ptr [r12 + 704]
 mov qword ptr [r12 + 576], rax
 mov rax, qword ptr [r12 + 712]
 mov qword ptr [r12 + 584], rax
  .section .rodata
  .Lrkfn23: .string "REPLACE"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn23]
 lea rsi, [r12 + 544]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [r12 + 528], rax
 mov qword ptr [r12 + 536], rdx
 cmp eax, 99
 je main_γ
 jmp xchain0_n17_α
 xchain0_n16_β:
 jmp main_γ
# IR_ASSIGN global
 xchain0_n17_α:
 mov rsi, qword ptr [r12 + 528]
 mov rdx, qword ptr [r12 + 536]
 mov rdi, qword ptr [rip + .Lx24_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 512], rax
 mov qword ptr [r12 + 520], rdx
 jmp main_γ
 xchain0_n17_β:
 jmp main_γ
.Lx24_0:
 .quad .Lx24_0_s
.Lx24_0_s:
 .string "OUTPUT"
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [r12 + 728]
pop r12
ret
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [r12 + 720]
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
main_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, qword ptr [r12 + 728]
pop r12
ret
