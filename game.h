#ifndef GAME_H
#define GAME_H

#include <string>

std::string getRandomImagePath();
bool isSolved();
int getClickedTile(int, int);
void drawPuzzle();
void game();

#endif