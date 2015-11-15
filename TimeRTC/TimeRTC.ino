/*
 * TimeRTC.pde
 * example code illustrating Time library with Real Time Clock.
 *
 */
#include <avr/sleep.h>
#include <DS3232RTC.h>  // a basic DS1307 library that returns time as a time_t  
#include <TM1637Display.h>
#include <Time.h>
#include <Wire.h>

#define CLK 5
#define DIO 6
TM1637Display display(CLK, DIO);

int WakePin = 2;
volatile int Sleep = 0;
uint8_t state = 0;

void OneHz()
{
  Sleep++;
}

void setup()  {
  pinMode(WakePin, INPUT);

  Serial.begin(9600);



  setSyncProvider(RTC.get);   // the function to get the time from the RTC
  if (timeStatus() != timeSet)
    Serial.println("Unable to sync with the RTC");
  else
    Serial.println("RTC has set the system time");
  RTC.squareWave(SQWAVE_1_HZ);
  attachInterrupt(0, OneHz, FALLING);
  display.setBrightness(0x0f);

}

void loop()
{
  set_sleep_mode(SLEEP_MODE_IDLE);

  while (Sleep == 0) {
    sleep_enable();
    sleep_cpu();
  }

  sleep_disable();
  Sleep = 0;

  //digitalClockDisplay();
  if (second() == 0) {
    lcdDisp();
  } else {
    //blink_colon(state++ % 2);
  }
}

void blink_colon(uint8_t state)
{
  int mn = minute();
  uint8_t s[4];
  s[0] = display.encodeDigit(mn / 10) | (state) ? 0x80 : 0x0;
  display.setSegments(s, 1, 2);
}

void lcdDisp()
{
  int hr = hour();
  int mn = minute();
  display.showNumberDec(hr, true, 2, 0);
  display.showNumberDec(mn, true, 2, 2);
  // blink colon
}

void digitalClockDisplay() {
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year());
  Serial.println();

  /* check RTC */
  tmElements_t t;
  RTC.read(t);

}
void printDigits(int digits) {
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}


