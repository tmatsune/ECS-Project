#include "app/states.h"
#include "app/common_data.h"
#include "app/state_machine.h"

#define DRIVE_FORWARD_TIMEOUT 40
#define DRIVE_ROTATE_TIMEOUT 80
#define MAX_MOVES 5

/* 
    Error codes 
    location:   none = 0, drive = 1 , avoid = 2, retreat = 3 
    func:       none = 0, init = 1, enter = 2, run = 3, 
    from_state: none = 0, drive = 1, avoid = 2, retreat = 3
    event:      timeout = 0, line = 1, obj = 2, finished = 3, none = 4
*/

// ----------------------------------    DRIVE STATE    ------------------------------ //

static Internal_Drive_State getRandomDriveState() {
    return static_cast<Internal_Drive_State>(random(0, 3));
}

void drive_state_init(struct Drive_State *drive_state){
    drive_state->internal_drive_state = DRIVE_STATE_FORWARD;
}

void drive_state_enter(struct Drive_State *drive_state, State from_state, Event event){
    switch(from_state){
        case STATE_DRIVE:
            switch(event){
                case EVENT_NONE:
                break;
                case EVENT_TIMEOUT:
                    if(drive_state->internal_drive_state == DRIVE_STATE_FORWARD) 
                        drive_state->internal_drive_state = DRIVE_STATE_ROTATE_LEFT;
                    else if(drive_state->internal_drive_state == DRIVE_STATE_ROTATE_LEFT) 
                        drive_state->internal_drive_state = DRIVE_STATE_ROTATE_RIGHT;
                    else if(drive_state->internal_drive_state == DRIVE_STATE_ROTATE_RIGHT)
                        drive_state->internal_drive_state = DRIVE_STATE_FORWARD;
                    drive_state_run(drive_state);
                break;
                default:
                    while(1){Serial.println("dr ent dr");}
                break;
            }
        break;
        case STATE_AVOID:
            switch(event){
                case EVENT_NONE:
                    drive_state_run(drive_state);
                break;
                case EVENT_FINISHED:
                    drive_state->internal_drive_state = DRIVE_STATE_FORWARD;
                    drive_state_run(drive_state);
                break;
                default:
                    while(1){Serial.println("dr ent avd");}
                break;
            }
        break;
        case STATE_RETREAT:
            switch(event){
                case EVENT_FINISHED:
                    drive_state->internal_drive_state = getRandomDriveState();
                    drive_state_run(drive_state);
                    break;
                default:
                    while(1){Serial.println("dr ent retr");}
                break;
            }
        break;
        default:
            while(1){Serial.println("dr ent frm");}
        break;
    }
}

void drive_state_run(struct Drive_State *state){
    switch(state->internal_drive_state){
        case DRIVE_STATE_ROTATE_LEFT:
            drive_set(DRIVE_DIR_WIDE_LEFT, DRIVE_SPEED_MAX);
            timer_set_timeout(state->common_data->timer, DRIVE_ROTATE_TIMEOUT);
            break;
        case DRIVE_STATE_ROTATE_RIGHT:
            drive_set(DRIVE_DIR_WIDE_RIGHT, DRIVE_SPEED_MAX);
            timer_set_timeout(state->common_data->timer, DRIVE_ROTATE_TIMEOUT);
            break;
        case DRIVE_STATE_FORWARD:
            drive_set(DRIVE_DIR_WIDE_LEFT, DRIVE_SPEED_MAX);
            timer_set_timeout(state->common_data->timer, DRIVE_ROTATE_TIMEOUT);
            break;
    }
}

// ---------------------------------        MOVES       ------------------------------- //

struct Move {
    drive_dir direction;
    drive_speed speed;
    int duration;
};

struct Drive_Move {
    struct Move moves[MAX_MOVES];
    uint8_t move_count;
};

