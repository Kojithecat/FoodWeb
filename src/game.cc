#include "game.hh"
#include "raylib.h"
#include <type_traits>
#include <iostream>
#include <cstdint>
#include <utility>
#include <random>
#include <set>
#include <queue>
#include <vector>

using u32 = uint_least32_t;
using engine = std::mt19937;

std::random_device os_seed;
const u32 seed = os_seed();
engine generator(seed);
std::uniform_int_distribution<u32> distribute(1,4);

int width = 30;
int height = 20;

const int SCREEN_TILES_X = 16;
const int SCREEN_TILES_Y = 12;


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
    //Set textures
    Texture2D sandTexture = LoadTexture("../assets/arena.png");
    Texture2D bedrockTexture = LoadTexture("../assets/bedrock_cutre.png");
       //RenderTexture2D background = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
    SetTextureFilter(bedrockTexture, TEXTURE_FILTER_BILINEAR);
    //Define entities
    Player p = Player(1, 1);

    Enemy e1 = Enemy(13,6,RAT);
    Enemy e2 = Enemy(6, 6, BAT);
    Enemy e3 = Enemy(12,12, SNAKE);

    //Rock r1 = initRock(6, 3);
    Rock r1 = Rock(13, 4);

    Rock r2 = Rock(5, 9);
    Rock r3 = Rock(6, 8);
    Rock r4 = Rock(7, 8);
    Rock r5 = Rock(8, 8);

    Bomb b = Bomb(6,3);

    Magnet m = Magnet(20,5);

    Poison v = Poison(3,10);

    LevelGoal w = LevelGoal(width - 2, height - 2, true);

    std::vector<Enemy> enemyVector;
    std::vector<Rock> rockVector;
    std::vector<Bomb> bombVector;
    std::vector<Magnet> magnetVector;
    std::vector<Poison> poisonVector;

    enemyVector.push_back(e1);
    enemyVector.push_back(e2);
    enemyVector.push_back(e3);
    //rockVector.push_back(r1);
    rockVector.push_back(r1);
    rockVector.push_back(r2);
    rockVector.push_back(r3);
    rockVector.push_back(r4);
    rockVector.push_back(r5);

    bombVector.push_back(b);

    magnetVector.push_back(m);

    poisonVector.push_back(v);

    std::set<std::pair<int,int>> sandlessCasillas;

    sandlessCasillas.insert(std::make_pair(6, 3));
    sandlessCasillas.insert(std::make_pair(6, 5));
    sandlessCasillas.insert(std::make_pair(6, 6));
    sandlessCasillas.insert(std::make_pair(6, 7));

    sandlessCasillas.insert(std::make_pair(13, 6));
    sandlessCasillas.insert(std::make_pair(13, 7));
    sandlessCasillas.insert(std::make_pair(13, 8));
    sandlessCasillas.insert(std::make_pair(1, 1));
    sandlessCasillas.insert(std::make_pair(13, 1));

    for(int y = 10; y < height - 1; y++){
        for(int x = 1; x < width - 1; x++){
            sandlessCasillas.insert(std::make_pair(x,y));
        }
    }

    fillMap(levelMap, p, w, enemyVector, rockVector, bombVector, magnetVector, poisonVector, sandlessCasillas);

    Camera2D camera;
    initCamera(camera, p);

    while(!IsKeyPressed(KEY_ESCAPE)){
        //Check entity collisions
        int updateScreen = checkCollisions(p, w, enemyVector, rockVector, bombVector, poisonVector);
        if(updateScreen == 0){
            //Player death
            return MENUSCREEN;
        }
        else if(updateScreen == 1){
            //Player beats level
            return LVL1;

        }

        //Update entities moveTimes

        p.moveTime += GetFrameTime();
        for(Enemy& e : enemyVector){
            e.moveTime += GetFrameTime();
            //std::cout << e.moveTime << std::endl;
        }
        for(Rock& r : rockVector)
            r.moveTime += GetFrameTime();
        for(Bomb& b : bombVector)
            b.moveTime += GetFrameTime();
        for(Poison& v : poisonVector)
            v.moveTime += GetFrameTime();

        //Check movement and falling entities
        checkPlayerMovement(p);
        for(Rock& r : rockVector)
            fallRock(r, enemyVector, p, levelMap);
        for (auto it = bombVector.begin(); it != bombVector.end(); ) {
            if (fallBomb(*it, enemyVector, p, levelMap)) {
                it = bombVector.erase(it);
            } else {
                ++it;
            }
        }

        moveTowardsMagnet(rockVector, bombVector, magnetVector, levelMap);

        expandPoison(poisonVector, levelMap);
        //move Entities
        movePlayer(p, rockVector, bombVector, magnetVector, levelMap);
        moveEnemies(enemyVector, p, levelMap);

        //Chack map-entities sync (temporal only)
        if(isEntityMapSync(p,  levelMap))
            std::cout << "Player Sync" << std::endl;
        else std::cout << "Player not Sync" << std::endl;
        for(Rock &r : rockVector)
            if(isEntityMapSync(r, levelMap))
                std::cout << "Rock Sync" << std::endl;
            else std::cout << "Rock not Sync" << std::endl;
        for(Bomb &b : bombVector)
            if(isEntityMapSync(b, levelMap))
                std::cout << "Bomb Sync" << std::endl;
            else std::cout << "Bomb not Sync" << std::endl;
        for(Enemy &e : enemyVector)
            if(isEntityMapSync(e, levelMap))
                std::cout << "Enemy Sync" << std::endl;
            else std::cout << "Enemy not Sync" << std::endl;
        for(Poison& v : poisonVector)
            if(isEntityMapSync(v, levelMap))
                std::cout << "Poison sync" << std::endl;
            else std::cout << "Poison not sync" << std::endl;

        //Camera Target
        updateCamera(camera, p);

        //Draw
        //drawLevel(camera, levelMap, )
        BeginDrawing();
        BeginMode2D(camera);
            ClearBackground(RAYWHITE);
            for(int i = 0; i < levelMap.size(); i++){
                for(int j = 0; j < levelMap[i].size(); j++){
                    if(levelMap[i][j].isBedrock) DrawTextureEx(bedrockTexture, (Vector2){(float) i*TILESIZE, (float) j*TILESIZE}, 0.0f, 4.0f, WHITE);//DrawRectangle(i*TILESIZE, j*TILESIZE, TILESIZE, TILESIZE, BLACK);
                    else if(levelMap[i][j].isFill) DrawTextureEx(sandTexture, (Vector2){(float) i*TILESIZE, (float) j*TILESIZE}, 0.0f, 2.0f, WHITE);//DrawRectangle(i*TILESIZE, j*TILESIZE, TILESIZE, TILESIZE, ORANGE);

                    }
                }
            DrawRectangle(w.x*TILESIZE, w.y*TILESIZE, TILESIZE, TILESIZE, GREEN);
            DrawTextureEx(p.texture, (Vector2){(float) p.x*TILESIZE, (float) p.y*TILESIZE}, 0.0f, 2.0f, WHITE);//DrawRectangle(p.x*TILESIZE, p.y*TILESIZE, TILESIZE, TILESIZE, YELLOW);
            for(Enemy& e : enemyVector)
                if(!e.dead)
                    DrawTextureEx(e.texture, (Vector2){(float) e.x*TILESIZE, (float) e.y*TILESIZE}, 0.0f, 2.0f, WHITE);//DrawRectangle(e.x*TILESIZE, e.y*TILESIZE, TILESIZE, TILESIZE, RED);
            for(Rock& r : rockVector)
                DrawRectangle(r.x*TILESIZE, r.y*TILESIZE, TILESIZE, TILESIZE, BROWN);
            for(Bomb& b : bombVector)
                DrawTextureEx(b.texture, (Vector2){(float) b.x*TILESIZE, (float) b.y*TILESIZE}, 0.0f, 2.0f, WHITE);//DrawRectangle(b.x*TILESIZE, b.y*TILESIZE, TILESIZE, TILESIZE, BLUE);
            for(Magnet& m : magnetVector){
                DrawRectangle(m.x*TILESIZE, m.y*TILESIZE, TILESIZE, TILESIZE, GRAY);
            }

            for(Poison& v : poisonVector)
                DrawTextureEx(v.texture, (Vector2){(float) v.x*TILESIZE, (float) v.y*TILESIZE}, 0.0f, 2.0f, WHITE);//DrawRectangle(v.x*TILESIZE, v.y*TILESIZE, TILESIZE, TILESIZE, PURPLE);
        EndMode2D();
        EndDrawing();

        //std::cout << e1.x << " " << e1.y << std::endl;
    }
    UnloadTexture(sandTexture);
    UnloadTexture(bedrockTexture);

    return MENUSCREEN;
}

