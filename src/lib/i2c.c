#include <avr/io.h>
#include <util/twi.h>
#include "i2c.h"
#include "utils.h"

uint8_t error = 0;
uint8_t NO_START = 1;
uint8_t NO_TSLA_ACK = 1 << 1;
uint8_t NO_RSLA_ACK = 1 << 2;
uint8_t NO_DATA_ACK = 1 << 4; 
uint8_t SSS = 3; 
uint8_t is_called = 0;

void i2c_init() {
  TWSR = 0x02;
  TWBR = 0x02;
  TWCR = (1 << TWEN);
}

void i2c_listen(uint8_t addr) {
  TWAR = addr;
  TWCR = (1<<TWIE) | (1<<TWEA) | (1<<TWINT) | (1<<TWEN);
}

uint8_t i2c_error() {
  uint8_t err =  error | (is_called << SSS);
  error = 0;
  is_called = 0;
  return err;
}

void dcalled() {
  is_called = 0;
}

void called() {
  is_called = 1;
}

void i2c_start() {
  TWCR = (1<<TWINT) | (1<<TWSTA) | (1 << TWEN);
  while (!(TWCR & (1<<TWINT)));
  switch (TW_STATUS) {
    case TW_START:
    case TW_REP_START:
      return;
    default:
      error |= NO_START;
  }
}

void i2c_stop() {
  TWCR = mask(TWINT) | mask(TWSTO) | mask(TWEN);
}

void i2c_write(uint8_t data) {
  if (error !=0)
    return;
  TWDR = data;
  TWCR = mask(TWINT) | mask(TWEN);
  while (!(TWCR & (1<<TWINT)));
  if (TW_STATUS != TW_MT_DATA_ACK) {
    error |= NO_DATA_ACK;
  }
}
void i2c_address(uint8_t data) {
  if (error != 0)
    return;
  TWDR = data | TW_WRITE;
  TWCR = mask(TWINT) | mask(TWEN);
  while (!(TWCR & (1<<TWINT)));
  if (TW_STATUS != TW_MT_SLA_ACK) {
    error |= NO_TSLA_ACK;
  }
}

void i2c_address_slave(uint8_t data) {
  if (error != 0)
    return;
  TWDR = data | TW_READ;
  TWCR = mask(TWINT) | mask(TWEN);
  while (!(TWCR & (1<<TWINT)));
  if (TW_STATUS != TW_MR_SLA_ACK) {
    error |= NO_RSLA_ACK;
  }
}

uint8_t i2c_read_ack() {
  TWCR = mask(TWINT) | mask(TWEN) | mask(TWEA);
  while (!(TWCR & (1<<TWINT)));
  return TWDR;
}
uint8_t i2c_read_nack() {
  TWCR = mask(TWINT) | mask(TWEN);
  while (!(TWCR & (1<<TWINT)));
  return TWDR;
}

uint8_t i2c_get_status() {
  uint8_t status;
  status = TWSR & 0xf8;
  return status;
}

void i2c_start_with_slave(uint8_t address) {
  i2c_start();
  i2c_address_slave(address);
}
void i2c_start_with(uint8_t address) {
  i2c_start();
  i2c_address(address);
}

