  .intel_syntax noprefix
  .text
  .section .rodata
  .Lgvan0: .string "myvar"
  .Lgvan1: .string "subject"
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
  call core_lib_init@PLT
  call rt_proc_reset@PLT
  lea rdi, [rip + __gva_names]
  lea rsi, [rip + __gva]
  mov edx, 2
  call gva_register@PLT
  mov rbx, rax
  call rt_frame@PLT
  mov rdi, rax
  xor esi, esi
  call flat_α
  xor eax, eax
  pop rbp
  ret
flat_α:
#=======================================================================================================================
    .global flat_α
    .global flat_β
    .global flat_γ
    .global flat_ω
push r12
  mov r12, rdi
  lea r10, [rip + Δ]
flat_α_body:
snoch0_n0_α:
# IR_LIT_scalar
bb1_α:
 jmp snoch0_n1_α
 snoch0_n0_β:
 jmp snoch0_n2_α
snoch0_n1_α:
bb2_α:
# IR_ASSIGN
 lea rdi, [rip + .S0]
 movabs rsi, 1
 call rt_gvar_assign_int@PLT
 jmp snoch0_n2_α
 snoch0_n1_β:
 jmp snoch0_n2_α
snoch0_n2_α:
bb3_α:
# BOX IR_CALL stack_init(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
  .section .rodata
  .Lbynamefn4: .string "stack_init"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn4]
 lea rsi, [r12 + 0]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 0], rax
 mov qword ptr [r12 + 8], rdx
 cmp eax, 99
 je snoch0_n3_α
 jmp snoch0_n3_α
snoch0_n2_β:
 jmp snoch0_n3_α
snoch0_n3_α:
# IR_LIT_S
bb4_α:
 mov qword ptr [r12 + 16], 1
 mov rax, qword ptr [rip + .Lx6_0]
 mov qword ptr [r12 + 24], rax
 jmp xgvarg5_done
 xgvarg5_β:
 jmp snoch0_n4_α
.Lx6_0:
 .quad .Lx6_0_s
.Lx6_0_s:
 .string "a"
xgvarg5_done:
bb5_α:
# BOX IR_CALL stack_push(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+16] -> [r12+48]
 mov rax, qword ptr [r12 + 16]
 mov qword ptr [r12 + 48], rax
 mov rax, qword ptr [r12 + 24]
 mov qword ptr [r12 + 56], rax
  .section .rodata
  .Lbynamefn8: .string "stack_push"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn8]
 lea rsi, [r12 + 48]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 cmp eax, 99
 je snoch0_n4_α
 jmp snoch0_n4_α
snoch0_n3_β:
 jmp snoch0_n4_α
snoch0_n4_α:
# IR_LIT_S
bb6_α:
 mov qword ptr [r12 + 64], 1
 mov rax, qword ptr [rip + .Lx10_0]
 mov qword ptr [r12 + 72], rax
 jmp xgvarg9_done
 xgvarg9_β:
 jmp snoch0_n5_α
.Lx10_0:
 .quad .Lx10_0_s
.Lx10_0_s:
 .string "b"
xgvarg9_done:
bb7_α:
# BOX IR_CALL stack_push(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+64] -> [r12+96]
 mov rax, qword ptr [r12 + 64]
 mov qword ptr [r12 + 96], rax
 mov rax, qword ptr [r12 + 72]
 mov qword ptr [r12 + 104], rax
  .section .rodata
  .Lbynamefn12: .string "stack_push"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn12]
 lea rsi, [r12 + 96]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 80], rax
 mov qword ptr [r12 + 88], rdx
 cmp eax, 99
 je snoch0_n5_α
 jmp snoch0_n5_α
snoch0_n4_β:
 jmp snoch0_n5_α
snoch0_n5_α:
# IR_LIT_S
bb8_α:
 mov qword ptr [r12 + 112], 1
 mov rax, qword ptr [rip + .Lx14_0]
 mov qword ptr [r12 + 120], rax
 jmp xgvarg13_done
 xgvarg13_β:
 jmp snoch0_n6_α
.Lx14_0:
 .quad .Lx14_0_s
