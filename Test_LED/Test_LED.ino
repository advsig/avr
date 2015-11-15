#include <stdio.h> /* for itoa(); */

//Due to a bug in Arduino, this needs to be included here too/first
#include <SPI.h>
#include <MAX7219.h>


const MAX7219_Topology topology[] = {{MAX7219_MODE_7SEGMENT, 0, 0, 0, 3},
                                     {MAX7219_MODE_7SEGMENT, 0, 4, 0, 7}};
/* we always wait a bit between updates of the display */
const byte delaytime = 250;

MAX7219 maxled;

void setup() {
  maxled.begin(topology, sizeof(topology) / sizeof(MAX7219_Topology));

 maxled.clearDisplay(0);

 maxled.setIntensity(6);
 maxled.set7Segment("0123",1, true);
 maxled.set7Segment("0123",0, true);
}




void loop() {
  // put your main code here, to run repeatedly:
 
}
