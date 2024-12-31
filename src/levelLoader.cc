#include <vector>
#include <string>
#include <fstream>
#include <iostream>


struct Casilla{

    bool isFill;
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


void loadMap(std::vector<std::string> &level, std::vector<std::vector<Casilla>> &map){
    int width = level[0].size()+2;
    int height = level.size()+1; //Reading error has the two last lines repeated

    std::vector<std::vector<Casilla>> levelMap(width, std::vector<Casilla>(height));

    for(int i = 0; i < level.size() +1; i++){
        for(int j = 0; j < level[i].size()+2; j++){
            //std::cout << level[i][j] << " ";
            if(i < level.size()-1 and j < level.size()){
                if(level[i][j] == 'P') levelMap[i][j].isPlayer = true;
                else if(level[i][j] == 'W') levelMap[i][j].isGoal = true;
                else if(level[i][j] == 'N') levelMap[i][j].isBomb = true;
                else if(level[i][j] == '-') levelMap[i][j].isFill = false;
                else if(level[i][j] == 'V') levelMap[i][j].isPoison = true;
                else if(level[i][j] == 'M') levelMap[i][j].isMagnet = true;
                else if(level[i][j] == 'O') levelMap[i][j].isRock = true;
                else if(level[i][j] == 'B' or level[i][j] == 'S' or level[i][j] == 'M' or level[i][j] == 'E' or level[i][j] == 'R')
                    levelMap[i][j].isEnemy = true;
            }
            if(i == 0 or i == level.size() or j == 0 or j == level[i].size()+1) levelMap[i][j].isBedrock = true;

        }
    }


}


int main(){

    std::vector<std::vector<Casilla>> levelMap;

    std::string line;
    std::vector<std::string> level;

    std::ifstream levelFile;
    levelFile.open("../levels/lvl1.txt");

    if(levelFile.is_open()){
        while(levelFile){
            levelFile >> line;
            std::cout << line << std::endl;
            level.push_back(line);
        }
    }
    int i = 0;
    for(auto l : level){
        std::cout << l << i << std::endl;
        i++;
    }
    std::cout << level.size() << " " << level[0].size() << std::endl;

    loadMap(level, levelMap);

}
