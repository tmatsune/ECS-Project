#ifndef STATES_H
#define STATES_H

#include "app/common_data.h"
#include "periph/pwm.h"
#include "periph/timer.h"

// ----------------------------------    DRIVE STATE    ------------------------------ //

typedef enum {
    DRIVE_STATE_ROTATE_LEFT,
    DRIVE_STATE_ROTATE_RIGHT,
    DRIVE_STATE_FORWARD,
} Internal_Drive_State; 

struct Drive_State{
    struct Common_Data *common_data;
    Internal_Drive_State internal_drive_state;
};

void drive_state_init(struct Drive_State *drive_state);
void drive_state_enter(struct Drive_State *drive_state, State from_state, Event event);
void drive_state_run(struct Drive_State *drive_state);

// ----------------------------------    AVOID STATE    ------------------------------ //

typedef enum {
    AVOID_STATE_REVERSE,
    AVOID_STATE_180_FLIP,
    AVOID_STATE_ALIGN_LEFT,
    AVOID_STATE_ALIGN_RIGHT,
} Internal_Avoid_State; 

struct Avoid_State {
    struct Common_Data *common_data;
    Internal_Avoid_State internal_avoid_state;
    uint8_t move_index;
};

void avoid_state_init(struct Avoid_State *avoid_state);
void avoid_state_enter(struct Avoid_State *avoid_state, State from_state, Event event);
void avoid_state_run(struct Avoid_State *avoid_state);

// ----------------------------------   RETREAT STATE   ------------------------------ //

typedef enum {
    RETREAT_STATE_REVERSE,
    RETREAT_STATE_FORWARD,
    RETREAT_STATE_REVERSE_FLIP_LEFT,
    RETREAT_STATE_REVERSE_FLIP_RIGHT,
    RETREAT_STATE_HALF_TURN_LEFT,
    RETREAT_STATE_HALF_TURN_RIGHT,
    RETREAT_STATE_ALIGN_LEFT,
    RETREAT_STATE_ALIGN_RIGHT,
} Internal_Retreat_State;

struct Retreat_State {
    struct Common_Data *common_data;
    Internal_Retreat_State internal_retreat_state;
    uint8_t move_index;
};

void retreat_state_init(struct Retreat_State *retreat_state);
void retreat_state_enter(struct Retreat_State *retreat_state, State from_state, Event event);
void retreat_state_run(struct Retreat_State *retreat_state);

#endif