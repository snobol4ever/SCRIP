  .intel_syntax noprefix
  .text
  .globl main
main:
  push rbp
  mov rbp, rsp
  call core_lib_init@PLT
  call rt_proc_reset@PLT
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
# IR_VAR
bb3_α:
 mov rdi, qword ptr [rip + .Lx4_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 0], rax
 mov qword ptr [r12 + 8], rdx
 jmp snoch0_n3_α
 snoch0_n2_β:
 jmp flat_γ
.Lx4_0:
 .quad .Lx4_0_s
.Lx4_0_s:
 .string "INPUT"
snoch0_n3_α:
bb4_α:
# IR_ASSIGN_VAR
 lea rdi, [rip + .S1]
 lea rsi, [rip + .S2]
 call rt_gvar_assign_var@PLT
 jmp snoch0_n4_α
 snoch0_n3_β:
 jmp flat_γ
snoch0_n4_α:
# IR_VAR
bb5_α:
 mov rdi, qword ptr [rip + .Lx7_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 16], rax
 mov qword ptr [r12 + 24], rdx
 jmp snoch0_n5_α
 snoch0_n4_β:
 jmp flat_γ
.Lx7_0:
 .quad .Lx7_0_s
.Lx7_0_s:
 .string "INPUT"
snoch0_n5_α:
bb6_α:
# IR_ASSIGN_VAR
 lea rdi, [rip + .S3]
 lea rsi, [rip + .S2]
 call rt_gvar_assign_var@PLT
 jmp snoch0_n6_α
 snoch0_n5_β:
 jmp flat_γ
snoch0_n6_α:
# IR_VAR
bb7_α:
 mov rdi, qword ptr [rip + .Lx10_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 32], rax
 mov qword ptr [r12 + 40], rdx
 jmp snoch0_n7_α
 snoch0_n6_β:
 jmp snoch0_n8_α
.Lx10_0:
 .quad .Lx10_0_s
.Lx10_0_s:
 .string "H"
snoch0_n7_α:
bb8_α:
# IR_ASSIGN_VAR
 lea rdi, [rip + .S4]
 lea rsi, [rip + .S1]
 call rt_gvar_assign_var@PLT
 jmp snoch0_n8_α
 snoch0_n7_β:
 jmp snoch0_n8_α
snoch0_n8_α:
# IR_LIT_scalar
bb9_α:
 jmp snoch0_n9_α
 snoch0_n8_β:
 jmp snoch0_n2_α
snoch0_n9_α:
 lea rdi, [rip + .S5]
 call rt_bomb@PLT
 ud2
snoch0_n10_α:
# IR_VAR
bb11_α:
 mov rdi, qword ptr [rip + .Lx15_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 48], rax
 mov qword ptr [r12 + 56], rdx
 jmp snoch0_n11_α
 snoch0_n10_β:
 jmp snoch0_n12_α
.Lx15_0:
 .quad .Lx15_0_s
.Lx15_0_s:
 .string "V"
snoch0_n11_α:
bb12_α:
# IR_ASSIGN_VAR
 lea rdi, [rip + .S6]
 lea rsi, [rip + .S3]
 call rt_gvar_assign_var@PLT
 jmp snoch0_n12_α
 snoch0_n11_β:
 jmp snoch0_n12_α
snoch0_n12_α:
# IR_LIT_scalar
bb13_α:
 jmp snoch0_n13_α
 snoch0_n12_β:
 jmp snoch0_n8_α
snoch0_n13_α:
 lea rdi, [rip + .S5]
 call rt_bomb@PLT
 ud2
snoch0_n14_α:
# IR_LIT_scalar
bb15_α:
 jmp snoch0_n15_α
 snoch0_n14_β:
 jmp snoch0_n16_α
snoch0_n15_α:
bb16_α:
# IR_ASSIGN_LIT_S
 lea rdi, [rip + .S7]
 lea rsi, [rip + .S8]
 call rt_gvar_assign_str@PLT
 jmp snoch0_n16_α
 snoch0_n15_β:
 jmp snoch0_n16_α
snoch0_n16_α:
# IR_VAR
bb17_α:
 mov rdi, qword ptr [rip + .Lx22_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 64], rax
 mov qword ptr [r12 + 72], rdx
 jmp snoch0_n17_α
 snoch0_n16_β:
 jmp snoch0_n18_α
.Lx22_0:
 .quad .Lx22_0_s
.Lx22_0_s:
 .string "V"
