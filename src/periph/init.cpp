#include "init.h"

void mc_init(void){
    Serial.begin(9600);
    Serial.println("Microcontroller Init");
    sei();
    pinMode(A5, OUTPUT);
    digitalWrite(A5, LOW);
}