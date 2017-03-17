/*
 * ILI9325.h
 *
 *  Created on: 23 gen 2017
 *      Author: paolo
 */

#ifndef ILI9325_H_
#define ILI9325_H_

#include <functional>
#include <tuple>
#include "GFX.h"
#include "RegisterILI9325.h"

// Driver for ILI9325 and S6D0129
class ILI9325: public GFX {
public:
    ILI9325(bool swapX=false);

    static bool checkPresence();
    void setRotation(RotationId rotation);
    void drawPixel(Point point, Color6Bit color) ;
    void drawPixel(Point && point, Color6Bit color) override;
    void drawFastHLine(Point point, uint16_t length, Color6Bit color) override;
    void drawFastVLine(Point point, uint16_t length, Color6Bit color) override;
    void fillRect(int16_t x1, int16_t y1, int16_t w, int16_t h, Color6Bit fillcolor);

    void setAddrWindow(uint16_t left, uint16_t top, uint16_t right, uint16_t bottom);
    void flood(Color6Bit color, uint32_t len);
    void setDriverQuality(DriverQuality driverQuality);
    void static reset();
private:
    void initILI9325();
    void initILI9320();
    void initS6D0129();

    struct InitCmd {
        constexpr InitCmd(ILI9325_REG reg, uint16_t arg) :
                reg { reg }, arg { arg } {
        }

        ILI9325_REG reg;
        uint16_t arg;
    };
    static const uint32_t DEFAULT_WIDTH = 240;
    static const uint32_t DEFAULT_HEIGTH = 320;

    static std::array<InitCmd, 48> ILI9235_init;
    static std::array<InitCmd, 43> ILI9320_init;
    static std::array<InitCmd, 29> S6D0129_init;
    bool swapX;

};

#endif /* ILI9325_H_ */
