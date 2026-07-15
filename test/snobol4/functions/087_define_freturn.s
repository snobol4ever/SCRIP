  .intel_syntax noprefix
  .text
  .globl proc_ispos_α
proc_ispos_α:
#=======================================================================================================================
    .global proc_ispos_α
    .global proc_ispos_β
    .global proc_ispos_γ
    .global proc_ispos_ω
  sub rsp, 688
  mov [rsp+8], rcx
  mov [rsp+16], rdx
  mov [rsp+24], r12
  lea r12, [rsp+32]
  mov rdi, r12
  mov ecx, 656
  xor eax, eax
  rep stosb
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  mov qword ptr [r12 + 648], rsp
 push rsi
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, [rsp + 8]
 mov qword ptr [r12 + 640], rax
 pop rsi
proc_ispos_α_body:
# IR_VAR
 xchain0_n0_α:
 mov rax, qword ptr [rbx + 16]
 mov rdx, qword ptr [rbx + 24]
 mov qword ptr [r12 + 224], rax
 mov qword ptr [r12 + 232], rdx
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp proc_ispos_ω
# IR_LIT_INTEGER
 xchain0_n1_α:
 mov qword ptr [r12 + 192], 6
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [r12 + 200], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp proc_ispos_ω
.Lx2_0:
 .quad 0
# IR_COERCE_NUMERIC
 xchain0_n2_α:
 mov eax, dword ptr [r12 + 224]
 cmp eax, 7
 je .Lx4_1
 cmp eax, 6
 jne .Lx4_0
 mov eax, dword ptr [r12 + 192]
 cmp eax, 6
 jne .Lx4_0
.Lx4_1:
 mov rax, qword ptr [r12 + 224]
 mov qword ptr [r12 + 160], rax
 mov rax, qword ptr [r12 + 232]
 mov qword ptr [r12 + 168], rax
 jmp .Lx4_2
.Lx4_0:
 lea rdi, [r12 + 224]
 lea rsi, [r12 + 192]
 lea rdx, [r12 + 160]
 mov rcx, 111
 call rt_coerce_num2_d@PLT
.Lx4_2:
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp proc_ispos_ω
# IR_COERCE_NUMERIC
 xchain0_n3_α:
 mov eax, dword ptr [r12 + 192]
 cmp eax, 7
 je .Lx6_1
 cmp eax, 6
 jne .Lx6_0
 mov eax, dword ptr [r12 + 224]
 cmp eax, 6
 jne .Lx6_0
.Lx6_1:
 mov rax, qword ptr [r12 + 192]
 mov qword ptr [r12 + 128], rax
 mov rax, qword ptr [r12 + 200]
 mov qword ptr [r12 + 136], rax
 jmp .Lx6_2
.Lx6_0:
 lea rdi, [r12 + 192]
 lea rsi, [r12 + 224]
 lea rdx, [r12 + 128]
 mov rcx, 112
 call rt_coerce_num2_d@PLT
.Lx6_2:
 jmp xchain0_n4_α
 xchain0_n3_β:
 jmp proc_ispos_ω
# IR_CMP_TEST
 xchain0_n4_α:
 lea rdi, [r12 + 160]
 lea rsi, [r12 + 128]
 call rt_cmp_d@PLT
 test eax, eax
 jle proc_ispos_ω
 mov qword ptr [r12 + 96], 0
 mov qword ptr [r12 + 104], 0
 jmp proc_ispos_γ
 xchain0_n4_β:
 jmp proc_ispos_ω
