#include "raylib.h"
#include <queue>

const float MOVE_INTERVAL = 0.2f;
const float BAT_MOVE_INTERVAL = 3*MOVE_INTERVAL;
const float SCORPION_MOVE_INTERVAL = 2*MOVE_INTERVAL;
const float RAT_MOVE_INTERVAL = MOVE_INTERVAL;
const float SNAKE_MOVE_INTERVAL = 5*MOVE_INTERVAL;


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

struct Player {
    int x;
    int y;
    float moveTime;
    std::queue<char> playerMoves;
    Texture2D texture;
    //std:vector<Item> objects;
};

struct Enemy {
    int x;
    int y;
    float moveTime;
    Texture2D texture;
    ETYPE type;
};

struct Casilla{
    bool filled;
    //...
};

struct Coord{
    int x;
    int y;
};

SCREEN showMenuScreen();

SCREEN runTestLevel();

Player initPlayer(int x, int y);

Enemy initEnemy(int x, int y, ETYPE type);


void checkPlayerMovement(Player &p);

void movePlayer(Player &p, std::vector<std::vector<bool>> &map);

void moveEnemy(Enemy &e, std::vector<std::vector<bool>> &map);
