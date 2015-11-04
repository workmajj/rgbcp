/*
ffmpeg \
    -loglevel panic \
    -f avfoundation \
    -pixel_format 0rgb \
    -framerate 30 \
    -video_size 320x240 \
    -i "default:none" \
    -f rawvideo - | ./rx
*/

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "types.h"

#define FRAME_W 320
#define FRAME_H 240

#define RGB_THRESH 64

#define HIST_SIZE 4

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

    // FIXME: round versus truncating
    avg.r = sum_r / (FRAME_W * FRAME_H);
    avg.g = sum_g / (FRAME_W * FRAME_H);
    avg.b = sum_b / (FRAME_W * FRAME_H);

    return avg;
}

FrameColor frame_get_color(void)
{
    FramePixel avg = frame_get_avg();

    if (avg.r < RGB_THRESH && avg.g < RGB_THRESH && avg.b < RGB_THRESH) {
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

void hist_step(FrameColor hist[])
{
    for (uint i = HIST_SIZE - 1; i > 0; i--) {
        hist[i] = hist[i - 1];
    }
}

bool hist_is_green_except_curr(FrameColor hist[])
{
    for (uint i = 1; i < HIST_SIZE; i++) {
        if (hist[i] != FRAME_GREEN) {
            return false;
        }
    }

    return (hist[0] != FRAME_GREEN);
}

bool hist_is_green(FrameColor hist[])
{
    for (uint i = 0; i < HIST_SIZE; i++) {
        if (hist[i] != FRAME_GREEN) {
            return false;
        }
    }

    return true;
}

int main(void)
{
    assert(HIST_SIZE > 1);

    FrameColor hist[HIST_SIZE] = {FRAME_NULL};

    bool streaming = false;

    char buf = 0;
    uint idx = 0;

    setbuf(stdout, NULL);

    while (1) {
        hist_step(hist);

        hist[0] = frame_get_color();

        if (!streaming && hist_is_green_except_curr(hist)) {
            streaming = true;
        }
        if (streaming && hist_is_green(hist)) {
            break;
        }

        if (streaming && hist[0] != hist[1]) {
            if (hist[0] == FRAME_RED) {
                if (idx == 7) {
                    printf("%c", buf);

                    buf = 0;
                    idx = 0;
                }
                else {
                    idx++;
                }
            }
            else if (hist[0] == FRAME_BLUE) {
                buf |= (0b10000000 >> idx);

                if (idx == 7) {
                    printf("%c", buf);

                    buf = 0;
                    idx = 0;
                }
                else {
                    idx++;
                }
            }
        }
    }

    return 0;
}
