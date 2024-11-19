#include "raylib.h"
#include <queue>
#include <set>
#include <vector>

#define TILESIZE 50

const float MOVE_INTERVAL = 0.2f;
const float BAT_MOVE_INTERVAL = 3*MOVE_INTERVAL;
const float SCORPION_MOVE_INTERVAL = 2*MOVE_INTERVAL;
const float RAT_MOVE_INTERVAL = MOVE_INTERVAL;
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
    bool dead;
    float moveTime;
    Texture2D texture;
    ETYPE type;
    float intervalMove;
};

struct Rock{
    int x;
    int y;
    float moveTime;
    Texture2D texture;
    bool falling;
};

struct Bomb{
    int x;
    int y;
    float moveTime;
    Texture2D texture;
    bool falling;

};

struct LevelGoal{
    int x;
    int y;
    bool open;
};

struct Casilla{
    bool isFill;
    bool isEnemy;
    bool isPlayer;
    bool isRock;
    bool isGoal;
    bool isBomb;

    //...
};

SCREEN showMenuScreen();

SCREEN runTestLevel();

int fillMap(std::vector<std::vector<Casilla>> &levelMap, Player &p, LevelGoal &w, std::vector<Enemy> &enemyVector, std::vector<Rock> &rockVector, std::set<std::pair<int,int>> sandlessSet);


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