.Lx14_0_s:
 .string "c"
xgvarg13_done:
bb9_α:
# BOX IR_CALL stack_push(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+112] -> [r12+144]
 mov rax, qword ptr [r12 + 112]
 mov qword ptr [r12 + 144], rax
 mov rax, qword ptr [r12 + 120]
 mov qword ptr [r12 + 152], rax
  .section .rodata
  .Lbynamefn16: .string "stack_push"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn16]
 lea rsi, [r12 + 144]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 128], rax
 mov qword ptr [r12 + 136], rdx
 cmp eax, 99
 je snoch0_n6_α
 jmp snoch0_n6_α
snoch0_n5_β:
 jmp snoch0_n6_α
snoch0_n6_α:
bb10_α:
# BOX IR_CALL stack_depth(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
  .section .rodata
  .Lbynamefn18: .string "stack_depth"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn18]
 lea rsi, [r12 + 160]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 160], rax
 mov qword ptr [r12 + 168], rdx
 cmp eax, 99
 je snoch0_n8_α
 jmp snoch0_n7_α
snoch0_n6_β:
 jmp snoch0_n8_α
snoch0_n7_α:
bb11_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S1]
 mov rsi, qword ptr [r12 + 160]
 mov rdx, qword ptr [r12 + 168]
 call rt_gvar_assign_descr@PLT
 jmp snoch0_n8_α
 snoch0_n7_β:
 jmp snoch0_n8_α
snoch0_n8_α:
bb12_α:
# BOX IR_CALL stack_pop(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
  .section .rodata
  .Lbynamefn21: .string "stack_pop"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn21]
 lea rsi, [r12 + 176]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 176], rax
 mov qword ptr [r12 + 184], rdx
 cmp eax, 99
 je snoch0_n10_α
 jmp snoch0_n9_α
snoch0_n8_β:
 jmp snoch0_n10_α
snoch0_n9_α:
bb13_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S1]
 mov rsi, qword ptr [r12 + 176]
 mov rdx, qword ptr [r12 + 184]
 call rt_gvar_assign_descr@PLT
 jmp snoch0_n10_α
 snoch0_n9_β:
 jmp snoch0_n10_α
snoch0_n10_α:
bb14_α:
# BOX IR_CALL stack_pop(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
  .section .rodata
  .Lbynamefn24: .string "stack_pop"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn24]
 lea rsi, [r12 + 192]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 192], rax
 mov qword ptr [r12 + 200], rdx
 cmp eax, 99
 je snoch0_n12_α
 jmp snoch0_n11_α
snoch0_n10_β:
 jmp snoch0_n12_α
snoch0_n11_α:
bb15_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S1]
 mov rsi, qword ptr [r12 + 192]
 mov rdx, qword ptr [r12 + 200]
 call rt_gvar_assign_descr@PLT
 jmp snoch0_n12_α
 snoch0_n11_β:
 jmp snoch0_n12_α
snoch0_n12_α:
bb16_α:
# BOX IR_CALL stack_depth(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
  .section .rodata
  .Lbynamefn27: .string "stack_depth"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn27]
 lea rsi, [r12 + 208]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 208], rax
 mov qword ptr [r12 + 216], rdx
 cmp eax, 99
 je snoch0_n14_α
 jmp snoch0_n13_α
snoch0_n12_β:
 jmp snoch0_n14_α
snoch0_n13_α:
bb17_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S1]
 mov rsi, qword ptr [r12 + 208]
 mov rdx, qword ptr [r12 + 216]
 call rt_gvar_assign_descr@PLT
 jmp snoch0_n14_α
 snoch0_n13_β:
 jmp snoch0_n14_α
snoch0_n14_α:
bb18_α:
# BOX IR_CALL stack_pop(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
  .section .rodata
  .Lbynamefn30: .string "stack_pop"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn30]
 lea rsi, [r12 + 224]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 224], rax
 mov qword ptr [r12 + 232], rdx
 cmp eax, 99
 je snoch0_n16_α
 jmp snoch0_n15_α
snoch0_n14_β:
 jmp snoch0_n16_α
