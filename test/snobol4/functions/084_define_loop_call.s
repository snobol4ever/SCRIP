  .intel_syntax noprefix
  .text
  .globl proc_bump_α
proc_bump_α:
#=======================================================================================================================
    .global proc_bump_α
    .global proc_bump_β
    .global proc_bump_γ
    .global proc_bump_ω
  sub rsp, 896
  mov [rsp+8], rcx
  mov [rsp+16], rdx
  mov [rsp+24], r12
  lea r12, [rsp+32]
  mov rdi, r12
  mov ecx, 864
  xor eax, eax
  rep stosb
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  mov qword ptr [r12 + 856], rsp
 push rsi
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, [rsp + 8]
 mov qword ptr [r12 + 848], rax
 pop rsi
proc_bump_α_body:
# IR_VAR
 xchain0_n0_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 144], rax
 mov qword ptr [r12 + 152], rdx
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp proc_bump_γ
# IR_LIT_INTEGER
 xchain0_n1_α:
 mov qword ptr [r12 + 176], 6
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [r12 + 184], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp proc_bump_γ
.Lx2_0:
 .quad 1
 xchain0_n2_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 144]
 cmp eax, 100
 je .Lx3_0
 mov eax, dword ptr [r12 + 144]
 cmp eax, 6
 jne .Lx3_2
.Lx3_1:
 mov rax, qword ptr [r12 + 152]
 mov rcx, 1
 add rax, rcx
 mov qword ptr [r12 + 112], 6
 mov qword ptr [r12 + 120], rax
 jmp xchain0_n3_α
.Lx3_0:
 mov rdi, qword ptr [r12 + 144]
 mov rsi, qword ptr [r12 + 152]
 mov rdx, qword ptr [r12 + 176]
 mov rcx, qword ptr [r12 + 184]
 mov r8d, 0
 lea r9, [r12 + 112]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx3_3
.Lx3_2:
 mov rdi, qword ptr [r12 + 144]
 mov rsi, qword ptr [r12 + 152]
 mov rdx, qword ptr [r12 + 176]
 mov rcx, qword ptr [r12 + 184]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je proc_bump_γ
 mov qword ptr [r12 + 112], rax
 mov qword ptr [r12 + 120], rdx
.Lx3_3:
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp proc_bump_γ
# IR_ASSIGN gva
 xchain0_n3_α:
 mov rax, qword ptr [r12 + 112]
 mov rdx, qword ptr [r12 + 120]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 96], rax
 mov qword ptr [r12 + 104], rdx
 jmp proc_bump_γ
 xchain0_n3_β:
 jmp proc_bump_γ
