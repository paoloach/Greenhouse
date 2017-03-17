/*
 * DHT.cpp
 *
 *  Created on: 28 feb 2017
 *      Author: paolo
 */

#include "DHT.h"
#include <array>
#include "diag/Trace.h"

void TIM_TimeBaseInit(TIM_TimeBaseInitTypeDef* TIM_TimeBaseInitStruct) {
    uint16_t tmpcr1 = 0;

    tmpcr1 = TIM2->CR1;

    tmpcr1 &= (uint16_t) (~((uint16_t) (TIM_CR1_DIR | TIM_CR1_CMS)));
    tmpcr1 |= (uint32_t) TIM_TimeBaseInitStruct->TIM_CounterMode;

    tmpcr1 &= (uint16_t) (~((uint16_t) TIM_CR1_CKD));
    tmpcr1 |= (uint32_t) TIM_TimeBaseInitStruct->TIM_ClockDivision;

    TIM2->CR1 = tmpcr1;
    TIM2->ARR = TIM_TimeBaseInitStruct->TIM_Period;
    TIM2->PSC = TIM_TimeBaseInitStruct->TIM_Prescaler;
    TIM2->EGR = TIM_PSCReloadMode_Immediate;
}

DHT::DHT() :
        indexLast(0), full(false), status(DHT_START) {
    RCC_APB2PeriphClockCmd(ClockIdDataPort(), ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = DATA_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(DATA_PORT, &GPIO_InitStructure);
    DATA_PORT->BSRR = DATA_PIN;
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
        GPIO_InitStructure.GPIO_Pin = DATA_PIN;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_Init(DATA_PORT, &GPIO_InitStructure);

        DATA_PORT->BRR = DATA_PIN;

        timerInit.TIM_Prescaler = 36000; // 500uS
        timerInit.TIM_Period = 420; // 210ms
        timerInit.TIM_CounterMode = TIM_CounterMode_Up;
        timerInit.TIM_ClockDivision = TIM_CKD_DIV1;
        TIM_TimeBaseInit( &timerInit);
        TIM2->CNT = 0;
        TIM2->SR = (uint16_t) ~TIM_FLAG_Update;
        TIM2->CR1 |= TIM_CR1_CEN;
        status = DHT_WAIT_END_START;
        break;
    case DHT_WAIT_END_START:
        if (TIM2->SR & TIM_FLAG_Update) {
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
        TIM_TimeBaseInit( &timerInit);
        TIM2->CNT = 0;
        TIM2->SR = (uint16_t) ~TIM_FLAG_Update;
        status = DHT_WAIT_SAMPLE;
        break;
    case DHT_WAIT_SAMPLE:
        if (TIM2->SR & TIM_FLAG_Update) {
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
    timerInit.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit( &timerInit);
    GPIO_Init(DATA_PORT, &GPIO_InitStructure);
    waitForuS(30);
    while (readBit())
        ;
    while (!readBit())
        ;
    while (readBit())
        ;
    for (int i = 0; i < 5; i++) {
        uint8_t tmpByte = 0;
        for (uint8_t i = 0; i < 8; i++) {
            while (!readBit())
                ;
            loadTimer(80);
            while (readBit())
                ;
            tmpByte <<= 1;
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
        trace_printf("Fail checksum\n");
    }

}

