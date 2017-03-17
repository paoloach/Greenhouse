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
    DHT();
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
private:
    static bool readBit() {
        return DATA_PORT->IDR & DATA_PIN;
    }
    static void loadTimer(uint16_t count) {
        TIM2->ARR = count;
        TIM2->CNT = 0;
    }

    static void waitForuS(uint16_t count) {
        TIM2->ARR = count;
        TIM2->CNT = 0;
        TIM2->SR = (uint16_t) ~TIM_FLAG_Update;
        while (!(TIM2->SR & TIM_FLAG_Update))
            ;
    }

    static bool getBit() {
        return TIM2->CNT > 60;
    }
private:

    constexpr static auto DATA_PORT = GPIOA;
    constexpr static auto DATA_PIN = GPIO_Pin_7;
    constexpr static int ClockIdDataPort() {
        return DATA_PORT == GPIOA ? RCC_APB2Periph_GPIOA : DATA_PORT == GPIOB ? RCC_APB2Periph_GPIOB : DATA_PORT == GPIOC ? RCC_APB2Periph_GPIOC : 0;
    }
};

#endif /* DHT_H_ */
