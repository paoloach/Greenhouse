/*
 * TouchScreen.cpp
 *
 *  Created on: 02 mar 2017
 *      Author: paolo
 */

#include <memory>
#include "TouchScreen.h"
#include "diag/Trace.h"
TouchScreen::TouchScreen(uint16_t width, uint16_t height) :
        width(width), height(height) {
    left = 720;
    right = 3500;
    top = 4000;
    bottom = 650;
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA, ENABLE);
    ADC_InitTypeDef ADC_InitStructure;
    ADC_DeInit(ADC1);

    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    //We will convert multiple channels
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    //select continuous conversion mode
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; //!
    //select no external triggering
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    //right 12-bit data alignment in ADC data register
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    //8 channels conversion
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    //load structure values to control and status registers
    ADC_Init(ADC1, &ADC_InitStructure);

    ADC_Cmd(ADC1, ENABLE);
    ADC_ResetCalibration(ADC1);
    //Check the end of ADC1 reset calibration register
    while (ADC_GetResetCalibrationStatus(ADC1))
        ;
    //Start ADC1 calibration
    ADC_StartCalibration(ADC1);
    //Check the end of ADC1 calibration
    while (ADC_GetCalibrationStatus(ADC1))
        ;

}

uint16_t TouchScreen::readADC1(uint8_t channel){
  ADC_RegularChannelConfig(ADC1, channel, 1, ADC_SampleTime_1Cycles5);
  // Start the conversion
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
  // Wait until conversion completion
  while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
  // Get the conversion value
  return ADC_GetConversionValue(ADC1);
}

Point TouchScreen::getPoint() {
    Point point(std::move(getRaw()));
    point.x = ((uint32_t) width * (point.x - left)) / (right - left);
    point.y = height -((uint32_t) height * (point.y - bottom)) / (top-bottom);
    return point;
}

Point TouchScreen::getRaw() {
    Point point;

    CS_PORT->BSRR = CS_PIN;
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_InitStructure.GPIO_Pin = YP_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(YP_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = YM_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(YM_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = XP_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(XP_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = XM_PIN;
    GPIO_Init(XM_PORT, &GPIO_InitStructure);

    XP_PORT->BRR = XP_PIN;
    XM_PORT->BSRR = XM_PIN;
    ADC_RegularChannelConfig(ADC1, 0, 1, ADC_SampleTime_41Cycles5);

    uint16_t sample = 0;

    sample += readADC1(YP_ADC_PORT);
    sample += readADC1(YP_ADC_PORT);
    point.x = sample / 2;

    GPIO_InitStructure.GPIO_Pin = YP_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(YP_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = YM_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(YM_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = XP_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(XP_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = XM_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(XM_PORT, &GPIO_InitStructure);

    YP_PORT->BSRR = YP_PIN;
    YM_PORT->BRR = YM_PIN;

    sample = 0;
    sample += readADC1(XM_ADC_PORT);
    sample += readADC1(XM_ADC_PORT);
    point.y = sample / 2;

    return point;
}
