/**
 * Memorise - an SDL2 game that tests one's SEQUENTIAL memory
 *
 * @author  Claude Müller
 */

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
        
        // The infamous "Game Loop"
        while (!quit)
        {
            // Check for events on each frame
            while (SDL_PollEvent(&e) != 0)
            {
                // Quit on the X window button
                if (e.type == SDL_QUIT)
                {
                    quit = true;
                }
                else if (e.type == SDL_KEYDOWN)
                {
                    // Or on 'q' keyboard
                    if (e.key.keysym.sym == SDLK_q)
                        quit = true;
                }
            }

            // Update the window
            SDL_UpdateWindowSurface(gWindow);
        }
    }

    // Clean up
    close();

    return 0;
}

/**
 * The inititialisation function
 *
 * @returns bool
 */
bool init()
{
    bool success = true;

    // SDL initialisation
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL Init error: SDL_Error.. %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        // SDL Window Creation
        gWindow = SDL_CreateWindow("Memorise", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL)
        {
            printf("Create Window error: SDL_Error.. %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            // Get a screen surface to work on
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

/**
 * Loading an image from a path onto a surface
 *
 * @param   string      path - the path to the asset on disk
 * 
 * @returns SDL_Surface*
 */
SDL_Surface* loadSurface(std::string path)
{
    SDL_Surface* loadedSurface = NULL;

    SDL_Surface* convertedSurface = NULL;

    return convertedSurface;
}

/**
 * The clean up function
 */
void close()
{
    SDL_FreeSurface(gScreenSurface);
    gScreenSurface = NULL;

    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    SDL_Quit();
}