proc_bump_res:
add rsp, 8
pop r12
proc_bump_β:
jmp proc_bump_ω
proc_bump_γ:
push r12
lea rax, [rip + proc_bump_res]
push rax
mov rax, [r12-24]
mov r12, [r12-8]
jmp rax
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [r12 + 848]
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
proc_bump_ω:
mov rax, [r12-16]
lea rsp, [r12 + 864]
mov r12, [r12-8]
jmp rax
proc_startup:
  sub rsp, 8
  .section .rodata
  .Lstartup_pname0: .string "bump"
  .Lstartup_pp0_0: .string "v"
  .align 8
  .Lstartup_pnames0:
  .quad .Lstartup_pp0_0
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname0]
  lea rsi, [rip + .Lstartup_pnames0]
  mov edx, 1
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname0]
  lea rsi, [rip + proc_bump_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 1
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 864
  call rt_proc_set_frame_bytes@PLT
  add rsp, 8
  ret
  .section .rodata
  .Lgvan0: .string "bump"
  .Lgvan1: .string "v"
  .Lgvan2: .string "S"
  .Lgvan3: .string "J"
  .align 8
__gva_names:
  .quad .Lgvan0
  .quad .Lgvan1
  .quad .Lgvan2
  .quad .Lgvan3
  .section .bss
  .align 16
__gva: .space 64, 0
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
  call proc_startup
  lea rdi, [rip + __gva_names]
  lea rsi, [rip + __gva]
  mov edx, 4
  call gva_register@PLT
  mov rbx, rax
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
  mov qword ptr [r12 + 856], rsp
 push rsi
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, [rsp + 8]
 mov qword ptr [r12 + 848], rax
 pop rsi
main_α_body:
# IR_LIT_STRING
 xchain5_n0_α:
 mov qword ptr [r12 + 224], 1
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [r12 + 232], rax
 jmp xchain5_n1_α
 xchain5_n0_β:
 jmp xchain5_n2_α
.Lx6_0:
 .quad .Lx6_0_s
.Lx6_0_s:
 .string ""
# IR_ASSIGN gva
 xchain5_n1_α:
 mov rax, qword ptr [r12 + 224]
 mov rdx, qword ptr [r12 + 232]
 mov qword ptr [rbx + 32], rax
 mov qword ptr [rbx + 40], rdx
 mov qword ptr [r12 + 208], rax
 mov qword ptr [r12 + 216], rdx
 jmp xchain5_n2_α
 xchain5_n1_β:
 jmp xchain5_n2_α
# IR_LIT_INTEGER
 xchain5_n2_α:
 mov qword ptr [r12 + 272], 6
 mov rax, qword ptr [rip + .Lx8_0]
 mov qword ptr [r12 + 280], rax
 jmp xchain5_n3_α
 xchain5_n2_β:
 jmp xchain5_n4_α
.Lx8_0:
 .quad 0
# IR_ASSIGN gva
 xchain5_n3_α:
 mov rax, qword ptr [r12 + 272]
 mov rdx, qword ptr [r12 + 280]
 mov qword ptr [rbx + 48], rax
 mov qword ptr [rbx + 56], rdx
 mov qword ptr [r12 + 256], rax
 mov qword ptr [r12 + 264], rdx
 jmp xchain5_n4_α
 xchain5_n3_β:
 jmp xchain5_n4_α
# IR_VAR
 xchain5_n4_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 352], rax
 mov qword ptr [r12 + 360], rdx
 jmp xchain5_n5_α
 xchain5_n4_β:
 jmp xchain5_n6_α
# IR_LIT_INTEGER
 xchain5_n5_α:
 mov qword ptr [r12 + 464], 6
 mov rax, qword ptr [rip + .Lx11_0]
 mov qword ptr [r12 + 472], rax
 jmp xchain5_n7_α
 xchain5_n5_β:
 jmp xchain5_n6_α
.Lx11_0:
 .quad 2
# IR_VAR
 xchain5_n6_α:
 mov rax, qword ptr [rbx + 48]
 mov rdx, qword ptr [rbx + 56]
 mov qword ptr [r12 + 576], rax
 mov qword ptr [r12 + 584], rdx
 jmp xchain5_n8_α
 xchain5_n6_β:
 jmp xchain5_n9_α
# IR_VAR
 xchain5_n7_α:
 mov rax, qword ptr [rbx + 48]
 mov rdx, qword ptr [rbx + 56]
 mov qword ptr [r12 + 496], rax
 mov qword ptr [r12 + 504], rdx
 jmp xchain5_n10_α
 xchain5_n7_β:
 jmp xchain5_n6_α
# IR_LIT_INTEGER
 xchain5_n8_α:
 mov qword ptr [r12 + 608], 6
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [r12 + 616], rax
 jmp xchain5_n11_α
 xchain5_n8_β:
 jmp xchain5_n9_α
.Lx14_0:
 .quad 1
# IR_VAR
 xchain5_n9_α:
 mov rax, qword ptr [rbx + 48]
 mov rdx, qword ptr [rbx + 56]
 mov qword ptr [r12 + 768], rax
 mov qword ptr [r12 + 776], rdx
 jmp xchain5_n12_α
 xchain5_n9_β:
 jmp xchain5_n13_α
 xchain5_n10_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 496]
 cmp eax, 100
 je .Lx16_0
 mov eax, dword ptr [r12 + 496]
 cmp eax, 6
 jne .Lx16_2
.Lx16_1:
 mov rax, 2
 mov rcx, qword ptr [r12 + 504]
 imul rax, rcx
 mov qword ptr [r12 + 432], 6
 mov qword ptr [r12 + 440], rax
 jmp xchain5_n14_α
