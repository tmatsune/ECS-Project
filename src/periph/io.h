#ifndef IO_H
#define IO_H
#include <Arduino.h>

typedef enum
{
    ZERO = 0, // Arduino digital pin 0 (RX)
    ONE = 1,  // Arduino digital pin 1 (TX)
    TWO = 2,
    THREE = 3,
    FOUR = 4,
    FIVE = 5,
    SIX = 6,
    SEVEN = 7,
    EIGHT = 8,
    NINE = 9,
    TEN = 10,
    ELEVEN = 11,
    TWELVE = 12,
    THIRTEEN = 13,
    A_ZERO = A0, // Arduino Analog pin A0
    A_ONE = A1,
    A_TWO = A2,
    A_THREE = A3,
    A_FOUR = A4, // Also I2C SDA
    A_FIVE = A5, // Also I2C SCL
    A_SIX = A6,
    A_SEVEN = A7,
} io_pins;

typedef enum {
    IO_DIR_IN = INPUT,
    IO_DIR_OUT = OUTPUT
} io_dir;
typedef enum {
    IO_STATE_LOW = LOW,
    IO_STATE_ON = HIGH,
} io_state;

struct io_config {
    int direction;
    int state;
};

typedef enum {
    RISING_EDGE,
    FALLING_EDGE,
} io_trigger;

void io_configure_pins(void);

typedef void (*isr_function)(void);
void config_interrupt_trigger(io_pins pin, io_trigger trigger);
void register_isr(io_pins pin, isr_function isr);
void unregiser_isr(io_pins pin);
void enable_interrupt(byte pin);
void disable_interrupt(byte pin);

#endif