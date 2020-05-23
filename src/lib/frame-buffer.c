#include <avr/io.h>
#include <stdlib.h>
#include "frame-buffer.h"
#include "constants.h"
#include "get-glyph-from-font.h"

uint32_t *framebuffer;
void fb_init() {
  framebuffer = malloc(128 * 4);
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


void fb_set_bitmap(const struct bitmap *image, uint16_t left, uint16_t top, enum OPERATION op) {
  uint16_t startingColumn = left;
  uint8_t startingBit = top;
  for (int i = 0; i < image->width; i++ ) {
    uint32_t row = 0;
    int height = image->height - 1;
    for (int j = 0; j < image->height; j++) {
      uint32_t value = image->buffer[j + i * image->height];
      uint8_t shift = 8 * (height -j);
      row |= (value << shift);
    }
    row = row << startingBit;
    uint32_t reversedRow = 0;
    for (int i = 0; i < 32; i ++) {
      reversedRow |= ((row >> i) & 1) << (31 - i);
    }
    switch (op) {
      case OVER:  
        framebuffer[startingColumn + i] = reversedRow;
        break;;
      case AND:
        framebuffer[startingColumn +i] &= reversedRow;
        break;
      case OR:
        framebuffer[startingColumn +i] |= reversedRow;
        break;
      }

  }
}

static struct bitmap *glyph = 0;
void fb_render_text(char* bytes, struct coords * to_coords, enum OPERATION op) {
  uint16_t shift = 0;
  if (glyph == 0) {
    glyph = malloc(sizeof(struct bitmap));
    glyph->width = 0;
    glyph->height = 0;
    glyph->buffer = 0;
  }
  char *ptr = bytes;
  while (*ptr != 0) {
    if (*ptr == 0x20) {
      shift += space_in_sentence + space_in_word;
      ptr++;
      continue;
    }
    get_glyph_from_font(*ptr, glyph);
    if (glyph->buffer != 0) {
      uint8_t small_glyph = glyph->width < space_in_sentence;
      uint8_t glyph_margins = small_glyph ? space_in_sentence - glyph->width: 0;
      uint8_t left_margin = glyph_margins == 1 ? 1 : glyph_margins / 2;
      uint8_t right_margin = glyph_margins - left_margin;
      shift += left_margin;
      fb_set_bitmap(glyph, to_coords->left + shift, to_coords->top, OR);
      shift += right_margin;
      shift += glyph->width;
      shift += space_in_word;
    };
    ptr++;
  }
}
