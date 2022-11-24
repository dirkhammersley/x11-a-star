#include "main.h"
#include <chrono>
#include <thread>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <map>
#include <cmath>


void handleEvents(window::XWindow window, StaticGrid* grid){
  XEvent event;
	KeySym key;
	char text[255]; // Buffer for key press events
  
  XNextEvent(window.getDisplay(), &event);
	
  /*if (event.type == Expose && event.xexpose.count == 0) {
    window.redraw();
  }*/

  // I guess this is how we get keypresses?
  if (event.type == KeyPress && XLookupString(&event.xkey, text, 255, &key, 0) == 1) {

    if (text[0] == 'q'){
      window.close();
    }

    // Left
    if (text[0] == 'a'){
      window.redraw();
      grid->drawGrid();
      auto curr_square = grid->getCurrentSquare();
      int x = curr_square->getCenter().first;
      int y = curr_square->getCenter().second;
      x -= 45;
      *curr_square = *grid->getSquareAt(x, y);
      curr_square->draw(window, true);
    }

    // Down
    if (text[0] == 's'){
      window.redraw();
      grid->drawGrid();
      auto curr_square = grid->getCurrentSquare();
      int x = curr_square->getCenter().first;
      int y = curr_square->getCenter().second;
      y += 45;
      *curr_square = *grid->getSquareAt(x, y);
      curr_square->draw(window, true);
    }

    // Up
    if (text[0] == 'w'){
      window.redraw();
      grid->drawGrid();
      auto curr_square = grid->getCurrentSquare();
      int x = curr_square->getCenter().first;
      int y = curr_square->getCenter().second;
      y -= 45;
      *curr_square = *grid->getSquareAt(x, y);
      curr_square->draw(window, true);
    }

    // Right
    if (text[0] == 'd'){
      window.redraw();
      grid->drawGrid();
      auto curr_square = grid->getCurrentSquare();
      int x = curr_square->getCenter().first;
      int y = curr_square->getCenter().second;
      x += 45;
      *curr_square = *grid->getSquareAt(x, y);
      curr_square->draw(window, true);
    }

    // Print info
    if (text[0] == 'p'){
      std::cout << "Number of squares in grid: " << grid->getNumSquares() << std::endl;
      auto start_location = grid->getStartSquare()->getCenter();
      std::cout << "Starting square X: " << start_location.first << std::endl;
      std::cout << "Starting square Y: " << start_location.second << std::endl;
      auto curr_location = grid->getCurrentSquare()->getCenter();
      std::cout << "Current square X: " << curr_location.first << std::endl;
      std::cout << "Current square Y: " << curr_location.second << std::endl;
      auto target_location = grid->getTargetSquare()->getCenter();
      std::cout << "Target square X: " << target_location.first << std::endl;
      std::cout << "Target square Y: " << target_location.second << std::endl;
    }
    

    //std::cout << "Keypress: " << text[0] << "\n";
  }

  if (event.type == ButtonPress) {
    window.redraw();
    XDrawLine(window.getDisplay(), window.getWindow(), window.getGc(), 0, 0, event.xbutton.x, event.xbutton.y);
    XFlush(window.getDisplay());
    std::cout << "Click at: " << event.xbutton.x << ", " << event.xbutton.y << "\n";
  }
}

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
  target_square_ = &squares_[32];
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
std::vector<GridSquare *> StaticGrid::getNeighboringSquares(GridSquare* square){
  std::vector<GridSquare *> result;
  for (auto s : squares_){
    //Add a feature for "isObstacle()" or similar to note if square should not be considered
    int test_x = s.getCenter().first;
    int test_y = s.getCenter().second;
    int x = square->getCenter().first;
    int y = square->getCenter().first;

    //Consider neighboring squares to be squares sharing an edge.
    if(((test_x == x + square_size_ || test_x == x - square_size_) && test_y == y) ||
        ((test_y == y + square_size_ || test_y == y - square_size_) && test_x == x)){
      result.push_back(&s);
    }
  }
  return result;
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

//TEST_ME
template <class T>
bool isClose(T a, T b, double tol){
  return abs(a - b) < tol;
}

//TEST_ME
double distanceBetweenSquares(GridSquare* square_one, GridSquare* square_two){
  // Distance formula
  int x1 = square_one->getCenter().first;
  int x2 = square_two->getCenter().first;
  int y1 = square_one->getCenter().second;
  int y2 = square_two->getCenter().second;

  return (double) pow(pow((x2 - x1), 2) + pow((y2 - y1), 2), 0.5);
}


//TODO probably should not use a template here.
//TEST_ME
template <class T>
GridSquare* getLowestScore(T &squares){
  double lowest_score = std::numeric_limits<double>::max();
  std::pair<GridSquare*, double> lowest;
  for (auto square : squares){
    if (squares[square.first] < lowest_score){
      lowest_score = squares[square.first];
      lowest = square;
    }
  }
  return lowest.first;
}

void runAStar(StaticGrid* grid){

  //Discovered nodes
  std::vector<GridSquare*> open_set;
  open_set.push_back(grid->getCurrentSquare());

  //Visited nodes
  std::vector<GridSquare*> came_from;

  //The cost to get to a square
  std::map<GridSquare*, double> g_score;
  g_score[grid->getStartSquare()] = 0.0;

  std::map<GridSquare*, double> f_score;
  f_score[grid->getStartSquare()] = distanceBetweenSquares(grid->getStartSquare(), grid->getTargetSquare());

  //While the open set is not empty, run the algorithm
  while (open_set.size() > 0){
    auto current_square = grid->getCurrentSquare();
    *current_square = *getLowestScore(f_score);

    if (current_square == grid->getTargetSquare()){
      //Return the path
    }

    //Remove current square from the open set
    open_set.erase(std::find(open_set.begin(), open_set.end(), current_square));
  }
  
}

int main(int argc, char** argv){
  // Init a window
  window::XWindow t_window;
  t_window.init(window_height_, window_width_);

  // Create a game grid
  StaticGrid grid(t_window, 12, 16, 40);

  // Wait a hot second
  std::this_thread::sleep_for(std::chrono::milliseconds(50));

  // Wait for a MapNotify event before doing anything else
  for(;;) {
    XEvent e;
    XNextEvent(t_window.getDisplay(), &e);
    if (e.type == MapNotify)
      break;
  }

  grid.drawGrid();

  while(1) {
    //TODO: I think this needs to be in a separate thread.
		handleEvents(t_window, &grid);
	}

  // Close the window
  t_window.close();
}