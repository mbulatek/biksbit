/*
 * OpenAL.cpp
 *
 *  Created on: Apr 19, 2023
 *      Author: Marek Bulatek <mbulatek@gmail.com>
 */

#include <iostream>

#include "OpenAL.h"

namespace backend {

#define al_check_error() \
    __al_check_error(__FILE__, __LINE__)

#define CASE_RETURN(err) case (err): return "##err"
const char* al_err_str(ALenum err) {
  switch (err) {
    CASE_RETURN(AL_NO_ERROR)
;      CASE_RETURN(AL_INVALID_NAME);
      CASE_RETURN(AL_INVALID_ENUM);
      CASE_RETURN(AL_INVALID_VALUE);
      CASE_RETURN(AL_INVALID_OPERATION);
      CASE_RETURN(AL_OUT_OF_MEMORY);
    }
    return "unknown";
  }
#undef CASE_RETURN

#define __al_check_error(file,line) \
    do { \
        ALenum err = alGetError(); \
        for(; err!=AL_NO_ERROR; err=alGetError()) { \
            std::cerr << "AL Error " << al_err_str(err) << " at " << file << ":" << line << std::endl; \
        } \
    }while(0)

OpenAL::OpenAL()
    :
    samplesBuff(0),
    src(0) {
}

OpenAL::~OpenAL() {
  ALCdevice *dev = NULL;
  ALCcontext *ctx = NULL;
  ctx = alcGetCurrentContext();
  dev = alcGetContextsDevice(ctx);

  alcMakeContextCurrent(NULL);
  alcDestroyContext(ctx);
  alcCloseDevice(dev);
}

void OpenAL::init() {
  /* initialize OpenAL */
  ALCdevice *dev = NULL;
  ALCcontext *ctx = NULL;

  const char *defname = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);
  std::cout << "Default device: " << defname << std::endl;

  dev = alcOpenDevice(defname);
  ctx = alcCreateContext(dev, NULL);
  alcMakeContextCurrent(ctx);

  /* Create buffer to store samples */
  alGenBuffers(1, &samplesBuff);
  //al_check_error();
}

void OpenAL::play(std::vector<short> &buf) {
  //for (int i = 0; i < buf.size(); i++ )
  //	std::cout << buf[i] << " ";

  ALint state;

  do {
    alGetSourcei(src, AL_SOURCE_STATE, &state);
  } while (AL_PLAYING == state);

  if (src != 0) {
    alDeleteSources(1, &src);
    src = 0;
  }

  alBufferData(samplesBuff, AL_FORMAT_MONO16, &(buf[0]), buf.size(), 22050);
  //al_check_error();

  /* Set-up sound source and play buffer */

  alGenSources(1, &src);
  alSourcei(src, AL_BUFFER, samplesBuff);
  alSourcePlay(src);
}

} /* namespace backend */
