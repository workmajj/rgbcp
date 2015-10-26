#include <assert.h>
#include <unistd.h>

#include "curses.h"

typedef unsigned int uint; // FIXME: shared

typedef enum {X, R, G, B} FrameColor; // FIXME: shared

void color_show(const FrameColor c, const uint rows, const uint cols)
{
    switch (c) {
    case R:
        attrset(COLOR_PAIR(1));
        break;
    case G:
        attrset(COLOR_PAIR(2));
        break;
    case B:
        attrset(COLOR_PAIR(3));
        break;
    default:
        assert(0 && "bad color");
    }

    for (uint i = 0; i < rows; i++) {
        for (uint j = 0; j < cols; j++) {
            move(i, j);
            printw(" ");
        }
    }

    refresh();
}

int main(void)
{
    const size_t RGB_SIZE = 4;
    const FrameColor RGB_STEP[] = {R, G, B, G};

    const size_t MS_SIZE = 3;
    const uint MS_STEP[] = {66, 66, 67}; // 15 fps

    uint rows, cols;

    initscr();
    getmaxyx(stdscr, rows, cols);

    // FIXME: has_colors()

    start_color();
    init_pair(1, COLOR_RED, COLOR_RED);
    init_pair(2, COLOR_GREEN, COLOR_GREEN);
    init_pair(3, COLOR_BLUE, COLOR_BLUE);

    uint idx_rgb = 0;
    uint idx_ms = 0;

    while (1) {
        color_show(RGB_STEP[idx_rgb], rows, cols);
        usleep(MS_STEP[idx_ms] * 1000);

        idx_rgb = (idx_rgb == RGB_SIZE - 1) ? 0 : idx_rgb + 1;
        idx_ms = (idx_ms == MS_SIZE - 1) ? 0 : idx_ms + 1;
    }

    endwin(); // FIXME: clean up on SIGINT

    return 0;
}
