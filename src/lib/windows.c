#include <stdbool.h>
#include <stdio.h>
#include <windows.h>

#ifdef UNICODE
#define GetMonitorInfo GetMonitorInfoW
#define LPSTR LPWSTR
#else
#define GetMonitorInfo GetMonitorInfoA
#endif

bool isDryRun = false;

int move_window(HWND window, int x, int y)
{
    if (isDryRun)
    {
        printf("Moving window to x: %d, y: %d\n", x, y);
        return 0;
    }
    return SetWindowPos(window, HWND_TOP, x, y, 0, 0, SWP_NOSIZE);
}

int resize_window(HWND window, int width, int height)
{
    if (isDryRun)
    {
        printf("Resizing window to width: %d, height: %d\n", width, height);
        return 0;
    }
    return SetWindowPos(window, HWND_TOP, 0, 0, width, height, SWP_NOMOVE);
}

int place_focused_window(bool isLeft, bool isRight, int width, int height, bool dryRun)
{
    isDryRun = dryRun;
    HWND handle;
    HMONITOR monitor;
    MONITORINFO screen;

    if (!isDryRun)
    {
        handle = GetForegroundWindow();

        if (handle == NULL)
        {
            fprintf(stderr, "Cannot get focused window\n");
            return 1;
        }

        monitor = MonitorFromWindow(handle, MONITOR_DEFAULTTOPRIMARY);

        if (monitor == NULL)
        {
            fprintf(stderr, "Cannot get monitor\n");
            return 1;
        }

        screen.cbSize = sizeof(MONITORINFO);
        GetMonitorInfo(monitor, &screen);
    }

    int screenWidth = isDryRun ? 3840 : screen.rcMonitor.right;
    int screenHeight = isDryRun ? 1080 : screen.rcMonitor.bottom;

    width = width == 0 ? screenWidth / 2 : width;
    height = height == 0 ? screenHeight : height;

    int result = 0;

    // Get taskbar height
    int taskbar_height;
    int iYScreenSize;
    RECT rWorkArea;

    if (!isDryRun)
    {
        iYScreenSize = GetSystemMetrics(SM_CYSCREEN);
        SystemParametersInfo(SPI_GETWORKAREA, 0, (PVOID)&rWorkArea, 0);
        taskbar_height = iYScreenSize - (rWorkArea.bottom - rWorkArea.top);
    }
    else
    {
        taskbar_height = 48;
    }

    if (isLeft || isRight)
    {
        int x = isLeft ? 0 : screenWidth * 0.75;
        result = move_window(handle, x, 0) + resize_window(handle, width, height - taskbar_height);
    }
    else
    {
        if (isDryRun)
        {
            printf("Direction: center\n");
        }
        result = move_window(handle, width / 2, 0) + resize_window(handle, width, height - taskbar_height);
    }

    return result;
}