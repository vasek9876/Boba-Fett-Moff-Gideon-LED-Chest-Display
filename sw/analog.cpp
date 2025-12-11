#include "analog.h"

#ifndef OLD  // current board cannot measure !!! - there is error in pinout - using PE0/1 for ADC but ADCS are PE2 and PE3


void analogEN() {
    // Set PE3 HIGH (Enable)
    PORTE |= (1 << MEASURE_EN_BIT); 
}

void analogDIS() {
    // Set PE3 LOW (Disable)
    PORTE &= ~(1 << MEASURE_EN_BIT); 
}

uint16_t readADC() {
    return analogRead(ADC_PIN);
}

float readVoltage() {
    analogEN();
    delay(100);

    uint32_t sum = 0;

    // oversampling 16x
    for (uint8_t i = 0; i < 16; i++) {
        sum += readADC();   // one raw ADC sample (0–1023)
        delay(2);           // light settling delay
    }

    analogDIS();

    float avg = sum / 16.0f;       // average raw value
    return avg / 1023.0f;          // 0.0–1.0 normalized
}

//#define MEASURE_POT // not used yet
#endif