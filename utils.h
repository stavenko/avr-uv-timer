#ifndef UTILS_H
#define UTILS_H
#include <avr/io.h>

uint8_t mask(uint8_t bit);
uint8_t getValue(volatile uint8_t *port, uint8_t bit);

#endif
