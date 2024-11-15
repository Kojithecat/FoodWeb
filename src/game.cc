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

    Player p = initPlayer(0, 0);
    filledMap[0][0] = false;

    Enemy e1 = initEnemy(13,6,RAT);
    filledMap[13][5] = false;
    filledMap[13][6] = false;
    filledMap[13][7] = false;

    Enemy e2 = initEnemy(6, 6, BAT);
    filledMap[6][5] = false;
    filledMap[6][6] = false;
    filledMap[6][7] = false;

    Rock r = initRock(6, 3);
    filledMap[6][3] = false;

    LevelGoal w = initGoal(GetScreenWidth()/TILESIZE - 1, GetScreenHeight()/TILESIZE - 1, true);
    //Uniform random umber generator from 1 to 4
    std::vector<Enemy> enemyVector;
    enemyVector.push_back(e1);
    enemyVector.push_back(e2);

    while(!IsKeyPressed(KEY_ESCAPE)){

        p.moveTime += GetFrameTime();
        e1.moveTime += GetFrameTime();
        e2.moveTime += GetFrameTime();
        r.moveTime += GetFrameTime();

        checkPlayerMovement(p);
        movePlayer(p, filledMap);
        moveEnemy(e1, filledMap);
        moveEnemy(e2, filledMap);
        moveRock(r, enemyVector, p, filledMap);

        BeginDrawing();
            ClearBackground(RAYWHITE);
            for(int i = 0; i < filledMap.size(); i++){
                for(int j = 0; j < filledMap[i].size(); j++){
                    if(filledMap[i][j]) DrawRectangle(i*TILESIZE, j*TILESIZE, TILESIZE, TILESIZE, ORANGE);
                    }
                }
            DrawRectangle(w.x*TILESIZE, w.y*TILESIZE, TILESIZE, TILESIZE, GREEN);
            DrawRectangle(p.x*TILESIZE, p.y*TILESIZE, TILESIZE, TILESIZE, BLACK);
            if(!e1.dead) DrawRectangle(e1.x*TILESIZE, e1.y*TILESIZE, TILESIZE, TILESIZE, RED);
            if(!e2.dead) DrawRectangle(e2.x*TILESIZE, e2.y*TILESIZE, TILESIZE, TILESIZE, RED);
            DrawRectangle(r.x*TILESIZE, r.y*TILESIZE, TILESIZE, TILESIZE, BROWN);

        EndDrawing();
        //Check player's death'
        if(r.x == e1.x && r.y == e1.y) e1.dead = true;
        if(r.x == e2.x && r.y == e2.y) e2.dead = true;
        if(e1.x == p.x && e1.y == p.y && !e1.dead || e2.x == p.x && e2.y == p.y && !e2.dead) return MENUSCREEN;
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
    e.dead = false;
    e.type = type;
    e.moveTime = 0;
    //switch(type) //set texture according to the enemy type
    return e;
}

Rock initRock(int x, int y){
    Rock r;
    r.x = x;
    r.y = y;
    r.moveTime = 0.0f;

    return r;

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
                p.y -= 1;
            if(Pmov == 'a' && p.x > 0)
                p.x -= 1;
            if(Pmov == 's' && GetScreenHeight()/TILESIZE - 1 > p.y)
                p.y += 1;
            if(Pmov == 'd' && GetScreenWidth()/TILESIZE - 1 > p.x)
                p.x +=1;
            map[p.x][p.y] = false;
        }
        p.moveTime = 0.0f;
        //Clear movement queue
        p.playerMoves = std::queue<char>();
    }
}


void moveEnemy(Enemy &e, std::vector<std::vector<bool>> &map){

    int width = GetScreenWidth()/TILESIZE;
    int height = GetScreenHeight();

    bool leftFilled = e.x > 0 && map[e.x-1][e.y];
    bool rightFilled = e.x < width - 1 && map[e.x+1][e.y];
    bool topFilled = e.y > 0 && map[e.x][e.y-1];
    bool botFilled = e.y < height - 1 && map[e.x][e.y+1];


    switch(e.type){
        case BAT:
            if(e.moveTime >= BAT_MOVE_INTERVAL){
                //Enemy movement
                int Emov = distribute(generator);
                if(Emov==1 && e.y > 0 && !topFilled)
                    e.y -= 1;
                if(Emov==2 && e.x > 0 && !leftFilled)
                    e.x -= 1;
                if(Emov==3 && height - 1 > e.y && !botFilled)
                    e.y += 1;
                if(Emov==4 && width- 1 > e.x && !rightFilled)
                    e.x += 1;

                //map[e.x/TILESIZE][e.y/TILESIZE] = false;
                e.moveTime = 0.0f;
            }
            break;
        case SNAKE:
            if(e.moveTime >= SNAKE_MOVE_INTERVAL){
                //Enemy movement
                int Emov = distribute(generator);
                if(Emov==1 && e.y > 0)
                    e.y -= 1;
                if(Emov==2 && e.x > 0)
                    e.x -= 1;
                if(Emov==3 && height - 1 > e.y)
                    e.y += 1;
                if(Emov==4 && width - 1 > e.x)
                    e.x +=1;

                //map[e.x/TILESIZE][e.y/TILESIZE] = false;

                e.moveTime = 0.0f;
            }
            break;
        case SCORPION:
            if(e.moveTime >= SCORPION_MOVE_INTERVAL){
                //Enemy movement
                int Emov = distribute(generator);
                if(Emov==1 && e.y > 0 )
                    e.y -= 1;
                if(Emov==2 && e.x > 0)
                    e.x -= 1;
                if(Emov==3 && height - 1 > e.y)
                    e.y += 1;
                if(Emov==4 && width - 1 > e.x)
                    e.x += 1;

                //map[e.x/TILESIZE][e.y/TILESIZE] = false;

                e.moveTime = 0.0f;
            }
            break;
        case RAT:
            if(e.moveTime >= RAT_MOVE_INTERVAL){
                //Enemy movement
                int Emov = distribute(generator);
                if(Emov==1 && e.y > 0 && !topFilled)
                    e.y -= 1;
                if(Emov==2 && e.x > 0 && !leftFilled)
                    e.x -= 1;
                if(Emov==3 && height -1 > e.y && !botFilled)
                    e.y += 1;
                if(Emov==4 && width -1 > e.x && !rightFilled)
                    e.x +=1;

                //map[e.x/TILESIZE][e.y/TILESIZE] = false;
                e.moveTime = 0.0f;
            }
            break;
    }
}

void moveRock(Rock &r, std::vector<Enemy> &enemyVector, Player &p, std::vector<std::vector<bool>> &map){

    if(!map[r.x][r.y+1] && !(r.x == p.x && r.y + 1 == p.y) && r.moveTime >= ROCK_FALL_INTERVAL){
        r.moveTime = 0;
        r.y += 1;
    }
}
