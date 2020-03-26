#include <stdio.h>
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <SDL2/SDL.h>

#include "chip8.hpp"

int main()
{
    // Emu Setup
    Chip8 chip8;
    chip8.loadGame("Games/PONG");
    chip8.loadFontset();

    // SDL Setup - adapted from https://github.com/JamesGriffin/CHIP-8-Emulator
    int w = 640;
    int h = 320;

    SDL_Window *window = NULL;

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    window = SDL_CreateWindow(
        "CHIP-8 Emulator",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        w, h, 0);
    if (window == NULL)
    {
        printf("Window could not be created! SDL_Error: %s\n",
               SDL_GetError());
        exit(1);
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_RenderSetLogicalSize(renderer, w, h);

    SDL_Texture *sdlTexture = SDL_CreateTexture(renderer,
                                                SDL_PIXELFORMAT_ARGB8888,
                                                SDL_TEXTUREACCESS_STREAMING,
                                                64,
                                                32);

    uint32_t pixel_buf[2048];

    bool quit = false;
    while (!quit)
    {
        chip8.emulateCycle();

        if (chip8.drawFlag)
        {
            chip8.drawFlag = false;
            for (int i = 0; i < 2048; i++)
            {
                pixel_buf[i] = chip8.gfx[i] == 0x0 ? 0xFF000000 : 0xFFFFFFFF;
            }
            SDL_UpdateTexture(sdlTexture, NULL, pixel_buf, 64 * sizeof(Uint32));
        }

        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
        }
        SDL_RenderClear(renderer); // Is this needed?
        SDL_RenderCopy(renderer, sdlTexture, NULL, NULL);
        SDL_RenderPresent(renderer);

        chip8.setKeys();
        std::this_thread::sleep_for(std::chrono::microseconds(16000));
    }
    return 0;
}