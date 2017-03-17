/*
 * ILI9341.h
 *
 *  Created on: 26 feb 2017
 *      Author: paolo
 */

#ifndef ILI9341_H_
#define ILI9341_H_

#include <functional>
#include <tuple>
#include "GFX.h"
#include "RegisterILI9341.h"

namespace ILI9341 {

class ILI9341: public GFX {
public:
    ILI9341();

    static bool checkPresence();
    void setRotation(RotationId rotation);
    void drawPixel(Point point, Color6Bit color);
    void drawPixel(Point && point, Color6Bit color) override;
    void drawFastHLine(Point point, uint16_t length, Color6Bit color) override;
    void drawFastVLine(Point point, uint16_t length, Color6Bit color) override;
    void fillRect(int16_t x1, int16_t y1, int16_t w, int16_t h, Color6Bit fillcolor);
    void setDriverQuality(DriverQuality driverQuality);
    void static reset();
protected:
    void setAddrWindow(uint16_t left, uint16_t top, uint16_t right, uint16_t bottom);
    void flood(Color6Bit color, uint32_t len);
private:
    static const uint32_t DEFAULT_WIDTH = 240;
    static const uint32_t DEFAULT_HEIGTH = 320;

};
}// end namespace ILI9341

#endif /* ILI9341_H_ */
