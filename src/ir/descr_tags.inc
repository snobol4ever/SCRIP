#ifndef DESCR_TAGS_INC
#define DESCR_TAGS_INC
#define DT_NUMERIC_BIT 0x01
#define DT_CHARS_BIT 0x02
#define DT_REAL_BIT 0x04
#define DT_NOTSTR_MASK 0xFFFFFFFD
#define DT_SNUL 0x00
#define DT_S 0x02
#define DT_I 0x03
#define DT_R 0x05
#define DT_P 0x08
#define DT_A 0x10
#define DT_T 0x18
#define DT_C 0x20
#define DT_N 0x28
#define DT_K 0x30
#define DT_E 0x38
#define DT_FH 0x40
#define DT_PLVAR 0x48
#define DT_PLREF 0x50
#define DT_X 0x58
#define DT_BLK 0x60
#define DT_FAIL 0x68
#define DT_DATA 0x70
#define DT_BIG 0x78
#define DT_CO 0x80
#define DT_BOOL 0x88
#define DT_ORDER 0x90
#define DT_RAW 0x98
#define DT_MAP 0xA0
#define DT_CPLX 0xA8
#define DT_PLATOM 0xB0
#define DT_EXTL 0xB8
#define RTX_NOT_HANDLED 0xFF
#define DT_NAMETRAP_LO (((2) << 32) | DT_N)
#endif