snoch0_n15_α:
bb19_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S1]
 mov rsi, qword ptr [r12 + 224]
 mov rdx, qword ptr [r12 + 232]
 call rt_gvar_assign_descr@PLT
 jmp snoch0_n16_α
 snoch0_n15_β:
 jmp snoch0_n16_α
snoch0_n16_α:
bb20_α:
# BOX IR_CALL stack_depth(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
  .section .rodata
  .Lbynamefn33: .string "stack_depth"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn33]
 lea rsi, [r12 + 240]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 240], rax
 mov qword ptr [r12 + 248], rdx
 cmp eax, 99
 je snoch0_n18_α
 jmp snoch0_n17_α
snoch0_n16_β:
 jmp snoch0_n18_α
snoch0_n17_α:
bb21_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S1]
 mov rsi, qword ptr [r12 + 240]
 mov rdx, qword ptr [r12 + 248]
 call rt_gvar_assign_descr@PLT
 jmp snoch0_n18_α
 snoch0_n17_β:
 jmp snoch0_n18_α
snoch0_n18_α:
bb22_α:
# BOX IR_CALL stack_pop(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
  .section .rodata
  .Lbynamefn36: .string "stack_pop"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn36]
 lea rsi, [r12 + 256]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 256], rax
 mov qword ptr [r12 + 264], rdx
 cmp eax, 99
 je snoch0_n20_α
 jmp snoch0_n19_α
snoch0_n18_β:
 jmp snoch0_n20_α
snoch0_n19_α:
# IR_LIT_scalar
bb23_α:
 jmp snoch0_n21_α
 snoch0_n19_β:
 jmp snoch0_n22_α
snoch0_n20_α:
# IR_LIT_scalar
bb24_α:
 jmp snoch0_n23_α
 snoch0_n20_β:
 jmp snoch0_n22_α
snoch0_n21_α:
bb25_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S2]
 call rt_gvar_assign_str@PLT
 jmp snoch0_n22_α
 snoch0_n21_β:
 jmp snoch0_n22_α
snoch0_n22_α:
bb26_α:
# BOX IR_CALL stack_init(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
  .section .rodata
  .Lbynamefn41: .string "stack_init"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn41]
 lea rsi, [r12 + 272]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 272], rax
 mov qword ptr [r12 + 280], rdx
 cmp eax, 99
 je snoch0_n24_α
 jmp snoch0_n24_α
snoch0_n22_β:
 jmp snoch0_n24_α
snoch0_n23_α:
bb27_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S3]
 call rt_gvar_assign_str@PLT
 jmp snoch0_n22_α
 snoch0_n23_β:
 jmp snoch0_n22_α
snoch0_n24_α:
# IR_LIT_S
bb28_α:
 mov qword ptr [r12 + 288], 1
 mov rax, qword ptr [rip + .Lx44_0]
 mov qword ptr [r12 + 296], rax
 jmp xgvarg43_done
 xgvarg43_β:
 jmp snoch0_n25_α
.Lx44_0:
 .quad .Lx44_0_s
.Lx44_0_s:
 .string "x"
xgvarg43_done:
bb29_α:
# BOX IR_CALL stack_push(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+288] -> [r12+320]
 mov rax, qword ptr [r12 + 288]
 mov qword ptr [r12 + 320], rax
 mov rax, qword ptr [r12 + 296]
 mov qword ptr [r12 + 328], rax
  .section .rodata
  .Lbynamefn46: .string "stack_push"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn46]
 lea rsi, [r12 + 320]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 304], rax
 mov qword ptr [r12 + 312], rdx
 cmp eax, 99
 je snoch0_n25_α
 jmp snoch0_n25_α
snoch0_n24_β:
 jmp snoch0_n25_α
snoch0_n25_α:
bb30_α:
# BOX IR_CALL stack_peek(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
  .section .rodata
  .Lbynamefn48: .string "stack_peek"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn48]
 lea rsi, [r12 + 336]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 336], rax
 mov qword ptr [r12 + 344], rdx
 cmp eax, 99
 je snoch0_n27_α
 jmp snoch0_n26_α
