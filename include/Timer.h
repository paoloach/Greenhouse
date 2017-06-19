//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

#ifndef TIMER_H_
#define TIMER_H_

#include <map>
#include <functional>
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
    static uint32_t tickets_ms;
    static Hour hour;
    static bool stop;
    using Callbacks =std::map<int32_t, std::function<void()>>;
    static Callbacks  callbacks;
public:
    // Default constructor
    Timer();

    inline void start(void) {
        // Use SysTick as reference for the delay loops.
        SysTick_Config(SystemCoreClock / FREQUENCY_HZ);
    }

    static uint32_t getMillisecond() {
        return tickets_ms;
    }
    Hour & getHour()  {
        return hour;
    }

    static void sleep(ticks_t ticks);

    inline static void tick(void);
};

// ----------------------------------------------------------------------------

#endif // TIMER_H_
