  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "CHARS"
  .Lgvan1: .string "LINES"
  .align 8
__gva_names:
  .quad .Lgvan0
  .quad .Lgvan1
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
  mov edi, 2
  call rt_gva_island@PLT
  mov rsi, rax
  lea rdi, [rip + __gva_names]
  mov edx, 2
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
  mov qword ptr [rsp + 744], rsp
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
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 mov qword ptr [rsp + 272], rax
 mov qword ptr [rsp + 280], rdx
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp xchain0_n5_α
# IR_VAR
 xchain0_n4_α:
 mov rdi, qword ptr [rip + .Lx6_0]
 call NV_GET_fn@PLT
 cmp eax, 99
 je xchain0_n5_α
 mov qword ptr [rsp + 352], rax
 mov qword ptr [rsp + 360], rdx
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp xchain0_n5_α
.Lx6_0:
 .quad .Lx6_0_s
.Lx6_0_s:
 .string "INPUT"
# IR_VAR
 xchain0_n5_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 mov qword ptr [rsp + 608], rax
 mov qword ptr [rsp + 616], rdx
 jmp xchain0_n7_α
 xchain0_n5_β:
 jmp main_γ
 xchain0_n6_α:
# BOX IR_CALL SIZE(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+352] -> [zr+320]
 mov rax, qword ptr [rsp + 352]
 mov qword ptr [rsp + 320], rax
 mov rax, qword ptr [rsp + 360]
 mov qword ptr [rsp + 328], rax
  .section .rodata
  .Lrkfn9: .string "SIZE"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn9]
 lea rsi, [rsp + 320]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rsp + 304], rax
 mov qword ptr [rsp + 312], rdx
 cmp eax, 99
 je xchain0_n5_α
 jmp xchain0_n8_α
 xchain0_n6_β:
 jmp xchain0_n5_α
# IR_LIT_STRING
 xchain0_n7_α:
 mov qword ptr [rsp + 640], 1
 mov rax, qword ptr [rip + .Lx10_0]
 mov qword ptr [rsp + 648], rax
 jmp xchain0_n9_α
 xchain0_n7_β:
 jmp main_γ
.Lx10_0:
 .quad .Lx10_0_s
.Lx10_0_s:
 .string " characters, "
 xchain0_n8_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [rsp + 272]
 cmp eax, 100
 je .Lx11_0
 mov eax, dword ptr [rsp + 304]
 cmp eax, 100
 je .Lx11_0
 mov eax, dword ptr [rsp + 272]
 cmp eax, 6
 jne .Lx11_2
 mov eax, dword ptr [rsp + 304]
 cmp eax, 6
 jne .Lx11_2
.Lx11_1:
 mov rax, qword ptr [rsp + 280]
 mov rcx, qword ptr [rsp + 312]
 add rax, rcx
 mov qword ptr [rsp + 240], 6
 mov qword ptr [rsp + 248], rax
 jmp xchain0_n10_α
.Lx11_0:
 mov rdi, qword ptr [rsp + 272]
 mov rsi, qword ptr [rsp + 280]
 mov rdx, qword ptr [rsp + 304]
 mov rcx, qword ptr [rsp + 312]
 mov r8d, 0
 lea r9, [rsp + 240]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx11_3
.Lx11_2:
 mov rdi, qword ptr [rsp + 272]
 mov rsi, qword ptr [rsp + 280]
 mov rdx, qword ptr [rsp + 304]
 mov rcx, qword ptr [rsp + 312]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n5_α
 mov qword ptr [rsp + 240], rax
 mov qword ptr [rsp + 248], rdx
.Lx11_3:
 jmp xchain0_n10_α
 xchain0_n8_β:
 jmp xchain0_n5_α
 xchain0_n9_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rsp + 608]
 mov rsi, qword ptr [rsp + 616]
 mov rdx, qword ptr [rsp + 640]
 mov rcx, qword ptr [rsp + 648]
 call str_concat_d@PLT
 mov qword ptr [rsp + 576], rax
 mov qword ptr [rsp + 584], rdx
 jmp xchain0_n11_α
 xchain0_n9_β:
 jmp main_γ
# IR_ASSIGN gva
 xchain0_n10_α:
 mov rax, qword ptr [rsp + 240]
 mov rdx, qword ptr [rsp + 248]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rsp + 224], rax
 mov qword ptr [rsp + 232], rdx
 jmp xchain0_n12_α
 xchain0_n10_β:
 jmp xchain0_n5_α
