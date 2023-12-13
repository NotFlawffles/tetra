#include "../../include/components/ppu.hpp"
#include "../../include/definitions/graphics.hpp"

PPU::PPU(void) {
    window = SDL_CreateWindow(
	Graphics::Window::TITLE,
	SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
	Graphics::Window::SCALE * Graphics::Window::ASPECT_RATIO, Graphics::Window::SCALE,
	0
    );

    renderer = SDL_CreateRenderer(window, -1, 0);
    texture = SDL_CreateTexture(renderer, Graphics::Texture::FORMAT, 0, Graphics::Texture::SCALE, Graphics::Texture::SCALE);
}

void PPU::setup(void) {
    SDL_Init(SDL_INIT_EVERYTHING);
}

void PPU::update_texture(unsigned short* buffer) {
    unsigned int pixels[Graphics::Texture::SCALE * Graphics::Texture::SCALE] = {0};

    for (size_t i = 0; i < Graphics::Texture::SCALE * Graphics::Texture::SCALE; i++) {
	pixels[i] = Graphics::ColorCodes[buffer[i]];
    }

    SDL_UpdateTexture(texture, nullptr, pixels, Graphics::Texture::SCALE * 4);
}

void PPU::draw_texture(void) {
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}
