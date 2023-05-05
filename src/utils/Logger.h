/*
 * Logger.h
 *
 *  Created on: Apr 21, 2023
 *      Author: Marek Bulatek <mbulatek@gmail.com>
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <mutex>

class Logger {

 public:

  enum class Level {
    Trace,
    Debug,
    Info,
    Warning,
    Error
  };

  static const inline std::map<Level, std::string> LEVEL_PREFIXES = { {
      Level::Trace, "T" }, { Level::Info, "I" }, { Level::Warning, "W" }, {
      Level::Error, "E" } };

  Logger(const std::string &func)
      :
      func(func) {
    std::lock_guard<std::mutex> lock(mtx);
    (*out) << "+++" << func << std::endl;
  }
  virtual ~Logger() {
    std::lock_guard<std::mutex> lock(mtx);
    (*out) << "---" << func << std::endl;
  }

  static void setLevel(Level level) {
    Logger::level = level;
  }

  static void setFile(const std::string &filename) {
    delete out;
    out = new std::ofstream(filename, std::ofstream::trunc);
  }

  virtual void write(const std::string &message) {
    write(Level::Info, message);
  }

  virtual void write(Level level, const std::string &message) {
    if (level < this->level)
      return;
    //std::cout << LEVEL_PREFIXES.at(level) << " " << "[" << func << "] " << message << std::endl;
    std::lock_guard<std::mutex> lock(mtx);
    (*out) << "[" << func << "] " << message << std::endl;
  }

  virtual void writeRaw(Level level, const std::string &message) {
    if (level < this->level)
      return;
    std::lock_guard<std::mutex> lock(mtx);
    (*out) << message;
  }

 private:

  std::string func;
  static std::ostream *out;
  static std::mutex mtx;
  static Level level;
};

#endif /* LOGGER_H_ */
