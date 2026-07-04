  .intel_syntax noprefix
  .text
proc_startup:
  push rbp
  mov rbp, rsp
  .section .rodata
  .Lclassspec0: .string "node(val,next)"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lclassspec0]
  call record_register@PLT
  pop rbp
  ret
  .section .rodata
  .Lgvan0: .string "N"
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
  call proc_startup
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
main_α_body:
xchain0_n0_α:
# IR_LIT_STRING
bb1_α:
 mov qword ptr [r12 + 48], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 56], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n2_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "node(val,next)"
xchain0_n1_α:
bb2_α:
# BOX IR_CALL DATA(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [r12+48] -> [r12+32]
 mov rax, qword ptr [r12 + 48]
 mov qword ptr [r12 + 32], rax
 mov rax, qword ptr [r12 + 56]
 mov qword ptr [r12 + 40], rax
  .section .rodata
  .Lrkfn3: .string "DATA"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn3]
 lea rsi, [r12 + 32]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 cmp eax, 99
 je xchain0_n2_α
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n2_α
xchain0_n2_α:
# IR_LIT_STRING
bb3_α:
 mov qword ptr [r12 + 128], 1
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [r12 + 136], rax
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n6_α
.Lx4_0:
 .quad .Lx4_0_s
.Lx4_0_s:
 .string "hello"
xchain0_n3_α:
# IR_LIT_STRING
bb4_α:
 mov qword ptr [r12 + 144], 1
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [r12 + 152], rax
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp xchain0_n6_α
.Lx5_0:
 .quad .Lx5_0_s
.Lx5_0_s:
 .string ""
xchain0_n4_α:
bb5_α:
# BOX IR_CALL node(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [r12+128] -> [r12+96]
 mov rax, qword ptr [r12 + 128]
 mov qword ptr [r12 + 96], rax
 mov rax, qword ptr [r12 + 136]
 mov qword ptr [r12 + 104], rax
# marshal arg1 = producer-box slot [r12+144] -> [r12+112]
 mov rax, qword ptr [r12 + 144]
 mov qword ptr [r12 + 112], rax
 mov rax, qword ptr [r12 + 152]
 mov qword ptr [r12 + 120], rax
  .section .rodata
  .Lrkfn7: .string "node"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn7]
 lea rsi, [r12 + 96]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 80], rax
 mov qword ptr [r12 + 88], rdx
 cmp eax, 99
 je xchain0_n6_α
 jmp xchain0_n5_α
 xchain0_n4_β:
 jmp xchain0_n6_α
xchain0_n5_α:
# IR_ASSIGN gva
bb6_α:
 mov rax, qword ptr [r12 + 80]
 mov rdx, qword ptr [r12 + 88]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 64], rax
 mov qword ptr [r12 + 72], rdx
 jmp xchain0_n6_α
 xchain0_n5_β:
 jmp xchain0_n6_α
xchain0_n6_α:
# IR_VAR gva
bb7_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 272], rax
 mov qword ptr [r12 + 280], rdx
 jmp xchain0_n7_α
 xchain0_n6_β:
 jmp xchain0_n9_α
xchain0_n7_α:
bb8_α:
# BOX IR_CALL DATATYPE(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [r12+272] -> [r12+256]
 mov rax, qword ptr [r12 + 272]
 mov qword ptr [r12 + 256], rax
 mov rax, qword ptr [r12 + 280]
 mov qword ptr [r12 + 264], rax
  .section .rodata
  .Lrkfn11: .string "DATATYPE"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn11]
 lea rsi, [r12 + 256]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 240], rax
 mov qword ptr [r12 + 248], rdx
 cmp eax, 99
 je xchain0_n9_α
 jmp xchain0_n8_α
 xchain0_n7_β:
 jmp xchain0_n9_α
