/*
 * CtrlEvent.h
 *
 *  Created on: May 4, 2023
 *      Author: Marek Bulatek <mbulatek@gmail.com>
 */

#ifndef ENGINE_CTRLEVENT_H_
#define ENGINE_CTRLEVENT_H_

namespace engine {

class CtrlEvent {
 public:
  enum class Type {
    Start,
    Stop,
    Restart,
    KeyDown,
    KeyUp
  };

  CtrlEvent(Type type, float value = 0)
      :
      type(type),
      value(value) {

  }
  virtual ~CtrlEvent() {
  }

  Type getType() const {
    return type;
  }

  float getValue() const {
    return value;
  }

 private:
  Type type;
  float value;
};

} /* namespace engine */

#endif /* ENGINE_CTRLEVENT_H_ */
