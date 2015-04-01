/**
 * Memorise - an SDL2 game that tests one's sequential memory
 *
 * @author  Claude Müller
 */

#include "main.h"

int main(int argc, char* argv[])
{
    if (!init())
    {
        printf("init error: SDL_Error.. %s\n", SDL_GetError());
        return 1;
    }
    else
    {
        if (!loadAssets())
        {
            printf("Asset loading error: SDL_Error.. %s\n", SDL_GetError());
        }
        else
        {
            bool quit = false;

            SDL_Event e;

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
    
                // Blit scaled down button surfaces
                SDL_Rect rect;
                for (int i = 0; i < BUTTON_SURFACE_TOTAL; i++)
                {
                    rect.x = 0;
                    rect.y = 0;
                    rect.w = 200;
                    rect.y = 200;
                    SDL_BlitScaled(gButtonSurfaces[i], &rect, gScreenSurface, NULL);
                }

                // Update the window
                SDL_UpdateWindowSurface(gWindow);
            }
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
            int imgFlags = IMG_INIT_PNG;
            // Check if flags returned from Init function are the same as flags sent in
            if (!(IMG_Init(imgFlags) & imgFlags))
            {
                printf("IMG Init error: IMG_Error.. %s\n", IMG_GetError());
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
    SDL_Surface* optimisedSurface = NULL;

    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL)
    {
        printf("IMG Loading error: IMG_Error... %s\n", IMG_GetError());
    }
    else
    {
        optimisedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, NULL);
        if (optimisedSurface == NULL)
        {
            printf("Surface Converting error: SDL_Error... %s\n", SDL_GetError());
        }

        // Free the original unoptimised surface
        SDL_FreeSurface(loadedSurface);
    }

    return optimisedSurface;
}

bool loadAssets()
{
    bool success = true;
    std::string path = "";

    path = ASSETS_DIR + "orange.png";
    gButtonSurfaces[BUTTON_SURFACE_ORANGE] = loadSurface(path);
    if (gButtonSurfaces[BUTTON_SURFACE_ORANGE] == NULL)
    {
        printf("Loading %s error: SDL_Error... %s\n", path.c_str(), SDL_GetError());
        success = false;
    }

    path = ASSETS_DIR + "green.png";
    gButtonSurfaces[BUTTON_SURFACE_GREEN] = loadSurface(path);
    if (gButtonSurfaces[BUTTON_SURFACE_GREEN] == NULL)
    {
        printf("Loading %s error: SDL_Error... %s\n", path.c_str(), SDL_GetError());
        success = false;
    }

    path = ASSETS_DIR + "purple.png";
    gButtonSurfaces[BUTTON_SURFACE_PURPLE] = loadSurface(path);
    if (gButtonSurfaces[BUTTON_SURFACE_PURPLE] == NULL)
    {
        printf("Loading %s error: SDL_Error... %s\n", path.c_str(), SDL_GetError());
        success = false;
    }

    path = ASSETS_DIR + "blue.png";
    gButtonSurfaces[BUTTON_SURFACE_BLUE] = loadSurface(path);
    if (gButtonSurfaces[BUTTON_SURFACE_BLUE] == NULL)
    {
        printf("Loading %s error: SDL_Error... %s\n", path.c_str(), SDL_GetError());
        success = false;
    }

    return success;
}

/**
 * The clean up function
 */
void close()
{
    for (int i = 0; i < BUTTON_SURFACE_TOTAL; i++)
    {
        SDL_FreeSurface(gButtonSurfaces[i]);
    }

    SDL_FreeSurface(gScreenSurface);
    gScreenSurface = NULL;

    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    SDL_Quit();
}
