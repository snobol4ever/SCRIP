  .intel_syntax noprefix
  .text
  .globl main
main:
  push rbp
  mov rbp, rsp
  call rt_frame@PLT
  mov rdi, rax
  xor esi, esi
  call main_α
  xor eax, eax
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
main_α_body:
xchain0_n0_α:
# IR_LIT_STRING
bb1_α:
 mov qword ptr [r12 + 128], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 136], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n3_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "hello"
xchain0_n1_α:
bb2_α:
# BOX IR_CALL DATATYPE(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [r12+128] -> [r12+112]
 mov rax, qword ptr [r12 + 128]
 mov qword ptr [r12 + 112], rax
 mov rax, qword ptr [r12 + 136]
 mov qword ptr [r12 + 120], rax
  .section .rodata
  .Lrkfn3: .string "DATATYPE"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn3]
 lea rsi, [r12 + 112]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 cmp eax, 99
 je xchain0_n3_α
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n3_α
xchain0_n2_α:
# IR_KEYWORD_read
bb3_α:
 mov rdi, qword ptr [rip + .Lx4_0]
 call rt_keyword_read@PLT
 mov qword ptr [r12 + 144], rax
 mov qword ptr [r12 + 152], rdx
 jmp xchain0_n4_α
 xchain0_n2_β:
 jmp xchain0_n3_α
.Lx4_0:
 .quad .Lx4_0_s
.Lx4_0_s:
 .string "LCASE"
xchain0_n3_α:
# IR_LIT_INTEGER
bb4_α:
 mov qword ptr [r12 + 320], 6
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [r12 + 328], rax
 jmp xchain0_n5_α
 xchain0_n3_β:
 jmp xchain0_n8_α
.Lx5_0:
 .quad 42
xchain0_n4_α:
# IR_KEYWORD_read
bb5_α:
 mov rdi, qword ptr [rip + .Lx6_0]
 call rt_keyword_read@PLT
 mov qword ptr [r12 + 176], rax
 mov qword ptr [r12 + 184], rdx
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp xchain0_n3_α
.Lx6_0:
 .quad .Lx6_0_s
.Lx6_0_s:
 .string "UCASE"
xchain0_n5_α:
bb6_α:
# BOX IR_CALL DATATYPE(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [r12+320] -> [r12+304]
 mov rax, qword ptr [r12 + 320]
 mov qword ptr [r12 + 304], rax
 mov rax, qword ptr [r12 + 328]
 mov qword ptr [r12 + 312], rax
  .section .rodata
  .Lrkfn8: .string "DATATYPE"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn8]
 lea rsi, [r12 + 304]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 288], rax
 mov qword ptr [r12 + 296], rdx
 cmp eax, 99
 je xchain0_n8_α
 jmp xchain0_n7_α
 xchain0_n5_β:
 jmp xchain0_n8_α
xchain0_n6_α:
bb7_α:
# BOX IR_CALL REPLACE(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [r12+96] -> [r12+48]
 mov rax, qword ptr [r12 + 96]
 mov qword ptr [r12 + 48], rax
 mov rax, qword ptr [r12 + 104]
 mov qword ptr [r12 + 56], rax
# marshal arg1 = producer-box slot [r12+144] -> [r12+64]
 mov rax, qword ptr [r12 + 144]
 mov qword ptr [r12 + 64], rax
 mov rax, qword ptr [r12 + 152]
 mov qword ptr [r12 + 72], rax
# marshal arg2 = producer-box slot [r12+176] -> [r12+80]
 mov rax, qword ptr [r12 + 176]
 mov qword ptr [r12 + 80], rax
 mov rax, qword ptr [r12 + 184]
 mov qword ptr [r12 + 88], rax
  .section .rodata
  .Lrkfn10: .string "REPLACE"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn10]
 lea rsi, [r12 + 48]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je xchain0_n3_α
 jmp xchain0_n9_α
 xchain0_n6_β:
 jmp xchain0_n3_α
xchain0_n7_α:
# IR_KEYWORD_read
bb8_α:
 mov rdi, qword ptr [rip + .Lx11_0]
 call rt_keyword_read@PLT
 mov qword ptr [r12 + 336], rax
 mov qword ptr [r12 + 344], rdx
 jmp xchain0_n10_α
 xchain0_n7_β:
 jmp xchain0_n8_α
.Lx11_0:
 .quad .Lx11_0_s
.Lx11_0_s:
 .string "LCASE"
xchain0_n8_α:
# IR_LIT_REAL
bb9_α:
 mov qword ptr [r12 + 512], 7
 mov rax, qword ptr [rip + .Lx12_0]
 mov qword ptr [r12 + 520], rax
 jmp xchain0_n11_α
 xchain0_n8_β:
 jmp main_ω
.Lx12_0:
 .quad 4614253070214989087
