#ifndef OLED_H
#define OLED_H
#include <avr/io.h>
#include <stdlib.h>
struct bitmap {
  uint8_t width;
  uint8_t height;
  uint8_t *buffer;
};
struct coords {
  uint8_t left;
  uint8_t top;
};

void oled_send_data(uint8_t address, uint8_t *data, uint8_t bytes);


void oled_send_command(uint8_t address, uint8_t *data, uint8_t bytes);

void oled_send_symbol(uint8_t address, struct bitmap *symbol, struct coords *to_coords);
  
  



#endif
