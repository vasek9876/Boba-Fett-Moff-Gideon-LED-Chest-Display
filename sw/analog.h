#ifndef analog_h
#define analog_h

#include <avr/io.h>
#include <Arduino.h>

#ifndef OLD  // current board cannot measure !!! - there is error in pinout - using PE0/1 for ADC but ADCS are PE2 and PE3

#define VREF 5.0
#define ADC 1023.0
#define DIVIDE 2
#define OFFSET 0.0
#define SCALE ADC/VREF

// ATmega direct register definitions for PORTE
#define PORTE (*(volatile uint8_t *)0x2E)
#define DDRE  (*(volatile uint8_t *)0x2D)
#define PINE  (*(volatile uint8_t *)0x2C)

#define ADC_PIN A0
#define MEASURE_EN_BIT 3  // PE3 physical pin

void analogEN();
void analogDIS();
float readVoltage();
uint16_t readADC();

#endif

#endif
