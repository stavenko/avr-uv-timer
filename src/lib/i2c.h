#include <avr/io.h>

#ifndef I2C_H
#define I2C_H
void i2c_init();
void i2c_listen(uint8_t addr);

void i2c_start();

void i2c_stop();
void i2c_write(uint8_t data);

uint8_t i2c_read_ack();
uint8_t i2c_read_nack();

uint8_t i2c_get_status();

void i2c_start_with(uint8_t address);
void i2c_start_with_slave(uint8_t address);
uint8_t i2c_error();
void dcalled();
void called();

#endif
