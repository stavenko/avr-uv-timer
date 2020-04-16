#include "util.h"

uint8_t mask(uint8_t bit) {
  return 1 << bit;
}

uint8_t getValue(volatile uint8_t *port, uint8_t bit) {
  return ((*port) & mask(bit)) >> bit;
}

