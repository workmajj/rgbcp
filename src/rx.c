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

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} FramePixel;

typedef enum {X, R, G, B} FrameColor;

FramePixel frame_get_avg()
{
    FramePixel avg;

    unsigned long sum_r = 0;
    unsigned long sum_g = 0;
    unsigned long sum_b = 0;

    unsigned long bytes = 0;
    unsigned char c;

    while (bytes < FRAME_W * FRAME_H * 4) { // 0rgb format
        c = fgetc(stdin);

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

FrameColor frame_get(FrameColor prev[])
{
    assert(prev != NULL);

    FrameColor curr;
    FrameColor note;

    FramePixel avg = frame_get_avg();

    printf("\tr: %d\tg: %d\tb: %d", avg.r, avg.g, avg.b);

    if (avg.r < 256 * 0.25 && avg.g < 256 * 0.25 && avg.b < 256 * 0.25) {
        curr = X;
        note = X;
    }
    else if (avg.r > avg.b && avg.r > avg.g) {
        if (prev[2] == R && prev[1] == G && prev[0] == G) {
            curr = R;
            note = R;
        }
        else {
            curr = R;
            note = X;
        }
    }
    else if (avg.g > avg.r && avg.g > avg.b) {
        if (prev[2] == G &&
            (prev[1] == R || prev[1] == B) &&
            (prev[0] == R || prev[0] == B)) {
                curr = G;
                note = G;
        }
        else {
            curr = G;
            note = X;
        }
    }
    else if (avg.b > avg.r && avg.b > avg.g) {
        if (prev[2] == B && prev[1] == G && prev[0] == G) {
            curr = B;
            note = B;
        }
        else {
            curr = B;
            note = X;
        }
    }
    else {
        curr = X;
        note = X;
    }

    prev[0] = prev[1];
    prev[1] = prev[2];
    prev[2] = note;

    return curr;
}

int main()
{
    unsigned long frame = 0;

    FrameColor prev[3];

    while (1) {
        printf("frame: %lu", frame + 1);

        switch (frame_get(prev)) {
        case X:
            printf("\t-\n");
            break;
        case R:
            printf("\tR\t0\n");
            break;
        case G:
            printf("\tG\n");
            break;
        case B:
            printf("\tB\t1\n");
            break;
        default:
            assert(0 && "not reached");
        }

        frame++;
        assert(frame > 0 && "overflow");
    }

    return 0;
}
