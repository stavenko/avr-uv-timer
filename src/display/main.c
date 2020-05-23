#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <string.h>
#include <avr/cpufunc.h> 
#include "../lib/timer.h"
#include "../lib/i2c.h"
#include "../lib/oled.h"
#include "../lib/timer.h"
#include "../lib/render-ui.h"
#include "../lib/constants.h"

uint8_t SIZE = 3;
uint8_t *input_buffer;
#define LED_PORT PORTD
#define LED PD5
const uint16_t ms1_ticks = 125;
const uint16_t count_1ms = 0xffff - ms1_ticks;
uint16_t ms_from_start = 0;
uint8_t oled_inited = 0;


void request_timer_state() {
  i2c_start_with_slave(SLAVE_ADDR);
  uint8_t err = i2c_error();
  if (err) {
    set_error(err);
    return;
  }
  
  for (uint8_t i = 0; i < SIZE - 1; i++ ) {
    input_buffer[i] = i2c_read_ack();
    err = i2c_error();
    if (err) {
      set_error(err);
      return;
    }
  }
  input_buffer[SIZE - 1] = i2c_read_nack();
  err = i2c_error();
  if (err) {
    set_error(err);
    return;
  }
  i2c_stop();
  set_timer_data(input_buffer);
}

void main_loop () {
  initAppState(777, 55, MainMenu);
  while (1) {
    if (oled_inited) {
      const struct Timer *appState = get_app_state();
      request_timer_state();
      render_ui(appState);
    }
    _NOP();
  }
}

ISR (TIMER1_OVF_vect)
{
  
  // uint8_t i2c_err = i2c_error();
  // LED_PORT = (biipeer << LED) | i2c_err; 
  if ((ms_from_start > 100) && (!oled_inited)) {
    i2c_init();
    oled_turn_sleep();
    oled_init();
    oled_clear_screen();
    oled_turn_on();
    oled_inited = 1;
    TIMSK = 0x0;
  } else {
    ms_from_start ++;
    TCNT1 = count_1ms;
  }
}

void setup_timer() {
  TCNT1 = count_1ms;
  TCCR1A = 0;
  TCCR1B = (1 << CS11) | (1<<CS10);
  TIMSK = (1 << TOIE1);
}

int main() {
  DDRD = (1 << LED) | (1 << PD0) | (1 << PD1) | (1 << PD2) | (1 << PD3);
  DDRB = 0x00;
  setup_timer();
  input_buffer = malloc(SIZE);
  memset(input_buffer, 0, SIZE);
  sei();
  main_loop();
}



