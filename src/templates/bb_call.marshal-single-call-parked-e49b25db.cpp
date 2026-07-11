/* PARKED 2026-07-11 (Lon approval, session s28) — OUT OF THE MAKEFILE, NEVER DELETED (PARK-NEVER-DELETE).
 * marshal_single_call() + its three call sites in bb_call.cpp (arith_opnd_a :129, arith_opnd_b :167,
 * marshal_call_arg :282).  PROVEN DEAD, MEASURED NOT REASONED: across a 592-file sweep of the live corpus
 * (corpus/crosscheck + corpus/programs/icon) compiled with scrip --compile at e49b25db, this helper emitted
 * ZERO of its signature .Lcallfn labels AND ZERO of its own bomb stubs -- i.e. it was never CALLED, not merely
 * bombing early.  It could only ever have served a ZERO-ARG call in arith-operand position (it bombs outright
 * on nn > 0), a shape LOWER no longer routes here; deterministic proc calls went to bb_call_proc_staged's
 * bcps_det_arm at NCB-1b.  Its two address-taking lines (rt_call_named_proc / rt_call_named_proc_sl) were the
 * ONLY remaining reason V1's body stayed linked from a template -- the NCB ledger's F8 finding.  The six
 * committed .s artifacts that still showed `call rt_call_named_proc` were STALE and LYING: all six recompile
 * to ZERO today.  If a future shape needs a call in arith-operand position, the NCB-1b window is the arm to
 * reuse (F5: no new encoder needed) -- do NOT resurrect this trampoline form. */
static std::string marshal_single_call(IR_t * lf, int aoff, int lblid) {
    const char * nfn = IR_LIT(lf).sval ? IR_LIT(lf).sval : "";
    int nn = (int) IR_LIT(lf).ival;
    int avbase = zoff(lf);
    if (avbase < 0 || nn > (lf ? lf->n_operands : 0)) return x86_bomb("marshal_single_call: no/short LOWER slot grant (TMP-ERADICATE)");
    avbase += 16;
    int isreg = (nfn[0] && rt_proc_is_registered(nfn));
    int nmig  = isreg && rt_proc_frame_nslots(nfn) >= 0;
    const char * rsym = isreg ? (nmig ? "rt_call_named_proc_sl" : "rt_call_named_proc") : "rt_call_arr";
    std::string s;
    if (nn > 0) return x86_bomb("marshal_single_call: arg marshal gouged, no sub-graph source (TMP-ERADICATE)");
    uint64_t fptr;
    if (nmig)       { DESCR_t (*fp)(const char *, DESCR_t *, int, void *) = rt_call_named_proc_sl; fptr = (uint64_t)(uintptr_t)(void*)fp; }
    else if (isreg) { DESCR_t (*fp)(const char *, DESCR_t *, int) = rt_call_named_proc; fptr = (uint64_t)(uintptr_t)(void*)fp; }
    else            { DESCR_t (*fp)(const char *, DESCR_t *, int) = rt_call_arr;        fptr = (uint64_t)(uintptr_t)(void*)fp; }
    std::string fl = std::string(".Lcallfn") + std::to_string(lblid);
    s += x86("directive", ".section .rodata");
    s += x86("directive", (fl + ": .string \"" + nfn + "\"").c_str());
    s += x86("directive", ".section .text");
    s += x86("directive", ".intel_syntax noprefix");
    s += x86("lea", "rdi", "[rip + __]", (uint64_t)(uintptr_t)nfn, fl.c_str());
    s += x86("lea", "rsi", FRQ(avbase));
    s += x86("mov32", "edx", (long)nn);
    s += IF(nmig, pas_sl_setup(nfn));
    s += x86("call", rsym, fptr);
    s += x86("mov", FRQ(aoff), "rax");
    s += x86("mov", FRQ(aoff + 8), "rdx");
    return s;
}
