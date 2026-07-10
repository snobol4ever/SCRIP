  .intel_syntax noprefix
  .text
  .globl proc_EXPR$0_α
proc_EXPR$0_α:
#=======================================================================================================================
    .global proc_EXPR$0_α
    .global proc_EXPR$0_β
    .global proc_EXPR$0_γ
    .global proc_EXPR$0_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 80], rax
 pop rsi
proc_EXPR$0_α_body:
# IR_LIT_STRING
 xchain0_n0_α:
 mov qword ptr [r12 + 48], 1
 mov rax, qword ptr [rip + .Lx1_0]
 mov qword ptr [r12 + 56], rax
 jmp xchain0_n1_α
 xchain0_n0_β:
 jmp proc_EXPR$0_ω
.Lx1_0:
 .quad .Lx1_0_s
.Lx1_0_s:
 .string "abc"
# IR_LIT_STRING
 xchain0_n1_α:
 mov qword ptr [r12 + 64], 1
 mov rax, qword ptr [rip + .Lx2_0]
 mov qword ptr [r12 + 72], rax
 jmp xchain0_n2_α
 xchain0_n1_β:
 jmp proc_EXPR$0_ω
.Lx2_0:
 .quad .Lx2_0_s
.Lx2_0_s:
 .string "def"
 xchain0_n2_α:
# IR_BINOP_CONCAT
 mov rdi, qword ptr [r12 + 48]
 mov rsi, qword ptr [r12 + 56]
 mov rdx, qword ptr [r12 + 64]
 mov rcx, qword ptr [r12 + 72]
 call str_concat_d@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain0_n3_α
 xchain0_n2_β:
 jmp proc_EXPR$0_ω
# IR_ASSIGN gva
 xchain0_n3_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 80], rax
 mov qword ptr [rbx + 88], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$0_γ
 xchain0_n3_β:
 jmp proc_EXPR$0_ω
proc_EXPR$0_β:
jmp proc_EXPR$0_ω
proc_EXPR$0_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 80]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$0_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$1_α
proc_EXPR$1_α:
#=======================================================================================================================
    .global proc_EXPR$1_α
    .global proc_EXPR$1_β
    .global proc_EXPR$1_γ
    .global proc_EXPR$1_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 48], rax
 pop rsi
proc_EXPR$1_α_body:
# IR_VAR
 xchain5_n0_α:
 mov rax, qword ptr [rbx + 32]
 mov rdx, qword ptr [rbx + 40]
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp xchain5_n1_α
 xchain5_n0_β:
 jmp proc_EXPR$1_ω
# IR_ASSIGN gva
 xchain5_n1_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 96], rax
 mov qword ptr [rbx + 104], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$1_γ
 xchain5_n1_β:
 jmp proc_EXPR$1_ω
proc_EXPR$1_β:
jmp proc_EXPR$1_ω
proc_EXPR$1_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 48]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$1_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
  .globl proc_EXPR$2_α
proc_EXPR$2_α:
#=======================================================================================================================
    .global proc_EXPR$2_α
    .global proc_EXPR$2_β
    .global proc_EXPR$2_γ
    .global proc_EXPR$2_ω
push r12
  mov r12, rdi
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 112], rax
 pop rsi
proc_EXPR$2_α_body:
# IR_LIT_INTEGER
 xchain8_n0_α:
 mov qword ptr [r12 + 80], 6
 mov rax, qword ptr [rip + .Lx9_0]
 mov qword ptr [r12 + 88], rax
 jmp xchain8_n1_α
 xchain8_n0_β:
 jmp proc_EXPR$2_ω
.Lx9_0:
 .quad 1
# IR_LIT_INTEGER
 xchain8_n1_α:
 mov qword ptr [r12 + 96], 6
 mov rax, qword ptr [rip + .Lx10_0]
 mov qword ptr [r12 + 104], rax
 jmp xchain8_n2_α
 xchain8_n1_β:
 jmp proc_EXPR$2_ω
.Lx10_0:
 .quad 2
 xchain8_n2_α:
# BOX IR_CALL ident(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+80] -> [zr+48]
 mov rax, qword ptr [r12 + 80]
 mov qword ptr [r12 + 48], rax
 mov rax, qword ptr [r12 + 88]
 mov qword ptr [r12 + 56], rax
# marshal arg1 = producer-box slot [zr+96] -> [zr+64]
 mov rax, qword ptr [r12 + 96]
 mov qword ptr [r12 + 64], rax
 mov rax, qword ptr [r12 + 104]
 mov qword ptr [r12 + 72], rax
  .section .rodata
  .Lbynamefn12: .string "ident"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn12]
 lea rsi, [r12 + 48]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je proc_EXPR$2_ω
 jmp xchain8_n3_α
