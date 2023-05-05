/*
 * main.cpp
 *
 *  Created on: Apr 24, 2023
 *      Author: Marek Bulatek <mbulatek@gmail.com>
 */

#include <backend/openal/OpenAL.h>
#include <backend/ALSA/ALSA.h>

#include <engine/generators/Sine.h>
#include <engine/Envelope.h>

#include <engine/Pipeline.h>
#include <engine/CtrlEvent.h>

#include <frontend/SDL.h>

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <memory>

#include <SDL2/SDL.h>

int main(int argc, char *argv[]) {

  Logger::setFile("log.txt");
  Logger::setLevel(Logger::Level::Info);
  Logger l(__PRETTY_FUNCTION__);

  engine::Pipeline pipeline;

  pipeline.addGenerator(std::make_unique<engine::generators::Sine>());

  engine::generators::Sine *sine = new engine::generators::Sine();
  sine->setFreq(-5);

  pipeline.addGenerator(std::unique_ptr<engine::generators::Sine>(sine));

  //engine::generators::Sine *sine2 = new engine::generators::Sine();
  //sine2->setFreq(3);
  //pipeline.addGenerator(std::unique_ptr<engine::generators::Sine>(sine2));

  pipeline.addMixer(std::make_unique<engine::SignalMixer>());

  pipeline.addProcessor(std::make_unique<engine::Envelope>());
  pipeline.addProcessor(std::make_unique<backend::ALSA>());

  if (SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO) < 0) {
    l.write("could not initialize SDL: " + std::string(SDL_GetError()));
    exit(-1);
  }

  SDL_Window *win = SDL_CreateWindow("SYNTH",
                                     SDL_WINDOWPOS_CENTERED,
                                     SDL_WINDOWPOS_CENTERED, 320, 200, 0);

  frontend::SDL sdl;
  sdl.setOnKeyCb([&pipeline](float key) {
    if (key == 0) {
      engine::CtrlEvent ctrlEvent2(engine::CtrlEvent::Type::KeyUp);
      pipeline.control(ctrlEvent2);
      return;
    }

    engine::CtrlEvent ctrlEvent2(engine::CtrlEvent::Type::KeyDown, key);
    pipeline.control(ctrlEvent2);
  });

  sdl.setOnExitCb([&pipeline]() {
    engine::CtrlEvent ctrlEvent2(engine::CtrlEvent::Type::Stop);
    pipeline.control(ctrlEvent2);
  });

  sdl.run();
  std::thread feThread(sdl);

  engine::CtrlEvent ctrlEvent1(engine::CtrlEvent::Type::Start);
  pipeline.control(ctrlEvent1);

  pipeline.process();

  feThread.join();

  return 0;
}
