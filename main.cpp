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
                        // Handle keyboard input
                        switch (e.key.keysym.sym)
                        {
                            // 'q' key to quit
                            case SDLK_q:
                                bQuit = true;
                                break;
                            case SDLK_UP:
                                std::cout << "Up" << std::endl;
                                break;
                            case SDLK_DOWN:
                                std::cout << "Down" << std::endl;
                                break;
                            case SDLK_LEFT:
                                std::cout << "Left" << std::endl;
                                break;
                            case SDLK_RIGHT:
                                std::cout << "Right" << std::endl;
                                break;
                            default:
                                break;
                        }
                    }
                }

                // White background
                SDL_FillRect(gScreenSurface, NULL, SDL_MapRGB(gScreenSurface->format, 0xFF, 0xFF, 0xFF));

                // Blit scaled down button surfaces
                SDL_Rect rect;
                for (int i = 0; i < BUTTON_SURFACE_TOTAL; i++)
                {
                    rect.x = gButtonPositions[i].x;
                    rect.y = gButtonPositions[i].y;
                    rect.w = gButtonWidth;
                    rect.h = gButtonHeight;
                    SDL_BlitSurface(gButtonSurfaces[i], NULL, gScreenSurface, &rect);
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
    float nRatio = 0.0;

    if (SCREEN_WIDTH < SCREEN_HEIGHT)
    {
        nRatio = (float)SCREEN_WIDTH / SCREEN_HEIGHT;
    }
    else
    {
        nRatio = (float)SCREEN_HEIGHT / SCREEN_WIDTH ;
    }

    // gButtonWidth = (float)BUTTON_IMG_WIDTH / nRatio;
    // gButtonHeight = (float)BUTTON_IMG_HEIGHT / nRatio;
    gButtonWidth = BUTTON_IMG_WIDTH;
    gButtonHeight = BUTTON_IMG_HEIGHT;

    position pos;

    pos.x = ((float)SCREEN_WIDTH / 2) - (gButtonWidth / 2);
    pos.y = 0;
    gButtonPositions[BUTTON_SURFACE_ORANGE] = pos;

    pos.x = SCREEN_WIDTH - gButtonWidth;
    pos.y = ((float)SCREEN_HEIGHT / 2) - (gButtonHeight / 2);
    gButtonPositions[BUTTON_SURFACE_GREEN] = pos;

    pos.x = ((float)SCREEN_WIDTH / 2) - (gButtonWidth / 2);
    pos.y = SCREEN_HEIGHT - gButtonHeight;
    gButtonPositions[BUTTON_SURFACE_PURPLE] = pos;

    pos.x = 0;
    pos.y = ((float)SCREEN_HEIGHT / 2) - (gButtonHeight / 2);
    gButtonPositions[BUTTON_SURFACE_BLUE] = pos;

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