snoch0_n17_α:
bb18_α:
# IR_ASSIGN_VAR
 lea rdi, [rip + .S9]
 lea rsi, [rip + .S3]
 call rt_gvar_assign_var@PLT
 jmp snoch0_n18_α
 snoch0_n17_β:
 jmp snoch0_n18_α
snoch0_n18_α:
# IR_LIT_scalar
bb19_α:
 jmp snoch0_n19_α
 snoch0_n18_β:
 jmp snoch0_n20_α
snoch0_n19_α:
 lea rdi, [rip + .S5]
 call rt_bomb@PLT
 ud2
snoch0_n20_α:
# IR_LIT_scalar
bb21_α:
 jmp snoch0_n21_α
 snoch0_n20_β:
 jmp snoch0_n12_α
snoch0_n21_α:
# IR_SUBJECT
bb22_α:
 lea rdi, [rip + .S9]
 lea rsi, [r12 + 80]
 push r10
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_subject_load_nv@PLT
 mov rsp, rbx
 pop rbx
 pop r10
 jmp xscan27_sγ
 xscan27_sβ:
 jmp snoch0_n12_α
xscan27_sγ:
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_dcap_begin@PLT
 mov rsp, rbx
 pop rbx
# IR_MATCH_HEAD
bb23_α:
 mov r13, qword ptr [r12 + 80]
 mov r15d, dword ptr [r12 + 88]
 mov dword ptr [r12 + 96], 0
 lea r10, [r12 + 104]
 jmp smatch30_retry
 snoch0_n21_β:
 jmp xscan27_dfail
smatch30_retry:
# IR_MATCH_RETRY
 mov r14d, dword ptr [r12 + 96]
# IR_MATCH_CAPTURE_SAVE
bb25_α:
 mov dword ptr [r12 + 112], r14d
# IR_MATCH_LEN
bb26_α:
 mov eax, r14d
 add eax, 1
 cmp eax, r15d
 jg smatch30_adv
 add r14d, 1
 jmp xcap33_γ
 smatch30_elemb:
 jmp smatch30_adv
xcap33_γ:
# IR_MATCH_CAPTURE_COND
 lea rdi, [rip + .S10]
 mov esi, dword ptr [r12 + 112]
 mov edx, r14d
 mov ecx, 0
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_cap_assign_cursor@PLT
 mov rsp, rbx
 pop rbx
 jmp xscan27_dok
smatch30_adv:
# IR_MATCH_ADVANCE
 add dword ptr [r12 + 96], 1
 mov eax, dword ptr [r12 + 96]
 cmp eax, r15d
 jg xscan27_dfail
 lea rcx, [rip + kw_anchor]
 mov rax, qword ptr [rcx]
 cmp rax, 0
 jne xscan27_dfail
 jmp smatch30_retry
xscan27_dok:
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_dcap_end_ok@PLT
 mov rsp, rbx
 pop rbx
# SPLICE_EMPTY
 lea rdi, [rip + .S9]
 mov esi, dword ptr [r12 + 96]
 mov edx, r14d
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_scan_splice_empty@PLT
 mov rsp, rbx
 pop rbx
jmp snoch0_n22_α
xscan27_dfail:
 push rbx
 mov rbx, rsp
 and rsp, -16
 call rt_dcap_end_fail@PLT
 mov rsp, rbx
 pop rbx
jmp snoch0_n12_α
snoch0_n22_α:
# IR_VAR
bb29_α:
 mov rdi, qword ptr [rip + .Lx40_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 128], rax
 mov qword ptr [r12 + 136], rdx
 jmp xgvarg38_done
 xgvarg38_β:
 jmp snoch0_n24_α
.Lx40_0:
 .quad .Lx40_0_s
.Lx40_0_s:
 .string "C"
xgvarg38_done:
# IR_LIT_S
bb30_α:
 mov qword ptr [r12 + 144], 1
 mov rax, qword ptr [rip + .Lx42_0]
 mov qword ptr [r12 + 152], rax
 jmp xgvarg41_done
 xgvarg41_β:
 jmp snoch0_n24_α
.Lx42_0:
 .quad .Lx42_0_s
.Lx42_0_s:
 .string "#"
xgvarg41_done:
bb31_α:
# BOX IR_CALL DIFFER(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+128] -> [r12+176]
 mov rax, qword ptr [r12 + 128]
 mov qword ptr [r12 + 176], rax
 mov rax, qword ptr [r12 + 136]
 mov qword ptr [r12 + 184], rax
