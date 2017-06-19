/*
 * SettingHour.cpp
 *
 *  Created on: 15 giu 2017
 *      Author: paolo
 */

#include "SettingHour.h"

void SettingHour::print() {
    setBuffer();
    gfx->setFont(&smallFont);
    if (selected) {
        gfx->setForeground(WHITE);
        gfx->setBackground(BLACK);
    } else {
        gfx->setForeground(BLACK);
        gfx->setBackground(WHITE);

    }
    gfx->drawString(point, buffer);
}
