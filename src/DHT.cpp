/*
 * DHT.cpp
 *
 *  Created on: 28 feb 2017
 *      Author: paolo
 */

#include "DHT.h"
#include <array>
#include "diag/Trace.h"

void DHT::TIM_TimeBaseInit(TIM_TimeBaseInitTypeDef* TIM_TimeBaseInitStruct) {
    uint16_t tmpcr1 = 0;

    tmpcr1 = timer->CR1;

    tmpcr1 &= (uint16_t) (~((uint16_t) (TIM_CR1_DIR | TIM_CR1_CMS)));
    tmpcr1 |= (uint32_t) TIM_TimeBaseInitStruct->TIM_CounterMode;

    tmpcr1 &= (uint16_t) (~((uint16_t) TIM_CR1_CKD));
    tmpcr1 |= (uint32_t) TIM_TimeBaseInitStruct->TIM_ClockDivision;

    timer->CR1 = tmpcr1;
    timer->ARR = TIM_TimeBaseInitStruct->TIM_Period;
    timer->PSC = TIM_TimeBaseInitStruct->TIM_Prescaler;
    timer->EGR = TIM_PSCReloadMode_Immediate;
}

DHT::DHT(GPIO_TypeDef * port, uint16_t pin, TIM_TypeDef * timer) :
        indexLast(0), full(false), status(DHT_START), timer(timer), DATA_PORT(port), DATA_PIN(pin) {
    RCC_APB2PeriphClockCmd(ClockIdDataPort(), ENABLE);
    if (timer == TIM2) {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    } else if (timer == TIM3) {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    } else if (timer == TIM4) {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    }

    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = DATA_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(DATA_PORT, &GPIO_InitStructure);
    DATA_PORT->BSRR = DATA_PIN;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIOA->BSRR = GPIO_Pin_5;
}

std::tuple<int16_t, int16_t> DHT::getMean() {
    int32_t temp = 0;
    int32_t humidity = 0;
    if (full) {
        for (const auto & value : last) {
            temp += std::get<0>(value);
            humidity += std::get<1>(value);
        }
        temp /= last.size();
        humidity /= last.size();
    } else {
        for (uint8_t i = 0; i < indexLast; i++) {
            temp += std::get<0>(last[i]);
            humidity += std::get<1>(last[i]);
        }
        temp /= indexLast;
        humidity /= indexLast;
    }
    return std::make_tuple(temp, humidity);
}

void DHT::exec() {
    TIM_TimeBaseInitTypeDef timerInit;
    GPIO_InitTypeDef GPIO_InitStructure;
    switch (status) {
    case DHT_START:
        GPIOA->BSRR = GPIO_Pin_5;
        GPIO_InitStructure.GPIO_Pin = DATA_PIN;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_Init(DATA_PORT, &GPIO_InitStructure);

        DATA_PORT->BRR = DATA_PIN;

        timerInit.TIM_Prescaler = 36000; // 500uS
        timerInit.TIM_Period = 420; // 210ms
        timerInit.TIM_CounterMode = TIM_CounterMode_Up;
        timerInit.TIM_ClockDivision = TIM_CKD_DIV1;
        TIM_TimeBaseInit(&timerInit);
        timer->CNT = 0;
        timer->SR = (uint16_t) ~TIM_FLAG_Update;
        timer->CR1 |= TIM_CR1_CEN;
        status = DHT_WAIT_END_START;
        break;
    case DHT_WAIT_END_START:
        if (timer->SR & TIM_FLAG_Update) {
            status = DHT_READ;
        }
        break;
    case DHT_READ:
        read();
        GPIO_InitStructure.GPIO_Pin = DATA_PIN;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_Init(DATA_PORT, &GPIO_InitStructure);

        DATA_PORT->BSRR = DATA_PIN;
        status = DHT_START_WAIT_SAMPLE;
        break;
    case DHT_START_WAIT_SAMPLE:
        timerInit.TIM_Prescaler = 36000; // 500uS
        timerInit.TIM_Period = 20000; // 10s
        timerInit.TIM_CounterMode = TIM_CounterMode_Up;
        timerInit.TIM_ClockDivision = TIM_CKD_DIV1;
        TIM_TimeBaseInit(&timerInit);
        timer->CNT = 0;
        timer->SR = (uint16_t) ~TIM_FLAG_Update;
        status = DHT_WAIT_SAMPLE;
        break;
    case DHT_WAIT_SAMPLE:
        if (timer->SR & TIM_FLAG_Update) {
            status = DHT_START;
        }
        break;
    }
}

void DHT::read() {
    TIM_TimeBaseInitTypeDef timerInit;
    GPIO_InitTypeDef GPIO_InitStructure;
    std::array<uint8_t, 5> dataRead;

    GPIO_InitStructure.GPIO_Pin = DATA_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;

    timerInit.TIM_Prescaler = 72; // 1uS
    timerInit.TIM_Period = 1000;
    timerInit.TIM_CounterMode = TIM_CounterMode_Up;

    GPIOA->BRR = GPIO_Pin_5;

    timerInit.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(&timerInit);
    GPIO_Init(DATA_PORT, &GPIO_InitStructure);
    loadTimer(500);
    GPIOA->BSRR = GPIO_Pin_5;
    while (readBit()) {
        if (timer->SR & TIM_FLAG_Update) {
            invalidRead(dataRead);
            return;
        }
    }
    loadTimer(500);
    while (!readBit()) {
        if (timer->SR & TIM_FLAG_Update) {
            invalidRead(dataRead);
            return;
        }
    }
    loadTimer(500);
    while (readBit()) {
        if (timer->SR & TIM_FLAG_Update) {
            invalidRead(dataRead);
            return;
        }
    }
    for (int i = 0; i < 5; i++) {
        uint8_t tmpByte = 0;
        for (uint8_t i = 0; i < 8; i++) {
            while (!readBit())
                ;
            GPIOA->BSRR = GPIO_Pin_5;
            loadTimer(80);
            while (readBit()) {
                if (timer->SR & TIM_FLAG_Update) {
                    invalidRead(dataRead);
                    return;
                }
            }
            tmpByte <<= 1;
            GPIOA->BRR = GPIO_Pin_5;
            tmpByte |= getBit();
        }
        dataRead[i] = tmpByte;
    }
    uint16_t checkSum = dataRead[0] + dataRead[1] + dataRead[2] + dataRead[3];
    if ((uint8_t) checkSum == dataRead[4]) {
        humidity = dataRead[0] * 10 + dataRead[1];
        temperature = dataRead[2] * 10 + dataRead[3];
        last[indexLast] = std::make_tuple(temperature, humidity);
        indexLast++;
        if (indexLast >= last.size()) {
            indexLast = 0;
            full = true;
        }
        update = true;
    } else {
        invalidRead(dataRead);
        trace_printf("Fail checksum\n");
    }

}

