#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/cpufunc.h> 
#include <util/twi.h>
#include <stdlib.h>
#include "../lib/i2c.h"
#include "../lib/constants.h"
#include "../lib/timer.h"
#include "../lib/utils.h"
#include "../lib/rotary-processor.h"

uint8_t SIZE = 3;
uint8_t *output_buffer;
uint8_t ptr = 0;
#define LED_PORT PORTD
#define LED PD5
#define SENSOR_DT PB0
#define SENSOR_CLK PB1
#define SENSOR_BTN PB2
#define DT_CLK (1 << SENSOR_DT) | (1 << SENSOR_CLK)
const uint16_t ms1_ticks = 125;
const uint16_t some_period = 0xffff - ms1_ticks;
uint8_t biipeer = 0;
uint16_t counted_ticks = 0;
uint32_t milliseconds_from_start = 0;
uint32_t last_clk_read = 0;
uint8_t throttle_read = 10;


uint8_t send_timer_data() {
  if (ptr >= SIZE) {
    TWDR = 0x0;
    return 0;
  }
  TWDR = output_buffer[ptr];
  ptr++;

  return ptr < SIZE;
}

enum RotaryState {
  Idle,
  ClkDown,
  ClkFirstDtNext,
  ClkUpDtDown,

  DtDown,
  DtFirstSlkNext,
  DtUpSlkDown,
} rotary_state = Idle; 
enum ButtonState {
  BtnIdle,
  BtnPressed,
} btnState = BtnIdle; 


void main_loop () {
  // uint8_t prev_clk = getThrottledValueB(SENSOR_CLK, &milliseconds_from_start);
  // uint8_t prev_dt= getThrottledValueB(SENSOR_DT, &milliseconds_from_start);
  while (1) {
    uint8_t clk = getValue(&PINB, SENSOR_CLK); // getThrottledValueB(SENSOR_CLK, &milliseconds_from_start);
    uint8_t dt = getValue(&PINB, SENSOR_DT);
    if (!clk && dt && rotary_state == Idle) {
      rotary_state = ClkDown;
    } else if (!clk && !dt && rotary_state == ClkDown) {
      rotary_state = ClkFirstDtNext;
    } else if (clk && !dt && rotary_state == ClkFirstDtNext) {
      rotary_state = ClkUpDtDown;
    } else if (clk && dt && rotary_state == ClkUpDtDown) {
      rotary_state = Idle;
      next_item();
    }

    else if (clk && !dt && rotary_state == Idle) {
      rotary_state = DtDown;
    } else if (!clk && !dt && rotary_state == DtDown) {
      rotary_state = DtFirstSlkNext;
    } else if (!clk && dt && rotary_state == DtFirstSlkNext) {
      rotary_state = DtUpSlkDown;
    } else if (clk && dt && rotary_state == DtUpSlkDown) {
      rotary_state = Idle;
      prev_item();
    }

    const uint8_t btn = getValue(&PINB, SENSOR_BTN);
    if (!btn && btnState == BtnIdle) {
      btnState = BtnPressed;
    }
    if (btn && btnState == BtnPressed) {
      btnState = BtnIdle;
      press_button();
    }
  }
}


ISR (TIMER1_OVF_vect)
{
  TCNT1 = some_period;
  counted_ticks = (counted_ticks+1) % 1000;
  ++milliseconds_from_start;
  if (!counted_ticks) {
    biipeer ^= 0x01;
  }
  uint8_t i2c_err = i2c_error();
  LED_PORT = (biipeer << LED); 
  PORTC |= i2c_err; 
  ms_tick();
  if (get_app_state()->state == TimerRunning) {
    PORTC |= (1 << PORTC3);
  } else {
    PORTC &= (0 << PORTC3);
  }
}

#define COMMON_CONTROL (1<<TWIE) | (1<<TWINT) | (1<<TWEA) | (1<<TWEN)

ISR(TWI_vect)
{
  switch(TW_STATUS)
  {
    case TW_SR_DATA_ACK:
      // received data from master, call the receive callback
      // I2C_recv(TWDR); 
      TWCR = COMMON_CONTROL;
      break;
    case TW_ST_SLA_ACK:
      // master is requesting data, call the request callback
      get_timer_data(output_buffer);
      
      ptr = 0;
      send_timer_data();
      TWCR = COMMON_CONTROL;
      break;
    case TW_ST_DATA_ACK:
      // master is requesting data, call the request callback
      // called();
      if (send_timer_data()) {
        TWCR = COMMON_CONTROL;
      } else {
        TWCR = (1<<TWIE) | (1<<TWINT) | (1<<TWEN);
      }
      break;

    case TW_ST_LAST_DATA:
    case TW_ST_DATA_NACK:
      TWCR = COMMON_CONTROL;
      break;
    case TW_BUS_ERROR:
      // some sort of erroneous state, prepare TWI to be readdressed
      TWCR = 0;
      TWCR = COMMON_CONTROL; 
      break;
    default:
      TWCR = COMMON_CONTROL;
      break;
  }
} 


void setup_timer() {
  TCNT1 = some_period;
  
  TCCR1A = 0;
  TCCR1B = (1 << CS11) | (1<<CS10);
  TIMSK = (1 << TOIE1);
}

int main() {
  DDRD = (1 << LED); // | (1 << PD0) | (1 << PD1) | (1 << PD2) | (1 << PD3);
  DDRC = 0x0f;
  DDRB = 0x00;
  // GICR = 0xc0;
  // MCUCSR = 0x05;

  _NOP();
  // PORTD |= (1 << PD2);
  // PORTB = (1 << SENSOR_DT ) | (1 << SENSOR_CLK ) | (1 << SENSOR_BTN );
  // prev_pinb = PINB & mask(SENSOR_CLK) >> SENSOR_CLK;
  // uint8_t next_dt = PINB & mask(SENSOR_DT)  >> SENSOR_DT;
  setup_timer();
  initAppState(2, 2, MainMenu);
  output_buffer = malloc(SIZE);
  i2c_init();
  i2c_listen(SLAVE_ADDR);
  sei();
  main_loop();
}




