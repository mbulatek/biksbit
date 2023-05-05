/*
 * Frontend.h
 *
 *  Created on: Apr 21, 2023
 *      Author: Marek Bulatek <mbulatek@gmail.com>
 */

#ifndef FRONTEND_FRONTEND_H_
#define FRONTEND_FRONTEND_H_

namespace frontend {

class Frontend {
 public:
  Frontend();
  virtual ~Frontend();

  virtual void process() = 0;
};

} /* namespace frontend */

#endif /* FRONTEND_FRONTEND_H_ */
