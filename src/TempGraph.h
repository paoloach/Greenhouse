/*
 * TempGraph.h
 *
 *  Created on: 31 ott 2016
 *      Author: paolo
 */

#ifndef TEMPGRAPH_H_
#define TEMPGRAPH_H_

#include "Timer.h"
#include "Hour.h"
#include "DHT.h"
#include "GFX.h"

class TempGraph {
public:
    TempGraph(uint16_t top, uint16_t bottom, const Timer & timer, DHT * dht, GFX * gfx,uint16_t min,uint16_t max, uint8_t index);
    void update(bool paint);
    void paintGraph(bool clear);
    void initGraph();
private:
    constexpr static uint8_t dateSize = 100;
    uint16_t top;
    uint16_t bottom;
    const Timer & timer;
    Hour next;
    DHT * dht;
    GFX * gfx;
    uint16_t min;
    uint16_t max;
    uint16_t right;
    uint8_t index;
    std::tuple<int16_t, int16_t> data[dateSize];


};

#endif /* TEMPGRAPH_H_ */
