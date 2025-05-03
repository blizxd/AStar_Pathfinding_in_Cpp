#pragma once

#include <SDL.h>
#include <functional>

namespace helper
{
    int getHeight();
    int getWidth();

    bool init();
    void shutdown();

    void setColor(uint8_t r, uint8_t g, uint8_t b);
    void drawLine(int x1, int y1, int x2, int y2);
    void drawRect(int x, int y, int width, int height);
    void drawCircle(int centreX, int centreY, int radius);
    void drawFilledCircle(int centreX, int centreY, int radius);

    void clear();
    void present();

}