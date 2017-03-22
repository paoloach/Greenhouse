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
#include "HX8367.h"
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



struct TimerVisual {
    TimeElement * second;
    TimeElement * minute;
    TimeElement * hour;

    void update(Timer & timer){
        second->update(timer);
        minute->update(timer);
        hour->update(timer);
        second->print();
        minute->print();
        hour->print();
    }
};

static void initGraphics();
static void setMainPage();
static void setTime(TouchScreen * touchScreen,Timer * timer, TimerVisual * tVis);

int main(int argc, char* argv[]) {
    int32_t prevSec = 0;
    DHT dht;
    Timer timer;
    BlinkLed blinkLed;
    Hour start(7, 0, 0);
    Hour end(21, 0, 0);
    TimerVisual tVis;

    trace_printf("System clock: %u Hz\n", SystemCoreClock);

    timer.start();
    initGraphics();

    TouchScreen touchScreen(graphics->width, graphics->height);

    TempGraph graph((graphics->height - BOTTOM_TEMP_INST) / 2 + BOTTOM_TEMP_INST, graphics->height, timer, &dht, graphics, 50, 350, 0);
    TempGraph graphHumidity(BOTTOM_TEMP_INST, (graphics->height - BOTTOM_TEMP_INST) / 2 + BOTTOM_TEMP_INST, timer, &dht, graphics, 400, 800, 1);

    tVis.second = new TimeElement(graphics, 11 * 16 + HOUR_START_POINT.x, [](char * buffer, const Timer & timer) {sprintf(buffer, "%.02u", timer.getHour().getSeconds());},
            [](Timer & timer) {timer.hour.incSecond();}, [](Timer & timer) {timer.hour.decSecond();});
    tVis.minute = new TimeElement (graphics, 8 * 16 + HOUR_START_POINT.x, [](char * buffer, const Timer & timer) {sprintf(buffer, "%.02u", timer.getHour().getMinutes());},
            [](Timer & timer) {timer.hour.incMinute();}, [](Timer & timer) {timer.hour.decMinute();});
    tVis.hour = new TimeElement(graphics, 5 * 16 + HOUR_START_POINT.x, [](char * buffer, const Timer & timer) {sprintf(buffer, "%.02u", timer.getHour().getHours());}, [](Timer & timer) {timer.hour.incHour();},
            [](Timer & timer) {timer.hour.decHour();});

    setMainPage();
    graph.initGraph();
    graphHumidity.initGraph();
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
            tVis.update(timer);

            prevSec = timer.getHour().seconds;
            graph.update(true);
            graphHumidity.update(true);
        };

        auto point = touchScreen.getPoint();
        GFX::initIO();
        if (point.y < 40 && point.y > 10 && point.x > 0) {
            setTime(&touchScreen, &timer, &tVis);
            setMainPage();
            graph.initGraph();
            graphHumidity.initGraph();
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
        if (ILI9341::ILI9341::checkPresence()) {
            trace_printf("Found ILI9341\n");
            graphics = new ILI9341::ILI9341 { };
            break;
        } else if (ILI9325::checkPresence()) {
            trace_printf("Found ILI925\n");
            graphics = new ILI9325 { };
            break;
        } else if (HX8367::HX8367::checkPresence()) {
            trace_printf("Found HX8367\n");
            graphics = new HX8367::HX8367 { };
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
    graphics->setAddrWindow(0,0,graphics->width-1,graphics->height-1);
    graphics->drawString(HOUR_START_POINT, "Hour:  :  :");
    graphics->setFont(&smallFont);
    graphics->drawString(TEMP_START_POINT, "Temp: 00.0 Humidity:");
}

struct TouchFunc{
    Point center;
    std::function<void()> func;
};



static void setTime(TouchScreen * touchScreen, Timer * timer, TimerVisual * tVis) {
    constexpr uint16_t touchRadious=100;
    TouchFunc  touchFuncs[] = {
            { Point{30,100},[tVis, timer](){tVis->hour->incTime(*timer);}},
            { Point{110,100},[tVis, timer](){tVis->minute->incTime(*timer);}},
            { Point{190,100},[tVis, timer](){tVis->second->incTime(*timer);}},
            { Point{30,200},[tVis, timer](){tVis->hour->decTime(*timer);}},
            { Point{110,200},[tVis, timer](){tVis->minute->decTime(*timer);}},
            { Point{190,200},[tVis, timer](){tVis->second->decTime(*timer);}},
    };


    int32_t prevSec = 0;
    graphics->fillRect(Point(0, 40), graphics->width, graphics->height, BLACK);

    graphics->setFont(&bigFont);
    graphics->setForeground(BLACK);

    graphics->fillCircle(Point(30,100),25, WHITE );
    graphics->fillCircle(Point(110,100),25, WHITE );
    graphics->fillCircle(Point(190,100),25, WHITE );

    graphics->fillCircle(Point(30,200),25, WHITE );
    graphics->fillCircle(Point(110,200),25, WHITE );
    graphics->fillCircle(Point(190,200),25, WHITE );

    graphics->drawString(Point(14, 100), "H+");
    graphics->drawString(Point(94, 100), "M+");
    graphics->drawString(Point(174, 100), "S+");

    graphics->drawString(Point(14, 200), "H-");
    graphics->drawString(Point(94, 200), "M-");
    graphics->drawString(Point(174, 200), "S-");

    while (true) {
        if (prevSec != timer->getHour().seconds) {
            tVis->update(*timer);
            prevSec = timer->getHour().seconds;
        };

        auto point = touchScreen->getPoint();
        GFX::initIO();
        trace_printf("touch: %d, %d\n", point.x, point.y);
        if (point.y < 40 && point.y > 10 && point.x > 0) {
            return;
        }
        for(auto & touchFunc: touchFuncs ){
            if (point.distSquare(touchFunc.center) < touchRadious){
                touchFunc.func();
            }
        }
    }
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
