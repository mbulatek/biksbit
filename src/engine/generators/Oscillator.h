/*
 * Oscillator.h
 *
 *  Created on: Apr 24, 2023
 *      Author: Marek Bulatek <mbulatek@gmail.com>
 */

#ifndef ENGINE_GENERATORS_OSCILLATOR_H_
#define ENGINE_GENERATORS_OSCILLATOR_H_

#include <engine/SignalGenerator.h>

#include <cstdint>
#include <vector>

namespace engine {
namespace generators {

class Oscillator : public SignalGenerator {

 public:
  Oscillator()
      :
      freq(0) {
  }
  ;
  virtual ~Oscillator() {
  }

  virtual float getFreq() const {
    return freq;
  }
  virtual void setFreq(float freq) {
    this->freq = freq;
  }

 protected:
  static constexpr float DEFAULT_FREQUENCY = 440.f;
  double freq;
};

} /* namespace generators */
} /* namespace engine */

#endif /* ENGINE_GENERATORS_OSCILLATOR_H_ */
