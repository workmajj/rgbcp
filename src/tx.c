#include <assert.h>
#include <unistd.h>

#include "curses.h"

#define SYNC_MS 1000

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

    // start
    color_show(G, rows, cols);
    usleep(SYNC_MS * 1000);

    int c;

    uint ms_idx = 0;

    while ((c = getchar()) != EOF) {
        for (uint i = 0; i < 8; i++) {
            if (c & (0b10000000 >> i)) {
                color_show(B, rows, cols); // 1

                usleep(MS_STEP[ms_idx] * 1000);
                ms_idx = (ms_idx == MS_SIZE - 1) ? 0 : ms_idx + 1;
            }
            else {
                color_show(R, rows, cols); // 0

                usleep(MS_STEP[ms_idx] * 1000);
                ms_idx = (ms_idx == MS_SIZE - 1) ? 0 : ms_idx + 1;
            }

            color_show(G, rows, cols);

            usleep(MS_STEP[ms_idx] * 1000);
            ms_idx = (ms_idx == MS_SIZE - 1) ? 0 : ms_idx + 1;
        }
    }

    // stop
    color_show(G, rows, cols);
    usleep(SYNC_MS * 1000);

    endwin(); // FIXME: clean up on SIGINT

    return 0;
}