snoch0_n25_β:
 jmp snoch0_n27_α
snoch0_n26_α:
bb31_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S1]
 mov rsi, qword ptr [r12 + 336]
 mov rdx, qword ptr [r12 + 344]
 call rt_gvar_assign_descr@PLT
 jmp snoch0_n27_α
 snoch0_n26_β:
 jmp snoch0_n27_α
snoch0_n27_α:
bb32_α:
# BOX IR_CALL stack_depth(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
  .section .rodata
  .Lbynamefn51: .string "stack_depth"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn51]
 lea rsi, [r12 + 352]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 352], rax
 mov qword ptr [r12 + 360], rdx
 cmp eax, 99
 je snoch0_n29_α
 jmp snoch0_n28_α
snoch0_n27_β:
 jmp snoch0_n29_α
snoch0_n28_α:
bb33_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S1]
 mov rsi, qword ptr [r12 + 352]
 mov rdx, qword ptr [r12 + 360]
 call rt_gvar_assign_descr@PLT
 jmp snoch0_n29_α
 snoch0_n28_β:
 jmp snoch0_n29_α
snoch0_n29_α:
bb34_α:
# BOX IR_CALL stack_pop(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
  .section .rodata
  .Lbynamefn54: .string "stack_pop"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn54]
 lea rsi, [r12 + 368]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 368], rax
 mov qword ptr [r12 + 376], rdx
 cmp eax, 99
 je snoch0_n31_α
 jmp snoch0_n30_α
snoch0_n29_β:
 jmp snoch0_n31_α
snoch0_n30_α:
bb35_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S1]
 mov rsi, qword ptr [r12 + 368]
 mov rdx, qword ptr [r12 + 376]
 call rt_gvar_assign_descr@PLT
 jmp snoch0_n31_α
 snoch0_n30_β:
 jmp snoch0_n31_α
snoch0_n31_α:
bb36_α:
# BOX IR_CALL stack_init(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
  .section .rodata
  .Lbynamefn57: .string "stack_init"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn57]
 lea rsi, [r12 + 384]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 384], rax
 mov qword ptr [r12 + 392], rdx
 cmp eax, 99
 je snoch0_n32_α
 jmp snoch0_n32_α
snoch0_n31_β:
 jmp snoch0_n32_α
snoch0_n32_α:
# IR_LIT_I
bb37_α:
 mov qword ptr [r12 + 400], 6
 mov rax, qword ptr [rip + .Lx59_0]
 mov qword ptr [r12 + 408], rax
 jmp xgvarg58_done
 xgvarg58_β:
 jmp snoch0_n33_α
.Lx59_0:
 .quad 42
xgvarg58_done:
bb38_α:
# BOX IR_CALL stack_push(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+400] -> [r12+432]
 mov rax, qword ptr [r12 + 400]
 mov qword ptr [r12 + 432], rax
 mov rax, qword ptr [r12 + 408]
 mov qword ptr [r12 + 440], rax
  .section .rodata
  .Lbynamefn61: .string "stack_push"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn61]
 lea rsi, [r12 + 432]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 416], rax
 mov qword ptr [r12 + 424], rdx
 cmp eax, 99
 je snoch0_n33_α
 jmp snoch0_n33_α
snoch0_n32_β:
 jmp snoch0_n33_α
snoch0_n33_α:
# IR_LIT_I
bb39_α:
 mov qword ptr [r12 + 448], 6
 mov rax, qword ptr [rip + .Lx63_0]
 mov qword ptr [r12 + 456], rax
 jmp xgvarg62_done
 xgvarg62_β:
 jmp snoch0_n34_α
.Lx63_0:
 .quad 99
xgvarg62_done:
bb40_α:
# BOX IR_CALL stack_push(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+448] -> [r12+480]
 mov rax, qword ptr [r12 + 448]
 mov qword ptr [r12 + 480], rax
 mov rax, qword ptr [r12 + 456]
 mov qword ptr [r12 + 488], rax
  .section .rodata
  .Lbynamefn65: .string "stack_push"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn65]
 lea rsi, [r12 + 480]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 464], rax
 mov qword ptr [r12 + 472], rdx
 cmp eax, 99
 je snoch0_n34_α
 jmp snoch0_n34_α
