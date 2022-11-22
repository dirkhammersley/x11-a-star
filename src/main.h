#include "x_window.h"

void getEvents(window::XWindow window);

class GridSquare{
  public:
    GridSquare(int x_loc, int y_loc, int width, int height) : x_loc_{x_loc},
                                                              y_loc_{y_loc},
                                                              width_{width},
                                                              height_{height} {}
    std::pair<int, int> getXExtents(){
      return std::make_pair(x_loc_ - width_ / 2, x_loc_ + width_ / 2);
    }

    std::pair<int, int> getYExtents(){
      return std::make_pair(y_loc_ - height_ / 2, y_loc_ + height_ / 2);
    }

    std::pair<int, int> getCenter(){
      return std::make_pair(x_loc_, y_loc_);
    }

    void setColor(ulong new_color){
      color_ = new_color;
    }

    void draw(window::XWindow window, bool full){
      if (full == true){
        XSetForeground(window.getDisplay(), window.getGc(), window.colors.cyber_red);
        XFillRectangle(window.getDisplay(), window.getWindow(), window.getGc(), x_loc_, y_loc_, width_, height_);
      }else{
        XSetForeground(window.getDisplay(), window.getGc(), window.colors.cyber_blue);
        XFillRectangle(window.getDisplay(), window.getWindow(), window.getGc(), x_loc_, y_loc_, width_, height_);
      }
    }

  private:
    ulong color_;
    int x_loc_;
    int y_loc_;
    int width_;
    int height_;
    bool visited_;
    uint16_t id_;
};

class StaticGrid{
  public:
    StaticGrid(window::XWindow window);

    void drawGrid(int w_cells, int h_cells);
    
    void drawSquare(GridSquare square, bool full);

    // Install a square in the grid
    void addSquareToGrid(GridSquare square);

    // Return a pointer to the square containing the given X / Y coordinates
    GridSquare* getSquareAt(int x, int y);

    size_t getNumSquares(){
      return squares_.size();
    }

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