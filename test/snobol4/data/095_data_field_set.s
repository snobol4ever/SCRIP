  .intel_syntax noprefix
  .text
proc_startup:
  sub rsp, 8
  .section .rodata
  .Lclassspec0: .string "point(x,y)"
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lclassspec0]
  call record_register@PLT
  add rsp, 8
  ret
  .section .rodata
  .Lgvan0: .string "P"
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
  call proc_startup
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
  mov qword ptr [rsp + 728], rsp
  mov r12, qword ptr [1879048192]
main_α_body:
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [rsp + 144], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [rsp + 152], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp xchain0_n2_α
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "point(x,y)"
 xchain0_n1_α:
# BOX IR_CALL DATA(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+144] -> [zr+112]
 mov rax, qword ptr [rsp + 144]
 mov qword ptr [rsp + 112], rax
 mov rax, qword ptr [rsp + 152]
 mov qword ptr [rsp + 120], rax
  .section .rodata
  .Lrkfn3: .string "DATA"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn3]
 lea rsi, [rsp + 112]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rsp + 96], rax
 mov qword ptr [rsp + 104], rdx
 cmp eax, 99
 je xchain0_n2_α
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp xchain0_n2_α
# IR_LIT_INTEGER
 xchain0_n2_α:
 mov qword ptr [rsp + 256], 6
 mov rax, qword ptr [rip + .Lx4_0]
 mov qword ptr [rsp + 264], rax
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp xchain0_n6_α
.Lx4_0:
 .quad 10
# IR_LIT_INTEGER
 xchain0_n3_α:
 mov qword ptr [rsp + 288], 6
 mov rax, qword ptr [rip + .Lx5_0]
 mov qword ptr [rsp + 296], rax
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp xchain0_n6_α
.Lx5_0:
 .quad 20
 xchain0_n4_α:
# BOX IR_CALL point(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+256] -> [zr+208]
 mov rax, qword ptr [rsp + 256]
 mov qword ptr [rsp + 208], rax
 mov rax, qword ptr [rsp + 264]
 mov qword ptr [rsp + 216], rax
# marshal arg1 = producer-box slot [zr+288] -> [zr+224]
 mov rax, qword ptr [rsp + 288]
 mov qword ptr [rsp + 224], rax
 mov rax, qword ptr [rsp + 296]
 mov qword ptr [rsp + 232], rax
  .section .rodata
  .Lrkfn7: .string "point"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn7]
 lea rsi, [rsp + 208]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [rsp + 192], rax
 mov qword ptr [rsp + 200], rdx
 cmp eax, 99
 je xchain0_n6_α
 jmp xchain0_n5_α
 xchain0_n4_β:
 jmp xchain0_n6_α
# IR_ASSIGN gva
 xchain0_n5_α:
 mov rax, qword ptr [rsp + 192]
 mov rdx, qword ptr [rsp + 200]
 mov qword ptr [1879052288], rax
 mov qword ptr [1879052296], rdx
 mov qword ptr [rsp + 176], rax
 mov qword ptr [rsp + 184], rdx
 jmp xchain0_n6_α
 xchain0_n5_β:
 jmp xchain0_n6_α
# IR_VAR
 xchain0_n6_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 mov qword ptr [rsp + 384], rax
 mov qword ptr [rsp + 392], rdx
 jmp xchain0_n7_α
 xchain0_n6_β:
 jmp xchain0_n8_α
 xchain0_n7_α:
# BOX IR_CALL x(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+384] -> [zr+352]
 mov rax, qword ptr [rsp + 384]
 mov qword ptr [rsp + 352], rax
 mov rax, qword ptr [rsp + 392]
 mov qword ptr [rsp + 360], rax
  .section .rodata
  .Lrkfn11: .string "x"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn11]
 lea rsi, [rsp + 352]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rsp + 336], rax
 mov qword ptr [rsp + 344], rdx
 cmp eax, 99
 je xchain0_n8_α
 jmp xchain0_n9_α
 xchain0_n7_β:
 jmp xchain0_n8_α
# IR_VAR
 xchain0_n8_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 mov qword ptr [rsp + 480], rax
 mov qword ptr [rsp + 488], rdx
 jmp xchain0_n10_α
 xchain0_n8_β:
 jmp xchain0_n11_α
# IR_ASSIGN global
 xchain0_n9_α:
 mov rsi, qword ptr [rsp + 336]
 mov rdx, qword ptr [rsp + 344]
 mov rdi, qword ptr [rip + .Lx13_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 320], rax
 mov qword ptr [rsp + 328], rdx
 jmp xchain0_n8_α
 xchain0_n9_β:
 jmp xchain0_n8_α
.Lx13_0:
 .quad .Lx13_0_s
