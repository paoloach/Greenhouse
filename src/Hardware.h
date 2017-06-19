/*
 * Hardware.h
 *
 *  Created on: 15 giu 2017
 *      Author: paolo
 */

#ifndef HARDWARE_H_
#define HARDWARE_H_

#include "Pins.h"
#include "Button.h"
#include "DHT.h"
#include "GFX.h"

extern DHT dht1;
extern DHT dht2;
extern GFX * gfx;
extern Button buttonSetting;
extern Button buttonInc;
extern Button buttonDec;
extern Button buttonNext;
extern Button button5;


#endif /* HARDWARE_H_ */
