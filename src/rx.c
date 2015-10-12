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
#include <string.h>

#define VID_PX_W 320
#define VID_PX_H 240

enum {X, R, G, B}; // 0rgb format

int main(int argc, char *argv[])
{
    unsigned long sum = 0;

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
            sum += c;
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

            printf("frame: %lu\tbrightness: %.2f%%\n", vid_frame,
                ((float)sum / (VID_PX_W * VID_PX_H * 3 * 255)) * 100);

            sum = 0;
        }
    }

    return 0;
}
