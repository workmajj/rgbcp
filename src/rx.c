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
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define FRAME_PX_W 320
#define FRAME_PX_H 240

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} Pixel;

Pixel frame_get_avg()
{
    Pixel avg;

    unsigned long sum_r = 0;
    unsigned long sum_g = 0;
    unsigned long sum_b = 0;

    unsigned long bytes = 0;
    unsigned char c;

    while (bytes < FRAME_PX_W * FRAME_PX_H * 4) {
        c = fgetc(stdin);

        switch (bytes % 4) { // 0rgb format
        case 0:
            assert(c == 255 && "video bytes misordered");
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

    // FIXME: round instead of truncating
    avg.r = sum_r / (FRAME_PX_W * FRAME_PX_H);
    avg.g = sum_g / (FRAME_PX_W * FRAME_PX_H);
    avg.b = sum_b / (FRAME_PX_W * FRAME_PX_H);

    return avg;
}

int main()
{
    unsigned long frame = 0;

    Pixel avg;

    while (1) {
        avg = frame_get_avg();

        printf("frame: %lu\tr: %d\tg: %d\tb: %d\n", frame + 1,
            avg.r, avg.g, avg.b);

        frame++;
        assert(frame > 0 && "overflow");
    }

    return 0;
}
