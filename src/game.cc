#include "game.hh"
#include "raylib.h"
#include <cstdint>
#include <vector>
#include <random>
#include <iostream>
#include <queue>

using u32 = uint_least32_t;
using engine = std::mt19937;



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
    Player p;
    p.x = 0;
    p.y = 0;
    Player e;
    e.x = 650;
    e.y = 300;
    Player w;
    w.x = GetScreenWidth()-50;
    w.y = GetScreenHeight()-50;
    std::random_device os_seed;
    const u32 seed = os_seed();
    engine generator(seed);
    std::uniform_int_distribution<u32> distribute(1,4);
    float moveTimer = 0.0f;
    std::queue<char> playerMoves;
    while(!IsKeyPressed(KEY_ESCAPE)){

        moveTimer += GetFrameTime();
        if(IsKeyDown(KEY_W))// && p.y > 0)
            playerMoves.push('w');//p.y -= 50;
        if(IsKeyDown(KEY_A))// && p.x > 0)
            playerMoves.push('a');//p.x -= 50;
        if(IsKeyDown(KEY_S))// && GetScreenHeight()-50 > p.y)
            playerMoves.push('s');//p.y += 50;
        if(IsKeyDown(KEY_D))// && GetScreenWidth()-50 > p.x)
            playerMoves.push('d');//p.x +=50;

        if(moveTimer >=MOVE_INTERVAL){
            //Player movement
            if(!playerMoves.empty()){
                char Pmov = playerMoves.back();
                if(Pmov == 'w' && p.y > 0)
                    p.y -= 50;
                if(Pmov == 'a' && p.x > 0)
                    p.x -= 50;
                if(Pmov == 's' && GetScreenHeight()-50 > p.y)
                    p.y += 50;
                if(Pmov == 'd' && GetScreenWidth()-50 > p.x)
                    p.x +=50;
                filledMap[p.x/50][p.y/50] = false;
            }
            //Enemy movement
            int Emov = distribute(generator);
            if(Emov==1 && e.y > 0)
                e.y -= 50;
            if(Emov==2 && e.x > 0)
                e.x -= 50;
            if(Emov==3 && GetScreenHeight()-50 > e.y)
                e.y += 50;
            if(Emov==4 && GetScreenWidth()-50 > e.x)
                e.x +=50;

            filledMap[e.x/50][e.y/50] = false;
            moveTimer = 0.0f;

            //Clear movement queue
            playerMoves = std::queue<char>();

        }

        BeginDrawing();
            ClearBackground(RAYWHITE);
            for(int i = 0; i < filledMap.size(); i++){
                for(int j = 0; j < filledMap[i].size(); j++){
                    if(filledMap[i][j]) DrawRectangle(i*50, j*50, 50, 50, ORANGE);
                    }
                }

            DrawRectangle(w.x, w.y, 50, 50, GREEN);
            DrawRectangle(p.x, p.y, 50, 50, BLACK);
            DrawRectangle(e.x,e.y, 50, 50, RED);
        EndDrawing();
        //Check player's death'
        if(e.x == p.x && e.y == p.y) return MENUSCREEN;
        if(w.x == p.x && w.y == p.y) return LVL1;
    }
    return MENUSCREEN;

}
