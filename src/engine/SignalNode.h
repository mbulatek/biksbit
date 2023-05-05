/*
 * SignalNode.h
 *
 *  Created on: May 4, 2023
 *      Author: Marek Bulatek <mbulatek@gmail.com>
 */

#ifndef ENGINE_SIGNALNODE_H_
#define ENGINE_SIGNALNODE_H_

#include <utils/BlockingQueue.h>
#include <utils/Params.h>
#include <engine/CtrlEvent.h>
#include <utils/Logger.h>

#include <future>
#include <vector>

namespace engine {

class SignalNode {

 public:
  enum class State {
    Started,
    Stopped
  };

  SignalNode()
      :
      state(State::Stopped),
      key(0) {
  }
  ;
  virtual ~SignalNode() {
  }
  ;

  virtual void process(
      BlockingQueue<std::vector<Params::bufferT>> &inputQueue,
      BlockingQueue<std::vector<Params::bufferT>> &outputQueue) = 0;

  virtual void control(const CtrlEvent &event) {
    Logger l(__PRETTY_FUNCTION__);
    std::lock_guard<std::mutex> lock(mtx);

    switch (event.getType()) {
      case CtrlEvent::Type::Start:
        state = State::Started;
        break;
      case CtrlEvent::Type::Stop:
        state = State::Stopped;
        break;
      case CtrlEvent::Type::KeyDown:
        key = event.getValue();
        break;
      case CtrlEvent::Type::KeyUp:
        l.write("KeyUp detected");
        key = 0;
        ;
        break;

    }
  }

 protected:
  State state;
  float key;
  std::mutex mtx;

};

} /* namespace engine */

#endif /* ENGINE_SIGNALNODE_H_ */
