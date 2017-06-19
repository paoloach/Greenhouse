/*
 * DHT.h
 *
 *  Created on: 28 feb 2017
 *      Author: paolo
 */

#ifndef DHT_H_
#define DHT_H_

#include <tuple>
#include "stm32f10x.h"

enum DHTStatus {
    DHT_START_WAIT_SAMPLE, DHT_WAIT_SAMPLE, DHT_START, DHT_WAIT_END_START, DHT_READ
};

class DHT {
public:
    DHT(GPIO_TypeDef * port, uint16_t pin, TIM_TypeDef * timer);
public:
    void exec();
    std::tuple<int16_t, int16_t> getIstantaneus() {
        update = false;
        return std::make_tuple(temperature, humidity);
    }

    std::tuple<int16_t, int16_t> getMean();
    bool isUpdate() {
        return update;
    }

private:
    bool update;
    int16_t temperature;
    int16_t humidity;
    std::array<std::tuple<int16_t, int16_t>, 90> last;
    uint8_t indexLast;
    bool full;
    DHTStatus status;
    void read();
    void invalidRead(std::array<uint8_t, 5> & dataRead) {
        auto iter = dataRead.begin();
        *iter = 0xFF;
        iter++;
        *iter = 0xFF;
        iter++;
        *iter = 0xFF;
        iter++;
        *iter = 0xFF;
        iter++;
        *iter = 0xFF;
    }
private:
    void TIM_TimeBaseInit(TIM_TimeBaseInitTypeDef* TIM_TimeBaseInitStruct);
    bool readBit() {
        return DATA_PORT->IDR & DATA_PIN;
    }
    void loadTimer(uint16_t count) {
        timer->ARR = count;
        timer->CNT = 0;
        timer->SR = (uint16_t) ~TIM_FLAG_Update;
    }

    void waitForuS(uint16_t count) {
        timer->ARR = count;
        timer->CNT = 0;
        timer->SR = (uint16_t) ~TIM_FLAG_Update;
        while (!(timer->SR & TIM_FLAG_Update))
            ;
    }

    bool getBit() {
        return timer->CNT > 60;
    }
private:

    TIM_TypeDef * timer;
    GPIO_TypeDef * DATA_PORT;
    uint16_t DATA_PIN;
    int ClockIdDataPort() {
        return DATA_PORT == GPIOA ? RCC_APB2Periph_GPIOA : DATA_PORT == GPIOB ? RCC_APB2Periph_GPIOB : DATA_PORT == GPIOC ? RCC_APB2Periph_GPIOC : 0;
    }
};

#endif /* DHT_H_ */
