/* Touch controls (touch.c): polled by pad.c, drawn by the renderer (gl/touch.c) */
#ifndef TOUCH_H
#define TOUCH_H
#include <stdint.h>

enum { TOUCH_RING, TOUCH_KNOB, TOUCH_CROSS, TOUCH_CIRCLE, TOUCH_TRIANGLE, TOUCH_SQUARE, TOUCH_START, TOUCH_SELECT, TOUCH_L1, TOUCH_R1 };

typedef struct TouchShape {
    float x, y, r; /* the window's pixels, from the top left */
    int kind;
    float alpha;
} TouchShape;

int touch_enabled(void); /* on Android, or DW3_TOUCH=1 */
/* The buttons held and the left stick (2 bytes, 0x80 centred: left alone when the stick is off) for a window of w x h pixels */
void touch_poll(int w, int h, uint16_t *held, uint8_t *lstick);
int touch_shapes(TouchShape *out, int max);
void gl_touch_draw(int w, int h); /* gl/touch.c */
#endif
