#include "game.hh"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

void loadMap(std::vector<std::string> &level, std::vector<std::vector<Casilla>> &levelMap){
    int width = level[0].size()+2;
    int height = level.size()+2; //Reading error has the two last lines repeated
    levelMap.resize(height, std::vector<Casilla>(width));
    std::cout << "Loading" << std::endl;
    for(int i = 0; i < level.size()-1; i++){
        for(int j = 0; j < level[i].size(); j++){
            //std::cout << level[i][j] << " ";
            //if(i > 0 and j > 0 and i < level.size()+1 and j < level.size()){
            std::cout << "i: " << i << " j: " << j << std::endl;
            if(level[i][j] == 'P') levelMap[i+1][j+1].isPlayer = true;
            else if(level[i][j] == 'W') levelMap[i+1][j+1].isGoal = true;
            else if(level[i][j] == 'N') levelMap[i+1][j+1].isBomb = true;
            else if(level[i][j] == '-') levelMap[i+1][j+1].isFill = false;
            else if(level[i][j] == 'V') levelMap[i+1][j+1].isPoison = true;
            else if(level[i][j] == 'M') levelMap[i+1][j+1].isMagnet = true;
            else if(level[i][j] == 'O') levelMap[i+1][j+1].isRock = true;
            else if(level[i][j] == 'B' or level[i][j] == 'S' or level[i][j] == 'M' or level[i][j] == 'E' or level[i][j] == 'R')
                levelMap[i+1][j+1].isEnemy = true;
            }
           // if(i == 0 or i == level.size() or j == 0 or j == level[i].size()+1) levelMap[i][j].isBedrock = true;
        }
    for(int i = 0; i < height; i++) {
        levelMap[i][0].isBedrock = true;
        levelMap[i][width-1].isBedrock = true;
    }

    for(int j = 0; j < width ; j++) {
            levelMap[0][j].isBedrock = true;
            levelMap[height-1][j].isBedrock = true;
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

void loadEntities(std::vector<std::string> &level, Player &p, LevelGoal &w, std::vector<Enemy> &enemyVector, std::vector<Rock> &rockVector, std::vector<Bomb> &bombVector, std::vector<Magnet> &magnetVector, std::vector<Poison> &poisonVector){




}
