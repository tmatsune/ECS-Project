#include "io.h"
#define PORT_COUNT (3u)
#define ISR_PIN_COUNT (2u)

typedef enum {
    INTERRUPT_PIN_2,
    INTERRUPT_PIN_3
} Interrupt_Pin;

static isr_function interrupt_functions[] = {
    [INTERRUPT_PIN_2] = NULL,
    [INTERRUPT_PIN_3] = NULL
};

static const struct io_config io_initial_config[] = {
    [ZERO] = {OUTPUT, LOW},     // Pin 0
    [ONE] = {OUTPUT, LOW},      // Pin 1
    [TWO] = {OUTPUT, LOW},      // Pin 2
    [THREE] = {OUTPUT, LOW},    // Pin 3
    [FOUR] = {OUTPUT, LOW},     // Pin 4
    [FIVE] = {OUTPUT, LOW},     // Pin 5
    [SIX] = {OUTPUT, LOW},      // Pin 6
    [SEVEN] = {OUTPUT, LOW},    // Pin 7
    [EIGHT] = {OUTPUT, LOW},    // Pin 8
    [NINE] = {OUTPUT, LOW},     // Pin 9
    [TEN] = {OUTPUT, LOW},      // Pin 10
    [ELEVEN] = {OUTPUT, LOW},   // Pin 11
    [TWELVE] = {OUTPUT, LOW},   // Pin 12
    [THIRTEEN] = {OUTPUT, LOW}, // Pin 13
    [A_ZERO] = {INPUT, LOW},    // Analog pin A0
    [A_ONE] = {INPUT, LOW},     // Analog pin A1
    [A_TWO] = {INPUT, LOW},     // Analog pin A2
    [A_THREE] = {INPUT, LOW},   // Analog pin A3
    [A_FOUR] = {INPUT, LOW},    // Analog pin A4 (I2C SDA)
    [A_FIVE] = {INPUT, LOW},    // Analog pin A5 (I2C SCL)
    [A_SIX] = {INPUT, LOW},     // Analog pin A6
    [A_SEVEN] = {INPUT, LOW},   // Analog pin A7
};

void config_interrupt_trigger(io_pins pin, io_trigger trigger){
    switch(pin){
        case TWO:
            switch(trigger){
                case FALLING_EDGE:
                    EICRA = (0 << ISC00) | (1 << ISC01);
                    break;
                case RISING_EDGE:
                    EICRA |= (1 << ISC01) | (1 << ISC00);
                    break;
                default:
                    break;
            }
            break;
        case THREE:
            switch(trigger){
                case FALLING_EDGE:
                    EICRA |= (0 << ISC10) | (1 << ISC11);
                    break;
                case RISING_EDGE:
                    EICRA |= (1 << ISC10) | (1 << ISC11);
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}
void register_isr(io_pins pin, isr_function isr){
    switch (pin)
    {
    case TWO:
        interrupt_functions[INTERRUPT_PIN_2] = isr;
        break;
    case THREE:
        interrupt_functions[INTERRUPT_PIN_3] = isr;
    default:
        break;
    }
}
void unregiser_isr(io_pins pin){
    switch (pin)
    {
    case TWO:
        interrupt_functions[INTERRUPT_PIN_2] = NULL;
        break;
    case THREE:
        interrupt_functions[INTERRUPT_PIN_3] = NULL;
    default:
        break;
    }
}
void enable_interrupt(byte pin){
    EIMSK |= (1 << pin);
}
void disable_interrupt(byte pin){
    EIMSK |= (0 << pin);
}



