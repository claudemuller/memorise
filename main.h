#include <cstdio>
#include <iostream>
#include <vector>
#include <sys/time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// Constants
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;
const int BUTTON_IMG_WIDTH = 344;
const int BUTTON_IMG_HEIGHT = 298;
const std::string ASSETS_DIR = "assets/";
// const std::string ASSETS_DIR = "assets\\";

enum buttonSurfaces
{
    BUTTON_SURFACE_ORANGE,
    BUTTON_SURFACE_GREEN,
    BUTTON_SURFACE_PURPLE,
    BUTTON_SURFACE_BLUE,
    BUTTON_SURFACE_TOTAL
};

enum buttonKeyPresses
{
    KEY_PRESS_UP,
    KEY_PRESS_RIGHT,
    KEY_PRESS_DOWN,
    KEY_PRESS_LEFT
};

// Global variables
SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Surface* gInterfaceSurface = NULL;
SDL_Surface* gButtonSurfaces[BUTTON_SURFACE_TOTAL];
float gButtonWidth, gButtonHeight = 0.0;
int gSequenceLength = 4;
int gDisplayDuration = 3;
std::vector<int> gSequence(gSequenceLength);

struct position {
    float x, y = 0.0;
};

position gButtonPositions[BUTTON_SURFACE_TOTAL];

// Functions
int buttonHandler(int pressedKey, int c);
bool init();
SDL_Surface* loadSurface(std::string path);
bool loadAssets();
void calculateSurfaceScale();
std::vector<int> generateSequence(int length);
void close();
