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

    while (bytes < FRAME_PX_W * FRAME_PX_H * 4) { // 0rgb format
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

    // FIXME: round instead of truncating
    avg.r = sum_r / (FRAME_PX_W * FRAME_PX_H);
    avg.g = sum_g / (FRAME_PX_W * FRAME_PX_H);
    avg.b = sum_b / (FRAME_PX_W * FRAME_PX_H);

    return avg;
}

typedef enum {X, R, G, B} FrameColor;

int main()
{
    unsigned long frame = 0;

    /*
    Pixel curr;
    Pixel last;

    while (1) {
        curr = frame_get_avg();

        printf("frame: %lu\tr: %d\tg: %d\tb: %d\n", frame + 1,
            avg.r, avg.g, avg.b);

        frame++;
        assert(frame > 0 && "overflow");

        last = curr;
    }
    */

    Pixel avg;

    FrameColor curr;
    FrameColor last[3];

    /*
    R R R => ?
    B B B => ?

    R R B => B <=
    B B R => R <=

    B R R => ?
    R B B => ?

    R B R => ?
    B R B => ?
    */

    while (1) {
        avg = frame_get_avg();

        printf("frame: %lu\tr: %d\tg: %d\tb: %d\t", frame + 1,
            avg.r, avg.g, avg.b);

        /*
        if (avg.r < 255 * 0.25 && avg.g < 255 * 0.25 && avg.b < 255 * 0.25) {
            printf("\t--\t--\n");
            curr = X;
        }
        else if (avg.r > 255 * 0.75 && avg.b < 255 * 0.25) {
            curr = R;
            printf("\tR\t1\n");
        }
        else if (avg.b > 255 * 0.75 && avg.r < 255 * 0.25) {
            curr = B;
            printf("\tB\t0\n");
        }
        else if (avg.r > avg.b) {
            if (last[2] == R && last[1] == B && last[0] == B) {
                curr = R;
                printf("\tR\t1\n");
            }
            else {
                curr = X;
                printf("\tR?\t1?\n");
            }
        }
        else if (avg.b > avg.r) {
            if (last[2] == B && last[1] == R && last[0] == R) {
                curr = B;
                printf("\tB\t0\n");
            }
            else {
                curr = X;
                printf("\tB?\t0?\n");
            }
        }
        else {
            printf("\t??\t??\n");
            curr = X;
        }
        */

        if (avg.r < 255 * 0.25 && avg.g < 255 * 0.25 && avg.b < 255 * 0.25) {
            printf("\t-\n");
            curr = X;
        }
        else if (avg.r > 255 * 0.75 && avg.g < 255 * 0.25 && avg.b < 255 * 0.25) {
            curr = R;
            printf("\tR\t0\n");
        }
        else if (avg.r < 255 * 0.25 && avg.g > 255 * 0.75 && avg.b < 255 * 0.25) {
            curr = R;
            printf("\tG\n");
        }
        else if (avg.r < 255 * 0.25 && avg.g < 255 * 0.25 && avg.b > 255 * 0.75) {
            curr = R;
            printf("\tB\t1\n");
        }
        else if (avg.r > avg.b && avg.r > avg.g) {
            if (last[2] == R && last[1] != R && last[0] != R) {
                curr = R;
                printf("\tR\t0\n");
            }
            else {
                curr = X;
                printf("\tR?\t0\n");
            }
        }
        else if (avg.g > avg.r && avg.g > avg.b) {
            if (last[2] == G && last[1] != G && last[0] != G) {
                curr = G;
                printf("\tG\n");
            }
            else {
                curr = X;
                printf("\tG?\n");
            }
        }
        else if (avg.b > avg.r && avg.b > avg.g) {
            if (last[2] == B && last[1] != B && last[0] != B) {
                curr = B;
                printf("\tB\t1\n");
            }
            else {
                curr = X;
                printf("\tB?\t1\n");
            }
        }
        else {
            printf("\t??\n");
            curr = X;
        }

        last[0] = last[1];
        last[1] = last[2];
        last[2] = curr;

        frame++;
        assert(frame > 0 && "overflow");
    }

    return 0;
}
