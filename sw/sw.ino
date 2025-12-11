#include "defaults.h"
#include "functions.h"
#include "analog.h"
//#include "bod_detect.cpp"

//#define NOP __asm__ __volatile__ ("nop\n\t") // Define the NOP instruction

bool bod_triggered = false;

void setup_bod_flag() {
  uint8_t status = MCUSR;

  // Clear flags immediately after reading to prepare for next event
  MCUSR = 0; 
  bod_triggered = true;
  led_on();

  if (status & (1 << BORF)) {
    bod_triggered = true;
    led_on();
    // Brown-out Reset occurred
    // {Link: www.avrfreaks.net https://www.avrfreaks.net/s/topic/a5C3l000000UY10EAG/t144548}
  }

  if (status & (1 << PORF)) {
    bod_triggered = false;
    led_off();
    // Power-on Reset occurred (also clears BORF if it happened)
  }
}

void getVoltageDigits(float v, int digits[5]) {
    int scaled = v*10000*2 + OFFSET*1000;

    digits[0] = (scaled / 1000) % 10;
    digits[1] = (scaled / 100)  % 10;
    digits[2] = (scaled / 10)   % 10;
    digits[3] =  scaled % 10;
    digits[4] = 0; 
}

void preloop() {
  float v;
  int digits[5];
  for (int n = 0; n < 3; n++) {
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
  setup_bod_flag();

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
  //BAR_EN_PORT &= ~(1 << BAR_EN_BIT);

  //DISABLE
  BAR_EN_PORT |= (1 << BAR_EN_BIT);

  // Configure Timer1 once
  TCCR1A = 0;            // Normal mode
  TCCR1B = (1 << CS10);  // Prescaler = 1

  if (bod_triggered)
  {
    for (int n = 0; n < 1; n++) {
      for (int k = 0; k < 100; k++) {
      lcd_write(4, PATTERN_E);
      delay(1);
      }
    }
  }
  else
  {
    bar_test();

    preloop();
  }

  ADCSRA &= ~(1 << ADEN); // disable ADC


  PRR = (1 << PRTWI) |    // Disable TWI (I2C)
          (1 << PRSPI) |    // Disable SPI
          (1 << PRUSART0) | // Disable USART (Serial)
          (1 << PRTIM2);   // Disable Timer 2
          //(1 << PRTIM0);    // Disable Timer 0 (Timer 1 is kept for LCD/Bar timing)
  

}

void loop() {
  if (bod_triggered) {
    set_sleep_mode(SLEEP_MODE_PWR_SAVE);
    sleep_enable();
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
