#pragma once

#include <SDL2/SDL.h>

class PPU {
    public:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event event;
    SDL_Texture* texture;
    bool running;

    PPU(void);
    void setup(void);
    void update_texture(unsigned short* buffer);
    void draw_texture(void);
};
