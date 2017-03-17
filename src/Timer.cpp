//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

#include "Timer.h"
#include "cortexm/ExceptionHandlers.h"

// ----------------------------------------------------------------------------

#if defined(USE_HAL_DRIVER)
extern "C" void HAL_IncTick(void);
#endif

// ----------------------------------------------------------------------------

volatile Timer::ticks_t Timer::ms_delayCount;
uint32_t Timer::tickets_seconds;
Hour Timer::hour(17,0,0);
// ----------------------------------------------------------------------------

Timer::Timer() {
	tickets_seconds = 0;
}

void Timer::tick(void) {
	// Decrement to zero the counter used by the delay routine.
	if (ms_delayCount != 0u) {
		--ms_delayCount;
	}
	if (tickets_seconds < FREQUENCY_HZ) {
		tickets_seconds++;
	} else {
		tickets_seconds = 0;
		hour++;
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
