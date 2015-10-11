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

    uint32_t byte_count = 0;
    uint32_t byte_sum = 0;

    uint8_t curr;

    while (1) {
        curr = fgetc(stdin);

        switch (byte_count % 4) {
        case X:
            assert(curr == 255); // sync
            break;
        case R:
        case G:
        case B:
            byte_sum += curr;
            break;
        default:
            assert(0 && "not reached");
        }

        byte_count++;

        if (byte_count == VID_PX_W * VID_PX_H * 4) {
            printf("frame: %d\tbyte_count: %d\tbyte_sum: %d\n",
                frame + 1, byte_count, byte_sum);

            frame++;

            byte_count = 0;
            byte_sum = 0;
        }
    }

    return 0;
}