snoch0_n33_β:
 jmp snoch0_n34_α
snoch0_n34_α:
# IR_LIT_S
bb41_α:
 mov qword ptr [r12 + 496], 1
 mov rax, qword ptr [rip + .Lx67_0]
 mov qword ptr [r12 + 504], rax
 jmp xgvarg66_done
 xgvarg66_β:
 jmp snoch0_n35_α
.Lx67_0:
 .quad .Lx67_0_s
.Lx67_0_s:
 .string "myvar"
xgvarg66_done:
bb42_α:
# BOX IR_CALL stack_pop(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+496] -> [r12+528]
 mov rax, qword ptr [r12 + 496]
 mov qword ptr [r12 + 528], rax
 mov rax, qword ptr [r12 + 504]
 mov qword ptr [r12 + 536], rax
  .section .rodata
  .Lbynamefn69: .string "stack_pop"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn69]
 lea rsi, [r12 + 528]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 512], rax
 mov qword ptr [r12 + 520], rdx
 cmp eax, 99
 je snoch0_n35_α
 jmp snoch0_n35_α
snoch0_n34_β:
 jmp snoch0_n35_α
snoch0_n35_α:
# IR_VAR gva
bb43_α:
 mov rax, qword ptr [rbx + 0]
 mov rdx, qword ptr [rbx + 8]
 mov qword ptr [r12 + 544], rax
 mov qword ptr [r12 + 552], rdx
 jmp snoch0_n36_α
 snoch0_n35_β:
 jmp snoch0_n37_α
snoch0_n36_α:
bb44_α:
# IR_ASSIGN_VAR
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S4]
 call rt_gvar_assign_var@PLT
 jmp snoch0_n37_α
 snoch0_n36_β:
 jmp snoch0_n37_α
snoch0_n37_α:
bb45_α:
# BOX IR_CALL stack_init(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
  .section .rodata
  .Lbynamefn74: .string "stack_init"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn74]
 lea rsi, [r12 + 560]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 560], rax
 mov qword ptr [r12 + 568], rdx
 cmp eax, 99
 je snoch0_n38_α
 jmp snoch0_n38_α
snoch0_n37_β:
 jmp snoch0_n38_α
snoch0_n38_α:
# IR_LIT_scalar
bb46_α:
 jmp snoch0_n39_α
 snoch0_n38_β:
 jmp snoch0_n40_α
snoch0_n39_α:
bb47_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S5]
 lea rsi, [rip + .S6]
 call rt_gvar_assign_str@PLT
 jmp snoch0_n40_α
 snoch0_n39_β:
 jmp snoch0_n40_α
snoch0_n40_α:
# IR_LIT_scalar
bb48_α:
 jmp snoch0_n41_α
 snoch0_n40_β:
 jmp snoch0_n42_α
snoch0_n41_α:
# IR_SUBJECT
bb49_α:
 lea rdi, [rip + .S5]
 lea rsi, [r12 + 576]
 push r10
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_subject_load_nv@PLT
 mov rsp, rbx
 pop rbx
 pop r10
 jmp xscan78_sγ
 xscan78_sβ:
 jmp snoch0_n42_α
xscan78_sγ:
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_dcap_begin@PLT
 mov rsp, rbx
 pop rbx
# IR_MATCH_HEAD
bb50_α:
 mov r13, qword ptr [r12 + 576]
 mov r15d, dword ptr [r12 + 584]
 mov dword ptr [r12 + 592], 0
 lea r10, [r12 + 600]
 jmp smatch81_retry
 snoch0_n41_β:
 jmp xscan78_dfail
smatch81_retry:
# IR_MATCH_RETRY
 mov r14d, dword ptr [r12 + 592]
# IR_MATCH_CAPTURE_SAVE
bb52_α:
 mov dword ptr [r12 + 608], r14d
# IR_MATCH_BREAK
bb53_α:
 mov dword ptr [r12 + 624], 0
