#include "game.hh"
#include "raylib.h"
#include <algorithm>
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

int width = 15;
int height = 10;

const int SCREEN_TILES_X = 20;
const int SCREEN_TILES_Y = 15;

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
    std::vector<std::vector<Casilla>> levelMap(width, std::vector<Casilla>(height));

    Player p = initPlayer(0, 0);

    Enemy e1 = initEnemy(13,4,RAT);
    Enemy e2 = initEnemy(6, 6, BAT);

    //Rock r1 = initRock(6, 3);
    Rock r2 = initRock(13, 1);

    Bomb b = initBomb(6,3);

    LevelGoal w = initGoal(width - 1, height - 1, true);
    //Uniform random umber generator from 1 to 4
    std::vector<Enemy> enemyVector;
    enemyVector.push_back(e1);
    enemyVector.push_back(e2);

    std::vector<Rock> rockVector;
    //rockVector.push_back(r1);
    rockVector.push_back(r2);

    std::vector<Bomb> bombVector;
    bombVector.push_back(b);

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

    Camera2D camera;
    camera.offset =  {(float) p.x,(float) p.y};
    camera.target = {(float) p.x,(float) p.y};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    while(!IsKeyPressed(KEY_ESCAPE)){

        for(Enemy& e : enemyVector){
            //std::cout << e.y << std::endl;
            if(collision(e,p) && !e.dead) return MENUSCREEN;
            for(Rock& r: rockVector){
               if(collision(e,r))  e.dead = true;
            }
        }
        if(collision(w,p)) return LVL1;

        p.moveTime += GetFrameTime();
        for(Enemy& e : enemyVector){
            e.moveTime += GetFrameTime();
            //std::cout << e.moveTime << std::endl;
        }
        for(Rock& r : rockVector)
            r.moveTime += GetFrameTime();
        for(Bomb& b : bombVector)
            b.moveTime += GetFrameTime();

        checkPlayerMovement(p);
        for(Rock& r : rockVector)
            fallRock(r, enemyVector, p, levelMap);
        for(Bomb& b : bombVector){
            fallBomb(b, enemyVector, p, levelMap);
        }
        movePlayer(p, rockVector, bombVector, levelMap);
        moveEnemies(enemyVector, levelMap);

        camera.target = {(float) p.x*TILESIZE - 5*TILESIZE,(float) p.y*TILESIZE - 5*TILESIZE};

        BeginDrawing();
        BeginMode2D(camera);
            ClearBackground(RAYWHITE);
            for(int i = 0; i < levelMap.size(); i++){
                for(int j = 0; j < levelMap[i].size(); j++){
                    if(levelMap[i][j].isFill) DrawRectangle(i*TILESIZE, j*TILESIZE, TILESIZE, TILESIZE, ORANGE);
                    }
                }
            DrawRectangle(w.x*TILESIZE, w.y*TILESIZE, TILESIZE, TILESIZE, GREEN);
            DrawRectangle(p.x*TILESIZE, p.y*TILESIZE, TILESIZE, TILESIZE, BLACK);
            for(Enemy& e : enemyVector)
                if(!e.dead)
                    DrawRectangle(e.x*TILESIZE, e.y*TILESIZE, TILESIZE, TILESIZE, RED);
            for(Rock& r : rockVector)
                DrawRectangle(r.x*TILESIZE, r.y*TILESIZE, TILESIZE, TILESIZE, BROWN);
            for(Bomb& b : bombVector)
                DrawRectangle(b.x*TILESIZE, b.y*TILESIZE, TILESIZE, TILESIZE, PURPLE);
        EndMode2D();
        EndDrawing();

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
    for(Enemy& e : enemyVector)
    levelMap[e.x][e.y].isEnemy = true;
    for(Rock& r : rockVector)
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
    //define move interval
    switch(type){
        case BAT:
            e.intervalMove = BAT_MOVE_INTERVAL;
            break;
        case RAT:
            e.intervalMove = RAT_MOVE_INTERVAL;
            break;
        case SNAKE:
            e.intervalMove = SNAKE_MOVE_INTERVAL;
            break;
        case SCORPION:
            e.intervalMove = SCORPION_MOVE_INTERVAL;
            break;
    }
    //switch(type) //set texture according to the enemy type
    return e;
}

