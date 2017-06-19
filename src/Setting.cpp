/*
 * Setting.cpp
 *
 *  Created on: 15 giu 2017
 *      Author: paolo
 */

#include "Setting.h"
#include "Hardware.h"

Setting::Setting(Hour & time, Hour & on, Hour & off):
        timeHours(time, Point(88, 4)),
        timeMinutes(time, Point(88,30)),
        timeSeconds(time, Point(88,50)),
        onHours(on, Point(160,80)),
        onMinutes(on, Point(160,100)),
        offHours(off, Point(168,130)),
        offMinutes(off, Point(168,150)),
        current(&timeHours)
        {

}


void Setting::run(){
    setPage();
    current->select();
    current->print();
    while(true){
        if (buttonSetting.check()){
            if (buttonSetting){
                return;
            }
        }
        if (buttonInc.check()){
            if (buttonInc){
                current->inc();
                current->print();
            }
        }
        if (buttonDec.check()){
            if (buttonDec){
                current->dec();
                current->print();
            }
        }
        if (buttonNext.check()){
            if (buttonNext){
                SettingHour * next=nullptr;
                if (current == &timeHours){
                    next = &timeMinutes;
                } else  if (current == &timeMinutes){
                    next = &timeSeconds;
                } else  if (current == &timeMinutes){
                    next = &timeSeconds;
                } else  if (current == &timeSeconds){
                    next = &onHours;
                } else  if (current == &onHours){
                    next = &onMinutes;
                } else  if (current == &onMinutes){
                    next = &offHours;
                } else  if (current == &offHours){
                    next = &offMinutes;
                }else  if (current == &offMinutes){
                    next = &timeHours;
                }

                if (next != nullptr){
                    current->deselect();
                    current->print();
                    current = next;
                    current->select();
                    current->print();
                }
                current->print();
            }
        }
    }
}

void Setting::setPage(){
    gfx->fillRect(Point(0, 0), gfx->width, gfx->height, BLACK);
    gfx->setForeground(WHITE);
    gfx->setFont(&smallFont);
    gfx->drawString(Point(0,4), "HOUR   -->");
    gfx->drawString(Point(0,30),"MINUTE -->");
    gfx->drawString(Point(0,50),"SECOND -->");
    gfx->drawString(Point(0,80),"LIGHT ON HOUR   -->");
    gfx->drawString(Point(0,100),"LIGHT ON MINUTE -->");
    gfx->drawString(Point(0,130),"LIGHT OFF HOUR   -->");
    gfx->drawString(Point(0,150),"LIGHT OFF MINUTE -->");
    timeHours.print();
    timeMinutes.print();
    timeSeconds.print();
    onHours.print();
    onMinutes.print();
    offHours.print();
    offMinutes.print();
}

