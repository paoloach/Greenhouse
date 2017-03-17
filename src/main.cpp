//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "diag/Trace.h"

#include "Timer.h"
#include "BlinkLed.h"
#include "TimeElement.h"
#include "TempGraph.h"
#include "DHT.h"
#include "ILI9325.h"
#include "ILI9341.h"
#include "TouchScreen.h"

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

enum class ModifyType {
    None, Seconds = 0, Minuts = 1, Hours = 2, Last = 3
};

GFX * graphics;

constexpr uint8_t WIDTH_FONT = 8;
constexpr Point HOUR_START_POINT { 10, 8 };
constexpr Point TEMP_START_POINT { 10, 40 };
constexpr Point TEMP_POINT { TEMP_START_POINT.x + 6 * WIDTH_FONT, 40 };
constexpr Point HUMIDITY_POINT { TEMP_START_POINT.x + 21 * WIDTH_FONT, 40 };
constexpr uint16_t BOTTOM_TEMP_INST = TEMP_POINT.y + 12 + 8;

static void initGraphics();
static void setMainPage();
static void setTime(TouchScreen * touchScreen);

int main(int argc, char* argv[]) {
    int32_t prevSec = 0;
    DHT dht;
    Timer timer;
    BlinkLed blinkLed;
    Hour start(7, 0, 0);
    Hour end(21, 0, 0);


    trace_printf("System clock: %u Hz\n", SystemCoreClock);

    timer.start();
    initGraphics();

    TouchScreen touchScreen(graphics->width, graphics->height);

    TempGraph graph((graphics->height - BOTTOM_TEMP_INST) / 2, graphics->height, timer, &dht, graphics);
    TimeElement noneElement { };
    TimeElement second(graphics, 11 * 16 + HOUR_START_POINT.x, [](char * buffer, const Timer & timer) {sprintf(buffer, "%.02u", timer.getHour().getSeconds());},
            [](Timer & timer) {timer.hour.incSecond();}, [](Timer & timer) {timer.hour.decSecond();});
    TimeElement minute(graphics, 8 * 16 + HOUR_START_POINT.x, [](char * buffer, const Timer & timer) {sprintf(buffer, "%.02u", timer.getHour().getMinutes());},
            [](Timer & timer) {timer.hour.incMinute();}, [](Timer & timer) {timer.hour.decMinute();});
    TimeElement hour(graphics, 5 * 16 + HOUR_START_POINT.x, [](char * buffer, const Timer & timer) {sprintf(buffer, "%.02u", timer.getHour().getHours());}, [](Timer & timer) {timer.hour.incHour();},
            [](Timer & timer) {timer.hour.decHour();});

    noneElement.next = &second;
    second.next = &minute;
    minute.next = &hour;
    hour.next = &noneElement;

    setMainPage();
    while (1) {
        dht.exec();
        if (dht.isUpdate()) {
            char buffer[10];
            auto info = dht.getIstantaneus();
            graphics->setFont(&smallFont);
            sprintf(buffer, "%d.%d", std::get<0>(info) / 10, std::get<0>(info) % 10);
            graphics->drawString(TEMP_POINT, buffer);
            sprintf(buffer, "%d.%d", std::get<1>(info) / 10, std::get<1>(info) % 10);
            graphics->drawString(HUMIDITY_POINT, buffer);
        }

        if (prevSec != timer.getHour().seconds) {
            if (timer.getHour() > start && end > timer.getHour()) {
                blinkLed.turnOff();
            } else {
                blinkLed.turnOn();
            }

            second.update(timer);
            minute.update(timer);
            hour.update(timer);
            second.print();
            minute.print();
            hour.print();
            prevSec = timer.getHour().seconds;
            graph.update(true);
        };

        auto point = touchScreen.getPoint();
        GFX::initIO();
      //  trace_printf("touch: %d, %d\n", point.x, point.y);
       if (point.y < 40 && point.y > 10) {
            setTime(&touchScreen);
            setMainPage();
        }
    }
}

void initGraphics() {
    GFX::initIO();

    activeCS();
    resetOn();
    Timer::sleep(5);
    resetOff();
    Timer::sleep(150);

    while (true) {
        trace_puts("Searching graphics display\n");
        if (ILI9341::ILI9341::checkPresence()) {
            trace_printf("Found ILI9341\n");
            graphics = new ILI9341::ILI9341 { };
            break;
        }
        if (ILI9325::checkPresence()) {
            trace_printf("Found ILI925\n");
            graphics = new ILI9325 { };
            break;
        }

    }
}

static void setMainPage() {

    graphics->fillRect(Point(0, 0), graphics->width, graphics->height, BLACK);
    graphics->drawRect(Point(1, 0), graphics->width - 2, 32, WHITE);
    graphics->drawRect(Point(1, 32), graphics->width - 2, 32, WHITE);
    graphics->setForeground(WHITE);
    graphics->setFont(&bigFont);
    graphics->drawString(HOUR_START_POINT, "Hour:  :  :");
    graphics->setFont(&smallFont);
    graphics->drawString(TEMP_START_POINT, "Temp: 00.0 Humidity:");
}

static void setTime(TouchScreen * touchScreen) {
    graphics->fillRect(Point(0, 40), graphics->width, graphics->height, BLACK);

    graphics->setFont(&bigFont);
    graphics->setForeground(WHITE);
    graphics->drawChar(Point(20, 100), 'H', 5);
    graphics->drawChar(Point(100, 100), 'M', 5);
    graphics->drawChar(Point(180, 100), 'S', 5);
    graphics->drawChar(Point(4, 200), '+', 2);
    graphics->drawChar(Point(44,200), '-',2);
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