void initCamera(Camera2D &camera, Player &p){

    camera.offset =  {(float) p.x,(float) p.y};
    camera.target = {(float) p.x,(float) p.y};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

}

int fillMap(std::vector<std::vector<Casilla>> &levelMap,Player &p, LevelGoal &w, std::vector<Enemy> &enemyVector, std::vector<Rock> &rockVector, std::vector<Bomb> &bombVector, std::vector<Magnet> &magnetVector, std::vector<Poison> &poisonVector, std::set<std::pair<int,int>> sandlessSet){

    for(int i = 0; i < levelMap.size(); i++) {
        for(int j = 0; j <levelMap[i].size(); j++){
            if(i == 0 || i == width - 1 || j == 0 || j == height - 1)
                levelMap[i][j].isBedrock = true;
            else if(sandlessSet.find(std::make_pair(i,j)) != sandlessSet.end()){
                levelMap[i][j].isFill = false;
            }
            else levelMap[i][j].isFill = true;
        }
    }
    levelMap[p.x][p.y].isPlayer = true;
    levelMap[w.x][w.y].isGoal = true;
    for(Enemy& e : enemyVector)
        levelMap[e.x][e.y].isEnemy = true;
    for(Rock& r : rockVector)
        levelMap[r.x][r.y].isRock = true;
    for(Bomb &b : bombVector)
        levelMap[b.x][b.y].isBomb = true;
    for(Magnet& m : magnetVector )
        levelMap[m.x][m.y].isMagnet = true;
    for(Poison& v : poisonVector)
        levelMap[v.x][v.y].isPoison = true;


    return 0;
}

