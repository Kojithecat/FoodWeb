#include "game.hh"
#include "raylib.h"
#include <iostream>
#include <cstdint>
#include <utility>
#include <random>
#include <set>
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
    std::vector<std::vector<Casilla>> levelMap(GetScreenWidth()/TILESIZE, std::vector<Casilla>(GetScreenHeight()/TILESIZE));

    Player p = initPlayer(0, 0);

    Enemy e1 = initEnemy(13,4,RAT);
    Enemy e2 = initEnemy(6, 6, BAT);

    Rock r1 = initRock(6, 3);
    Rock r2 = initRock(13, 1);

    LevelGoal w = initGoal(GetScreenWidth()/TILESIZE - 1, GetScreenHeight()/TILESIZE - 1, true);
    //Uniform random umber generator from 1 to 4
    std::vector<Enemy> enemyVector;
    enemyVector.push_back(e1);
    enemyVector.push_back(e2);
    std::vector<Rock> rockVector;
    rockVector.push_back(r1);
    rockVector.push_back(r2);

    std::set<std::pair<int,int>> sandlessCasillas;

    sandlessCasillas.insert(std::make_pair(6, 3));
    sandlessCasillas.insert(std::make_pair(6, 5));
    sandlessCasillas.insert(std::make_pair(6, 6));
    sandlessCasillas.insert(std::make_pair(6, 7));
    sandlessCasillas.insert(std::make_pair(13, 3));
    sandlessCasillas.insert(std::make_pair(13, 4));
    sandlessCasillas.insert(std::make_pair(13, 5));
    sandlessCasillas.insert(std::make_pair(0, 0));
    sandlessCasillas.insert(std::make_pair(13, 1));

    fillMap(levelMap, p, w, enemyVector, rockVector, sandlessCasillas);

    while(!IsKeyPressed(KEY_ESCAPE)){

        p.moveTime += GetFrameTime();
        for(Enemy e : enemyVector){
            e.moveTime += GetFrameTime();
            std::cout << e.moveTime << std::endl;
        }
        for(Rock r : rockVector)
            r.moveTime += GetFrameTime();

        checkPlayerMovement(p);
        movePlayer(p, levelMap);
        moveEnemies(enemyVector, levelMap);
        moveRock(r1, enemyVector, p, levelMap);
        moveRock(r2, enemyVector, p, levelMap);

        BeginDrawing();
            ClearBackground(RAYWHITE);
            for(int i = 0; i < levelMap.size(); i++){
                for(int j = 0; j < levelMap[i].size(); j++){
                    if(levelMap[i][j].isFill) DrawRectangle(i*TILESIZE, j*TILESIZE, TILESIZE, TILESIZE, ORANGE);
                    }
                }
            DrawRectangle(w.x*TILESIZE, w.y*TILESIZE, TILESIZE, TILESIZE, GREEN);
            DrawRectangle(p.x*TILESIZE, p.y*TILESIZE, TILESIZE, TILESIZE, BLACK);
            for(Enemy e : enemyVector)
                if(!e.dead)
                    DrawRectangle(e.x*TILESIZE, e.y*TILESIZE, TILESIZE, TILESIZE, RED);
            for(Rock r : rockVector)
                DrawRectangle(r.x*TILESIZE, r.y*TILESIZE, TILESIZE, TILESIZE, BROWN);

        EndDrawing();
        //Check player's death'
        if(r1.x == e1.x && r1.y == e1.y || r2.x == e1.x && r2.y == e1.y) e1.dead = true;
        if(r1.x == e2.x && r1.y == e2.y || r2.x == e2.x && r2.y == e2.y) e2.dead = true;
        if(e1.x == p.x && e1.y == p.y && !e1.dead || e2.x == p.x && e2.y == p.y && !e2.dead) return MENUSCREEN;
        if(w.x == p.x && w.y == p.y) return LVL1;

        //std::cout << e1.x << " " << e1.y << std::endl;
    }
    return MENUSCREEN;
}

int fillMap(std::vector<std::vector<Casilla>> &levelMap,Player &p, LevelGoal &w, std::vector<Enemy> &enemyVector, std::vector<Rock> &rockVector, std::set<std::pair<int,int>> sandlessSet){

    for(int i = 0; i < levelMap.size(); i++) {
        for(int j = 0; j <levelMap[i].size(); j++){
            if(sandlessSet.find(std::make_pair(i,j)) != sandlessSet.end()){
                levelMap[i][j].isFill = false;
            }
            else levelMap[i][j].isFill = true;
            levelMap[i][j].isEnemy = false;
            levelMap[i][j].isGoal = false;
            levelMap[i][j].isRock = false;
            levelMap[i][j].isPlayer = false;
        }
    }
    levelMap[p.x][p.y].isPlayer = true;
    levelMap[w.x][w.y].isGoal = true;
    for(Enemy e : enemyVector)
    levelMap[e.x][e.y].isEnemy = true;
    for(Rock r : rockVector)
    levelMap[r.x][r.y].isRock = true;

    return 0;
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

void movePlayer(Player &p, std::vector<std::vector<Casilla>> &map){

    if(p.moveTime>=MOVE_INTERVAL){
        //Player movement
        int prex = p.x;
        int prey = p.y;
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
            map[p.x][p.y].isFill = false;
        }
        p.moveTime = 0.0f;
        //Clear movement queue
        p.playerMoves = std::queue<char>();
        //Update map
        map[prex][prey].isPlayer = false;
        map[p.x][p.y].isPlayer = true;
    }
}

void moveEnemies(std::vector<Enemy> &enemyVector, std::vector<std::vector<Casilla>> &map){
    for(Enemy e : enemyVector){
        int prex = e.x;
        int prey = e.y;
        moveEnemy(e, map);
        if(prex != e.x || prey != e.y){
            map[prex][prey].isEnemy = false;
            map[e.x][e.y].isEnemy = true;
        }
    }
}

void moveEnemy(Enemy &e, std::vector<std::vector<Casilla>> &map){

    int width = GetScreenWidth()/TILESIZE;
    int height = GetScreenHeight();

    bool leftFilled = e.x > 0 && map[e.x-1][e.y].isFill;
    bool rightFilled = e.x < width - 1 && map[e.x+1][e.y].isFill;
    bool topFilled = e.y > 0 && map[e.x][e.y-1].isFill;
    bool botFilled = e.y < height - 1 && map[e.x][e.y+1].isFill;
    //std::cout << leftFilled << rightFilled << topFilled << botFilled << std::endl;

    switch(e.type){
        case BAT:
            std::cout << "Hii" << std::endl;
            if(e.moveTime >= BAT_MOVE_INTERVAL){
                std::cout << "hiii" << std::endl;
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
                std::cout << "hiii" << std::endl;
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
                std::cout << "hiii" << std::endl;
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
                std::cout << "hiii" << std::endl;
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

void moveRock(Rock &r, std::vector<Enemy> &enemyVector, Player &p, std::vector<std::vector<Casilla>> &map){

    if(!map[r.x][r.y+1].isFill && !(r.x == p.x && r.y + 1 == p.y) && r.moveTime >= ROCK_FALL_INTERVAL){
        r.moveTime = 0;
        map[r.x][r.y].isRock = false;
        r.y += 1;
        map[r.x][r.y].isRock = true;
    }
}
