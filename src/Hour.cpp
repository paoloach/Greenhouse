/*
 * Hour.cpp
 *
 *  Created on: 09 ott 2016
 *      Author: paolo
 */

#include "Hour.h"

Hour::Hour() {
	seconds = 0;
}

Hour::Hour(uint32_t hours, uint32_t minuts, uint32_t seconds): seconds(seconds+minuts*60+hours*3600){
}

