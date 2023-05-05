/*
 * Backend.h
 *
 *  Created on: Apr 19, 2023
 *      Author: Marek Bulatek <mbulatek@gmail.com>
 */

#ifndef BACKEND_BACKEND_H_
#define BACKEND_BACKEND_H_

#include <engine/SignalProcessor.h>

#include <vector>

namespace backend {

class Backend : public engine::SignalProcessor {
 public:
  Backend() {
  }
  ;
  virtual ~Backend() {
  }
  ;

  virtual void init() = 0;
  virtual void reset() = 0;
};

} /* namespace backend */

#endif /* BACKEND_BACKEND_H_ */
