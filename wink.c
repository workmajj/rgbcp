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
#include <stdio.h>
#include <stdlib.h>

#define VID_PX_W 320
#define VID_PX_H 240

enum {X, R, G, B}; // 0rgb format

int main(int argc, char *argv[])
{
    uint32_t frame = 0;
    uint32_t bytes = 0; // per frame

    uint8_t curr;

    uint32_t sum = 0;

    while (1) {
        curr = fgetc(stdin);

        switch (bytes % 4) {
        case X:
            assert(curr == 255); // sync
            break;
        case R:
        case G:
        case B:
            sum += curr;
            break;
        default:
            assert(0 && "not reached");
        }

        bytes++;

        if (bytes == VID_PX_W * VID_PX_H * 4) {
            printf("frame: %d, bytes: %d, sum: %d\n", frame + 1, bytes, sum);

            frame++;

            bytes = 0;
            sum = 0;
        }
    }

    return 0;
}
