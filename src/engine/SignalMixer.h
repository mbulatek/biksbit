/*
 * SignalMixer.h
 *
 *  Created on: May 4, 2023
 *      Author: Marek Bulatek <mbulatek@gmail.com>
 */

#ifndef ENGINE_SIGNALMIXER_H_
#define ENGINE_SIGNALMIXER_H_

#include "SignalNode.h"
#include <utils/Logger.h>

#include <iostream>
#include <sstream>

namespace engine {

class SignalMixer : public SignalNode {

  using InputQueue = BlockingQueue<std::vector<Params::bufferT>>;
  using DataType = std::vector<Params::bufferT>;

 public:
  SignalMixer();
  virtual ~SignalMixer();

  void process(std::vector<InputQueue> &inputQueues,
               BlockingQueue<std::vector<Params::bufferT>> &outputQueue);

  void process(BlockingQueue<std::vector<Params::bufferT>> &inputQueue,
               BlockingQueue<std::vector<Params::bufferT>> &outputQueue)
                   override {
    Logger l(__PRETTY_FUNCTION__);
    throw (std::runtime_error("method not implemented"));
  }
};

} /* namespace engine */

#endif /* ENGINE_SIGNALMIXER_H_ */
