#include "grid.h"

StaticGrid::StaticGrid(window::XWindow window, int cells_high, int cells_wide, int size) : window_{window},
                                                                                 cells_high_{cells_high},
                                                                                 cells_wide_{cells_wide},
                                                                                 square_size_{size}{
  int x_start = 50;
  int y_start = 50;
  int cell_height = square_size_;
  int cell_width = square_size_;
  
  for (int i = 0; i < cells_high_; i++){
    for (int j = 0; j < cells_wide_; j++){
      GridSquare square(x_start + j * cell_height, y_start + i * cell_width, cell_height, cell_width);
      addSquareToGrid(square);
    }
  }
  start_square_ = &squares_[0];
  curr_square_ = start_square_;
  target_square_ = &squares_[150];
}

void StaticGrid::drawGrid(){

  window_.redraw();
  for (auto square : squares_) {
    square.draw(window_, false);
  }
  XFlush(window_.getDisplay());
}

void StaticGrid::drawSquare(GridSquare square, bool full){
  square.draw(window_, full);
}

//TEST_ME
void StaticGrid::getNeighboringSquares(GridSquare* square, std::vector<GridSquare *> &output){
  for (auto s : squares_){
    //Add a feature for "isObstacle()" or similar to note if square should not be considered
    int test_x = s.getCenter().first;
    int test_y = s.getCenter().second;
    int x = square->getCenter().first;
    int y = square->getCenter().first;

    //Consider neighboring squares to be squares sharing an edge.
    if(((test_x == x + square_size_ || test_x == x - square_size_) && test_y == y) ||
        ((test_y == y + square_size_ || test_y == y - square_size_) && test_x == x)){
     std::cout << "Is nieghbor!" << std::endl;
     output.push_back(&s);
     std::cout << "Found " << output.size() << " neighbors!" << std::endl;
    }
  }
}

void StaticGrid::addSquareToGrid(GridSquare square){
  squares_.push_back(square);
}

// TODO: This whole function is bad
GridSquare* StaticGrid::getSquareAt(int x, int y){
  for (auto i = squares_.begin(); i < squares_.end(); i++) {
    if (x <= i->getXExtents().second &&
        x > i->getXExtents().first &&
        y <= i->getYExtents().second &&
        y > i->getYExtents().first){
          return &(*i);
    }
  }
  std::cout << "Unable to find square with these coordinates, returning start square: " << x << ", " << y << std::endl;
  return &squares_[0];
}