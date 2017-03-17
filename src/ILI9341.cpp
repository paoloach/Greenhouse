/*
 * ILI9341.cpp
 *
 *  Created on: 26 feb 2017
 *      Author: paolo
 */

#include "diag/Trace.h"
#include "Timer.h"
#include "ILI9341.h"

namespace ILI9341 {

ILI9341::ILI9341() {
    width = DEFAULT_WIDTH;
    height = DEFAULT_HEIGTH;
    driver = LcdID::ID_9341;
    activeCS();

    resetOn();
    Timer::sleep(5);
    resetOff();
    Timer::sleep(150);

    DisplayOff::apply();
    PowerCtl1::write(0x23);
    PowerCtl2::write(0x10);
    VCOMCtl1::write(0x3e28);
    VCOMCtl2::write(0x86);
    //VCOMCtl2::write(0xC0);
    MemAccessCtrl::write (MADCTL_MY);
    PixelFormatSet::write(0x66);
    FrameCtrl::write(0x1B00);
    DisplayFnCtl::write(0x08, 0x82, 0x27);
    EntryModeSet::write(0x07);

    SleepOff::apply();
    Timer::sleep(150);
    DisplayOn::apply();
    Timer::sleep(500);

    setAddrWindow(0, 0, width - 1, height - 1);


    auto displayId = ReadDisplayIdentification::read();
    trace_printf("Display Identificaction: %02X, %02X, %02X\n", displayId[0], displayId[1], displayId[2]);
    auto status= DisplayStatus::read();
    trace_printf("Display status: %02X, %02X, %02X, %02X\n", status[0], status[1], status[2], status[3]);
    auto powerMode = DisplayPowerMode::read();
    trace_printf("Power mode: %02X\n", powerMode);
    auto selfDiagnostic = SelfDiagnostic::read();
    trace_printf("selfDiagnostic mode: %02X\n", selfDiagnostic);
    auto CtrlDisplay = ReadDisplayCtl::read();
    trace_printf("Read Ctrl display: %02X\n", CtrlDisplay);

    flood(Color6Bit(255,255,255), width*height);

    idleCS();
}

bool ILI9341::checkPresence() {
    activeCS();
    resetOn();
    Timer::sleep(5);
    resetOff();
    Timer::sleep(150);

    bool result = false;
    auto ILI9341Id = ReadId::read();
    idleCS();

    if (ILI9341Id[0] == 0x00 && ILI9341Id[1] == 0x93 && ILI9341Id[2] == 0x41)
        result = true;
    return result;
}

void ILI9341::setAddrWindow(uint16_t left, uint16_t top, uint16_t right, uint16_t bottom) {
    ColAddrSet::write((left >> 8), left, right >> 8, right);
    PageAddrSet::write(top >> 8, top, bottom >> 8, bottom);
}

void ILI9341::flood(Color6Bit color, uint32_t len) {
    MemoryWrite::write(color, len);
}

void ILI9341::setRotation(RotationId rotation) {
    uint8_t dir;

    switch (rotation) {
    case RotationId::ROT_180:
        dir = MADCTL_MX;
        break;
    case RotationId::ROT_270:
        dir = MADCTL_MV;
        break;
    case RotationId::ROT_0:
        dir = MADCTL_MY;
        break;
    case RotationId::ROT_90:
        dir = MADCTL_MX | MADCTL_MY | MADCTL_MV;
        break;
    }
    activeCS();
    MemAccessCtrl::write (dir);
    idleCS();
}

void ILI9341::drawPixel(Point p, Color6Bit color) {
    if ((p.x < 0) || (p.y < 0) || (p.x >= width) || (p.y >= height))
        return;
    activeCS();
    setAddrWindow(p.x, p.y, width - 1, height - 1);
    MemoryWrite::write(color);
    idleCS();
}

void ILI9341::drawPixel(Point && p, Color6Bit color) {
    if ((p.x < 0) || (p.y < 0) || (p.x >= width) || (p.y >= height))
        return;
    activeCS();
    setAddrWindow(p.x, p.y, width - 1, height - 1);
    MemoryWrite::write(color);
    idleCS();
}

void ILI9341::drawFastHLine(Point p, uint16_t length, Color6Bit color) {
    int16_t x2;

    // Initial off-screen clipping
    if ((p.y < 0) || (p.y >= height) || (p.x >= width) || ((x2 = (p.x + length - 1)) < 0))
        return;

    if (p.x < 0) {        // Clip left
        length += p.x;
        p.x = 0;
    }
    if (x2 >= width) { // Clip right
        x2 = width - 1;
        length = x2 - p.x + 1;
    }

    activeCS();
    setAddrWindow(p.x, p.y, x2, p.y);
    flood(color, length);
    setAddrWindow(0, 0, width - 1, height - 1);
    idleCS();
}

void ILI9341::drawFastVLine(Point p, uint16_t length, Color6Bit color) {
    int16_t y2;

    // Initial off-screen clipping
    if ( (p.x < 0) || (p.x >= width) || (p.y >= height) || ((y2 = (p.y + length - 1)) < 0))
        return;
    if (p.y < 0) {
        length += p.y;
        p.y = 0;
    }
    if (y2 >= height) {
        y2 = height - 1;
        length = y2 - p.y + 1;
    }

    activeCS();
    setAddrWindow(p.x, p.y, p.x, y2);
    flood(color, length);
    setAddrWindow(0, 0, width - 1, height - 1);
    idleCS();
}

void ILI9341::fillRect(int16_t x1, int16_t y1, int16_t w, int16_t h, Color6Bit fillcolor) {
    int16_t x2, y2;

    if ((w <= 0) || (h <= 0) || (x1 >= width) || (y1 >= height) || ((x2 = x1 + w - 1) < 0) || ((y2 = y1 + h - 1) < 0))
        return;
    if (x1 < 0) {
        w += x1;
        x1 = 0;
    }
    if (y1 < 0) {
        h += y1;
        y1 = 0;
    }
    if (x2 >= width) {
        x2 = width - 1;
        w = x2 - x1 + 1;
    }
    if (y2 >= height) {
        y2 = height - 1;
        h = y2 - y1 + 1;
    }

    activeCS();
    setAddrWindow(x1, y1, x2, y2);
    flood(fillcolor, w * h);
    setAddrWindow(0, 0, width - 1, height - 1);
    idleCS();
}

} // end namespace ILI9341
