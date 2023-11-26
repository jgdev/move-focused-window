#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include "lib/windows.c"
#elif __linux__
#include "lib/linux.c"
#endif

int arg_count;
char **arg_values;
char *commands[] = {"--left", "--right", "--width", "--height"};

int get_arg_index(char *argument)
{
    int index = -1;
    for (int i = 0; i < arg_count; i++)
    {
        if (strcmp(arg_values[i], argument) == 0)
        {
            index = i;
        }
    }
    return index;
}

bool is_arg_command(char *argument)
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

char *get_arg_value(char *argument)
{
    char *result = "";
    int index = get_arg_index(argument);
    if (index > -1)
    {
        char *arg_value = arg_values[index + 1];
        if (arg_value == NULL || is_arg_command(arg_value))
            return "";
        return arg_value;
    }
    return result;
}

int main(int argc, char const *argv[])
{
    arg_count = argc;
    memcpy(&arg_values, &argv, sizeof(char **));

    // Define width and height parameters
    bool isLeft = get_arg_index("--left") > -1;
    bool isRight = get_arg_index("--right") > -1;
    bool dryRun = get_arg_index("--dry-run") > -1;
    char *width_arg = get_arg_value("--width");
    char *height_arg = get_arg_value("--height");

    int width = width_arg[0] == '\0' ? 0 : atoi(width_arg);
    int height = height_arg[0] == '\0' ? 0 : atoi(height_arg);

    // return 0;
    return place_focused_window(isLeft, isRight, width, height, dryRun);
}
