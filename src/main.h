#include "x_window.h"

void getEvents(window::XWindow window);

class StaticGrid{
  public:
    StaticGrid(window::XWindow window);

    void drawGrid(int w_cells, int h_cells);

    void drawSquare(int x, int y, int w, int h, bool full);

  private:
    window::XWindow window_;
};

// Check equality accounting for floating point error
template <class T>
bool isClose(T a, T b, double tol);

// ...
double distanceBetweenPoints();

uint16_t window_height_ = 900;
uint16_t window_width_ = 600;