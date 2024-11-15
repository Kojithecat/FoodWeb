#include "game.hh"
#include "raylib.h"
#include <cstdint>
#include <vector>
#include <random>
#include <iostream>
#include <queue>

using u32 = uint_least32_t;
using engine = std::mt19937;


std::random_device os_seed;
const u32 seed = os_seed();
engine generator(seed);
std::uniform_int_distribution<u32> distribute(1,4);

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
    Player p = initPlayer(0, 0);
    Enemy e1 = initEnemy(650,300,RAT);
    Enemy e2 = initEnemy(300, 450, BAT);
    Player w;
    w.x = GetScreenWidth()-50;
    w.y = GetScreenHeight()-50;
    //Uniform random umber generator from 1 to 4
    float moveTimer = 0.0f;


    while(!IsKeyPressed(KEY_ESCAPE)){

        p.moveTime += GetFrameTime();
        e1.moveTime += GetFrameTime();
        e2.moveTime += GetFrameTime();

        checkPlayerMovement(p);
        movePlayer(p, filledMap);
        moveEnemy(e1, filledMap);
        moveEnemy(e2, filledMap);

        BeginDrawing();
            ClearBackground(RAYWHITE);
            for(int i = 0; i < filledMap.size(); i++){
                for(int j = 0; j < filledMap[i].size(); j++){
                    if(filledMap[i][j]) DrawRectangle(i*50, j*50, 50, 50, ORANGE);
                    }
                }
            DrawRectangle(w.x, w.y, 50, 50, GREEN);
            DrawRectangle(p.x, p.y, 50, 50, BLACK);
            DrawRectangle(e1.x,e1.y, 50, 50, RED);
            DrawRectangle(e2.x,e2.y, 50, 50, RED);

        EndDrawing();
        //Check player's death'
        if(e1.x == p.x && e1.y == p.y || e2.x == p.x && e2.y == p.y) return MENUSCREEN;
        if(w.x == p.x && w.y == p.y) return LVL1;
    }
    return MENUSCREEN;
}

Player initPlayer(int x, int y){
    Player p;
    p.x = x;
    p.y = y;
    p.moveTime = 0;
    std::queue<char> playerMoves;
    //Further initialization steps...
    return p;
}

Enemy initEnemy(int x, int y, ETYPE type){
    Enemy e;
    e.x = x;
    e.y = y;
    e.type = type;
    e.moveTime = 0;
    //switch(type) //set texture according to the enemy type
    return e;
}

void checkPlayerMovement(Player &p){

    if(IsKeyDown(KEY_W))// && p.y > 0)
        p.playerMoves.push('w');//p.y -= 50;
    if(IsKeyDown(KEY_A))// && p.x > 0)
        p.playerMoves.push('a');//p.x -= 50;
    if(IsKeyDown(KEY_S))// && GetScreenHeight()-50 > p.y)
        p.playerMoves.push('s');//p.y += 50;
    if(IsKeyDown(KEY_D))// && GetScreenWidth()-50 > p.x)
        p.playerMoves.push('d');//p.x +=50;
}

void movePlayer(Player &p, std::vector<std::vector<bool>> &map){

    if(p.moveTime>=MOVE_INTERVAL){
        //Player movement
        if(!p.playerMoves.empty()){
            char Pmov = p.playerMoves.back();
            if(Pmov == 'w' && p.y > 0)
                p.y -= 50;
            if(Pmov == 'a' && p.x > 0)
                p.x -= 50;
            if(Pmov == 's' && GetScreenHeight()-50 > p.y)
                p.y += 50;
            if(Pmov == 'd' && GetScreenWidth()-50 > p.x)
                p.x +=50;
            map[p.x/50][p.y/50] = false;
        }
        p.moveTime = 0.0f;
        //Clear movement queue
        p.playerMoves = std::queue<char>();
    }
}


void moveEnemy(Enemy &e, std::vector<std::vector<bool>> &map){

    if(e.moveTime >= BAT_MOVE_INTERVAL){
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

        map[e.x/50][e.y/50] = false;

        e.moveTime = 0.0f;
    }
}
