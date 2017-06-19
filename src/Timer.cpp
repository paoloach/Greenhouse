//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

#include <vector>
#include "Timer.h"
#include "cortexm/ExceptionHandlers.h"

// ----------------------------------------------------------------------------

#if defined(USE_HAL_DRIVER)
extern "C" void HAL_IncTick(void);
#endif

// ----------------------------------------------------------------------------

volatile Timer::ticks_t Timer::ms_delayCount;
uint32_t Timer::tickets_seconds;
uint32_t Timer::tickets_ms;
Hour Timer::hour(19, 15, 0);
Timer::Callbacks Timer::callbacks;
bool Timer::stop;
// ----------------------------------------------------------------------------

Timer::Timer() {
    tickets_seconds = 0;
    tickets_ms = 0;
}

void Timer::tick(void) {
    // Decrement to zero the counter used by the delay routine.
    tickets_ms++;
    if (ms_delayCount != 0u) {
        --ms_delayCount;
    }
    if (tickets_seconds < FREQUENCY_HZ) {
        tickets_seconds++;
    } else {
        tickets_seconds = 0;
        if (!stop)
            hour++;

        std::vector<Callbacks::iterator> toRemove;
        for (Callbacks::iterator it = callbacks.begin(); it != callbacks.end(); it++) {
            if ((*it).first < hour.seconds) {
                (*it).second();
                toRemove.push_back(it);
            }
        }
        for (auto & removing : toRemove) {
            callbacks.erase(removing);
        }
    }
}

void Timer::sleep(ticks_t milliseconds) {
    ms_delayCount = milliseconds;
    while (ms_delayCount != 0u)
        ;
}

// ----- SysTick_Handler() ----------------------------------------------------

extern "C" void SysTick_Handler(void) {
    Timer::tick();
}

// ----------------------------------------------------------------------------
