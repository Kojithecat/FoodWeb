#include <iostream>
#include "raylib.h"
#include "game.hh"

int main(){

    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 16*TILESIZE;
    const int screenHeight = 12*TILESIZE;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    SCREEN currentScreen = MENUSCREEN;
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        switch(currentScreen){
            case MENUSCREEN:
                currentScreen = showMenuScreen();
                break;
            case LVL1:
                currentScreen = runLevel(1);
                break;
            case LVL2:
                currentScreen = runLevel(2);
                break;
            case LVL3:
                currentScreen = runLevel(3);
                break;
            case LVL4:
                currentScreen = runLevel(4);
                break;
            case LVL5:
                currentScreen = runLevel(5);
                break;
            defalt:
                currentScreen = MENUSCREEN;

            //currentScreen = runLevel(pathToLVL,LVL1,LVL2)

        }
    }

    // De-Initialization
   //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
    return 0;

}
