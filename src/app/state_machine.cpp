#include "app/state_machine.h"

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

static const Scene_Transition transitions[] = {
    {.from_state = STATE_DRIVE,   .event = EVENT_NONE,        .to_state = STATE_DRIVE},
    {.from_state = STATE_DRIVE,   .event = EVENT_TIMEOUT,     .to_state = STATE_DRIVE},
    {.from_state = STATE_DRIVE,   .event = EVENT_LINE,        .to_state = STATE_RETREAT},
    {.from_state = STATE_DRIVE,   .event = EVENT_OBJ,         .to_state = STATE_AVOID},

    {.from_state = STATE_AVOID,   .event = EVENT_OBJ,         .to_state = STATE_AVOID},
    {.from_state = STATE_AVOID,   .event = EVENT_LINE,        .to_state = STATE_RETREAT},
    {.from_state = STATE_AVOID,   .event = EVENT_TIMEOUT,     .to_state = STATE_AVOID},
    {.from_state = STATE_AVOID,   .event = EVENT_FINISHED,    .to_state = STATE_DRIVE},
    {.from_state = STATE_AVOID,   .event = EVENT_NONE,        .to_state = STATE_AVOID},

    {.from_state = STATE_RETREAT, .event = EVENT_LINE,        .to_state = STATE_RETREAT},
    {.from_state = STATE_RETREAT, .event = EVENT_TIMEOUT,     .to_state = STATE_RETREAT},
    {.from_state = STATE_RETREAT, .event = EVENT_OBJ,         .to_state = STATE_RETREAT},
    {.from_state = STATE_RETREAT, .event = EVENT_FINISHED,    .to_state = STATE_DRIVE},
    {.from_state = STATE_RETREAT, .event = EVENT_NONE,        .to_state = STATE_RETREAT},
};

void state_machine_init(struct State_Machine *state_machine, Timer *timer){
    state_machine->state = STATE_DRIVE;
    state_machine->timer = timer;
    state_machine->common_data.state_machine = state_machine;
    state_machine->common_data.obj.range = OBJ_RANGE_NONE;
    state_machine->common_data.line = LINE_NONE;
    state_machine->common_data.timer = state_machine->timer;
    state_machine->internal_event = EVENT_NONE;

    state_machine->drive_state.common_data = &state_machine->common_data;
    state_machine->avoid_state.common_data = &state_machine->common_data;
    state_machine->retreat_state.common_data = &state_machine->common_data;
    drive_state_init(&state_machine->drive_state);
    avoid_state_init(&state_machine->avoid_state);
    retreat_state_init(&state_machine->retreat_state);
    Serial.println("SM init");
}

void post_internal_event(struct State_Machine *state_machine, Event event){
    state_machine->internal_event = event;
}

static bool has_internal_event(struct State_Machine *state_machine) {
    return state_machine->internal_event != EVENT_NONE;
}

static Event take_internal_event(struct State_Machine *state_machine){
    if (!has_internal_event(state_machine))
        while(1) Serial.println("no inter");
    Event event = state_machine->internal_event;
    state_machine->internal_event = EVENT_NONE;
    return event; 
}

static Event state_machine_process_input(State_Machine *state_machine, uint16_t range){
    state_machine->common_data.obj.range = get_object_range(range);
    state_machine->common_data.line = get_lines();

    if(has_internal_event(state_machine)){
        return take_internal_event(state_machine);
    }else if (timer_timed_out(state_machine->timer)){
        return EVENT_TIMEOUT;
    }else if(state_machine->common_data.line != LINE_NONE){
        return EVENT_LINE;
    }else if(obj_detected(state_machine->common_data.obj.range)){
        return EVENT_OBJ;
    }
    return EVENT_NONE;
}

static void state_machine_enter(State_Machine *state_machine, State from_state, Event event, State to_state){
    if(from_state != to_state){
        timer_hard_reset(state_machine->timer);
        state_machine->state = to_state;
    }
    if(to_state == STATE_DRIVE){
        drive_state_enter(&state_machine->drive_state, from_state, event);
        return;
    }else if(to_state == STATE_AVOID){
        avoid_state_enter(&state_machine->avoid_state, from_state, event);
        return; 
    }else if(to_state == STATE_RETREAT){
        retreat_state_enter(&state_machine->retreat_state, from_state, event);
        return; 
    }
}

static void state_machine_process_event(State_Machine *state_machine, Event event) {
    for (uint16_t i = 0; i < ARRAY_SIZE(transitions); i++) {
        if (transitions[i].from_state == state_machine->state && transitions[i].event == event) {
            state_machine_enter(state_machine, transitions[i].from_state, event, transitions[i].to_state);
            return;
        }
    }
}

void state_machine_run(struct State_Machine *state_machine, uint16_t range){
    Event event = state_machine_process_input(state_machine, range);
    state_machine_process_event(state_machine, event);
    switch(state_machine->state){
        case STATE_DRIVE:
            Serial.println("DR");
            break;
        case STATE_AVOID:
            Serial.println("AVD");
            Serial.println(state_machine->avoid_state.move_index);

            break;
        case STATE_RETREAT:
            Serial.println(state_machine->retreat_state.move_index);
            Serial.println("RET");
            switch(state_machine->retreat_state.internal_retreat_state){
                case RETREAT_STATE_REVERSE: Serial.println("ret rev");
                    break;
                case RETREAT_STATE_FORWARD: Serial.println("ret fwd");
                    break;
                case RETREAT_STATE_REVERSE_FLIP_LEFT: Serial.println("rev flp L");
                    break;
                case RETREAT_STATE_REVERSE_FLIP_RIGHT: Serial.println("rev flp R");
                    break;
                case RETREAT_STATE_HALF_TURN_LEFT: Serial.println("hlf L");
                    break;
                case RETREAT_STATE_HALF_TURN_RIGHT: Serial.println("hlf R");
                    break;
                case RETREAT_STATE_ALIGN_LEFT: Serial.println("algn L");
                    break;
                case RETREAT_STATE_ALIGN_RIGHT: Serial.println("algn R");
                    break;
            }
            break;
    }
}
/*
            switch(state_machine->drive_state.internal_drive_state){
                case DRIVE_STATE_FORWARD:
                    Serial.println("drv fwd");
                    break;
                case DRIVE_STATE_ROTATE_LEFT:
                    Serial.println("drv rot L");
                    break;
                case DRIVE_STATE_ROTATE_RIGHT:
                    Serial.println("drv rot R");
                    break;
            }

            switch(state_machine->avoid_state.internal_avoid_state){
                case AVOID_STATE_180_FLIP:
                    Serial.println("avd 180");
                break;
                case AVOID_STATE_ALIGN_LEFT:
                    Serial.println("avd algn L");
                    break;
                case AVOID_STATE_ALIGN_RIGHT:
                    Serial.println("avd algn L");
                    break;
                case AVOID_STATE_REVERSE:
                    Serial.println("avd rev");
                    break;
            }


*/