xchain8_n2_β:
 jmp proc_EXPR$2_ω
# IR_ASSIGN gva
 xchain8_n3_α:
 mov rax, qword ptr [r12 + 32]
 mov rdx, qword ptr [r12 + 40]
 mov qword ptr [rbx + 112], rax
 mov qword ptr [rbx + 120], rdx
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp proc_EXPR$2_γ
 xchain8_n3_β:
 jmp proc_EXPR$2_ω
proc_EXPR$2_β:
jmp proc_EXPR$2_ω
proc_EXPR$2_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 112]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
proc_EXPR$2_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
proc_startup:
  push rbp
  mov rbp, rsp
  .section .rodata
  .Lstartup_pname0: .string "EXPR$0"
  .align 8
  .Lstartup_pnames0:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname0]
  lea rsi, [rip + .Lstartup_pnames0]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname0]
  lea rsi, [rip + proc_EXPR$0_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname0]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  .section .rodata
  .Lstartup_pname1: .string "EXPR$1"
  .align 8
  .Lstartup_pnames1:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname1]
  lea rsi, [rip + .Lstartup_pnames1]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname1]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname1]
  lea rsi, [rip + proc_EXPR$1_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname1]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  .section .rodata
  .Lstartup_pname2: .string "EXPR$2"
  .align 8
  .Lstartup_pnames2:
  .quad 0
  .section .text
  .intel_syntax noprefix
  lea rdi, [rip + .Lstartup_pname2]
  lea rsi, [rip + .Lstartup_pnames2]
  mov edx, 0
  call rt_proc_register@PLT
  lea rdi, [rip + .Lstartup_pname2]
  mov esi, 1
  call rt_proc_set_dyn_scope@PLT
  lea rdi, [rip + .Lstartup_pname2]
  lea rsi, [rip + proc_EXPR$2_α]
  call rt_proc_set_fn@PLT
  lea rdi, [rip + .Lstartup_pname2]
  mov esi, 0
  call rt_proc_set_nparams@PLT
  pop rbp
  ret
  .section .rodata
  .Lgvan0: .string "expr"
  .Lgvan1: .string "output"
  .Lgvan2: .string "q"
  .Lgvan3: .string "sexp"
  .Lgvan4: .string "fexp"
  .Lgvan5: .string "EXPR$0"
  .Lgvan6: .string "EXPR$1"
  .Lgvan7: .string "EXPR$2"
  .align 8
__gva_names:
  .quad .Lgvan0
  .quad .Lgvan1
  .quad .Lgvan2
  .quad .Lgvan3
  .quad .Lgvan4
  .quad .Lgvan5
  .quad .Lgvan6
  .quad .Lgvan7
  .section .bss
  .align 16
__gva: .space 128, 0
  .section .text
  .intel_syntax noprefix
  .globl main
main:
  push rbp
  mov rbp, rsp
  push rdi
  push rsi
  call core_lib_init@PLT
  call proc_startup
  lea rdi, [rip + __gva_names]
  lea rsi, [rip + __gva]
  mov edx, 8
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
  lea rax, [rip + g_gva_base]
  mov rbx, qword ptr [rax]
  push rbp
  mov rbp, rsp
  sub rsp, 8
 push rsi
 push rbp
 mov rbp, rsp
 and rsp, -16
 call rt_zls_mark@PLT
 mov rsp, rbp
 pop rbp
 mov qword ptr [r12 + 688], rax
 pop rsi
main_α_body:
# IR_LIT_STRING
 xchain14_n0_α:
 mov qword ptr [r12 + 112], 1
 mov rax, qword ptr [rip + .Lx15_0]
 mov qword ptr [r12 + 120], rax
 jmp xchain14_n1_α
 xchain14_n0_β:
 jmp xchain14_n3_α
.Lx15_0:
 .quad .Lx15_0_s
.Lx15_0_s:
 .string "EXPR$0"
 xchain14_n1_α:
# BOX IR_CALL SNO$MKEXPR(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+112] -> [zr+96]
 mov rax, qword ptr [r12 + 112]
 mov qword ptr [r12 + 96], rax
 mov rax, qword ptr [r12 + 120]
 mov qword ptr [r12 + 104], rax
  .section .rodata
  .Lrkfn17: .string "SNO$MKEXPR"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn17]
 lea rsi, [r12 + 96]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 80], rax
 mov qword ptr [r12 + 88], rdx
 cmp eax, 99
 je xchain14_n3_α
 jmp xchain14_n2_α
 xchain14_n1_β:
 jmp xchain14_n3_α
