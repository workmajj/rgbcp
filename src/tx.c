#include <assert.h>
#include <stdlib.h>
#include <unistd.h>

#include "curses.h"

#include "types.h"

#define SYNC_MS 1000

uint term_rows, term_cols;

uint frame_step(void)
{
    const size_t SIZE = 3;
    const uint DURATION_MS[] = {66, 66, 67}; // 15 fps

    static uint idx = 0;

    uint ms = DURATION_MS[idx];
    idx = (idx == SIZE - 1) ? 0 : idx + 1;

    return ms;
}

void frame_show(const FrameColor c, const uint duration_ms)
{
    switch (c) {
    case FRAME_RED:
        attrset(COLOR_PAIR(1));
        break;
    case FRAME_GREEN:
        attrset(COLOR_PAIR(2));
        break;
    case FRAME_BLUE:
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

    usleep(duration_ms * 1000); // to micros
}

int main(void)
{
    initscr();

    if (!has_colors()) {
        fprintf(stderr, "terminal doesn't support color\n");
        exit(1);
    }

    getmaxyx(stdscr, term_rows, term_cols);

    start_color();
    init_pair(1, COLOR_RED, COLOR_RED);
    init_pair(2, COLOR_GREEN, COLOR_GREEN);
    init_pair(3, COLOR_BLUE, COLOR_BLUE);

    int c;

    frame_show(WAIT, SYNC_MS); // start

    while ((c = getchar()) != EOF) {
        for (uint i = 0; i < 8; i++) {
            if (c & (0b10000000 >> i)) {
                frame_show(ONE, frame_step());
            }
            else {
                frame_show(ZERO, frame_step());
            }

            frame_show(WAIT, frame_step());
        }
    }

    frame_show(WAIT, SYNC_MS); // stop

    endwin();

    return 0;
}
