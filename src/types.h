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
