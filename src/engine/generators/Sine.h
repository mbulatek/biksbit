/*
 * Sine.h
 *
 *  Created on: Apr 19, 2023
 *      Author: Marek Bulatek <mbulatek@gmail.com>
 */

#ifndef ENGINE_GENERATORS_SINE_H_
#define ENGINE_GENERATORS_SINE_H_

#include <utils/Logger.h>

#include <engine/generators/Oscillator.h>
#include <utils/Params.h>

#include <cmath>
#include <vector>
#include <iostream>
#include <memory>
#include <list>
#include <thread>
#include <atomic>

namespace engine {
namespace generators {

class Oscillator;

class Sine : public Oscillator {

 public:
  Sine();
  virtual ~Sine();

  void control(const CtrlEvent &event) override;
  void process(BlockingQueue<std::vector<Params::bufferT>> &outputQueue)
      override;
  void process(BlockingQueue<std::vector<Params::bufferT>> &inputQueue,
               BlockingQueue<std::vector<Params::bufferT>> &outputQueue)
                   override {
    Logger l(__PRETTY_FUNCTION__);
    throw (std::runtime_error("method not implemented"));
  }


 private:
  double releaseKey;
  uint32_t sampleCount;
  uint32_t samplesPerPeriod;
};

} /* namespace generators */
} /* namespace engine */

#endif /* ENGINE_GENERATORS_SINE_H_ */
