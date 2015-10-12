/*
ffmpeg \
    -loglevel panic \
    -f avfoundation \
    -pixel_format 0rgb \
    -framerate 30 \
    -video_size 320x240 \
    -i "default:none" \
    -f rawvideo - | ./wink
*/

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VID_PX_W 320
#define VID_PX_H 240

#define FRAMES_PER_BIT 15

enum {X, R, G, B}; // 0rgb format

void draw(unsigned long *buf, const unsigned long frame)
{
    unsigned long sum = 0;

    for (int i = 0; i < FRAMES_PER_BIT; i++) {
        sum += buf[i];
    }

    float bright = (sum / (float)FRAMES_PER_BIT) /
        (VID_PX_W * VID_PX_H * 3 * 255);

    printf("bit frame: %lu\tbrightness: %.2f%%\n",
        frame / FRAMES_PER_BIT, bright * 100);
}

int main(int argc, char *argv[])
{
    unsigned long buf[FRAMES_PER_BIT] = {0};

    unsigned long vid_bytes = 0;
    unsigned long vid_frame = 0;

    unsigned char c;

    while (1) {
        c = fgetc(stdin);

        switch (vid_bytes % 4) {
        case X:
            assert(c == 255 && "not synced");
            break;
        case R: // fallthru
        case G: // fallthru
        case B:
            buf[vid_frame % FRAMES_PER_BIT] += c;
            break;
        default:
            assert(0 && "not reached");
        }

        vid_bytes++;
        assert(vid_bytes > 0 && "overflow");

        if (vid_bytes == VID_PX_W * VID_PX_H * 4) {
            vid_bytes = 0;

            vid_frame++;
            assert(vid_frame > 0 && "overflow");

            if (vid_frame % FRAMES_PER_BIT == 0) {
                draw(buf, vid_frame);
                memset(buf, 0, sizeof(buf));
            }
        }
    }

    return 0;
}
