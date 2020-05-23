#include "utils.h"

static volatile  uint32_t lastBAccess = 0;
static volatile uint8_t  lastBValue = 0;
static const uint16_t throttle_time = 30;

uint8_t mask(uint8_t bit) {
  return 1 << bit;
}

uint8_t getValue(volatile uint8_t *port, uint8_t bit) {
  return ((*port) & mask(bit)) >> bit;
}


uint8_t getThrottledValueB(uint8_t bit, uint32_t *timestamp) {
  if ((*timestamp  - lastBAccess) < throttle_time && (*timestamp > lastBAccess)) {
    return lastBValue & mask(bit) >> bit;
  }
  lastBValue = PINB;
  lastBAccess = *timestamp;
  return (lastBValue & mask(bit)) >> bit;
}
