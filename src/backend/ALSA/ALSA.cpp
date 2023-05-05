/*
 * ALSA.cpp
 *
 *  Created on: Apr 21, 2023
 *      Author: Marek Bulatek <mbulatek@gmail.com>
 */

#include <backend/ALSA/ALSA.h>
#include <utils/Logger.h>

#include <iostream>

namespace backend {

ALSA::ALSA()
    :
    pcm(nullptr),
    toBeReset(false) {
}

ALSA::~ALSA() {
  if (pcm != nullptr) {
    snd_pcm_drain(pcm);
    snd_pcm_close(pcm);
  }
}

void ALSA::init() {
  std::unique_lock<std::mutex> lock(mtx);

  snd_pcm_open(&pcm, "default", SND_PCM_STREAM_PLAYBACK, SND_PCM_NONBLOCK);

  snd_pcm_hw_params_t *hw_params;
  snd_pcm_hw_params_alloca(&hw_params);

  snd_pcm_hw_params_any(pcm, hw_params);
  snd_pcm_hw_params_set_access(pcm, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED);
  snd_pcm_hw_params_set_format(pcm, hw_params, SND_PCM_FORMAT_S16_LE);
  snd_pcm_hw_params_set_channels(pcm, hw_params, 1);
  snd_pcm_hw_params_set_rate(pcm, hw_params, engine::Params::sampleRate, 0);
  snd_pcm_hw_params_set_periods(pcm, hw_params, 4, 0);
  snd_pcm_hw_params_set_period_time(pcm, hw_params, 20000, 0);
  snd_pcm_hw_params_set_buffer_size(pcm, hw_params, 256);

  std::cout << "period time: "
            << engine::Params::bufferSize * 1000000 / engine::Params::sampleRate
            << std::endl;

  snd_pcm_hw_params(pcm, hw_params);
}

void ALSA::reset() {
  Logger l(__PRETTY_FUNCTION__);
  std::unique_lock<std::mutex> lock(mtx);

  toBeReset = true;
}

void ALSA::control(const engine::CtrlEvent &event) {
  SignalNode::control(event);
  switch (event.getType()) {
    case engine::CtrlEvent::Type::KeyDown:
      reset();
      break;
    case engine::CtrlEvent::Type::KeyUp:
      break;
  }
}

void ALSA::process(
    engine::BlockingQueue<std::vector<engine::Params::bufferT>> &inputQueue,
    engine::BlockingQueue<std::vector<engine::Params::bufferT>> &outputQueue) {
  Logger l(__PRETTY_FUNCTION__);

  init();

  while (state == State::Started) {

    std::vector<engine::Params::bufferT> data;
    try {
      data = inputQueue.pop();
    } catch (std::runtime_error &e) {
      l.write("queue flushed");
      continue;
    }

    std::stringstream ss;
    ss << "ALSA: ";
    for (auto d : data) {
      ss << d << " ";
    }
    ss << std::endl;
    l.writeRaw(Logger::Level::Trace, ss.str());

    while (!data.empty()) {
      if (toBeReset) {
        snd_pcm_drop(pcm);
        //snd_pcm_drain(pcm);
        snd_pcm_prepare(pcm);
        toBeReset = false;
      }
      snd_pcm_sframes_t framesWritten = snd_pcm_writei(pcm, &data[0],
                                                       data.size());
      if (framesWritten == -EAGAIN)
        continue;

      if (framesWritten == -EPIPE) {
        //reset();
        l.write("ALSA recover!!!");
        snd_pcm_recover(pcm, framesWritten, 0);
        continue;
      } else if (framesWritten < 0) {
        //throw std::runtime_error("Cannot write ALSA buffer");
        snd_pcm_recover(pcm, framesWritten, 0);
        l.write("Cannot write ALSA buffer: " + std::to_string(framesWritten));
      } else {
        data.erase(data.begin(), data.begin() + framesWritten);
        l.write(Logger::Level::Debug,
                "frames written: " + std::to_string(framesWritten));
      }
    }
  }
}

}
/* namespace backend */
