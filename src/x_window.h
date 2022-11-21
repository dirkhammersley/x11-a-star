// Make a simple window with a monocolor background

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

#include <memory>
#include <iostream>
#include <vector>

namespace window{

class XWindow {
  public:

    XWindow();

    Display *getDisplay(){
      return dis_;
    }

    Window getWindow(){
      return win_;
    }

    int getScreen(){
      return screen_;
    }

    GC getGc(){
      return gc_;
    }

    void init(uint16_t height, uint16_t width);

    void redraw();

    void close();

  private:
    Display *dis_;
		int screen_;
		Window win_;
		GC gc_;

    std::vector<XEvent> event_buffer_;
	  KeySym key_;
	  char text[255]; // Buffer for key press events

};

} //namespace window

