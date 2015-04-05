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
            int c = gSequenceLength - 1;

            SDL_Event e;
            SDL_Rect rect;

            timeval t1;
            for (int i = 0; i < gSequence.size(); i++)
            {
                gettimeofday(&t1, NULL);
                srand(t1.tv_usec * t1.tv_sec);

                gSequence[i] = rand() % gSequenceLength;
                std::cout << gSequence.at(i) << std::endl;
            }

            int nTimeNow = time(NULL);
            int nD = 0;
            int nCurrentButton = gSequence[nD];

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
                                c = buttonHandler(KEY_PRESS_UP, c);
                                break;
                            case SDLK_RIGHT:
                                c = buttonHandler(KEY_PRESS_RIGHT, c);
                                break;
                            case SDLK_DOWN:
                                c = buttonHandler(KEY_PRESS_DOWN, c);
                                break;
                            case SDLK_LEFT:
                                c = buttonHandler(KEY_PRESS_LEFT, c);
                                break;
                            default:
                                break;
                        }

                        if (c < 0)
                        {
                            std::cout << "You won!" << std::endl;
                            bQuit = true;
                        }
                    }
                }

                // std::cout << (time(NULL) - nTimeNow) << std::endl;

                int nHeightOffset = 0;
                if (((time(NULL) - nTimeNow) < gDisplayDuration) && (nD < gSequenceLength))
                {
                    nHeightOffset = gButtonHeight / 3;
                }
                else
                {
                    nTimeNow = time(NULL);
                    nHeightOffset = 0;
                    nD++;
                    nCurrentButton = gSequence.at(nD);
                }

                // White background
                SDL_FillRect(gScreenSurface, NULL, SDL_MapRGB(gScreenSurface->format, 0xFF, 0xFF, 0xFF));

                // Blit scaled down button surfaces
                for (int i = 0; i < BUTTON_SURFACE_TOTAL; i++)
                {
                    rect.x = gButtonPositions[i].x;
                    rect.w = gButtonWidth;
                    if (i == nCurrentButton)
                    {
                        rect.y = gButtonPositions[i].y + nHeightOffset;
                        rect.h = gButtonHeight - nHeightOffset;
                    }
                    else
                    {
                        rect.y = gButtonPositions[i].y;
                        rect.h = gButtonHeight;
                    }

                    SDL_BlitScaled(gButtonSurfaces[i], NULL, gScreenSurface, &rect);
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

int buttonHandler(int pressedKey, int c)
{
    std::cout << "Up" << std::endl;
    std::cout << pressedKey << " - " << gSequence.at(c) << std::endl;
    if (pressedKey == gSequence.at(c))
    {
        std::cout << "yes" << std::endl;
        c--;
    }
    else
    {
        c = gSequenceLength - 1;
    }

    return c;
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
 * Show sequence to remember
 *
 * @TODO    make it work in v0.2
 */
std::vector<int> generateSequence(int length)
{
    std::vector<int> sequence;

    for (int i = 0; i < length; i++)
    {
        sequence.back() = (rand() % length) - BUTTON_SURFACE_ORANGE;
    }

    return sequence;
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
