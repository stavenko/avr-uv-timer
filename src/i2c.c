#include <avr/io.h>
#include "i2c.h"
#include "utils.h"

void i2c_init() {
  TWSR = 0x00;
  TWBR = 0x0C;
  TWCR = (1 << TWEN);
}

void i2c_start() {
  TWCR = mask(TWINT) | mask(TWSTA) | mask(TWEN);
  while (getValue(&TWCR, TWINT) == 0);
}

void i2c_stop() {
  TWCR = mask(TWINT) | mask(TWSTO) | mask(TWEN);
}

void i2c_write(uint8_t data) {
  TWDR = data;
  TWCR = mask(TWINT) | mask(TWEN);
  while (getValue(&TWCR, TWINT) == 0);
}

uint8_t i2c_read_ack() {
  TWCR = mask(TWINT) | mask(TWEN) | mask(TWEA);
  while (getValue(&TWCR, TWINT) == 0);
  return TWDR;
}
uint8_t i2c_read_nack() {
  TWCR = mask(TWINT) | mask(TWEN);
  while (getValue(&TWCR, TWINT) == 0);
  return TWDR;
}

uint8_t i2c_get_status() {
  uint8_t status;
  status = TWSR & 0xf8;
  return status;
}

void i2c_start_with(uint8_t address) {
  i2c_start();
  i2c_write(address);
}

