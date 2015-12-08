#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "types.h"

#define FRAME_W 320
#define FRAME_H 240

#define THRESH 64

FramePixel frame_get_avg(void)
{
    FramePixel avg;

    ulong sum_r = 0;
    ulong sum_g = 0;
    ulong sum_b = 0;

    ulong bytes = 0;

    int c;

    while (bytes < FRAME_W * FRAME_H * 4) { // 0rgb format
        c = getchar();

        switch (bytes % 4) {
        case 0:
            assert(c == 255 && "bytes misordered");
            break;
        case 1:
            sum_r += c;
            break;
        case 2:
            sum_g += c;
            break;
        case 3:
            sum_b += c;
            break;
        default:
            assert(0 && "not reached");
        }

        bytes++;
        assert(bytes > 0 && "overflow");
    }

    // TODO: round versus truncating
    avg.r = sum_r / (FRAME_W * FRAME_H);
    avg.g = sum_g / (FRAME_W * FRAME_H);
    avg.b = sum_b / (FRAME_W * FRAME_H);

    return avg;
}

FrameColor frame_get(void)
{
    FramePixel avg = frame_get_avg();

    if (avg.r < THRESH && avg.g < THRESH && avg.b < THRESH) {
        return FRAME_NULL;
    }

    if (avg.r > avg.b && avg.r > avg.g) {
        return FRAME_RED;
    }
    if (avg.g > avg.r && avg.g > avg.b) {
        return FRAME_GREEN;
    }
    if (avg.b > avg.r && avg.b > avg.g) {
        return FRAME_BLUE;
    }

    return FRAME_NULL;
}

int main(void)
{
    FrameColor buf[4] = {FRAME_NULL}; // most recent first

    bool f_wait = true;

    char out = 0;
    uint idx = 0;

    setbuf(stdout, NULL);

    while (true) {
        buf[3] = buf[2];
        buf[2] = buf[1];
        buf[1] = buf[0];
        buf[0] = frame_get();

        // start
        if (f_wait &&
            buf[3] == WAIT &&
            buf[2] == WAIT &&
            buf[1] == WAIT &&
            buf[0] != WAIT) {
                f_wait = false;
        }

        // stop
        if (!f_wait &&
            buf[3] == WAIT &&
            buf[2] == WAIT &&
            buf[1] == WAIT &&
            buf[0] == WAIT) {
                assert(out == 0 && idx == 0 && "last byte not flushed");
                break;
        }

        if (!f_wait && buf[0] != buf[1] && (buf[0] == ZERO || buf[0] == ONE)) {
            if (buf[0] == ONE) {
                out |= (0b10000000 >> idx); // flip initial 0 to 1
            }

            if (idx == 7) {
                putchar(out);

                out = 0;
                idx = 0;
            }
            else {
                idx++;
            }
        }
    }

    return 0;
}
