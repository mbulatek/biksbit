/*
 * SignalMixer.cpp
 *
 *  Created on: May 4, 2023
 *      Author: Marek Bulatek <mbulatek@gmail.com>
 */

#include <engine/SignalMixer.h>

namespace engine {

SignalMixer::SignalMixer() {
}

SignalMixer::~SignalMixer() {
}

void SignalMixer::process(
    std::vector<InputQueue> &inputQueues,
    BlockingQueue<std::vector<Params::bufferT>> &outputQueue) {
  Logger l(__PRETTY_FUNCTION__);

  while (state == State::Started) {
    std::vector<std::vector<Params::bufferT>> inputData(0);

    for (auto &input : inputQueues) {
      try {
        inputData.emplace_back(input.pop());
      } catch (std::runtime_error &e) {
        l.write("queue flushed");
        continue;
      }
    }

    std::vector<Params::bufferT> outputData(Params::bufferSize, 0);

    for (int i = 0; i < Params::bufferSize; i++) {
      for (auto d : inputData) {
        outputData[i] += d[i];
      }
      outputData[i] /= inputQueues.size();
    }

    std::stringstream ss;
    ss << "Mixer: ";
    for (auto s : outputData) {
      ss << s << " ";
    }
    ss << std::endl;
    l.writeRaw(Logger::Level::Trace, ss.str());

    outputQueue.push(outputData);
  }
}

} /* namespace engine */
