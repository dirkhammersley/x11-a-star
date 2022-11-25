// Make a simple window with a monocolor background
#pragma once
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
    
    // Colors can be defined with B + (G<<8) + (R<<16)
    struct window_colors {
      ulong cyber_red = 127 + (0<<8) + (255<<16);
      ulong cyber_blue = 255 + (0<<8) + (127<<16);
      ulong cyber_green = 200 + (255<<8) + (0<<16);
      ulong cyber_white = 252 + (255<<8) + (255<<16);
    };

    window_colors colors;

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