// ----------------------------------    AVOID STATE    ------------------------------ //
static const Drive_Move avoid_states[] = {
    [AVOID_STATE_REVERSE] = {
        .moves = {{.direction = DRIVE_DIR_REVERSE, .speed = DRIVE_SPEED_MAX, .duration = 100}},
        .move_count = 1,
    },
    [AVOID_STATE_180_FLIP] = {
        .moves = {
            {.direction = DRIVE_DIR_REVERSE, .speed = DRIVE_SPEED_MAX, .duration = 120},
            {.direction = DRIVE_DIR_ROTATE_LEFT, .speed = DRIVE_SPEED_MAX, .duration = 60},
        },
        .move_count = 2,
    },
    [AVOID_STATE_ALIGN_LEFT] = {
        .moves = {
            {.direction = DRIVE_DIR_REVERSE, .speed = DRIVE_SPEED_MAX, .duration = 40},
            {.direction = DRIVE_DIR_SHARP_LEFT, .speed = DRIVE_SPEED_MAX, .duration = 80},
            {.direction = DRIVE_DIR_MID_RIGHT, .speed = DRIVE_SPEED_MAX, .duration = 120},
            {.direction = DRIVE_DIR_FORWARD, .speed = DRIVE_SPEED_MAX, .duration = 150},
        },
        .move_count = 4,
    },
    [AVOID_STATE_ALIGN_RIGHT] = {
        .moves = {
            {.direction = DRIVE_DIR_REVERSE, .speed = DRIVE_SPEED_MAX, .duration = 40},
            {.direction = DRIVE_DIR_SHARP_RIGHT, .speed = DRIVE_SPEED_MAX, .duration = 80},
            {.direction = DRIVE_DIR_MID_LEFT, .speed = DRIVE_SPEED_MAX, .duration = 120},
            {.direction = DRIVE_DIR_FORWARD, .speed = DRIVE_SPEED_MAX, .duration = 150},
        },
        .move_count = 4,
    },
};

static Internal_Avoid_State getRandomAvoidState() {
    return (rand() % 2 == 0) ? AVOID_STATE_ALIGN_LEFT : AVOID_STATE_ALIGN_RIGHT;
}

static Move current_avoid_move(const struct Avoid_State *avoid_state){
    return avoid_states[avoid_state->internal_avoid_state].moves[avoid_state->move_index];
}

static void start_avoid_move(const struct Avoid_State *avoid_state){
    if(avoid_state->move_index > avoid_states[avoid_state->internal_avoid_state].move_count){
        while(1) Serial.println("avoid idx out of bound");
    }
    struct Move move = current_avoid_move(avoid_state);
    timer_set_timeout(avoid_state->common_data->timer, move.duration);
    drive_set(move.direction, move.speed);
}

static Internal_Avoid_State next_avoid_state(struct Avoid_State *avoid_state){
    Obj_Range obj_range = avoid_state->common_data->obj.range;
    switch(obj_range){
        case OBJ_RANGE_CLOSE:
            return AVOID_STATE_REVERSE;
        case OBJ_RANGE_MID:
        case OBJ_RANGE_FAR:
            return getRandomAvoidState();
        case OBJ_RANGE_NONE:
            return AVOID_STATE_REVERSE;
    }
    while(1) Serial.println("avd: range not found");
    return AVOID_STATE_REVERSE;
}

bool avoid_state_done(const Avoid_State *avoid_state){
    return avoid_state->move_index == avoid_states[avoid_state->internal_avoid_state].move_count;
}

void avoid_state_enter(Avoid_State *avoid_state, State from_state, Event event) {
    switch (from_state) {
        case STATE_DRIVE:
            switch (event) {
                case EVENT_OBJ:
                    avoid_state_run(avoid_state);
                    break;
                default:
                    // TODO: Handle error - FROM DRIVE, ONLY OBJ EVENT SHOULD CAUSE AVOID ENTER
                    while(1) Serial.println("in avd, from dr");
                break;
            }
            break;

        case STATE_AVOID:
            switch (event) {
                case EVENT_LINE:
                    avoid_state_run(avoid_state);
                    break;
                case EVENT_TIMEOUT:
                    avoid_state->move_index += 1;
                    if (avoid_state_done(avoid_state)) {
                        // TODO: Post event finished since retreat state is the only place that knows if it's done
                        // State machine will pick up this event and transition to new state
                        post_internal_event(avoid_state->common_data->state_machine, EVENT_FINISHED);
                    } else {
                        start_avoid_move(avoid_state);
                    }
                    break;

                case EVENT_OBJ:
                    if (avoid_state->move_index != 0) {
                        avoid_state_run(avoid_state); // TODO: PROBLEM HERE, KEEPS REVERSING
                    }
                    break;

                case EVENT_NONE: // DO NOTHING
                    break;

                default:
                    while(1) Serial.println("in avd, evnt !fnd");
                    break;
            }
            break;

        case STATE_RETREAT:
            while(1) Serial.println("in avd, from ret");
            break;

        default:
            while(1) Serial.println("in avd, stat not fnd");
            break;
    }
}
void avoid_state_run(struct Avoid_State *avoid_state){
    avoid_state->move_index = 0;
    avoid_state->internal_avoid_state = next_avoid_state(avoid_state);
    start_avoid_move(avoid_state);
}

void avoid_state_init(struct Avoid_State *avoid_state){
    avoid_state->internal_avoid_state = AVOID_STATE_180_FLIP;
    avoid_state->move_index = 0;
}

// ----------------------------------   RETREAT STATE   ------------------------------ //

void retreat_state_init(struct Retreat_State *retreat_state){

}