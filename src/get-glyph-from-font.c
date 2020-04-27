#include <stdlib.h>
#include "fonts.h"
#include "frame-buffer.h"

static uint8_t texture2[] = {
  0x81, 0x81,
  0x83, 0xc1,
  0x83, 0xc1,
  0x81, 0x81
};

void get_default_glyph(struct bitmap *glyph) {
  glyph->width = 4;
  glyph->height = 2;
  glyph->buffer = texture2;
}

void get_glyph_from_font(uint16_t symbol, struct bitmap *glyph) {
  uint16_t index = 0;
  for (; index < SYMBOLS_AMOUNT; index++) {
    if (symbols_index[index] == symbol) {
      break;
    }
  }
  if (index == SYMBOLS_AMOUNT) {
    get_default_glyph(glyph);
    return;
  }

  uint32_t ptr = pointer_index[index];

  uint8_t width = data_array[ptr];
  uint8_t height = data_array[ptr + 1];
  glyph->width = width;
  glyph->height = height;
  glyph->buffer = (data_array + ptr + 2);
}