xchain0_n8_α:
# IR_KEYWORD_SNOBOL4_read
bb9_α:
 mov rdi, qword ptr [rip + .Lx12_0]
 call rt_keyword_read_snobol4@PLT
 mov qword ptr [r12 + 288], rax
 mov qword ptr [r12 + 296], rdx
 jmp xchain0_n10_α
 xchain0_n8_β:
 jmp xchain0_n9_α
.Lx12_0:
 .quad .Lx12_0_s
.Lx12_0_s:
 .string "LCASE"
xchain0_n9_α:
# IR_VAR gva
bb10_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 368], rax
 mov qword ptr [r12 + 376], rdx
 jmp xchain0_n11_α
 xchain0_n9_β:
 jmp main_γ
xchain0_n10_α:
# IR_KEYWORD_SNOBOL4_read
bb11_α:
 mov rdi, qword ptr [rip + .Lx14_0]
 call rt_keyword_read_snobol4@PLT
 mov qword ptr [r12 + 304], rax
 mov qword ptr [r12 + 312], rdx
 jmp xchain0_n12_α
 xchain0_n10_β:
 jmp xchain0_n9_α
.Lx14_0:
 .quad .Lx14_0_s
.Lx14_0_s:
 .string "UCASE"
xchain0_n11_α:
bb12_α:
# BOX IR_CALL val(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [r12+368] -> [r12+352]
 mov rax, qword ptr [r12 + 368]
 mov qword ptr [r12 + 352], rax
 mov rax, qword ptr [r12 + 376]
 mov qword ptr [r12 + 360], rax
  .section .rodata
  .Lrkfn16: .string "val"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn16]
 lea rsi, [r12 + 352]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 336], rax
 mov qword ptr [r12 + 344], rdx
 cmp eax, 99
 je main_γ
 jmp xchain0_n13_α
 xchain0_n11_β:
 jmp main_γ
xchain0_n12_α:
bb13_α:
# BOX IR_CALL REPLACE(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [r12+240] -> [r12+192]
 mov rax, qword ptr [r12 + 240]
 mov qword ptr [r12 + 192], rax
 mov rax, qword ptr [r12 + 248]
 mov qword ptr [r12 + 200], rax
# marshal arg1 = producer-box slot [r12+288] -> [r12+208]
 mov rax, qword ptr [r12 + 288]
 mov qword ptr [r12 + 208], rax
 mov rax, qword ptr [r12 + 296]
 mov qword ptr [r12 + 216], rax
# marshal arg2 = producer-box slot [r12+304] -> [r12+224]
 mov rax, qword ptr [r12 + 304]
 mov qword ptr [r12 + 224], rax
 mov rax, qword ptr [r12 + 312]
 mov qword ptr [r12 + 232], rax
  .section .rodata
  .Lrkfn18: .string "REPLACE"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn18]
 lea rsi, [r12 + 192]
 mov edx, 3
 call rt_call_arr@PLT
 mov qword ptr [r12 + 176], rax
 mov qword ptr [r12 + 184], rdx
 cmp eax, 99
 je xchain0_n9_α
 jmp xchain0_n14_α
 xchain0_n12_β:
 jmp xchain0_n9_α
xchain0_n13_α:
# IR_ASSIGN global
bb14_α:
 mov rsi, qword ptr [r12 + 336]
 mov rdx, qword ptr [r12 + 344]
 mov rdi, qword ptr [rip + .Lx19_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 320], rax
 mov qword ptr [r12 + 328], rdx
 jmp main_γ
 xchain0_n13_β:
 jmp main_γ
.Lx19_0:
 .quad .Lx19_0_s
.Lx19_0_s:
 .string "OUTPUT"
xchain0_n14_α:
# IR_ASSIGN global
bb15_α:
 mov rsi, qword ptr [r12 + 176]
 mov rdx, qword ptr [r12 + 184]
 mov rdi, qword ptr [rip + .Lx20_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 160], rax
 mov qword ptr [r12 + 168], rdx
 jmp xchain0_n9_α
 xchain0_n14_β:
 jmp xchain0_n9_α
.Lx20_0:
 .quad .Lx20_0_s
.Lx20_0_s:
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