.Lx16_0:
 mov rdi, qword ptr [r12 + 464]
 mov rsi, qword ptr [r12 + 472]
 mov rdx, qword ptr [r12 + 496]
 mov rcx, qword ptr [r12 + 504]
 mov r8d, 2
 lea r9, [r12 + 432]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx16_3
.Lx16_2:
 mov rdi, qword ptr [r12 + 464]
 mov rsi, qword ptr [r12 + 472]
 mov rdx, qword ptr [r12 + 496]
 mov rcx, qword ptr [r12 + 504]
 mov r8d, 2
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain5_n6_α
 mov qword ptr [r12 + 432], rax
 mov qword ptr [r12 + 440], rdx
.Lx16_3:
 jmp xchain5_n14_α
 xchain5_n10_β:
 jmp xchain5_n6_α
 xchain5_n11_α:
# IR_BINOP_ARITH
 mov eax, dword ptr [r12 + 576]
 cmp eax, 100
 je .Lx17_0
 mov eax, dword ptr [r12 + 576]
 cmp eax, 6
 jne .Lx17_2
.Lx17_1:
 mov rax, qword ptr [r12 + 584]
 mov rcx, 1
 add rax, rcx
 mov qword ptr [r12 + 544], 6
 mov qword ptr [r12 + 552], rax
 jmp xchain5_n15_α
.Lx17_0:
 mov rdi, qword ptr [r12 + 576]
 mov rsi, qword ptr [r12 + 584]
 mov rdx, qword ptr [r12 + 608]
 mov rcx, qword ptr [r12 + 616]
 mov r8d, 0
 lea r9, [r12 + 544]
 call rt_binop_overload@PLT
 test eax, eax
 jne .Lx17_3
.Lx17_2:
 mov rdi, qword ptr [r12 + 576]
 mov rsi, qword ptr [r12 + 584]
 mov rdx, qword ptr [r12 + 608]
 mov rcx, qword ptr [r12 + 616]
 mov r8d, 0
 call rt_num_arith@PLT
 cmp eax, 99
 je xchain5_n9_α
 mov qword ptr [r12 + 544], rax
 mov qword ptr [r12 + 552], rdx
.Lx17_3:
 jmp xchain5_n15_α
 xchain5_n11_β:
 jmp xchain5_n9_α
# IR_LIT_INTEGER
 xchain5_n12_α:
 mov qword ptr [r12 + 736], 6
 mov rax, qword ptr [rip + .Lx18_0]
 mov qword ptr [r12 + 744], rax
 jmp xchain5_n16_α
 xchain5_n12_β:
 jmp xchain5_n13_α
.Lx18_0:
 .quad 5
# IR_VAR
 xchain5_n13_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 816], rax
 mov qword ptr [r12 + 824], rdx
 jmp xchain5_n17_α
 xchain5_n13_β:
 jmp main_γ
 xchain5_n14_α:
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov edi, 0
 mov rsi, qword ptr [r12 + 432]
 mov rdx, qword ptr [r12 + 440]
 call rt_arg_stage@PLT
 mov rdi, qword ptr [rip + .Lx21_0]
 mov esi, 1
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx21_1
 call rt_proc_open_fn@PLT
 push r12
 sub rsp, 8
 lea rcx, [rip + .Lx21_3]
 lea rdx, [rip + .Lx21_4]
 mov r12, rsp
 jmp rax
.Lx21_3:
 mov rax, rsp
 mov rax, qword ptr [rax + 8]
 mov rdi, qword ptr [rax + 0]
 mov rsi, qword ptr [rax + 8]
 mov rsp, r12
 add rsp, 8
 pop r12
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx21_2
.Lx21_4:
 mov rsp, r12
 add rsp, 8
 pop r12
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx21_2
.Lx21_1:
 call rt_faildescr@PLT
.Lx21_2:
 mov rsp, [rsp + 8]
 mov qword ptr [r12 + 384], rax
 mov qword ptr [r12 + 392], rdx
 cmp eax, 99
 je xchain5_n6_α
 jmp xchain5_n18_α
 xchain5_n14_β:
 jmp xchain5_n6_α
.Lx21_0:
 .quad .Lx21_0_s
.Lx21_0_s:
 .string "bump"
