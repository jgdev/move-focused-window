#include <stdbool.h>
#include <stdio.h>
#include <X11/Xlib.h>

Display *getDisplay()
{
    Display *display = XOpenDisplay(NULL);
    if (display == NULL)
    {
        fprintf(stderr, "Cannot open display\n");
        return NULL;
    }
    return display;
}

Window *getWindow(Display *display)
{
    Window window;
    int revert_to;
    XGetInputFocus(display, &window, &revert_to);
    return window;
}

int resizeWindow(int width, int height, bool isDryRun)
{
    Display *display = getDisplay();
    Window *window = getWindow(display);
    XResizeWindow(display, window, width, height);
    XFlush(display);
    XCloseDisplay(display);
}

int moveWindow(int y, int x, bool isDryRun)
{
    Display *display = getDisplay();
    Window *window = getWindow(display);
    XMoveWindow(display, window, x, y);
    XFlush(display);
    XCloseDisplay(display);
}

struct ScreenResolution *getScreenResolution(bool isDryRun)
{
    struct ScreenResolution *resolution = malloc(sizeof(struct ScreenResolution));

    if (isDryRun)
    {
        resolution->width = 3840;
        resolution->height = 1080;
        return resolution;
    }

    Display *display = getDisplay(isDryRun);
    Screen *screen = XDefaultScreenOfDisplay(display);

    resolution->width = screen->width;
    resolution->height = screen->height;

    if (display != NULL)
    {
        XCloseDisplay(display);
    }

    return resolution;
}

int placeFocusedWindow(int top, int left, int width, int height, bool isDryRun)
{
    if (isDryRun)
    {
        printf("Resizing window to width: %d, height: %d\n", width, height);
        printf("Moving window to left: %d, top: %d\n", x, y);
        return 0;
    }
    moveWindow(top, left, isDryRun);
    resizeWindow(width, height, isDryRun);
}