.Lx13_0_s:
 .string "OUTPUT"
 xchain0_n10_α:
# BOX IR_CALL y(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+480] -> [zr+448]
 mov rax, qword ptr [rsp + 480]
 mov qword ptr [rsp + 448], rax
 mov rax, qword ptr [rsp + 488]
 mov qword ptr [rsp + 456], rax
  .section .rodata
  .Lrkfn15: .string "y"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn15]
 lea rsi, [rsp + 448]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rsp + 432], rax
 mov qword ptr [rsp + 440], rdx
 cmp eax, 99
 je xchain0_n11_α
 jmp xchain0_n12_α
 xchain0_n10_β:
 jmp xchain0_n11_α
# IR_VAR
 xchain0_n11_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 mov qword ptr [rsp + 512], rax
 mov qword ptr [rsp + 520], rdx
 jmp xchain0_n13_α
 xchain0_n11_β:
 jmp xchain0_n14_α
# IR_ASSIGN global
 xchain0_n12_α:
 mov rsi, qword ptr [rsp + 432]
 mov rdx, qword ptr [rsp + 440]
 mov rdi, qword ptr [rip + .Lx17_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 416], rax
 mov qword ptr [rsp + 424], rdx
 jmp xchain0_n11_α
 xchain0_n12_β:
 jmp xchain0_n11_α
.Lx17_0:
 .quad .Lx17_0_s
.Lx17_0_s:
 .string "OUTPUT"
# IR_FIELD_GET lv
 xchain0_n13_α:
 mov rdi, qword ptr [rip + .Lx18_0]
 mov rsi, qword ptr [rsp + 512]
 mov rdx, qword ptr [rsp + 520]
 call rt_field_var@PLT
 cmp eax, 99
 je xchain0_n14_α
 mov qword ptr [rsp + 544], rax
 mov qword ptr [rsp + 552], rdx
 jmp xchain0_n15_α
 xchain0_n13_β:
 jmp xchain0_n14_α
.Lx18_0:
 .quad .Lx18_0_s
.Lx18_0_s:
 .string "x"
# IR_VAR
 xchain0_n14_α:
 mov rax, qword ptr [1879052288]
 mov rdx, qword ptr [1879052296]
 mov qword ptr [rsp + 688], rax
 mov qword ptr [rsp + 696], rdx
 jmp xchain0_n16_α
 xchain0_n14_β:
 jmp main_γ
# IR_LIT_INTEGER
 xchain0_n15_α:
 mov qword ptr [rsp + 576], 6
 mov rax, qword ptr [rip + .Lx20_0]
 mov qword ptr [rsp + 584], rax
 jmp xchain0_n17_α
 xchain0_n15_β:
 jmp xchain0_n14_α
.Lx20_0:
 .quad 99
 xchain0_n16_α:
# BOX IR_CALL x(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+688] -> [zr+656]
 mov rax, qword ptr [rsp + 688]
 mov qword ptr [rsp + 656], rax
 mov rax, qword ptr [rsp + 696]
 mov qword ptr [rsp + 664], rax
  .section .rodata
  .Lrkfn22: .string "x"
  .section .text
  .intel_syntax noprefix
 lea rdi, [rip + .Lrkfn22]
 lea rsi, [rsp + 656]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [rsp + 640], rax
 mov qword ptr [rsp + 648], rdx
 cmp eax, 99
 je main_γ
 jmp xchain0_n18_α
 xchain0_n16_β:
 jmp main_γ
# IR_ASSIGN_VAR
 xchain0_n17_α:
 mov rdi, qword ptr [rsp + 544]
 mov rsi, qword ptr [rsp + 552]
 mov rdx, qword ptr [rsp + 576]
 mov rcx, qword ptr [rsp + 584]
 call rt_assign_var@PLT
 cmp eax, 99
 je xchain0_n14_α
 mov qword ptr [rsp + 608], rax
 mov qword ptr [rsp + 616], rdx
 jmp xchain0_n14_α
 xchain0_n17_β:
 jmp xchain0_n14_α
# IR_ASSIGN global
 xchain0_n18_α:
 mov rsi, qword ptr [rsp + 640]
 mov rdx, qword ptr [rsp + 648]
 mov rdi, qword ptr [rip + .Lx24_0]
 call NV_SET_fn@PLT
 mov qword ptr [rsp + 624], rax
 mov qword ptr [rsp + 632], rdx
 jmp main_γ
 xchain0_n18_β:
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
mov rsp, qword ptr [rsp + 728]
add rsp, 65544
ret
main_ω:
mov rsp, qword ptr [rsp + 728]
mov dword ptr [rsp+0], 99
mov dword ptr [rsp+4], 0
mov qword ptr [rsp+8], 0
mov eax, 99
xor edx, edx
add rsp, 65544
ret
