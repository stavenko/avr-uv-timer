#include <avr/pgmspace.h>
#include <stdlib.h>
#include "fonts.h"
#include "frame-buffer.h"

static uint8_t texture2[] = {
  0x81, 0x81,
  0x83, 0xc1,
  0x83, 0xc1,
  0x81, 0x81
};

static uint8_t *local_buffer = 0;

void get_default_glyph(struct bitmap *glyph) {
  glyph->width = 4;
  glyph->height = 2;
  glyph->buffer = texture2;
}

void get_glyph_from_font(uint16_t symbol, struct bitmap *glyph) {
  uint16_t index = 0;
  uint16_t symbols_amount = pgm_read_word(&SYMBOLS_AMOUNT);
  for (; index < symbols_amount; index++) {
    uint16_t item = pgm_read_word(&symbols_index[index]);
    if (item == symbol) {
      break;
    }
  }
  if (index == symbols_amount) {
    get_default_glyph(glyph);
    return;
  }

  uint16_t ptr = pgm_read_word(&pointer_index[index]);

  uint8_t width = pgm_read_byte(&data_array[ptr]);
  uint8_t height = pgm_read_byte(&data_array[ptr + 1]);

  if (local_buffer == 0) {
    local_buffer = malloc(5*16);
  }

  memcpy_P(local_buffer, &data_array[ptr + 2], width * height);
  
  glyph->width = width;
  glyph->height = height;
  glyph->buffer = local_buffer;
}

