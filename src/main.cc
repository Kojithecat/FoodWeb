#include <iostream>
#include "raylib.h"
#include "game.hh"

int main(){

    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

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
            currentScreen = runTestLevel();
            //currentScreen = runLevel(pathToLVL,LVL1,LVL2)

        }
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
    return 0;

}
