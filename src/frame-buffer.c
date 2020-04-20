#include <avr/io.h>
#include <stdlib.h>
#include "frame-buffer.h"

uint32_t *framebuffer;
void fb_init() {
  framebuffer = malloc(128 * sizeof(uint32_t));
  fb_clear();
}

void fb_clear() {
  for (int i = 0; i < 128; i++) {
    framebuffer[i] = 0;
  }
}
void fb_free() {
  free(framebuffer);
}
uint16_t fb_bytes() {
  return 128 * 4;
}

uint8_t *fb_ptr() {
  return (uint8_t*) framebuffer;
}


void fb_set_bitmap(struct bitmap *image, struct coords *offset, enum OPERATION op) {

  uint8_t startingColumn = offset->left;
  uint8_t startingBit = offset->top;
  for (int i = 0; i < image->width; i++ ) {
    uint32_t row = 0;
    int height = image->height - 1;
    for (int j = 0; j < image->height; j++) {
      uint32_t value = image->buffer[j + i * image->height];
      uint8_t shift = 8 * (height -j);
      row |= (value << shift);
    }
    row = row << startingBit;
    switch (op) {
      case OVER:  
        framebuffer[startingColumn + i] = row;
        break;;
      case AND:
        framebuffer[startingColumn +i] &= row;
        break;
      case OR:
        framebuffer[startingColumn +i] |= row;
        break;
      }

  }
}

