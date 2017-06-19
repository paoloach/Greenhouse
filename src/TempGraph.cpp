/*
 * TempGraph.cpp
 *
 *  Created on: 31 ott 2016
 *      Author: paolo
 */

#include <string.h>
#include <stdio.h>
#include "diag/Trace.h"
#include "TempGraph.h"
#include "Hardware.h"

TempGraph::TempGraph(uint16_t top, uint16_t bottom, Timer & timer, DHT * dht, uint16_t min, uint16_t max, uint8_t index) :
        dht(dht), minuteFromPoint(20), top(top), bottom(bottom), timer(timer), next(timer.getHour()), min(min), max(max), right((max + min) / 2), index(index) {
    for (int i = 0; i < dataSize; i++) {
        data[i] = std::make_tuple(1000, 1000);
    }

}

void TempGraph::update(bool paint) {
    if (timer.getHour() > next) {
        //auto value = dht->getMean();
        auto value = dht->getIstantaneus();
        next = timer.getHour();
        next.incMinute(minuteFromPoint);
        if (paint) {
            paintGraph(true);
        }
        memmove(data + 1, data, 99 * sizeof(std::tuple<uint16_t, uint16_t>));

        data[0] = value;
        if (paint) {
            paintGraph(false);
        }
    }
}

void TempGraph::initGraph() {
    char buffer[10];
    uint16_t height = bottom - top;
    gfx->fillRect(Point(0, top), gfx->width, height, WHITE);
    gfx->setForeground(BLACK);
    gfx->setBackground(WHITE);
    gfx->setFont(&smallFont);
    uint16_t step = height / 6;
    uint16_t stepVal = (max - min) / 6;
    for (uint8_t i = 1; i < 6; i++) {
        sprintf(buffer, "%2u", (min + i * stepVal) / 10);
        gfx->drawString(0, bottom - i * step, buffer);
    }
}

void TempGraph::paintGraph(bool clear) {
    uint32_t height = bottom - top;
    uint16_t step = height / 6;

    uint16_t temp;
    Point prevTemp;
    Point p(39, 0);
    for (int i = 0; i < dataSize; i++) {
        if (index == 0)
            temp = std::get<0>(data[i]);
        else
            temp = std::get<1>(data[i]);
        if (temp < min)
            temp = min;
        if (temp > max)
            temp = max;
        p.y = bottom - ((temp - min) * height) / (max - min);
        int32_t blue;
        int32_t red;
        uint32_t green;
        if (clear == false) {
            blue = (256 * (right - temp)) / right;
            if (blue < 0)
                blue = 0;
            if (blue > 255)
                blue = 255;
            red = (256 * (temp - right)) / right;
            if (red < 0)
                red = 0;
            if (red > 255)
                red = 255;
            green = 0;
        } else {
            blue = 255;
            green = 255;
            red = 255;
        }
        if (i > 0) {
            gfx->drawLine(prevTemp, p, Color6Bit(red, green, blue));
        }
        prevTemp = p;
        p.x += 2;
    }

    for (uint8_t i = 1; i <= 5; i++) {
        gfx->drawFastHLine(Point(38, bottom - i * step), gfx->width - 38, GRAY);
    }

}

