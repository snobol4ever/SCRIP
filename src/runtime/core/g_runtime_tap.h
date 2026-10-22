#ifndef G_RUNTIME_TAP_H
#define G_RUNTIME_TAP_H
typedef struct { long tap_run; long tap_failed; long tap_planned; int tap_no_plan; long tap_todo_upto; const char *tap_todo_reason; int tap_done_run; } g_tap_t;
#endif
