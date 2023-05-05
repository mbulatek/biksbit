/*
 * Logger.cpp
 *
 *  Created on: Apr 25, 2023
 *      Author: Marek Bulatek <mbulatek@gmail.com>
 */

#include <utils/Logger.h>

std::ostream *Logger::out = new std::ostream(std::cout.rdbuf());
std::mutex Logger::mtx = std::mutex();
Logger::Level Logger::level = Logger::Level::Trace;
