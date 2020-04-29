#include "timer.h"
#include "constants.h"
#include "oled.h"
#include "get-glyph-from-font.h"
#include <stdlib.h>
#include <string.h>
const char colon = ':';
const char space = ' ';
char display_string[] = "000:00";

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


void get_minutes_and_seconds(const struct Timer * state, uint16_t *minutes_left, uint8_t* seconds_left) {
  if (state->state == TimerRunning) {
    uint8_t total_seconds = state->mseconds / 1000;
    uint8_t seconds_passed = total_seconds % 60;
    uint16_t minutes_passed = total_seconds / 60;

    *seconds_left = state->seconds - seconds_passed;
    *minutes_left = state->minutes - minutes_passed;
    return;
  }
  *seconds_left = state->seconds;
  *minutes_left = state->minutes;
}


void render_clock(
  const struct Timer  *state
    ) {
  uint8_t is_shown = (state->mseconds / 800) % 2;
  uint8_t is_minutes_edited = state->state == TimerSetup && state->editNow == Minutes;
  uint8_t is_seconds_edited = state->state == TimerSetup && state->editNow == Seconds;

  uint16_t minutes_left;
  uint8_t seconds_left;
  get_minutes_and_seconds(state, &minutes_left, &seconds_left);
  // char *minutes_label = malloc(4);
  // char *seconds_label = malloc(3);
  uint8_t char_len = 3+2+1+1; // minutes + seconds + : + \0
  // char *total_label = malloc(char_len); 
  // memset(display_string, 0, char_len);
  // memset(display_string, '0', char_len-1);
  

  // if(!(is_minutes_edited && is_shown)) {
     parse_value(display_string, minutes_left, 3, '0');
  // } else {
    // memset(minutes_label, ' ', 3);
  // }
  // if(!(is_minutes_edited && is_shown)) {
    parse_value(display_string + 4, seconds_left, 2, '0');
  // } else {
    // memset(minutes_label, ' ', 3);
  // }
  //
  // minutes_label[0] = '0';
  // minutes_label[1] = '0';
  // minutes_label[2] = '0';


  // memcpy(display_string, minutes_label, 3);

  // uint8_t is_colon_shown = state->state ==  TimerRunning ? is_shown: 1;

  // if (is_colon_shown) {
    memcpy(display_string+3, &colon, 1);
  // } else {
    // memcpy(display_string+3, &space, 1);
  // }
  // memcpy(display_string+4, seconds_label, 2);

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
