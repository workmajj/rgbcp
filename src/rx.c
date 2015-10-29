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

#define FRAME_W 320
#define FRAME_H 240

#define RGB_THRESH 64

typedef unsigned long ulong;

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} FramePixel;

typedef enum {X, R, G, B} FrameColor; // FIXME: shared

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
        return X;
    }

    if (avg.r > avg.b && avg.r > avg.g) {
        return R;
    }
    else if (avg.g > avg.r && avg.g > avg.b) {
        return G;
    }
    else if (avg.b > avg.r && avg.b > avg.g) {
        return B;
    }

    return X;
}

int main(void)
{
    ulong frame = 0;

    FrameColor curr = X;
    FrameColor last = X;

    setbuf(stdout, NULL);

    while (1) {
        // printf("frame: %lu\t", frame);

        curr = frame_get_color();

        if (curr != last && curr == R) {
            printf("0");
        }
        else if (curr != last && curr == B) {
            printf("1");
        }

        // printf("\n");

        last = curr;

        frame++;
        assert(frame > 0 && "overflow");
    }

    return 0;
}
