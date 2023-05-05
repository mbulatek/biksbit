/*
 * Envelope.cpp
 *
 *  Created on: Apr 24, 2023
 *      Author: Marek Bulatek <mbulatek@gmail.com>
 */

#include <engine/Envelope.h>

namespace engine {

Envelope::Envelope()
    :
    A(300),
    D(100),
    S(0.7f),
    R(100),
    currentPhase(Phase::Standby),
    sampleCount(0) {
}

Envelope::~Envelope() {
}

void Envelope::control(const CtrlEvent &event) {
  Logger l(__PRETTY_FUNCTION__);
  SignalNode::control(event);
  switch (event.getType()) {
    case CtrlEvent::Type::KeyDown:
      currentPhase = Phase::Attack;
      l.write("attack");
      sampleCount = 0;
      break;
    case CtrlEvent::Type::KeyUp:
      currentPhase = Phase::Release;
      l.write("release");
      sampleCount = 0;
      break;
  }
}

void Envelope::process(BlockingQueue<std::vector<Params::bufferT>> &inputQueue,
             BlockingQueue<std::vector<Params::bufferT>> &outputQueue) {
  Logger l(__PRETTY_FUNCTION__);

  float samplesPerMs = Params::sampleRate / 1000.0f;

  double Asampl = A * samplesPerMs;
  double Dsampl = D * samplesPerMs;
  double Rsampl = R * samplesPerMs;

  Asampl = (Asampl == 0) ? 1 : Asampl;
  Dsampl = (Dsampl == 0) ? 1 : Dsampl;
  Rsampl = (Rsampl == 0) ? 1 : Rsampl;

  while (state == State::Started) {
    if (currentPhase == Phase::Standby) {
      std::vector<Params::bufferT> zero(Params::bufferSize, 0);
      outputQueue.push(zero);
      continue;
    }

    std::vector<Params::bufferT> data;
    try {
      data = inputQueue.pop();
    } catch (std::runtime_error &e) {
      l.write("queue flushed");
      continue;
    }

    for (auto it = data.begin(); it != data.end(); it++) {
      auto &s = *it;

      switch (currentPhase) {
        case Phase::Attack:
          if (sampleCount >= Asampl) {
            currentPhase = Phase::Decay;
            l.write("decay");
            sampleCount = 0;
            break;
          }
          s = s * sampleCount / Asampl;
          sampleCount++;
          break;
        case Phase::Decay:
          if (sampleCount >= Dsampl) {
            currentPhase = Phase::Sustain;
            sampleCount = 0;
            break;
          }
          s = s * ((S - 1) / Dsampl) * sampleCount + s;
          sampleCount++;
          break;
        case Phase::Sustain:
          s = s * S;
          break;
        case Phase::Release:
          if (sampleCount >= Rsampl - 2) {
            currentPhase = Phase::Standby;
            for (auto i = it; i != data.end(); i++)
              *i = 0;
            l.write("standby");
            sampleCount = 0;
            break;
          }
          s = s * -1.0f * (S) / (Rsampl) * sampleCount + s * S;
          sampleCount++;
          break;
        default:
          break;
      }
    }

    std::stringstream ss;
    ss << "Envelope: ";
    for (auto s : data) {
      ss << s << " ";
    }
    ss << std::endl;
    l.writeRaw(Logger::Level::Trace, ss.str());

    outputQueue.push(data);
  }
}

} /* namespace engine */
