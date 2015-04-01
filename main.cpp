#include "main.h"

int main(int argc, char* argv[])
{
    bool quit = false;

    SDL_Event e;

    if (!init())
    {
        printf("init error: SDL_Error.. %s\n", SDL_GetError());
        return 1;
    }
    else
    {
        // loadimages
        
        while (!quit)
        {
            while (SDL_PollEvent(&e) != 0)
            {
                if (e.type == SDL_QUIT)
                {
                    quit = true;
                }
                else if (e.type == SDL_KEYDOWN)
                {
                    if (e.key.keysym.sym == SDLK_q)
                        quit = true;
                }
            }

            // update screen
            SDL_UpdateWindowSurface(gWindow);
        }
    }

    close();

    return 0;
}

bool init()
{
    bool success = true;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL Init error: SDL_Error.. %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        gWindow = SDL_CreateWindow("Memorise", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL)
        {
            printf("Create Window error: SDL_Error.. %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            gScreenSurface = SDL_GetWindowSurface(gWindow);
            if (gScreenSurface == NULL)
            {
                printf("Get Window Surface error: SDL_Error.. %s\n", SDL_GetError());
                success = false;
            }
        }
    }

    return success;
}

SDL_Surface* loadSurface(std::string path)
{
    SDL_Surface* loadedSurface = NULL;

    SDL_Surface* convertedSurface = NULL;

    return convertedSurface;
}

void close()
{
    SDL_FreeSurface(gScreenSurface);
    gScreenSurface = NULL;

    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    SDL_Quit();
}
