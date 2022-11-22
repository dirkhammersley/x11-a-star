#include "main.h"
#include <chrono>
#include <thread>
#include <unordered_set>


void getEvents(window::XWindow window){
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

    if (text[0] == 'a'){
      
    }

    std::cout << "Keypress: " << text[0] << "\n";
  }

  if (event.type == ButtonPress) {
    window.redraw();
    XDrawLine(window.getDisplay(), window.getWindow(), window.getGc(), 0, 0, event.xbutton.x, event.xbutton.y);
    XFlush(window.getDisplay());
    std::cout << "Click at: " << event.xbutton.x << ", " << event.xbutton.y << "\n";
  }
}

StaticGrid::StaticGrid(window::XWindow window) : window_{window}{}

void StaticGrid::drawGrid(int w_cells, int h_cells){
  int x_start = 50;
  int y_start = 50;
  int cell_height = 40;
  int cell_width = 40;

  window_.redraw();
  for (int i = 0; i < w_cells; i++){
    bool should_fill = i % 2 == 0;
    for (int j = 0; j < h_cells; j++){
      should_fill = !should_fill;
      drawSquare(x_start + j * cell_height, y_start + i * cell_width, cell_height, cell_width, should_fill);
    }
  }
  XFlush(window_.getDisplay());
}

void StaticGrid::drawSquare(int x, int y, int w, int h, bool full){
  
  if (full == true){
    XSetForeground(window_.getDisplay(), window_.getGc(), window_.colors.cyber_red);
    XFillRectangle(window_.getDisplay(), window_.getWindow(), window_.getGc(), x, y, w, h);
  }else{
    XSetForeground(window_.getDisplay(), window_.getGc(), window_.colors.cyber_blue);
    XFillRectangle(window_.getDisplay(), window_.getWindow(), window_.getGc(), x, y, w, h);
  }
}

template <class T>
bool isClose(T a, T b, double tol){
  return abs(a - b) < tol;
}

double distanceBetweenPoints(){
  // Distance formula
  return 0;
}

int main(int argc, char** argv){
  // Init a window
  window::XWindow t_window;
  t_window.init(window_height_, window_width_);
  StaticGrid grid(t_window);

  // Wait a hot second
  std::this_thread::sleep_for(std::chrono::milliseconds(50));

  for(;;) {
    XEvent e;
    XNextEvent(t_window.getDisplay(), &e);
    if (e.type == MapNotify)
      break;
  }

  grid.drawGrid(8, 8);

  while(1) {
    //TODO: I think this needs to be in a separate thread.
		getEvents(t_window);
	}

  // Close the window
  t_window.close();
}