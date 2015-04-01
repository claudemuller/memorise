#include <cstdio>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

// Constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Global variables
SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;

// Functions
bool init();
SDL_Surface* loadSurface(std::string path);
void close();
