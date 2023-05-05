/*
 * Params.h
 *
 *  Created on: Apr 20, 2023
 *      Author: Marek Bulatek <mbulatek@gmail.com>
 */

#ifndef ENGINE_PARAMS_H_
#define ENGINE_PARAMS_H_

#include <memory>
#include <cstdint>

namespace engine {

class Params {
  Params();

 public:

  static uint32_t sampleRate;
  static uint32_t sampleDepth;
  static uint16_t bufferSize;
  static uint16_t bufferThreshold;
  static uint16_t bufferMinFulfill;
  using bufferT = int16_t;

  struct Oscillator {
    static uint16_t bufferSize;
  };
  static Oscillator oscillator;

  virtual ~Params();
};

} /* namespace engine */

#endif /* ENGINE_PARAMS_H_ */
