/*
 * OpenAL.h
 *
 *  Created on: Apr 19, 2023
 *      Author: Marek Bulatek <mbulatek@gmail.com>
 */

#ifndef BACKEND_OPENAL_OPENAL_H_
#define BACKEND_OPENAL_OPENAL_H_

#include <vector>

#include <AL/al.h>
#include <AL/alc.h>

#include <backend/Backend.h>

namespace backend {

class OpenAL : public Backend {
  ALuint samplesBuff;
  ALuint src;

 public:
  OpenAL();
  virtual ~OpenAL();

  void init();
  void play(std::vector<short> &buf);
};

} /* namespace backend */

#endif /* BACKEND_OPENAL_OPENAL_H_ */