# marshal arg1 = producer-box slot [r12+144] -> [r12+192]
 mov rax, qword ptr [r12 + 144]
 mov qword ptr [r12 + 192], rax
 mov rax, qword ptr [r12 + 152]
 mov qword ptr [r12 + 200], rax
  .section .rodata
  .Lbynamefn44: .string "DIFFER"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn44]
 lea rsi, [r12 + 176]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 160], rax
 mov qword ptr [r12 + 168], rdx
 cmp eax, 99
 je snoch0_n24_α
 jmp snoch0_n23_α
snoch0_n22_β:
 jmp snoch0_n24_α
snoch0_n23_α:
# IR_LIT_S
bb32_α:
 mov qword ptr [r12 + 208], 1
 mov rax, qword ptr [rip + .Lx46_0]
 mov qword ptr [r12 + 216], rax
 jmp xgvarg45_done
 xgvarg45_β:
 jmp snoch0_n24_α
.Lx46_0:
 .quad .Lx46_0_s
.Lx46_0_s:
 .string " "
xgvarg45_done:
# IR_VAR
bb33_α:
 mov rdi, qword ptr [rip + .Lx49_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 224], rax
 mov qword ptr [r12 + 232], rdx
 jmp xgvarg47_done
 xgvarg47_β:
 jmp snoch0_n24_α
.Lx49_0:
 .quad .Lx49_0_s
.Lx49_0_s:
 .string "NH"
xgvarg47_done:
bb34_α:
# BOX IR_CALL DUPL(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+208] -> [r12+256]
 mov rax, qword ptr [r12 + 208]
 mov qword ptr [r12 + 256], rax
 mov rax, qword ptr [r12 + 216]
 mov qword ptr [r12 + 264], rax
# marshal arg1 = producer-box slot [r12+224] -> [r12+272]
 mov rax, qword ptr [r12 + 224]
 mov qword ptr [r12 + 272], rax
 mov rax, qword ptr [r12 + 232]
 mov qword ptr [r12 + 280], rax
  .section .rodata
  .Lbynamefn51: .string "DUPL"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn51]
 lea rsi, [r12 + 256]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 240], rax
 mov qword ptr [r12 + 248], rdx
 cmp eax, 99
 je snoch0_n24_α
 jmp snoch0_n25_α
snoch0_n23_β:
 jmp snoch0_n24_α
snoch0_n24_α:
# IR_VAR
bb35_α:
 mov rdi, qword ptr [rip + .Lx53_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 288], rax
 mov qword ptr [r12 + 296], rdx
 jmp snoch0_n26_α
 snoch0_n24_β:
 jmp snoch0_n20_α
.Lx53_0:
 .quad .Lx53_0_s
.Lx53_0_s:
 .string "H"
snoch0_n25_α:
# IR_VAR
bb36_α:
 mov rdi, qword ptr [rip + .Lx57_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 304], rax
 mov qword ptr [r12 + 312], rdx
 jmp xgvarg55_done
 xgvarg55_β:
 jmp snoch0_n24_α
.Lx57_0:
 .quad .Lx57_0_s
.Lx57_0_s:
 .string "C"
xgvarg55_done:
# IR_LIT_S
bb37_α:
 mov qword ptr [r12 + 320], 1
 mov rax, qword ptr [rip + .Lx59_0]
 mov qword ptr [r12 + 328], rax
 jmp xgvarg58_done
 xgvarg58_β:
 jmp snoch0_n24_α
.Lx59_0:
 .quad .Lx59_0_s
.Lx59_0_s:
 .string "#"
xgvarg58_done:
bb38_α:
# BOX IR_CALL DIFFER(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+128] -> [r12+352]
 mov rax, qword ptr [r12 + 128]
 mov qword ptr [r12 + 352], rax
 mov rax, qword ptr [r12 + 136]
 mov qword ptr [r12 + 360], rax
# marshal arg1 = producer-box slot [r12+144] -> [r12+368]
 mov rax, qword ptr [r12 + 144]
 mov qword ptr [r12 + 368], rax
 mov rax, qword ptr [r12 + 152]
 mov qword ptr [r12 + 376], rax
  .section .rodata
  .Lbynamefn61: .string "DIFFER"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn61]
 lea rsi, [r12 + 352]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 336], rax
 mov qword ptr [r12 + 344], rdx
 cmp eax, 99
 je snoch0_n24_α
 jmp xgvcat54_0d
xgvcat54_0b:
 jmp snoch0_n24_α
