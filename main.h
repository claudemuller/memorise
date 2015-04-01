#include <cstdio>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

// Constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
// const std::string ASSETS_DIR = "assets/";
const std::string ASSETS_DIR = "assets\\";

enum buttonSurfaces
{
    BUTTON_SURFACE_ORANGE,
    BUTTON_SURFACE_GREEN,
    BUTTON_SURFACE_PURPLE,
    BUTTON_SURFACE_BLUE,
    BUTTON_SURFACE_TOTAL
};

// Global variables
SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Surface* gButtonSurfaces[BUTTON_SURFACE_TOTAL];

// Functions
bool init();
SDL_Surface* loadSurface(std::string path);
bool loadAssets();
void close();