# IR_ASSIGN gva
 xchain14_n2_α:
 mov rax, qword ptr [r12 + 80]
 mov rdx, qword ptr [r12 + 88]
 mov qword ptr [rbx + 0], rax
 mov qword ptr [rbx + 8], rdx
 mov qword ptr [r12 + 64], rax
 mov qword ptr [r12 + 72], rdx
 jmp xchain14_n3_α
 xchain14_n2_β:
 jmp xchain14_n3_α
# IR_VAR
 xchain14_n3_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 208], rax
 mov qword ptr [r12 + 216], rdx
 jmp xchain14_n4_α
 xchain14_n3_β:
 jmp xchain14_n5_α
 xchain14_n4_α:
# BOX IR_CALL eval(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+208] -> [zr+192]
 mov rax, qword ptr [r12 + 208]
 mov qword ptr [r12 + 192], rax
 mov rax, qword ptr [r12 + 216]
 mov qword ptr [r12 + 200], rax
  .section .rodata
  .Lbynamefn21: .string "eval"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn21]
 lea rsi, [r12 + 192]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 176], rax
 mov qword ptr [r12 + 184], rdx
 cmp eax, 99
 je xchain14_n5_α
 jmp xchain14_n6_α
xchain14_n4_β:
 jmp xchain14_n5_α
# IR_LIT_STRING
 xchain14_n5_α:
 mov qword ptr [r12 + 288], 1
 mov rax, qword ptr [rip + .Lx22_0]
 mov qword ptr [r12 + 296], rax
 jmp xchain14_n7_α
 xchain14_n5_β:
 jmp xchain14_n9_α
.Lx22_0:
 .quad .Lx22_0_s
.Lx22_0_s:
 .string "qqq"
# IR_LIT_STRING
 xchain14_n6_α:
 mov qword ptr [r12 + 224], 1
 mov rax, qword ptr [rip + .Lx23_0]
 mov qword ptr [r12 + 232], rax
 jmp xchain14_n8_α
 xchain14_n6_β:
 jmp xchain14_n5_α
.Lx23_0:
 .quad .Lx23_0_s
.Lx23_0_s:
 .string "abcdef"
# IR_ASSIGN gva
 xchain14_n7_α:
 mov rax, qword ptr [r12 + 288]
 mov rdx, qword ptr [r12 + 296]
 mov qword ptr [rbx + 32], rax
 mov qword ptr [rbx + 40], rdx
 mov qword ptr [r12 + 272], rax
 mov qword ptr [r12 + 280], rdx
 jmp xchain14_n9_α
 xchain14_n7_β:
 jmp xchain14_n9_α
 xchain14_n8_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+176] -> [zr+144]
 mov rax, qword ptr [r12 + 176]
 mov qword ptr [r12 + 144], rax
 mov rax, qword ptr [r12 + 184]
 mov qword ptr [r12 + 152], rax
# marshal arg1 = producer-box slot [zr+224] -> [zr+160]
 mov rax, qword ptr [r12 + 224]
 mov qword ptr [r12 + 160], rax
 mov rax, qword ptr [r12 + 232]
 mov qword ptr [r12 + 168], rax
  .section .rodata
  .Lbynamefn26: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn26]
 lea rsi, [r12 + 144]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 128], rax
 mov qword ptr [r12 + 136], rdx
 cmp eax, 99
 je xchain14_n5_α
 jmp xchain14_n10_α
xchain14_n8_β:
 jmp xchain14_n5_α
# IR_LIT_STRING
 xchain14_n9_α:
 mov qword ptr [r12 + 352], 1
 mov rax, qword ptr [rip + .Lx27_0]
 mov qword ptr [r12 + 360], rax
 jmp xchain14_n11_α
 xchain14_n9_β:
 jmp xchain14_n14_α
.Lx27_0:
 .quad .Lx27_0_s
.Lx27_0_s:
 .string "EXPR$1"
# IR_LIT_STRING
 xchain14_n10_α:
 mov qword ptr [r12 + 256], 1
 mov rax, qword ptr [rip + .Lx28_0]
 mov qword ptr [r12 + 264], rax
 jmp xchain14_n12_α
 xchain14_n10_β:
 jmp main_γ
.Lx28_0:
 .quad .Lx28_0_s
.Lx28_0_s:
 .string "FAIL 1016/001: eval concat expr"
 xchain14_n11_α:
