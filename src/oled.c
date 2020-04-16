#include "oled.h"
#include "i2c.h"

void oled_send_data(uint8_t address, uint8_t *data, uint8_t bytes) {
  i2c_start_with(address);
  i2c_write(0x40); // 
  for (int i = 0; i < bytes; i++) {
    i2c_write(data[i]);
  }
  
  i2c_stop();
}

void oled_send_command(uint8_t address, uint8_t *data, uint8_t bytes) {
  i2c_start_with(address);
  i2c_write(0x00); // 
  for (int i = 0; i < bytes; i++) {
    i2c_write(data[i]);
  }
  
  i2c_stop();

}

void oled_set_contrast(uint8_t address, uint8_t contrast) {
  uint8_t command[] = {0x81, contrast};
  oled_send_command(address, command, 2);
}

void oled_turn_on(uint8_t address) {
  uint8_t command[] = {0xae};
  oled_send_command(address, command, 1);
}

void oled_turn_sleep(uint8_t address) {
  uint8_t command[] = {0xaf};
  oled_send_command(address, command, 1);
}

void oled_set_horisontal_memory_mode(uint8_t address) {
  uint8_t command[] = {0x20, 0x00};
  oled_send_command(address, command, 2);
}

void oled_setup_page(uint8_t address, uint8_t page_start, uint8_t page_end, uint8_t col_start, uint8_t col_end) {
  uint8_t set_page[] = {0x22, page_start, page_end};
  oled_send_command(address, set_page, 3);
  uint8_t set_col[] = {0x22, col_start, col_end};
  oled_send_command(address, set_col, 3);
}

void oled_send_symbol(uint8_t address, struct bitmap *symbol, struct coords *to_coords) {
  uint8_t page_start = to_coords->top;
  uint8_t page_end = page_start + 1;
  uint8_t col_start = to_coords->left;
  uint8_t col_end = col_start + symbol->width;
  oled_setup_page(address, page_start, page_end, col_start, col_end);
  oled_send_data(address, symbol->buffer, symbol->height * symbol->width);
  

}

