#ifndef LINE_H 
#define LINE_H

#include <Arduino.h>
#include "periph/adc.h"

typedef enum {
    LINE_NONE,
    LINE_FRONT,
    LINE_BACK,
    LINE_LEFT,
    LINE_RIGHT,
    LINE_FRONT_LEFT,
    LINE_FRONT_RIGHT,
    LINE_BACK_LEFT,
    LINE_BACK_RIGHT,
} Line_Pos;

void lines_init(void);
Line_Pos get_lines();

#endif