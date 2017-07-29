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
#include "TimeElement.h"
#include "TempGraph.h"
#include "DHT.h"
#include "ILI9325.h"
#include "ILI9341.h"
#include "HX8367.h"
#include "TimerLight.h"
#include "Hardware.h"
#include "Setting.h"

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

    void update(Timer & timer) {
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
static void updateDHT(DHT * dht);
static void updateUpDown(DHT * currentDHT);

int main(int argc, char* argv[]) {
    int32_t prevSec = 0;
    Timer timer;
    TimerVisual tVis;
    TimerLight timerLight(timer);

    trace_printf("System clock: %u Hz\n", SystemCoreClock);

    Configuration::read();
    timer.start();
    initGraphics();

    TempGraph graph((gfx->height - BOTTOM_TEMP_INST) / 2 + BOTTOM_TEMP_INST, gfx->height, timer, &dht1, 100, 300, 0);
    TempGraph graphHumidity(BOTTOM_TEMP_INST, (gfx->height - BOTTOM_TEMP_INST) / 2 + BOTTOM_TEMP_INST, timer, &dht1, 500, 900, 1);

    tVis.second = new TimeElement(11 * 16 + HOUR_START_POINT.x, [](char * buffer, Timer & timer) mutable {sprintf(buffer, "%.02u", timer.getHour().getSeconds());},
            [](Timer & timer) {timer.hour.incSecond();}, [](Timer & timer) {timer.hour.decSecond();});
    tVis.minute = new TimeElement(8 * 16 + HOUR_START_POINT.x, [](char * buffer, Timer & timer) mutable {sprintf(buffer, "%.02u", timer.getHour().getMinutes());},
            [](Timer & timer) {timer.hour.incMinute();}, [](Timer & timer) {timer.hour.decMinute();});
    tVis.hour = new TimeElement(5 * 16 + HOUR_START_POINT.x, [](char * buffer, Timer & timer) mutable {sprintf(buffer, "%.02u", timer.getHour().getHours());},
            [](Timer & timer) {timer.hour.incHour();}, [](Timer & timer) {timer.hour.decHour();});

    setMainPage();
    graph.initGraph();
    graphHumidity.initGraph();
    DHT * currentDHT = &dht1;
    updateUpDown(currentDHT);

    while (true) {
        if (buttonSetting.check()) {
            if (buttonSetting) {
                timer.stop = true;
                std::unique_ptr<Setting> setting(new Setting(timer.getHour()));
                setting->run();
                timer.stop = false;
                setMainPage();
                graph.initGraph();
                graphHumidity.initGraph();
                updateUpDown(currentDHT);
            }
        }

        if (buttonNext.check()) {
            if (buttonNext) {
                if (currentDHT == &dht1) {
                    currentDHT = &dht2;
                } else {
                    currentDHT = &dht1;
                }
                updateUpDown(currentDHT);
                graph.dht = currentDHT;
                graphHumidity.dht = currentDHT;
                updateDHT(currentDHT);
            }
        }

        dht1.exec();
        dht2.exec();
        if (currentDHT->isUpdate()) {
            updateDHT(currentDHT);
        }
        if (dht1.isUpdate()) {
            heaters.updateUp(std::get<0>(dht1.getIstantaneus()));
        }
        if (dht2.isUpdate()) {
            heaters.updateUp(std::get<0>(dht2.getIstantaneus()));
        }
        timerLight.exec();
        if (prevSec != timer.getHour().seconds) {
            tVis.update(timer);

            prevSec = timer.getHour().seconds;
            graph.update(true);
            graphHumidity.update(true);
        };

    }
}

void updateUpDown(DHT * currentDHT) {
    char c;
    if (currentDHT == &dht2) {
        c = 'd';
    } else {
        c = 'u';
    }
    gfx->setFont(&bigFont);
    gfx->drawChar(220, TEMP_POINT.y, c);
}

void updateDHT(DHT * dht) {
    char buffer[10];
    auto info = dht->getIstantaneus();
    gfx->setFont(&smallFont);
    sprintf(buffer, "%d.%d", std::get<0>(info) / 10, std::get<0>(info) % 10);
    gfx->drawString(TEMP_POINT, buffer);
    sprintf(buffer, "%d.%d", std::get<1>(info) / 10, std::get<1>(info) % 10);
    gfx->drawString(HUMIDITY_POINT, buffer);
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
            gfx = new ILI9341::ILI9341 { };
            break;
        } else if (ILI9325::checkPresence()) {
            trace_printf("Found ILI925\n");
            gfx = new ILI9325 { };
            break;
        } else if (HX8367::HX8367::checkPresence()) {
            trace_printf("Found HX8367\n");
            gfx = new HX8367::HX8367 { };
            break;
        }

    }
}

static void setMainPage() {
    gfx->fillRect(Point(0, 0), gfx->width, gfx->height, BLACK);
    gfx->drawRect(Point(1, 0), gfx->width - 2, 32, WHITE);
    gfx->drawRect(Point(1, 32), gfx->width - 2, 32, WHITE);
    gfx->setForeground(WHITE);
    gfx->setFont(&bigFont);
    gfx->setAddrWindow(0, 0, gfx->width - 1, gfx->height - 1);
    gfx->drawString(HOUR_START_POINT, "Hour:  :  :");
    gfx->setFont(&smallFont);
    gfx->drawString(TEMP_START_POINT, "Temp: 00.0 Humidity:");
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
