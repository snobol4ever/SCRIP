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
 mov qword ptr [r12 + 64], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 72], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp main_ω
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "b"
xchain0_n1_α:
# IR_LIT_STRING
bb2_α:
 mov qword ptr [r12 + 80], 1
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [r12 + 88], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp main_ω
.Lx2_0:
 .quad .Lx2_0_s
.Lx2_0_s:
 .string "a"
xchain0_n2_α:
bb3_α:
# BOX IR_CALL LGT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [r12+64] -> [r12+32]
 mov rax, qword ptr [r12 + 64]
 mov qword ptr [r12 + 32], rax
 mov rax, qword ptr [r12 + 72]
 mov qword ptr [r12 + 40], rax
# marshal arg1 = producer-box slot [r12+80] -> [r12+48]
 mov rax, qword ptr [r12 + 80]
 mov qword ptr [r12 + 48], rax
 mov rax, qword ptr [r12 + 88]
 mov qword ptr [r12 + 56], rax
  .section .rodata
  .Lrkfn4: .string "LGT"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn4]
 lea rsi, [r12 + 32]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 cmp eax, 99
 je main_ω
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp main_ω
xchain0_n3_α:
# IR_LIT_STRING
bb4_α:
 mov qword ptr [r12 + 112], 1
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [r12 + 120], rax
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp xchain0_n5_α
.Lx5_0:
 .quad .Lx5_0_s
.Lx5_0_s:
 .string "b > a"
xchain0_n4_α:
# IR_ASSIGN global
bb5_α:
 mov rsi, qword ptr [r12 + 112]
 mov rdx, qword ptr [r12 + 120]
 mov rdi, qword ptr [rip + .Lx6_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 jmp xchain0_n5_α
 xchain0_n4_β:
 jmp xchain0_n5_α
.Lx6_0:
 .quad .Lx6_0_s
.Lx6_0_s:
 .string "OUTPUT"
xchain0_n5_α:
# IR_LIT_STRING
bb6_α:
 mov qword ptr [r12 + 176], 1
 mov rax, qword ptr [rip + .Lx7_0]
 mov qword ptr [r12 + 184], rax
 jmp xchain0_n6_α
 xchain0_n5_β:
 jmp main_ω
.Lx7_0:
 .quad .Lx7_0_s
.Lx7_0_s:
 .string "a"
xchain0_n6_α:
# IR_LIT_STRING
bb7_α:
 mov qword ptr [r12 + 192], 1
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [r12 + 200], rax
 jmp xchain0_n7_α
 xchain0_n6_β:
 jmp main_ω
.Lx8_0:
 .quad .Lx8_0_s
.Lx8_0_s:
 .string "b"
xchain0_n7_α:
bb8_α:
# BOX IR_CALL LLT(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [r12+176] -> [r12+144]
 mov rax, qword ptr [r12 + 176]
 mov qword ptr [r12 + 144], rax
 mov rax, qword ptr [r12 + 184]
 mov qword ptr [r12 + 152], rax
# marshal arg1 = producer-box slot [r12+192] -> [r12+160]
 mov rax, qword ptr [r12 + 192]
 mov qword ptr [r12 + 160], rax
 mov rax, qword ptr [r12 + 200]
 mov qword ptr [r12 + 168], rax
  .section .rodata
  .Lrkfn10: .string "LLT"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn10]
 lea rsi, [r12 + 144]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 128], rax
 mov qword ptr [r12 + 136], rdx
 cmp eax, 99
 je main_ω
 jmp xchain0_n8_α
 xchain0_n7_β:
 jmp main_ω
xchain0_n8_α:
# IR_LIT_STRING
bb9_α:
 mov qword ptr [r12 + 224], 1
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [r12 + 232], rax
 jmp xchain0_n9_α
 xchain0_n8_β:
 jmp xchain0_n10_α
.Lx11_0:
 .quad .Lx11_0_s
.Lx11_0_s:
 .string "a < b"
xchain0_n9_α:
# IR_ASSIGN global
bb10_α:
 mov rsi, qword ptr [r12 + 224]
 mov rdx, qword ptr [r12 + 232]
 mov rdi, qword ptr [rip + .Lx12_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 208], rax
 mov qword ptr [r12 + 216], rdx
 jmp xchain0_n10_α
 xchain0_n9_β:
 jmp xchain0_n10_α
.Lx12_0:
 .quad .Lx12_0_s
.Lx12_0_s:
 .string "OUTPUT"
xchain0_n10_α:
# IR_LIT_STRING
bb11_α:
 mov qword ptr [r12 + 288], 1
 mov rax, qword ptr [rip + .Lx13_0]
 mov qword ptr [r12 + 296], rax
 jmp xchain0_n11_α
 xchain0_n10_β:
 jmp main_ω
.Lx13_0:
 .quad .Lx13_0_s
