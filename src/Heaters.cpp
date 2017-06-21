/*
 * Heaters.cpp
 *
 *  Created on: 21 giu 2017
 *      Author: paolo
 */


#include "stm32f10x_gpio.h"
#include "Configuration.h"
#include "Pins.h"
#include "Heaters.h"

Heaters::Heaters() {
    GPIO_InitTypeDef GPIO_InitStructure;

    // Configure pin in output push/pull mode
    GPIO_InitStructure.GPIO_Pin = HEATER_UP_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(HEATER_UP_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = HEATER_DOWN_PIN;
    GPIO_Init(HEATER_DOWN_PORT, &GPIO_InitStructure);

    HEATER_UP_PORT->BRR = HEATER_UP_PIN;
    HEATER_DOWN_PORT->BRR = HEATER_DOWN_PIN;
}

void Heaters::updateUp(int16_t temp){
    if (temp < Configuration::heaterUp){
        HEATER_UP_PORT->BSRR=HEATER_UP_PIN;
    } else {
        HEATER_UP_PORT->BRR=HEATER_UP_PIN;
    }
}
void Heaters::updateDown(int16_t temp){
    if (temp < Configuration::heaterDown){
        HEATER_DOWN_PORT->BSRR=HEATER_DOWN_PIN;
    } else {
        HEATER_DOWN_PORT->BRR=HEATER_DOWN_PIN;
    }
}

