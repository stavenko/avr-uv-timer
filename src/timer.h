#ifndef TIMER_H
#define TIMER_H
#include <avr/io.h>

enum AppState{
  MainMenu,
  TimerSetup,
  TimerRunning
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
};

void press_button();
void next_item();
void prev_item();
void ms_tick();
void _debug();
const struct Timer *get_app_state();

#endif
