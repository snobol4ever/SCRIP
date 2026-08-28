/* portcount.h -- exact per-box α/β port execution counters (slice 4, route (a)).  Global granted by Lon in-chat via CEO, 2026-08-28, GOAL-CEO CEO-75. */
#ifndef PORTCOUNT_H
#define PORTCOUNT_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
int        rt_port_counts_on(void);                                            /* SCRIP_PORT_COUNTS=1 -- read once, telemetry defaults OFF */
uint64_t * rt_port_counts_slot(int uid, int port, const char * label);         /* EMISSION time: stable cell address for (box, port); port 0=α 1=β */
void       rt_port_counts_dump(void);                                          /* RUN time: print the table, or REFUSE if armed and nothing registered */
#ifdef __cplusplus
}
#endif
#endif
