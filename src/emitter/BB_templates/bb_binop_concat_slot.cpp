/* bb_binop_concat_slot.cpp — TEMPLATE-REVAMP: emptied (Lon directive 2026-06-02).  This file held a non-static *_str helper
   consumed ONLY by its router (bb_binop.cpp), which is now a LOUD x86_bomb() stub.  With no caller, the helper
   is dead, so the translation unit is intentionally empty until the owning GOAL-*-BB session rewrites the
   router + its arms as real x86() self-encoding.  Keeps the Makefile RT_PIC_SRCS line valid (compiles to an
   empty object). */
