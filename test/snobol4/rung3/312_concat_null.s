  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "x"
  .Lgvan1: .string "output"
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
  mov qword ptr [rsp + 680], rsp
  mov r12, qword ptr [1879048192]
  mov rbp, rsp
main_α_body:
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [rsp + 112], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [rsp + 120], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n2_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "hello"
# IR_ASSIGN gva
 xchain0_n1_α:
 mov rax, qword ptr [rsp + 112]
 mov rdx, qword ptr [rsp + 120]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rsp + 96], rax
 mov qword ptr [rsp + 104], rdx
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n2_α
# IR_VAR
 xchain0_n2_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 mov qword ptr [rsp + 240], rax
 mov qword ptr [rsp + 248], rdx
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n4_α
# IR_LIT_STRING
 xchain0_n3_α:
 mov qword ptr [rsp + 272], 1
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [rsp + 280], rax
 jmp xchain0_n5_α
 xchain0_n3_β:
 jmp xchain0_n4_α
.Lx4_0:
 .quad .Lx4_0_s
.Lx4_0_s:
 .string ""
# IR_LIT_STRING
 xchain0_n4_α:
 mov qword ptr [rsp + 480], 1
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [rsp + 488], rax
 jmp xchain0_n6_α
 xchain0_n4_β:
 jmp xchain0_n9_α
.Lx5_0:
 .quad .Lx5_0_s
.Lx5_0_s:
 .string ""
 xchain0_n5_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rsp + 240]
 mov rsi, qword ptr [rsp + 248]
 mov rdx, qword ptr [rsp + 272]
 mov rcx, qword ptr [rsp + 280]
 call str_concat_d@PLT
 mov qword ptr [rsp + 208], rax
 mov qword ptr [rsp + 216], rdx
 jmp xchain0_n7_α
 xchain0_n5_β:
 jmp xchain0_n4_α
# IR_VAR
 xchain0_n6_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 mov qword ptr [rsp + 512], rax
 mov qword ptr [rsp + 520], rdx
 jmp xchain0_n8_α
 xchain0_n6_β:
 jmp xchain0_n9_α
# IR_VAR
 xchain0_n7_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 mov qword ptr [rsp + 304], rax
 mov qword ptr [rsp + 312], rdx
 jmp xchain0_n10_α
 xchain0_n7_β:
 jmp xchain0_n4_α
 xchain0_n8_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [rsp + 480]
 mov rsi, qword ptr [rsp + 488]
 mov rdx, qword ptr [rsp + 512]
 mov rcx, qword ptr [rsp + 520]
 call str_concat_d@PLT
 mov qword ptr [rsp + 448], rax
 mov qword ptr [rsp + 456], rdx
 jmp xchain0_n11_α
 xchain0_n8_β:
 jmp xchain0_n9_α
# IR_LIT_STRING
 xchain0_n9_α:
 mov qword ptr [rsp + 640], 1
 mov rax, qword ptr [rip + .Lx10_0]
 mov qword ptr [rsp + 648], rax
 jmp xchain0_n12_α
 xchain0_n9_β:
 jmp main_γ
.Lx10_0:
 .quad .Lx10_0_s
.Lx10_0_s:
 .string "PASS 312_concat_null (2/2)"
 xchain0_n10_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+208] -> [zr+160]
 mov rax, qword ptr [rsp + 208]
 mov qword ptr [rsp + 160], rax
 mov rax, qword ptr [rsp + 216]
 mov qword ptr [rsp + 168], rax
# marshal arg1 = producer-box slot [zr+304] -> [zr+176]
 mov rax, qword ptr [rsp + 304]
 mov qword ptr [rsp + 176], rax
 mov rax, qword ptr [rsp + 312]
 mov qword ptr [rsp + 184], rax
  .section .rodata
  .Lbynamefn11: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn11]
 lea rsi, [rsp + 160]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 144], rax
 mov qword ptr [rsp + 152], rdx
 cmp eax, 99
 je xchain0_n4_α
 jmp xchain0_n13_α
 xchain0_n10_β:
 jmp xchain0_n4_α
# IR_VAR
 xchain0_n11_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 mov qword ptr [rsp + 544], rax
 mov qword ptr [rsp + 552], rdx
 jmp xchain0_n14_α
 xchain0_n11_β:
 jmp xchain0_n9_α
# IR_ASSIGN gva
 xchain0_n12_α:
 mov rax, qword ptr [rsp + 640]
 mov rdx, qword ptr [rsp + 648]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rsp + 624], rax
 mov qword ptr [rsp + 632], rdx
 jmp main_γ
 xchain0_n12_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n13_α:
 mov qword ptr [rsp + 352], 1
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [rsp + 360], rax
 jmp xchain0_n15_α
 xchain0_n13_β:
 jmp main_γ
.Lx14_0:
 .quad .Lx14_0_s
.Lx14_0_s:
 .string "FAIL 312/001: null right identity"
 xchain0_n14_α:
# BOX CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+448] -> [zr+400]
 mov rax, qword ptr [rsp + 448]
 mov qword ptr [rsp + 400], rax
 mov rax, qword ptr [rsp + 456]
 mov qword ptr [rsp + 408], rax
# marshal arg1 = producer-box slot [zr+544] -> [zr+416]
 mov rax, qword ptr [rsp + 544]
 mov qword ptr [rsp + 416], rax
 mov rax, qword ptr [rsp + 552]
 mov qword ptr [rsp + 424], rax
  .section .rodata
  .Lbynamefn15: .string "differ"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lbynamefn15]
 lea rsi, [rsp + 400]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 384], rax
 mov qword ptr [rsp + 392], rdx
 cmp eax, 99
 je xchain0_n9_α
 jmp xchain0_n16_α
 xchain0_n14_β:
 jmp xchain0_n9_α
# IR_ASSIGN gva
 xchain0_n15_α:
 mov rax, qword ptr [rsp + 352]
 mov rdx, qword ptr [rsp + 360]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rsp + 336], rax
 mov qword ptr [rsp + 344], rdx
 jmp main_γ
 xchain0_n15_β:
 jmp main_γ
# IR_LIT_STRING
 xchain0_n16_α:
 mov qword ptr [rsp + 592], 1
 mov rax, qword ptr [rip + .Lx17_0]
 mov qword ptr [rsp + 600], rax
 jmp xchain0_n17_α
 xchain0_n16_β:
 jmp main_γ
.Lx17_0:
 .quad .Lx17_0_s
.Lx17_0_s:
 .string "FAIL 312/002: null left identity"
# IR_ASSIGN gva
 xchain0_n17_α:
 mov rax, qword ptr [rsp + 592]
 mov rdx, qword ptr [rsp + 600]
 mov qword ptr [1879052304], rax
 mov qword ptr [1879052312], rdx
 mov qword ptr [rsp + 576], rax
 mov qword ptr [rsp + 584], rdx
 jmp main_γ
 xchain0_n17_β:
 jmp main_γ
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [rsp + 680]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rsp + 680]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
add rsp, 65544
ret
