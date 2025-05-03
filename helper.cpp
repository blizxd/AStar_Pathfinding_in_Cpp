#include "helper.h"
#include <SDL.h>
#include <functional>
#include <iostream>
#include <unordered_map>

using namespace std;
using namespace helper;

namespace
{

    const int WIDTH = 1200;
    const int HEIGHT = 960;

    SDL_Renderer *renderer;
    SDL_Window *window;
}

namespace helper
{

    int getWidth()
    {
        return WIDTH;
    }

    int getHeight()
    {
        return HEIGHT;
    }

    void drawCircle(int centreX, int centreY, int radius)
    {
        const int32_t diameter = (radius * 2);

        int x = (radius - 1);
        int y = 0;
        int tx = 1;
        int ty = 1;
        int error = (tx - diameter);

        while (x >= y)
        {
            //  Each of the following renders an octant of the circle
            SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
            SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
            SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
            SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
            SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
            SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
            SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
            SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

            if (error <= 0)
            {
                ++y;
                error += ty;
                ty += 2;
            }

            if (error > 0)
            {
                --x;
                tx += 2;
                error += (tx - diameter);
            }
        }
    }

    void drawFilledCircle(int centreX, int centreY, int radius)
    {
        int offsetx, offsety, d;

        offsetx = 0;
        offsety = radius;
        d = radius - 1;

        while (offsety >= offsetx)
        {

            SDL_RenderDrawLine(renderer, centreX - offsety, centreY + offsetx,
                               centreX + offsety, centreY + offsetx);
            SDL_RenderDrawLine(renderer, centreX - offsetx, centreY + offsety,
                               centreX + offsetx, centreY + offsety);
            SDL_RenderDrawLine(renderer, centreX - offsetx, centreY - offsety,
                               centreX + offsetx, centreY - offsety);
            SDL_RenderDrawLine(renderer, centreX - offsety, centreY - offsetx,
                               centreX + offsety, centreY - offsetx);

            if (d >= 2 * offsetx)
            {
                d -= 2 * offsetx + 1;
                offsetx += 1;
            }
            else if (d < 2 * (radius - offsety))
            {
                d += 2 * offsety - 1;
                offsety -= 1;
            }
            else
            {
                d += 2 * (offsety - offsetx - 1);
                offsety -= 1;
                offsetx += 1;
            }
        }
    }

    void setColor(uint8_t r, uint8_t g, uint8_t b)
    {
        SDL_SetRenderDrawColor(renderer, r, g, b, 0xFF);
    }

    void drawLine(int x1, int y1, int x2, int y2)
    {
        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    }

    void drawRect(int x, int y, int width, int height)
    {
        SDL_Rect rect = {x, y, width, height};
        SDL_RenderFillRect(renderer, &rect);
    }

    void present()
    {
        SDL_RenderPresent(renderer);
    }

    void clear()
    {
        SDL_Color old;
        SDL_GetRenderDrawColor(renderer, &old.r, &old.g, &old.b, &old.a);

        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, old.r, old.g, old.b, old.a);
    }

    bool init()
    {
        // See last example for comments
        if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        {
            cout << "Error initializing SDL: " << SDL_GetError() << endl;
            return false;
        }

        window = SDL_CreateWindow("Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
        if (!window)
        {
            cout << "Error creating window: " << SDL_GetError() << endl;
            return false;
        }

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer)
        {
            cout << "Error creating renderer: " << SDL_GetError() << endl;
            return false;
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        return true;
    }

    void shutdown()
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
}