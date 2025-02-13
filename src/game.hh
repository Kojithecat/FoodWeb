#ifndef GAME_HH
#define GAME_HH

#include "raylib.h"
#include <queue>
#include <set>
#include <vector>

#define TILESIZE 64

const float MOVE_INTERVAL = 0.2f;
const float SCORPION_MOVE_INTERVAL = 2*MOVE_INTERVAL + GetFrameTime();
const float RAT_MOVE_INTERVAL = MOVE_INTERVAL + GetFrameTime();
const float BAT_MOVE_INTERVAL = 3*MOVE_INTERVAL + GetFrameTime();
const float SNAKE_MOVE_INTERVAL = 5*MOVE_INTERVAL + GetFrameTime();
const float MANTIS_MOVE_INTERVAL = 3*MOVE_INTERVAL + GetFrameTime();
const float ROCK_FALL_INTERVAL = MOVE_INTERVAL;
const float POISON_EXPAND_INTERVAL = 6*MOVE_INTERVAL + GetFrameTime();



typedef enum {
    LVL1,
    LVL2,
    LVL3,
    LVL4,
    LVL5,
    //...
    MENUSCREEN,

} SCREEN;

typedef enum {
    SCORPION,
    RAT,
    BAT,
    SNAKE,
    MANTIS
} ETYPE;

const std::vector<float> enemyInterval = {SCORPION_MOVE_INTERVAL, RAT_MOVE_INTERVAL, BAT_MOVE_INTERVAL, SNAKE_MOVE_INTERVAL, MANTIS_MOVE_INTERVAL};

struct Player {
    int x;
    int y;
    float moveTime = 0.0f;
    std::queue<char> playerMoves;
    bool immortal = false;
    Texture2D texture = LoadTexture("../assets/character_cutre.png");

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
    Texture2D texture = LoadTexture("../assets/metal_ball.png");
    bool falling = false;

    Rock(int startX, int startY) : x(startX), y(startY) {}
};

struct Bomb{
    int x;
    int y;
    float moveTime = 0.0f;
    Texture2D texture = LoadTexture("../assets/bomb.png");
    bool falling = false;

    Bomb(int startX, int startY) : x(startX), y(startY) {}
};

struct Poison{
    int x;
    int y;
    float moveTime = 0.0f;
    Texture2D texture = LoadTexture("../assets/veneno_cutre.png");
    bool isContained = true;

    Poison(int startX, int startY) : x(startX), y(startY) {}

};

struct Magnet{
    int x;
    int y;
    Texture2D texture;

    Magnet(int startX, int startY) : x(startX), y(startY) {}

};

struct LevelGoal{
    int x;
    int y;
    bool open = false;

    LevelGoal(int startX, int startY, bool open) : x(startX), y(startY), open(open) {}
};

struct Casilla{
    bool isFill = true;
    bool isEnemy = false;
    bool isPlayer = false;
    bool isRock = false;
    bool isGoal = false;
    bool isBomb = false;
    bool isMagnet = false;
    bool isBedrock = false;

    bool isPoison = false;
    //...
};

SCREEN showMenuScreen();

SCREEN runLevel(int lvl);

void initCamera(Camera2D &camera, Player &p);

int fillMap(std::vector<std::vector<Casilla>> &levelMap, Player &p, LevelGoal &w, std::vector<Enemy> &enemyVector, std::vector<Rock> &rockVector, std::vector<Bomb> &bombVector, std::vector<Magnet> &magnetVector, std::vector<Poison> &poisonVector, std::set<std::pair<int,int>> sandlessSet);

Player initPlayer(int x, int y);

Enemy initEnemy(int x, int y, ETYPE type);

Rock initRock(int x, int y);

Bomb initBomb(int x, int y);

LevelGoal initGoal(int x, int y, bool open);

void checkPlayerMovement(Player &p);

void movePlayer(Player &p, std::vector<Rock> &rockVector, std::vector<Bomb> &bombVector, std::vector<Magnet> &magnetVector, std::vector<std::vector<Casilla>> &map);

void moveEnemies(std::vector<Enemy> &enemyVector, Player &p, std::vector<std::vector<Casilla>> &map);

void moveEnemyRandom(Enemy &e, std::vector<std::vector<Casilla>> &map);

void moveEnemyTowardsPlayer(Enemy &e, Player &p, std::vector<std::vector<Casilla>> &map);

void fallRock(Rock &r, std::vector<Enemy> &enemyVector, Player &p, std::vector<std::vector<Casilla>> &map);

int fallBomb(Bomb &b, std::vector<Enemy> &enemyVector, Player &p, std::vector<std::vector<Casilla>> &map);

void expandPoison(std::vector<Poison> &poisonVector, std::vector<std::vector<Casilla>> &map);

void moveTowardsMagnet(std::vector<Rock> &rockVector, std::vector<Bomb> &bombVector, std::vector<Magnet> &magnetVector, std::vector<std::vector<Casilla>> &map);

template<class T>
int moveObject(T &o, std::vector<std::vector<Casilla>> &map, int deltax, int deltay);

void checkCollisions(Player &p, LevelGoal &w, std::vector<Enemy> &enemyVector, std::vector<Rock> &rockVector, std::vector<Bomb> &bombVector, std::vector<Poison> &poisonVector);

template <class T, class U>
bool collision(const T& e1, const U& e2);

template <class T>
bool isEntityMapSync(const T& o, const std::vector<std::vector<Casilla>> &map);

void updateCamera(Camera2D &camera, Player &p);

SCREEN nextLevel(int lvl);

#endif
