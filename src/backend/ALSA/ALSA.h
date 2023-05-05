/*
 * ALSA.h
 *
 *  Created on: Apr 21, 2023
 *      Author: Marek Bulatek <mbulatek@gmail.com>
 */

#ifndef BACKEND_ALSA_ALSA_H_
#define BACKEND_ALSA_ALSA_H_

#include <backend/Backend.h>
#include <utils/Params.h>
#include <utils/Logger.h>

#include <vector>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <atomic>
#include <iostream>
#include <sstream>

#include <alsa/asoundlib.h>

namespace backend {

class ALSA : public Backend {

 public:
  ALSA();
  virtual ~ALSA();

  void init() override;
  void reset() override;

  void control(const engine::CtrlEvent &event) override;
  void process(
      engine::BlockingQueue<std::vector<engine::Params::bufferT>> &inputQueue,
      engine::BlockingQueue<std::vector<engine::Params::bufferT>> &outputQueue)
          override;


 private:
  snd_pcm_t *pcm;
  std::mutex mtx;
  bool toBeReset;
};

} /* namespace backend */

#endif /* BACKEND_ALSA_ALSA_H_ */
