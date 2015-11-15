#include <Time.h>
#include <TM1638.h>

// define a module on data pin 8, clock pin 9 and strobe pin 7
TM1638 module(11, 13, 10);

void setup() {
  // display a hexadecimal number and set the left 4 dots
  setTime(14, 05, 00, 13, 9, 2015);
}

void loop() {
  unsigned long timeStr;
  byte dots;
  timeStr =  day() + month()*100;
  timeStr = timeStr*100 + hour();
  timeStr = timeStr*100 + minute();

  dots = (second() % 2) ? 0x44 : 0x40;
 
  module.setDisplayToDecNumber(timeStr, dots);
  byte keys = module.getButtons();

  // light the first 4 red LEDs and the last 4 green LEDs as the buttons are pressed
  module.setLEDs(keys & 0xff);
}
