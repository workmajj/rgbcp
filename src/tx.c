#include <assert.h>
#include <unistd.h>

#include "curses.h"

#define SYNC_DURATION 1000

typedef unsigned int uint;

typedef enum {X, R, G, B} FrameColor; // FIXME: shared

uint term_rows, term_cols;

uint frame_step(void)
{
    const size_t SIZE = 3;
    const uint DURATION_MS[] = {66, 66, 67}; // 15 fps

    static uint idx = 0;
    uint r = idx;

    idx = (idx == SIZE - 1) ? 0 : idx + 1;

    return DURATION_MS[r];
}

void frame_show(const FrameColor c, const uint duration_ms)
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

    for (uint i = 0; i < term_rows; i++) {
        for (uint j = 0; j < term_cols; j++) {
            move(i, j);
            printw(" ");
        }
    }

    refresh();

    usleep(duration_ms * 1000); // millis to micros
}

int main(void)
{
    initscr();

    // FIXME: check has_colors()

    getmaxyx(stdscr, term_rows, term_cols);

    start_color();
    init_pair(1, COLOR_RED, COLOR_RED);
    init_pair(2, COLOR_GREEN, COLOR_GREEN);
    init_pair(3, COLOR_BLUE, COLOR_BLUE);

    frame_show(G, SYNC_DURATION); // start

    int c;

    while ((c = getchar()) != EOF) {
        for (uint i = 0; i < 8; i++) {
            if (c & (0b10000000 >> i)) {
                frame_show(B, frame_step()); // 1
            }
            else {
                frame_show(R, frame_step()); // 0
            }

            frame_show(G, frame_step());
        }
    }

    frame_show(G, SYNC_DURATION); // stop

    endwin();

    return 0;
}
