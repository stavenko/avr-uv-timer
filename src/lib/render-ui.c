#include <avr/pgmspace.h>
#include "timer.h"
#include "constants.h"
#include "oled.h"
#include "i2c.h"
#include "get-glyph-from-font.h"
#include <stdlib.h>
#include <string.h>
const char colon = ':';
const char space = ' ';
char display_string[] = "000:00";
const char errorLabl[] PROGMEM = "er";

char map_symbol(uint8_t val) {
  return val + 0x30;
}

void parse_value(char *buffer, uint16_t value, int len, char def) {
  char *ptr = buffer + len - 1;
  while(1) {
    uint16_t div = value / 10;
    uint8_t rem = value % 10;
    if (div == 0 && rem == 0) {
      *ptr = def;
    } else {
      *ptr = map_symbol(rem);
    }
    --ptr;
    value = div;
    if (--len < 0) break;
  }
}

void render_clock(const struct Timer  *state) {
  uint8_t is_minutes_edited = (state->state == TimerSetup) && (state->editNow == Minutes);
  uint8_t is_seconds_edited = (state->state == TimerSetup) && (state->editNow == Seconds);
  uint8_t is_blink = state->is_blink;

  uint16_t minutes_left;
  uint8_t seconds_left;
  get_minutes_and_seconds(state, &minutes_left, &seconds_left);
  // if ((seconds_left > 60) && (minutes_left < 1000)) {
    // parse_value(display_string, minutes_left, 3, '0');
    // memcpy_P(display_string + 4, errorLabl, 2);
    // memcpy(display_string+3, &colon, 1);
  // } else {
    if (is_minutes_edited && !is_blink) {
      memset(display_string, ' ', 3);
    } else {
      parse_value(display_string, minutes_left, 3, '0');
    }
    if (state->state == TimerSetup && state->editNow == Seconds && !is_blink) {
      memset(display_string+4, ' ', 2);
    } else {
      parse_value(display_string + 4, seconds_left, 2, '0');
    }
    if (state->state == TimerRunning && !is_blink) {
      memset(display_string+3, ' ', 1);
    } else {
      memcpy(display_string+3, &colon, 1);
    }
  // }

  fb_render_text(display_string, &clock_timer_coords, OR);

}

void calculate_width_height(const char *str, uint8_t *width, uint8_t *height) {
  struct bitmap *glyph = malloc(sizeof(struct bitmap));
  memset(glyph, 0, sizeof(struct bitmap));
  uint8_t ptr = 0;
  while (str[ptr++]) {
    get_glyph_from_font(*str, glyph);
    *width += glyph->width;
    *height = *height > glyph->height ? *height : glyph->height;
  }
  *width += ptr - 1;
}


void render_clock_icon(const struct Timer  *state) {
  if (state->menuPosition == SetupTimer) {
    fb_set_bitmap(&frameTexture, 0, 20, OR);
  }
  fb_set_bitmap(&clockImageTexture, 2, 22, OR);
}
void render_run_icon(const struct Timer  *state) {
  if (state->menuPosition == RunTimer) {
    fb_set_bitmap(&frameTexture, 0, 9, OR);
  }
  fb_set_bitmap(&arrowImageTexture, 1, 11, OR);
}

void render_ui(const struct Timer  *state) {
  fb_clear();
  render_clock(state);
  render_clock_icon(state);
  render_run_icon(state);
  oled_send_framebuffer();
}
