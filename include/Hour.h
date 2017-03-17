/*
 * Hour.h
 *
 *  Created on: 09 ott 2016
 *      Author: paolo
 */

#ifndef HOUR_H_
#define HOUR_H_

#include <sys/types.h>
#include "cmsis_device.h"

class Hour {
public:
    Hour();
    Hour(uint32_t hours, uint32_t minuts, uint32_t seconds);
public:
    uint getSeconds() {
        return seconds % 60;
    }

    uint getMinutes() {
        return (seconds / 60) % 60;
    }

    uint getHours() {
        return seconds / 3600;
    }

    void operator++(int) {
        seconds++;
        if (seconds > 86400) {
            seconds -= 86400;
        }
    }
    bool operator>(const Hour & other) {
        return seconds > other.seconds;
    }

    int32_t operator-(const Hour & other){
        return seconds - other.seconds;
    }


    void incHour() {
        seconds += 3600;
        if (seconds > 86400) {
            seconds -= 86400;
        }
    }
    void decHour() {
        seconds -= 3600;
        if (seconds < 0) {
            seconds = 86400 + seconds;
        }
    }
    void incMinute() {
        seconds += 60;
        if (seconds > 86400) {
            seconds -= 86400;
        }
    }

    void incMinute(uint minuts) {
        seconds += 60*minuts;
        if (seconds > 86400) {
            seconds -= 86400;
        }
    }
    void decMinute() {
        seconds -= 60;
        if (seconds < 0) {
            seconds = 86400 + seconds;
        }
    }

    void incSecond() {
        seconds ++;
        if (seconds > 86400) {
            seconds -= 86400;
        }
    }
    void decSecond() {
        seconds --;
        if (seconds < 0) {
            seconds = 86400 + seconds;
        }
    }
public:
    int32_t seconds;
};

#endif /* HOUR_H_ */
