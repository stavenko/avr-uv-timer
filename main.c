#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/cpufunc.h> 
#include <stdlib.h>
#include "utils.h"

#define LED PD4
#define LED_PORT PORTD
#define SENSOR_DT PB0
#define SENSOR_CLK PB1
#define SENSOR_BTN PB2
#define OLED_I2C_SLAVE_ADDR 0x78
const uint16_t ms1_ticks = 125;
const uint16_t ms100_ticks = ms1_ticks * 100;

const uint16_t some_period = 0xffff - ms100_ticks;
const uint8_t increment = 140;
uint16_t milliseconds_passed = 0;
uint8_t isPressed;
int16_t additional = 0;



ISR (TIMER1_OVF_vect)
{
  LED_PORT ^= (1 << LED);
  TCNT1 = some_period + (additional);
  milliseconds_passed += 1;
}


uint8_t sensor_clk = 0;


void process_sensor_state() {
  uint8_t clk = getValue(&PINB, SENSOR_CLK); 
  if (sensor_clk != clk) {
    sensor_clk = clk;
    uint8_t dt = getValue(&PINB, SENSOR_DT); 
    if (dt != clk) {
      additional += increment;
    } else {
      additional -= increment;
    }
  }
  uint8_t btn = getValue(&PINB, SENSOR_BTN); 

  if (btn == 0) {
    additional = 0;
  }
}

void setup_timer() {
  TCNT1 = some_period;
  
  TCCR1A = 0;
  TCCR1B = (1 << CS11) | (1<<CS10);
  TIMSK = (1 << TOIE1);
}


int main() {
  DDRD = (1 << LED);
  setup_timer();
  PORTB = (1 << SENSOR_DT ) | (1 << SENSOR_CLK ) | (1 << SENSOR_BTN );
  DDRB = 0x00;
  _NOP();
  sensor_clk = getValue(&PINB, SENSOR_CLK);

  sei();
  while (1) {
    process_sensor_state();
  };
  return 1;
}
