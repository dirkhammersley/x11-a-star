#include "x_window.h"

void getEvents(window::XWindow window);

// Check equality accounting for floating point error
template <class T>
bool isClose(T a, T b, double tol);

// ...
double distanceBetweenPoints();

uint16_t window_height_ = 900;
uint16_t window_width_ = 600;