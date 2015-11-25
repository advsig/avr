#include <TimeAlarms.h>
#include <Time.h>
#include <DS3231RTC.h>
#include <LiquidCrystal_I2C.h>
#include <pcf8574.h>
#include <Wire.h>
#include <shine.h>

/* Globals */
// adjust addresses if needed
PCF8574 PCF(0x21);        // add switches to lines  (used as input)
LiquidCrystal_I2C lcd(0x22, 16, 2);  // Set the LCD I2C address
volatile uint8_t but, HzTick, rtci;
long tzOffset;
bool DST;

/* mask for PCF8574 */
uint8_t xmask;

AlarmID_t tTimer;
uint16_t tTask;    /* timer task countdown */

/* 8474 Pin assignments */
enum pcfP { YEL, BLU, RED, GRE, BUZ, BUT1, BUT2, BUT3, DEF = 0xE7 };
/* Timer States */
enum Tstates {IDLEX, TASK, INTERRUPT, PAUSE};
uint8_t curState;
uint16_t sTimers[4];
uint16_t secs;
const uint8_t DefTaskTime = 20; // minutes

void setup()
{
  if (Serial) {
    Serial.begin(115200);
  }
  lcd.begin(16,2);               // initialize the lcd 
  
  /* test to see if expander is active */
  uint8_t value = PCF.read8();
  if (PCF.lastError())
  {
    Serial.print("PCF read error\r\n");
  }
  xmask = DEF;
  PCF.write8(xmask);

 /* set up int0 to handle RTC alarm interrupt */
  pinMode(2, INPUT);
  attachInterrupt(0, RTCAlarm, FALLING);
  
 /* set up int1 to handle expander interrupt */
  pinMode(3, INPUT);
  attachInterrupt(1, button, FALLING);

  /* enable ICP1 as input, will use for 32KHz clock input */
  pinMode(5, INPUT);
  
  but = 0;
  secs = 0;
  
  setSyncProvider(RTC.get);
  lcd.backlight();
  setT1();
  //tTimer = Alarm.timerRepeat(1, secTick);
  ledInit();
  curState = IDLEX;
  DST = true;
  tzOffset = 3600L * (DST) ? 7L : 8L;
  prtTime();
  setLed(GRE, 1);
}

void setT1()
{
  cli();
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 0x8000;// 32kHz input from RTC module
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12 and CS11 for T1 falling edge count
  TCCR1B |= (1 << CS12) | (1 << CS11);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  sei();
}

/* 1 Hz Timer tick */
ISR(TIMER1_COMPA_vect)
{
  HzTick++;
}

void button()
// Button hit interrupt
{
  but++;
}

void RTCAlarm()
{
  /* Falling edge from RTC alarm */
  rtci++;
}

void loop()
{
  uint8_t rv;
  Alarm.delay(10);
  
  if (HzTick) {
    HzTick = 0;
    secTick();
  }

  /* check for button press */
  if (but == 0) {
    return;
  }
  
  rv = PCF.read8();

  but = 0;

  /* start task */
  if (bitRead(rv, BUT1) == 0) {
    startTask(DefTaskTime, rv);
  }
  
  if (bitRead(rv, BUT2) == 0) {
    toglInterrupt(rv);
  }
  
  if (bitRead(rv, BUT3) == 0) {
    //toglPause(rv);
    endTask();
  }
}

void endTask()
{
  sTimers[TASK] = 0;
  tTask = 0;
  curState = IDLEX;
  beepit(40);
  setLed(BLU, 0);
  setLed(GRE, 1);
  prtTime();
}

void startTask(uint8_t ival, uint8_t bp)
/* interval, button pressed */
{
  /* set the timer interval in seconds */
  tTask = ival * 60;
  sTimers[TASK] = 0;
  sTimers[IDLEX] = 0;
  secs = 0;
 
  // light Blue Led
  setLed(BLU, 1);
  setLed(GRE, 0);
  curState = TASK;
  lcd.backlight();
  prtTime();
}

/* called from loop() every second */
void secTick()
{
  sTimers[curState] += 1;
  //Serial.print('T');
  switch (curState) {
    case IDLEX:
      if (sTimers[IDLEX] > 15*60) {
        lcd.noBacklight();
      }
    break;
    case TASK:
      if (sTimers[TASK] >= tTask) {
        /* task expired */
        endTask();
      }
    break;
    case INTERRUPT:
    break;
    case PAUSE:
    break;
  }
  

  secs++;
  if ((secs % 60) == 0) {
    //Serial.println('M');
    /* long beep in interrupt state? */
    prtTime();
  }

}

void toglInterrupt(uint8_t rv)
/* process interrupt button */
{
  if (curState == TASK) {
    setLed(RED, 1);
    curState = INTERRUPT;
  } else if (curState == INTERRUPT && tTask) {
    setLed(RED, 0);
    curState = TASK;
  }
}

void toglPause(uint8_t rv)
{
  if (curState == TASK) {
    setLed(YEL, 1);
    curState = PAUSE;
  } else if (curState == PAUSE && tTask) {
    setLed(YEL, 0);
    curState = TASK;
  }
}

/* read time from RTC, print current time and counters for Task & IDLE */
void prtTime()
{
  tmElements_t tm;
  RTC.read(tm);
  time_t t = makeTime(tm);
  uint8_t i;
  
  // Adjust for DST, GMT -8
  bool dst = false;
  long offset = 8L;
  t -= 3600L * offset ;
  breakTime(t, tm);
  char buf[17];
  
  uint8_t Talarm = (tTask - sTimers[TASK]) / 60;
  uint8_t Tinterrupt = sTimers[INTERRUPT] / 60;
  sprintf(buf, "%02d:%02d T%02d I%02d", tm.Hour, tm.Minute, Talarm, Tinterrupt);
  lcd.setCursor(0,0);
  /* strange. As of 1.6.6, lcd.print doesn't work with char[] arrays, so send them one at a time */
  for (i = 0; buf[i]; i++) {
    lcd.print(buf[i]);
  }

  /* send time and countdown to LED display */
  uint16_t lt = tm.Hour;
  lt *= 100;
  lt += tm.Minute;
  ledDisp(lt, 0, true);
  ledDisp(Talarm, 1, false);
  //Serial.println(buf);
}

void beepit(uint8_t len)
{
   PCF.write8(xmask | (1<<BUZ));
   Alarm.delay(len);
   PCF.write8(xmask);
}

void setLed(uint8_t led, uint8_t state)
{
  // low is on
  if (state) {
    bitClear(xmask, led);
  } else {
    bitSet(xmask, led);
  }
  PCF.write8(xmask);
}
