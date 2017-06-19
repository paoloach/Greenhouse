/*
 * Button.h
 *
 *  Created on: 11 giu 2017
 *      Author: paolo
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include <stdint.h>
#include "stm32f10x.h"

class Button {
public:
    Button(GPIO_TypeDef * port, uint16_t pin);
    bool get() const {
        return status;
    }
    operator bool(){
        return status;
    }
    bool check();
private:
    GPIO_TypeDef * port;
    uint16_t pin;
    bool status;
    uint8_t previous;
};

#endif /* BUTTON_H_ */