# BOX IR_CALL SNO$MKEXPR(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+352] -> [zr+336]
 mov rax, qword ptr [r12 + 352]
 mov qword ptr [r12 + 336], rax
 mov rax, qword ptr [r12 + 360]
 mov qword ptr [r12 + 344], rax
  .section .rodata
  .Lrkfn30: .string "SNO$MKEXPR"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn30]
 lea rsi, [r12 + 336]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 320], rax
 mov qword ptr [r12 + 328], rdx
 cmp eax, 99
 je xchain14_n14_α
 jmp xchain14_n13_α
 xchain14_n11_β:
 jmp xchain14_n14_α
# IR_ASSIGN gva
 xchain14_n12_α:
 mov rax, qword ptr [r12 + 256]
 mov rdx, qword ptr [r12 + 264]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 240], rax
 mov qword ptr [r12 + 248], rdx
 jmp main_γ
 xchain14_n12_β:
 jmp main_γ
# IR_ASSIGN gva
 xchain14_n13_α:
 mov rax, qword ptr [r12 + 320]
 mov rdx, qword ptr [r12 + 328]
 mov qword ptr [rbx + 48], rax
 mov qword ptr [rbx + 56], rdx
 mov qword ptr [r12 + 304], rax
 mov qword ptr [r12 + 312], rdx
 jmp xchain14_n14_α
 xchain14_n13_β:
 jmp xchain14_n14_α
# IR_VAR
 xchain14_n14_α:
 mov rax, qword ptr [rbx + 48]
 mov rdx, qword ptr [rbx + 56]
 mov qword ptr [r12 + 448], rax
 mov qword ptr [r12 + 456], rdx
 jmp xchain14_n15_α
 xchain14_n14_β:
 jmp xchain14_n16_α
 xchain14_n15_α:
# BOX IR_CALL eval(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+448] -> [zr+432]
 mov rax, qword ptr [r12 + 448]
 mov qword ptr [r12 + 432], rax
 mov rax, qword ptr [r12 + 456]
 mov qword ptr [r12 + 440], rax
  .section .rodata
  .Lbynamefn35: .string "eval"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn35]
 lea rsi, [r12 + 432]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 416], rax
 mov qword ptr [r12 + 424], rdx
 cmp eax, 99
 je xchain14_n16_α
 jmp xchain14_n17_α
xchain14_n15_β:
 jmp xchain14_n16_α
# IR_LIT_STRING
 xchain14_n16_α:
 mov qword ptr [r12 + 560], 1
 mov rax, qword ptr [rip + .Lx36_0]
 mov qword ptr [r12 + 568], rax
 jmp xchain14_n18_α
 xchain14_n16_β:
 jmp xchain14_n21_α
.Lx36_0:
 .quad .Lx36_0_s
.Lx36_0_s:
 .string "EXPR$2"
# IR_LIT_STRING
 xchain14_n17_α:
 mov qword ptr [r12 + 464], 1
 mov rax, qword ptr [rip + .Lx37_0]
 mov qword ptr [r12 + 472], rax
 jmp xchain14_n19_α
 xchain14_n17_β:
 jmp xchain14_n16_α
.Lx37_0:
 .quad .Lx37_0_s
.Lx37_0_s:
 .string "qqq"
 xchain14_n18_α:
# BOX IR_CALL SNO$MKEXPR(...) -> rt_call_arr [operand-marshal, FAIL->ω]
# marshal arg0 = producer-box slot [zr+560] -> [zr+544]
 mov rax, qword ptr [r12 + 560]
 mov qword ptr [r12 + 544], rax
 mov rax, qword ptr [r12 + 568]
 mov qword ptr [r12 + 552], rax
  .section .rodata
  .Lrkfn39: .string "SNO$MKEXPR"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lrkfn39]
 lea rsi, [r12 + 544]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 528], rax
 mov qword ptr [r12 + 536], rdx
 cmp eax, 99
 je xchain14_n21_α
 jmp xchain14_n20_α
 xchain14_n18_β:
 jmp xchain14_n21_α
 xchain14_n19_α:
# BOX IR_CALL differ(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+416] -> [zr+384]
 mov rax, qword ptr [r12 + 416]
 mov qword ptr [r12 + 384], rax
 mov rax, qword ptr [r12 + 424]
 mov qword ptr [r12 + 392], rax
