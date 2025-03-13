#include "pwm.h"
#include "utils/assert_handler.h"

#define LEFT_PWM_PIN 11
#define RIGHT_PWM_PIN 3
// 11 = A 
// 12 & 13

// PINS 12 & 13 for LEFT_MOTOR
#define LEFT_MOTOR_PIN_1 12 // A2
#define LEFT_MOTOR_PIN_2 13 // A1

// PINS 8 & 9 for RIGHT MOTOR
#define RIGHT_MOTOR_PIN_1 8 // B1
#define RIGHT_MOTOR_PIN_2 9 // B2

//-------------------------------------------------PWM-------------------------------------------------//
static int left_duty_cycle_percent = 0;
static int right_duty_cycle_percent = 0;

void pwm_init(void){
    pinMode(LEFT_PWM_PIN, OUTPUT);
    pinMode(RIGHT_PWM_PIN, OUTPUT);

    TCCR2A = (1 << WGM21) | (1 << WGM20);       // Fast PWM mode
    TCCR2A |= (1 << COM2A1) | (1 << COM2B1);    // Clear OC2A and OC2B on Compare Match
    TIMSK2 = (1 << TOIE2);
    TCCR2B = (1 << CS21);                       // Prescaler = 8 (~7.8kHz PWM on 16MHz clock

    OCR2A = 0;                                  // OC2RA IS TIMER COMPARE MATCH FOR PIN 11(PB3)
    OCR2B = 0;                                  // OC2RB IS TIMER COMPARE MATCH FOR PIN 3(PD3)

    pinMode(RIGHT_MOTOR_PIN_1, OUTPUT);
    pinMode(RIGHT_MOTOR_PIN_2, OUTPUT);
    digitalWrite(RIGHT_MOTOR_PIN_1, LOW);
    digitalWrite(RIGHT_MOTOR_PIN_2, LOW);

    pinMode(LEFT_MOTOR_PIN_1, OUTPUT);
    pinMode(LEFT_MOTOR_PIN_2, OUTPUT);
    digitalWrite(LEFT_MOTOR_PIN_1, LOW);
    digitalWrite(LEFT_MOTOR_PIN_2, LOW);
}


ISR(TIMER2_OVF_vect){
    OCR2A = left_duty_cycle_percent;
    OCR2B = right_duty_cycle_percent;
}

void pwm_set_duty_cycle(pwm pwm_side, int duty_cycle){
    switch (pwm_side)
    {
    case LEFT_PWM:
        left_duty_cycle_percent = duty_cycle;
        break;
    case RIGHT_PWM:
        right_duty_cycle_percent = duty_cycle;
        break;
    default:
        ASSERT(0, "duty_cycle");
        break;
    }
}
//---------------------------------------------- MOTOR DRIVER --------------------------------------------//
  
struct motor {
    volatile uint8_t pin_1;
    volatile uint8_t pin_2;
};

static struct motor motors[] = {
    [LEFT_MOTOR] = {
        .pin_1 = LEFT_MOTOR_PIN_1,
        .pin_2 = LEFT_MOTOR_PIN_2,
    },
    [RIGHT_MOTOR] = {
        .pin_1 = RIGHT_MOTOR_PIN_1,
        .pin_2 = RIGHT_MOTOR_PIN_2,
    }
};

void motor_driver_set_pwm(motor_driver driver, int duty_cycle) { 
    pwm_set_duty_cycle((pwm)driver, duty_cycle); // FIX THIS
}

void motor_driver_set_mode(motor_driver driver, motor_driver_mode mode){
    switch(driver){
        case LEFT_MOTOR:
            switch(mode){
                case FORWARD:
                    digitalWrite(motors[LEFT_MOTOR].pin_1, LOW);
                    digitalWrite(motors[LEFT_MOTOR].pin_2, HIGH);
                    break;
                case REVERSE:
                    digitalWrite(motors[LEFT_MOTOR].pin_1, HIGH);
                    digitalWrite(motors[LEFT_MOTOR].pin_2, LOW);
                    break;
                case STOP:
                    digitalWrite(motors[LEFT_MOTOR].pin_1, LOW);
                    digitalWrite(motors[LEFT_MOTOR].pin_2, LOW);
                    break;
                default:
                break;
            }
            break;
        case RIGHT_MOTOR:
            switch(mode){
                case FORWARD:
                    digitalWrite(motors[RIGHT_MOTOR].pin_1, LOW);
                    digitalWrite(motors[RIGHT_MOTOR].pin_2, HIGH);
                    break;
                case REVERSE:
                    digitalWrite(motors[RIGHT_MOTOR].pin_1, HIGH);
                    digitalWrite(motors[RIGHT_MOTOR].pin_2, LOW);
                    break;
                case STOP:
                    digitalWrite(motors[RIGHT_MOTOR].pin_1, LOW);
                    digitalWrite(motors[RIGHT_MOTOR].pin_2, LOW);
                    break;
                default:
                break;
            }
            break;
        default:
            ASSERT(0, "ERR MOTOR DRIVER SET MODE");
            break;
    }
}

//-------------------------------------------------- DRIVE ----------------------------------------------//
struct drive_setting
{
    int16_t left;  // LEFT_MOTOR
    int16_t right; // RIGHT_MOTOR
};

