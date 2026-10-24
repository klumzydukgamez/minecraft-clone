/* Copyright (c) 2026 Klumzy Duk Gamez.
 * See LICENSE.md for full terms. */

#if !defined MC_WINDOW_H
#define MC_WINDOW_H

#include "mc_third_party.h"

bool mc_init_window(SDL_Window** window, SDL_GLContext* glContext);
void mc_cleanup_window(SDL_Window** window, SDL_GLContext* glContext);

#endif /* MC_WINDOW_H */
