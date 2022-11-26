#include "main.h"
#include <chrono>
#include <thread>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <map>
#include <cmath>

#include "grid.h"


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
  }

  if (event.type == ButtonPress) {
    bool is_obs = grid->getSquareAt(event.xbutton.x, event.xbutton.y)->isObstacle();
    if (!is_obs){
      window.redraw();
      grid->setTargetSquare(grid->getSquareAt(event.xbutton.x, event.xbutton.y));
      grid->setStartSquare(grid->getCurrentSquare());
      grid->drawGrid();
      runAStar(grid, window);
    }
    std::cout << "Click at: " << event.xbutton.x << ", " << event.xbutton.y << "\n";
  }
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



//TEST_ME
GridSquare* getLowestScore(std::map<GridSquare*, double> &scores, std::vector<GridSquare*> &squares){
  double lowest_score = std::numeric_limits<double>::max();
  GridSquare* lowest;
  for (auto square : squares){
    if (scores[square] < lowest_score){
      lowest_score = scores[square];
      lowest = square;
    }
  }
  return lowest;
}

void runAStar(StaticGrid* grid, window::XWindow window){
  //Discovered nodes
  std::vector<GridSquare*> open_set;
  open_set.push_back(grid->getStartSquare());

  //Visited nodes
  std::map<GridSquare*, GridSquare*> came_from;

  //The cost to get to a square
  std::map<GridSquare*, double> g_score;
  g_score[grid->getStartSquare()] = std::numeric_limits<double>::max();

  std::map<GridSquare*, double> f_score;
  f_score[grid->getStartSquare()] = distanceBetweenSquares(grid->getStartSquare(), grid->getTargetSquare());

  bool reached_goal = false;

  //While the open set is not empty, run the algorithm
  while (open_set.size() > 0){
    std::cout << "Beginning A* loop!" << std::endl;
    auto current_square = getLowestScore(f_score, open_set);
    grid->setCurrentSquare(current_square);
    std::vector<GridSquare*> neighbors;
    std::cout << "Choosing square at (" << current_square->getCenter().first << 
                  ", " << current_square->getCenter().second << ")" 
                  << std::endl;
    current_square->draw(window, window.colors.cyber_red);
    //Wait so we can see progress
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    if (*current_square == *(grid->getTargetSquare())){
      reached_goal = true;
      std::cout << "Made it to target square!!" << std::endl;
      break;
      //Return the path
    }

    //Remove current square from the open set
    open_set.erase(std::find(open_set.begin(), open_set.end(), current_square));

    //Explore each neighboring square
    grid->getNeighboringSquares(current_square, neighbors);
    for (auto n : neighbors){
      int x = n->getCenter().first;
      int y = n->getCenter().second;
      std::cout << "Evaluating neighboring square at (" << x << 
                  ", " << y << ")" 
                  << std::endl;
      XSetForeground(window.getDisplay(), window.getGc(), window.colors.cyber_white);
      auto guess_g_score = distanceBetweenSquares(current_square, grid->getTargetSquare()) + (double) grid->getSquareSize();
      auto curr_g_score = distanceBetweenSquares(n, grid->getTargetSquare());
      #ifdef debug
      XDrawString(window.getDisplay(), 
              window.getWindow(), 
              window.getGc(), 
              n->getCenter().first,
              n->getCenter().second,  
              std::to_string((int) curr_g_score).c_str(), 
              std::to_string((int) curr_g_score).size());
      XFlush(window.getDisplay());
      #endif
      std::cout << "Estimated G score for current square: " << curr_g_score << std::endl;
      std::cout << "Estimated G score for this neighbor square: " << guess_g_score << std::endl;
      if ( curr_g_score < guess_g_score){
        came_from[n] = current_square;
        g_score[n] = guess_g_score;
        f_score[n] = g_score[n] + distanceBetweenSquares(n, grid->getTargetSquare());
        if (std::find(open_set.begin(), open_set.end(), n) == open_set.end()){
          open_set.push_back(n);
        }
      }
    }
  }
  //Failure case
  if (!reached_goal) {std::cout << "Failed to find path!" << std::endl;}
}

//TODO 1) Add obstacles
//TODO 2) Get target square from a click before calling A-star, on a new click record
//        current position of seeker as the "start" for a new plan
int main(int argc, char** argv){
  // Init a window
  window::XWindow t_window;
  t_window.init(window_height_, window_width_);

  // Create a game grid
  StaticGrid grid(t_window, 100, 120, 10);

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