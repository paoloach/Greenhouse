/*
 * Hardware.cpp
 *
 *  Created on: 15 giu 2017
 *      Author: paolo
 */
#include "Hardware.h"

DHT dht1(DTH_1_PORT, DTH_1_PIN, TIM2);
DHT dht2(DTH_2_PORT, DTH_2_PIN, TIM3);
GFX * gfx;

Button buttonSetting(BUTTON_1_PORT, BUTTON_1_PIN);
Button buttonInc(BUTTON_2_PORT, BUTTON_2_PIN);
Button buttonDec(BUTTON_3_PORT, BUTTON_3_PIN);
Button buttonNext(BUTTON_4_PORT, BUTTON_4_PIN);
Button button5(BUTTON_5_PORT, BUTTON_5_PIN);



