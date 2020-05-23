#include "oled.h"
#include "i2c.h"
#include "constants.h"
#include "get-glyph-from-font.h"
#include <string.h>

void oled_send_zero_data(uint16_t times) {
  i2c_start_with(OLED_I2C_SLAVE_ADDR);
  i2c_write(0x40); // 
  for (int i = 0; i < times; i++) {
    i2c_write(0x00);
  }
  
  i2c_stop();
}
void oled_send_data(uint8_t *data, uint16_t bytes) {
  i2c_start_with(OLED_I2C_SLAVE_ADDR);
  i2c_write(0x40); // 
  for (int i = 0; i < bytes; i++) {
    i2c_write(data[i]);
  }
  
  i2c_stop();
}

void oled_send_command(uint8_t *data, uint8_t bytes) {
  i2c_start_with(OLED_I2C_SLAVE_ADDR);
  i2c_write(0x00); // 
  for (int i = 0; i < bytes; i++) {
    i2c_write(data[i]);
  }
  
  i2c_stop();

}

void oled_set_contrast(uint8_t contrast) {
  uint8_t command[] = {0x81, contrast};
  oled_send_command(command, 2);
}

void oled_turn_on() {
  uint8_t command[] = {0xaf};
  oled_send_command(command, 1);
}

void oled_ignore_ram(uint8_t ignore) {
  uint8_t command[] = {ignore ? 0xa5 : 0xa4};
  oled_send_command(command, 1);
}

void oled_turn_sleep() {
  uint8_t command[] = {0xae};
  oled_send_command(command, 1);
}

void oled_set_horisontal_memory_mode() {
  uint8_t command[] = {0x20, 0x00};
  oled_send_command(command, 2);
}

void oled_setup_page(uint8_t page_start, uint8_t page_end, uint8_t col_start, uint8_t col_end) {
  uint8_t set_page[] = {0x22, page_start, page_end};
  oled_send_command(set_page, 3);
  uint8_t set_col[] = {0x21, col_start, col_end};
  oled_send_command(set_col, 3);
}
const uint8_t init_sequence [] = {    // Initialization Sequence
    0x20, 0b01,      // Set Memory Addressing Mode
    // 00=Horizontal Addressing Mode; 01=Vertical Addressing Mode;
    // 10=Page Addressing Mode (RESET); 11=Invalid
    0xB0,            // Set Page Start Address for Page Addressing Mode, 0-7
    0xC8,            // Set COM Output Scan Direction
    0x00,            // --set low column address
    0x10,            // --set high column address
    0x40,            // --set start line address
    0x81, 0x3F,      // Set contrast control register
    0xA1,            // Set Segment Re-map. A0=address mapped; A1=address 127 mapped.
    0xA6,            // Set display mode. A6=Normal; A7=Inverse
    0xA8, 32-1, // Set multiplex ratio(1 to 64)
    0xA4,            // Output RAM to Display
					 // 0xA4=Output follows RAM content; 0xA5,Output ignores RAM content
    0xD3, 0x00,      // Set display offset. 00 = no offset
    0xD5,            // --set display clock divide ratio/oscillator frequency
    0xF0,            // --set divide ratio
    0xD9, 0x22,      // Set pre-charge period
		     // Set com pins hardware configuration
    0xDA, 0x02,
    0xDB,            // --set vcomh
    0x20,            // 0x20,0.77xVcc
    0x8D, 0x14,      // Set DC-DC enable
};
void oled_init(){
  // uint8_t command[] = {0xda, 0x02};
  uint8_t size = sizeof(init_sequence);
  oled_send_command((uint8_t*)init_sequence, size);
  fb_init();
}

void oled_send_symbol(struct bitmap *symbol, struct coords *to_coords) {
  fb_set_bitmap(symbol, to_coords->left, to_coords->top, OVER);
  oled_setup_page(0, 3, 0, 127);
  oled_send_data(fb_ptr(), fb_bytes());
}

void oled_send_framebuffer() {
  oled_setup_page(0, 3, 0, 127);
  oled_send_data(fb_ptr(), fb_bytes());
}

void oled_render_text(char *bytes, struct coords *to_coords) {
  fb_render_text(bytes, to_coords, OR);
  oled_setup_page(0, 3, 0, 127);
  oled_send_data(fb_ptr(), fb_bytes());
}

void oled_render_text_w(wchar_t *bytes, uint16_t num_bytes, struct coords *to_coords) {
  uint16_t shift = 0;
  struct bitmap *glyph = malloc(sizeof(struct bitmap));
  memset(glyph, 0, sizeof(struct bitmap));
  wchar_t *ptr = bytes;
  while (*ptr != 0) {
    if (*ptr == 0x20) {
      shift += 2;
      ptr++;
      continue;
    }
    get_glyph_from_font(*ptr, glyph);
    if (glyph->buffer != 0) {
      fb_set_bitmap(glyph, to_coords->left + shift, to_coords->top, OR);
      shift += glyph->width + 2;
    };
    ptr++;
  }
  oled_setup_page(0, 3, 0, 127);
  oled_send_data(fb_ptr(), fb_bytes());
}

void oled_clear_screen() {
  uint8_t page_start = 0;
  uint8_t page_end = 3;
  uint8_t col_start = 0;
  uint8_t col_end = 127;
  oled_setup_page(page_start, page_end, col_start, col_end);
  oled_send_zero_data(4 * 128);
}

