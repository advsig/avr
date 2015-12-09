/* clock with 4-digit MAX7219 LED display */
#include <Time.h>
void setup() {
  ledInit();
  setTime(23,05,0,12,7,15);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (second() == 0) {
    unsigned t = hour() * 100 + minute();
    ledDisp(t, true);
  }
}
