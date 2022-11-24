#pragma once
#include "grid.h"

// Right now, most of the "game logic" goes here.
void handleEvents(window::XWindow window, GridSquare* grid);

void runAStar(StaticGrid* grid, window::XWindow window);

// Check equality accounting for floating point error
template <class T>
bool isClose(T a, T b, double tol);

// ...
double distanceBetweenSquares(GridSquare* square_one, GridSquare* square_two);

uint16_t window_height_ = 900;
uint16_t window_width_ = 600;