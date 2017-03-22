/*
 * TimeElement.h
 *
 *  Created on: 13 ott 2016
 *      Author: paolo
 */

#ifndef TIMEELEMENT_H_
#define TIMEELEMENT_H_

#include <functional>
#include "Timer.h"
#include "GFX.h"

class TimeElement {
public:
    TimeElement() :
            gfx(nullptr),xPos(-1), func(nullptr), inc(nullptr), dec(nullptr) {
    }
    ;
    TimeElement(GFX * gfx, int xPos, std::function<void(char * str, const Timer & timer)> func, std::function<void(Timer & timer)> inc, std::function<void(Timer & timer)> dec) :
           gfx(gfx),  xPos(xPos), func(func), inc(inc), dec(dec) {
    }
    ;
    void update(const Timer & timer) {
        func(str, timer);
    }
    void print() {
        if (xPos >= 0) {
            gfx->setBackground(BLACK);
            gfx->setForeground(WHITE);
            gfx->setFont(&bigFont);
            gfx->drawString(xPos, 8, str);
        }
    }
    void clear() {
        if (xPos >= 0) {
            gfx->setBackground(BLACK);
            gfx->setForeground(WHITE);
            gfx->setFont(&bigFont);
            gfx->drawString(xPos, 8, "  ");
        }
    }

    void incTime(Timer & timer) {
        if (inc != nullptr) {
            gfx->setBackground(BLACK);
            gfx->setForeground(WHITE);
            inc(timer);
            print();
        }
    }
    void decTime(Timer & timer) {
        if (dec != nullptr) {
            dec(timer);
            print();
        }
    }

    char * getStr() {
        return str;
    }

private:
    GFX * gfx;
    int xPos;
    char str[4];
    std::function<void(char * str, const Timer & timer)> func;
    std::function<void(Timer & timer)> inc;
    std::function<void(Timer & timer)> dec;

};

#endif /* TIMEELEMENT_H_ */
