//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

#ifndef TIMER_H_
#define TIMER_H_

#include "cmsis_device.h"
#include "Hour.h"
// ----------------------------------------------------------------------------

class Timer {
public:
    typedef uint32_t ticks_t;
    static constexpr ticks_t FREQUENCY_HZ = 1010u;

public:
    static volatile ticks_t ms_delayCount;
    static uint32_t tickets_seconds;
    static Hour hour;
public:
    // Default constructor
    Timer();

    inline void start(void) {
        // Use SysTick as reference for the delay loops.
        SysTick_Config(SystemCoreClock / FREQUENCY_HZ);
    }

    static uint32_t getMillisecond() {
        return tickets_seconds;
    }
    Hour getHour() const {
        return hour;
    }

    static void sleep(ticks_t ticks);

    inline static void tick(void);
};

// ----------------------------------------------------------------------------

#endif // TIMER_H_
