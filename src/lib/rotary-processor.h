#ifndef  ROTATY_PROCESSOR_H
#define  ROTATY_PROCESSOR_H
#include <avr/io.h>

void process_clk(uint32_t *ms_from_start);
void process_dt(uint32_t *ms_from_start);

#endif