.Lx88_0:
 mov eax, r14d
 add eax, dword ptr [r12 + 624]
 cmp eax, r15d
 jge xcat84_ω
 movsxd rcx, eax
 movzx esi, byte ptr [r13+rcx]
 lea rdi, [rip + .S8]
 sub rsp, 8
 call strchr@PLT
 add rsp, 8
 test rax, rax
 jnz .Lx88_1
 add dword ptr [r12 + 624], 1
 jmp .Lx88_0
.Lx88_1:
 mov eax, r14d
 add eax, dword ptr [r12 + 624]
 mov r14d, eax
 jmp xcap85_γ
 xcat84_left_β:
 mov eax, r14d
 sub eax, dword ptr [r12 + 624]
 mov r14d, eax
 jmp xcat84_ω
xcap85_γ:
# IR_MATCH_CAPTURE_COND
 lea rdi, [rip + .S7]
 mov esi, dword ptr [r12 + 608]
 mov edx, r14d
 mov ecx, 0
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_cap_assign_cursor@PLT
 mov rsp, rbx
 pop rbx
 jmp xcat84_γ
xcat84_γ:
# IR_LIT
bb55_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg xcat84_left_β
 movsxd rcx, r14d
 lea rdi, [r13 + rcx]
 lea rsi, [rip + .S8]
 mov rdx, 1
 sub rsp, 8
 call memcmp@PLT
 add rsp, 8
 test eax, eax
 jne xcat84_left_β
 add r14d, 1
 jmp xcat84_mid1_γ
 xcat84_mid1_β:
 sub r14d, 1
 jmp xcat84_left_β
xcat84_mid1_γ:
# IR_MATCH_CAPTURE_SAVE
bb56_α:
 mov dword ptr [r12 + 628], r14d
# IR_MATCH_REM
bb57_α:
 mov r14d, r15d
 jmp xcap91_γ
 xcat84_mid2_β:
 jmp xcat84_mid1_β
xcap91_γ:
# IR_MATCH_CAPTURE_COND
 lea rdi, [rip + .S9]
 mov esi, dword ptr [r12 + 628]
 mov edx, r14d
 mov ecx, 0
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_cap_assign_cursor@PLT
 mov rsp, rbx
 pop rbx
 jmp xscan78_dok
xcat84_right_ω:
 jmp xcat84_left_β
smatch81_elemb:
 jmp xcat84_mid2_β
xcat84_ω:
 jmp smatch81_adv
smatch81_adv:
# IR_MATCH_ADVANCE
 add dword ptr [r12 + 592], 1
 mov eax, dword ptr [r12 + 592]
 cmp eax, r15d
 jg xscan78_dfail
 lea rcx, [rip + kw_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne xscan78_dfail
 jmp smatch81_retry
xscan78_dok:
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_dcap_end_ok@PLT
 mov rsp, rbx
 pop rbx
# SPLICE_EMPTY
 lea rdi, [rip + .S5]
 mov esi, dword ptr [r12 + 592]
 mov edx, r14d
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_scan_splice_empty@PLT
 mov rsp, rbx
 pop rbx
jmp snoch0_n43_α
xscan78_dfail:
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_dcap_end_fail@PLT
 mov rsp, rbx
 pop rbx
jmp snoch0_n42_α
snoch0_n42_α:
# IR_LIT_scalar
bb61_α:
 jmp snoch0_n44_α
 snoch0_n42_β:
 jmp flat_γ
snoch0_n43_α:
# IR_VAR
bb62_α:
 mov rdi, qword ptr [rip + .Lx100_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 644], rax
 mov qword ptr [r12 + 652], rdx
 jmp xgvarg98_done
 xgvarg98_β:
 jmp snoch0_n45_α
.Lx100_0:
 .quad .Lx100_0_s
.Lx100_0_s:
 .string "w1"
xgvarg98_done:
bb63_α:
# BOX IR_CALL stack_push(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+644] -> [r12+676]
 mov rax, qword ptr [r12 + 644]
 mov qword ptr [r12 + 676], rax
 mov rax, qword ptr [r12 + 652]
 mov qword ptr [r12 + 684], rax
  .section .rodata
  .Lbynamefn102: .string "stack_push"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn102]
 lea rsi, [r12 + 676]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 660], rax
 mov qword ptr [r12 + 668], rdx
 cmp eax, 99
 je snoch0_n45_α
 jmp snoch0_n45_α
