/*
 * SDL.h
 *
 *  Created on: May 5, 2023
 *      Author: Marek Bulatek <mbulatek@gmail.com>
 */

#ifndef FRONTEND_SDL_H_
#define FRONTEND_SDL_H_

#include "Frontend.h"
#include <utils/Logger.h>

#include <functional>

#include <SDL2/SDL.h>

namespace frontend {

// TODO - temporary implementation, needs to be redesigned immediately
class SDL : public Frontend {
 public:
  SDL();
  virtual ~SDL();

  void run() {
    isRunning = true;
  }

  void setOnKeyCb(std::function<void(float)> cb) {
    onKeyCb = cb;
  }

  void setOnExitCb(std::function<void()> cb) {
    onExitCb = cb;
  }

  void operator()() {
    Logger l(__PRETTY_FUNCTION__);
    process();
  }

  void process() override;

 private:
  bool isRunning;
  int keyPressed;
  std::function<void(float)> onKeyCb;
  std::function<void()> onExitCb;
};

} /* namespace frontend */

#endif /* FRONTEND_SDL_H_ */
