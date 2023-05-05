/*
 * Sine.cpp
 *
 *  Created on: Apr 19, 2023
 *      Author: Marek Bulatek <mbulatek@gmail.com>
 */

#include <engine/generators/Sine.h>
#include <utils/Logger.h>

#include <sstream>

namespace engine {
namespace generators {

Sine::Sine()
    :
    releaseKey(0),
    sampleCount(0),
    samplesPerPeriod(0) {
}

Sine::~Sine() {
}

void Sine::control(const CtrlEvent &event) {
  Logger l(__PRETTY_FUNCTION__);
  SignalNode::control(event);

  std::lock_guard<std::mutex> lock(mtx);

  if (event.getType() == CtrlEvent::Type::KeyDown) {
    l.write("KeyDown detected");
    samplesPerPeriod = key * Params::sampleRate;
    releaseKey = key;
    if (freq != 0) {
      if (freq > 0)
        releaseKey *= pow(pow(2, 0.0833), freq);
      else
        freq = 1.f * freq;
      releaseKey /= pow(pow(2, 0.0833), freq);
    }
    l.write("actual frequency set to: " + std::to_string(releaseKey));
  }

  if (event.getType() == CtrlEvent::Type::KeyUp) {
    l.write("KeyUp detected");
  }
  l.write("key = " + std::to_string(key));
}

void Sine::process(BlockingQueue<std::vector<Params::bufferT>> &outputQueue) {

  Logger l(__PRETTY_FUNCTION__);

  while (state == State::Started) {
    std::unique_lock<std::mutex> lock(mtx);
    if (key == 0 && releaseKey == 0)
      continue;

    l.write(Logger::Level::Debug, "preparing data");

    std::vector<Params::bufferT> waveform(Params::bufferSize);

    for (auto &s : waveform) {
      // TODO consider more common range instead of sample depth (maybe <-1.0,1.0>)
      // conversion to actual sample depth should be done in backend rather
      s = (Params::sampleDepth / 2.0f
          * sin(
              (2.f * float(M_PI) * releaseKey) / Params::sampleRate
                  * sampleCount++));
      if (sampleCount > samplesPerPeriod)
        sampleCount = 0;
    }

    std::stringstream ss;
    ss << "Sine: ";
    for (auto s : waveform) {
      ss << s << " ";
    }
    ss << std::endl;
    l.writeRaw(Logger::Level::Trace, ss.str());

    lock.unlock();

    outputQueue.push(waveform);
  }
}

} /* namespace generators */
} /* namespace engine */