.Lx13_0_s:
 .string "cat"
xchain0_n11_α:
# IR_LIT_STRING
bb12_α:
 mov qword ptr [r12 + 304], 1
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [r12 + 312], rax
 jmp xchain0_n12_α
 xchain0_n11_β:
 jmp main_ω
.Lx14_0:
 .quad .Lx14_0_s
.Lx14_0_s:
 .string "cat"
xchain0_n12_α:
bb13_α:
# BOX IR_CALL LEQ(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [r12+288] -> [r12+256]
 mov rax, qword ptr [r12 + 288]
 mov qword ptr [r12 + 256], rax
 mov rax, qword ptr [r12 + 296]
 mov qword ptr [r12 + 264], rax
# marshal arg1 = producer-box slot [r12+304] -> [r12+272]
 mov rax, qword ptr [r12 + 304]
 mov qword ptr [r12 + 272], rax
 mov rax, qword ptr [r12 + 312]
 mov qword ptr [r12 + 280], rax
  .section .rodata
  .Lrkfn16: .string "LEQ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn16]
 lea rsi, [r12 + 256]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 240], rax
 mov qword ptr [r12 + 248], rdx
 cmp eax, 99
 je main_ω
 jmp xchain0_n13_α
 xchain0_n12_β:
 jmp main_ω
xchain0_n13_α:
# IR_LIT_STRING
bb14_α:
 mov qword ptr [r12 + 336], 1
 mov rax, qword ptr [rip + .Lx17_0]
 mov qword ptr [r12 + 344], rax
 jmp xchain0_n14_α
 xchain0_n13_β:
 jmp xchain0_n15_α
.Lx17_0:
 .quad .Lx17_0_s
.Lx17_0_s:
 .string "cat = cat"
xchain0_n14_α:
# IR_ASSIGN global
bb15_α:
 mov rsi, qword ptr [r12 + 336]
 mov rdx, qword ptr [r12 + 344]
 mov rdi, qword ptr [rip + .Lx18_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 320], rax
 mov qword ptr [r12 + 328], rdx
 jmp xchain0_n15_α
 xchain0_n14_β:
 jmp xchain0_n15_α
.Lx18_0:
 .quad .Lx18_0_s
.Lx18_0_s:
 .string "OUTPUT"
xchain0_n15_α:
# IR_LIT_STRING
bb16_α:
 mov qword ptr [r12 + 400], 1
 mov rax, qword ptr [rip + .Lx19_0]
 mov qword ptr [r12 + 408], rax
 jmp xchain0_n16_α
 xchain0_n15_β:
 jmp main_ω
.Lx19_0:
 .quad .Lx19_0_s
.Lx19_0_s:
 .string "cat"
xchain0_n16_α:
# IR_LIT_STRING
bb17_α:
 mov qword ptr [r12 + 416], 1
 mov rax, qword ptr [rip + .Lx20_0]
 mov qword ptr [r12 + 424], rax
 jmp xchain0_n17_α
 xchain0_n16_β:
 jmp main_ω
.Lx20_0:
 .quad .Lx20_0_s
.Lx20_0_s:
 .string "dog"
xchain0_n17_α:
bb18_α:
# BOX IR_CALL LNE(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [r12+400] -> [r12+368]
 mov rax, qword ptr [r12 + 400]
 mov qword ptr [r12 + 368], rax
 mov rax, qword ptr [r12 + 408]
 mov qword ptr [r12 + 376], rax
# marshal arg1 = producer-box slot [r12+416] -> [r12+384]
 mov rax, qword ptr [r12 + 416]
 mov qword ptr [r12 + 384], rax
 mov rax, qword ptr [r12 + 424]
 mov qword ptr [r12 + 392], rax
  .section .rodata
  .Lrkfn22: .string "LNE"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn22]
 lea rsi, [r12 + 368]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 352], rax
 mov qword ptr [r12 + 360], rdx
 cmp eax, 99
 je main_ω
 jmp xchain0_n18_α
 xchain0_n17_β:
 jmp main_ω
xchain0_n18_α:
# IR_LIT_STRING
bb19_α:
 mov qword ptr [r12 + 448], 1
 mov rax, qword ptr [rip + .Lx23_0]
 mov qword ptr [r12 + 456], rax
 jmp xchain0_n19_α
 xchain0_n18_β:
 jmp main_ω
.Lx23_0:
 .quad .Lx23_0_s
.Lx23_0_s:
 .string "cat != dog"
xchain0_n19_α:
# IR_ASSIGN global
bb20_α:
 mov rsi, qword ptr [r12 + 448]
 mov rdx, qword ptr [r12 + 456]
 mov rdi, qword ptr [rip + .Lx24_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 432], rax
 mov qword ptr [r12 + 440], rdx
 jmp main_γ
 xchain0_n19_β:
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
