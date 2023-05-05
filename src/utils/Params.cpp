/*
 * Params.cpp
 *
 *  Created on: Apr 20, 2023
 *      Author: Marek Bulatek <mbulatek@gmail.com>
 */

#include <utils/Params.h>

namespace engine {

//uint32_t Params::sampleRate = 22050;
uint32_t Params::sampleRate = 44100;
uint32_t Params::sampleDepth = 32768;
uint16_t Params::bufferSize = 256;
uint16_t Params::bufferThreshold = Params::bufferSize * 9 / 10;
uint16_t Params::bufferMinFulfill = Params::bufferSize / 10;

Params::Oscillator Params::oscillator = Params::Oscillator();
uint16_t Params::Oscillator::bufferSize = 2048;

Params::Params() {
}

Params::~Params() {
}

} /* namespace engine */
