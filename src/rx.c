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
#include <stdio.h>
#include <stdlib.h>

#include "types.h"

#define FRAME_W 320
#define FRAME_H 240

#define RGB_THRESH 64

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

    // printf("r: %d\tg: %d\tb: %d\t", avg.r, avg.g, avg.b);

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

int main(void)
{
    ulong frame = 0;

    FrameColor hist[4] = {FRAME_NULL};

    int f_started = 0;

    char c = 0;
    uint idx = 0;

    setbuf(stdout, NULL);

    while (1) {
        // printf("frame: %lu\t", frame);

        hist[0] = hist[1];
        hist[1] = hist[2];
        hist[2] = hist[3];
        hist[3] = frame_get_color();

        if (!f_started &&
            hist[0] == FRAME_GREEN &&
            hist[1] == FRAME_GREEN &&
            hist[2] == FRAME_GREEN &&
            hist[3] != FRAME_GREEN) {
                f_started = 1;
        }

        if (f_started &&
            hist[0] == FRAME_GREEN &&
            hist[1] == FRAME_GREEN &&
            hist[2] == FRAME_GREEN &&
            hist[3] == FRAME_GREEN) {
                break;
        }

        if (f_started && hist[3] != hist[2]) {
            if (hist[3] == FRAME_RED) {
                if (idx == 7) {
                    printf("%c", c);

                    c = 0;
                    idx = 0;
                }
                else {
                    idx++;
                }
            }
            else if (hist[3] == FRAME_BLUE) {
                c |= (0b10000000 >> idx);
                if (idx == 7) {
                    printf("%c", c);

                    c = 0;
                    idx = 0;
                }
                else {
                    idx++;
                }
            }
        }

        // frame++;
        // assert(frame > 0 && "overflow");
    }

    return 0;
}
