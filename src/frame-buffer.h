#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#include <avr/io.h>
struct bitmap {
  uint8_t width; // how many columns this bitmap contains;
  uint8_t height; // how many bytes per column value could be 1 - 4
  const uint8_t * buffer;
};
struct coords {
  uint16_t left;
  uint16_t top;
};

enum OPERATION {
  AND, OR, OVER
};
void fb_init();
void fb_free();
void fb_clear();
void fb_set_bitmap(const struct bitmap *image, uint16_t left, uint16_t top, uint8_t inversed, enum OPERATION);
void fb_render_text(char* bytes, struct coords * to_coords, enum OPERATION op);

uint8_t *fb_ptr();
uint16_t fb_bytes();

#endif
