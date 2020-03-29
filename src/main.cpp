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
    chip8.loadGame("Games/TANK");
    chip8.loadFontset();

    // Keymap definition
    SDL_Keycode keymap[16] = {
        SDLK_x,
        SDLK_1,
        SDLK_2,
        SDLK_3,
        SDLK_q,
        SDLK_w,
        SDLK_e,
        SDLK_a,
        SDLK_s,
        SDLK_d,
        SDLK_z,
        SDLK_c,
        SDLK_4,
        SDLK_r,
        SDLK_f,
        SDLK_v,
    };

    // SDL Setup
    int w = 640;
    int h = 320;

    SDL_Window *window = NULL;

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        std::cout << "SDL failed to initialize: " << SDL_GetError() << "\n";
        exit(1);
    }

    window = SDL_CreateWindow(
        "CHIP-8 Emulator",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        w, h, 0);
    if (window == NULL)
    {
        std::cout << "Failed to create window: " << SDL_GetError() << "\n";
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
    //for (int inst = 0; inst < 1000; inst++)
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
            if (e.type == SDL_KEYDOWN)
            {
                for (int i = 0; i < 16; i++)
                {
                    if (e.key.keysym.sym == keymap[i])
                    {
                        chip8.keyPressed(i);
                    }
                }
            }
            if (e.type == SDL_KEYUP)
            {
                for (int i = 0; i < 16; i++)
                {
                    if (e.key.keysym.sym == keymap[i])
                    {
                        chip8.keyReleased(i);
                    }
                }
            }
        }
        SDL_RenderCopy(renderer, sdlTexture, NULL, NULL);
        SDL_RenderPresent(renderer);

        std::this_thread::sleep_for(std::chrono::microseconds(1200));
    }
    chip8.printState();
    return 0;
}