#include <avr/interrupt.h>
#include "i2c.h"
#include "timer.h"
#include "utils.h"

#define CLK PD2
#define DT PD3

const uint8_t throttle_time = 15;
uint32_t last_signal_process = 0;
int8_t state = 0;


enum State {
  Idle,
  ClkRecv,
  DtRecv,
} currentState;

void setCount(int8_t state) {          // Устанавливаем значение счетчика
  if (state == 4) {
    // next_item();
  }
  if (state == -4) {
    // prev_item();
  }
}


void process_clk(uint32_t *ms_from_start) {
  // if (*ms_from_start - last_signal_process < throttle_time) return;  // Если с момента последнего изменения состояния не прошло
  // достаточно времени - выходим из прерывания
  
  uint8_t clk_value= getValue(&PINB, CLK);            // Получаем состояние пинов A и B
  uint8_t dt_value = getValue(&PINB, DT);
  if (clk_value) {
    next_item();
    last_signal_process = *ms_from_start;
    return;
  } else {
    return;
  }

  // cli();    // Запрещаем обработку прерываний, чтобы не отвлекаться
  if ((state == 0  && !clk_value &&  dt_value) || (state == 2  && clk_value && !dt_value)) {
    state += 1; // Если выполняется условие, наращиваем переменную state
    // last_signal_process = *ms_from_start;
  }
  if ((state == -1 && !clk_value && !dt_value) || (state == -3 && clk_value &&  dt_value)) {
    state -= 1; // Если выполняется условие, наращиваем в минус переменную state
    // last_signal_process = *ms_from_start;
  }
  setCount(state); // Проверяем не было ли полного шага из 4 изменений сигналов (2 импульсов)
  // sei(); // Разрешаем обработку прерываний

  if (clk_value && dt_value && state != 0) state = 0; // Если что-то пошло не так, возвращаем статус в исходное состояние
}

void process_dt(uint32_t *ms_from_start) {
  // if (*ms_from_start - last_signal_process < throttle_time) return;

  uint8_t clk_value= getValue(&PINB, CLK);            // Получаем состояние пинов A и B
  uint8_t dt_value = getValue(&PINB, DT);

  // cli();
  if ((state == 1 && !clk_value && !dt_value) || (state == 3 && clk_value && dt_value)) {
    state += 1; // Если выполняется условие, наращиваем переменную state
    last_signal_process = *ms_from_start;
  }
  if ((state == 0 && clk_value && !dt_value) || (state == -2 && !clk_value && dt_value)) {
    state -= 1; // Если выполняется условие, наращиваем в минус переменную state
    last_signal_process = *ms_from_start;
  }
  setCount(state); // Проверяем не было ли полного шага из 4 изменений сигналов (2 импульсов)
  // sei();
  
  if (clk_value && dt_value && state != 0) state = 0; // Если что-то пошло не так, возвращаем статус в исходное состояние
}


