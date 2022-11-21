#include "x_window.h"

void getEvents(window::XWindow window);

class GridSquare{
  public:
    GridSquare(int x_loc, int y_loc);

  private:
    int x_loc;
    int y_loc;
    bool visited_;
};

class StaticGrid{
  public:
    StaticGrid(window::XWindow window);

    void drawGrid(int w_cells, int h_cells);

    void drawSquare(int x, int y, int w, int h, bool full);

  private:
    window::XWindow window_;
    std::vector<GridSquare> squares_;
};

// Check equality accounting for floating point error
template <class T>
bool isClose(T a, T b, double tol);

// ...
double distanceBetweenPoints();

uint16_t window_height_ = 900;
uint16_t window_width_ = 600;