void checkPlayerMovement(Player &p){

    if(p.moveTime > MOVE_INTERVAL/4){
        if(IsKeyDown(KEY_W))// && p.y > 0)
            p.playerMoves.push('w');//p.y -= TILESIZE;
        if(IsKeyDown(KEY_A))// && p.x > 0)
            p.playerMoves.push('a');//p.x -= TILESIZE;
        if(IsKeyDown(KEY_S))// && GetScreenHeight()-TILESIZE > p.y)
            p.playerMoves.push('s');//p.y += TILESIZE;
        if(IsKeyDown(KEY_D))// && GetScreenWidth()-TILESIZE > p.x)
            p.playerMoves.push('d');//p.x +=TILESIZE;
    }
}

void movePlayer(Player &p, std::vector<Rock> &rockVector, std::vector<Bomb> &bombVector, std::vector<Magnet> &magnetVector, std::vector<std::vector<Casilla>> &map){

    if(p.moveTime>=MOVE_INTERVAL){
        //Player movement
        int prex = p.x;
        int prey = p.y;
        char Pmov = ' ';
        if(!p.playerMoves.empty()){
            Pmov = p.playerMoves.back();
            if(Pmov == 'w' && !map[prex][prey-1].isBedrock){
                p.y -= 1;
                for(Rock& r : rockVector){
                    if(collision(r, p)){
                        //std::cout << "hii" << std::endl;
                        if(!moveObject(r, map, 0, -1))
                            p.y +=1;
                    }
                }
                for(Bomb& b : bombVector){
                    if(collision(b, p)){
                        //std::cout << "hii" << std::endl;
                        if(!moveObject(b, map, 0, -1))
                            p.y +=1;
                    }
                }
                for(Magnet& m : magnetVector){
                    if(collision(m, p)){
                        //std::cout << "hii" << std::endl;
                        if(!moveObject(m, map, 0, -1))
                            p.y +=1;
                    }
                }
            }
            if(Pmov == 'a' && !map[prex-1][prey].isBedrock){
                p.x -= 1;
                for(Rock& r : rockVector){
                    if(collision(r, p)){
                        //std::cout << "hii" << std::endl;
                        if(!moveObject(r, map, -1, 0))
                            p.x +=1;
                    }
                }
                for(Bomb& b : bombVector){
                    if(collision(b, p)){
                        //std::cout << "hii" << std::endl;
                        if(!moveObject(b, map, -1, 0))
                            p.x +=1;
                    }
                }
                for(Magnet& m : magnetVector){
                    if(collision(m, p)){
                        //std::cout << "hii" << std::endl;
                        if(!moveObject(m, map, -1, 0))
                            p.x +=1;
                    }
                }
            }

            if(Pmov == 's' && !map[prex][prey+1].isBedrock){
                p.y += 1;
                for(Rock& r : rockVector){
                    if(collision(r, p)){
                        //std::cout << "hii" << std::endl;
                        if(!moveObject(r, map, 0, 1))
                            p.y -=1;
                    }
                }
                for(Bomb& b : bombVector){
                    if(collision(b, p)){
                        //std::cout << "hii" << std::endl;
                        if(!moveObject(b, map, 0, 1))
                            p.y -=1;
                    }
                }
                for(Magnet& m : magnetVector){
                    if(collision(m, p)){
                        //std::cout << "hii" << std::endl;
                        if(!moveObject(m, map, 0, 1))
                            p.y -=1;
                    }
                }
            }

            if(Pmov == 'd' && !map[prex+1][prey].isBedrock){
                p.x +=1;
                for(Rock& r : rockVector){
                    if(collision(r, p)){
                        std::cout << "hii" << std::endl;
                        if(!moveObject(r, map, 1, 0))
                            p.x -=1;
                    }
                }
                for(Bomb& b : bombVector){
                    if(collision(b, p)){
                        //std::cout << "hii" << std::endl;
                        if(!moveObject(b, map, 1, 0))
                            p.x -=1;
                    }
                }
                for(Magnet& m : magnetVector){
                    if(collision(m, p)){
                        //std::cout << "hii" << std::endl;
                        if(!moveObject(m, map, 1, 0))
                            p.x -=1;
                    }
                }
            }
            map[p.x][p.y].isFill = false;
        }
        p.moveTime = 0.0f;
        //Clear movement queue
        p.playerMoves = std::queue<char>();
        //Update map
        map[prex][prey].isPlayer = false;
        map[p.x][p.y].isPlayer = true;
        //Move rocks in front of the player
        /*for(Rock& r : rockVector){
            if(collision(r, p) && Pmov == 'a'){
                //std::cout << "hii" << std::endl;
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
        }*/
        /*for(Bomb& b : bombVector){
            if(collision(b, p) && Pmov == 'a'){
                //std::cout << "hii" << std::endl;
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
        }*/
    }
}

