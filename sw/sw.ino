#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/io.h>

#include "defaults.h"

#define NOP __asm__ __volatile__ ("nop\n\t") // Define the NOP instruction


#define LED_PIN PB5
void led_on()  { PORTB |=  (1 << LED_PIN); }
void led_off() { PORTB &= ~(1 << LED_PIN); }
// LCD segments on PORTC bits 5..1
// const int lcd_addr[] = { A5, A4, A3, A2, A1 };  // Segments: PC5,PC4,PC3,PC2,PC1
#define LCD_PORT PORTC
#define LCD_DDR DDRC
#define LCD_MASK ((1 << PC5) | (1 << PC4) | (1 << PC3) | (1 << PC2) | (1 << PC1))

#define NUM_SEG 5  // Number of LCD segments

// Bar LEDs on PORTB bits 0,2,3,1 (in that order)
// const int bar_leds[] = { 8, 10, 11, 9 };  // PB0, PB2, PB3, PB1
#define BAR_PORT PORTB
#define BAR_DDR DDRB
#define BAR_MASK ((1 << PB0) | (1 << PB2) | (1 << PB3) | (1 << PB1))

// map bits to actual PORTB bits
static const uint8_t barBits[4] = { (1 << PB0), (1 << PB2), (1 << PB3), (1 << PB1) };

// const int bar_leds_en = 12;  // PB4
#define BAR_EN_PORT PORTB
#define BAR_EN_DDR DDRB
#define BAR_EN_BIT PB4



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
    delay(200);

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

// const int default_led = LED_BUILTIN;  // Pin 13 (PB5)


bool bod_triggered = false;

// Reads the VCC voltage in millivolts (mV)
long readVcc() {
    // 1. Set the ADC reference to VCC and the input to the internal 1.1V Bandgap Reference
    // MUX[3:0] = 1110 (0b1110) for VBG on ATmega328P/PB
    // REFS0 = 1 (selects AVCC as reference)
    ADMUX = (1 << REFS0) | (1 << MUX3) | (1 << MUX2) | (1 << MUX1);
    
    // 2. Wait for Vref to settle (required after changing ADMUX)
    // The datasheet recommends a short wait, e.g., 200us
    // You may need to use a delay function appropriate for your environment
    delayMicroseconds(250); 
    
    // 3. Start the conversion
    ADCSRA |= (1 << ADSC); 
    
    // 4. Wait for the conversion to complete (ADSC bit clears when done)
    while (ADCSRA & (1 << ADSC));
    
    // 5. Calculate VCC in millivolts
    // VCC = (V_BG * 1024) / ADC_Result
    // V_BG is the nominal 1100mV (1.1V). Use 'long' for math stability.
    // The actual V_BG can vary (e.g., 1080mV to 1120mV) for better precision,
    // you would calibrate this 1100mV constant for your specific chip.
    return (1100UL * 1024) / ADC; 
}

void setup_bod_flag() {
    if (readVcc() <=1000){
      bod_triggered = true;
    }
    else 
    {
      bod_triggered = false;
    }
}


void getVoltageDigits(float v, int digits[5]) {
    // najdi nejvyšší možnou hodnotu, kterou chceme zobrazit

    // clamp input
    //if (v < 0)     v = 0;
    //if (v > maxVal) v = maxVal;

    int scaled = v*10000*2 + OFFSET*1000;//(int)(v / maxVal * 99999);

    digits[0] = (scaled / 1000) % 10;
    digits[1] = (scaled / 100)  % 10;
    digits[2] = (scaled / 10)   % 10;
    digits[3] =  scaled % 10;
    digits[4] = 0; 
}

void preloop() {
  float v;
  int digits[5];
  for (int n = 0; n < 5; n++) {
    v = readVoltage();
    getVoltageDigits(v, digits);

    for (int k = 0; k < 100; k++) {
      lcd_write(0, PATTERN_U);
      lcd_write(1, pattern_numbers[digits[3]]);
      lcd_write(2, pattern_numbers[digits[2]]);
      lcd_write(3, pattern_numbers[digits[1]]);
      lcd_write(4, pattern_numbers_DP[digits[0]]);
    }
  }
}

void setup() {

  // ----------------- NEW --------------------

  DDRB |= (1 << LED_PIN); // led setup as output

  // ADC MEASURE
  // 1. Set PC0 (ADC0) as input pin
  // --- ADC MĚŘENÍ ---
  // 1. Nastavte PC0 (ADC0) jako vstup (ruční nastavení DDRC)
  DDRC &= ~(1 << PC0); 

  // 2. Nastavte Referenci (AVcc) a kanál (ADC0)
  ADMUX = (1 << REFS0); 
  
  // 3. Povolte ADC, Prescaler 128 a vynulujte ADCSRB
  ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
  ADCSRB = 0; // K zamezení interních měření

  // Set PE3 as OUTPUT and initialize to LOW
  DDRE |= (1 << MEASURE_EN_BIT);  // PE3 as output
  PORTE &= ~(1 << MEASURE_EN_BIT);  // Initialize to LOW

  DDRD = 0xFF;
  LCD_DDR |= LCD_MASK;  // outputs
  BAR_DDR |= BAR_MASK;  // outputs
  BAR_EN_DDR |= (1 << BAR_EN_BIT);

  // Start with all off
  LCD_PORT &= ~LCD_MASK;
  BAR_PORT &= ~BAR_MASK;

  // ENABLE
  BAR_EN_PORT &= ~(1 << BAR_EN_BIT);

  // Configure Timer1 once
  TCCR1A = 0;            // Normal mode
  TCCR1B = (1 << CS10);  // Prescaler = 1

  preloop();

  ADCSRA &= ~(1 << ADEN); // disable ADC


    PRR = (1 << PRTWI) |    // Disable TWI (I2C)
          (1 << PRSPI) |    // Disable SPI
          (1 << PRUSART0) | // Disable USART (Serial)
          (1 << PRTIM2);   // Disable Timer 2
          //(1 << PRTIM0);    // Disable Timer 0 (Timer 1 is kept for LCD/Bar timing)
}

void loop() {
  setup_bod_flag();
  if (bod_triggered) {
    led_on();
  }
  else
  {
  for (int i = 0; i < REPEAT_FUNC1; i++) {
    call_function(pattern5, 9);
  }

  for (int i = 0; i < REPEAT_FUNC2; i++) {
    call_function(pattern2, 6);
  }

  for (int i = 0; i < REPEAT_FUNC3; i++) {
    call_function(pattern1, 11);
  }

  BAR_EN_PORT |= (1 << BAR_EN_BIT);
  for (int i = 0; i < REPEAT_FUNC4; i++) {
    call_bare_function(pattern4);
  }

  for (int i = 0; i < REPEAT_FUNC5; i++) {
    call_function(pattern2, 3);
  }

  for (int i = 0; i < REPEAT_FUNC6; i++) {
    call_function(pattern3, 0);
  }
  }
}

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

  NOP; NOP; NOP; NOP; NOP; 
  NOP; NOP; NOP; NOP; NOP;

  // vypnutí všech LED bitů
  PORTD = 0;
  //delay_cycles(1);
  NOP; NOP; NOP; NOP; NOP; 
  NOP; NOP; NOP; NOP; NOP;

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
  
  NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;
    NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;
    NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;
    NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;
    NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;

}


void delay_cycles(uint16_t cycles) {
    // Convert cycles to microseconds (approx)

    set_sleep_mode(SLEEP_MODE_IDLE);  // idle keeps timers running

    sleep_enable();
    sleep_cpu();  // CPU sleeps but timer keeps running
    sleep_disable();
}
