/*
 * Pipeline.h
 *
 *  Created on: May 4, 2023
 *      Author: Marek Bulatek <mbulatek@gmail.com>
 */

#ifndef ENGINE_PIPELINE_H_
#define ENGINE_PIPELINE_H_

#include <utils/Logger.h>
#include <engine/SignalNode.h>
#include <engine/SignalGenerator.h>
#include <engine/SignalProcessor.h>
#include <engine/SignalMixer.h>
#include <engine/CtrlEvent.h>

#include <memory>
#include <vector>
#include <future>

namespace engine {

class Pipeline {

 public:
  Pipeline();
  virtual ~Pipeline();

  void addGenerator(std::unique_ptr<SignalGenerator> processor) {
    generators.push_back(std::move(processor));
    genQueues.emplace_back(BlockingQueue<std::vector<Params::bufferT>>());
  }

  void addMixer(std::unique_ptr<SignalMixer> processor) {
    mixer = std::move(processor);
  }

  void addProcessor(std::unique_ptr<SignalProcessor> processor) {
    processors.push_back(std::move(processor));
    queues.emplace_back(BlockingQueue<std::vector<Params::bufferT>>());
  }

  void control(const CtrlEvent &event);
  void process();


private:
  std::vector<std::unique_ptr<SignalGenerator>> generators;
  std::vector<std::unique_ptr<SignalProcessor>> processors;
  std::unique_ptr<SignalMixer> mixer;
  std::vector<std::thread> threads;

  std::vector<BlockingQueue<std::vector<Params::bufferT>>> genQueues;
  std::vector<BlockingQueue<std::vector<Params::bufferT>>> queues;
};

} /* namespace engine */

#endif /* ENGINE_PIPELINE_H_ */
