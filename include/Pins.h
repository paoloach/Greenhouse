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

 * CS --> B0
 * RESET--> A8
 * RS --> A1
 * WR --> A0
 * RD --> B6
 * D0 --> B7
 * D1 --> B8
 * D2 --> B9
 * D3 --> A11
 * D4 --> B15
 * D5 --> B12
 * D6 --> B13
 * D7 --> B14
 *
 * Button1 --> A9
 * Button2 --> A10
 * Button3 --> A12
 * Button4 --> A13
 * Button5 --> A15
 *
 * Sensor1 --> A7
 * Sensor2 --> A6
 *
 * HeaterUp -> A2
 * HeaterDown -> A3
 *
 * Rele-N -->  B10
 * Rele   -->  B11
 *
 */

constexpr auto HEATER_UP_PORT=GPIOA;
constexpr auto HEATER_UP_PIN=GPIO_Pin_2;
constexpr auto HEATER_DOWN_PORT=GPIOA;
constexpr auto HEATER_DOWN_PIN=GPIO_Pin_3;

constexpr auto BUTTON_1_PORT=GPIOA;
constexpr auto BUTTON_1_PIN=GPIO_Pin_9;
constexpr auto BUTTON_2_PORT=GPIOA;
constexpr auto BUTTON_2_PIN=GPIO_Pin_10;
constexpr auto BUTTON_3_PORT=GPIOA;
constexpr auto BUTTON_3_PIN=GPIO_Pin_12;
constexpr auto BUTTON_4_PORT=GPIOA;
constexpr auto BUTTON_4_PIN=GPIO_Pin_15;
constexpr auto BUTTON_5_PORT=GPIOA;
constexpr auto BUTTON_5_PIN=GPIO_Pin_13;

constexpr auto DTH_1_PORT=GPIOA;
constexpr auto DTH_1_PIN=GPIO_Pin_7;

constexpr auto DTH_2_PORT=GPIOA;
constexpr auto DTH_2_PIN=GPIO_Pin_6;

constexpr auto RELE_PORT=GPIOB;
constexpr auto RELE_PIN=GPIO_Pin_10;
constexpr auto RELE_N_PORT=GPIOB;
constexpr auto RELE_N_PIN=GPIO_Pin_11;


constexpr auto RESET_PORT=GPIOA;
#define RESET_PIN  GPIO_Pin_8
constexpr auto CS_PORT=GPIOB;
#define CS_PIN  GPIO_Pin_0
constexpr auto RS_PORT=GPIOA;
#define RS_PIN  GPIO_Pin_1
constexpr auto WR_PORT=GPIOA;
#define WR_PIN  GPIO_Pin_0
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
