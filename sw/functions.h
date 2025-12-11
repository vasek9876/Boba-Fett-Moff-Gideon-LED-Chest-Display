#ifndef functions_h
#define functions_h

#include <Arduino.h>
#include <avr/sleep.h>
#include <avr/power.h>


#include "defaults.h"

void call_function(const byte *pattern, int i);
void call_bare_function(const byte *pattern);
void lcd_write(int seg, byte pattern);
void bar_select(uint8_t channel);
void delay_cycles(uint16_t cycles);
void bar_test();


void led_on();
void led_off();
#endif