void moveEnemies(std::vector<Enemy> &enemyVector, Player &p, std::vector<std::vector<Casilla>> &map){
    for(Enemy& e : enemyVector){
        int prex = e.x;
        int prey = e.y;
        moveEnemyTowardsPlayer(e, p, map);
        if(prex != e.x || prey != e.y){
            map[prex][prey].isEnemy = false;
            map[e.x][e.y].isEnemy = true;
        }
    }
}

void moveEnemyRandom(Enemy &e, std::vector<std::vector<Casilla>> &map){

    bool leftFilled = e.x > 0 && (map[e.x-1][e.y].isFill || map[e.x-1][e.y].isRock || map[e.x-1][e.y].isBomb || map[e.x-1][e.y].isBedrock);
    bool rightFilled = e.x < width - 1 && (map[e.x+1][e.y].isFill || map[e.x+1][e.y].isRock || map[e.x+1][e.y].isBomb || map[e.x+1][e.y].isBedrock);
    bool topFilled = e.y > 0 && (map[e.x][e.y-1].isFill|| map[e.x][e.y-1].isRock || map[e.x][e.y-1].isBomb || map[e.x][e.y-1].isBedrock);
    bool botFilled = e.y < height - 1 && (map[e.x][e.y+1].isFill || map[e.x][e.y+1].isRock || map[e.x][e.y+1].isBomb || map[e.x][e.y+1].isBedrock);
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

void moveEnemyTowardsPlayer(Enemy &e, Player &p, std::vector<std::vector<Casilla>> &map){
    //Implement A* pr Djikstra
    bool leftFilled = e.x > 0 && map[e.x-1][e.y].isFill;
    bool rightFilled = e.x < width - 1 && map[e.x+1][e.y].isFill;
    bool topFilled = e.y > 0 && map[e.x][e.y-1].isFill;
    bool botFilled = e.y < height - 1 && map[e.x][e.y+1].isFill;
    //std::cout << leftFilled << rightFilled << topFilled << botFilled << std::endl;

    std::vector<std::pair<int,int>> allowedMovements;
    if(e.y > 0 && !topFilled && !map[e.x][e.y-1].isBedrock)
        allowedMovements.push_back(std::make_pair(e.x, e.y - 1));
    if(e.x > 0 && !leftFilled && !map[e.x-1][e.y].isBedrock)
        allowedMovements.push_back(std::make_pair(e.x - 1, e.y));
    if(height - 1 > e.y && !botFilled && !map[e.x][e.y+1].isBedrock)
        allowedMovements.push_back(std::make_pair(e.x, e.y + 1));
    if(width - 1 > e.x && !rightFilled && !map[e.x+1][e.y].isBedrock)
        allowedMovements.push_back(std::make_pair(e.x + 1, e.y));

             //std::cout << "Hii" << std::endl;
    if(e.moveTime >= e.intervalMove){
        //std::cout << "hiii" << std::endl;
        if(allowedMovements.size()){

            std::pair<int, int> bestMove = allowedMovements[0];
            int bestDistance = std::abs(p.x - bestMove.first) + std::abs(p.y - bestMove.second);

            for(const auto &move : allowedMovements) {
                int distance = std::abs(p.x - move.first) + std::abs(p.y - move.second);
                if (distance < bestDistance){
                    bestDistance = distance;
                    bestMove = move;
                }

            }
            //std::uniform_int_distribution<u32> randomMove(0, allowedMovements.size() -1);
            //auto Emov = allowedMovements[randomMove(generator)]; Not random this time
            map[e.x][e.y].isEnemy = false;
            e.x = bestMove.first;
            e.y = bestMove.second;
            map[e.x][e.y].isEnemy = true;
        }

         e.moveTime = 0.0f;
     }

}

void fallRock(Rock &r, std::vector<Enemy> &enemyVector, Player &p, std::vector<std::vector<Casilla>> &map){

    if( !map[r.x][r.y+1].isBedrock //Rock would not fall outbounds
        && !map[r.x][r.y+1].isFill //The bottom tile is empty
        && !(r.x == p.x && r.y + 1 == p.y) //No player is below the rock
        && r.moveTime >= ROCK_FALL_INTERVAL){
        //std::cout << r.y << " " << r.moveTime << std::endl;
        r.moveTime = 0;
        map[r.x][r.y].isRock = false;
        r.y += 1;
        map[r.x][r.y].isRock = true;
        r.falling = true;
    }
    else r.falling = false;
}


int fallBomb(Bomb &b, std::vector<Enemy> &enemyVector, Player &p, std::vector<std::vector<Casilla>> &map){

    if(!map[b.x][b.y+1].isBedrock //bomb would not fall outbounds
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

void expandPoison(std::vector<Poison> &poisonVector, std::vector<std::vector<Casilla>> &map){
    std::set<std::pair<int,int>> newPoisonCoords;
    for(Poison& v : poisonVector){
        if(v.moveTime > POISON_EXPAND_INTERVAL){

            if(!map[v.x+1][v.y].isFill && !map[v.x+1][v.y].isBedrock && !map[v.x+1][v.y].isPoison) {
                map[v.x+1][v.y].isPoison = true;
                newPoisonCoords.insert(std::make_pair(v.x+1,v.y));
            }
            if(!map[v.x-1][v.y].isFill && !map[v.x-1][v.y].isBedrock && !map[v.x-1][v.y].isPoison) {
                map[v.x-1][v.y].isPoison = true;
                newPoisonCoords.insert(std::make_pair(v.x-1,v.y));
            }
            if(!map[v.x][v.y+1].isFill && !map[v.x][v.y+1].isBedrock && !map[v.x][v.y+1].isPoison) {
                map[v.x][v.y+1].isPoison = true;
                newPoisonCoords.insert(std::make_pair(v.x,v.y+1));
            }
            if(!map[v.x][v.y-1].isFill && !map[v.x][v.y-1].isBedrock && !map[v.x][v.y-1].isPoison) {
                map[v.x][v.y-1].isPoison = true;
                newPoisonCoords.insert(std::make_pair(v.x,v.y-1));
            }

            v.moveTime = 0.0f;
        }
    }
    for(std::pair<int,int> p : newPoisonCoords){
        Poison newv = Poison(p.first,p.second);
        poisonVector.push_back(newv);
    }
}

template<class T>
int moveObject(T &o, std::vector<std::vector<Casilla>> &map, int deltax, int deltay){
    if(map[o.x + deltax][o.y + deltay].isBedrock || map[o.x + deltax][o.y + deltay].isRock || map[o.x + deltax][o.y + deltay].isBomb )
        return 0; //Movement not possible
    else if(std::is_same<T, Rock>::value){
        map[o.x][o.y].isRock = false;
        map[o.x + deltax][o.y + deltay].isRock = true;
    }
    else if(std::is_same<T, Bomb>::value){
        map[o.x][o.y].isBomb = false;
        map[o.x + deltax][o.y + deltay].isBomb = true;
    }
    map[o.x][o.y].isFill = false;
    o.x += deltax;
    o.y += deltay;
    map[o.x][o.y].isFill = false;
    return 1; //Movement possible and done
}

int checkCollisions(Player &p, LevelGoal &w, std::vector<Enemy> &enemyVector, std::vector<Rock> &rockVector, std::vector<Bomb> &bombVector, std::vector<Poison> &poisonVector){
    for(Enemy& e : enemyVector){
        //std::cout << e.y << std::endl;
        if(collision(e,p) && !e.dead) return MENUSCREEN;
        for(Rock& r: rockVector)
            if(collision(e,r)) e.dead = true;
        for(Bomb& b: bombVector)
            if(collision(e,b)) e.dead = true;
        for(Poison& v : poisonVector)
            if(collision(e,v)) e.dead = true;
    }
    for(Poison& v : poisonVector)
        if(collision(v,p)) return 0;
    if(collision(w,p)) return 1;

    return -1;
}

void moveTowardsMagnet(std::vector<Rock> &rockVector, std::vector<Bomb> &bombVector, std::vector<Magnet> &magnetVector, std::vector<std::vector<Casilla>> &map){
    for(Magnet& m : magnetVector){
        for(Rock& r : rockVector){
            int diff;
            if(r.y == m.y and r.moveTime > MOVE_INTERVAL){
                diff = r.x-m.x;
                if(diff > 1){
                //Move left
                    moveObject(r, map, -1, 0);
                }
                else if(diff < -1){
                //Move right
                    moveObject(r, map, 1, 0);
                }
                r.moveTime = 0.0f;
            }
        }
        for(Bomb& b : bombVector){
            int diff;
            if(b.y == m.y and b.moveTime > MOVE_INTERVAL){
                diff = b.x-m.x;
                if(diff > 1){
                //Move left
                    moveObject(b, map, -1, 0);
                }
                else if(diff < -1){
                //Move right
                    moveObject(b, map, 1, 0);
                }
                b.moveTime = 0.0f;
            }
        }
    }
}

template <class T, class U>
bool collision(const T& e1, const U& e2){
    return e1.x == e2.x && e1.y == e2.y;
}

template <class T>
bool isEntityMapSync(const T& o, const std::vector<std::vector<Casilla>> &map){
    if constexpr (std::is_same<T, Enemy>::value){
        return map[o.x][o.y].isEnemy;
    }
    else if constexpr (std::is_same<T, Rock>::value){
        return map[o.x][o.y].isRock;
    }
    else if constexpr (std::is_same<T, Bomb>::value){
        return map[o.x][o.y].isBomb;
    }
    if constexpr (std::is_same<T, Player>::value){
        return map[o.x][o.y].isPlayer;
    }
    if constexpr (std::is_same<T, Poison>::value){
        return map[o.x][o.y].isPoison;
    }
}

void updateCamera(Camera2D &camera, Player &p){
    float targetx = p.x*TILESIZE - SCREEN_TILES_X*TILESIZE/2.0;
    float targety = p.y*TILESIZE - SCREEN_TILES_Y*TILESIZE/2.0;
    camera.target = {targetx, targety};
}
