#include "line.h"
#define LINE_THRESHOLD 700

void lines_init(void){
    adc_init();
    Serial.println('lines inited');
}  

Line_Pos get_lines(){
    adc_input_values adc_vals;
    get_adc_input_values(adc_vals); // adc_vals[2] test
    
    const bool front_left = adc_vals[0] < LINE_THRESHOLD;
    const bool front_right = adc_vals[1] < LINE_THRESHOLD;
    const bool back_left = adc_vals[2] < LINE_THRESHOLD;
    const bool back_right = adc_vals[3] < LINE_THRESHOLD;
    // 4 = FRONT LEFT 
    // 1 = FRONT RIGHT
    // 2 = BACK LEFT
    // 3 = BACK RIGHT

    if(front_left){
        if(front_right) return LINE_FRONT;
        else if(back_left) return LINE_LEFT;
        else return LINE_FRONT_LEFT;
    } else if(front_right){
        if(back_right)return LINE_RIGHT;
        return LINE_FRONT_RIGHT;
    } else if(back_left){
        if(back_right) return LINE_BACK;
        return LINE_BACK_LEFT;
    }else if(back_right){
        if(back_left) return LINE_BACK;
        return LINE_BACK_RIGHT;
    }
    return LINE_NONE;
}

/*

*/