proc_ispos_res:
add rsp, 8
pop r12
proc_ispos_β:
jmp proc_ispos_ω
proc_ispos_γ:
push r12
lea rax, [rip + proc_ispos_res]
push rax
mov rax, [r12-24]
mov r12, [r12-8]
jmp rax
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [r12 + 640]
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
proc_ispos_ω:
mov rax, [r12-16]
lea rsp, [r12 + 656]
mov r12, [r12-8]
jmp rax
proc_startup:
  sub rsp, 8
  .section .rodata
  .Lstartup_pname0: .string "ispos"
  .Lstartup_pp0_0: .string "x"
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
  lea rsi, [rip + proc_ispos_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 1
  call rt_proc_set_nparams@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 656
  call rt_proc_set_frame_bytes@PLT
  add rsp, 8
  ret
  .section .rodata
  .Lgvan0: .string "ispos"
  .Lgvan1: .string "x"
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
  sub rsp, 8
  push rdi
  push rsi
  call core_lib_init@PLT
  call proc_startup
  lea rdi, [rip + __gva_names]
  lea rsi, [rip + __gva]
  mov edx, 2
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
  mov qword ptr [r12 + 648], rsp
 push rsi
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, [rsp + 8]
 mov qword ptr [r12 + 640], rax
 pop rsi
main_α_body:
# IR_LIT_INTEGER
 xchain9_n0_α:
 mov qword ptr [r12 + 304], 6
 mov rax, qword ptr [rip + .Lx10_0]
 mov qword ptr [r12 + 312], rax
 jmp xchain9_n1_α
 xchain9_n0_β:
 jmp xchain9_n3_α
.Lx10_0:
 .quad 5
 xchain9_n1_α:
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov edi, 0
 mov rsi, qword ptr [r12 + 304]
 mov rdx, qword ptr [r12 + 312]
 call rt_arg_stage@PLT
 mov rdi, qword ptr [rip + .Lx12_0]
 mov esi, 1
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx12_1
 call rt_proc_open_fn@PLT
 push r12
 sub rsp, 8
 lea rcx, [rip + .Lx12_3]
 lea rdx, [rip + .Lx12_4]
 mov r12, rsp
 jmp rax
.Lx12_3:
 mov rax, rsp
 mov rax, qword ptr [rax + 8]
 mov rdi, qword ptr [rax + 0]
 mov rsi, qword ptr [rax + 8]
 mov rsp, r12
 add rsp, 8
 pop r12
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx12_2
.Lx12_4:
 mov rsp, r12
 add rsp, 8
 pop r12
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx12_2
.Lx12_1:
 call rt_faildescr@PLT
.Lx12_2:
 mov rsp, [rsp + 8]
 mov qword ptr [r12 + 256], rax
 mov qword ptr [r12 + 264], rdx
 cmp eax, 99
 je xchain9_n3_α
 jmp xchain9_n2_α
 xchain9_n1_β:
 jmp xchain9_n3_α
.Lx12_0:
 .quad .Lx12_0_s
.Lx12_0_s:
 .string "ispos"
# IR_LIT_STRING
 xchain9_n2_α:
 mov qword ptr [r12 + 352], 1
 mov rax, qword ptr [rip + .Lx13_0]
 mov qword ptr [r12 + 360], rax
 jmp xchain9_n4_α
 xchain9_n2_β:
 jmp xchain9_n6_α
.Lx13_0:
 .quad .Lx13_0_s
.Lx13_0_s:
 .string "positive"
# IR_LIT_STRING
 xchain9_n3_α:
 mov qword ptr [r12 + 400], 1
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [r12 + 408], rax
 jmp xchain9_n5_α
 xchain9_n3_β:
 jmp xchain9_n6_α
.Lx14_0:
 .quad .Lx14_0_s
.Lx14_0_s:
 .string "wrong"
# IR_ASSIGN global
 xchain9_n4_α:
 mov rsi, qword ptr [r12 + 352]
 mov rdx, qword ptr [r12 + 360]
 mov rdi, qword ptr [rip + .Lx15_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 336], rax
 mov qword ptr [r12 + 344], rdx
 jmp xchain9_n6_α
 xchain9_n4_β:
 jmp xchain9_n6_α
.Lx15_0:
 .quad .Lx15_0_s
.Lx15_0_s:
 .string "OUTPUT"
# IR_ASSIGN global
 xchain9_n5_α:
 mov rsi, qword ptr [r12 + 400]
 mov rdx, qword ptr [r12 + 408]
 mov rdi, qword ptr [rip + .Lx16_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 384], rax
 mov qword ptr [r12 + 392], rdx
 jmp xchain9_n6_α
 xchain9_n5_β:
 jmp xchain9_n6_α
.Lx16_0:
 .quad .Lx16_0_s
.Lx16_0_s:
 .string "OUTPUT"
# IR_LIT_INTEGER
 xchain9_n6_α:
 mov qword ptr [r12 + 512], 6
 mov rax, qword ptr [rip + .Lx17_0]
 mov qword ptr [r12 + 520], rax
 jmp xchain9_n7_α
 xchain9_n6_β:
 jmp xchain9_n9_α
.Lx17_0:
 .quad 3
# IR_UNOP
 xchain9_n7_α:
 mov rdi, qword ptr [r12 + 512]
 mov rsi, qword ptr [r12 + 520]
 call rt_num_neg@PLT
 mov qword ptr [r12 + 480], rax
 mov qword ptr [r12 + 488], rdx
 jmp xchain9_n8_α
 xchain9_n7_β:
 jmp xchain9_n9_α
 xchain9_n8_α:
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov edi, 0
 mov rsi, qword ptr [r12 + 480]
 mov rdx, qword ptr [r12 + 488]
 call rt_arg_stage@PLT
 mov rdi, qword ptr [rip + .Lx20_0]
 mov esi, 1
 call rt_proc_call_open@PLT
 test rax, rax
 je .Lx20_1
 call rt_proc_open_fn@PLT
 push r12
 sub rsp, 8
 lea rcx, [rip + .Lx20_3]
 lea rdx, [rip + .Lx20_4]
 mov r12, rsp
 jmp rax
.Lx20_3:
 mov rax, rsp
 mov rax, qword ptr [rax + 8]
 mov rdi, qword ptr [rax + 0]
 mov rsi, qword ptr [rax + 8]
 mov rsp, r12
 add rsp, 8
 pop r12
 call rt_proc_call_epilogue_γ@PLT
 jmp .Lx20_2
.Lx20_4:
 mov rsp, r12
 add rsp, 8
 pop r12
 call rt_proc_call_epilogue_ω@PLT
 jmp .Lx20_2
.Lx20_1:
 call rt_faildescr@PLT
.Lx20_2:
 mov rsp, [rsp + 8]
 mov qword ptr [r12 + 432], rax
 mov qword ptr [r12 + 440], rdx
 cmp eax, 99
 je xchain9_n9_α
 jmp xchain9_n10_α
 xchain9_n8_β:
 jmp xchain9_n9_α
.Lx20_0:
 .quad .Lx20_0_s
.Lx20_0_s:
 .string "ispos"
# IR_LIT_STRING
 xchain9_n9_α:
 mov qword ptr [r12 + 608], 1
 mov rax, qword ptr [rip + .Lx21_0]
 mov qword ptr [r12 + 616], rax
 jmp xchain9_n11_α
 xchain9_n9_β:
 jmp main_γ
.Lx21_0:
 .quad .Lx21_0_s
.Lx21_0_s:
 .string "not positive"
# IR_LIT_STRING
 xchain9_n10_α:
 mov qword ptr [r12 + 560], 1
 mov rax, qword ptr [rip + .Lx22_0]
 mov qword ptr [r12 + 568], rax
 jmp xchain9_n12_α
 xchain9_n10_β:
 jmp main_γ
.Lx22_0:
 .quad .Lx22_0_s
.Lx22_0_s:
 .string "wrong"
# IR_ASSIGN global
 xchain9_n11_α:
 mov rsi, qword ptr [r12 + 608]
 mov rdx, qword ptr [r12 + 616]
 mov rdi, qword ptr [rip + .Lx23_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 592], rax
 mov qword ptr [r12 + 600], rdx
 jmp main_γ
 xchain9_n11_β:
 jmp main_γ
.Lx23_0:
 .quad .Lx23_0_s
.Lx23_0_s:
 .string "OUTPUT"
# IR_ASSIGN global
 xchain9_n12_α:
 mov rsi, qword ptr [r12 + 560]
 mov rdx, qword ptr [r12 + 568]
 mov rdi, qword ptr [rip + .Lx24_0]
 call NV_SET_fn@PLT
 mov qword ptr [r12 + 544], rax
 mov qword ptr [r12 + 552], rdx
 jmp main_γ
 xchain9_n12_β:
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
mov rsp, qword ptr [r12 + 648]
pop r12
ret
 push rsp
 push qword ptr [rsp]
 and rsp, -16
 mov rdi, qword ptr [r12 + 640]
 call rt_zls_release_to@PLT
 mov rsp, [rsp + 8]
main_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, qword ptr [r12 + 648]
pop r12
ret
