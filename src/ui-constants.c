#include "frame-buffer.h"
struct coords clock_minutes_coords = {
  15, 
  10
};
struct coords clock_timer_coords = {
  15, 
  10
};
uint8_t space_in_word = 2;
uint8_t space_in_sentence = 0x0e;

const struct bitmap clockImageTexture = {
  8, 1, (uint8_t[]){
  0b00111100, 
  0b01001010, 
  0b10000001, 
  0b11111011, 
  0b10001001, 
  0b10001001, 
  0b01001010, 
  0b00111100, 
  }
           
};
const struct bitmap arrowImageTexture= {
  8, 1, (uint8_t []) {
  0b00111100, 
  0b00111100, 
  0b00111100, 
  0b00111100, 
  0b11111111, 
  0b01111110, 
  0b00111100, 
  0b00011000, 
  }           
};

const struct bitmap frameTexture= {
  12, 2, (uint8_t []) {
  0b00001111, 0b11111111,
  0b00001000, 0b00000001,
  0b00001000, 0b00000001,
  0b00001000, 0b00000001,
  0b00001000, 0b00000001,
  0b00001000, 0b00000001,
  0b00001000, 0b00000001,
  0b00001000, 0b00000001,
  0b00001000, 0b00000001,
  0b00001000, 0b00000001,
  0b00001000, 0b00000001,
  0b00001111, 0b11111111,
  }           
};
