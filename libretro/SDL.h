#ifndef _FAKE_SDL_H_
#define _FAKE_SDL_H_

#include <stdio.h>
#include <string.h>
#include "libretro.h"

extern unsigned int FAKE_SDL_TICKS;

extern retro_get_cpu_features_t perf_get_cpu_features_cb;
extern retro_perf_get_counter_t perf_get_counter_cb;
extern retro_perf_log_t perf_log_cb;
extern retro_log_printf_t log_cb;
extern retro_perf_register_t perf_register_cb;

#define SDL_GetTicks() FAKE_SDL_TICKS

#ifdef _MSC_VER
#define strcasecmp stricmp
#endif

#endif
