#include <Arduino.h>
#include <periph/init.h>
#include <periph/io.h>
#include <periph/i2c.h>
#include <periph/timer.h>
#include <periph/pwm.h>
#include <app/obj.h>
#include <app/line.h>
#include <app/state_machine.h>

uint16_t range = 0;
struct Timer timer = {.time = 0, .timeout = 0};
struct State_Machine state_machine;

// ------- TEST ------ //
void test_function(){}
// ------------------- //

void init_app(){
    init_timer();
    i2c_init();
    pwm_init();
    lines_init();
    state_machine_init(&state_machine, &timer);
}

void setup() {
  mc_init();
  init_app();
  timer_set_timeout(&timer, 100);
}

void loop() {
    uint32_t time1 = get_time();
    timer_tick(&timer, time1);
    
    bool can_read_1 = read_range(LOC1_ADDR, &range);
    if(can_read_1){
       state_machine_run(&state_machine, range);
    }else{
        while(1) Serial.println("VL5 ERR");
    }
}

/*
PRINT LINE
switch (line_pos)
    {
    case LINE_NONE:
        Serial.println("LINE NONE");
        break;
    case LINE_FRONT:
        Serial.println("LINE FRONT");
        break;
    case LINE_BACK:
        Serial.println("LINE BACK");
        break;
    case LINE_LEFT:
        Serial.println("LINE LEFT");
        break;
    case LINE_RIGHT:
        Serial.println("LINE RIGHT");
        break;
    case LINE_FRONT_LEFT:
        Serial.println("LINE FRONT LEFT");
        break;
    case LINE_FRONT_RIGHT:
        Serial.println("LINE FRONT RIGHT");
        break;
    case LINE_BACK_LEFT:
        Serial.println("LINE BACK LEFT");
        break;
    case LINE_BACK_RIGHT:
        Serial.println("LINE BACK RIGHT");
        break;
    default:
        Serial.println("UNKNOWN LINE POSITION");
        break;
    }
*/
