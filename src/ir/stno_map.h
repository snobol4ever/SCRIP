#ifndef SCRIP_STNO_MAP_H
#define SCRIP_STNO_MAP_H
#include <stdint.h>
#include <stddef.h>
typedef struct sno_stno_rec_t { uint64_t pc; int32_t stno; int32_t line; const char * file; } sno_stno_rec_t;
#ifdef __cplusplus
static_assert(sizeof(sno_stno_rec_t) == 24,
    "sno_stno_rec_t is three sealed quads beside the code in both media (TASK error-voice-statement-number-from-a-code-map-no-per-statement-store, cto ruling 2026-10-01): pc, stno|line, file (cto 20"
    "26-10-02: the line and the file ride the same record so a fatal voice outside --stlimit names prog.sno(65) as sbl does), ascending by pc -- the emitter's g_emit.stno_map vector (mode 3) and the"
    " scrip_stno_map linker section (mode 4) both pack this exact shape so core_error_voice's floor search is one code path over one record shape in both modes");
#else
_Static_assert(sizeof(sno_stno_rec_t) == 24,
    "sno_stno_rec_t is three sealed quads beside the code in both media (TASK error-voice-statement-number-from-a-code-map-no-per-statement-store, cto ruling 2026-10-01): pc, stno|line, file (cto 20"
    "26-10-02: the line and the file ride the same record so a fatal voice outside --stlimit names prog.sno(65) as sbl does), ascending by pc -- the emitter's g_emit.stno_map vector (mode 3) and the"
    " scrip_stno_map linker section (mode 4) both pack this exact shape so core_error_voice's floor search is one code path over one record shape in both modes");
#endif
#ifdef __cplusplus
extern "C" {
#endif
    const sno_stno_rec_t * scrip_emit_stno_table(uint32_t * out_count) __attribute__((weak));
#ifdef __cplusplus
}
#endif
#endif
