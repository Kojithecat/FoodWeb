#include "game.hh"
#include "raylib.h"
#include <vector>

struct Player {

    int x;
    int y;
    Texture2D texture;
    //std:vector<Item> objects;

};

struct Casilla{
    bool filled;
    //...
};

struct Coord{
    int x;
    int y;
};

SCREEN showMenuScreen(){
    BeginDrawing();

        ClearBackground(RAYWHITE);
        DrawText("Press ENTER to begin", 190, 200, 20, LIGHTGRAY);

    EndDrawing();
    if(IsKeyPressed(KEY_ENTER))
        return LVL1;
    else return MENUSCREEN;
}


SCREEN runTestLevel(){
    std::vector<std::vector<bool>> filledMap(GetScreenWidth()/50, std::vector<bool>(GetScreenHeight()/50,true));
    Coord playerPosition;
    playerPosition.x = 0;
    playerPosition.y = 0;
    while(!IsKeyPressed(KEY_ESCAPE)){
        BeginDrawing();
            ClearBackground(RAYWHITE);
            for(int i = 0; i < filledMap.size(); i++){
                for(int j = 0; j < filledMap[i].size(); j++){
                    if(filledMap[i][j]) DrawRectangle(i*50, j*50, 50, 50, ORANGE);
                }
            }
            DrawRectangle(playerPosition.x, playerPosition.y, 50, 50, BLACK);
        EndDrawing();

        if(IsKeyPressed(KEY_W) && playerPosition.y > 0)
            playerPosition.y -= 50;
        if(IsKeyPressed(KEY_A) && playerPosition.x > 0)
            playerPosition.x -= 50;
        if(IsKeyPressed(KEY_S) && GetScreenHeight() > playerPosition.y)
            playerPosition.y += 50;
        if(IsKeyPressed(KEY_D) && GetScreenWidth() > playerPosition.x)
            playerPosition.x +=50;
        filledMap[playerPosition.x/50][playerPosition.y/50] = false;
    }
    return MENUSCREEN;
}
