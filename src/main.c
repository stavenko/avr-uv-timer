#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/cpufunc.h> 
#include <stdlib.h>
#include "utils.h"
#include "i2c.h"
#include "oled.h"

#define LED PD5
#define LED_PORT PORTD
#define SENSOR_DT PB0
#define SENSOR_CLK PB1
#define SENSOR_BTN PB2
const uint16_t ms1_ticks = 125;
const uint16_t ms100_ticks = ms1_ticks * 100;

const uint16_t some_period = 0xffff - ms100_ticks;
const uint8_t increment = 140;
uint16_t milliseconds_passed = 0;
uint8_t isPressed;
int16_t additional = 0;

uint8_t texture[] = {
  0xff, 
  0xf0,
  0xf0,
  0xff,
  0xff, 
  0x0f,
  0x0f,
  0xff
};
uint8_t texture2[] = {
  0x81, 0x81,
  0x83, 0xc1,
  0x83, 0xc1,
  0x81, 0x81
};

uint8_t ignore = 0x01;

uint8_t biipeer = 0;

struct coords c = {0, 0};

ISR (TIMER1_OVF_vect)
{
  biipeer ^= 0x01;
  LED_PORT = (biipeer << LED)| i2c_error();
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
  DDRD = (1 << LED) | (1 << PD0) | (1 << PD1) | (1 << PD2) | (1 << PD3);
  setup_timer();
  PORTB = (1 << SENSOR_DT ) | (1 << SENSOR_CLK ) | (1 << SENSOR_BTN );
  DDRB = 0x00;
  _NOP();
  sensor_clk = getValue(&PINB, SENSOR_CLK);
  i2c_init();
  oled_turn_sleep();
  oled_init();
  oled_clear_screen();
  oled_turn_on();

  struct bitmap bm;
  bm.buffer = texture2;
  bm.width = 4;
  bm.height = 2;
  c.top = 4;
  oled_send_symbol(&bm, &c);


  sei();
  while (1) {
    process_sensor_state();
  };
  return 1;
}
