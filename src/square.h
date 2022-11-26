#pragma once
#include "x_window.h"

class GridSquare{
  public:
    GridSquare(int x_loc, int y_loc, int width, int height, bool is_obstacle) : x_loc_{x_loc},
                                                              y_loc_{y_loc},
                                                              width_{width},
                                                              height_{height},
                                                              is_obstacle_{is_obstacle} {}
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

  friend bool operator== (GridSquare &lhs, GridSquare &rhs){
    bool is_equal = lhs.getCenter().first == rhs.getCenter().first && lhs.getCenter().second == rhs.getCenter().second;
    return is_equal;
  }

  private:
    ulong color_;
    int x_loc_;
    int y_loc_;
    int width_;
    int height_;
    bool visited_;

    //Whether or not the square can be traversed. If not, it's an obstacle.
    bool is_obstacle_ = false;
    uint16_t id_;
};
