#include <stdlib.h>
#include "./timer.h"
#include "i2c.h"

const  uint8_t MainMenuItemsCount = 2;
const uint8_t EditNowCount= 2;

static struct  Timer *ApplicationState;//   = { TimerSetup, SetupTimer, Minutes, 0, 5, 0 };

void initAppState(uint16_t min, uint8_t sec, enum AppState state) {
  ApplicationState = malloc(sizeof(struct Timer));
  ApplicationState->minutes = min;
  ApplicationState->seconds = sec;
  ApplicationState->state = state;
  ApplicationState->menuPosition = SetupTimer;
  ApplicationState->editNow = Minutes;
  ApplicationState->mseconds = 0;
  ApplicationState->is_blink = 0;
}

void press_button() {
  if (ApplicationState->state == MainMenu) {
    if (ApplicationState->menuPosition == RunTimer) {
      ApplicationState->state = TimerRunning;
      ApplicationState->mseconds = 0;
      return;
    }
    if (ApplicationState->menuPosition == SetupTimer) {
      ApplicationState->state = TimerSetup;
      return;
    }
    return;
  }

  if (ApplicationState->state == TimerSetup) {
    if(ApplicationState->editNow == Minutes) {
      ApplicationState->editNow = Seconds;
      return;
    } 
    if(ApplicationState->editNow == Seconds) {
      ApplicationState->editNow = Minutes;
      ApplicationState->state = MainMenu;
      return;
    } 
  }
};

void inc(uint8_t *value, uint16_t limit) {
  if (++*value > limit) *value = 0;
}

void dec(uint8_t *value, uint16_t limit) {
  if (--*value > limit) *value = limit;
}

void inc16(uint16_t *value, uint16_t limit) {
  if (++*value > limit) *value = 0;
}

void dec16(uint16_t *value, uint16_t limit) {
  if (--*value > limit) *value = limit;
}

void next_item(){
  if (ApplicationState->state == MainMenu) {
    inc16(&ApplicationState->menuPosition, MainMenuItemsCount - 1);
  }
  
  if (ApplicationState->state == TimerSetup && ApplicationState->editNow == Minutes) {
    inc16(&ApplicationState->minutes, 999);
  }

  if (ApplicationState->state == TimerSetup && ApplicationState->editNow == Seconds) {
    inc(&ApplicationState->seconds, 60);
  }

}
void prev_item(){
  if (ApplicationState->state == MainMenu) {
    dec16(&ApplicationState->menuPosition, MainMenuItemsCount - 1);
  }
  
  if (ApplicationState->state == TimerSetup && ApplicationState->editNow == Minutes) {
    dec16(&ApplicationState->minutes, 999);
  }

  if (ApplicationState->state == TimerSetup && ApplicationState->editNow == Seconds) {
    dec(&ApplicationState->seconds, 60);
  }
}
void ms_tick() {
  ApplicationState->mseconds++;
  ApplicationState->is_blink = (ApplicationState->mseconds / 333) % 2;
  if (ApplicationState->state == TimerRunning) {
    uint32_t mms = ApplicationState->minutes * 60;
    uint32_t mmms = ApplicationState->seconds;
    uint32_t ms = 1000 * (mms + mmms);

    if (ms <= ApplicationState->mseconds) {
      ApplicationState->state = MainMenu;
      ApplicationState->mseconds = 0;
    }
  }
}
const struct Timer *get_app_state() {
  return ApplicationState;
}

void get_minutes_and_seconds(const struct Timer * state, uint16_t *minutes_left, uint8_t* seconds_left) {
  if (state->state == TimerRunning) {
    uint32_t total_seconds_gone = state->mseconds / 1000;
    uint32_t total_seconds = (state->minutes * 60);
    total_seconds += (uint32_t) state->seconds;
    uint32_t total_seconds_left  = total_seconds - total_seconds_gone;

    *seconds_left = total_seconds_left % 60;
    *minutes_left = total_seconds_left / 60;
    return;
  }
  *seconds_left = state->seconds;
  *minutes_left = state->minutes;
}

void get_timer_data(uint8_t *bytes) {
  const struct Timer *timer = ApplicationState;
  uint16_t minutes;
  uint8_t seconds;
  get_minutes_and_seconds(timer, &minutes,  &seconds);
  uint8_t is_blink_and_show = timer->is_blink;
  uint8_t fst = (minutes >> 8) & 0x0003;
  fst |= ((uint8_t) timer->state & 0b00000011) << 6; 
  fst |= timer->menuPosition << 5;
  fst |= timer->editNow << 4;
  fst |= is_blink_and_show << 3;
  uint8_t snd = minutes & 0x00ff; 
  bytes[0] = fst;
  bytes[1] = snd;
  bytes[2] = seconds;
}

void set_timer_data(uint8_t *bytes) {
  uint16_t bt0 = bytes[0] & 0x03;
  uint16_t bt1 = bytes[1];

  uint16_t minutes = bt1 | ( bt0 << 8);
  uint8_t seconds = bytes[2];
  uint8_t state = (bytes[0] & 0b11000000) >> 6;
  uint8_t menuPosition = (bytes[0] & 0b00100000) >> 5;
  uint8_t editNow = (bytes[0] & 0b00010000) >> 4;
  uint8_t is_blink_and_show = (bytes[0] & 0b00001000) >> 3;
  ApplicationState->is_blink = is_blink_and_show;
  ApplicationState->minutes = minutes;
  ApplicationState->seconds = seconds;
  ApplicationState->state = state;
  ApplicationState->menuPosition = menuPosition;
  ApplicationState->editNow = editNow;
  ApplicationState->mseconds = 0;
}
void _debug(uint16_t v) {
  ApplicationState->minutes = v;
}

void set_error(uint16_t err){
  // ApplicationState->seconds = 255;
  // ApplicationState->minutes = err;
}
