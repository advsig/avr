#include <stdio.h> /* for itoa(); */

//Due to a bug in Arduino, this needs to be included here too/first
#include <SPI.h>
#include <MAX7219.h>
#define _MAX7219_DEMO_DIGITS 8

const MAX7219_Topology topology[] = {{MAX7219_MODE_7SEGMENT, 0, 0, 0, 3}};
/* we always wait a bit between updates of the display */
const byte delaytime = 250;

MAX7219 maxled;

void setup() {
  maxled.begin(topology, sizeof(topology) / sizeof(MAX7219_Topology));

 maxled.setScanLimit(4,0);
 maxled.clearDisplay(0);

 maxled.setIntensity(6);
 maxled.zeroDisplay(0);
 maxled.set7Segment("2501", 0);
}




void loop() {
  // put your main code here, to run repeatedly:
 
}
