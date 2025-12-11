#ifndef bod_h
#define bod_h

#include <avr/io.h>
#include <avr/eeprom.h>

#define EEPROM_SIZE 1024  // B

uint8_t load_last(); // loads true or false - last reset state - if it was correct or not
void write_flag(uint8_t flag); // writes true when the init goes well

#endif