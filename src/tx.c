#include <assert.h>
#include <unistd.h>

#include "curses.h"

#define SYNC_MS 1000

typedef enum {X, R, G, B} FrameColor; // FIXME: shared

unsigned int screen_rows;
unsigned int screen_cols;

unsigned int step_gen(void)
{
    const size_t STEP_SIZE = 3;
    const unsigned int STEP[] = {66, 66, 67}; // 15 fps

    static unsigned int idx = 0;
    unsigned int r = idx;

    idx = (idx == STEP_SIZE - 1) ? 0 : idx + 1;

    return STEP[r];
}

void frame_show(const FrameColor c, const unsigned int sleep_ms)
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

    for (int i = 0; i < screen_rows; i++) {
        for (int j = 0; j < screen_cols; j++) {
            move(i, j);
            printw(" ");
        }
    }

    refresh();

    usleep(sleep_ms * 1000); // millis to micros
}

int main(void)
{
    initscr();
    getmaxyx(stdscr, screen_rows, screen_cols); // globals

    // FIXME: has_colors()

    start_color();
    init_pair(1, COLOR_RED, COLOR_RED);
    init_pair(2, COLOR_GREEN, COLOR_GREEN);
    init_pair(3, COLOR_BLUE, COLOR_BLUE);

    frame_show(G, SYNC_MS); // start

    int c;

    while ((c = getchar()) != EOF) {
        for (int i = 0; i < 8; i++) {
            if (c & (0b10000000 >> i)) {
                frame_show(B, step_gen()); // 1
            }
            else {
                frame_show(R, step_gen()); // 0
            }

            frame_show(G, step_gen());
        }
    }

    frame_show(G, SYNC_MS); // stop

    endwin(); // FIXME: clean up on SIGINT

    return 0;
}
