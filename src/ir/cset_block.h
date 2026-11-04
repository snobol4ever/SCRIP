#ifndef CSET_BLOCK_H
#define CSET_BLOCK_H
#include <stdint.h>
#include <string.h>
#include "ir/descr.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline void cset_block_header(unsigned char * base, const char * chars, int len) {
    int32_t n = (int32_t) len;
    uint32_t m = CSET_BLK_MAGIC;
    memcpy(base, &n, 4);
    memcpy(base + 4, &m, 4);
    memset(base + 8, 0, 32);
    for (int i = 0; i < len; i++) base[8 + ((unsigned char) chars[i] >> 3)] |= (unsigned char) (1u << ((unsigned char) chars[i] & 7));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static inline char * cset_block_init(char * base, const char * chars, int len) {
    cset_block_header((unsigned char *) base, chars, len);
    if (len > 0) memcpy(base + CSET_BLK_HDR, chars, (size_t) len);
    base[CSET_BLK_HDR + (len > 0 ? len : 0)] = '\0';
    return base + CSET_BLK_HDR;
}
#endif
