typedef unsigned int uint;

typedef unsigned long ulong;

typedef struct FramePixel {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} FramePixel;

typedef enum FrameColor {
    FRAME_NULL,
    FRAME_RED,
    FRAME_GREEN,
    FRAME_BLUE
} FrameColor;

#define WAIT FRAME_GREEN
#define ZERO FRAME_RED
#define ONE FRAME_BLUE
