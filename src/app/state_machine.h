#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H
#include <Arduino.h>
#include <stdint.h>
#include "app/obj.h"
#include "app/line.h"
#include "app/states.h"
#include "app/common_data.h"
#include "periph/timer.h"

typedef struct {
    State from_state;
    Event event;
    State to_state;
} Scene_Transition; 

struct State_Machine{
    State state;
    Common_Data common_data;
    struct Drive_State drive_state;
    struct Avoid_State avoid_state;
    struct Retreat_State retreat_state;
    Event internal_event;
    Timer *timer;
};

void state_machine_init(struct State_Machine *state_machine, Timer *timer);
void state_machine_run(struct State_Machine *state_machine, uint16_t range);

#endif