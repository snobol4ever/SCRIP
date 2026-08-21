#ifndef AB_ABI_H
#define AB_ABI_H
#define AB_OFF_GW       (-0x10)
#define AB_OFF_WW       (-0x18)
#define AB_OFF_ERSP     (-0x20)
#define AB_OFF_ANCHOR   (-0x28)
#define AB_OFF_WN       (-0x30)
#define AB_OFF_VTMARK   (-0x38)
#define AB_OFF_BADDR    (-0x40)
#define AB_OFF_SIGMA    (-0x48)
#define AB_OFF_SIGMALEN (-0x50)
#define AB_OFF_RES0     (-0x58)
#define AB_OFF_RES1     (-0x60)
#define AB_OFF_PAD      (-0x68)
#define AB_OFF_SAVE0    (-0x70)
#define AB_META_BYTES   96
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline long ab_frame_k(long nsave) { long t = AB_META_BYTES + nsave * 16L; return ((t + 8 + 15) & ~15L) - 8; }
static inline int ab_save_off(long nsave, int k) { (void)nsave; return AB_OFF_SAVE0 - (int)(16L * (long)k); }
#define AB_ARGREG "r9"
#define AB_TYPECODE_REG "cl"
#define AB_TC_RETURN    0
#define AB_TC_NRETURN   1
#define AB_TC_FRETURN   2
#endif
