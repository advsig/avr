/* read keypad via bit-banged serial interface, read key pressed.  Pins are hard-coded for now */

enum kpModel {
  numKeys = 16
};

/* Ton-Touch TTP229BST strapped for 16 keys, serial output, active high */

uint8_t readKey(void)   // read serial data via software clock/data "bit Bang"
{
  uint16_t receive = 0;
  uint8_t  keyHit = 0xff;

  /* for some reason, the transitions are offset by 1. Should throw away first clock cycle */
  for(int i=0; i < numKeys +1; i++)
  {
    PORTD |= _BV(7);  // SCK high
    delayMicroseconds(12);
    receive |=  ((PIND & _BV(2)) ? 0 : 1);
    PORTD &= ~_BV(7); // SCK low
    delayMicroseconds(12);

    if (receive & 1) {
      keyHit = i;
    }
    
    receive <<= 1;  
  }
  return keyHit;         // Return the received data
}
