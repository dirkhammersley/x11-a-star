#pragma once
#include "x_window.h"
#include "square.h"


class StaticGrid{
  public:
    StaticGrid(window::XWindow window, int cells_high, int cells_wide, int size);

    void drawGrid();
    
    void drawSquare(GridSquare square, bool full);

    GridSquare* getStartSquare(){
      return start_square_;
    }

    void setStartSquare(GridSquare* start){
      start_square_ = start;
    }

    GridSquare* getTargetSquare(){
      return target_square_;
    }

    void setTargetSquare(GridSquare* target){
      target_square_ = target;
    }

    GridSquare* getCurrentSquare(){
      return curr_square_;
    }

    void setCurrentSquare(GridSquare* curr){
      curr_square_ = curr;
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