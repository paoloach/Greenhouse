/*
 * Pins.h
 *
 *  Created on: 16 gen 2017
 *      Author: paolo
 */

#ifndef PINS_H_
#define PINS_H_

#include "stm32f10x.h"


/**
 *
 * CS --> B0
 * RS --> B1
 * WR --> B5
 * RD --> B6
 * RESET -> A8
 * D0 --> B7
 * D1 --> B8
 * D2 --> B9
 * D3 --> A12
 * D4 --> B15
 * D5 --> B12
 * D6 --> B13
 * D7 --> B14
 */

constexpr auto RESET_PORT=GPIOA;
#define RESET_PIN  GPIO_Pin_8
constexpr auto CS_PORT=GPIOB;
#define CS_PIN  GPIO_Pin_0
constexpr auto RS_PORT=GPIOB;
#define RS_PIN  GPIO_Pin_1
constexpr auto WR_PORT=GPIOB;
#define WR_PIN  GPIO_Pin_5
constexpr auto RD_PORT=GPIOB;
#define RD_PIN  GPIO_Pin_6
constexpr auto D0_PORT=GPIOB;
#define D0_PIN  GPIO_Pin_7
constexpr auto D1_PORT=GPIOB;
#define D1_PIN  GPIO_Pin_8
constexpr auto D2_PORT=GPIOB;
#define D2_PIN  GPIO_Pin_9
constexpr auto D3_PORT=GPIOA;
#define D3_PIN  GPIO_Pin_11
constexpr auto D4_PORT=GPIOB;
#define D4_PIN  GPIO_Pin_15
constexpr auto D5_PORT=GPIOB;
#define D5_PIN  GPIO_Pin_12
constexpr auto D6_PORT=GPIOB;
#define D6_PIN  GPIO_Pin_13
constexpr auto D7_PORT=GPIOB;
#define D7_PIN  GPIO_Pin_14




#endif /* PINS_H_ */
