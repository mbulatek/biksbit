/*
 * utils/BlockingQueue.h
 *
 *  Created on: May 4, 2023
 *      Author: Marek Bulatek <mbulatek@gmail.com>
 */

#ifndef ENGINE_UTILS_BLOCKINGQUEUE_H_
#define ENGINE_UTILS_BLOCKINGQUEUE_H_

#include <utils/Logger.h>

#include <queue>
#include <mutex>
#include <condition_variable>

namespace engine {

template<typename T>
class BlockingQueue {
 public:

  BlockingQueue()
      :
      max_size(10),
      flushed(false) {
  }
  BlockingQueue(BlockingQueue<T>&&)
      :
      max_size(10),
      flushed(false) {
  }

  void push(const T &item) {
    std::unique_lock<std::mutex> lock(mutex);
    if (queue.size() >= max_size) {
      cond.wait(lock, [this] {
        return queue.size() < max_size;
      });
      if (flushed) {
        flushed = false;
        return;
      }
    }
    queue.push(item);
    lock.unlock();
    cond.notify_one();
  }

  T pop() {
    std::unique_lock<std::mutex> lock(mutex);
    cond.wait(lock, [this] {
      return !queue.empty() || flushed;
    });
    if (flushed) {
      flushed = false;
      throw(std::runtime_error("queue flushed"));
    }
    T item = queue.front();
    queue.pop();
    if (queue.size() >= max_size - 1) {
      lock.unlock();
      cond.notify_one();
    }
    return item;
  }

  void flush() {
    Logger l(__PRETTY_FUNCTION__);
    std::unique_lock<std::mutex> lock(mutex);
    while (!queue.empty())
      queue.pop();
    flushed = true;
    lock.unlock();
    cond.notify_all();
  }

 private:
  std::queue<T> queue;
  std::mutex mutex;
  std::condition_variable cond;
  int max_size;
  bool flushed;
};
} /* namespace engine */

#endif /* ENGINE_UTILS_BLOCKINGQUEUE_H_ */
