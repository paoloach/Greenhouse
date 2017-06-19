/*
 * SettingHour.h
 *
 *  Created on: 15 giu 2017
 *      Author: paolo
 */

#ifndef SETTINGHOUR_H_
#define SETTINGHOUR_H_

#include <stdio.h>

#include "Hour.h"
#include "Hardware.h"

class SettingHour {
public:
    SettingHour(Hour & hour, Point point): hour(hour), point(point), selected(false){};
    virtual void inc()=0;
    virtual void dec()=0;
    virtual void setBuffer()=0;
    void print();
    void select() {selected=true;}
    void deselect() {selected=false;}
protected:
    Hour & hour;
    Point point;
    bool selected;
    char buffer[4];
};

class SettingHours : public SettingHour {
public:
    SettingHours(Hour & hour, Point point): SettingHour(hour, point){};
    virtual void inc()  {
        hour.incHour();
    }
    virtual void dec() {
        hour.decHour();
    }
    virtual void setBuffer() {
        sprintf(buffer, "%.02u", hour.getHours());
    }
};
class SettingMinutes : public SettingHour {
public:
    SettingMinutes(Hour & hour, Point point): SettingHour(hour, point){};
    virtual  void inc() {
        hour.incMinute();
    }
    virtual  void dec() {
        hour.decMinute();
    }
    virtual void setBuffer() {
        sprintf(buffer, "%.02u", hour.getMinutes());
    }
};

class SettingSeconds : public SettingHour {
public:
    SettingSeconds(Hour & hour, Point point): SettingHour(hour, point){};
    virtual  void inc() {
        hour.incSecond();
    }
    virtual void dec() {
        hour.decSecond();
    }
    virtual void setBuffer() {
        sprintf(buffer, "%.02u", hour.getSeconds());
    }
};

#endif /* SETTINGHOUR_H_ */
