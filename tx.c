#include <unistd.h>

#include "curses.h"

#include "wink.h" // FRAMES_PER_BIT

int main()
{
    initscr();

    if (has_colors()) {
        start_color();

        init_pair(1, COLOR_BLACK, COLOR_BLACK);
        init_pair(2, COLOR_RED, COLOR_RED);
        init_pair(3, COLOR_GREEN, COLOR_GREEN);
        init_pair(4, COLOR_YELLOW, COLOR_YELLOW);
        init_pair(5, COLOR_BLUE, COLOR_BLUE);
        init_pair(6, COLOR_MAGENTA, COLOR_MAGENTA);
        init_pair(7, COLOR_CYAN, COLOR_CYAN);
        init_pair(8, COLOR_WHITE, COLOR_WHITE);

    }

    int rows, cols;

    getmaxyx(stdscr, rows, cols);

    int num = 0;

    while (1) {
        attrset(COLOR_PAIR((num % 8) + 1));

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                move(i, j);
                printw(" ");
            }
        }

        refresh();
        sleep(1);

        num = (num < 7) ? num + 1 : 0;
    }

    endwin(); // FIXME: clean up on SIGINT

    return 0;
}
