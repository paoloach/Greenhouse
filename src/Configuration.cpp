/*
 * Configuration.cpp
 *
 *  Created on: 21 giu 2017
 *      Author: paolo
 */

#include <stm32f10x_flash.h>
#include "Configuration.h"

__attribute__((section(".config.data")))   uint16_t Configuration::data[50] = {20,20,20,7,0,21,0};
uint16_t Configuration::minuteFromPoint;
uint16_t Configuration::heaterUp;
uint16_t Configuration::heaterDown;
Hour Configuration::onHour;
Hour Configuration::offHour;

Configuration::Configuration() {
    read();
}

void Configuration::save() {
    bool modified = false;
    if (readMinuteFromPoint() != minuteFromPoint)
        modified = true;
    if (readHeaterUp() != heaterUp)
        modified = true;
    if (readHeaterDown() != heaterDown)
        modified = true;
    if (readOnHour() != onHour.getHours())
        modified = true;
    if (readOffHour() != offHour.getHours())
        modified = true;
    if (readOnMinute() != onHour.getMinutes())
        modified = true;
    if (readOffMinute() != offHour.getMinutes())
        modified = true;
//    if (modified) {
//        FLASH_Unlock();
//        FLASH_ErasePage((uint32_t) data);
//        FLASH_ProgramHalfWord((uint32_t) data[0], minuteFromPoint);
//        FLASH_ProgramHalfWord((uint32_t) data[1], heaterUp);
//        FLASH_ProgramHalfWord((uint32_t) data[2], heaterDown);
//        FLASH_ProgramHalfWord((uint32_t) data[3], onHour.getHours());
//        FLASH_ProgramHalfWord((uint32_t) data[4], onHour.getMinutes());
//        FLASH_ProgramHalfWord((uint32_t) data[5], offHour.getHours());
//        FLASH_ProgramHalfWord((uint32_t) data[6], offHour.getMinutes());
//        FLASH_Lock();
//    }
}
void Configuration::read() {
    minuteFromPoint = readMinuteFromPoint();
    heaterUp = readHeaterUp();
    heaterDown = readHeaterDown();
    onHour = Hour(readOnHour(), readOnMinute(), 0);
    offHour = Hour(readOffHour(), readOffMinute(), 0);
}

