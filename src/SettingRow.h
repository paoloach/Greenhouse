/*
 * SettingRow.h
 *
 *  Created on: 19 giu 2017
 *      Author: paolo
 */

#ifndef SETTINGROW_H_
#define SETTINGROW_H_

#include <stdlib.h>
#include "Configuration.h"

class SettingRow {
public:
    SettingRow() :
            selected(false) {
    }
    virtual ~SettingRow() = default;
    virtual void inc()=0;
    virtual void dec()=0;
    virtual void print()=0;
    void select() {
        selected = true;
    }
    void deselect() {
        selected = false;
    }
protected:
    bool selected;
};

class SettingConfiguration: public SettingRow {
public:
    SettingConfiguration(Point && point) :
            point(std::move(point)) {
    }
    virtual ~SettingConfiguration() = default;
protected:
    virtual int getValue() = 0;
public:
    virtual void print() {
        char buffer[8];
        gfx->setFont(&smallFont);
        if (selected) {
            gfx->setForeground(WHITE);
            gfx->setBackground(BLACK);
        } else {
            gfx->setForeground(BLACK);
            gfx->setBackground(WHITE);

        }
        itoa(getValue(), buffer, 10);
        gfx->drawString(point, buffer);
    }
protected:
    Point point;
};

class SettingTempHeaterUp: public SettingConfiguration {
public:
    SettingTempHeaterUp(Point && point) :SettingConfiguration(std::move(point)){}
    virtual ~SettingTempHeaterUp() = default;
private:
    virtual void inc() {
        if (Configuration::heaterUp < 50)
            Configuration::heaterUp++;
    }
    virtual void dec() {
        if (Configuration::heaterUp > 0)
            Configuration::heaterUp--;
    }
    virtual int getValue() {
        return Configuration::heaterUp;
    }
};

class SettingTempHeaterDown: public SettingConfiguration {
public:
    SettingTempHeaterDown(Point && point) :SettingConfiguration(std::move(point)){}
    virtual ~SettingTempHeaterDown() = default;
private:
    virtual void inc() {
        if (Configuration::heaterDown < 50)
            Configuration::heaterDown++;
    }
    virtual void dec() {
        if (Configuration::heaterDown > 0)
            Configuration::heaterDown--;
    }
    virtual int getValue() {
        return Configuration::heaterDown;
    }
};

class SettingSampleTime: public SettingConfiguration {
public:
    SettingSampleTime(Point && point) :SettingConfiguration(std::move(point)){}
    virtual ~SettingSampleTime() = default;
private:
    virtual void inc() {
        if (Configuration::minuteFromPoint < 1180)
            Configuration::minuteFromPoint++;
    }
    virtual void dec() {
        if (Configuration::minuteFromPoint > 0)
            Configuration::minuteFromPoint--;
    }
    virtual int getValue() {
        return Configuration::minuteFromPoint;
    }
};
#endif /* SETTINGROW_H_ */