xgvcat54_0d:
# IR_LIT_S
bb39_α:
 mov qword ptr [r12 + 384], 1
 mov rax, qword ptr [rip + .Lx64_0]
 mov qword ptr [r12 + 392], rax
 jmp xgvarg63_done
 xgvarg63_β:
 jmp snoch0_n24_α
.Lx64_0:
 .quad .Lx64_0_s
.Lx64_0_s:
 .string " "
xgvarg63_done:
# IR_VAR
bb40_α:
 mov rdi, qword ptr [rip + .Lx67_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 400], rax
 mov qword ptr [r12 + 408], rdx
 jmp xgvarg65_done
 xgvarg65_β:
 jmp snoch0_n24_α
.Lx67_0:
 .quad .Lx67_0_s
.Lx67_0_s:
 .string "NH"
xgvarg65_done:
bb41_α:
# BOX IR_CALL DUPL(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+208] -> [r12+432]
 mov rax, qword ptr [r12 + 208]
 mov qword ptr [r12 + 432], rax
 mov rax, qword ptr [r12 + 216]
 mov qword ptr [r12 + 440], rax
# marshal arg1 = producer-box slot [r12+224] -> [r12+448]
 mov rax, qword ptr [r12 + 224]
 mov qword ptr [r12 + 448], rax
 mov rax, qword ptr [r12 + 232]
 mov qword ptr [r12 + 456], rax
  .section .rodata
  .Lbynamefn69: .string "DUPL"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn69]
 lea rsi, [r12 + 432]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 416], rax
 mov qword ptr [r12 + 424], rdx
 cmp eax, 99
 je snoch0_n24_α
 jmp xgvcat62_1d
xgvcat62_1b:
 jmp snoch0_n24_α
xgvcat62_1d:
bb42_α:
# IR_BINOP_GVAR_CONCAT
 mov rdi, qword ptr [r12 + 160]
 mov rsi, qword ptr [r12 + 168]
 mov rdx, qword ptr [r12 + 240]
 mov rcx, qword ptr [r12 + 248]
 call str_concat_d@PLT
 mov qword ptr [r12 + 464], rax
 mov qword ptr [r12 + 472], rdx
 jmp snoch0_n27_α
 snoch0_n25_β:
 jmp snoch0_n24_α
snoch0_n26_α:
bb43_α:
# IR_ASSIGN_VAR
 lea rdi, [rip + .S7]
 lea rsi, [rip + .S1]
 call rt_gvar_assign_var@PLT
 jmp snoch0_n20_α
 snoch0_n26_β:
 jmp snoch0_n20_α
snoch0_n27_α:
# IR_VAR
bb44_α:
 mov rdi, qword ptr [rip + .Lx73_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 480], rax
 mov qword ptr [r12 + 488], rdx
 jmp snoch0_n28_α
 snoch0_n27_β:
 jmp snoch0_n24_α
.Lx73_0:
 .quad .Lx73_0_s
.Lx73_0_s:
 .string "C"
snoch0_n28_α:
# IR_VAR
bb45_α:
 mov rdi, qword ptr [rip + .Lx78_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 496], rax
 mov qword ptr [r12 + 504], rdx
 jmp xgvarg76_done
 xgvarg76_β:
 jmp snoch0_n24_α
.Lx78_0:
 .quad .Lx78_0_s
.Lx78_0_s:
 .string "C"
xgvarg76_done:
# IR_LIT_S
bb46_α:
 mov qword ptr [r12 + 512], 1
 mov rax, qword ptr [rip + .Lx80_0]
 mov qword ptr [r12 + 520], rax
 jmp xgvarg79_done
 xgvarg79_β:
 jmp snoch0_n24_α
.Lx80_0:
 .quad .Lx80_0_s
.Lx80_0_s:
 .string "#"
xgvarg79_done:
bb47_α:
# BOX IR_CALL DIFFER(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+128] -> [r12+544]
 mov rax, qword ptr [r12 + 128]
 mov qword ptr [r12 + 544], rax
 mov rax, qword ptr [r12 + 136]
 mov qword ptr [r12 + 552], rax
# marshal arg1 = producer-box slot [r12+144] -> [r12+560]
 mov rax, qword ptr [r12 + 144]
 mov qword ptr [r12 + 560], rax
 mov rax, qword ptr [r12 + 152]
 mov qword ptr [r12 + 568], rax
  .section .rodata
  .Lbynamefn82: .string "DIFFER"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn82]
 lea rsi, [r12 + 544]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 528], rax
 mov qword ptr [r12 + 536], rdx
 cmp eax, 99
 je snoch0_n24_α
 jmp xgvcat75_0d
