#include "timer.h"
#include "constants.h"
#include "oled.h"
#include "get-glyph-from-font.h"
#include <stdlib.h>
#include <string.h>
const char colon = ':';
const char space = ' ';

void get_minutes_and_seconds(const struct Timer * state, uint16_t *minutes_left, uint8_t* seconds_left) {
  uint8_t total_seconds = state->mseconds / 1000;
  uint8_t seconds_passed = total_seconds % 60;
  uint16_t minutes_passed = total_seconds / 60;

  *seconds_left = state->seconds - seconds_passed;
  *minutes_left = state->minutes - minutes_passed;
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
  char *minutes_label = malloc(4);
  char *seconds_label = malloc(3);

  if(!(is_minutes_edited && is_shown)) {
    utoa(minutes_left, minutes_label, 10);
  } else {
    memset(minutes_label, ' ', 3);
  }
  if(!(is_minutes_edited && is_shown)) {
    utoa(seconds_left, seconds_label, 10);
  } else {
    memset(minutes_label, ' ', 3);
  }
  char *total_label = malloc(3+2+1+1); // minutes + seconds + : + \0


  memcpy(total_label, minutes_label, 3);

  uint8_t is_colon_shown = state->state ==  TimerRunning ? is_shown: 1;

  if (is_colon_shown) {
    memcpy(total_label+3, &colon, 1);
  } else {
    memcpy(total_label+3, &space, 1);
  }
  memcpy(total_label+4, seconds_label, 2);
  total_label[5] = '\0';

  fb_render_text(total_label, &clock_timer_coords, OR);
  free(minutes_label);
  free(seconds_label);
  free(total_label);

}

void calculate_width_height(const char *str, uint8_t *width, uint8_t *height) {
  struct bitmap *glyph = malloc(sizeof(struct bitmap));
  uint8_t ptr = 0;
  while (str[ptr++]) {
    get_glyph_from_font(*str, glyph);
    *width += glyph->width;
    *height = *height > glyph->height ? *height : glyph->height;
  }
  *width += ptr - 1;
}


/*
void render_clock_icon(const struct Timer  *state) {
  fb_set_bitmap(&clockImageTexture, 0, 0, state->menuPosition == RunTimer,  OR);
}
void render_run_icon(const struct Timer  *state) {
  fb_set_bitmap(&arrowImageTexture, 0, arrowImageTexture.width, state->menuPosition == SetupTimer,  OR);
}
*/

void render_ui(const struct Timer  *state) {
  fb_clear();
  render_clock(state);
  // render_clock_icon(state);
  // render_run_icon(state);
  oled_send_framebuffer();
}
