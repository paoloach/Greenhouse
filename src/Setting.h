/*
 * Setting.h
 *
 *  Created on: 15 giu 2017
 *      Author: paolo
 */

#ifndef SETTING_H_
#define SETTING_H_

#include "Setting.h"
#include "SettingHour.h"

class Setting {
public:
    Setting(Hour & time);
    ~Setting() {
        Configuration::save();
    }
    void run();
private:
    void setPage();

    SettingHours timeHours;
    SettingMinutes timeMinutes;
    SettingSeconds timeSeconds;
    SettingHours onHours;
    SettingMinutes onMinutes;
    SettingHours offHours;
    SettingMinutes offMinutes;
    SettingSampleTime sampleTime;
    SettingTempHeaterUp heaterUp;
    SettingTempHeaterDown heaterDown;
    SettingRow * current;



};

#endif /* SETTING_H_ */
