/*
 * SDL.cpp
 *
 *  Created on: May 5, 2023
 *      Author: Marek Bulatek <mbulatek@gmail.com>
 */

#include <frontend/SDL.h>

namespace frontend {

SDL::SDL()
    :
    isRunning(false),
    keyPressed(0) {
}

SDL::~SDL() {
}

void SDL::process() {
  Logger l(__PRETTY_FUNCTION__);

  std::vector<float> freqs { 261.6256, 277.1826, 293.6648, 311.127, 329.6276,
      349.2282, 369.9944, 391.9954, 415.3047, 440, 466.1638, 493.8833,
      523.2511 };
  SDL_Event event;

  while (isRunning) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_KEYDOWN:
          if (keyPressed == event.key.keysym.sym) {
            continue;
          }
          keyPressed = event.key.keysym.sym;
          switch (event.key.keysym.sym) {
            case SDLK_a:
              if (onKeyCb)
                onKeyCb(freqs[0]);
              break;
            case SDLK_w:
              if (onKeyCb)
                onKeyCb(freqs[1]);
              break;
            case SDLK_s:
              if (onKeyCb)
                onKeyCb(freqs[2]);
              break;
            case SDLK_e:
              if (onKeyCb)
                onKeyCb(freqs[3]);
              break;
            case SDLK_d:
              if (onKeyCb)
                onKeyCb(freqs[4]);
              break;
            case SDLK_f:
              if (onKeyCb)
                onKeyCb(freqs[5]);
              break;
            case SDLK_t:
              if (onKeyCb)
                onKeyCb(freqs[6]);
              break;
            case SDLK_g:
              if (onKeyCb)
                onKeyCb(freqs[7]);
              break;
            case SDLK_y:
              if (onKeyCb)
                onKeyCb(freqs[8]);
              break;
            case SDLK_h:
              if (onKeyCb)
                onKeyCb(freqs[9]);
              break;
            case SDLK_u:
              if (onKeyCb)
                onKeyCb(freqs[10]);
              break;
            case SDLK_j:
              if (onKeyCb)
                onKeyCb(freqs[11]);
              break;
            case SDLK_k:
              if (onKeyCb)
                onKeyCb(freqs[12]);
              break;
            default:
              break;
          }
          break;

        case SDL_KEYUP:
          if (keyPressed == 0) {
            continue;
          }
          keyPressed = 0;
          if (onKeyCb)
            onKeyCb(0);
          break;

        case SDL_QUIT:
          isRunning = false;
          break;

        case SDL_WINDOWEVENT:
          switch (event.window.event) {
            case SDL_WINDOWEVENT_CLOSE:
              isRunning = false;
              break;

            default:
              break;
          }
          break;

        default:
          break;
      }
      SDL_FlushEvent(SDL_KEYDOWN);
    }
  }

  SDL_Quit();

  if (onExitCb)
    onExitCb();
}

} /* namespace frontend */
