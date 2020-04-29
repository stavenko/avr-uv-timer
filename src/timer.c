#include "timer.h"
const  uint8_t MainMenuItemsCount = 2;
const uint8_t EditNowCount= 2;
static struct Timer ApplicationState = { MainMenu, RunTimer, Minutes, 0, 40, 0 };
void press_button() {
  if (ApplicationState.state == MainMenu) {
    if (ApplicationState.menuPosition == RunTimer) {
      ApplicationState.state = TimerRunning;
    }
    if (ApplicationState.menuPosition == RunTimer) {
      ApplicationState.state = TimerRunning;
    }
  }

  if (ApplicationState.state == TimerSetup) {
    if(ApplicationState.editNow == Minutes) {
      ApplicationState.editNow = Seconds;
      return;
    } 
    if(ApplicationState.editNow == Seconds) {
      ApplicationState.editNow = Minutes;
      return;
    } 
  }
};

void inc(uint8_t *value, uint16_t limit) {
  *value = ( *value + 1) % limit;
}
void dec(uint8_t *value, uint16_t limit) {
  *value = ( *value - 1) % limit;
}

void inc16(uint16_t *value, uint16_t limit) {
  *value = ( *value + 1) % limit;
}
void dec16(uint16_t *value, uint16_t limit) {
  *value = ( *value - 1) % limit;
}
void next_item(){
  if (ApplicationState.state == MainMenu) {
    inc16(&ApplicationState.menuPosition, MainMenuItemsCount);
  }
  
  if (ApplicationState.state == TimerSetup && ApplicationState.editNow == Minutes) {
    inc16(&ApplicationState.minutes, 999);
  }

  if (ApplicationState.state == TimerSetup && ApplicationState.editNow == Seconds) {
    inc(&ApplicationState.seconds, 60);
  }

}
void prev_item(){
  if (ApplicationState.state == MainMenu) {
    dec16(&ApplicationState.menuPosition, MainMenuItemsCount);
  }
  
  if (ApplicationState.state == TimerSetup && ApplicationState.editNow == Minutes) {
    dec16(&ApplicationState.minutes, 999);
  }

  if (ApplicationState.state == TimerSetup && ApplicationState.editNow == Seconds) {
    dec(&ApplicationState.seconds, 60);
  }
}
void ms_tick() {
  if (ApplicationState.state ==TimerRunning) {
    ApplicationState.mseconds++;
    uint16_t ms = 1000 * (ApplicationState.minutes * 60  + ApplicationState.seconds);
    if (ms <= ApplicationState.mseconds) {
      ApplicationState.state = MainMenu;
      ApplicationState.mseconds = 0;
    }
  }
}
const struct Timer *get_app_state() {
  return &ApplicationState;
}

void _debug(uint16_t v) {
  ApplicationState.minutes = v;
}
