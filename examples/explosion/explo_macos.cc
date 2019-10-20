// ************************************
// Copyrights by Jin Fagang
// 1/19/19-19-12
// xplo_linux
// jinfagang19@gmail.com
//
// Explosion Lab
// ************************************

//
// Created by jintain on 1/19/19.
//


#include <X11/Xlib.h>
#include <assert.h>
#include <unistd.h>
#include <X11/Xutil.h>
#include <vector>
#include <iostream>
#include <string.h>
#include <stdio.h>

#define NIL (0)

using namespace std;

static const char *event_names[] = {
        "",
        "",
        "KeyPress",
        "KeyRelease",
        "ButtonPress",
        "ButtonRelease",
        "MotionNotify",
        "EnterNotify",
        "LeaveNotify",
        "FocusIn",
        "FocusOut",
        "KeymapNotify",
        "Expose",
        "GraphicsExpose",
        "NoExpose",
        "VisibilityNotify",
        "CreateNotify",
        "DestroyNotify",
        "UnmapNotify",
        "MapNotify",
        "MapRequest",
        "ReparentNotify",
        "ConfigureNotify",
        "ConfigureRequest",
        "GravityNotify",
        "ResizeRequest",
        "CirculateNotify",
        "CirculateRequest",
        "PropertyNotify",
        "SelectionClear",
        "SelectionRequest",
        "SelectionNotify",
        "ColormapNotify",
        "ClientMessage",
        "MappingNotify"
};


void drawSimpleWindow() {
  Display *dpy = XOpenDisplay(NIL);
  assert(dpy);

  // Get some colors

  int blackColor = BlackPixel(dpy, DefaultScreen(dpy));
  int whiteColor = WhitePixel(dpy, DefaultScreen(dpy));

  // Create the window
  Window w = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), 0, 0,
                                 200, 100, 0, blackColor, blackColor);

  // We want to get MapNotify events
  XSelectInput(dpy, w, StructureNotifyMask);
  // "Map" the window (that is, make it appear on the screen)
  XMapWindow(dpy, w);
  // Create a "Graphics Context"
  GC gc = XCreateGC(dpy, w, 0, NIL);
  // Tell the GC we draw using the white color
  XSetForeground(dpy, gc, whiteColor);
  // Wait for the MapNotify event

  for (;;) {
    XEvent e;
    XNextEvent(dpy, &e);
    if (e.type == MapNotify)
      break;
  }

  // Draw the line
  XDrawLine(dpy, w, gc, 10, 60, 180, 20);
  // Send the "DrawLine" request to the server
  XFlush(dpy);
  // Wait for 10 seconds
  sleep(10);
}

void mouseMove(Display &display, int x, int y) {
  if (&display != nullptr) {
    XWarpPointer(&display, None, None, 0, 0, 0, 0, x, y);
    XCloseDisplay(&display);
  }
  XFlush(&display);
}

void getCurseCoords(Display &display, int &x, int &y) {
  XEvent event;
  XQueryPointer (&display, DefaultRootWindow (&display),
                 &event.xbutton.root, &event.xbutton.window,
                 &event.xbutton.x_root, &event.xbutton.y_root,
                 &event.xbutton.x, &event.xbutton.y,
                 &event.xbutton.state);
  x = event.xbutton.x;
  y = event.xbutton.y;
}

void mouseClick(Display *display, int button){
  cout << "start to click.\n";
  XEvent event;
  memset(&event, 0x00, sizeof(event));

  event.type = ButtonPress;
  event.xbutton.button = button;
  event.xbutton.same_screen = True;

  XQueryPointer(display, RootWindow(display, DefaultScreen(display)), &event.xbutton.root, &event.xbutton.window, &event.xbutton.x_root, &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);

  event.xbutton.subwindow = event.xbutton.window;

  while(event.xbutton.subwindow)
  {
    event.xbutton.window = event.xbutton.subwindow;

    XQueryPointer(display, event.xbutton.window, &event.xbutton.root, &event.xbutton.subwindow, &event.xbutton.x_root, &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);
  }

  if(XSendEvent(display, PointerWindow, True, 0xfff, &event) == 0) fprintf(stderr, "Error\n");

  XFlush(display);

  usleep(100000);

  event.type = ButtonRelease;
  event.xbutton.state = 0x100;

  if(XSendEvent(display, PointerWindow, True, 0xfff, &event) == 0) fprintf(stderr, "Error\n");

  XFlush(display);
}

void mouseClickHold(){

}


void monitorEvent() {
  Display* display = XOpenDisplay(NULL);
  if (display == NULL) {
    exit(0);
  }

  int screen = DefaultScreen(display);
  GC gc = DefaultGC(display, screen);
  Window parent_window = DefaultRootWindow(display);

  int x = 0;
  int y = 0;

  unsigned int width = 400;
  unsigned int height = 40;

  unsigned int border_width = 1;

  unsigned int border_color = BlackPixel(display, screen);
  unsigned int background_color = WhitePixel(display, screen);

  // Create window
  Window hello_window = XCreateSimpleWindow(display, parent_window,
                                            x,
                                            y,
                                            width,
                                            height,
                                            border_width,
                                            border_color,
                                            background_color);

  long event_mask = ExposureMask
                    | KeyPressMask
                    | KeyReleaseMask
                    | ButtonPressMask
                    | ButtonReleaseMask
                    | FocusChangeMask
  ;

  // Select window events
  XSelectInput(display, hello_window, event_mask);
  // Make window visible
  XMapWindow(display, hello_window);
  // Set window title
  XStoreName(display, hello_window, "Hello, World!");
  // Get WM_DELETE_WINDOW atom
  Atom wm_delete = XInternAtom(display, "WM_DELETE_WINDOW", True);
  // Subscribe WM_DELETE_WINDOW message
  XSetWMProtocols(display, hello_window, &wm_delete, 1);

  char msg[1024] = "";
  char key[32];

  // Event loop
  for (;;) {
    // Get next event from queue
    XEvent event;
    XNextEvent(display, &event);

    // Print event type
    printf("got event: %s\n", event_names[event.type]);

    // Keyboard
    if (event.type == KeyPress) {
      int len = XLookupString(&event.xkey, key, sizeof(key) - 1, 0, 0);
      key[len] = 0;

      if (strlen(msg) > 50)
        msg[0] = 0;

      strcat(msg, key);
      strcat(msg, " ");
    }

    // Refresh
    if (event.type == KeyPress || event.type == Expose) {
      XClearWindow(display, hello_window);
      XDrawString(display, hello_window, gc, 10, 20, msg, strlen(msg));
    }

    // Close button
    if (event.type == ClientMessage) {
      if (event.xclient.data.l[0] == wm_delete) {
        break;
      }
    }
  }

  XCloseDisplay(display);
}

int main() {
  // Open the display
//  drawSimpleWindow();

//  cout << "start monitor event.\n";
//  monitorEvent();

  sleep(4);
  Display *display = XOpenDisplay(NULL);
  if (display == NULL) {
    cerr << "can not open display.\n";
  } else {
//    int x, y;
//    getCurseCoords(*display, x, y);
//    printf("current pos: x: %d, y: %d\n", x, y);
    mouseMove(*display, 400, 10);
//    XFlush(display);
    mouseClick(display, Button1);
    mouseClick(display, Button2);
    cout << "right click\n";
    mouseClick(display, Button3);
  }



  return 0;


}
