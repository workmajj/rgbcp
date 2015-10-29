#include <stdint.h>

typedef unsigned int uint;

typedef unsigned long ulong;

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} FramePixel;

typedef enum {
    FRAME_NULL,
    FRAME_RED,
    FRAME_BLUE,
    FRAME_GREEN
} FrameColor;
