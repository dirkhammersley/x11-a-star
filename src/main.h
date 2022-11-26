#pragma once
#include "grid.h"
#include <map>

// Right now, most of the "game logic" goes here.
void handleEvents(window::XWindow window, GridSquare* grid);

// Check equality accounting for floating point error
template <class T>
bool isClose(T a, T b, double tol);

// ...
double distanceBetweenSquares(GridSquare* square_one, GridSquare* square_two);

//
GridSquare* getLowestScore(std::map<GridSquare*, double> &scores, std::vector<GridSquare*> &squares);

//A* algorithm
void runAStar(StaticGrid* grid, window::XWindow window);

uint16_t window_height_ = 1200;
uint16_t window_width_ = 800;