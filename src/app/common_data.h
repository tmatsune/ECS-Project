#ifndef COMMON_DATA_H 
#define COMMON_DATA_H

#include "app/line.h"
#include "app/obj.h"

typedef enum {
	EVENT_TIMEOUT,
	EVENT_LINE,
	EVENT_OBJ, 
	EVENT_FINISHED,
	EVENT_NONE,
} Event; 

typedef enum {
    STATE_DRIVE,
    STATE_RETREAT,
    STATE_AVOID
} State;

struct State_Machine;
struct Timer;

struct Common_Data{
    State_Machine* state_machine;
    Timer* timer;
    Obj obj; 
    Line_Pos line;
};

void post_internal_event(struct State_Machine *state_machine, Event event);

#endif 