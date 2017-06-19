/*
 * Button.cpp
 *
 *  Created on: 11 giu 2017
 *      Author: paolo
 */

#include "Button.h"

Button::Button(GPIO_TypeDef * port, uint16_t pin) :
        port(port), pin(pin) {
    GPIO_InitTypeDef GPIO_InitStructure;

    // Configure pin in output push/pull mode
    GPIO_InitStructure.GPIO_Pin = pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(port, &GPIO_InitStructure);
    if (port->IDR & pin){
        status=true;
        previous = 0xFF;
    } else {
        status = false;
        previous=0;
    }
}

bool Button::check() {
    bool changed=false;
    previous = (previous << 1) ;
    if (port->IDR & pin)
        previous |= 1;
    if (status && (previous == 0)){
        status = false;
        changed=true;
    } else if(!status && (previous == 0xFF)){
        status = true;
        changed=true;
    }
    return changed;
}

