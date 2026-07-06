  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "N"
  .Lgvan1: .string "S"
  .align 8
__gva_names:
  .quad .Lgvan0
  .quad .Lgvan1
  .section .bss
  .align 16
__gva: .space 32, 0
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  push rbp
  mov rbp, rsp
  push rdi
  push rsi
  lea rdi, [rip + __gva_names]
  lea rsi, [rip + __gva]
  mov edx, 2
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
 mov qword ptr [r12 + 64], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 72], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n3_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "TRIM"
xchain0_n1_α:
# IR_LIT_INTEGER
bb2_α:
 mov qword ptr [r12 + 80], 6
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [r12 + 88], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n3_α
.Lx2_0:
 .quad 1
xchain0_n2_α:
bb3_α:
# BOX IR_CALL SNO$KWSET(...) -> rt_call_arr [operand-marshal, FAIL->ω]
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
  .Lrkfn4: .string "SNO$KWSET"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn4]
 lea rsi, [r12 + 32]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 cmp eax, 99
 je xchain0_n3_α
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n3_α
xchain0_n3_α:
# IR_LIT_INTEGER
bb4_α:
 mov qword ptr [r12 + 112], 6
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [r12 + 120], rax
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp xchain0_n5_α
.Lx5_0:
 .quad 0
xchain0_n4_α:
# IR_ASSIGN gva
bb5_α:
 mov rax, qword ptr [r12 + 112]
 mov rdx, qword ptr [r12 + 120]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 jmp xchain0_n5_α
 xchain0_n4_β:
 jmp xchain0_n5_α
xchain0_n5_α:
# IR_VAR
bb6_α:
 mov rdi, qword ptr [rip + .Lx7_0]
 call NV_GET_fn@PLT
 cmp eax, 99
 je main_γ
 mov qword ptr [r12 + 144], rax
 mov qword ptr [r12 + 152], rdx
 jmp xchain0_n6_α
 xchain0_n5_β:
 jmp main_γ
.Lx7_0:
 .quad .Lx7_0_s
.Lx7_0_s:
 .string "INPUT"
xchain0_n6_α:
# IR_ASSIGN gva
bb7_α:
 mov rax, qword ptr [r12 + 144]
 mov rdx, qword ptr [r12 + 152]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 128], rax
 mov qword ptr [r12 + 136], rdx
 jmp xchain0_n7_α
 xchain0_n6_β:
 jmp main_γ
xchain0_n7_α:
# IR_LIT_STRING
bb8_α:
 mov qword ptr [r12 + 240], 1
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [r12 + 248], rax
 jmp xchain0_n8_α
 xchain0_n7_β:
 jmp xchain0_n11_α
.Lx9_0:
 .quad .Lx9_0_s
.Lx9_0_s:
 .string " "
xchain0_n8_α:
# IR_LIT_INTEGER
bb9_α:
 mov qword ptr [r12 + 288], 6
 mov rax, qword ptr [rip + .Lx10_0]
 mov qword ptr [r12 + 296], rax
 jmp xchain0_n9_α
 xchain0_n8_β:
 jmp xchain0_n11_α
.Lx10_0:
 .quad 80
xchain0_n9_α:
# IR_VAR gva
bb10_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 336], rax
 mov qword ptr [r12 + 344], rdx
 jmp xchain0_n10_α
 xchain0_n9_β:
 jmp xchain0_n11_α
xchain0_n10_α:
bb11_α:
# BOX IR_CALL SIZE(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [r12+336] -> [r12+320]
 mov rax, qword ptr [r12 + 336]
 mov qword ptr [r12 + 320], rax
 mov rax, qword ptr [r12 + 344]
 mov qword ptr [r12 + 328], rax
  .section .rodata
  .Lrkfn13: .string "SIZE"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn13]
 lea rsi, [r12 + 320]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 304], rax
 mov qword ptr [r12 + 312], rdx
 cmp eax, 99
 je xchain0_n11_α
 jmp xchain0_n12_α
 xchain0_n10_β:
 jmp xchain0_n11_α
xchain0_n11_α:
# IR_VAR gva
bb12_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 464], rax
 mov qword ptr [r12 + 472], rdx
 jmp xchain0_n13_α
 xchain0_n11_β:
 jmp xchain0_n14_α
xchain0_n12_α:
bb13_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 288]
 cmp eax, 100
 je .Lx15_0
 mov eax, dword ptr [r12 + 304]
 cmp eax, 100
 je .Lx15_0
 mov eax, dword ptr [r12 + 288]
 cmp eax, 6
 jne .Lx15_2
 mov eax, dword ptr [r12 + 304]
 cmp eax, 6
 jne .Lx15_2
.Lx15_1:
 mov rax, qword ptr [r12 + 296]
 mov rcx, qword ptr [r12 + 312]
 sub rax, rcx
 mov qword ptr [r12 + 272], 6
 mov qword ptr [r12 + 280], rax
 jmp xchain0_n15_α
