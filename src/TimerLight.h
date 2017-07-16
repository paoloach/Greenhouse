/*
 * TimerLight.h
 *
 *  Created on: 12 giu 2017
 *      Author: paolo
 */

#ifndef TIMERLIGHT_H_
#define TIMERLIGHT_H_
#include "stm32f10x.h"
#include "Pins.h"
#include "Timer.h"

class TimerLight {
public:
    TimerLight(Timer & timer);
    void exec();
private:
    void turnOff() {
        RELE_PORT->BRR = RELE_PIN;
        GPIO_InitTypeDef GPIO_InitStructure;
        GPIO_InitStructure.GPIO_Pin = RELE_N_PIN;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_Init(RELE_N_PORT, &GPIO_InitStructure);

        Timer::callbacks.insert( { currentTime + 20, []() {
            GPIO_InitTypeDef GPIO_InitStructure;
            GPIO_InitStructure.GPIO_Pin = RELE_N_PIN;
            GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
            GPIO_Init(RELE_N_PORT, &GPIO_InitStructure);
            RELE_N_PORT->BRR = RELE_N_PIN;} });

    }

    void turnOn() {
        RELE_N_PORT->BRR = RELE_PIN;
        GPIO_InitTypeDef GPIO_InitStructure;
        GPIO_InitStructure.GPIO_Pin = RELE_PIN;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_Init(RELE_PORT, &GPIO_InitStructure);

        Timer::callbacks.insert( { currentTime + 20, []() {
            GPIO_InitTypeDef GPIO_InitStructure;
            GPIO_InitStructure.GPIO_Pin = RELE_PIN;
            GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
            GPIO_Init(RELE_PORT, &GPIO_InitStructure);
            RELE_PORT->BRR = RELE_PIN;} });
    }

    Timer & timer;
    bool on;
    int32_t currentTime;
};

#endif /* TIMERLIGHT_H_ */
