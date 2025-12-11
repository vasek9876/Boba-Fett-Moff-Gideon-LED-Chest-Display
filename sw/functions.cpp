#include "functions.h"

void call_function(const byte *pattern, int i) {
  led_off();
  lcd_write(0, pattern[0]);
  bar_select(i++);
  lcd_write(1, pattern[1]);
  bar_select(i++);
  lcd_write(2, pattern[2]);
  bar_select(i++);
  lcd_write(3, pattern[3]);
  bar_select(i++);
  lcd_write(4, pattern[4]);
  bar_select(i++);
}


void call_bare_function(const byte *pattern) {
  lcd_write(0, pattern[0]);
  lcd_write(1, pattern[1]);
  lcd_write(2, pattern[2]);
  lcd_write(3, pattern[3]);
  lcd_write(4, pattern[4]);
}
void lcd_write(int seg, byte pattern) {
  // vypnutí všech segmentových výstupů
  LCD_PORT &= ~LCD_MASK;

  // vypnutí všech LED bitů
  PORTD = 0;
  //delay_cycles(1);

  // aktivuj jen požadovaný segment
  LCD_PORT |= (1 << (seg + 1));

  // po jeho aktivaci nastav vzor
  PORTD = pattern;
  delay_cycles(1);

  // po zápisu segment opět vypni
  LCD_PORT &= ~LCD_MASK;
  PORTD = 0;
}

void bar_select(uint8_t channel) {
  // if (channel > 15) return;

  BAR_EN_PORT |= (1 << BAR_EN_BIT);

  // clear all 4 bits first
  BAR_PORT &= ~BAR_MASK;

  // set bits according to lookup
  for (uint8_t i = 0; i < 4; i++) {
    if (bar_map[channel][i]) {
      BAR_PORT |= barBits[i];
    }
  }
  BAR_EN_PORT &= ~(1 << BAR_EN_BIT);
}


// multiplex pro více kanálů
void bar_multiplex(uint8_t max_channel) {
    for (uint8_t ch = 0; ch <= max_channel; ch++) {
        bar_select(ch);
        //_delay_ms(2); // krátké zpoždění, aby multiplexování bylo plynulé
    }
}

void bar_test() {
    for (uint8_t step = 0; step < 16; step++) {
        // postupně rozsvěcujeme LEDky
        for (uint8_t i = 0; i < 200; i++) {  // loop pro blikání, aby se stalo plynule
            bar_multiplex(step);
            
        }
        //delay(100);
    }
    // nakonec všechny zhasnout
    for (uint8_t i = 0; i < 50; i++) {
        bar_multiplex(0); // nebo BAR_PORT &= ~BAR_MASK
    }
    //DISABLE
    BAR_EN_PORT |= (1 << BAR_EN_BIT);
}


void delay_cycles(uint16_t cycles) {
    // Convert cycles to microseconds (approx)

    set_sleep_mode(SLEEP_MODE_IDLE);  // idle keeps timers running

    sleep_enable();
    sleep_cpu();  // CPU sleeps but timer keeps running
    sleep_disable();
}

void led_on()  { PORTB |=  (1 << LED_PIN); }
void led_off() { PORTB &= ~(1 << LED_PIN); }