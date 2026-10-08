#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "core.h"
#include "sil_macros.h"
#include "name_t.h"
typedef struct { int live; NAME_t name; const char *substr; int slen; } NAME_entry_t;
