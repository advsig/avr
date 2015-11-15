#include <LedControl.h>
#include <SPI.h>

SPISettings LedBd(125000, MSBFIRST, SPI_MODE0);
volatile byte tick = 0;
static const unsigned char lmap[] PROGMEM = {
  0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F
};

#define CSON (PORTB &= ~_BV(4))
#define CSOFF (PORTB |= _BV(4))

ISR(TIMER2_OVF_vect) {
  tick++;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  Serial.println("Starting");
  SFIOR |= _BV(PSR2);       // clear prescaler
  ASSR  |= _BV(AS2);       // Use TOSC2 with 32khz xtal
  TCCR2 |= _BV(CS22)|_BV(CS20);   // 1 hz tick
  TCNT2 = 0;
  TIMSK |= _BV(TOIE2);    // Enable interrupt
  SPI.begin();
  DDRB |= _BV(4);
  PORTB |= _BV(4);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (tick) {
    tick = 0;
    Serial.print('.');
    SPI.beginTransaction(LedBd);
    byte zero[] = {0x40, 0x3f};
    byte i;
    CSON;
    for (i = 0; i < sizeof(zero); i++) {
      SPI.transfer(zero[i]);
    }
    CSOFF;

    /* Set brightness */
    CSON;
    SPI.transfer(0x8f);
    CSOFF;
    SPI.endTransaction();
  }
}
