#include "bod_detect.h"

// najde první volný záznam (2 bity)
uint16_t find_pos() {
    for (uint16_t i = 0; i < EEPROM_SIZE * 4; i++) { // 4 záznamy/bajt
        uint16_t byte_i = i / 4;
        uint8_t shift = (i % 4) * 2;
        uint8_t b = (eeprom_read_byte((uint8_t*)byte_i) >> shift) & 0b11;
        if (b == 0b11) return i;
    }
    return EEPROM_SIZE*4; // plná
}

void erase_all() {
    for (uint16_t i = 0; i < EEPROM_SIZE; i++)
        eeprom_write_byte((uint8_t*)i, 0xFF);
}

void write_flag(uint8_t flag) {
    uint16_t pos = find_pos();
    if (pos >= EEPROM_SIZE*4) { erase_all(); pos = 0; }

    uint16_t byte_i = pos / 4;
    uint8_t shift = (pos % 4) * 2;

    uint8_t b = eeprom_read_byte((uint8_t*)byte_i);
    uint8_t val = (flag ? 0b10 : 0b00) << shift;
    uint8_t mask = ~(0b11 << shift);

    eeprom_write_byte((uint8_t*)byte_i, (b & mask) | val);
}

uint8_t load_last() {
    uint16_t pos = find_pos();
    if (pos == 0) return 0xFF;

    pos--;
    uint16_t byte_i = pos / 4;
    uint8_t shift = (pos % 4) * 2;

    uint8_t b = eeprom_read_byte((uint8_t*)byte_i);
    uint8_t val = (b >> shift) & 0b11;
    return (val == 0b10) ? 1 : 0;
}