Rock initRock(int x, int y){
    Rock r;
    r.x = x;
    r.y = y;
    r.moveTime = 0.0f;
    r.falling = false;

    return r;
}

Bomb initBomb(int x, int y){
    Bomb b;
    b.x = x;
    b.y = y;
    b.moveTime = 0.0f;
    b.falling = false;

    return b; //Can I simplify the bomb and rock code into one?
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

void movePlayer(Player &p, std::vector<Rock> &rockVector, std::vector<Bomb> &bombVector, std::vector<std::vector<Casilla>> &map){

    if(p.moveTime>=MOVE_INTERVAL){
        //Player movement
        int prex = p.x;
        int prey = p.y;
        char Pmov = ' ';
        if(!p.playerMoves.empty()){
            Pmov = p.playerMoves.back();
            if(Pmov == 'w' && p.y > 0)
                p.y -= 1;
            if(Pmov == 'a' && p.x > 0)
                p.x -= 1;
            if(Pmov == 's' && height - 1 > p.y)
                p.y += 1;
            if(Pmov == 'd' && width - 1 > p.x)
                p.x +=1;
            map[p.x][p.y].isFill = false;
        }
        p.moveTime = 0.0f;
        //Clear movement queue
        p.playerMoves = std::queue<char>();
        //Update map
        map[prex][prey].isPlayer = false;
        map[p.x][p.y].isPlayer = true;
        //Move rocks in front of the player
        for(Rock& r : rockVector){
            if(collision(r, p) && Pmov == 'a'){
                std::cout << "hii" << std::endl;
                moveObject(r, map, -1, 0);
            }
            else if(collision(r, p) && Pmov == 'd'){
                moveObject(r, map, 1, 0);
            }
            else if(collision(r, p) && Pmov == 'w'){
                moveObject(r, map, 0, -1);
            }
            else if(collision(r, p) && Pmov == 's'){
                moveObject(r, map, 0, 1);
            }
        }
        for(Bomb& b : bombVector){
            if(collision(b, p) && Pmov == 'a'){
                std::cout << "hii" << std::endl;
                moveObject(b, map, -1, 0);
            }
            else if(collision(b, p) && Pmov == 'd'){
                moveObject(b, map, 1, 0);
            }
            else if(collision(b, p) && Pmov == 'w'){
                moveObject(b, map, 0, -1);
            }
            else if(collision(b, p) && Pmov == 's'){
                moveObject(b, map, 0, 1);
            }
        }
    }
}

void moveEnemies(std::vector<Enemy> &enemyVector, std::vector<std::vector<Casilla>> &map){
    for(Enemy& e : enemyVector){
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

    bool leftFilled = e.x > 0 && map[e.x-1][e.y].isFill;
    bool rightFilled = e.x < width - 1 && map[e.x+1][e.y].isFill;
    bool topFilled = e.y > 0 && map[e.x][e.y-1].isFill;
    bool botFilled = e.y < height - 1 && map[e.x][e.y+1].isFill;
    //std::cout << leftFilled << rightFilled << topFilled << botFilled << std::endl;

    std::vector<std::pair<int,int>> allowedMovements;
    if(e.y > 0 && !topFilled)
        allowedMovements.push_back(std::make_pair(e.x, e.y - 1));
    if(e.x > 0 && !leftFilled)
        allowedMovements.push_back(std::make_pair(e.x - 1, e.y));
    if(height - 1 > e.y && !botFilled)
        allowedMovements.push_back(std::make_pair(e.x, e.y + 1));
    if(width - 1 > e.x && !rightFilled)
        allowedMovements.push_back(std::make_pair(e.x + 1, e.y));

            //std::cout << "Hii" << std::endl;
    if(e.moveTime >= e.intervalMove){
        //std::cout << "hiii" << std::endl;
        if(allowedMovements.size()){
            std::uniform_int_distribution<u32> randomMove(0, allowedMovements.size() -1);
            auto Emov = allowedMovements[randomMove(generator)];
            map[e.x][e.y].isEnemy = false;
            e.x = Emov.first;
            e.y = Emov.second;
            map[e.x][e.y].isEnemy = true;
            }
        //Enemy movement
        /*
        int Emov = distribute(generator);
        if(Emov==1 && e.y > 0 && !topFilled)
            e.y -= 1;
        if(Emov==2 && e.x > 0 && !leftFilled)
            e.x -= 1;
        if(Emov==3 && height - 1 > e.y && !botFilled)
            e.y += 1;
        if(Emov==4 && width- 1 > e.x && !rightFilled)
            e.x += 1;
            */
        //map[e.x/TILESIZE][e.y/TILESIZE] = false;
        e.moveTime = 0.0f;
    }
}

void fallRock(Rock &r, std::vector<Enemy> &enemyVector, Player &p, std::vector<std::vector<Casilla>> &map){

    if(r.y +1 < map[0].size() //Rock would not fall outbounds
        && !map[r.x][r.y+1].isFill //The bottom tile is empty
        && !(r.x == p.x && r.y + 1 == p.y) //No player is below the rock
        && r.moveTime >= ROCK_FALL_INTERVAL){
        std::cout << r.y << " " << r.moveTime << std::endl;
        r.moveTime = 0;
        map[r.x][r.y].isRock = false;
        r.y += 1;
        map[r.x][r.y].isRock = true;
        r.falling = true;
    }
    else r.falling = false;
}


int fallBomb(Bomb &b, std::vector<Enemy> &enemyVector, Player &p, std::vector<std::vector<Casilla>> &map){

    if(b.y +1 < map[0].size() //Rock would not fall outbounds
        && !map[b.x][b.y+1].isFill //The bottom tile is empty
        && !(b.x == p.x && b.y + 1 == p.y) //No player is below the rock
        && b.moveTime >= ROCK_FALL_INTERVAL){
        //std::cout << b.y << " " << r.moveTime << std::endl;
        b.moveTime = 0;
        map[b.x][b.y].isBomb = false;
        b.y += 1;
        map[b.x][b.y].isBomb = true;
        b.falling = true;
        return 0;
    }
    else if(b.falling &&  b.moveTime >= ROCK_FALL_INTERVAL){
        b.falling = false;

        bool leftFilled = b.x > 0 && map[b.x-1][b.y].isFill;
        bool rightFilled = b.x < width - 1 && map[b.x+1][b.y].isFill;
        bool topFilled = b.y > 0 && map[b.x][b.y-1].isFill;
        bool botFilled = b.y < height - 1 && map[b.x][b.y+1].isFill;

        //TODO check bomb explosions in margins
        map[b.x+1][b.y].isFill = false;
        map[b.x+1][b.y+1].isFill = false;
        map[b.x+1][b.y-1].isFill = false;
        map[b.x-1][b.y].isFill = false;
        map[b.x-1][b.y+1].isFill = false;
        map[b.x-1][b.y-1].isFill = false;
        map[b.x][b.y+1].isFill = false;
        map[b.x][b.y-1].isFill = false;
        return 1; //Self destroy
    }
    return 0;
}

template<class T>
void moveObject(T &o, std::vector<std::vector<Casilla>> &map, int deltax, int deltay){
    if(typeid(o).name() == "Rock"){
        map[o.x][o.y].isRock = false;
        map[o.x + deltax][o.y + deltay].isRock = true;
        map[o.x][o.y].isFill = false;
    }
    else if(typeid(o).name() == "Bomb"){
        map[o.x][o.y].isBomb = false;
        map[o.x + deltax][o.y + deltay].isBomb = true;
        map[o.x][o.y].isFill = false;
    }
    o.x += deltax;
    o.y += deltay;

}

template <class T, class U>
bool collision(const T& e1, const U& e2){
    return e1.x == e2.x && e1.y == e2.y;
}
