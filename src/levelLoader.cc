#include "game.hh"
#include <raylib.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

void loadMap(std::vector<std::string> &level, std::vector<std::vector<Casilla>> &levelMap){
    int width = level[0].size()+2;
    int height = level.size()+2; //Reading error has the two last lines repeated
    levelMap.resize(width, std::vector<Casilla>(height));
    std::cout << "Loading" << std::endl;
    for(int i = 0; i < level.size(); i++){
        for(int j = 0; j < level[i].size(); j++){
            //std::cout << level[i][j] << " ";
            //if(i > 0 and j > 0 and i < level.size()+1 and j < level.size()){
            std::cout << "i: " << i << " j: " << j << std::endl;
            if(level[i][j] == 'P') {
                levelMap[j+1][i+1].isPlayer = true;
                levelMap[j+1][i+1].isFill = false;
            }
            else if(level[i][j] == 'W'){
                levelMap[j+1][i+1].isGoal = true;
                levelMap[j+1][i+1].isFill = false;
            }
            else if(level[i][j] == 'N'){
                levelMap[j+1][i+1].isBomb = true;
                levelMap[j+1][i+1].isFill = false;
            }
            else if(level[i][j] == '-') levelMap[j+1][i+1].isFill = false;
            else if(level[i][j] == 'V'){
                levelMap[j+1][i+1].isPoison = true;
                levelMap[j+1][i+1].isFill = false;
                std::cout << "i: " << i << " j: " << j << "POSION" << std::endl;
            }
            else if(level[i][j] == 'I'){
                levelMap[j+1][i+1].isMagnet = true;
                levelMap[j+1][i+1].isFill = false;
            }
            else if(level[i][j] == 'O'){
                levelMap[j+1][i+1].isRock = true;
                levelMap[j+1][i+1].isFill = false;
            }
            else if(level[i][j] == 'B' or level[i][j] == 'S' or level[i][j] == 'M' or level[i][j] == 'E' or level[i][j] == 'R'){
                levelMap[j+1][i+1].isEnemy = true;
                levelMap[j+1][i+1].isFill = false;
            }
        }
           // if(i == 0 or i == level.size() or j == 0 or j == level[i].size()+1) levelMap[i][j].isBedrock = true;
    }
    for(int i = 0; i < width; i++) {
        levelMap[i][0].isBedrock = true;
        levelMap[i][height-1].isBedrock = true;
    }

    for(int j = 0; j < height ; j++) {
            levelMap[0][j].isBedrock = true;
            levelMap[width-1][j].isBedrock = true;
    }


    std::cout << "Finished loading" << std::endl;

}


std::vector<std::string> readLevel(int levelNum){
    //std::vector<std::vector<Casilla>> levelMap;

    std::string line;
    std::vector<std::string> level;

    std::ifstream levelFile;
    levelFile.open("../levels/lvl"+ std::to_string(levelNum) +".txt");

    if(levelFile.is_open()){
        while(std::getline(levelFile, line)){
            std::cout << line << std::endl;
            level.push_back(line);
        }
    }
    return level;
}

void loadEntities(std::vector<std::string> &level, std::vector<Enemy> &enemyVector, std::vector<Rock> &rockVector, std::vector<Bomb> &bombVector, std::vector<Magnet> &magnetVector, std::vector<Poison> &poisonVector){

    for(int i = 0; i < level.size(); i++){
        for(int j = 0; j < level[i].size(); j++){
            if(level[i][j] == 'N'){
                Bomb n = Bomb(j+1,i+1);
                bombVector.push_back(n);
            }
            else if(level[i][j] == 'V')
            {
                Poison v = Poison(j+1,i+1);
                poisonVector.push_back(v);
            }
            else if(level[i][j] == 'I'){
                Magnet m = Magnet(j+1,i+1);
                magnetVector.push_back(m);
            }
            else if(level[i][j] == 'O'){
                Rock o = Rock(j+1,i+1);
                rockVector.push_back(o);
            }
            else if(level[i][j] == 'B'){
                Enemy b = Enemy(j+1,i+1, BAT);
                //b.texture = LoadTexture("../assets/bat.png");
                enemyVector.push_back(b);
            }
            else if(level[i][j] == 'S')
            {
                Enemy s = Enemy(j+1,i+1, SNAKE);
                s.texture = LoadTexture("../assets/snake.png");
                enemyVector.push_back(s);
            }
            else if(level[i][j] == 'M')
            {
                Enemy m = Enemy(j+1,i+1, MANTIS);
                m.texture = LoadTexture("../assets/mantis.png");
                enemyVector.push_back(m);
            }
            else if(level[i][j] == 'E')
            {
                Enemy e = Enemy(j+1,i+1, SCORPION);
                e.texture = LoadTexture("../assets/scorpion.png");
                enemyVector.push_back(e);
            }
            else if(level[i][j] == 'R')
            {
                Enemy r = Enemy(j+1,i+1, RAT);
                r.texture = LoadTexture("../assets/rat.png");
                enemyVector.push_back(r);
            }
        }
    }
}
