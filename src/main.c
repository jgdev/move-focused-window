#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

#ifdef _WIN32
#include "lib/windows.c"
#elif __linux__
#include "lib/linux.c"
#endif

int arg_count;
char **argValues;
char *commands[] = {"--direction", "--top", "--left", "--width", "--height"};

int getArgIndex(char *argument)
{
    int index = -1;
    for (int i = 0; i < arg_count; i++)
    {
        if (strcmp(argValues[i], argument) == 0)
        {
            index = i;
        }
    }
    return index;
}

bool isArgCommand(char *argument)
{
    int length = sizeof(commands) / sizeof(commands[0]);

    for (int i = 0; i < length; i++)
    {
        if (strcmp(commands[i], argument) == 0)
        {
            return true;
        }
    }
    return false;
}

char *getArgValue(char *argument, char *defaultValue)
{
    char *result = "";
    int index = getArgIndex(argument);
    if (index > -1)
    {
        char *argValue = argValues[index + 1];
        if (argValue == NULL || argValue == '\0' || isArgCommand(argValue))
            return defaultValue;
        return argValue;
    }
    return defaultValue;
}

int getIntArgValue(char *argument, int defaultValue)
{
    char *arg = getArgValue(argument, "");
    int result = arg[0] == '\0' ? defaultValue : atoi(arg);
    return result;
}

int main(int argc, char const *argv[])
{
    arg_count = argc;
    memcpy(&argValues, &argv, sizeof(char **));

    bool isDryRun = getArgIndex("--dry-run") > -1;

    struct ScreenResolution *resolution = getScreenResolution(isDryRun);

    int top = getIntArgValue("--top", 0);
    int left = getIntArgValue("--left", 0);
    char *direction = getArgValue("--direction", "center");
    int width = getIntArgValue("--width", resolution->width / 2);
    int height = getIntArgValue("--height", resolution->height);

    top = getArgIndex("--height") > -1 ? (resolution->height / 2) - (height / 2) : 0;

    if (strcmp(direction, "left") == 0)
    {
        left = 0;
    }
    else if (strcmp(direction, "right") == 0)
    {
        left = (width - resolution->width) * -1;
    }
    else
    {
        left = (resolution->width / 2) - (width / 2);
    }

    if (isDryRun)
        printf("Direction: %s\n", direction);

    placeFocusedWindow(top, left, width, height, isDryRun);
}
