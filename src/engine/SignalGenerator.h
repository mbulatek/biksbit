/*
 * SignalGenerator.h
 *
 *  Created on: May 4, 2023
 *      Author: Marek Bulatek <mbulatek@gmail.com>
 */

#ifndef ENGINE_SIGNALGENERATOR_H_
#define ENGINE_SIGNALGENERATOR_H_

#include "SignalNode.h"

namespace engine {

class SignalGenerator : public SignalNode {
 public:
  SignalGenerator();
  virtual ~SignalGenerator();

  virtual void process(
      BlockingQueue<std::vector<Params::bufferT>> &outputQueue) = 0;

};

} /* namespace engine */

#endif /* ENGINE_SIGNALGENERATOR_H_ */
