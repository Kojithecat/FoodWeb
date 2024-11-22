#ifndef GAME_HH
#define GAME_HH

#include "raylib.h"
#include <queue>
#include <set>
#include <vector>

#define TILESIZE 64

const float MOVE_INTERVAL = 0.2f;
const float SCORPION_MOVE_INTERVAL = 2*MOVE_INTERVAL;
const float RAT_MOVE_INTERVAL = MOVE_INTERVAL;
const float BAT_MOVE_INTERVAL = 3*MOVE_INTERVAL;
const float SNAKE_MOVE_INTERVAL = 5*MOVE_INTERVAL;
const float ROCK_FALL_INTERVAL = MOVE_INTERVAL;



typedef enum {
    LVL1,
    LVL2,
    //...
    MENUSCREEN,

} SCREEN;

typedef enum {
    SCORPION,
    RAT,
    BAT,
    SNAKE,
} ETYPE;

const std::vector<float> enemyInterval = {SCORPION_MOVE_INTERVAL, RAT_MOVE_INTERVAL, BAT_MOVE_INTERVAL, SNAKE_MOVE_INTERVAL};

struct Player {
    int x;
    int y;
    float moveTime = 0.0f;
    std::queue<char> playerMoves;
    Texture2D texture;
    //std:vector<Item> objects;

    Player(int startX, int startY) : x(startX), y(startY) {}
};

struct Enemy {
    int x;
    int y;
    bool dead = false;
    float moveTime = 0.0f;
    Texture2D texture;
    ETYPE type;
    float intervalMove;

    Enemy(int startX, int startY, ETYPE enemyType) : x(startX), y(startY), type(enemyType), intervalMove(enemyInterval[enemyType]) {}
};

struct Rock{
    int x;
    int y;
    float moveTime = 0.0f;
    Texture2D texture;
    bool falling = false;

    Rock(int startX, int startY) : x(startX), y(startY) {}
};

struct Bomb{
    int x;
    int y;
    float moveTime = 0.0f;
    Texture2D texture;
    bool falling = false;

    Bomb(int startX, int startY) : x(startX), y(startY) {}
};

struct LevelGoal{
    int x;
    int y;
    bool open;

    LevelGoal(int startX, int startY, bool open) : x(startX), y(startY), open(open) {}
};

struct Casilla{
    bool isFill;
    bool isEnemy = false;
    bool isPlayer = false;
    bool isRock = false;
    bool isGoal = false;
    bool isBomb = false;
    bool isBedrock = false;

    //...
};

SCREEN showMenuScreen();

SCREEN runTestLevel();

int fillMap(std::vector<std::vector<Casilla>> &levelMap, Player &p, LevelGoal &w, std::vector<Enemy> &enemyVector, std::vector<Rock> &rockVector, std::vector<Bomb> &bombVector, std::set<std::pair<int,int>> sandlessSet);

Player initPlayer(int x, int y);

Enemy initEnemy(int x, int y, ETYPE type);

Rock initRock(int x, int y);

Bomb initBomb(int x, int y);

LevelGoal initGoal(int x, int y, bool open);

void checkPlayerMovement(Player &p);

void movePlayer(Player &p, std::vector<Rock> &rockVector, std::vector<Bomb> &bombVector, std::vector<std::vector<Casilla>> &map);

void moveEnemies(std::vector<Enemy> &enemyVector, std::vector<std::vector<Casilla>> &map);

void moveEnemy(Enemy &e, std::vector<std::vector<Casilla>> &map);

void fallRock(Rock &r, std::vector<Enemy> &enemyVector, Player &p, std::vector<std::vector<Casilla>> &map);

int fallBomb(Bomb &b, std::vector<Enemy> &enemyVector, Player &p, std::vector<std::vector<Casilla>> &map);

template<class T>
void moveObject(T &o, std::vector<std::vector<Casilla>> &map, int deltax, int deltay);

template <class T, class U>
bool collision(const T& e1, const U& e2);

template <class T>
bool isEntityMapSync(const T& o, const std::vector<std::vector<Casilla>> &map);

#endif
