#include <stdbool.h>
#include <stdio.h>
#include <windows.h>

#ifdef UNICODE
#define GetMonitorInfo GetMonitorInfoW
#define LPSTR LPWSTR
#else
#define GetMonitorInfo GetMonitorInfoA
#endif

struct ScreenResolution *getScreenResolution(bool isDryRun)
{
    struct ScreenResolution *resolution = malloc(sizeof(struct ScreenResolution));

    if (isDryRun)
    {
        resolution->width = 3840;
        resolution->height = 1080;
        return resolution;
    }

    HWND handle = GetForegroundWindow();
    HMONITOR monitor;
    MONITORINFO screen;

    if (handle == NULL)
    {
        fprintf(stderr, "Cannot get focused window\n");
        return NULL;
    }

    monitor = MonitorFromWindow(handle, MONITOR_DEFAULTTOPRIMARY);

    if (monitor == NULL)
    {
        fprintf(stderr, "Cannot get monitor\n");
        return NULL;
    }

    screen.cbSize = sizeof(MONITORINFO);
    GetMonitorInfo(monitor, &screen);

    resolution->width = screen.rcMonitor.right;
    resolution->height = screen.rcMonitor.bottom;

    return resolution;
}

int placeFocusedWindow(int top, int left, int width, int height, bool isDryRun)
{
    if (isDryRun)
    {
        printf("Resizing window to width: %d, height: %d\n", width, height);
        printf("Moving window to left: %d, top: %d\n", left, top);
        return 0;
    }

    HWND handle = GetForegroundWindow();
    int taskbar_height;
    int iYScreenSize;
    RECT rWorkArea;

    iYScreenSize = GetSystemMetrics(SM_CYSCREEN);
    SystemParametersInfo(SPI_GETWORKAREA, 0, (PVOID)&rWorkArea, 0);
    taskbar_height = iYScreenSize - (rWorkArea.bottom - rWorkArea.top);
    SetWindowPos(handle, HWND_TOP, 0, 0, width, height, SWP_NOMOVE);
    SetWindowPos(handle, HWND_TOP, left, top, 0, 0, SWP_NOSIZE);
    return 0;
}
