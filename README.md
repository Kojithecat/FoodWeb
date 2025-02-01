# FoodWeb
First attemp to develop a small puzzle game. The core mechanic is that enemies will have a preference of eating each other before coming for you.

Install raylib (on ubuntu):

> sudo apt install build-essential git
> sudo apt install libasound2-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev libwayland-dev libxkbcommon-dev


Commands to compile and execute the game (Linux):
> cd src/
> g++ main.cc game.cc levelLoader.cc -o game -lraylib -ldl -lm -lpthread
> ./game
