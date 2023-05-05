/*
 * SignalProcessor.h
 *
 *  Created on: May 4, 2023
 *      Author: Marek Bulatek <mbulatek@gmail.com>
 */

#ifndef ENGINE_SIGNALPROCESSOR_H_
#define ENGINE_SIGNALPROCESSOR_H_

#include "SignalNode.h"

namespace engine {

class SignalProcessor : public SignalNode {
 public:
  SignalProcessor();
  virtual ~SignalProcessor();
};

} /* namespace engine */

#endif /* ENGINE_SIGNALPROCESSOR_H_ */
