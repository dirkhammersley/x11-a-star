#include <chrono>
#include <thread>

#include "x_window.h"

namespace window{

XWindow::XWindow() {}

void XWindow::init(uint16_t height, uint16_t width){

  std::cout << "Creating test window!\n";

  dis_ = XOpenDisplay(0);
  screen_ = DefaultScreen(dis_);

  // Color black
  ulong black = BlackPixel(dis_, screen_);

  // Color white
	ulong white = WhitePixel(dis_, screen_);  

  win_ = XCreateSimpleWindow(dis_, DefaultRootWindow(dis_), 0, 0,	
		height, width, 5, white, black);

  XSetStandardProperties(dis_ ,win_ , "t_window", "TEST", None, NULL, 0, NULL);

  XSelectInput(dis_, win_, StructureNotifyMask | ExposureMask | ButtonPressMask | KeyPressMask);

  gc_ = XCreateGC(dis_, win_, 0 , 0);

  XSetBackground(dis_, gc_, black);
	XSetForeground(dis_, gc_, white);

  XClearWindow(dis_, win_);
	XMapRaised(dis_, win_);
  XFlush(dis_);
}

void XWindow::redraw(){
  XClearWindow(dis_, win_);
}

void XWindow::close(){
  std::cout << "Closing test window!\n";
	XFreeGC(dis_, gc_);
	XDestroyWindow(dis_, win_);
	XCloseDisplay(dis_);	
	exit(1);	
}

}

