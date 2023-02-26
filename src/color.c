#include <stdio.h>
#include <stdlib.h>
#include "color.h"

char *COLORS[] = {BLK, RED, GRN, YEL, BLU, MAG, CYN, WHT};
const int colorsSize = sizeof(COLORS) / sizeof(char *);

char *get_color(int index)
{
    if (index < colorsSize)
    {
        return COLORS[index];
    }
    else
    {
        fprintf(stderr, "Color with index %d not found\n", index);
        printf("Available colors: \n");
        printf("%s %d  - Black\n", BLK, 0);
        printf("%s %d  - Red\n", RED, 1);
        printf("%s %d  - Green\n", GRN, 2);
        printf("%s %d  - Yellow\n", YEL, 3);
        printf("%s %d  - Blue\n", BLU, 4);
        printf("%s %d  - Magenta\n", MAG, 5);
        printf("%s %d  - Cyan\n", CYN, 6);
        printf("%s %d  - White\n", WHT, 7);
    }

    return 0;
}

int color(char **args)
{
    const char *color = NULL;

    if (args[0] == NULL)
    {
        fprintf(stderr, "hshell: expected argument to \"cd\"\n");
    }

    if (args[1] == NULL)
    {
        color = get_color(7);
    }
    else
    {
        char *colorIndex = args[1];
        const int index = atoi(colorIndex);
        color = get_color(index);
    }

    printf("%s", color);
    return 1;
}