static const struct drive_setting drive_settings[][4] = {
    [DRIVE_DIR_FORWARD] = {
        [DRIVE_SPEED_SLOW] = {.left = 20, .right = 20},
        [DRIVE_SPEED_MEDIUM] = {.left = 100, .right = 100},
        [DRIVE_SPEED_FAST] = {.left = 150, .right = 150},
        [DRIVE_SPEED_MAX] = {.left = 254, .right = 254},
    },
    [DRIVE_DIR_REVERSE] = {
        [DRIVE_SPEED_SLOW] = {.left = -20, .right = -20},
        [DRIVE_SPEED_MEDIUM] = {.left = -100, .right = -100},
        [DRIVE_SPEED_FAST] = {.left = -150, .right = -150},
        [DRIVE_SPEED_MAX] = {.left = -255, .right = -254},
    },
    [DRIVE_DIR_ROTATE_LEFT] = {
        [DRIVE_SPEED_SLOW] = {.left = -20, .right = 20},
        [DRIVE_SPEED_MEDIUM] = {.left = -100, .right = 100},
        [DRIVE_SPEED_FAST] = {.left = -150, .right = 150},
        [DRIVE_SPEED_MAX] = {.left = -254, .right = 254},
    },
    [DRIVE_DIR_ROTATE_RIGHT] = {
        [DRIVE_SPEED_SLOW] = {.left = 25, .right = -25},
        [DRIVE_SPEED_MEDIUM] = {.left = 100, .right = -100},
        [DRIVE_SPEED_FAST] = {.left = 150, .right = -150},
        [DRIVE_SPEED_MAX] = {.left = 254, .right = -254},
    },
    [DRIVE_DIR_SHARP_LEFT] = {
        [DRIVE_SPEED_SLOW] = {.left = -0, .right = 50},
        [DRIVE_SPEED_MEDIUM] = {.left = -10, .right = 50},
        [DRIVE_SPEED_FAST] = {.left = -25, .right = 75},
        [DRIVE_SPEED_MAX] = {.left = 50, .right = 254},
    },
    [DRIVE_DIR_SHARP_RIGHT] = {
        [DRIVE_SPEED_SLOW] = {.left = 25, .right = -0},
        [DRIVE_SPEED_MEDIUM] = {.left = 50, .right = -10},
        [DRIVE_SPEED_FAST] = {.left = 75, .right = -25},
        [DRIVE_SPEED_MAX] = {.left = 254, .right = 50},
    },
    [DRIVE_DIR_MID_LEFT] = {
        [DRIVE_SPEED_SLOW] = {.left = 15, .right = 30},
        [DRIVE_SPEED_MEDIUM] = {.left = 25, .right = 50},
        [DRIVE_SPEED_FAST] = {.left = 35, .right = 70},
        [DRIVE_SPEED_MAX] = {.left = 128, .right = 254},
    },
    [DRIVE_DIR_MID_RIGHT] = {
        [DRIVE_SPEED_SLOW] = {.left = 30, .right = 15},
        [DRIVE_SPEED_MEDIUM] = {.left = 50, .right = 25},
        [DRIVE_SPEED_FAST] = {.left = 70, .right = 35},
        [DRIVE_SPEED_MAX] = {.left = 254, .right = 128},
    },
    [DRIVE_DIR_WIDE_LEFT] = {
        [DRIVE_SPEED_SLOW] = {.left = 20, .right = 25},
        [DRIVE_SPEED_MEDIUM] = {.left = 40, .right = 50},
        [DRIVE_SPEED_FAST] = {.left = 60, .right = 70},
        [DRIVE_SPEED_MAX] = {.left = 200, .right = 254},
    },
    [DRIVE_DIR_WIDE_RIGHT] = {
        [DRIVE_SPEED_SLOW] = {.left = 25, .right = 20},
        [DRIVE_SPEED_MEDIUM] = {.left = 50, .right = 40},
        [DRIVE_SPEED_FAST] = {.left = 70, .right = 60},
        [DRIVE_SPEED_MAX] = {.left = 254, .right = 200},
    },
};

void drive_stop(void){
    left_duty_cycle_percent = 0;
    right_duty_cycle_percent = 0;
    motor_driver_set_mode(LEFT_MOTOR, STOP);
    motor_driver_set_mode(RIGHT_MOTOR, STOP);
}

void drive_set(drive_dir dir, drive_speed speed){
    int16_t left_motor_speed = drive_settings[dir][speed].left;
    int16_t right_motor_speed = drive_settings[dir][speed].right;
    motor_driver_mode left_mode = left_motor_speed > 0 ? FORWARD : REVERSE;
    motor_driver_mode right_mode = right_motor_speed > 0 ? FORWARD : REVERSE;
    if (left_motor_speed < 0)
    {
        left_motor_speed *= -1;
    }
    if (right_motor_speed < 0)
    {
        right_motor_speed *= -1;
    }
    motor_driver_set_pwm(LEFT_MOTOR, left_motor_speed);
    motor_driver_set_pwm(RIGHT_MOTOR, right_motor_speed);
    motor_driver_set_mode(LEFT_MOTOR, left_mode);
    motor_driver_set_mode(RIGHT_MOTOR, right_mode);
}