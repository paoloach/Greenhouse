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
        RELE_PORT->BRR = GPIO_Pin_10;
        RELE_N_PORT->BSRR = RELE_N_PIN;
        Timer::callbacks.insert({currentTime+2, [](){RELE_N_PORT->BRR = RELE_N_PIN;}});
    }

    void turnOn() {
        RELE_N_PORT->BRR = RELE_PIN;
        RELE_PORT->BSRR = RELE_PIN;
        Timer::callbacks.insert({currentTime+2, [](){RELE_PORT->BRR = RELE_PIN;}});
    }

    Timer & timer;
    bool on;
    int32_t currentTime;
};

#endif /* TIMERLIGHT_H_ */
