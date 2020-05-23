#ifndef TIMER_H
#define TIMER_H
#include <avr/io.h>

enum AppState{
  MainMenu = 0,
  TimerSetup = 1,
  TimerRunning = 2
};

extern const  uint8_t MainMenuItemsCount;
extern const uint8_t EditNowCount;
enum MainMenuItems {
  RunTimer,
  SetupTimer
};

enum EditNow {
  Minutes,
  Seconds
};


struct Timer{
  enum AppState state;
  enum MainMenuItems menuPosition;
  enum EditNow editNow;
  uint16_t minutes;
  uint8_t seconds;
  uint32_t mseconds;
  uint8_t is_blink;
};

void initAppState(uint16_t min, uint8_t sec, enum AppState state);
void get_minutes_and_seconds(const struct Timer * state, uint16_t *minutes_left, uint8_t* seconds_left);
void get_timer_data( uint8_t *bytes);
void press_button();
void next_item();
void prev_item();
void ms_tick();
void set_timer_data(uint8_t *data);
void set_error(uint16_t err);
const struct Timer *get_app_state();

#endif
