/*
 * Setting.h
 *
 *  Created on: 15 giu 2017
 *      Author: paolo
 */

#ifndef SETTING_H_
#define SETTING_H_

#include "SettingHour.h"

class Setting {
public:
    Setting(Hour & time, Hour & on, Hour & off );

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
    SettingHour * current;



};

#endif /* SETTING_H_ */
