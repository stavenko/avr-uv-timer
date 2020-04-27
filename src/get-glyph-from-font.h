#ifndef GET_GLYPH_FROM_FONT
#define GET_GLYPH_FROM_FONT
#include <avr/io.h>
#include "frame-buffer.h"
void get_glyph_from_font(uint16_t symbol, struct bitmap *bm);
#endif

