#ifndef SYG_SDL_HEADERS_H
#define SYG_SDL_HEADERS_H

#if __has_include("SDL2/SDL.h")
#	include "SDL2/SDL.h"
#elif __has_include("SDL.h")
#	include "SDL.h"
#endif

#endif // SYG_SDL_HEADERS_H
