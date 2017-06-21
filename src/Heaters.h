/*
 * Heaters.h
 *
 *  Created on: 21 giu 2017
 *      Author: paolo
 */

#ifndef HEATERS_H_
#define HEATERS_H_

class Heaters {
public:
    Heaters();
    void updateUp(int16_t temp);
    void updateDown(int16_t temp);
};

#endif /* HEATERS_H_ */
