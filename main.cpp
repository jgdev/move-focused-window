#include <stdio.h>
#include <X11/Xlib.h>

int resizeWindow(Display *display, Window window, int width, int height)
{
    XResizeWindow(display, window, width, height);
    XFlush(display);
    return 0;
}

int moveWindow(Display *display, Window window, int x, int y)
{
    XMoveWindow(display, window, x, y);
    XFlush(display);
    return 0;
}

int main(int argc, char const *argv[])
{
    // Get current display
    Display *display;

    if (!(display = XOpenDisplay(NULL)))
    {
        printf("No current display\n");
        return -1;
    }

    Window focusedWindow;
    int rev;

    // Get current window
    XGetInputFocus(display, &focusedWindow, &rev);

    // Get current screen
    Screen *screen = XDefaultScreenOfDisplay(display);

    // Resize current window
    resizeWindow(display, focusedWindow, screen->width / 2, screen->height);

    // Move current window
    moveWindow(display, focusedWindow, screen->width / 4, 0);

    // Close display
    return XCloseDisplay(display);
}