xchain0_n9_α:
# IR_ASSIGN global
bb10_α:
 mov rsi, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov rdi, qword ptr [rip + .Lx13_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp xchain0_n3_α
 xchain0_n9_β:
 jmp xchain0_n3_α
.Lx13_0:
 .quad .Lx13_0_s
.Lx13_0_s:
 .string "OUTPUT"
xchain0_n10_α:
# IR_KEYWORD_read
bb11_α:
 mov rdi, qword ptr [rip + .Lx14_0]
 call rt_keyword_read@PLT
 mov qword ptr [r12 + 368], rax
 mov qword ptr [r12 + 376], rdx
 jmp xchain0_n12_α
 xchain0_n10_β:
 jmp xchain0_n8_α
.Lx14_0:
 .quad .Lx14_0_s
.Lx14_0_s:
 .string "UCASE"
xchain0_n11_α:
bb12_α:
# BOX IR_CALL DATATYPE(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [r12+512] -> [r12+496]
 mov rax, qword ptr [r12 + 512]
 mov qword ptr [r12 + 496], rax
 mov rax, qword ptr [r12 + 520]
 mov qword ptr [r12 + 504], rax
  .section .rodata
  .Lrkfn16: .string "DATATYPE"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn16]
 lea rsi, [r12 + 496]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 480], rax
 mov qword ptr [r12 + 488], rdx
 cmp eax, 99
 je main_ω
 jmp xchain0_n13_α
 xchain0_n11_β:
 jmp main_ω
xchain0_n12_α:
bb13_α:
# BOX IR_CALL REPLACE(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [r12+288] -> [r12+240]
 mov rax, qword ptr [r12 + 288]
 mov qword ptr [r12 + 240], rax
 mov rax, qword ptr [r12 + 296]
 mov qword ptr [r12 + 248], rax
# marshal arg1 = producer-box slot [r12+336] -> [r12+256]
 mov rax, qword ptr [r12 + 336]
 mov qword ptr [r12 + 256], rax
 mov rax, qword ptr [r12 + 344]
 mov qword ptr [r12 + 264], rax
# marshal arg2 = producer-box slot [r12+368] -> [r12+272]
 mov rax, qword ptr [r12 + 368]
 mov qword ptr [r12 + 272], rax
 mov rax, qword ptr [r12 + 376]
 mov qword ptr [r12 + 280], rax
  .section .rodata
  .Lrkfn18: .string "REPLACE"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn18]
 lea rsi, [r12 + 240]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [r12 + 224], rax
 mov qword ptr [r12 + 232], rdx
 cmp eax, 99
 je xchain0_n8_α
 jmp xchain0_n14_α
 xchain0_n12_β:
 jmp xchain0_n8_α
xchain0_n13_α:
# IR_KEYWORD_read
bb14_α:
 mov rdi, qword ptr [rip + .Lx19_0]
 call rt_keyword_read@PLT
 mov qword ptr [r12 + 528], rax
 mov qword ptr [r12 + 536], rdx
 jmp xchain0_n15_α
 xchain0_n13_β:
 jmp main_ω
.Lx19_0:
 .quad .Lx19_0_s
.Lx19_0_s:
 .string "LCASE"
xchain0_n14_α:
# IR_ASSIGN global
bb15_α:
 mov rsi, qword ptr [r12 + 224]
 mov rdx, qword ptr [r12 + 232]
 mov rdi, qword ptr [rip + .Lx20_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 208], rax
 mov qword ptr [r12 + 216], rdx
 jmp xchain0_n8_α
 xchain0_n14_β:
 jmp xchain0_n8_α
.Lx20_0:
 .quad .Lx20_0_s
.Lx20_0_s:
 .string "OUTPUT"
xchain0_n15_α:
# IR_KEYWORD_read
bb16_α:
 mov rdi, qword ptr [rip + .Lx21_0]
 call rt_keyword_read@PLT
 mov qword ptr [r12 + 560], rax
 mov qword ptr [r12 + 568], rdx
 jmp xchain0_n16_α
 xchain0_n15_β:
 jmp main_ω
.Lx21_0:
 .quad .Lx21_0_s
.Lx21_0_s:
 .string "UCASE"
xchain0_n16_α:
bb17_α:
# BOX IR_CALL REPLACE(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [r12+480] -> [r12+432]
 mov rax, qword ptr [r12 + 480]
 mov qword ptr [r12 + 432], rax
 mov rax, qword ptr [r12 + 488]
 mov qword ptr [r12 + 440], rax
# marshal arg1 = producer-box slot [r12+528] -> [r12+448]
 mov rax, qword ptr [r12 + 528]
 mov qword ptr [r12 + 448], rax
 mov rax, qword ptr [r12 + 536]
 mov qword ptr [r12 + 456], rax
# marshal arg2 = producer-box slot [r12+560] -> [r12+464]
 mov rax, qword ptr [r12 + 560]
 mov qword ptr [r12 + 464], rax
 mov rax, qword ptr [r12 + 568]
 mov qword ptr [r12 + 472], rax
  .section .rodata
  .Lrkfn23: .string "REPLACE"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn23]
 lea rsi, [r12 + 432]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [r12 + 416], rax
 mov qword ptr [r12 + 424], rdx
 cmp eax, 99
 je main_ω
 jmp xchain0_n17_α
 xchain0_n16_β:
 jmp main_ω
xchain0_n17_α:
# IR_ASSIGN global
bb18_α:
 mov rsi, qword ptr [r12 + 416]
 mov rdx, qword ptr [r12 + 424]
 mov rdi, qword ptr [rip + .Lx24_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 400], rax
 mov qword ptr [r12 + 408], rdx
 jmp main_γ
 xchain0_n17_β:
 jmp main_ω
.Lx24_0:
 .quad .Lx24_0_s
.Lx24_0_s:
 .string "OUTPUT"
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
pop r12
ret
main_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
pop r12
ret