# IR_VAR
 xchain0_n11_α:
 mov rax, qword ptr [1879052304]
 mov rdx, qword ptr [1879052312]
 mov qword ptr [rsp + 672], rax
 mov qword ptr [rsp + 680], rdx
 jmp xchain0_n13_α
 xchain0_n11_β:
 jmp main_γ
# IR_VAR
 xchain0_n12_α:
 mov rax, qword ptr [1879052304]
 mov rdx, qword ptr [1879052312]
 mov qword ptr [rsp + 432], rax
 mov qword ptr [rsp + 440], rdx
 jmp xchain0_n14_α
 xchain0_n12_β:
 jmp xchain0_n3_α
 xchain0_n13_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rsp + 576]
 mov rsi, qword ptr [rsp + 584]
 mov rdx, qword ptr [rsp + 672]
 mov rcx, qword ptr [rsp + 680]
 call str_concat_d@PLT
 mov qword ptr [rsp + 544], rax
 mov qword ptr [rsp + 552], rdx
 jmp xchain0_n15_α
 xchain0_n13_β:
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n14_α:
 mov qword ptr [rsp + 464], 6
 mov rax, qword ptr [rip + .Lx17_0]
 mov qword ptr [rsp + 472], rax
 jmp xchain0_n16_α
 xchain0_n14_β:
 jmp xchain0_n3_α
.Lx17_0:
 .quad 1
# IR_LIT_STRING
 xchain0_n15_α:
 mov qword ptr [rsp + 704], 1
 mov rax, qword ptr [rip + .Lx18_0]
 mov qword ptr [rsp + 712], rax
 jmp xchain0_n17_α
 xchain0_n15_β:
 jmp main_γ
.Lx18_0:
 .quad .Lx18_0_s
.Lx18_0_s:
 .string " lines read"
 xchain0_n16_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [rsp + 432]
 cmp eax, 100
 je .Lx19_0
 mov eax, dword ptr [rsp + 432]
 cmp eax, 6
 jne .Lx19_2
.Lx19_1:
 mov rax, qword ptr [rsp + 440]
 mov rcx, 1
 add rax, rcx
 mov qword ptr [rsp + 400], 6
 mov qword ptr [rsp + 408], rax
 jmp xchain0_n18_α
.Lx19_0:
 mov rdi, qword ptr [rsp + 432]
 mov rsi, qword ptr [rsp + 440]
 mov rdx, qword ptr [rsp + 464]
 mov rcx, qword ptr [rsp + 472]
 mov r8d, 0
 lea r9, [rsp + 400]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx19_3
.Lx19_2:
 mov rdi, qword ptr [rsp + 432]
 mov rsi, qword ptr [rsp + 440]
 mov rdx, qword ptr [rsp + 464]
 mov rcx, qword ptr [rsp + 472]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain0_n3_α
 mov qword ptr [rsp + 400], rax
 mov qword ptr [rsp + 408], rdx
.Lx19_3:
 jmp xchain0_n18_α
 xchain0_n16_β:
 jmp xchain0_n3_α
 xchain0_n17_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rsp + 544]
 mov rsi, qword ptr [rsp + 552]
 mov rdx, qword ptr [rsp + 704]
 mov rcx, qword ptr [rsp + 712]
 call str_concat_d@PLT
 mov qword ptr [rsp + 512], rax
 mov qword ptr [rsp + 520], rdx
 jmp xchain0_n19_α
 xchain0_n17_β:
 jmp main_γ
# IR_ASSIGN gva
 xchain0_n18_α:
 mov rax, qword ptr [rsp + 400]
 mov rdx, qword ptr [rsp + 408]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rsp + 384], rax
 mov qword ptr [rsp + 392], rdx
 jmp xchain0_n3_α
 xchain0_n18_β:
 jmp xchain0_n3_α
# IR_ASSIGN global
 xchain0_n19_α:
 mov rsi, qword ptr [rsp + 512]
 mov rdx, qword ptr [rsp + 520]
 mov rdi, qword ptr [rip + .Lx22_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 496], rax
 mov qword ptr [rsp + 504], rdx
 jmp main_γ
 xchain0_n19_β:
 jmp main_γ
.Lx22_0:
 .quad .Lx22_0_s
.Lx22_0_s:
 .string "OUTPUT"
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rsp + 744]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rsp + 744]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
add rsp, 65544
ret
