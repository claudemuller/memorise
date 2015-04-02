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
            bool bQuit = false;

            SDL_Event e;

            // The infamous "Game Loop"
            while (!bQuit)
            {
                // Check for events on each frame
                while (SDL_PollEvent(&e) != 0)
                {
                    // Quit on the X window button
                    if (e.type == SDL_QUIT)
                    {
                        bQuit = true;
                    }
                    else if (e.type == SDL_KEYDOWN)
                    {
                        // Or on 'q' keyboard
                        if (e.key.keysym.sym == SDLK_q)
                            bQuit = true;
                    }
                }

                // Blit scaled down button surfaces
                SDL_Rect rect;
                for (int i = 0; i < BUTTON_SURFACE_TOTAL; i++)
                {
                    rect.x = gButtonPositions[i].x;
                    rect.y = gButtonPositions[i].y;
                    rect.w = SCREEN_WIDTH / 3;
                    rect.y = SCREEN_HEIGHT / 3;
                    SDL_BlitSurface(gButtonSurfaces[i], NULL, gScreenSurface, &rect);
                }

                // SDL_BlitSurface(gInterfaceSurface, NULL, gScreenSurface, NULL);

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
    bool bSuccess = true;

    // SDL initialisation
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL Init error: SDL_Error.. %s\n", SDL_GetError());
        bSuccess = false;
    }
    else
    {
        // SDL Window Creation
        gWindow = SDL_CreateWindow("Memorise", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL)
        {
            printf("Create Window error: SDL_Error.. %s\n", SDL_GetError());
            bSuccess = false;
        }
        else
        {
            int nImgFlags = IMG_INIT_PNG;
            // Check if flags returned from Init function are the same as flags sent in
            if (!(IMG_Init(nImgFlags) & nImgFlags))
            {
                printf("IMG Init error: IMG_Error.. %s\n", IMG_GetError());
                bSuccess = false;
            }
            else
            {
                // Get a screen surface to work on
                gScreenSurface = SDL_GetWindowSurface(gWindow);
                if (gScreenSurface == NULL)
                {
                    printf("Get Window Surface error: SDL_Error.. %s\n", SDL_GetError());
                    bSuccess = false;
                }

                calculateSurfaceScale();
            }
        }
    }

    return bSuccess;
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
        optimisedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, 0);
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
    bool bSuccess = true;
    std::string sPath = "";

    sPath = ASSETS_DIR + "orange.png";
    gButtonSurfaces[BUTTON_SURFACE_ORANGE] = loadSurface(sPath);
    if (gButtonSurfaces[BUTTON_SURFACE_ORANGE] == NULL)
    {
        printf("Loading %s error: SDL_Error... %s\n", sPath.c_str(), SDL_GetError());
        bSuccess = false;
    }

    sPath = ASSETS_DIR + "green.png";
    gButtonSurfaces[BUTTON_SURFACE_GREEN] = loadSurface(sPath);
    if (gButtonSurfaces[BUTTON_SURFACE_GREEN] == NULL)
    {
        printf("Loading %s error: SDL_Error... %s\n", sPath.c_str(), SDL_GetError());
        bSuccess = false;
    }

    sPath = ASSETS_DIR + "purple.png";
    gButtonSurfaces[BUTTON_SURFACE_PURPLE] = loadSurface(sPath);
    if (gButtonSurfaces[BUTTON_SURFACE_PURPLE] == NULL)
    {
        printf("Loading %s error: SDL_Error... %s\n", sPath.c_str(), SDL_GetError());
        bSuccess = false;
    }

    sPath = ASSETS_DIR + "blue.png";
    gButtonSurfaces[BUTTON_SURFACE_BLUE] = loadSurface(sPath);
    if (gButtonSurfaces[BUTTON_SURFACE_BLUE] == NULL)
    {
        printf("Loading %s error: SDL_Error... %s\n", sPath.c_str(), SDL_GetError());
        bSuccess = false;
    }

    return bSuccess;
}

/**
 * Calculate the size of the surface to display depending on the resolution
 *
 * @TODO    but not bigger than their max dims and center them
 */
void calculateSurfaceScale()
{
    if (SCREEN_WIDTH > SCREEN_HEIGHT)
    {
        gButtonHeight = SCREEN_HEIGHT / 3;
        gButtonWidth = SCREEN_WIDTH * (3 / SCREEN_HEIGHT);
    }
    else
    {
        gButtonWidth = SCREEN_WIDTH / 3;
        gButtonHeight = SCREEN_HEIGHT * (3 / SCREEN_WIDTH);
    }

    position orangePos;
    orangePos.x = (SCREEN_WIDTH / 2) - (gButtonWidth / 2);
    orangePos.y = gButtonWidth;
    gButtonPositions[BUTTON_SURFACE_ORANGE] = orangePos;

    position greenPos;
    greenPos.x = 0;
    greenPos.y = (SCREEN_HEIGHT / 2) - (gButtonHeight / 2);
    gButtonPositions[BUTTON_SURFACE_GREEN] = greenPos;

    position purplePos;
    purplePos.x = (SCREEN_WIDTH / 2) - (gButtonWidth / 2);
    purplePos.y = SCREEN_HEIGHT;
    gButtonPositions[BUTTON_SURFACE_PURPLE] = purplePos;

    position bluePos;
    bluePos.x = SCREEN_WIDTH - gButtonWidth;
    bluePos.y = (SCREEN_HEIGHT / 2) - (gButtonHeight / 2);
    gButtonPositions[BUTTON_SURFACE_BLUE] = bluePos;

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

    IMG_Quit();
    SDL_Quit();
}
