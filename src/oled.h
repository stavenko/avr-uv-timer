#ifndef OLED_H
#define OLED_H
#include <avr/io.h>
#include <stdlib.h>
#include "frame-buffer.h"
#define OLED_I2C_SLAVE_ADDR 0x78

void oled_send_data(uint8_t *data, uint16_t bytes);
void oled_send_command(uint8_t *data, uint8_t bytes);
void oled_send_symbol(struct bitmap *symbol, struct coords *to_coords);
void oled_turn_on();
void oled_turn_sleep();
void oled_set_horisontal_memory_mode();
void oled_set_contrast(uint8_t contrast);
void oled_ignore_ram(uint8_t ignore);
void oled_init();

void oled_clear_screen();

#endif
