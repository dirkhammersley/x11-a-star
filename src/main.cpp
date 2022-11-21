#include "main.h"
#include <chrono>
#include <thread>
#include <unordered_set>


void getEvents(window::XWindow window){
  XEvent event;
	KeySym key;
	char text[255]; // Buffer for key press events
  
  XNextEvent(window.getDisplay(), &event);
	
  if (event.type == Expose && event.xexpose.count == 0) {
    window.redraw();
  }

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

  // Wait a hot second
  std::this_thread::sleep_for(std::chrono::milliseconds(50));

  for(;;) {
    XEvent e;
    XNextEvent(t_window.getDisplay(), &e);
    if (e.type == MapNotify)
      break;
  }

  while(1) {
    //TODO: I think this needs to be in a separate thread.
		getEvents(t_window);
	}

  // Close the window
  t_window.close();
}