# marshal arg1 = producer-box slot [zr+464] -> [zr+400]
 mov rax, qword ptr [r12 + 464]
 mov qword ptr [r12 + 400], rax
 mov rax, qword ptr [r12 + 472]
 mov qword ptr [r12 + 408], rax
  .section .rodata
  .Lbynamefn41: .string "differ"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn41]
 lea rsi, [r12 + 384]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 368], rax
 mov qword ptr [r12 + 376], rdx
 cmp eax, 99
 je xchain14_n16_α
 jmp xchain14_n22_α
xchain14_n19_β:
 jmp xchain14_n16_α
# IR_ASSIGN gva
 xchain14_n20_α:
 mov rax, qword ptr [r12 + 528]
 mov rdx, qword ptr [r12 + 536]
 mov qword ptr [rbx + 64], rax
 mov qword ptr [rbx + 72], rdx
 mov qword ptr [r12 + 512], rax
 mov qword ptr [r12 + 520], rdx
 jmp xchain14_n21_α
 xchain14_n20_β:
 jmp xchain14_n21_α
# IR_VAR
 xchain14_n21_α:
 mov rax, qword ptr [rbx + 64]
 mov rdx, qword ptr [rbx + 72]
 mov qword ptr [r12 + 608], rax
 mov qword ptr [r12 + 616], rdx
 jmp xchain14_n23_α
 xchain14_n21_β:
 jmp xchain14_n24_α
# IR_LIT_STRING
 xchain14_n22_α:
 mov qword ptr [r12 + 496], 1
 mov rax, qword ptr [rip + .Lx44_0]
 mov qword ptr [r12 + 504], rax
 jmp xchain14_n25_α
 xchain14_n22_β:
 jmp main_γ
.Lx44_0:
 .quad .Lx44_0_s
.Lx44_0_s:
 .string "FAIL 1016/002: eval var ref"
 xchain14_n23_α:
# BOX IR_CALL eval(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [zr+608] -> [zr+592]
 mov rax, qword ptr [r12 + 608]
 mov qword ptr [r12 + 592], rax
 mov rax, qword ptr [r12 + 616]
 mov qword ptr [r12 + 600], rax
  .section .rodata
  .Lbynamefn46: .string "eval"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn46]
 lea rsi, [r12 + 592]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 576], rax
 mov qword ptr [r12 + 584], rdx
 cmp eax, 99
 je xchain14_n24_α
 jmp xchain14_n26_α
xchain14_n23_β:
 jmp xchain14_n24_α
# IR_LIT_STRING
 xchain14_n24_α:
 mov qword ptr [r12 + 672], 1
 mov rax, qword ptr [rip + .Lx47_0]
 mov qword ptr [r12 + 680], rax
 jmp xchain14_n27_α
 xchain14_n24_β:
 jmp main_γ
.Lx47_0:
 .quad .Lx47_0_s
.Lx47_0_s:
 .string "PASS 1016_eval (3/3)"
# IR_ASSIGN gva
 xchain14_n25_α:
 mov rax, qword ptr [r12 + 496]
 mov rdx, qword ptr [r12 + 504]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 480], rax
 mov qword ptr [r12 + 488], rdx
 jmp main_γ
 xchain14_n25_β:
 jmp main_γ
# IR_LIT_STRING
 xchain14_n26_α:
 mov qword ptr [r12 + 640], 1
 mov rax, qword ptr [rip + .Lx49_0]
 mov qword ptr [r12 + 648], rax
 jmp xchain14_n28_α
 xchain14_n26_β:
 jmp main_γ
.Lx49_0:
 .quad .Lx49_0_s
.Lx49_0_s:
 .string "FAIL 1016/003: eval failing expr should fail"
# IR_ASSIGN gva
 xchain14_n27_α:
 mov rax, qword ptr [r12 + 672]
 mov rdx, qword ptr [r12 + 680]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 656], rax
 mov qword ptr [r12 + 664], rdx
 jmp main_γ
 xchain14_n27_β:
 jmp main_γ
# IR_ASSIGN gva
 xchain14_n28_α:
 mov rax, qword ptr [r12 + 640]
 mov rdx, qword ptr [r12 + 648]
 mov qword ptr [rbx + 16], rax
 mov qword ptr [rbx + 24], rdx
 mov qword ptr [r12 + 624], rax
 mov qword ptr [r12 + 632], rdx
 jmp main_γ
 xchain14_n28_β:
 jmp main_γ
main_β:
jmp main_ω
main_γ:
mov eax, 1
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
 push rbp
 mov rbp, rsp
 and rsp, -16
 mov rdi, qword ptr [r12 + 688]
 call rt_zls_release_to@PLT
 mov rsp, rbp
 pop rbp
main_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
mov rsp, rbp
pop rbp
pop r12
ret
