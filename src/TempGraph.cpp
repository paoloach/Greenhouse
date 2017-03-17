/*
 * TempGraph.cpp
 *
 *  Created on: 31 ott 2016
 *      Author: paolo
 */

#include <string.h>

#include "TempGraph.h"

TempGraph::TempGraph(uint16_t top, uint16_t bottom, const Timer & timer, DHT * dht, GFX * gfx) :
        top(top), bottom(bottom), timer(timer), next(timer.getHour()), dht(dht), gfx(gfx) {
    for (int i = 0; i < 100; i++) {
        data[i] = std::make_tuple(1000, 1000);
    }
}

void TempGraph::update(bool paint) {
    if (timer.getHour() > next) {
        auto value = dht->getMean();
        next = timer.getHour();
        //next.incMinute(15);
        next.incSecond();
        next.incSecond();
        next.incSecond();
        next.incSecond();
        next.incSecond();
        next.incSecond();

        memmove(data + 1, data, 99 * sizeof(std::tuple<uint16_t, uint16_t>));
        data[0] = value;
        if (paint) {
            paintGraph();
        }
    }
}

void TempGraph::paintGraph() {
    uint32_t height = bottom - top;
    constexpr uint16_t minTemp = 50;
    constexpr uint16_t maxTemp = 350;

    gfx->fillRect(Point(0, top), gfx->width, height, WHITE);
    gfx->setForeground(BLACK);
    gfx->setBackground(WHITE);
    gfx->setFont(&smallFont);
    uint16_t step = height / 6;
    gfx->drawString(0, bottom - step, "10");
    gfx->drawString(0, bottom - 2 * step, "15");
    gfx->drawString(0, bottom - 3 * step, "20");
    gfx->drawString(0, bottom - 4 * step, "25");
    gfx->drawString(0, bottom - 5 * step, "30");
    gfx->drawFastVLine(Point(38,top), height, DARKGRAY);

    uint16_t temp;
    Point prevTemp;
    Point p(39,0);
    for (int i = 0; i < dateSize; i++) {
        temp = std::get<0>(data[i]);
        if (temp < minTemp)
            temp = minTemp;
        if (temp > maxTemp)
            temp = maxTemp;
        p.y = bottom - ((temp - minTemp) * height) / (maxTemp - minTemp);
        int32_t blue = 255 * ((250 - temp) / 150.0);
        if (blue < 0)
            blue = 0;
        if (blue > 255)
            blue = 255;
        int32_t red;
        if (temp > 250)
            red = 255 * (1 - (350 - temp) / 100.0);
        else
            red = 0;
        if (red < 0)
            red = 0;
        if (red > 255)
            red = 255;
        uint32_t green;
        if (temp <= 250) {
            green = 255 - blue;
        } else {
            green = 255 - red;
        }
        if (i > 0) {
            gfx->drawLine(prevTemp, p, Color6Bit(red, green, blue));
        }
        prevTemp = p;
        p.x +=2;
    }

    for(uint8_t i =1 ;i <= 5; i++){
        gfx->drawFastHLine(Point(38,bottom-i*step), gfx->width-38, GRAY);
    }


}

