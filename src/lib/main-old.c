/*
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/cpufunc.h> 
#include <stdlib.h>
#include "utils.h"
#include "i2c.h"
#include "get-glyph-from-font.h"
#include "oled.h"
#include "constants.h"
#include "timer.h"
#include "render-ui.h"

#define LED PD5
#define LED_PORT PORTD
#define SENSOR_DT PB0
#define SENSOR_CLK PB1
#define SENSOR_BTN PB2
#define DT_CLK (1 << SENSOR_DT) | (1 << SENSOR_CLK)
const uint16_t ms1_ticks = 125;
const uint16_t ms100_ticks = ms1_ticks * 100;

const uint16_t some_period = 0xffff - ms1_ticks;
const uint8_t increment = 140;
struct bitmap clock_icon;
uint8_t isPressed;
int16_t additional = 0;


uint8_t biipeer = 0;
uint16_t counter = 0;


ISR (TIMER1_OVF_vect)
{
  TCNT1 = some_period;
  biipeer ^= 0x01;
  LED_PORT = (biipeer << LED) | i2c_error();
  counter = 0;

  ms_tick();

  const struct Timer* timer_state = get_app_state();
  // render_ui(timer_state);
}




void process_sensor_state() {
  uint8_t new_value = PINB & DT_CLK;
  uint8_t diff = prev_pinb ^ new_value;
  if (diff) {
    while(1) {
      uint8_t next_port_value = PINB & DT_CLK;
      uint8_t next_diff = new_value ^ next_port_value;
      if (next_diff) {
        if (diff != next_diff) {
          if (diff & mask(SENSOR_CLK) && next_diff & mask(SENSOR_DT)) {
            additional += increment;
            next_item();
          }
          if (diff & mask(SENSOR_DT) && next_diff & mask(SENSOR_CLK)) {
            additional -= increment;
            prev_item();
          }
        }
        prev_pinb = next_port_value;
        break;
      }
    }
  } else {
    if ((new_value & mask(SENSOR_BTN)) == 0) {
      additional = 0;
      press_button();
    }
    prev_pinb = new_value;
  }
}

void setup_timer() {
  TCNT1 = some_period;
  
  TCCR1A = 0;
  TCCR1B = (1 << CS11) | (1<<CS10);
  TIMSK = (1 << TOIE1);
}


int main_old() { 
  DDRD = (1 << LED) | (1 << PD0) | (1 << PD1) | (1 << PD2) | (1 << PD3);
  setup_timer();
  DDRB = 0x00;
  _NOP();
  // prev_pinb = PINB & DT_CLK;;
  PORTB = (1 << SENSOR_DT ) | (1 << SENSOR_CLK ) | (1 << SENSOR_BTN );
  _NOP();
  prev_pinb = PINB & ((1<< SENSOR_CLK));
  i2c_init();
  oled_turn_sleep();
  oled_init();
  oled_clear_screen();
  oled_turn_on();

  sei();
  while (1) {
    process_sensor_state();
  };
  return 1;
}
*/
