/*
 * TouchScreen.h
 *
 *  Created on: 02 mar 2017
 *      Author: paolo
 */

#ifndef TOUCHSCREEN_H_
#define TOUCHSCREEN_H_

#include "Pins.h"
#include "GFX.h"

class TouchScreen {
public:
    TouchScreen(uint16_t width, uint16_t height);
    Point getPoint();
    Point getRaw();
    uint16_t    width;
    uint16_t    height;
    uint16_t    left;
    uint16_t    right;
    uint16_t    top;
    uint16_t    bottom;
private:
    uint16_t readADC1(uint8_t channel);
    constexpr static int NUM_SAMPLES=2;
};

#endif /* TOUCHSCREEN_H_ */
