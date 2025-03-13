#ifndef PWM_H
#define PWM_H
#include <Arduino.h>
#include <stdint.h>

typedef enum
{
    LEFT_PWM,
    RIGHT_PWM,
} pwm;

typedef enum
{
    LEFT_MOTOR,
    RIGHT_MOTOR,
} motor_driver;

typedef enum
{
    FORWARD,
    REVERSE,
    STOP,
} motor_driver_mode;

typedef enum
{
    DRIVE_DIR_FORWARD,
    DRIVE_DIR_REVERSE,
    DRIVE_DIR_ROTATE_LEFT,
    DRIVE_DIR_ROTATE_RIGHT,
    DRIVE_DIR_SHARP_LEFT,
    DRIVE_DIR_SHARP_RIGHT,
    DRIVE_DIR_MID_LEFT,
    DRIVE_DIR_MID_RIGHT,
    DRIVE_DIR_WIDE_LEFT,
    DRIVE_DIR_WIDE_RIGHT,
    DRIVE_DIR_FLIP_LEFT,
    DRIVE_DIR_FLIP_RIGHT,
    DRIVE_DIR_NONE,
} drive_dir;

typedef enum
{
    DRIVE_SPEED_SLOW,
    DRIVE_SPEED_MEDIUM,
    DRIVE_SPEED_FAST,
    DRIVE_SPEED_MAX
} drive_speed;

// ----------------- PWM LAYER ---------------- //
void pwm_init(void);
void pwm_set_duty_cycle(pwm pwm_side, int duty_cycle);

// --------------- MOTOR DRIVER -------------- //
void motor_driver_set_mode(motor_driver driver, motor_driver_mode mode);
void motor_driver_set_pwm(motor_driver driver, int duty_cycle);

// --------------- DRIVE INTERFACE ----------- //
void drive_stop(void);
void drive_set(drive_dir direction, drive_speed speed);

#endif