#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <stdarg.h>
#include "menu.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool init();

//The window we'll be rendering to
SDL_Window *gWindow = NULL;

//The surface contained by the window
SDL_Surface *gScreenSurface = NULL;

//the font stuff
TTF_Font *font;
SDL_Color fColor;
SDL_Rect fontRect;

//input
SDL_Event Event;



void freeSDL()
{

    //Destroy window
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    //Quit SDL subsystems
    SDL_Quit();
}

bool init()
{
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        //Create window
        gWindow = SDL_CreateWindow("LCD Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL)
        {
            fprintf(stderr, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            //Get window surface
            gScreenSurface = SDL_GetWindowSurface(gWindow);
        }
    }

    if (!success)
    {
        freeSDL();
    }

    return success;
}

//Initialize the font, set to white
void fontInit()
{
    TTF_Init();
    font = TTF_OpenFont("Tahoma.ttf", 18);
    fColor.r = 255;
    fColor.g = 255;
    fColor.b = 255;
}

//Print the designated string at the specified coordinates
void showTextAtLocation(char *c, int x, int y)
{
    //clear current Screen
    SDL_FillRect(gScreenSurface, NULL, 0x000000);

    //create text surface
    SDL_Surface *gTextSurface = TTF_RenderText_Solid(font, c, fColor);
    fontRect.x = x;
    fontRect.y = y;
    //blit it
    SDL_BlitSurface(gTextSurface, NULL, gScreenSurface, &fontRect);
    //free it
    SDL_FreeSurface(gTextSurface);

    //update current screen
    SDL_UpdateWindowSurface(gWindow);
}

void showText(char *fmt, ...)
{
    char buf[128]; // resulting string limited to 128 chars
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, 128, fmt, args);
    va_end(args);
    showTextAtLocation(buf, gScreenSurface->w / 2 - 11 * 3, gScreenSurface->h / 2);
}

int main(int argc, char *args[])
{
    //Start up SDL and create window

    if (!init())
    {
        fprintf(stderr, "Failed to initialize SDL!\n");
        //Failure here makes everything pointless, so exit early
        return 1;
    }

    
    if(!setupMenus()) {
        fprintf(stderr, "Failed to initialize Menus!\n");
        //Failure here makes everything pointless, so exit early
        return 1;
    }
    
    //Load fonts... should probably error check but w/e
    fontInit();

    //start!
     assignMenu(rootMenu);
     updateButtonPress(BTN_NONE);

    /*
     * Just keep looping and processing keys.
     * The SDL/Display part is just meant to emulate the LCD and buttons
     * Only special case is Escape... otherwise, just pass the button event
     */

    bool isLooping = true;
    while (isLooping)
    {
        while (SDL_PollEvent(&Event))
        {
            switch (Event.type)
            {

            case SDL_KEYDOWN:
                if (Event.key.keysym.sym == SDLK_ESCAPE)
                {
                    isLooping = false;
                }
                else
                {
                    updateButtonPress(Event.key.keysym.sym);
                }
                break;
            case SDL_QUIT:
                isLooping = false;
            default:
                updateButtonPress(BTN_NONE);
                break;
            }
        }

        SDL_Delay(10);
    }

    freeSDL();
    freeMenus();

    return 0;
}