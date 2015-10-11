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

#define FRAME_PX VID_PX_W * VID_PX_H
#define FRAME_BYTES_TOT FRAME_PX * 4
#define FRAME_BYTES_RGB FRAME_PX * 3

enum {X, R, G, B}; // 0rgb format

int main(int argc, char *argv[])
{
    uint32_t frame = 0;

    uint32_t byte_count = 0;
    uint32_t byte_sum = 0;

    uint8_t curr;

    while (1) {
        curr = fgetc(stdin);

        switch (byte_count % 4) {
        case X:
            assert(curr == 255); // sync
            break;
        case R: // fall through
        case G: // fall through
        case B:
            byte_sum += curr;
            break;
        default:
            assert(0 && "not reached");
        }

        byte_count++;

        if (byte_count == FRAME_BYTES_TOT) {
            printf("frame: %d\tbrightness: %.2f%%\n",
                frame + 1, ((float)byte_sum / (FRAME_BYTES_RGB * 255)) * 100);

            frame++;

            byte_count = 0;
            byte_sum = 0;
        }
    }

    return 0;
}
