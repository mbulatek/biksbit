/*
 * Pipeline.cpp
 *
 *  Created on: May 4, 2023
 *      Author: Marek Bulatek <mbulatek@gmail.com>
 */

#include <engine/Pipeline.h>

namespace engine {

Pipeline::Pipeline()
    :
    genQueues(0),
    queues(1) {
}

Pipeline::~Pipeline() {
}

void Pipeline::control(const CtrlEvent &event) {
  Logger l(__PRETTY_FUNCTION__);

  if (event.getType() == CtrlEvent::Type::KeyUp)
    l.write("KeyUp detected");

  for (auto &generator : generators) {
    generator->control(event);
  }
  mixer->control(event);
  for (auto &processor : processors) {
    processor->control(event);
  }

  if (event.getType() == CtrlEvent::Type::Stop
      || event.getType() == CtrlEvent::Type::KeyDown) {
    for (auto &q : genQueues) {
      q.flush();
    }
    for (auto &q : queues) {
      q.flush();
    }
  }
}

void Pipeline::process() {
  for (size_t i = 0; i < generators.size(); ++i) {
    threads.emplace_back([this, i, &genQueues = genQueues]() {
      generators[i]->process(genQueues[i]);
    });
  }

  threads.emplace_back([this, &genQueues = genQueues, &queues = queues]() {
    mixer->process(genQueues, queues[0]);
  });

  for (size_t i = 0; i < processors.size(); ++i) {
    threads.emplace_back([this, i, &queues = queues]() {
      processors[i]->process(queues[i], queues[i + 1]);
    });
  }

  for (auto &t : threads) {
    t.join();
  }
}

} /* namespace engine */
