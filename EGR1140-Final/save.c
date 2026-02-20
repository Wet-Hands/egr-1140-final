#include <stdio.h>
#include "save.h"

int load_highscore(void)
{
    int score = 0;
    FILE *f = fopen("do_not_open.txt", "r");
    if (f != NULL)
    {
        fscanf(f, "%d", &score);
        fclose(f);
    }
    return score;
}

void save_highscore(int score)
{
    int current = load_highscore();
    if (score > current)
    {
        FILE *f = fopen("do_not_open.txt", "w");
        if (f != NULL)
        {
            fprintf(f, "%d", score);
            fclose(f);
        }
    }
}
