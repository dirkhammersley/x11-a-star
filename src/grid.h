#pragma once
#include "x_window.h"

class GridSquare{
  public:
    GridSquare(int x_loc, int y_loc, int width, int height) : x_loc_{x_loc},
                                                              y_loc_{y_loc},
                                                              width_{width},
                                                              height_{height} {}
    std::pair<int, int> getXExtents(){
      return std::make_pair(x_loc_, x_loc_ + width_);
    }

    std::pair<int, int> getYExtents(){
      return std::make_pair(y_loc_, y_loc_ + height_);
    }

    std::pair<int, int> getCenter(){
      return std::make_pair(x_loc_ + width_ / 2, y_loc_ + height_ / 2);
    }

    void setColor(ulong new_color){
      color_ = new_color;
    }

    bool isObstacle(){
      return is_obstacle_;
    }

    void draw(window::XWindow window, ulong color){
      XSetForeground(window.getDisplay(), window.getGc(), color);
      XFillRectangle(window.getDisplay(), window.getWindow(), window.getGc(), x_loc_, y_loc_, width_, height_);
      XFlush(window.getDisplay());
    }

  private:
    ulong color_;
    int x_loc_;
    int y_loc_;
    int width_;
    int height_;
    bool visited_;

    //Whether or not the square can be traversed. If not, it's an obstacle.
    bool is_obstacle_;
    uint16_t id_;
};

class StaticGrid{
  public:
    StaticGrid(window::XWindow window, int cells_high, int cells_wide, int size);

    void drawGrid();
    
    void drawSquare(GridSquare square, bool full);

    GridSquare* getStartSquare(){
      return start_square_;
    }

    GridSquare* getTargetSquare(){
      return target_square_;
    }

    GridSquare* getCurrentSquare(){
      return curr_square_;
    }

    int getSquareSize(){
        return square_size_;
    }

    void getNeighboringSquares(GridSquare* square, std::vector<GridSquare *> &output);

    // Install a square in the grid
    void addSquareToGrid(GridSquare square);

    // Return a pointer to the square containing the given X / Y coordinates
    GridSquare* getSquareAt(int x, int y);

    size_t getNumSquares(){
      return squares_.size();
    }

  private:
    window::XWindow window_;
    int cells_high_;
    int cells_wide_;
    int square_size_;
    ulong base_color_ = window_.colors.cyber_blue;
    std::vector<GridSquare> squares_;
    GridSquare* curr_square_;
    GridSquare* last_square_;
    GridSquare* start_square_;
    GridSquare* target_square_;
};