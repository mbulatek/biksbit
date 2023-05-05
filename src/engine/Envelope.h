/*
 * Envelope.h
 *
 *  Created on: Apr 24, 2023
 *      Author: Marek Bulatek <mbulatek@gmail.com>
 */

#ifndef ENGINE_ENVELOPE_H_
#define ENGINE_ENVELOPE_H_

#include <engine/SignalProcessor.h>
#include <utils/Logger.h>

#include <thread>
#include <atomic>
#include <sstream>

namespace engine {

class Envelope : public SignalProcessor {

 public:
  Envelope();
  virtual ~Envelope();

  void control(const CtrlEvent &event) override;
  void process(BlockingQueue<std::vector<Params::bufferT>> &inputQueue,
               BlockingQueue<std::vector<Params::bufferT>> &outputQueue) override;


 private:
  enum class Phase {
    Attack,
    Decay,
    Sustain,
    Release,
    Standby
  };

  uint16_t A, D, R;
  float S;

  Phase currentPhase;
  double sampleCount;

  std::mutex mtx;
};

} /* namespace engine */

#endif /* ENGINE_ENVELOPE_H_ */
