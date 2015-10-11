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

enum {X, R, G, B}; // 0rgb

int main(int argc, char *argv[])
{
    uint32_t frame = 0;
    uint32_t bytes = 0; // per frame

    uint8_t curr;
    uint8_t r, g, b;

    while (1) {
        if (bytes == 0) {
            printf("frame: %d\n", frame + 1);
        }

        curr = fgetc(stdin);

        switch (bytes % 4) {
        case X:
            break; // nop
        case R:
            r = curr;
            break;
        case G:
            g = curr;
            break;
        case B:
            b = curr;
            break;
        default:
            assert(0 && "not reached");
        }

        bytes++;

        if (bytes == VID_PX_W * VID_PX_H * 4) { // bytes per pixel
            frame++;
            bytes = 0;
        }
    }

    return 0;
}
