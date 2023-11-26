#include <stdbool.h>
#include <stdio.h>
#include <X11/Xlib.h>

bool isDryRun = false;

int resize_window(Display *display, Window window, int width, int height)
{
    if (isDryRun)
    {
        printf("Resizing window to width: %d, height: %d\n", width, height);
        return 0;
    }
    return XResizeWindow(display, window, width, height) + XFlush(display);
}

int move_window(Display *display, Window window, int x, int y)
{
    if (isDryRun)
    {
        printf("Moving window to x: %d, y: %d\n", x, y);
        return 0;
    }
    return XMoveWindow(display, window, x, y) + XFlush(display);
}

int place_focused_window(bool isLeft, bool isRight, int width, int height, bool dryRun)
{
    isDryRun = dryRun;

    Display *display = isDryRun ? NULL : XOpenDisplay(NULL);
    int result = 0;

    if (display == NULL && !isDryRun)
    {
        fprintf(stderr, "Cannot open display\n");
        return 1;
    }

    Window focused_window;
    int revert_to;
    Screen *screen;

    if (!isDryRun)
    {
        XGetInputFocus(display, &focused_window, &revert_to);
        screen = XDefaultScreenOfDisplay(display);
    }

    int screenWidth = isDryRun ? 3840 : screen->width;
    int screenHeight = isDryRun ? 1080 : screen->height;

    width = width == 0 ? screenWidth / 2 : width;
    height = height == 0 ? screenHeight : height;

    if (isLeft || isRight)
    {

        result = move_window(display, focused_window, isLeft ? 0 : screenWidth / 2, 0) + resize_window(display, focused_window, width, height);
    }
    else
    {
        if (isDryRun)
        {
            printf("Direction: center\n");
        }
        result = move_window(display, focused_window, width / 2, 0) + resize_window(display, focused_window, width, height);
    }

    return result;
}