# IR_ASSIGN gva
 xchain5_n15_α:
 mov rax, qword ptr [r12 + 544]
 mov rdx, qword ptr [r12 + 552]
 mov qword ptr [rbx + 48], rax
 mov qword ptr [rbx + 56], rdx
 mov qword ptr [r12 + 528], rax
 mov qword ptr [r12 + 536], rdx
 jmp xchain5_n9_α
 xchain5_n15_β:
 jmp xchain5_n9_α
# IR_COERCE_NUMERIC
 xchain5_n16_α:
 mov eax, dword ptr [r12 + 768]
 cmp eax, 7
 je .Lx24_1
 cmp eax, 6
 jne .Lx24_0
 mov eax, dword ptr [r12 + 736]
 cmp eax, 6
 jne .Lx24_0
.Lx24_1:
 mov rax, qword ptr [r12 + 768]
 mov qword ptr [r12 + 704], rax
 mov rax, qword ptr [r12 + 776]
 mov qword ptr [r12 + 712], rax
 jmp .Lx24_2
.Lx24_0:
 lea rdi, [r12 + 768]
 lea rsi, [r12 + 736]
 lea rdx, [r12 + 704]
 mov rcx, 147
 call rt_coerce_num2_d@PLT
.Lx24_2:
 jmp xchain5_n19_α
 xchain5_n16_β:
 jmp xchain5_n13_α
# IR_ASSIGN global
 xchain5_n17_α:
 mov rsi, qword ptr [r12 + 816]
 mov rdx, qword ptr [r12 + 824]
 mov rdi, qword ptr [rip + .Lx25_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 800], rax
 mov qword ptr [r12 + 808], rdx
 jmp main_γ
 xchain5_n17_β:
 jmp main_γ
.Lx25_0:
 .quad .Lx25_0_s
.Lx25_0_s:
 .string "OUTPUT"
 xchain5_n18_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 352]
 mov rsi, qword ptr [r12 + 360]
 mov rdx, qword ptr [r12 + 384]
 mov rcx, qword ptr [r12 + 392]
 call str_concat_d@PLT
 mov qword ptr [r12 + 320], rax
 mov qword ptr [r12 + 328], rdx
 jmp xchain5_n20_α
 xchain5_n18_β:
 jmp xchain5_n6_α
# IR_COERCE_NUMERIC
 xchain5_n19_α:
 mov eax, dword ptr [r12 + 736]
 cmp eax, 7
 je .Lx28_1
 cmp eax, 6
 jne .Lx28_0
 mov eax, dword ptr [r12 + 768]
 cmp eax, 6
 jne .Lx28_0
.Lx28_1:
 mov rax, qword ptr [r12 + 736]
 mov qword ptr [r12 + 672], rax
 mov rax, qword ptr [r12 + 744]
 mov qword ptr [r12 + 680], rax
 jmp .Lx28_2
.Lx28_0:
 lea rdi, [r12 + 736]
 lea rsi, [r12 + 768]
 lea rdx, [r12 + 672]
 mov rcx, 148
 call rt_coerce_num2_d@PLT
.Lx28_2:
 jmp xchain5_n21_α
 xchain5_n19_β:
 jmp xchain5_n13_α
# IR_ASSIGN gva
 xchain5_n20_α:
 mov rax, qword ptr [r12 + 320]
 mov rdx, qword ptr [r12 + 328]
 mov qword ptr [rbx + 32], rax
 mov qword ptr [rbx + 40], rdx
 mov qword ptr [r12 + 304], rax
 mov qword ptr [r12 + 312], rdx
 jmp xchain5_n6_α
 xchain5_n20_β:
 jmp xchain5_n6_α
# IR_CMP_TEST
 xchain5_n21_α:
 lea rdi, [r12 + 704]
 lea rsi, [r12 + 672]
 call rt_cmp_d@PLT
 test eax, eax
 jns xchain5_n13_α
 mov qword ptr [r12 + 640], 0
 mov qword ptr [r12 + 648], 0
 jmp xchain5_n4_α
 xchain5_n21_β:
 jmp xchain5_n13_α
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, qword ptr [r12 + 856]
pop r12
ret
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [r12 + 848]
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
main_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, qword ptr [r12 + 856]
pop r12
ret