xgvcat75_0b:
 jmp snoch0_n24_α
xgvcat75_0d:
# IR_LIT_S
bb48_α:
 mov qword ptr [r12 + 576], 1
 mov rax, qword ptr [rip + .Lx85_0]
 mov qword ptr [r12 + 584], rax
 jmp xgvarg84_done
 xgvarg84_β:
 jmp snoch0_n24_α
.Lx85_0:
 .quad .Lx85_0_s
.Lx85_0_s:
 .string " "
xgvarg84_done:
# IR_VAR
bb49_α:
 mov rdi, qword ptr [rip + .Lx88_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 592], rax
 mov qword ptr [r12 + 600], rdx
 jmp xgvarg86_done
 xgvarg86_β:
 jmp snoch0_n24_α
.Lx88_0:
 .quad .Lx88_0_s
.Lx88_0_s:
 .string "NH"
xgvarg86_done:
bb50_α:
# BOX IR_CALL DUPL(...) -> rt_call_arr by-name [four-port, FAIL->ω.node]
# marshal arg0 = producer-box slot [r12+208] -> [r12+624]
 mov rax, qword ptr [r12 + 208]
 mov qword ptr [r12 + 624], rax
 mov rax, qword ptr [r12 + 216]
 mov qword ptr [r12 + 632], rax
# marshal arg1 = producer-box slot [r12+224] -> [r12+640]
 mov rax, qword ptr [r12 + 224]
 mov qword ptr [r12 + 640], rax
 mov rax, qword ptr [r12 + 232]
 mov qword ptr [r12 + 648], rax
  .section .rodata
  .Lbynamefn90: .string "DUPL"
  .section .text
  .intel_syntax noprefix
   lea rdi, [rip + .Lbynamefn90]
 lea rsi, [r12 + 624]
 mov edx, 2
 call rt_call_arr@PLT
 mov qword ptr [r12 + 608], rax
 mov qword ptr [r12 + 616], rdx
 cmp eax, 99
 je snoch0_n24_α
 jmp xgvcat83_1d
xgvcat83_1b:
 jmp snoch0_n24_α
xgvcat83_1d:
bb51_α:
# IR_BINOP_GVAR_CONCAT
 mov rdi, qword ptr [r12 + 160]
 mov rsi, qword ptr [r12 + 168]
 mov rdx, qword ptr [r12 + 240]
 mov rcx, qword ptr [r12 + 248]
 call str_concat_d@PLT
 mov qword ptr [r12 + 656], rax
 mov qword ptr [r12 + 664], rdx
 jmp xgvcat74_0d
 xgvcat74_0b:
 jmp snoch0_n24_α
xgvcat74_0d:
# IR_VAR
bb52_α:
 mov rdi, qword ptr [rip + .Lx94_0]
 call NV_GET_fn@PLT
 mov qword ptr [r12 + 672], rax
 mov qword ptr [r12 + 680], rdx
 jmp xgvcat92_1d
 xgvcat92_1b:
 jmp snoch0_n24_α
.Lx94_0:
 .quad .Lx94_0_s
.Lx94_0_s:
 .string "C"
xgvcat92_1d:
bb53_α:
# IR_BINOP_GVAR_CONCAT
 mov rdi, 6
 mov rsi, qword ptr [r12 + 464]
 mov rdx, qword ptr [r12 + 480]
 mov rcx, qword ptr [r12 + 488]
 call str_concat_d@PLT
 mov qword ptr [r12 + 688], rax
 mov qword ptr [r12 + 696], rdx
 jmp snoch0_n29_α
 snoch0_n28_β:
 jmp snoch0_n24_α
snoch0_n29_α:
bb54_α:
# IR_ASSIGN
 lea rdi, [rip + .S7]
 mov rsi, qword ptr [r12 + 688]
 mov rdx, qword ptr [r12 + 696]
 call rt_gvar_assign_descr@PLT
 jmp snoch0_n20_α
 snoch0_n29_β:
 jmp snoch0_n24_α
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
.S1: .string "H"
.S2: .string "INPUT"
.S3: .string "V"
.S4: .string "HC"
.S5: .string "bb_scan: TEXT(mode-4) non-literal pattern needs native PB-RB graph (pending)"
.S6: .string "VC"
.S7: .string "OUTPUT"
.S8: .string ""
.S9: .string "PRINTV"
.S10: .string "C"
.text