.Lx15_0:
 mov rdi, qword ptr [r12 + 288]
 mov rsi, qword ptr [r12 + 296]
 mov rdx, qword ptr [r12 + 304]
 mov rcx, qword ptr [r12 + 312]
 mov r8d, 1
 lea r9, [r12 + 272]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx15_3
.Lx15_2:
 mov rdi, qword ptr [r12 + 288]
 mov rsi, qword ptr [r12 + 296]
 mov rdx, qword ptr [r12 + 304]
 mov rcx, qword ptr [r12 + 312]
 mov r8d, 1
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n11_α
 mov qword ptr [r12 + 272], rax
 mov qword ptr [r12 + 280], rdx
.Lx15_3:
 jmp xchain0_n15_α
 xchain0_n12_β:
 jmp xchain0_n11_α
xchain0_n13_α:
# IR_LIT_INTEGER
bb14_α:
 mov qword ptr [r12 + 480], 6
 mov rax, qword ptr [rip + .Lx16_0]
 mov qword ptr [r12 + 488], rax
 jmp xchain0_n16_α
 xchain0_n13_β:
 jmp xchain0_n14_α
.Lx16_0:
 .quad 1
xchain0_n14_α:
# IR_VAR gva
bb15_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 576], rax
 mov qword ptr [r12 + 584], rdx
 jmp xchain0_n17_α
 xchain0_n14_β:
 jmp xchain0_n5_α
xchain0_n15_α:
# IR_LIT_INTEGER
bb16_α:
 mov qword ptr [r12 + 352], 6
 mov rax, qword ptr [rip + .Lx18_0]
 mov qword ptr [r12 + 360], rax
 jmp xchain0_n18_α
 xchain0_n15_β:
 jmp xchain0_n11_α
.Lx18_0:
 .quad 2
xchain0_n16_α:
bb17_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 464]
 cmp eax, 100
 je .Lx19_0
 mov eax, dword ptr [r12 + 480]
 cmp eax, 100
 je .Lx19_0
 mov eax, dword ptr [r12 + 464]
 cmp eax, 6
 jne .Lx19_2
 mov eax, dword ptr [r12 + 480]
 cmp eax, 6
 jne .Lx19_2
.Lx19_1:
 mov rax, qword ptr [r12 + 472]
 mov rcx, qword ptr [r12 + 488]
 add rax, rcx
 mov qword ptr [r12 + 448], 6
 mov qword ptr [r12 + 456], rax
 jmp xchain0_n19_α
.Lx19_0:
 mov rdi, qword ptr [r12 + 464]
 mov rsi, qword ptr [r12 + 472]
 mov rdx, qword ptr [r12 + 480]
 mov rcx, qword ptr [r12 + 488]
 mov r8d, 0
 lea r9, [r12 + 448]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx19_3
.Lx19_2:
 mov rdi, qword ptr [r12 + 464]
 mov rsi, qword ptr [r12 + 472]
 mov rdx, qword ptr [r12 + 480]
 mov rcx, qword ptr [r12 + 488]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n14_α
 mov qword ptr [r12 + 448], rax
 mov qword ptr [r12 + 456], rdx
.Lx19_3:
 jmp xchain0_n19_α
 xchain0_n16_β:
 jmp xchain0_n14_α
xchain0_n17_α:
# IR_LIT_INTEGER
bb18_α:
 mov qword ptr [r12 + 592], 6
 mov rax, qword ptr [rip + .Lx20_0]
 mov qword ptr [r12 + 600], rax
 jmp xchain0_n20_α
 xchain0_n17_β:
 jmp xchain0_n5_α
.Lx20_0:
 .quad 0
xchain0_n18_α:
bb19_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 272]
 cmp eax, 100
 je .Lx21_0
 mov eax, dword ptr [r12 + 352]
 cmp eax, 100
 je .Lx21_0
 mov eax, dword ptr [r12 + 272]
 cmp eax, 6
 jne .Lx21_2
 mov eax, dword ptr [r12 + 352]
 cmp eax, 6
 jne .Lx21_2
.Lx21_1:
 mov rax, qword ptr [r12 + 280]
 mov rcx, qword ptr [r12 + 360]
 cqo
 idiv rcx
 mov qword ptr [r12 + 256], 6
 mov qword ptr [r12 + 264], rax
 jmp xchain0_n21_α
.Lx21_0:
 mov rdi, qword ptr [r12 + 272]
 mov rsi, qword ptr [r12 + 280]
 mov rdx, qword ptr [r12 + 352]
 mov rcx, qword ptr [r12 + 360]
 mov r8d, 3
 lea r9, [r12 + 256]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx21_3
.Lx21_2:
 mov rdi, qword ptr [r12 + 272]
 mov rsi, qword ptr [r12 + 280]
 mov rdx, qword ptr [r12 + 352]
 mov rcx, qword ptr [r12 + 360]
 mov r8d, 3
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n11_α
 mov qword ptr [r12 + 256], rax
 mov qword ptr [r12 + 264], rdx
.Lx21_3:
 jmp xchain0_n21_α
 xchain0_n18_β:
 jmp xchain0_n11_α
