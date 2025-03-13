#include "adc.h"

#define NUM_CHANNELS 4                                      // Number of ADC channels to sample
volatile uint16_t adc_values[NUM_CHANNELS];                      // Store ADC results
volatile uint8_t current_channel = 0;                       // Track ADC channel 

void adc_init(){
    ADMUX = (1 << REFS0);                                   // AVcc as reference, start with A0
    ADCSRA = (1 << ADEN) |                                  // Enable ADC
             (1 << ADATE) |                                 // Auto trigger enable
             (1 << ADIE) |                                  // Enable ADC interrupt
             (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);    // Prescaler = 128 (125kHz ADC clock)

    ADCSRB = 0;                                             // Free-running mode
    ADCSRA |= (1 << ADSC);                                  // Start ADC conversion
}

void get_adc_input_values(adc_input_values vals){
    vals[0] = adc_values[0];
    vals[1] = adc_values[1];
    vals[2] = adc_values[2];
    vals[3] = adc_values[3];
}

ISR(ADC_vect) {
    adc_values[current_channel] = ADC;                      // Store ADC result
    current_channel++;  
    if (current_channel >= NUM_CHANNELS) {
        current_channel = 0;                                // Reset channel index
    }

    ADMUX = (ADMUX & 0xF8) | current_channel;               // Switch ADC channel
    ADCSRA |= (1 << ADSC);                                  // Start new conversion
}