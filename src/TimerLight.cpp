/*
 * TimerLight.cpp
 *
 *  Created on: 12 giu 2017
 *      Author: paolo
 */

#include "diag/Trace.h"
#include "TimerLight.h"
#include "Configuration.h"

TimerLight::TimerLight(Timer & timer) :
        timer(timer), on(false), currentTime(timer.hour.seconds) {
    // Enable GPIO Peripheral clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;

    // Configure pin in output push/pull mode
    GPIO_InitStructure.GPIO_Pin = RELE_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(RELE_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = RELE_N_PIN;
    GPIO_Init(RELE_N_PORT, &GPIO_InitStructure);

    RELE_N_PORT->BRR = RELE_N_PIN;
    RELE_PORT->BRR = RELE_PIN;
}

void TimerLight::exec() {
    if (currentTime != timer.hour.seconds) {
        currentTime = timer.hour.seconds;
        if (timer.hour > Configuration::onHour && Configuration::offHour > timer.hour) {
            if (!on) {
                turnOn();
                trace_puts("ON\n");
                on = true;
            }

        } else {
            if (on) {
                trace_puts("OFF\n");
                turnOff();
                on = false;
            }
        }
    }
}

