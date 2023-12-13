#pragma once

#include <array>
#include <SDL2/SDL.h>

namespace Graphics {
    namespace Window {
	const char TITLE[] = "tetra";
	const unsigned int SCALE = 512;
	const float ASPECT_RATIO = 4.0/3;
    }

    namespace Texture {
	const SDL_PixelFormatEnum FORMAT = SDL_PIXELFORMAT_RGBA8888;
	const unsigned int SCALE = 90;
    }

    enum Colors {
	BLACK,
	WHITE,
	RED,
	GREEN,
	BLUE
    };

    const std::array<unsigned int, 0xF> ColorCodes {
	0x000000FF,
	0xFFFFFFFF,
	0xFF0000FF,
	0x00FF00FF,
	0x0000FFFF
    };
}
