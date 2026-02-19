#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include "ui.h"
#include "types.h"

void setColor(int colorValue)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, colorValue);
}

int title_screen(int highscore)
{
    int selected = 0;

    while (1)
    {
        system("cls");
        setColor(12);
        printf("\n");
        printf("  ██████╗ ██╗   ██╗███╗   ██╗ ██████╗ ███████╗ ██████╗ ███╗   ██╗\n");
        printf("  ██╔══██╗██║   ██║████╗  ██║██╔════╝ ██╔════╝██╔═══██╗████╗  ██║\n");
        printf("  ██║  ██║██║   ██║██╔██╗ ██║██║  ███╗█████╗  ██║   ██║██╔██╗ ██║\n");
        printf("  ██║  ██║██║   ██║██║╚██╗██║██║   ██║██╔══╝  ██║   ██║██║╚██╗██║\n");
        printf("  ██████╔╝╚██████╔╝██║ ╚████║╚██████╔╝███████╗╚██████╔╝██║ ╚████║\n");
        printf("  ╚═════╝  ╚═════╝ ╚═╝  ╚═══╝ ╚═════╝ ╚══════╝ ╚═════╝ ╚═╝  ╚═══╝\n");
        printf("\n");
        setColor(14);
        printf("                         ██████╗  ██████╗  ██████╗ ███╗   ███╗\n");
        printf("                         ██╔══██╗██╔═══██╗██╔═══██╗████╗ ████║\n");
        printf("                         ██║  ██║██║   ██║██║   ██║██╔████╔██║\n");
        printf("                         ██║  ██║██║   ██║██║   ██║██║╚██╔╝██║\n");
        printf("                         ██████╔╝╚██████╔╝╚██████╔╝██║ ╚═╝ ██║\n");
        printf("                         ╚═════╝  ╚═════╝  ╚═════╝ ╚═╝     ╚═╝\n");
        printf("\n");

        if (highscore > 0)
        {
            setColor(14);
            printf("                        \u2605 High Score: %d Points \u2605\n\n", highscore);
        }
        else
        {
            setColor(8);
            printf("                          High Score: None\n\n");
        }

        setColor(7);
        printf("                     W / S to navigate  |  ENTER to confirm\n\n");

        if (selected == 0) setColor(10); else setColor(8);
        printf("                        %s  NORMAL (Score x 1)\n", selected == 0 ? ">" : " ");
        printf("                             HP: %-3d  Stamina: %d\n\n",
               PLAYER_HEALTH_NORMAL, PLAYER_STAMINA_NORMAL);

        if (selected == 1) setColor(12); else setColor(8);
        printf("                        %s  HARD (Score x 2)\n", selected == 1 ? ">" : " ");
        printf("                             HP: %-3d  Stamina: %d\n\n",
               PLAYER_HEALTH_HARD, PLAYER_STAMINA_HARD);

        if (selected == 2) setColor(13); else setColor(8);
        printf("                        %s  LUKA (Score x 5)\n", selected == 2 ? ">" : " ");
        printf("                             HP: %-3d  Stamina: %d\n\n",
               PLAYER_HEALTH_LUKA, PLAYER_STAMINA_LUKA);

        setColor(8);
        printf("                             Q to quit\n");
        setColor(7);

        char key = _getch();

        if (key == 'w' && selected > 0) selected--;
        if (key == 's' && selected < 2) selected++;
        if (key == '\r') break;
        if (key == 'q') return -1;
    }

    return selected + 1;
}
