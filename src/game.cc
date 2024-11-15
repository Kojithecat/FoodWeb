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
    std::vector<std::vector<bool>> filledMap(GetScreenWidth()/TILESIZE, std::vector<bool>(GetScreenHeight()/TILESIZE,true));

    filledMap[13][5] = false;
    filledMap[13][6] = false;
    filledMap[13][7] = false;

    filledMap[6][5] = false;
    filledMap[6][6] = false;
    filledMap[6][7] = false;


    Player p = initPlayer(0, 0);
    Enemy e1 = initEnemy(13*TILESIZE,6*TILESIZE,RAT);
    Enemy e2 = initEnemy(6*TILESIZE, 6*TILESIZE, BAT);
    LevelGoal w = initGoal(GetScreenWidth()-TILESIZE, GetScreenHeight()-TILESIZE, true);
    //Uniform random umber generator from 1 to 4

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
                    if(filledMap[i][j]) DrawRectangle(i*TILESIZE, j*TILESIZE, TILESIZE, TILESIZE, ORANGE);
                    }
                }
            DrawRectangle(w.x, w.y, TILESIZE, TILESIZE, GREEN);
            DrawRectangle(p.x, p.y, TILESIZE, TILESIZE, BLACK);
            DrawRectangle(e1.x,e1.y, TILESIZE, TILESIZE, RED);
            DrawRectangle(e2.x,e2.y, TILESIZE, TILESIZE, RED);

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

LevelGoal initGoal(int x, int y, bool open){
    LevelGoal g;
    g.x = x;
    g.y = y;
    g.open = open;

    return g;
}

void checkPlayerMovement(Player &p){

    if(IsKeyDown(KEY_W))// && p.y > 0)
        p.playerMoves.push('w');//p.y -= TILESIZE;
    if(IsKeyDown(KEY_A))// && p.x > 0)
        p.playerMoves.push('a');//p.x -= TILESIZE;
    if(IsKeyDown(KEY_S))// && GetScreenHeight()-TILESIZE > p.y)
        p.playerMoves.push('s');//p.y += TILESIZE;
    if(IsKeyDown(KEY_D))// && GetScreenWidth()-TILESIZE > p.x)
        p.playerMoves.push('d');//p.x +=TILESIZE;
}

void movePlayer(Player &p, std::vector<std::vector<bool>> &map){

    if(p.moveTime>=MOVE_INTERVAL){
        //Player movement
        if(!p.playerMoves.empty()){
            char Pmov = p.playerMoves.back();
            if(Pmov == 'w' && p.y > 0)
                p.y -= TILESIZE;
            if(Pmov == 'a' && p.x > 0)
                p.x -= TILESIZE;
            if(Pmov == 's' && GetScreenHeight()-TILESIZE > p.y)
                p.y += TILESIZE;
            if(Pmov == 'd' && GetScreenWidth()-TILESIZE > p.x)
                p.x +=TILESIZE;
            map[p.x/TILESIZE][p.y/TILESIZE] = false;
        }
        p.moveTime = 0.0f;
        //Clear movement queue
        p.playerMoves = std::queue<char>();
    }
}


void moveEnemy(Enemy &e, std::vector<std::vector<bool>> &map){

    int width = GetScreenWidth();
    int height = GetScreenHeight();

    int relx = e.x/TILESIZE; //Tile position
    int rely = e.y/TILESIZE;

    bool leftFilled = relx > 0 && map[relx-1][rely];
    bool rightFilled = relx < width -1 && map[relx+1][rely];
    bool topFilled = rely > 0 && map[relx][rely-1];
    bool botFilled = rely < height -1 && map[relx][rely+1];


    switch(e.type){
        case BAT:
            if(e.moveTime >= BAT_MOVE_INTERVAL){
                //Enemy movement
                int Emov = distribute(generator);
                if(Emov==1 && e.y > 0 && !topFilled)
                    e.y -= TILESIZE;
                if(Emov==2 && e.x > 0 && !leftFilled)
                    e.x -= TILESIZE;
                if(Emov==3 && GetScreenHeight()-TILESIZE > e.y && !botFilled)
                    e.y += TILESIZE;
                if(Emov==4 && GetScreenWidth()-TILESIZE > e.x && !rightFilled)
                    e.x +=TILESIZE;

                //map[e.x/TILESIZE][e.y/TILESIZE] = false;
                e.moveTime = 0.0f;
            }
            break;
        case SNAKE:
            if(e.moveTime >= SNAKE_MOVE_INTERVAL){
                //Enemy movement
                int Emov = distribute(generator);
                if(Emov==1 && e.y > 0)
                            e.y -= TILESIZE;
                if(Emov==2 && e.x > 0)
                    e.x -= TILESIZE;
                if(Emov==3 && GetScreenHeight()-TILESIZE > e.y)
                    e.y += TILESIZE;
                if(Emov==4 && GetScreenWidth()-TILESIZE > e.x)
                    e.x +=TILESIZE;

                //map[e.x/TILESIZE][e.y/TILESIZE] = false;

                e.moveTime = 0.0f;
            }
            break;
        case SCORPION:
            if(e.moveTime >= SCORPION_MOVE_INTERVAL){
                //Enemy movement
                int Emov = distribute(generator);
                if(Emov==1 && e.y > 0 )
                    e.y -= TILESIZE;
                if(Emov==2 && e.x > 0)
                    e.x -= TILESIZE;
                if(Emov==3 && GetScreenHeight()-TILESIZE > e.y)
                    e.y += TILESIZE;
                if(Emov==4 && GetScreenWidth()-TILESIZE > e.x)
                    e.x +=TILESIZE;

                //map[e.x/TILESIZE][e.y/TILESIZE] = false;

                e.moveTime = 0.0f;
            }
            break;
        case RAT:
            if(e.moveTime >= RAT_MOVE_INTERVAL){
                //Enemy movement
                int Emov = distribute(generator);
                if(Emov==1 && e.y > 0 && !topFilled)
                    e.y -= TILESIZE;
                if(Emov==2 && e.x > 0 && !leftFilled)
                    e.x -= TILESIZE;
                if(Emov==3 && GetScreenHeight()-TILESIZE > e.y && !botFilled)
                    e.y += TILESIZE;
                if(Emov==4 && GetScreenWidth()-TILESIZE > e.x && !rightFilled)
                    e.x +=TILESIZE;

                //map[e.x/TILESIZE][e.y/TILESIZE] = false;
                e.moveTime = 0.0f;
            }
            break;
    }
}
