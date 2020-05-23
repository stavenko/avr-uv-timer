#ifndef  constants_h
#define  constants_h
#include "frame-buffer.h"
#define SLAVE_ADDR 0xBA

extern struct coords clock_timer_coords;
extern struct coords clock_icon_coords; 
extern uint8_t space_in_word;
extern uint8_t space_in_sentence;
extern const struct bitmap arrowImageTexture;
extern const struct bitmap clockImageTexture;
extern const struct bitmap frameTexture;

#endif
