#ifndef LEVELLOADER_HH
#define LEVELLOADER_HH

#include "game.hh"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>


void loadMap(std::vector<std::string> &level, std::vector<std::vector<Casilla>> &map);

std::vector<std::string> readLevel(int levelNum);

void loadEntities(std::vector<std::string> &level, std::vector<Enemy> &enemyVector, std::vector<Rock> &rockVector, std::vector<Bomb> &bombVector, std::vector<Magnet> &magnetVector, std::vector<Poison> &poisonVector);

#endif
