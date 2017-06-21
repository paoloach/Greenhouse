/*
 * Configuration.h
 *
 *  Created on: 21 giu 2017
 *      Author: paolo
 */

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <stdint.h>
#include "Hour.h"

class Configuration {
public:
    Configuration();
    static void save();
    static void read();
public:
    static uint16_t minuteFromPoint;
    static uint16_t heaterUp;
    static uint16_t heaterDown;
    static Hour onHour;
    static Hour offHour;
    static uint16_t data[50];
private:
    static uint16_t readMinuteFromPoint() {
        return data[0];
    }
    static uint16_t readHeaterUp() {
        return data[1];
    }
    static uint16_t readHeaterDown() {
        return data[2];
    }
    static uint16_t readOnHour() {
        return data[3];
    }
    static uint16_t readOnMinute() {
        return data[4];
    }
    static uint16_t readOffHour() {
        return data[5];
    }
    static uint16_t readOffMinute() {
        return data[6];
    }
};

#endif /* CONFIGURATION_H_ */