xchain0_n19_α:
# IR_LIT_INTEGER
bb20_α:
 mov qword ptr [r12 + 496], 6
 mov rax, qword ptr [rip + .Lx22_0]
 mov qword ptr [r12 + 504], rax
 jmp xchain0_n22_α
 xchain0_n19_β:
 jmp xchain0_n14_α
.Lx22_0:
 .quad 3
xchain0_n20_α:
bb21_α:
# BOX IR_CALL EQ(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [r12+576] -> [r12+544]
 mov rax, qword ptr [r12 + 576]
 mov qword ptr [r12 + 544], rax
 mov rax, qword ptr [r12 + 584]
 mov qword ptr [r12 + 552], rax
# marshal arg1 = producer-box slot [r12+592] -> [r12+560]
 mov rax, qword ptr [r12 + 592]
 mov qword ptr [r12 + 560], rax
 mov rax, qword ptr [r12 + 600]
 mov qword ptr [r12 + 568], rax
  .section .rodata
  .Lrkfn24: .string "EQ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn24]
 lea rsi, [r12 + 544]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 528], rax
 mov qword ptr [r12 + 536], rdx
 cmp eax, 99
 je xchain0_n5_α
 jmp xchain0_n23_α
 xchain0_n20_β:
 jmp xchain0_n5_α
xchain0_n21_α:
bb22_α:
# BOX IR_CALL DUPL(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [r12+240] -> [r12+208]
 mov rax, qword ptr [r12 + 240]
 mov qword ptr [r12 + 208], rax
 mov rax, qword ptr [r12 + 248]
 mov qword ptr [r12 + 216], rax
# marshal arg1 = producer-box slot [r12+256] -> [r12+224]
 mov rax, qword ptr [r12 + 256]
 mov qword ptr [r12 + 224], rax
 mov rax, qword ptr [r12 + 264]
 mov qword ptr [r12 + 232], rax
  .section .rodata
  .Lrkfn26: .string "DUPL"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn26]
 lea rsi, [r12 + 208]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 192], rax
 mov qword ptr [r12 + 200], rdx
 cmp eax, 99
 je xchain0_n11_α
 jmp xchain0_n24_α
 xchain0_n21_β:
 jmp xchain0_n11_α
xchain0_n22_α:
bb23_α:
# BOX IR_CALL REMDR(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [r12+448] -> [r12+416]
 mov rax, qword ptr [r12 + 448]
 mov qword ptr [r12 + 416], rax
 mov rax, qword ptr [r12 + 456]
 mov qword ptr [r12 + 424], rax
# marshal arg1 = producer-box slot [r12+496] -> [r12+432]
 mov rax, qword ptr [r12 + 496]
 mov qword ptr [r12 + 432], rax
 mov rax, qword ptr [r12 + 504]
 mov qword ptr [r12 + 440], rax
  .section .rodata
  .Lrkfn28: .string "REMDR"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn28]
 lea rsi, [r12 + 416]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 400], rax
 mov qword ptr [r12 + 408], rdx
 cmp eax, 99
 je xchain0_n14_α
 jmp xchain0_n25_α
 xchain0_n22_β:
 jmp xchain0_n14_α
xchain0_n23_α:
# IR_ASSIGN global
bb24_α:
 mov rsi, qword ptr [r12 + 528]
 mov rdx, qword ptr [r12 + 536]
 mov rdi, qword ptr [rip + .Lx29_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 512], rax
 mov qword ptr [r12 + 520], rdx
 jmp xchain0_n5_α
 xchain0_n23_β:
 jmp xchain0_n5_α
.Lx29_0:
 .quad .Lx29_0_s
.Lx29_0_s:
 .string "OUTPUT"
xchain0_n24_α:
# IR_VAR gva
bb25_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 368], rax
 mov qword ptr [r12 + 376], rdx
 jmp xchain0_n26_α
 xchain0_n24_β:
 jmp xchain0_n11_α
xchain0_n25_α:
# IR_ASSIGN gva
bb26_α:
 mov rax, qword ptr [r12 + 400]
 mov rdx, qword ptr [r12 + 408]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 384], rax
 mov qword ptr [r12 + 392], rdx
 jmp xchain0_n14_α
 xchain0_n25_β:
 jmp xchain0_n14_α
xchain0_n26_α:
bb27_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 192]
 mov rsi, qword ptr [r12 + 200]
 mov rdx, qword ptr [r12 + 368]
 mov rcx, qword ptr [r12 + 376]
 call str_concat_d@PLT
 mov qword ptr [r12 + 176], rax
 mov qword ptr [r12 + 184], rdx
 jmp xchain0_n27_α
 xchain0_n26_β:
 jmp xchain0_n11_α
xchain0_n27_α:
# IR_ASSIGN global
bb28_α:
 mov rsi, qword ptr [r12 + 176]
 mov rdx, qword ptr [r12 + 184]
 mov rdi, qword ptr [rip + .Lx33_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 160], rax
 mov qword ptr [r12 + 168], rdx
 jmp xchain0_n11_α
 xchain0_n27_β:
 jmp xchain0_n11_α
.Lx33_0:
 .quad .Lx33_0_s
.Lx33_0_s:
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