snoch0_n43_β:
 jmp snoch0_n45_α
snoch0_n44_α:
bb64_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S10]
 call rt_gvar_assign_str@PLT
 jmp flat_γ
 snoch0_n44_β:
 jmp flat_γ
snoch0_n45_α:
# IR_VAR
bb65_α:
 mov rdi, qword ptr [rip + .Lx106_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 692], rax
 mov qword ptr [r12 + 700], rdx
 jmp xgvarg104_done
 xgvarg104_β:
 jmp snoch0_n46_α
.Lx106_0:
 .quad .Lx106_0_s
.Lx106_0_s:
 .string "w2"
xgvarg104_done:
bb66_α:
# BOX IR_CALL stack_push(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+692] -> [r12+724]
 mov rax, qword ptr [r12 + 692]
 mov qword ptr [r12 + 724], rax
 mov rax, qword ptr [r12 + 700]
 mov qword ptr [r12 + 732], rax
  .section .rodata
  .Lbynamefn108: .string "stack_push"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn108]
 lea rsi, [r12 + 724]
 mov edx, 1
 call rt_call_arr@PLT
 mov qword ptr [r12 + 708], rax
 mov qword ptr [r12 + 716], rdx
 cmp eax, 99
 je snoch0_n46_α
 jmp snoch0_n46_α
snoch0_n45_β:
 jmp snoch0_n46_α
snoch0_n46_α:
bb67_α:
# BOX IR_CALL stack_pop(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
  .section .rodata
  .Lbynamefn110: .string "stack_pop"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn110]
 lea rsi, [r12 + 740]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 740], rax
 mov qword ptr [r12 + 748], rdx
 cmp eax, 99
 je snoch0_n48_α
 jmp snoch0_n47_α
snoch0_n46_β:
 jmp snoch0_n48_α
snoch0_n47_α:
bb68_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S1]
 mov rsi, qword ptr [r12 + 740]
 mov rdx, qword ptr [r12 + 748]
 call rt_gvar_assign_descr@PLT
 jmp snoch0_n48_α
 snoch0_n47_β:
 jmp snoch0_n48_α
snoch0_n48_α:
bb69_α:
# BOX IR_CALL stack_pop(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
  .section .rodata
  .Lbynamefn113: .string "stack_pop"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn113]
 lea rsi, [r12 + 756]
 mov edx, 0
 call rt_call_arr@PLT
 mov qword ptr [r12 + 756], rax
 mov qword ptr [r12 + 764], rdx
 cmp eax, 99
 je flat_γ
 jmp snoch0_n49_α
snoch0_n48_β:
 jmp flat_γ
snoch0_n49_α:
bb70_α:
# IR_ASSIGN_CALL
 lea rdi, [rip + .S1]
 mov rsi, qword ptr [r12 + 756]
 mov rdx, qword ptr [r12 + 764]
 call rt_gvar_assign_descr@PLT
 jmp flat_γ
 snoch0_n49_β:
 jmp flat_γ
flat_β:
jmp flat_ω
flat_γ:
mov eax, 1
xor edx, edx
pop r12
ret
flat_ω:
# GZ-10 PROC FAIL EXIT: write FAILDESCR to frame[0] so rt_call_proc_descr sees failure
mov dword ptr [r12+0], 99
mov dword ptr [r12+4], 0
mov qword ptr [r12+8], 0
mov eax, 99
xor edx, edx
pop r12
ret
.section .rodata
.S0: .string "TRIM"
.S1: .string "OUTPUT"
.S2: .string "FAIL: empty pop should FRETURN"
.S3: .string "empty ok"
.S4: .string "myvar"
.S5: .string "subject"
.S6: .string "hello world"
.S7: .string "w1"
.S8: .string " "
.S9: .string "w2"
.S10: .string "FAIL: pattern match failed"
.text
