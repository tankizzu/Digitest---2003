/* Touch controls (touch.c): polled by pad.c, drawn by the renderer (gl/touch.c) */
#ifndef TOUCH_H
#define TOUCH_H
#include <stdint.h>

enum { TOUCH_RING, TOUCH_KNOB, TOUCH_CROSS, TOUCH_CIRCLE, TOUCH_TRIANGLE, TOUCH_SQUARE, TOUCH_START, TOUCH_SELECT, TOUCH_L1, TOUCH_R1, TOUCH_PROGRESS, TOUCH_MENU, TOUCH_BACK, TOUCH_AUTO, TOUCH_LABEL, TOUCH_GEAR, TOUCH_BACK_ARROW };

typedef struct TouchShape {
    float x, y, r; /* the window's pixels, from the top left */
    int kind;
    float alpha;
    float value; /* TOUCH_PROGRESS: how much of the ring is filled, 0 to 1 */
} TouchShape;

int touch_enabled(void); /* on Android, or DW3_TOUCH=1 */
/* The buttons held and the left stick (2 bytes, 0x80 centred: left alone when the stick is off) for a window of w x h pixels */
void touch_poll(int w, int h, uint16_t *held, uint8_t *lstick);
int touch_shapes(TouchShape *out, int max);
void gl_touch_draw(int w, int h); /* gl/touch.c */
/* Where the game's picture is in the window (x, y, w, h, top left) and the PSX display it shows (gl/post.c) */
int touch_arrow(const uint32_t **pixels, int *w, int *h); /* the balloons' arrow read from VRAM (touch.c) */
void touch_set_picture(int x, int y, int w, int h, int disp_w, int disp_h);
void touch_script_tap(int px, int py); /* DW3_PRESS's "t<x>y<y>": a tap at the PSX display's pixel, for tests */
void native_touch_target(int id, int x, int y, int w, int h);
int native_touch_tap(void);
void native_touch_menu_available(void); /* the field: the menu button shows (and is Start) this moment */
void native_touch_in_menu(void);        /* a menu: the back button shows (and is Triangle) this moment */
int touch_arrow_plain(const uint32_t **pixels, int *w, int *h); /* the arrow without its speed dashes */
#endif
